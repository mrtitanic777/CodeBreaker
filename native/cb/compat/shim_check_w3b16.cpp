// Compile-only check of the shims at the old header paths for verify/authored/repair, verify/gate16/scoreboard16 and xna/port (the old
// py:: API over the new modules). Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumers
// are refactored.
#include "codebreaker/verify/authored/repair.hpp"
#include "codebreaker/verify/gate16/scoreboard16.hpp"
#include "codebreaker/xna/port.hpp"

namespace shim_check {

inline void touch_w3b16() {
    namespace rp = codebreaker::verify::authored::repair;
    namespace sb = codebreaker::verify::gate16::scoreboard16;
    namespace xp = codebreaker::xna::port;
    (void)&rp::_split;
    (void)&rp::_frame;
    (void)&rp::instructions;
    (void)&rp::score;
    (void)&rp::_blank;
    (void)&rp::_depth;
    (void)&rp::_tests;
    (void)&rp::_toggle_register;
    (void)&rp::_knobs;
    (void)&rp::repair;
    (void)&rp::repair_function;
    (void)&rp::truth;
    (void)&rp::_scratch;
    (void)&rp::best_flags;
    (void)&rp::run_powerpc;
    (void)&rp::run;
    (void)&rp::main;
    (void)&sb::PATTERNS;
    (void)&sb::watcom_flags;
    (void)&sb::_same;
    (void)&sb::_watcom;
    (void)&sb::run;
    (void)&sb::report;
    (void)&sb::show;
    (void)&sb::rows_json;
    (void)&sb::main;
    (void)&xp::rewrite;
    (void)&xp::build;
    (void)&xp::main;
}

}  // namespace shim_check
