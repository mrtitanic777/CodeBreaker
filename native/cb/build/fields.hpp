// cb/build/fields.hpp -- single little-endian fields of a binary at a file offset, read and written with checks.
//
// The layout builders (mzfile, vxdfile, stubjoin, exepack, period16) move whole headers around and patch a field
// here and there: "the word at 0x3C", "the dword at lfa+0x88". These are those two operations. A read past the
// end is a TruncatedError; a write outside the buffer, or of a value that does not fit the field, is a
// FormatError (the file's own numbers no longer fit once something moved: the build must stop, not wrap).
#pragma once

#include <cstdint>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::build {

std::uint16_t u16_at(ByteView data, std::uint64_t offset, std::string_view reader);
std::uint32_t u32_at(ByteView data, std::uint64_t offset, std::string_view reader);

// `value` is signed because a moved offset is `old + delta` and may come out negative: that is the error.
void put_u16_at(ByteSpan data, std::uint64_t offset, std::int64_t value, std::string_view reader);
void put_u32_at(ByteSpan data, std::uint64_t offset, std::int64_t value, std::string_view reader);

// `n` rounded up to a multiple of `unit` (> 0). FormatError instead of overflow.
std::uint64_t align_up(std::uint64_t n, std::uint64_t unit, std::string_view reader);

}  // namespace cb::build
