// cb/source/ne_asm/recon.hpp -- hand-written assembly reconstructed deterministically, byte for byte.
//
// The 16-bit core (KERNEL, USER, GDI, the .drv and .386 drivers) was written in assembly. The faithful reconstruction is assembly that
// re-assembles to the shipped bytes, and that can be made mechanically, with no search:
//
//   1. the carved function, disassembled (segment::Seg),
//   2. each instruction transcribed to MASM; a branch inside the function becomes a label (its self-relative displacement does not depend on
//      the place, so it reproduces exactly); data, outside calls and far pointers become `db`,
//   3. assembled, and compared with the shipped bytes (segdiff); any instruction the assembler does not encode identically is written as `db`
//      of its original bytes, and the source is assembled again.
//
// The db fallback is a guarantee: the loop ends byte-exact, in the worst case all db. What is measured is the share that stays live, real
// instructions rather than db. Printable data is written readably (`db 'text'`), and is not counted as instructions.
//
//     codebreaker ne-asm recon MODULE [--seg N] [--func 0xOFF] [--out FILE]
//     codebreaker ne-asm recon --selftest
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/bytes.hpp"
#include "cb/source/ne_asm/datacode.hpp"
#include "cb/source/ne_asm/fixups.hpp"
#include "cb/source/ne_asm/segdiff.hpp"
#include "cb/source/ne_asm/segment.hpp"

namespace cb::source::ne_asm::recon {

// What an assembler gave for a source: the object file when it made one, MASM's listing and its messages.
struct Assembled {
    std::optional<Bytes> object;
    std::string listing, log;
};
// Assemble one source text (16-bit OMF).
using Assemble = std::function<Assembled(std::string_view source)>;

// ---- MASM text ---------------------------------------------------------------------------------------------------------------

// A number as MASM writes it: 1fh, 0abh (a number that starts with a letter takes a leading zero).
std::string masm_hex(std::uint32_t value);

// The label a branch target has, if it has one.
using LabelOf = std::function<std::optional<std::string>(std::uint32_t offset)>;

// One instruction as objdump (Intel syntax) wrote it, in MASM's syntax: string instructions get their size suffix, a direct branch to a labelled
// offset goes to the label, `WORD PTR` is lower case and numbers are MASM's.
std::string to_masm(std::string_view instruction, const LabelOf& label_of);

// Append `db` lines for `raw` (at segment offset `off`, with `note` on the first), sixteen bytes to a line so that no line passes MASM's limit:
// objdump folds a run of equal bytes into one row that can be a hundred bytes long.
void emit_db(std::vector<std::string>& lines, ByteView raw, std::uint32_t off, std::string_view note);

// Append a DATA region as readable data: printable text as `db 'text'` (at most 40 characters a line: MASM 4.00 refuses a longer source line, and
// one such line would lose the whole module), the rest as db hex.
void emit_data(std::vector<std::string>& lines, ByteView code, std::uint32_t start, std::uint32_t end, datacode::Kind kind);

// A safe data region: [start, end) of the segment, row-aligned, with no branch target inside.
struct DataSpan {
    std::uint32_t start = 0, end = 0;
    datacode::Kind kind = datacode::Kind::Data;
};

// The MASM body of the function [start, end): the rows in `force_db`, branches that leave the function, far calls and jumps (absolute
// segment:offset pointers), rows objdump could not decode and rows longer than any instruction as db; `data_spans` as readable data.
std::vector<std::string> emit_function(const segment::Seg& seg, std::uint32_t start, std::uint32_t end, const std::set<std::uint32_t>& force_db,
                                       std::span<const DataSpan> data_spans = {});

// The body wrapped as a complete source: the retf macro, `_TEXT segment`, and the end.
std::string wrap(std::span<const std::string> body);

// ---- finding what the assembler changed ----------------------------------------------------------------------------------------

// The offsets (of the original) of the instructions MASM wrote another way, found from its listing: each instruction's place in the listing
// against its place in the original and the length between them, which a later difference of length does not disturb, so one pass finds every one.
struct Divergers {
    std::set<std::uint32_t> length;     // another length: they shift what follows, and are fixed first
    std::set<std::uint32_t> re_encode;  // the same length, other bytes (not counting what a relocation or fix-up covers)
};
// `spans`: the original instruction offset -> where the next begins. `our` and `shipped` are the assembled and the shipped bytes (the latter from
// `start`), each with the offsets a loader fills in.
Divergers divergers(std::string_view listing, ByteView our, const segdiff::Mask& our_mask, ByteView shipped, const segdiff::Mask& shipped_mask,
                    const std::map<std::uint32_t, std::uint32_t>& spans, std::uint32_t start);

// Where each instruction of a listing is in the assembled bytes and in the original (the `; NNNN` comment each source line carries): the pairs a
// listing of any assembler gives, from which the divergers are found.
using ListedPair = std::pair<std::uint32_t, std::optional<std::uint32_t>>;
// MASM's listing: a line with an offset and a byte field, the comment from it or from the line it wraps onto.
std::vector<ListedPair> masm_listing_pairs(std::string_view listing);
// nasm's listing: `<line> <offset, 8 digits> <bytes> <source ; NNNN>`; lines its macros expanded carry no offset.
std::vector<ListedPair> nasm_listing_pairs(std::string_view listing);
Divergers divergers_of(std::span<const ListedPair> pairs, ByteView our, const segdiff::Mask& our_mask, ByteView shipped, const segdiff::Mask& shipped_mask,
                       const std::map<std::uint32_t, std::uint32_t>& spans, std::uint32_t start);
// The source lines the assembler's messages name as in error: `(12): error` (MASM), `file:12: error` (nasm).
std::vector<std::size_t> masm_error_lines(std::string_view messages);
std::vector<std::size_t> nasm_error_lines(std::string_view messages);
// The offset in the `; NNNN` comment (lower-case hex, `digits` long) of a source line.
std::optional<std::uint32_t> comment_offset(std::string_view line, std::size_t digits = 4);

// The data regions of [start, end) that are safe (reachability): snapped to row boundaries, then checked again so that snapping never takes a branch
// target in. Writing a safe region as data drops no label.
std::vector<DataSpan> safe_data_spans(const segment::Seg& seg, std::uint32_t start, std::uint32_t end);

// ---- the loop ----------------------------------------------------------------------------------------------------------------

struct Reconciled {
    std::string source;
    std::optional<segdiff::Verdict> verdict;   // none when nothing assembled
    std::set<std::uint32_t> force_db;          // the instructions written as db
    std::size_t instructions = 0;              // the code instructions, not counting what is data
    bool exact() const { return verdict && verdict->exact; }
};

// Reconstruct [start, end) of the segment to byte-exact by the db fallback. MASM's messages name the instruction they refuse (by the `; NNNN`
// offset each line carries) and that instruction is db -- it is data MASM cannot encode, and is left out of the live count. Then, in two phases:
// the instructions of another length first (they restore the layout, so a neighbouring near jump comes out right and stays live), then the
// same-length re-encodings. Converges in a few passes with nearly the least db. `seg_mask`: the offsets of the shipped segment a loader fills in.
Reconciled reconcile(const segment::Seg& seg, std::uint32_t start, std::uint32_t end, const segdiff::Mask& seg_mask, const Assemble& assemble,
                     std::size_t max_passes = 200);

// ---- the command -------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;
    Assemble assemble;                                                       // MASM
    fixups::Objdump objdump = fixups::run_objdump;
};
// `MODULE [--seg N] [--func 0xOFF] [--out FILE]` or `--selftest`: reconstruct one function (the smallest of the segment when none is named) and
// say how much of it stayed live. Returns 0 when it came out byte-exact.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::ne_asm::recon
