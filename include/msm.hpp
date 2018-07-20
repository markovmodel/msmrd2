//
// Created by maojrs on 7/4/18.
//

#pragma once
#include <random>
#include <array>
#include <algorithm>
#include "particle.hpp"

/**
 * Abstract base class for Markov state models
 */
class msmbase {
public:
    int msmid;
    double lagtime;
    std::vector<std::vector<double>> tmatrix;
    unsigned int nstates;
    std::vector<double> Dlist;
    std::vector<double> Drotlist;
    /**
     * Constructors
     * @param msmid ID of the msm, corresponds to the particle type
     * @param lagtime msm lagtime (in ctmsm it is calculated after each propagation step)
     * @param tmatrix transition matrix (for ctmsm transition rate matrix)
     * @param nstates number of states in the msm (obtained directly from matrix size)
     * Additional variables
     * @param Dlist list of diffusion coefficients for each state
     * @param Drotlist list of rotational diffusion coefficients for each state
     */
    // Main constructor, can receive std::vectior matrix or numpy array matrix (through pybind)
    msmbase(int msmid,  std::vector<std::vector<double>> &tempmatrix, double lagtime)
            : msmid(msmid), lagtime(lagtime) {
        nstates = static_cast<int>(tempmatrix.size());
        Dlist.resize(nstates);
        Drotlist.resize(nstates);
        tmatrix.resize(nstates);
        for (int i=0; i<nstates; i++) {
            tmatrix[i].resize(nstates);
            std::copy_n(tempmatrix[i].begin(), nstates, tmatrix[i].begin());
        }
    };

    // Main functions definitions (=0 for abstract class)
    virtual void propagate(particle part) = 0;
    void setD(std::vector<double> &D){
        Dlist.resize(nstates);
        Dlist=D;
    }
    void setDrot(std::vector<double> &Drot){
        Drotlist.resize(nstates);
        Drotlist=Drot;
    }

    // Get properties functions for pybinding
    int getID() { return  msmid; }
    int getNstates() { return  nstates; }
    double getLagtime() {return lagtime; }
    std::vector<std::vector<double>> getTmatrix() { return  tmatrix; }
};


/**
 * Child classes of msmbase, discrete-time (msm) and continuous-time (ctmsm)
 */

class msm: public msmbase {
public:
    using msmbase::msmbase; // constructors inheritance
    virtual void propagate(particle part) override;
};


class ctmsm: public msmbase {
public:
    using msmbase::msmbase; // constructors inheritance
    virtual void propagate(particle part) override;
};






