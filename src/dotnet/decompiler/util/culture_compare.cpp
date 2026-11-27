// The culture-sensitive comparison of .NET (ICU collation, en-US): see culture_compare.hpp for what is and is not implemented.
//
// Each string becomes a list of collation elements (one to several per character, from culture_compare_data.inc), and two strings are
// compared level by level as ICU compares sort keys: first the primary weights of both strings, if those are equal the secondary weights
// (accents), then the tertiary weights (case and variants). Ignorable weights (0) are left out of their level.
//
// An element is a 32-bit value as ICU's collation element iterator hands it out: primary (16 bits), secondary (8), tertiary (8). A primary
// that does not fit 16 bits comes as two elements, the second with 0xC0 set in its tertiary byte and the primary's low half in its top 16
// bits; as every level is compared as a sequence of its non-zero units, the pair needs no special handling.
#include "dotnet/decompiler/util/culture_compare.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace cb::dotnet::decompiler::util {
namespace {

struct Entry {
    std::uint32_t cp;
    std::uint32_t first;
    std::uint32_t count;
};

#include "dotnet/decompiler/util/culture_compare_data.inc"

constexpr std::uint32_t kHanExtensionAStart = 0x3400, kHanExtensionAEnd = 0x4DBF;
constexpr std::uint32_t kHanStart = 0x4E00, kHanEnd = 0x9FFF;
constexpr std::uint32_t kHangulStart = 0xAC00, kHangulEnd = 0xD7A3;

// UTF-8 text as code points; a byte that is not part of a valid sequence is U+FFFD.
std::vector<std::uint32_t> decode_utf8(std::string_view s) {
    std::vector<std::uint32_t> out;
    out.reserve(s.size());
    std::size_t i = 0;
    while (i < s.size()) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        std::uint32_t cp = 0xFFFD;
        std::size_t len = 1;
        if (c < 0x80) {
            cp = c;
        } else if (c >= 0xC2 && c < 0xE0) {
            len = 2;
            cp = c & 0x1F;
        } else if (c >= 0xE0 && c < 0xF0) {
            len = 3;
            cp = c & 0x0F;
        } else if (c >= 0xF0 && c < 0xF5) {
            len = 4;
            cp = c & 0x07;
        }
        if (len > 1) {
            bool ok = i + len <= s.size();
            for (std::size_t k = 1; ok && k < len; ++k) {
                unsigned char d = static_cast<unsigned char>(s[i + k]);
                if ((d & 0xC0) != 0x80) ok = false;
                else cp = (cp << 6) | (d & 0x3F);
            }
            if (!ok || (len == 3 && cp < 0x800) || (len == 4 && (cp < 0x10000 || cp > 0x10FFFF)) || (cp >= 0xD800 && cp < 0xE000)) {
                cp = 0xFFFD;
                len = 1;
            }
        }
        out.push_back(cp);
        i += len;
    }
    return out;
}

// ICU's elements for a code point that has no data: the "unassigned" primary, computed from the code point (after every assigned weight).
void append_unassigned(std::uint32_t cp, std::vector<std::uint32_t>& out) {
    std::uint64_t c = static_cast<std::uint64_t>(cp) + 1;
    std::uint32_t primary = 2 + static_cast<std::uint32_t>(c % 18) * 14;
    c /= 18;
    primary |= (2 + static_cast<std::uint32_t>(c % 254)) << 8;
    c /= 254;
    primary |= (4 + static_cast<std::uint32_t>(c % 251)) << 16;
    primary |= 0xFE000000u;
    out.push_back((primary & 0xFFFF0000u) | 0x0505u);
    out.push_back((primary << 16) | 0xC0u);
}

void append_elements(std::uint32_t cp, std::vector<std::uint32_t>& out) {
    if (cp >= kHangulStart && cp <= kHangulEnd) {
        // a syllable is its jamo: leading consonant, vowel and, if any, trailing consonant
        std::uint32_t index = cp - kHangulStart;
        append_elements(0x1100 + index / (21 * 28), out);
        append_elements(0x1161 + (index % (21 * 28)) / 28, out);
        if (index % 28 != 0) append_elements(0x11A7 + index % 28, out);
        return;
    }
    std::uint32_t han_rank = 0;
    if (cp >= kHanExtensionAStart && cp <= kHanExtensionAEnd) han_rank = kHan3400[cp - kHanExtensionAStart];
    else if (cp >= kHanStart && cp <= kHanEnd) han_rank = kHan4E00[cp - kHanStart];
    if (han_rank != 0) {
        // Han: every character has its own primary, in radical-stroke order, after all other scripts
        out.push_back(0xFA000505u);
        out.push_back(((0x100 + han_rank) << 16) | 0xC0u);
        return;
    }
    const Entry* begin = kEntries;
    const Entry* end = kEntries + sizeof(kEntries) / sizeof(kEntries[0]);
    const Entry* it = std::lower_bound(begin, end, cp, [](const Entry& e, std::uint32_t v) { return e.cp < v; });
    if (it != end && it->cp == cp) {
        out.insert(out.end(), kElements + it->first, kElements + it->first + it->count);
        return;
    }
    append_unassigned(cp, out);
}

int compare_sequences(const std::vector<std::uint32_t>& a, const std::vector<std::uint32_t>& b, int shift, std::uint32_t mask) {
    std::size_t i = 0, j = 0;
    for (;;) {
        while (i < a.size() && ((a[i] >> shift) & mask) == 0) ++i;
        while (j < b.size() && ((b[j] >> shift) & mask) == 0) ++j;
        if (i == a.size() || j == b.size()) {
            if (i == a.size() && j == b.size()) return 0;
            return i == a.size() ? -1 : 1;
        }
        std::uint32_t x = (a[i] >> shift) & mask, y = (b[j] >> shift) & mask;
        if (x != y) return x < y ? -1 : 1;
        ++i;
        ++j;
    }
}

int compare(std::string_view a, std::string_view b, bool tertiary) {
    std::vector<std::uint32_t> ea, eb;
    for (std::uint32_t cp : decode_utf8(a)) append_elements(cp, ea);
    for (std::uint32_t cp : decode_utf8(b)) append_elements(cp, eb);
    if (int r = compare_sequences(ea, eb, 16, 0xFFFF)) return r;
    if (int r = compare_sequences(ea, eb, 8, 0xFF)) return r;
    if (tertiary) return compare_sequences(ea, eb, 0, 0x3F);
    return 0;
}

}  // namespace

int culture_compare(std::string_view a, std::string_view b) { return compare(a, b, true); }

int culture_compare_ignore_case(std::string_view a, std::string_view b) { return compare(a, b, false); }

}  // namespace cb::dotnet::decompiler::util
