#include "pystr.hpp"

#include <charconv>
#include <cmath>
#include <cstring>

#include "value.hpp"

namespace py {

bool is_space_cp(uint32_t c) {
    return c == ' ' || (c >= 0x09 && c <= 0x0d) || (c >= 0x1c && c <= 0x1f) || c == 0x85 || c == 0xa0 ||
           c == 0x1680 || (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f ||
           c == 0x205f || c == 0x3000;
}

uint32_t decode_cp(const std::string& s, size_t& pos) {
    unsigned char c = (unsigned char)s[pos];
    if (c < 0x80) {
        pos++;
        return c;
    }
    int n = c >= 0xf0 ? 3 : c >= 0xe0 ? 2 : c >= 0xc0 ? 1 : 0;
    uint32_t cp = n == 3 ? (c & 7) : n == 2 ? (c & 15) : n == 1 ? (c & 31) : c;
    pos++;
    for (int k = 0; k < n && pos < s.size() && ((unsigned char)s[pos] & 0xc0) == 0x80; k++)
        cp = (cp << 6) | ((unsigned char)s[pos++] & 0x3f);
    return cp;
}

size_t prev_cp(const std::string& s, size_t pos) {
    if (pos == 0) return 0;
    pos--;
    while (pos > 0 && ((unsigned char)s[pos] & 0xc0) == 0x80) pos--;
    return pos;
}

static bool space_at(const std::string& s, size_t pos, size_t& next) {
    size_t p = pos;
    uint32_t cp = decode_cp(s, p);
    next = p;
    return is_space_cp(cp);
}

std::vector<std::string> split_ws(const std::string& s, int maxsplit) {
    std::vector<std::string> out;
    size_t pos = 0, n = s.size();
    while (pos < n) {
        size_t nx;
        while (pos < n && space_at(s, pos, nx)) pos = nx;
        if (pos >= n) break;
        if (maxsplit >= 0 && (int)out.size() == maxsplit) {
            // the rest, its trailing whitespace stripped
            out.push_back(rstrip(s.substr(pos)));
            return out;
        }
        size_t start = pos;
        while (pos < n && !space_at(s, pos, nx)) pos = nx;
        out.push_back(s.substr(start, pos - start));
    }
    return out;
}

std::vector<std::string> split(const std::string& s, const std::string& sep, int maxsplit) {
    std::vector<std::string> out;
    size_t pos = 0;
    while (true) {
        if (maxsplit >= 0 && (int)out.size() == maxsplit) break;
        size_t k = s.find(sep, pos);
        if (k == std::string::npos) break;
        out.push_back(s.substr(pos, k - pos));
        pos = k + sep.size();
    }
    out.push_back(s.substr(pos));
    return out;
}

std::vector<std::string> rsplit(const std::string& s, const std::string& sep, int maxsplit) {
    std::vector<std::string> out;
    size_t end = s.size();
    while ((int)out.size() < maxsplit) {
        if (end < sep.size()) break;
        size_t k = s.rfind(sep, end - sep.size());
        if (k == std::string::npos) break;
        out.insert(out.begin(), s.substr(k + sep.size(), end - k - sep.size()));
        end = k;
    }
    out.insert(out.begin(), s.substr(0, end));
    return out;
}

std::vector<std::string> splitlines(const std::string& s) {
    std::vector<std::string> out;
    size_t pos = 0, start = 0, n = s.size();
    while (pos < n) {
        size_t p = pos;
        uint32_t cp = decode_cp(s, p);
        bool br = cp == '\n' || cp == '\r' || cp == 0x0b || cp == 0x0c || cp == 0x1c || cp == 0x1d || cp == 0x1e ||
                  cp == 0x85 || cp == 0x2028 || cp == 0x2029;
        if (br) {
            out.push_back(s.substr(start, pos - start));
            if (cp == '\r' && p < n && s[p] == '\n') p++;
            start = p;
        }
        pos = p;
    }
    if (start < n) out.push_back(s.substr(start));
    return out;
}

std::string lstrip(const std::string& s) {
    size_t pos = 0, nx;
    while (pos < s.size() && space_at(s, pos, nx)) pos = nx;
    return s.substr(pos);
}
std::string rstrip(const std::string& s) {
    size_t end = s.size();
    while (end > 0) {
        size_t p = prev_cp(s, end), q = p;
        if (!is_space_cp(decode_cp(s, q))) break;
        end = p;
    }
    return s.substr(0, end);
}
std::string strip(const std::string& s) { return lstrip(rstrip(s)); }
std::string lstrip_chars(const std::string& s, const std::string& chars) {
    size_t pos = 0;
    while (pos < s.size() && chars.find(s[pos]) != std::string::npos) pos++;
    return s.substr(pos);
}
std::string rstrip_chars(const std::string& s, const std::string& chars) {
    size_t end = s.size();
    while (end > 0 && chars.find(s[end - 1]) != std::string::npos) end--;
    return s.substr(0, end);
}
std::string strip_chars(const std::string& s, const std::string& chars) { return lstrip_chars(rstrip_chars(s, chars), chars); }

bool startswith(const std::string& s, const std::string& p) { return s.size() >= p.size() && s.compare(0, p.size(), p) == 0; }
bool endswith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}

std::string replace(const std::string& s, const std::string& old, const std::string& rep, int count) {
    if (count == 0) return s;
    std::string out;
    if (old.empty()) {
        // (Python puts `rep` between every character, and at both ends)
        int made = 0;
        size_t pos = 0;
        while (pos <= s.size()) {
            if (count < 0 || made < count) {
                out += rep;
                made++;
            }
            if (pos == s.size()) break;
            size_t p = pos;
            decode_cp(s, p);
            out.append(s, pos, p - pos);
            pos = p;
        }
        return out;
    }
    size_t pos = 0;
    int made = 0;
    while (count < 0 || made < count) {
        size_t k = s.find(old, pos);
        if (k == std::string::npos) break;
        out.append(s, pos, k - pos);
        out += rep;
        pos = k + old.size();
        made++;
    }
    out.append(s, pos, std::string::npos);
    return out;
}

std::string join(const std::string& sep, const std::vector<std::string>& parts) {
    std::string out;
    for (size_t n = 0; n < parts.size(); n++) {
        if (n) out += sep;
        out += parts[n];
    }
    return out;
}

size_t count(const std::string& s, const std::string& sub) {
    if (sub.empty()) return s.size() + 1;
    size_t n = 0, pos = 0;
    while ((pos = s.find(sub, pos)) != std::string::npos) {
        n++;
        pos += sub.size();
    }
    return n;
}

std::string upper(const std::string& s) {
    std::string o = s;
    for (auto& c : o)
        if (c >= 'a' && c <= 'z') c = (char)(c - 32);
    return o;
}
std::string lower(const std::string& s) {
    std::string o = s;
    for (auto& c : o)
        if (c >= 'A' && c <= 'Z') c = (char)(c + 32);
    return o;
}

Parts partition(const std::string& s, const std::string& sep) {
    size_t k = s.find(sep);
    if (k == std::string::npos) return {s, "", ""};
    return {s.substr(0, k), sep, s.substr(k + sep.size())};
}
Parts rpartition(const std::string& s, const std::string& sep) {
    size_t k = s.rfind(sep);
    if (k == std::string::npos) return {"", "", s};
    return {s.substr(0, k), sep, s.substr(k + sep.size())};
}

std::string slice(const std::string& s, int64_t a, int64_t b) {
    int64_t n = (int64_t)s.size();
    if (a < 0) a += n;
    if (b < 0) b += n;
    if (a < 0) a = 0;
    if (b > n) b = n;
    if (a >= b) return "";
    return s.substr((size_t)a, (size_t)(b - a));
}

std::string spaces(int64_t n) { return n > 0 ? std::string((size_t)n, ' ') : std::string(); }

int64_t int_parse(const std::string& text, int base) {
    std::string s = strip(text);
    auto bad = [&]() -> int64_t {
        raise("ValueError", "invalid literal for int() with base " + std::to_string(base) + ": " + str_repr(text));
    };
    size_t p = 0;
    bool neg = false;
    if (p < s.size() && (s[p] == '+' || s[p] == '-')) neg = s[p++] == '-';
    int b = base;
    if ((base == 0 || base == 16) && p + 1 < s.size() && s[p] == '0' && (s[p + 1] == 'x' || s[p + 1] == 'X')) {
        b = 16;
        p += 2;
        if (p < s.size() && s[p] == '_') p++;
    } else if (base == 0 && p + 1 < s.size() && s[p] == '0' && (s[p + 1] == 'o' || s[p + 1] == 'O')) {
        b = 8;
        p += 2;
        if (p < s.size() && s[p] == '_') p++;
    } else if (base == 0 && p + 1 < s.size() && s[p] == '0' && (s[p + 1] == 'b' || s[p + 1] == 'B')) {
        b = 2;
        p += 2;
        if (p < s.size() && s[p] == '_') p++;
    } else if (base == 0) {
        b = 10;
        // (a decimal of base 0 may not start with 0 unless it is all zeros)
        if (p < s.size() && s[p] == '0') {
            for (size_t q = p; q < s.size(); q++)
                if (s[q] != '0' && s[q] != '_') bad();
        }
    }
    if (p >= s.size()) bad();
    uint64_t v = 0;
    bool last_us = true;
    for (; p < s.size(); p++) {
        char c = s[p];
        if (c == '_') {
            if (last_us) bad();
            last_us = true;
            continue;
        }
        int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'z' ? c - 'a' + 10 : c >= 'A' && c <= 'Z' ? c - 'A' + 10 : 99;
        if (d >= b) bad();
        v = v * (uint64_t)b + (uint64_t)d;
        last_us = false;
    }
    if (last_us) bad();
    return neg ? -(int64_t)v : (int64_t)v;
}

std::string hex(uint64_t v) {
    char buf[32];
    auto r = std::to_chars(buf, buf + 32, v, 16);
    return std::string(buf, r.ptr);
}
std::string HEX(uint64_t v) { return upper(hex(v)); }
std::string hexpad(uint64_t v, int width, bool up) {
    std::string h = up ? HEX(v) : hex(v);
    if ((int)h.size() < width) h = std::string((size_t)(width - (int)h.size()), '0') + h;
    return h;
}
std::string dec(int64_t v) { return std::to_string(v); }

std::string float_repr(double f) {
    if (std::isnan(f)) return "nan";
    if (std::isinf(f)) return f < 0 ? "-inf" : "inf";
    if (f == 0) return std::signbit(f) ? "-0.0" : "0.0";
    char buf[64];
    auto r = std::to_chars(buf, buf + 64, f, std::chars_format::scientific);
    std::string sci(buf, r.ptr);                          // [-]d[.ddd]e[+-]xx
    bool neg = sci[0] == '-';
    if (neg) sci = sci.substr(1);
    size_t e = sci.find('e');
    std::string mant = sci.substr(0, e);
    int exp = std::stoi(sci.substr(e + 1));
    std::string digits;
    for (char c : mant)
        if (c != '.') digits += c;
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    std::string out;
    if (exp >= -5 && exp < 16) {                          // (Python: repr is fixed for -4 <= exponent < 16)
        if (exp < -4) goto sci_form;
        int point = exp + 1;                              // digits before the point
        if (point <= 0) {
            out = "0." + std::string((size_t)(-point), '0') + digits;
        } else if ((int)digits.size() <= point) {
            out = digits + std::string((size_t)(point - (int)digits.size()), '0') + ".0";
        } else {
            out = digits.substr(0, (size_t)point) + "." + digits.substr((size_t)point);
        }
        return (neg ? "-" : "") + out;
    }
sci_form:
    out = digits.substr(0, 1);
    if (digits.size() > 1) out += "." + digits.substr(1);
    {
        char eb[16];
        std::snprintf(eb, sizeof eb, "e%c%02d", exp < 0 ? '-' : '+', exp < 0 ? -exp : exp);
        out += eb;
    }
    return (neg ? "-" : "") + out;
}

std::string str_repr(const std::string& s) {
    bool sq = s.find('\'') != std::string::npos, dq = s.find('"') != std::string::npos;
    char q = sq && !dq ? '"' : '\'';
    std::string out(1, q);
    size_t pos = 0;
    while (pos < s.size()) {
        uint32_t c = decode_cp(s, pos);
        if (c == (uint32_t)q || c == '\\') {
            out += '\\';
            out += (char)c;
        } else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c < 0x20 || c == 0x7f) {
            char b[8];
            std::snprintf(b, sizeof b, "\\x%02x", c);
            out += b;
        } else if (c < 0x80) out += (char)c;
        else if (c < 0xa0 || c == 0xad) {
            char b[8];
            std::snprintf(b, sizeof b, "\\x%02x", c);
            out += b;
        } else {
            // (printable: as it is; the UTF-8 of it)
            if (c < 0x800) {
                out += (char)(0xc0 | (c >> 6));
                out += (char)(0x80 | (c & 0x3f));
            } else if (c < 0x10000) {
                out += (char)(0xe0 | (c >> 12));
                out += (char)(0x80 | ((c >> 6) & 0x3f));
                out += (char)(0x80 | (c & 0x3f));
            } else {
                out += (char)(0xf0 | (c >> 18));
                out += (char)(0x80 | ((c >> 12) & 0x3f));
                out += (char)(0x80 | ((c >> 6) & 0x3f));
                out += (char)(0x80 | (c & 0x3f));
            }
        }
    }
    out += q;
    return out;
}

}  // namespace py
