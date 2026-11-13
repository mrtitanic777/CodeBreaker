// Compile-only check of the shims at the old header paths that stand over the lane kit (engine/lane_kit) and the new lanes: verify/lanes/win16 and win32,
// verify/gate16/isolated16, engine/lanes, engine/win16, engine/lanes_wiring, jobs/workers and build/module (whose services are the kit's). Nothing here runs;
// it is in cb_shim_check so the shims stay honest until their last consumers are refactored.
#include "codebreaker/build/module.hpp"
#include "codebreaker/engine/lanes.hpp"
#include "codebreaker/engine/lanes_wiring.hpp"
#include "codebreaker/engine/win16.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "codebreaker/verify/gate16/isolated16.hpp"
#include "codebreaker/verify/lanes/win16.hpp"
#include "codebreaker/verify/lanes/win32.hpp"
#include "codebreaker/verify/reconstruct.hpp"

namespace shim_check {

inline void touch_lane_kit() {
    namespace w32 = codebreaker::verify::lanes::win32;
    namespace w16 = codebreaker::verify::lanes::win16;
    namespace gate = codebreaker::verify::gate16::isolated16;
    namespace rec = codebreaker::verify::reconstruct;
    namespace workers = codebreaker::jobs::workers;
    (void)w32::KIND;
    (void)w32::ARITY();
    (void)&w32::settings_profile;
    (void)&w32::prepare;
    (void)&w32::functions;
    (void)w32::LANES.at(py::str("pe")).key(0x1000);
    (void)w16::KIND;
    (void)w16::REV;
    (void)&gate::check;
    (void)&workers::dispatch;
    (void)&workers::count;
    (void)&rec::run;
    (void)&rec::identity;
    (void)&rec::digests;
    (void)&rec::kind_name;
    (void)&codebreaker::build::module::build;
    (void)&codebreaker::build::module::build_many;
}

}  // namespace shim_check
