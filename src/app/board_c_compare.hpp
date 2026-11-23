// app/board_c_compare.hpp -- one function side by side: the C the conversion wrote, and the original's assembly from the conversion's listing.
// The bytes the period compiler made from that C are not kept, so this is for reading, not a byte diff. A listing labels code by address: sub_1_0024 is s1_L24 in a
// 16-bit module and sub_00BA30 is L40ba30 at image base 0x400000. A function known only by name has no label to find, and the panel says so.
// No operating-system API (board_c_compare_test.cpp).
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"
#include "app/board_scores.hpp"
#include "app/source_preview.hpp"

namespace cb::app::board {

inline constexpr std::size_t kCCompareMostLines = 1500;

struct CCodeSlice {
    std::vector<std::string> lines;    // tabs as four spaces
    bool cut = false;                  // longer than kCCompareMostLines
};

// The definition of `name` in the C, its signature to its closing brace. Empty: not there.
CCodeSlice c_function_source(std::string_view c_text, std::string_view name);
// The listing's label for a numbered function; nullopt for one known only by name.
std::optional<std::string> c_function_label(std::string_view name, std::uint64_t image_base);
// "IMAGE equ 0x400000" (0 when the listing has none).
std::uint64_t c_listing_base(std::string_view asm_text);
// The labels of every numbered function defined in the C: where a function's assembly stops.
std::set<std::string, std::less<>> c_function_labels(std::string_view c_text, std::uint64_t image_base);
// The listing from `label` to the next of `ends` or the end of its section. Empty: no such label.
CCodeSlice c_function_assembly(std::string_view asm_text, std::string_view label, const std::set<std::string, std::less<>>& ends);

struct CFunctionCompare {
    std::string program, function, measure;
    std::vector<std::string> source, assembly;
    std::vector<std::vector<preview::Span>> source_spans, assembly_spans;
    std::string source_note, assembly_note;   // why a side is empty, or that it was cut
};
// What the conversion of this card (or a copy of it) left for `function`. `measure`: the words of its measured result.
CFunctionCompare load_c_compare(const Board& board, const Index& index, std::uint32_t card, const CScores* scores, std::string_view function, std::string_view measure);

}  // namespace cb::app::board
