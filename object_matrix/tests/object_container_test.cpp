#define BOOST_TEST_MODULE "object_container_test"

#include <functional>
#include <iostream>
#include <cmath>
#include <set>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "sphere.hpp"
#include "object_container.hpp"

BOOST_AUTO_TEST_CASE(insert)
{
    typedef object_container<sphere<double>, int> oc_type;
    typedef oc_type::position_type pos;
    oc_type oc(1.0, 10);
    BOOST_CHECK_CLOSE(0.1, oc.cell_size(), 0.001);

    {
        std::pair<oc_type::iterator, bool> ir(
                oc.insert(std::make_pair(
                    0, oc_type::mapped_type(pos(0.2, 0.6, 0.4), 0.5))));
        BOOST_CHECK_EQUAL(true, ir.second);
        BOOST_CHECK(oc.end() != oc.find(0));
        BOOST_CHECK(oc.end() == oc.find(1));
    }
    {
        std::pair<oc_type::iterator, bool> ir(
                oc.insert(std::make_pair(
                    0, oc_type::mapped_type(pos(0.2, 0.65, 0.4), 0.5))));
        BOOST_CHECK_EQUAL(false, ir.second);
        BOOST_CHECK_EQUAL(oc_type::mapped_type(pos(0.2, 0.65, 0.4), 0.5),
                (*ir.first).second);
        BOOST_CHECK(oc.end() != oc.find(0));
        BOOST_CHECK(oc.end() == oc.find(1));
    }
    {
        std::pair<oc_type::iterator, bool> ir(
                oc.insert(std::make_pair(
                    0, oc_type::mapped_type(pos(0.2, 0.2, 0.4), 0.5))));
        BOOST_CHECK_EQUAL(false, ir.second);
        BOOST_CHECK_EQUAL(oc_type::mapped_type(pos(0.2, 0.2, 0.4), 0.5),
                (*ir.first).second);
        BOOST_CHECK(oc.end() != oc.find(0));
        BOOST_CHECK(oc.end() == oc.find(1));
    }
}

template<typename Toc_>
struct collector
{
    collector(): result() {}

    void operator()(typename Toc_::iterator i)
    {
        result.insert((*i).first);
    }

    std::set<typename Toc_::key_type> result;
};

template<typename Toc_>
struct collector2
{
    void operator()(typename Toc_::iterator i,
            const typename Toc_::position_type& pos_off)
    {
        result.insert((*i).first);
    }

    std::set<typename Toc_::key_type> result;
};

BOOST_AUTO_TEST_CASE(each_neighbor)
{
    typedef object_container<sphere<double>, int> oc_type;
    typedef oc_type::position_type pos;
    oc_type oc(1.0, 10);
    BOOST_CHECK_CLOSE(0.1, oc.cell_size(), 0.001);

    oc.insert(std::make_pair(0, oc_type::mapped_type(pos(0.2, 0.6, 0.4), 0.5)));
    BOOST_CHECK(oc.end() != oc.find(0));
    BOOST_CHECK(oc.end() == oc.find(1));
    oc.insert(std::make_pair(1, oc_type::mapped_type(pos(0.2, 0.7, 0.5), 0.5)));
    BOOST_CHECK(oc.end() != oc.find(0));
    BOOST_CHECK(oc.end() != oc.find(1));
    BOOST_CHECK(oc.end() == oc.find(2));
    oc.insert(std::make_pair(2, oc_type::mapped_type(pos(0.9, 0.1, 0.4), 0.5)));
    BOOST_CHECK(oc.end() != oc.find(0));
    BOOST_CHECK(oc.end() != oc.find(1));
    BOOST_CHECK(oc.end() != oc.find(2));
    BOOST_CHECK(oc.end() == oc.find(3));
    oc.insert(std::make_pair(3, oc_type::mapped_type(pos(0.9, 0.95, 0.4), 0.5)));
    BOOST_CHECK(oc.end() != oc.find(0));
    BOOST_CHECK(oc.end() != oc.find(1));
    BOOST_CHECK(oc.end() != oc.find(2));
    BOOST_CHECK(oc.end() != oc.find(3));
    BOOST_CHECK(oc.end() == oc.find(4));

    {
        collector<oc_type> col;
        oc.each_neighbor(oc.index(pos(0.2, 0.6, 0.4)), col);
        BOOST_CHECK_EQUAL(col.result.size(), 2);
        BOOST_CHECK(col.result.find(0) != col.result.end());
        BOOST_CHECK(col.result.find(1) != col.result.end());
    }

    {
        collector<oc_type> col;
        oc.each_neighbor(oc.index(pos(0.0, 0.1, 0.4)), col);
        BOOST_CHECK_EQUAL(col.result.size(), 0);
    }

    {
        collector2<oc_type> col2;
        oc.each_neighbor_cyclic(oc.index(pos(0.0, 0.1, 0.4)), col2);
        BOOST_CHECK_EQUAL(col2.result.size(), 1);
    }
    {
        collector2<oc_type> col2;
        oc.each_neighbor_cyclic(oc.index(pos(0.9, 0.0, 0.4)), col2);
        BOOST_CHECK_EQUAL(col2.result.size(), 2);
    }
}

template<typename Toc_>
struct collector3
{
    collector3(): count(0) {}

    void operator()(typename Toc_::iterator i)
    {
        ++count;
    }

    int count;
};

BOOST_AUTO_TEST_CASE(each_neighbor2)
{
    typedef object_container<sphere<double>, int> oc_type;
    typedef oc_type::position_type pos;
    oc_type oc(1.0, 10);
    BOOST_CHECK_CLOSE(0.1, oc.cell_size(), 0.001);

    pos centre(0.55, 0.55, 0.55);
    double radius = 0.02;

    for (int i = 0; i < 10000; ++i) {
        double t1(M_PI * 2 * (i % 100) / 100.), t2(M_PI * 2 * (i / 100) / 100.);
        const double _x = cos(t1) * radius;
        const pos p(centre.x() + _x * cos(t2),
                    centre.y() + sin(t1) * radius,
                    centre.z() + _x * sin(t2));
        oc.insert(std::make_pair(i, oc_type::mapped_type(p, radius)));


        collector3<oc_type> col;
        oc.each_neighbor(oc.index(centre), col);
        BOOST_CHECK_EQUAL(col.count, i + 1);
    }
}

