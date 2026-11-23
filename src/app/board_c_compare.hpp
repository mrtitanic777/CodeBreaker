// app/board_c_compare.hpp -- one function side by side: the C the conversion wrote, and the original's code.
// The bytes the period compiler made from that C are not kept, so this is for reading, not a byte diff. The original's side is disassembled from the file itself: a
// PE function at its address (from its name, or its export) for the length the scorer measured; an NE function at its segment and offset (from its name, or the
// entry table) up to the next function the scorer knows. Where that cannot be done it falls back on the conversion's listing, which labels only code something
// jumps to: sub_1_0024 is s1_L24 in a 16-bit module and sub_00BA30 is L40ba30 at image base 0x400000.
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
#include "core/bytes.hpp"

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

// `code` as lines of "00401000  mov eax, 1"; a byte that decodes to nothing is a `db`. `bits`: 16 or 32. Throws when the disassembler cannot be loaded.
CCodeSlice c_disassemble(ByteView code, std::uint64_t address, int bits);
// Where a function starting at `start` ends: the next of `starts` after it, else `limit`.
std::uint64_t c_function_end(const std::vector<std::uint64_t>& starts, std::uint64_t start, std::uint64_t limit);
// The original's code of `function`, from the program's bytes and its results file (the text of c_scores/<hash>.json). nullopt when it cannot be placed.
std::optional<CCodeSlice> c_original_code(ByteView file, std::string_view format, std::string_view function, std::string_view results);

struct CFunctionCompare {
    std::string program, function, measure;
    std::vector<std::string> source, assembly;
    std::vector<std::vector<preview::Span>> source_spans, assembly_spans;
    std::string source_note, assembly_note;   // why a side is empty, or that it was cut
};
// What the conversion of this card (or a copy of it) left for `function`. `measure`: the words of its measured result.
CFunctionCompare load_c_compare(const Board& board, const Index& index, std::uint32_t card, const CScores* scores, std::string_view function, std::string_view measure);

}  // namespace cb::app::board
