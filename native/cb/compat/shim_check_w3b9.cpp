// Compile-only check of the shims at the old header paths of wave 3 batch 9 (java/projects/minecraft/package, knowledge/package,
// knowledge/store): the old types they take and give. Nothing here runs; it is in cb_shim_check so the shims stay honest until their
// last consumers are refactored.
#include "codebreaker/java/projects/minecraft/package.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/store.hpp"

namespace shim_check {

inline void touch_w3b9() {
    namespace mc = codebreaker::java::projects::minecraft;
    (void)&mc::load_profiles;
    (void)&mc::PROJECT;
    mc::Minecraft project;
    (void)project.default_profile;

    // the platform vocabulary forwards to cb/knowledge/package.hpp; the rest of the old base is legacy over engine/native_engine
    (void)&codebreaker::knowledge::PLATFORMS;
    (void)codebreaker::knowledge::platform(py::str("pe"));
    (void)codebreaker::knowledge::_bare(py::str("USER32!FindWindowA"));
    (void)&codebreaker::knowledge::store::default_path;
}

}  // namespace shim_check
