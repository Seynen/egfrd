#if !defined( __DEFS_HPP )
#define __DEFS_HPP

#include <cstddef>

typedef double Real;
typedef long int Integer;
typedef unsigned long int UnsignedInteger;

#define STR( S ) #S
#define THROW_UNLESS( CLASS, EXPRESSION )\
    if(!(EXPRESSION))\
      throw CLASS("Check ["+std::string(STR(EXPRESSION)) + "] failed.");

const Real SEPARATION_TOLERANCE( 1e-07  );
const Real MINIMAL_SEPARATION_FACTOR( 1.0 + SEPARATION_TOLERANCE );


#include <gsl/gsl_math.h>

#ifdef _MSC_VER

#ifdef GF_EXPORT
#define GF_CLASS __declspec(dllexport)
#else
#define GF_CLASS __declspec(dllimport)
#endif

#ifdef GFRD_EXPORT
#define GFRD_CLASS __declspec(dllexport)
#else
#define GFRD_CLASS __declspec(dllimport)
#endif


#else
#define GF_CLASS
#define GFRD_CLASS
#endif



#endif // __DEFS_HPP
