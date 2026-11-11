// Compile-only check of the build shims at the old header paths (build/msc6.hpp, build/asm16.hpp) and of the one call that cannot
// run in cb_tests: msc6::compile_many goes through the old DOS runner (build/dos_tools), which is not linked here. Nothing here
// runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/build/asm16.hpp"
#include "codebreaker/build/msc6.hpp"

namespace shim_check {

inline void touch_build() {
    namespace om = codebreaker::build::msc6;
    namespace oa = codebreaker::build::asm16;
    (void)&om::compile_many;
    (void)&om::wine_typing;
    (void)&om::wine_definition;
    (void)&om::cdecl_imports;
    (void)&om::sdk_prototypes;
    (void)&om::sdk_calls;
    (void)&om::_native;
    (void)&om::_refused;
    (void)&oa::check_piece;
    (void)&oa::differences;
}

}  // namespace shim_check
