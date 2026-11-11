// cb/formats/le_internal.hpp -- helpers shared by the .cpp files of the LE reader (le.cpp, le_parts.cpp, le_w4.cpp).
#pragma once

#include <cstdint>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats::le::detail {

inline constexpr std::string_view kReader = "le";

// A big-endian value of up to 3 bytes at `off` (a page-map number); tolerant of a table that is cut short.
std::uint32_t be24(ByteView d, std::uint64_t off);
// bytes.rstrip(b" \x00")
ByteView rstrip_blanks(ByteView b);

}  // namespace cb::formats::le::detail
