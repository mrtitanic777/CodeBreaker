// cb/translate/frontends/python/structfmt.hpp -- a Python `struct` format string, read the way CPython reads it.
//
// The Python front end turns struct.pack/unpack/calcsize calls into C++; it needs the layout a format string
// stands for. Only the standard-size forms the generated C++ support header implements are accepted: a byte-order
// prefix of < > ! = (or none / @, native, which on Windows x64 makes l and L four bytes), and the codes
// x c b B ? h H i I l L q Q f d s. Anything else is refused (the call is left to the front end's fallback).
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace cb::translate::frontends::python::structfmt {

enum class Kind { Pad, Bytes, Bool, Float, Int };

// One element of a layout. `code` is the format character. `size` is the number of bytes (for 'x' and 's' the
// repeat count written in the format: "4s" is one item of size 4). `lo`..`hi` is the range an Int or Bool value
// may hold ('Q' reaches 2**64-1, hence the unsigned `hi`); for Bytes it is the length a value must have
// ('c' is 1, 's' is the count); Pad and Float have none (0, 0).
struct Item {
    char code = 0;
    std::uint64_t size = 0;
    Kind kind = Kind::Pad;
    std::int64_t lo = 0;
    std::uint64_t hi = 0;
    bool operator==(const Item&) const = default;
};

// The word the front end uses for a kind: "pad", "bytes", "bool", "float", "int".
std::string_view kind_word(Kind kind);

// The layout of a format, or nullopt when the format is refused: a code outside the list, a count with no code,
// a character that is not part of a format, a native-alignment ('@' or no prefix) format that mixes sized
// numbers with anything else (alignment would pad between them), a repeat count past 2**20 or a struct larger than
// 2 GiB. "4H" gives four 'H' items; "c" repeated likewise; "" gives an empty layout.
std::optional<std::vector<Item>> parse(std::string_view format);

// The size of the struct in bytes.
std::uint64_t size(const std::vector<Item>& items);

// The items that carry a value (everything but the pads).
std::vector<Item> values(const std::vector<Item>& items);

}  // namespace cb::translate::frontends::python::structfmt
