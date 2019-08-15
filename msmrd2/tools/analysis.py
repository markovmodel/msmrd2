import numpy as np
import pickle

# Functions to help analyze data

def bootstrappingMFPTs(fptList, numBootstrapSamples):
    '''
    Computes the bootsrapping statistic of a list of first passage time.
    :param fptList: list or array of first passage times
    :param numBootstrapSamples: number of bottstrapped samples used to calculate the
    mean first passage time and its standard deviation
    :return: mean first passage time and its standard deviation
    '''
    numValues = len(fptList)
    mfpts = np.zeros(numBootstrapSamples)
    # Calculate boostrapped samples
    for i in range(numBootstrapSamples):
        bootstrapSample = np.random.choice(fptList, size = numValues, replace = True)
        mfpts[i] = bootstrapSample.mean()
    # Compute mean and standard deviation over bootstrapped samples
    mfpt = mfpts.mean()
    fptStdDev = np.std(mfpts)
    return mfpt, fptStdDev

def writeParameters(filename, parameterDictionary):
    '''
    Writes parameters into .dat file for easy reading and into a .pickle file for
    easy loading.
    :param filename: filename (and location) to write parameters
    :param parameterDictionary: dictionary of parameters to be written
    '''
    with open(filename + ".dat", 'w') as file:
        for key, value in parameterDictionary.items():
            file.write(key + ' ' + str(value) + '\n')

    pickle_out = open(filename + ".pickle","wb")
    pickle.dump(parameterDictionary, pickle_out)
    pickle_out.close()


def readParameters(filename):
    pickle_in = open(filename + ".pickle","rb")
    parameterDictionary = pickle.load(pickle_in)
    return parameterDictionary
