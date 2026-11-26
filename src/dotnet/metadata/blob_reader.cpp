// dotnet/metadata/blob_reader.cpp -- the blob reader (System.Reflection.Metadata's BlobReader: the same encodings, the same
// rejections).
#include "dotnet/metadata/blob_reader.hpp"

#include <bit>

namespace cb::dotnet::metadata {

void BlobReader::need(std::size_t count) const {
    if (count > bytes_.size() - pos_) throw BadImageFormatError(base_ + pos_, "a read past the end of a blob");
}

void BlobReader::set_offset(int offset) {
    if (offset < 0 || static_cast<std::size_t>(offset) > bytes_.size()) throw BadImageFormatError(base_, "an offset outside a blob");
    pos_ = static_cast<std::size_t>(offset);
}

std::uint8_t BlobReader::read_byte() {
    need(1);
    return bytes_[pos_++];
}

std::uint64_t BlobReader::read_le(int size) {
    need(static_cast<std::size_t>(size));
    std::uint64_t v = 0;
    for (int i = size - 1; i >= 0; --i) v = v << 8 | bytes_[pos_ + static_cast<std::size_t>(i)];
    pos_ += static_cast<std::size_t>(size);
    return v;
}

float BlobReader::read_single() { return std::bit_cast<float>(read_uint32()); }
double BlobReader::read_double() { return std::bit_cast<double>(read_uint64()); }

ByteView BlobReader::read_bytes(int count) {
    if (count < 0) throw BadImageFormatError(base_ + pos_, "a negative length");
    need(static_cast<std::size_t>(count));
    const ByteView v = bytes_.subspan(pos_, static_cast<std::size_t>(count));
    pos_ += static_cast<std::size_t>(count);
    return v;
}

std::u16string BlobReader::read_utf16(int byte_count) {
    const ByteView b = read_bytes(byte_count);
    std::u16string s;
    s.reserve(b.size() / 2);
    for (std::size_t i = 0; i + 1 < b.size(); i += 2) s.push_back(static_cast<char16_t>(b[i] | b[i + 1] << 8));
    return s;
}

std::string BlobReader::read_utf8(int byte_count) {
    const ByteView b = read_bytes(byte_count);
    return std::string(b.begin(), b.end());
}

std::optional<int> BlobReader::try_read_compressed_integer() {
    if (pos_ >= bytes_.size()) return std::nullopt;
    const std::uint8_t b0 = bytes_[pos_];
    if ((b0 & 0x80) == 0) {
        pos_ += 1;
        return b0;
    }
    if ((b0 & 0xC0) == 0x80) {
        if (bytes_.size() - pos_ < 2) return std::nullopt;
        const int v = (b0 & 0x3F) << 8 | bytes_[pos_ + 1];
        pos_ += 2;
        return v;
    }
    if ((b0 & 0xE0) == 0xC0) {
        if (bytes_.size() - pos_ < 4) return std::nullopt;
        const int v = (b0 & 0x1F) << 24 | bytes_[pos_ + 1] << 16 | bytes_[pos_ + 2] << 8 | bytes_[pos_ + 3];
        pos_ += 4;
        return v;
    }
    return std::nullopt;
}

int BlobReader::read_compressed_integer() {
    const std::size_t at = pos_;
    if (const auto v = try_read_compressed_integer()) return *v;
    throw BadImageFormatError(base_ + at, "an invalid compressed integer");
}

int BlobReader::read_compressed_signed_integer() {
    // II.23.2: the value rotated left by one within its width, the sign in bit 0.
    const std::size_t at = pos_;
    if (pos_ >= bytes_.size()) throw BadImageFormatError(base_ + at, "an invalid compressed integer");
    const std::uint8_t b0 = bytes_[pos_];
    const int raw = read_compressed_integer();
    const bool negative = (raw & 1) != 0;
    const int v = raw >> 1;
    if (!negative) return v;
    if ((b0 & 0x80) == 0) return v - 0x40;
    if ((b0 & 0xC0) == 0x80) return v - 0x2000;
    return v - 0x10000000;
}

SignatureTypeCode BlobReader::read_signature_type_code() {
    const std::uint8_t b = read_byte();
    if (b == kElementTypeClass || b == kElementTypeValueType) return SignatureTypeCode::TypeHandle;
    return static_cast<SignatureTypeCode>(b);
}

EntityHandle BlobReader::read_type_handle() {
    const int v = read_compressed_integer();
    const std::uint32_t row = static_cast<std::uint32_t>(v) >> 2;
    switch (v & 3) {
    case 0: return EntityHandle(HandleKind::TypeDefinition, row);
    case 1: return EntityHandle(HandleKind::TypeReference, row);
    case 2: return EntityHandle(HandleKind::TypeSpecification, row);
    default: return EntityHandle();
    }
}

std::optional<std::string> BlobReader::read_serialized_string() {
    if (pos_ < bytes_.size() && bytes_[pos_] == 0xFF) {
        ++pos_;
        return std::nullopt;
    }
    const int length = read_compressed_integer();
    return read_utf8(length);
}

}  // namespace cb::dotnet::metadata
