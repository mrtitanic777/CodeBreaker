// json.h -- the few JSON writers the answers need (ASCII out, \u escapes for anything else).
#pragma once
#include <cstdint>
#include <cstdio>
#include <string>

namespace cba {

inline void put_int(std::string &o, int64_t v) {
    char b[32];
    snprintf(b, sizeof b, "%lld", (long long)v);
    o += b;
}

inline void put_str(std::string &o, const std::string &s) {
    o += '"';
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') o += '\\', o += (char)c;
        else if (c < 0x20 || c >= 0x7F) {
            char b[8];
            snprintf(b, sizeof b, "\\u%04x", c);
            o += b;
        } else o += (char)c;
    }
    o += '"';
}

// a UTF-8 string (lone surrogates allowed, as Python's surrogatepass writes them) as a JSON string
inline void put_ustr(std::string &o, const std::string &s) {
    o += '"';
    size_t i = 0, n = s.size();
    auto esc = [&](unsigned u) {
        char b[8];
        snprintf(b, sizeof b, "\\u%04x", u);
        o += b;
    };
    while (i < n) {
        unsigned char c = (unsigned char)s[i];
        unsigned cp;
        int len;
        if (c < 0x80) cp = c, len = 1;
        else if ((c & 0xE0) == 0xC0) cp = c & 0x1F, len = 2;
        else if ((c & 0xF0) == 0xE0) cp = c & 0x0F, len = 3;
        else cp = c & 0x07, len = 4;
        for (int k = 1; k < len && i + k < n; k++) cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
        i += len;
        if (cp == '"' || cp == '\\') o += '\\', o += (char)cp;
        else if (cp >= 0x20 && cp < 0x7F) o += (char)cp;
        else if (cp < 0x10000) esc(cp);
        else {
            cp -= 0x10000;
            esc(0xD800 + (cp >> 10));
            esc(0xDC00 + (cp & 0x3FF));
        }
    }
    o += '"';
}

}  // namespace cba
