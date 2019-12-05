import msmrd2
import msmrd2.tools.analysis as analysisTools

# Given an MD simulation of patchyProteins, generate discrete trajectories, using patchyProtin discrete trajectory.


# Load parameters from parameters file (from original MD simulation)
parentDirectory = '../../data/patchyProtein/benchmark/'
parameterDictionary = analysisTools.readParameters(parentDirectory + "parameters")
nfiles = parameterDictionary['numFiles']
numParticles = parameterDictionary['numParticles']
dt = parameterDictionary['dt']
stride = parameterDictionary['stride']
totalTimeSteps = parameterDictionary['timesteps']
boxsize = parameterDictionary['boxsize']
boundaryType = parameterDictionary['boundaryType']

# Calculated parameters
#dtEffective = dt*stride # needed to obtain rate dictionary
effectivetimeSteps = int(totalTimeSteps/stride)
fnamebase = parentDirectory + 'simPatchyProtein_'
bufferSize = effectivetimeSteps

# Set trajectory discretizator
radialLowerBound = 1.25
radialUpperBound = 2.25
discretizator = msmrd2.trajectories.patchyProtein(numParticles, bufferSize, radialLowerBound, radialUpperBound)

# Set boundary (important for discretizer)
boxBoundary = msmrd2.box(boxsize, boxsize, boxsize, boundaryType)
discretizator.setBoundary(boxBoundary)

# Load H5 files and generates discrete trajectories at once directly on c++.
dtrajs = []
for i in range(nfiles):
    filename = fnamebase + str(i).zfill(4) + '.h5'
    dtraj = discretizator.discretizeTrajectoryH5(filename)
    dtrajs.append(dtraj)
    print("Loading file ", i+1, " of ", nfiles, " done.", end="\r")
print()
print("Done loading files")

# Write discrete trajectory to xyz files
for i, dtraj in enumerate(dtrajs):
    datafile  = open(fnamebase + str(i).zfill(4) + '_discrete.xyz', 'w')
    for j in range(len(dtraj)):
        datafile.write(str(dtraj[j]) + '\n')
    datafile.close()
    print("Writing discrete trajectory ", i+1, " of ", nfiles, " done.", end="\r")
print()
print("Done writing discrete trajectories")
