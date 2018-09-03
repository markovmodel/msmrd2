//
// Created by maojrs on 8/16/18.
//

#include "integrators/odLangevin.hpp"

/**
 * Implementation of over-damped Lanegvin dynamics integrator class
 * @param dt time step
 * @param seed random generator seed (Note seed = -1 corresponds to random device)
 * @param rotation boolean to indicate if rotational degrees of freedom should be integrated
 */
odLangevin::odLangevin(double dt, long seed, bool rotation) : integrator(dt,seed, rotation) {};


// Integrate one particle from the particle list main routine (visible only inside the class)
void odLangevin::integrateOne(int partIndex, std::vector<particle> &parts, double timestep) {
    vec3<double> force;
    vec3<double> torque;
    std::array<vec3<double>, 2> forctorq;
    std::array<vec3<double>, 2> forctorqPairs;
    forctorq = getExternalForceTorque(parts[partIndex]);
    forctorqPairs = getPairsForceTorque(partIndex, parts);
    force = forctorq[0] + forctorqPairs[0];
    torque = forctorq[1] + forctorqPairs[1];
    translate(parts[partIndex], force, timestep);
    if (rotation) {
        rotate(parts[partIndex], torque, timestep);
    }
}

void odLangevin::translate(particle &part, vec3<double> force, double dt0){
    vec3<double> dr;
    dr = force*dt0*part.D/KbTemp + std::sqrt(2*dt0*part.D)*randg.normal3D(0,1);
    part.setPosition(part.position + dr);
}

void odLangevin::rotate(particle &part, vec3<double> torque, double dt0){
    vec3<double> dphi;
    quaternion<double> dquat;
    dphi = torque*dt0*part.Drot/KbTemp + std::sqrt(2*dt0*part.Drot)*randg.normal3D(0,1);
    dquat = axisanglerep2quaternion(dphi);
    part.setOrientation(dquat * part.orientation);
    // Updated orientation vector, useful with rodlike particles
    vec3<double> neworientvector = rotateVec(part.orientvector, dquat);
    part.setOrientVector(neworientvector);
}
