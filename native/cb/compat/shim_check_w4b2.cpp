// Compile-only check of the shim at the old header path of analyze/origin: the old py:: surface (Origin and Result with py containers, the
// Counter-shaped summary, module16 over a path, main) over cb/analyze/origin. It calls the old describe layer and scratch folders, which
// cannot be linked into cb_tests, so nothing here runs; it is in cb_shim_check so it stays honest until its last consumer is refactored.
#include "codebreaker/analyze/origin.hpp"

namespace shim_check_w4b2 {

inline void touch() {
    namespace o = codebreaker::analyze::origin;
    (void)&o::module16;
    (void)&o::_module16;
    (void)&o::main;
    (void)&o::most_common;
    o::Result r;
    (void)r.summary();
    (void)o::Origin().repr();
}

}  // namespace shim_check_w4b2
