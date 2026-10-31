// literals.hpp -- decoding Python literals and printing Python reprs (internal).
#pragma once

#include <string>
#include <string_view>

#include "pyfront/ast.hpp"

namespace pyfront {

// Append the UTF-8 (generalized: surrogates allowed) encoding of code point `cp`.
void append_utf8(std::string& out, uint32_t cp);

// Decode the body of a string literal (between the quotes).  For f-string pieces pass
// fstring=true: `{{` and `}}` become single braces.  On a bad escape returns false and sets
// `err` to CPython's message and `err_off` to the offset of the offending backslash.
bool decode_string(std::string_view body, bool raw, bool bytes, bool fstring, std::string& out,
                   std::string& err, size_t& err_off);

// A NUMBER token's value: Int, Float or Complex.
ConstValue parse_number(std::string_view tok);

// Python's repr() of each constant kind.
std::string repr_str(std::string_view utf8);
std::string repr_bytes(std::string_view bytes);
std::string repr_float(double d, bool complex_part = false);
std::string repr_const(const ConstValue& v);

}  // namespace pyfront
