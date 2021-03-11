# msmrd2:
MSM/RD: A general framework to couple Markov state models (MSM) of molecular kinetics with particle-based reaction-diffusion (RD) simulations. The software can also be used to simulate Brownian dynamics of rigid bodies with all the translational and rotational degrees of freedom, potential interactions and Markovian switching. 

All the figures in the paper ``Multiscale molecular kinetics by coupling Markov state models and reaction-diffusion dynamics'' by Mauricio J. del Razo, Manuel Dibak, Christof Schütte and Frank Noé were generated with this code. The exact data used for the paper is available upon request to the authors.

This is the second version of the code written in c++ with python bindings and additional functionality. The first version (v1.0.0) was a much simpler implementation written in python, and it can be found [here](https://github.com/markovmodel/msmrd). The code is still in constant development, and there is no official release.

Active development of the code is done in [this github repository](https://github.com/maojrs/msmrd)

## Installation
```
git clone https://github.com/markovmodel/msmrd2.git
cd msmrd2
git submodule update --init --recursive
python setup.py install

```
We recommend using the conda package manager to install all the dependencies required by msmrd. The fastest way to start is to [install miniconda](https://conda.io/projects/conda/en/latest/user-guide/install/index.html), and then create a new environment for msmrd:

```
conda create --name msmrd-env
conda activate msmrd-env 
```

To install packages into your environment simply type `conda install {PACKAGE_NAME}`. Click [here](https://conda.io/projects/conda/en/latest/index.html) for more detailed information on conda. 

## Software dependencies 
Check the environment.yml for the specific dependencies in the conda environment. The main dependencies required are the following:
- GCC 10.2.0
- cmake 3.19.4
- HDF5 1.10.4
- python 3.8.3
- numpy 1.18.1
- scipy 1.4.1
- matplotlib 3.2.1
- h5py 2.10.0
- jupyter v.1.0.0

Versions below may work; versions above will most likely work. We also recommend [VMD](http://www.ks.uiuc.edu/Research/vmd/) for visualization. 

The setup also automatically install two submodules: [pybind11](https://github.com/pybind/pybind11), to create Python bindings of existing C++ code and [catch2](https://github.com/catchorg/Catch2/tree/v2.x) a test framework for C++.

If you want to generate your own Markov models to use with MSM/RD, we also recommend installing [pyEMMA](http://emma-project.org/latest/).


## Getting started
To test your installation worked, you can load one of the notebook examples available. First activate the corresponding conda environment using `conda activate {ENVIRONMENT-NAME}`. Then:
```
cd examples/models
jupyter notebook
```
Click on one of the notebooks, e.g. `odLangevin.ipynb`. Then go to cell and click on run all. You should be able to see the output produced at the end of the notebook.

## Visualization
We recommend [VMD](http://www.ks.uiuc.edu/Research/vmd/) to visualize the output of the particle-based simulations. Parts of the code will even generate files to automatically load into vmd. As an example try the following:
- Install VMD
- Run all the code in the notebook `patchyParticlesAngular2.ipynb`
- This will generate files `patchyParticlesAngular2.xyz`  and `patchyParticlesAngular2_2vmd.tcl` in the folder `data/vmd`.
- On a terminal prompt run `vmd -e patchyParticlesAngular2_2vmd.tcl`

