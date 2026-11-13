// cb/formats/xbox/crypto/hypervisor.hpp -- the Xbox 360 hypervisor image (256 KB): what it says about itself, and cleaning the RGH patches
// out of a dumped one.
//
//     hypervisor::Info info = hypervisor::info(image);                    // build, QFE, the memory-encryption keys, the hash table
//     Bytes clean = hypervisor::clean(dumped, original);                  // the dumped image with the original's code put back
//
// After Hypervisor Manager. A decrypted image keeps the three memory keys (white, AES, hash; 16 bytes each) at 0x10100 and the security
// hash of its code at 0x100F0; a console that runs with the test keys has all three keys 0x55. Six (address, size) pairs of the memory
// ranges the hash covers are at 0x10878.
//
// Commands of `crypto hypervisor`: `info <hv.bin>` and `clean <dumped> <base> <out>`.
#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::hypervisor {

inline constexpr std::size_t kSize = 0x40000;

struct Range {
    std::uint32_t address = 0;
    std::uint32_t size = 0;
};

struct Info {
    std::size_t size = 0;
    std::uint16_t build = 0;
    std::uint16_t qfe = 0;
    std::array<std::uint8_t, 16> white_key{};
    std::array<std::uint8_t, 16> aes_key{};
    std::array<std::uint8_t, 16> hash_key{};
    bool keys_are_55 = false;                          // every key byte 0x55: an image for a console on the test keys
    std::array<std::uint8_t, 16> security_hash{};      // at 0x100F0
    std::array<Range, 6> hash_ranges{};
};

// FormatError for an image shorter than 0x40000 bytes.
Info info(ByteView image);

// The first 0x40000 bytes of `dumped` with each region the patches change replaced by the same region of `original`. A region that
// does not lie inside both is left alone.
Bytes clean(ByteView dumped, ByteView original);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::hypervisor
