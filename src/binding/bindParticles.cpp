//
// Created by maojrs on 9/7/18.
//
#include "binding.hpp"
#include "particle.hpp"

namespace msmrd {
    /*
     * pyBinders for the c++ particles classes
     */
    void bindParticles(py::module &m) {
        py::class_<particle>(m, "particle")
                .def(py::init<double &, double &, std::string &, std::vector<double> &, std::vector<double> &>())
                .def_property_readonly("ID", &particle::getID)
                .def_property_readonly("D", &particle::getD)
                .def_property_readonly("Drot", &particle::getDrot)
                .def_property_readonly("bodytype", &particle::getBodyType)
                .def_property_readonly("position", [](const particle &part) {
                    return vec2numpy(3, part.position);
                })
                .def_property_readonly("orientvector", [](const particle &part) {
                    return vec2numpy(3, part.orientvector);
                })
                .def_property_readonly("orientation", [](const particle &part) {
                    return vec2numpy(4, part.orientation);
                })
                .def("setD", &particle::setD)
                .def("setDrot", &particle::setDrot)
                .def("setBodyType", &particle::setBodyType)
                .def("setPosition", &particle::setPositionPyBind)
                .def("setOrientVector", &particle::setOrientVectorPyBind)
                .def("setOrientation", &particle::setOrientationPyBind);

        py::class_<particleMS, particle>(m, "particleMS")
                .def(py::init<int &, int &, double &, double &, std::string &, std::vector<double> &, std::vector<double> &>())
                .def_property_readonly("type", &particleMS::getType)
                .def_property_readonly("state", &particleMS::getState)
                .def_property_readonly("lagtime", &particleMS::getLagtime)
                .def("setState", &particleMS::setState)
                .def("setType", &particleMS::setType)
                .def("setLagtime", &particleMS::setLagtime);
    }

}

