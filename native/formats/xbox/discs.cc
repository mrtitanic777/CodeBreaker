// discs.cc -- the Xbox disc containers: a byte device (a file, a Games-on-Demand folder, a CSO / CCI
// image), the XDVDFS (GDF) volume read through it, GoD's part writer, the LZ4 blocks of CSO / CCI.
// codebreaker/formats/xbox/{gdf,god,ciso,lz4block}.py, line for line.
//
// A device is opened once and kept in a table (a disc image is gigabytes: it is streamed, never
// loaded); the Python object holds its number and closes it.
#include <algorithm>
#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>

#include "../core/build.h"
#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "crypto/sha1.h"
#include "pyfmt.h"

namespace cbf::xbox {
namespace {

int64_t floordiv(int64_t a, int64_t b) { return a / b - ((a % b != 0) && ((a < 0) != (b < 0))); }
int64_t floormod(int64_t a, int64_t b) { return a - floordiv(a, b) * b; }

std::string py_repr(const std::string &s) {   // repr() of a str given as UTF-8 (the common cases)
    char q = (s.find('\'') != std::string::npos && s.find('"') == std::string::npos) ? '"' : '\'';
    std::string r(1, q);
    for (char c : s) {
        if (c == q || c == '\\') { r += '\\'; r += c; }
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\t') r += "\\t";
        else r += c;
    }
    return r + q;
}

#ifdef _WIN32
FILE *open_rb(const std::string &path) {
    std::wstring w = [&] {
        std::wstring out;
        for (uint32_t cp : codepoints(path)) {
            if (cp >= 0x10000) { cp -= 0x10000; out += wchar_t(0xD800 + (cp >> 10)); out += wchar_t(0xDC00 + (cp & 0x3FF)); }
            else out += wchar_t(cp);
        }
        return out;
    }();
    return _wfopen(w.c_str(), L"rb");
}
int seek64(FILE *f, int64_t at) { return _fseeki64(f, at, SEEK_SET); }
int64_t tell64(FILE *f) { return _ftelli64(f); }
#else
FILE *open_rb(const std::string &path) { return std::fopen(path.c_str(), "rb"); }
int seek64(FILE *f, int64_t at) { return fseeko(f, at, SEEK_SET); }
int64_t tell64(FILE *f) { return ftello(f); }
#endif

// a file the Python way: open() raises, seek(negative) raises, read(n) is short at the end
struct File {
    FILE *f = nullptr;
    explicit File(const std::string &path) {
        f = open_rb(path);
        if (!f) raise_open_error(path);
    }
    File(const File &) = delete;
    ~File() { if (f) std::fclose(f); }
    void seek(int64_t at) {
        if (at < 0) raise("OSError", "[Errno 22] Invalid argument");
        seek64(f, at);
    }
    std::string read(int64_t n) {   // -1: to the end; below that, Python's ValueError
        if (n < -1) raise("ValueError", "read length must be non-negative or -1");
        std::string out;
        char buf[1 << 16];
        while (n < 0 || int64_t(out.size()) < n) {
            size_t want = n < 0 ? sizeof buf : size_t(std::min<int64_t>(int64_t(sizeof buf), n - int64_t(out.size())));
            size_t got = std::fread(buf, 1, want, f);
            out.append(buf, got);
            if (got < want) break;
        }
        return out;
    }
};

struct Device {
    virtual ~Device() {}
    virtual std::string read_at(int64_t offset, int64_t length) = 0;
};

struct FileDevice : Device {
    File f;
    explicit FileDevice(const std::string &p) : f(p) {}
    std::string read_at(int64_t offset, int64_t length) override {
        f.seek(offset);
        return f.read(length);
    }
};

// ---- GoD / SVOD -------------------------------------------------------------------------------
constexpr int64_t BLOCK = 0x1000, BLOCKS_PER_SUBPART = 0xCC, SUBPARTS_PER_PART = 0xCB, BLOCKS_PER_PART = 0xA1C4;
constexpr int64_t SUBPART_SIZE = BLOCK * BLOCKS_PER_SUBPART;

int64_t phys_offset_in_part(int64_t block_in_part) {
    int64_t subpart = block_in_part / BLOCKS_PER_SUBPART, in_sub = block_in_part % BLOCKS_PER_SUBPART;
    return BLOCK + subpart * (BLOCK + SUBPART_SIZE) + BLOCK + in_sub * BLOCK;
}

struct SvodDevice : Device {
    std::vector<std::string> paths;
    std::map<int64_t, std::unique_ptr<File>> fh;
    explicit SvodDevice(const std::string &dir) {
        std::vector<std::string> names, parts;
        if (!list_dir(dir, names)) raise_open_error(dir);
        for (auto &n : names) {
            std::vector<uint32_t> cp = codepoints(n);
            std::string low;
            for (size_t k = 0; k < 4 && k < cp.size(); ++k) low += char(cp[k] < 128 ? std::tolower(int(cp[k])) : 0x80);
            if (low == "data") parts.push_back(n);
        }
        std::sort(parts.begin(), parts.end(), [](const std::string &a, const std::string &b) {
            return codepoints(a) < codepoints(b);
        });
        if (parts.empty()) raise("ValueError", "no Data#### part files in " + py_repr(dir));
        for (auto &n : parts) paths.push_back(join(dir, n));
    }
    File &file(int64_t idx) {
        auto it = fh.find(idx);
        if (it == fh.end()) it = fh.emplace(idx, std::make_unique<File>(py_at(paths, idx))).first;
        return *it->second;
    }
    std::string read_at(int64_t offset, int64_t length) override {
        std::string out;
        while (length > 0) {
            int64_t block = floordiv(offset, BLOCK), in_block = floormod(offset, BLOCK);
            int64_t part = floordiv(block, BLOCKS_PER_PART);
            if (part >= int64_t(paths.size())) break;
            int64_t phys = phys_offset_in_part(floormod(block, BLOCKS_PER_PART)) + in_block;
            int64_t take = std::min(length, BLOCK - in_block);
            File &f = file(part);
            f.seek(phys);
            std::string chunk = f.read(take);
            if (chunk.empty()) break;
            out += chunk;
            offset += int64_t(chunk.size());
            length -= int64_t(chunk.size());
        }
        return out;
    }
};

// ---- LZ4 raw blocks (lz4block.py) ---------------------------------------------------------------
std::string lz4_decode(Bytes src, int64_t usize) {
    if (usize == 0) return std::string();
    const int64_t n = src.size();
    if (n == 0) raise("Lz4Error", "empty LZ4 block");
    std::string dst;
    int64_t i = 0;
    auto need = [&](int64_t at) -> uint8_t {
        if (at >= n) raise("Lz4Error", "truncated LZ4 block");
        return src.data()[at];
    };
    while (i < n) {
        int64_t token = need(i++);
        int64_t length = token >> 4;
        if (length == 15)
            for (;;) {
                int64_t b = need(i++);
                length += b;
                if (b != 255) break;
            }
        if (length) {
            Bytes lit = src.slice(i, i + length);
            dst.append(reinterpret_cast<const char *>(lit.data()), size_t(lit.size()));
            i += length;
        }
        if (i == n) break;
        int64_t offset = need(i) | (int64_t(need(i + 1)) << 8);
        i += 2;
        if (offset == 0) raise("Lz4Error", "invalid zero match offset");
        length = (token & 0x0F) + 4;
        if ((token & 0x0F) == 15)
            for (;;) {
                int64_t b = need(i++);
                length += b;
                if (b != 255) break;
            }
        int64_t start = int64_t(dst.size()) - offset;
        if (start < 0) raise("Lz4Error", "match before start of output");
        for (int64_t k = 0; k < length; ++k) dst += dst[size_t(start + k)];
    }
    if (int64_t(dst.size()) != usize)
        raise("Lz4Error", "LZ4 block decoded to " + dec(int64_t(dst.size())) + " bytes, expected " + dec(usize));
    return dst;
}

void emit_length(std::string &out, int64_t n) {
    while (n >= 255) { out += char(255); n -= 255; }
    out += char(n);
}

std::string lz4_compress(Bytes src) {
    const int64_t n = src.size();
    const uint8_t *s = src.data();
    std::string out;
    if (n < 13) {
        out += char(std::min<int64_t>(n, 15) << 4);
        out.append(reinterpret_cast<const char *>(s), size_t(n));
        return out;
    }
    std::unordered_map<uint32_t, int64_t> htab;
    auto seq = [&](int64_t at) { return uint32_t(s[at]) | uint32_t(s[at + 1]) << 8 | uint32_t(s[at + 2]) << 16 | uint32_t(s[at + 3]) << 24; };
    int64_t anchor = 0, i = 0;
    const int64_t limit = n - 12;
    while (i < limit) {
        uint32_t q = seq(i);
        auto it = htab.find(q);
        bool have = it != htab.end();
        int64_t cand = have ? it->second : 0;
        htab[q] = i;
        if (have && (i - cand) <= 0xFFFF && seq(cand) == q) {
            int64_t mlen = 4, maxm = n - 5 - i;
            while (mlen < maxm && s[cand + mlen] == s[i + mlen]) ++mlen;
            int64_t litlen = i - anchor;
            out += char((std::min<int64_t>(litlen, 15) << 4) | std::min<int64_t>(mlen - 4, 15));
            if (litlen >= 15) emit_length(out, litlen - 15);
            out.append(reinterpret_cast<const char *>(s + anchor), size_t(i - anchor));
            out += char((i - cand) & 0xFF);
            out += char(((i - cand) >> 8) & 0xFF);
            if (mlen - 4 >= 15) emit_length(out, mlen - 4 - 15);
            for (int64_t j = i + 1; j < std::min(i + mlen, limit); ++j) htab.emplace(seq(j), j);
            i += mlen;
            anchor = i;
        } else {
            ++i;
        }
    }
    int64_t litlen = n - anchor;
    out += char(litlen < 15 ? litlen << 4 : 0xF0);
    if (litlen >= 15) emit_length(out, litlen - 15);
    out.append(reinterpret_cast<const char *>(s + anchor), size_t(litlen));
    return out;
}

// ---- CSO / CCI ----------------------------------------------------------------------------------
constexpr int64_t BLOCK_SIZE = 2048;

struct CompDevice : Device {
    File f;
    bool cci = false;
    int64_t size = 0, nblocks = 0;
    std::vector<int64_t> offs;
    std::vector<uint8_t> flags;
    int64_t cache_idx = -1;
    std::string cache;

    explicit CompDevice(const std::string &path, bool is_cci) : f(path), cci(is_cci) {
        const int64_t hsize_want = cci ? 32 : 0x18;
        std::string head = f.read(hsize_want);
        Bytes h(head);
        if (h.size() != hsize_want) raise("struct.error", "unpack requires a buffer of " + dec(hsize_want) + " bytes");
        int64_t hsize = h.u32(4), ioff = 0x18;
        size = int64_t(h.u64(8));
        int64_t bsize;
        if (cci) { ioff = int64_t(h.u64(16)); bsize = h.u32(24); }
        else bsize = h.u32(16);
        if (!h.slice_is(0, 4, cci ? "CCIM" : "CISO", 4) || hsize != hsize_want || bsize != BLOCK_SIZE)
            raise("CisoError", cci ? "not a CCI image" : "not a CSO image");
        nblocks = size / BLOCK_SIZE;
        f.seek(ioff);
        std::string raw = f.read((nblocks + 1) * 4);
        if (int64_t(raw.size()) < (nblocks + 1) * 4) raise("CisoError", "truncated index");
        Bytes r(raw);
        for (int64_t k = 0; k <= nblocks; ++k) {
            uint32_t v = r.u32(k * 4);
            offs.push_back(int64_t(v & 0x7FFFFFFF) << 2);
            flags.push_back((v & 0x80000000u) != 0);
        }
        if (flags[size_t(nblocks)]) raise("CisoError", "index terminator has compressed bit set");
    }
    int64_t compressed() const {
        int64_t c = 0;
        for (size_t k = 0; k + 1 < flags.size(); ++k) c += flags[k];
        return c;
    }
    std::string decode_block(int64_t bi) {
        if (!cci) {
            f.seek(py_at(offs, bi));
            if (!py_at(flags, bi)) return f.read(BLOCK_SIZE);
            std::string w = f.read(4);
            if (w.size() != 4) raise("struct.error", "unpack requires a buffer of 4 bytes");
            uint32_t word = Bytes(w).u32(0);
            std::string data = f.read(word & 0x7FFFFFFF);
            if (word & 0x80000000u) return data;
            return lz4_decode(Bytes(data), BLOCK_SIZE);
        }
        int64_t stored = py_at(offs, bi + 1) - py_at(offs, bi);
        f.seek(py_at(offs, bi));
        if (!py_at(flags, bi) && stored == BLOCK_SIZE) return f.read(BLOCK_SIZE);
        std::string rec = f.read(stored);
        if (rec.empty()) raise("IndexError", "index out of range");
        int64_t pad = uint8_t(rec[0]);
        Bytes payload = Bytes(rec).slice(1, stored - pad);
        return lz4_decode(payload, BLOCK_SIZE);
    }
    std::string read_at(int64_t offset, int64_t length) override {
        std::string out;
        int64_t end = std::min(offset + length, size);
        while (offset < end) {
            int64_t bi = floordiv(offset, BLOCK_SIZE);
            if (bi != cache_idx) {
                cache = decode_block(bi);
                cache_idx = bi;
            }
            int64_t o = floormod(offset, BLOCK_SIZE), take = std::min(BLOCK_SIZE - o, end - offset);
            Bytes part = Bytes(cache).slice(o, o + take);
            out.append(reinterpret_cast<const char *>(part.data()), size_t(part.size()));
            offset += take;
        }
        return out;
    }
};

// ---- the device table -----------------------------------------------------------------------------
std::mutex table_lock;
std::map<int64_t, std::shared_ptr<Device>> table;
int64_t next_id = 1;

int64_t keep(std::shared_ptr<Device> d) {
    std::lock_guard<std::mutex> g(table_lock);
    table[next_id] = std::move(d);
    return next_id++;
}
std::shared_ptr<Device> device(int64_t id) {
    std::lock_guard<std::mutex> g(table_lock);
    auto it = table.find(id);
    if (it == table.end()) raise("ValueError", "I/O operation on closed file");
    return it->second;
}

void open_file(Call &c) { c.out.integer(keep(std::make_shared<FileDevice>(c.str_arg(0)))); }

void open_svod(Call &c) {   // -> (id, [part paths])
    auto d = std::make_shared<SvodDevice>(c.str_arg(0));
    Out &o = c.out;
    o.begin_tuple();
    o.integer(keep(d));
    o.begin_list();
    for (auto &p : d->paths) o.text(p);
    o.end_list();
    o.end_tuple();
}

void open_image(Call &c) {   // -> (id, kind, size, blocks, compressed blocks)
    std::string path = c.str_arg(0), magic;
    {
        File f(path);
        magic = f.read(4);
    }
    bool cci = magic == "CCIM";
    if (magic != "CISO" && !cci) raise("CisoError", path + " is not a CSO or CCI image");
    auto d = std::make_shared<CompDevice>(path, cci);
    Out &o = c.out;
    o.begin_tuple();
    o.integer(keep(d));
    o.text(cci ? "CCI" : "CSO");
    o.integer(d->size);
    o.integer(d->nblocks);
    o.integer(d->compressed());
    o.end_tuple();
}

void read_q(Call &c) { c.out.bytes(Bytes(device(c.int_arg(0))->read_at(c.int_arg(1), c.int_arg(2)))); }

void close_q(Call &c) {
    std::lock_guard<std::mutex> g(table_lock);
    table.erase(c.int_arg(0));
    c.out.none();
}

// ---- XDVDFS (gdf.py) --------------------------------------------------------------------------------
constexpr int64_t SECTOR = 0x800;
constexpr const char MAGIC[] = "MICROSOFT*XBOX*MEDIA";
constexpr int64_t XGD_BASES[] = {0, 0xFD90000, 0x2080000, 0xFB20000, 0x18300000, 0x10000};
constexpr int64_t MAX_DIR = 1 << 20;

struct Entry {
    std::string path;          // UTF-8 of the latin-1 name
    bool is_dir;
    int64_t start, size;
};

std::vector<uint32_t> lower_cp(const std::string &utf8) {
    std::vector<uint32_t> cp = codepoints(utf8);
    for (auto &c : cp) {
        if (c >= 'A' && c <= 'Z') c += 32;
        else if (c >= 0xC0 && c <= 0xDE && c != 0xD7) c += 32;
    }
    return cp;
}

std::vector<Entry> walk_dir(Device &dev, int64_t base, int64_t sector, int64_t size, const std::string &prefix,
                            std::set<int64_t> &seen) {
    std::vector<Entry> out;
    if (seen.count(sector) || !(0 < size && size <= MAX_DIR)) return out;
    seen.insert(sector);
    std::string raw = dev.read_at(base + sector * SECTOR, (size + SECTOR - 1) / SECTOR * SECTOR);
    Bytes data(raw);
    std::vector<int64_t> todo{0};
    std::set<int64_t> visited;
    while (!todo.empty()) {
        int64_t pos = todo.back();
        todo.pop_back();
        if (visited.count(pos) || pos + 0x0E > data.size()) continue;
        visited.insert(pos);
        int64_t left = data.u16(pos), right = data.u16(pos + 2), start = data.u32(pos + 4), fsize = data.u32(pos + 8);
        int64_t attr = data.u8(pos + 0x0C), nlen = data.u8(pos + 0x0D);
        if (nlen == 0 || nlen == 0xFF || pos + 0x0E + nlen > data.size()) continue;
        std::string name = latin1(data.slice(pos + 0x0E, pos + 0x0E + nlen));
        std::string full = prefix.empty() ? name : prefix + "/" + name;
        bool is_dir = (attr & 0x10) != 0;
        out.push_back({full, is_dir, start, fsize});
        if (is_dir) {
            auto sub = walk_dir(dev, base, start, fsize, full, seen);
            out.insert(out.end(), sub.begin(), sub.end());
        }
        for (int64_t nxt : {left, right})
            if (nxt != 0xFFFF) todo.push_back(nxt * 4);
    }
    std::vector<std::pair<std::vector<uint32_t>, size_t>> keys;
    for (size_t k = 0; k < out.size(); ++k) keys.push_back({lower_cp(out[k].path), k});
    std::stable_sort(keys.begin(), keys.end(), [](const auto &a, const auto &b) { return a.first < b.first; });
    std::vector<Entry> sorted;
    for (auto &k : keys) sorted.push_back(out[k.second]);
    return sorted;
}

// gdf.open(device, base | None, descriptor | None) -> (base, root sector, root size)
void gdf_open(Call &c) {
    auto dev = device(c.int_arg(0));
    int64_t base;
    bool have = !c.arg(1).is_none();
    if (have) base = c.int_arg(1);
    else {
        have = false;
        base = 0;
        for (int64_t b : XGD_BASES)
            if (dev->read_at(b + 32 * SECTOR, 20) == std::string(MAGIC, 20)) { base = b; have = true; break; }
    }
    int64_t where = c.arg(2).is_none() ? base + 32 * SECTOR : c.int_arg(2);
    if (!have) raise("ValueError", "no XDVDFS volume found");
    std::string vd = dev->read_at(where, SECTOR);
    if (vd.compare(0, 20, MAGIC, 20) != 0 || vd.size() < 20) raise("AssertionError", "");
    Bytes v(vd);
    Out &o = c.out;
    o.begin_tuple();
    o.integer(base);
    o.integer(v.u32(0x14));
    o.integer(v.u32(0x18));
    o.end_tuple();
}

void emit_entries(Out &o, const std::vector<Entry> &es, bool files_only) {
    o.begin_list();
    for (auto &e : es) {
        if (files_only && e.is_dir) continue;
        o.begin_tuple();
        o.text(e.path);
        if (files_only) o.integer(e.size);
        else {
            o.boolean(e.is_dir);
            o.integer(e.start);
            o.integer(e.size);
        }
        o.end_tuple();
    }
    o.end_list();
}

// gdf.walk(device, base, sector, size, prefix) -> [(path, is dir, start, size)]
void gdf_walk(Call &c) {
    auto dev = device(c.int_arg(0));
    std::set<int64_t> seen;
    emit_entries(c.out, walk_dir(*dev, c.int_arg(1), c.int_arg(2), c.int_arg(3), c.str_arg(4), seen), false);
}

// gdf.files(device, base, root sector, root size) -> [(path, size)]
void gdf_files(Call &c) {
    auto dev = device(c.int_arg(0));
    std::set<int64_t> seen;
    emit_entries(c.out, walk_dir(*dev, c.int_arg(1), c.int_arg(2), c.int_arg(3), "", seen), true);
}

// gdf.read(device, base, root sector, root size, path) -> bytes, or None
void gdf_read(Call &c) {
    auto dev = device(c.int_arg(0));
    int64_t base = c.int_arg(1);
    std::set<int64_t> seen;
    auto es = walk_dir(*dev, base, c.int_arg(2), c.int_arg(3), "", seen);
    std::vector<uint32_t> want = lower_cp(c.str_arg(4));
    for (auto &e : es)
        if (!e.is_dir && lower_cp(e.path) == want) {
            c.out.bytes(Bytes(dev->read_at(base + e.start * SECTOR, e.size)));
            return;
        }
    c.out.none();
}

std::vector<uint32_t> upper_cp(const std::string &utf8) {
    std::vector<uint32_t> out;
    for (uint32_t c : codepoints(utf8)) {
        if (c >= 'a' && c <= 'z') out.push_back(c - 32);
        else if (c == 0xDF) { out.push_back('S'); out.push_back('S'); }
        else if (c == 0xB5) out.push_back(0x39C);
        else if (c == 0xFF) out.push_back(0x178);
        else if (c >= 0xE0 && c <= 0xFE && c != 0xF7) out.push_back(c - 32);
        else out.push_back(c);
    }
    return out;
}

// gdf.build_xdvdfs({name: data}) -> bytes
void build_xdvdfs(Call &c) {
    const Value &files = c.arg(0);
    std::vector<size_t> order;
    for (size_t k = 0; k < files.size(); ++k) order.push_back(k);
    std::stable_sort(order.begin(), order.end(), [&](size_t a, size_t b) {
        return upper_cp(files.items[2 * a].s) < upper_cp(files.items[2 * b].s);
    });
    const int64_t n = int64_t(order.size());
    std::vector<std::string> names;
    int64_t sizes = 0;
    for (size_t k : order) {
        names.push_back(to_latin1(files.items[2 * k].s));
        sizes += 0x0E + int64_t(names.back().size()) + 3;
    }
    const int64_t root_sector = 33;
    const int64_t data_sector = 34 + (sizes + SECTOR - 1) / SECTOR;
    std::string entries;
    int64_t sec = data_sector;
    std::vector<std::pair<int64_t, const std::string *>> placed;
    for (int64_t i = 0; i < n; ++i) {
        const std::string &data = files.items[2 * order[size_t(i)] + 1].s;
        int64_t nb = (int64_t(data.size()) + SECTOR - 1) / SECTOR;
        const std::string &nm = names[size_t(i)];
        int64_t size = (0x0E + int64_t(nm.size()) + 3) & ~int64_t(3);
        if (int64_t(entries.size()) / SECTOR != (int64_t(entries.size()) + size - 1) / SECTOR)
            entries.append(size_t(floormod(-int64_t(entries.size()), SECTOR)), '\xff');
        int64_t here = int64_t(entries.size());
        int64_t right = i == n - 1 ? 0xFFFF : (here + size) / 4;
        std::string e;
        auto p16 = [&](int64_t v) { e += char(v & 0xFF); e += char((v >> 8) & 0xFF); };
        auto p32 = [&](int64_t v) { p16(v & 0xFFFF); p16((v >> 16) & 0xFFFF); };
        p16(0xFFFF); p16(right); p32(sec); p32(int64_t(data.size()));
        e += char(0x20);
        e += char(nm.size());
        e += nm;
        e.append(size_t((-int64_t(e.size())) & 3), '\xff');
        entries += e;
        placed.push_back({sec, &data});
        sec += nb;
    }
    const int64_t root_size = int64_t(entries.size());
    std::string vol(size_t(sec * SECTOR), '\0');
    const int64_t vd = 32 * SECTOR;
    std::memcpy(&vol[size_t(vd)], MAGIC, 20);
    for (int k = 0; k < 4; ++k) {
        vol[size_t(vd + 0x14 + k)] = char((root_sector >> (8 * k)) & 0xFF);
        vol[size_t(vd + 0x18 + k)] = char((root_size >> (8 * k)) & 0xFF);
    }
    std::memcpy(&vol[size_t(vd + 0x7EC)], MAGIC, 20);
    if (!entries.empty()) std::memcpy(&vol[size_t(root_sector * SECTOR)], entries.data(), entries.size());
    for (auto &pl : placed)
        if (!pl.second->empty()) std::memcpy(&vol[size_t(pl.first * SECTOR)], pl.second->data(), pl.second->size());
    c.out.bytes(Bytes(vol));
}

// ---- GoD writing (god.py) -------------------------------------------------------------------------
std::string sha1(const void *p, size_t n) {
    auto d = xex::crypto::Sha1::hash(p, n);
    return std::string(reinterpret_cast<const char *>(d.data()), d.size());
}

// god.part(blocks) -> (the part file's bytes, sha1 of its master hash block); `blocks`: the volume's
// blocks of this part, each already padded to 0x1000
void god_part(Call &c) {
    const std::string &vol = c.arg(0).s;
    const int64_t total = int64_t(vol.size()) / BLOCK;
    std::string master(size_t(BLOCK), '\0'), body;
    for (int64_t sidx = 0; sidx < SUBPARTS_PER_PART; ++sidx) {
        int64_t base_block = sidx * BLOCKS_PER_SUBPART;
        if (base_block >= total) break;
        std::string sub_hash(size_t(BLOCK), '\0'), blocks;
        int64_t filled = 0;
        for (int64_t k = 0; k < BLOCKS_PER_SUBPART; ++k) {
            int64_t g = base_block + k;
            if (g >= total) break;
            std::string h = sha1(vol.data() + g * BLOCK, size_t(BLOCK));
            std::memcpy(&sub_hash[size_t(k * 20)], h.data(), 20);
            blocks.append(vol, size_t(g * BLOCK), size_t(BLOCK));
            ++filled;
        }
        std::string mh = sha1(sub_hash.data(), sub_hash.size());
        std::memcpy(&master[size_t(sidx * 20)], mh.data(), 20);
        body += sub_hash;
        body += blocks;
        if (filled < BLOCKS_PER_SUBPART) break;
    }
    Out &o = c.out;
    o.begin_tuple();
    o.bytes(Bytes(master + body));
    o.bytes(Bytes(sha1(master.data(), master.size())));
    o.end_tuple();
}

void put_be(std::string &b, int64_t at, int64_t v, int width, const char *what) {
    if (v < 0 || (width < 8 && v >= (int64_t(1) << (8 * width)))) raise("struct.error", std::string("argument out of range"));
    if (int64_t(b.size()) < at + width) raise("struct.error", std::string("pack_into requires a buffer of at least ") + dec(at + width) + " bytes");
    (void)what;
    for (int k = 0; k < width; ++k) b[size_t(at + k)] = char((v >> (8 * (width - 1 - k))) & 0xFF);
}
void put_le(std::string &b, int64_t at, int64_t v, int width) {
    if (v < 0 || (width < 8 && v >= (int64_t(1) << (8 * width)))) raise("struct.error", "argument out of range");
    if (int64_t(b.size()) < at + width) raise("struct.error", std::string("pack_into requires a buffer of at least ") + dec(at + width) + " bytes");
    for (int k = 0; k < width; ++k) b[size_t(at + k)] = char((v >> (8 * k)) & 0xFF);
}

// god.con_header(template, title_id, media_id, content_type, blocks_allocated, part_count,
//                parts_total_size, mht_hash, title) -> bytes
void god_con_header(Call &c) {
    std::string b = c.arg(0).s;
    put_be(b, 0x0344, c.int_arg(3), 4, "");
    put_be(b, 0x0354, c.int_arg(2), 4, "");
    put_be(b, 0x0360, c.int_arg(1), 4, "");
    int64_t blocks = c.int_arg(4);
    if (b.size() < 0x395) raise("IndexError", "bytearray index out of range");
    b[0x0392] = char((blocks >> 16) & 0xFF);
    b[0x0393] = char((blocks >> 8) & 0xFF);
    b[0x0394] = char(blocks & 0xFF);
    put_be(b, 0x0395, 0, 2, "");
    put_le(b, 0x03A0, c.int_arg(5), 4);
    put_be(b, 0x03A4, c.int_arg(6) / 0x100, 4, "");
    if (c.arg(7).kind != Value::Bytes) raise("TypeError", "can assign only bytes, buffers, or iterables of ints in range(0, 256)");
    slice_assign(b, 0x037D, 0x037D + 20, c.arg(7).s);
    std::string title = c.str_arg(8);
    if (!title.empty()) {
        std::string t;
        for (uint32_t cp : codepoints(title)) {
            if (cp >= 0xD800 && cp < 0xE000) raise("UnicodeEncodeError", "surrogates not allowed");
            if (cp >= 0x10000) { cp -= 0x10000; uint32_t hi = 0xD800 + (cp >> 10), lo = 0xDC00 + (cp & 0x3FF); t += char(hi >> 8); t += char(hi); t += char(lo >> 8); t += char(lo); }
            else { t += char(cp >> 8); t += char(cp); }
        }
        if (t.size() > 0x7E) t.resize(0x7E);
        slice_assign(b, 0x0411, 0x0411 + int64_t(t.size()), t);
        slice_assign(b, 0x1691, 0x1691 + int64_t(t.size()), t);
    }
    if (b.size() <= 0x0391) raise("IndexError", "bytearray index out of range");
    b[0x035B] = 0; b[0x035F] = 0; b[0x0391] = 0;
    std::string h = sha1(b.data() + 0x0344, std::min<size_t>(0xACBC, b.size() - 0x0344));
    b.replace(0x032C, 20, h);
    c.out.bytes(Bytes(b));
}

// god.install_to_god(data) -> bytes
void god_install(Call &c) {
    if (c.data.size() < 0x413) raise("ValueError", "not an STFS package header (need >= 0x413 bytes)");
    std::string b = c.data.str();
    std::memcpy(&b[0], "LIVE", 4);
    for (int i = 0x22C; i < 0x234; ++i) b[size_t(i)] = char(0xFF);
    b[0x340] = 0; b[0x341] = 0; b[0x342] = char(0xAD); b[0x343] = 0x0E; b[0x346] = 0x70;
    b[0x379] = 0x24; b[0x37A] = 0x05; b[0x37B] = 0x05; b[0x37C] = 0x11;
    for (int i = 0x3FD; i < 0x412; ++i) b[size_t(i)] = 0;
    std::string h = sha1(b.data() + 0x344, std::min<size_t>(0xACBC, b.size() - 0x344));
    std::memcpy(&b[0x32C], h.data(), 20);
    c.out.bytes(Bytes(b));
}

// ---- CSO / CCI writing (ciso.py) -----------------------------------------------------------------------
struct Out2 {
    FILE *f = nullptr;
    explicit Out2(const std::string &path) {
#ifdef _WIN32
        std::wstring w;
        for (uint32_t cp : codepoints(path)) {
            if (cp >= 0x10000) { cp -= 0x10000; w += wchar_t(0xD800 + (cp >> 10)); w += wchar_t(0xDC00 + (cp & 0x3FF)); }
            else w += wchar_t(cp);
        }
        f = _wfopen(w.c_str(), L"wb");
#else
        f = std::fopen(path.c_str(), "wb");
#endif
        if (!f) raise("OSError", "[Errno 2] No such file or directory: '" + path + "'");
    }
    ~Out2() { if (f) std::fclose(f); }
    void write(const std::string &s) { if (!s.empty()) std::fwrite(s.data(), 1, s.size(), f); }
    int64_t tell() { return tell64(f); }
    void seek(int64_t at) { seek64(f, at); }
};

void le(std::string &s, uint64_t v, int w) { for (int k = 0; k < w; ++k) s += char((v >> (8 * k)) & 0xFF); }

std::string index_bytes(const std::vector<uint32_t> &e) {
    std::string s;
    for (uint32_t v : e) le(s, v, 4);
    return s;
}

std::string read_block(File &src) {
    std::string raw = src.read(BLOCK_SIZE);
    raw.append(size_t(BLOCK_SIZE - int64_t(raw.size())), '\0');
    return raw;
}

void write_cso(Call &c) {
    std::string iso = c.str_arg(0), outp = c.str_arg(1);
    uint64_t sz;
    if (!file_size(iso, sz)) raise_open_error(iso);
    int64_t usize = (int64_t(sz) + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE, n = usize / BLOCK_SIZE;
    File src(iso);
    Out2 out(outp);
    std::string head = "CISO";
    le(head, 0x18, 4); le(head, uint64_t(usize), 8); le(head, BLOCK_SIZE, 4); le(head, 2, 1); le(head, 2, 1); le(head, 0, 2);
    out.write(head);
    out.write(std::string(size_t((n + 1) * 4), '\0'));
    std::vector<uint32_t> entries;
    for (int64_t b = 0; b < n; ++b) {
        std::string raw = read_block(src);
        int64_t pos = out.tell();
        std::string comp = lz4_compress(Bytes(raw));
        if (int64_t(comp.size()) + 4 < BLOCK_SIZE - 12) {
            std::string w;
            le(w, comp.size(), 4);
            out.write(w + comp);
            entries.push_back(uint32_t((pos >> 2) | 0x80000000));
        } else {
            out.write(raw);
            entries.push_back(uint32_t(pos >> 2));
        }
        int64_t pad = floormod(-out.tell(), 4);
        if (pad) out.write(std::string(size_t(pad), '\0'));
    }
    entries.push_back(uint32_t(out.tell() >> 2));
    out.seek(0x18);
    out.write(index_bytes(entries));
    c.out.integer(usize);
}

void write_cci(Call &c) {
    std::string iso = c.str_arg(0), outp = c.str_arg(1);
    uint64_t sz;
    if (!file_size(iso, sz)) raise_open_error(iso);
    int64_t usize = (int64_t(sz) + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE, n = usize / BLOCK_SIZE;
    File src(iso);
    Out2 out(outp);
    auto header = [&](int64_t ioff) {
        std::string h = "CCIM";
        le(h, 32, 4); le(h, uint64_t(usize), 8); le(h, uint64_t(ioff), 8); le(h, BLOCK_SIZE, 4); le(h, 1, 1); le(h, 2, 1); le(h, 0, 2);
        return h;
    };
    out.write(header(0));
    std::vector<uint32_t> entries;
    for (int64_t b = 0; b < n; ++b) {
        std::string raw = read_block(src);
        int64_t pos = out.tell();
        std::string comp = lz4_compress(Bytes(raw));
        if (int64_t(comp.size()) < 2040) {
            int64_t total = (1 + int64_t(comp.size()) + 3) & ~int64_t(3), pad = total - 1 - int64_t(comp.size());
            out.write(std::string(1, char(pad)) + comp + std::string(size_t(pad), '\0'));
            entries.push_back(uint32_t((pos >> 2) | 0x80000000));
        } else {
            out.write(raw);
            entries.push_back(uint32_t(pos >> 2));
        }
    }
    int64_t index_offset = out.tell();
    entries.push_back(uint32_t(index_offset >> 2));
    out.write(index_bytes(entries));
    out.seek(0);
    out.write(header(index_offset));
    c.out.integer(usize);
}

void to_iso(Call &c) {
    std::string image = c.str_arg(0), outp = c.str_arg(1);
    std::string magic;
    {
        File f(image);
        magic = f.read(4);
    }
    bool cci = magic == "CCIM";
    if (magic != "CISO" && !cci) raise("CisoError", image + " is not a CSO or CCI image");
    CompDevice r(image, cci);
    Out2 out(outp);
    int64_t off = 0;
    while (off < r.size) {
        std::string chunk = r.read_at(off, 4 * 1024 * 1024);
        if (chunk.empty()) break;
        out.write(chunk);
        off += int64_t(chunk.size());
    }
    c.out.integer(off);
}

void lz4_decode_q(Call &c) { c.out.bytes(Bytes(lz4_decode(c.data, c.int_arg(0)))); }
void lz4_compress_q(Call &c) { c.out.bytes(Bytes(lz4_compress(c.data))); }

}  // namespace

CBF_QUESTIONS(discs) {
    add("xdev.open_file", open_file, "int");
    add("xdev.open_svod", open_svod);
    add("xdev.open_image", open_image);
    add("xdev.read", read_q, "bytes");
    add("xdev.close", close_q);
    add("gdf.open", gdf_open);
    add("gdf.walk", gdf_walk);
    add("gdf.files", gdf_files);
    add("gdf.read", gdf_read, "bytes?");
    add("gdf.build_xdvdfs", build_xdvdfs, "bytes");
    add("god.part", god_part);
    add("god.con_header", god_con_header, "bytes");
    add("god.install_to_god", god_install, "bytes");
    add("ciso.write_cso", write_cso, "int");
    add("ciso.write_cci", write_cci, "int");
    add("ciso.to_iso", to_iso, "int");
    add("lz4.decode_block", lz4_decode_q, "bytes");
    add("lz4.compress_block", lz4_compress_q, "bytes");
}

}  // namespace cbf::xbox
