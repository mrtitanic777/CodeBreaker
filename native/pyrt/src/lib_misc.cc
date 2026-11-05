// lib_misc.cc -- the library modules beyond the OS: json, struct, hashlib, collections, functools,
// itertools, math, string, textwrap, copy, bisect, heapq, operator, contextlib, threading, importlib,
// types, typing, abc, enum, dataclasses, uuid, traceback, warnings, atexit, binascii, zlib (crc32),
// keyword; and the modules with no C++ yet (pickle, sqlite3 ...), which raise when used.
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>

#include "rt.hpp"

namespace py {

void init_os_modules();
void init_re_module(const var& mod);
extern var g_sys;
std::string str_repr_(const std::string& s);
std::string fixed_(double v, int prec);
var bytes_decode_(const std::string& raw, const std::string& enc, const std::string& errors);

// ------------------------------------------------------------------------------------------ json
struct JsonIn {
    const std::string& s;
    size_t i = 0;
    var object_hook, object_pairs_hook;
    [[noreturn]] void fail(const std::string& msg) {
        int64_t line = 1, col = 1;
        for (size_t k = 0; k < i && k < s.size(); k++) { if (s[k] == '\n') { line++; col = 1; } else col++; }
        static var E_json;
        var e = import("json").getattr("JSONDecodeError");
        raise(make_exc_args(T(e), {new_str(msg + ": line " + std::to_string(line) + " column " + std::to_string(col) + " (char " + std::to_string(i) + ")")}));
    }
    void ws() { while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) i++; }
    var value() {
        ws();
        if (i >= s.size()) fail("Expecting value");
        char c = s[i];
        if (c == '{') return object();
        if (c == '[') {
            i++;
            std::vector<var> v;
            ws();
            if (i < s.size() && s[i] == ']') { i++; return new_list(v); }
            for (;;) {
                v.push_back(value());
                ws();
                if (i < s.size() && s[i] == ',') { i++; continue; }
                if (i < s.size() && s[i] == ']') { i++; return new_list(v); }
                fail("Expecting ',' delimiter");
            }
        }
        if (c == '"') return new_str(string());
        if (s.compare(i, 4, "true") == 0) { i += 4; return true; }
        if (s.compare(i, 5, "false") == 0) { i += 5; return false; }
        if (s.compare(i, 4, "null") == 0) { i += 4; return None; }
        if (s.compare(i, 3, "NaN") == 0) { i += 3; return NAN; }
        if (s.compare(i, 8, "Infinity") == 0) { i += 8; return INFINITY; }
        if (s.compare(i, 9, "-Infinity") == 0) { i += 9; return -INFINITY; }
        if (c == '-' || (c >= '0' && c <= '9')) {
            size_t st = i;
            if (s[i] == '-') i++;
            if (i >= s.size() || !isdigit((unsigned char)s[i])) { i = st; fail("Expecting value"); }
            if (s[i] == '0') i++;
            else while (i < s.size() && isdigit((unsigned char)s[i])) i++;
            bool flt = false;
            if (i < s.size() && s[i] == '.' && i + 1 < s.size() && isdigit((unsigned char)s[i + 1])) { flt = true; i++; while (i < s.size() && isdigit((unsigned char)s[i])) i++; }
            if (i < s.size() && (s[i] == 'e' || s[i] == 'E')) {
                size_t save = i;
                i++;
                if (i < s.size() && (s[i] == '+' || s[i] == '-')) i++;
                if (i < s.size() && isdigit((unsigned char)s[i])) { flt = true; while (i < s.size() && isdigit((unsigned char)s[i])) i++; }
                else i = save;
            }
            std::string num = s.substr(st, i - st);
            if (flt) return std::strtod(num.c_str(), nullptr);
            return int_from(num, 10);
        }
        fail("Expecting value");
    }
    std::string string() {
        i++;   // "
        std::string o;
        for (;;) {
            if (i >= s.size()) fail("Unterminated string starting at");
            char c = s[i++];
            if (c == '"') return o;
            if (c == '\\') {
                if (i >= s.size()) fail("Unterminated string starting at");
                char e = s[i++];
                switch (e) {
                case '"': o.push_back('"'); break;
                case '\\': o.push_back('\\'); break;
                case '/': o.push_back('/'); break;
                case 'b': o.push_back('\b'); break;
                case 'f': o.push_back('\f'); break;
                case 'n': o.push_back('\n'); break;
                case 'r': o.push_back('\r'); break;
                case 't': o.push_back('\t'); break;
                case 'u': {
                    auto hex4 = [&]() -> uint32_t {
                        if (i + 4 > s.size()) fail("Invalid \\uXXXX escape");
                        uint32_t v = (uint32_t)std::stoul(s.substr(i, 4), nullptr, 16);
                        i += 4;
                        return v;
                    };
                    uint32_t u = hex4();
                    if (u >= 0xD800 && u < 0xDC00 && i + 1 < s.size() && s[i] == '\\' && s[i + 1] == 'u') {
                        size_t save = i;
                        i += 2;
                        uint32_t l = hex4();
                        if (l >= 0xDC00 && l < 0xE000) u = 0x10000 + ((u - 0xD800) << 10) + (l - 0xDC00);
                        else i = save;
                    }
                    append_utf8(o, u);
                    break;
                }
                default: i--; fail("Invalid \\escape");
                }
                continue;
            }
            if ((unsigned char)c < 0x20) { i--; fail("Invalid control character at"); }
            o.push_back(c);
        }
    }
    var object() {
        i++;
        std::vector<std::pair<var, var>> pairs;
        ws();
        if (i < s.size() && s[i] == '}') { i++; return finish(pairs); }
        for (;;) {
            ws();
            if (i >= s.size() || s[i] != '"') fail("Expecting property name enclosed in double quotes");
            var k = new_str(string());
            ws();
            if (i >= s.size() || s[i] != ':') fail("Expecting ':' delimiter");
            i++;
            var v = value();
            pairs.push_back({k, v});
            ws();
            if (i < s.size() && s[i] == ',') { i++; continue; }
            if (i < s.size() && s[i] == '}') { i++; return finish(pairs); }
            fail("Expecting ',' delimiter");
        }
    }
    var finish(std::vector<std::pair<var, var>>& pairs) {
        if (!object_pairs_hook.is_none() && !object_pairs_hook.is_unbound()) {
            std::vector<var> v;
            for (auto& [k, x] : pairs) v.push_back(new_tuple({k, x}));
            return call1(object_pairs_hook, new_list(v));
        }
        var d = new_dict_obj();
        for (auto& [k, x] : pairs) d.setitem(k, x);
        if (!object_hook.is_none() && !object_hook.is_unbound()) return call1(object_hook, d);
        return d;
    }
};

static std::string json_str(const std::string& s, bool ensure_ascii) {
    std::string o = "\"";
    for (char32_t c : to_u32(s)) {
        switch (c) {
        case '"': o += "\\\""; break;
        case '\\': o += "\\\\"; break;
        case '\n': o += "\\n"; break;
        case '\r': o += "\\r"; break;
        case '\t': o += "\\t"; break;
        case '\b': o += "\\b"; break;
        case '\f': o += "\\f"; break;
        default:
            if (c < 0x20 || (ensure_ascii && c >= 0x7F)) {
                char b[16];
                if (c >= 0x10000) {
                    char32_t v = c - 0x10000;
                    snprintf(b, sizeof b, "\\u%04x\\u%04x", (unsigned)(0xD800 + (v >> 10)), (unsigned)(0xDC00 + (v & 0x3FF)));
                } else snprintf(b, sizeof b, "\\u%04x", (unsigned)c);
                o += b;
            } else append_utf8(o, c);
        }
    }
    return o + "\"";
}
struct JsonOut {
    bool sort_keys = false, ensure_ascii = true, allow_nan = true, skipkeys = false;
    std::string indent;
    bool has_indent = false;
    std::string item_sep = ", ", key_sep = ": ";
    var dflt;
    std::vector<const Obj*> stack;
    void put(std::string& o, const var& v, int level) {
        if (v.is_none()) { o += "null"; return; }
        if (v.is_bool()) { o += v.small() ? "true" : "false"; return; }
        if (is_int(v)) { o += to_big(v).str(); return; }
        if (v.is_float() || has_layout(v, Layout::Float)) {
            double d = v.as_double();
            if (std::isnan(d)) { if (!allow_nan) raise(E_ValueError, "Out of range float values are not JSON compliant"); o += "NaN"; return; }
            if (std::isinf(d)) { if (!allow_nan) raise(E_ValueError, "Out of range float values are not JSON compliant"); o += d > 0 ? "Infinity" : "-Infinity"; return; }
            o += repr_float(d);
            return;
        }
        if (is_str(v)) { o += json_str(str_utf8(v), ensure_ascii); return; }
        bool islist = is_list(v) || is_tuple(v);
        if (islist || is_dict(v)) {
            if (std::find(stack.begin(), stack.end(), v.ref()) != stack.end()) raise(E_ValueError, "Circular reference detected");
            stack.push_back(v.ref());
            std::string nl, nl_end;
            if (has_indent) {
                nl = "\n";
                for (int k = 0; k <= level; k++) nl += indent;
                nl_end = "\n";
                for (int k = 0; k < level; k++) nl_end += indent;
            }
            if (islist) {
                const std::vector<var>& items = is_list(v) ? list_items(v) : tuple_items(v);
                if (items.empty()) o += "[]";
                else {
                    o += "[";
                    for (size_t k = 0; k < items.size(); k++) {
                        if (k) o += item_sep;
                        o += nl;
                        put(o, items[k], level + 1);
                    }
                    o += nl_end + "]";
                }
            } else {
                std::vector<std::pair<var, var>> items;
                for (var k : each(v)) items.push_back({k, v.getitem(k)});
                if (items.empty()) { o += "{}"; stack.pop_back(); return; }
                std::vector<std::pair<std::string, var>> conv;
                for (auto& [k, x] : items) {
                    std::string ks;
                    if (is_str(k)) ks = str_utf8(k);
                    else if (k.is_bool()) ks = k.small() ? "true" : "false";
                    else if (k.is_none()) ks = "null";
                    else if (is_int(k)) ks = to_big(k).str();
                    else if (k.is_float()) ks = repr_float(k.flt());
                    else if (skipkeys) continue;
                    else raise(E_TypeError, "keys must be str, int, float, bool or None, not " + tname(k));
                    conv.push_back({ks, x});
                }
                if (sort_keys) std::stable_sort(conv.begin(), conv.end(), [](auto& a, auto& b) { return a.first < b.first; });
                o += "{";
                for (size_t k = 0; k < conv.size(); k++) {
                    if (k) o += item_sep;
                    o += nl + json_str(conv[k].first, ensure_ascii) + key_sep;
                    put(o, conv[k].second, level + 1);
                }
                o += nl_end + "}";
            }
            stack.pop_back();
            return;
        }
        if (!dflt.is_none() && !dflt.is_unbound()) { put(o, call1(dflt, v), level); return; }
        raise(E_TypeError, "Object of type " + tname(v) + " is not JSON serializable");
    }
};
static std::string json_dumps(const var& obj, const CallArgs& a, size_t first_kw) {
    (void)first_kw;
    JsonOut j;
    j.sort_keys = kwarg(a, "sort_keys", false).truthy();
    j.ensure_ascii = kwarg(a, "ensure_ascii", true).truthy();
    j.allow_nan = kwarg(a, "allow_nan", true).truthy();
    j.skipkeys = kwarg(a, "skipkeys", false).truthy();
    j.dflt = kwarg(a, "default", None);
    var ind = kwarg(a, "indent", None);
    if (!ind.is_none()) {
        j.has_indent = true;
        j.indent = is_str(ind) ? str_utf8(ind) : std::string((size_t)std::max<int64_t>(0, ind.as_int64()), ' ');
        j.item_sep = ",";
    }
    var seps = kwarg(a, "separators", None);
    if (!seps.is_none()) {
        auto v = collect(seps);
        j.item_sep = str_utf8(v[0]);
        j.key_sep = str_utf8(v[1]);
    }
    std::string o;
    j.put(o, obj, 0);
    return o;
}
static var json_loads(const var& sv, const CallArgs& a) {
    std::string s = is_str(sv) ? str_utf8(sv) : str_utf8(bytes_decode_(raw_of(sv), "utf-8-sig", "strict"));
    JsonIn p{s};
    p.object_hook = kwarg(a, "object_hook", None);
    p.object_pairs_hook = kwarg(a, "object_pairs_hook", None);
    if (s.size() >= 3 && s.compare(0, 3, "\xEF\xBB\xBF") == 0) p.fail("Unexpected UTF-8 BOM (decode using utf-8-sig)");
    var v = p.value();
    p.ws();
    if (p.i != s.size()) p.fail("Extra data");
    return v;
}

// ------------------------------------------------------------------------------------------ struct
struct FmtItem { char code; int64_t count; int size; };
static std::vector<FmtItem> parse_fmt(const std::string& f, bool& little, bool& native_align) {
    little = true;
    native_align = true;
    size_t i = 0;
    if (!f.empty() && strchr("@=<>!", f[0])) {
        char c = f[0];
        little = c == '<' || c == '@' || c == '=';
        native_align = c == '@';
        i = 1;
    }
    std::vector<FmtItem> out;
    while (i < f.size()) {
        if (isspace((unsigned char)f[i])) { i++; continue; }
        int64_t n = -1;
        if (isdigit((unsigned char)f[i])) { n = 0; while (i < f.size() && isdigit((unsigned char)f[i])) n = n * 10 + (f[i++] - '0'); }
        if (i >= f.size()) raise(make_exc_args(T(import("struct").getattr("error")), {"repeat count given without format specifier"_s}));
        char c = f[i++];
        int sz;
        switch (c) {
        case 'x': case 'c': case 'b': case 'B': case '?': case 's': case 'p': sz = 1; break;
        case 'h': case 'H': case 'e': sz = 2; break;
        case 'i': case 'I': case 'l': case 'L': case 'f': sz = 4; break;
        case 'q': case 'Q': case 'd': sz = 8; break;
        case 'n': case 'N': case 'P': sz = 8; break;
        default: raise(make_exc_args(T(import("struct").getattr("error")), {"bad char in struct format"_s}));
        }
        out.push_back({c, n < 0 ? 1 : n, sz});
    }
    return out;
}
static int64_t calcsize(const std::vector<FmtItem>& items, bool align) {
    int64_t n = 0;
    for (auto& it : items) {
        if (align && it.size > 1 && it.code != 's' && it.code != 'p') n = (n + it.size - 1) / it.size * it.size;
        n += (it.code == 's' || it.code == 'p') ? it.count : it.size * it.count;
    }
    return n;
}
static var struct_error(const std::string& m) { return make_exc_args(T(import("struct").getattr("error")), {new_str(m)}); }
static var unpack_from(const std::string& fmt, const std::string& buf, int64_t off) {
    bool little, align;
    auto items = parse_fmt(fmt, little, align);
    int64_t need = calcsize(items, align);
    if (off < 0) off += (int64_t)buf.size();
    if (off < 0 || (int64_t)buf.size() - off < need) {
        if (off == 0 && fmt.size())
            raise(struct_error("unpack requires a buffer of " + std::to_string(need) + " bytes"));
        raise(struct_error("unpack_from requires a buffer of at least " + std::to_string(need + off) + " bytes for unpacking " +
                           std::to_string(need) + " bytes at offset " + std::to_string(off) + " (actual buffer size is " + std::to_string(buf.size()) + ")"));
    }
    const unsigned char* p = (const unsigned char*)buf.data() + off;
    int64_t pos = 0;
    std::vector<var> out;
    for (auto& it : items) {
        if (align && it.size > 1 && it.code != 's' && it.code != 'p') pos = (pos + it.size - 1) / it.size * it.size;
        if (it.code == 's' || it.code == 'p') {
            out.push_back(new_bytes(std::string((const char*)p + pos, (size_t)it.count)));
            pos += it.count;
            continue;
        }
        for (int64_t k = 0; k < it.count; k++) {
            if (it.code == 'x') { pos++; continue; }
            uint64_t v = 0;
            for (int b = 0; b < it.size; b++) {
                int idx = little ? b : it.size - 1 - b;
                v |= (uint64_t)p[pos + idx] << (8 * b);
            }
            pos += it.size;
            switch (it.code) {
            case 'c': out.push_back(new_bytes(std::string(1, (char)v))); break;
            case 'b': out.push_back(var((int64_t)(int8_t)v)); break;
            case 'B': out.push_back(var((int64_t)(uint8_t)v)); break;
            case '?': out.push_back(var(v != 0)); break;
            case 'h': out.push_back(var((int64_t)(int16_t)v)); break;
            case 'H': out.push_back(var((int64_t)(uint16_t)v)); break;
            case 'i': case 'l': out.push_back(var((int64_t)(int32_t)v)); break;
            case 'I': case 'L': out.push_back(var((int64_t)(uint32_t)v)); break;
            case 'q': case 'n': out.push_back(var((int64_t)v)); break;
            case 'Q': case 'N': case 'P': out.push_back(var((unsigned long long)v)); break;
            case 'f': { uint32_t u = (uint32_t)v; float f; memcpy(&f, &u, 4); out.push_back(var((double)f)); break; }
            case 'd': { double d; memcpy(&d, &v, 8); out.push_back(var(d)); break; }
            case 'e': {
                uint16_t h = (uint16_t)v;
                int s = h >> 15, e = (h >> 10) & 31, m = h & 1023;
                double d = e == 0 ? std::ldexp(m, -24) : e == 31 ? (m ? NAN : INFINITY) : std::ldexp(m + 1024, e - 25);
                out.push_back(var(s ? -d : d));
                break;
            }
            }
        }
    }
    return new_tuple(out);
}
static std::string pack(const std::string& fmt, const std::vector<var>& args, size_t first) {
    bool little, align;
    auto items = parse_fmt(fmt, little, align);
    std::string out;
    size_t ai = first;
    size_t nargs = 0;
    for (auto& it : items) nargs += (it.code == 'x') ? 0 : (it.code == 's' || it.code == 'p') ? 1 : (size_t)it.count;
    if (args.size() - first != nargs) raise(struct_error("pack expected " + std::to_string(nargs) + " items for packing (got " + std::to_string(args.size() - first) + ")"));
    for (auto& it : items) {
        if (align && it.size > 1 && it.code != 's' && it.code != 'p') while (out.size() % (size_t)it.size) out.push_back('\0');
        if (it.code == 's' || it.code == 'p') {
            const var& v = args[ai++];
            if (!is_bytes(v)) raise(struct_error("argument for 's' must be a bytes object"));
            std::string b = raw_of(v);
            b.resize((size_t)it.count, '\0');
            out += b;
            continue;
        }
        for (int64_t k = 0; k < it.count; k++) {
            if (it.code == 'x') { out.push_back('\0'); continue; }
            const var& v = args[ai++];
            uint64_t bits = 0;
            if (it.code == 'f' || it.code == 'd' || it.code == 'e') {
                double d = v.as_double();
                if (it.code == 'f') {
                    float f = (float)d;
                    if (std::isinf(f) && !std::isinf(d)) raise(E_OverflowError, "float too large to pack with f format");
                    uint32_t u;
                    memcpy(&u, &f, 4);
                    bits = u;
                } else if (it.code == 'd') memcpy(&bits, &d, 8);
                else {
                    float f = (float)d;
                    uint32_t u;
                    memcpy(&u, &f, 4);
                    uint16_t s = (uint16_t)((u >> 16) & 0x8000);
                    int e = (int)((u >> 23) & 0xFF) - 127 + 15;
                    uint32_t m = u & 0x7FFFFF;
                    if (e <= 0) bits = s;
                    else if (e >= 31) bits = s | 0x7C00;
                    else bits = s | (uint16_t)(e << 10) | (uint16_t)(m >> 13);
                }
            } else if (it.code == 'c') {
                const std::string& b = raw_of(v);
                if (b.size() != 1) raise(struct_error("char format requires a bytes object of length 1"));
                bits = (unsigned char)b[0];
            } else if (it.code == '?') {
                bits = v.truthy() ? 1 : 0;
            } else {
                if (!is_int(v)) {
                    var m;
                    if (!lookup_special(v, "__index__", m)) raise(struct_error("required argument is not an integer"));
                }
                Big b = to_big(is_int(v) ? v : call0(v.getattr("__index__")));
                bool sig = islower((unsigned char)it.code) && it.code != 'n';
                Big lo = sig ? -shl(Big(1), it.size * 8 - 1) : Big(0);
                Big hi = sig ? shl(Big(1), it.size * 8 - 1) - Big(1) : shl(Big(1), it.size * 8) - Big(1);
                if (b < lo || b > hi) {
                    std::string fmtc(1, it.code);
                    if (sig) raise(struct_error("'" + fmtc + "' format requires " + lo.str() + " <= number <= " + hi.str()));
                    raise(struct_error("'" + fmtc + "' format requires 0 <= number <= " + hi.str()));
                }
                Big m = b.sign() < 0 ? b + shl(Big(1), it.size * 8) : b;
                if (m.fits_i64()) bits = (uint64_t)m.i64();
                else bits = (uint64_t)std::stoull(m.str());
            }
            for (int bi = 0; bi < it.size; bi++) {
                int sh = little ? bi : it.size - 1 - bi;
                out.push_back((char)((bits >> (8 * sh)) & 0xFF));
            }
        }
    }
    return out;
}

// ------------------------------------------------------------------------------------------ hashlib
struct HashObj : DataObj {
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_HASH_HANDLE h = nullptr;
    std::string name;
    std::string data;   // (kept so copy() and digest() can run more than once)
    int size = 0, block = 64;
};
static TypeObj* T_hash;
static LPCWSTR alg_of(const std::string& n, int& size, int& block) {
    block = 64;
    if (n == "sha1") { size = 20; return BCRYPT_SHA1_ALGORITHM; }
    if (n == "sha256") { size = 32; return BCRYPT_SHA256_ALGORITHM; }
    if (n == "md5") { size = 16; return BCRYPT_MD5_ALGORITHM; }
    if (n == "sha384") { size = 48; block = 128; return BCRYPT_SHA384_ALGORITHM; }
    if (n == "sha512") { size = 64; block = 128; return BCRYPT_SHA512_ALGORITHM; }
    return nullptr;
}
static std::string digest_of(const std::string& name, const std::string& data) {
    int size, block;
    LPCWSTR a = alg_of(name, size, block);
    if (!a) raise(E_ValueError, "unsupported hash type " + name);
    BCRYPT_ALG_HANDLE alg;
    BCryptOpenAlgorithmProvider(&alg, a, nullptr, 0);
    BCRYPT_HASH_HANDLE h;
    BCryptCreateHash(alg, &h, nullptr, 0, nullptr, 0, 0);
    BCryptHashData(h, (PUCHAR)data.data(), (ULONG)data.size(), 0);
    std::string out((size_t)size, '\0');
    BCryptFinishHash(h, (PUCHAR)out.data(), (ULONG)size, 0);
    BCryptDestroyHash(h);
    BCryptCloseAlgorithmProvider(alg, 0);
    return out;
}
static var new_hash(const std::string& name, const var& data) {
    int size, block;
    if (!alg_of(name, size, block)) raise(E_ValueError, "unsupported hash type " + name);
    auto* o = new HashObj();
    o->type = T_hash;
    o->name = name;
    o->size = size;
    o->block = block;
    if (!data.is_unbound() && !data.is_none()) o->data = raw_of(data);
    return var(static_cast<Obj*>(o));
}

// ------------------------------------------------------------------------------------------ helpers
static var cls(const char* name, std::vector<var> bases, std::vector<std::pair<const char*, var>> members, const char* module) {
    var ns = new_dict();
    for (auto& [k, v] : members) ns.setitem(new_str(k), v);
    return make_class(name, name, bases, ns, {}, module);
}
static var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }

// functools.partial
static TypeObj* T_partial;
struct PartialObj : DataObj { var func; std::vector<var> args; std::vector<std::pair<var, var>> kw; };

// collections.deque
static TypeObj* T_deque;
struct DequeObj : DataObj { std::vector<var> v; int64_t maxlen = -1; };
static DequeObj* DQ(const var& v) { return static_cast<DequeObj*>(v.ref()); }

// lru_cache
static TypeObj* T_lru;
struct LruObj : DataObj { var func; var cache; int64_t maxsize = 128; int64_t hits = 0, misses = 0; bool typed = false; };



static var mk_namedtuple(const CallArgs& a);
void init_more_modules();

static void reg(const char* name, native_module_init init) { register_native_module(name, init); }

// a struct format: str, or bytes as CPython also takes (zipfile's b"<4s4H2LH")
static std::string fmt_str(const var& f) { return is_str(f) ? str_utf8(f) : raw_of(f); }

void make_collections_abc(const var& m);
std::string dict_repr_(const var& d);
bool module_registered_(const std::string& name);
void init_modules() {
    init_os_modules();
    reg("re", init_re_module);

    reg("json", [](const var& m) {
        var dec = make_class("JSONDecodeError", "JSONDecodeError", {t::ValueError}, new_dict(), {}, "json");
        mod_set(m, "JSONDecodeError", dec);
        mod_def(m, "dumps", [](const CallArgs& a) { return new_str(json_dumps(a.pos.at(0), a, 1)); });
        mod_def(m, "dump", [](const CallArgs& a) { a.pos.at(1).call("write", new_str(json_dumps(a.pos.at(0), a, 2))); return None; });
        mod_def(m, "loads", [](const CallArgs& a) { return json_loads(a.pos.at(0), a); });
        mod_def(m, "load", [](const CallArgs& a) { return json_loads(a.pos.at(0).call("read"), a); });
    });

    reg("struct", [](const var& m) {
        mod_set(m, "error", make_class("error", "error", {t::Exception}, new_dict(), {}, "struct"));
        mod_def(m, "pack", [](const CallArgs& a) { return new_bytes(pack(fmt_str(a.pos.at(0)), a.pos, 1)); });
        mod_def(m, "unpack", [](const CallArgs& a) {
            bool l, al;
            const std::string& f = fmt_str(a.pos.at(0));
            auto items = parse_fmt(f, l, al);   // (first: calcsize reads the alignment it sets)
            int64_t need = calcsize(items, al);
            const std::string& b = raw_of(a.pos.at(1));
            if ((int64_t)b.size() != need) raise(struct_error("unpack requires a buffer of " + std::to_string(need) + " bytes"));
            return unpack_from(f, b, 0);
        });
        mod_def(m, "unpack_from", [](const CallArgs& a) { return unpack_from(fmt_str(a.pos.at(0)), raw_of(arg(a, 1, "buffer")), arg(a, 2, "offset", 0).as_int64()); });
        mod_def(m, "pack_into", [](const CallArgs& a) {
            std::string data = pack(fmt_str(a.pos.at(0)), a.pos, 3);
            var buf = a.pos.at(1);
            int64_t off = a.pos.at(2).as_int64();
            auto& s = static_cast<BytesObj*>(buf.ref())->s;
            if (off < 0) off += (int64_t)s.size();
            if (off + (int64_t)data.size() > (int64_t)s.size())
                raise(struct_error("pack_into requires a buffer of at least " + std::to_string(off + data.size()) + " bytes for packing " +
                                   std::to_string(data.size()) + " bytes at offset " + std::to_string(off) + " (actual buffer size is " + std::to_string(s.size()) + ")"));
            s.replace((size_t)off, data.size(), data);
            return None;
        });
        mod_def(m, "calcsize", [](const CallArgs& a) { bool l, al; auto it = parse_fmt(fmt_str(a.pos.at(0)), l, al); return var(calcsize(it, al)); });
        mod_def(m, "iter_unpack", [](const CallArgs& a) {
            std::string f = fmt_str(a.pos.at(0)), b = raw_of(a.pos.at(1));
            bool l, al;
            auto items = parse_fmt(f, l, al);
            int64_t n = calcsize(items, al);
            std::vector<var> out;
            for (int64_t off = 0; off + n <= (int64_t)b.size(); off += n) out.push_back(unpack_from(f, b, off));
            return iter(new_list(out));
        });
        // Struct(fmt): the format kept, the module's functions as methods
        var sns = new_dict();
        sns.setitem("__init__"_s, def("__init__", "self, format", {}, [](var self, var f) {
            self.setattr("format", f);
            bool l, al;
            auto items = parse_fmt(fmt_str(f), l, al);
            self.setattr("size", var(calcsize(items, al)));
            return var(None);
        }));
        sns.setitem("unpack"_s, def("unpack", "self, buffer", {}, [](var self, var b) { return import("struct").call("unpack", self.getattr("format"), b); }));
        sns.setitem("unpack_from"_s, def("unpack_from", "self, buffer, offset=", {0}, [](var self, var b, var o) { return import("struct").call("unpack_from", self.getattr("format"), b, o); }));
        sns.setitem("pack"_s, def("pack", "self, *args", {}, [](var self, var args) {
            return import("struct").call("pack", self.getattr("format"), star(args));
        }));
        sns.setitem("pack_into"_s, def("pack_into", "self, buffer, offset, *args", {}, [](var self, var b, var o, var args) {
            return import("struct").call("pack_into", self.getattr("format"), b, o, star(args));
        }));
        sns.setitem("iter_unpack"_s, def("iter_unpack", "self, buffer", {}, [](var self, var b) { return import("struct").call("iter_unpack", self.getattr("format"), b); }));
        mod_set(m, "Struct", make_class("Struct", "Struct", {}, sns, {}, "struct"));
    });

    reg("hashlib", [](const var& m) {
        T_hash = new_type("HASH", {}, Layout::NativeData, true, "_hashlib");
        add_method(T_hash, "update", [](const CallArgs& a) { static_cast<HashObj*>(a.pos[0].ref())->data += raw_of(a.pos.at(1)); return None; });
        add_method(T_hash, "digest", [](const CallArgs& a) { auto* h = static_cast<HashObj*>(a.pos[0].ref()); return new_bytes(digest_of(h->name, h->data)); });
        add_method(T_hash, "hexdigest", [](const CallArgs& a) {
            auto* h = static_cast<HashObj*>(a.pos[0].ref());
            std::string d = digest_of(h->name, h->data), o;
            static const char* hx = "0123456789abcdef";
            for (unsigned char c : d) { o.push_back(hx[c >> 4]); o.push_back(hx[c & 15]); }
            return new_str(o);
        });
        add_method(T_hash, "copy", [](const CallArgs& a) {
            auto* h = static_cast<HashObj*>(a.pos[0].ref());
            var c = new_hash(h->name, None);
            static_cast<HashObj*>(c.ref())->data = h->data;
            return c;
        });
        add_getter(T_hash, "name", [](const var& s) { return new_str(static_cast<HashObj*>(s.ref())->name); });
        add_getter(T_hash, "digest_size", [](const var& s) { return var((int64_t)static_cast<HashObj*>(s.ref())->size); });
        add_getter(T_hash, "block_size", [](const var& s) { return var((int64_t)static_cast<HashObj*>(s.ref())->block); });
        for (const char* n : {"sha1", "sha256", "md5", "sha384", "sha512"}) {
            std::string name = n;
            mod_def(m, n, [name](const CallArgs& a) { return new_hash(name, a.pos.empty() ? kwarg(a, "data", kwarg(a, "string", None)) : a.pos[0]); });
        }
        mod_def(m, "new", [](const CallArgs& a) {
            std::string n = str_utf8(a.pos.at(0));
            for (auto& c : n) c = (char)tolower((unsigned char)c);
            return new_hash(n, a.pos.size() > 1 ? a.pos[1] : var(None));
        });
        mod_set(m, "algorithms_available", set({"sha1"_s, "sha256"_s, "md5"_s, "sha384"_s, "sha512"_s}));
        mod_set(m, "algorithms_guaranteed", m.getattr("algorithms_available"));
    });

    reg("collections", [](const var& m) {
        // defaultdict / OrderedDict / Counter: dicts with a kind
        auto dict_kind = [](const char* name, int kind) {
            TypeObj* t = new_type(name, {T_dict}, Layout::Dict, true, "collections");
            add_static(t, "__new__", [t, kind](const CallArgs& a) {
                var d = new_dict_obj(T(a.pos.at(0)));
                static_cast<DictObj*>(d.ref())->ordered_kind = kind;
                return d;
            });
            // a __repr__ of its own (not dict's object): pprint dispatches on type(x).__repr__'s identity
            add_method(t, "__repr__", [](const CallArgs& a) { return new_str(dict_repr_(a.pos[0])); });
            return t;
        };
        TypeObj* dd = dict_kind("defaultdict", 3);
        add_method(dd, "__init__", [](const CallArgs& a) {
            auto* d = static_cast<DictObj*>(a.pos[0].ref());
            d->default_factory = a.pos.size() > 1 ? a.pos[1] : var(None);
            if (a.pos.size() > 2) dict_update_(a.pos[0], a.pos[2]);
            for (auto& [k, v] : a.kw) d->set(k, v);
            return None;
        });
        add_method(dd, "__missing__", [](const CallArgs& a) {
            auto* d = static_cast<DictObj*>(a.pos[0].ref());
            if (d->default_factory.is_none()) raise(make_exc_args(E_KeyError, {a.pos.at(1)}));
            var v = call0(d->default_factory);
            d->set(a.pos[1], v);
            return v;
        });
        add_getter(dd, "default_factory", [](const var& s) { return static_cast<DictObj*>(s.ref())->default_factory; });
        add_method(dd, "copy", [](const CallArgs& a) {
            var d = new_dict_obj(type_of(a.pos[0]));
            static_cast<DictObj*>(d.ref())->ordered_kind = 3;
            static_cast<DictObj*>(d.ref())->default_factory = static_cast<DictObj*>(a.pos[0].ref())->default_factory;
            dict_update_(d, a.pos[0]);
            return d;
        });
        mod_set(m, "defaultdict", tv(dd));
        TypeObj* od = dict_kind("OrderedDict", 1);
        add_method(od, "move_to_end", [](const CallArgs& a) {
            auto* d = static_cast<DictObj*>(a.pos[0].ref());
            var k = a.pos.at(1);
            bool last = arg(a, 2, "last", true).truthy();
            var* v = d->get(k);
            if (!v) raise(make_exc_args(E_KeyError, {k}));
            var val = *v;
            d->del(k);
            if (last) d->set(k, val);
            else {
                std::vector<std::pair<var, var>> rest;
                for (auto& e : d->entries) if (!e.key.is_unbound()) rest.push_back({e.key, e.value});
                d->clear();
                d->set(k, val);
                for (auto& [x, y] : rest) d->set(x, y);
            }
            return None;
        });
        add_method(od, "popitem", [](const CallArgs& a) {
            auto* d = static_cast<DictObj*>(a.pos[0].ref());
            bool last = arg(a, 1, "last", true).truthy();
            if (d->used == 0) raise(E_KeyError, "dictionary is empty");
            if (last) {
                for (size_t i = d->entries.size(); i-- > 0;)
                    if (!d->entries[i].key.is_unbound()) { var k = d->entries[i].key, v = d->entries[i].value; d->del(k); return new_tuple({k, v}); }
            }
            for (auto& e : d->entries)
                if (!e.key.is_unbound()) { var k = e.key, v = e.value; d->del(k); return new_tuple({k, v}); }
            return None_;
        });
        mod_set(m, "OrderedDict", tv(od));
        TypeObj* ct = dict_kind("Counter", 2);
        auto counter_update = [](const var& self, const var& src, int sign) {
            auto* d = static_cast<DictObj*>(self.ref());
            if (src.is_none() || src.is_unbound()) return;
            if (is_dict(src)) {
                for (var k : each(src)) {
                    var* v = d->get(k);
                    var n = src.getitem(k);
                    d->set(k, v ? (sign > 0 ? *v + n : *v - n) : (sign > 0 ? n : -n));
                }
                return;
            }
            for (var k : each(src)) {
                var* v = d->get(k);
                d->set(k, v ? *v + var(sign) : var(sign));
            }
        };
        add_method(ct, "__init__", [counter_update](const CallArgs& a) {
            if (a.pos.size() > 1) counter_update(a.pos[0], a.pos[1], 1);
            for (auto& [k, v] : a.kw) static_cast<DictObj*>(a.pos[0].ref())->set(k, v);
            return None;
        });
        add_method(ct, "__missing__", [](const CallArgs&) { return var(0); });
        add_method(ct, "update", [counter_update](const CallArgs& a) {
            if (a.pos.size() > 1) counter_update(a.pos[0], a.pos[1], 1);
            for (auto& [k, v] : a.kw) {
                auto* d = static_cast<DictObj*>(a.pos[0].ref());
                var* x = d->get(k);
                d->set(k, x ? *x + v : v);
            }
            return None;
        });
        add_method(ct, "subtract", [counter_update](const CallArgs& a) { if (a.pos.size() > 1) counter_update(a.pos[0], a.pos[1], -1); return None; });
        add_method(ct, "most_common", [](const CallArgs& a) {
            std::vector<var> items;
            for (var k : each(a.pos[0])) items.push_back(new_tuple({k, a.pos[0].getitem(k)}));
            std::stable_sort(items.begin(), items.end(), [](const var& x, const var& y) { return tuple_items(y)[1] < tuple_items(x)[1]; });
            if (a.pos.size() > 1 && !a.pos[1].is_none()) items.resize((size_t)std::min<int64_t>(a.pos[1].as_int64(), (int64_t)items.size()));
            return new_list(items);
        });
        add_method(ct, "elements", [](const CallArgs& a) {
            std::vector<var> out;
            for (var k : each(a.pos[0])) for (int64_t i = 0; i < a.pos[0].getitem(k).as_int64(); i++) out.push_back(k);
            return iter(new_list(out));
        });
        add_method(ct, "total", [](const CallArgs& a) {
            var t0 = 0;
            for (var k : each(a.pos[0])) t0 = t0 + a.pos[0].getitem(k);
            return t0;
        });
        add_method(ct, "__add__", [](const CallArgs& a) {
            var r = call0(tv(type_of(a.pos[0])));
            for (var k : each(a.pos[0])) r.setitem(k, a.pos[0].getitem(k));
            for (var k : each(a.pos.at(1))) r.setitem(k, r.getitem(k) + a.pos[1].getitem(k));
            var out = call0(tv(type_of(a.pos[0])));
            for (var k : each(r)) if (r.getitem(k) > 0) out.setitem(k, r.getitem(k));
            return out;
        });
        mod_set(m, "Counter", tv(ct));
        mod_def(m, "namedtuple", mk_namedtuple);
        // deque
        T_deque = new_type("deque", {}, Layout::NativeData, true, "collections");
        add_static(T_deque, "__new__", [](const CallArgs& a) {
            auto* d = new DequeObj();
            d->type = T(a.pos.at(0));
            var it = arg(a, 1, "iterable", None);
            var ml = arg(a, 2, "maxlen", None);
            if (!ml.is_none()) d->maxlen = ml.as_int64();
            if (!it.is_none()) for (var x : each(it)) { d->v.push_back(x); if (d->maxlen >= 0 && (int64_t)d->v.size() > d->maxlen) d->v.erase(d->v.begin()); }
            return var(static_cast<Obj*>(d));
        });
        add_method(T_deque, "__init__", [](const CallArgs&) { return None; });
        add_method(T_deque, "append", [](const CallArgs& a) {
            auto* d = DQ(a.pos[0]);
            d->v.push_back(a.pos.at(1));
            if (d->maxlen >= 0 && (int64_t)d->v.size() > d->maxlen) d->v.erase(d->v.begin());
            return None;
        });
        add_method(T_deque, "appendleft", [](const CallArgs& a) {
            auto* d = DQ(a.pos[0]);
            d->v.insert(d->v.begin(), a.pos.at(1));
            if (d->maxlen >= 0 && (int64_t)d->v.size() > d->maxlen) d->v.pop_back();
            return None;
        });
        add_method(T_deque, "pop", [](const CallArgs& a) {
            auto* d = DQ(a.pos[0]);
            if (d->v.empty()) raise(E_IndexError, "pop from an empty deque");
            var x = d->v.back();
            d->v.pop_back();
            return x;
        });
        add_method(T_deque, "popleft", [](const CallArgs& a) {
            auto* d = DQ(a.pos[0]);
            if (d->v.empty()) raise(E_IndexError, "pop from an empty deque");
            var x = d->v.front();
            d->v.erase(d->v.begin());
            return x;
        });
        add_method(T_deque, "extend", [](const CallArgs& a) { for (var x : each(a.pos.at(1))) a.pos[0].call("append", x); return None; });
        add_method(T_deque, "extendleft", [](const CallArgs& a) { for (var x : each(a.pos.at(1))) a.pos[0].call("appendleft", x); return None; });
        add_method(T_deque, "clear", [](const CallArgs& a) { DQ(a.pos[0])->v.clear(); return None; });
        add_method(T_deque, "rotate", [](const CallArgs& a) {
            auto& v = DQ(a.pos[0])->v;
            if (v.empty()) return None;
            int64_t n = a.pos.size() > 1 ? a.pos[1].as_int64() : 1;
            int64_t sz = (int64_t)v.size();
            n = ((n % sz) + sz) % sz;
            std::rotate(v.begin(), v.begin() + (sz - n), v.end());
            return None;
        });
        add_method(T_deque, "__len__", [](const CallArgs& a) { return var((int64_t)DQ(a.pos[0])->v.size()); });
        add_method(T_deque, "__getitem__", [](const CallArgs& a) { auto& v = DQ(a.pos[0])->v; return v[(size_t)norm_index(a.pos.at(1).as_int64(), (int64_t)v.size(), "deque")]; });
        add_method(T_deque, "__iter__", [](const CallArgs& a) { return iter(new_list(DQ(a.pos[0])->v)); });
        add_method(T_deque, "__contains__", [](const CallArgs& a) { for (auto& x : DQ(a.pos[0])->v) if (x == a.pos.at(1)) return var(true); return var(false); });
        add_method(T_deque, "__repr__", [](const CallArgs& a) { return new_str("deque(" + repr_(new_list(DQ(a.pos[0])->v)) + ")"); });
        add_method(T_deque, "__bool__", [](const CallArgs& a) { return var(!DQ(a.pos[0])->v.empty()); });
        mod_set(m, "deque", tv(T_deque));
        mod_set(m, "abc", import("collections.abc"));
        // ChainMap, UserDict, UserList, UserString: CPython's own Python (collections/__init__.py, translated
        // as _pycollections when stdlib.txt lists it)
        mod_def(m, "__getattr__", [](const CallArgs& a) -> var {
            std::string n = str_utf8(a.pos.at(0));
            for (const char* k : {"ChainMap", "UserDict", "UserList", "UserString"})
                if (n == k) {
                    if (!module_registered_("_pycollections")) break;
                    var v = import("_pycollections").getattr(k);
                    mod_set(import("collections"), k, v);
                    return v;
                }
            raise(E_AttributeError, "module 'collections' has no attribute '" + n + "'");
        });
    });
    reg("collections.abc", [](const var& m) { make_collections_abc(m); });   // (lib_more.cc: ABCs over the builtins and the protocols)
    reg("_collections_abc", [](const var& m) {
        var src = import("collections.abc");
        for (var k : each(src.getattr("__dict__")))
            if (str_utf8(k)[0] != '_') mod_set(m, str_utf8(k).c_str(), src.getattr(str_utf8(k).c_str()));
    });

    reg("functools", [](const var& m) {
        T_partial = new_type("partial", {}, Layout::NativeData, true, "functools");
        add_static(T_partial, "__new__", [](const CallArgs& a) {
            auto* p = new PartialObj();
            p->type = T_partial;
            p->func = a.pos.at(1);
            p->args.assign(a.pos.begin() + 2, a.pos.end());
            p->kw = a.kw;
            return var(static_cast<Obj*>(p));
        });
        add_method(T_partial, "__init__", [](const CallArgs&) { return None; });
        add_method(T_partial, "__call__", [](const CallArgs& a) {
            auto* p = static_cast<PartialObj*>(a.pos[0].ref());
            CallArgs b;
            b.pos = p->args;
            b.pos.insert(b.pos.end(), a.pos.begin() + 1, a.pos.end());
            b.kw = p->kw;
            for (auto& kv : a.kw) {
                bool replaced = false;
                for (auto& x : b.kw) if (str_utf8(x.first) == str_utf8(kv.first)) { x.second = kv.second; replaced = true; }
                if (!replaced) b.kw.push_back(kv);
            }
            return call_obj(p->func, b);
        });
        add_getter(T_partial, "func", [](const var& s) { return static_cast<PartialObj*>(s.ref())->func; });
        add_getter(T_partial, "args", [](const var& s) { return new_tuple(static_cast<PartialObj*>(s.ref())->args); });
        mod_set(m, "partial", tv(T_partial));
        mod_def(m, "reduce", [](const CallArgs& a) {
            var f = a.pos.at(0), it = iter(a.pos.at(1));
            var acc;
            if (a.pos.size() > 2) acc = a.pos[2];
            else if (!next_(it, acc)) raise(E_TypeError, "reduce() of empty iterable with no initial value");
            var x;
            while (next_(it, x)) acc = call2(f, acc, x);
            return acc;
        });
        // lru_cache / cache: the function memoised on its arguments
        T_lru = new_type("_lru_cache_wrapper", {}, Layout::NativeData, true, "functools");
        add_method(T_lru, "__call__", [](const CallArgs& a) {
            auto* l = static_cast<LruObj*>(a.pos[0].ref());
            std::vector<var> key(a.pos.begin() + 1, a.pos.end());
            for (auto& [k, v] : a.kw) { key.push_back(k); key.push_back(v); }
            var kt = new_tuple(key);
            auto* d = static_cast<DictObj*>(l->cache.ref());
            if (var* v = d->get(kt)) { l->hits++; return *v; }
            l->misses++;
            CallArgs b;
            b.pos.assign(a.pos.begin() + 1, a.pos.end());
            b.kw = a.kw;
            var r = call_obj(l->func, b);
            if (l->maxsize < 0 || d->used < l->maxsize) d->set(kt, r);
            else if (l->maxsize > 0) {
                for (auto& e : d->entries) if (!e.key.is_unbound()) { d->del(var(e.key)); break; }
                d->set(kt, r);
            }
            return r;
        });
        add_method(T_lru, "cache_clear", [](const CallArgs& a) { static_cast<DictObj*>(static_cast<LruObj*>(a.pos[0].ref())->cache.ref())->clear(); return None; });
        add_method(T_lru, "cache_info", [](const CallArgs& a) {
            auto* l = static_cast<LruObj*>(a.pos[0].ref());
            return new_tuple({var(l->hits), var(l->misses), l->maxsize < 0 ? var(None) : var(l->maxsize), var(static_cast<DictObj*>(l->cache.ref())->used)});
        });
        add_method(T_lru, "__get__", [](const CallArgs& a) {
            if (a.pos.at(1).is_none()) return a.pos[0];
            return var(static_cast<Obj*>(new MethodObj())).is_unbound() ? var() : [&]() {
                auto* mo = new MethodObj();
                mo->type = T_method;
                mo->func = a.pos[0];
                mo->self = a.pos[1];
                return var(static_cast<Obj*>(mo));
            }();
        });
        add_getter(T_lru, "__wrapped__", [](const var& s) { return static_cast<LruObj*>(s.ref())->func; });
        add_getter(T_lru, "__name__", [](const var& s) { return static_cast<LruObj*>(s.ref())->func.getattr("__name__"); });
        auto make_lru = [](const var& f, int64_t maxsize) {
            auto* l = new LruObj();
            l->type = T_lru;
            l->func = f;
            l->cache = new_dict_obj();
            l->maxsize = maxsize;
            return var(static_cast<Obj*>(l));
        };
        mod_def(m, "lru_cache", [make_lru](const CallArgs& a) {
            var first = a.pos.empty() ? kwarg(a, "maxsize", 128) : a.pos[0];
            if (callable(first).truthy() && !is_int(first)) return make_lru(first, 128);
            int64_t ms = first.is_none() ? -1 : first.as_int64();
            return new_native("decorating_function", [make_lru, ms](const CallArgs& b) { return make_lru(b.pos.at(0), ms); });
        });
        mod_def(m, "cache", [make_lru](const CallArgs& a) { return make_lru(a.pos.at(0), -1); });
        mod_def(m, "wraps", [](const CallArgs& a) {
            var wrapped = a.pos.at(0);
            return new_native("update_wrapper", [wrapped](const CallArgs& b) {
                var w = b.pos.at(0);
                for (const char* at : {"__module__", "__name__", "__qualname__", "__doc__"}) {
                    try { w.setattr(at, wrapped.getattr(at)); } catch (error&) {}
                }
                try {
                    var d = wrapped.getattr("__dict__");
                    for (var k : each(d)) w.setattr(str_utf8(k).c_str(), d.getitem(k));
                } catch (error&) {}
                try { w.setattr("__wrapped__", wrapped); } catch (error&) {}
                return w;
            });
        });
        mod_def(m, "update_wrapper", [](const CallArgs& a) {
            var w = a.pos.at(0), wrapped = a.pos.at(1);
            for (const char* at : {"__module__", "__name__", "__qualname__", "__doc__"}) {
                try { w.setattr(at, wrapped.getattr(at)); } catch (error&) {}
            }
            try { w.setattr("__wrapped__", wrapped); } catch (error&) {}
            return w;
        });
        mod_def(m, "cmp_to_key", [](const CallArgs& a) {
            var cmpf = a.pos.at(0);
            var ns = new_dict();
            ns.setitem("__init__"_s, def("__init__", "self, obj", {}, [](var self, var obj) { self.setattr("obj", obj); return var(None); }));
            ns.setitem("__lt__"_s, def("__lt__", "self, other", {}, [cmpf](var self, var other) { return var(call2(cmpf, self.getattr("obj"), other.getattr("obj")) < 0); }));
            ns.setitem("__gt__"_s, def("__gt__", "self, other", {}, [cmpf](var self, var other) { return var(call2(cmpf, self.getattr("obj"), other.getattr("obj")) > 0); }));
            ns.setitem("__eq__"_s, def("__eq__", "self, other", {}, [cmpf](var self, var other) { return var(call2(cmpf, self.getattr("obj"), other.getattr("obj")) == 0); }));
            return make_class("K", "K", {}, ns, {}, "functools");
        });
        mod_def(m, "total_ordering", [](const CallArgs& a) {
            var c = a.pos.at(0);
            TypeObj* t = T(c);
            bool has_lt = t->dict->get("__lt__") != nullptr;
            if (has_lt) {
                if (!t->dict->get("__gt__")) t->dict->set("__gt__"_s, def("__gt__", "self, other", {}, [](var s, var o) { return var(o < s); }));
                if (!t->dict->get("__le__")) t->dict->set("__le__"_s, def("__le__", "self, other", {}, [](var s, var o) { return var(!(o < s)); }));
                if (!t->dict->get("__ge__")) t->dict->set("__ge__"_s, def("__ge__", "self, other", {}, [](var s, var o) { return var(!(s < o)); }));
            }
            return c;
        });
        mod_def(m, "cached_property", [](const CallArgs& a) {
            var f = a.pos.at(0);
            var ns = new_dict();
            ns.setitem("__get__"_s, def("__get__", "self, obj, cls=", {None}, [f](var self, var obj, var cls) {
                if (obj.is_none()) return self;
                var name = f.getattr("__name__");
                var v = call1(f, obj);
                obj.setattr(str_utf8(name).c_str(), v);
                return v;
            }));
            return call0(make_class("cached_property", "cached_property", {}, ns, {}, "functools"));
        });
    });

    reg("itertools", [](const var& m) {
        mod_def(m, "count", [](const CallArgs& a) {
            auto n = std::make_shared<var>(arg(a, 0, "start", 0));
            var step = arg(a, 1, "step", 1);
            return new_iter([n, step](var& out) { out = *n; *n = *n + step; return true; });
        });
        mod_def(m, "chain", [](const CallArgs& a) {
            auto its = std::make_shared<std::vector<var>>(a.pos);
            auto cur = std::make_shared<var>();
            auto k = std::make_shared<size_t>(0);
            return new_iter([its, cur, k](var& out) {
                for (;;) {
                    if (cur->is_unbound()) {
                        if (*k >= its->size()) return false;
                        *cur = iter((*its)[(*k)++]);
                    }
                    if (next_(*cur, out)) return true;
                    *cur = var();
                }
            });
        });
        mod_def(m, "product", [](const CallArgs& a) {
            std::vector<std::vector<var>> pools;
            int64_t rep = kwarg(a, "repeat", 1).as_int64();
            for (int64_t r = 0; r < rep; r++) for (auto& x : a.pos) pools.push_back(collect(x));
            std::vector<var> out;
            std::vector<std::vector<var>> res{{}};
            for (auto& p : pools) {
                std::vector<std::vector<var>> nr;
                for (auto& x : res) for (auto& y : p) { auto z = x; z.push_back(y); nr.push_back(z); }
                res.swap(nr);
            }
            for (auto& r : res) out.push_back(new_tuple(r));
            return iter(new_list(out));
        });
        auto combos = [](const std::vector<var>& pool, int64_t r, bool perm, bool repl) {
            std::vector<var> out;
            int64_t n = (int64_t)pool.size();
            if (r > n && !repl) return out;
            std::vector<int64_t> idx;
            std::function<void(std::vector<int64_t>&, std::vector<bool>&)> rec = [&](std::vector<int64_t>& cur, std::vector<bool>& used) {
                if ((int64_t)cur.size() == r) {
                    std::vector<var> t2;
                    for (auto i : cur) t2.push_back(pool[(size_t)i]);
                    out.push_back(new_tuple(t2));
                    return;
                }
                int64_t start = perm ? 0 : (cur.empty() ? 0 : cur.back() + (repl ? 0 : 1));
                for (int64_t i = start; i < n; i++) {
                    if (perm && used[(size_t)i]) continue;
                    cur.push_back(i);
                    if (perm) used[(size_t)i] = true;
                    rec(cur, used);
                    if (perm) used[(size_t)i] = false;
                    cur.pop_back();
                }
            };
            std::vector<bool> used((size_t)n);
            rec(idx, used);
            return out;
        };
        mod_def(m, "combinations", [combos](const CallArgs& a) { return iter(new_list(combos(collect(a.pos.at(0)), a.pos.at(1).as_int64(), false, false))); });
        mod_def(m, "combinations_with_replacement", [combos](const CallArgs& a) { return iter(new_list(combos(collect(a.pos.at(0)), a.pos.at(1).as_int64(), false, true))); });
        mod_def(m, "permutations", [combos](const CallArgs& a) {
            auto pool = collect(a.pos.at(0));
            int64_t r = a.pos.size() > 1 && !a.pos[1].is_none() ? a.pos[1].as_int64() : (int64_t)pool.size();
            return iter(new_list(combos(pool, r, true, false)));
        });
        mod_def(m, "zip_longest", [](const CallArgs& a) {
            std::vector<var> its;
            for (auto& x : a.pos) its.push_back(iter(x));
            var fill = kwarg(a, "fillvalue", None);
            auto done = std::make_shared<std::vector<bool>>(its.size());
            return new_iter([its, fill, done](var& out) {
                std::vector<var> row;
                bool any = false;
                for (size_t i = 0; i < its.size(); i++) {
                    var x;
                    if (!(*done)[i] && next_(its[i], x)) { row.push_back(x); any = true; }
                    else { (*done)[i] = true; row.push_back(fill); }
                }
                if (!any) return false;
                out = new_tuple(row);
                return true;
            });
        });
        mod_def(m, "islice", [](const CallArgs& a) {
            var it = iter(a.pos.at(0));
            int64_t start = 0, stop = -1, step = 1;
            if (a.pos.size() == 2) stop = a.pos[1].is_none() ? -1 : a.pos[1].as_int64();
            else if (a.pos.size() >= 3) {
                start = a.pos[1].is_none() ? 0 : a.pos[1].as_int64();
                stop = a.pos[2].is_none() ? -1 : a.pos[2].as_int64();
                if (a.pos.size() > 3 && !a.pos[3].is_none()) step = a.pos[3].as_int64();
            }
            auto i = std::make_shared<int64_t>(0);
            auto nextpos = std::make_shared<int64_t>(start);
            return new_iter([it, i, nextpos, stop, step](var& out) {
                for (;;) {
                    if (stop >= 0 && *nextpos >= stop) return false;
                    var x;
                    if (!next_(it, x)) return false;
                    if ((*i)++ == *nextpos) { *nextpos += step; out = x; return true; }
                }
            });
        });
        mod_def(m, "groupby", [](const CallArgs& a) {
            auto items = collect(a.pos.at(0));
            var key = arg(a, 1, "key", None);
            std::vector<var> out;
            size_t i = 0;
            while (i < items.size()) {
                var k = key.is_none() ? items[i] : call1(key, items[i]);
                std::vector<var> grp;
                while (i < items.size()) {
                    var k2 = key.is_none() ? items[i] : call1(key, items[i]);
                    if (!(k2 == k)) break;
                    grp.push_back(items[i++]);
                }
                out.push_back(new_tuple({k, iter(new_list(grp))}));
            }
            return iter(new_list(out));
        });
        mod_def(m, "accumulate", [](const CallArgs& a) {
            auto items = collect(a.pos.at(0));
            var f = arg(a, 1, "func", None);
            var init = kwarg(a, "initial", None);
            std::vector<var> out;
            var acc;
            bool have = false;
            if (!init.is_none()) { acc = init; have = true; out.push_back(acc); }
            for (auto& x : items) {
                if (!have) { acc = x; have = true; }
                else acc = f.is_none() ? acc + x : call2(f, acc, x);
                out.push_back(acc);
            }
            return iter(new_list(out));
        });
        mod_def(m, "repeat", [](const CallArgs& a) {
            var x = a.pos.at(0);
            auto n = std::make_shared<int64_t>(a.pos.size() > 1 ? a.pos[1].as_int64() : -1);
            return new_iter([x, n](var& out) {
                if (*n == 0) return false;
                if (*n > 0) (*n)--;
                out = x;
                return true;
            });
        });
        mod_def(m, "takewhile", [](const CallArgs& a) {
            var f = a.pos.at(0), it = iter(a.pos.at(1));
            auto stop = std::make_shared<bool>(false);
            return new_iter([f, it, stop](var& out) {
                if (*stop) return false;
                var x;
                if (!next_(it, x)) return false;
                if (!call1(f, x)) { *stop = true; return false; }
                out = x;
                return true;
            });
        });
        mod_def(m, "dropwhile", [](const CallArgs& a) {
            var f = a.pos.at(0), it = iter(a.pos.at(1));
            auto dropping = std::make_shared<bool>(true);
            return new_iter([f, it, dropping](var& out) {
                var x;
                while (next_(it, x)) {
                    if (*dropping && call1(f, x)) continue;
                    *dropping = false;
                    out = x;
                    return true;
                }
                return false;
            });
        });
        mod_def(m, "starmap", [](const CallArgs& a) {
            var f = a.pos.at(0), it = iter(a.pos.at(1));
            return new_iter([f, it](var& out) {
                var x;
                if (!next_(it, x)) return false;
                CallArgs c;
                c.pos = collect(x);
                out = call_obj(f, c);
                return true;
            });
        });
        mod_def(m, "pairwise", [](const CallArgs& a) {
            auto items = collect(a.pos.at(0));
            std::vector<var> out;
            for (size_t i = 1; i < items.size(); i++) out.push_back(new_tuple({items[i - 1], items[i]}));
            return iter(new_list(out));
        });
        mod_def(m, "tee", [](const CallArgs& a) {
            auto items = collect(a.pos.at(0));
            int64_t n = a.pos.size() > 1 ? a.pos[1].as_int64() : 2;
            std::vector<var> out;
            for (int64_t i = 0; i < n; i++) out.push_back(iter(new_list(items)));
            return new_tuple(out);
        });
        mod_def(m, "filterfalse", [](const CallArgs& a) {
            var f = a.pos.at(0), it = iter(a.pos.at(1));
            return new_iter([f, it](var& out) {
                var x;
                while (next_(it, x)) if (!(f.is_none() ? x.truthy() : call1(f, x).truthy())) { out = x; return true; }
                return false;
            });
        });
        mod_def(m, "batched", [](const CallArgs& a) {
            auto items = collect(a.pos.at(0));
            int64_t n = a.pos.at(1).as_int64();
            std::vector<var> out;
            for (size_t i = 0; i < items.size(); i += (size_t)n)
                out.push_back(new_tuple(std::vector<var>(items.begin() + (long long)i, items.begin() + (long long)std::min(items.size(), i + (size_t)n))));
            return iter(new_list(out));
        });
    });

    reg("math", [](const var& m) {
        mod_set(m, "pi", 3.141592653589793);
        mod_set(m, "e", 2.718281828459045);
        mod_set(m, "tau", 6.283185307179586);
        mod_set(m, "inf", INFINITY);
        mod_set(m, "nan", NAN);
        auto f1 = [m](const char* n, double (*f)(double)) {
            mod_def(m, n, [f, n](const CallArgs& a) {
                double x = a.pos.at(0).as_double();
                double r = f(x);
                if (std::isnan(r) && !std::isnan(x)) raise(E_ValueError, "math domain error");
                return var(r);
            });
        };
        f1("sqrt", [](double x) { return x < 0 ? NAN : std::sqrt(x); });
        f1("sin", std::sin); f1("cos", std::cos); f1("tan", std::tan); f1("atan", std::atan); f1("asin", std::asin); f1("acos", std::acos);
        f1("exp", std::exp); f1("fabs", std::fabs); f1("log10", [](double x) { return x <= 0 ? NAN : std::log10(x); });
        f1("log2", [](double x) { return x <= 0 ? NAN : std::log2(x); });
        f1("sinh", std::sinh); f1("cosh", std::cosh); f1("tanh", std::tanh);
        f1("gamma", [](double x) { return x <= 0 && x == std::floor(x) ? NAN : std::tgamma(x); });
        f1("lgamma", [](double x) { return x <= 0 && x == std::floor(x) ? NAN : std::lgamma(x); });
        f1("erf", std::erf); f1("erfc", std::erfc); f1("expm1", std::expm1); f1("log1p", [](double x) { return x <= -1 ? NAN : std::log1p(x); });
        f1("asinh", std::asinh); f1("acosh", [](double x) { return x < 1 ? NAN : std::acosh(x); }); f1("atanh", [](double x) { return x <= -1 || x >= 1 ? NAN : std::atanh(x); });
        f1("cbrt", std::cbrt); f1("exp2", std::exp2);
        mod_def(m, "log", [](const CallArgs& a) {
            double x = a.pos.at(0).as_double();
            if (x <= 0) raise(E_ValueError, "math domain error");
            double r = std::log(x);
            if (a.pos.size() > 1) r /= std::log(a.pos[1].as_double());
            return var(r);
        });
        mod_def(m, "atan2", [](const CallArgs& a) { return var(std::atan2(a.pos.at(0).as_double(), a.pos.at(1).as_double())); });
        mod_def(m, "pow", [](const CallArgs& a) { return var(std::pow(a.pos.at(0).as_double(), a.pos.at(1).as_double())); });
        mod_def(m, "hypot", [](const CallArgs& a) { double s = 0; for (auto& x : a.pos) s += x.as_double() * x.as_double(); return var(std::sqrt(s)); });
        mod_def(m, "copysign", [](const CallArgs& a) { return var(std::copysign(a.pos.at(0).as_double(), a.pos.at(1).as_double())); });
        mod_def(m, "fmod", [](const CallArgs& a) { return var(std::fmod(a.pos.at(0).as_double(), a.pos.at(1).as_double())); });
        mod_def(m, "isfinite", [](const CallArgs& a) { return var(std::isfinite(a.pos.at(0).as_double())); });
        mod_def(m, "isinf", [](const CallArgs& a) { return var(std::isinf(a.pos.at(0).as_double())); });
        mod_def(m, "isnan", [](const CallArgs& a) { return var(std::isnan(a.pos.at(0).as_double())); });
        mod_def(m, "isclose", [](const CallArgs& a) {
            double x = a.pos.at(0).as_double(), y = a.pos.at(1).as_double();
            double rt = kwarg(a, "rel_tol", 1e-9).as_double(), at = kwarg(a, "abs_tol", 0.0).as_double();
            if (x == y) return var(true);
            return var(std::fabs(x - y) <= std::max(rt * std::max(std::fabs(x), std::fabs(y)), at));
        });
        auto to_int = [](double d) {
            if (std::isinf(d)) raise(E_OverflowError, "cannot convert float infinity to integer");
            if (std::isnan(d)) raise(E_ValueError, "cannot convert float NaN to integer");
            if (std::fabs(d) < 9e18) return var((int64_t)d);
            char b[400];
            snprintf(b, sizeof b, "%.0f", d);
            return new_int(Big::from_decimal(b));
        };
        mod_def(m, "floor", [to_int](const CallArgs& a) { return is_int(a.pos.at(0)) ? a.pos[0] : to_int(std::floor(a.pos[0].as_double())); });
        mod_def(m, "ceil", [to_int](const CallArgs& a) { return is_int(a.pos.at(0)) ? a.pos[0] : to_int(std::ceil(a.pos[0].as_double())); });
        mod_def(m, "trunc", [to_int](const CallArgs& a) { return is_int(a.pos.at(0)) ? a.pos[0] : to_int(std::trunc(a.pos[0].as_double())); });
        mod_def(m, "gcd", [](const CallArgs& a) {
            Big g(0);
            for (auto& x : a.pos) {
                Big y = to_big(x).abs();
                while (y.sign() != 0) { Big r = floormod(g, y); g = y; y = r; }
            }
            return new_int(g);
        });
        mod_def(m, "lcm", [](const CallArgs& a) {
            Big l(1);
            for (auto& x : a.pos) {
                Big y = to_big(x).abs();
                if (y.sign() == 0) return var(0);
                Big g = l, h = y;
                while (h.sign() != 0) { Big r = floormod(g, h); g = h; h = r; }
                l = floordiv(l * y, g);
            }
            return new_int(l);
        });
        mod_def(m, "factorial", [](const CallArgs& a) {
            int64_t n = a.pos.at(0).as_int64();
            if (n < 0) raise(E_ValueError, "factorial() not defined for negative values");
            Big r(1);
            for (int64_t i = 2; i <= n; i++) r = r * Big(i);
            return new_int(r);
        });
        mod_def(m, "isqrt", [](const CallArgs& a) {
            Big n = to_big(a.pos.at(0));
            if (n.sign() < 0) raise(E_ValueError, "isqrt() argument must be nonnegative");
            if (n.sign() == 0) return var(0);
            Big x = shl(Big(1), (n.bit_length() + 1) / 2);
            for (;;) {
                Big y = shr(x + floordiv(n, x), 1);
                if (y >= x) return new_int(x);
                x = y;
            }
        });
        mod_def(m, "degrees", [](const CallArgs& a) { return var(a.pos.at(0).as_double() * 180.0 / 3.141592653589793); });
        mod_def(m, "radians", [](const CallArgs& a) { return var(a.pos.at(0).as_double() * 3.141592653589793 / 180.0); });
        mod_def(m, "prod", [](const CallArgs& a) { var p = kwarg(a, "start", 1); for (var x : each(a.pos.at(0))) p = p * x; return p; });
        mod_def(m, "fsum", [](const CallArgs& a) { double s = 0; for (var x : each(a.pos.at(0))) s += x.as_double(); return var(s); });
        mod_def(m, "frexp", [](const CallArgs& a) { int e; double mm = std::frexp(a.pos.at(0).as_double(), &e); return new_tuple({var(mm), var(e)}); });
        mod_def(m, "ldexp", [](const CallArgs& a) { return var(std::ldexp(a.pos.at(0).as_double(), (int)a.pos.at(1).as_int64())); });
        mod_def(m, "modf", [](const CallArgs& a) { double ip; double fp = std::modf(a.pos.at(0).as_double(), &ip); return new_tuple({var(fp), var(ip)}); });
        mod_def(m, "comb", [](const CallArgs& a) {
            int64_t n = a.pos.at(0).as_int64(), k = a.pos.at(1).as_int64();
            if (k < 0 || k > n) return var(0);
            Big r(1);
            for (int64_t i = 1; i <= k; i++) r = floordiv(r * Big(n - k + i), Big(i));
            return new_int(r);
        });
    });

    reg("string", [](const var& m) {
        mod_set(m, "ascii_letters", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"_s);
        mod_set(m, "ascii_lowercase", "abcdefghijklmnopqrstuvwxyz"_s);
        mod_set(m, "ascii_uppercase", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"_s);
        mod_set(m, "digits", "0123456789"_s);
        mod_set(m, "hexdigits", "0123456789abcdefABCDEF"_s);
        mod_set(m, "octdigits", "01234567"_s);
        mod_set(m, "punctuation", "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"_s);
        mod_set(m, "whitespace", " \t\n\r\x0b\x0c"_s);
        mod_set(m, "printable", "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n\r\x0b\x0c"_s);
        mod_def(m, "capwords", [](const CallArgs& a) {
            var words = a.pos.at(0).call("split");
            std::vector<var> o;
            for (var w : each(words)) o.push_back(w.call("capitalize"));
            return " "_s.call("join", new_list(o));
        });
    });

    reg("textwrap", [](const var& m) {
        mod_def(m, "dedent", [](const CallArgs& a) {
            std::string s = str_utf8(a.pos.at(0));
            // lines of only whitespace normalised to "\n"; the common leading whitespace removed
            std::vector<std::string> lines;
            size_t st = 0;
            for (size_t i = 0; i <= s.size(); i++)
                if (i == s.size() || s[i] == '\n') { lines.push_back(s.substr(st, i - st)); st = i + 1; }
            bool trailing_nl = !s.empty() && s.back() == '\n';
            if (trailing_nl) lines.pop_back();
            std::string margin;
            bool have = false;
            for (auto& l : lines) {
                size_t k = l.find_first_not_of(" \t");
                if (k == std::string::npos) continue;
                std::string ind = l.substr(0, k);
                if (!have) { margin = ind; have = true; continue; }
                size_t c = 0;
                while (c < margin.size() && c < ind.size() && margin[c] == ind[c]) c++;
                margin = margin.substr(0, c);
            }
            std::string out;
            for (size_t i = 0; i < lines.size(); i++) {
                const std::string& l = lines[i];
                if (l.find_first_not_of(" \t") == std::string::npos) out += "";
                else out += l.substr(margin.size());
                if (i + 1 < lines.size() || trailing_nl) out += "\n";
            }
            return new_str(out);
        });
        mod_def(m, "indent", [](const CallArgs& a) {
            std::string s = str_utf8(a.pos.at(0)), pre = str_utf8(a.pos.at(1)), out;
            size_t st = 0;
            for (size_t i = 0; i <= s.size(); i++) {
                if (i == s.size() || s[i] == '\n') {
                    std::string line = s.substr(st, i - st + (i < s.size() ? 1 : 0));
                    if (line.find_first_not_of(" \t\r\n") != std::string::npos) out += pre;
                    out += line;
                    st = i + 1;
                }
            }
            return new_str(out);
        });
        auto wrap = [](const CallArgs& a) {
            std::string s = str_utf8(a.pos.at(0));
            int64_t width = arg(a, 1, "width", 70).as_int64();
            std::string ind = str_utf8(kwarg(a, "initial_indent", ""_s)), sub = str_utf8(kwarg(a, "subsequent_indent", ""_s));
            std::vector<std::string> words;
            std::string w;
            for (char c : s) {
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { if (!w.empty()) words.push_back(w); w.clear(); }
                else w.push_back(c);
            }
            if (!w.empty()) words.push_back(w);
            std::vector<var> lines;
            std::string cur;
            for (auto& x : words) {
                std::string pre = lines.empty() ? ind : sub;
                if (cur.empty()) cur = pre + x;
                else if ((int64_t)(cur.size() + 1 + x.size()) <= width) cur += " " + x;
                else { lines.push_back(new_str(cur)); cur = sub + x; }
            }
            if (!cur.empty()) lines.push_back(new_str(cur));
            return new_list(lines);
        };
        mod_def(m, "wrap", wrap);
        mod_def(m, "fill", [wrap](const CallArgs& a) { return "\n"_s.call("join", wrap(a)); });
        mod_def(m, "shorten", [](const CallArgs& a) {
            std::string s = str_utf8(a.pos.at(0));
            int64_t width = arg(a, 1, "width", 70).as_int64();
            std::string ph = str_utf8(kwarg(a, "placeholder", " [...]"_s));
            std::vector<std::string> words;
            std::string w;
            for (char c : s) { if (isspace((unsigned char)c)) { if (!w.empty()) words.push_back(w); w.clear(); } else w.push_back(c); }
            if (!w.empty()) words.push_back(w);
            std::string joined;
            for (size_t i = 0; i < words.size(); i++) joined += (i ? " " : "") + words[i];
            if ((int64_t)joined.size() <= width) return new_str(joined);
            std::string cur;
            for (auto& x : words) {
                std::string next = cur.empty() ? x : cur + " " + x;
                if ((int64_t)(next.size() + ph.size()) > width) break;
                cur = next;
            }
            return new_str(cur + ph);
        });
    });

    reg("copy", [](const var& m) {
        mod_def(m, "copy", [](const CallArgs& a) {
            var x = a.pos.at(0);
            if (is_list(x)) return new_list(list_items(x));
            if (is_dict(x)) {
                var d = new_dict_obj(x.ref()->type == T_dict ? nullptr : type_of(x));
                static_cast<DictObj*>(d.ref())->ordered_kind = static_cast<DictObj*>(x.ref())->ordered_kind;
                static_cast<DictObj*>(d.ref())->default_factory = static_cast<DictObj*>(x.ref())->default_factory;
                dict_update_(d, x);
                return d;
            }
            if (x.is_ref() && x.ref()->type->layout == Layout::Set) return x.call("copy");
            if (x.is_ref() && x.ref()->type->layout == Layout::ByteArray) return new_bytes(raw_of(x), true);
            var cp;
            if (lookup_special(x, "__copy__", cp)) return call0(cp);
            if (x.is_ref() && !x.ref()->type->builtin && x.ref()->type->layout == Layout::Object) {
                auto* o = new Obj();
                o->type = x.ref()->type;
                var r(o);
                if (x.ref()->attrs) { var d = new_dict_obj(); dict_update_(d, var(x.ref()->attrs)); o->attrs = d.ref(); incref(o->attrs); }
                return r;
            }
            return x;
        });
        mod_def(m, "deepcopy", [](const CallArgs& a) {
            std::function<var(const var&, var&)> dc = [&](const var& x, var& memo) -> var {
                if (!x.is_ref()) return x;
                var key = id(x);
                if (var* v = static_cast<DictObj*>(memo.ref())->get(key)) return *v;
                TypeObj* t = x.ref()->type;
                var r;
                if (t->layout == Layout::List) {
                    r = new_list();
                    memo.setitem(key, r);
                    for (auto& y : list_items(x)) list_items(r).push_back(dc(y, memo));
                } else if (t->layout == Layout::Tuple) {
                    std::vector<var> v;
                    for (auto& y : tuple_items(x)) v.push_back(dc(y, memo));
                    r = new_tuple(v);
                    if (t != T_tuple) { r.ref()->type = t; }
                } else if (t->layout == Layout::Dict) {
                    r = new_dict_obj(t == T_dict ? nullptr : t);
                    static_cast<DictObj*>(r.ref())->ordered_kind = static_cast<DictObj*>(x.ref())->ordered_kind;
                    static_cast<DictObj*>(r.ref())->default_factory = static_cast<DictObj*>(x.ref())->default_factory;
                    memo.setitem(key, r);
                    for (var k : each(x)) r.setitem(dc(k, memo), dc(x.getitem(k), memo));
                } else if (t->layout == Layout::Set) {
                    r = new_set();
                    for (var k : each(x)) r.call("add", dc(k, memo));
                } else if (t->layout == Layout::ByteArray) {
                    r = new_bytes(raw_of(x), true);
                } else if (!t->builtin && (t->layout == Layout::Object || t->layout == Layout::Exception)) {
                    var cp;
                    if (lookup_special(x, "__deepcopy__", cp)) return call1(cp, memo);
                    if (t->layout == Layout::Exception) {
                        auto* e = new ExcObj();
                        e->type = t;
                        e->args = static_cast<ExcObj*>(x.ref())->args;
                        r = var(static_cast<Obj*>(e));
                    } else {
                        auto* o = new Obj();
                        o->type = t;
                        r = var(o);
                    }
                    memo.setitem(key, r);
                    if (x.ref()->attrs) {
                        var d = new_dict_obj();
                        for (var k : each(var(x.ref()->attrs))) d.setitem(k, dc(var(x.ref()->attrs).getitem(k), memo));
                        r.ref()->attrs = d.ref();
                        incref(d.ref());
                    }
                } else r = x;
                return r;
            };
            var memo = new_dict_obj();
            return dc(a.pos.at(0), memo);
        });
    });

    reg("bisect", [](const var& m) {
        auto bis = [](const CallArgs& a, bool right) {
            var lst = a.pos.at(0), x = a.pos.at(1);
            int64_t lo = arg(a, 2, "lo", 0).as_int64();
            var hiv = arg(a, 3, "hi", None);
            var key = kwarg(a, "key", None);
            int64_t hi = hiv.is_none() ? len_(lst) : hiv.as_int64();
            while (lo < hi) {
                int64_t mid = (lo + hi) / 2;
                var v = lst.getitem(mid);
                if (!key.is_none()) v = call1(key, v);
                if (right ? (x < v) : (v < x)) { if (right) hi = mid; else lo = mid + 1; }
                else { if (right) lo = mid + 1; else hi = mid; }
            }
            return lo;
        };
        mod_def(m, "bisect_left", [bis](const CallArgs& a) { return var(bis(a, false)); });
        mod_def(m, "bisect_right", [bis](const CallArgs& a) { return var(bis(a, true)); });
        mod_def(m, "bisect", [bis](const CallArgs& a) { return var(bis(a, true)); });
        mod_def(m, "insort", [bis](const CallArgs& a) { a.pos[0].call("insert", bis(a, true), a.pos.at(1)); return None; });
        mod_def(m, "insort_right", [bis](const CallArgs& a) { a.pos[0].call("insert", bis(a, true), a.pos.at(1)); return None; });
        mod_def(m, "insort_left", [bis](const CallArgs& a) { a.pos[0].call("insert", bis(a, false), a.pos.at(1)); return None; });
    });

    init_more_modules();
}

// collections.namedtuple(typename, field_names, *, defaults=None, module=None): a tuple subclass
static var mk_namedtuple(const CallArgs& a) {
    std::string name = str_utf8(a.pos.at(0));
    var fields_v = a.pos.at(1);
    std::vector<std::string> fields;
    if (is_str(fields_v)) {
        std::string s = str_utf8(fields_v);
        for (auto& c : s) if (c == ',') c = ' ';
        for (var w : each(new_str(s).call("split"))) fields.push_back(str_utf8(w));
    } else for (var f : each(fields_v)) fields.push_back(str_utf8(f));
    var defaults = kwarg(a, "defaults", None);
    std::vector<var> dflts = defaults.is_none() ? std::vector<var>{} : collect(defaults);
    var modv = kwarg(a, "module", None);
    std::string mod = modv.is_none() ? "__main__" : str_utf8(modv);
    var ns = new_dict();
    std::vector<var> fv;
    for (auto& f : fields) fv.push_back(new_str(f));
    var ftuple = new_tuple(fv);
    ns.setitem("_fields"_s, ftuple);
    var dd = new_dict_obj();
    for (size_t i = 0; i < dflts.size(); i++) dd.setitem(fv[fields.size() - dflts.size() + i], dflts[i]);
    ns.setitem("_field_defaults"_s, dd);
    ns.setitem("__slots__"_s, new_tuple());
    size_t n = fields.size();
    ns.setitem("__new__"_s, native("__new__", [n, fields, dflts, name](const CallArgs& c) {
        TypeObj* cls = T(c.pos.at(0));
        std::vector<var> vals(n);
        size_t np = c.pos.size() - 1;
        if (np > n) raise(E_TypeError, name + ".__new__() takes " + std::to_string(n + 1) + " positional arguments but " + std::to_string(np + 1) + " were given");
        for (size_t i = 0; i < np; i++) vals[i] = c.pos[i + 1];
        for (auto& [k, v] : c.kw) {
            auto it = std::find(fields.begin(), fields.end(), str_utf8(k));
            if (it == fields.end()) raise(E_TypeError, name + ".__new__() got an unexpected keyword argument '" + str_utf8(k) + "'");
            vals[(size_t)(it - fields.begin())] = v;
        }
        for (size_t i = 0; i < n; i++)
            if (vals[i].is_unbound()) {
                size_t first_dflt = n - dflts.size();
                if (i >= first_dflt) vals[i] = dflts[i - first_dflt];
                else raise(E_TypeError, name + ".__new__() missing 1 required positional argument: '" + fields[i] + "'");
            }
        var t = new_tuple(vals);
        if (vals.empty()) { auto* o = new TupleObj(); o->type = cls; return var(static_cast<Obj*>(o)); }
        t.ref()->type = cls;
        return t;
    }));
    for (size_t i = 0; i < n; i++) {
        auto* p = new PropertyObj();
        p->type = T_property;
        p->name = fields[i];
        p->fget = native(fields[i].c_str(), [i](const CallArgs& c) { return tuple_items(c.pos.at(0))[i]; });
        ns.setitem(fv[i], var(static_cast<Obj*>(p)));
    }
    ns.setitem("__repr__"_s, native("__repr__", [fields, name](const CallArgs& c) {
        std::string r = type_of(c.pos[0])->name + "(";
        auto& v = tuple_items(c.pos[0]);
        for (size_t i = 0; i < fields.size(); i++) r += (i ? ", " : "") + fields[i] + "=" + repr_(v[i]);
        return new_str(r + ")");
    }));
    ns.setitem("_asdict"_s, native("_asdict", [fields](const CallArgs& c) {
        var d = new_dict_obj();
        auto& v = tuple_items(c.pos[0]);
        for (size_t i = 0; i < fields.size(); i++) d.setitem(new_str(fields[i]), v[i]);
        return d;
    }));
    ns.setitem("_replace"_s, native("_replace", [fields](const CallArgs& c) {
        std::vector<var> v = tuple_items(c.pos[0]);
        for (auto& [k, x] : c.kw) {
            auto it = std::find(fields.begin(), fields.end(), str_utf8(k));
            if (it == fields.end()) raise(E_ValueError, "Got unexpected field names: ['" + str_utf8(k) + "']");
            v[(size_t)(it - fields.begin())] = x;
        }
        var t = new_tuple(v);
        t.ref()->type = type_of(c.pos[0]);
        return t;
    }));
    ns.setitem("__getnewargs__"_s, native("__getnewargs__", [](const CallArgs& c) { return new_tuple(tuple_items(c.pos[0])); }));
    ns.setitem("__module__"_s, new_str(mod));
    var c = make_class(name.c_str(), name.c_str(), {t::tuple}, ns, {}, mod.c_str());
    TypeObj* ct = T(c);
    add_classmethod(ct, "_make", [](const CallArgs& c2) {
        CallArgs b;
        b.pos = collect(c2.pos.at(1));
        return call_obj(c2.pos[0], b);
    });
    return c;
}

}  // namespace py
