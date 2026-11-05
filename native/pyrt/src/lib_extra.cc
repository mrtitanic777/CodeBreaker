// lib_extra.cc -- the C halves of standard modules whose Python half is CPython's own, translated by
// pyengine --dynamic (native/pyrt/stdlib.txt): gettext (no catalogues: messages as given), _random
// (CPython's Mersenne Twister, so seeded sequences are CPython's), _operator, _csv, zlib's deflate (the
// zlib 1.3.1 copy in native/decompiler), and the small modules the tree touches (msvcrt, faulthandler,
// signal, weakref, locale, _winapi).
#include <conio.h>
#include <io.h>
#include <sys/locking.h>

#include <cstring>
#include <ctime>

#include <windows.h>

#include "rt.hpp"
#include "zlib.h"

namespace py {

TypeObj* T(const var& v);
static var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }
static var mkcls(const char* name, std::vector<var> bases, std::vector<std::pair<const char*, var>> members, const char* module) {
    var ns = new_dict();
    for (auto& [k, v] : members) ns.setitem(new_str(k), v);
    return make_class(name, name, bases, ns, {}, module);
}

// ------------------------------------------------------------------------------------------ gettext
static void init_gettext(const var& m) {
    mod_def(m, "gettext", [](const CallArgs& a) { return a.pos.at(0); });
    mod_def(m, "ngettext", [](const CallArgs& a) { return a.pos.at(2).as_int64() == 1 ? a.pos.at(0) : a.pos.at(1); });
    mod_def(m, "dgettext", [](const CallArgs& a) { return a.pos.at(1); });
    mod_def(m, "dngettext", [](const CallArgs& a) { return a.pos.at(3).as_int64() == 1 ? a.pos.at(1) : a.pos.at(2); });
    mod_def(m, "pgettext", [](const CallArgs& a) { return a.pos.at(1); });
    mod_def(m, "npgettext", [](const CallArgs& a) { return a.pos.at(3).as_int64() == 1 ? a.pos.at(1) : a.pos.at(2); });
    mod_def(m, "textdomain", [](const CallArgs& a) { return a.pos.empty() ? new_str("messages") : a.pos[0]; });
    mod_def(m, "bindtextdomain", [](const CallArgs& a) { return a.pos.size() > 1 ? a.pos[1] : None; });
}

// ------------------------------------------------------------------------------------------ _random
// CPython's Modules/_randommodule.c: MT19937, init_by_array over the seed's 32-bit words (least
// significant first), random() from 53 bits, getrandbits in 32-bit words.
struct RandObj : DataObj {
    uint32_t mt[624];
    int index = 625;
    void init_genrand(uint32_t s) {
        mt[0] = s;
        for (int i = 1; i < 624; i++) mt[i] = 1812433253u * (mt[i - 1] ^ (mt[i - 1] >> 30)) + (uint32_t)i;
        index = 624;
    }
    void init_by_array(const std::vector<uint32_t>& key) {
        init_genrand(19650218u);
        size_t i = 1, j = 0, n = key.empty() ? 1 : key.size();
        for (size_t k = 624 > n ? 624 : n; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525u)) + (key.empty() ? 0 : key[j]) + (uint32_t)j;
            i++;
            j++;
            if (i >= 624) { mt[0] = mt[623]; i = 1; }
            if (j >= n) j = 0;
        }
        for (size_t k = 623; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941u)) - (uint32_t)i;
            i++;
            if (i >= 624) { mt[0] = mt[623]; i = 1; }
        }
        mt[0] = 0x80000000u;
    }
    uint32_t next() {
        static const uint32_t mag01[2] = {0x0u, 0x9908b0dfu};
        uint32_t y;
        if (index >= 624) {
            int kk;
            for (kk = 0; kk < 624 - 397; kk++) {
                y = (mt[kk] & 0x80000000u) | (mt[kk + 1] & 0x7fffffffu);
                mt[kk] = mt[kk + 397] ^ (y >> 1) ^ mag01[y & 1];
            }
            for (; kk < 623; kk++) {
                y = (mt[kk] & 0x80000000u) | (mt[kk + 1] & 0x7fffffffu);
                mt[kk] = mt[kk + (397 - 624)] ^ (y >> 1) ^ mag01[y & 1];
            }
            y = (mt[623] & 0x80000000u) | (mt[0] & 0x7fffffffu);
            mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 1];
            index = 0;
        }
        y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680u;
        y ^= (y << 15) & 0xefc60000u;
        y ^= (y >> 18);
        return y;
    }
};
static RandObj* RO(const var& v) { return static_cast<RandObj*>(v.ref()); }
static std::vector<uint32_t> words_of(var n) {
    std::vector<uint32_t> key;
    if (n < 0) n = -n;
    var mask = var((int64_t)0xFFFFFFFF);
    while (n > 0) {
        key.push_back((uint32_t)(n & mask).as_int64());
        n = n >> var(32);
    }
    return key;
}
static void rand_seed(RandObj* r, const var& arg) {
    if (arg.is_unbound() || arg.is_none()) {
        std::vector<uint32_t> key(624);
        var b = import("os").call("urandom", var((int64_t)(key.size() * 4)));
        memcpy(key.data(), raw_of(b).data(), key.size() * 4);
        r->init_by_array(key);
        return;
    }
    var n = is_int(arg) ? arg : var(hash(arg));
    r->init_by_array(words_of(n));
}
static void init_random_c(const var& m) {
    TypeObj* t = new_type("Random", {}, Layout::NativeData, true, "_random");
    add_static(t, "__new__", [](const CallArgs& a) {
        auto* r = new RandObj();
        r->type = T(a.pos.at(0));
        rand_seed(r, var());
        return var(static_cast<Obj*>(r));
    });
    add_method(t, "__init__", [](const CallArgs& a) {
        if (a.pos.size() > 1) rand_seed(RO(a.pos[0]), a.pos[1]);
        return None;
    });
    add_method(t, "seed", [](const CallArgs& a) {
        rand_seed(RO(a.pos[0]), a.pos.size() > 1 ? a.pos[1] : var(None));
        return None;
    });
    add_method(t, "random", [](const CallArgs& a) {
        RandObj* r = RO(a.pos[0]);
        uint32_t x = r->next() >> 5, y = r->next() >> 6;
        return var((x * 67108864.0 + y) * (1.0 / 9007199254740992.0));
    });
    add_method(t, "getrandbits", [](const CallArgs& a) -> var {
        RandObj* r = RO(a.pos[0]);
        int64_t k = a.pos.at(1).as_int64();
        if (k < 0) raise(E_ValueError, "number of bits must be non-negative");
        if (k == 0) return var(0);
        if (k <= 32) return var((int64_t)(r->next() >> (32 - k)));
        var out = var(0);
        int64_t shift = 0;
        for (int64_t left = k; left > 0; left -= 32, shift += 32) {
            uint32_t w = r->next();
            if (left < 32) w >>= (32 - left);
            out = out | (var((int64_t)w) << var(shift));
        }
        return out;
    });
    add_method(t, "getstate", [](const CallArgs& a) {
        RandObj* r = RO(a.pos[0]);
        std::vector<var> v;
        for (int i = 0; i < 624; i++) v.push_back(var((int64_t)r->mt[i]));
        v.push_back(var((int64_t)r->index));
        return new_tuple(v);
    });
    add_method(t, "setstate", [](const CallArgs& a) {
        RandObj* r = RO(a.pos[0]);
        auto v = collect(a.pos.at(1));
        if (v.size() != 625) raise(E_ValueError, "state vector is the wrong size");
        for (int i = 0; i < 624; i++) r->mt[i] = (uint32_t)v[(size_t)i].as_int64();
        r->index = (int)v[624].as_int64();
        return None;
    });
    mod_set(m, "Random", tv(t));
}

// ------------------------------------------------------------------------------------------ _csv
// The writer and reader of Modules/_csv.c for the dialect attributes csv.py's Dialect classes carry.
struct CsvDialect {
    std::string delimiter = ",", quotechar = "\"", escapechar, lineterminator = "\r\n";
    bool doublequote = true, skipinitialspace = false, strict = false;
    int quoting = 0;   // QUOTE_MINIMAL, ALL, NONNUMERIC, NONE, STRINGS, NOTNULL
};
static var g_csv_dialects;
static CsvDialect dialect_of(const CallArgs& a, size_t pos) {
    CsvDialect d;
    var src = arg(a, pos, "dialect", "excel"_s);
    if (is_str(src)) {
        var* reg = static_cast<DictObj*>(g_csv_dialects.ref())->get(src);
        if (!reg) raise(T(import("_csv").getattr("Error")), "unknown dialect");
        src = *reg;
    }
    auto get = [&](const char* name) -> var {
        var k = kwarg(a, name, var());
        if (!k.is_unbound()) return k;
        return getattr(src, interned(name), var());
    };
    auto s = [&](const char* name, std::string& out) {
        var v = get(name);
        if (!v.is_unbound()) out = v.is_none() ? "" : str_utf8(v);
    };
    s("delimiter", d.delimiter);
    s("quotechar", d.quotechar);
    s("escapechar", d.escapechar);
    s("lineterminator", d.lineterminator);
    if (var v = get("doublequote"); !v.is_unbound()) d.doublequote = v.truthy();
    if (var v = get("skipinitialspace"); !v.is_unbound()) d.skipinitialspace = v.truthy();
    if (var v = get("strict"); !v.is_unbound()) d.strict = v.truthy();
    if (var v = get("quoting"); !v.is_unbound()) d.quoting = (int)v.as_int64();
    return d;
}
struct CsvWriter : DataObj { var file; CsvDialect d; };
static std::string csv_field(const CsvDialect& d, const var& f) {
    bool is_num = is_int(f) || f.is_float();
    if (f.is_none() && d.quoting != 5) return "";
    std::string s = f.is_none() ? "" : str_(f);
    bool quote = d.quoting == 1 || (d.quoting == 2 && !is_num) || (d.quoting == 4 && is_str(f)) || (d.quoting == 5 && !f.is_none());
    if (d.quoting != 3 && !quote) {
        if (s.empty() && false) quote = true;
        for (char c : s)
            if (d.delimiter.find(c) != std::string::npos || (!d.quotechar.empty() && c == d.quotechar[0]) || c == '\n' || c == '\r' ||
                d.lineterminator.find(c) != std::string::npos) { quote = true; break; }
        if (!s.empty() && (s[0] == ' ' || s[0] == '\t') && d.skipinitialspace) quote = true;
    }
    std::string o;
    for (char c : s) {
        bool special = (!d.quotechar.empty() && c == d.quotechar[0]) || (!d.escapechar.empty() && c == d.escapechar[0]);
        if (!d.quotechar.empty() && c == d.quotechar[0] && quote && d.doublequote) { o += c; o += c; continue; }
        if (special || (d.quoting == 3 && (d.delimiter.find(c) != std::string::npos || c == '\n' || c == '\r'))) {
            if (d.escapechar.empty()) raise(T(import("_csv").getattr("Error")), "need to escape, but no escapechar set");
            o += d.escapechar;
        }
        o += c;
    }
    return quote ? d.quotechar + o + d.quotechar : o;
}
static void init_csv_c(const var& m) {
    var err = mkcls("Error", {t::Exception}, {}, "_csv");
    mod_set(m, "Error", err);
    const char* qn[] = {"QUOTE_MINIMAL", "QUOTE_ALL", "QUOTE_NONNUMERIC", "QUOTE_NONE", "QUOTE_STRINGS", "QUOTE_NOTNULL"};
    for (int i = 0; i < 6; i++) mod_set(m, qn[i], var((int64_t)i));
    mod_set(m, "__version__", "1.0"_s);
    g_csv_dialects = new_dict();
    var dialect = mkcls("Dialect", {}, {}, "_csv");
    mod_set(m, "Dialect", dialect);
    mod_def(m, "register_dialect", [](const CallArgs& a) {
        var d = a.pos.size() > 1 ? a.pos[1] : var(None);
        if (d.is_none() || !a.kw.empty()) {
            var ns = mkcls("dialect", {}, {}, "_csv");
            if (!d.is_none()) for (const char* n : {"delimiter", "quotechar", "escapechar", "lineterminator", "doublequote", "skipinitialspace", "quoting", "strict"})
                if (hasattr(d, interned(n)).truthy()) ns.setattr(n, d.getattr(n));
            for (auto& [k, v] : a.kw) ns.setattr(str_utf8(k).c_str(), v);
            d = ns;
        }
        g_csv_dialects.setitem(a.pos.at(0), d);
        return None;
    });
    mod_def(m, "unregister_dialect", [](const CallArgs& a) { g_csv_dialects.delitem(a.pos.at(0)); return None; });
    mod_def(m, "get_dialect", [](const CallArgs& a) { return g_csv_dialects.getitem(a.pos.at(0)); });
    mod_def(m, "list_dialects", [](const CallArgs&) { return new_list(collect(g_csv_dialects)); });
    mod_def(m, "field_size_limit", [](const CallArgs&) { return var((int64_t)131072); });
    static TypeObj* T_w = new_type("writer", {}, Layout::NativeData, true, "_csv");
    add_method(T_w, "writerow", [](const CallArgs& a) {
        auto* w = static_cast<CsvWriter*>(a.pos[0].ref());
        std::string line;
        bool first = true;
        for (var f : each(a.pos.at(1))) {
            if (!first) line += w->d.delimiter;
            first = false;
            line += csv_field(w->d, f);
        }
        line += w->d.lineterminator;
        return w->file.call("write", new_str(line));
    });
    add_method(T_w, "writerows", [](const CallArgs& a) {
        for (var row : each(a.pos.at(1))) a.pos[0].call("writerow", row);
        return None;
    });
    mod_def(m, "writer", [](const CallArgs& a) {
        auto* w = new CsvWriter();
        w->type = T_w;
        w->file = a.pos.at(0);
        w->d = dialect_of(a, 1);
        return var(static_cast<Obj*>(w));
    });
    // reader: rows of a line iterator, quoted fields across lines, as _csv.c's state machine
    mod_def(m, "reader", [](const CallArgs& a) {
        var lines = iter(a.pos.at(0));
        CsvDialect d = dialect_of(a, 1);
        return new_iter([lines, d](var& out) mutable -> bool {
            std::vector<var> row;
            std::string field;
            bool in_q = false, any = false, was_q = false;
            for (;;) {
                var ln;
                if (!next_(lines, ln)) {
                    if (!any) return false;
                    if (in_q && d.strict) raise(T(import("_csv").getattr("Error")), "unexpected end of data");
                    row.push_back(new_str(field));
                    break;
                }
                any = true;
                std::string s = str_utf8(ln);
                size_t i = 0;
                bool done = false;
                while (i < s.size()) {
                    char c = s[i];
                    if (in_q) {
                        if (!d.escapechar.empty() && c == d.escapechar[0] && i + 1 < s.size()) { field += s[i + 1]; i += 2; continue; }
                        if (!d.quotechar.empty() && c == d.quotechar[0]) {
                            if (d.doublequote && i + 1 < s.size() && s[i + 1] == c) { field += c; i += 2; continue; }
                            in_q = false; i++; continue;
                        }
                        field += c; i++; continue;
                    }
                    if (c == '\r' || c == '\n') { done = true; break; }
                    if (!d.escapechar.empty() && c == d.escapechar[0] && i + 1 < s.size()) { field += s[i + 1]; i += 2; continue; }
                    if (d.delimiter.size() && c == d.delimiter[0]) {
                        row.push_back(d.quoting == 2 && !was_q && !field.empty() ? call1(t::float_, new_str(field)) : new_str(field));
                        field.clear(); was_q = false; i++;
                        if (d.skipinitialspace) while (i < s.size() && s[i] == ' ') i++;
                        continue;
                    }
                    if (d.quoting != 3 && !d.quotechar.empty() && c == d.quotechar[0] && field.empty()) { in_q = true; was_q = true; i++; continue; }
                    field += c; i++;
                }
                if (in_q) { field += "\n"; continue; }
                (void)done;
                if (row.empty() && field.empty() && !was_q) { out = new_list(); return true; }
                row.push_back(d.quoting == 2 && !was_q && !field.empty() ? call1(t::float_, new_str(field)) : new_str(field));
                break;
            }
            out = new_list(row);
            return true;
        });
    });
}

// ------------------------------------------------------------------------------------------ zlib
// zlib's own deflate and inflate (native/decompiler/core/zlib, 1.3.1): compress/decompress and the
// streaming objects, wbits as CPython's (negative raw, 9..15 zlib, 25..31 gzip, 40..47 auto).
static var zerror(int rc, const char* what, z_stream* s) {
    std::string msg = std::string("Error ") + std::to_string(rc) + " " + what;
    if (s && s->msg) msg += ": " + std::string(s->msg);
    return make_exc_args(T(import("zlib").getattr("error")), {new_str(msg)});
}
struct ZObj : DataObj {
    z_stream s{};
    bool compress = false, ended = false, eof = false;
    var unused_data, unconsumed_tail;
    ~ZObj() override {
        if (!ended) { if (compress) deflateEnd(&s); else inflateEnd(&s); }
    }
};
static ZObj* ZO(const var& v) { return static_cast<ZObj*>(v.ref()); }
static std::string zrun_deflate(z_stream& s, const std::string& in, int flush) {
    std::string out;
    s.next_in = (Bytef*)in.data();
    s.avail_in = (uInt)in.size();
    char buf[65536];
    do {
        s.next_out = (Bytef*)buf;
        s.avail_out = sizeof buf;
        int rc = deflate(&s, flush);
        if (rc == Z_STREAM_ERROR) raise(zerror(rc, "while compressing data", &s));
        out.append(buf, sizeof buf - s.avail_out);
    } while (s.avail_out == 0 || (flush == Z_FINISH && s.avail_in > 0));
    return out;
}
static void init_zlib_c(const var& m) {
    mod_set(m, "error", mkcls("error", {t::Exception}, {}, "zlib"));
    struct { const char* n; int64_t v; } consts[] = {
        {"MAX_WBITS", MAX_WBITS}, {"DEFLATED", Z_DEFLATED}, {"DEF_MEM_LEVEL", 8}, {"DEF_BUF_SIZE", 16384},
        {"Z_NO_COMPRESSION", Z_NO_COMPRESSION}, {"Z_BEST_SPEED", Z_BEST_SPEED}, {"Z_BEST_COMPRESSION", Z_BEST_COMPRESSION},
        {"Z_DEFAULT_COMPRESSION", Z_DEFAULT_COMPRESSION}, {"Z_FILTERED", Z_FILTERED}, {"Z_HUFFMAN_ONLY", Z_HUFFMAN_ONLY},
        {"Z_RLE", Z_RLE}, {"Z_FIXED", Z_FIXED}, {"Z_DEFAULT_STRATEGY", Z_DEFAULT_STRATEGY}, {"Z_NO_FLUSH", Z_NO_FLUSH},
        {"Z_PARTIAL_FLUSH", Z_PARTIAL_FLUSH}, {"Z_SYNC_FLUSH", Z_SYNC_FLUSH}, {"Z_FULL_FLUSH", Z_FULL_FLUSH},
        {"Z_FINISH", Z_FINISH}, {"Z_BLOCK", Z_BLOCK}, {"Z_TREES", Z_TREES}};
    for (auto& c : consts) mod_set(m, c.n, var(c.v));
    mod_set(m, "ZLIB_VERSION", new_str(ZLIB_VERSION));
    mod_set(m, "ZLIB_RUNTIME_VERSION", new_str(zlibVersion()));
    mod_def(m, "crc32", [](const CallArgs& a) {
        const std::string& b = raw_of(a.pos.at(0));
        uLong c = a.pos.size() > 1 ? (uLong)(a.pos[1].as_int64() & 0xFFFFFFFF) : 0;
        return var((int64_t)crc32_z(c, (const Bytef*)b.data(), b.size()));
    });
    mod_def(m, "adler32", [](const CallArgs& a) {
        const std::string& b = raw_of(a.pos.at(0));
        uLong c = a.pos.size() > 1 ? (uLong)(a.pos[1].as_int64() & 0xFFFFFFFF) : 1;
        return var((int64_t)adler32_z(c, (const Bytef*)b.data(), b.size()));
    });
    mod_def(m, "compress", [](const CallArgs& a) {
        z_stream s{};
        int level = (int)arg(a, 1, "level", var((int64_t)Z_DEFAULT_COMPRESSION)).as_int64();
        int wbits = (int)arg(a, 2, "wbits", var((int64_t)MAX_WBITS)).as_int64();
        int rc = deflateInit2(&s, level, Z_DEFLATED, wbits, 8, Z_DEFAULT_STRATEGY);
        if (rc != Z_OK) raise(zerror(rc, "while compressing data", &s));
        std::string out = zrun_deflate(s, raw_of(a.pos.at(0)), Z_FINISH);
        deflateEnd(&s);
        return new_bytes(out);
    });
    mod_def(m, "decompress", [](const CallArgs& a) {
        z_stream s{};
        int wbits = (int)arg(a, 1, "wbits", var((int64_t)MAX_WBITS)).as_int64();
        int rc = inflateInit2(&s, wbits);
        if (rc != Z_OK) raise(zerror(rc, "while preparing to decompress data", &s));
        const std::string& in = raw_of(a.pos.at(0));
        s.next_in = (Bytef*)in.data();
        s.avail_in = (uInt)in.size();
        std::string out;
        char buf[65536];
        for (;;) {
            s.next_out = (Bytef*)buf;
            s.avail_out = sizeof buf;
            rc = inflate(&s, Z_NO_FLUSH);
            out.append(buf, sizeof buf - s.avail_out);
            if (rc == Z_STREAM_END) break;
            if (rc == Z_BUF_ERROR && s.avail_in == 0) {
                inflateEnd(&s);
                raise(zerror(-5, "while decompressing data: incomplete or truncated stream", nullptr));
            }
            if (rc != Z_OK && rc != Z_BUF_ERROR) {
                var e = zerror(rc, "while decompressing data", &s);
                inflateEnd(&s);
                raise(e);
            }
        }
        inflateEnd(&s);
        return new_bytes(out);
    });
    static TypeObj* T_c = new_type("Compress", {}, Layout::NativeData, true, "zlib");
    static TypeObj* T_d = new_type("Decompress", {}, Layout::NativeData, true, "zlib");
    mod_def(m, "compressobj", [](const CallArgs& a) {
        auto* z = new ZObj();
        z->type = T_c;
        z->compress = true;
        int level = (int)arg(a, 0, "level", var((int64_t)Z_DEFAULT_COMPRESSION)).as_int64();
        int method = (int)arg(a, 1, "method", var((int64_t)Z_DEFLATED)).as_int64();
        int wbits = (int)arg(a, 2, "wbits", var((int64_t)MAX_WBITS)).as_int64();
        int mem = (int)arg(a, 3, "memLevel", var((int64_t)8)).as_int64();
        int strat = (int)arg(a, 4, "strategy", var((int64_t)Z_DEFAULT_STRATEGY)).as_int64();
        var self(static_cast<Obj*>(z));
        int rc = deflateInit2(&z->s, level, method, wbits, mem, strat);
        if (rc != Z_OK) { z->ended = true; raise(zerror(rc, "while creating compression object", &z->s)); }
        return self;
    });
    add_method(T_c, "compress", [](const CallArgs& a) { return new_bytes(zrun_deflate(ZO(a.pos[0])->s, raw_of(a.pos.at(1)), Z_NO_FLUSH)); });
    add_method(T_c, "flush", [](const CallArgs& a) {
        ZObj* z = ZO(a.pos[0]);
        int mode = a.pos.size() > 1 ? (int)a.pos[1].as_int64() : Z_FINISH;
        if (mode == Z_NO_FLUSH) return new_bytes("");
        std::string out = zrun_deflate(z->s, "", mode);
        if (mode == Z_FINISH) { deflateEnd(&z->s); z->ended = true; }
        return new_bytes(out);
    });
    mod_def(m, "decompressobj", [](const CallArgs& a) {
        auto* z = new ZObj();
        z->type = T_d;
        z->unused_data = new_bytes("");
        z->unconsumed_tail = new_bytes("");
        int wbits = (int)arg(a, 0, "wbits", var((int64_t)MAX_WBITS)).as_int64();
        var self(static_cast<Obj*>(z));
        int rc = inflateInit2(&z->s, wbits);
        if (rc != Z_OK) { z->ended = true; raise(zerror(rc, "while creating decompression object", &z->s)); }
        return self;
    });
    add_method(T_d, "decompress", [](const CallArgs& a) {
        ZObj* z = ZO(a.pos[0]);
        std::string in = raw_of(a.pos.at(1));
        int64_t max_length = arg(a, 2, "max_length", var(0)).as_int64();
        if (z->eof) { z->unused_data = new_bytes(raw_of(z->unused_data) + in); return new_bytes(""); }
        z->s.next_in = (Bytef*)in.data();
        z->s.avail_in = (uInt)in.size();
        std::string out;
        char buf[65536];
        for (;;) {
            size_t room = sizeof buf;
            if (max_length > 0 && (int64_t)(out.size() + room) > max_length) room = (size_t)(max_length - (int64_t)out.size());
            if (room == 0) break;
            z->s.next_out = (Bytef*)buf;
            z->s.avail_out = (uInt)room;
            int rc = inflate(&z->s, Z_SYNC_FLUSH);
            out.append(buf, room - z->s.avail_out);
            if (rc == Z_STREAM_END) { z->eof = true; break; }
            if (rc == Z_BUF_ERROR || (rc == Z_OK && z->s.avail_in == 0 && z->s.avail_out != 0)) break;
            if (rc != Z_OK) raise(zerror(rc, "while decompressing data", &z->s));
        }
        std::string rest((const char*)z->s.next_in, z->s.avail_in);
        if (z->eof) { z->unused_data = new_bytes(raw_of(z->unused_data) + rest); z->unconsumed_tail = new_bytes(""); }
        else z->unconsumed_tail = new_bytes(rest);
        return new_bytes(out);
    });
    add_method(T_d, "flush", [](const CallArgs& a) {
        ZObj* z = ZO(a.pos[0]);
        std::string tail = raw_of(z->unconsumed_tail);
        if (tail.empty()) return new_bytes("");
        return a.pos[0].call("decompress", new_bytes(tail));
    });
    // _ZlibDecompressor (gzip's reader): input kept between calls, max_length bounding each answer,
    // needs_input saying whether more input is wanted (the _bz2 / _lzma decompressor protocol)
    static TypeObj* T_zd = new_type("_ZlibDecompressor", {}, Layout::NativeData, true, "zlib");
    add_static(T_zd, "__new__", [](const CallArgs& a) {
        auto* z = new ZObj();
        z->type = T_zd;
        z->unused_data = new_bytes("");
        z->unconsumed_tail = new_bytes("");   // (the input not yet given to inflate)
        int wbits = (int)arg(a, 1, "wbits", var((int64_t)MAX_WBITS)).as_int64();
        var self(static_cast<Obj*>(z));
        self.setattr("needs_input", true);
        int rc = inflateInit2(&z->s, wbits);
        if (rc != Z_OK) { z->ended = true; raise(zerror(rc, "while creating decompression object", &z->s)); }
        return self;
    });
    add_method(T_zd, "decompress", [](const CallArgs& a) {
        ZObj* z = ZO(a.pos[0]);
        if (z->eof) raise(E_EOFError, "End of stream already reached");
        std::string in = raw_of(z->unconsumed_tail) + raw_of(a.pos.at(1));
        int64_t max_length = arg(a, 2, "max_length", var(-1)).as_int64();
        z->s.next_in = (Bytef*)in.data();
        z->s.avail_in = (uInt)in.size();
        std::string out;
        char buf[65536];
        bool limited = false;
        for (;;) {
            size_t room = sizeof buf;
            if (max_length >= 0 && (int64_t)(out.size() + room) > max_length) room = (size_t)(max_length - (int64_t)out.size());
            if (room == 0) { limited = true; break; }
            z->s.next_out = (Bytef*)buf;
            z->s.avail_out = (uInt)room;
            int rc = inflate(&z->s, Z_SYNC_FLUSH);
            out.append(buf, room - z->s.avail_out);
            if (rc == Z_STREAM_END) { z->eof = true; break; }
            if (rc == Z_BUF_ERROR || (z->s.avail_in == 0 && z->s.avail_out != 0)) break;
            if (rc != Z_OK) raise(zerror(rc, "while decompressing data", &z->s));
        }
        std::string rest((const char*)z->s.next_in, z->s.avail_in);
        if (z->eof) { z->unused_data = new_bytes(rest); z->unconsumed_tail = new_bytes(""); }
        else z->unconsumed_tail = new_bytes(rest);
        a.pos[0].setattr("needs_input", var(!z->eof && !limited && rest.empty()));
        return new_bytes(out);
    });
    add_getter(T_zd, "unused_data", [](const var& s) { return ZO(s)->unused_data; });
    add_getter(T_zd, "eof", [](const var& s) { return var(ZO(s)->eof); });
    mod_set(m, "_ZlibDecompressor", tv(T_zd));
    add_getter(T_d, "unused_data", [](const var& s) { return ZO(s)->unused_data; });
    add_getter(T_d, "unconsumed_tail", [](const var& s) { return ZO(s)->unconsumed_tail; });
    add_getter(T_d, "eof", [](const var& s) { return var(ZO(s)->eof); });
}

// ------------------------------------------------------------------------------------------ small ones
static void init_operator_c(const var& m) {
    mod_def(m, "_compare_digest", [](const CallArgs& a) {
        // constant time over the longer, as CPython's
        std::string x = is_str(a.pos.at(0)) ? str_utf8(a.pos[0]) : raw_of(a.pos[0]);
        std::string y = is_str(a.pos.at(1)) ? str_utf8(a.pos[1]) : raw_of(a.pos[1]);
        unsigned r = x.size() != y.size();
        for (size_t i = 0; i < x.size() && i < y.size(); i++) r |= (unsigned char)x[i] ^ (unsigned char)y[i];
        return var(r == 0);
    });
}
static void init_msvcrt(const var& m) {
    mod_set(m, "LK_UNLCK", var((int64_t)_LK_UNLCK));
    mod_set(m, "LK_LOCK", var((int64_t)_LK_LOCK));
    mod_set(m, "LK_NBLCK", var((int64_t)_LK_NBLCK));
    mod_set(m, "LK_RLCK", var((int64_t)_LK_RLCK));
    mod_set(m, "LK_NBRLCK", var((int64_t)_LK_NBRLCK));
    mod_def(m, "locking", [](const CallArgs& a) {
        int fd = (int)a.pos.at(0).as_int64();
        int mode = (int)a.pos.at(1).as_int64();
        long n = (long)a.pos.at(2).as_int64();
        int rc;
        {
            gil_release g;
            rc = _locking(fd, mode, n);
        }
        if (rc != 0) {
            int e = errno;
            raise(make_exc_args(E_OSError, {var((int64_t)e), new_str(strerror(e))}));
        }
        return None;
    });
    mod_def(m, "kbhit", [](const CallArgs&) { return var(_kbhit() != 0); });
    mod_def(m, "getch", [](const CallArgs&) { return new_bytes(std::string(1, (char)_getch())); });
    mod_def(m, "getwch", [](const CallArgs&) { std::string s; append_utf8(s, (char32_t)_getwch()); return new_str(s); });
    mod_def(m, "setmode", [](const CallArgs& a) { return var((int64_t)_setmode((int)a.pos.at(0).as_int64(), (int)a.pos.at(1).as_int64())); });
    mod_def(m, "get_osfhandle", [](const CallArgs& a) { return var((int64_t)_get_osfhandle((int)a.pos.at(0).as_int64())); });
}
static void init_faulthandler(const var& m) {
    // tracebacks of a translated program are its own; these keep their callers working
    for (const char* n : {"enable", "disable", "dump_traceback", "dump_traceback_later", "cancel_dump_traceback_later", "register", "unregister"})
        mod_def(m, n, [](const CallArgs&) { return None; });
    mod_def(m, "is_enabled", [](const CallArgs&) { return var(false); });
}
static void init_signal(const var& m) {
    struct { const char* n; int64_t v; } sigs[] = {{"SIGINT", 2}, {"SIGILL", 4}, {"SIGFPE", 8}, {"SIGSEGV", 11}, {"SIGTERM", 15},
                                                   {"SIGBREAK", 21}, {"SIGABRT", 22}, {"CTRL_C_EVENT", 0}, {"CTRL_BREAK_EVENT", 1}};
    for (auto& s : sigs) mod_set(m, s.n, var(s.v));
    mod_set(m, "SIG_DFL", var(0));
    mod_set(m, "SIG_IGN", var(1));
    static var handlers = new_dict();
    mod_def(m, "signal", [](const CallArgs& a) {
        var old = handlers.call("get", a.pos.at(0), var(0));
        handlers.setitem(a.pos[0], a.pos.at(1));
        return old;
    });
    mod_def(m, "getsignal", [](const CallArgs& a) { return handlers.call("get", a.pos.at(0), var(0)); });
    mod_def(m, "default_int_handler", [](const CallArgs&) -> var { raise(E_KeyboardInterrupt, ""); });
    // the file (a socket on Windows) a signal's number is written to (asyncio's loops wake on it): kept, given back
    static int64_t wakeup_fd = -1;
    mod_def(m, "set_wakeup_fd", [](const CallArgs& a) {
        int64_t old = wakeup_fd;
        wakeup_fd = a.pos.at(0).as_int64();
        return var(old);
    });
    mod_def(m, "valid_signals", [](const CallArgs&) {
        var s = new_set();
        for (int64_t n : {2, 4, 8, 11, 15, 21, 22}) s.call("add", var(n));
        return s;
    });
}
static void init_weakref(const var& m) {
    // references are strong here (the runtime counts references and has no cycle collector, so a weak
    // reference could only differ by dying sooner): ref(x)() is x, the containers are their builtins
    mod_def(m, "ref", [](const CallArgs& a) {
        var x = a.pos.at(0);
        return nat("weakref", [x](const CallArgs&) { return x; });
    });
    mod_def(m, "proxy", [](const CallArgs& a) { return a.pos.at(0); });
    mod_set(m, "WeakValueDictionary", t::dict);
    mod_set(m, "WeakKeyDictionary", t::dict);
    mod_set(m, "WeakSet", t::set);
    mod_set(m, "ReferenceType", t::function);
    var fin = mkcls("finalize", {}, {}, "weakref");
    mod_set(m, "finalize", fin);
}
static void init_locale(const var& m) {
    mod_set(m, "LC_ALL", var(0));
    mod_set(m, "LC_CTYPE", var(2));
    mod_set(m, "LC_NUMERIC", var(4));
    mod_set(m, "Error", mkcls("Error", {t::Exception}, {}, "locale"));
    mod_def(m, "setlocale", [](const CallArgs&) { return "C"_s; });
    mod_def(m, "getpreferredencoding", [](const CallArgs&) { return "utf-8"_s; });
    mod_def(m, "getencoding", [](const CallArgs&) { return "utf-8"_s; });
    mod_def(m, "getlocale", [](const CallArgs&) { return new_tuple({None, None}); });
    mod_def(m, "localeconv", [](const CallArgs&) {
        var d = new_dict();
        d.setitem("decimal_point"_s, "."_s);
        d.setitem("thousands_sep"_s, ""_s);
        d.setitem("grouping"_s, new_list());
        return d;
    });
}

// ------------------------------------------------------------------------------------------ codecs
// The encodings str.encode and bytes.decode know (utf-8, latin-1, ascii, cp437 ...), as codecs' API:
// lookup, encode/decode, the incremental coders (a UTF-8 decoder keeps a split sequence for the next
// call), the BOMs and the error handler registry.
static std::string norm_encoding(std::string e) {
    for (auto& c : e) c = c == '-' || c == ' ' ? '_' : (char)tolower((unsigned char)c);
    if (e == "utf8" || e == "u8" || e == "cp65001") return "utf_8";
    if (e == "latin_1" || e == "latin1" || e == "iso8859_1" || e == "iso_8859_1" || e == "l1") return "latin_1";
    if (e == "us_ascii" || e == "646") return "ascii";
    return e;
}
static void init_codecs(const var& m) {
    var info = mkcls("CodecInfo", {t::tuple}, {}, "codecs");
    mod_set(m, "CodecInfo", info);
    mod_set(m, "BOM_UTF8", new_bytes("\xEF\xBB\xBF"));
    mod_set(m, "BOM_UTF16_LE", new_bytes(std::string("\xFF\xFE", 2)));
    mod_set(m, "BOM_UTF16_BE", new_bytes(std::string("\xFE\xFF", 2)));
    mod_set(m, "BOM_UTF16", new_bytes(std::string("\xFF\xFE", 2)));
    mod_set(m, "BOM_LE", new_bytes(std::string("\xFF\xFE", 2)));
    mod_set(m, "BOM_BE", new_bytes(std::string("\xFE\xFF", 2)));
    mod_set(m, "BOM_UTF32_LE", new_bytes(std::string("\xFF\xFE\0\0", 4)));
    mod_set(m, "BOM_UTF32_BE", new_bytes(std::string("\0\0\xFE\xFF", 4)));
    mod_def(m, "encode", [](const CallArgs& a) {
        return a.pos.at(0).call("encode", arg(a, 1, "encoding", "utf-8"_s), arg(a, 2, "errors", "strict"_s));
    });
    mod_def(m, "decode", [](const CallArgs& a) {
        return a.pos.at(0).call("decode", arg(a, 1, "encoding", "utf-8"_s), arg(a, 2, "errors", "strict"_s));
    });
    mod_def(m, "lookup", [](const CallArgs& a) -> var {
        std::string e = norm_encoding(str_utf8(a.pos.at(0)));
        try {
            ""_s.call("encode", new_str(e));
        } catch (error& x) {
            if (isinstance_(x.value, E_LookupError)) raise(E_LookupError, "unknown encoding: " + str_utf8(a.pos[0]));
            throw;
        }
        var ci = call1(import("codecs").getattr("CodecInfo"), new_tuple({None, None, None, None}));
        ci.setattr("name", new_str(e == "utf_8" ? "utf-8" : e == "latin_1" ? "iso8859-1" : e));
        return ci;
    });
    // incremental coders: classes, as codecs.getincrementaldecoder(enc) gives
    var dns = new_dict();
    dns.setitem("__init__"_s, def("__init__", "self, errors=", {"strict"_s}, [](var self, var errors) {
        self.setattr("errors", errors);
        self.setattr("_pending", new_bytes(""));
        return var(None);
    }));
    dns.setitem("decode"_s, def("decode", "self, input, final=", {var(false)}, [](var self, var input, var final_) {
        std::string b = raw_of(self.getattr("_pending")) + raw_of(input);
        std::string enc = str_utf8(self.getattr("_encoding"));
        size_t keep = 0;
        if (enc == "utf_8" && !final_.truthy()) {
            // a sequence cut at the end: kept for the next call
            size_t n = b.size();
            for (size_t back = 1; back <= 3 && back <= n; back++) {
                unsigned char c = (unsigned char)b[n - back];
                if ((c & 0xC0) == 0x80) continue;
                size_t need = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
                if (need > back) keep = back;
                break;
            }
        }
        self.setattr("_pending", new_bytes(b.substr(b.size() - keep)));
        return new_bytes(b.substr(0, b.size() - keep)).call("decode", new_str(enc), self.getattr("errors"));
    }));
    dns.setitem("reset"_s, def("reset", "self", {}, [](var self) { self.setattr("_pending", new_bytes("")); return var(None); }));
    dns.setitem("getstate"_s, def("getstate", "self", {}, [](var self) { return new_tuple({self.getattr("_pending"), var(0)}); }));
    dns.setitem("setstate"_s, def("setstate", "self, state", {}, [](var self, var st) { self.setattr("_pending", st[var(0)]); return var(None); }));
    dns.setitem("_encoding"_s, "utf_8"_s);
    var dec = make_class("IncrementalDecoder", "IncrementalDecoder", {}, dns, {}, "codecs");
    mod_set(m, "IncrementalDecoder", dec);
    var ens = new_dict();
    ens.setitem("__init__"_s, def("__init__", "self, errors=", {"strict"_s}, [](var self, var errors) { self.setattr("errors", errors); return var(None); }));
    ens.setitem("encode"_s, def("encode", "self, input, final=", {var(false)}, [](var self, var input, var) {
        return input.call("encode", self.getattr("_encoding"), self.getattr("errors"));
    }));
    ens.setitem("reset"_s, def("reset", "self", {}, [](var) { return var(None); }));
    ens.setitem("getstate"_s, def("getstate", "self", {}, [](var) { return var(0); }));
    ens.setitem("setstate"_s, def("setstate", "self, state", {}, [](var, var) { return var(None); }));
    ens.setitem("_encoding"_s, "utf_8"_s);
    var encc = make_class("IncrementalEncoder", "IncrementalEncoder", {}, ens, {}, "codecs");
    mod_set(m, "IncrementalEncoder", encc);
    mod_def(m, "getincrementaldecoder", [dec](const CallArgs& a) {
        std::string e = norm_encoding(str_utf8(a.pos.at(0)));
        var ns = new_dict();
        ns.setitem("_encoding"_s, new_str(e));
        return make_class("IncrementalDecoder", "IncrementalDecoder", {dec}, ns, {}, "codecs");
    });
    mod_def(m, "getincrementalencoder", [encc](const CallArgs& a) {
        std::string e = norm_encoding(str_utf8(a.pos.at(0)));
        var ns = new_dict();
        ns.setitem("_encoding"_s, new_str(e));
        return make_class("IncrementalEncoder", "IncrementalEncoder", {encc}, ns, {}, "codecs");
    });
    static var handlers = new_dict();
    mod_def(m, "register_error", [](const CallArgs& a) { handlers.setitem(a.pos.at(0), a.pos.at(1)); return None; });
    mod_def(m, "lookup_error", [](const CallArgs& a) -> var {
        var* h = static_cast<DictObj*>(handlers.ref())->get(a.pos.at(0));
        if (h) return *h;
        std::string n = str_utf8(a.pos[0]);
        for (const char* k : {"strict", "ignore", "replace", "backslashreplace", "surrogateescape", "surrogatepass", "xmlcharrefreplace", "namereplace"})
            if (n == k) return nat(k, [](const CallArgs& b) -> var { raise(b.pos.at(0)); });
        raise(E_LookupError, "unknown error handler name '" + n + "'");
    });
    mod_def(m, "utf_8_decode", [](const CallArgs& a) {
        var b = a.pos.at(0);
        return new_tuple({b.call("decode", "utf-8"_s, arg(a, 1, "errors", "strict"_s)), var(len_(b))});
    });
}

// _collections: the C half collections/__init__.py imports (the runtime's own deque, defaultdict, OrderedDict)
static void init_collections_c(const var& m) {
    var c = import("collections");
    for (const char* n : {"deque", "defaultdict", "OrderedDict"}) mod_set(m, n, c.getattr(n));
    mod_def(m, "_count_elements", [](const CallArgs& a) {
        var mapping = a.pos.at(0);
        var get = mapping.getattr("get");
        for (var x : each(a.pos.at(1))) mapping.setitem(x, call2(get, x, var(0)) + var(1));
        return None;
    });
}

void init_ctypes(const var& m);
void init_ctypes_util(const var& m);
void init_wintypes(const var& m);
void init_pytest(const var& m);
void init_unittest(const var& m);
void init_crypto_modules();
void init_async_modules();

// memoryview: a view of bytes, a bytearray or a ctypes object (raw_of reads it through .obj)
static void make_memoryview() {
    var ns = new_dict();
    ns.setitem("__init__"_s, def("__init__", "self, obj", {}, [](var self, var obj) {
        if (!is_bytes(obj) && !has_layout(obj, Layout::ByteArray) && !hasattr(obj, "__bytes__"_s).truthy() &&
            !(obj.is_ref() && type_of(obj)->name == "memoryview"))
            raise(E_TypeError, "memoryview: a bytes-like object is required, not '" + tname(obj) + "'");
        if (obj.is_ref() && type_of(obj)->name == "memoryview") obj = obj.getattr("obj");
        self.setattr("obj", obj);
        self.setattr("readonly", var(is_bytes(obj) && !has_layout(obj, Layout::ByteArray)));
        self.setattr("itemsize", var(1));
        self.setattr("format", "B"_s);
        self.setattr("ndim", var(1));
        return var(None);
    }));
    auto raw = [](const var& self) -> std::string {
        var o = self.getattr("obj");
        if (is_bytes(o) || has_layout(o, Layout::ByteArray)) return raw_of(o);
        return raw_of(call1(t::bytes, o));
    };
    ns.setitem("tobytes"_s, def("tobytes", "self, order=", {None}, [raw](var self, var) { return new_bytes(raw(self)); }));
    ns.setitem("__bytes__"_s, def("__bytes__", "self", {}, [raw](var self) { return new_bytes(raw(self)); }));
    ns.setitem("hex"_s, def("hex", "self", {}, [raw](var self) { return new_bytes(raw(self)).call("hex"); }));
    ns.setitem("tolist"_s, def("tolist", "self", {}, [raw](var self) { return call1(t::list, new_bytes(raw(self))); }));
    ns.setitem("__len__"_s, def("__len__", "self", {}, [raw](var self) { return var((int64_t)raw(self).size()); }));
    ns.setitem("nbytes"_s, property(nat("nbytes", [raw](const CallArgs& a) { return var((int64_t)raw(a.pos.at(0)).size()); })));
    ns.setitem("__getitem__"_s, def("__getitem__", "self, key", {}, [raw](var self, var k) -> var {
        var b = new_bytes(raw(self));
        if (has_layout(k, Layout::Slice)) return call1(import("builtins").getattr("memoryview"), b.getitem(k));
        return b.getitem(k);
    }));
    ns.setitem("__setitem__"_s, def("__setitem__", "self, key, value", {}, [](var self, var k, var v) {
        var o = self.getattr("obj");
        if (!has_layout(o, Layout::ByteArray)) raise(E_TypeError, "cannot modify read-only memory");
        o.setitem(k, v);
        return var(None);
    }));
    ns.setitem("__eq__"_s, def("__eq__", "self, other", {}, [raw](var self, var o) { return var(raw(self) == raw_of(is_bytes(o) ? o : call1(t::bytes, o))); }));
    ns.setitem("cast"_s, def("cast", "self, format, shape=", {None}, [](var self, var, var) { return self; }));
    ns.setitem("release"_s, def("release", "self", {}, [](var) { return var(None); }));
    ns.setitem("__enter__"_s, def("__enter__", "self", {}, [](var self) { return self; }));
    ns.setitem("__exit__"_s, def("__exit__", "self, *args", {}, [](var, var) { return var(false); }));
    var mv = make_class("memoryview", "memoryview", {}, ns, {}, "builtins");
    extern var g_builtins;
    mod_set(g_builtins, "memoryview", mv);
    t::memoryview = mv;
}

// ------------------------------------------------------------------------------------------ jsonschema
// Draft 2020-12 validation over the keywords CodeBreaker's schemas use (L2's: type, properties, required,
// additionalProperties, items, minItems, enum, const, anyOf / oneOf / allOf, if / then / else, minimum,
// $ref into the document's $defs): validate() raises ValidationError with the first failure
namespace {
struct SchemaCheck {
    var root;
    std::string why;
    var resolve(const std::string& ref) {
        if (ref.empty() || ref[0] != '#') raise(E_NotImplementedError, "jsonschema: only local $ref (" + ref + ")");
        var s = root;
        size_t i = 1;
        while (i < ref.size()) {
            if (ref[i] == '/') i++;
            size_t j = ref.find('/', i);
            std::string part = ref.substr(i, j == std::string::npos ? std::string::npos : j - i);
            s = s.getitem(new_str(part));
            if (j == std::string::npos) break;
            i = j;
        }
        return s;
    }
    bool type_ok(const std::string& t, const var& v) {
        if (t == "string") return is_str(v);
        if (t == "integer") return (is_int(v) && !v.is_bool()) || (v.is_float() && v.as_double() == std::floor(v.as_double()));
        if (t == "number") return (is_int(v) && !v.is_bool()) || v.is_float();
        if (t == "boolean") return v.is_bool();
        if (t == "object") return is_dict(v);
        if (t == "array") return is_list(v);
        if (t == "null") return v.is_none();
        return true;
    }
    bool ok(const var& s, const var& v, const std::string& at) {
        if (s.is_bool()) { if (!s.truthy()) why = at + ": False schema does not allow " + repr_(v); return s.truthy(); }
        if (!is_dict(s)) return true;
        auto has = [&](const char* k) { return in(new_str(k), s); };
        auto get = [&](const char* k) { return s.getitem(new_str(k)); };
        if (has("$ref") && !ok(resolve(str_utf8(get("$ref"))), v, at)) return false;
        if (has("type")) {
            var t = get("type");
            bool any = false;
            if (is_str(t)) any = type_ok(str_utf8(t), v);
            else for (var x : each(t)) if (type_ok(str_utf8(x), v)) any = true;
            if (!any) { why = at + ": " + repr_(v) + " is not of type " + repr_(t); return false; }
        }
        if (has("const") && !(get("const") == v && (get("const").is_bool() == v.is_bool()))) { why = at + ": " + repr_(get("const")) + " was expected"; return false; }
        if (has("enum")) {
            bool found = false;
            for (var x : each(get("enum"))) if (x == v && x.is_bool() == v.is_bool()) found = true;
            if (!found) { why = at + ": " + repr_(v) + " is not one of " + repr_(get("enum")); return false; }
        }
        if (has("minimum") && (is_int(v) || v.is_float()) && v < get("minimum")) { why = at + ": " + repr_(v) + " is less than the minimum of " + repr_(get("minimum")); return false; }
        if (is_dict(v)) {
            if (has("required"))
                for (var k : each(get("required")))
                    if (!in(k, v)) { why = at + ": " + repr_(k) + " is a required property"; return false; }
            var props = has("properties") ? get("properties") : var(None);
            for (var k : each(v)) {
                std::string p = at + "/" + str_(k);
                if (!props.is_none() && in(k, props)) { if (!ok(props.getitem(k), v.getitem(k), p)) return false; }
                else if (has("additionalProperties")) {
                    var ap = get("additionalProperties");
                    if (ap.is_bool() && !ap.truthy()) { why = at + ": Additional properties are not allowed (" + repr_(k) + " was unexpected)"; return false; }
                    if (!ap.is_bool() && !ok(ap, v.getitem(k), p)) return false;
                }
            }
        }
        if (is_list(v)) {
            if (has("minItems") && len_(v) < get("minItems").as_int64()) { why = at + ": " + repr_(v) + " is too short"; return false; }
            if (has("items")) {
                int64_t i = 0;
                for (var x : each(v)) if (!ok(get("items"), x, at + "/" + std::to_string(i++))) return false;
            }
        }
        if (has("allOf")) for (var x : each(get("allOf"))) if (!ok(x, v, at)) return false;
        if (has("anyOf")) {
            bool any = false;
            std::string keep = why;
            for (var x : each(get("anyOf"))) if (ok(x, v, at)) { any = true; break; }
            if (!any) { why = at + ": " + repr_(v) + " is not valid under any of the given schemas"; return false; }
            why = keep;
        }
        if (has("oneOf")) {
            int n = 0;
            for (var x : each(get("oneOf"))) if (ok(x, v, at)) n++;
            if (n != 1) { why = at + ": " + repr_(v) + (n ? " is valid under each of several schemas" : " is not valid under any of the given schemas"); return false; }
        }
        if (has("if")) {
            std::string keep = why;
            bool cond = ok(get("if"), v, at);
            why = keep;
            if (cond && has("then") && !ok(get("then"), v, at)) return false;
            if (!cond && has("else") && !ok(get("else"), v, at)) return false;
        }
        return true;
    }
};
}  // namespace
static void init_jsonschema(const var& m) {
    var err = mkcls("ValidationError", {t::Exception}, {}, "jsonschema.exceptions");
    mod_set(m, "ValidationError", err);
    mod_set(m, "SchemaError", mkcls("SchemaError", {t::Exception}, {}, "jsonschema.exceptions"));
    var ns = new_dict();
    ns.setitem("__init__"_s, def("__init__", "self, schema", {}, [](var self, var schema) { self.setattr("schema", schema); return var(None); }));
    ns.setitem("validate"_s, def("validate", "self, instance", {}, [err](var self, var inst) {
        SchemaCheck c;
        c.root = self.getattr("schema");
        if (!c.ok(c.root, inst, "$")) raise(make_exc_args(T(err), {new_str(c.why)}));
        return var(None);
    }));
    ns.setitem("is_valid"_s, def("is_valid", "self, instance", {}, [](var self, var inst) {
        SchemaCheck c;
        c.root = self.getattr("schema");
        return var(c.ok(c.root, inst, "$"));
    }));
    var v = make_class("Draft202012Validator", "Draft202012Validator", {}, ns, {}, "jsonschema");
    add_static(T(v), "check_schema", [](const CallArgs& a) -> var {
        if (!is_dict(a.pos.at(0)) && !a.pos[0].is_bool()) raise(T(import("jsonschema").getattr("SchemaError")), "a schema is an object or a boolean");
        return None;
    });
    mod_set(m, "Draft202012Validator", v);
    mod_set(m, "Draft7Validator", v);
    mod_def(m, "validate", [v](const CallArgs& a) { call1(v, a.pos.at(1)).call("validate", a.pos.at(0)); return None; });
}

var eval_expr_(const var& src, const var& globals, const var& locals);   // lib_ast.cc
var run_module_(const std::string& name, const std::string& run_name);  // module.cc
const char* module_for_file_(const std::string& path);                  // module.cc

void init_extra_modules() {
    make_memoryview();
    {
        // eval of an expression (lib_ast.cc evaluates pyfront's tree); exec / compile still need Python
        extern var g_builtins;
        mod_def(g_builtins, "eval", [](const CallArgs& a) {
            return eval_expr_(a.pos.at(0), arg(a, 1, "globals", None), arg(a, 2, "locals", None));
        });
    }
    register_native_module("runpy", [](const var& m) {
        mod_def(m, "run_module", [](const CallArgs& a) {
            var run_name = kwarg(a, "run_name", a.pos.size() > 2 ? a.pos[2] : var(None));
            std::string mod = str_utf8(a.pos.at(0));
            var g = run_module_(mod, run_name.is_none() ? mod : str_utf8(run_name));
            return g;
        });
        mod_def(m, "run_path", [](const CallArgs& a) -> var {
            const char* mn = module_for_file_(fs_path(a.pos.at(0)));
            if (!mn) raise(E_NotImplementedError, "runpy.run_path: " + fs_path(a.pos[0]) + " is not a module of this program");
            var run_name = kwarg(a, "run_name", var("<run_path>"_s));
            return run_module_(mn, str_utf8(run_name));
        });
    });
    register_native_module("ctypes", init_ctypes);          // (lib_ctypes.cc)
    register_native_module("ctypes.util", init_ctypes_util);
    register_native_module("ctypes.wintypes", init_wintypes);
    register_native_module("pytest", init_pytest);          // (lib_pytest.cc: the API and the runner)
    register_native_module("unittest", init_unittest);      // (lib_pytest.cc: TestCase, the skips, main)
    init_crypto_modules();                                  // (lib_crypto.cc: cryptography / Crypto over bcrypt)
    init_async_modules();                                   // (lib_async.cc: sys's asyncgen hooks; _socket, select, _overlapped, _winapi)
    register_native_module("jsonschema", init_jsonschema);
    // z3: not in the C++ runtime until libz3 is vendored (a decision pending): importing it says so
    register_native_module("z3", [](const var&) {
        raise(E_ImportError, "z3 is not in the C++ runtime: libz3 is not vendored yet (emulate/symbolic.py and "
                             "verify/concolic.py need it; its C API is the way in once it is)");
    });
    register_native_module("codecs", init_codecs);
    register_native_module("_collections", init_collections_c);
    register_native_module("gettext", init_gettext);
    register_native_module("_random", init_random_c);
    register_native_module("_csv", init_csv_c);
    register_native_module("zlib", init_zlib_c);
    register_native_module("_operator", init_operator_c);
    register_native_module("msvcrt", init_msvcrt);
    register_native_module("faulthandler", init_faulthandler);
    register_native_module("signal", init_signal);
    register_native_module("_signal", init_signal);
    register_native_module("weakref", init_weakref);
    register_native_module("_weakref", init_weakref);
    register_native_module("locale", init_locale);
}

}  // namespace py
