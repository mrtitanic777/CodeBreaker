// codebreaker/formats/xbox/crypto/kv_tool.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.kv_tool.
//
// Builds the files a devkit-style console image needs from a banned KV: a fresh CPU key, the KV
// (KV/banned.bin) with a zero DVD key and encrypted with that CPU key, and the 12-line fuse set for the
// key and the console type.
//
// Typing notes (the Python mixes types in a few places):
//   * set_fuseline's fuse_data is bytes or bytearray: one overload each. A bytes is copied to a new
//     bytearray; a bytearray is changed in place and returned (the same buffer, as in the Python).
//   * set_fuseline's value is a str, an int, bytes or a bytearray: a std::variant. An int is packed as
//     ">Q" (struct.error when negative); the int is an int64_t, so a value of 2**63 or more cannot be given.
//     (The Python's `type(value) is int` is false for a bool: a bool is not a value here.)
//   * ConsoleType is an IntEnum: an enum class over int64_t.
//   * main() reads and writes KV/banned.bin, Output/Zero/fuses.bin and Output/Zero/kv_enc.bin relative to the
//     current folder, prints with py::print, and returns the exit status (the Python's `exit(main())`).
//   * The CPU key and the KV's random fields come from Python's `random` generator through the XeCrypt port
//     (XeCrypt::random_seed(a) seeds it like random.seed(a)).
#pragma once

#include <cstdint>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::kv_tool {

enum class ConsoleType : int64_t {
    RETAIL_PHAT = 0,
    RETAIL_SLIM = 1,
    TEST_KIT = 2,
    DEVKIT = 3,
};

using FuseValue = std::variant<py::str, int64_t, py::bytes, py::bytearray>;

// AssertionError("Invalid fuse line") for a hex string that is not 16 characters, or a value that is not
// 8 bytes; ValueError for a string that is not hexadecimal; struct.error for a negative int.
py::bytearray set_fuseline(const py::bytes& fuse_data, int64_t line, const FuseValue& value);
py::bytearray set_fuseline(const py::bytearray& fuse_data, int64_t line, const FuseValue& value);

int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::kv_tool
