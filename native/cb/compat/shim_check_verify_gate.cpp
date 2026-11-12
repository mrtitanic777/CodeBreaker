// Compile-only check of the shims at the old header paths for cb/verify/gate16 (contract16, setup16) and cb/verify/lanes
// (dos, image_lane): the old py:: API over the new modules. Nothing here runs; it is in cb_shim_check so the shims stay
// honest until their last consumer is refactored.
#include "codebreaker/verify/gate16/contract16.hpp"
#include "codebreaker/verify/gate16/setup16.hpp"
#include "codebreaker/verify/lanes/dos.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"
#include "codebreaker/verify/lanes/msx.hpp"
#include "codebreaker/verify/lanes/zx.hpp"

namespace shim_check {

inline void touch_verify_gate() {
    namespace contract = codebreaker::verify::gate16::contract16;
    namespace setup = codebreaker::verify::gate16::setup16;
    namespace dos = codebreaker::verify::lanes::dos;
    namespace image = codebreaker::verify::lanes::image_lane;
    (void)&contract::_calls;
    (void)&contract::mismatches;
    (void)&setup::_paths;
    (void)&setup::_free_port;
    (void)&setup::make_cd;
    (void)&setup::continue_test;
    (void)&setup::_license;
    (void)&setup::_error_box;
    (void)&setup::_welcome;
    (void)&setup::_welcome_count;
    (void)&setup::_black;
    (void)&setup::Image::from_ppm;
    (void)&setup::set_iso_cab;
    (void)&dos::set_packager;
    (void)&dos::profile;
    (void)&dos::prepare;
    (void)&dos::build;
    (void)&dos::judge;
    (void)&image::Lane::judge;
    (void)&image::Lane::build;
    (void)&image::Lane::prepare;
    (void)&image::Lane::profile;
}

inline void touch_flat_machines() {
    // the flat Z80 lane's two machines: the new modules' values as the old layer's, and the lane made over the old z80_flat
    namespace msx = codebreaker::verify::lanes::msx;
    namespace zx = codebreaker::verify::lanes::zx;
    (void)msx::KIND;
    (void)msx::REV;
    (void)&msx::SAME;
    (void)msx::ROM_LO;
    (void)msx::ROM_HI;
    (void)&msx::lane;
    (void)zx::KIND;
    (void)zx::REV;
    (void)&zx::SAME;
    (void)zx::ROM_LO;
    (void)zx::ROM_HI;
    (void)&zx::lane;
}

}  // namespace shim_check
