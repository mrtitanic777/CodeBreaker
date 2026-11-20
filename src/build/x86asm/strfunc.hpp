// build/x86asm/strfunc.hpp -- NASM's string functions: the __utf16__ ... __utf32be__ transforms (asm/strfunc.c) and the
// conversion of a string to the integer it stands for in a numeric context (asm/rdstrnum.c).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "build/x86asm/types.hpp"

namespace cb::build::x86asm {

// Convert a string of UTF-8 bytes to UTF-16 or UTF-32, little or big endian as the function says (the plain __utf16__ and
// __utf32__ are little endian). nullopt for a string that is not valid UTF-8 (a stray continuation byte, a truncated or
// overlong sequence, a surrogate, and for UTF-16 a value above 0x10FFFF) -- NASM's `(size_t)-1`. (Like NASM, the
// big-endian UTF-16 form writes a supplementary character's two units low-surrogate first.)
std::optional<std::string> string_transform(std::string_view str, StrFunc func);

// The integer a string stands for when it is used as a number: its bytes, first byte lowest (readstrnum). `warn` is set
// if the string is longer than 8 bytes, in which case only the first 8 count.
std::int64_t readstrnum(std::string_view str, bool& warn);

}  // namespace cb::build::x86asm
