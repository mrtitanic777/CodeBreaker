#include "formats/cpc.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/probe.hpp"

namespace cb::formats::cpc {

namespace {

constexpr std::string_view kReader = "cpc";
constexpr std::size_t kHeader = 128;
constexpr std::size_t kSnaMemory = 0x100;
constexpr std::size_t kDirectoryEntries = 64;

bool starts_with(ByteView d, const char* text) { return d.size() >= std::strlen(text) && std::memcmp(d.data(), text, std::strlen(text)) == 0; }

std::string trimmed(ByteView d) {
    std::string s;
    for (const std::uint8_t b : d) s.push_back(static_cast<char>(b & 0x7F));
    while (!s.empty() && (s.back() == ' ' || s.back() == 0)) s.pop_back();
    return s;
}

// ---- disk images ----------------------------------------------------------------------------------------------------------------

struct Sector {
    std::uint8_t id = 0;
    ByteView data;
};

// The sectors of each track (side 0), in the order they are on the track.
std::vector<std::vector<Sector>> read_tracks(ByteView d) {
    const bool extended = starts_with(d, "EXTENDED");
    if (!extended && !starts_with(d, "MV - CPC")) throw FormatError(kReader, 0, "not a CPC disk image");
    if (d.size() < 0x100) throw TruncatedError(kReader, 0, 0x100, d.size());
    const std::size_t tracks = d[0x30], sides = d[0x31];
    if (tracks == 0 || sides == 0) throw FormatError(kReader, 0x30, "a disk with no tracks");
    std::vector<std::vector<Sector>> out(tracks);
    std::size_t offset = 0x100;
    for (std::size_t t = 0; t < tracks; ++t) {
        for (std::size_t side = 0; side < sides; ++side) {
            const std::size_t size = extended ? std::size_t{d[0x34 + t * sides + side]} * 256 : (std::size_t{d[0x32]} | std::size_t{d[0x33]} << 8);
            const std::size_t at = offset;
            offset += size;
            if (side != 0 || size == 0) continue;
            if (at + 0x100 > d.size() || !starts_with(d.subspan(at), "Track-Info")) throw FormatError(kReader, at, "no track information");
            const std::size_t count = d[at + 0x15];
            std::size_t data_at = at + 0x100;
            for (std::size_t s = 0; s < count; ++s) {
                const std::size_t info = at + 0x18 + s * 8;
                const std::size_t length = extended ? (std::size_t{d[info + 6]} | std::size_t{d[info + 7]} << 8) : (std::size_t{128} << (d[info + 3] & 7));
                if (data_at + length > d.size()) throw TruncatedError(kReader, data_at, length, d.size() - std::min(d.size(), data_at));
                out[t].push_back({d[info + 2], d.subspan(data_at, length)});
                data_at += length;
            }
        }
    }
    return out;
}

std::optional<AmsdosHeader> header_of(ByteView d) {
    if (d.size() < kHeader) return std::nullopt;
    unsigned sum = 0;
    for (std::size_t i = 0; i < 67; ++i) sum += d[i];
    if ((sum & 0xFFFF) != static_cast<unsigned>(d[67] | d[68] << 8)) return std::nullopt;
    AmsdosHeader h;
    h.name = trimmed(d.subspan(1, 8));
    const std::string ext = trimmed(d.subspan(9, 3));
    if (!ext.empty()) h.name += "." + ext;
    h.type = d[18];
    h.load = static_cast<std::uint16_t>(d[21] | d[22] << 8);
    h.length = static_cast<std::uint16_t>(d[24] | d[25] << 8);
    h.exec = static_cast<std::uint16_t>(d[26] | d[27] << 8);
    h.logical_length = static_cast<std::uint32_t>(d[64] | d[65] << 8 | d[66] << 16);
    return h;
}

bool is_binary(const AmsdosHeader& h) { return (h.type == 2 || h.type == 3) && h.length != 0; }

Program from_binary(const AmsdosHeader& h, ByteView file) {
    Program p;
    p.kind = Kind::Binary;
    p.name = h.name;
    p.memory.assign(kMemorySize, 0);
    p.origin = h.load;
    const ByteView body = file.subspan(kHeader);
    const std::size_t room = std::min<std::size_t>(kMemorySize - h.load, std::min<std::size_t>(h.length, body.size()));
    p.image.assign(body.begin(), body.begin() + static_cast<std::ptrdiff_t>(room));
    const std::uint32_t end = p.origin + static_cast<std::uint32_t>(p.image.size());
    const std::uint16_t start = h.exec >= p.origin && h.exec < end ? h.exec : static_cast<std::uint16_t>(p.origin);
    p.entries.push_back({start, "start"});
    return p;
}

bool runs_inside(const AmsdosHeader& x) { return x.exec >= x.load && static_cast<std::uint32_t>(x.exec) < static_cast<std::uint32_t>(x.load) + x.length; }

}  // namespace

std::optional<AmsdosHeader> amsdos_header(ByteView data) { return header_of(data); }

std::vector<DiskFile> disk_files(ByteView data) {
    const std::vector<std::vector<Sector>> tracks = read_tracks(data);
    if (tracks[0].empty()) throw FormatError(kReader, 0, "an unformatted first track");
    std::uint8_t first = 0xFF;
    for (const Sector& s : tracks[0]) first = std::min(first, s.id);
    std::size_t reserved = 1;                                  // IBM format: sectors $01-$08
    if ((first & 0xC0) == 0xC0) reserved = 0;                  // data format: $C1-$C9
    else if ((first & 0xC0) == 0x40) reserved = 2;             // system format: $41-$49
    // Logical sector n: track reserved + n / (sectors per track), the sector whose id is the track's lowest + n % (sectors per track).
    const auto logical = [&](std::size_t n) -> ByteView {
        for (std::size_t t = reserved; t < tracks.size(); ++t) {
            const std::size_t spt = tracks[t].size();
            if (spt == 0) throw FormatError(kReader, 0, "an unformatted track");
            if (n >= spt) {
                n -= spt;
                continue;
            }
            std::uint8_t lowest = 0xFF;
            for (const Sector& s : tracks[t]) lowest = std::min(lowest, s.id);
            for (const Sector& s : tracks[t])
                if (s.id == lowest + n) return s.data;
            throw FormatError(kReader, 0, "a sector missing from its track");
        }
        throw FormatError(kReader, 0, "a block past the last track");
    };
    const auto block = [&](std::size_t b) {
        Bytes out;
        for (std::size_t s = 0; s < 2; ++s) {
            const ByteView v = logical(b * 2 + s);
            out.insert(out.end(), v.begin(), v.end());
        }
        return out;
    };
    Bytes directory;
    for (std::size_t b = 0; b < 2; ++b) {
        const Bytes part = block(b);
        directory.insert(directory.end(), part.begin(), part.end());
    }
    struct Extent {
        unsigned number;
        unsigned records;
        std::vector<std::uint8_t> blocks;
    };
    std::map<std::string, std::vector<Extent>> by_name;
    std::vector<std::string> order;
    for (std::size_t e = 0; e < kDirectoryEntries && (e + 1) * 32 <= directory.size(); ++e) {
        const ByteView entry = ByteView(directory).subspan(e * 32, 32);
        if (entry[0] != 0) continue;   // user 0 only ($E5 is deleted)
        std::string name = trimmed(entry.subspan(1, 8));
        const std::string ext = trimmed(entry.subspan(9, 3));
        if (!ext.empty()) name += "." + ext;
        if (!by_name.contains(name)) order.push_back(name);
        Extent x{static_cast<unsigned>(entry[12] & 0x1F) | static_cast<unsigned>(entry[14]) << 5, entry[15], {}};
        for (std::size_t i = 16; i < 32; ++i)
            if (entry[i] != 0) x.blocks.push_back(entry[i]);
        by_name[name].push_back(std::move(x));
    }
    std::vector<DiskFile> files;
    for (const std::string& name : order) {
        std::vector<Extent>& extents = by_name[name];
        std::sort(extents.begin(), extents.end(), [](const Extent& a, const Extent& b) { return a.number < b.number; });
        DiskFile f;
        f.name = name;
        for (const Extent& x : extents) {
            Bytes raw;
            for (const std::uint8_t b : x.blocks) {
                const Bytes part = block(b);
                raw.insert(raw.end(), part.begin(), part.end());
            }
            raw.resize(std::min<std::size_t>(raw.size(), std::size_t{x.records} * 128));
            f.data.insert(f.data.end(), raw.begin(), raw.end());
        }
        f.header = header_of(f.data);
        files.push_back(std::move(f));
    }
    return files;
}

Program parse(ByteView data, std::string_view extension) {
    if (text::iequals(extension, ".sna")) {
        if (!starts_with(data, "MV - SNA")) throw FormatError(kReader, 0, "not a CPC snapshot");
        if (data.size() >= 0x6D && (data[0x6B] | data[0x6C] << 8) == 0) throw UnsupportedError(kReader, 0x6B, "a chunked (version 3) snapshot, not read yet");
        if (data.size() < kSnaMemory + kMemorySize) throw TruncatedError(kReader, 0, kSnaMemory + kMemorySize, data.size());
        Program p;
        p.kind = Kind::Snapshot;
        Registers r;
        r.f = data[0x11], r.a = data[0x12], r.c = data[0x13], r.b = data[0x14], r.e = data[0x15], r.d = data[0x16], r.l = data[0x17], r.h = data[0x18];
        r.r = data[0x19], r.i = data[0x1A];
        r.ix = static_cast<std::uint16_t>(data[0x1D] | data[0x1E] << 8);
        r.iy = static_cast<std::uint16_t>(data[0x1F] | data[0x20] << 8);
        r.sp = static_cast<std::uint16_t>(data[0x21] | data[0x22] << 8);
        r.pc = static_cast<std::uint16_t>(data[0x23] | data[0x24] << 8);
        r.im = data[0x25];
        p.registers = r;
        p.memory.assign(data.begin() + kSnaMemory, data.begin() + kSnaMemory + kMemorySize);
        p.origin = kSnapshotStart;
        p.image.assign(p.memory.begin() + kSnapshotStart, p.memory.begin() + kSnapshotEnd);
        if (r.pc >= kSnapshotStart && r.pc < kSnapshotEnd) p.entries.push_back({r.pc, "start"});
        return p;
    }
    if (text::iequals(extension, ".dsk")) {
        const std::vector<DiskFile> files = disk_files(data);
        const DiskFile* best = nullptr;
        std::vector<std::string> listing;
        for (const DiskFile& f : files) {
            if (f.header) listing.push_back(std::format("{} type {} load ${:04X} length ${:04X} exec ${:04X}", f.name, f.header->type, f.header->load, f.header->length, f.header->exec));
            else listing.push_back(f.name + " (no header)");
            if (!f.header || !is_binary(*f.header)) continue;
            const AmsdosHeader& h = *f.header;
            if (!best || (runs_inside(h) && !runs_inside(*best->header)) || (runs_inside(h) == runs_inside(*best->header) && h.length > best->header->length)) best = &f;
        }
        if (!best) throw FormatError(kReader, 0, "no AMSDOS binary on the disk");
        Program p = from_binary(*best->header, best->data);
        p.kind = Kind::Disk;
        p.files = std::move(listing);
        return p;
    }
    const std::optional<AmsdosHeader> h = header_of(data);
    if (!h || !is_binary(*h)) throw FormatError(kReader, 0, "no AMSDOS binary header");
    return from_binary(*h, data);
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path));
}

bool probe(const std::filesystem::path& path) {
    const std::string ext = extension_of(path);
    if (ext != ".dsk" && ext != ".sna" && ext != ".bin") return false;
    if (ext == ".sna") {
        const std::optional<Bytes> head = read_prefix(path, 0, 8);
        return head && starts_with(*head, "MV - SNA");
    }
    if (ext == ".bin") {
        const std::optional<Bytes> head = read_prefix(path, 0, kHeader);
        if (!head) return false;
        const std::optional<AmsdosHeader> h = header_of(*head);
        return h && is_binary(*h);
    }
    const std::optional<Bytes> head = read_prefix(path, 0, 8);
    if (!head || !(starts_with(*head, "MV - CPC") || starts_with(*head, "EXTENDED"))) return false;
    try {
        const fs::MappedFile file(path);
        (void)parse(file.view(), ".dsk");
        return true;
    } catch (const Error&) {   // isolation boundary: a disk with no binary, or a damaged one, is not a program
        return false;
    }
}

const std::map<std::uint32_t, std::string>& firmware_names() {
    // config/knowledge/cpc/registers.json
    static const std::map<std::uint32_t, std::string> kNames = knowledge::standard().table_names("cpc", "firmware");
    return kNames;
}

}  // namespace cb::formats::cpc
