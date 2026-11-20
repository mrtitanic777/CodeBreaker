// formats/xbox/crypto/cpukey_ecd.hpp -- check a CPU key the way its error correction and detection works.
//
//     cpukey_ecd::Validation v = cpukey_ecd::validate("C0DE...");     // 32 hex digits
//     cpukey_ecd::Key sealed = cpukey_ecd::with_ecd(key);              // the ECD bits set
//
// From SK.Xbox360.CPUKey by Stelio Kontos. A valid key is 16 bytes, not all zero, with exactly 0x35 bits set among the first 106 (the rest
// are the ECD) and the ECD bits the ones the Xbox 360's LFSR variant (the constant 0x360325) gives for those 106. Bits are numbered as the
// algorithm walks them: bit i is bit (i & 7) of byte (i >> 3), bits 106..126 the correction bits and bit 127 the overall parity.
//
// The command `crypto cpukey_ecd <hex key>` prints the verdict and the reasons; `crypto cpukey_ecd gen` prints a new valid key.
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::cpukey_ecd {

inline constexpr int kValidHammingWeight = 0x35;
inline constexpr std::uint32_t kEcdMagic = 0x360325;

using Key = std::array<std::uint8_t, 16>;

// The key with its ECD bits (106..127) set to what the first 106 bits call for.
Key with_ecd(const Key& key);
bool verify_ecd(const Key& key);

// The number of bits set in the first 106 (the 13 bytes before the ECD and the low two bits of the next).
int hamming_weight(const Key& key);

struct Validation {
    bool valid = false;
    bool length_ok = false;
    bool non_zero = false;
    bool hamming_ok = false;
    int hamming_weight = 0;
    bool ecd_ok = false;
    std::vector<std::string> errors;    // why it is not valid
};

// A key as typed: spaces and dashes are ignored; 32 hex digits are wanted.
Validation validate(std::string_view key_hex);

// A random key that passes (the weight right, the ECD set).
Key generate(xecrypt::RandomSource& random = xecrypt::os_random());

cli::Command command();

}  // namespace cb::formats::xbox::crypto::cpukey_ecd
