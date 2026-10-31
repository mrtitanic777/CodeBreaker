// text.cc -- Python's decoders, written out (text.h).
#include "text.h"

#include <cstdio>

namespace cbf {

namespace {
void put_utf8(std::string &out, uint32_t cp) {
    if (cp < 0x80) {
        out += static_cast<char>(cp);
    } else if (cp < 0x800) {
        out += static_cast<char>(0xC0 | (cp >> 6));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        out += static_cast<char>(0xE0 | (cp >> 12));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (cp >> 18));
        out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (cp & 0x3F));
    }
}
constexpr uint32_t kReplacement = 0xFFFD;
}  // namespace

std::string latin1(Bytes b) {
    std::string out;
    out.reserve(static_cast<size_t>(b.size()));
    for (uint8_t c : b) put_utf8(out, c);
    return out;
}

std::string utf16le(Bytes b) {
    std::string out;
    const int64_t n = b.size();
    int64_t i = 0;
    while (i < n) {
        if (n - i < 2) {                                  // a cut-off last byte
            put_utf8(out, kReplacement);
            break;
        }
        uint32_t u = b.data()[i] | (b.data()[i + 1] << 8);
        if (u < 0xD800 || u > 0xDFFF) {
            put_utf8(out, u);
            i += 2;
        } else if (u >= 0xDC00) {                         // a low surrogate alone
            put_utf8(out, kReplacement);
            i += 2;
        } else if (n - i < 4) {                           // a high surrogate cut off: the rest is one error
            put_utf8(out, kReplacement);
            break;
        } else {
            uint32_t lo = b.data()[i + 2] | (b.data()[i + 3] << 8);
            if (lo >= 0xDC00 && lo <= 0xDFFF) {
                put_utf8(out, 0x10000 + ((u - 0xD800) << 10) + (lo - 0xDC00));
                i += 4;
            } else {                                      // a high surrogate alone
                put_utf8(out, kReplacement);
                i += 2;
            }
        }
    }
    return out;
}

std::string ascii(Bytes b) {
    for (int64_t i = 0; i < b.size(); ++i)
        if (b.data()[i] >= 0x80) {
            char msg[160];
            std::snprintf(msg, sizeof msg, "'ascii' codec can't decode byte 0x%02x in position %lld: ordinal not in range(128)",
                          b.data()[i], static_cast<long long>(i));
            raise("UnicodeDecodeError", msg);
        }
    return b.str();
}

std::string ascii_replace(Bytes b) {
    std::string out;
    for (uint8_t c : b) put_utf8(out, c < 0x80 ? c : kReplacement);
    return out;
}

namespace {
bool alpha(uint8_t c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool digit(uint8_t c) { return c >= '0' && c <= '9'; }
}  // namespace

bool b_isalnum(Bytes b) {
    if (b.empty()) return false;
    for (uint8_t c : b)
        if (!alpha(c) && !digit(c)) return false;
    return true;
}

bool b_isalpha(Bytes b) {
    if (b.empty()) return false;
    for (uint8_t c : b)
        if (!alpha(c)) return false;
    return true;
}

bool b_isdigit(Bytes b) {
    if (b.empty()) return false;
    for (uint8_t c : b)
        if (!digit(c)) return false;
    return true;
}

bool b_is_upper_form(Bytes b) {
    for (uint8_t c : b)
        if (c >= 'a' && c <= 'z') return false;
    return true;
}

std::string to_latin1(const std::string &utf8) {
    std::string out;
    for (size_t i = 0; i < utf8.size();) {
        uint8_t c = uint8_t(utf8[i]);
        uint32_t cp;
        size_t n;
        if (c < 0x80) cp = c, n = 1;
        else if ((c & 0xE0) == 0xC0) cp = c & 0x1F, n = 2;
        else if ((c & 0xF0) == 0xE0) cp = c & 0x0F, n = 3;
        else cp = c & 0x07, n = 4;
        for (size_t k = 1; k < n && i + k < utf8.size(); ++k) cp = (cp << 6) | (uint8_t(utf8[i + k]) & 0x3F);
        if (cp > 0xFF) raise("UnicodeEncodeError", "'latin-1' codec can't encode character: ordinal not in range(256)");
        out += char(cp);
        i += n;
    }
    return out;
}

std::string hex(uint64_t v, bool upper) {
    char buf[24];
    std::snprintf(buf, sizeof buf, upper ? "%llX" : "%llx", static_cast<unsigned long long>(v));
    return buf;
}

std::string dec(int64_t v) { return std::to_string(v); }

}  // namespace cbf
