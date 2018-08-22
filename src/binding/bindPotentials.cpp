#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include "potentials/potentials.hpp"
#include "potentials/dipole.hpp"
#include "potentials/gaussians3D.hpp"
#include "potentials/harmonicRepulsion.hpp"
#include "vec3.hpp"


namespace py = pybind11;

/*
 * pyBinders for the c++ potentials
 */
void bindPotentials(py::module& m) {
    pybind11::class_<externalPotential<> >(m, "externalPotential");
    pybind11::class_<externalPotential<vec3<double>> >(m, "externalRodPotential");

    py::class_<gaussians3D, externalPotential<> >(m, "gaussians3D")
        .def(py::init<int&, double&, double&, long&>())
        .def("evaluate", (double (gaussians3D::*)(std::vector<double>) ) &gaussians3D::evaluatePyBind)
        .def("force", (std::vector<double> (gaussians3D::*)(std::vector<double>) ) &gaussians3D::forceTorquePyBind);

    py::class_<dipole, externalPotential<vec3<double>> >(m, "dipole")
            .def(py::init<double&, std::vector<double>& >())
            .def("evaluate", (double (dipole::*)(std::vector<double>, std::vector<double>) ) &dipole::evaluatePyBind)
            .def("forceTorque", (std::vector<std::vector<double>> (dipole::*)(std::vector<double>, std::vector<double>) ) &dipole::forceTorquePyBind);
}
