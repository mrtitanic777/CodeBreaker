// text.h -- bytes turned into text as Python's decoders turn them (the answers carry UTF-8).
#pragma once

#include <string>

#include "bytes.h"

namespace cbf {

// data.decode("latin1"): every byte is the code point of its value
std::string latin1(Bytes b);
// data.decode("utf-16-le", "replace"): unpaired surrogates and a cut-off last unit become U+FFFD
std::string utf16le(Bytes b);
// data.decode("ascii"): UnicodeDecodeError on a byte past 0x7F
std::string ascii(Bytes b);
// data.decode("ascii", "replace")
std::string ascii_replace(Bytes b);

// bytes.isalnum() / isalpha() / isdigit() / isupper-style checks: ASCII only, False when empty
bool b_isalnum(Bytes b);
bool b_isalpha(Bytes b);
bool b_isdigit(Bytes b);
// b == b.upper()
bool b_is_upper_form(Bytes b);

// "%x" / "%X" / "%d"
std::string hex(uint64_t v, bool upper = false);
std::string dec(int64_t v);

}  // namespace cbf
