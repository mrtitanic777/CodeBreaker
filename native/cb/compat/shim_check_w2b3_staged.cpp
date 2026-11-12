// Compile-only check of the staged shim of build/masm (compat/shims/codebreaker/build/masm.hpp): the entry points the old callers use. They
// reach the old DOS executive and the DOSBox runner (not linked into cb_tests), so nothing here runs; it is in cb_shim_check so the shim stays
// honest until its last consumer is refactored.
#include "shims/codebreaker/build/masm.hpp"

namespace shim_check_w2b3 {

inline void touch_masm() {
    namespace om = codebreaker::build::masm;
    (void)&om::assemble;
    (void)&om::assemble_many;
    (void)&om::functions;
    (void)&om::_selftest;
    (void)&om::main;
    (void)&om::close_pools;
}

}  // namespace shim_check_w2b3
