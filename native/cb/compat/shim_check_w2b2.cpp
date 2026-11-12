// Compile-only check of the shims at the old header paths for cb/analyze (dispatch, structures, dos_functions) and cb/verify
// (lane_common): the old py:: API over the new modules, and the bridge that makes a cb Image of an old analyze::image::Image.
// Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "codebreaker/analyze/cb_image.hpp"
#include "codebreaker/analyze/dispatch.hpp"
#include "codebreaker/analyze/dos_functions.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/verify/lane_common.hpp"

namespace shim_check {

inline void touch_w2b2() {
    namespace dispatch = codebreaker::analyze::dispatch;
    namespace structures = codebreaker::analyze::structures;
    namespace df = codebreaker::analyze::dos_functions;
    namespace bridge = codebreaker::analyze::bridge;
    namespace lc = codebreaker::verify::lane_common;
    (void)&dispatch::switches;
    (void)&dispatch::messages;
    (void)&dispatch::keywords;
    (void)&dispatch::interrupts;
    (void)&dispatch::control_messages;
    (void)&dispatch::analyse;
    (void)&dispatch::MESSAGES;
    (void)&dispatch::CONTROL_MESSAGES;
    (void)&structures::recover;
    (void)&structures::_recover;
    (void)&structures::layout;
    (void)&structures::_describe;
    (void)&structures::field_type;
    (void)&structures::analyse;
    (void)&df::discover;
    (void)&df::main;
    (void)&bridge::cb_image_of;
    (void)&bridge::cb_graph_of;
    (void)&bridge::convention_facts_of;
    (void)&lc::remembered;
    (void)&lc::judged;
    (void)&lc::guided;
}

}  // namespace shim_check
