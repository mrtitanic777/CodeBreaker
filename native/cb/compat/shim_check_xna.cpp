// Compile-only check of the xna shims at the old header paths (xna/mgfx.hpp, xna/fixups.hpp) and of the old ports that
// still use them (xna/port.cpp builds its effects with mgfx, xna/run.cpp cleans the decompiled C# with fixups). Nothing
// here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/xna/fixups.hpp"
#include "codebreaker/xna/mgfx.hpp"

namespace shim_check {

inline void touch_xna() {
    namespace fix = codebreaker::xna::fixups;
    namespace fx = codebreaker::xna::mgfx;
    (void)&fix::ref_casts;
    (void)&fix::constructors;
    (void)&fix::data_holders;
    (void)&fix::apply;
    (void)&fx::add_parameters;
}

}  // namespace shim_check
