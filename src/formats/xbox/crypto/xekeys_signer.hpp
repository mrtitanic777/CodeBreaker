// formats/xbox/crypto/xekeys_signer.hpp -- sign a payload for XeKeysExecute: the hypervisor's "execute this code" call.
//
//     Bytes image = xekeys_signer::sign(hvx_key, bl1_key, payload, xekeys_signer::PayloadMagic::Devkit, random);
//
// The image is a 0x120-byte header (magic, 0xDEAD, flags, the header size, the total size, a 16-byte random key, and 0x100 bytes for the
// signature), then the payload. The signature, salt "XBOX_EX_01", is over the rot-sum-SHA of the first 0x10 bytes and everything from 0x120 on;
// everything after the first 0x20 bytes is then RC4-encrypted with HMAC-SHA1(1BL key, the random key)[0..16].
//
// The command `crypto xekeys_signer <input> <output> [--retail]` signs with the key store's private HVX key (checked against its CRC) and the
// 1BL key file (1BL_key.bin) of the same folder.
#pragma once

#include <cstdint>
#include <string_view>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xbox/crypto/keystore.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xekeys_signer {

inline constexpr std::string_view kSalt = "XBOX_EX_01";
inline constexpr std::size_t kHeaderSize = 0x120;

enum class PayloadMagic : std::uint16_t {
    Devkit = 0x5D4D,
    Retail = 0x4D4D,
};

// `bl1_key` is the 1BL's 16-byte key. UsageError for a key that is not 16 bytes; whatever the signing throws for a key that cannot sign.
Bytes sign(const xecrypt::RsaKey& hvx_private_key, ByteView bl1_key, ByteView payload, PayloadMagic magic, xecrypt::RandomSource& random);

cli::Command command(const keystore::KeyStore& store);

}  // namespace cb::formats::xbox::crypto::xekeys_signer
