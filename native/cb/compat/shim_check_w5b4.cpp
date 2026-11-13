// Compile-only check of the shim at the old header path of translate/backends/c_java: the old API over the new module. Nothing here runs; it is
// in cb_shim_check so the shim stays honest until its last consumer is refactored.
#include "codebreaker/translate/backends/c_java.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"

namespace shim_check {

inline void touch_w5b4_cpp17() {
    namespace c17 = codebreaker::translate::backends::cpp17;
    py::list<codebreaker::translate::ir::NodeP> units;
    (void)c17::program(units);
    (void)c17::program(units, py::str("Main"));
    codebreaker::translate::ir::NodeP unit;
    (void)c17::write_unit(unit);
    (void)c17::write_unit(unit, py::str("banner"));
    (void)c17::header_path(py::str("pkg.mod"));
    (void)c17::source_path(py::str("pkg.mod"));
    (void)c17::FLAGS;
    (void)c17::RUNTIME();
    (void)c17::API_MAPS();
    try {
        (void)c17::program(units);
    } catch (const c17::Decline&) {
    }
}

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
