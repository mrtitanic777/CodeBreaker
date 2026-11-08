// codebreaker/formats/xbox/crypto/fuses.hpp -- C++ port of codebreaker.formats.xbox.crypto.fuses.
//
// Xbox 360 eFuse and CPU key tools.
//
// Typing notes:
//   set_fuseline's `value` is an int, a 16-character hex string or 8 bytes in the Python: a std::variant
//   of the three (the one place that module mixes kinds).  An int beyond int64 (a 64-bit unsigned line
//   >= 2**63) cannot be passed; it would raise OverflowError at the caller's conversion, not here.
//   parse_fuses returns a dict of text values and one int ("ldv"): a std::variant<py::str, int64_t>.
//   main() reads sys.argv[1:] in the Python; here the argument list is passed in.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::fuses {

using FuseValue = std::variant<int64_t, py::str, py::bytes>;
using FuseInfo = py::dict<py::str, std::variant<py::str, int64_t>>;

// Set a fuse line (8 bytes) in fuse data.
py::bytearray set_fuseline(const py::bytes& fuse_data, int64_t line, const FuseValue& value);

// Get a fuse line (8 bytes) from fuse data.
py::bytes get_fuseline(const py::bytes& fuse_data, int64_t line);

// Extract CPU key from fuse data (lines 3-4).
py::bytes get_cpu_key_from_fuses(const py::bytes& fuse_data);

// Parse fuse data into structured info.
FuseInfo parse_fuses(const py::bytes& fuse_data);

extern const py::str DESCRIPTION;

// argv is sys.argv[1:]
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::fuses
