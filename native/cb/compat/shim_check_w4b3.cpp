// Compile-only check of the build/module shim at the old header path: its translation unit (typed/cpp/codebreaker/build/module.cpp) calls the
// old ports (cache, corpus, converted, watcom, definitions, describe, the emulators, the gate, the lanes) and cannot run in cb_tests. Nothing
// here runs; it is in cb_shim_check so the shim stays honest until its last consumer is refactored.
#include "codebreaker/build/module.hpp"

namespace shim_check {

inline void touch_build_module() {
    namespace om = codebreaker::build::module;
    (void)&om::build;
    (void)&om::build_many;
    (void)&om::build_worker;
    (void)&om::main;
    (void)&om::check;
    (void)&om::run;
    (void)&om::_build16;
    (void)&om::_build32;
    (void)&om::_whole_file_image;
    om::Built b;
    (void)b.report();
}

}  // namespace shim_check
