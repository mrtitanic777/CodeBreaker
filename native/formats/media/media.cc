// media.cc -- floppy, hard-disk and CD images: codebreaker/formats/media.py's FAT and ISO-9660
// readers, the partition table, El Torito, and put() (files written into a FAT volume in place).
//
// The volume objects stay in Python (their attributes are the dicts written here); each method
// asks again with the volume's place (path, offset) and the facts it needs, the FAT itself
// passed as the call's bytes.  Files are read on demand, never the whole image.
#include <cstdio>
#include <ctime>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::media {
namespace {

// bytes of an image file from an offset on, read as asked for (media._Source)
struct Source {
    std::string path;
    int64_t offset = 0;
    FILE *f = nullptr;
    uint64_t size = 0;
    Source(const std::string &p, int64_t off) : path(p), offset(off) {}
    ~Source() {
        if (f) std::fclose(f);
    }
    std::string read(int64_t at, int64_t n) {
        if (!f) {                     // (Python opened the file at each read; once per question here)
            f = open_shared(path);
            if (!f) raise_open_error(path);
            file_size(path, size);
        }
        int64_t pos = offset + at;
        if (pos < 0) raise("OSError", "[Errno 22] Invalid argument");
        if (n <= 0 || static_cast<uint64_t>(pos) >= size) return std::string();
        uint64_t left = size - static_cast<uint64_t>(pos);
        size_t take = static_cast<size_t>(static_cast<uint64_t>(n) < left ? static_cast<uint64_t>(n) : left);
        std::string out(take, '\0');
        _fseeki64(f, pos, SEEK_SET);
        out.resize(std::fread(&out[0], 1, take, f));
        return out;
    }
    static FILE *open_shared(const std::string &path);
};

}  // namespace
}  // namespace cbf::media

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace cbf::media {
namespace {

FILE *Source::open_shared(const std::string &path) {
#ifdef _WIN32
    int n = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    std::wstring w(static_cast<size_t>(n > 0 ? n : 1), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &w[0], n);
    return _wfopen(w.c_str(), L"rb");
#else
    return std::fopen(path.c_str(), "rb");
#endif
}

// ---- text ----------------------------------------------------------------------------------
std::string rstrip_sp0(const std::string &s) {
    size_t n = s.size();
    while (n && (s[n - 1] == ' ' || s[n - 1] == '\0')) --n;
    return s.substr(0, n);
}
std::string text(Bytes raw) { return rstrip_sp0(cp437(raw)); }

bool py_space(uint32_t c) {
    return (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x20) || c == 0x85 || c == 0xA0 || c == 0x1680 ||
           (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}
std::string strip(const std::string &s) {
    std::vector<uint32_t> cps = codepoints(s);
    size_t a = 0, b = cps.size();
    while (a < b && py_space(cps[a])) ++a;
    while (b > a && py_space(cps[b - 1])) --b;
    return from_codepoints(std::vector<uint32_t>(cps.begin() + a, cps.begin() + b));
}
std::string fmt(const char *f, long long a) {
    char buf[64];
    std::snprintf(buf, sizeof buf, f, a);
    return buf;
}
std::string when_text(int64_t y, int64_t mo, int64_t d, int64_t h, int64_t mi) {
    char buf[96];
    std::snprintf(buf, sizeof buf, "%04lld-%02lld-%02lld %02lld:%02lld", (long long)y, (long long)mo, (long long)d,
                  (long long)h, (long long)mi);
    return buf;
}

struct Entry {
    std::string path;
    int64_t size = 0;
    bool is_dir = false;
    int64_t start = 0;
    std::string when, shortname;
    int64_t attributes = 0;
    std::vector<std::pair<int64_t, int64_t>> extents;   // ISO only
    bool iso = false;
};

void write_entry(Out &o, const Entry &e) {
    o.begin_tuple();
    o.text(e.path);
    o.integer(e.size);
    o.boolean(e.is_dir);
    o.integer(e.start);
    o.text(e.when);
    o.text(e.shortname);
    o.integer(e.attributes);
    if (e.iso) {
        o.begin_list();
        for (auto &x : e.extents) {
            o.begin_tuple();
            o.integer(x.first);
            o.integer(x.second);
            o.end_tuple();
        }
        o.end_list();
    }
    o.end_tuple();
}

// ---- FAT -----------------------------------------------------------------------------------
struct OldFloppy {
    int media;
    int64_t spc, roots, spf, total;
};
const OldFloppy kOld[] = {{0xFE, 1, 64, 1, 320}, {0xFF, 2, 112, 1, 640}, {0xFC, 1, 64, 2, 360}, {0xFD, 2, 112, 2, 720}};

struct Fat {
    std::string where;
    bool old = false;
    int64_t bps, spc, roots, fats, spf, fat_at, root_at, data_at, clusters, bits, root_cluster;
    int64_t reserved, total, media, spt, heads, hidden;
    std::string system, serial, label, filesystem, signature, boot_code;
    std::string fat;
};

std::string jump(Bytes b) {
    if (b.at(0) == 0xEB) return fmt("jmp %03llXh", 2 + b.at(1));
    if (b.at(0) == 0xE9) return fmt("jmp %03llXh", (3 + b.u16(1)) & 0xFFFF);
    return "";
}

int64_t floordiv(int64_t a, int64_t b) {
    if (b == 0) raise("ZeroDivisionError", "integer division or modulo by zero");
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

// FatVolume(source, where, size): has_size false for size=None
Fat open_fat(Source &src, const std::string &where, bool has_size, int64_t size) {
    std::string bs = src.read(0, 512);
    if (bs.size() < 512) raise("ValueError", "no boot sector");
    Bytes b(bs);
    int64_t bps = b.u16(11), spc = b.u8(13), reserved = b.u16(14), fats = b.u8(16), roots = b.u16(17),
            total16 = b.u16(19), media = b.u8(21), spf16 = b.u16(22), spt = b.u16(24), heads = b.u16(26),
            hidden = b.u32(28), total32 = b.u32(32);
    bool ok = (bps == 512 || bps == 1024 || bps == 2048 || bps == 4096) && spc && (spc & (spc - 1)) == 0 && 1 <= fats &&
              fats <= 2 && reserved && media >= 0xF0 && (total16 || total32) &&
              (b.at(0) == 0xEB || b.at(0) == 0xE9 || b.at(0) == 0x69);
    Fat v;
    v.where = where;
    if (!ok) {
        std::string f0 = src.read(512, 3);
        Bytes fat0(f0);
        const OldFloppy *hit = nullptr;
        if (fat0.size() == 3)
            for (const OldFloppy &k : kOld)
                if (k.media == fat0.at(0)) hit = &k;
        if (hit && fat0.slice(1).is("\xFF\xFF", 2) && (!has_size || size <= 512 * 720)) {
            spc = hit->spc;
            roots = hit->roots;
            spf16 = hit->spf;
            total16 = hit->total;
            bps = 512, reserved = 1, fats = 2, media = fat0.at(0), spt = 8, heads = 0, hidden = 0, total32 = 0;
            v.old = true;
        } else {
            raise("ValueError", "not a FAT volume");
        }
    }
    int64_t spf = spf16 ? spf16 : b.u32(36);
    int64_t total = total16 ? total16 : total32;
    v.bps = bps, v.spc = spc, v.roots = roots, v.fats = fats, v.spf = spf;
    v.fat_at = reserved * bps;
    v.root_at = v.fat_at + fats * spf * bps;
    v.data_at = v.root_at + floordiv(roots * 32 + bps - 1, bps) * bps;
    v.clusters = floordiv(total * bps - v.data_at, spc * bps);
    v.bits = v.clusters < 4085 ? 12 : v.clusters < 65525 ? 16 : 32;
    v.root_cluster = v.bits == 32 ? b.u32(44) : 0;
    int64_t ext = v.bits == 32 ? 64 : 36;
    bool has_ext = !v.old && (b.at(ext + 2) == 0x28 || b.at(ext + 2) == 0x29);
    v.reserved = reserved, v.total = total, v.media = media, v.spt = spt, v.heads = heads, v.hidden = hidden;
    v.system = v.old ? "(no parameter block)" : text(b.slice(3, 11));
    v.serial = has_ext ? fmt("%08llX", b.u32(ext + 3)) : "";
    v.label = has_ext ? text(b.slice(ext + 7, ext + 18)) : "";
    v.filesystem = has_ext && b.at(ext + 2) == 0x29 ? text(b.slice(ext + 18, ext + 26)) : "";
    char sig[8];
    std::snprintf(sig, sizeof sig, "%02X%02X", b.at(510), b.at(511));
    v.signature = sig;
    v.boot_code = v.old ? "" : jump(b);
    v.fat = src.read(v.fat_at, spf * bps);
    return v;
}

void write_fat(Out &o, const Fat &v) {
    o.begin_dict();
    o.item_text("where", v.where);
    o.key("old");
    o.boolean(v.old);
    o.item("bps", v.bps);
    o.item("spc", v.spc);
    o.item("roots", v.roots);
    o.item("fats", v.fats);
    o.item("spf", v.spf);
    o.item("fat_at", v.fat_at);
    o.item("root_at", v.root_at);
    o.item("data_at", v.data_at);
    o.item("clusters", v.clusters);
    o.item("bits", v.bits);
    o.item_text("kind", "fat" + std::to_string(v.bits));
    o.item("root_cluster", v.root_cluster);
    o.key("boot");
    o.begin_dict();
    o.item_text("system", v.system);
    o.item("bytes per sector", v.bps);
    o.item("sectors per cluster", v.spc);
    o.item("reserved sectors", v.reserved);
    o.item("fats", v.fats);
    o.item("root entries", v.roots);
    o.item("total sectors", v.total);
    o.item_text("media", fmt("%02llX", v.media));
    o.item("sectors per fat", v.spf);
    o.item("sectors per track", v.spt);
    o.item("heads", v.heads);
    o.item("hidden sectors", v.hidden);
    o.item("clusters", v.clusters);
    o.item_text("serial", v.serial);
    o.item_text("label", v.label);
    o.item_text("file system", v.filesystem);
    o.item_text("signature", v.signature);
    o.item_text("boot code", v.boot_code);
    o.end_dict();
    o.key("fat");
    o.bytes(Bytes(v.fat));
    o.item_text("label", v.label);
    o.end_dict();
}

int64_t fat_next(Bytes f, int64_t bits, int64_t c) {
    if (bits == 12) {
        if (c * 3 / 2 + 2 > f.size()) return 0xFFF;
        int64_t v = f.at(c * 3 / 2) | (f.at(c * 3 / 2 + 1) << 8);
        return c & 1 ? v >> 4 : v & 0xFFF;
    }
    if (bits == 16) return c * 2 + 2 <= f.size() ? f.u16(c * 2) : 0xFFFF;
    return c * 4 + 4 <= f.size() ? (f.u32(c * 4) & 0x0FFFFFFF) : 0x0FFFFFFF;
}

struct FatView {             // what a FAT volume's reading needs
    Bytes fat;
    int64_t bits, spc, bps, data_at;
};

std::string chain(Source &src, const FatView &v, int64_t cluster, int64_t size /* -1: all */) {
    int64_t last = (v.bits < 32 ? (int64_t(1) << v.bits) : (int64_t(1) << 28)) - 16;
    int64_t step = v.spc * v.bps, got = 0;
    std::vector<int64_t> seen;
    std::string data;
    auto was = [&](int64_t c) {
        for (int64_t s : seen)
            if (s == c) return true;
        return false;
    };
    while (2 <= cluster && cluster < last && !was(cluster) && (size < 0 || got < size)) {
        seen.push_back(cluster);
        data += src.read(v.data_at + (cluster - 2) * step, step);
        got += step;
        cluster = fat_next(v.fat, v.bits, cluster);
    }
    if (size >= 0 && static_cast<int64_t>(data.size()) > size) data.resize(static_cast<size_t>(size));
    return data;
}

std::vector<Entry> fat_folder(Bytes raw, const std::string &prefix, const std::string &short_prefix, int64_t bits,
                              std::string &label) {
    std::vector<std::string> long_parts;   // the last piece first
    std::vector<Entry> out;
    for (int64_t i = 0; i < raw.size() - 31; i += 32) {
        Bytes e = raw.slice(i, i + 32);
        if (e.at(0) == 0) break;
        int64_t attr = e.at(11);
        if (e.at(0) == 0xE5) {
            long_parts.clear();
            continue;
        }
        if ((attr & 0x3F) == 0x0F) {
            if (e.at(0) & 0x40) long_parts.clear();
            long_parts.insert(long_parts.begin(), e.slice(1, 11).str() + e.slice(14, 26).str() + e.slice(28, 32).str());
            continue;
        }
        std::string first(1, static_cast<char>(e.at(0) == 5 ? 0xE5 : e.at(0)));
        std::string base = text(Bytes(first + e.slice(1, 8).str())), ext = text(e.slice(8, 11));
        std::string shortname = base + (ext.empty() ? "" : "." + ext);
        std::string name = shortname;
        if (!long_parts.empty()) {
            std::string joined;
            for (auto &p : long_parts) joined += p;
            name = utf16le(Bytes(joined));
            size_t z = name.find('\0');
            if (z != std::string::npos) name = name.substr(0, z);
        }
        long_parts.clear();
        if (attr & 0x08) {
            if (label.empty()) label = text(e.slice(0, 11));
            continue;
        }
        if (shortname == "." || shortname == "..") continue;
        int64_t time_ = e.u16(22), date = e.u16(24);
        Entry x;
        x.path = prefix + name;
        x.size = e.u32(28);
        x.is_dir = (attr & 0x10) != 0;
        x.start = e.u16(26) | (bits == 32 ? (static_cast<int64_t>(e.u16(20)) << 16) : 0);
        x.when = when_text(1980 + (date >> 9), (date >> 5) & 15, date & 31, time_ >> 11, (time_ >> 5) & 63);
        x.shortname = short_prefix + shortname;
        x.attributes = attr;
        out.push_back(x);
    }
    return out;
}

FatView view_of(Call &c, size_t k) {   // args k..: bits, spc, bps, data_at
    return FatView{c.data, c.int_arg(k), c.int_arg(k + 1), c.int_arg(k + 2), c.int_arg(k + 3)};
}

// (fat) path, offset, bits, spc, bps, data_at, root_at, roots, root_cluster, label -> (entries, label)
void fat_files_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    FatView v = view_of(c, 2);
    int64_t root_at = c.int_arg(6), roots = c.int_arg(7), root_cluster = c.int_arg(8);
    std::string label = c.str_arg(9);
    std::string root = v.bits == 32 ? chain(src, v, root_cluster, -1) : src.read(root_at, roots * 32);
    std::vector<Entry> out, work = fat_folder(Bytes(root), "", "", v.bits, label);
    std::vector<int64_t> seen;
    size_t head = 0;
    while (head < work.size()) {
        Entry e = work[head];
        work.erase(work.begin() + static_cast<long>(head));
        out.push_back(e);
        bool was = false;
        for (int64_t s : seen)
            if (s == e.start) was = true;
        if (e.is_dir && !was && seen.size() < 20000) {
            seen.push_back(e.start);
            std::string sub = chain(src, v, e.start, -1);
            std::vector<Entry> kids = fat_folder(Bytes(sub), e.path + "/", e.shortname + "/", v.bits, label);
            work.insert(work.begin(), kids.begin(), kids.end());
        }
    }
    c.out.begin_tuple();
    c.out.begin_list();
    for (auto &e : out) write_entry(c.out, e);
    c.out.end_list();
    c.out.text(label);
    c.out.end_tuple();
}

// (fat) path, offset, bits, spc, bps, data_at, start, size -> bytes
void fat_read_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    FatView v = view_of(c, 2);
    c.out.bytes(Bytes(chain(src, v, c.int_arg(6), c.arg(7).is_none() ? -1 : c.int_arg(7))));
}

// (fat) bits, cluster -> the next cluster of its chain (FatVolume._next)
void fat_next_q(Call &c) { c.out.integer(fat_next(c.data, c.int_arg(0), c.int_arg(1))); }

// ---- ISO 9660 ------------------------------------------------------------------------------
struct Iso {
    int64_t sector, skip;
    bool boot_catalog_set = false;
    int64_t boot_catalog = 0;
    bool long_names = false;
    int64_t block = 2048, root_block = 0, root_size = 0;
    std::string label, system, plabel, created;
    int64_t blocks = 0;
    bool joliet = false;
};

std::string sectors(Source &src, int64_t sector, int64_t skip, int64_t n, int64_t count) {
    if (sector == 2048) return src.read(n * 2048, count * 2048);
    std::string out;
    for (int64_t i = 0; i < count; ++i) out += src.read((n + i) * sector + skip, 2048);
    return out;
}

std::string iso_bytes(Source &src, int64_t sector, int64_t skip, int64_t block, int64_t size) {
    std::string s = sectors(src, sector, skip, block, floordiv(size + 2047, 2048));
    Bytes b(s);
    return b.slice(0, size).str();
}

Iso open_iso(Source &src, int64_t sector, int64_t skip, bool long_names) {
    Iso v;
    v.sector = sector, v.skip = skip;
    std::string primary, joliet;
    bool have_p = false, have_j = false;
    for (int64_t n = 16; n < 48; ++n) {
        std::string ds = sectors(src, sector, skip, n, 1);
        Bytes d(ds);
        if (d.size() < 2048 || !d.slice_is(1, 6, "CD001", 5) || d.at(0) == 255) break;
        if (d.at(0) == 1 && !have_p) {
            primary = ds;
            have_p = true;
        } else if (d.at(0) == 2 && (d.slice_is(88, 91, "%/@", 3) || d.slice_is(88, 91, "%/C", 3) || d.slice_is(88, 91, "%/E", 3))) {
            joliet = ds;
            have_j = true;
        } else if (d.at(0) == 0 && d.slice_is(7, 30, "EL TORITO SPECIFICATION", 23)) {
            v.boot_catalog = d.u32(71);
            v.boot_catalog_set = true;
        }
    }
    if (!have_p) raise("ValueError", "not an ISO-9660 volume");
    v.joliet = have_j;
    v.long_names = have_j && long_names;
    Bytes p(primary), d(v.long_names ? joliet : primary);
    v.block = p.u16(128) ? p.u16(128) : 2048;
    v.label = strip(v.long_names ? utf16be(d.slice(40, 72)) : text(d.slice(40, 72)));
    v.root_block = d.u32(158);
    v.root_size = d.u32(166);
    v.system = text(p.slice(8, 40));
    v.plabel = text(p.slice(40, 72));
    v.blocks = p.u32(80);
    v.created = text(p.slice(813, 829));
    return v;
}

void write_iso(Out &o, const Iso &v) {
    o.begin_dict();
    o.item("sector", v.sector);
    o.item("skip", v.skip);
    o.item_text("where", "");
    o.key("boot_catalog");
    if (v.boot_catalog_set) o.integer(v.boot_catalog);
    else o.none();
    o.key("long_names");
    o.boolean(v.long_names);
    o.item("block", v.block);
    o.item_text("label", v.label);
    o.key("root");
    o.begin_tuple();
    o.integer(v.root_block);
    o.integer(v.root_size);
    o.end_tuple();
    o.key("boot");
    o.begin_dict();
    o.item_text("system", v.system);
    o.item_text("label", v.plabel);
    o.item("blocks", v.blocks);
    o.item("block size", v.block);
    o.item_text("created", v.created);
    o.item_text("long names", v.joliet ? "yes" : "no");
    o.item_text("bootable", v.boot_catalog_set && v.boot_catalog ? "yes" : "no");
    o.end_dict();
    o.end_dict();
}

// path, offset, sector, skip, long_names, root_block, root_size -> [entries]
void iso_files_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    int64_t sector = c.int_arg(2), skip = c.int_arg(3);
    bool long_names = c.bool_arg(4);
    struct Work {
        int64_t block, size;
        std::string prefix;
    };
    std::vector<Work> work{{c.int_arg(5), c.int_arg(6), ""}};
    std::vector<Entry> out;
    std::vector<int64_t> seen;
    size_t head = 0;
    while (head < work.size()) {
        Work w = work[head++];
        bool was = false;
        for (int64_t s : seen)
            if (s == w.block) was = true;
        if (was || seen.size() > 50000) continue;
        seen.push_back(w.block);
        std::string ds = iso_bytes(src, sector, skip, w.block, w.size);
        Bytes d(ds);
        int64_t i = 0;
        std::vector<Entry> found;
        while (i < d.size()) {
            int64_t n = d.at(i);
            if (n == 0) {
                i = (i / 2048 + 1) * 2048;
                continue;
            }
            Bytes r = d.slice(i, i + n);
            i += n;
            Bytes raw = r.slice(33, 33 + r.at(32));
            if (raw.is("\0", 1) || raw.is("\1", 1) || r.size() < 34) continue;
            std::string name = long_names ? utf16be(raw) : latin1(raw);
            size_t semi = name.find(';');
            if (semi != std::string::npos) name = name.substr(0, semi);
            if (!name.empty() && name.back() == '.') name.pop_back();
            Entry e;
            e.iso = true;
            e.path = w.prefix + name;
            e.size = r.u32(10);
            e.is_dir = (r.at(25) & 2) != 0;
            e.start = static_cast<int64_t>(r.u32(2)) + r.at(1);
            e.when = when_text(1900 + r.at(18), r.at(19), r.at(20), r.at(21), r.at(22));
            e.shortname = e.path;
            if (!found.empty() && found.back().path == e.path && !e.is_dir) {
                found.back().extents.emplace_back(e.start, e.size);
                found.back().size += e.size;
                continue;
            }
            e.extents.emplace_back(e.start, e.size);
            found.push_back(e);
        }
        for (auto &e : found) {
            out.push_back(e);
            if (e.is_dir) work.push_back({e.start, e.size, e.path + "/"});
        }
    }
    // folders before what is in them
    std::vector<std::pair<std::string, std::vector<size_t>>> order;
    for (size_t k = 0; k < out.size(); ++k) {
        const std::string &p = out[k].path;
        size_t slash = p.rfind('/');
        std::string parent = slash == std::string::npos ? "" : p.substr(0, slash);
        bool done = false;
        for (auto &kv : order)
            if (kv.first == parent) {
                kv.second.push_back(k);
                done = true;
                break;
            }
        if (!done) order.push_back({parent, {k}});
    }
    std::vector<size_t> result;
    std::function<void(const std::string &, int)> walk = [&](const std::string &prefix, int depth) {
        if (depth > 990) raise("RecursionError", "maximum recursion depth exceeded");
        for (auto &kv : order)
            if (kv.first == prefix) {
                for (size_t k : kv.second) {
                    result.push_back(k);
                    if (out[k].is_dir) walk(out[k].path, depth + 1);
                }
                break;
            }
    };
    walk("", 0);
    c.out.begin_list();
    for (size_t k : result) write_entry(c.out, out[k]);
    c.out.end_list();
}

// path, offset, sector, skip, [(block, size)] -> bytes
void iso_read_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    std::string out;
    for (const Value &x : c.arg(4).items) out += iso_bytes(src, c.int_arg(2), c.int_arg(3), x[0].as_int(), x[1].as_int());
    c.out.bytes(Bytes(out));
}

struct Boot {
    bool any = false;
    int64_t at = 0, size = 0, media = 0;
};
Boot boot_image(Source &src, int64_t sector, int64_t skip, bool set, int64_t catalog) {
    Boot b;
    if (!set || !catalog) return b;
    std::string cs = sectors(src, sector, skip, catalog, 1);
    Bytes c(cs);
    if (c.at(0) != 1 || !c.slice_is(30, 32, "\x55\xAA", 2) || c.at(32) != 0x88) return b;
    int64_t media = c.at(33) & 15, count = c.u16(38), block = c.u32(40);
    b.any = true;
    b.size = media == 1 ? 1228800 : media == 2 ? 1474560 : media == 3 ? 2949120 : count * 512;
    b.at = block * sector + skip;
    b.media = media;
    return b;
}

// path, offset, sector, skip, boot_catalog -> (offset, size, media) or None
void iso_boot_image_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    Boot b = boot_image(src, c.int_arg(2), c.int_arg(3), !c.arg(4).is_none(), c.int_arg(4));
    if (!b.any) {
        c.out.none();
        return;
    }
    c.out.begin_tuple();
    c.out.integer(b.at);
    c.out.integer(b.size);
    c.out.integer(b.media);
    c.out.end_tuple();
}

// ---- partitions, and the volumes of an image -----------------------------------------------
struct Part {
    int64_t first, count, kind;
};
std::vector<Part> partitions(Source &src, int64_t size) {
    static const int kFat[] = {0x01, 0x04, 0x06, 0x0B, 0x0C, 0x0E, 0x11, 0x14, 0x16, 0x1B, 0x1C, 0x1E};
    std::vector<Part> out;
    std::vector<int64_t> tables{0}, seen;
    bool have_ext = false;
    int64_t extended_base = 0;
    while (!tables.empty()) {
        int64_t at = tables.front();
        tables.erase(tables.begin());
        bool was = false;
        for (int64_t s : seen)
            if (s == at) was = true;
        if (was || seen.size() > 64) continue;
        seen.push_back(at);
        std::string bs = src.read(at * 512, 512);
        Bytes b(bs);
        if (b.size() < 512 || !b.slice_is(510, 512, "\x55\xAA", 2)) continue;
        for (int i = 0; i < 4; ++i) {
            int64_t kind = b.at(446 + 16 * i + 4);
            int64_t first = b.u32(446 + 16 * i + 8), count = b.u32(446 + 16 * i + 12);
            if (!kind || !count) continue;
            if (kind == 0x05 || kind == 0x0F || kind == 0x85) {
                if (!have_ext) {
                    have_ext = true;
                    extended_base = first;
                    tables.push_back(first);
                } else {
                    tables.push_back(extended_base + first);
                }
            } else {
                bool fat = false;
                for (int k : kFat)
                    if (k == kind) fat = true;
                if (fat && (at + first) * 512 < size) out.push_back({at + first, count, kind});
            }
        }
    }
    return out;
}

// path, long_names -> [("iso" | "fat", offset, attrs)]; [] when nothing here is recognised
void volumes_q(Call &c) {
    const std::string path = c.str_arg(0);
    bool long_names = c.bool_arg(1, true);
    uint64_t usize;
    if (!file_size(path, usize)) raise_open_error(path);
    int64_t size = static_cast<int64_t>(usize);
    Source src(path, 0);
    Out &o = c.out;
    const int64_t kinds[3][2] = {{2048, 0}, {2352, 16}, {2352, 24}};
    for (auto &k : kinds) {
        if (src.read(16 * k[0] + k[1] + 1, 5) == "CD001") {
            Iso iso = open_iso(src, k[0], k[1], long_names);
            o.begin_list();
            o.begin_tuple();
            o.text("iso");
            o.integer(0);
            write_iso(o, iso);
            o.end_tuple();
            Boot b = boot_image(src, k[0], k[1], iso.boot_catalog_set, iso.boot_catalog);
            if (b.any && b.media >= 1 && b.media <= 3 && k[0] == 2048) {
                Source inner(path, b.at);
                try {
                    Fat v = open_fat(inner, "boot floppy inside the disc", true, b.size);
                    o.begin_tuple();
                    o.text("fat");
                    o.integer(b.at);
                    write_fat(o, v);
                    o.end_tuple();
                } catch (const PyError &e) {
                    if (e.kind != "ValueError") throw;
                }
            }
            o.end_list();
            return;
        }
    }
    try {
        Fat v = open_fat(src, "", true, size);
        o.begin_list();
        o.begin_tuple();
        o.text("fat");
        o.integer(0);
        write_fat(o, v);
        o.end_tuple();
        o.end_list();
        return;
    } catch (const PyError &e) {
        if (e.kind != "ValueError") throw;
    }
    o.begin_list();
    for (const Part &p : partitions(src, size)) {
        Source inner(path, p.first * 512);
        try {
            char where[96];
            std::snprintf(where, sizeof where, "partition at sector %lld, type %02llX", (long long)p.first, (long long)p.kind);
            Fat v = open_fat(inner, where, true, p.count * 512);
            o.begin_tuple();
            o.text("fat");
            o.integer(p.first * 512);
            write_fat(o, v);
            o.end_tuple();
        } catch (const PyError &e) {
            if (e.kind != "ValueError") throw;
        }
    }
    o.end_list();
}

// FatVolume(source, where, size): path, offset, where, size or None -> attrs
void fat_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    Fat v = open_fat(src, c.str_arg(2), !c.arg(3).is_none(), c.int_arg(3));
    write_fat(c.out, v);
}

// IsoVolume(source, sector, skip, long_names): path, offset, sector, skip, long_names -> attrs
void iso_q(Call &c) {
    Source src(c.str_arg(0), c.int_arg(1));
    write_iso(c.out, open_iso(src, c.int_arg(2), c.int_arg(3), c.bool_arg(4, true)));
}

// ---- writing into a FAT volume -------------------------------------------------------------
struct Writer {
    Fat v;
    int64_t base, step, end, free_from = 2;
    std::string fat;
    RwFile f;
    Writer(const std::string &path, const Fat &vol, int64_t offset)
        : v(vol), base(offset), step(vol.spc * vol.bps),
          end(vol.bits == 12 ? 0xFFF : vol.bits == 16 ? 0xFFFF : 0x0FFFFFFF), fat(vol.fat), f(path) {
        if (!f.ok()) raise_open_error(path);
    }
    void close() {
        for (int64_t i = 0; i < v.fats; ++i) f.write(static_cast<uint64_t>(base + v.fat_at + i * v.spf * v.bps), fat);
        f.close();
    }
    int64_t get(int64_t c) { return fat_next(Bytes(fat), v.bits, c); }
    uint8_t &fb(int64_t i) {
        if (i < 0) i += static_cast<int64_t>(fat.size());
        if (i < 0 || i >= static_cast<int64_t>(fat.size())) raise("IndexError", "bytearray index out of range");
        return reinterpret_cast<uint8_t &>(fat[static_cast<size_t>(i)]);
    }
    void pack(int64_t at, uint64_t value, int width) {
        if (at < 0 || at + width > static_cast<int64_t>(fat.size()))
            raise("struct.error", "pack_into requires a buffer of at least " + std::to_string(at + width) + " bytes");
        for (int k = 0; k < width; ++k) fat[static_cast<size_t>(at + k)] = static_cast<char>(value >> (8 * k));
    }
    void set(int64_t c, int64_t value) {
        if (v.bits == 12) {
            int64_t at = c * 3 / 2;
            int64_t old = fb(at) | (fb(at + 1) << 8);
            int64_t nv = c & 1 ? (old & 0x000F) | ((value & 0xFFF) << 4) : (old & 0xF000) | (value & 0xFFF);
            fb(at) = static_cast<uint8_t>(nv & 0xFF);
            fb(at + 1) = static_cast<uint8_t>(nv >> 8);
        } else if (v.bits == 16) {
            pack(c * 2, static_cast<uint64_t>(value & 0xFFFF), 2);
        } else {
            uint64_t keep = Bytes(fat).u32(c * 4) & 0xF0000000u;
            pack(c * 4, keep | (static_cast<uint64_t>(value) & 0x0FFFFFFF), 4);
        }
    }
    std::vector<int64_t> chain_of(int64_t c) {
        std::vector<int64_t> out;
        while (2 <= c && c < end - 7) {
            bool was = false;
            for (int64_t s : out)
                if (s == c) was = true;
            if (was) break;
            out.push_back(c);
            c = get(c);
        }
        return out;
    }
    std::vector<int64_t> allocate(int64_t n) {
        std::vector<int64_t> got;
        int64_t c = free_from;
        while (static_cast<int64_t>(got.size()) < n) {
            if (c >= v.clusters + 2) raise("ValueError", "the volume is full");
            if (get(c) == 0) got.push_back(c);
            ++c;
        }
        free_from = c;
        for (size_t i = 0; i < got.size(); ++i) set(got[i], i + 1 < got.size() ? got[i + 1] : end);
        return got;
    }
    void write_clusters(const std::vector<int64_t> &ch, const std::string &data) {
        for (size_t i = 0; i < ch.size(); ++i) {
            Bytes piece = Bytes(data).slice(static_cast<int64_t>(i) * step, static_cast<int64_t>(i + 1) * step);
            std::string s = piece.str();
            s.resize(static_cast<size_t>(step > static_cast<int64_t>(s.size()) ? step : static_cast<int64_t>(s.size())), '\0');
            f.write(static_cast<uint64_t>(base + v.data_at + (ch[i] - 2) * step), s);
        }
    }
    std::vector<std::pair<int64_t, std::string>> slots(int64_t start) {
        std::vector<int64_t> at;
        if (start == 0 && v.bits != 32) {
            for (int64_t i = 0; i < v.roots; ++i) at.push_back(base + v.root_at + i * 32);
        } else {
            for (int64_t c : chain_of(start ? start : v.root_cluster))
                for (int64_t i = 0; i < step; i += 32) at.push_back(base + v.data_at + (c - 2) * step + i);
        }
        std::vector<std::pair<int64_t, std::string>> out;
        for (int64_t a : at) out.emplace_back(a, f.read(static_cast<uint64_t>(a), 32));
        return out;
    }
    bool find(int64_t start, const std::string &shortname, int64_t &at, std::string &entry) {
        for (auto &s : slots(start)) {
            Bytes e(s.second);
            if (e.at(0) == 0) break;
            if (e.at(0) != 0xE5 && (e.at(11) & 0x3F) != 0x0F && !(e.at(11) & 0x08) && e.slice(0, 11).str() == shortname) {
                at = s.first;
                entry = s.second;
                return true;
            }
        }
        return false;
    }
    int64_t new_slot(int64_t start) {
        for (auto &s : slots(start)) {
            Bytes e(s.second);
            if (e.at(0) == 0 || e.at(0) == 0xE5) return s.first;
        }
        if (start == 0 && v.bits != 32) raise("ValueError", "the root folder is full");
        int64_t c = allocate(1)[0];
        std::vector<int64_t> ch = chain_of(start ? start : v.root_cluster);
        set(py_at(ch, -1), c);
        write_clusters({c}, "");
        return base + v.data_at + (c - 2) * step;
    }
    int64_t start_of(const std::string &e) {
        Bytes b(e);
        return b.u16(26) | (v.bits == 32 ? (static_cast<int64_t>(b.u16(20)) << 16) : 0);
    }
    void set_start(std::string &e, int64_t cluster) {
        e[26] = static_cast<char>(cluster & 0xFF);
        e[27] = static_cast<char>((cluster >> 8) & 0xFF);
        if (v.bits == 32) {
            if (cluster >> 16 > 0xFFFF) raise("struct.error", "ushort format requires 0 <= number <= 0xffff");
            e[20] = static_cast<char>((cluster >> 16) & 0xFF);
            e[21] = static_cast<char>((cluster >> 24) & 0xFF);
        }
    }
    static void put16(std::string &e, size_t at, int64_t v) {
        e[at] = static_cast<char>(v & 0xFF);
        e[at + 1] = static_cast<char>((v >> 8) & 0xFF);
    }
    int64_t mkdir(int64_t folder, const std::string &shortname, int64_t t, int64_t d) {
        int64_t c = allocate(1)[0];
        std::string dot(32, '\0');
        dot.replace(0, 11, ".          ");
        dot[11] = 0x10;
        put16(dot, 22, t);
        put16(dot, 24, d);
        set_start(dot, c);
        std::string dotdot = dot;
        dotdot.replace(0, 11, "..         ");
        set_start(dotdot, folder == v.root_cluster ? 0 : folder);
        write_clusters({c}, dot + dotdot);
        std::string e = dot;
        e.replace(0, 11, shortname);
        f.write(static_cast<uint64_t>(new_slot(folder)), e);
        return c;
    }
    const char *write_file(int64_t folder, const std::string &shortname, const std::string &data, int64_t t, int64_t d) {
        int64_t at;
        std::string e;
        const char *how;
        if (find(folder, shortname, at, e)) {
            if (static_cast<uint8_t>(e[11]) & 0x10) raise("ValueError", cp437(Bytes(shortname)) + " is a folder");
            for (int64_t cl : chain_of(start_of(e))) set(cl, 0);
            free_from = 2;
            how = "replaced";
        } else {
            at = new_slot(folder);
            e.assign(32, '\0');
            e.replace(0, 11, shortname);
            e[11] = 0x20;
            how = "added";
        }
        int64_t n = (static_cast<int64_t>(data.size()) + step - 1) / step;
        std::vector<int64_t> ch = n ? allocate(n) : std::vector<int64_t>();
        write_clusters(ch, data);
        set_start(e, ch.empty() ? 0 : ch[0]);
        put16(e, 22, t);
        put16(e, 24, d);
        uint32_t len = static_cast<uint32_t>(data.size());
        for (int k = 0; k < 4; ++k) e[28 + k] = static_cast<char>(len >> (8 * k));
        f.write(static_cast<uint64_t>(at), e);
        return how;
    }
};

// an 8.3 name as a directory entry holds it ("vtd.vxd" -> "VTD     VXD")
std::string short_name(const std::string &name) {
    std::vector<uint32_t> cps = codepoints(name);
    for (uint32_t &ch : cps)
        if (ch >= 'a' && ch <= 'z') ch -= 0x20;
        else if (ch >= 0xE0 && ch <= 0xFE && ch != 0xF7) ch -= 0x20;
    std::vector<uint32_t> base, ext;
    bool dot = false;
    for (uint32_t ch : cps) {
        if (!dot && ch == '.') {
            dot = true;
            continue;
        }
        (dot ? ext : base).push_back(ch);
    }
    bool bad = base.empty() || base.size() > 8 || ext.size() > 3;
    std::vector<uint32_t> all = base;
    all.insert(all.end(), ext.begin(), ext.end());
    static const char *kBad = " .\"/\\[]:;=,+*?<>|";
    for (uint32_t ch : all) {
        if (ch < 33) bad = true;
        for (const char *p = kBad; *p; ++p)
            if (ch == static_cast<uint8_t>(*p)) bad = true;
    }
    if (bad) raise("ValueError", "not an 8.3 name: '" + name + "'");
    while (base.size() < 8) base.push_back(' ');
    while (ext.size() < 3) ext.push_back(' ');
    std::string out;
    for (auto *part : {&base, &ext})
        for (uint32_t ch : *part) {
            uint8_t b;
            if (!cp437_byte(ch, b)) raise("UnicodeEncodeError", "'charmap' codec can't encode character");
            out += static_cast<char>(b);
        }
    return out;
}

// path, offset, [(name, data)] -> [(path, "replaced" | "added")]
void put_q(Call &c) {
    const std::string path = c.str_arg(0);
    int64_t offset = c.int_arg(1);
    Fat vol;
    {
        Source src(path, offset);
        vol = open_fat(src, "", false, 0);
    }
    if (vol.old) raise("ValueError", "not a FAT volume with a parameter block");
    std::time_t now = std::time(nullptr);
    std::tm t = *std::localtime(&now);
    int64_t tt = (t.tm_hour << 11) | (t.tm_min << 5) | (t.tm_sec / 2);
    int64_t td = ((t.tm_year + 1900 - 1980) << 9) | ((t.tm_mon + 1) << 5) | t.tm_mday;
    Writer w(path, vol, offset);
    std::vector<std::pair<std::string, std::string>> done;
    try {
        for (const Value &item : c.arg(2).items) {
            std::string name = item[0].as_str();
            const std::string data = item[1].as_bytes().str();
            for (char &ch : name)
                if (ch == '\\') ch = '/';
            std::vector<std::string> parts;
            size_t s = 0;
            while (s <= name.size()) {
                size_t e = name.find('/', s);
                if (e == std::string::npos) e = name.size();
                if (e > s) parts.push_back(name.substr(s, e - s));
                s = e + 1;
            }
            int64_t folder = w.v.bits == 32 ? w.v.root_cluster : 0;
            for (size_t k = 0; k + 1 < parts.size(); ++k) {
                int64_t at;
                std::string e;
                std::string sn = short_name(parts[k]);
                if (!w.find(folder, sn, at, e)) folder = w.mkdir(folder, sn, tt, td);
                else if (static_cast<uint8_t>(e[11]) & 0x10) folder = w.start_of(e);
                else raise("ValueError", item[0].as_str() + ": " + parts[k] + " is not a folder");
            }
            if (parts.empty()) raise("IndexError", "list index out of range");
            std::string joined;
            for (size_t k = 0; k < parts.size(); ++k) joined += (k ? "/" : "") + parts[k];
            const char *how = w.write_file(folder, short_name(parts.back()), data, tt, td);
            done.emplace_back(joined, how);
        }
    } catch (...) {
        w.close();
        throw;
    }
    w.close();
    c.out.begin_list();
    for (auto &d : done) {
        c.out.begin_tuple();
        c.out.text(d.first);
        c.out.text(d.second);
        c.out.end_tuple();
    }
    c.out.end_list();
}

}  // namespace

CBF_QUESTIONS(media) {
    add("media.volumes", volumes_q, "list[(str, int)]");
    add("media.fat", fat_q);
    add("media.iso", iso_q);
    add("media.fat_files", fat_files_q);
    add("media.fat_read", fat_read_q, "bytes");
    add("media.fat_next", fat_next_q, "int");
    add("media.iso_files", iso_files_q);
    add("media.iso_read", iso_read_q, "bytes");
    add("media.iso_boot_image", iso_boot_image_q, "(int, int, int)?");
    add("media.put", put_q);
}

}  // namespace cbf::media
