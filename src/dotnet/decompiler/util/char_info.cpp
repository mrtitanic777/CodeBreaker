// dotnet/decompiler/util/char_info.cpp -- .NET's character classification, from .NET's own category table.
#include "dotnet/decompiler/util/char_info.hpp"

#include <algorithm>
#include <array>
#include <iterator>

namespace cb::dotnet::decompiler::util {
namespace {

struct Run {
    std::uint16_t first;
    std::uint8_t category;
};

constexpr Run kRuns[] = {
#include "dotnet/decompiler/util/char_categories.inc"
};

}  // namespace

UnicodeCategory get_unicode_category(char16_t c) {
    const auto it = std::upper_bound(std::begin(kRuns), std::end(kRuns), static_cast<std::uint16_t>(c),
                                     [](std::uint16_t v, const Run& r) { return v < r.first; });
    return static_cast<UnicodeCategory>(std::prev(it)->category);
}

bool is_letter(char16_t c) { return static_cast<int>(get_unicode_category(c)) <= static_cast<int>(UnicodeCategory::OtherLetter); }

bool is_digit(char16_t c) { return get_unicode_category(c) == UnicodeCategory::DecimalDigitNumber; }

bool is_letter_or_digit(char16_t c) {
    const UnicodeCategory k = get_unicode_category(c);
    return static_cast<int>(k) <= static_cast<int>(UnicodeCategory::OtherLetter) || k == UnicodeCategory::DecimalDigitNumber;
}

bool is_control(char16_t c) { return get_unicode_category(c) == UnicodeCategory::Control; }

bool is_white_space(char16_t c) {
    // Latin-1 as .NET lists it: tab .. carriage return, space, NEL, NBSP; above it the separator categories
    if (c <= 0xFF) return (c >= 0x09 && c <= 0x0D) || c == 0x20 || c == 0x85 || c == 0xA0;
    const UnicodeCategory k = get_unicode_category(c);
    return k == UnicodeCategory::SpaceSeparator || k == UnicodeCategory::LineSeparator || k == UnicodeCategory::ParagraphSeparator;
}

std::u16string utf16(std::string_view s) {
    std::u16string out;
    out.reserve(s.size());
    std::size_t i = 0;
    auto byte = [&](std::size_t k) { return static_cast<std::uint8_t>(s[k]); };
    auto cont = [&](std::size_t k) { return k < s.size() && (byte(k) & 0xC0) == 0x80; };
    while (i < s.size()) {
        const std::uint8_t b0 = byte(i);
        char32_t cp = 0xFFFD;
        std::size_t n = 1;
        if (b0 < 0x80) {
            cp = b0;
        } else if ((b0 & 0xE0) == 0xC0 && b0 >= 0xC2 && cont(i + 1)) {
            cp = static_cast<char32_t>((b0 & 0x1F) << 6 | (byte(i + 1) & 0x3F));
            n = 2;
        } else if ((b0 & 0xF0) == 0xE0 && cont(i + 1) && cont(i + 2)) {
            cp = static_cast<char32_t>((b0 & 0x0F) << 12 | (byte(i + 1) & 0x3F) << 6 | (byte(i + 2) & 0x3F));
            n = 3;
            if (cp < 0x800) {
                cp = 0xFFFD;
                n = 1;
            }
        } else if ((b0 & 0xF8) == 0xF0 && cont(i + 1) && cont(i + 2) && cont(i + 3)) {
            cp = static_cast<char32_t>((b0 & 0x07) << 18 | (byte(i + 1) & 0x3F) << 12 | (byte(i + 2) & 0x3F) << 6 | (byte(i + 3) & 0x3F));
            n = 4;
            if (cp < 0x10000 || cp > 0x10FFFF) {
                cp = 0xFFFD;
                n = 1;
            }
        }
        if (cp >= 0x10000) {
            out.push_back(static_cast<char16_t>(0xD800 + ((cp - 0x10000) >> 10)));
            out.push_back(static_cast<char16_t>(0xDC00 + ((cp - 0x10000) & 0x3FF)));
        } else {
            out.push_back(static_cast<char16_t>(cp));
        }
        i += n;
    }
    return out;
}

std::string utf8(std::u16string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        char32_t cp = s[i];
        if (is_high_surrogate(s[i]) && i + 1 < s.size() && is_low_surrogate(s[i + 1])) {
            cp = 0x10000 + ((static_cast<char32_t>(s[i]) - 0xD800) << 10) + (static_cast<char32_t>(s[i + 1]) - 0xDC00);
            ++i;
        }
        if (cp < 0x80) {
            out.push_back(static_cast<char>(cp));
        } else if (cp < 0x800) {
            out.push_back(static_cast<char>(0xC0 | cp >> 6));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else if (cp < 0x10000) {
            out.push_back(static_cast<char>(0xE0 | cp >> 12));
            out.push_back(static_cast<char>(0x80 | (cp >> 6 & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        } else {
            out.push_back(static_cast<char>(0xF0 | cp >> 18));
            out.push_back(static_cast<char>(0x80 | (cp >> 12 & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp >> 6 & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }
    return out;
}

}  // namespace cb::dotnet::decompiler::util
