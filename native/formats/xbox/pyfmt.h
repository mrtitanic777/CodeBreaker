// pyfmt.h -- Python's own text for values the Xbox readers put in messages and in packed fields.
#pragma once

#include <string>

#include "../core/bytes.h"
#include "../core/text.h"

namespace cbf::xbox {

// repr() of a bytes
inline std::string bytes_repr(Bytes b) {
    bool sq = b.find_byte('\'') >= 0, dq = b.find_byte('"') >= 0;
    char q = (sq && !dq) ? '"' : '\'';
    std::string r = "b";
    r += q;
    static const char *hexd = "0123456789abcdef";
    for (const uint8_t *p = b.begin(); p != b.end(); ++p) {
        uint8_t c = *p;
        if (c == uint8_t(q) || c == '\\') { r += '\\'; r += char(c); }
        else if (c == '\t') r += "\\t";
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c < 0x20 || c >= 0x7F) { r += "\\x"; r += hexd[c >> 4]; r += hexd[c & 15]; }
        else r += char(c);
    }
    r += q;
    return r;
}

// struct.pack of an unsigned value of `width` bytes, little or big endian: struct.error when it does not fit
inline void pack_uint(std::string &out, int64_t v, int width, bool big) {
    if (v < 0 || (width < 8 && v >= (int64_t(1) << (8 * width)))) raise("struct.error", "argument out of range");
    for (int k = 0; k < width; ++k) {
        int sh = big ? 8 * (width - 1 - k) : 8 * k;
        out += char((v >> sh) & 0xFF);
    }
}

// bytearray[start:stop] = data, Python's slice assignment (the array grows or shrinks to fit)
inline void slice_assign(std::string &a, int64_t start, int64_t stop, const std::string &data) {
    int64_t n = int64_t(a.size());
    auto norm = [&](int64_t i) { if (i < 0) { i += n; if (i < 0) i = 0; } return i > n ? n : i; };
    start = norm(start);
    stop = norm(stop);
    if (stop < start) stop = start;
    a.replace(size_t(start), size_t(stop - start), data);
}

}  // namespace cbf::xbox
