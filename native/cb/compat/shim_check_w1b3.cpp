// Compile-only check of the parts of the shim at codebreaker/build/flat_thunk.hpp that reach the old corpus and the old
// prototypes table (and so cannot be linked into cb_tests). Nothing here runs; it is in cb_shim_check, so the shim stays honest
// until its last consumer is refactored.
#include "codebreaker/base/converted.hpp"
#include "codebreaker/build/flat_thunk.hpp"

namespace shim_check_w1b3 {

inline void touch() {
    namespace ft = codebreaker::build::flat_thunk;
    (void)&ft::recover_flat_thunk;
    (void)&ft::build_flat_thunk;
    (void)&ft::build_flat_thunk_ow;
    (void)&ft::compile_ow;
    (void)&ft::toolchain;
    (void)&ft::main;
    (void)&ft::detail::services;
}

}  // namespace shim_check_w1b3
