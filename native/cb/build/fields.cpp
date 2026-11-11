#include "cb/build/fields.hpp"

#include <format>

#include "cb/core/io.hpp"

namespace cb::build {

std::uint16_t u16_at(ByteView data, std::uint64_t offset, std::string_view reader) {
    io::ByteReader r(data, reader);
    r.seek(offset);
    return r.u16();
}

std::uint32_t u32_at(ByteView data, std::uint64_t offset, std::string_view reader) {
    io::ByteReader r(data, reader);
    r.seek(offset);
    return r.u32();
}

namespace {

void put_le(ByteSpan data, std::uint64_t offset, std::int64_t value, unsigned width, std::string_view reader) {
    if (offset > data.size() || data.size() - offset < width) {
        throw TruncatedError(reader, offset, width, offset > data.size() ? 0 : data.size() - offset);
    }
    const std::int64_t top = (std::int64_t{1} << (8 * width)) - 1;
    if (value < 0 || value > top) {
        throw FormatError(reader, offset, std::format("{} does not fit the {}-byte field here", value, width));
    }
    for (unsigned i = 0; i < width; ++i) data[offset + i] = static_cast<std::uint8_t>(value >> (8 * i));
}

}  // namespace

void put_u16_at(ByteSpan data, std::uint64_t offset, std::int64_t value, std::string_view reader) {
    put_le(data, offset, value, 2, reader);
}

void put_u32_at(ByteSpan data, std::uint64_t offset, std::int64_t value, std::string_view reader) {
    put_le(data, offset, value, 4, reader);
}

std::uint64_t align_up(std::uint64_t n, std::uint64_t unit, std::string_view reader) {
    if (unit == 0) throw FormatError(reader, 0, "an alignment of zero");
    const std::uint64_t padded = n + (unit - 1);
    if (padded < n) throw FormatError(reader, n, "offset overflows when aligned");
    return padded / unit * unit;
}

}  // namespace cb::build
