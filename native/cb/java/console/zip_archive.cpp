#include "cb/java/console/zip_archive.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <format>

#include <zlib.h>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::java::console {

namespace {

constexpr std::string_view kReader = "zip";
constexpr std::uint16_t kUtf8NameFlag = 0x800;
constexpr std::uint16_t kEncryptedFlag = 0x1;
constexpr std::size_t kEndRecordBytes = 22;
constexpr std::size_t kMaxComment = 65535;
constexpr std::uint32_t kZip64Marker = 0xFFFFFFFF;

std::string decode_name(ByteView raw, std::uint16_t flags) {
    if ((flags & kUtf8NameFlag) != 0) return text::sanitize_utf8(as_text(raw));
    return text::from_cp437(raw);   // an entry without the UTF-8 flag is in the DOS code page
}

// The name as an entry is looked up by: cut at a NUL, with '\' read as '/'.
std::string lookup_name(std::string name) {
    if (const std::size_t nul = name.find('\0'); nul != std::string::npos) name.resize(nul);
    std::replace(name.begin(), name.end(), '\\', '/');
    return name;
}

}  // namespace

ZipArchive ZipArchive::open(const std::filesystem::path& path) {
    return from_bytes(fs::read_file(path), fs::to_text(path.filename()));
}

ZipArchive ZipArchive::from_bytes(Bytes data, std::string name) {
    ZipArchive z;
    z.data_ = std::move(data);
    z.name_ = std::move(name);
    const ByteView d = z.data_;

    // the end-of-central-directory record: in the last 22 bytes, or (a comment follows) found by scanning back
    constexpr std::array<std::uint8_t, 4> kEndSignature = {'P', 'K', 5, 6};
    std::optional<std::size_t> pos;
    if (d.size() >= kEndRecordBytes) {
        const std::size_t last = d.size() - kEndRecordBytes;
        const std::size_t lowest = d.size() > kMaxComment + kEndRecordBytes ? d.size() - kMaxComment - kEndRecordBytes : 0;
        for (std::size_t i = last + 1; i-- > lowest;) {
            if (std::memcmp(d.data() + i, kEndSignature.data(), 4) == 0) {
                pos = i;
                break;
            }
        }
    }
    if (!pos) throw FormatError(kReader, 0, z.name_ + ": not a zip file (no end-of-central-directory record)");

    io::ByteReader end(d, kReader);
    end.seek(*pos + 10);
    const std::uint16_t entry_count = end.u16();
    const std::uint32_t directory_size = end.u32();
    const std::uint32_t directory_offset = end.u32();
    if (directory_offset == kZip64Marker || directory_size == kZip64Marker || entry_count == 0xFFFF)
        throw UnsupportedError(kReader, 0, z.name_ + ": zip64 archives are not read");
    // data may precede the archive (a self-extractor): the directory is where the end record says, less the prefix
    const std::int64_t prefix = static_cast<std::int64_t>(*pos) - directory_size - directory_offset;
    if (prefix < 0) throw FormatError(kReader, *pos, "bad offset for the central directory");

    io::ByteReader dir(d, kReader);
    dir.seek(static_cast<std::uint64_t>(directory_offset) + static_cast<std::uint64_t>(prefix));
    std::uint64_t consumed = 0;
    while (consumed < directory_size) {
        const std::size_t at = dir.pos();
        if (dir.remaining() < 46 || std::memcmp(d.data() + at, "PK\x01\x02", 4) != 0)
            throw FormatError(kReader, at, "bad magic number for the central directory");
        dir.skip(8);
        Entry e;
        e.flags = dir.u16();
        e.method = dir.u16();
        dir.skip(4);   // modification time and date
        e.crc = dir.u32();
        e.compressed_size = dir.u32();
        e.size = dir.u32();
        const std::uint16_t name_length = dir.u16(), extra_length = dir.u16(), comment_length = dir.u16();
        dir.skip(8);   // disk number, internal and external attributes
        const std::uint32_t header_offset = dir.u32();
        const ByteView raw_name = dir.bytes(name_length);
        dir.skip(static_cast<std::uint64_t>(extra_length) + comment_length);
        if (e.compressed_size == kZip64Marker || e.size == kZip64Marker || header_offset == kZip64Marker)
            throw UnsupportedError(kReader, at, z.name_ + ": zip64 entries are not read");
        e.stored_name = decode_name(raw_name, e.flags);
        e.name = lookup_name(e.stored_name);
        e.header_offset = static_cast<std::uint64_t>(header_offset) + static_cast<std::uint64_t>(prefix);
        consumed += 46 + static_cast<std::uint64_t>(name_length) + extra_length + comment_length;
        z.by_name_[e.name] = z.entries_.size();   // a repeated name: the last one wins
        z.entries_.push_back(std::move(e));
    }
    return z;
}

const ZipArchive::Entry* ZipArchive::find(std::string_view name) const {
    const auto it = by_name_.find(name);
    return it == by_name_.end() ? nullptr : &entries_[it->second];
}

Bytes ZipArchive::read(const Entry& e) const {
    if ((e.flags & kEncryptedFlag) != 0) throw UnsupportedError(kReader, e.header_offset, std::format("{}: entry '{}' is encrypted", name_, e.name));
    if (e.method != 0 && e.method != 8)
        throw UnsupportedError(kReader, e.header_offset, std::format("{}: entry '{}' uses compression method {}", name_, e.name, e.method));

    io::ByteReader local(data_, kReader);
    local.seek(e.header_offset);
    if (local.remaining() < 30 || std::memcmp(data_.data() + local.pos(), "PK\x03\x04", 4) != 0)
        throw FormatError(kReader, e.header_offset, "bad magic number for an entry header");
    local.skip(6);
    const std::uint16_t local_flags = local.u16();
    local.skip(18);   // method, time, date, CRC and the two sizes
    const std::uint16_t name_length = local.u16(), extra_length = local.u16();
    const ByteView header_name = local.bytes(name_length);
    if (decode_name(header_name, local_flags) != e.stored_name)
        throw FormatError(kReader, e.header_offset, std::format("name in the directory ('{}') and in the entry header differ", e.name));
    local.skip(extra_length);
    const ByteView body = local.bytes(e.compressed_size);   // TruncatedError when the archive ends early

    Bytes out;
    if (e.method == 0) {
        out.assign(body.begin(), body.end());
    } else {
        // deflate cannot expand more than about 1032 to 1; a directory that claims more is lying about the size
        if (e.size > static_cast<std::uint64_t>(e.compressed_size) * 1032 + 1024)
            throw FormatError(kReader, e.header_offset, std::format("entry '{}' claims an impossible size", e.name));
        out.resize(e.size);
        z_stream zs;
        std::memset(&zs, 0, sizeof zs);
        if (inflateInit2(&zs, -MAX_WBITS) != Z_OK) throw InternalError("zlib could not start");   // a raw deflate stream
        zs.next_in = const_cast<Bytef*>(body.data());
        zs.avail_in = static_cast<uInt>(body.size());
        zs.next_out = out.data();
        zs.avail_out = static_cast<uInt>(out.size());
        const int rc = out.empty() ? Z_STREAM_END : inflate(&zs, Z_FINISH);
        const std::size_t produced = out.size() - zs.avail_out;
        inflateEnd(&zs);
        if (rc != Z_STREAM_END || produced != out.size())
            throw FormatError(kReader, e.header_offset, std::format("entry '{}' is damaged (the compressed data is bad)", e.name));
    }
    if (crypto::crc32(out) != e.crc) throw FormatError(kReader, e.header_offset, std::format("bad CRC-32 for entry '{}'", e.name));
    return out;
}

Bytes ZipArchive::read(std::string_view name) const {
    const Entry* e = find(name);
    if (!e) throw FormatError(kReader, 0, std::format("{}: there is no entry '{}'", name_, name));
    return read(*e);
}

}  // namespace cb::java::console
