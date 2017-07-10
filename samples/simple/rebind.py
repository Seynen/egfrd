#!/usr/bin/env python

# $ export PYTHONPATH=$HOME/egfrd
# $ python -O rebind.py 


# Modules
import sys
import time
from egfrd import *
import model
import gfrdbase
import gfrdVisualizer


# Constants
N = 100
seed =  0x1234CAFE
matrix = 8
size = 3.42e-6
end_time = 90


# Setup
print('Setup Custom simulation');
m = model.ParticleModel(size)
A = model.Species('A', 1e-12, 1e-9)
m.add_species_type(A) 
B = model.Species('B', 1e-12, 1e-9)
m.add_species_type(B) 
C = model.Species('C', 1e-12, 1e-9)
m.add_species_type(C) 

r1 = model.create_binding_reaction_rule(A, B, C, 1e-19)
m.network_rules.add_reaction_rule(r1)
r2 = model.create_unbinding_reaction_rule(C, A, B, 2e-2)
m.network_rules.add_reaction_rule(r2)

w = gfrdbase.create_world(m, matrix)
myrandom.seed(seed)
s = EGFRDSimulator(w, myrandom.rng)
throw_in_particles(w, A, N)
throw_in_particles(w, B, N)



# print parameters
print('time local = {0}'.format( time.asctime()));
print('world size = {0} [m]'.format( size ));
print('matrix size = {0}x{0}x{0}'.format(matrix));
print('end_time = {0} [s]'.format(end_time));
print('seed = 0x%X'% seed );
print('particles = {0}'.format( N ));


# Run
start_time = time.time()
print('Start of simulation')
try:
    while s.t < end_time :
        if (s.step_counter % 200000 == 0)  :
            print "step#:%s, t:%s, #A:%s, #B:%s, #C:%s" %(s.step_counter, s.t, len(w.get_particle_ids(A)), len(w.get_particle_ids(B)), len(w.get_particle_ids(C)))
            gfrdVisualizer.export('D:\\dump.log', s, s.step_counter > 0)
        s.step()
except KeyboardInterrupt:
    print("Aborted after {0}".format(s.t));
	
end_time = time.time()
print("Simulation took {0} seconds / {1} steps.".format(end_time - start_time , s.step_counter));
