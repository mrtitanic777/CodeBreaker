// codecs.cc -- cp437, utf-16-be and utf-8 with "replace", as Python's decoders give them (text.h).
#include "text.h"

namespace cbf {

namespace {
const uint16_t kCp437High[128] = {
    0x00C7,0x00FC,0x00E9,0x00E2,0x00E4,0x00E0,0x00E5,0x00E7,0x00EA,0x00EB,0x00E8,0x00EF,0x00EE,0x00EC,0x00C4,0x00C5,
    0x00C9,0x00E6,0x00C6,0x00F4,0x00F6,0x00F2,0x00FB,0x00F9,0x00FF,0x00D6,0x00DC,0x00A2,0x00A3,0x00A5,0x20A7,0x0192,
    0x00E1,0x00ED,0x00F3,0x00FA,0x00F1,0x00D1,0x00AA,0x00BA,0x00BF,0x2310,0x00AC,0x00BD,0x00BC,0x00A1,0x00AB,0x00BB,
    0x2591,0x2592,0x2593,0x2502,0x2524,0x2561,0x2562,0x2556,0x2555,0x2563,0x2551,0x2557,0x255D,0x255C,0x255B,0x2510,
    0x2514,0x2534,0x252C,0x251C,0x2500,0x253C,0x255E,0x255F,0x255A,0x2554,0x2569,0x2566,0x2560,0x2550,0x256C,0x2567,
    0x2568,0x2564,0x2565,0x2559,0x2558,0x2552,0x2553,0x256B,0x256A,0x2518,0x250C,0x2588,0x2584,0x258C,0x2590,0x2580,
    0x03B1,0x00DF,0x0393,0x03C0,0x03A3,0x03C3,0x00B5,0x03C4,0x03A6,0x0398,0x03A9,0x03B4,0x221E,0x03C6,0x03B5,0x2229,
    0x2261,0x00B1,0x2265,0x2264,0x2320,0x2321,0x00F7,0x2248,0x00B0,0x2219,0x00B7,0x221A,0x207F,0x00B2,0x25A0,0x00A0};
constexpr uint32_t kReplacement = 0xFFFD;
}  // namespace

void put_codepoint(std::string &out, uint32_t cp) {
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

std::string cp437(Bytes b) {
    std::string out;
    out.reserve(static_cast<size_t>(b.size()));
    for (uint8_t c : b) put_codepoint(out, c < 0x80 ? c : kCp437High[c - 0x80]);
    return out;
}

bool cp437_byte(uint32_t cp, uint8_t &byte) {
    if (cp < 0x80) {
        byte = static_cast<uint8_t>(cp);
        return true;
    }
    for (int k = 0; k < 128; ++k)
        if (kCp437High[k] == cp) {
            byte = static_cast<uint8_t>(0x80 + k);
            return true;
        }
    return false;
}

std::string utf16be(Bytes b) {
    std::string swapped(static_cast<size_t>(b.size()), '\0');
    const int64_t n = b.size();
    for (int64_t i = 0; i + 1 < n; i += 2) {
        swapped[static_cast<size_t>(i)] = static_cast<char>(b.data()[i + 1]);
        swapped[static_cast<size_t>(i + 1)] = static_cast<char>(b.data()[i]);
    }
    if (n & 1) swapped[static_cast<size_t>(n - 1)] = static_cast<char>(b.data()[n - 1]);
    return utf16le(Bytes(swapped));
}

std::string utf8_replace(Bytes b) {
    std::string out;
    const uint8_t *p = b.data();
    const int64_t n = b.size();
    int64_t i = 0;
    while (i < n) {
        uint8_t c = p[i];
        if (c < 0x80) {
            out += static_cast<char>(c);
            ++i;
            continue;
        }
        int need;
        uint8_t lo = 0x80, hi = 0xBF;
        if (c >= 0xC2 && c <= 0xDF) {
            need = 1;
        } else if (c >= 0xE0 && c <= 0xEF) {
            need = 2;
            if (c == 0xE0) lo = 0xA0;
            if (c == 0xED) hi = 0x9F;
        } else if (c >= 0xF0 && c <= 0xF4) {
            need = 3;
            if (c == 0xF0) lo = 0x90;
            if (c == 0xF4) hi = 0x8F;
        } else {
            put_codepoint(out, kReplacement);
            ++i;
            continue;
        }
        uint32_t cp = c & (0x3F >> need);
        int64_t j = i + 1;
        bool ok = true;
        for (int k = 0; k < need; ++k, ++j) {
            uint8_t l = k == 0 ? lo : 0x80, h = k == 0 ? hi : 0xBF;
            if (j >= n || p[j] < l || p[j] > h) {
                ok = false;
                break;
            }
            cp = (cp << 6) | (p[j] & 0x3F);
        }
        put_codepoint(out, ok ? cp : kReplacement);
        i = j;
    }
    return out;
}

std::vector<uint32_t> codepoints(const std::string &utf8) {
    std::vector<uint32_t> out;
    const auto *p = reinterpret_cast<const uint8_t *>(utf8.data());
    size_t n = utf8.size(), i = 0;
    while (i < n) {
        uint8_t c = p[i];
        int extra = c < 0x80 ? 0 : c < 0xE0 ? 1 : c < 0xF0 ? 2 : 3;
        uint32_t cp = extra == 0 ? c : c & (0x3F >> extra);
        for (int k = 1; k <= extra && i + k < n; ++k) cp = (cp << 6) | (p[i + k] & 0x3F);
        out.push_back(cp);
        i += 1 + extra;
    }
    return out;
}

std::string from_codepoints(const std::vector<uint32_t> &cps) {
    std::string out;
    for (uint32_t c : cps) put_codepoint(out, c);
    return out;
}

}  // namespace cbf
