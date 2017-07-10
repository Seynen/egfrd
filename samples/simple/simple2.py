#!/usr/bin/env python

# $ export PYTHONPATH=$HOME/egfrd
# $ python -O simple2.py 30 0xFD1CF6EA


# Modules
import sys
import time
from egfrd import *
import model
import gfrdbase


# Constants
N = int(sys.argv[1])
seed = int(sys.argv[2], 16)
matrix = 8
size = 1e-6
end_time = 1


# Setup
print('Setup Custom simulation');
m = model.ParticleModel(size)
A = model.Species('A', 1e-12, 1e-9)
m.add_species_type(A) 
w = gfrdbase.create_world(m, matrix)
myrandom.seed(seed)
s = EGFRDSimulator(w, myrandom.rng)
throw_in_particles(w, A, N)


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
        if (s.step_counter % 1000 == 0)  :
            gfrdVisualizer.export('D:\\dump.log', s, s.step_counter > 0)
            sys.stdout.write('.')
        s.step()
except KeyboardInterrupt:
    print("Aborted after {0}".format(s.t));

end_time = time.time()
print("Simulation took {0} seconds / {1} steps.".format(end_time - start_time , s.step_counter));
