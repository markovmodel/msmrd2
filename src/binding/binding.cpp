
#include "binding.hpp"

/**
 * Bind msmrd2 modules and submodules (functions defined in binding.hpp
 * and implemented in src/bind****.py)
 */
PYBIND11_MODULE(msmrd2binding, module) {

    module.attr("__name__") = "msmrd2";
    module.doc() =  "msmrd with python bindings";

    // Internal functionality submodule should be loaded before other submodules
    auto internalSubmodule = module.def_submodule("_internal", "msmrd _internal submodule");
    msmrd::bindInternal(internalSubmodule);

    // Load classes in main module
    msmrd::bindBoundaries(module);
    msmrd::bindMarkovModels(module);
    msmrd::bindParticles(module);

    // Load main submodules
    auto integratorsSubmodule = module.def_submodule("integrators", "msmrd integrators submodule");
    msmrd::bindIntegrators(integratorsSubmodule);
    auto potentialsSubmodule = module.def_submodule("potentials", "msmrd potentials submodule");
    msmrd::bindPotentials(potentialsSubmodule);
    auto trajectoriesSubmodule = module.def_submodule("trajectories", "msmrd trajectories submodule");
    msmrd::bindTrajectories(trajectoriesSubmodule);


}
