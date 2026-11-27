#include "formats/zip.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "formats/deflate.hpp"

namespace cb::formats {

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

ZipArchive ZipArchive::from_bytes(Bytes data, std::string name, Options options) {
    ZipArchive z;
    z.owned_ = std::move(data);
    z.name_ = std::move(name);
    z.options_ = options;
    return parse(std::move(z));
}

ZipArchive ZipArchive::view(ByteView data, std::string name, Options options) {
    ZipArchive z;
    z.borrowed_ = data;
    z.name_ = std::move(name);
    z.options_ = options;
    return parse(std::move(z));
}

ZipArchive ZipArchive::parse(ZipArchive z) {
    const ByteView d = z.data();

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
    std::uint64_t entry_count = end.u16();
    std::uint64_t directory_size = end.u32();
    std::uint64_t directory_offset = end.u32();
    std::int64_t prefix = 0;
    if (directory_offset == kZip64Marker || directory_size == kZip64Marker || entry_count == 0xFFFF) {
        if (!z.options_.zip64) throw UnsupportedError(kReader, 0, z.name_ + ": zip64 archives are not read");
        // the record the locator before the end record names
        io::ByteReader locator(d, kReader);
        if (*pos < 20) throw FormatError(kReader, *pos, "a zip64 archive without its locator");
        locator.seek(*pos - 20);
        if (locator.u32() != 0x07064B50) throw FormatError(kReader, *pos, "a zip64 archive without its locator");
        locator.skip(4);
        const std::uint64_t record = locator.u64();
        io::ByteReader big(d, kReader);
        big.seek(record);
        if (big.u32() != 0x06064B50) throw FormatError(kReader, record, "the zip64 end record has no signature");
        big.seek(record + 32);
        entry_count = big.u64();
        directory_size = big.u64();
        directory_offset = big.u64();
        if (directory_offset > d.size() || entry_count > 100000) throw FormatError(kReader, *pos, "the central directory is outside the file, or lists too many files");
    } else {
        // data may precede the archive (a self-extractor): the directory is where the end record says, less the prefix
        prefix = static_cast<std::int64_t>(*pos) - static_cast<std::int64_t>(directory_size) - static_cast<std::int64_t>(directory_offset);
        if (prefix < 0) throw FormatError(kReader, *pos, "bad offset for the central directory");
    }

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
        std::uint64_t header_offset = dir.u32();
        const ByteView raw_name = dir.bytes(name_length);
        const ByteView extra = dir.bytes(extra_length);
        dir.skip(comment_length);
        if (e.compressed_size == kZip64Marker || e.size == kZip64Marker || header_offset == kZip64Marker) {
            if (!z.options_.zip64) throw UnsupportedError(kReader, at, z.name_ + ": zip64 entries are not read");
            // a field too large for 32 bits is 0xFFFFFFFF and the zip64 extra field (id 1) holds it, in the order size, compressed size, header offset
            io::ByteReader fields(extra, kReader);
            while (fields.remaining() >= 4) {
                const std::uint16_t id = fields.u16(), length = fields.u16();
                const ByteView body = fields.bytes(std::min<std::size_t>(length, fields.remaining()));
                if (id != 1) continue;
                io::ByteReader field(body, kReader);
                if (e.size == kZip64Marker) e.size = field.u64();
                if (e.compressed_size == kZip64Marker) e.compressed_size = field.u64();
                if (header_offset == kZip64Marker) header_offset = field.u64();
            }
        }
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

    const ByteView d = data();
    io::ByteReader local(d, kReader);
    local.seek(e.header_offset);
    if (local.remaining() < 30 || std::memcmp(d.data() + local.pos(), "PK\x03\x04", 4) != 0)
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
        if (e.size > e.compressed_size * 1032 + 1024)
            throw FormatError(kReader, e.header_offset, std::format("entry '{}' claims an impossible size", e.name));
        deflate::Inflated r = deflate::inflate_exact(body, e.size, deflate::Wrap::raw);   // a raw deflate stream
        if (r.status != deflate::Status::end || r.data.size() != e.size)
            throw FormatError(kReader, e.header_offset, std::format("entry '{}' is damaged (the compressed data is bad)", e.name));
        out = std::move(r.data);
    }
    if (options_.check_crc && crypto::crc32(out) != e.crc) throw FormatError(kReader, e.header_offset, std::format("bad CRC-32 for entry '{}'", e.name));
    return out;
}

Bytes ZipArchive::read(std::string_view name) const {
    const Entry* e = find(name);
    if (!e) throw FormatError(kReader, 0, std::format("{}: there is no entry '{}'", name_, name));
    return read(*e);
}

// ---- ZipWriter

namespace {
void zw_u16(Bytes& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
}
void zw_u32(Bytes& b, std::uint32_t v) {
    zw_u16(b, v & 0xFFFF);
    zw_u16(b, v >> 16);
}
}  // namespace

void ZipWriter::add(const std::string& name, ByteView data) {
    if (out_.size() > 0xFFFFFFFFull || data.size() > 0xFFFFFFFFull) throw InternalError("a zip over 4 GB (no zip64)");
    Written w;
    w.name = name;
    w.size = static_cast<std::uint32_t>(data.size());
    w.crc = crypto::crc32(data);
    w.offset = static_cast<std::uint32_t>(out_.size());
    zw_u32(out_, 0x04034b50);
    zw_u16(out_, 10);           // version needed
    zw_u16(out_, 0x0800);       // UTF-8 names
    zw_u16(out_, 0);            // stored
    zw_u16(out_, 0);            // time
    zw_u16(out_, 0x21);         // date: 1980-01-01
    zw_u32(out_, w.crc);
    zw_u32(out_, w.size);
    zw_u32(out_, w.size);
    zw_u16(out_, static_cast<std::uint32_t>(name.size()));
    zw_u16(out_, 0);
    out_.insert(out_.end(), name.begin(), name.end());
    out_.insert(out_.end(), data.begin(), data.end());
    written_.push_back(std::move(w));
}

Bytes ZipWriter::finish() {
    const auto directory = static_cast<std::uint32_t>(out_.size());
    for (const Written& w : written_) {
        zw_u32(out_, 0x02014b50);
        zw_u16(out_, 20);
        zw_u16(out_, 10);
        zw_u16(out_, 0x0800);
        zw_u16(out_, 0);
        zw_u16(out_, 0);
        zw_u16(out_, 0x21);
        zw_u32(out_, w.crc);
        zw_u32(out_, w.size);
        zw_u32(out_, w.size);
        zw_u16(out_, static_cast<std::uint32_t>(w.name.size()));
        zw_u16(out_, 0);
        zw_u16(out_, 0);
        zw_u16(out_, 0);
        zw_u16(out_, 0);
        zw_u32(out_, 0);
        zw_u32(out_, w.offset);
        out_.insert(out_.end(), w.name.begin(), w.name.end());
    }
    const auto size = static_cast<std::uint32_t>(out_.size()) - directory;
    if (written_.size() > 0xFFFF) throw InternalError("a zip of over 65535 entries (no zip64)");
    zw_u32(out_, 0x06054b50);
    zw_u16(out_, 0);
    zw_u16(out_, 0);
    zw_u16(out_, static_cast<std::uint32_t>(written_.size()));
    zw_u16(out_, static_cast<std::uint32_t>(written_.size()));
    zw_u32(out_, size);
    zw_u32(out_, directory);
    zw_u16(out_, 0);
    written_.clear();
    Bytes out = std::move(out_);
    out_.clear();
    return out;
}

}  // namespace cb::formats
