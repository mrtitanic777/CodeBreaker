// cb/exact/symbols.hpp -- a project's SYMBOLS: where each name the program uses is.
//
// A name is a function (Code), a variable (Data), a segment word ("name@seg", Segment) or a place whose kind is not
// said (Unknown). The address is an image address or a data offset; it is signed because a place learnt from the
// program (the place a word implies) can come out negative when the program disagrees with the object, which then
// surfaces as a note rather than a wrap-around.
#pragma once

#include <cstdint>
#include <string>

#include "cb/core/ordered_map.hpp"

namespace cb::exact {

enum class SymKind { Code, Data, Segment, Unknown };

struct SymPlace {
    SymKind kind = SymKind::Unknown;
    std::int64_t address = 0;
    friend bool operator==(const SymPlace&, const SymPlace&) = default;
};

// Insertion order is the meaning (the first name for a place wins in `readable`'s mapping; `compare` appends what
// it learns in the order it learnt it).
using SymbolTable = OrderedMap<std::string, SymPlace>;

// SYMBOLS writes the kind as one letter: C, D, S, ?
constexpr char sym_kind_letter(SymKind k) {
    switch (k) {
        case SymKind::Code: return 'C';
        case SymKind::Data: return 'D';
        case SymKind::Segment: return 'S';
        case SymKind::Unknown: break;
    }
    return '?';
}

}  // namespace cb::exact
