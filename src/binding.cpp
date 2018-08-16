#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/cast.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <numeric>
#include <iostream>
#include <random>
#include "msm.hpp"
#include "vec3.hpp"
#include "quaternion.hpp"
#include "particle.hpp"
#include "simulation.hpp"
#include "integrators/odLangevin.hpp"
#include "integrators/odLangevinMarkovSwitch.hpp"
#include "potentials/gaussians3D.hpp"
#include "potentials/harmonicRepulsion.hpp"



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

// Needed to connect lists/arrays of particles in python with c methods
PYBIND11_MAKE_OPAQUE(std::vector<particle>);
PYBIND11_MAKE_OPAQUE(std::vector<particleMS>);


namespace py = pybind11;

double arr_mean(const py::array_t<double> &arr) {
    double sum=0;
    for (int i=0; i<arr.size(); i++) {
        sum += arr.at(i);
    }
    return sum / arr.size() ;
}

void print_list(const std::vector<double> &v) {
    std::cout << v.size();
    for (int i=0; i<v.size(); i++){
        std::cout << v[i] ;
    }
}

py::array_t<double> normal_array(const py::ssize_t size) {
    std::mt19937 generator;
    std::random_device r;
    generator.seed(r());
    std::normal_distribution<double> distribution(0.0, 1.0);
    auto result = py::array_t<double>(size);
    py::buffer_info buf = result.request();
    double *ptr = (double *) buf.ptr;
    for (size_t idx=0; idx<size; idx++)
        ptr[idx] = distribution(generator);
    return result; // py::array_t<double>(size);
}

int main() {
    auto q1 = quaternion<double>(1,1,3,1);
    auto q2 = quaternion<double>(3,4,5,6);
    std::cout << q1+q2 << '\n';
    std::cout << q1-q2 << '\n';
    std::cout << q1 << '\n';
    std::cout << q1*q2 << '\n';
    std::cout << q1 << '\n';
    std::cout << q1.conj() << '\n';
    std::cout << q1*q1.conj() << '\n';
}

int add(int i, int j) {
    return i + j;
}

/* Convert c++ vectors/arrays to numpy arrays for pybind */
template <typename ndvec>
py::array_t<double> vec2numpy(int size, ndvec v) {
    auto result = py::array_t<double>(size);
    py::buffer_info buf = result.request();
    auto *ptr = (double *) buf.ptr;
    for (size_t idx=0; idx<size; idx++)
        ptr[idx] = v[idx];
    return result;
}

// Template declarations for pybinding template classes
//namespace {
//    template<unsigned int N>
//    static void declare_msm(py::module &mod, std::string const &suffix) {
//        using Class = msm<N>;
//        using PyClass = py::class_<Class, std::shared_ptr<Class>>;
//
//        PyClass cls(mod, ("msm" + suffix).c_str());
//
//        cls.def(py::init<int&, std::vector<std::vector<double>>&, double&>());
//        cls.def_property("ID", &Class::getID, nullptr);
//        cls.def_property("nstates", &Class::getNstates, nullptr);
//        cls.def_property("lagtime", &Class::getLagtime, nullptr);
//        cls.def_property("getTmatrix", &Class::getTmatrix,  nullptr);
//
////        //Alternative method:
////        std::string pyclass_name = std::string("msm") + suffix;
////        py::class_<Class>(mod, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
////                .def(py::init<int&, double&>());
//    }
//}

namespace py = pybind11;

PYBIND11_MODULE(msmrd2binding, m) {

    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: msmrd2

        .. autosummary::
           :toctree: _generate

           particle
           msm
           ctmsm
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Defines particle

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("mean", &arr_mean, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("main", &main, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("normal_array", &normal_array, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

    m.def("print_list", &print_list);

    /*
     * pyBinders for all the relevant c++ classes
     */

    py::class_<particle>(m, "particle")
            .def(py::init<double&, double&, std::vector<double>&, std::vector<double>& >())
            .def_property("ID", &particle::getID, nullptr)
            .def_property("D", &particle::getD, nullptr)
            .def_property("Drot", &particle::getDrot, nullptr)
            .def_property("position", [](const particle &part) {
                return vec2numpy(3,part.position);
            }, nullptr)
            .def_property("orientation", [](const particle &part) {
                return vec2numpy(4,part.orientation);
            }, nullptr)
            .def("setD", &particle::setD)
            .def("setDrot", &particle::setDrot)
            .def("setPosition", &particle::setPositionPyBind)
            .def("setOrientation", &particle::setOrientationPyBind);

    py::class_<particleMS>(m, "particleMS")
            .def(py::init<int&, int&, double&, double&, std::vector<double>&, std::vector<double>& >())
            .def_property("ID", &particleMS::getID, nullptr)
            .def_property("type", &particleMS::getType, nullptr)
            .def_property("state", &particleMS::getState, nullptr)
            .def_property("lagtime", &particleMS::getLagtime, nullptr)
            .def_property("D", &particleMS::getD, nullptr)
            .def_property("Drot", &particleMS::getDrot, nullptr)
            .def_property("position", [](const particleMS &part) {
                return vec2numpy(3,part.position);
            }, nullptr)
            .def_property("orientation", [](const particleMS &part) {
                return vec2numpy(4,part.orientation);
            }, nullptr)
            .def("setD", &particle::setD)
            .def("setDrot", &particle::setDrot)
            .def("setState", &particleMS::setState)
            .def("setType", &particleMS::setType)
            .def("setLagtime", &particleMS::setLagtime)
            .def("setPosition", &particleMS::setPositionPyBind)
            .def("setOrientation", &particleMS::setOrientationPyBind);

    py::class_<msm>(m, "msm")
            .def(py::init<int&, std::vector<std::vector<double>>&, double&, long&>())
            .def_property("ID", &msm::getID, nullptr)
            .def_property("nstates", &msm::getNstates, nullptr)
            .def_property("lagtime", &msm::getLagtime, nullptr)
            .def_property("tmatrix", &msm::getTmatrix,  nullptr)
            .def_property("D", [](const msm &currentmsm) {
                return vec2numpy(currentmsm.nstates,currentmsm.Dlist);
            }, nullptr)
            .def_property("Drot", [](const msm &currentmsm) {
                return vec2numpy(currentmsm.nstates,currentmsm.Drotlist);
            }, nullptr)
            .def("setD", &msm::setD)
            .def("setDrot", &msm::setDrot)
            .def("propagate", &msm::propagate);

    py::class_<ctmsm>(m, "ctmsm")
            .def(py::init<int&, std::vector<std::vector<double>>&, long&>())
            .def_property("ID", &ctmsm::getID, nullptr)
            .def_property("nstates", &ctmsm::getNstates, nullptr)
            .def_property("lagtime", &ctmsm::getLagtime, nullptr)
            .def_property("tmatrix", &ctmsm::getTmatrix,  nullptr)
            .def_property("D", [](const ctmsm &currentmsm) {
                return vec2numpy(currentmsm.nstates,currentmsm.Dlist);
            }, nullptr)
            .def_property("Drot", [](const msm &currentmsm) {
                return vec2numpy(currentmsm.nstates,currentmsm.Drotlist);
            }, nullptr)
            .def("setD", &ctmsm::setD)
            .def("setDrot", &ctmsm::setDrot)
            .def("propagate", &ctmsm::propagate);

    py::class_<odLangevin>(m, "odLangevin")
            .def(py::init<double&, long&, bool&>())
            .def_property("clock", &odLangevin::getClock, nullptr)
            .def("setPotential", &odLangevin::setPotential)
            .def("integrate", &odLangevin::integrate)
            .def("integrateList", &odLangevin::integrateList);

    py::class_<odLangevinMarkovSwitch<ctmsm>>(m, "odLangevinMarkovSwitch")
            .def(py::init<ctmsm&, double&, long&, bool&>())
            .def_property("clock", &odLangevinMarkovSwitch<ctmsm>::getClock, nullptr)
            .def("integrate", &odLangevinMarkovSwitch<ctmsm>::integrate)
            .def("integrateList", &odLangevinMarkovSwitch<ctmsm>::integrateList);

    py::class_<gaussians3D>(m, "gaussians3D")
            .def(py::init<int&, double&, double&, long&>())
            .def("evaluate", &gaussians3D::evaluatePyBind)
            .def("force", &gaussians3D::forcePyBind);




    // Created c++ compatible particle list/vector/array of particles in python
    py::bind_vector<std::vector<particle>>(m, "particleList");
    py::bind_vector<std::vector<particleMS>>(m, "particleMSList");





#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
#pragma clang diagnostic pop