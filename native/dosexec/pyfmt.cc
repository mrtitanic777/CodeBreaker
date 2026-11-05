// pyfmt.cc -- the text Python would have made: the messages a run reports (they reach callers,
// e.g. port/verify.py keeps a run's error text as its console), and str operations on latin-1 text.
#include "dosexec.h"
#include "cbdos.h"

#include <cstdarg>
#include <cstdio>

namespace cbd {

PyErr runtime_error(const std::string &m) { return PyErr{CBD_RUNTIME, 0, m, L""}; }
PyErr uc_error(int e) { return PyErr{CBD_UC, e, UC.strerror ? UC.strerror(uc_err(e)) : "unicorn error", L""}; }

std::string fmt(const char *f, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, f);
    int n = vsnprintf(buf, sizeof buf, f, ap);
    va_end(ap);
    if (n < 0)
        return "";
    if (size_t(n) < sizeof buf)
        return std::string(buf, size_t(n));
    std::string s(size_t(n) + 1, '\0');
    va_start(ap, f);
    vsnprintf(&s[0], s.size(), f, ap);
    va_end(ap);
    s.resize(size_t(n));
    return s;
}

std::string py_hex(int64_t v, int width, bool upper) {
    bool neg = v < 0;
    uint64_t u = neg ? uint64_t(-(v + 1)) + 1 : uint64_t(v);
    char digits[32];
    snprintf(digits, sizeof digits, upper ? "%llX" : "%llx", (unsigned long long)u);
    std::string d(digits);
    int w = width - (neg ? 1 : 0);
    while (int(d.size()) < w)
        d = "0" + d;
    return neg ? "-" + d : d;
}

std::string bytes_repr(const uint8_t *b, size_t n) {
    bool sq = false, dq = false;
    for (size_t i = 0; i < n; i++) {
        if (b[i] == '\'') sq = true;
        if (b[i] == '"') dq = true;
    }
    char q = (sq && !dq) ? '"' : '\'';
    std::string s = "b";
    s += q;
    for (size_t i = 0; i < n; i++) {
        uint8_t c = b[i];
        if (c == q || c == '\\') { s += '\\'; s += char(c); }
        else if (c == '\t') s += "\\t";
        else if (c == '\n') s += "\\n";
        else if (c == '\r') s += "\\r";
        else if (c < 0x20 || c >= 0x7F) s += fmt("\\x%02x", c);
        else s += char(c);
    }
    s += q;
    return s;
}

std::string hex_str(const uint8_t *b, size_t n) {
    std::string s;
    for (size_t i = 0; i < n; i++)
        s += fmt("%02x", b[i]);
    return s;
}

std::wstring py_upper(const std::wstring &s) {
    std::wstring o;
    o.reserve(s.size());
    for (wchar_t c : s) {
        if (c >= L'a' && c <= L'z') o += wchar_t(c - 32);
        else if (c >= 0xE0 && c <= 0xFE && c != 0xF7) o += wchar_t(c - 32);
        else if (c == 0xFF) o += wchar_t(0x178);
        else if (c == 0xB5) o += wchar_t(0x39C);
        else if (c == 0xDF) o += L"SS";
        else o += c;
    }
    return o;
}

bool py_isspace(wchar_t c) {
    return (c >= 0x09 && c <= 0x0D) || (c >= 0x1C && c <= 0x20) || c == 0x85 || c == 0xA0;
}

bool py_isalpha(wchar_t c) {
    return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || c == 0xAA || c == 0xB5 || c == 0xBA ||
           (c >= 0xC0 && c <= 0xD6) || (c >= 0xD8 && c <= 0xF6) || (c >= 0xF8 && c <= 0xFF);
}

std::wstring rstrip_ws(const std::wstring &s) {
    size_t e = s.size();
    while (e > 0 && py_isspace(s[e - 1]))
        e--;
    return s.substr(0, e);
}

std::wstring latin1(const uint8_t *b, size_t n) {
    std::wstring s;
    s.reserve(n);
    for (size_t i = 0; i < n; i++)
        s += wchar_t(b[i]);
    return s;
}

std::wstring latin1(const Bytes &b) { return latin1(b.data(), b.size()); }

bool to_latin1(const std::wstring &s, Bytes &out) {
    out.clear();
    for (wchar_t c : s) {
        if (c > 0xFF)
            return false;
        out.push_back(uint8_t(c));
    }
    return true;
}

std::string utf8(const std::wstring &s) {
    std::string o;
    for (size_t i = 0; i < s.size(); i++) {
        uint32_t c = s[i];
        if (c >= 0xD800 && c < 0xDC00 && i + 1 < s.size()) {
            c = 0x10000 + ((c - 0xD800) << 10) + (uint32_t(s[i + 1]) - 0xDC00);
            i++;
        }
        if (c < 0x80) o += char(c);
        else if (c < 0x800) { o += char(0xC0 | (c >> 6)); o += char(0x80 | (c & 0x3F)); }
        else if (c < 0x10000) { o += char(0xE0 | (c >> 12)); o += char(0x80 | ((c >> 6) & 0x3F)); o += char(0x80 | (c & 0x3F)); }
        else { o += char(0xF0 | (c >> 18)); o += char(0x80 | ((c >> 12) & 0x3F)); o += char(0x80 | ((c >> 6) & 0x3F)); o += char(0x80 | (c & 0x3F)); }
    }
    return o;
}

Bytes py_slice(const Bytes &b, int64_t start, int64_t stop) {
    int64_t n = int64_t(b.size());
    if (start < 0) start = std::max<int64_t>(0, start + n);
    if (stop < 0) stop = std::max<int64_t>(0, stop + n);
    start = std::min(start, n);
    stop = std::min(stop, n);
    if (stop <= start)
        return Bytes();
    return Bytes(b.begin() + start, b.begin() + stop);
}

}  // namespace cbd
