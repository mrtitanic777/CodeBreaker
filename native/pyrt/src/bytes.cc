// bytes.cc -- bytes and bytearray (an item is an int, slices clamp, the mutating methods of
// bytearray), and the codecs str.encode / bytes.decode use: utf-8, ascii, latin-1, cp437, cp1252,
// utf-16(-le/-be) with the errors strict, replace, ignore, backslashreplace, surrogateescape.
#include <algorithm>
#include <cstring>

#include "rt.hpp"

namespace py {

var new_bytes(std::string raw, bool bytearray) {
    auto* o = new BytesObj();
    o->type = bytearray ? T_bytearray : T_bytes;
    o->s = std::move(raw);
    return var(static_cast<Obj*>(o));
}
var bytes_from(const std::string& raw) { return new_bytes(raw); }
bool is_bytes(const var& x) { return x.is_ref() && (x.ref()->type->layout == Layout::Bytes || x.ref()->type->layout == Layout::ByteArray); }
const std::string& raw_of(const var& b) {
    if (!is_bytes(b)) {
        if (b.is_ref() && b.ref()->type->name == "memoryview") return raw_of(b.getattr("obj"));
        raise(E_TypeError, "a bytes-like object is required, not '" + tname(b) + "'");
    }
    return static_cast<BytesObj*>(b.ref())->s;
}
const std::string& bytes_raw(const var& b) { return raw_of(b); }
static BytesObj* B(const var& v) { return static_cast<BytesObj*>(v.ref()); }
static bool is_ba(const var& v) { return v.is_ref() && v.ref()->type->layout == Layout::ByteArray; }

int64_t bytes_hash_(const BytesObj* s) {
    if (s->h != -1) return s->h;
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s->s) { h ^= c; h *= 1099511628211ULL; }
    int64_t r = (int64_t)(h >> 1);
    if (r == -1) r = -2;
    s->h = r;
    return r;
}

std::string bytes_repr_(const std::string& s, bool bytearray) {
    bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
    char q = (sq && !dq) ? '"' : '\'';
    std::string o = bytearray ? "bytearray(b" : "b";
    o.push_back(q);
    for (unsigned char c : s) {
        if (c == (unsigned char)q || c == '\\') { o.push_back('\\'); o.push_back((char)c); }
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c < 0x20 || c >= 0x7F) { char b[8]; snprintf(b, sizeof b, "\\x%02x", c); o += b; }
        else o.push_back((char)c);
    }
    o.push_back(q);
    if (bytearray) o += ")";
    return o;
}

// ------------------------------------------------------------------------------------------ codecs
static std::string norm_enc(std::string e) {
    for (auto& c : e) c = (char)tolower((unsigned char)c);
    std::replace(e.begin(), e.end(), '_', '-');
    if (e == "utf8" || e == "u8" || e == "utf") return "utf-8";
    if (e == "latin-1" || e == "latin1" || e == "iso-8859-1" || e == "iso8859-1" || e == "l1" || e == "8859") return "latin-1";
    if (e == "us-ascii" || e == "646") return "ascii";
    if (e == "437" || e == "ibm437") return "cp437";
    if (e == "windows-1252") return "cp1252";
    if (e == "utf-16le") return "utf-16-le";
    if (e == "utf-16be") return "utf-16-be";
    if (e == "utf-32le") return "utf-32-le";
    if (e == "utf-32be") return "utf-32-be";
    if (e == "utf-8-sig") return "utf-8-sig";
    return e;
}
static const std::string BOM32LE = std::string{(char)0xFF, (char)0xFE, 0, 0}, BOM32BE = std::string{0, 0, (char)0xFE, (char)0xFF};
static const char32_t cp437_hi[128] = {
    0xC7, 0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5, 0xE7, 0xEA, 0xEB, 0xE8, 0xEF, 0xEE, 0xEC, 0xC4, 0xC5,
    0xC9, 0xE6, 0xC6, 0xF4, 0xF6, 0xF2, 0xFB, 0xF9, 0xFF, 0xD6, 0xDC, 0xA2, 0xA3, 0xA5, 0x20A7, 0x192,
    0xE1, 0xED, 0xF3, 0xFA, 0xF1, 0xD1, 0xAA, 0xBA, 0xBF, 0x2310, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x3B1, 0xDF, 0x393, 0x3C0, 0x3A3, 0x3C3, 0xB5, 0x3C4, 0x3A6, 0x398, 0x3A9, 0x3B4, 0x221E, 0x3C6, 0x3B5, 0x2229,
    0x2261, 0xB1, 0x2265, 0x2264, 0x2320, 0x2321, 0xF7, 0x2248, 0xB0, 0x2219, 0xB7, 0x221A, 0x207F, 0xB2, 0x25A0, 0xA0};
static const char32_t cp1252_80[32] = {0x20AC, 0, 0x201A, 0x192, 0x201E, 0x2026, 0x2020, 0x2021, 0x2C6, 0x2030, 0x160, 0x2039, 0x152, 0, 0x17D, 0,
                                       0, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x2DC, 0x2122, 0x161, 0x203A, 0x153, 0, 0x17E, 0x178};

[[noreturn]] static void decode_error(const std::string& enc, const std::string& raw, size_t st, size_t en, const char* reason) {
    raise(make_exc_args(E_UnicodeDecodeError, {new_str(enc), new_bytes(raw), var((int64_t)st), var((int64_t)en), new_str(reason)}));
}
[[noreturn]] static void encode_error(const std::string& enc, const std::string& s, size_t st, size_t en, const char* reason) {
    raise(make_exc_args(E_UnicodeEncodeError, {new_str(enc), new_str(s), var((int64_t)st), var((int64_t)en), new_str(reason)}));
}

var bytes_decode_(const std::string& raw, const std::string& enc_in, const std::string& errors) {
    std::string enc = norm_enc(enc_in);
    std::string out;
    auto bad = [&](size_t i, size_t len, const char* reason) {
        if (errors == "strict") decode_error(enc == "utf-8" ? "utf-8" : enc, raw, i, i + len, reason);
        if (errors == "replace") append_utf8(out, 0xFFFD);
        else if (errors == "backslashreplace") { for (size_t k = 0; k < len; k++) { char b[8]; snprintf(b, sizeof b, "\\x%02x", (unsigned char)raw[i + k]); out += b; } }
        else if (errors == "surrogateescape") { for (size_t k = 0; k < len; k++) append_utf8(out, 0xDC00 + (unsigned char)raw[i + k]); }
    };
    if (enc == "utf-8" || enc == "utf-8-sig") {
        size_t i = 0, n = raw.size();
        if (enc == "utf-8-sig" && n >= 3 && raw.compare(0, 3, "\xEF\xBB\xBF") == 0) i = 3;
        while (i < n) {
            unsigned char c = (unsigned char)raw[i];
            if (c < 0x80) { out.push_back((char)c); i++; continue; }
            int len = 0;
            char32_t cp = 0, minv = 0;
            if (c >= 0xC2 && c <= 0xDF) { len = 2; cp = c & 0x1F; minv = 0x80; }
            else if (c >= 0xE0 && c <= 0xEF) { len = 3; cp = c & 0x0F; minv = 0x800; }
            else if (c >= 0xF0 && c <= 0xF4) { len = 4; cp = c & 0x07; minv = 0x10000; }
            else { bad(i, 1, "invalid start byte"); i++; continue; }
            int k = 1;
            bool ok = true;
            for (; k < len; k++) {
                if (i + k >= n) { ok = false; break; }
                unsigned char d = (unsigned char)raw[i + k];
                if ((d & 0xC0) != 0x80) { ok = false; break; }
                // the second byte's narrower ranges (overlongs and surrogates)
                if (k == 1) {
                    if (c == 0xE0 && d < 0xA0) { ok = false; break; }
                    if (c == 0xED && d > 0x9F) { ok = false; break; }
                    if (c == 0xF0 && d < 0x90) { ok = false; break; }
                    if (c == 0xF4 && d > 0x8F) { ok = false; break; }
                }
                cp = (cp << 6) | (d & 0x3F);
            }
            if (!ok) {
                bool end = i + k >= n;
                bad(i, (size_t)k, end ? "unexpected end of data" : "invalid continuation byte");
                i += (size_t)k;
                continue;
            }
            (void)minv;
            append_utf8(out, cp);
            i += (size_t)len;
        }
        return new_str(out);
    }
    if (enc == "ascii") {
        for (size_t i = 0; i < raw.size(); i++) {
            unsigned char c = (unsigned char)raw[i];
            if (c < 0x80) out.push_back((char)c);
            else bad(i, 1, "ordinal not in range(128)");
        }
        return new_str(out);
    }
    if (enc == "latin-1") {
        for (unsigned char c : raw) append_utf8(out, c);
        return new_str(out);
    }
    if (enc == "cp437") {
        for (unsigned char c : raw) append_utf8(out, c < 0x80 ? c : cp437_hi[c - 0x80]);
        return new_str(out);
    }
    if (enc == "cp1252") {
        for (size_t i = 0; i < raw.size(); i++) {
            unsigned char c = (unsigned char)raw[i];
            if (c >= 0x80 && c < 0xA0) {
                char32_t m = cp1252_80[c - 0x80];
                if (!m) { bad(i, 1, "character maps to <undefined>"); continue; }
                append_utf8(out, m);
            } else append_utf8(out, c);
        }
        return new_str(out);
    }
    if (enc == "utf-16-le" || enc == "utf-16-be" || enc == "utf-16") {
        bool be = enc == "utf-16-be";
        size_t i = 0;
        if (enc == "utf-16" && raw.size() >= 2) {
            if ((unsigned char)raw[0] == 0xFF && (unsigned char)raw[1] == 0xFE) i = 2;
            else if ((unsigned char)raw[0] == 0xFE && (unsigned char)raw[1] == 0xFF) { i = 2; be = true; }
        }
        auto u = [&](size_t k) { return be ? ((unsigned char)raw[k] << 8 | (unsigned char)raw[k + 1]) : ((unsigned char)raw[k + 1] << 8 | (unsigned char)raw[k]); };
        while (i + 1 < raw.size()) {
            char32_t w = (char32_t)u(i);
            if (w >= 0xD800 && w < 0xDC00 && i + 3 < raw.size()) {
                char32_t w2 = (char32_t)u(i + 2);
                if (w2 >= 0xDC00 && w2 < 0xE000) { append_utf8(out, 0x10000 + ((w - 0xD800) << 10) + (w2 - 0xDC00)); i += 4; continue; }
            }
            append_utf8(out, w);
            i += 2;
        }
        if (i < raw.size()) bad(i, 1, "truncated data");
        return new_str(out);
    }
    if (enc == "utf-32-le" || enc == "utf-32-be" || enc == "utf-32") {
        bool be = enc == "utf-32-be";
        size_t i = 0;
        if (enc == "utf-32" && raw.size() >= 4) {
            if (raw.compare(0, 4, BOM32LE) == 0) i = 4;
            else if (raw.compare(0, 4, BOM32BE) == 0) { i = 4; be = true; }
        }
        for (; i + 3 < raw.size(); i += 4) {
            uint32_t c = 0;
            for (int k = 0; k < 4; k++) c |= (uint32_t)(unsigned char)raw[i + k] << (be ? 24 - 8 * k : 8 * k);
            if (c > 0x10FFFF || (c >= 0xD800 && c < 0xE000)) bad(i, 4, "code point not in range(0x110000)");
            else append_utf8(out, (char32_t)c);
        }
        if (i < raw.size()) bad(i, raw.size() - i, "truncated data");
        return new_str(out);
    }
    raise(make_exc(E_LookupError, "unknown encoding: " + enc_in));
}

var str_encode_(const std::string& s, const std::string& enc_in, const std::string& errors) {
    std::string enc = norm_enc(enc_in);
    if (enc == "utf-8") {
        // lone surrogates cannot be encoded (surrogateescape gives their bytes back)
        bool has_sur = false;
        for (size_t i = 0; i + 2 < s.size(); i++)
            if ((unsigned char)s[i] == 0xED && ((unsigned char)s[i + 1] & 0xE0) == 0xA0) { has_sur = true; break; }
        if (!has_sur) return new_bytes(s);
        std::u32string u = to_u32(s);
        std::string out;
        for (size_t i = 0; i < u.size(); i++) {
            char32_t c = u[i];
            if (c >= 0xD800 && c < 0xE000) {
                if (errors == "surrogateescape" && c >= 0xDC80) { out.push_back((char)(c - 0xDC00)); continue; }
                if (errors == "strict") encode_error("utf-8", s, i, i + 1, "surrogates not allowed");
                if (errors == "replace") { out.push_back('?'); continue; }
                continue;
            }
            append_utf8(out, c);
        }
        return new_bytes(out);
    }
    if (enc == "utf-8-sig") return new_bytes("\xEF\xBB\xBF" + s);
    std::u32string u = to_u32(s);
    std::string out;
    auto bad = [&](size_t i, const char* reason) {
        if (errors == "strict") encode_error(enc, s, i, i + 1, reason);
        if (errors == "replace") out.push_back('?');
        else if (errors == "backslashreplace") {
            char b[16];
            char32_t c = u[i];
            if (c < 0x100) snprintf(b, sizeof b, "\\x%02x", (unsigned)c);
            else if (c < 0x10000) snprintf(b, sizeof b, "\\u%04x", (unsigned)c);
            else snprintf(b, sizeof b, "\\U%08x", (unsigned)c);
            out += b;
        } else if (errors == "xmlcharrefreplace") out += "&#" + std::to_string((uint32_t)u[i]) + ";";
    };
    if (enc == "ascii" || enc == "latin-1") {
        char32_t lim = enc == "ascii" ? 0x80 : 0x100;
        for (size_t i = 0; i < u.size(); i++) {
            if (u[i] < lim) out.push_back((char)u[i]);
            else if (errors == "surrogateescape" && u[i] >= 0xDC80 && u[i] < 0xDD00) out.push_back((char)(u[i] - 0xDC00));
            else bad(i, enc == "ascii" ? "ordinal not in range(128)" : "ordinal not in range(256)");
        }
        return new_bytes(out);
    }
    if (enc == "cp437" || enc == "cp1252") {
        for (size_t i = 0; i < u.size(); i++) {
            char32_t c = u[i];
            if (c < 0x80) { out.push_back((char)c); continue; }
            int found = -1;
            if (enc == "cp437") { for (int k = 0; k < 128; k++) if (cp437_hi[k] == c) { found = 0x80 + k; break; } }
            else {
                if (c >= 0xA0 && c < 0x100) found = (int)c;
                else for (int k = 0; k < 32; k++) if (cp1252_80[k] == c) { found = 0x80 + k; break; }
            }
            if (found < 0) bad(i, "character maps to <undefined>");
            else out.push_back((char)found);
        }
        return new_bytes(out);
    }
    if (enc == "utf-16-le" || enc == "utf-16-be" || enc == "utf-16") {
        bool be = enc == "utf-16-be";
        if (enc == "utf-16") out += "\xFF\xFE";
        auto put = [&](uint16_t w) {
            if (be) { out.push_back((char)(w >> 8)); out.push_back((char)w); }
            else { out.push_back((char)w); out.push_back((char)(w >> 8)); }
        };
        for (char32_t c : u) {
            if (c >= 0x10000) { c -= 0x10000; put((uint16_t)(0xD800 + (c >> 10))); put((uint16_t)(0xDC00 + (c & 0x3FF))); }
            else put((uint16_t)c);
        }
        return new_bytes(out);
    }
    if (enc == "utf-32-le" || enc == "utf-32-be" || enc == "utf-32") {
        bool be = enc == "utf-32-be";
        if (enc == "utf-32") out += BOM32LE;
        for (char32_t c : u)
            for (int k = 0; k < 4; k++) out.push_back((char)(be ? c >> (24 - 8 * k) : c >> (8 * k)));
        return new_bytes(out);
    }
    raise(make_exc(E_LookupError, "unknown encoding: " + enc_in));
}

// ------------------------------------------------------------------------------------------ methods
static std::string barg(const var& v) {
    if (is_bytes(v)) return raw_of(v);
    if (is_int(v)) {
        int64_t x = v.as_int64();
        if (x < 0 || x > 255) raise(E_ValueError, "byte must be in range(0, 256)");
        return std::string(1, (char)x);
    }
    if (v.is_ref() && v.ref()->type->name == "memoryview") return raw_of(v.getattr("obj"));
    raise(E_TypeError, "a bytes-like object is required, not '" + tname(v) + "'");
}
static var same_kind(const var& self, std::string s) { return new_bytes(std::move(s), is_ba(self)); }

var bytes_getitem_(const var& self, const var& key) {
    const std::string& s = B(self)->s;
    if (is_int(key)) return var((int64_t)(unsigned char)s[(size_t)norm_index(key.as_int64(), (int64_t)s.size(), is_ba(self) ? "bytearray" : "index")]);
    if (has_layout(key, Layout::Slice)) {
        auto* sl = static_cast<SliceObj*>(key.ref());
        int64_t lo, hi, st, cnt;
        slice_indices(sl->start, sl->stop, sl->step, (int64_t)s.size(), lo, hi, st, cnt);
        if (st == 1) return same_kind(self, s.substr((size_t)lo, (size_t)cnt));
        std::string o;
        for (int64_t k = 0, i = lo; k < cnt; k++, i += st) o.push_back(s[(size_t)i]);
        return same_kind(self, o);
    }
    raise(E_TypeError, "byte indices must be integers or slices, not " + tname(key));
}

static std::vector<std::string> bsplit_ws(const std::string& s, int64_t maxsplit) {
    std::vector<std::string> out;
    size_t i = 0, n = s.size();
    auto sp = [](unsigned char c) { return c == ' ' || (c >= 9 && c <= 13); };
    while (i < n) {
        while (i < n && sp((unsigned char)s[i])) i++;
        if (i >= n) break;
        if (maxsplit >= 0 && (int64_t)out.size() >= maxsplit) {
            size_t e = n;
            while (e > i && sp((unsigned char)s[e - 1])) e--;
            out.push_back(s.substr(i, e - i));
            return out;
        }
        size_t j = i;
        while (j < n && !sp((unsigned char)s[j])) j++;
        out.push_back(s.substr(i, j - i));
        i = j;
    }
    return out;
}

var bytes_new_(TypeObj* cls, const CallArgs& a, bool ba);
std::string bytes_from_obj(const var& x, const CallArgs& a);

void init_bytes() {
    for (TypeObj* t : {T_bytes, T_bytearray}) {
        bool ba = t == T_bytearray;
        add_method(t, "__len__", [](const CallArgs& a) { return var((int64_t)B(a.pos[0])->s.size()); });
        add_method(t, "__getitem__", [](const CallArgs& a) { return bytes_getitem_(a.pos[0], a.pos.at(1)); });
        add_method(t, "__contains__", [](const CallArgs& a) { return var(B(a.pos[0])->s.find(barg(a.pos.at(1))) != std::string::npos); });
        add_method(t, "__iter__", [](const CallArgs& a) { return iter(a.pos[0]); });
        add_method(t, "decode", [](const CallArgs& a) {
            return bytes_decode_(B(a.pos[0])->s, str_utf8(arg(a, 1, "encoding", "utf-8"_s)), str_utf8(arg(a, 2, "errors", "strict"_s)));
        });
        add_method(t, "hex", [](const CallArgs& a) {
            static const char* d = "0123456789abcdef";
            const std::string& s = B(a.pos[0])->s;
            std::string sep = a.pos.size() > 1 ? (is_str(a.pos[1]) ? str_utf8(a.pos[1]) : raw_of(a.pos[1])) : "";
            std::string o;
            for (size_t i = 0; i < s.size(); i++) {
                if (i && !sep.empty()) o += sep;
                o.push_back(d[(unsigned char)s[i] >> 4]);
                o.push_back(d[(unsigned char)s[i] & 15]);
            }
            return new_str(o);
        });
        add_classmethod(t, "fromhex", [ba](const CallArgs& a) {
            const std::string& h = str_utf8(a.pos.at(1));
            std::string o;
            auto hv = [](char c) { return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1; };
            for (size_t i = 0; i < h.size();) {
                if (h[i] == ' ' || (h[i] >= 9 && h[i] <= 13)) { i++; continue; }
                if (i + 1 >= h.size() || hv(h[i]) < 0 || hv(h[i + 1]) < 0)
                    raise(E_ValueError, "non-hexadecimal number found in fromhex() arg at position " + std::to_string(hv(h[i]) < 0 ? i : i + 1));
                o.push_back((char)(hv(h[i]) * 16 + hv(h[i + 1])));
                i += 2;
            }
            return new_bytes(o, ba);
        });
        // startswith / endswith (prefix[, start[, end]]): within s[start:end], the bounds as a slice's
        auto starts_ends = [](const CallArgs& a, bool ends) {
            const std::string& s = B(a.pos[0])->s;
            int64_t n = (int64_t)s.size();
            auto bound = [n](const var& v, int64_t dflt) {
                if (v.is_none() || v.is_unbound()) return dflt;
                int64_t i = v.as_int64();
                if (i < 0) i = std::max<int64_t>(0, i + n);
                return std::min(i, n);
            };
            int64_t lo = bound(a.pos.size() > 2 ? a.pos[2] : var(None), 0);
            int64_t hi = bound(a.pos.size() > 3 ? a.pos[3] : var(None), n);
            auto test = [&](const var& p) {
                std::string q = barg(p);
                int64_t m = (int64_t)q.size();
                if (lo > n || hi - lo < m) return false;
                return s.compare((size_t)(ends ? hi - m : lo), (size_t)m, q) == 0;
            };
            if (is_tuple(a.pos.at(1))) { for (auto& p : tuple_items(a.pos[1])) if (test(p)) return var(true); return var(false); }
            return var(test(a.pos[1]));
        };
        add_method(t, "startswith", [starts_ends](const CallArgs& a) { return starts_ends(a, false); });
        add_method(t, "endswith", [starts_ends](const CallArgs& a) { return starts_ends(a, true); });
        for (int which = 0; which < 4; which++) {
            static const char* names[] = {"find", "rfind", "index", "rindex"};
            add_method(t, names[which], [which](const CallArgs& a) {
                const std::string& s = B(a.pos[0])->s;
                std::string sub = barg(a.pos.at(1));
                int64_t n = (int64_t)s.size();
                int64_t lo = a.pos.size() > 2 && !a.pos[2].is_none() ? a.pos[2].as_int64() : 0;
                int64_t hi = a.pos.size() > 3 && !a.pos[3].is_none() ? a.pos[3].as_int64() : n;
                if (lo < 0) lo = std::max<int64_t>(0, lo + n);
                if (hi < 0) hi = std::max<int64_t>(0, hi + n);
                if (hi > n) hi = n;
                int64_t r = -1;
                if (lo <= hi && (int64_t)sub.size() <= hi - lo) {
                    size_t p = (which & 1) ? s.rfind(sub, (size_t)(hi - (int64_t)sub.size())) : s.find(sub, (size_t)lo);
                    if (p != std::string::npos && (int64_t)p >= lo && (int64_t)(p + sub.size()) <= hi) r = (int64_t)p;
                }
                if (r < 0 && which >= 2) raise(E_ValueError, "subsection not found");
                return var(r);
            });
        }
        add_method(t, "count", [](const CallArgs& a) {
            const std::string& s = B(a.pos[0])->s;
            std::string sub = barg(a.pos.at(1));
            if (sub.empty()) return var((int64_t)s.size() + 1);
            int64_t n = 0;
            for (size_t p = s.find(sub); p != std::string::npos; p = s.find(sub, p + sub.size())) n++;
            return var(n);
        });
        add_method(t, "replace", [](const CallArgs& a) {
            const std::string& s = B(a.pos[0])->s;
            std::string old = barg(a.pos.at(1)), rep = barg(a.pos.at(2));
            int64_t count = a.pos.size() > 3 ? a.pos[3].as_int64() : -1;
            std::string out;
            if (old.empty()) {
                int64_t k = 0;
                for (size_t i = 0; i <= s.size(); i++) {
                    if (count < 0 || k < count) { out += rep; k++; }
                    if (i < s.size()) out.push_back(s[i]);
                }
                return same_kind(a.pos[0], out);
            }
            size_t p = 0;
            int64_t k = 0;
            for (;;) {
                size_t q = s.find(old, p);
                if (q == std::string::npos || (count >= 0 && k >= count)) break;
                out.append(s, p, q - p);
                out += rep;
                p = q + old.size();
                k++;
            }
            out.append(s, p, std::string::npos);
            return same_kind(a.pos[0], out);
        });
        add_method(t, "split", [](const CallArgs& a) {
            const std::string& s = B(a.pos[0])->s;
            var sep = arg(a, 1, "sep", None);
            int64_t maxsplit = arg(a, 2, "maxsplit", -1).as_int64();
            std::vector<var> out;
            if (sep.is_none()) {
                for (auto& x : bsplit_ws(s, maxsplit)) out.push_back(same_kind(a.pos[0], x));
                return new_list(out);
            }
            std::string p = barg(sep);
            if (p.empty()) raise(E_ValueError, "empty separator");
            size_t pos = 0;
            while (maxsplit < 0 || (int64_t)out.size() < maxsplit) {
                size_t q = s.find(p, pos);
                if (q == std::string::npos) break;
                out.push_back(same_kind(a.pos[0], s.substr(pos, q - pos)));
                pos = q + p.size();
            }
            out.push_back(same_kind(a.pos[0], s.substr(pos)));
            return new_list(out);
        });
        add_method(t, "rsplit", [](const CallArgs& a) {
            const std::string& s = B(a.pos[0])->s;
            var sep = arg(a, 1, "sep", None);
            int64_t maxsplit = arg(a, 2, "maxsplit", -1).as_int64();
            if (sep.is_none() && maxsplit < 0) {
                std::vector<var> out;
                for (auto& x : bsplit_ws(s, -1)) out.push_back(same_kind(a.pos[0], x));
                return new_list(out);
            }
            std::string p = sep.is_none() ? " " : barg(sep);
            std::vector<var> out;
            size_t end = s.size();
            while (maxsplit < 0 || (int64_t)out.size() < maxsplit) {
                if (end < p.size()) break;
                size_t q = s.rfind(p, end - p.size());
                if (q == std::string::npos) break;
                out.push_back(same_kind(a.pos[0], s.substr(q + p.size(), end - q - p.size())));
                end = q;
            }
            out.push_back(same_kind(a.pos[0], s.substr(0, end)));
            std::reverse(out.begin(), out.end());
            return new_list(out);
        });
        add_method(t, "join", [](const CallArgs& a) {
            const std::string& sep = B(a.pos[0])->s;
            std::string out;
            size_t i = 0;
            for (var x : each(a.pos.at(1))) {
                if (i++) out += sep;
                out += barg(x);
            }
            return same_kind(a.pos[0], out);
        });
        for (int which = 1; which <= 3; which++) {
            static const char* names[] = {"", "lstrip", "rstrip", "strip"};
            add_method(t, names[which], [which](const CallArgs& a) {
                const std::string& s = B(a.pos[0])->s;
                std::string cs = a.pos.size() > 1 && !a.pos[1].is_none() ? barg(a.pos[1]) : std::string(" \t\n\r\x0b\x0c");
                size_t x = 0, y = s.size();
                if (which & 1) while (x < y && cs.find(s[x]) != std::string::npos) x++;
                if (which & 2) while (y > x && cs.find(s[y - 1]) != std::string::npos) y--;
                return same_kind(a.pos[0], s.substr(x, y - x));
            });
        }
        add_method(t, "lower", [](const CallArgs& a) { std::string s = B(a.pos[0])->s; for (auto& c : s) if (c >= 'A' && c <= 'Z') c += 32; return same_kind(a.pos[0], s); });
        add_method(t, "upper", [](const CallArgs& a) { std::string s = B(a.pos[0])->s; for (auto& c : s) if (c >= 'a' && c <= 'z') c -= 32; return same_kind(a.pos[0], s); });
        add_method(t, "isdigit", [](const CallArgs& a) { auto& s = B(a.pos[0])->s; return var(!s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; })); });
        add_method(t, "isalpha", [](const CallArgs& a) { auto& s = B(a.pos[0])->s; return var(!s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return isalpha((unsigned char)c) != 0; })); });
        add_method(t, "isalnum", [](const CallArgs& a) { auto& s = B(a.pos[0])->s; return var(!s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return isalnum((unsigned char)c) != 0; })); });
        add_method(t, "isspace", [](const CallArgs& a) { auto& s = B(a.pos[0])->s; return var(!s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c == ' ' || (c >= 9 && c <= 13); })); });
        add_method(t, "partition", [](const CallArgs& a) {
            const std::string& s = B(a.pos[0])->s;
            std::string sep = barg(a.pos.at(1));
            size_t p = s.find(sep);
            if (p == std::string::npos) return new_tuple({same_kind(a.pos[0], s), same_kind(a.pos[0], ""), same_kind(a.pos[0], "")});
            return new_tuple({same_kind(a.pos[0], s.substr(0, p)), same_kind(a.pos[0], sep), same_kind(a.pos[0], s.substr(p + sep.size()))});
        });
        add_method(t, "ljust", [](const CallArgs& a) {
            std::string s = B(a.pos[0])->s;
            int64_t w = a.pos.at(1).as_int64();
            char f = a.pos.size() > 2 ? raw_of(a.pos[2])[0] : ' ';
            if ((int64_t)s.size() < w) s += std::string((size_t)(w - (int64_t)s.size()), f);
            return same_kind(a.pos[0], s);
        });
        add_method(t, "rjust", [](const CallArgs& a) {
            std::string s = B(a.pos[0])->s;
            int64_t w = a.pos.at(1).as_int64();
            char f = a.pos.size() > 2 ? raw_of(a.pos[2])[0] : ' ';
            if ((int64_t)s.size() < w) s = std::string((size_t)(w - (int64_t)s.size()), f) + s;
            return same_kind(a.pos[0], s);
        });
        add_method(t, "translate", [](const CallArgs& a) {
            std::string s = B(a.pos[0])->s;
            var tb = a.pos.at(1);
            std::string del = a.pos.size() > 2 ? barg(a.pos[2]) : "";
            std::string out;
            const std::string* tab = tb.is_none() ? nullptr : &raw_of(tb);
            for (unsigned char c : s) {
                if (del.find((char)c) != std::string::npos) continue;
                out.push_back(tab ? (*tab)[c] : (char)c);
            }
            return same_kind(a.pos[0], out);
        });
        add_static(t, "maketrans", [](const CallArgs& a) {
            std::string x = barg(a.pos.at(0)), y = barg(a.pos.at(1));
            std::string tab(256, 0);
            for (int i = 0; i < 256; i++) tab[(size_t)i] = (char)i;
            for (size_t i = 0; i < x.size(); i++) tab[(unsigned char)x[i]] = y[i];
            return new_bytes(tab);
        });
        add_method(t, "__mod__", [](const CallArgs& a) { return same_kind(a.pos[0], percent_format(a.pos[0], a.pos.at(1), true)); });
    }
    // bytearray's mutations
    TypeObj* t = T_bytearray;
    add_method(t, "__setitem__", [](const CallArgs& a) {
        std::string& s = B(a.pos[0])->s;
        const var& k = a.pos.at(1);
        if (is_int(k)) {
            int64_t i = norm_index(k.as_int64(), (int64_t)s.size(), "bytearray");
            int64_t v = a.pos.at(2).as_int64();
            if (v < 0 || v > 255) raise(E_ValueError, "byte must be in range(0, 256)");
            s[(size_t)i] = (char)v;
            return None;
        }
        auto* sl = static_cast<SliceObj*>(k.ref());
        int64_t lo, hi, st, cnt;
        slice_indices(sl->start, sl->stop, sl->step, (int64_t)s.size(), lo, hi, st, cnt);
        std::string v = is_int(a.pos.at(2)) ? std::string((size_t)a.pos[2].as_int64(), '\0') : bytes_from_obj(a.pos[2], CallArgs());
        if (st == 1) { s.replace((size_t)lo, (size_t)cnt, v); return None; }
        if ((int64_t)v.size() != cnt) raise(E_ValueError, "attempt to assign bytes of size " + std::to_string(v.size()) + " to extended slice of size " + std::to_string(cnt));
        for (int64_t q = 0, i = lo; q < cnt; q++, i += st) s[(size_t)i] = v[(size_t)q];
        return None;
    });
    add_method(t, "__delitem__", [](const CallArgs& a) {
        std::string& s = B(a.pos[0])->s;
        const var& k = a.pos.at(1);
        if (is_int(k)) { s.erase((size_t)norm_index(k.as_int64(), (int64_t)s.size(), "bytearray"), 1); return None; }
        auto* sl = static_cast<SliceObj*>(k.ref());
        int64_t lo, hi, st, cnt;
        slice_indices(sl->start, sl->stop, sl->step, (int64_t)s.size(), lo, hi, st, cnt);
        if (st == 1) { s.erase((size_t)lo, (size_t)cnt); return None; }
        std::string o;
        std::vector<bool> drop(s.size());
        for (int64_t q = 0, i = lo; q < cnt; q++, i += st) drop[(size_t)i] = true;
        for (size_t i = 0; i < s.size(); i++) if (!drop[i]) o.push_back(s[i]);
        s = o;
        return None;
    });
    add_method(t, "append", [](const CallArgs& a) {
        int64_t v = a.pos.at(1).as_int64();
        if (v < 0 || v > 255) raise(E_ValueError, "byte must be in range(0, 256)");
        B(a.pos[0])->s.push_back((char)v);
        return None;
    });
    add_method(t, "extend", [](const CallArgs& a) { B(a.pos[0])->s += bytes_from_obj(a.pos.at(1), CallArgs()); return None; });
    add_method(t, "__iadd__", [](const CallArgs& a) { B(a.pos[0])->s += barg(a.pos.at(1)); return a.pos[0]; });
    add_method(t, "insert", [](const CallArgs& a) {
        std::string& s = B(a.pos[0])->s;
        int64_t i = a.pos.at(1).as_int64(), n = (int64_t)s.size();
        if (i < 0) i = std::max<int64_t>(0, i + n);
        if (i > n) i = n;
        s.insert(s.begin() + i, (char)a.pos.at(2).as_int64());
        return None;
    });
    add_method(t, "pop", [](const CallArgs& a) {
        std::string& s = B(a.pos[0])->s;
        if (s.empty()) raise(E_IndexError, "pop from empty bytearray");
        int64_t i = norm_index(a.pos.size() > 1 ? a.pos[1].as_int64() : -1, (int64_t)s.size(), "pop");
        int64_t v = (unsigned char)s[(size_t)i];
        s.erase((size_t)i, 1);
        return var(v);
    });
    add_method(t, "clear", [](const CallArgs& a) { B(a.pos[0])->s.clear(); return None; });
    add_method(t, "copy", [](const CallArgs& a) { return new_bytes(B(a.pos[0])->s, true); });
    add_method(t, "reverse", [](const CallArgs& a) { auto& s = B(a.pos[0])->s; std::reverse(s.begin(), s.end()); return None; });
    add_static(T_bytes, "__new__", [](const CallArgs& a) { return bytes_new_(T(a.pos.at(0)), a, false); });
    add_static(T_bytearray, "__new__", [](const CallArgs& a) { return bytes_new_(T(a.pos.at(0)), a, true); });
    add_method(T_bytearray, "__init__", [](const CallArgs&) { return None; });
}

std::string bytes_from_obj(const var& x, const CallArgs& a) {
    if (is_bytes(x)) return raw_of(x);
    if (is_str(x)) {
        var enc = a.pos.size() > 2 ? a.pos[2] : kwarg(a, "encoding");
        if (enc.is_unbound()) raise(E_TypeError, "string argument without an encoding");
        var err = a.pos.size() > 3 ? a.pos[3] : kwarg(a, "errors", "strict"_s);
        return raw_of(str_encode_(str_utf8(x), str_utf8(enc), str_utf8(err)));
    }
    if (is_int(x)) {
        int64_t n = x.as_int64();
        if (n < 0) raise(E_ValueError, "negative count");
        return std::string((size_t)n, '\0');
    }
    var m;
    if (x.is_ref() && !x.ref()->type->builtin && lookup_special(x, "__bytes__", m)) return raw_of(call0(m));
    if (x.is_ref() && x.ref()->type->name == "memoryview") return raw_of(x.getattr("obj"));
    std::string out;
    for (var v : each(x)) {
        int64_t b = index_(v);
        if (b < 0 || b > 255) raise(E_ValueError, "bytes must be in range(0, 256)");
        out.push_back((char)b);
    }
    return out;
}
var bytes_new_(TypeObj* cls, const CallArgs& a, bool ba) {
    std::string s;
    if (a.pos.size() > 1) s = bytes_from_obj(a.pos[1], a);
    else if (var src = kwarg(a, "source"); !src.is_unbound()) s = bytes_from_obj(src, a);
    var r = new_bytes(s, ba);
    if (cls != (ba ? T_bytearray : T_bytes)) {
        r.ref()->type = cls;
        incref(cls);
    }
    return r;
}

}  // namespace py
