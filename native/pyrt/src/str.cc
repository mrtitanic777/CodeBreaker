// str.cc -- Python's str: UTF-8 held, code points counted (an ASCII string indexes its bytes
// directly), every method CodeBreaker calls with CPython's results and errors, repr's quoting and
// escapes, the format-spec mini-language (format(), f-strings, str.format) and %-formatting.
#include <windows.h>

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstring>

#include "../../ported/include/py_unicode.hpp"
#include "rt.hpp"

namespace py {

// ------------------------------------------------------------------------------------------ UTF-8
int64_t cp_count(const std::string& s) {
    int64_t n = 0;
    for (unsigned char c : s) n += (c & 0xC0) != 0x80;
    return n;
}
void append_utf8(std::string& out, char32_t c) {
    if (c < 0x80) out.push_back((char)c);
    else if (c < 0x800) { out.push_back((char)(0xC0 | (c >> 6))); out.push_back((char)(0x80 | (c & 0x3F))); }
    else if (c < 0x10000) {
        out.push_back((char)(0xE0 | (c >> 12)));
        out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (c & 0x3F)));
    } else {
        out.push_back((char)(0xF0 | (c >> 18)));
        out.push_back((char)(0x80 | ((c >> 12) & 0x3F)));
        out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (c & 0x3F)));
    }
}
std::u32string to_u32(const std::string& s) {
    std::u32string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size();) {
        unsigned char c = (unsigned char)s[i];
        char32_t cp;
        int len;
        if (c < 0x80) { cp = c; len = 1; }
        else if ((c >> 5) == 6) { cp = c & 0x1F; len = 2; }
        else if ((c >> 4) == 14) { cp = c & 0x0F; len = 3; }
        else { cp = c & 0x07; len = 4; }
        for (int k = 1; k < len && i + k < s.size(); ++k) cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
        out.push_back(cp);
        i += len;
    }
    return out;
}
std::string from_u32(const std::u32string& s) {
    std::string out;
    out.reserve(s.size());
    for (char32_t c : s) append_utf8(out, c);
    return out;
}
const std::u32string& StrObj::u32() const {
    if (!cps) cps = new std::u32string(to_u32(s));
    return *cps;
}
size_t cp_offset(const StrObj* s, int64_t i) {
    if (s->ascii) return (size_t)i;
    size_t b = 0;
    for (int64_t k = 0; k < i && b < s->s.size(); k++) {
        b++;
        while (b < s->s.size() && ((unsigned char)s->s[b] & 0xC0) == 0x80) b++;
    }
    return b;
}
int64_t cp_index(const StrObj* s, size_t byte) {
    if (s->ascii) return (int64_t)byte;
    int64_t n = 0;
    for (size_t k = 0; k < byte && k < s->s.size(); k++) n += ((unsigned char)s->s[k] & 0xC0) != 0x80;
    return n;
}

var new_str(std::string utf8) {
    auto* o = new StrObj();
    o->type = T_str;
    o->n = 0;
    bool ascii = true;
    for (unsigned char c : utf8) {
        if (c >= 0x80) ascii = false;
        o->n += (c & 0xC0) != 0x80;
    }
    o->ascii = ascii;
    o->s = std::move(utf8);
    return var(static_cast<Obj*>(o));
}
var new_str_cp(const std::u32string& cps) { return new_str(from_u32(cps)); }
var str_from(const std::string& utf8) { return new_str(utf8); }
bool is_str(const var& x) { return x.is_ref() && x.ref()->type->layout == Layout::Str; }
const std::string& str_utf8(const var& s) {
    if (!is_str(s)) raise(E_TypeError, "expected str, got " + tname(s));
    return static_cast<StrObj*>(s.ref())->s;
}
std::string utf8_of(const var& s) { return str_utf8(s); }
const std::string& var::utf8() const { return str_utf8(*this); }
static StrObj* S(const var& v) { return static_cast<StrObj*>(v.ref()); }

int64_t str_hash_(const StrObj* s) {
    if (s->h != -1) return s->h;
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s->s) { h ^= c; h *= 1099511628211ULL; }
    int64_t r = (int64_t)(h >> 1);
    if (r == -1) r = -2;
    s->h = r;
    return r;
}

// ------------------------------------------------------------------------------------------ classes of characters
static bool is_space_cp(char32_t c) {
    return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F) || c == 0x85 || c == 0xA0 || c == 0x1680 ||
           (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
}
static char32_t map_case(char32_t c, bool upper) {
    if (c < 0x80) {
        if (upper && c >= 'a' && c <= 'z') return c - 32;
        if (!upper && c >= 'A' && c <= 'Z') return c + 32;
        return c;
    }
    if (c > 0xFFFF) return c;
    wchar_t in = (wchar_t)c, out = 0;
    if (LCMapStringEx(LOCALE_NAME_INVARIANT, upper ? LCMAP_UPPERCASE : LCMAP_LOWERCASE, &in, 1, &out, 1, nullptr, nullptr, 0) == 1)
        return out;
    return c;
}
static std::string case_map(const StrObj* s, int mode) {   // 0 lower, 1 upper, 2 swapcase
    if (s->ascii) {
        std::string o = s->s;
        for (auto& c : o) {
            if (mode == 0 || (mode == 2 && c >= 'A' && c <= 'Z')) { if (c >= 'A' && c <= 'Z') c += 32; }
            else if (c >= 'a' && c <= 'z') c -= 32;
        }
        return o;
    }
    std::string o;
    for (char32_t c : s->u32()) {
        if (mode == 1 && c == 0xDF) { o += "SS"; continue; }
        if (mode == 2) {
            char32_t u = map_case(c, true), l = map_case(c, false);
            append_utf8(o, u != c ? u : l);
        } else {
            append_utf8(o, map_case(c, mode == 1));
        }
    }
    return o;
}
static bool is_upper_cp(char32_t c) { return map_case(c, false) != c; }
static bool is_lower_cp(char32_t c) { return map_case(c, true) != c || c == 0xDF; }
static bool is_printable_cp(char32_t c) {
    if (c < 0x20 || c == 0x7F) return false;
    if (c >= 0x80 && c <= 0xA0) return false;
    if (c == 0xAD) return false;
    if (c >= 0xD800 && c <= 0xDFFF) return false;
    if (c == 0x2028 || c == 0x2029 || (c >= 0x2000 && c <= 0x200F) || (c >= 0x202A && c <= 0x202F) || (c >= 0x2060 && c <= 0x206F)) return false;
    if (c == 0xFEFF || (c >= 0xFFF0 && c <= 0xFFFB) || c == 0xFFFE || c == 0xFFFF) return false;
    if (c >= 0xE000 && c <= 0xF8FF) return false;
    if (c > 0x10FFFF) return false;
    return true;
}
static bool is_alnum_cp(char32_t c) { return unicode_::isalpha(c) || unicode_::isnumeric(c) || unicode_::isdecimal(c) || unicode_::isdigit(c); }

std::string str_repr_(const std::string& s) {
    bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
    char q = (sq && !dq) ? '"' : '\'';
    std::string o;
    o.push_back(q);
    for (char32_t c : to_u32(s)) {
        if (c == (char32_t)q || c == '\\') { o.push_back('\\'); o.push_back((char)c); }
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c < 0x20 || c == 0x7F) { char b[8]; snprintf(b, sizeof b, "\\x%02x", (unsigned)c); o += b; }
        else if (c < 0x7F) o.push_back((char)c);
        else if (is_printable_cp(c)) append_utf8(o, c);
        else if (c < 0x100) { char b[8]; snprintf(b, sizeof b, "\\x%02x", (unsigned)c); o += b; }
        else if (c < 0x10000) { char b[8]; snprintf(b, sizeof b, "\\u%04x", (unsigned)c); o += b; }
        else { char b[12]; snprintf(b, sizeof b, "\\U%08x", (unsigned)c); o += b; }
    }
    o.push_back(q);
    return o;
}

// ------------------------------------------------------------------------------------------ slices of strs
static var str_sub(const StrObj* s, int64_t a, int64_t b) {   // code points [a, b)
    if (a >= b) return str_lit_("", 0);
    size_t x = cp_offset(s, a);
    size_t y = s->ascii ? (size_t)b : x + (cp_offset(s, b) - cp_offset(s, a));
    if (!s->ascii) y = cp_offset(s, b);
    return new_str(s->s.substr(x, y - x));
}
var str_getitem_(const var& self, const var& key) {
    StrObj* s = S(self);
    if (is_int(key)) {
        int64_t i = norm_index(key.as_int64(), s->n, "string");
        if (s->ascii) return new_str(std::string(1, s->s[(size_t)i]));
        std::string o;
        append_utf8(o, s->u32()[(size_t)i]);
        return new_str(o);
    }
    if (has_layout(key, Layout::Slice)) {
        auto* sl = static_cast<SliceObj*>(key.ref());
        int64_t lo, hi, st, cnt;
        slice_indices(sl->start, sl->stop, sl->step, s->n, lo, hi, st, cnt);
        if (st == 1) return str_sub(s, lo, lo + cnt);
        std::string o;
        if (s->ascii) for (int64_t k = 0, i = lo; k < cnt; k++, i += st) o.push_back(s->s[(size_t)i]);
        else for (int64_t k = 0, i = lo; k < cnt; k++, i += st) append_utf8(o, s->u32()[(size_t)i]);
        return new_str(o);
    }
    raise(E_TypeError, "string indices must be integers, not '" + tname(key) + "'");
}

// ------------------------------------------------------------------------------------------ search helpers
static void sub_range(const StrObj* s, const CallArgs& a, size_t first, int64_t& lo, int64_t& hi) {
    int64_t n = s->n;
    lo = 0; hi = n;
    if (a.pos.size() > first && !a.pos[first].is_none()) { lo = a.pos[first].as_int64(); if (lo < 0) lo = std::max<int64_t>(0, lo + n); }
    if (a.pos.size() > first + 1 && !a.pos[first + 1].is_none()) { hi = a.pos[first + 1].as_int64(); if (hi < 0) hi = std::max<int64_t>(0, hi + n); }
    if (hi > n) hi = n;
}
static int64_t str_find(const StrObj* s, const std::string& sub, int64_t lo, int64_t hi, bool rev) {
    if (lo > hi) return -1;
    if (lo > s->n) return -1;
    size_t a = cp_offset(s, lo), b = cp_offset(s, hi);
    if (sub.size() > b - a) return -1;
    size_t p;
    if (!rev) {
        p = s->s.find(sub, a);
        if (p == std::string::npos || p + sub.size() > b) return -1;
    } else {
        p = s->s.rfind(sub, b - sub.size());
        if (p == std::string::npos || p < a) return -1;
    }
    return cp_index(s, p);
}
static std::vector<std::string> split_ws(const std::string& s, int64_t maxsplit, bool ascii) {
    std::vector<std::string> out;
    if (ascii) {
        size_t i = 0, n = s.size();
        while (i < n) {
            while (i < n && is_space_cp((unsigned char)s[i])) i++;
            if (i >= n) break;
            if (maxsplit >= 0 && (int64_t)out.size() >= maxsplit) {
                size_t e = n;
                while (e > i && is_space_cp((unsigned char)s[e - 1])) e--;
                out.push_back(s.substr(i, e - i));
                return out;
            }
            size_t j = i;
            while (j < n && !is_space_cp((unsigned char)s[j])) j++;
            out.push_back(s.substr(i, j - i));
            i = j;
        }
        return out;
    }
    std::u32string u = to_u32(s);
    size_t i = 0, n = u.size();
    while (i < n) {
        while (i < n && is_space_cp(u[i])) i++;
        if (i >= n) break;
        if (maxsplit >= 0 && (int64_t)out.size() >= maxsplit) {
            size_t e = n;
            while (e > i && is_space_cp(u[e - 1])) e--;
            out.push_back(from_u32(u.substr(i, e - i)));
            return out;
        }
        size_t j = i;
        while (j < n && !is_space_cp(u[j])) j++;
        out.push_back(from_u32(u.substr(i, j - i)));
        i = j;
    }
    return out;
}
static std::vector<std::string> rsplit_ws(const std::string& s, int64_t maxsplit) {
    std::u32string u = to_u32(s);
    std::vector<std::string> out;
    int64_t i = (int64_t)u.size();
    while (i > 0) {
        while (i > 0 && is_space_cp(u[(size_t)i - 1])) i--;
        if (i <= 0) break;
        if (maxsplit >= 0 && (int64_t)out.size() >= maxsplit) {
            int64_t b = 0;
            while (b < i && is_space_cp(u[(size_t)b])) b++;
            out.push_back(from_u32(u.substr((size_t)b, (size_t)(i - b))));
            break;
        }
        int64_t j = i;
        while (j > 0 && !is_space_cp(u[(size_t)j - 1])) j--;
        out.push_back(from_u32(u.substr((size_t)j, (size_t)(i - j))));
        i = j;
    }
    std::reverse(out.begin(), out.end());
    return out;
}
static var str_list(const std::vector<std::string>& v) {
    std::vector<var> out;
    out.reserve(v.size());
    for (auto& x : v) out.push_back(new_str(x));
    return new_list(std::move(out));
}
static std::string strip_impl(const StrObj* s, const var& chars, int which) {   // 1 left, 2 right, 3 both
    if (chars.is_unbound() || chars.is_none()) {
        if (s->ascii) {
            size_t a = 0, b = s->s.size();
            if (which & 1) while (a < b && is_space_cp((unsigned char)s->s[a])) a++;
            if (which & 2) while (b > a && is_space_cp((unsigned char)s->s[b - 1])) b--;
            return s->s.substr(a, b - a);
        }
        const auto& u = s->u32();
        size_t a = 0, b = u.size();
        if (which & 1) while (a < b && is_space_cp(u[a])) a++;
        if (which & 2) while (b > a && is_space_cp(u[b - 1])) b--;
        return from_u32(u.substr(a, b - a));
    }
    std::u32string cs = to_u32(str_utf8(chars));
    const auto& u = s->u32();
    size_t a = 0, b = u.size();
    if (which & 1) while (a < b && cs.find(u[a]) != std::u32string::npos) a++;
    if (which & 2) while (b > a && cs.find(u[b - 1]) != std::u32string::npos) b--;
    return from_u32(u.substr(a, b - a));
}
static std::string replace_impl(const std::string& s, const std::string& old, const std::string& rep, int64_t count, bool ascii) {
    if (count == 0) return s;
    std::string out;
    if (old.empty()) {
        // between every code point
        std::u32string u = to_u32(s);
        int64_t n = 0;
        for (size_t i = 0; i <= u.size(); i++) {
            if (count < 0 || n < count) { out += rep; n++; }
            if (i < u.size()) append_utf8(out, u[i]);
        }
        (void)ascii;
        return out;
    }
    size_t p = 0;
    int64_t n = 0;
    for (;;) {
        size_t q = s.find(old, p);
        if (q == std::string::npos || (count >= 0 && n >= count)) break;
        out.append(s, p, q - p);
        out += rep;
        p = q + old.size();
        n++;
    }
    out.append(s, p, std::string::npos);
    return out;
}
static std::vector<std::string> splitlines_impl(const std::string& s, bool keep) {
    std::vector<std::string> out;
    std::u32string u = to_u32(s);
    size_t i = 0, start = 0;
    while (i < u.size()) {
        char32_t c = u[i];
        bool br = c == '\n' || c == '\r' || c == 0x0B || c == 0x0C || c == 0x1C || c == 0x1D || c == 0x1E || c == 0x85 || c == 0x2028 || c == 0x2029;
        if (!br) { i++; continue; }
        size_t eol = i;
        i++;
        if (c == '\r' && i < u.size() && u[i] == '\n') i++;
        out.push_back(from_u32(u.substr(start, (keep ? i : eol) - start)));
        start = i;
    }
    if (start < u.size()) out.push_back(from_u32(u.substr(start)));
    return out;
}

// ------------------------------------------------------------------------------------------ format spec
struct Spec {
    char32_t fill = ' ';
    char align = 0;
    char sign = '-';
    bool alt = false, zero = false;
    int64_t width = -1, prec = -1;
    char grouping = 0;
    char type = 0;
};
static Spec parse_spec(const std::string& spec_utf8) {
    Spec p;
    std::u32string s = to_u32(spec_utf8);
    size_t i = 0;
    auto is_align = [](char32_t c) { return c == '<' || c == '>' || c == '^' || c == '='; };
    if (s.size() >= 2 && is_align(s[1])) { p.fill = s[0]; p.align = (char)s[1]; i = 2; }
    else if (!s.empty() && is_align(s[0])) { p.align = (char)s[0]; i = 1; }
    if (i < s.size() && (s[i] == '+' || s[i] == '-' || s[i] == ' ')) p.sign = (char)s[i++];
    if (i < s.size() && s[i] == '#') { p.alt = true; i++; }
    if (i < s.size() && s[i] == '0') { p.zero = true; i++; }
    if (i < s.size() && s[i] >= '0' && s[i] <= '9') {
        p.width = 0;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') p.width = p.width * 10 + (s[i++] - '0');
    }
    if (i < s.size() && (s[i] == ',' || s[i] == '_')) p.grouping = (char)s[i++];
    if (i < s.size() && s[i] == '.') {
        i++;
        p.prec = 0;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') p.prec = p.prec * 10 + (s[i++] - '0');
    }
    if (i < s.size()) p.type = (char)s[i++];
    if (i < s.size()) raise(E_ValueError, "Invalid format specifier '" + spec_utf8 + "'");
    if (p.zero && !p.align) { p.fill = '0'; p.align = '='; }
    return p;
}
static std::string pad(const std::string& sign, const std::string& body, const Spec& p, char dflt_align) {
    int64_t len = cp_count(sign) + cp_count(body);
    char al = p.align ? p.align : dflt_align;
    if (p.width <= len) return sign + body;
    int64_t n = p.width - len;
    std::string f;
    append_utf8(f, p.fill);
    auto rep = [&](int64_t k) { std::string o; for (int64_t i = 0; i < k; i++) o += f; return o; };
    if (al == '<') return sign + body + rep(n);
    if (al == '>') return rep(n) + sign + body;
    if (al == '^') return rep(n / 2) + sign + body + rep(n - n / 2);
    return sign + rep(n) + body;   // '='
}
static std::string group_digits(const std::string& d, char sep, int every) {
    std::string out;
    int64_t n = (int64_t)d.size();
    for (int64_t i = 0; i < n; i++) {
        out.push_back(d[(size_t)i]);
        int64_t left = n - 1 - i;
        if (left > 0 && left % every == 0) out.push_back(sep);
    }
    return out;
}
std::string int_to_base(const Big& b, int base, bool upper) {
    if (base == 10) return b.abs().str();
    Big x = b.abs();
    if (x.sign() == 0) return "0";
    std::string out;
    const char* dig = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    int bits = base == 16 ? 4 : base == 8 ? 3 : 1;
    while (x.sign() > 0) {
        Big r = floormod(x, Big(base));
        out.push_back(dig[r.i64()]);
        x = shr(x, bits);
    }
    std::reverse(out.begin(), out.end());
    return out;
}
static std::string format_int(const Big& v, const Spec& p) {
    char t = p.type ? p.type : 'd';
    std::string body, prefix;
    switch (t) {
    case 'd': case 'n': body = v.abs().str(); break;
    case 'x': body = int_to_base(v, 16); if (p.alt) prefix = "0x"; break;
    case 'X': body = int_to_base(v, 16, true); if (p.alt) prefix = "0X"; break;
    case 'o': body = int_to_base(v, 8); if (p.alt) prefix = "0o"; break;
    case 'b': body = int_to_base(v, 2); if (p.alt) prefix = "0b"; break;
    case 'c': { std::string o; append_utf8(o, (char32_t)v.i64()); return pad("", o, p, '<'); }
    default: raise(E_ValueError, std::string("Unknown format code '") + t + "' for object of type 'int'");
    }
    if (p.grouping) body = group_digits(body, p.grouping, (t == 'd' || t == 'n') ? 3 : 4);
    std::string sign = v.sign() < 0 ? "-" : (p.sign == '+' ? "+" : p.sign == ' ' ? " " : "");
    if (p.align == '=' && p.fill == '0' && p.grouping && p.width > 0) {
        // zero padding also takes the separators
        std::string digits = v.abs().str();
        int64_t want = p.width - (int64_t)(sign.size() + prefix.size());
        std::string d = digits;
        while ((int64_t)group_digits(d, p.grouping, 3).size() < want) d = "0" + d;
        return sign + prefix + group_digits(d, p.grouping, 3);
    }
    return pad(sign + prefix, body, p, '>');
}
static std::string fixed(double v, int prec) {
    char buf[512];
    snprintf(buf, sizeof buf, "%.*f", prec, v);
    return buf;
}
static std::string format_float(double v, const Spec& p) {
    char t = p.type;
    std::string sign = std::signbit(v) && !std::isnan(v) ? "-" : (p.sign == '+' ? "+" : p.sign == ' ' ? " " : "");
    double a = std::fabs(v);
    std::string body;
    if (std::isinf(a) || std::isnan(a)) {
        body = std::isnan(a) ? "nan" : "inf";
        if (t == 'F' || t == 'E' || t == 'G') for (auto& c : body) c = (char)toupper(c);
        return pad(sign, body, p, '>');
    }
    int prec = p.prec < 0 ? 6 : (int)p.prec;
    char buf[512];
    switch (t) {
    case 'f': case 'F': body = fixed(a, prec); break;
    case 'e': case 'E':
        snprintf(buf, sizeof buf, t == 'e' ? "%.*e" : "%.*E", prec, a);
        body = buf;
        break;
    case '%': body = fixed(a * 100, prec) + "%"; break;
    case 'g': case 'G': case 0: {
        if (t == 0 && p.prec < 0) { body = repr_float(a); break; }
        int pr = prec == 0 ? 1 : prec;
        if (t == 0 && p.prec >= 0) {
            // like 'g' but at least one digit after the point when fixed
            snprintf(buf, sizeof buf, "%.*g", pr, a);
            body = buf;
            if (body.find_first_of(".e") == std::string::npos && body != "inf") body += ".0";
            auto e = body.find('e');
            if (e != std::string::npos) {
                std::string m = body.substr(0, e), ex = body.substr(e + 1);
                int en = std::atoi(ex.c_str());
                char eb[16];
                snprintf(eb, sizeof eb, "e%c%02d", en < 0 ? '-' : '+', en < 0 ? -en : en);
                body = m + eb;
            }
            break;
        }
        snprintf(buf, sizeof buf, p.alt ? "%#.*g" : "%.*g", pr, a);
        body = buf;
        if (t == 'G') for (auto& c : body) c = (char)toupper(c);
        break;
    }
    default: raise(E_ValueError, std::string("Unknown format code '") + t + "' for object of type 'float'");
    }
    if (p.grouping) {
        auto dot = body.find_first_of(".eE%");
        std::string ip = body.substr(0, dot), rest = dot == std::string::npos ? "" : body.substr(dot);
        body = group_digits(ip, p.grouping, 3) + rest;
    }
    return pad(sign, body, p, '>');
}
std::string format_spec(const var& v, const std::string& spec) {
    if (is_str(v)) {
        if (spec.empty()) return str_utf8(v);
        Spec p = parse_spec(spec);
        if (p.type && p.type != 's') raise(E_ValueError, std::string("Unknown format code '") + p.type + "' for object of type 'str'");
        std::string s = str_utf8(v);
        if (p.prec >= 0 && cp_count(s) > p.prec) s = from_u32(to_u32(s).substr(0, (size_t)p.prec));
        return pad("", s, p, '<');
    }
    if (v.is_bool() && spec.empty()) return v.small() ? "True" : "False";
    if (is_int(v)) {
        if (spec.empty()) return to_big(v).str();
        Spec p = parse_spec(spec);
        if (p.type && strchr("eEfFgG%", p.type)) return format_float(to_big(v).fits_i64() ? (double)v.as_int64() : v.as_double(), p);
        return format_int(to_big(v), p);
    }
    if (v.is_float() || has_layout(v, Layout::Float)) {
        Spec p = parse_spec(spec);
        return format_float(v.as_double(), p);
    }
    if (spec.empty()) return str_(v);
    var m;
    if (v.is_ref() && !v.ref()->type->builtin && lookup_special(v, "__format__", m)) return str_utf8(call1(m, new_str(spec)));
    raise(E_TypeError, "unsupported format string passed to " + tname(v) + ".__format__");
}
var format(const var& x, const var& spec) {
    std::string s = spec.is_none() || spec.is_unbound() ? "" : str_utf8(spec);
    var m;
    if (x.is_ref() && !x.ref()->type->builtin && lookup_special(x, "__format__", m)) return call1(m, new_str(s));
    return new_str(format_spec(x, s));
}
var fmt(const var& v, char conv, const var& spec) {
    var x = v;
    if (conv == 'r') x = new_str(repr_(v));
    else if (conv == 's') x = new_str(str_(v));
    else if (conv == 'a') x = ascii(v);
    if (spec.is_none() || spec.is_unbound()) {
        if (is_str(x)) return x;
        if (x.is_small_int()) return new_str(std::to_string(x.small()));
        return format(x, None);
    }
    return format(x, spec);
}
var fstr(std::initializer_list<var> parts) {
    std::string out;
    for (auto& p : parts) out += str_utf8(p);
    return new_str(std::move(out));
}

// str.format(*args, **kwargs)
static var format_field(const std::string& field, const CallArgs& a, size_t& auto_idx) {
    // field: name or index, then .attr / [key] chains
    size_t i = 0;
    while (i < field.size() && field[i] != '.' && field[i] != '[') i++;
    std::string head = field.substr(0, i);
    var v;
    if (head.empty()) {
        if (auto_idx >= a.pos.size()) raise(E_IndexError, "Replacement index " + std::to_string(auto_idx) + " out of range for positional args tuple");
        v = a.pos[auto_idx++];
    } else if (isdigit((unsigned char)head[0])) {
        size_t k = std::stoul(head);
        if (k >= a.pos.size()) raise(E_IndexError, "Replacement index " + head + " out of range for positional args tuple");
        v = a.pos[k];
    } else {
        bool found = false;
        for (auto& [k, x] : a.kw)
            if (str_utf8(k) == head) { v = x; found = true; break; }
        if (!found) raise(E_KeyError, "'" + head + "'");
    }
    while (i < field.size()) {
        if (field[i] == '.') {
            size_t j = i + 1;
            while (j < field.size() && field[j] != '.' && field[j] != '[') j++;
            v = v.getattr(new_str(field.substr(i + 1, j - i - 1)));
            i = j;
        } else {
            size_t j = field.find(']', i);
            std::string key = field.substr(i + 1, j - i - 1);
            bool num = !key.empty() && std::all_of(key.begin(), key.end(), [](char c) { return isdigit((unsigned char)c); });
            v = v.getitem(num ? var((int64_t)std::stoll(key)) : new_str(key));
            i = j + 1;
        }
    }
    return v;
}
static std::string str_format(const std::string& f, const CallArgs& a, size_t& auto_idx, int depth = 0) {
    std::string out;
    for (size_t i = 0; i < f.size(); i++) {
        char c = f[i];
        if (c == '{') {
            if (i + 1 < f.size() && f[i + 1] == '{') { out.push_back('{'); i++; continue; }
            int lvl = 1;
            size_t j = i + 1;
            for (; j < f.size() && lvl; j++) {
                if (f[j] == '{') lvl++;
                else if (f[j] == '}') lvl--;
            }
            if (lvl) raise(E_ValueError, "expected '}' before end of string");
            std::string inner = f.substr(i + 1, j - i - 2);
            std::string field = inner, spec;
            char conv = 0;
            size_t colon = std::string::npos, bang = std::string::npos;
            int br = 0;
            for (size_t k = 0; k < inner.size(); k++) {
                if (inner[k] == '[') br++;
                else if (inner[k] == ']') br--;
                else if (!br && inner[k] == '!' && bang == std::string::npos && colon == std::string::npos) bang = k;
                else if (!br && inner[k] == ':' && colon == std::string::npos) { colon = k; break; }
            }
            if (colon != std::string::npos) { spec = inner.substr(colon + 1); field = inner.substr(0, colon); }
            if (bang != std::string::npos) { conv = field[bang + 1]; field = field.substr(0, bang); }
            var v = format_field(field, a, auto_idx);
            if (spec.find('{') != std::string::npos && depth < 2) spec = str_format(spec, a, auto_idx, depth + 1);
            out += str_utf8(fmt(v, conv, new_str(spec)));
            i = j - 1;
        } else if (c == '}') {
            if (i + 1 < f.size() && f[i + 1] == '}') { out.push_back('}'); i++; continue; }
            raise(E_ValueError, "Single '}' encountered in format string");
        } else {
            out.push_back(c);
        }
    }
    return out;
}

// ------------------------------------------------------------------------------------------ % formatting
std::string percent_format(const var& fmtv, const var& args, bool bytes) {
    const std::string& f = bytes ? raw_of(fmtv) : str_utf8(fmtv);
    std::vector<var> items;
    var mapping;
    if (is_tuple(args)) items = tuple_items(args);
    else if (is_dict(args) || (args.is_ref() && !is_str(args) && !args.ref()->type->builtin && args.hasattr("keys") && args.hasattr("__getitem__"))) {
        mapping = args;
        items.push_back(args);
    } else items.push_back(args);
    size_t ai = 0;
    std::string out;
    bool used_mapping = false;
    for (size_t i = 0; i < f.size(); i++) {
        char c = f[i];
        if (c != '%') { out.push_back(c); continue; }
        i++;
        if (i >= f.size()) raise(E_ValueError, "incomplete format");
        var v;
        bool have = false;
        if (f[i] == '(') {
            size_t j = f.find(')', i);
            if (mapping.is_unbound()) raise(E_TypeError, "format requires a mapping");
            v = mapping.getitem(new_str(f.substr(i + 1, j - i - 1)));
            have = true;
            used_mapping = true;
            i = j + 1;
        }
        Spec p;
        p.align = '>';
        std::string flags;
        while (i < f.size() && strchr("-+ #0", f[i])) flags.push_back(f[i++]);
        if (i < f.size() && f[i] == '*') { p.width = items.at(ai++).as_int64(); i++; }
        else if (i < f.size() && isdigit((unsigned char)f[i])) { p.width = 0; while (i < f.size() && isdigit((unsigned char)f[i])) p.width = p.width * 10 + (f[i++] - '0'); }
        if (i < f.size() && f[i] == '.') {
            i++;
            p.prec = 0;
            if (i < f.size() && f[i] == '*') { p.prec = items.at(ai++).as_int64(); i++; }
            else while (i < f.size() && isdigit((unsigned char)f[i])) p.prec = p.prec * 10 + (f[i++] - '0');
        }
        while (i < f.size() && (f[i] == 'l' || f[i] == 'h' || f[i] == 'L')) i++;
        if (i >= f.size()) raise(E_ValueError, "incomplete format");
        char t = f[i];
        if (t == '%') { out.push_back('%'); continue; }
        if (!have) {
            if (ai >= items.size()) raise(E_TypeError, "not enough arguments for format string");
            v = items[ai++];
        }
        bool left = flags.find('-') != std::string::npos, zero = flags.find('0') != std::string::npos && !left;
        if (left) p.align = '<';
        if (flags.find('+') != std::string::npos) p.sign = '+';
        else if (flags.find(' ') != std::string::npos) p.sign = ' ';
        p.alt = flags.find('#') != std::string::npos;
        std::string piece;
        switch (t) {
        case 's': case 'r': case 'a': {
            std::string s;
            if (bytes && t == 's') s = is_bytes(v) ? raw_of(v) : str_utf8(call1(v.getattr("__bytes__"), None));
            else s = t == 's' ? str_(v) : t == 'r' ? repr_(v) : str_utf8(ascii(v));
            if (p.prec >= 0) s = bytes ? s.substr(0, (size_t)p.prec) : from_u32(to_u32(s).substr(0, (size_t)std::min<int64_t>(p.prec, cp_count(s))));
            piece = pad("", s, p, '>');
            break;
        }
        case 'b':
            if (!bytes) raise(E_ValueError, "unsupported format character 'b' (0x62)");
            piece = pad("", raw_of(v), p, '>');
            break;
        case 'c':
            if (is_int(v)) { std::string o; if (bytes) o.push_back((char)v.as_int64()); else append_utf8(o, (char32_t)v.as_int64()); piece = pad("", o, p, '>'); }
            else piece = pad("", bytes ? raw_of(v) : str_utf8(v), p, '>');
            break;
        case 'd': case 'i': case 'u': case 'x': case 'X': case 'o': {
            var iv = v;
            if (v.is_float()) iv = new_int(Big::from_decimal(fixed(std::trunc(v.flt()), 0)));
            else if (!is_int(v)) {
                var m;
                if (lookup_special(v, "__index__", m) || lookup_special(v, "__int__", m)) iv = call0(m);
                else raise(E_TypeError, std::string("%") + t + " format: a real number is required, not " + tname(v));
            }
            Big b = to_big(iv);
            int base = (t == 'x' || t == 'X') ? 16 : t == 'o' ? 8 : 10;
            std::string body = int_to_base(b, base, t == 'X');
            if (p.prec >= 0) while ((int64_t)body.size() < p.prec) body = "0" + body;
            std::string pre;
            if (p.alt && base == 16) pre = t == 'X' ? "0X" : "0x";
            if (p.alt && base == 8) pre = "0o";
            std::string sign = b.sign() < 0 ? "-" : (p.sign == '+' ? "+" : p.sign == ' ' ? " " : "");
            if (zero) { p.fill = '0'; p.align = '='; }
            piece = pad(sign + pre, body, p, '>');
            break;
        }
        case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': {
            double d = v.as_double();
            Spec q = p;
            q.type = t;
            if (q.prec < 0) q.prec = 6;
            if (zero) { q.fill = '0'; q.align = '='; }
            piece = format_float(d, q);
            break;
        }
        default: {
            char b[64];
            snprintf(b, sizeof b, "unsupported format character '%c' (0x%x) at index %zu", t, (unsigned char)t, i);
            raise(E_ValueError, b);
        }
        }
        out += piece;
    }
    if (!used_mapping && ai < items.size() && mapping.is_unbound())
        raise(E_TypeError, std::string("not all arguments converted during ") + (bytes ? "bytes" : "string") + " formatting");
    return out;
}

// ------------------------------------------------------------------------------------------ the type
static const StrObj* self_s(const CallArgs& a) {
    if (a.pos.empty() || !is_str(a.pos[0])) raise(E_TypeError, "descriptor requires a 'str' object");
    return S(a.pos[0]);
}
static std::string sarg(const CallArgs& a, size_t i, const char* what) {
    if (i >= a.pos.size()) raise(E_TypeError, std::string(what) + "() takes at least " + std::to_string(i) + " argument");
    const var& v = a.pos[i];
    if (!is_str(v)) raise(E_TypeError, std::string("must be str, not ") + tname(v));
    return str_utf8(v);
}
static bool starts_or_ends(const StrObj* s, const CallArgs& a, bool ends) {
    int64_t lo, hi;
    sub_range(s, a, 2, lo, hi);
    size_t x = cp_offset(s, std::min(lo, s->n)), y = cp_offset(s, hi);
    if (lo > s->n) return false;
    auto test = [&](const var& p) {
        if (!is_str(p)) raise(E_TypeError, std::string(ends ? "endswith" : "startswith") + " first arg must be str or a tuple of str, not " + tname(p));
        const std::string& q = str_utf8(p);
        if (q.size() > y - x) return false;
        return ends ? s->s.compare(y - q.size(), q.size(), q) == 0 : s->s.compare(x, q.size(), q) == 0;
    };
    const var& p = a.pos.at(1);
    if (is_tuple(p)) {
        for (auto& q : tuple_items(p))
            if (test(q)) return true;
        return false;
    }
    return test(p);
}
var str_new_(TypeObj* cls, const CallArgs& a);
var str_join_(const std::string& sep, const var& iterable);
var str_encode_(const std::string& s, const std::string& enc, const std::string& errors);
var bytes_decode_(const std::string& raw, const std::string& enc, const std::string& errors);

var ascii(const var& x) {
    std::string r = repr_(x), o;
    for (char32_t c : to_u32(r)) {
        if (c < 0x80) o.push_back((char)c);
        else if (c < 0x100) { char b[8]; snprintf(b, sizeof b, "\\x%02x", (unsigned)c); o += b; }
        else if (c < 0x10000) { char b[8]; snprintf(b, sizeof b, "\\u%04x", (unsigned)c); o += b; }
        else { char b[12]; snprintf(b, sizeof b, "\\U%08x", (unsigned)c); o += b; }
    }
    return new_str(o);
}

void init_str() {
    TypeObj* t = T_str;
    add_static(t, "__new__", [](const CallArgs& a) { return str_new_(T(a.pos.at(0)), a); });
    add_method(t, "__len__", [](const CallArgs& a) { return var(self_s(a)->n); });
    add_method(t, "__getitem__", [](const CallArgs& a) { return str_getitem_(a.pos[0], a.pos.at(1)); });
    add_method(t, "__contains__", [](const CallArgs& a) { return var(self_s(a)->s.find(sarg(a, 1, "in")) != std::string::npos); });
    add_method(t, "__str__", [](const CallArgs& a) { return a.pos[0].ref()->type == T_str ? a.pos[0] : new_str(self_s(a)->s); });
    add_method(t, "__repr__", [](const CallArgs& a) { return new_str(str_repr_(self_s(a)->s)); });
    add_method(t, "__hash__", [](const CallArgs& a) { return var(str_hash_(self_s(a))); });
    add_method(t, "__eq__", [](const CallArgs& a) { return is_str(a.pos.at(1)) ? var(self_s(a)->s == str_utf8(a.pos[1])) : NotImplemented; });
    add_method(t, "__lt__", [](const CallArgs& a) { return is_str(a.pos.at(1)) ? var(self_s(a)->s < str_utf8(a.pos[1])) : NotImplemented; });
    add_method(t, "__add__", [](const CallArgs& a) { return is_str(a.pos.at(1)) ? new_str(self_s(a)->s + str_utf8(a.pos[1])) : NotImplemented; });
    add_method(t, "__mul__", [](const CallArgs& a) { return a.pos[0] * a.pos.at(1); });
    add_method(t, "__mod__", [](const CallArgs& a) { return new_str(percent_format(a.pos[0], a.pos.at(1), false)); });
    add_method(t, "__format__", [](const CallArgs& a) { return new_str(format_spec(new_str(self_s(a)->s), str_utf8(a.pos.at(1)))); });
    add_method(t, "__iter__", [](const CallArgs& a) { return iter(a.pos[0]); });
    add_method(t, "join", [](const CallArgs& a) { return str_join_(self_s(a)->s, a.pos.at(1)); });
    add_method(t, "split", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        var sep = arg(a, 1, "sep", None);
        int64_t maxsplit = arg(a, 2, "maxsplit", -1).as_int64();
        if (sep.is_none()) return str_list(split_ws(s->s, maxsplit, s->ascii));
        std::string p = str_utf8(sep);
        if (p.empty()) raise(E_ValueError, "empty separator");
        std::vector<std::string> out;
        size_t pos = 0;
        while (maxsplit < 0 || (int64_t)out.size() < maxsplit) {
            size_t q = s->s.find(p, pos);
            if (q == std::string::npos) break;
            out.push_back(s->s.substr(pos, q - pos));
            pos = q + p.size();
        }
        out.push_back(s->s.substr(pos));
        return str_list(out);
    });
    add_method(t, "rsplit", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        var sep = arg(a, 1, "sep", None);
        int64_t maxsplit = arg(a, 2, "maxsplit", -1).as_int64();
        if (sep.is_none()) return str_list(rsplit_ws(s->s, maxsplit));
        std::string p = str_utf8(sep);
        if (p.empty()) raise(E_ValueError, "empty separator");
        std::vector<std::string> out;
        size_t end = s->s.size();
        while (maxsplit < 0 || (int64_t)out.size() < maxsplit) {
            if (end < p.size()) break;
            size_t q = s->s.rfind(p, end - p.size());
            if (q == std::string::npos) break;
            out.push_back(s->s.substr(q + p.size(), end - q - p.size()));
            end = q;
        }
        out.push_back(s->s.substr(0, end));
        std::reverse(out.begin(), out.end());
        return str_list(out);
    });
    add_method(t, "splitlines", [](const CallArgs& a) { return str_list(splitlines_impl(self_s(a)->s, arg(a, 1, "keepends", false).truthy())); });
    add_method(t, "strip", [](const CallArgs& a) { return new_str(strip_impl(self_s(a), a.pos.size() > 1 ? a.pos[1] : var(), 3)); });
    add_method(t, "lstrip", [](const CallArgs& a) { return new_str(strip_impl(self_s(a), a.pos.size() > 1 ? a.pos[1] : var(), 1)); });
    add_method(t, "rstrip", [](const CallArgs& a) { return new_str(strip_impl(self_s(a), a.pos.size() > 1 ? a.pos[1] : var(), 2)); });
    add_method(t, "startswith", [](const CallArgs& a) { return var(starts_or_ends(self_s(a), a, false)); });
    add_method(t, "endswith", [](const CallArgs& a) { return var(starts_or_ends(self_s(a), a, true)); });
    add_method(t, "replace", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        int64_t count = arg(a, 3, "count", -1).as_int64();
        return new_str(replace_impl(s->s, sarg(a, 1, "replace"), sarg(a, 2, "replace"), count, s->ascii));
    });
    for (int which = 0; which < 4; which++) {
        static const char* names[] = {"find", "rfind", "index", "rindex"};
        add_method(t, names[which], [which](const CallArgs& a) {
            const StrObj* s = self_s(a);
            int64_t lo, hi;
            sub_range(s, a, 2, lo, hi);
            int64_t r = str_find(s, sarg(a, 1, "find"), lo, hi, which & 1);
            if (r < 0 && which >= 2) raise(E_ValueError, "substring not found");
            return var(r);
        });
    }
    add_method(t, "count", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        std::string sub = sarg(a, 1, "count");
        int64_t lo, hi;
        sub_range(s, a, 2, lo, hi);
        if (lo > s->n) return var(0);
        if (sub.empty()) return var(std::max<int64_t>(0, hi - lo) + 1);
        size_t x = cp_offset(s, lo), y = cp_offset(s, hi);
        int64_t n = 0;
        for (size_t p = s->s.find(sub, x); p != std::string::npos && p + sub.size() <= y; p = s->s.find(sub, p + sub.size())) n++;
        return var(n);
    });
    add_method(t, "lower", [](const CallArgs& a) { return new_str(case_map(self_s(a), 0)); });
    add_method(t, "casefold", [](const CallArgs& a) { return new_str(case_map(self_s(a), 0)); });
    add_method(t, "upper", [](const CallArgs& a) { return new_str(case_map(self_s(a), 1)); });
    add_method(t, "swapcase", [](const CallArgs& a) { return new_str(case_map(self_s(a), 2)); });
    add_method(t, "capitalize", [](const CallArgs& a) {
        std::u32string u = self_s(a)->u32();
        for (size_t i = 0; i < u.size(); i++) u[i] = map_case(u[i], i == 0);
        return new_str_cp(u);
    });
    add_method(t, "title", [](const CallArgs& a) {
        std::u32string u = self_s(a)->u32();
        bool prev = false;
        for (auto& c : u) {
            bool cased = is_upper_cp(c) || is_lower_cp(c) || unicode_::isalpha(c);
            c = cased ? map_case(c, !prev) : c;
            prev = cased;
        }
        return new_str_cp(u);
    });
    auto pred = [t](const char* name, bool (*f)(char32_t), bool empty_true) {
        add_method(t, name, [f, empty_true](const CallArgs& a) {
            const StrObj* s = self_s(a);
            if (s->n == 0) return var(empty_true);
            for (char32_t c : s->u32()) if (!f(c)) return var(false);
            return var(true);
        });
    };
    pred("isdigit", [](char32_t c) { return unicode_::isdigit(c); }, false);
    pred("isdecimal", [](char32_t c) { return unicode_::isdecimal(c); }, false);
    pred("isnumeric", [](char32_t c) { return unicode_::isnumeric(c); }, false);
    pred("isalpha", [](char32_t c) { return unicode_::isalpha(c); }, false);
    pred("isalnum", [](char32_t c) { return is_alnum_cp(c); }, false);
    pred("isspace", [](char32_t c) { return is_space_cp(c); }, false);
    pred("isprintable", [](char32_t c) { return is_printable_cp(c); }, true);
    pred("isascii", [](char32_t c) { return c < 0x80; }, true);
    add_method(t, "isupper", [](const CallArgs& a) {
        bool cased = false;
        for (char32_t c : self_s(a)->u32()) {
            if (is_lower_cp(c)) return var(false);
            if (is_upper_cp(c)) cased = true;
        }
        return var(cased);
    });
    add_method(t, "islower", [](const CallArgs& a) {
        bool cased = false;
        for (char32_t c : self_s(a)->u32()) {
            if (is_upper_cp(c)) return var(false);
            if (is_lower_cp(c)) cased = true;
        }
        return var(cased);
    });
    add_method(t, "isidentifier", [](const CallArgs& a) {
        const auto& u = self_s(a)->u32();
        if (u.empty()) return var(false);
        for (size_t i = 0; i < u.size(); i++) {
            char32_t c = u[i];
            bool ok = c == '_' || unicode_::isalpha(c) || (i > 0 && (unicode_::isdigit(c) || unicode_::isdecimal(c)));
            if (!ok) return var(false);
        }
        return var(true);
    });
    add_method(t, "zfill", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        int64_t w = a.pos.at(1).as_int64();
        if (s->n >= w) return new_str(s->s);
        std::string body = s->s, sign;
        if (!body.empty() && (body[0] == '-' || body[0] == '+')) { sign = body.substr(0, 1); body = body.substr(1); }
        return new_str(sign + std::string((size_t)(w - s->n), '0') + body);
    });
    for (int which = 0; which < 3; which++) {
        static const char* names[] = {"ljust", "rjust", "center"};
        add_method(t, names[which], [which](const CallArgs& a) {
            const StrObj* s = self_s(a);
            int64_t w = a.pos.at(1).as_int64();
            std::string fill = a.pos.size() > 2 ? str_utf8(a.pos[2]) : " ";
            if (cp_count(fill) != 1) raise(E_TypeError, "The fill character must be exactly one character long");
            if (s->n >= w) return new_str(s->s);
            int64_t n = w - s->n;
            auto rep = [&](int64_t k) { std::string o; for (int64_t i = 0; i < k; i++) o += fill; return o; };
            if (which == 0) return new_str(s->s + rep(n));
            if (which == 1) return new_str(rep(n) + s->s);
            int64_t left = n / 2 + (n & w & 1);
            return new_str(rep(left) + s->s + rep(n - left));
        });
    }
    add_method(t, "expandtabs", [](const CallArgs& a) {
        int64_t ts = arg(a, 1, "tabsize", 8).as_int64();
        std::string o;
        int64_t col = 0;
        for (char32_t c : self_s(a)->u32()) {
            if (c == '\t') { if (ts > 0) { int64_t n = ts - col % ts; o += std::string((size_t)n, ' '); col += n; } }
            else if (c == '\n' || c == '\r') { append_utf8(o, c); col = 0; }
            else { append_utf8(o, c); col++; }
        }
        return new_str(o);
    });
    for (int which = 0; which < 2; which++) {
        add_method(t, which ? "rpartition" : "partition", [which](const CallArgs& a) {
            const StrObj* s = self_s(a);
            std::string sep = sarg(a, 1, "partition");
            if (sep.empty()) raise(E_ValueError, "empty separator");
            size_t p = which ? s->s.rfind(sep) : s->s.find(sep);
            if (p == std::string::npos)
                return which ? new_tuple({str_lit_("", 0), str_lit_("", 0), new_str(s->s)}) : new_tuple({new_str(s->s), str_lit_("", 0), str_lit_("", 0)});
            return new_tuple({new_str(s->s.substr(0, p)), new_str(sep), new_str(s->s.substr(p + sep.size()))});
        });
    }
    add_method(t, "removeprefix", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        std::string p = sarg(a, 1, "removeprefix");
        return s->s.compare(0, p.size(), p) == 0 ? new_str(s->s.substr(p.size())) : new_str(s->s);
    });
    add_method(t, "removesuffix", [](const CallArgs& a) {
        const StrObj* s = self_s(a);
        std::string p = sarg(a, 1, "removesuffix");
        return !p.empty() && s->s.size() >= p.size() && s->s.compare(s->s.size() - p.size(), p.size(), p) == 0
                   ? new_str(s->s.substr(0, s->s.size() - p.size())) : new_str(s->s);
    });
    add_method(t, "encode", [](const CallArgs& a) {
        return str_encode_(self_s(a)->s, str_utf8(arg(a, 1, "encoding", "utf-8"_s)), str_utf8(arg(a, 2, "errors", "strict"_s)));
    });
    add_method(t, "format", [](const CallArgs& a) {
        CallArgs b;
        b.pos.assign(a.pos.begin() + 1, a.pos.end());
        b.kw = a.kw;
        size_t ai = 0;
        return new_str(str_format(self_s(a)->s, b, ai));
    });
    add_method(t, "format_map", [](const CallArgs& a) {
        CallArgs b;
        var m = a.pos.at(1);
        for (auto& k : collect(m)) b.kw.push_back({k, m.getitem(k)});
        size_t ai = 0;
        return new_str(str_format(self_s(a)->s, b, ai));
    });
    add_static(t, "maketrans", [](const CallArgs& a) {
        var d = new_dict_obj();
        auto* dd = static_cast<DictObj*>(d.ref());
        if (a.pos.size() == 1) {
            for (auto& k : collect(a.pos[0])) {
                var key = is_str(k) ? var((int64_t)to_u32(str_utf8(k))[0]) : k;
                dd->set(key, a.pos[0].getitem(k));
            }
            return d;
        }
        auto x = to_u32(str_utf8(a.pos[0])), y = to_u32(str_utf8(a.pos[1]));
        if (x.size() != y.size()) raise(E_ValueError, "the first two maketrans arguments must have equal length");
        for (size_t i = 0; i < x.size(); i++) dd->set(var((int64_t)x[i]), var((int64_t)y[i]));
        if (a.pos.size() > 2)
            for (char32_t c : to_u32(str_utf8(a.pos[2]))) dd->set(var((int64_t)c), None);
        return d;
    });
    add_method(t, "translate", [](const CallArgs& a) {
        var table = a.pos.at(1);
        std::string o;
        for (char32_t c : self_s(a)->u32()) {
            var r;
            try {
                r = table.getitem(var((int64_t)c));
            } catch (error& e) {
                if (!isinstance_(e.value, E_LookupError)) throw;
                append_utf8(o, c);
                continue;
            }
            if (r.is_none()) continue;
            if (is_int(r)) append_utf8(o, (char32_t)r.as_int64());
            else o += str_utf8(r);
        }
        return new_str(o);
    });
}

// str(x), str(b, encoding, errors)
var str_new_(TypeObj* cls, const CallArgs& a) {
    var r;
    if (a.pos.size() <= 1 && a.kw.empty()) r = a.pos.size() == 0 ? str_lit_("", 0) : new_str(str_(a.pos[0]));
    else {
        var obj = arg(a, 1, "object", str_lit_("", 0));
        var enc = kwarg(a, "encoding", a.pos.size() > 2 ? a.pos[2] : var("utf-8"_s));
        var err = kwarg(a, "errors", a.pos.size() > 3 ? a.pos[3] : var("strict"_s));
        r = bytes_decode_(raw_of(obj), str_utf8(enc), str_utf8(err));
    }
    if (cls == T_str) return r;
    auto* o = new StrObj();
    o->type = cls;
    incref(cls);
    o->s = str_utf8(r);
    o->n = S(r)->n;
    o->ascii = S(r)->ascii;
    return var(static_cast<Obj*>(o));
}
var str(const var& x) { return is_str(x) && x.ref()->type == T_str ? x : new_str(str_(x)); }

var str_join_(const std::string& sep, const var& iterable) {
    std::string out;
    size_t i = 0;
    auto add = [&](const var& x) {
        if (!is_str(x)) raise(E_TypeError, "sequence item " + std::to_string(i) + ": expected str instance, " + tname(x) + " found");
        if (i++) out += sep;
        out += static_cast<StrObj*>(x.ref())->s;
    };
    if (is_list(iterable)) for (auto& x : list_items(iterable)) add(x);
    else if (is_tuple(iterable)) for (auto& x : tuple_items(iterable)) add(x);
    else for (var x : each(iterable)) add(x);
    return new_str(std::move(out));
}

}  // namespace py
