#include "cb/formats/xbox/fatx.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/xex.hpp"

namespace cb::formats::xbox::fatx {

namespace {

constexpr std::string_view kReader = "fatx";
constexpr int kMaxDirectoryDepth = 900;
constexpr std::uint32_t kMagic = 0x58544146;   // "FATX" read little-endian

[[noreturn]] void bad(std::string what, std::uint64_t offset = 0) { throw FormatError(kReader, offset, std::move(what)); }
void check_name(std::string_view name);

std::uint32_t read_u32(ByteView d, std::size_t at, ByteOrder order) {
    io::ByteReader r(d, kReader);
    r.seek(at);
    return order == ByteOrder::Big ? r.u32_be() : r.u32();
}
std::uint16_t read_u16(ByteView d, std::size_t at, ByteOrder order) {
    io::ByteReader r(d, kReader);
    r.seek(at);
    return order == ByteOrder::Big ? r.u16_be() : r.u16();
}
void put_u32(Bytes& b, std::size_t at, std::uint32_t v, ByteOrder order) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (order == ByteOrder::Big ? 24 - 8 * i : 8 * i));
}

bool is_leap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }
int days_in_month(int y, int m) {
    static constexpr int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return m == 2 && is_leap(y) ? 29 : d[m - 1];
}

// A path as components: '/' and '\\' separate, empty parts are skipped
std::vector<std::string> split_path(std::string_view path) {
    std::vector<std::string> out;
    const std::string slashes = text::replace_all(path, "\\", "/");
    for (std::string_view part : text::split(slashes, '/')) {
        if (!part.empty()) out.emplace_back(part);
    }
    return out;
}

// A Windows device name (NUL, CON, COM1 ...), with or without an extension: opening it writes to the device, not a file.
bool is_reserved_device_name(std::string_view name) {
    const std::string base = text::upper(name.substr(0, name.find('.')));
    if (base == "CON" || base == "PRN" || base == "AUX" || base == "NUL") return true;
    return base.size() == 4 && (base.starts_with("COM") || base.starts_with("LPT")) && base[3] >= '1' && base[3] <= '9';
}

// Where an entry's path lands under `root`; a path that would leave it is refused (a real FATX name has none of these).
std::filesystem::path safe_destination(const std::filesystem::path& root, const std::string& path) {
    std::filesystem::path dest = root;
    bool any = false;
    for (const std::string& part : split_path(path)) {
        if (part == "." || part == ".." || part.find(':') != std::string::npos) bad("refusing to write the unsafe name '" + path + "'");
        dest /= fs::from_text(part);
        any = true;
    }
    if (!any) bad("an entry with no name");
    return dest;
}

}  // namespace

bool is_valid_name_byte(std::uint8_t b) {
    static const auto table = [] {
        std::array<bool, 256> t{};
        for (unsigned char c : std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 !#$%&'()-.@[]^_`{}~")) t[c] = true;
        t[0xFF] = true;
        return t;
    }();
    return table[b];
}

const std::array<PartitionDef, 5> kPartitions = {{
    {"SystemCache", 0x80000, 0x80000000},
    {"GameCache", 0x80080000, 0xA0E30000},
    {"SystemAux", 0x8000400, 0x6E000000},
    {"SystemExtended", 0x12000400, 0xDFFFC00},
    {"Content", 0x10C080000, 0},
}};
const std::array<PartitionDef, 1> kUsbPartitions = {{{"Data", 0x8000, 0}}};

std::string Timestamp::to_string() const { return std::format("{:04}-{:02}-{:02} {:02}:{:02}", year, month, day, hour, minute); }

std::optional<Timestamp> decode_timestamp(std::uint32_t t, int epoch) {
    if (t == 0) return std::nullopt;
    Timestamp ts;
    ts.year = static_cast<int>((t & 0xFE000000u) >> 25) + epoch;
    ts.month = static_cast<int>((t & 0x01E00000u) >> 21);
    ts.day = static_cast<int>((t & 0x001F0000u) >> 16);
    ts.hour = static_cast<int>((t & 0x0000F800u) >> 11);
    ts.minute = static_cast<int>((t & 0x000007E0u) >> 5);
    ts.second = std::min(static_cast<int>(t & 0x1Fu) * 2, 59);
    if (ts.year < 1 || ts.year > 9999 || ts.month < 1 || ts.month > 12 || ts.day < 1 || ts.day > days_in_month(ts.year, ts.month) ||
        ts.hour > 23 || ts.minute > 59) {
        return std::nullopt;   // not a date
    }
    return ts;
}

std::uint32_t encode_timestamp(const Timestamp& t, int epoch) {
    const std::uint32_t y = static_cast<std::uint32_t>(std::max(0, t.year - epoch));
    return (y << 25) | (static_cast<std::uint32_t>(t.month) << 21) | (static_cast<std::uint32_t>(t.day) << 16) |
           (static_cast<std::uint32_t>(t.hour) << 11) | (static_cast<std::uint32_t>(t.minute) << 5) | static_cast<std::uint32_t>(t.second / 2);
}

// ---- the volume ------------------------------------------------------------------------------------------------

Volume::Volume(const std::filesystem::path& path) : file_(path, fs::FileMode::Read), size_(file_.size()) {}

Bytes Volume::read_at(std::uint64_t offset, std::size_t count) { return file_.read_at(offset, count); }

void Volume::make_writable() {
    if (file_.mode() == fs::FileMode::Read) file_ = fs::RandomFile(std::filesystem::path(file_.path()), fs::FileMode::ReadWrite);
}

void Volume::write_at(std::uint64_t offset, ByteView data) { file_.write_at(offset, data); }

std::optional<Partition> Volume::mount(const PartitionDef& def) {
    if (def.offset + 0x10 > size_) return std::nullopt;
    const Bytes hdr = file_.read_at(def.offset, 0x10);
    if (hdr.size() < 0x10 || read_u32(hdr, 0, ByteOrder::Little) != kMagic) return std::nullopt;
    Partition p;
    p.name = std::string(def.name);
    p.offset = def.offset;
    p.size = def.size > 0 ? def.size : size_ - def.offset;
    // Auto-detect the byte order: a real sectors_per_cluster is a small power of two, so whichever order yields one in
    // {1..256} is right. Xbox 360 is big-endian (epoch 1980); the original Xbox little-endian (2000).
    bool found = false;
    for (const auto [order, epoch] : {std::pair{ByteOrder::Big, 1980}, std::pair{ByteOrder::Little, 2000}}) {
        const std::uint32_t spc = read_u32(hdr, 8, order);
        if (spc == 1 || spc == 2 || spc == 4 || spc == 8 || spc == 16 || spc == 32 || spc == 64 || spc == 128 || spc == 256) {
            p.order = order;
            p.epoch = epoch;
            p.sectors_per_cluster = spc;
            const std::uint32_t root = read_u32(hdr, 12, order);
            p.root_cluster = root ? root : 1;
            found = true;
            break;
        }
    }
    if (!found) return std::nullopt;
    p.bytes_per_cluster = p.sectors_per_cluster * kSectorSize;
    p.max_clusters = p.size / p.bytes_per_cluster + 1;
    p.fat16 = p.max_clusters < 0xFFF0;
    std::uint64_t bytes_per_fat = p.max_clusters * (p.fat16 ? 2 : 4);
    bytes_per_fat = (bytes_per_fat + kPageSize - 1) & ~std::uint64_t{kPageSize - 1};
    p.fat_offset = def.offset + kPageSize;
    p.data_offset = p.fat_offset + bytes_per_fat;
    return p;
}

std::vector<Partition> Volume::detect_partitions() {
    std::vector<Partition> found;
    std::vector<std::uint64_t> seen;
    std::vector<PartitionDef> candidates = {{"Partition", 0, 0}};
    candidates.insert(candidates.end(), kPartitions.begin(), kPartitions.end());
    candidates.insert(candidates.end(), kUsbPartitions.begin(), kUsbPartitions.end());
    for (const PartitionDef& def : candidates) {
        if (std::find(seen.begin(), seen.end(), def.offset) != seen.end()) continue;
        if (std::optional<Partition> p = mount(def)) {
            found.push_back(std::move(*p));
            seen.push_back(def.offset);
        }
    }
    return found;
}

const std::vector<std::uint32_t>& Volume::fat(const Partition& p) {
    const auto cached = fat_cache_.find(p.offset);
    if (cached != fat_cache_.end()) return cached->second;
    const std::size_t width = p.fat16 ? 2 : 4;
    const Bytes raw = file_.read_at(p.fat_offset, static_cast<std::size_t>(p.max_clusters * width));
    std::vector<std::uint32_t> table(raw.size() / width);
    for (std::size_t i = 0; i < table.size(); ++i) table[i] = p.fat16 ? read_u16(raw, i * 2, p.order) : read_u32(raw, i * 4, p.order);
    return fat_cache_.emplace(p.offset, std::move(table)).first->second;
}

std::uint64_t Volume::cluster_offset(const Partition& p, std::uint64_t cluster) const {
    return p.data_offset + std::uint64_t{p.bytes_per_cluster} * (cluster - 1);
}

Bytes Volume::read_cluster(const Partition& p, std::uint64_t cluster) { return file_.read_at(cluster_offset(p, cluster), p.bytes_per_cluster); }

std::vector<std::uint32_t> Volume::cluster_chain(const Partition& p, std::uint32_t first) {
    const std::vector<std::uint32_t>& table = fat(p);
    const std::uint32_t reserved = p.fat16 ? 0xFFF0u : 0xFFFFFFF0u;
    std::vector<std::uint32_t> chain = {first};
    std::uint32_t cur = first;
    while (true) {
        if (cur >= table.size()) break;
        const std::uint32_t next = table[cur];
        if (next >= reserved || next == 0 || next >= table.size()) break;
        chain.push_back(next);
        cur = next;
        if (chain.size() > p.max_clusters) break;
    }
    return chain;
}

std::optional<Entry> Volume::parse_dirent(const Partition& p, ByteView slot, std::string_view base, std::uint32_t cluster) const {
    const std::uint8_t name_len = slot[0];
    if (name_len == kDirentNeverUsed || name_len == kDirentNeverUsed2) return std::nullopt;   // the end of the directory
    Entry e;
    e.cluster = cluster;
    e.is_deleted = name_len == kDirentDeleted;
    const std::size_t n = e.is_deleted ? kMaxFilename : std::min<std::size_t>(name_len & 0x3F, kMaxFilename);
    // the name: up to the first 0xFF or NUL; a byte that is not ASCII reads as U+FFFD
    for (std::size_t i = 0; i < n; ++i) {
        const std::uint8_t c = slot[2 + i];
        if (c == 0xFF || c == 0) break;
        if (c < 0x80) e.name.push_back(static_cast<char>(c));
        else e.name += "\xEF\xBF\xBD";
    }
    e.attributes = slot[1];
    e.first_cluster = read_u32(slot, 44, p.order);
    e.file_size = read_u32(slot, 48, p.order);
    e.created = decode_timestamp(read_u32(slot, 52, p.order), p.epoch);
    e.modified = decode_timestamp(read_u32(slot, 56, p.order), p.epoch);
    e.accessed = decode_timestamp(read_u32(slot, 60, p.order), p.epoch);
    e.is_directory = (e.attributes & kAttrDirectory) != 0;
    e.path = base.empty() ? e.name : std::string(base) + "/" + e.name;
    return e;
}

std::vector<Entry> Volume::list_directory_at(const Partition& p, std::uint32_t cluster, std::string_view base, bool include_deleted,
                                             std::vector<std::uint32_t>& ancestors) {
    if (ancestors.size() > static_cast<std::size_t>(kMaxDirectoryDepth)) bad("directories nested too deeply");
    ancestors.push_back(cluster);
    std::vector<Entry> entries;
    for (std::uint32_t cl : cluster_chain(p, cluster)) {
        const Bytes data = read_cluster(p, cl);
        bool stop = false;
        for (std::size_t i = 0; i + kDirentSize <= data.size(); i += kDirentSize) {
            std::optional<Entry> e = parse_dirent(p, ByteView(data).subspan(i, kDirentSize), base, cl);
            if (!e) {
                stop = true;
                break;
            }
            if (e->is_deleted && !include_deleted) continue;
            entries.push_back(std::move(*e));
        }
        if (stop) break;
    }
    for (Entry& e : entries) {
        if (e.is_directory && e.first_cluster > 0 && e.first_cluster <= p.max_clusters && !e.is_deleted) {
            if (std::find(ancestors.begin(), ancestors.end(), e.first_cluster) != ancestors.end()) bad("a directory that contains itself: " + e.path);
            e.children = list_directory_at(p, e.first_cluster, e.path, include_deleted, ancestors);
        }
    }
    ancestors.pop_back();
    return entries;
}

std::vector<Entry> Volume::list_directory(const Partition& p, std::uint32_t cluster, std::string_view base, bool include_deleted) {
    std::vector<std::uint32_t> ancestors;
    return list_directory_at(p, cluster == 0 ? p.root_cluster : cluster, base, include_deleted, ancestors);
}

void Volume::extract_file(const Partition& p, const Entry& e, const std::filesystem::path& out, bool contiguous) {
    fs::RandomFile dest(out, fs::FileMode::Create);
    std::uint64_t remaining = e.file_size;
    std::vector<std::uint32_t> chain;
    if (contiguous || e.is_deleted) {
        // a deleted file's FAT chain is cleared: read the clusters in order
        const std::uint64_t need = (remaining + p.bytes_per_cluster - 1) / p.bytes_per_cluster;
        for (std::uint64_t i = 0; i < need; ++i) chain.push_back(static_cast<std::uint32_t>(e.first_cluster + i));
    } else {
        chain = cluster_chain(p, e.first_cluster);
    }
    std::uint64_t written = 0;
    for (std::uint32_t cl : chain) {
        if (remaining == 0) break;
        Bytes chunk = read_cluster(p, cl);
        chunk.resize(std::min<std::uint64_t>(chunk.size(), std::min<std::uint64_t>(remaining, p.bytes_per_cluster)));
        dest.write_at(written, chunk);
        written += chunk.size();
        remaining -= chunk.size();
    }
}

namespace {

void flatten(const std::vector<Entry>& entries, std::vector<const Entry*>& out) {
    for (const Entry& e : entries) {
        out.push_back(&e);
        flatten(e.children, out);
    }
}

}  // namespace

std::size_t Volume::extract_all(const Partition& p, const std::filesystem::path& out_dir) {
    const std::vector<Entry> tree = list_directory(p);
    std::vector<const Entry*> all;
    flatten(tree, all);
    std::size_t n = 0;
    for (const Entry* e : all) {
        const std::filesystem::path dst = safe_destination(out_dir, e->path);
        if (is_reserved_device_name(e->name)) continue;   // DIVERGES: the old reader wrote such a file to the device (the data vanished)
        if (e->is_directory) {
            fs::make_dirs(dst);
        } else {
            extract_file(p, *e, dst);
            ++n;
        }
    }
    return n;
}

// ---- recovery --------------------------------------------------------------------------------------------------

std::vector<Entry> Volume::recover(const Partition& p) {
    std::vector<Entry> orphans;
    for (std::uint64_t cl = 1; cl < p.max_clusters; ++cl) {
        const std::uint64_t off = cluster_offset(p, cl);
        if (off + p.bytes_per_cluster > size_) break;
        const Bytes data = file_.read_at(off, p.bytes_per_cluster);
        for (std::size_t i = 0; i + kDirentSize <= data.size(); i += kDirentSize) {
            const ByteView slot = ByteView(data).subspan(i, kDirentSize);
            const std::uint8_t nl = slot[0];
            if (nl == kDirentNeverUsed || nl == 0x01 || nl == kDirentNeverUsed2) continue;
            if (nl != kDirentDeleted && (nl & 0x3F) > kMaxFilename) continue;
            if ((slot[1] & ~kValidAttributes) != 0) continue;
            std::optional<Entry> e = parse_dirent(p, slot, {}, static_cast<std::uint32_t>(cl));
            if (!e) continue;
            // a dirent that looks real: a first cluster that exists, a name of the characters FATX allows, valid attributes
            if (!(e->first_cluster > 0 && e->first_cluster <= p.max_clusters)) continue;
            if (e->name.empty() || !std::all_of(e->name.begin(), e->name.end(), [](char c) { return is_valid_name_byte(static_cast<std::uint8_t>(c)); })) continue;
            if ((e->attributes & ~kValidAttributes) != 0) continue;
            orphans.push_back(std::move(*e));
        }
    }
    // rebuild the folders by cluster-chain membership: what sits in a directory orphan's clusters is its content
    std::unordered_map<std::uint32_t, std::vector<std::size_t>> by_cluster;   // lookup only
    for (std::size_t i = 0; i < orphans.size(); ++i) by_cluster[orphans[i].cluster].push_back(i);
    for (std::size_t i = 0; i < orphans.size(); ++i) {
        if (!orphans[i].is_directory || orphans[i].first_cluster == 0 || orphans[i].first_cluster > p.max_clusters) continue;
        for (std::uint32_t member : cluster_chain(p, orphans[i].first_cluster)) {
            const auto it = by_cluster.find(member);
            if (it == by_cluster.end()) continue;
            for (std::size_t child : it->second) {
                if (child != i) orphans[child].path = orphans[i].name + "/" + orphans[child].name;
            }
        }
    }
    return orphans;
}

std::size_t Volume::recover_all(const Partition& p, const std::filesystem::path& out_dir, bool deleted_only) {
    std::size_t n = 0;
    for (const Entry& e : recover(p)) {
        if (deleted_only && !e.is_deleted) continue;
        if (e.is_directory || e.file_size == 0) continue;
        try {   // one file of many: a name that cannot be written is skipped
            extract_file(p, e, safe_destination(out_dir, e.path), true);
            ++n;
        } catch (const IoError&) {
        } catch (const FormatError&) {
        }
    }
    return n;
}

// ---- the carver ------------------------------------------------------------------------------------------------

namespace {

std::optional<std::uint64_t> stfs_length(ByteView head) {
    if (head.size() < 0x399) return std::nullopt;
    const std::uint32_t total_blocks = read_u32(head, 0x395, ByteOrder::Little) & 0xFFFFFF;
    if (total_blocks > 0 && total_blocks < 0x100000) return 0xB000u + std::uint64_t{total_blocks} * 0x1000;
    return std::nullopt;
}

std::uint64_t page(std::uint64_t n) { return (n + kPageSize - 1) & ~std::uint64_t{kPageSize - 1}; }

// Exact on-disk XEX2 file length = data_offset + stored-data length, derived from the file-format-info: walk the block
// chain (normal), sum the data runs (basic), or add the image size (none). nullopt if the header is short or the file is an
// encrypted normal one (whose block headers need the title key).
std::optional<std::uint64_t> xex_length(Volume& v, std::uint64_t pos, ByteView head, std::uint64_t avail) {
    if (head.size() < 0x18) return std::nullopt;
    const std::uint32_t data_offset = read_u32(head, 0x08, ByteOrder::Big);
    const std::uint32_t sec = read_u32(head, 0x10, ByteOrder::Big);
    const std::uint32_t opt_count = read_u32(head, 0x14, ByteOrder::Big);
    if (data_offset == 0 || data_offset > avail || opt_count > 0x400) return std::nullopt;
    Bytes reread;
    ByteView hdr = head;
    if (head.size() < data_offset) {
        reread = v.read_at(pos, data_offset);
        hdr = reread;
    }
    if (hdr.size() < std::min<std::uint64_t>(data_offset, 0x18 + std::uint64_t{opt_count} * 8)) return std::nullopt;
    auto image_size = [&]() -> std::uint64_t {
        const std::uint64_t img = std::uint64_t{sec} + 8 + 256;   // the image-info block
        return img + 0x14 <= hdr.size() ? read_u32(hdr, img + 0x10, ByteOrder::Big) : 0;
    };
    std::optional<xex::OptionalHeader> ffi;
    try {
        ffi = xex::find_optional_header(hdr, xex::kFileFormatInfo);
    } catch (const FormatError&) {
        return std::nullopt;   // a damaged table
    }
    if (!ffi || ffi->value + 8ull > hdr.size()) {   // uncompressed, no file format info
        const std::uint64_t sz = image_size();
        return sz ? std::optional<std::uint64_t>(page(data_offset + sz)) : std::nullopt;
    }
    const std::uint64_t at = ffi->value;
    const std::uint16_t enc = read_u16(hdr, at + 4, ByteOrder::Big);
    const std::uint16_t comp = read_u16(hdr, at + 6, ByteOrder::Big);
    if (comp == 1) {   // basic: run sizes, in the header
        const std::uint32_t info_size = read_u32(hdr, at, ByteOrder::Big);
        std::uint64_t total = 0;
        for (std::uint64_t o = at + 8; o + 8 <= at + info_size && o + 8 <= hdr.size(); o += 8) total += read_u32(hdr, o, ByteOrder::Big);
        return page(data_offset + total);
    }
    if (comp == 2) {   // normal: LZX in blocks
        if (enc) return std::nullopt;   // the block headers are encrypted
        std::uint64_t block = read_u32(hdr, at + 12, ByteOrder::Big);
        std::uint64_t total = 0, cur = pos + data_offset;
        for (std::uint32_t guard = 0; block != 0 && guard < 0x100000; ++guard) {
            total += block;
            const Bytes next = v.read_at(cur, 4);
            if (next.size() < 4) break;
            cur += block;
            block = read_u32(next, 0, ByteOrder::Big);
        }
        return page(data_offset + total);
    }
    const std::uint64_t sz = image_size();   // comp == 0 (none)
    return sz ? std::optional<std::uint64_t>(page(data_offset + sz)) : std::nullopt;
}

std::optional<std::uint64_t> pe_length(Volume& v, std::uint64_t pos, ByteView head, std::uint64_t avail) {
    if (head.size() < 0x40) return std::nullopt;
    const std::uint32_t e_lfanew = read_u32(head, 0x3C, ByteOrder::Little);
    if (e_lfanew + 0x40ull > avail) return std::nullopt;
    const Bytes sig = v.read_at(pos + e_lfanew, 4);
    if (sig.size() < 4 || std::memcmp(sig.data(), "PE\0\0", 4) != 0) return std::nullopt;
    const Bytes coff = v.read_at(pos + e_lfanew + 4, 20);
    if (coff.size() < 20) return std::nullopt;
    const std::uint16_t sections = read_u16(coff, 2, ByteOrder::Little);
    const std::uint16_t optional_size = read_u16(coff, 16, ByteOrder::Little);
    std::uint64_t end = 0;
    std::uint64_t at = pos + e_lfanew + 24 + optional_size;
    for (unsigned i = 0; i < std::min<unsigned>(sections, 96); ++i, at += 40) {
        const Bytes sh = v.read_at(at, 40);
        if (sh.size() < 40) break;
        end = std::max<std::uint64_t>(end, std::uint64_t{read_u32(sh, 20, ByteOrder::Little)} + read_u32(sh, 16, ByteOrder::Little));
    }
    return end ? std::optional<std::uint64_t>(end) : std::nullopt;
}

struct Found {
    std::string kind;
    std::uint64_t length;
};

std::optional<Found> carve_at(Volume& v, std::uint64_t pos, std::uint64_t avail) {
    const Bytes head = v.read_at(pos, static_cast<std::size_t>(std::min<std::uint64_t>(0x400, avail)));
    if (head.size() < 0x18) return std::nullopt;
    const std::string_view magic = as_text(ByteView(head).first(4));
    if (magic == "XEX2") {
        const auto n = xex_length(v, pos, head, avail);
        if (n && *n >= 0x1000 && *n <= avail) return Found{"XEX", *n};
        return std::nullopt;
    }
    if (magic == "XBEH") {
        const Bytes size = v.read_at(pos + 0x10C, 4);   // the XBE's SizeOfImage
        const std::uint32_t n = size.size() == 4 ? read_u32(size, 0, ByteOrder::Little) : 0;
        if (n >= 0x1000 && n <= avail) return Found{"XBE", n};
        return std::nullopt;
    }
    if (magic == "CON " || magic == "LIVE" || magic == "PIRS") {
        const auto n = stfs_length(head);
        if (n && *n <= avail) return Found{"STFS", *n};
        return std::nullopt;
    }
    if (head[0] == 'M' && head[1] == 'Z') {
        const auto n = pe_length(v, pos, head, avail);
        if (n && *n >= 0x200 && *n <= avail) return Found{"PE", *n};
        return std::nullopt;
    }
    return std::nullopt;
}

std::string_view extension_for(const std::string& kind) {
    if (kind == "XEX") return "xex";
    if (kind == "XBE") return "xbe";
    if (kind == "STFS") return "stfs";
    if (kind == "PE") return "exe";
    return "bin";
}

}  // namespace

std::vector<Carved> Volume::carve(const Partition& p, const std::filesystem::path& out_dir, std::uint32_t interval, bool /*decrypt*/) {
    if (interval == 0) throw UsageError("carve: the interval must be positive");
    std::vector<Carved> results;
    const std::uint64_t base = p.data_offset;
    const std::uint64_t end = std::min<std::uint64_t>(p.offset + p.size, size_);
    std::uint64_t pos = base;
    while (pos + 4 <= end) {
        if (const std::optional<Found> f = carve_at(*this, pos, end - pos)) {
            const Bytes blob = file_.read_at(pos, static_cast<std::size_t>(f->length));
            const std::filesystem::path dst = out_dir / "carved" / fs::from_text(std::format("{:08X}_{}.{}", pos - base, f->kind, extension_for(f->kind)));
            fs::write_file(dst, blob);
            results.push_back({pos - base, f->kind, f->length, dst});
            pos += (f->length + interval - 1) / interval * interval;
        } else {
            pos += interval;
        }
    }
    return results;
}

// ---- writing ---------------------------------------------------------------------------------------------------

void Volume::set_fat(const Partition& p, std::uint32_t cluster, std::uint32_t value) {
    make_writable();
    const std::uint32_t masked = p.fat16 ? value & 0xFFFFu : value;
    const std::size_t width = p.fat16 ? 2 : 4;
    Bytes raw(width);
    if (p.fat16) {
        raw[0] = static_cast<std::uint8_t>(masked >> (p.order == ByteOrder::Big ? 8 : 0));
        raw[1] = static_cast<std::uint8_t>(masked >> (p.order == ByteOrder::Big ? 0 : 8));
    } else {
        put_u32(raw, 0, masked, p.order);
    }
    write_at(p.fat_offset + std::uint64_t{cluster} * width, raw);
    (void)fat(p);
    std::vector<std::uint32_t>& table = fat_cache_.at(p.offset);
    if (cluster < table.size()) table[cluster] = masked;
}

std::vector<std::uint32_t> Volume::allocate(const Partition& p, std::size_t n) {
    const std::vector<std::uint32_t>& table = fat(p);
    std::vector<std::uint32_t> free;
    for (std::uint64_t c = 1; c < p.max_clusters && free.size() < n; ++c) {
        if (c < table.size() && table[c] == 0) free.push_back(static_cast<std::uint32_t>(c));
    }
    if (free.size() < n) throw IoError(file_.path(), 0, std::format("not enough free clusters ({} needed, {} free)", n, free.size()));
    return free;
}

std::uint32_t Volume::resolve_dir(const Partition& p, std::string_view path) {
    std::uint32_t cluster = p.root_cluster;
    for (const std::string& part : split_path(path)) {
        bool found = false;
        for (const Entry& e : list_directory(p, cluster)) {
            if (e.is_directory && text::iequals(e.name, part)) {
                cluster = e.first_cluster;
                found = true;
                break;
            }
        }
        if (!found) throw NotFoundError(fs::from_text(part), 0, "directory not found: " + part);
    }
    return cluster;
}

void Volume::write_dirent(const Partition& p, std::uint32_t dir_cluster, std::string_view name, std::uint8_t attr, std::uint32_t first,
                          std::uint32_t size) {
    check_name(name);
    const std::string_view cut = name.substr(0, kMaxFilename);
    const Timestamp now{2020, 1, 1, 0, 0, 0};
    const std::uint32_t ts = encode_timestamp(now, p.epoch);
    Bytes rec(kDirentSize, 0xFF);
    rec[0] = static_cast<std::uint8_t>(cut.size());
    rec[1] = attr;
    std::copy(cut.begin(), cut.end(), rec.begin() + 2);
    put_u32(rec, 44, first, p.order);
    put_u32(rec, 48, size, p.order);
    put_u32(rec, 52, ts, p.order);
    put_u32(rec, 56, ts, p.order);
    put_u32(rec, 60, ts, p.order);
    const std::vector<std::uint32_t> chain = cluster_chain(p, dir_cluster);
    for (std::uint32_t cl : chain) {
        const std::uint64_t base = cluster_offset(p, cl);
        const Bytes data = file_.read_at(base, p.bytes_per_cluster);
        for (std::size_t i = 0; i + kDirentSize <= data.size(); i += kDirentSize) {
            if (data[i] == kDirentNeverUsed || data[i] == kDirentNeverUsed2 || data[i] == kDirentDeleted) {
                write_at(base + i, rec);
                return;
            }
        }
    }
    // the directory is full: extend it with a fresh cluster
    const std::uint32_t fresh = allocate(p, 1)[0];
    set_fat(p, chain.back(), fresh);
    set_fat(p, fresh, p.fat16 ? 0xFFFFu : 0xFFFFFFFFu);
    write_at(cluster_offset(p, fresh), Bytes(p.bytes_per_cluster, 0xFF));
    write_at(cluster_offset(p, fresh), rec);
}

namespace {

// What may be written as a name: ASCII, not empty (a name longer than the field is cut at it).
void check_name(std::string_view name) {
    if (name.empty()) throw UsageError("a FATX entry needs a name");
    if (std::any_of(name.begin(), name.end(), [](char c) { return static_cast<unsigned char>(c) >= 0x80; })) {
        throw UsageError("FATX names are ASCII: " + std::string(name));
    }
}

// "dir/sub/name" -> ("dir/sub", "name")
std::pair<std::string, std::string> split_parent(std::string_view path) {
    const std::string slashes = text::replace_all(path, "\\", "/");
    const std::size_t cut = slashes.rfind('/');
    if (cut == std::string::npos) return {"", slashes};
    return {slashes.substr(0, cut), slashes.substr(cut + 1)};
}

}  // namespace

void Volume::add_file(const Partition& p, std::string_view dest_path, ByteView data) {
    make_writable();
    const auto [parent, name] = split_parent(dest_path);
    check_name(name);
    const std::uint32_t dir_cluster = resolve_dir(p, parent);
    const std::size_t n = std::max<std::size_t>(1, (data.size() + p.bytes_per_cluster - 1) / p.bytes_per_cluster);
    const std::vector<std::uint32_t> clusters = allocate(p, n);
    for (std::size_t i = 0; i < clusters.size(); ++i) {   // the data
        Bytes chunk(p.bytes_per_cluster, 0);
        const std::size_t from = i * p.bytes_per_cluster;
        if (from < data.size()) std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(from), std::min<std::size_t>(p.bytes_per_cluster, data.size() - from), chunk.begin());
        write_at(cluster_offset(p, clusters[i]), chunk);
    }
    const std::uint32_t end = p.fat16 ? 0xFFFFu : 0xFFFFFFFFu;
    for (std::size_t i = 0; i < clusters.size(); ++i) set_fat(p, clusters[i], i + 1 < clusters.size() ? clusters[i + 1] : end);   // the chain
    write_dirent(p, dir_cluster, name, kAttrArchive, clusters[0], static_cast<std::uint32_t>(data.size()));
}

void Volume::mkdir(const Partition& p, std::string_view dest_path) {
    make_writable();
    const auto [parent, name] = split_parent(dest_path);
    check_name(name);
    const std::uint32_t dir_cluster = resolve_dir(p, parent);
    const std::uint32_t fresh = allocate(p, 1)[0];
    set_fat(p, fresh, p.fat16 ? 0xFFFFu : 0xFFFFFFFFu);
    write_at(cluster_offset(p, fresh), Bytes(p.bytes_per_cluster, 0xFF));   // an empty directory
    write_dirent(p, dir_cluster, name, kAttrDirectory, fresh, 0);
}

void Volume::remove(const Partition& p, std::string_view path) {
    make_writable();
    const auto [parent, name] = split_parent(path);
    const std::uint32_t dir_cluster = resolve_dir(p, parent);
    for (std::uint32_t cl : cluster_chain(p, dir_cluster)) {
        const std::uint64_t base = cluster_offset(p, cl);
        const Bytes data = file_.read_at(base, p.bytes_per_cluster);
        for (std::size_t i = 0; i + kDirentSize <= data.size(); i += kDirentSize) {
            const std::optional<Entry> e = parse_dirent(p, ByteView(data).subspan(i, kDirentSize), {}, cl);
            if (!e) break;   // the end of this cluster's entries (the directory's next cluster is still looked at)
            if (!e->is_deleted && text::iequals(e->name, name)) {
                write_at(base + i, Bytes{kDirentDeleted});   // the deleted marker
                const std::vector<std::uint32_t>& table = fat(p);
                const std::uint32_t reserved = p.fat16 ? 0xFFF0u : 0xFFFFFFF0u;
                std::uint32_t c = e->first_cluster;   // free the FAT chain
                while (c > 0 && c < table.size() && table[c] < reserved) {
                    const std::uint32_t next = table[c];
                    set_fat(p, c, 0);
                    c = next;
                }
                if (c > 0 && c < table.size()) set_fat(p, c, 0);
                return;
            }
        }
    }
    throw NotFoundError(fs::from_text(std::string(path)), 0, "not found: " + std::string(path));
}

}  // namespace cb::formats::xbox::fatx
