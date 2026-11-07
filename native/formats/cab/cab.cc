// cab.cc -- Microsoft cabinet (MSCF) files: codebreaker/formats/cab.py.  The headers read, a
// folder's stream decompressed (stored, MSZIP, Quantum), files extracted, and a cabinet written.
//
// MSZIP is zlib's raw deflate: inflate gives the same bytes whatever zlib it is; deflate is
// zlib 1.3.1's (the copy native/decompiler carries), where Python's zlib module is zlib-ng -- a
// cabinet written here holds the same files, but its compressed blocks are zlib's own.
#include <cstdio>
#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"
#include "zlib.h"

namespace cbf::cab {
namespace {

constexpr int64_t kFromPrev = 0xFFFD, kToNext = 0xFFFE, kPrevAndNext = 0xFFFF;
constexpr int64_t kBlock = 32768, kShortBlock = 32756;

[[noreturn]] void cab_error(const std::string &m) { raise("CabError", m); }

struct Entry {
    std::string name;
    int64_t size, folder, offset, date, time, attributes;
    std::string compression;
};
struct Folder {
    int64_t offset, count, type;
};
struct Cabinet {
    std::string path;
    std::string prev, next, prev_disk, next_disk;
    bool has_prev = false, has_next = false;
    int64_t set_id = 0, index = 0, flags = 0, size = 0, minor = 0, major = 0;
    std::vector<std::string> kinds;
    std::vector<Entry> files;
    std::vector<Folder> folders;
    int64_t block_reserve = 0;
};

const char *compression_name(int64_t t) {
    switch (t) {
    case 0: return "NONE";
    case 1: return "MSZIP";
    case 2: return "QUANTUM";
    case 3: return "LZX";
    default: return nullptr;
    }
}

Bytes text_at(Bytes data, int64_t &at) {
    int64_t end = data.index_byte(0, at);
    Bytes s = data.slice(at, end);
    at = end + 1;
    return s;
}

Cabinet read_cab(const std::string &path) {
    std::string raw;
    if (!read_file(path, raw)) raise_open_error(path);
    Bytes data(raw);
    if (!data.slice_is(0, 4, "MSCF", 4) || data.size() < 36) cab_error(path + ": not a cabinet (no MSCF signature)");
    Cabinet c;
    c.path = path;
    c.size = data.u32(8);
    int64_t files_at = data.u32(16);
    c.minor = data.u8(24);
    c.major = data.u8(25);
    int64_t n_folders = data.u16(26), n_files = data.u16(28);
    c.flags = data.u16(30);
    c.set_id = data.u16(32);
    c.index = data.u16(34);
    int64_t at = 36, folder_reserve = 0;
    if (c.flags & 4) {
        int64_t header_reserve = data.u16(at);
        folder_reserve = data.u8(at + 2);
        c.block_reserve = data.u8(at + 3);
        at += 4 + header_reserve;
    }
    if (c.flags & 1) {
        c.prev = latin1(text_at(data, at));
        c.prev_disk = latin1(text_at(data, at));
        c.has_prev = true;
    }
    if (c.flags & 2) {
        c.next = latin1(text_at(data, at));
        c.next_disk = latin1(text_at(data, at));
        c.has_next = true;
    }
    for (int64_t k = 0; k < n_folders; ++k) {
        Folder f{data.u32(at), data.u16(at + 4), data.u16(at + 6)};
        c.folders.push_back(f);
        at += 8 + folder_reserve;
    }
    for (const Folder &f : c.folders) {
        const char *n = compression_name(f.type & 15);
        c.kinds.push_back(n ? n : "?" + std::to_string(f.type & 15));
    }
    at = files_at;
    for (int64_t k = 0; k < n_files; ++k) {
        Entry e;
        // unpack_from("<IIHHHH", data, at): one struct.error for the whole 16 bytes
        if (at + 16 > data.size())
            raise("struct.error", "unpack_from requires a buffer of at least " + std::to_string(at + 16) +
                                      " bytes for unpacking 16 bytes at offset " + std::to_string(at) +
                                      " (actual buffer size is " + std::to_string(data.size()) + ")");
        e.size = data.u32(at);
        e.offset = data.u32(at + 4);
        e.folder = data.u16(at + 8);
        e.date = data.u16(at + 10);
        e.time = data.u16(at + 12);
        e.attributes = data.u16(at + 14);
        at += 16;
        Bytes name = text_at(data, at);
        e.name = e.attributes & 0x80 ? utf8_replace(name) : latin1(name);
        int64_t kf = e.folder == kFromPrev || e.folder == kPrevAndNext ? 0 : e.folder == kToNext ? n_folders - 1 : e.folder;
        e.compression = 0 <= kf && kf < n_folders ? c.kinds[static_cast<size_t>(kf)] : "?";
        c.files.push_back(e);
    }
    return c;
}

void write_info(Out &o, const Cabinet &c) {
    o.begin_dict();
    auto opt = [&](const char *k, bool has, const std::string &v) {
        o.key(k);
        if (has) o.text(v);
        else o.none();
    };
    opt("prev", c.has_prev, c.prev);
    opt("prev_disk", c.has_prev, c.prev_disk);
    opt("next", c.has_next, c.next);
    opt("next_disk", c.has_next, c.next_disk);
    o.item("set_id", c.set_id);
    o.item("index", c.index);
    o.item("flags", c.flags);
    o.key("folders");
    o.begin_list();
    for (auto &k : c.kinds) o.text(k);
    o.end_list();
    o.item("size", c.size);
    o.item_text("version", std::to_string(c.major) + "." + std::to_string(c.minor));
    o.end_dict();
}

void read_q(Call &c) {
    Cabinet cab = read_cab(c.str_arg(0));
    Out &o = c.out;
    o.begin_tuple();
    write_info(o, cab);
    o.begin_list();
    for (const Entry &e : cab.files) {
        o.begin_tuple();
        o.text(e.name);
        o.integer(e.size);
        o.integer(e.folder);
        o.integer(e.offset);
        o.integer(e.date);
        o.integer(e.time);
        o.integer(e.attributes);
        o.text(e.compression);
        o.end_tuple();
    }
    o.end_list();
    o.begin_list();
    for (const Folder &f : cab.folders) {
        o.begin_tuple();
        o.integer(f.offset);
        o.integer(f.count);
        o.integer(f.type);
        o.end_tuple();
    }
    o.end_list();
    o.integer(cab.block_reserve);
    o.end_tuple();
}

// ---- decompression -------------------------------------------------------------------------
uint32_t checksum(Bytes payload, int64_t uncompressed) {
    std::string d;
    d += static_cast<char>(payload.size() & 0xFF);
    d += static_cast<char>((payload.size() >> 8) & 0xFF);
    d += static_cast<char>(uncompressed & 0xFF);
    d += static_cast<char>((uncompressed >> 8) & 0xFF);
    d.append(reinterpret_cast<const char *>(payload.data()), static_cast<size_t>(payload.size()));
    size_t whole = d.size() & ~size_t(3);
    uint32_t v = 0;
    for (size_t i = whole; i < d.size(); ++i) v = (v << 8) | static_cast<uint8_t>(d[i]);
    Bytes b(d);
    for (size_t i = 0; i < whole; i += 4) v ^= b.u32(static_cast<int64_t>(i));
    return v;
}

void checksum_q(Call &c) {   // (payload, uncompressed)
    int64_t u = c.int_arg(1);
    if (u < 0 || u > 0xFFFF || c.arg(0).as_bytes().size() > 0xFFFF) raise("struct.error", "ushort format requires 0 <= number <= 0xffff");
    c.out.integer(checksum(c.arg(0).as_bytes(), u));
}

std::string lower(const std::string &s) {
    std::vector<uint32_t> cps = codepoints(s);
    for (uint32_t &ch : cps)
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 0xC0 && ch <= 0xDE && ch != 0xD7)) ch += 0x20;
    return from_codepoints(cps);
}

Cabinet neighbour(const Cabinet &cab, bool next) {
    bool has = next ? cab.has_next : cab.has_prev;
    const std::string &name = next ? cab.next : cab.prev;
    std::string folder = dirname(abspath(cab.path));
    if (has) {
        std::vector<std::string> names;
        if (!list_dir(folder, names)) raise_open_error(folder);
        for (const std::string &cand : names)
            if (lower(cand) == lower(name)) return read_cab(join(folder, cand));
    }
    cab_error(cab.path + ": the " + (next ? "next" : "prev") + " cabinet of the set (" + (has ? name : "None") +
              ") is not beside it");
}

bool continued_from(const Cabinet &c) {
    for (auto &e : c.files)
        if (e.folder == kFromPrev || e.folder == kPrevAndNext) return true;
    return false;
}
bool continues_into(const Cabinet &c) {
    for (auto &e : c.files)
        if (e.folder == kToNext || e.folder == kPrevAndNext) return true;
    return false;
}

using Buf = std::vector<uint8_t>;

struct Step {
    virtual ~Step() = default;
    virtual void step(Bytes payload, int64_t usize, Buf &out) = 0;
};

struct Stored : Step {
    void step(Bytes payload, int64_t, Buf &out) override { out.insert(out.end(), payload.begin(), payload.end()); }
};

struct Mszip : Step {
    std::string window;
    void step(Bytes payload, int64_t usize, Buf &out) override {
        if (!payload.slice_is(0, 2, "CK", 2)) cab_error("MSZIP block without the CK signature");
        Bytes in = payload.slice(2);
        z_stream z{};
        if (inflateInit2(&z, -15) != Z_OK) raise("zlib.error", "Error while initializing decompression object");
        if (!window.empty() &&
            inflateSetDictionary(&z, reinterpret_cast<const Bytef *>(window.data()), static_cast<uInt>(window.size())) != Z_OK) {
            inflateEnd(&z);
            raise("zlib.error", "Error while setting zdict");
        }
        std::string got;
        z.next_in = const_cast<Bytef *>(in.data());
        z.avail_in = static_cast<uInt>(in.size());
        char chunk[65536];
        int rc = Z_OK;
        do {
            z.next_out = reinterpret_cast<Bytef *>(chunk);
            z.avail_out = sizeof chunk;
            rc = inflate(&z, Z_SYNC_FLUSH);
            got.append(chunk, sizeof chunk - z.avail_out);
            if (rc == Z_BUF_ERROR && z.avail_out != 0) rc = Z_OK, z.avail_in = 0;   // no more input: not an error
        } while (rc == Z_OK && (z.avail_in > 0 || z.avail_out == 0));
        if (rc != Z_OK && rc != Z_STREAM_END && rc != Z_BUF_ERROR) {
            std::string msg = z.msg ? z.msg : "";
            inflateEnd(&z);
            raise("zlib.error", "Error " + std::to_string(rc) + " while decompressing data: " + msg);
        }
        inflateEnd(&z);
        if (static_cast<int64_t>(got.size()) != usize)
            cab_error("MSZIP block inflated to " + std::to_string(got.size()) + " bytes, header says " + std::to_string(usize));
        out.insert(out.end(), got.begin(), got.end());
        std::string w = got + (got.size() < window.size() ? window.substr(got.size()) : std::string());
        if (w.size() > 32768) w = w.substr(w.size() - 32768);
        w.resize(32768 > w.size() ? 32768 : w.size(), '\0');
        window = w;
    }
};

// Quantum (formats/cab.py's decoder, its Python int arithmetic kept)
int64_t floordiv(int64_t a, int64_t b) {
    if (b == 0) raise("ZeroDivisionError", "integer division or modulo by zero");
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
int bit_length(int64_t v) {
    uint64_t u = v < 0 ? static_cast<uint64_t>(-v) : static_cast<uint64_t>(v);
    int n = 0;
    while (u) ++n, u >>= 1;
    return n;
}

struct Model {
    std::vector<int64_t> symbols, cum;
    int countdown = 4;
    Model(int64_t first, int64_t count) {
        for (int64_t i = 0; i < count; ++i) symbols.push_back(first + i);
        for (int64_t i = count; i >= 0; --i) cum.push_back(i);
    }
};

struct QTables {
    int64_t pos_extra[42], pos_base[42], len_extra[27], len_base[27];
    QTables() {
        pos_extra[0] = pos_extra[1] = 0;
        for (int i = 0; i < 40; ++i) pos_extra[2 + i] = i / 2;
        for (int i = 0; i < 42; ++i) {
            int64_t s = 0;
            for (int k = 0; k < i; ++k) s += int64_t(1) << pos_extra[k];
            pos_base[i] = s;
        }
        int k = 0;
        for (int i = 0; i < 6; ++i) len_extra[k++] = 0;
        for (int n = 1; n <= 5; ++n)
            for (int r = 0; r < 4; ++r) len_extra[k++] = n;
        len_extra[k++] = 0;
        for (int i = 0; i < 27; ++i) {
            int64_t s = 0;
            for (int j = 0; j < i; ++j) s += int64_t(1) << len_extra[j];
            len_base[i] = s;
        }
    }
};

struct Quantum : Step {
    int window_bits;
    std::vector<Model> literals, positions;
    Model lengths{0, 27}, selector{0, 7};
    int64_t produced = 0;

    explicit Quantum(int64_t kind) {
        window_bits = static_cast<int>((kind >> 8) & 0x1F);
        if (!(10 <= window_bits && window_bits <= 21))
            cab_error("Quantum window of 2^" + std::to_string(window_bits) + " bytes is not valid");
        for (int i = 0; i < 4; ++i) literals.emplace_back(64 * i, 64);
        positions.emplace_back(0, window_bits * 2 < 24 ? window_bits * 2 : 24);
        positions.emplace_back(0, window_bits * 2 < 36 ? window_bits * 2 : 36);
        positions.emplace_back(0, window_bits * 2);
    }

    static int64_t &at(std::vector<int64_t> &v, int64_t i) {
        if (i < 0) i += static_cast<int64_t>(v.size());
        if (i < 0 || i >= static_cast<int64_t>(v.size())) raise("IndexError", "list index out of range");
        return v[static_cast<size_t>(i)];
    }

    static void rescale(Model &m) {
        auto &symbols = m.symbols;
        auto &cum = m.cum;
        int64_t count = static_cast<int64_t>(symbols.size());
        m.countdown -= 1;
        if (m.countdown) {
            for (int64_t i = count - 1; i >= 0; --i) {
                cum[i] >>= 1;
                if (cum[i] <= cum[i + 1]) cum[i] = cum[i + 1] + 1;
            }
            return;
        }
        m.countdown = 50;
        std::vector<int64_t> freq(static_cast<size_t>(count));
        for (int64_t i = 0; i < count; ++i) freq[i] = (cum[i] - cum[i + 1] + 1) >> 1;
        for (int64_t i = 0; i < count - 1; ++i) {
            int64_t fi = freq[i];
            for (int64_t j = i + 1; j < count; ++j)
                if (fi < freq[j]) {
                    freq[i] = freq[j];
                    freq[j] = fi;
                    std::swap(symbols[i], symbols[j]);
                    fi = freq[i];
                }
        }
        for (int64_t i = count - 1; i >= 0; --i) cum[i] = cum[i + 1] + freq[i];
    }

    void step(Bytes payload, int64_t usize, Buf &out) override {
        static const QTables T;
        std::string data(reinterpret_cast<const char *>(payload.data()), static_cast<size_t>(payload.size()));
        data.append(8, '\0');
        Bytes d(data);
        const int64_t dlen = d.size();
        int64_t pos = 2, have = 0;
        uint64_t buf = 0;
        int64_t high = 0xFFFF, low = 0, code = (static_cast<int64_t>(d.at(0)) << 8) | d.at(1);
        produced += usize;
        const int64_t goal = produced;

        auto bits = [&](int64_t n) -> int64_t {
            if (have < n) {
                buf = ((buf & ((uint64_t(1) << have) - 1)) << 32) | d.slice(pos, pos + 4).from_be();
                pos += 4;
                have += 32;
                if (pos > dlen + 64) cab_error("Quantum block ran out of data");
            }
            have -= n;
            return static_cast<int64_t>((buf >> have) & ((uint64_t(1) << n) - 1));
        };
        auto symbol = [&](Model &m) -> int64_t {
            auto &cum = m.cum;
            int64_t total = at(cum, 0);
            int64_t span = high - low + 1;
            int64_t target = floordiv((code - low + 1) * total - 1, span);
            int64_t i = 1;
            while (at(cum, i) > target) ++i;
            int64_t value = at(m.symbols, i - 1);
            high = low + floordiv(at(cum, i - 1) * span, total) - 1;
            low += floordiv(at(cum, i) * span, total);
            for (int64_t j = 0; j < i; ++j) cum[j] += 8;
            if (cum[0] > 3800) rescale(m);
            while (true) {
                int64_t differ = high ^ low, n;
                if (differ & 0x8000) {
                    if (!((low & 0x4000) && !(high & 0x4000))) break;
                    code ^= 0x4000;
                    low &= 0x3FFF;
                    high |= 0x4000;
                    n = 1;
                } else {
                    n = 16 - bit_length(differ);
                }
                if (n < 0) raise("ValueError", "negative shift count");
                low = (low << n) & 0xFFFF;
                high = ((high << n) | ((int64_t(1) << n) - 1)) & 0xFFFF;
                code = ((code << n) | bits(n)) & 0xFFFF;
            }
            return value;
        };

        while (static_cast<int64_t>(out.size()) < goal) {
            int64_t which = symbol(selector);
            if (which < 4) {
                int64_t v = symbol(literals.at(static_cast<size_t>(which)));
                if (v < 0 || v > 255) raise("ValueError", "byte must be in range(0, 256)");
                out.push_back(static_cast<uint8_t>(v));
                continue;
            }
            int64_t length, slot;
            if (which == 6) {
                slot = symbol(lengths);
                length = T.len_base[slot] + bits(T.len_extra[slot]) + 5;
            } else {
                length = which - 1;
            }
            slot = symbol(positions.at(static_cast<size_t>(which - 4)));
            int64_t distance = T.pos_base[slot] + bits(T.pos_extra[slot]) + 1;
            int64_t start = static_cast<int64_t>(out.size()) - distance;
            if (start < 0) cab_error("Quantum match reaches before the start of the folder");
            if (distance >= length) {
                Buf piece(out.begin() + start, out.begin() + start + length);
                out.insert(out.end(), piece.begin(), piece.end());
            } else {
                Buf piece(out.begin() + start, out.end());
                for (int64_t k = 0; k < length; ++k) out.push_back(piece[static_cast<size_t>(k % distance)]);
            }
        }
    }
};

// the stream of the folder that folder k of cab is part of, at least `upto` bytes of it
Buf folder_bytes(Cabinet cab, int64_t k, int64_t upto) {
    while (k == 0 && continued_from(cab)) {
        cab = neighbour(cab, false);
        k = static_cast<int64_t>(cab.folders.size()) - 1;
    }
    int64_t kind = py_at(cab.folders, k).type;
    std::unique_ptr<Step> step;
    switch (kind & 15) {
    case 0: step.reset(new Stored()); break;
    case 1: step.reset(new Mszip()); break;
    case 2: step.reset(new Quantum(kind)); break;
    case 3: cab_error("LZX-compressed cabinets are not supported");
    default: cab_error(cab.path + ": unknown compression type " + std::to_string(kind & 15));
    }
    Buf out;
    std::string held;
    while (true) {
        const Folder &f = py_at(cab.folders, k);
        std::string raw;
        if (!read_file(cab.path, raw)) raise_open_error(cab.path);
        int64_t p = f.offset;
        auto take = [&](int64_t n) {
            Bytes b = Bytes(raw).slice(p < static_cast<int64_t>(raw.size()) ? p : static_cast<int64_t>(raw.size()), p + n);
            p += b.size();
            return b;
        };
        for (int64_t i = 0; i < f.count; ++i) {
            Bytes h = take(8);
            h.exact(8);
            int64_t stored = h.u32(0), csize = h.u16(4), usize = h.u16(6);
            take(cab.block_reserve);
            Bytes part = take(csize);
            if (part.size() != csize) cab_error(cab.path + ": truncated data block");
            if (stored && stored != checksum(part, usize)) cab_error(cab.path + ": data block checksum mismatch");
            if (usize == 0 && csize) {
                held += part.str();
                continue;
            }
            std::string whole = held + part.str();
            held.clear();
            if (usize) step->step(Bytes(whole), usize, out);
            if (upto >= 0 && static_cast<int64_t>(out.size()) >= upto) return out;
        }
        if (k != static_cast<int64_t>(cab.folders.size()) - 1 || !continues_into(cab)) return out;
        cab = neighbour(cab, true);
        k = 0;
    }
}

void folder_bytes_q(Call &c) {   // (path, k, upto)
    Cabinet cab = read_cab(c.str_arg(0));
    Buf out = folder_bytes(cab, c.int_arg(1), c.arg(2).is_none() ? -1 : c.int_arg(2));
    c.out.bytes(Bytes(out));
}

int64_t folder_of(const Cabinet &c, const Entry &e) {
    if (e.folder == kFromPrev || e.folder == kPrevAndNext) return 0;
    return e.folder == kToNext ? static_cast<int64_t>(c.folders.size()) - 1 : e.folder;
}

std::string safe_path(const std::string &outdir, const std::string &name) {
    std::string n = name;
    for (char &ch : n)
        if (ch == '\\') ch = '/';
    std::vector<std::string> parts;
    size_t s = 0;
    while (s <= n.size()) {
        size_t e = n.find('/', s);
        if (e == std::string::npos) e = n.size();
        std::string part = n.substr(s, e - s);
        if (!part.empty() && part != ".") parts.push_back(part);
        s = e + 1;
    }
    bool bad = parts.empty() || name.find(':') != std::string::npos;
    for (auto &p : parts)
        if (p == "..") bad = true;
    if (bad) {
        // repr(name): the cabinet's names are printable text
        cab_error("refusing to extract the unsafe name '" + name + "'");
    }
    std::string out = outdir;
    for (auto &p : parts) out = join(out, p);
    return out;
}

void extract_q(Call &c) {   // (path, outdir, names or None)
    const std::string path = c.str_arg(0), outdir = c.str_arg(1);
    Cabinet cab = read_cab(path);
    const Value &names = c.arg(2);
    std::vector<std::string> wanted;
    for (const Value &v : names.items) wanted.push_back(lower(v.as_str()));
    std::map<int64_t, std::vector<const Entry *>> groups;
    for (const Entry &e : cab.files) {
        bool ok = names.is_none();
        if (!ok) {
            std::string l = lower(e.name);
            for (auto &w : wanted)
                if (w == l) ok = true;
        }
        if (ok) groups[folder_of(cab, e)].push_back(&e);
    }
    std::vector<std::pair<std::string, std::string>> written;
    for (auto &g : groups) {
        int64_t upto = 0;
        bool first = true;
        for (const Entry *e : g.second)
            if (first || e->offset + e->size > upto) upto = e->offset + e->size, first = false;
        Buf stream = folder_bytes(cab, g.first, upto);
        for (const Entry *e : g.second) {
            Bytes data = Bytes(stream).slice(e->offset, e->offset + e->size);
            if (data.size() != e->size) cab_error(path + ": " + e->name + " is cut short");
            std::string target = safe_path(outdir, e->name);
            if (!make_dirs(dirname(target))) raise("OSError", "cannot make the folder for " + target);
            if (!write_file(target, data.str())) raise_open_error(target);
            int64_t d = e->date, t = e->time;
            std::tm tm{};
            tm.tm_year = static_cast<int>(1980 + (d >> 9) - 1900);
            tm.tm_mon = static_cast<int>(((d >> 5) & 15) - 1);
            tm.tm_mday = static_cast<int>(d & 31);
            tm.tm_hour = static_cast<int>(t >> 11);
            tm.tm_min = static_cast<int>((t >> 5) & 63);
            tm.tm_sec = static_cast<int>((t & 31) * 2);
            tm.tm_isdst = -1;
            tm.tm_wday = -1;
            std::time_t stamp = std::mktime(&tm);
            if (!(stamp == static_cast<std::time_t>(-1) && tm.tm_wday == -1)) set_times(target, static_cast<double>(stamp));
            bool found = false;
            for (auto &w : written)
                if (w.first == e->name) w.second = target, found = true;
            if (!found) written.emplace_back(e->name, target);
        }
    }
    c.out.begin_dict();
    for (auto &w : written) {
        c.out.text(w.first);
        c.out.text(w.second);
    }
    c.out.end_dict();
}

// ---- writing -------------------------------------------------------------------------------
std::string deflate_raw(Bytes chunk, Bytes dictionary) {
    z_stream z{};
    if (deflateInit2(&z, 9, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) raise("zlib.error", "deflateInit2 failed");
    if (!dictionary.empty())
        deflateSetDictionary(&z, dictionary.data(), static_cast<uInt>(dictionary.size()));
    std::string out(deflateBound(&z, static_cast<uLong>(chunk.size())) + 64, '\0');
    z.next_in = const_cast<Bytef *>(chunk.data());
    z.avail_in = static_cast<uInt>(chunk.size());
    z.next_out = reinterpret_cast<Bytef *>(&out[0]);
    z.avail_out = static_cast<uInt>(out.size());
    int rc = deflate(&z, Z_FINISH);
    out.resize(out.size() - z.avail_out);
    deflateEnd(&z);
    if (rc != Z_STREAM_END) raise("zlib.error", "deflate did not finish");
    return out;
}

std::vector<std::pair<std::string, int64_t>> mszip_blocks(Bytes stream, bool short_blocks) {
    std::vector<std::pair<std::string, int64_t>> blocks;
    int64_t at = 0;
    Bytes previous;
    while (at < stream.size()) {
        Bytes chunk = stream.slice(at, at + kBlock);
        std::string body = deflate_raw(chunk, previous);
        previous = chunk;
        if (static_cast<int64_t>(body.size()) + 2 > 32768 && short_blocks) {
            chunk = stream.slice(at, at + kShortBlock);
            int64_t n = chunk.size();
            body.clear();
            body += '\x01';
            body += static_cast<char>(n & 0xFF);
            body += static_cast<char>(n >> 8);
            body += static_cast<char>((n ^ 0xFFFF) & 0xFF);
            body += static_cast<char>((n ^ 0xFFFF) >> 8);
            body += chunk.str();
            previous = Bytes();
        }
        blocks.emplace_back("CK" + body, chunk.size());
        at += chunk.size();
    }
    return blocks;
}

std::string chain_text(const Value &v) {
    if (!v.truthy()) return "";
    std::string name, disk;
    if (v.kind == Value::Str) {
        name = v.as_str();
    } else {
        if (v.items.size() != 2) raise("ValueError", "too many values to unpack (expected 2)");
        name = v[0].as_str();
        disk = v[1].truthy() ? v[1].as_str() : std::string();
    }
    return to_latin1(name) + std::string(1, '\0') + to_latin1(disk) + std::string(1, '\0');
}

void put16(std::string &s, int64_t v) {
    s += static_cast<char>(v & 0xFF);
    s += static_cast<char>((v >> 8) & 0xFF);
}
void put32(std::string &s, int64_t v) {
    put16(s, v & 0xFFFF);
    put16(s, (v >> 16) & 0xFFFF);
}

void write_q(Call &c) {   // (files, out_path, compress, set_id, index, prev, next, short_blocks)
    const Value &files = c.arg(0);
    const std::string out_path = c.str_arg(1), compress = c.str_arg(2, "mszip");
    if (compress != "mszip" && compress != "none")
        cab_error("compress must be 'mszip' or 'none', not '" + compress + "'");
    std::string stream;
    for (const Value &f : files.items) stream += f[1].as_bytes().str();
    std::vector<std::pair<std::string, int64_t>> blocks;
    if (compress == "mszip") {
        blocks = mszip_blocks(Bytes(stream), c.bool_arg(7, true));
    } else {
        for (int64_t at = 0; at < static_cast<int64_t>(stream.size()); at += kBlock) {
            int64_t n = static_cast<int64_t>(stream.size()) - at < kBlock ? static_cast<int64_t>(stream.size()) - at : kBlock;
            blocks.emplace_back(stream.substr(static_cast<size_t>(at), static_cast<size_t>(n)), n);
        }
    }
    if (blocks.size() > 0xFFFF || files.items.size() > 0xFFFF)
        cab_error("too much for one cabinet folder (" + std::to_string(blocks.size()) + " blocks, " +
                  std::to_string(files.items.size()) + " files)");
    const Value &prev = c.arg(5), &next = c.arg(6);
    std::string chain = chain_text(prev) + chain_text(next);
    int64_t flags = (prev.truthy() ? 1 : 0) | (next.truthy() ? 2 : 0);
    std::time_t now = std::time(nullptr);
    std::tm st = *std::localtime(&now);
    int64_t date = ((st.tm_year + 1900 - 1980) << 9) | ((st.tm_mon + 1) << 5) | st.tm_mday;
    int64_t clock = (st.tm_hour << 11) | (st.tm_min << 5) | (st.tm_sec / 2);
    std::string entries;
    int64_t offset = 0;
    for (const Value &f : files.items) {
        const std::string &name = f[0].as_str();
        int64_t len = f[1].as_bytes().size();
        bool ascii_only = true;
        for (unsigned char ch : name)
            if (ch >= 0x80) ascii_only = false;
        put32(entries, len);
        put32(entries, offset);
        put16(entries, 0);
        put16(entries, date);
        put16(entries, clock);
        put16(entries, ascii_only ? 0x20 : 0xA0);
        entries += name;
        entries += '\0';
        offset += len;
    }
    int64_t files_at = 36 + static_cast<int64_t>(chain.size()) + 8;
    int64_t data_at = files_at + static_cast<int64_t>(entries.size());
    std::string body;
    for (auto &b : blocks) {
        put32(body, 0);
        put16(body, static_cast<int64_t>(b.first.size()));
        put16(body, b.second);
        body += b.first;
    }
    int64_t total = data_at + static_cast<int64_t>(body.size());
    std::string header = "MSCF";
    put32(header, 0);
    put32(header, total);
    put32(header, 0);
    put32(header, files_at);
    put32(header, 0);
    header += '\x03';
    header += '\x01';
    put16(header, 1);
    put16(header, static_cast<int64_t>(files.items.size()));
    put16(header, flags);
    put16(header, c.int_arg(3));
    put16(header, c.int_arg(4));
    std::string folder;
    put32(folder, data_at);
    put16(folder, static_cast<int64_t>(blocks.size()));
    put16(folder, compress == "mszip" ? 1 : 0);
    if (!write_file(out_path, header + chain + folder + entries + body)) raise_open_error(out_path);
    c.out.integer(total);
}

}  // namespace

CBF_QUESTIONS(cab) {
    add("cab.read", read_q, "(CabInfo{prev: str?, prev_disk: str?, next: str?, next_disk: str?, set_id: int, index: int, flags: int, folders: list[str], size: int, version: str}, list[(str, int, int, int, int, int, int, str)], list[(int, int, int)], int)");
    add("cab.checksum", checksum_q, "int");
    add("cab.folder_bytes", folder_bytes_q, "bytes");
    add("cab.extract", extract_q, "dict[str, str]");
    add("cab.write", write_q, "int");
}

}  // namespace cbf::cab
