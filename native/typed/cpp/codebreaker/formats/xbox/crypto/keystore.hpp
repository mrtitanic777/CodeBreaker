// codebreaker/formats/xbox/crypto/keystore.hpp -- COMPAT SHIM over cb/formats/xbox/crypto/keystore.hpp (docs/CPP_DESIGN.md 6.3), for
// the old shadowboot / sd_signer / se_patcher / xekeys_signer ports. Deleted with its last py:: consumer.
//
// The key store is native/cb/formats/xbox/crypto/keystore.hpp. The old API is kept: KEY_PATH() (the setting "xbox360_keys", "Keys"
// when not set, read the first time it is asked for), the CKSM_* constants, and load_and_verify_*() giving the old
// XeCryptRsaKey. Conversion and forwarding only.
//
// DIVERGES, for the old callers: a wrong checksum is still py::AssertionError (the message now also names the file and both
// checksums, "Invalid key checksum!" first); a missing file is py::FileNotFoundError, a damaged key a py::ValueError as before.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::keystore {

using XeCrypt::XeCryptRsaKey;

namespace detail {
namespace ks = ::cb::formats::xbox::crypto::keystore;

inline XeCryptRsaKey load(ks::KeyFile which, const py::str& folder) {
    try {
        return XeCryptRsaKey(ks::KeyStore(::cb::fs::from_text(folder.utf8())).load_and_verify(which));
    } catch (const ::cb::FormatError& e) {
        const std::string what = e.what();
        if (what.find("invalid key checksum") != std::string::npos) throw py::AssertionError("Invalid key checksum! " + what);
        throw;
    }
}
}  // namespace detail

// Path(settings.get("xbox360_keys") or "Keys")
inline const py::str& KEY_PATH() {
    static const py::str path = [] {
        py::str folder = py::settings::get_str(py::str("xbox360_keys"));
        if (folder.u32().empty()) folder = py::str("Keys");
        return folder;
    }();
    return path;
}

// checksums - public keys
inline constexpr int64_t CKSM_1BL_PUB = 0xD416B5E1;
inline constexpr int64_t CKSM_XMACS_PUB = 0xE4F01473;
inline constexpr int64_t CKSM_MASTER_PUB = 0xE86E10FD;
// checksums - private keys
inline constexpr int64_t CKSM_SB_PRV = 0x490C9D35;
inline constexpr int64_t CKSM_HVX_PRV = 0xDCC4B906;

inline XeCryptRsaKey load_and_verify_1bl_pub() { return ::cb::compat::guard([] { return detail::load(detail::ks::KeyFile::Bl1Pub, KEY_PATH()); }); }
inline XeCryptRsaKey load_and_verify_xmacs_pub() { return ::cb::compat::guard([] { return detail::load(detail::ks::KeyFile::XmacsPub, KEY_PATH()); }); }
inline XeCryptRsaKey load_and_verify_sb_prv() { return ::cb::compat::guard([] { return detail::load(detail::ks::KeyFile::SbPrv, KEY_PATH()); }); }
inline XeCryptRsaKey load_and_verify_hvx_prv() { return ::cb::compat::guard([] { return detail::load(detail::ks::KeyFile::HvxPrv, KEY_PATH()); }); }
inline XeCryptRsaKey load_and_verify_master_pub() { return ::cb::compat::guard([] { return detail::load(detail::ks::KeyFile::MasterPub, KEY_PATH()); }); }

}  // namespace codebreaker::formats::xbox::crypto::keystore
