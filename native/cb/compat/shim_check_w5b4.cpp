// Compile-only check of the shim at the old header path of translate/backends/c_java: the old API over the new module. Nothing here runs; it is
// in cb_shim_check so the shim stays honest until its last consumer is refactored.
#include "codebreaker/translate/backends/c_java.hpp"

namespace shim_check {

inline void touch_w5b4() {
    namespace cj = codebreaker::translate::backends::c_java;
    py::list<codebreaker::translate::ir::NodeP> units;
    (void)cj::program(units);
    (void)cj::program(units, py::str("Main"));
    (void)cj::build(units, py::str("out"));
    (void)cj::CLANG;
    (void)cj::FLAGS;
    (void)cj::RUNTIME();
    try {
        (void)cj::program(units);
    } catch (const cj::Decline&) {
    }
}

}  // namespace shim_check
