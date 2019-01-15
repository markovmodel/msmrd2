//
// Created by dibakma on 22.06.18.
//

//#include <random>
#include "simulation.hpp"
#include "trajectories/trajectory.hpp"


namespace msmrd {

    simulation::simulation(integrator &integ)
            : integ(integ) {};

    void simulation::run(std::vector<particle> &particleList, const int Nsteps, trajectory &traj, int stride, std::string filename) {
        int bufferCounter = 0;
        for (int step=0; step < Nsteps; step++) {
            integ.integrate(particleList);
            if (step % stride == 0) {
                bufferCounter++;
                traj.sample(step, particleList);
                if (bufferCounter == traj.bufferSize) {
                    bufferCounter = 0;
                    traj.write2H5file(filename, traj.data);
                }
            }
        }
    }
}