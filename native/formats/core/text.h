// text.h -- bytes turned into text as Python's decoders turn them (the answers carry UTF-8).
#pragma once

#include <string>
#include <vector>

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

// str.encode("latin1") of a str given as UTF-8: UnicodeEncodeError past U+00FF
std::string to_latin1(const std::string &utf8);

// "%x" / "%X" / "%d"
std::string hex(uint64_t v, bool upper = false);
std::string dec(int64_t v);

// (codecs.cc) data.decode("cp437") -- every byte maps; cp437_byte: the byte a code point encodes to
std::string cp437(Bytes b);
bool cp437_byte(uint32_t cp, uint8_t &byte);
// data.decode("utf-16-be", "replace")
std::string utf16be(Bytes b);
// data.decode("utf-8", "replace"): CPython's rule, one U+FFFD per maximal ill-formed subpart
std::string utf8_replace(Bytes b);
// a str (UTF-8) as code points and back; one code point appended as UTF-8
std::vector<uint32_t> codepoints(const std::string &utf8);
std::string from_codepoints(const std::vector<uint32_t> &cps);
void put_codepoint(std::string &out, uint32_t cp);

}  // namespace cbf
