// Compile-only check of the shims at the old header paths for exact/carve, exact/bss and exact/names (the old py:: API over the new modules, through the cb project
// the old exact::project::Project holds). Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/exact/bss.hpp"
#include "codebreaker/exact/carve.hpp"
#include "codebreaker/exact/names.hpp"

namespace shim_check {

inline void touch_w5b3() {
    namespace carve = codebreaker::exact::carve;
    namespace bss = codebreaker::exact::bss;
    namespace names = codebreaker::exact::names;
    (void)&carve::region;
    (void)&carve::functions;
    (void)&carve::_hexrx;
    (void)&carve::main;
    (void)&bss::propose;
    (void)&bss::lines;
    (void)&bss::main;
    (void)&names::known;
    (void)&names::functions;
    (void)&names::read;
    (void)&names::carry;
    (void)&names::main;
}

}  // namespace shim_check
