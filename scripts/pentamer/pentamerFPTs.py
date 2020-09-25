import numpy as np
import msmrd2
import msmrd2.tools.quaternions as quats
from msmrd2.potentials import patchyParticleAngular2
from msmrd2.integrators import overdampedLangevin as odLangevin
import msmrd2.tools.particleTools as particleTools
import multiprocessing
from multiprocessing import Pool
import os

'''
Creates an MD simulation of five particles and calculates their first passage times (FPTs) 
from a random configuration to a pentamer configuration. The data is written to 
'../data/pentamer/first_passage_times/filename_here. It uses the patchy dimer model of molecules
with only one stable angular configuration created in the trimer example.
'''

# Main parameters for particle and integrator
numparticles = 5
dt = 0.0001 #0.00001 #0.000005
bodytype = 'rigidbody'
D = 1.0
Drot = 1.0
overlapThreshold = 1.5 # to avoid overlapping when randomly generating particles
numTrajectories = 6000
# Other important parameters
boxsize = 6 #2.5 #6 #8 #6

# Parameters of patchy Particle potential with angular dependence (make sure it is consistent with msmrd data)
sigma = 1.0
strength = 160.0
angularStrength = 20.0
angleDiff = 3*np.pi/5.0
patch1 = np.array([np.cos(angleDiff/2),np.sin(angleDiff/2),0.])
patch2 = np.array([np.cos(-angleDiff/2),np.sin(-angleDiff/2),0.])
patchesCoordinates = [patch1, patch2]

# All this bound states between two molecules corespond to be bound in a U-shape.
boundStates = [1, 2, 3, 4]

# Chooses parent directory
parentDirectory = "../../data/pentamer/first_passage_times/"
# parentDirectory = "/group/ag_cmb/scratch/maojrs/msmrd2_data/pentamer/first_passage_times/"

# Creates parent directory if it doesn't exist already
try:
    os.mkdir(parentDirectory)
except OSError as error:
    print("First passage times directory already exists. Simulation continues.")

# Create empty files to save the data in parallel algorithm
filename = parentDirectory  + 'pentamerFPTs_trajs' + str(numTrajectories) \
           +'_boxsize' + str(boxsize) + '.xyz'

def simulationFPT(trajectorynum):
    '''
    Calculates first passage time of a trajectory with random initial
    conditions and final state the trimer state
    :param trajectorynum: number of trajectories on which to calculate the FPT
    :return: state, first passage time
    '''

    # Define dummy trajectory to extract bound states from python (needed to use getState function)
    dummyTraj = msmrd2.trajectories.patchyDimer2(2,1)
    # Require more lax tolerances since trimer bends the dimer binding angle and shortens the distance
    # (can be tested this works very well by plotting with vmd).
    dummyTraj.setTolerances(0.50, 0.5*2*np.pi)

    # Define simulation boundaries (choose either spherical or box)
    boxBoundary = msmrd2.box(boxsize, boxsize, boxsize, 'periodic')

    # Define potential
    potentialPatchyParticleAngular2 = patchyParticleAngular2(sigma, strength, angularStrength, patchesCoordinates)

    # Define integrator and boundary (over-damped Langevin)
    seed = int(-1*trajectorynum) # Negative seed, uses random device as seed
    integrator = odLangevin(dt, seed, bodytype)
    integrator.setBoundary(boxBoundary)
    integrator.setPairPotential(potentialPatchyParticleAngular2)

    # Generate random position and orientation particle list with two particles
    seed = int(trajectorynum)
    partlist = particleTools.randomParticleList(numparticles, boxsize, overlapThreshold, D, Drot, seed)

    # Calculates the first passage times for the trime. Each trajectory is integrated until
    # a bound state is reached. The output in the files is the elapsed time.
    unbound = True
    ii = 0
    while(unbound):
        ii += 1
        integrator.integrate(partlist)
        # Pentamer needs at least five bindings
        numBindings = 0
        bindingsList = [0,0,0,0,0] # number of bindings for each particle (must be two for each one for the pentamer)
        for i in range(5-1):
            for j in range(i+1,5):
                binding = dummyTraj.getState(partlist[i], partlist[j])
                if binding in boundStates:
                    numBindings += 1
                    bindingsList[i] += 1
                    bindingsList[j] += 1
        #if (ii % 50000):
        #    print('%.4f' %integrator.clock, numBindings, bindingsList)
        if ( numBindings >= 5):# and bindingsList == [2,2,2,2,2]):
            unbound = False
            return "pentamer", integrator.clock
        elif integrator.clock >= 600.0:
            unbound = False
            return 'Failed at:', integrator.clock



def multiprocessingHandler():
    '''
    Handles parallel processing of simulationFPT and writes to same file in parallel
    '''
    num_cores = multiprocessing.cpu_count()
    pool = Pool(processes=num_cores)
    trajNumList = list(range(numTrajectories))
    with open(filename, 'w') as file:
        for index, result in enumerate(pool.imap(simulationFPT, trajNumList)):
            state, time = result
            if state == 'pentamer':
                file.write(state + ' ' + str(time) + '\n')
                print("Simulation " + str(index) + ", done. Success!")
            else:
                print("Simulation " + str(index) + ", done. Failed :(")


# Run parallel code
multiprocessingHandler()

# # Serial code for testing with gdb
# with open(filename, 'w') as file:
#     for index in range(numTrajectories):
#         state, time = simulationFPT(index)
#         if state == 'pentamer':
#             file.write(state + ' ' + str(time) + '\n')
#             print("Simulation " + str(index) + ", done. Success!")
#         else:
#             print("Simulation " + str(index) + ", done. Failed :(")




