//
// Created by maojrs on 8/14/18.
//
#pragma once
#include "vec3.hpp"
#include "randomgen.hpp"

/**
 * Abstract base class declaration for external potentials
 * @param ...ORIENTATION variadic template to define orientation. If there are no rotations, ...ORIENTATION has
 * zero arguments); otherwise, it can be described by an orientation vector or even a quaternion. If the
 * orientation is simply a vector (vec3<double>) it describes rod-like particles.
 */
template<typename ...ORIENTATION>
class externalPotential{
public:
    externalPotential() = default;

    // Virtual functions to calculate value of potential and force/torque at position "pos" and orientation u
    virtual double evaluate(vec3<double> pos, ORIENTATION... u) = 0;
    virtual std::array<vec3<double>, 2> forceTorque(vec3<double> pos, ORIENTATION... u) = 0;

    // PyBind evaluation functions for potentials of particles with no orientation
    double evaluatePyBind(std::vector<double> pos);
    std::vector<double> forceTorquePyBind(std::vector<double> pos);
    // PyBind evaluation functions for potentials of particles with rod-like orientation
    double evaluatePyBind(std::vector<double> pos, std::vector<double> u);
    std::vector<std::vector<double>> forceTorquePyBind(std::vector<double> pos, std::vector<double> u);
};


/**
 * Abstract base class declaration for pair potentials
 * @param ...ORIENTATION variadic template to define orientation. If there are no rotations, ...ORIENTATION has
 * zero arguments); otherwise, it can be described by an orientation vector or even a quaternion. If the
 * orientation is simply a vector (vec3<double>) it describes rod-like particles.
 */
template<typename ...ORIENTATION>
class pairPotential{
public:
    pairPotential() = default;

    // Virtual functions to calculate value of potential and force/torque at positions "pos1" and "pos2" and orientations u
    virtual double evaluate(vec3<double> pos1, vec3<double> pos2, ORIENTATION... u) = 0;
    virtual std::array<vec3<double>, 2> forceTorque(vec3<double> pos1, vec3<double> pos2, ORIENTATION... u) = 0;

    // PyBind evaluation functions for pair-ppotentials of particles with no orientation
    double evaluatePyBind(std::vector<double> pos1, std::vector<double> pos2);
    std::vector<double> forceTorquePyBind(std::vector<double> pos1, std::vector<double> pos2);

    // PyBind evaluation functions for pair-ppotentials of particles with rod-like orientation
    double evaluatePyBind(std::vector<double> pos1, std::vector<double> pos2, std::vector<double> u1, std::vector<double> u2);
    std::vector<std::vector<double>> forceTorquePyBind(std::vector<double> pos1, std::vector<double> pos2,std::vector<double> u1, std::vector<double> u2);
};



