//
// Created by maojrs on 8/16/18.
//

#include "integrators/odLangevinMarkovSwitch.hpp"
#include "particle.hpp"
#include "msm.hpp"

/**
* Class implementation of over-damped Langevin dynamics with Markovian switch integrator
* constructors defined in headers since templates were used.
*/


// Integrates rotation/translation and Markovian switch of one particle (visible only inside the class)
template<>
void odLangevinMarkovSwitch<ctmsm>::integrateOne(particleMS &part) {
    double resdt;
    // propagate CTMSM/MSM when synchronized and update diffusion coefficients
    part.tcount = 0;
    // Runs one timestep dt, if lagtime < dt, propagates MSM as many times as needed
    if (part.lagtime <= dt) {
        // Run loop until integration for one timestep dt is achieved
        while (part.tcount < dt) {
            // Propagates MSM only when diffusion and rotation are in sync
            if (part.propagateTMSM) {
                tmsm.propagate(part, 1);
                part.setD(tmsm.Dlist[part.state]);
                part.setDrot(tmsm.Drotlist[part.state]);
            }
            // Integrates for one lagtime as long as integration is still under dt
            if ( part.tcount + part.lagtime < dt ) {
                translate(part, part.lagtime);
                rotate(part, part.lagtime);
                part.tcount += part.lagtime;
                part.propagateTMSM = true;
                // If current lagtime overtakes dt, integrate up to dt (by resdt) and reset lagtime to remaining portion
            } else {
                resdt = dt - part.tcount;
                translate(part, resdt);
                rotate(part, resdt);
                part.setLagtime(part.lagtime + part.tcount - dt);
                part.tcount += resdt; // this means part.tcount = dt, so will exit while loop.
                // If lag time = 0 MSM must propagate in next step, otherwise it needs to integrate remaining lagtime.
                if (part.lagtime == 0) {
                    part.propagateTMSM = true;
                } else {
                    part.propagateTMSM = false;
                };
            };
        }
        part.tcount = 0;
    } else {
        // Runs one full time step when lagtime > dt and update remaining lagtime.
        translate(part, dt);
        rotate(part, dt);
        part.setLagtime(part.lagtime - dt);
        // If lag time = 0 MSM must propagate in next step, otherwise it needs to integrate remaining lagtime.
        if (part.lagtime == 0) {
            part.propagateTMSM = true;
        } else {
            part.propagateTMSM = false;
        };
    };
};


// Same as integrateOne, but updates time and publicly visible
template<>
void odLangevinMarkovSwitch<ctmsm>::integrate(particleMS &part) {
    integrateOne(part);
    clock += dt;
};


// Integrates list of particlesMS (needs to override parent function because it is template based and uses particleMS)
template<>
void odLangevinMarkovSwitch<ctmsm>::integrateList(std::vector<particleMS> &parts) {
    for (int i=0; i<parts.size(); i++) {
        integrateOne(parts[i]);
    }
    clock += dt;
};
