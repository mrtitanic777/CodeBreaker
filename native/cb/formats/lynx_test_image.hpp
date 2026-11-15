// cb/formats/lynx_test_image.hpp -- a small bootable Lynx cartridge for tests (header only; a test includes it).
//
// The boot ROM decrypts the cartridge's first stage with the public key (cube modulo N); the file the tests need is one the ROM would run,
// so its first stage is *encrypted* here with the matching private exponent (the pair Handy and Felix publish): c = m^d mod N, where m is the
// 51-byte block (50 bytes of differences of the plain code, and the check byte $15 on top).
//
//     Bytes file = lynx_test_image::cartridge(program);   // a `.lnx` that loads `program` at $0400 and runs it
//
// The first stage is 16 bytes of 65C02 that read `program.size()` bytes (at most 255) from RCART0 into $0400 and jump there; the program
// follows the encrypted block in the cartridge, so it is loaded from file offset 64 + 52.
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats::lynx_test_image {

namespace detail {

using Limbs = std::array<std::uint32_t, 17>;

inline Limbs from_hex(std::string_view hex) {
    Limbs n{};
    std::size_t bit = 0;
    for (std::size_t i = hex.size(); i-- > 0;) {
        const char c = hex[i];
        n[bit / 32] |= static_cast<std::uint32_t>(c <= '9' ? c - '0' : c - 'a' + 10) << (bit % 32);
        bit += 4;
    }
    return n;
}
inline bool less(const Limbs& a, const Limbs& b) {
    for (std::size_t i = a.size(); i-- > 0;) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}
inline void sub(Limbs& a, const Limbs& b) {
    std::uint64_t borrow = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t v = static_cast<std::uint64_t>(a[i]) - b[i] - borrow;
        a[i] = static_cast<std::uint32_t>(v);
        borrow = (v >> 63) & 1;
    }
}
inline void add(Limbs& a, const Limbs& b) {
    std::uint64_t carry = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t v = static_cast<std::uint64_t>(a[i]) + b[i] + carry;
        a[i] = static_cast<std::uint32_t>(v);
        carry = v >> 32;
    }
}
inline Limbs mul_mod(const Limbs& a, const Limbs& b, const Limbs& n) {
    Limbs r{};
    for (std::size_t bit = 512; bit-- > 0;) {
        std::uint32_t carry = 0;
        for (std::uint32_t& limb : r) {
            const std::uint32_t next = limb >> 31;
            limb = (limb << 1) | carry;
            carry = next;
        }
        if (!less(r, n)) sub(r, n);
        if ((b[bit / 32] >> (bit % 32)) & 1) {
            add(r, a);
            if (!less(r, n)) sub(r, n);
        }
    }
    return r;
}

}  // namespace detail

// One 51-byte encrypted block of `plain` (50 bytes whose last is 0, so the running sum ends at 0).
inline Bytes encrypt_block(const std::array<std::uint8_t, 50>& plain) {
    using namespace detail;
    static const Limbs n = from_hex("35b5a3942806d8a22695d771b23cfd561c4a19b6a3b02600365a306e3c4d63381bd41c136489364cf2ba2a58f4fee1fdac7e79");
    static const Limbs d = from_hex("23ce6d0d7004906c19b93a4bcc28a8e412dc11246d2019557987ab5ca818a3d3c8e3276d4270cb8021d6bda4296d47b1e5e2a3");
    Limbs m{};
    std::uint8_t previous = 0;
    for (std::size_t i = 0; i < 50; ++i) {
        const std::uint8_t diff = static_cast<std::uint8_t>(plain[i] - previous);
        previous = plain[i];
        m[i / 4] |= static_cast<std::uint32_t>(diff) << (8 * (i % 4));
    }
    m[50 / 4] |= std::uint32_t{0x15} << (8 * (50 % 4));
    Limbs result{};
    result[0] = 1;
    Limbs base = m;
    for (std::size_t bit = 0; bit < 512; ++bit) {   // square and multiply, low bit first
        if ((d[bit / 32] >> (bit % 32)) & 1) result = mul_mod(result, base, n);
        base = mul_mod(base, base, n);
    }
    Bytes out;
    for (std::size_t i = 0; i < 51; ++i) out.push_back(static_cast<std::uint8_t>(result[i / 4] >> (8 * (i % 4))));
    return out;
}

// A `.lnx`: the header, the count byte $FF (one block), the encrypted first stage, then `program`, padded with $FF to `size` bytes of cartridge.
inline Bytes cartridge(ByteView program, std::string_view name = "test", std::size_t size = 1024) {
    std::array<std::uint8_t, 50> stage{};
    const std::uint8_t count = static_cast<std::uint8_t>(program.size());
    const std::uint8_t code[] = {0xA2, 0x00,                // LDX #0
                                 0xAD, 0xB2, 0xFC,          // LDA RCART0
                                 0x9D, 0x00, 0x04,          // STA $0400,X
                                 0xE8,                      // INX
                                 0xE0, count,               // CPX #count
                                 0xD0, 0xF5,                // BNE back to the LDA
                                 0x4C, 0x00, 0x04};         // JMP $0400
    std::copy(std::begin(code), std::end(code), stage.begin());
    Bytes file = {'L', 'Y', 'N', 'X', 0x00, 0x04, 0, 0, 1, 0};
    file.resize(64, 0);
    for (std::size_t i = 0; i < name.size() && i < 31; ++i) file[10 + i] = static_cast<std::uint8_t>(name[i]);
    file.push_back(0xFF);
    const Bytes block = encrypt_block(stage);
    file.insert(file.end(), block.begin(), block.end());
    file.insert(file.end(), program.begin(), program.end());
    file.resize(64 + size, 0xFF);
    return file;
}

}  // namespace cb::formats::lynx_test_image
