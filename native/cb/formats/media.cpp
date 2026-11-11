#include "cb/formats/media.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <ctime>
#include <format>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::media {

namespace {

constexpr const char* kReader = "media";
constexpr std::size_t kIsoSector = 2048;

// ---- small readers over a sector ----------------------------------------------------------------------------------------

std::uint8_t u8_at(ByteView b, std::size_t at) {
    if (at >= b.size()) throw TruncatedError(kReader, at, 1, b.size() > at ? b.size() - at : 0);
    return b[at];
}
std::uint16_t u16_at(ByteView b, std::size_t at) {
    io::ByteReader r(b, kReader);
    r.seek(at);
    return r.u16();
}
std::uint32_t u32_at(ByteView b, std::size_t at) {
    io::ByteReader r(b, kReader);
    r.seek(at);
    return r.u32();
}
// [from, to) clamped to the data (a name field that runs off the end is shorter).
ByteView slice(ByteView b, std::size_t from, std::size_t to) {
    from = std::min(from, b.size());
    to = std::min(std::max(to, from), b.size());
    return b.subspan(from, to - from);
}
bool bytes_are(ByteView b, std::size_t from, std::string_view what) {
    return b.size() >= from + what.size() && std::equal(what.begin(), what.end(), b.begin() + static_cast<std::ptrdiff_t>(from),
                                                        [](char c, std::uint8_t x) { return static_cast<std::uint8_t>(c) == x; });
}

// ---- text -----------------------------------------------------------------------------------------------------------------

std::string rstrip_blank(std::string s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    return s;
}
// A name field of a volume: CP437, trailing blanks and NULs cut.
std::string name_text(ByteView raw) { return rstrip_blank(text::from_cp437(raw)); }

std::string utf16be_text(ByteView raw) {
    Bytes swapped(raw.begin(), raw.end());
    for (std::size_t i = 0; i + 1 < swapped.size(); i += 2) std::swap(swapped[i], swapped[i + 1]);
    return text::from_utf16le(swapped);
}

std::string when_text(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute) {
    return std::format("{:04}-{:02}-{:02} {:02}:{:02}", year, month, day, hour, minute);
}

BootField number_field(std::string name, std::int64_t value) { return {std::move(name), std::to_string(value), value}; }
BootField text_field(std::string name, std::string value) { return {std::move(name), std::move(value), std::nullopt}; }

// ---- FAT directory entries ------------------------------------------------------------------------------------------------

constexpr std::uint8_t kAttrVolumeLabel = 0x08, kAttrDirectory = 0x10, kAttrLongName = 0x0F;

// The entries of one folder's bytes. `prefix` / `short_prefix` are the folder's path in long and 8.3 form. A volume-label
// entry sets `label` if it is still empty.
std::vector<Entry> fat_folder(ByteView raw, const std::string& prefix, const std::string& short_prefix, int bits,
                              std::string& label) {
    std::vector<Bytes> long_parts;   // the last piece first
    std::vector<Entry> out;
    for (std::size_t i = 0; i + 32 <= raw.size(); i += 32) {
        const ByteView e = raw.subspan(i, 32);
        if (e[0] == 0) break;
        const std::uint8_t attr = e[11];
        if (e[0] == 0xE5) {   // a deleted entry (and its long-name pieces)
            long_parts.clear();
            continue;
        }
        if ((attr & 0x3F) == kAttrLongName) {   // a piece of a long name: 5 + 6 + 2 UTF-16 units
            if (e[0] & 0x40) long_parts.clear();   // the last piece comes first: a new name begins
            Bytes piece;
            for (ByteView part : {e.subspan(1, 10), e.subspan(14, 12), e.subspan(28, 4)}) piece.insert(piece.end(), part.begin(), part.end());
            long_parts.insert(long_parts.begin(), std::move(piece));
            continue;
        }
        Bytes first_and_base(e.begin(), e.begin() + 8);
        if (first_and_base[0] == 5) first_and_base[0] = 0xE5;   // a name that really starts with 0xE5 is stored as 5
        const std::string base = name_text(first_and_base), ext = name_text(e.subspan(8, 3));
        const std::string short_name = base + (ext.empty() ? "" : "." + ext);
        std::string name = short_name;
        if (!long_parts.empty()) {
            Bytes joined;
            for (const Bytes& p : long_parts) joined.insert(joined.end(), p.begin(), p.end());
            name = text::from_utf16le(joined);
            if (const std::size_t z = name.find('\0'); z != std::string::npos) name.resize(z);
        }
        long_parts.clear();
        if (attr & kAttrVolumeLabel) {
            if (label.empty()) label = name_text(e.subspan(0, 11));
            continue;
        }
        if (short_name == "." || short_name == "..") continue;
        const std::uint32_t time = u16_at(e, 22), date = u16_at(e, 24);
        Entry x;
        x.path = prefix + name;
        x.size = u32_at(e, 28);
        x.is_dir = (attr & kAttrDirectory) != 0;
        x.start = u16_at(e, 26) | (bits == 32 ? static_cast<std::uint64_t>(u16_at(e, 20)) << 16 : 0);
        x.when = when_text(1980 + (date >> 9), (date >> 5) & 15, date & 31, time >> 11, (time >> 5) & 63);
        x.short_name = short_prefix + short_name;
        x.attributes = attr;
        out.push_back(std::move(x));
    }
    return out;
}

// The entry of the FAT chain: the next cluster, read from a FAT of `bits`; a cluster outside the table ends the chain.
std::uint32_t fat_next(ByteView fat, int bits, std::uint64_t c) {
    if (bits == 12) {
        const std::uint64_t at = c * 3 / 2;
        if (at + 2 > fat.size()) return 0xFFF;
        const std::uint32_t v = fat[at] | (static_cast<std::uint32_t>(fat[at + 1]) << 8);
        return (c & 1) ? v >> 4 : v & 0xFFF;
    }
    if (bits == 16) return c * 2 + 2 <= fat.size() ? u16_at(fat, c * 2) : 0xFFFF;
    return c * 4 + 4 <= fat.size() ? (u32_at(fat, c * 4) & 0x0FFFFFFF) : 0x0FFFFFFF;
}

// ---- the old floppies ---------------------------------------------------------------------------------------------------

// A floppy from before DOS 2.0 has no parameter block: its layout follows from the media byte (the first byte of the FAT).
struct OldFloppy {
    std::uint8_t media;
    std::uint32_t sectors_per_cluster, root_entries, sectors_per_fat, total_sectors;
};
constexpr OldFloppy kOldFloppies[] = {
    {0xFE, 1, 64, 1, 320}, {0xFF, 2, 112, 1, 640}, {0xFC, 1, 64, 2, 360}, {0xFD, 2, 112, 2, 720}};
constexpr std::uint64_t kOldFloppyLimit = 512 * 720;   // no old floppy is bigger than a 360 KB disk

}  // namespace

// ---- Source ---------------------------------------------------------------------------------------------------------------

Source::Source(const std::filesystem::path& file, std::uint64_t offset)
    : file_(std::make_shared<fs::RandomFile>(file, fs::FileMode::Read)), offset_(offset), file_size_(file_->size()) {}

Source Source::shifted(std::uint64_t more) const {
    Source s = *this;
    s.offset_ += more;
    return s;
}

Bytes Source::read(std::uint64_t at, std::uint64_t count) const {
    if (count == 0 || at > file_size_ || offset_ > file_size_ - at) return {};
    const std::uint64_t pos = offset_ + at;
    return file_->read_at(pos, static_cast<std::size_t>(std::min(count, file_size_ - pos)));
}

std::string_view kind_name(VolumeKind kind) {
    switch (kind) {
    case VolumeKind::Fat12: return "fat12";
    case VolumeKind::Fat16: return "fat16";
    case VolumeKind::Fat32: return "fat32";
    case VolumeKind::Iso9660: return "iso9660";
    case VolumeKind::Bundle: return "bundle";
    }
    return "?";
}

// ---- FatVolume ---------------------------------------------------------------------------------------------------------------

std::optional<FatVolume> FatVolume::try_open(Source source, std::string where, std::optional<std::uint64_t> size) {
    const Bytes b = source.read(0, 512);
    if (b.size() < 512) return std::nullopt;
    std::uint64_t bps = u16_at(b, 11), spc = u8_at(b, 13), reserved = u16_at(b, 14), fats = u8_at(b, 16), roots = u16_at(b, 17),
                  total16 = u16_at(b, 19), media = u8_at(b, 21), spf16 = u16_at(b, 22), spt = u16_at(b, 24),
                  heads = u16_at(b, 26), hidden = u32_at(b, 28), total32 = u32_at(b, 32);
    const bool has_parameter_block =
        (bps == 512 || bps == 1024 || bps == 2048 || bps == 4096) && spc != 0 && (spc & (spc - 1)) == 0 && fats >= 1 && fats <= 2 &&
        reserved != 0 && media >= 0xF0 && (total16 != 0 || total32 != 0) && (b[0] == 0xEB || b[0] == 0xE9 || b[0] == 0x69);
    bool old = false;
    if (!has_parameter_block) {
        const Bytes fat0 = source.read(512, 3);
        const OldFloppy* hit = nullptr;
        if (fat0.size() == 3)
            for (const OldFloppy& k : kOldFloppies)
                if (k.media == fat0[0]) hit = &k;
        if (hit == nullptr || fat0[1] != 0xFF || fat0[2] != 0xFF || (size && *size > kOldFloppyLimit)) return std::nullopt;
        spc = hit->sectors_per_cluster;
        roots = hit->root_entries;
        spf16 = hit->sectors_per_fat;
        total16 = hit->total_sectors;
        bps = 512, reserved = 1, fats = 2, media = fat0[0], spt = 8, heads = 0, hidden = 0, total32 = 0;
        old = true;
    }
    const std::uint64_t spf = spf16 != 0 ? spf16 : u32_at(b, 36);
    const std::uint64_t total = total16 != 0 ? total16 : total32;

    FatVolume v(std::move(source));
    v.where_ = std::move(where);
    FatGeometry& g = v.geometry_;
    g.old = old;
    g.bytes_per_sector = static_cast<std::uint32_t>(bps);
    g.sectors_per_cluster = static_cast<std::uint32_t>(spc);
    g.root_entries = static_cast<std::uint32_t>(roots);
    g.fats = static_cast<std::uint32_t>(fats);
    g.sectors_per_fat = static_cast<std::uint32_t>(spf);
    g.fat_at = reserved * bps;
    g.root_at = g.fat_at + fats * spf * bps;
    g.data_at = g.root_at + ((roots * 32 + bps - 1) / bps) * bps;
    if (total * bps <= g.data_at) return std::nullopt;   // the volume ends before its data begins: not a volume
    g.clusters = static_cast<std::uint32_t>((total * bps - g.data_at) / (spc * bps));
    g.bits = g.clusters < 4085 ? 12 : g.clusters < 65525 ? 16 : 32;
    g.root_cluster = g.bits == 32 ? u32_at(b, 44) : 0;

    const std::size_t ext = g.bits == 32 ? 64 : 36;   // the extended boot record
    const bool has_ext = !old && (b[ext + 2] == 0x28 || b[ext + 2] == 0x29);
    std::string jump;
    if (!old && b[0] == 0xEB) jump = std::format("jmp {:03X}h", 2 + b[1]);
    else if (!old && b[0] == 0xE9) jump = std::format("jmp {:03X}h", (3 + u16_at(b, 1)) & 0xFFFF);

    v.label_ = has_ext ? name_text(slice(b, ext + 7, ext + 18)) : "";
    v.boot_ = {text_field("system", old ? "(no parameter block)" : name_text(slice(b, 3, 11))),
               number_field("bytes per sector", static_cast<std::int64_t>(bps)),
               number_field("sectors per cluster", static_cast<std::int64_t>(spc)),
               number_field("reserved sectors", static_cast<std::int64_t>(reserved)),
               number_field("fats", static_cast<std::int64_t>(fats)),
               number_field("root entries", static_cast<std::int64_t>(roots)),
               number_field("total sectors", static_cast<std::int64_t>(total)),
               text_field("media", std::format("{:02X}", media)),
               number_field("sectors per fat", static_cast<std::int64_t>(spf)),
               number_field("sectors per track", static_cast<std::int64_t>(spt)),
               number_field("heads", static_cast<std::int64_t>(heads)),
               number_field("hidden sectors", static_cast<std::int64_t>(hidden)),
               number_field("clusters", g.clusters),
               text_field("serial", has_ext ? std::format("{:08X}", u32_at(b, ext + 3)) : ""),
               text_field("label", v.label_),
               text_field("file system", has_ext && b[ext + 2] == 0x29 ? name_text(slice(b, ext + 18, ext + 26)) : ""),
               text_field("signature", std::format("{:02X}{:02X}", b[510], b[511])),
               text_field("boot code", jump)};
    v.fat_ = v.source_.read(g.fat_at, spf * bps);
    return v;
}

FatVolume FatVolume::open(Source source, std::string where, std::optional<std::uint64_t> size) {
    std::optional<FatVolume> v = try_open(std::move(source), std::move(where), size);
    if (!v) throw FormatError(kReader, 0, "not a FAT volume");
    return std::move(*v);
}

VolumeKind FatVolume::kind() const {
    return geometry_.bits == 12 ? VolumeKind::Fat12 : geometry_.bits == 16 ? VolumeKind::Fat16 : VolumeKind::Fat32;
}

std::uint32_t FatVolume::next_cluster(std::uint32_t cluster) const { return fat_next(fat_, geometry_.bits, cluster); }

Bytes FatVolume::read_chain(std::uint32_t cluster, std::optional<std::uint64_t> size) const {
    const std::uint64_t last = (geometry_.bits < 32 ? (std::uint64_t(1) << geometry_.bits) : (std::uint64_t(1) << 28)) - 16;
    const std::uint64_t step = static_cast<std::uint64_t>(geometry_.sectors_per_cluster) * geometry_.bytes_per_sector;
    std::unordered_set<std::uint32_t> seen;   // a chain that loops on itself ends where it comes back
    Bytes data;
    std::uint64_t got = 0;
    while (cluster >= 2 && cluster < last && !seen.contains(cluster) && (!size || got < *size)) {
        seen.insert(cluster);
        append(data, source_.read(geometry_.data_at + static_cast<std::uint64_t>(cluster - 2) * step, step));
        got += step;
        cluster = next_cluster(cluster);
    }
    if (size && data.size() > *size) data.resize(static_cast<std::size_t>(*size));
    return data;
}

const std::vector<Entry>& FatVolume::files() {
    if (entries_) return *entries_;
    const FatGeometry& g = geometry_;
    constexpr std::size_t kMaxFolders = 20000;   // a folder cycle (or a corrupt table) must end
    const Bytes root = g.bits == 32 ? read_chain(g.root_cluster) : source_.read(g.root_at, static_cast<std::uint64_t>(g.root_entries) * 32);
    // Depth first: each folder is followed by what is in it. A folder met a second time (the same start) is not entered again.
    std::vector<Entry> out;
    std::vector<Entry> pending = fat_folder(root, "", "", g.bits, label_);
    std::ranges::reverse(pending);   // popped from the back
    std::unordered_set<std::uint64_t> entered;
    while (!pending.empty()) {
        Entry e = std::move(pending.back());
        pending.pop_back();
        if (e.is_dir && !entered.contains(e.start) && entered.size() < kMaxFolders) {
            entered.insert(e.start);
            const Bytes sub = read_chain(static_cast<std::uint32_t>(e.start));
            std::vector<Entry> kids = fat_folder(sub, e.path + "/", e.short_name + "/", g.bits, label_);
            out.push_back(std::move(e));
            for (auto it = kids.rbegin(); it != kids.rend(); ++it) pending.push_back(std::move(*it));
            continue;
        }
        out.push_back(std::move(e));
    }
    entries_ = std::move(out);
    return *entries_;
}

Bytes FatVolume::read(const Entry& entry) {
    return entry.is_dir ? Bytes() : read_chain(static_cast<std::uint32_t>(entry.start), entry.size);
}

// ---- ISO 9660 ------------------------------------------------------------------------------------------------------------------

namespace {

// Sector `n` of an ISO image in the image file's own layout (raw sectors carry `skip` bytes of header before the 2048).
Bytes iso_sectors(const Source& src, const IsoLayout& l, std::uint64_t n, std::uint64_t count) {
    if (l.sector == kIsoSector) return src.read(n * kIsoSector, count * kIsoSector);
    Bytes out;
    for (std::uint64_t i = 0; i < count; ++i) append(out, src.read((n + i) * l.sector + l.skip, kIsoSector));
    return out;
}

Bytes iso_extent(const Source& src, const IsoLayout& l, std::uint64_t block, std::uint64_t size) {
    Bytes data = iso_sectors(src, l, block, (size + kIsoSector - 1) / kIsoSector);
    if (data.size() > size) data.resize(static_cast<std::size_t>(size));
    return data;
}

}  // namespace

IsoVolume IsoVolume::open(Source source, std::uint32_t sector, std::uint32_t skip, bool long_names) {
    IsoVolume v(std::move(source));
    IsoLayout& l = v.layout_;
    l.sector = sector;
    l.skip = skip;
    Bytes primary, joliet;
    for (std::uint64_t n = 16; n < 48; ++n) {   // the volume descriptors, up to the terminator
        Bytes d = iso_sectors(v.source_, l, n, 1);
        if (d.size() < kIsoSector || !bytes_are(d, 1, "CD001") || d[0] == 255) break;
        if (d[0] == 1 && primary.empty()) {
            primary = std::move(d);
        } else if (d[0] == 2 && (bytes_are(d, 88, "%/@") || bytes_are(d, 88, "%/C") || bytes_are(d, 88, "%/E"))) {   // Joliet levels 1-3
            joliet = std::move(d);
        } else if (d[0] == 0 && bytes_are(d, 7, "EL TORITO SPECIFICATION")) {
            l.boot_catalog = u32_at(d, 71);
        }
    }
    if (primary.empty()) throw FormatError(kReader, 16 * kIsoSector, "not an ISO 9660 volume");
    l.has_joliet = !joliet.empty();
    l.long_names = l.has_joliet && long_names;
    const Bytes& d = l.long_names ? joliet : primary;
    l.block = u16_at(primary, 128) != 0 ? u16_at(primary, 128) : 2048;
    const ByteView label_field = slice(d, 40, 72);
    v.label_ = std::string(text::trim(l.long_names ? utf16be_text(label_field) : name_text(label_field)));
    l.root_block = u32_at(d, 158);
    l.root_size = u32_at(d, 166);
    v.boot_ = {text_field("system", name_text(slice(primary, 8, 40))),
               text_field("label", name_text(slice(primary, 40, 72))),
               number_field("blocks", u32_at(primary, 80)),
               number_field("block size", l.block),
               text_field("created", name_text(slice(primary, 813, 829))),
               text_field("long names", l.has_joliet ? "yes" : "no"),
               text_field("bootable", l.boot_catalog && *l.boot_catalog != 0 ? "yes" : "no")};
    return v;
}

std::optional<BootImage> IsoVolume::boot_image() const {
    if (!layout_.boot_catalog || *layout_.boot_catalog == 0) return std::nullopt;
    const Bytes c = iso_sectors(source_, layout_, *layout_.boot_catalog, 1);
    // a validation entry (header id 1, key bytes 55 AA) then the default entry (bootable, 0x88)
    if (c.size() < 64 || c[0] != 1 || c[30] != 0x55 || c[31] != 0xAA || c[32] != 0x88) return std::nullopt;
    BootImage b;
    b.media = c[33] & 15;
    const std::uint32_t count = u16_at(c, 38), block = u32_at(c, 40);
    b.size = b.media == 1 ? 1228800 : b.media == 2 ? 1474560 : b.media == 3 ? 2949120 : static_cast<std::uint64_t>(count) * 512;
    b.offset = static_cast<std::uint64_t>(block) * layout_.sector + layout_.skip;
    return b;
}

const std::vector<Entry>& IsoVolume::files() {
    if (entries_) return *entries_;
    constexpr std::size_t kMaxFolders = 50000;   // a directory cycle (or a corrupt tree) must end
    struct Folder {
        std::uint64_t block, size;
        std::string prefix;
    };
    std::vector<Folder> work{{layout_.root_block, layout_.root_size, ""}};
    std::vector<Entry> found_all;
    std::unordered_set<std::uint64_t> seen;
    for (std::size_t head = 0; head < work.size(); ++head) {
        const Folder w = work[head];
        if (seen.contains(w.block) || seen.size() > kMaxFolders) continue;
        seen.insert(w.block);
        const Bytes d = iso_extent(source_, layout_, w.block, w.size);
        std::vector<Entry> found;
        std::size_t i = 0;
        while (i < d.size()) {
            const std::size_t n = d[i];
            if (n == 0) {   // records do not cross a sector: the rest of this one is padding
                i = (i / kIsoSector + 1) * kIsoSector;
                continue;
            }
            const ByteView r = slice(d, i, i + n);
            i += n;
            if (r.size() < 34) continue;
            const ByteView raw = slice(r, 33, 33 + r[32]);
            if (raw.size() == 1 && raw[0] <= 1) continue;   // "." and ".."
            std::string name = layout_.long_names ? utf16be_text(raw) : text::from_latin1(raw);
            if (const std::size_t semi = name.find(';'); semi != std::string::npos) name.resize(semi);   // ";1": the version
            if (!name.empty() && name.back() == '.') name.pop_back();
            Entry e;
            e.path = w.prefix + name;
            e.size = u32_at(r, 10);
            e.is_dir = (r[25] & 2) != 0;
            e.start = static_cast<std::uint64_t>(u32_at(r, 2)) + r[1];   // the block, past the extended attribute record
            e.when = when_text(1900u + r[18], r[19], r[20], r[21], r[22]);
            e.short_name = e.path;
            if (!found.empty() && found.back().path == e.path && !e.is_dir) {   // a further extent of the same file
                found.back().extents.push_back({e.start, e.size});
                found.back().size += e.size;
                continue;
            }
            e.extents.push_back({e.start, e.size});
            found.push_back(std::move(e));
        }
        for (Entry& e : found) {
            if (e.is_dir) work.push_back({e.start, e.size, e.path + "/"});
            found_all.push_back(std::move(e));
        }
    }
    // Folders before what is in them: each folder's children follow it, in the order its own records gave.
    std::unordered_map<std::string, std::vector<std::size_t>> children;   // parent path -> entries; looked up only
    for (std::size_t k = 0; k < found_all.size(); ++k) {
        const std::string& p = found_all[k].path;
        const std::size_t slash = p.rfind('/');
        children[slash == std::string::npos ? std::string() : p.substr(0, slash)].push_back(k);
    }
    std::vector<Entry> out;
    struct Level {
        const std::vector<std::size_t>* list;
        std::size_t next;
    };
    std::vector<Level> stack;
    if (auto root = children.find(""); root != children.end()) stack.push_back({&root->second, 0});
    while (!stack.empty()) {
        Level& top = stack.back();
        if (top.next >= top.list->size()) {
            stack.pop_back();
            continue;
        }
        const Entry& e = found_all[(*top.list)[top.next++]];
        out.push_back(e);
        if (e.is_dir)
            if (auto kids = children.find(e.path); kids != children.end()) stack.push_back({&kids->second, 0});   // (`top` may dangle)
    }
    entries_ = std::move(out);
    return *entries_;
}

Bytes IsoVolume::read(const Entry& entry) {
    Bytes out;
    if (entry.is_dir) return out;
    for (const Extent& x : entry.extents) append(out, iso_extent(source_, layout_, x.block, x.size));
    return out;
}

// ---- BundleVolume -------------------------------------------------------------------------------------------------------------

std::optional<BundleVolume> BundleVolume::try_open(const std::filesystem::path& file) {
    constexpr std::uint64_t kMaxBundleBytes = 64ull << 20;   // the boot images are a few hundred KB; a big file is not one
    std::error_code size_error;
    const std::uint64_t size = std::filesystem::file_size(file, size_error);
    if (!size_error && size > kMaxBundleBytes) return std::nullopt;
    BundleVolume v;
    try {
        v.modules_ = ne::open_boot_image_files(file);
    } catch (const FormatError&) {   // the boot-image reader reports "this is not a boot image" as a malformed file
        return std::nullopt;
    }
    if (v.modules_.size() < 2) return std::nullopt;
    v.label_ = file.filename().string();
    v.boot_ = {number_field("modules", static_cast<std::int64_t>(v.modules_.size()))};
    v.built_.resize(v.modules_.size());
    return v;
}

const Bytes& BundleVolume::built(std::size_t module_index) {
    std::optional<Bytes>& slot = built_.at(module_index);
    if (!slot) slot = modules_[module_index].second.standalone();
    return *slot;
}

const std::vector<Entry>& BundleVolume::files() {
    if (entries_) return *entries_;
    std::vector<Entry> out;
    for (std::size_t k = 0; k < modules_.size(); ++k) {
        Entry e;
        e.path = e.short_name = modules_[k].first + ".EXE";
        e.size = built(k).size();
        e.start = modules_[k].second.ne_offset();
        out.push_back(std::move(e));
    }
    entries_ = std::move(out);
    return *entries_;
}

Bytes BundleVolume::read(const Entry& entry) {
    const std::string name = entry.path.size() > 4 ? entry.path.substr(0, entry.path.size() - 4) : std::string();
    for (std::size_t k = 0; k < modules_.size(); ++k)
        if (modules_[k].first == name) return built(k);
    throw NotFoundError(std::filesystem::path(entry.path), 0, "no such module in the bundle");
}

// ---- partitions and the volumes of an image ---------------------------------------------------------------------------------

namespace {

struct Partition {
    std::uint64_t first_sector, sectors;
    std::uint8_t type;
};

bool is_fat_partition_type(std::uint8_t t) {
    static constexpr std::uint8_t kTypes[] = {0x01, 0x04, 0x06, 0x0B, 0x0C, 0x0E, 0x11, 0x14, 0x16, 0x1B, 0x1C, 0x1E};
    return std::ranges::find(kTypes, t) != std::end(kTypes);
}

// The FAT partitions of an MBR disk, logical drives of extended partitions included, that start inside the image.
std::vector<Partition> partitions(const Source& src, std::uint64_t image_size) {
    constexpr std::size_t kMaxTables = 64;   // a loop of extended tables (or a corrupt chain) must end
    std::vector<Partition> out;
    std::vector<std::uint64_t> tables{0};   // sectors holding a partition table, in the order found
    std::unordered_set<std::uint64_t> seen;
    std::optional<std::uint64_t> extended_base;
    for (std::size_t head = 0; head < tables.size(); ++head) {
        const std::uint64_t at = tables[head];
        if (seen.contains(at) || seen.size() > kMaxTables) continue;
        seen.insert(at);
        const Bytes b = src.read(at * 512, 512);
        if (b.size() < 512 || b[510] != 0x55 || b[511] != 0xAA) continue;
        for (std::size_t i = 0; i < 4; ++i) {
            const std::size_t entry = 446 + 16 * i;
            const std::uint8_t type = b[entry + 4];
            const std::uint64_t first = u32_at(b, entry + 8), count = u32_at(b, entry + 12);
            if (type == 0 || count == 0) continue;
            if (type == 0x05 || type == 0x0F || type == 0x85) {   // an extended partition: the first one sets the base
                if (!extended_base) {
                    extended_base = first;
                    tables.push_back(first);
                } else {
                    tables.push_back(*extended_base + first);
                }
            } else if (is_fat_partition_type(type) && (at + first) * 512 < image_size) {
                out.push_back({at + first, count, type});
            }
        }
    }
    return out;
}

}  // namespace

std::vector<std::unique_ptr<Volume>> volumes(const std::filesystem::path& image, bool long_names) {
    std::error_code ec;
    if (std::filesystem::is_directory(image, ec))
        throw FormatError(kReader, 0, std::format("{}: a folder, not an image file", image.filename().string()));
    const Source whole(image);
    std::vector<std::unique_ptr<Volume>> out;

    // A disc (checked first: an Xbox 360 disc and other discs have more than one tree).
    constexpr std::uint32_t kLayouts[3][2] = {{2048, 0}, {2352, 16}, {2352, 24}};   // plain, raw mode 1, raw mode 2
    for (const auto& [sector, skip] : kLayouts) {
        if (!bytes_are(whole.read(16ull * sector + skip + 1, 5), 0, "CD001")) continue;
        auto iso = std::make_unique<IsoVolume>(IsoVolume::open(whole, sector, skip, long_names));
        const std::optional<BootImage> boot = iso->boot_image();
        out.push_back(std::move(iso));
        // a bootable disc that emulates a floppy holds the floppy's FAT volume
        if (boot && boot->media >= 1 && boot->media <= 3 && sector == 2048)
            if (auto floppy = FatVolume::try_open(whole.shifted(boot->offset), "boot floppy inside the disc", boot->size))
                out.push_back(std::make_unique<FatVolume>(std::move(*floppy)));
        return out;
    }
    if (auto fat = FatVolume::try_open(whole, "", whole.file_size())) {
        out.push_back(std::make_unique<FatVolume>(std::move(*fat)));
        return out;
    }
    for (const Partition& p : partitions(whole, whole.file_size())) {
        const std::string where = std::format("partition at sector {}, type {:02X}", p.first_sector, p.type);
        if (auto fat = FatVolume::try_open(whole.shifted(p.first_sector * 512), where, p.sectors * 512))
            out.push_back(std::make_unique<FatVolume>(std::move(*fat)));
    }
    if (!out.empty()) return out;
    if (auto bundle = BundleVolume::try_open(image)) {
        out.push_back(std::make_unique<BundleVolume>(std::move(*bundle)));
        return out;
    }
    throw FormatError(kReader, 0,
                      std::format("{}: no FAT volume, partition table, ISO 9660 volume or module bundle found", image.filename().string()));
}

// ---- writing into a FAT volume ---------------------------------------------------------------------------------------------------

namespace {

using ShortName = std::array<std::uint8_t, 11>;   // an 8.3 name as a directory entry holds it ("vtd.vxd" -> "VTD     VXD")

std::vector<char32_t> code_points(std::string_view s) {
    std::vector<char32_t> out;
    for (std::size_t i = 0; i < s.size();) {
        const auto c = static_cast<std::uint8_t>(s[i]);
        const std::size_t len = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
        char32_t cp = len == 1 ? c : c & (0xFF >> (len + 1));
        for (std::size_t k = 1; k < len && i + k < s.size(); ++k) cp = (cp << 6) | (static_cast<std::uint8_t>(s[i + k]) & 0x3F);
        out.push_back(cp);
        i += len;
    }
    return out;
}

// "vtd.vxd" -> "VTD     VXD". Upper-cases ASCII and Latin-1 letters; a name that is not 8.3, has a character FAT forbids, or
// that CP437 cannot hold is a UsageError.
ShortName short_name_of(std::string_view name) {
    std::vector<char32_t> cps = code_points(name);
    for (char32_t& ch : cps) {
        if (ch >= 'a' && ch <= 'z') ch -= 0x20;
        else if (ch >= 0xE0 && ch <= 0xFE && ch != 0xF7) ch -= 0x20;
    }
    std::vector<char32_t> base, ext;
    bool after_dot = false;
    for (char32_t ch : cps) {
        if (!after_dot && ch == '.') {
            after_dot = true;
            continue;
        }
        (after_dot ? ext : base).push_back(ch);
    }
    bool bad = base.empty() || base.size() > 8 || ext.size() > 3;
    constexpr std::string_view kForbidden = " .\"/\\[]:;=,+*?<>|";
    for (const auto* part : {&base, &ext})
        for (char32_t ch : *part)
            if (ch < 33 || (ch < 128 && kForbidden.find(static_cast<char>(ch)) != std::string_view::npos)) bad = true;
    if (bad) throw UsageError(std::format("not an 8.3 name: '{}'", name));
    std::string padded;
    for (const auto* part : {&base, &ext}) {
        std::string utf8;
        for (char32_t ch : *part) text::append_utf8(utf8, ch);
        padded += utf8;
        const std::size_t width = part == &base ? 8 : 3;
        padded.append(width - part->size(), ' ');
    }
    const Bytes encoded = text::to_cp437(padded);
    if (text::from_cp437(encoded) != padded) throw UsageError(std::format("'{}' has a character the DOS code page cannot hold", name));
    ShortName out{};
    std::copy(encoded.begin(), encoded.end(), out.begin());
    return out;
}

void put16(Bytes& e, std::size_t at, std::uint32_t v) {
    e[at] = static_cast<std::uint8_t>(v);
    e[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

// The edits to one FAT volume: its table in memory, the clusters and directory entries written as they go, the table's
// copies written when done.
class FatWriter {
public:
    FatWriter(const std::filesystem::path& image, const FatVolume& volume)
        : g_(volume.geometry()),
          base_(volume.source().offset()),
          step_(static_cast<std::uint64_t>(g_.sectors_per_cluster) * g_.bytes_per_sector),
          end_(g_.bits == 12 ? 0xFFF : g_.bits == 16 ? 0xFFFF : 0x0FFFFFFF),
          fat_(volume.fat().begin(), volume.fat().end()),
          file_(image, fs::FileMode::ReadWrite) {}

    // Every copy of the FAT back into the image.
    void flush_fats() {
        for (std::uint32_t i = 0; i < g_.fats; ++i)
            file_.write_at(base_ + g_.fat_at + static_cast<std::uint64_t>(i) * g_.sectors_per_fat * g_.bytes_per_sector, fat_);
        file_.flush();
    }

    // The folder (0 for FAT12/16's fixed root) the path's folders lead to, making what is missing.
    std::uint32_t folder_of(const std::vector<std::string>& parts, std::uint16_t time, std::uint16_t date) {
        std::uint32_t folder = g_.bits == 32 ? g_.root_cluster : 0;
        for (std::size_t k = 0; k + 1 < parts.size(); ++k) {
            const ShortName sn = short_name_of(parts[k]);
            if (auto hit = find(folder, sn)) {
                if (!(hit->entry[11] & kAttrDirectory)) throw UsageError(std::format("{} is not a folder", parts[k]));
                folder = start_of(hit->entry);
            } else {
                folder = make_folder(folder, sn, time, date);
            }
        }
        return folder;
    }

    PutOutcome write_file(std::uint32_t folder, const ShortName& name, ByteView data, std::uint16_t time, std::uint16_t date) {
        PutOutcome outcome = PutOutcome::Added;
        std::uint64_t at = 0;
        Bytes e(32, 0);
        if (auto hit = find(folder, name)) {
            if (hit->entry[11] & kAttrDirectory) throw UsageError(std::format("{} is a folder", text::from_cp437(name)));
            for (std::uint32_t cl : chain_of(start_of(hit->entry))) set(cl, 0);   // free the old data
            free_from_ = 2;
            at = hit->at;
            e = std::move(hit->entry);
            outcome = PutOutcome::Replaced;
        } else {
            at = new_slot(folder);
            std::copy(name.begin(), name.end(), e.begin());
            e[11] = 0x20;   // archive
        }
        const std::uint64_t n = (data.size() + step_ - 1) / step_;
        const std::vector<std::uint32_t> chain = n != 0 ? allocate(n) : std::vector<std::uint32_t>();
        write_clusters(chain, data);
        set_start(e, chain.empty() ? 0 : chain[0]);
        put16(e, 22, time);
        put16(e, 24, date);
        const auto len = static_cast<std::uint32_t>(data.size());
        for (std::size_t k = 0; k < 4; ++k) e[28 + k] = static_cast<std::uint8_t>(len >> (8 * k));
        file_.write_at(at, e);
        return outcome;
    }

private:
    struct Slot {
        std::uint64_t at;   // where in the image file
        Bytes entry;        // the 32 bytes
    };
    struct Hit {
        std::uint64_t at;
        Bytes entry;
    };

    std::uint32_t get(std::uint32_t c) const { return fat_next(fat_, g_.bits, c); }

    std::uint8_t& fat_byte(std::uint64_t i) {
        if (i >= fat_.size()) throw FormatError(kReader, i, "a cluster outside the FAT");
        return fat_[static_cast<std::size_t>(i)];
    }
    void set(std::uint32_t c, std::uint32_t value) {
        if (g_.bits == 12) {
            const std::uint64_t at = static_cast<std::uint64_t>(c) * 3 / 2;
            const std::uint32_t old = fat_byte(at) | (static_cast<std::uint32_t>(fat_byte(at + 1)) << 8);
            const std::uint32_t nv = (c & 1) ? (old & 0x000F) | ((value & 0xFFF) << 4) : (old & 0xF000) | (value & 0xFFF);
            fat_byte(at) = static_cast<std::uint8_t>(nv & 0xFF);
            fat_byte(at + 1) = static_cast<std::uint8_t>(nv >> 8);
        } else if (g_.bits == 16) {
            const std::uint64_t at = static_cast<std::uint64_t>(c) * 2;
            fat_byte(at + 1);
            fat_[at] = static_cast<std::uint8_t>(value);
            fat_[at + 1] = static_cast<std::uint8_t>(value >> 8);
        } else {
            const std::uint64_t at = static_cast<std::uint64_t>(c) * 4;
            fat_byte(at + 3);
            const std::uint32_t keep = u32_at(fat_, at) & 0xF0000000u;   // the top four bits are reserved: kept
            const std::uint32_t nv = keep | (value & 0x0FFFFFFF);
            for (std::size_t k = 0; k < 4; ++k) fat_[at + k] = static_cast<std::uint8_t>(nv >> (8 * k));
        }
    }
    std::vector<std::uint32_t> chain_of(std::uint32_t c) const {
        std::vector<std::uint32_t> out;
        std::unordered_set<std::uint32_t> seen;
        while (c >= 2 && c < end_ - 7 && seen.insert(c).second) {
            out.push_back(c);
            c = get(c);
        }
        return out;
    }
    std::vector<std::uint32_t> allocate(std::uint64_t n) {
        std::vector<std::uint32_t> got;
        std::uint32_t c = free_from_;
        while (got.size() < n) {
            if (c >= g_.clusters + 2) throw Error("the volume is full");
            if (get(c) == 0) got.push_back(c);
            ++c;
        }
        free_from_ = c;
        for (std::size_t i = 0; i < got.size(); ++i) set(got[i], i + 1 < got.size() ? got[i + 1] : end_);
        return got;
    }
    void write_clusters(const std::vector<std::uint32_t>& chain, ByteView data) {
        for (std::size_t i = 0; i < chain.size(); ++i) {
            const std::size_t from = std::min<std::size_t>(i * step_, data.size()), to = std::min<std::size_t>((i + 1) * step_, data.size());
            Bytes piece(data.begin() + static_cast<std::ptrdiff_t>(from), data.begin() + static_cast<std::ptrdiff_t>(to));
            piece.resize(static_cast<std::size_t>(step_), 0);
            file_.write_at(base_ + g_.data_at + static_cast<std::uint64_t>(chain[i] - 2) * step_, piece);
        }
    }
    // The directory slots of a folder: the fixed root of FAT12/16 (start 0), else the folder's cluster chain.
    std::vector<Slot> slots(std::uint32_t start) {
        std::vector<std::pair<std::uint64_t, std::uint64_t>> regions;   // (offset in the image file, length)
        if (start == 0 && g_.bits != 32) {
            regions.push_back({base_ + g_.root_at, static_cast<std::uint64_t>(g_.root_entries) * 32});
        } else {
            for (std::uint32_t c : chain_of(start != 0 ? start : g_.root_cluster))
                regions.push_back({base_ + g_.data_at + static_cast<std::uint64_t>(c - 2) * step_, step_});
        }
        std::vector<Slot> out;
        for (const auto& [at, len] : regions) {
            const Bytes raw = file_.read_at(at, static_cast<std::size_t>(len));
            for (std::size_t i = 0; i + 32 <= raw.size(); i += 32)
                out.push_back({at + i, Bytes(raw.begin() + static_cast<std::ptrdiff_t>(i), raw.begin() + static_cast<std::ptrdiff_t>(i) + 32)});
        }
        return out;
    }
    std::optional<Hit> find(std::uint32_t start, const ShortName& name) {
        for (Slot& s : slots(start)) {
            if (s.entry[0] == 0) break;
            if (s.entry[0] != 0xE5 && (s.entry[11] & 0x3F) != kAttrLongName && !(s.entry[11] & kAttrVolumeLabel) &&
                std::equal(name.begin(), name.end(), s.entry.begin()))
                return Hit{s.at, std::move(s.entry)};
        }
        return std::nullopt;
    }
    // A free directory slot, growing the folder by a cluster when it is full (the fixed root cannot grow).
    std::uint64_t new_slot(std::uint32_t start) {
        for (const Slot& s : slots(start))
            if (s.entry[0] == 0 || s.entry[0] == 0xE5) return s.at;
        if (start == 0 && g_.bits != 32) throw Error("the root folder is full");
        const std::uint32_t c = allocate(1)[0];
        const std::vector<std::uint32_t> chain = chain_of(start != 0 ? start : g_.root_cluster);
        if (chain.empty()) throw FormatError(kReader, 0, "a folder with no clusters");
        set(chain.back(), c);
        write_clusters({c}, {});
        return base_ + g_.data_at + static_cast<std::uint64_t>(c - 2) * step_;
    }
    std::uint32_t start_of(const Bytes& e) const {
        return u16_at(e, 26) | (g_.bits == 32 ? static_cast<std::uint32_t>(u16_at(e, 20)) << 16 : 0);
    }
    void set_start(Bytes& e, std::uint32_t cluster) const {
        put16(e, 26, cluster & 0xFFFF);
        if (g_.bits == 32) put16(e, 20, cluster >> 16);
    }
    std::uint32_t make_folder(std::uint32_t parent, const ShortName& name, std::uint16_t time, std::uint16_t date) {
        const std::uint32_t c = allocate(1)[0];
        Bytes dot(32, 0);
        constexpr std::string_view kDot = ".          ";
        std::copy(kDot.begin(), kDot.end(), dot.begin());
        dot[11] = kAttrDirectory;
        put16(dot, 22, time);
        put16(dot, 24, date);
        set_start(dot, c);
        Bytes dotdot = dot;
        constexpr std::string_view kDotDot = "..         ";
        std::copy(kDotDot.begin(), kDotDot.end(), dotdot.begin());
        set_start(dotdot, parent == g_.root_cluster ? 0 : parent);   // ".." of a folder in the root points at 0
        Bytes both = dot;
        append(both, dotdot);
        write_clusters({c}, both);
        Bytes e = dot;
        std::copy(name.begin(), name.end(), e.begin());
        file_.write_at(new_slot(parent), e);
        return c;
    }

    FatGeometry g_;
    std::uint64_t base_, step_;
    std::uint32_t end_;
    Bytes fat_;
    fs::RandomFile file_;
    std::uint32_t free_from_ = 2;
};

}  // namespace

std::vector<PutResult> put(const std::filesystem::path& image, std::span<const NewFile> files, std::size_t volume) {
    std::vector<std::unique_ptr<Volume>> all = volumes(image);
    if (volume >= all.size()) throw UsageError(std::format("{}: no volume number {}", image.filename().string(), volume));
    const VolumeKind kind = all[volume]->kind();
    if (kind != VolumeKind::Fat12 && kind != VolumeKind::Fat16 && kind != VolumeKind::Fat32)
        throw UnsupportedError(kReader, 0, "not a FAT volume with a parameter block");
    const auto* fat = static_cast<const FatVolume*>(all[volume].get());
    if (fat->geometry().old) throw UnsupportedError(kReader, 0, "not a FAT volume with a parameter block");

    const std::time_t now = std::time(nullptr);
    std::tm t{};
    localtime_s(&t, &now);
    const auto time = static_cast<std::uint16_t>((t.tm_hour << 11) | (t.tm_min << 5) | (t.tm_sec / 2));
    const auto date = static_cast<std::uint16_t>(((t.tm_year + 1900 - 1980) << 9) | ((t.tm_mon + 1) << 5) | t.tm_mday);

    FatWriter writer(image, *fat);
    std::vector<PutResult> done;
    try {
        for (const NewFile& f : files) {
            std::vector<std::string> parts;
            const std::string slashed = text::replace_all(f.path, "\\", "/");
            for (std::string_view part : text::split(slashed, '/'))
                if (!part.empty()) parts.emplace_back(part);
            if (parts.empty()) throw UsageError("an empty file name");
            const std::uint32_t folder = writer.folder_of(parts, time, date);
            const PutOutcome how = writer.write_file(folder, short_name_of(parts.back()), f.data, time, date);
            done.push_back({text::join(parts, "/"), how});
        }
    } catch (...) {   // the files already written must stay reachable: the table goes back before the error leaves
        try {
            writer.flush_fats();
        } catch (const Error&) {   // the original error is the one to report
        }
        throw;
    }
    writer.flush_fats();
    return done;
}

// ---- extract, wildcards, listing ------------------------------------------------------------------------------------------------

std::filesystem::path safe_relative_path(std::string_view path) {
    std::filesystem::path out;
    const std::string slashed = text::replace_all(path, "\\", "/");
    for (std::string_view part : text::split(slashed, '/')) {
        if (part.empty() || part == "." || part == "..") continue;
        std::string clean(part);
        for (char& c : clean)
            if (std::string_view("<>:\"|?*").find(c) != std::string_view::npos || static_cast<unsigned char>(c) < 32) c = '_';
        out /= std::filesystem::u8path(clean);
    }
    if (out.empty()) throw FormatError(kReader, 0, std::format("'{}' has no usable file name", path));
    return out;
}

namespace {

struct WildToken {
    enum class Kind { Star, Any, Char, Set } kind = Kind::Char;
    char c = 0;
    bool negate = false;
    std::vector<std::pair<char, char>> ranges;
};

char fold(char c) { return c == '\\' ? '/' : static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

std::vector<WildToken> wildcard_tokens(std::string_view pattern) {
    std::vector<WildToken> out;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        const char c = pattern[i];
        WildToken t;
        if (c == '*') {
            if (!out.empty() && out.back().kind == WildToken::Kind::Star) continue;
            t.kind = WildToken::Kind::Star;
        } else if (c == '?') {
            t.kind = WildToken::Kind::Any;
        } else if (c == '[') {
            std::size_t j = i + 1;
            if (j < pattern.size() && pattern[j] == '!') ++j;
            if (j < pattern.size() && pattern[j] == ']') ++j;   // a ']' first is a member, not the end
            while (j < pattern.size() && pattern[j] != ']') ++j;
            if (j >= pattern.size()) {   // no closing bracket: a plain '['
                t.c = fold(c);
            } else {
                t.kind = WildToken::Kind::Set;
                std::size_t k = i + 1;
                if (pattern[k] == '!') {
                    t.negate = true;
                    ++k;
                }
                for (; k < j; ++k) {
                    if (k + 2 < j && pattern[k + 1] == '-') {
                        t.ranges.emplace_back(fold(pattern[k]), fold(pattern[k + 2]));
                        k += 2;
                    } else {
                        t.ranges.emplace_back(fold(pattern[k]), fold(pattern[k]));
                    }
                }
                i = j;
            }
        } else {
            t.c = fold(c);
        }
        out.push_back(std::move(t));
    }
    return out;
}

bool token_matches(const WildToken& t, char c) {
    switch (t.kind) {
    case WildToken::Kind::Any: return true;
    case WildToken::Kind::Char: return t.c == c;
    case WildToken::Kind::Set: {
        const bool in = std::ranges::any_of(t.ranges, [c](const auto& r) { return r.first <= c && c <= r.second; });
        return in != t.negate;
    }
    case WildToken::Kind::Star: return false;
    }
    return false;
}

}  // namespace

bool wildcard_match(std::string_view pattern, std::string_view name) {
    const std::vector<WildToken> tokens = wildcard_tokens(pattern);
    std::string subject(name);
    for (char& c : subject) c = fold(c);
    // positions of the subject the tokens so far can have consumed
    std::vector<char> now(subject.size() + 1, 0), next;
    now[0] = 1;
    for (const WildToken& t : tokens) {
        next.assign(subject.size() + 1, 0);
        if (t.kind == WildToken::Kind::Star) {
            bool open = false;
            for (std::size_t k = 0; k <= subject.size(); ++k) {
                open = open || now[k];
                next[k] = open;
            }
        } else {
            for (std::size_t k = 0; k < subject.size(); ++k)
                if (now[k] && token_matches(t, subject[k])) next[k + 1] = 1;
        }
        now.swap(next);
    }
    return now[subject.size()] != 0;
}

std::size_t extract(const std::filesystem::path& image, const std::filesystem::path& folder, std::span<const std::string> names,
                    bool long_names) {
    std::size_t written = 0;
    std::size_t index = 0;
    for (const std::unique_ptr<Volume>& v : volumes(image, long_names)) {
        const std::filesystem::path base = index == 0 ? folder : folder / std::format("volume{}", index + 1);
        ++index;
        for (const Entry& e : v->files()) {
            if (e.is_dir) continue;
            if (!names.empty()) {
                const std::string_view last = std::string_view(e.path).substr(e.path.rfind('/') == std::string::npos ? 0 : e.path.rfind('/') + 1);
                const bool wanted = std::ranges::any_of(names, [&](const std::string& w) { return wildcard_match(w, e.path) || wildcard_match(w, last); });
                if (!wanted) continue;
            }
            const std::filesystem::path out = base / safe_relative_path(e.path);
            if (!out.parent_path().empty()) std::filesystem::create_directories(out.parent_path());
            fs::write_file(out, v->read(e));
            ++written;
        }
    }
    return written;
}

std::string listing_text(Volume& volume, bool long_names, bool boot_fields) {
    const std::vector<Entry>& files = volume.files();
    std::size_t nfiles = 0, nfolders = 0;
    std::uint64_t nbytes = 0;
    for (const Entry& e : files) {
        if (e.is_dir) {
            ++nfolders;
        } else {
            ++nfiles;
            nbytes += e.size;
        }
    }
    std::string out = std::format("{} volume '{}'{}: {} files, {} folders, {} bytes\n", kind_name(volume.kind()), volume.label(),
                                  volume.where().empty() ? "" : std::format(" ({})", volume.where()), nfiles, nfolders, nbytes);
    if (boot_fields)
        for (const BootField& f : volume.boot()) out += std::format("    {:<20} {}\n", f.name, f.text);
    for (const Entry& e : files)
        out += std::format("  {:>10}  {:<16}  {}\n", e.is_dir ? std::string("<folder>") : std::to_string(e.size), e.when,
                           long_names ? e.path : e.short_name);
    return out;
}

}  // namespace cb::formats::media
