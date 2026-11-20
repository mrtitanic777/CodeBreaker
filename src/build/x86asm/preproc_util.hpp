// build/x86asm/preproc_util.hpp -- the small text helpers the preprocessor uses: NASM's character classes (include/nctype.h),
// case-blind comparison, quoting and unquoting of strings (asm/quote.c) and digit strings (nasmlib/numstr.c).
//
// The scanner and evaluator modules are other agents' work; these are the few pure functions the preprocessor needs on its
// own, ported exactly, so that it stands (and is tested) without them. They are in namespace `pp` so they cannot clash
// with the same functions a later module of the engine may declare.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace cb::build::x86asm::pp {

// ---- character classes (nctype.h) ----

enum CharClass : std::uint16_t {
    NCT_CTRL = 0x0001, NCT_SPACE = 0x0002, NCT_ASCII = 0x0004, NCT_LOWER = 0x0008, NCT_UPPER = 0x0010,
    NCT_DIGIT = 0x0020, NCT_HEX = 0x0040, NCT_ID = 0x0080, NCT_IDSTART = 0x0100, NCT_MINUS = 0x0200,
    NCT_DOLLAR = 0x0400, NCT_UNDER = 0x0800, NCT_QUOTE = 0x1000
};

namespace detail {
constexpr std::array<std::uint16_t, 256> make_ctype() {
    std::array<std::uint16_t, 256> t{};
    for (int i = 0; i < 256; ++i) {
        unsigned ct = 0;
        const bool is_ctrl = i < 32 || i == 127;
        const bool is_space = i == ' ' || (i >= 9 && i <= 13);
        const bool is_lower = i >= 'a' && i <= 'z';
        const bool is_upper = i >= 'A' && i <= 'Z';
        const bool is_digit = i >= '0' && i <= '9';
        const bool is_xdigit = is_digit || (i >= 'a' && i <= 'f') || (i >= 'A' && i <= 'F');
        const bool is_punct = i > 32 && i < 127 && !is_lower && !is_upper && !is_digit;
        if (is_ctrl) ct |= NCT_CTRL;
        if (i < 128) ct |= NCT_ASCII;
        if (is_space && i != '\n') ct |= NCT_SPACE;
        if (is_lower || is_upper) ct |= (is_lower ? NCT_LOWER : NCT_UPPER) | NCT_ID | NCT_IDSTART;
        if (is_digit) ct |= NCT_DIGIT | NCT_ID;
        if (is_xdigit) ct |= NCT_HEX;
        if (!ct && !is_punct) ct |= NCT_ID | NCT_IDSTART;   // a non-ASCII byte: part of an identifier
        t[i] = static_cast<std::uint16_t>(ct);
    }
    t['-'] |= NCT_MINUS;
    t['$'] |= NCT_DOLLAR | NCT_ID;
    t['_'] |= NCT_UNDER | NCT_ID | NCT_IDSTART;
    t['.'] |= NCT_ID | NCT_IDSTART;
    t['@'] |= NCT_ID | NCT_IDSTART;
    t['?'] |= NCT_ID | NCT_IDSTART;
    t['#'] |= NCT_ID;
    t['~'] |= NCT_ID;
    t['\''] |= NCT_QUOTE;
    t['"'] |= NCT_QUOTE;
    t['`'] |= NCT_QUOTE;
    return t;
}
inline constexpr std::array<std::uint16_t, 256> kCtype = make_ctype();
}  // namespace detail

constexpr bool is_class(char c, unsigned mask) { return (detail::kCtype[static_cast<unsigned char>(c)] & mask) != 0; }
constexpr bool isspace_(char c) { return is_class(c, NCT_SPACE); }
constexpr bool isalpha_(char c) { return is_class(c, NCT_LOWER | NCT_UPPER); }
constexpr bool isalnum_(char c) { return is_class(c, NCT_LOWER | NCT_UPPER | NCT_DIGIT); }
constexpr bool isdigit_(char c) { return is_class(c, NCT_DIGIT); }
constexpr bool isxdigit_(char c) { return is_class(c, NCT_HEX); }
constexpr bool isidstart(char c) { return is_class(c, NCT_IDSTART); }
constexpr bool isidchar(char c) { return is_class(c, NCT_ID); }
constexpr bool isnumstart(char c) { return is_class(c, NCT_DIGIT | NCT_DOLLAR); }
constexpr bool isnumchar(char c) { return is_class(c, NCT_DIGIT | NCT_LOWER | NCT_UPPER | NCT_UNDER); }
constexpr bool isquote(char c) { return is_class(c, NCT_QUOTE); }

constexpr char tolower_(char c) { return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c; }

// A hexadecimal digit's value, 16 or more when it is not one (nasm_hexval).
constexpr unsigned hexval(char c) {
    const unsigned v = static_cast<unsigned char>(c);
    return (v >= '0' && v <= '9') ? v - '0' : (v | 0x20) - 'a' + 10;
}

// ---- comparison without regard to case (nasm_stricmp and friends) ----

inline int stricmp_(std::string_view a, std::string_view b) {
    std::size_t i = 0;
    for (;; ++i) {
        const unsigned char c1 = i < a.size() ? static_cast<unsigned char>(tolower_(a[i])) : 0;
        const unsigned char c2 = i < b.size() ? static_cast<unsigned char>(tolower_(b[i])) : 0;
        if (c1 != c2) return static_cast<int>(c1) - static_cast<int>(c2);
        if (!c1) return 0;
    }
}
// The first `n` characters (a shorter string compares as if ended by NUL).
inline int strnicmp_(std::string_view a, std::string_view b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned char c1 = i < a.size() ? static_cast<unsigned char>(tolower_(a[i])) : 0;
        const unsigned char c2 = i < b.size() ? static_cast<unsigned char>(tolower_(b[i])) : 0;
        if (c1 != c2) return static_cast<int>(c1) - static_cast<int>(c2);
        if (!c1) return 0;
    }
    return 0;
}
inline bool ieq(std::string_view a, std::string_view b) { return a.size() == b.size() && stricmp_(a, b) == 0; }
inline bool memieq(const char* a, const char* b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        if (tolower_(a[i]) != tolower_(b[i])) return false;
    return true;
}
inline std::string lcase(std::string_view s) {
    std::string r(s);
    for (char& c : r) c = tolower_(c);
    return r;
}

// Skip spaces / a word (nasm_skip_spaces, nasm_skip_word).
inline const char* skip_blanks(const char* p) {
    while (*p && isspace_(*p)) ++p;
    return p;
}
inline const char* skip_word(const char* p) {
    while (*p && !isspace_(*p)) ++p;
    return p;
}

// ---- quoting (quote.c) ----

inline constexpr char STR_C = '"';       // the initial quotation mark of a C string
inline constexpr char STR_NASM = '`';    // ... of a NASM string
// The control characters allowed in a C string: BEL BS TAB ESC; BADCTL is every other one.
inline constexpr std::uint32_t OKCTL = (1u << 7) | (1u << 8) | (1u << 9) | (1u << 27);
inline constexpr std::uint32_t BADCTL = ~OKCTL;

// A string as a quoted source token: 'single', "double" when it holds a single quote, else `backquoted` with escapes.
std::string quote(std::string_view str);

// The text a quoted string stands for. `str` starts at the quote (or is unquoted text, which is copied as it is); the text
// ends at the closing quote. `ctl_error` is set when a control character in `badctl` appeared (the caller reports it).
// `end` (optional) gets the number of characters of `str` consumed, up to the closing quote.
std::string unquote(std::string_view str, std::uint32_t badctl, char qstart, bool& ctl_error, std::size_t* end = nullptr);

// The end of a quoted string: the index of its closing quote, or str.size() when it is not closed (nasm_skip_string).
// Returns npos when `str` does not start with a quote character at all.
std::size_t skip_string(const char* str);

// ---- numbers (numstr.c) ----

inline constexpr unsigned NUMSTR_MAXBASE = 64;

// The digit characters, lower- or upper-case first (nasm_digit_chars).
const char* digit_chars(bool ucase);

// The digits of `n` in `base`: at least `digits` of them (a negative `digits` means "as many as there are, but at least
// -digits"? no: digits <= 0 is "no minimum, more allowed"), as numstr() builds them. Returns "" for an invalid base.
std::string numstr(std::uint64_t n, int digits, unsigned base, bool ucase);

// The base a radix letter stands for (radix_letter in nasmlib.h): b y -> 2, d t -> 10, o q -> 8, h x -> 16.
unsigned radix_letter(char c);

}  // namespace cb::build::x86asm::pp
