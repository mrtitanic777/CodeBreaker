// Compile-only check of the shims at the old header paths for java/signatures, java/projects/package and knowledge/show
// (the old pipeline, the Minecraft project and the knowledge store they need cannot be linked into cb_tests). Nothing here
// runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/projects/package.hpp"
#include "codebreaker/java/scaffold.hpp"
#include "codebreaker/java/signatures.hpp"
#include "codebreaker/knowledge/show.hpp"

namespace shim_check {

inline void touch_w2b9() {
    namespace signatures = codebreaker::java::signatures;
    namespace projects = codebreaker::java::projects;
    namespace scaffold = codebreaker::java::scaffold;
    namespace show = codebreaker::knowledge::show;
    namespace own = codebreaker::java::emit::ownership;
    namespace hd = codebreaker::java::emit::header;
    (void)&signatures::run;
    (void)&projects::manifests;
    (void)&projects::load;
    (void)&projects::for_profile;
    (void)&projects::GENERIC;
    (void)static_cast<std::shared_ptr<projects::Project> (*)(const py::list<projects::ClassRef>&, const std::optional<py::str>&,
                                                           const std::optional<py::str>&, const std::optional<py::str>&)>(&projects::choose);
    (void)&scaffold::run;
    (void)&show::main;
    (void)&show::dumps;
    (void)&own::params_str;
    (void)&own::retained_param_positions;
    (void)&hd::render_header;
    (void)&hd::_banner;
}

}  // namespace shim_check
