// codebreaker/formats/xbox/crypto/patch_checker.hpp -- C++ port of codebreaker.formats.xbox.crypto.patch_checker.
//
// patch_checker.py -- reads an xeBuild patch file (big-endian records: address, size in words, the patch
// bytes; ended by the address 0xFFFFFFFF) and prints every pair of patches whose address ranges overlap.
//
// Typing notes:
//   * eval_patches takes bytes or bytearray (the Python's `(bytes, bytearray)` annotation): one overload each.
//   * write_file likewise takes bytes or bytearray.
//   * `from .StreamIO import *` has no C++ counterpart; the StreamIO header is included instead.
//   * main() returns 0; the `exit(main())` of the script's __main__ guard is left to the caller.
#pragma once

#include <cstdint>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::patch_checker {

// Do the ranges [x1, x2] and [y1, y2] overlap?
bool is_overlapping(int64_t x1, int64_t x2, int64_t y1, int64_t y2);

// Prints "Patch Conflict!" and the two ranges for every ordered pair of overlapping patches.
// (struct.error, here py::StructError, if the data ends inside a record's address or size.)
void eval_patches(const py::bytes& patch_data);
void eval_patches(const py::bytearray& patch_data);

// The whole content of a file.
py::bytes read_file(const py::str& filename);

// Writes the bytes to a file (replacing it).
void write_file(const py::str& filename, const py::bytes& data);
void write_file(const py::str& filename, const py::bytearray& data);

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::patch_checker
