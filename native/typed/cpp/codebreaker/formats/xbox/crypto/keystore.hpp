// codebreaker/formats/xbox/crypto/keystore.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.keystore.
//
// The Xbox 360 key store: each key is read from the key store folder (the xbox360_keys setting, "Keys"
// when not set) and checked against the CRC-32 of the real key before it is used.
//
// Typing notes:
//   * KEY_PATH is the Python's module-level Path, evaluated at import; here it is read the first time it is
//     asked for (the setting is not known at static-initialisation time) and kept. It is a str: the Path is
//     only ever used to name a file, KEY_PATH / "1BL_pub.bin" being the folder joined with the name.
//   * A wrong checksum is the Python's `assert`: py::AssertionError("Invalid key checksum!"). A missing key
//     file is FileNotFoundError, as Path.read_bytes raises it.
//   * The keys are XeCrypt::XeCryptRsaKey objects (the class of the XeCrypt port).
//   * not ported: `__all__` (an export list).
#pragma once

#include <cstdint>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::keystore {

using XeCrypt::XeCryptRsaKey;

// Path(settings.get("xbox360_keys") or "Keys")
const py::str& KEY_PATH();

// checksums - public keys
inline constexpr int64_t CKSM_1BL_PUB = 0xD416B5E1;
inline constexpr int64_t CKSM_XMACS_PUB = 0xE4F01473;
inline constexpr int64_t CKSM_MASTER_PUB = 0xE86E10FD;
// checksums - private keys
inline constexpr int64_t CKSM_SB_PRV = 0x490C9D35;
inline constexpr int64_t CKSM_HVX_PRV = 0xDCC4B906;

XeCryptRsaKey load_and_verify_1bl_pub();
XeCryptRsaKey load_and_verify_xmacs_pub();
XeCryptRsaKey load_and_verify_sb_prv();
XeCryptRsaKey load_and_verify_hvx_prv();
XeCryptRsaKey load_and_verify_master_pub();

}  // namespace codebreaker::formats::xbox::crypto::keystore
