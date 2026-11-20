// formats/xbox/crypto/exp_signer.hpp -- sign a payload for HvxExpansionInstall: the hypervisor's "install this expansion" call.
//
//     Bytes expansion = exp_signer::sign(hvx_key, payload, {.magic = exp_signer::ExpansionMagic::Hxpr, .id = 0x48565050}, random);
//
// An expansion is a 0x170-byte header and the payload padded to 16. The header (big-endian): magic, flags, the total length; the SHA-1 of
// everything from 0x130 on (at 0xC); the AES IV (0x20); the signature (0x30, 0x100 bytes); the expansion info at 0x130 (its id, the sections'
// offset and size); the section info at 0x160. The signature is over the first 0x30 bytes:
//     HXPR  the rot-sum-SHA of them, signed with the salt "XBOX360EXP"
//     SIGM  the same hash, with PKCS#1
//     HXPC, SIGC  the HMAC-SHA1 of them under the console's CPU key, with the salt
// and an HXPR expansion may be encrypted: a random IV is written at 0x20 and everything from 0x130 on is AES-128-CBC encrypted with the 1BL key.
// (The signature is taken before the IV is written: the IV place is zero in what it covers.)
//
// Errors: UsageError for an HXPC/SIGC expansion with no valid CPU key, or an encryption without the 1BL key.
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xbox/crypto/keystore.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::exp_signer {

inline constexpr std::string_view kSalt = "XBOX360EXP";
inline constexpr std::uint32_t kDefaultId = 0x48565050;      // "HVPP"
inline constexpr std::uint32_t kXbOnlineId = 0x48564050;
inline constexpr std::size_t kHeaderSize = 0x170;

enum class ExpansionMagic : std::uint32_t {
    Hxpr = 0x48585052,
    Hxpc = 0x48585043,
    Sigm = 0x5349474D,
    Sigc = 0x53494743,
};

struct Options {
    ExpansionMagic magic = ExpansionMagic::Hxpr;
    std::uint32_t id = kDefaultId;
    bool encrypt = true;                          // HXPR only
    std::optional<Bytes> bl1_key;                 // the 1BL's AES key; needed to encrypt
    std::optional<Bytes> cpu_key;                 // the console's; needed by HXPC and SIGC
};

Bytes sign(const xecrypt::RsaKey& hvx_private_key, ByteView payload, const Options& options, xecrypt::RandomSource& random = xecrypt::os_random());

// `exp_signer <input> [-o OUT] [-i ID] [-k KEYFILE] [--encrypt]`; the key is the store's HVX key unless -k names another file (read as it is).
cli::Command command(const keystore::KeyStore& store);

}  // namespace cb::formats::xbox::crypto::exp_signer
