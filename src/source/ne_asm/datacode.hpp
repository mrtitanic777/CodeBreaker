// source/ne_asm/datacode.hpp -- separate DATA from CODE in a segment.
//
// Code segments carry embedded data -- strings, jump tables, constant blobs -- which a linear disassembler (objdump) blindly
// decodes as bogus instructions (`outs dx,...`, `arpl`, `imul bp,[di+0x61],0x7972`...). Those show up as `db` byte-dumps and
// drag down the "live %" even though they were never instructions. This classifier finds the data regions, so the assembly
// reconstruction can write them as readable data (`db 'primary',0`, `dw L_case1,...`) and leave them out of the instruction
// count. Two detectors:
//
//   * strings  -- runs of at least kMinString printable ASCII bytes that read as text (optionally NUL-terminated)
//   * garbage  -- clusters of the mnemonics objdump emits only over data (outs/ins/arpl/segment-prefix soup)
//
//     codebreaker ne-asm datacode MODULE [--seg N]       # the data regions found
#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/corpus.hpp"
#include "core/bytes.hpp"
#include "source/ne_asm/fixups.hpp"
#include "source/ne_asm/segment.hpp"

namespace cb::source::ne_asm::datacode {

// The shortest printable run that counts as a string.
inline constexpr std::size_t kMinString = 5;

// A run of printable text: [start, end) of the segment (a NUL that ends it is included) and the characters, NUL left out.
struct TextRun {
    std::uint32_t start = 0, end = 0;
    std::string text;
};

// What a data region is. The order is the tie-break when two regions start and end together: Data comes first.
enum class Kind { Data, String };

// A region [start, end) of the segment that is data.
struct Region {
    std::uint32_t start = 0, end = 0;
    Kind kind = Kind::Data;
};

// "str" or "data": how the report names a kind.
std::string_view kind_name(Kind kind);

// Printable-ASCII runs of at least `min_length` bytes that look like text. A run looks like text when it holds a lowercase
// letter: real strings nearly always do, while register push/pop runs (SQRVW) and arithmetic tables ($(,048<@) do not.
std::vector<TextRun> find_strings(ByteView code, std::size_t min_length = kMinString);

// [start, end) over every run of three or more consecutive rows whose instruction is the kind objdump produces only over
// data: a mnemonic of the garbage set, a segment prefix with an operand, "(bad)", or a 32-bit register (16-bit modules never
// use eax/edi/...: it is a 0x66/0x67 prefix byte decoded as one). A run ends where the next row starts.
std::vector<Region> garbage_spans(std::span<const segment::Row> rows);

// The string and garbage detectors merged into sorted, non-overlapping regions (overlapping or touching ones join, the first
// one's kind is kept).
std::vector<Region> data_regions(ByteView code, std::span<const segment::Row> rows);

// The report of `--seg`: a summary line, then up to 40 regions with a 24-byte preview of each.
std::string report(std::string_view module_name, std::size_t segment, ByteView code, std::span<const Region> regions);

// `datacode MODULE [--seg N] [--ords FILE]`. The module is a corpus name or a path. Returns the exit code.
int main(const base::Corpus& corpus, std::span<const std::string> args, const fixups::Objdump& objdump = fixups::run_objdump);

}  // namespace cb::source::ne_asm::datacode
