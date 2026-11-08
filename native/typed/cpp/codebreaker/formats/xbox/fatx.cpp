// codebreaker/formats/xbox/fatx.cpp -- C++17 port of codebreaker.formats.xbox.fatx (see fatx.hpp).

#include "codebreaker/formats/xbox/fatx.hpp"

#include <algorithm>

namespace codebreaker::formats::xbox::fatx {

// ── Constants ───────────────────────────────────────────
const py::bytes FATX_MAGIC("FATX");

const py::set<int64_t> _VALID_NAME = [] {
    py::set<int64_t> s;
    const char* chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
        " !#$%&'()-.@[]^_`{}~";
    for (const char* c = chars; *c; ++c) s.add(static_cast<unsigned char>(*c));
    s.add(0xFF);
    return s;
}();

// Standard Xbox 360 retail HDD partition map (offset, size; size 0 = to end).
const py::list<PartitionDef> PARTITIONS = {
    PartitionDef{py::str("SystemCache"), 0x80000, 0x80000000},
    PartitionDef{py::str("GameCache"), 0x80080000, 0xA0E30000},
    PartitionDef{py::str("SystemAux"), 0x8000400, 0x6E000000},
    PartitionDef{py::str("SystemExtended"), 0x12000400, 0xDFFFC00},
    PartitionDef{py::str("Content"), 0x10C080000, 0},
};
const py::list<PartitionDef> USB_PARTITIONS = {PartitionDef{py::str("Data"), 0x8000, 0}};

namespace {

// '>' and '!' are big-endian; '<', '=', '@' and '' little-endian (the same bytes for the formats used here)
bool big_endian_(const py::str& endian) {
    if (endian == ">" || endian == "!") return true;
    if (endian == "<" || endian == "=" || endian == "@" || endian.empty()) return false;
    throw py::StructError("bad char in struct format");
}

// the unsigned `width`-byte value at `off` (the caller has checked the bytes are there)
int64_t get_uint_(const py::bytes& b, int64_t off, int64_t width, bool big) {
    int64_t v = 0;
    for (int64_t k = 0; k < width; ++k) v |= b.byte_(off + (big ? width - 1 - k : k)) << (8 * k);
    return v;
}

void put_uint_(std::string& out, int64_t v, int64_t width, bool big) {
    for (int64_t k = 0; k < width; ++k) {
        int64_t shift = 8 * (big ? width - 1 - k : k);
        out.push_back(static_cast<char>((v >> shift) & 0xFF));
    }
}

bool leap_(int64_t y) {
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

// the Python's `x.ljust(width, b"\0")` on bytes
py::bytes pad_to_(const py::bytes& b, int64_t width) {
    if (b.size() >= width) return b;
    return py::bytes(b.raw() + std::string(static_cast<size_t>(width - b.size()), '\0'));
}

}  // namespace

// ── Timestamp ───────────────────────────────────────────
py::str DateTime::strftime_minute() const {
    return py::percent("%04d-%02d-%02d %02d:%02d", year, month, day, hour, minute);
}

std::optional<DateTime> decode_timestamp(int64_t t, int64_t epoch) {
    if (t == 0) {
        return std::nullopt;
    }
    int64_t year = ((t & 0xFE000000) >> 25) + epoch;
    int64_t month = (t & 0x01E00000) >> 21;
    int64_t day = (t & 0x001F0000) >> 16;
    int64_t hour = (t & 0x0000F800) >> 11;
    int64_t minute = (t & 0x000007E0) >> 5;
    int64_t second = (t & 0x0000001F) * 2;
    second = py::min(second, int64_t(59));
    // datetime(year, month, day, hour, minute, second): a ValueError is None
    static const int64_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year < 1 || year > 9999 || month < 1 || month > 12) return std::nullopt;
    int64_t dim = days[month - 1] + (month == 2 && leap_(year) ? 1 : 0);
    if (day < 1 || day > dim || hour > 23 || minute > 59) return std::nullopt;
    return DateTime{year, month, day, hour, minute, second};
}

// ── Volume reader ───────────────────────────────────────
FatxReader::FatxReader(const py::str& path_) : path(path_) {
    _stream = py::open_rb(path_);
    _stream->seek(0, 2);
    _size = _stream->tell();
}

FatxReader::~FatxReader() {
    try {
        close();
    } catch (...) {
    }
}

void FatxReader::close() {
    if (_stream.has_value()) {
        _stream->close();
        _stream.reset();
    }
}

py::file& FatxReader::stream_(const char* method) {
    if (!_stream.has_value()) {
        throw py::AttributeError(std::string("'NoneType' object has no attribute '") + method + "'");
    }
    return *_stream;
}

// -- partition discovery --------------------------------------------
py::list<PartitionRef> FatxReader::detect_partitions() {
    py::list<PartitionRef> found;
    py::set<int64_t> seen;
    py::list<PartitionDef> candidates = {PartitionDef{py::str("Partition"), 0, 0}};
    candidates.extend(PARTITIONS);
    candidates.extend(USB_PARTITIONS);
    for (const PartitionDef& pdef : candidates) {
        if (seen.contains(pdef.offset)) {
            continue;
        }
        PartitionRef p = _mount(pdef);
        if (p && p->valid) {
            found.append(p);
            seen.add(pdef.offset);
        }
    }
    return found;
}

py::bytes FatxReader::_read(int64_t off, int64_t n) {
    py::file& s = stream_("seek");
    if (off < 0) {
        throw py::OSError("[Errno 22] Invalid argument");
    }
    s.seek(off);
    return s.read(n);
}

PartitionRef FatxReader::_mount(const PartitionDef& pdef) {
    int64_t off = pdef.offset;
    if (off + 0x10 > _size || _read(off, 4) != FATX_MAGIC) {
        return nullptr;
    }
    PartitionRef p = std::make_shared<FatxPartition>();
    p->name = pdef.name;
    p->offset = off;
    p->size = pdef.size > 0 ? pdef.size : (_size - off);

    // Auto-detect endianness: a real sectors_per_cluster is a small power of
    // two, so whichever byte order yields one in {1..256} is correct. Xbox
    // 360 is big-endian (epoch 1980); original Xbox little-endian (2000).
    py::bytes hdr = _read(off, 0x10);
    bool found = false;
    for (bool big : {true, false}) {
        int64_t epoch = big ? 1980 : 2000;
        if (hdr.size() != 0x10) throw py::StructError("unpack requires a buffer of 16 bytes");
        int64_t spc = get_uint_(hdr, 8, 4, big);
        int64_t root = get_uint_(hdr, 12, 4, big);
        if (spc == 1 || spc == 2 || spc == 4 || spc == 8 || spc == 16 || spc == 32 || spc == 64 || spc == 128 || spc == 256) {
            p->endian = py::str(big ? ">" : "<");
            p->epoch = epoch;
            p->sectors_per_cluster = spc;
            p->root_cluster = root != 0 ? root : 1;
            found = true;
            break;
        }
    }
    if (!found) {
        return nullptr;
    }
    p->bytes_per_cluster = p->sectors_per_cluster * FATX_SECTOR_SIZE;
    p->max_clusters = py::floordiv(p->size, p->bytes_per_cluster) + 1;
    p->fat16x = p->max_clusters < 0xFFF0;
    int64_t bytes_per_fat = p->max_clusters * (p->fat16x ? 2 : 4);
    bytes_per_fat = (bytes_per_fat + FATX_PAGE_SIZE - 1) & ~(FATX_PAGE_SIZE - 1);
    p->fat_offset = off + FATX_PAGE_SIZE;
    p->data_offset = p->fat_offset + bytes_per_fat;
    p->valid = true;
    return p;
}

// -- FAT + clusters -------------------------------------------------
py::list<int64_t> FatxReader::_fat(const PartitionRef& p) {
    std::optional<py::list<int64_t>> cached = _fat_cache.get(p->offset);
    if (cached.has_value()) {
        return *cached;
    }
    int64_t n = p->max_clusters;
    int64_t width = p->fat16x ? 2 : 4;
    py::bytes raw = _read(p->fat_offset, n * width);
    bool big = big_endian_(p->endian);   // (struct.error for an endian text struct does not know)
    int64_t count = py::floordiv(py::len(raw), width);
    py::list<int64_t> fat;
    for (int64_t i = 0; i < count; ++i) fat.append(get_uint_(raw, i * width, width, big));
    _fat_cache.set(p->offset, fat);
    return fat;
}

int64_t FatxReader::cluster_offset(const PartitionRef& p, int64_t cluster) const {
    return p->data_offset + p->bytes_per_cluster * (cluster - 1);
}

py::bytes FatxReader::read_cluster(const PartitionRef& p, int64_t cluster) {
    return _read(cluster_offset(p, cluster), p->bytes_per_cluster);
}

py::list<int64_t> FatxReader::cluster_chain(const PartitionRef& p, int64_t first) {
    py::list<int64_t> fat = _fat(p);
    int64_t reserved = p->fat16x ? 0xFFF0 : 0xFFFFFFF0;
    py::list<int64_t> chain = {first};
    int64_t cur = first;
    while (true) {
        if (cur >= py::len(fat)) {
            break;
        }
        int64_t nxt = fat[cur];
        if (nxt >= reserved || nxt == 0 || nxt >= py::len(fat)) {
            break;
        }
        chain.append(nxt);
        cur = nxt;
        if (py::len(chain) > p->max_clusters) {
            break;
        }
    }
    return chain;
}

// -- dirent parsing -------------------------------------------------
EntryRef FatxReader::_parse_dirent(const PartitionRef& p, const py::bytes& data, const py::str& base, int64_t cluster) {
    int64_t name_len = data[0];
    if (name_len == DIRENT_NEVER_USED || name_len == DIRENT_NEVER_USED2) {
        return nullptr;   // end of directory
    }
    // struct.unpack(endian + "BB42sLLLLL", data)
    bool big = big_endian_(p->endian);
    if (data.size() != 0x40) throw py::StructError("unpack requires a buffer of 64 bytes");
    int64_t attr = data.byte_(1);
    py::bytes name_bytes = data.slice(2, 44);
    int64_t first = get_uint_(data, 44, 4, big);
    int64_t size = get_uint_(data, 48, 4, big);
    int64_t ct = get_uint_(data, 52, 4, big);
    int64_t mt = get_uint_(data, 56, 4, big);
    int64_t at = get_uint_(data, 60, 4, big);
    EntryRef e = std::make_shared<FatxEntry>();
    e->cluster = cluster;
    e->is_deleted = (name_len == DIRENT_DELETED);
    int64_t n = e->is_deleted ? FATX_MAX_FILENAME : py::min(name_len & 0x3F, FATX_MAX_FILENAME);
    // name_bytes[:n].split(b"\xff")[0].split(b"\x00")[0]
    py::bytes named = name_bytes.slice(0, n);
    int64_t cut = named.size();
    for (int64_t k = 0; k < named.size(); ++k) {
        if (named.byte_(k) == 0xFF || named.byte_(k) == 0x00) {
            cut = k;
            break;
        }
    }
    e->name = named.slice(0, cut).decode("ascii", "replace");
    e->attributes = attr;
    e->first_cluster = first;
    e->file_size = size;
    e->created = decode_timestamp(ct, p->epoch);
    e->modified = decode_timestamp(mt, p->epoch);
    e->accessed = decode_timestamp(at, p->epoch);
    e->is_directory = (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
    e->path = !base.empty() ? base + py::str("/") + e->name : e->name;
    return e;
}

py::list<EntryRef> FatxReader::list_directory(const PartitionRef& p, int64_t cluster, const py::str& base, bool include_deleted) {
    return list_directory_(p, cluster, base, include_deleted, 0);
}

py::list<EntryRef> FatxReader::list_directory_(const PartitionRef& p, int64_t cluster, const py::str& base, bool include_deleted,
                                               int64_t depth) {
    if (depth > 900) {
        throw RecursionError("maximum recursion depth exceeded");
    }
    py::list<EntryRef> entries = read_entries_(p, cluster, base, include_deleted);
    for (const EntryRef& e : entries) {
        if (e->is_directory && 0 < e->first_cluster && e->first_cluster <= p->max_clusters && !e->is_deleted) {
            e->children = list_directory_(p, e->first_cluster, e->path, include_deleted, depth + 1);
        }
    }
    return entries;
}

py::list<EntryRef> FatxReader::read_entries_(const PartitionRef& p, int64_t cluster, const py::str& base, bool include_deleted) {
    if (cluster == 0) {
        cluster = p->root_cluster;
    }
    py::list<EntryRef> entries;
    for (int64_t cl : cluster_chain(p, cluster)) {
        py::bytes data = read_cluster(p, cl);
        bool stop = false;
        for (int64_t i = 0; i < py::len(data); i += FATX_DIRENT_SIZE) {
            py::bytes slot = data.slice(i, i + FATX_DIRENT_SIZE);
            if (py::len(slot) < FATX_DIRENT_SIZE) {
                break;
            }
            EntryRef e = _parse_dirent(p, slot, base, cl);
            if (!e) {
                stop = true;
                break;
            }
            if (e->is_deleted && !include_deleted) {
                continue;
            }
            entries.append(e);
        }
        if (stop) {
            break;
        }
    }
    return entries;
}

// -- extraction -----------------------------------------------------
void FatxReader::extract_file(const PartitionRef& p, const EntryRef& e, const py::str& out, bool contiguous) {
    py::str folder = py::os::path::dirname(out);
    py::os::makedirs(!folder.empty() ? folder : py::str("."), 0777, true);
    int64_t remaining = e->file_size;
    py::list<int64_t> chain;
    if (contiguous || e->is_deleted) {
        // deleted files have a cleared FAT chain: read clusters in order.
        int64_t need = py::floordiv(remaining + p->bytes_per_cluster - 1, p->bytes_per_cluster);
        for (int64_t k = e->first_cluster; k < e->first_cluster + need; ++k) chain.append(k);
    } else {
        chain = cluster_chain(p, e->first_cluster);
    }
    py::file f = py::open(out, py::str("wb"));
    for (int64_t cl : chain) {
        if (remaining <= 0) {
            break;
        }
        py::bytes chunk = read_cluster(p, cl).slice(0, py::min(remaining, p->bytes_per_cluster));
        f.write(chunk);
        remaining -= py::len(chunk);
    }
    f.close();
}

py::list<EntryRef> FatxReader::_flatten(const py::list<EntryRef>& entries) {
    py::list<EntryRef> out;
    for (const EntryRef& e : entries) {
        out.append(e);
        if (!e->children.empty()) {
            out.extend(_flatten(e->children));
        }
    }
    return out;
}

int64_t FatxReader::extract_all(const PartitionRef& p, const py::str& out_dir) {
    int64_t n = 0;
    for (const EntryRef& e : _flatten(list_directory(p))) {
        py::str dst = py::os::path::join(out_dir, e->path.replace(py::str("/"), py::os::sep));
        if (e->is_directory) {
            py::os::makedirs(dst, 0777, true);
        } else {
            extract_file(p, e, dst);
            n += 1;
        }
    }
    return n;
}

// -- deleted-file recovery (metadata / orphan analysis) -------------
bool FatxReader::_dirent_valid(const PartitionRef& p, const EntryRef& e) const {
    if (!(0 < e->first_cluster && e->first_cluster <= p->max_clusters)) {
        return false;
    }
    if (e->name.empty()) {
        return false;
    }
    for (int64_t c : e->name.encode("latin-1", "replace")) {
        if (!_VALID_NAME.contains(c)) {
            return false;
        }
    }
    if ((e->attributes & ~VALID_FILE_ATTRIBUTES) != 0) {
        return false;
    }
    return true;
}

py::list<EntryRef> FatxReader::recover(const PartitionRef& p) {
    py::list<EntryRef> orphans;
    for (int64_t cl = 1; cl < p->max_clusters; ++cl) {
        int64_t off = cluster_offset(p, cl);
        if (off + p->bytes_per_cluster > _size) {
            break;
        }
        py::bytes data = _read(off, p->bytes_per_cluster);
        for (int64_t i = 0; i < py::len(data); i += FATX_DIRENT_SIZE) {
            py::bytes slot = data.slice(i, i + FATX_DIRENT_SIZE);
            if (py::len(slot) < FATX_DIRENT_SIZE) {
                break;
            }
            int64_t nl = slot[0];
            if (nl == DIRENT_NEVER_USED || nl == 0x01 || nl == DIRENT_NEVER_USED2) {
                continue;
            }
            if (nl != DIRENT_DELETED && (nl & 0x3F) > FATX_MAX_FILENAME) {
                continue;
            }
            if ((slot[1] & ~VALID_FILE_ATTRIBUTES) != 0) {
                continue;
            }
            EntryRef e;
            try {
                e = _parse_dirent(p, slot, py::str(""), cl);
            } catch (const py::StructError&) {
                continue;
            }
            if (e && _dirent_valid(p, e)) {
                orphans.append(e);
            }
        }
    }
    // rebuild parent/child by cluster-chain membership
    py::dict<int64_t, py::list<EntryRef>> by_cluster;
    for (const EntryRef& o : orphans) {
        by_cluster.setdefault(o->cluster, py::list<EntryRef>()).append(o);
    }
    for (const EntryRef& o : orphans) {
        if (o->is_directory && 0 < o->first_cluster && o->first_cluster <= p->max_clusters) {
            for (int64_t member : cluster_chain(p, o->first_cluster)) {
                for (const EntryRef& child : by_cluster.get(member, py::list<EntryRef>())) {
                    if (child != o) {
                        child->path = o->name + py::str("/") + child->name;
                        o->children.append(child);
                    }
                }
            }
        }
    }
    return orphans;
}

int64_t FatxReader::recover_all(const PartitionRef& p, const py::str& out_dir, bool deleted_only) {
    int64_t n = 0;
    for (const EntryRef& e : recover(p)) {
        if (deleted_only && !e->is_deleted) {
            continue;
        }
        if (e->is_directory || e->file_size == 0) {
            continue;
        }
        py::str dst = py::os::path::join(out_dir, e->path.replace(py::str("/"), py::os::sep));
        try {
            extract_file(p, e, dst, true);
            n += 1;
        } catch (const py::OSError&) {
        }
    }
    return n;
}

// -- signature carver -----------------------------------------------
py::list<std::tuple<int64_t, py::str, int64_t, py::str>> FatxReader::carve(const PartitionRef& p, const py::str& out_dir,
                                                                           int64_t interval, bool decrypt) {
    py::list<std::tuple<int64_t, py::str, int64_t, py::str>> results;
    int64_t base = p->data_offset;
    int64_t end = py::min(p->offset + p->size, _size);
    int64_t pos = base;
    int64_t n = 0;
    while (pos + 4 <= end) {
        std::optional<std::tuple<py::str, int64_t>> length = _carve_at(stream_("seek"), pos, end - pos, decrypt);
        if (length.has_value()) {
            const auto& [kind, ln] = *length;
            py::file& s = stream_("seek");
            s.seek(pos);
            py::bytes blob = s.read(ln);
            py::str ext = _EXT.get(kind, py::str("bin"));
            py::str dst = py::os::path::join(out_dir, py::str("carved"), py::percent("%08X_%s.%s", pos - base, kind, ext));
            py::os::makedirs(py::os::path::dirname(dst), 0777, true);
            py::file f = py::open(dst, py::str("wb"));
            f.write(blob);
            f.close();
            results.append(std::make_tuple(pos - base, kind, ln, dst));
            n += 1;
            pos += py::floordiv(ln + interval - 1, interval) * interval;
        } else {
            pos += interval;
        }
    }
    return results;
}

// -- write side: inject / mkdir / delete ----------------------------
void FatxReader::_writable() {
    if (!_stream.has_value() || !_writable_mode) {
        stream_("close").close();
        _stream = py::open(path, py::str("r+b"));
        _writable_mode = true;
    }
}

void FatxReader::_write(int64_t off, const py::bytes& data) {
    py::file& s = stream_("seek");
    if (off < 0) {
        throw py::OSError("[Errno 22] Invalid argument");
    }
    s.seek(off);
    s.write(data);
}

void FatxReader::_set_fat(const PartitionRef& p, int64_t cluster, int64_t value) {
    int64_t width = p->fat16x ? 2 : 4;
    bool big = big_endian_(p->endian);
    int64_t mask = p->fat16x ? 0xFFFF : 0xFFFFFFFF;
    std::string packed;
    put_uint_(packed, value & mask, width, big);
    _write(p->fat_offset + cluster * width, py::bytes(packed));
    py::list<int64_t> fat = _fat(p);
    if (cluster < py::len(fat)) {
        fat[cluster] = value & mask;
    }
}

int64_t FatxReader::_end(const PartitionRef& p) const {
    return p->fat16x ? 0xFFFF : 0xFFFFFFFF;
}

py::list<int64_t> FatxReader::_alloc(const PartitionRef& p, int64_t n) {
    py::list<int64_t> fat = _fat(p);
    py::list<int64_t> free_clusters;
    for (int64_t c = 1; c < p->max_clusters; ++c) {
        if (c < py::len(fat) && fat[c] == 0) free_clusters.append(c);
    }
    free_clusters = free_clusters.slice(0, n);
    if (py::len(free_clusters) < n) {
        throw py::OSError(py::percent("not enough free clusters (%d needed, %d free)", n, py::len(free_clusters)).utf8());
    }
    return free_clusters;
}

int64_t FatxReader::_encode_ts(const PartitionRef& p, const std::optional<DateTime>& dt_in) const {
    DateTime dt = dt_in.has_value() ? *dt_in : DateTime{2020, 1, 1, 0, 0, 0};
    int64_t y = py::max(int64_t(0), dt.year - p->epoch);
    return ((y << 25) | (dt.month << 21) | (dt.day << 16) | (dt.hour << 11) | (dt.minute << 5) | py::floordiv(dt.second, 2)) & 0xFFFFFFFF;
}

int64_t FatxReader::_resolve_dir(const PartitionRef& p, const py::str& path_) {
    int64_t cluster = p->root_cluster;
    for (const py::str& part : py::split(path_.replace(py::str("\\"), py::str("/")), py::str("/"))) {
        if (part.empty()) {
            continue;
        }
        bool found = false;
        for (const EntryRef& e : list_directory(p, cluster)) {
            if (e->is_directory && e->name.lower() == part.lower()) {
                cluster = e->first_cluster;
                found = true;
                break;
            }
        }
        if (!found) {
            throw py::OSError(py::percent("directory not found: %s", part).utf8());
        }
    }
    return cluster;
}

void FatxReader::_write_dirent(const PartitionRef& p, int64_t dir_cluster, const py::str& name, int64_t attr, int64_t first,
                               int64_t size) {
    int64_t ts = _encode_ts(p);
    py::bytes nb = name.encode("ascii").slice(0, FATX_MAX_FILENAME);
    // struct.pack(endian + "BB42sLLLLL", len(nb), attr, nb + b"\xff" * (42 - len(nb)), first, size, ts, ts, ts)
    bool big = big_endian_(p->endian);
    auto u32_ok = [](int64_t v) {
        if (v < 0 || v > 0xFFFFFFFFLL) throw py::StructError("argument out of range");
        return v;
    };
    if (attr < 0 || attr > 255) throw py::StructError("argument out of range");
    std::string packed;
    packed.push_back(static_cast<char>(py::len(nb)));
    packed.push_back(static_cast<char>(attr));
    packed += nb.raw();
    packed += std::string(static_cast<size_t>(FATX_MAX_FILENAME - py::len(nb)), '\xff');
    put_uint_(packed, u32_ok(first), 4, big);
    put_uint_(packed, u32_ok(size), 4, big);
    put_uint_(packed, u32_ok(ts), 4, big);
    put_uint_(packed, u32_ok(ts), 4, big);
    put_uint_(packed, u32_ok(ts), 4, big);
    py::bytes rec(packed);
    py::list<int64_t> chain = cluster_chain(p, dir_cluster);
    for (int64_t cl : chain) {
        int64_t base = cluster_offset(p, cl);
        py::bytes data = _read(base, p->bytes_per_cluster);
        for (int64_t i = 0; i < py::len(data); i += FATX_DIRENT_SIZE) {
            int64_t nl = data[i];
            if (nl == DIRENT_NEVER_USED || nl == DIRENT_NEVER_USED2 || nl == DIRENT_DELETED) {
                _write(base + i, rec);
                return;
            }
        }
    }
    // directory full -- extend it with a fresh cluster
    int64_t fresh = _alloc(p, 1)[0];
    _set_fat(p, chain[-1], fresh);
    _set_fat(p, fresh, _end(p));
    _write(cluster_offset(p, fresh), py::bytes(std::string(static_cast<size_t>(p->bytes_per_cluster), '\xff')));
    _write(cluster_offset(p, fresh), rec);
}

void FatxReader::add_file(const PartitionRef& p, const py::str& dest_path_in, const py::bytes& data) {
    _writable();
    py::str dest_path = dest_path_in.replace(py::str("\\"), py::str("/"));
    auto [parent, _sep, name] = dest_path.rpartition(py::str("/"));
    int64_t dir_cluster = _resolve_dir(p, parent);
    int64_t n = py::max(int64_t(1), py::floordiv(py::len(data) + p->bytes_per_cluster - 1, p->bytes_per_cluster));
    py::list<int64_t> clusters = _alloc(p, n);
    int64_t i = 0;
    for (int64_t cl : clusters) {   // write data
        py::bytes chunk = data.slice(i * p->bytes_per_cluster, (i + 1) * p->bytes_per_cluster);
        _write(cluster_offset(p, cl), pad_to_(chunk, p->bytes_per_cluster));
        ++i;
    }
    i = 0;
    for (int64_t cl : clusters) {   // link the chain
        _set_fat(p, cl, i + 1 < py::len(clusters) ? clusters[i + 1] : _end(p));
        ++i;
    }
    _write_dirent(p, dir_cluster, name, 0x20, clusters[0], py::len(data));
}

void FatxReader::mkdir(const PartitionRef& p, const py::str& dest_path_in) {
    _writable();
    py::str dest_path = dest_path_in.replace(py::str("\\"), py::str("/"));
    auto [parent, _sep, name] = dest_path.rpartition(py::str("/"));
    int64_t dir_cluster = _resolve_dir(p, parent);
    int64_t fresh = _alloc(p, 1)[0];
    _set_fat(p, fresh, _end(p));
    _write(cluster_offset(p, fresh), py::bytes(std::string(static_cast<size_t>(p->bytes_per_cluster), '\xff')));   // empty dir
    _write_dirent(p, dir_cluster, name, FILE_ATTRIBUTE_DIRECTORY, fresh, 0);
}

bool FatxReader::remove(const PartitionRef& p, const py::str& path_in) {
    _writable();
    py::str path_ = path_in.replace(py::str("\\"), py::str("/"));
    auto [parent, _sep, name] = path_.rpartition(py::str("/"));
    int64_t dir_cluster = _resolve_dir(p, parent);
    for (int64_t cl : cluster_chain(p, dir_cluster)) {
        int64_t base = cluster_offset(p, cl);
        py::bytes data = _read(base, p->bytes_per_cluster);
        for (int64_t i = 0; i < py::len(data); i += FATX_DIRENT_SIZE) {
            EntryRef e = _parse_dirent(p, data.slice(i, i + FATX_DIRENT_SIZE), py::str(""), cl);
            if (!e) {
                break;
            }
            if (!e->is_deleted && e->name.lower() == name.lower()) {
                _write(base + i, py::bytes("\xE5", 1));   // deleted marker
                int64_t c = e->first_cluster;             // free the FAT chain
                py::list<int64_t> fat = _fat(p);
                int64_t reserved = p->fat16x ? 0xFFF0 : 0xFFFFFFF0;
                while (0 < c && c < py::len(fat) && fat[c] < reserved) {
                    int64_t nxt = fat[c];
                    _set_fat(p, c, 0);
                    c = nxt;
                }
                if (0 < c && c < py::len(fat)) {
                    _set_fat(p, c, 0);
                }
                return true;
            }
        }
    }
    throw py::OSError(py::percent("not found: %s", path_).utf8());
}

// ── carver signatures ───────────────────────────────────
const py::dict<py::str, py::str> _EXT = {{py::str("XEX"), py::str("xex")},
                                         {py::str("XBE"), py::str("xbe")},
                                         {py::str("STFS"), py::str("stfs")},
                                         {py::str("PE"), py::str("exe")}};

int64_t _u32be(const py::bytes& b, int64_t o) {
    return py::unpack_from1<int64_t>(">I", b, o);
}

std::optional<std::tuple<py::str, int64_t>> _carve_at(py::file& stream, int64_t pos, int64_t avail, bool decrypt) {
    stream.seek(pos);
    py::bytes head = stream.read(py::min(int64_t(0x400), avail));
    if (py::len(head) < 0x18) {
        return std::nullopt;
    }
    if (head.slice(0, 4) == py::bytes("XEX2")) {
        std::optional<int64_t> ln = _xex_length(stream, pos, head, avail, decrypt);
        if (ln.has_value() && *ln != 0 && 0x1000 <= *ln && *ln <= avail) {
            return std::make_tuple(py::str("XEX"), *ln);
        }
        return std::nullopt;
    }
    if (head.slice(0, 4) == py::bytes("XBEH")) {
        stream.seek(pos + 0x10C);
        int64_t ln = _rd_u32le(stream);   // XBE SizeOfImage (LE)
        if (0x1000 <= ln && ln <= avail) {
            return std::make_tuple(py::str("XBE"), ln);
        }
        return std::nullopt;
    }
    py::bytes magic = head.slice(0, 4);
    if (magic == py::bytes("CON ") || magic == py::bytes("LIVE") || magic == py::bytes("PIRS")) {
        std::optional<int64_t> ln = _stfs_length(head);
        if (ln.has_value() && *ln != 0 && *ln <= avail) {
            return std::make_tuple(py::str("STFS"), *ln);
        }
        return std::nullopt;
    }
    if (head.slice(0, 2) == py::bytes("MZ")) {
        std::optional<int64_t> ln = _pe_length(stream, pos, head, avail);
        if (ln.has_value() && *ln != 0 && 0x200 <= *ln && *ln <= avail) {
            return std::make_tuple(py::str("PE"), *ln);
        }
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<int64_t> _xex_length(py::file& stream, int64_t pos, const py::bytes& head, int64_t avail, bool decrypt) {
    if (py::len(head) < 0x18) {
        return std::nullopt;
    }
    int64_t data_offset = _u32be(head, 0x08);
    int64_t sec = _u32be(head, 0x10);
    int64_t opt_count = _u32be(head, 0x14);
    if (data_offset == 0 || data_offset > avail || opt_count > 0x400) {
        return std::nullopt;
    }
    py::bytes hdr = py::len(head) >= data_offset ? head : _reread(stream, pos, data_offset);
    if (py::len(hdr) < py::min(data_offset, 0x18 + opt_count * 8)) {
        return std::nullopt;
    }
    auto _img_size = [&]() -> int64_t {
        int64_t img = sec + 8 + 256;   // image-info block
        return img + 0x14 <= py::len(hdr) ? _u32be(hdr, img + 0x10) : 0;
    };
    auto _page = [](int64_t n) { return (n + FATX_PAGE_SIZE - 1) & ~(FATX_PAGE_SIZE - 1); };

    std::optional<int64_t> ffi;
    for (const auto& [_at, k, v] : xex::optional_headers(hdr)) {
        if (k == xex::FILE_FORMAT_INFO) {
            ffi = v;
            break;
        }
    }
    if (!ffi.has_value() || *ffi + 8 > py::len(hdr)) {   // uncompressed, no FFI
        int64_t sz = _img_size();
        return sz != 0 ? std::optional<int64_t>(_page(data_offset + sz)) : std::nullopt;
    }
    int64_t at = *ffi;
    int64_t enc = py::unpack_from1<int64_t>(">H", hdr, at + 4);
    int64_t comp = py::unpack_from1<int64_t>(">H", hdr, at + 6);
    if (comp == 1) {   // basic: run sizes (in header)
        int64_t info_size = _u32be(hdr, at);
        int64_t total = 0, o = at + 8;
        while (o + 8 <= at + info_size && o + 8 <= py::len(hdr)) {
            total += _u32be(hdr, o);   // data size (skip zero size)
            o += 8;
        }
        return _page(data_offset + total);
    }
    if (comp == 2) {   // normal
        int64_t first_block = _u32be(hdr, at + 12);
        if (enc != 0) {   // block headers are encrypted
            if (!decrypt) {
                return std::nullopt;
            }
            int64_t total = _xex_decrypt_datalen(stream, pos, data_offset, first_block, avail);
            return total != 0 ? std::optional<int64_t>(_page(data_offset + total)) : std::nullopt;
        }
        int64_t block = first_block;
        int64_t total = 0, cur = pos + data_offset, guard = 0;
        while (block != 0 && guard < 0x100000) {
            total += block;
            stream.seek(cur);
            py::bytes nxt = stream.read(4);
            if (py::len(nxt) < 4) {
                break;
            }
            cur += block;
            block = py::unpack1<int64_t>(">I", nxt);
            guard += 1;
        }
        return _page(data_offset + total);
    }
    int64_t sz = _img_size();   // comp == 0 (none)
    return sz != 0 ? std::optional<int64_t>(_page(data_offset + sz)) : std::nullopt;
}

py::bytes _reread(py::file& stream, int64_t pos, int64_t n) {
    stream.seek(pos);
    return stream.read(n);
}

int64_t _xex_decrypt_datalen(py::file& stream, int64_t pos, int64_t data_offset, int64_t first_block, int64_t avail) {
    (void)stream;
    (void)pos;
    (void)data_offset;
    (void)first_block;
    (void)avail;
    return 0;
}

int64_t _rd_u32le(py::file& stream) {
    py::bytes d = stream.read(4);
    return py::len(d) == 4 ? py::unpack1<int64_t>("<I", d) : 0;
}

std::optional<int64_t> _stfs_length(const py::bytes& head) {
    try {
        int64_t total_blocks = py::unpack_from1<int64_t>("<I", head, 0x395) & 0xFFFFFF;
        if (0 < total_blocks && total_blocks < 0x100000) {
            return 0xB000 + total_blocks * 0x1000;
        }
    } catch (const py::StructError&) {
    }
    return std::nullopt;
}

std::optional<int64_t> _pe_length(py::file& stream, int64_t pos, const py::bytes& head, int64_t avail) {
    try {
        int64_t e_lfanew = py::unpack_from1<int64_t>("<I", head, 0x3C);
        if (e_lfanew + 0x40 > avail) {
            return std::nullopt;
        }
        stream.seek(pos + e_lfanew);
        py::bytes sig = stream.read(4);
        if (sig != py::bytes("PE\0\0", 4)) {
            return std::nullopt;
        }
        py::bytes coff = stream.read(20);
        int64_t num_sec = py::unpack_from1<int64_t>("<H", coff, 2);
        int64_t opt_size = py::unpack_from1<int64_t>("<H", coff, 16);
        stream.seek(pos + e_lfanew + 24 + opt_size);
        int64_t end = 0;
        for (int64_t k = 0; k < py::min(num_sec, int64_t(96)); ++k) {
            py::bytes sh = stream.read(40);
            if (py::len(sh) < 40) {
                break;
            }
            int64_t raw_size = py::unpack_from1<int64_t>("<I", sh, 16);
            int64_t raw_ptr = py::unpack_from1<int64_t>("<I", sh, 20);
            end = py::max(end, raw_ptr + raw_size);
        }
        return end != 0 ? std::optional<int64_t>(end) : std::nullopt;
    } catch (const py::StructError&) {
        return std::nullopt;
    }
}

const py::str DESCRIPTION("read an Xbox 360 FATX filesystem (info/list/extract) + recover deleted files and carve by signature");

// ── CLI ─────────────────────────────────────────────────
void _print_tree(const py::list<EntryRef>& entries, int64_t depth) {
    for (const EntryRef& e : entries) {
        py::str tag = e->is_directory ? py::str("/") : py::str("");
        py::str ts = e->modified.has_value() ? e->modified->strftime_minute() : py::str("");
        // "  %s%-*s %10s  %s": the star width is 40 - 2 * depth, a negative one meaning its size, left-justified
        int64_t width = 40 - 2 * depth;
        if (width < 0) width = -width;
        py::str size_text = e->is_directory ? py::str("") : py::str(std::to_string(e->file_size));
        py::print(py::str("  ") + py::str(std::string(static_cast<size_t>(2 * depth), ' ')) + (e->name + tag).ljust(width) +
                  py::str(" ") + size_text.rjust(10) + py::str("  ") + ts);
        if (!e->children.empty()) {
            _print_tree(e->children, depth + 1);
        }
    }
}

void main(const py::list<py::str>& a) {
    if (py::len(a) == 0 || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str(
            "usage:\n"
            "  fatx <image> [list]            list live files\n"
            "  fatx info    <image>           partition + geometry summary\n"
            "  fatx extract <image> <outdir>  extract all live files\n"
            "  fatx recover <image> <outdir>  recover deleted/orphaned files\n"
            "  fatx carve   <image> <outdir> [--decrypt]\n"
            "                                 carve files by signature (XEX/XBE/STFS/PE);\n"
            "                                 --decrypt measures encrypted XEX exactly\n"
            "  fatx add   <image> <src-file> <dest-path>   inject a file\n"
            "  fatx mkdir <image> <dest-path>              create a directory\n"
            "  fatx rm    <image> <path>                   delete a file/empty dir"));
        return;
    }
    if (a[0] == "add" || a[0] == "mkdir" || a[0] == "rm") {
        FatxReader r(a[1]);
        py::list<PartitionRef> parts = r.detect_partitions();
        if (parts.empty()) {
            py::print(py::str("no FATX partition found"));
            return;
        }
        PartitionRef p = parts[0];
        if (a[0] == "add") {
            py::file src = py::open_rb(a[2]);
            r.add_file(p, a[3], src.read());
            py::print(py::percent("injected %s -> %s", a[2], a[3]));
        } else if (a[0] == "mkdir") {
            r.mkdir(p, a[2]);
            py::print(py::percent("created directory %s", a[2]));
        } else {
            r.remove(p, a[2]);
            py::print(py::percent("deleted %s", a[2]));
        }
        r.close();
        return;
    }
    py::str op = (a[0] == "info" || a[0] == "list" || a[0] == "extract" || a[0] == "recover" || a[0] == "carve") ? a[0] : py::str("list");
    py::str img = (op != "list" || (py::len(a) > 1 && a[0] == "list")) ? a[1] : a[0];
    if (op == "list" && a[0] != "list") {
        img = a[0];
    }
    FatxReader r(img);
    py::list<PartitionRef> parts = r.detect_partitions();
    if (parts.empty()) {
        py::print(py::percent("no FATX partition found in %s", img));
        return;
    }

    if (op == "info") {
        for (const PartitionRef& p : parts) {
            py::print(py::percent("partition %-14s @0x%X  %s  cluster=%dKB  clusters=%d  FAT%d  root=%d", p->name, p->offset,
                                  p->endian == ">" ? py::str("X360-BE") : py::str("Xbox-LE"), py::floordiv(p->bytes_per_cluster, 1024),
                                  p->max_clusters, p->fat16x ? int64_t(16) : int64_t(32), p->root_cluster));
        }
        return;
    }
    if (op == "extract") {
        int64_t n = 0;
        for (const PartitionRef& p : parts) n += r.extract_all(p, a[2]);
        py::print(py::percent("extracted %d files -> %s", n, a[2]));
        return;
    }
    if (op == "recover") {
        int64_t n = 0;
        for (const PartitionRef& p : parts) n += r.recover_all(p, a[2]);
        py::print(py::percent("recovered %d deleted/orphaned files -> %s", n, a[2]));
        return;
    }
    if (op == "carve") {
        bool decrypt = a.contains(py::str("--decrypt"));
        int64_t total = 0;
        for (const PartitionRef& p : parts) {
            for (const auto& [off, kind, ln, dst] : r.carve(p, a[2], FATX_PAGE_SIZE, decrypt)) {
                py::print(py::percent("  %-5s +0x%08X  %8d bytes  %s", kind, off, ln, py::os::path::basename(dst)));
                total += 1;
            }
        }
        py::print(py::percent("carved %d files -> %s/carved%s", total, a[2], decrypt ? py::str("  (+decrypt)") : py::str("")));
        return;
    }

    for (const PartitionRef& p : parts) {
        py::print(py::percent("partition %-14s @0x%X  (%s)", p->name, p->offset, p->endian == ">" ? py::str("X360-BE") : py::str("Xbox-LE")));
        try {
            _print_tree(r.list_directory(p));
        } catch (const py::Exception& ex) {
            py::print(py::percent("   (list error: %s)", py::str(std::string(ex.what()))));
        }
    }
    r.close();
}

}  // namespace codebreaker::formats::xbox::fatx
