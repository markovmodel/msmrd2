//
// Created by maojrs on 1/22/19.
//

#pragma once
#include <memory>
#include "trajectories/trajectoryPositionOrientation.hpp"
#include "discretizations/positionOrientationPartition.hpp"
#include "tools.hpp"

namespace msmrd {
    /**
     * Trajectory class for patchy  dimer trajectories. Patchy dimer here refers to two
     * patchy particles with two patches. The main difference with the general class
     * trajectoryPositionOrientation is that this class can sample the discrete trajectory that is
     * specific for the patchy dimer application. In short words,  it chooses how to discretize the full
     * trajectory of two particle into a discretized trajectory to be analyzed and extracted into
     * a Markov state model.
     *
     * Patchy dimer will have one unbound state (0), two (or more) bound states (1,2) and
     * angularStates = numSections(numSections+1)/2 angular discrete states (3-3+angularStates).
     * Note in the current indexing implementation only up to 9 bound states are supported.
     */
    class patchyDimer : public trajectoryPositionOrientation {
    private:
        std::unique_ptr<positionOrientationPartition> positionOrientationPart;
        std::array< std::tuple<vec3<double>, quaternion<double>>, 8> boundStates{};
        int maxNumberBoundStates = 10;
        double tolerancePosition = 0.1;
        double toleranceOrientation = 2*M_PI*0.1;
        int prevsample = 0;
    public:
        /*
         * @positionOrientationPart full six dimensional partition of phase space of relative position and orientation.
         * This is required to sample the discrete trajectory in the transition regions given by this discretization.
         * @boundStates vector of tuples. Each tuple contains a vector and a quaternion indicating one of the 8 bound
         * states. The vector corresponds to the relative position (pos2-pos1) in the frame of reference of particle 1
         * (smaller index) between particle 1 and 2. The fixed frame of reference also assumes particle 1 is in its
         * default initial orientation. The quaternion corresponds to the relative orientation between the two
         * particles, also measuerd from the fixed frame of reference of particle 1. This variable is calculated by
         * the setMetastableRegions function.
         * @param maxNumberBoundStates maximum number of bound states supported. It is used to determine how to
         * count (index) the transition states. The state maxNumberBoundStates + 1 will correspond not to a bound state
         * but to the first transition state. This parameter has to be consistent with the one used
         * by the msmrd integrator and the msmrdMarkovModel.
         * @param tolerancePosition is the maximum acceptable difference between the relative position and the
         * calculated relative position of a metstable region to still be considered part of a bound state.
         * @param toleranceOrientation is the maximum acceptable angle-distance difference between the relative
         * orientation and the relative orientation calculated of a given metastable region to still be considerer
         * part of a bound state.
         * @param prevsample keeps calue of previous sample when sampling discrete trajectory, useful for
         * coreMSM approach
         */

        patchyDimer(unsigned long Nparticles, int bufferSize);

        void sampleDiscreteTrajectory(double time, std::vector<particle> &particleList) override;

        void setMetastableRegions();

        int getBoundState(vec3<double> relativePosition, quaternion<double> relativeOrientation);

        int getBoundStatePyBind(particle part1, particle part2);


    };

}