// cb/decompile/labels.hpp -- the two label tables the assemblers and disassemblers of the small CPUs share.
//
// A disassembler writes an address that has a label as the label (`AddressLabels`, address -> name); an assembler
// resolves a label to an address (`NameLabels`, name -> address). Both are looked up and never iterated into an
// output, so they are hash maps. An address is whatever the CPU's space holds (16 bits for the Z80 and the 6502), a
// name's value is signed and wide because an assembler's first pass hands out placeholder values.
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace cb::decompile {

// Label name by address; an empty name counts as no label.
using AddressLabels = std::unordered_map<std::uint32_t, std::string>;

struct NameHash {
    using is_transparent = void;
    std::size_t operator()(std::string_view s) const { return std::hash<std::string_view>()(s); }
};

// Address by label name (looked up by string_view without building a string).
using NameLabels = std::unordered_map<std::string, std::int64_t, NameHash, std::equal_to<>>;

}  // namespace cb::decompile
