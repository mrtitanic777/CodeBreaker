#include "cb/core/io.hpp"

#include <algorithm>
#include <format>

namespace cb::io {

std::optional<ByteView> try_range(ByteView data, std::uint64_t offset, std::uint64_t length) {
    // offset <= size and length <= size - offset cannot overflow, unlike offset + length.
    if (offset > data.size() || length > data.size() - offset) return std::nullopt;
    return data.subspan(static_cast<std::size_t>(offset), static_cast<std::size_t>(length));
}

ByteView checked_range(ByteView data, std::uint64_t offset, std::uint64_t length) {
    if (auto r = try_range(data, offset, length)) return *r;
    throw FormatError("range", offset,
                      std::format("range of {} bytes lies outside the {} bytes of data", length, data.size()));
}

const std::uint8_t* ByteReader::take(std::uint64_t n) {
    if (n > remaining()) throw TruncatedError(name_, pos_, n, remaining());
    const std::uint8_t* p = data_.data() + pos_;
    pos_ += static_cast<std::size_t>(n);
    return p;
}

void ByteReader::seek(std::uint64_t offset) {
    if (offset > data_.size()) throw TruncatedError(name_, offset, 0, 0);
    pos_ = static_cast<std::size_t>(offset);
}

void ByteReader::skip(std::uint64_t count) { (void)take(count); }

void ByteReader::align(std::size_t alignment) {
    CB_ASSERT(alignment != 0 && (alignment & (alignment - 1)) == 0, "alignment must be a power of two");
    const std::size_t target = (pos_ + alignment - 1) & ~(alignment - 1);
    skip(target - pos_);
}

Far16 ByteReader::far16() {
    Far16 p;
    p.off = u16();
    p.seg = u16();
    return p;
}

ByteView ByteReader::bytes(std::uint64_t n) {
    const std::uint8_t* p = take(n);
    return {p, static_cast<std::size_t>(n)};
}

Bytes ByteReader::bytes_copy(std::uint64_t n) {
    ByteView v = bytes(n);
    return Bytes(v.begin(), v.end());
}

ByteView ByteReader::cstr() {
    const std::uint8_t* start = data_.data() + pos_;
    const std::uint8_t* end = data_.data() + data_.size();
    const std::uint8_t* nul = std::find(start, end, std::uint8_t{0});
    if (nul == end) throw TruncatedError(name_, pos_, remaining() + 1, remaining());
    const std::size_t len = static_cast<std::size_t>(nul - start);
    pos_ += len + 1;
    return {start, len};
}

ByteReader ByteReader::sub(std::uint64_t n, std::string_view name) {
    ByteView v = bytes(n);
    return ByteReader(v, name.empty() ? name_ : name);
}

std::optional<std::uint8_t> ByteReader::peek_u8() const {
    ByteReader copy = *this;
    return copy.try_u8();
}
std::optional<std::uint16_t> ByteReader::peek_u16() const {
    ByteReader copy = *this;
    return copy.try_u16();
}
std::optional<std::uint32_t> ByteReader::peek_u32() const {
    ByteReader copy = *this;
    return copy.try_u32();
}

std::optional<ByteView> ByteReader::try_bytes(std::uint64_t n) {
    if (n > remaining()) return std::nullopt;
    return bytes(n);
}

void ByteWriter::align(std::size_t alignment) {
    CB_ASSERT(alignment != 0 && (alignment & (alignment - 1)) == 0, "alignment must be a power of two");
    const std::size_t target = (buf_.size() + alignment - 1) & ~(alignment - 1);
    zeros(target - buf_.size());
}

void ByteWriter::patch_u16(std::size_t offset, std::uint16_t v) {
    CB_ASSERT(offset <= buf_.size() && buf_.size() - offset >= 2, "patch outside the written bytes");
    buf_[offset] = static_cast<std::uint8_t>(v);
    buf_[offset + 1] = static_cast<std::uint8_t>(v >> 8);
}

void ByteWriter::patch_u32(std::size_t offset, std::uint32_t v) {
    CB_ASSERT(offset <= buf_.size() && buf_.size() - offset >= 4, "patch outside the written bytes");
    for (std::size_t i = 0; i < 4; ++i) buf_[offset + i] = static_cast<std::uint8_t>(v >> (8 * i));
}

}  // namespace cb::io
