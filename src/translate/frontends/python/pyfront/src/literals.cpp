// literals.cpp -- Python literal decoding and repr().  See literals.hpp.
#include "literals.hpp"
#include "unicode.hpp"

#include <charconv>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace pyfront {

void append_utf8(std::string& out, uint32_t cp) {
    if (cp < 0x80) {
        out.push_back((char)cp);
    } else if (cp < 0x800) {
        out.push_back((char)(0xC0 | (cp >> 6)));
        out.push_back((char)(0x80 | (cp & 0x3F)));
    } else if (cp < 0x10000) {
        out.push_back((char)(0xE0 | (cp >> 12)));
        out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (cp & 0x3F)));
    } else {
        out.push_back((char)(0xF0 | (cp >> 18)));
        out.push_back((char)(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (cp & 0x3F)));
    }
}

namespace {

int hexval(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Decode one UTF-8 sequence (generalized: surrogates accepted); returns its length.
size_t utf8_next(std::string_view s, size_t i, uint32_t& cp) {
    unsigned char c = (unsigned char)s[i];
    if (c < 0x80) { cp = c; return 1; }
    int n = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
    if (n == 1 || i + n > s.size()) { cp = c; return 1; }
    cp = c & (0x7F >> n);
    for (int k = 1; k < n; ++k) cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
    return (size_t)n;
}

// str.isprintable() for one code point: false for the categories Cc, Cf, Cs, Co, Zl, Zp,
// Zs other than the space, and the unassigned points a source file is likely to hold.
bool is_printable(uint32_t cp) {
    if (cp < 0x20 || (cp >= 0x7F && cp <= 0xA0) || cp == 0xAD) return false;
    if (cp < 0x0378) return true;
    int known = uni::printable(cp);
    if (known >= 0) return known == 1;
    struct R { uint32_t lo, hi; };
    static const R ranges[] = {
        {0x0378, 0x0379}, {0x0380, 0x0383}, {0x038B, 0x038B}, {0x038D, 0x038D}, {0x03A2, 0x03A2},
        {0x0530, 0x0530}, {0x0557, 0x0558}, {0x058B, 0x058C}, {0x0590, 0x0590}, {0x05C8, 0x05CF},
        {0x05EB, 0x05EE}, {0x05F5, 0x0605}, {0x061C, 0x061C}, {0x06DD, 0x06DD}, {0x070E, 0x070F},
        {0x0890, 0x0891}, {0x08E2, 0x08E2}, {0x180E, 0x180E}, {0x1680, 0x1680}, {0x2000, 0x200F},
        {0x2028, 0x202F}, {0x205F, 0x206F}, {0x2072, 0x2073}, {0x208F, 0x208F}, {0x3000, 0x3000},
        {0xD800, 0xF8FF}, {0xFEFF, 0xFEFF}, {0xFFF0, 0xFFFB}, {0xFFFE, 0xFFFF}, {0x110BD, 0x110BD},
        {0x110CD, 0x110CD}, {0x13430, 0x1343F}, {0x1BCA0, 0x1BCA3}, {0x1D173, 0x1D17A},
        {0xE0000, 0xE00FF}, {0xF0000, 0x10FFFF},
    };
    for (const R& r : ranges) {
        if (cp < r.lo) return true;
        if (cp <= r.hi) return false;
    }
    return true;
}

// Length of the strict UTF-8 sequence at b[i] (a byte >= 0x80), or 0 with CPython's reason.
size_t utf8_seq(std::string_view b, size_t i, const char*& why) {
    unsigned char c = (unsigned char)b[i];
    size_t n;
    unsigned char lo = 0x80, hi = 0xBF;
    if (c >= 0xC2 && c <= 0xDF) n = 2;
    else if (c >= 0xE0 && c <= 0xEF) { n = 3; if (c == 0xE0) lo = 0xA0; if (c == 0xED) hi = 0x9F; }
    else if (c >= 0xF0 && c <= 0xF4) { n = 4; if (c == 0xF0) lo = 0x90; if (c == 0xF4) hi = 0x8F; }
    else { why = "invalid start byte"; return 0; }
    for (size_t k = 1; k < n; ++k) {
        if (i + k >= b.size()) { why = "unexpected end of data"; return 0; }
        unsigned char d = (unsigned char)b[i + k];
        if (d < (k == 1 ? lo : 0x80) || d > (k == 1 ? hi : 0xBF)) { why = "invalid continuation byte"; return 0; }
    }
    return n;
}

void append_hex(std::string& out, const char* prefix, uint32_t v, int digits) {
    static const char* hx = "0123456789abcdef";
    out += prefix;
    for (int k = digits - 1; k >= 0; --k) out.push_back(hx[(v >> (4 * k)) & 0xF]);
}

}  // namespace

bool decode_string(std::string_view b, bool raw, bool bytes, bool fstring, std::string& out,
                   std::string& err, size_t& err_off) {
    out.clear();
    out.reserve(b.size());
    size_t i = 0;
    const size_t n = b.size();
    while (i < n) {
        char c = b[i];
        if (fstring && (c == '{' || c == '}') && i + 1 < n && b[i + 1] == c) {
            out.push_back(c);
            i += 2;
            continue;
        }
        if (bytes && (unsigned char)c >= 0x80) {
            err = "bytes can only contain ASCII literal characters";
            err_off = i;
            return false;
        }
        if (!bytes && (unsigned char)c >= 0x80) {
            const char* why = "";
            size_t len = utf8_seq(b, i, why);
            if (!len) {
                char hx[8];
                snprintf(hx, sizeof hx, "0x%02x", (unsigned char)c);
                err = std::string("(unicode error) 'utf-8' codec can't decode byte ") + hx + " in position " + std::to_string(i) + ": " + why;
                err_off = i;
                return false;
            }
            out.append(b.substr(i, len));
            i += len;
            continue;
        }
        if (c != '\\' || raw) {
            out.push_back(c);
            ++i;
            continue;
        }
        if (i + 1 >= n) {  // a lone trailing backslash (only possible in f-string pieces)
            out.push_back(c);
            ++i;
            continue;
        }
        char e = b[i + 1];
        size_t at = i;
        i += 2;
        switch (e) {
        case '\n': break;
        case '\\': out.push_back('\\'); break;
        case '\'': out.push_back('\''); break;
        case '"': out.push_back('"'); break;
        case 'a': out.push_back('\a'); break;
        case 'b': out.push_back('\b'); break;
        case 'f': out.push_back('\f'); break;
        case 'n': out.push_back('\n'); break;
        case 'r': out.push_back('\r'); break;
        case 't': out.push_back('\t'); break;
        case 'v': out.push_back('\v'); break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
            uint32_t v = (uint32_t)(e - '0');
            for (int k = 0; k < 2 && i < n && b[i] >= '0' && b[i] <= '7'; ++k, ++i) v = v * 8 + (uint32_t)(b[i] - '0');
            if (bytes) out.push_back((char)(v & 0xFF));
            else append_utf8(out, v);
            break;
        }
        case 'x': {
            if (i + 2 > n || hexval(b[i]) < 0 || hexval(b[i + 1]) < 0) {
                err = bytes ? "(value error) invalid \\x escape at position " + std::to_string(at)
                            : "(unicode error) 'unicodeescape' codec can't decode bytes in position " + std::to_string(at) + "-" + std::to_string(i - 1) + ": truncated \\xXX escape";
                err_off = at;
                return false;
            }
            uint32_t v = (uint32_t)(hexval(b[i]) * 16 + hexval(b[i + 1]));
            i += 2;
            if (bytes) out.push_back((char)v);
            else append_utf8(out, v);
            break;
        }
        case 'u': case 'U': {
            if (bytes) { out.push_back('\\'); out.push_back(e); break; }
            int len = e == 'u' ? 4 : 8;
            uint32_t v = 0;
            int k = 0;
            for (; k < len && i < n && hexval(b[i]) >= 0; ++k, ++i) v = v * 16 + (uint32_t)hexval(b[i]);
            if (k < len) {
                err = std::string("(unicode error) 'unicodeescape' codec can't decode bytes in position ") + std::to_string(at) + "-" + std::to_string(i - 1) + ": truncated \\" + (e == 'u' ? "uXXXX" : "UXXXXXXXX") + " escape";
                err_off = at;
                return false;
            }
            if (v > 0x10FFFF) {
                err = "(unicode error) 'unicodeescape' codec can't decode bytes in position " + std::to_string(at) + "-" + std::to_string(i - 1) + ": illegal Unicode character";
                err_off = at;
                return false;
            }
            append_utf8(out, v);
            break;
        }
        case 'N': {
            if (bytes) { out.push_back('\\'); out.push_back('N'); break; }
            size_t close = i < n && b[i] == '{' ? b.find('}', i) : std::string_view::npos;
            if (close == std::string_view::npos || close == i + 1) {
                err = "(unicode error) 'unicodeescape' codec can't decode bytes in position " + std::to_string(at) + "-" + std::to_string(at + 1) + ": malformed \\N character escape";
                err_off = at;
                return false;
            }
            uint32_t cp = 0;
            if (!uni::lookup_name(b.substr(i + 1, close - i - 1), cp)) {
                err = "(unicode error) 'unicodeescape' codec can't decode bytes in position " + std::to_string(at) + "-" + std::to_string(close) +
                      (uni::available() ? ": unknown Unicode character name" : ": no Unicode name database on this system");
                err_off = at;
                return false;
            }
            append_utf8(out, cp);
            i = close + 1;
            break;
        }
        default:
            // An unknown escape keeps its backslash (CPython warns, it does not fail).
            out.push_back('\\');
            i = at + 1;
            break;
        }
    }
    return true;
}

ConstValue parse_number(std::string_view tok) {
    ConstValue v;
    std::string t;
    t.reserve(tok.size());
    for (char c : tok)
        if (c != '_') t.push_back(c);
    char last = (char)(t.back() | 0x20);
    bool is_hex = t.size() > 1 && t[0] == '0' && (t[1] | 0x20) == 'x';
    if (last == 'j') {
        v.kind = ConstValue::Kind::Complex;
        t.pop_back();
        v.f = std::strtod(t.c_str(), nullptr);
        return v;
    }
    if (!is_hex && (t.find('.') != std::string::npos || t.find_first_of("eE") != std::string::npos)) {
        v.kind = ConstValue::Kind::Float;
        double d = 0;
        auto r = std::from_chars(t.data(), t.data() + t.size(), d);
        if (r.ec != std::errc()) d = std::strtod(t.c_str(), nullptr);
        v.f = d;
        return v;
    }
    v.kind = ConstValue::Kind::Int;
    int base = 10;
    size_t s = 0;
    if (t.size() > 1 && t[0] == '0') {
        char k = (char)(t[1] | 0x20);
        if (k == 'x') { base = 16; s = 2; }
        else if (k == 'o') { base = 8; s = 2; }
        else if (k == 'b') { base = 2; s = 2; }
    }
    if (base == 10) {
        while (s + 1 < t.size() && t[s] == '0') ++s;
        v.text = t.substr(s);
    } else {
        // Big-number conversion to decimal: limbs of 10^9, least significant first.
        std::vector<uint32_t> limbs{0};
        for (size_t i = s; i < t.size(); ++i) {
            uint64_t carry = (uint64_t)hexval(t[i]);
            for (uint32_t& l : limbs) {
                uint64_t x = (uint64_t)l * (uint64_t)base + carry;
                l = (uint32_t)(x % 1000000000u);
                carry = x / 1000000000u;
            }
            while (carry) { limbs.push_back((uint32_t)(carry % 1000000000u)); carry /= 1000000000u; }
        }
        std::string out = std::to_string(limbs.back());
        for (size_t i = limbs.size() - 1; i-- > 0;) {
            std::string part = std::to_string(limbs[i]);
            out.append(9 - part.size(), '0');
            out += part;
        }
        v.text = out;
    }
    static const std::string max64 = "9223372036854775807";
    if (v.text.size() < max64.size() || (v.text.size() == max64.size() && v.text <= max64)) {
        v.fits_i64 = true;
        v.i64 = std::strtoll(v.text.c_str(), nullptr, 10);
    }
    return v;
}

std::string repr_str(std::string_view s) {
    bool has_sq = s.find('\'') != std::string_view::npos;
    bool has_dq = s.find('"') != std::string_view::npos;
    char q = (has_sq && !has_dq) ? '"' : '\'';
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back(q);
    for (size_t i = 0; i < s.size();) {
        uint32_t cp;
        size_t len = utf8_next(s, i, cp);
        if (cp == (uint32_t)q || cp == '\\') { out.push_back('\\'); out.push_back((char)cp); }
        else if (cp == '\t') out += "\\t";
        else if (cp == '\n') out += "\\n";
        else if (cp == '\r') out += "\\r";
        else if (cp < 0x20 || cp == 0x7F) append_hex(out, "\\x", cp, 2);
        else if (cp < 0x7F) out.push_back((char)cp);
        else if (is_printable(cp)) out.append(s.substr(i, len));
        else if (cp <= 0xFF) append_hex(out, "\\x", cp, 2);
        else if (cp <= 0xFFFF) append_hex(out, "\\u", cp, 4);
        else append_hex(out, "\\U", cp, 8);
        i += len;
    }
    out.push_back(q);
    return out;
}

std::string repr_bytes(std::string_view s) {
    bool has_sq = s.find('\'') != std::string_view::npos;
    bool has_dq = s.find('"') != std::string_view::npos;
    char q = (has_sq && !has_dq) ? '"' : '\'';
    std::string out = "b";
    out.push_back(q);
    for (char ch : s) {
        unsigned char c = (unsigned char)ch;
        if (c == (unsigned char)q || c == '\\') { out.push_back('\\'); out.push_back((char)c); }
        else if (c == '\t') out += "\\t";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c < 0x20 || c >= 0x7F) append_hex(out, "\\x", c, 2);
        else out.push_back((char)c);
    }
    out.push_back(q);
    return out;
}

std::string repr_float(double d, bool complex_part) {
    if (std::isinf(d)) return d > 0 ? "inf" : "-inf";
    if (std::isnan(d)) return "nan";
    char buf[64];
    auto r = std::to_chars(buf, buf + sizeof buf, d, std::chars_format::scientific);
    std::string sci(buf, r.ptr);
    bool neg = false;
    if (!sci.empty() && sci[0] == '-') { neg = true; sci.erase(0, 1); }
    size_t epos = sci.find('e');
    std::string mant = sci.substr(0, epos);
    int exp10 = std::atoi(sci.c_str() + epos + 1);
    std::string digits;
    for (char c : mant)
        if (c != '.') digits.push_back(c);
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    int decpt = exp10 + 1;
    std::string out = neg ? "-" : "";
    if (decpt <= -4 || decpt > 16) {
        out.push_back(digits[0]);
        if (digits.size() > 1) { out.push_back('.'); out.append(digits, 1, std::string::npos); }
        int e = decpt - 1;
        out.push_back('e');
        out.push_back(e < 0 ? '-' : '+');
        int ae = e < 0 ? -e : e;
        if (ae < 10) out.push_back('0');
        out += std::to_string(ae);
    } else if (decpt <= 0) {
        out += "0.";
        out.append((size_t)-decpt, '0');
        out += digits;
    } else if ((size_t)decpt >= digits.size()) {
        out += digits;
        out.append((size_t)decpt - digits.size(), '0');
        if (!complex_part) out += ".0";
    } else {
        out.append(digits, 0, (size_t)decpt);
        out.push_back('.');
        out.append(digits, (size_t)decpt, std::string::npos);
    }
    return out;
}

std::string repr_const(const ConstValue& v) {
    switch (v.kind) {
    case ConstValue::Kind::None: return "None";
    case ConstValue::Kind::True: return "True";
    case ConstValue::Kind::False: return "False";
    case ConstValue::Kind::Ellipsis: return "Ellipsis";
    case ConstValue::Kind::Int: return v.text;
    case ConstValue::Kind::Float: return repr_float(v.f);
    case ConstValue::Kind::Complex: return repr_float(v.f, true) + "j";
    case ConstValue::Kind::Str: return repr_str(v.text);
    case ConstValue::Kind::Bytes: return repr_bytes(v.text);
    }
    return "?";
}

}  // namespace pyfront
