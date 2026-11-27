// dotnet/decompiler/ilspycmd/single_file_bundle.cpp -- see single_file_bundle.hpp.
#include "dotnet/decompiler/ilspycmd/single_file_bundle.hpp"

#include <algorithm>
#include <array>
#include <cstring>

#include "dotnet/decompiler/util/resources_file.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

// A BinaryReader over the data from an offset.
class Reader {
public:
    Reader(ByteView data, std::int64_t position) : data_(data), position_(position) {}

    std::int64_t position() const { return position_; }
    std::int64_t length() const { return static_cast<std::int64_t>(data_.size()); }

    std::uint8_t read_byte() {
        need(1);
        return data_[static_cast<std::size_t>(position_++)];
    }
    std::uint32_t read_uint32() { return static_cast<std::uint32_t>(read_unsigned(4)); }
    std::int32_t read_int32() { return static_cast<std::int32_t>(read_unsigned(4)); }
    std::int64_t read_int64() { return static_cast<std::int64_t>(read_unsigned(8)); }
    std::uint64_t read_uint64() { return read_unsigned(8); }
    // BinaryReader.ReadString: a 7-bit encoded length, then that many bytes of UTF-8.
    std::string read_string() {
        std::uint32_t length = 0;
        int shift = 0;
        while (true) {
            if (shift == 35) throw InvalidDataError("Too many bytes in what should have been a 7-bit encoded integer.");
            const std::uint8_t b = read_byte();
            length |= static_cast<std::uint32_t>(b & 0x7F) << shift;
            shift += 7;
            if ((b & 0x80) == 0) break;
        }
        if (static_cast<std::int32_t>(length) < 0) throw InvalidDataError("Invalid string length.");
        need(length);
        std::string text(reinterpret_cast<const char*>(data_.data()) + position_, length);
        position_ += length;
        return text;
    }

private:
    void need(std::uint64_t count) const {
        if (position_ < 0 || static_cast<std::uint64_t>(position_) + count > data_.size()) throw util::EndOfStreamError();
    }
    std::uint64_t read_unsigned(int bytes) {
        need(static_cast<std::uint64_t>(bytes));
        std::uint64_t v = 0;
        for (int i = 0; i < bytes; ++i) v |= static_cast<std::uint64_t>(data_[static_cast<std::size_t>(position_ + i)]) << (8 * i);
        position_ += bytes;
        return v;
    }

    ByteView data_;
    std::int64_t position_;
};

}  // namespace

bool SingleFileBundle::is_bundle(ByteView data, std::int64_t* bundle_header_offset) {
    // 32 bytes represent the bundle signature: SHA-256 for ".net core bundle"
    static constexpr std::array<std::uint8_t, 32> bundle_signature = {
        0x8b, 0x12, 0x02, 0xb9, 0x6a, 0x61, 0x20, 0x38, 0x72, 0x7b, 0x93, 0x02, 0x14, 0xd7, 0xa0, 0x32,
        0x13, 0xf5, 0xb9, 0xe6, 0xef, 0xae, 0x33, 0x18, 0xee, 0x3b, 0x2d, 0xce, 0x24, 0xb3, 0x6a, 0xae};
    const std::int64_t size = static_cast<std::int64_t>(data.size());
    const std::int64_t end = size - static_cast<std::int64_t>(bundle_signature.size());
    for (std::int64_t p = 0; p < end; ++p) {
        if (data[static_cast<std::size_t>(p)] == 0x8b && std::memcmp(data.data() + p, bundle_signature.data(), bundle_signature.size()) == 0) {
            // A genuine bundle stores the 8-byte header offset immediately before the signature, so the signature never appears within
            // the first sizeof(long) bytes of the file.
            if (p >= 8) {
                std::int64_t offset;
                std::memcpy(&offset, data.data() + p - 8, 8);
                if (offset > 0 && offset < size) {
                    *bundle_header_offset = offset;
                    return true;
                }
            }
        }
    }
    *bundle_header_offset = 0;
    return false;
}

SingleFileBundle::Header SingleFileBundle::read_manifest(ByteView data, std::int64_t bundle_header_offset) {
    Reader reader(data, bundle_header_offset);
    Header header;
    header.major_version = reader.read_uint32();
    header.minor_version = reader.read_uint32();

    // Major versions 3, 4 and 5 were skipped to align bundle versioning with .NET versioning scheme
    if (header.major_version < 1 || header.major_version > 6)
        throw InvalidDataError("Unsupported manifest version: " + std::to_string(header.major_version) + "." + std::to_string(header.minor_version));
    header.file_count = reader.read_int32();
    // Each entry occupies at least one byte in the stream, so a count larger than the bytes that remain cannot be honest.
    const std::int64_t remaining_bytes = reader.length() - reader.position();
    if (header.file_count < 0 || header.file_count > remaining_bytes)
        throw InvalidDataError("Invalid bundle manifest: FileCount " + std::to_string(header.file_count) + " exceeds available data.");
    header.bundle_id = reader.read_string();
    if (header.major_version >= 2) {
        header.deps_json_offset = reader.read_int64();
        header.deps_json_size = reader.read_int64();
        header.runtime_config_json_offset = reader.read_int64();
        header.runtime_config_json_size = reader.read_int64();
        header.flags = reader.read_uint64();
    }
    header.entries.reserve(static_cast<std::size_t>(header.file_count));
    for (int i = 0; i < header.file_count; ++i) {
        Entry entry;
        entry.offset = reader.read_int64();
        entry.size = reader.read_int64();
        entry.compressed_size = header.major_version >= 6 ? reader.read_int64() : 0;
        entry.type = static_cast<FileType>(reader.read_byte());
        entry.relative_path = reader.read_string();
        header.entries.push_back(std::move(entry));
    }
    return header;
}

}  // namespace cb::dotnet::decompiler::ilspycmd
