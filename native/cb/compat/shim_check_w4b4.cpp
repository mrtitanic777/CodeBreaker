// Compile-only check of the shims at the old header paths for exact/plm86, exact/mspascal and exact/xenix_c (the old toolchain classes over
// the new plugins). Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumers are refactored.
#include "codebreaker/exact/mspascal.hpp"
#include "codebreaker/exact/plm86.hpp"
#include "codebreaker/exact/xenix_c.hpp"

namespace shim_check {

inline void touch_w4b4() {
    namespace pl = codebreaker::exact::plm86;
    namespace pa = codebreaker::exact::mspascal;
    namespace xc = codebreaker::exact::xenix_c;
    (void)&pl::register_plugin;
    (void)&pa::register_plugin;
    (void)&pa::emit_order;
    (void)&pa::publish;
    (void)&xc::register_plugin;
    (void)&xc::rule_u;
    (void)&xc::rule_d;
    (void)&xc::rule_m;
    (void)&xc::rule_b;
    (void)&xc::c2_hooks;
    (void)&xc::_c1;
    (void)&xc::_c2;
    // the classes are made by toolchain::load(): a profile and a variant
    codebreaker::exact::toolchain::Profile profile;
    (void)sizeof(pl::PLM86);
    (void)sizeof(pa::MSPascal);
    (void)sizeof(xc::XenixC);
    (void)profile;
}

}  // namespace shim_check
