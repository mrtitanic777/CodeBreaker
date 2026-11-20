// build/x86asm/nctype.hpp -- NASM's character classes (include/nctype.h, nasmlib/nctype.c) and the digit helpers of
// include/nasmlib.h.
//
// NASM's identifiers, numbers and quotes are decided by its own table, not by <ctype.h>: `.`, `@`, `?`, `_` start an
// identifier, `$` and `#` and `~` continue one, every byte of 128 and above is an identifier character (so UTF-8 names
// work), and a newline is not a space. The table here is the one NASM builds for the C locale, computed at compile time.
#pragma once

#include <array>
#include <cstdint>

namespace cb::build::x86asm {

// The class bits (enum nasm_ctype).
namespace nct {

// (In a namespace of their own: the preprocessor has an enumeration of the same names.)
enum : std::uint16_t {
    NCT_CTRL = 0x0001,
    NCT_SPACE = 0x0002,
    NCT_ASCII = 0x0004,
    NCT_LOWER = 0x0008,    // isalpha(x) && tolower(x) == x
    NCT_UPPER = 0x0010,    // isalpha(x) && tolower(x) != x
    NCT_DIGIT = 0x0020,
    NCT_HEX = 0x0040,
    NCT_ID = 0x0080,
    NCT_IDSTART = 0x0100,
    NCT_MINUS = 0x0200,    // -
    NCT_DOLLAR = 0x0400,   // $
    NCT_UNDER = 0x0800,    // _
    NCT_QUOTE = 0x1000     // " ' `
};

}  // namespace nct

namespace nctype_detail {

constexpr std::array<std::uint16_t, 256> make_ctype_table() {
    std::array<std::uint16_t, 256> t{};
    for (int i = 0; i < 256; ++i) {
        unsigned ct = 0;
        const bool ascii = i < 128;
        const bool lower = i >= 'a' && i <= 'z';
        const bool upper = i >= 'A' && i <= 'Z';
        const bool digit = i >= '0' && i <= '9';
        const bool xdigit = digit || (i >= 'a' && i <= 'f') || (i >= 'A' && i <= 'F');
        const bool cntrl = i < 32 || i == 127;
        const bool space = i == ' ' || (i >= 9 && i <= 13);   // C-locale isspace
        const bool punct = ascii && !cntrl && !lower && !upper && !digit && i != ' ';
        if (cntrl) ct |= nct::NCT_CTRL;
        if (ascii) ct |= nct::NCT_ASCII;
        if (space && i != '\n') ct |= nct::NCT_SPACE;
        if (lower) ct |= nct::NCT_LOWER | nct::NCT_ID | nct::NCT_IDSTART;
        if (upper) ct |= nct::NCT_UPPER | nct::NCT_ID | nct::NCT_IDSTART;
        if (digit) ct |= nct::NCT_DIGIT | nct::NCT_ID;
        if (xdigit) ct |= nct::NCT_HEX;
        // A non-ASCII byte no ctype recognises (UTF-8 and the like) is an identifier character.
        if (!ct && !punct) ct |= nct::NCT_ID | nct::NCT_IDSTART;
        t[static_cast<std::size_t>(i)] = static_cast<std::uint16_t>(ct);
    }
    t['-'] = static_cast<std::uint16_t>(t['-'] | (nct::NCT_MINUS));
    t['$'] = static_cast<std::uint16_t>(t['$'] | (nct::NCT_DOLLAR | nct::NCT_ID));
    t['_'] = static_cast<std::uint16_t>(t['_'] | (nct::NCT_UNDER | nct::NCT_ID | nct::NCT_IDSTART));
    t['.'] = static_cast<std::uint16_t>(t['.'] | (nct::NCT_ID | nct::NCT_IDSTART));
    t['@'] = static_cast<std::uint16_t>(t['@'] | (nct::NCT_ID | nct::NCT_IDSTART));
    t['?'] = static_cast<std::uint16_t>(t['?'] | (nct::NCT_ID | nct::NCT_IDSTART));
    t['#'] = static_cast<std::uint16_t>(t['#'] | (nct::NCT_ID));
    t['~'] = static_cast<std::uint16_t>(t['~'] | (nct::NCT_ID));
    t['\''] = static_cast<std::uint16_t>(t['\''] | (nct::NCT_QUOTE));
    t['"'] = static_cast<std::uint16_t>(t['"'] | (nct::NCT_QUOTE));
    t['`'] = static_cast<std::uint16_t>(t['`'] | (nct::NCT_QUOTE));
    return t;
}

inline constexpr std::array<std::uint16_t, 256> kCtypeTable = make_ctype_table();

}  // namespace nctype_detail

// True if the byte is in any of the classes of `mask`.
constexpr bool nasm_ctype(char x, std::uint16_t mask) { return (nctype_detail::kCtypeTable[static_cast<unsigned char>(x)] & mask) != 0; }

constexpr char nasm_tolower(char x) { return (x >= 'A' && x <= 'Z') ? static_cast<char>(x + ('a' - 'A')) : x; }
constexpr bool nasm_isspace(char x) { return nasm_ctype(x, nct::NCT_SPACE); }
constexpr bool nasm_isalpha(char x) { return nasm_ctype(x, nct::NCT_LOWER | nct::NCT_UPPER); }
constexpr bool nasm_isdigit(char x) { return nasm_ctype(x, nct::NCT_DIGIT); }
constexpr bool nasm_isalnum(char x) { return nasm_ctype(x, nct::NCT_LOWER | nct::NCT_UPPER | nct::NCT_DIGIT); }
constexpr bool nasm_isxdigit(char x) { return nasm_ctype(x, nct::NCT_HEX); }
constexpr bool nasm_isidstart(char x) { return nasm_ctype(x, nct::NCT_IDSTART); }
constexpr bool nasm_isidchar(char x) { return nasm_ctype(x, nct::NCT_ID); }
constexpr bool nasm_isbrcchar(char x) { return nasm_ctype(x, nct::NCT_ID | nct::NCT_MINUS); }
constexpr bool nasm_isnumstart(char x) { return nasm_ctype(x, nct::NCT_DIGIT | nct::NCT_DOLLAR); }
constexpr bool nasm_isnumchar(char x) { return nasm_ctype(x, nct::NCT_DIGIT | nct::NCT_LOWER | nct::NCT_UPPER | nct::NCT_UNDER); }
constexpr bool nasm_isquote(char x) { return nasm_ctype(x, nct::NCT_QUOTE); }

// The value of a hexadecimal digit; 16 or more if `c` is not one (nasm_hexval).
constexpr unsigned nasm_hexval(char c) {
    const unsigned v = static_cast<unsigned char>(c);
    if (v >= '0' && v <= '9') return v - '0';
    return (v | 0x20) - 'a' + 10;
}

// The value of a digit, with no check that it is one (numvalue).
constexpr unsigned numvalue(unsigned char c) {
    c = static_cast<unsigned char>(c | 0x20);
    return c >= 'a' ? c - 'a' + 10u : c - '0';
}

// The same, except that a non-digit gives ~0u so the result compares as too big for any base (numvalue_chk).
constexpr unsigned numvalue_chk(unsigned char c) {
    unsigned v = static_cast<unsigned>(c) - '0';
    if (v < 10) return v;
    v = (static_cast<unsigned>(c) | 0x20) - 'a';
    if (v < 26) return v + 10;
    return ~0u;
}

// The numeric base a radix letter stands for (b/y 2, o/q 8, h/x 16, d/t 10), or 0 if it is not one (nasm_radix_letter).
constexpr unsigned nasm_radix_letter(char c) {
    switch (c) {
    case 'b': case 'B': case 'y': case 'Y': return 2;
    case 'o': case 'O': case 'q': case 'Q': return 8;
    case 'h': case 'H': case 'x': case 'X': return 16;
    case 'd': case 'D': case 't': case 'T': return 10;
    default: return 0;
    }
}

}  // namespace cb::build::x86asm
