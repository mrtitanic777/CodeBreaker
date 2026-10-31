// unicode.hpp -- the Unicode database pyfront needs (internal): character names for \N{...},
// general categories for repr()'s printable test, XID_Start/XID_Continue for identifiers,
// NFKC for identifier normalization, and legacy source encodings.
//
// On Windows these come from the operating system's own ICU (System32\icu.dll, present
// since Windows 10 1903) and code pages, loaded on first use; nothing is downloaded or
// bundled.  Where they are unavailable each query says so and pyfront falls back: no \N{...},
// a built-in printable table, identifiers taken as written, UTF-8 and Latin-1 sources only.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace pyfront::uni {

bool available();

// \N{name}: the code point of a character name or alias (case-insensitive).
bool lookup_name(std::string_view name, uint32_t& cp);

// str.isprintable() for one code point: 1 printable, 0 not, -1 unknown (no database).
int printable(uint32_t cp);

// XID_Start / XID_Continue: 1 yes, 0 no, -1 unknown.
int xid_start(uint32_t cp);
int xid_continue(uint32_t cp);

// NFKC of a UTF-8 identifier; false when no database (out is then the input).
bool nfkc(std::string_view utf8, std::string& out);

// Convert `bytes` in the named source encoding (a PEP 263 coding cookie, already
// normalized to lower case) to UTF-8.  Returns false when the encoding is unknown.
bool to_utf8(std::string_view encoding, std::string_view bytes, std::string& out);

}  // namespace pyfront::uni
