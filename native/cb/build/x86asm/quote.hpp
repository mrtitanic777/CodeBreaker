// cb/build/x86asm/quote.hpp -- NASM's string quoting rules (asm/quote.c, asm/quote.h).
//
// NASM has three kinds of string literal: 'single' and "double" quotes hold their text exactly (no escapes, and no
// doubling of the quote mark), and `backquotes` hold C-like escapes (\n \x41 € \^A \o101 \d065 \e ...). A string
// is bytes: it can hold NUL and any control character. These functions make a quoted string from bytes, find where a
// quoted string ends, and turn a quoted string into its bytes.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace cb::build::x86asm {

class Assembly;

// The control characters a string for use as a C string may hold: BEL BS TAB ESC. BADCTL is every other one.
inline constexpr std::uint32_t OKCTL = (1u << 7) | (1u << 8) | (1u << 9) | (1u << 27);
inline constexpr std::uint32_t BADCTL = ~OKCTL;

// The lead marker of a string with escapes: NASM style (backquote) or C style (double quote, where a backquote is not
// special at all).
inline constexpr char STR_C = '"';
inline constexpr char STR_NASM = '`';

// Make the NASM quoted string for these bytes: plain '...' or "..." when the bytes allow it, else a `...` with
// escapes (nasm_quote).
std::string nasm_quote(std::string_view str);

// What unquoting a string gave: the bytes, and `end`, the offset in the input of the terminating character (the closing
// quote, or the end of the input when the string was not terminated).
struct Unquoted {
    std::string text;
    std::size_t end = 0;
};

// Unquote the string that starts at the first character of `str` (nasm_unquote_anystr). A NUL byte ends the string like
// the end of the input does. Reports "control character in string not allowed here" if the result would hold a control
// character whose bit is set in `badctl`; the text is then cut off before the first such character (the end is not).
// `qstart` is STR_NASM or STR_C, the lead marker whose strings have escapes.
Unquoted nasm_unquote_anystr(Assembly& as, std::string_view str, std::uint32_t badctl, char qstart);

// Unquote any string; may produce any bytes (nasm_unquote).
Unquoted nasm_unquote(Assembly& as, std::string_view str);

// Unquote a string meant as a C string: most control characters are rejected (nasm_unquote_cstr).
Unquoted nasm_unquote_cstr(Assembly& as, std::string_view str);

// The offset of the terminating character of a NASM-style quoted string (the closing quote, or the end of the input
// when unterminated), or nullopt if `str` does not start with a quote mark (nasm_skip_string).
std::optional<std::size_t> nasm_skip_string(std::string_view str);

}  // namespace cb::build::x86asm
