// cb/build/x86asm/strfunc.cpp -- NASM's string functions; see strfunc.hpp. A port of asm/strfunc.c and asm/rdstrnum.c.
#include "cb/build/x86asm/strfunc.hpp"

#include <vector>

namespace cb::build::x86asm {

namespace {

// Decode UTF-8 (including the old 5- and 6-byte forms, which NASM accepts) into code points, as NASM's four transform
// functions do: a continuation byte where a lead byte should be, a lead byte of 0xfe or 0xff, an overlong value, a
// surrogate, or (when `limit_unicode`) a value above 0x10ffff is invalid. `units` gets, for each code point, the values to
// emit in order: one value, or for UTF-16 above 0xffff the pair (high, low) surrogates.
//
// `big_endian_16` marks the quirk of utf8_to_16be, which emits the two surrogates of a pair low first.
std::optional<std::vector<std::uint32_t>> strfunc_decode(std::string_view str, bool utf16, bool big_endian_16) {
    std::vector<std::uint32_t> out;
    int expect = 0;
    std::uint32_t v = 0, vmin = 0;
    for (const char ch : str) {
        const auto c = static_cast<std::uint8_t>(ch);
        if (expect) {
            if ((c & 0xc0) != 0x80) return std::nullopt;
            v = (v << 6) | (c & 0x3f);
            if (!--expect) {
                if (v < vmin || (v >= 0xd800 && v <= 0xdfff) || (utf16 && v > 0x10ffff)) return std::nullopt;
                if (utf16 && v > 0xffff) {
                    v -= 0x10000;
                    const std::uint32_t hi = 0xd800 | (v >> 10);
                    const std::uint32_t lo = 0xdc00 | (v & 0x3ff);
                    if (big_endian_16) {
                        out.push_back(lo);
                        out.push_back(hi);
                    } else {
                        out.push_back(hi);
                        out.push_back(lo);
                    }
                } else {
                    out.push_back(v);
                }
            }
            continue;
        }
        if (c < 0x80) {
            out.push_back(c);
        } else if (c < 0xc0 || c >= 0xfe) {
            return std::nullopt;   // invalid UTF-8
        } else if (c < 0xe0) {
            v = c & 0x1f;
            expect = 1;
            vmin = 0x80;
        } else if (c < 0xf0) {
            v = c & 0x0f;
            expect = 2;
            vmin = 0x800;
        } else if (c < 0xf8) {
            v = c & 0x07;
            expect = 3;
            vmin = 0x10000;
        } else if (c < 0xfc) {
            v = c & 0x03;
            expect = 4;
            vmin = 0x200000;
        } else {
            v = c & 0x01;
            expect = 5;
            vmin = 0x4000000;
        }
    }
    if (expect) return std::nullopt;
    return out;
}

}  // namespace

std::optional<std::string> string_transform(std::string_view str, StrFunc func) {
    const bool utf16 = func == STRFUNC_UTF16 || func == STRFUNC_UTF16LE || func == STRFUNC_UTF16BE;
    const bool big = func == STRFUNC_UTF16BE || func == STRFUNC_UTF32BE;
    const auto units = strfunc_decode(str, utf16, utf16 && big);
    if (!units) return std::nullopt;

    std::string out;
    out.reserve(units->size() * (utf16 ? 2 : 4));
    for (const std::uint32_t u : *units) {
        if (utf16) {
            if (big) {
                out.push_back(static_cast<char>(u >> 8));
                out.push_back(static_cast<char>(u));
            } else {
                out.push_back(static_cast<char>(u));
                out.push_back(static_cast<char>(u >> 8));
            }
        } else if (big) {
            out.push_back(static_cast<char>(u >> 24));
            out.push_back(static_cast<char>(u >> 16));
            out.push_back(static_cast<char>(u >> 8));
            out.push_back(static_cast<char>(u));
        } else {
            out.push_back(static_cast<char>(u));
            out.push_back(static_cast<char>(u >> 8));
            out.push_back(static_cast<char>(u >> 16));
            out.push_back(static_cast<char>(u >> 24));
        }
    }
    return out;
}

std::int64_t readstrnum(std::string_view str, bool& warn) {
    warn = false;
    std::size_t length = str.size();
    if (length > 8) {
        warn = true;
        length = 8;
    }
    std::uint64_t charconst = 0;
    for (std::size_t i = 0; i < length; ++i) charconst += static_cast<std::uint64_t>(static_cast<std::uint8_t>(str[i])) << (i * 8);
    return static_cast<std::int64_t>(charconst);
}

}  // namespace cb::build::x86asm
