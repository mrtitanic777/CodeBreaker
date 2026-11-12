// Compile-only check of the shim at the old header path for formats/xbox/crypto/keystore (W2-B8): the surface the old ports
// (shadowboot, xekeys_signer, sd_signer, se_patcher) call, over native/cb. Nothing here runs.
#include "codebreaker/formats/xbox/crypto/keystore.hpp"

namespace codebreaker::shim_check {

int64_t w2b8_shims() {
    namespace ks = formats::xbox::crypto::keystore;
    (void)&ks::load_and_verify_1bl_pub;
    (void)&ks::load_and_verify_xmacs_pub;
    (void)&ks::load_and_verify_sb_prv;
    (void)&ks::load_and_verify_hvx_prv;
    (void)&ks::load_and_verify_master_pub;
    return ks::CKSM_SB_PRV + static_cast<int64_t>(ks::KEY_PATH().u32().size());
}

}  // namespace codebreaker::shim_check
