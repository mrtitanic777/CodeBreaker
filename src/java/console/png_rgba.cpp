#include "java/console/png_rgba.hpp"

#include <array>
#include <cstring>

namespace cb::java::console::png {

namespace {

constexpr std::array<std::uint8_t, 8> kSignature = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};

bool has_signature(ByteView d) { return d.size() >= 8 && std::memcmp(d.data(), kSignature.data(), 8) == 0; }

}  // namespace

std::optional<std::uint8_t> color_type(ByteView d) {
    if (d.size() < 26 || !has_signature(d)) return std::nullopt;
    return d[25];
}

bool needs_rgba(ByteView d) {
    const std::optional<std::uint8_t> ct = color_type(d);
    return ct && *ct != 6;
}

Converted normalize_to_rgba(ByteView d) {
    if (!needs_rgba(d)) return {Bytes(d.begin(), d.end()), false};
    return {encode_rgba(decode_rgba(d)), true};
}

}  // namespace cb::java::console::png
