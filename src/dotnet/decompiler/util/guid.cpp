#include "dotnet/decompiler/util/guid.hpp"

#include <cstdint>
#include <random>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util::guid {

Guid new_guid() {
    static thread_local std::mt19937_64 engine{std::random_device{}()};
    Guid g;
    const std::uint64_t a = engine(), b = engine();
    for (int i = 0; i < 8; ++i) {
        g.bytes[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(a >> (8 * i));
        g.bytes[static_cast<std::size_t>(8 + i)] = static_cast<std::uint8_t>(b >> (8 * i));
    }
    // version 4: the high nibble of the third group (bytes 6-7 little-endian: byte 7 holds the high byte), variant 1: the top bits of byte 8
    g.bytes[7] = static_cast<std::uint8_t>((g.bytes[7] & 0x0F) | 0x40);
    g.bytes[8] = static_cast<std::uint8_t>((g.bytes[8] & 0x3F) | 0x80);
    return g;
}

namespace {

int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

}  // namespace

Guid parse(std::string_view text) {
    std::string_view t = text;
    // trim white space
    while (!t.empty() && (t.front() == ' ' || t.front() == '\t')) t.remove_prefix(1);
    while (!t.empty() && (t.back() == ' ' || t.back() == '\t')) t.remove_suffix(1);
    auto bad = [&]() -> Guid { throw UsageError("the text is not a GUID: " + std::string(text)); };
    if (t.size() >= 2 && ((t.front() == '{' && t.back() == '}') || (t.front() == '(' && t.back() == ')'))) t = t.substr(1, t.size() - 2);
    else if (!t.empty() && (t.front() == '{' || t.front() == '(' || t.back() == '}' || t.back() == ')')) return bad();
    std::string digits;
    if (t.size() == 36) {
        for (std::size_t i = 0; i < t.size(); ++i) {
            if (i == 8 || i == 13 || i == 18 || i == 23) {
                if (t[i] != '-') return bad();
            } else {
                digits.push_back(t[i]);
            }
        }
    } else if (t.size() == 32) {
        digits = std::string(t);
    } else {
        return bad();
    }
    std::uint8_t raw[16];
    for (int i = 0; i < 16; ++i) {
        const int hi = hex_value(digits[static_cast<std::size_t>(2 * i)]);
        const int lo = hex_value(digits[static_cast<std::size_t>(2 * i + 1)]);
        if (hi < 0 || lo < 0) return bad();
        raw[i] = static_cast<std::uint8_t>(hi * 16 + lo);
    }
    // the text order is a, b, c (int, short, short, little-endian in memory), then eight bytes in order
    Guid g;
    g.bytes[0] = raw[3];
    g.bytes[1] = raw[2];
    g.bytes[2] = raw[1];
    g.bytes[3] = raw[0];
    g.bytes[4] = raw[5];
    g.bytes[5] = raw[4];
    g.bytes[6] = raw[7];
    g.bytes[7] = raw[6];
    for (int i = 8; i < 16; ++i) g.bytes[static_cast<std::size_t>(i)] = raw[i];
    return g;
}

std::string to_string(const Guid& guid, char format) {
    static const char* const kDigits = "0123456789abcdef";
    // the 16 bytes in text order
    const std::uint8_t order[16] = {guid.bytes[3], guid.bytes[2], guid.bytes[1], guid.bytes[0], guid.bytes[5], guid.bytes[4], guid.bytes[7], guid.bytes[6],
                                    guid.bytes[8], guid.bytes[9], guid.bytes[10], guid.bytes[11], guid.bytes[12], guid.bytes[13], guid.bytes[14], guid.bytes[15]};
    std::string digits;
    const bool dashes = format != 'N';
    for (int i = 0; i < 16; ++i) {
        if (dashes && (i == 4 || i == 6 || i == 8 || i == 10)) digits.push_back('-');
        digits.push_back(kDigits[order[i] >> 4]);
        digits.push_back(kDigits[order[i] & 15]);
    }
    switch (format) {
    case 'D':
    case 'N': return digits;
    case 'B': return "{" + digits + "}";
    case 'P': return "(" + digits + ")";
    default: throw UsageError(std::string("an unknown GUID format: ") + format);
    }
}

std::string to_string_b_upper(const Guid& guid) {
    std::string s = to_string(guid, 'B');
    for (char& c : s)
        if (c >= 'a' && c <= 'f') c = static_cast<char>(c - 'a' + 'A');
    return s;
}

}  // namespace cb::dotnet::decompiler::util::guid
