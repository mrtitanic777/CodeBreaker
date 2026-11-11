// cb/recompile/switches.hpp -- the switch tables of an Xbox 360 image, for XenonRecomp.
//
// XenonAnalyse matches fixed instruction sequences, which miss compilers whose instruction order differs or whose
// linker interleaves nops. This scanner walks back from every `bctr`, evaluates CTR symbolically and classifies the
// table by the expression's shape:
//
//     absolute : ctr = load32(T + (idx << 2))
//     byte     : ctr = B + (load8(T + idx) << s)            (s may be 0)
//     short    : ctr = B + (load16(T + (idx << 1)) << s)
//
// `idx` is the register the guarding `cmplwi crN, rI, max` + `bgt/ble crN` checks (or, with no check, the table's
// length comes from its entries). Every label is checked against the function bounds .pdata gives. Every
// executable section is scanned.
//
// The module reads a CodeImage -- the bytes of the code and the function table of a PowerPC PE image. Building one
// from an .xex belongs to the formats modules (powerpc_image, xex); this module has no file format in it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::recompile::switches {

// A function's bounds, [begin, end).
struct Extent {
    std::uint32_t begin = 0;
    std::uint32_t end = 0;
    friend constexpr bool operator==(const Extent&, const Extent&) = default;
};

// What the scanner needs of an image: its code, and the functions the exception table (.pdata) lists.
struct CodeImage {
    std::uint32_t base = 0;                // the address of data[0]
    Bytes data;                            // the image from `base` through the end of its code
    std::uint32_t code_begin = 0;          // the code: every executable section, [code_begin, code_end)
    std::uint32_t code_end = 0;
    std::vector<Extent> functions;         // sorted by begin
};

// The kinds the scan finds, and the one for a table that came from XenonAnalyse's own output instead (the project
// code adds those for the bctrs this scan left unresolved; the scan itself never makes one).
enum class TableKind { Absolute, Byte, Short, XenonAnalyse };
std::string_view kind_name(TableKind kind);   // "absolute", "byte", "short", "XenonAnalyse"

// One switch table found.
struct SwitchTable {
    std::uint32_t bctr = 0;                    // the address of the indirect jump
    std::uint32_t index_reg = 0;               // the register holding the case index
    TableKind kind = TableKind::Absolute;
    std::vector<std::uint32_t> labels;         // the case targets, in table order
    std::uint32_t default_target = 0;          // 0: the check names none
    std::optional<std::uint32_t> guard;        // address of the cmplwi; nullopt for a table with no bounds check
    std::uint32_t table = 0;                   // unguarded tables only: where the table is
    bool inline_table = false;                 // unguarded tables only: the data follows the bctr
    std::optional<Extent> function;            // the .pdata function that holds the bctr
};

struct Unresolved {                            // a bctr whose table the scanner could not follow
    std::uint32_t bctr = 0;
    std::string why;
};
struct Outside {                               // a table with a label outside its own function
    std::uint32_t bctr = 0;
    Extent function;
};

struct Report {
    std::size_t tables = 0;
    std::map<TableKind, std::size_t> kinds;    // how many of each kind (kinds in order of the enum)
    std::vector<Unresolved> unresolved;
    std::vector<Outside> outside;
    std::vector<std::uint32_t> pointer_calls;  // a bctr with an indexed load before it and no table found
};

struct Found {
    std::vector<SwitchTable> tables;           // in address order
    Report report;
};

// Every `bctr` of the image's code.
Found find(const CodeImage& image);

// XenonRecomp's switch-table file, as text and written to a file.
std::string format_file(std::span<const SwitchTable> tables);
void write(const std::filesystem::path& path, std::span<const SwitchTable> tables);

}  // namespace cb::recompile::switches
