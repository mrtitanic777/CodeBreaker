// cb/port/asmdata.hpp -- the data of an assembler module, as C (part of the port; see asm16.hpp for its code).
//
// A module of data alone is the old linker's business: its bytes were placed, and the names in them were offsets. Here
// they are C: each table given the C program's types where it declares them (and where a pointer of a declared one points:
// that type too), its words that held an offset a pointer now, initialized as the bytes were.
//
//   image       the module's segments: their bytes, which words were the offset of a name (relocations), the labels'
//               places, each segment's class
//   messages    a table of numbered messages for the C library's __NMSG_TEXT / __FMSG_TEXT (segments of class MSG /
//               FAR_MSG: a word, the number; the text; a 0), as C: the table and the two lookups
//   typed       the tables of a data module with pointers in them, as C definitions with initializers
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/port/asm16.hpp"

namespace cb::port::asmdata {

// A word of a segment that was the offset of a name: the name, and what was added to it (`sym+4`).
struct Reloc {
    std::string symbol;
    std::int64_t displacement = 0;
};

struct Item {
    std::size_t offset = 0, size = 0;   // a DB/DW/DD item: where it is, how long
};

struct Segment {
    std::string name;
    std::optional<std::string> cls;           // `'CODE'`, `'MSG'` ...
    Bytes data;
    std::map<std::size_t, Reloc> reloc;       // by the offset of the word
    OrderedMap<std::string, std::size_t> labels;   // name: offset, in the order they were met
    std::vector<Item> items;                  // each item of data, in order
};

using Image = OrderedMap<std::string, Segment>;   // by segment name, in the order they were met

// The module's segments. Throws asm16::Asm16Error for a data item that is not understood.
Image image(std::string_view text);

// Bytes as a C string literal (its terminating 0 left to C): printable ASCII as it is, the rest an octal escape; an
// escape followed by a digit is closed off so that the digit is not part of it.
std::string c_string(ByteView bytes);

// The names the module declares PUBLIC.
std::set<std::string> publics(std::string_view text);

struct TypedData {
    std::string text;                    // the C: the declarations, then the definitions with their initializers
    std::vector<std::string> notes;      // what could not be typed (the head of the text names the first twenty)
};

// The C of a data module whose words hold addresses (a table of chains, of names): each table typed -- by the C program's
// declaration of its name, or by what points to it -- and initialized as its bytes were. nullopt when the module has no
// addresses in it. A table whose type has no known size is left out (and noted).
std::optional<TypedData> typed(std::string_view text, const asm16::CTypes& types);

// The C of a module of numbered messages (segments of class MSG and FAR_MSG), or nullopt: a table of each -- the near ones
// __NMSG_TEXT's, the far ones __FMSG_TEXT's (their numbers are their own: the same number in both is two messages).
std::optional<std::string> messages(std::string_view text);

}  // namespace cb::port::asmdata
