#include "cb/formats/xbox/crypto/cpukey_ecd.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::cpukey_ecd {

namespace {

constexpr int kDataBits = 0x6A;      // bits 0..105 carry the payload
constexpr int kEcdEnd = 0x7F;        // 106..126 are the correction bits, 127 the parity

bool all_zero(const Key& key) {
    for (const std::uint8_t b : key)
        if (b) return false;
    return true;
}

}  // namespace

Key with_ecd(const Key& key) {
    Key buf = key;
    std::uint32_t acc1 = 0;
    std::uint32_t acc2 = 0;
    for (int i = 0; i < 128; ++i) {
        std::uint8_t& byte = buf[static_cast<std::size_t>(i >> 3)];
        const std::uint32_t bit = (byte >> (i & 7)) & 1u;
        if (i < kDataBits) {
            acc1 ^= bit;
            if (acc1 & 1u) acc1 ^= kEcdMagic;
            acc2 ^= bit;
        } else if (i < kEcdEnd) {
            if (bit != (acc1 & 1u)) byte = static_cast<std::uint8_t>(byte ^ (1u << (i & 7)));
            acc2 ^= acc1 & 1u;
        } else if (bit != acc2) {
            buf[0xF] = static_cast<std::uint8_t>(buf[0xF] ^ 0x80u);
        }
        acc1 >>= 1;
    }
    return buf;
}

bool verify_ecd(const Key& key) { return with_ecd(key) == key; }

int hamming_weight(const Key& key) {
    int weight = 0;
    for (std::size_t i = 0; i < 13; ++i) weight += std::popcount(key[i]);
    return weight + std::popcount(static_cast<std::uint8_t>(key[13] & 0x03));
}

Validation validate(std::string_view key_hex) {
    Validation result;
    std::string digits;
    for (const char c : text::trim(key_hex))
        if (c != ' ' && c != '-') digits += c;
    if (digits.size() != 32) {
        result.errors.push_back("Length " + std::to_string(digits.size()) + " != 32 hex chars");
        return result;
    }
    result.length_ok = true;
    Key key{};
    try {
        const Bytes bytes = cb::crypto::from_hex(digits);
        std::copy(bytes.begin(), bytes.end(), key.begin());
    } catch (const FormatError&) {
        result.errors.push_back("Invalid hex characters");
        return result;
    }
    if (all_zero(key)) {
        result.errors.push_back("Key is all zeros");
        return result;
    }
    result.non_zero = true;
    result.hamming_weight = hamming_weight(key);
    if (result.hamming_weight == kValidHammingWeight)
        result.hamming_ok = true;
    else
        result.errors.push_back(std::format("Hamming weight {} != {} (0x{:X})", result.hamming_weight, kValidHammingWeight, kValidHammingWeight));
    if (verify_ecd(key))
        result.ecd_ok = true;
    else
        result.errors.push_back("ECD bits invalid");
    result.valid = result.hamming_ok && result.ecd_ok;
    return result;
}

Key generate(xecrypt::RandomSource& random) {
    for (;;) {
        const Bytes bytes = random.bytes(16);
        Key key{};
        std::copy(bytes.begin(), bytes.end(), key.begin());
        if (hamming_weight(key) == kValidHammingWeight) return with_ecd(key);
    }
}

cli::Command command() {
    return cli::Command{
        "cpukey_ecd",
        "validate or generate an Xbox 360 CPU key (ECD and Hamming weight)",
        {cli::Positional{"key", "a 32-digit hex CPU key, or `gen` for a new one"}},
        [](const cli::Args& args) {
            const std::string what = args.required("key");
            if (what == "gen" || what == "generate") {
                const Key key = generate();
                cb::out("{}\n", support::hex_upper(key));
                return 0;
            }
            const Validation v = validate(what);
            const auto yes = [](bool b) { return b ? "yes" : "no"; };
            cb::out("  {:<18} {}\n  {:<18} {}\n  {:<18} {}\n  {:<18} {}\n  {:<18} {}\n  {:<18} {}\n", "valid", yes(v.valid), "length_ok", yes(v.length_ok),
                    "non_zero", yes(v.non_zero), "hamming_ok", yes(v.hamming_ok), "hamming_weight", v.hamming_weight, "ecd_ok", yes(v.ecd_ok));
            for (const std::string& error : v.errors) cb::out("  error: {}\n", error);
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::cpukey_ecd
