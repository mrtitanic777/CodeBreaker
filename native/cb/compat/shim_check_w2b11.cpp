// Compile-only check of the shim at the old header path for recompile/project (the image, .xex header, import names, title
// locator and projects folder it reads are the old ports', which cannot be linked into cb_tests). Nothing here runs; it is in
// cb_shim_check so the shim stays honest until its last consumer is refactored.
#include "codebreaker/recompile/project.hpp"

namespace shim_check {

inline void touch_w2b11() {
    namespace project = codebreaker::recompile::project;
    (void)&project::scaffold;
    (void)&project::main;
    (void)&project::set_root;
    (void)&project::_clean_name;
    (void)&project::ROOT;
    (void)&project::_REXGLUE;
}

}  // namespace shim_check
