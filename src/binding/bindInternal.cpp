//
// Created by maojrs on 10/8/18.
//
#include "binding.hpp"
#include "boundaries/boundary.hpp"
#include "integrators/integrator.hpp"
#include "markovModels/markovModel.hpp"
#include "potentials/potentials.hpp"
#include "trajectories/trajectory.hpp"



namespace msmrd {
    /*
     * pyBinders for the c++ parent classes (mostly virtual) that should remain hidden under _internal in python
     */
    void bindInternal(py::module &m) {

        /* Bind boundaries parent class*/
        pybind11::class_<boundary>(m, "boundary");

        /* Bind integrators parent class*/
        pybind11::class_<integrator>(m, "integrator");

        /* Bind Markov models parent class*/
        pybind11::class_<markovModel>(m, "markovModel");

        /* Bind trajectories parent class*/
        pybind11::class_<trajectory>(m, "trajectory");

        /* Bind external potential parent class  */
        pybind11::class_<externalPotential>(m, "externalPotential");

        /* Bind pair potential parent class */
        pybind11::class_<pairPotential>(m, "pairPotential");
    }

}
