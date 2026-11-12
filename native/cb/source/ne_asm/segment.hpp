// cb/source/ne_asm/segment.hpp -- a call-graph-tracing disassembler for the code segments of an NE module.
//
// Where fixups.hpp lists one segment, this automates the tedious part of reconstruction: given an exported entry it
// discovers every function reachable from it, carves each function's boundaries, disassembles it, and resolves every
// call to a real name -- KERNEL/GDI/USER ordinals through ORDINALS.md, internal calls to their function label.
//
// Function discovery combines the signals a segment gives: exported entry points, internal relocation targets (segN:0xNNNN
// fix-ups), intra-segment near-call targets, the `push bp` / `mov bp,sp` and `enter N,0` prologues, and the shared
// routine that two or more short thunks tail-jump to. A function runs from its start to the next start, trailing padding
// trimmed.
//
//     codebreaker ne-asm segment FILE --map                  exports, segments, function inventory
//     codebreaker ne-asm segment FILE --list [SEG]           every discovered function of a segment
//     codebreaker ne-asm segment FILE --fn 0xADDR [SEG]      one function, annotated
//     codebreaker ne-asm segment FILE --trace ORD            an export and its whole reachable call graph
//     codebreaker ne-asm segment FILE --trace 0xADDR [SEG]   the same, from an address
//     codebreaker ne-asm segment FILE --cskel 0xADDR [SEG]   a C skeleton of a window or dialog procedure
//     codebreaker ne-asm segment FILE --wndprocs [SEG]       the window procedures of the code segments
//   with --ords PATH to name the ordinals from another ORDINALS.md.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ne.hpp"
#include "cb/source/ne_asm/fixups.hpp"

namespace cb::source::ne_asm::segment {

// ---- names -----------------------------------------------------------------------------------------------------------------

// "USER.39" -> "BeginPaint": what ORDINALS.md calls the routines of the system modules.
using Ordinals = std::unordered_map<std::string, std::string>;
using SharedOrdinals = std::shared_ptr<const Ordinals>;

// The table rows of ORDINALS.md (`| MODULE.N | name |`); the module name is taken as it comes, not from a fixed list, so no
// export of a Windows 3.1 module (254 of them, none with a fixed name) is dropped. A row met twice keeps the later name.
Ordinals parse_ordinals(std::string_view markdown);
// The same from the file; none when the file does not exist.
Ordinals load_ordinals(const std::filesystem::path& file);

// The name of a window message ("WM_PAINT") or nullptr; of a DOS INT 21h function ("OPEN") or nullptr.
const char* window_message_name(std::uint32_t message);
const char* dos_function_name(std::uint32_t function);

// ---- disassembly -----------------------------------------------------------------------------------------------------------

// One instruction of the listing: its offset in the segment and its assembly as objdump writes it.
struct Row {
    std::uint32_t offset = 0;
    std::string text;
};

// objdump's listing of `code` as 16-bit code, one Row per line that has an address. `base` is the address of the first byte.
std::vector<Row> disassemble(ByteView code, std::uint32_t base = 0, const fixups::Objdump& objdump = fixups::run_objdump);

// A function the carve found: [start, end) and its name.
struct Func {
    std::uint32_t start = 0, end = 0;
    std::string name;
};

// What a call or jump resolves to.
enum class CallKind {
    System,     // an imported routine: `label` ("KERNEL.91"), and its `name` when ORDINALS.md has one
    Internal,   // a function of this segment: `target`
    Far,        // another segment of the module: `label` ("seg4:0x0010")
};
struct Call {
    CallKind kind = CallKind::System;
    std::string label;
    std::optional<std::string> name;
    std::uint32_t target = 0;
};

// A function's rows: a view into the segment's listing, trailing padding left out.
struct FunctionRows {
    Func func;
    std::span<const Row> rows;
};

// One message of a window procedure's dispatch and the offset of the code that handles it.
struct Dispatch {
    std::uint32_t message = 0, handler = 0;
};

using NameOverrides = std::map<std::uint32_t, std::string>;

// One code segment: disassembled, with its functions discovered and its calls resolved.
class Seg {
public:
    // Segment `index` (from 1) of `module`. `overrides` are real names for functions the export table leaves anonymous (a
    // recovered symbol): a pure label rename, so the code is unchanged; an export name still wins at its offset.
    // UsageError when the module has no such segment; FormatError when it lies outside the file.
    Seg(const formats::ne::Module& module, std::size_t index, SharedOrdinals ordinals, NameOverrides overrides = {},
        const fixups::Objdump& objdump = fixups::run_objdump);

    std::size_t index() const { return index_; }
    const formats::ne::Segment& info() const { return info_; }
    ByteView code() const { return code_; }
    const std::vector<Row>& rows() const { return rows_; }
    // Where an instruction starts: whether `offset` is the start of a row.
    bool is_instruction(std::uint32_t offset) const { return row_at_.contains(offset); }
    const fixups::FixupMap& fixups() const { return fixups_; }
    const std::vector<std::string>& modules() const { return modules_; }
    // Exported entries that land in this segment: offset -> name.
    const std::map<std::uint32_t, std::string>& export_names() const { return export_names_; }
    const std::set<std::uint32_t>& starts() const { return starts_; }
    // Start -> function, in address order.
    const std::map<std::uint32_t, Func>& functions() const { return functions_; }

    // The function's name: an export, else an override, else sub_XXXX.
    std::string name_at(std::uint32_t offset) const;

    // What the call or jump at `offset` (whose text is `instruction`) goes to: a fix-up anywhere in its bytes names the
    // target, else an intra-segment near call; nullopt when it resolves to nothing.
    std::optional<Call> resolve_call(std::uint32_t offset, std::string_view instruction) const;
    // The function that starts at `start` (a start the carve did not find gets a span to the next one) and its rows.
    FunctionRows function_rows(std::uint32_t start) const;
    // The distinct calls a function makes, in order of first appearance. A jump is a call only when it leaves the function.
    std::vector<Call> calls_in(std::uint32_t start) const;
    // The positive [bp+N] frame slots a function reads: its incoming PASCAL arguments (N >= 4), ascending.
    std::vector<std::uint32_t> args_used(std::uint32_t start) const;
    // A little-endian word of the segment, nullopt outside it.
    std::optional<std::uint32_t> word_at(std::uint32_t offset) const;

    // If `start` is a window procedure, its messages and handlers in order of discovery; else none. Both forms are read:
    // the compare chain, and the jump table (`sub ax,BASE; cmp ax,N; jbe; add ax,ax; jmp WORD cs:[bx+TAB]`).
    std::vector<Dispatch> dispatch(std::uint32_t start) const;
    // The calls one dispatch handler makes: from `handler` to the next handler, a ret, or a jump out to the shared epilogue.
    std::vector<std::string> handler_calls(std::uint32_t start, std::uint32_t handler, const std::vector<std::uint32_t>& handler_starts) const;
    // The shared dispatcher when `start` is the `mov bx,IMM; push bx; jmp TARGET` thunk of a MOVEABLE entry.
    std::optional<std::uint32_t> is_thunk(std::uint32_t start) const;

private:
    void find_starts();
    void carve();

    std::size_t index_ = 0;
    formats::ne::Segment info_;
    SharedOrdinals ordinals_;
    NameOverrides overrides_;
    std::vector<std::string> modules_;
    fixups::FixupMap fixups_;
    Bytes code_;
    std::vector<Row> rows_;
    std::unordered_map<std::uint32_t, std::size_t> row_at_;   // offset -> index in rows_; lookup only
    std::map<std::uint32_t, std::string> export_names_;
    std::set<std::uint32_t> starts_;
    std::map<std::uint32_t, Func> functions_;
};

// (segment, offset) of the entry with this ordinal, nullopt when there is none.
std::optional<std::pair<std::uint32_t, std::uint32_t>> find_entry(const formats::ne::Module& module, std::uint32_t ordinal);

// ---- the annotated output -------------------------------------------------------------------------------------------------

// A call as the reports write it: `KERNEL.91 = GetVersion` (`  [unidentified]` for an ordinal ORDINALS.md does not know),
// `-> sub_1234 (seg1:0x1234)`, `-> seg4:0x0010 (other segment)`.
std::string describe_call(const Seg& seg, const Call& call);
// A row with the calls it makes named (`; KERNEL.91 = GetVersion`, `; -> sub_1234`) and DOS calls flagged.
std::string annotate_row(const Seg& seg, const Row& row);
// A function: name, arguments, the window procedure's dispatch, the calls it makes, and its annotated rows with `body`.
std::string function_report(const Seg& seg, std::uint32_t start, bool body = true);
// The functions reachable from `start` through internal calls, `start` first, in breadth-first order.
std::vector<std::uint32_t> call_graph(const Seg& seg, std::uint32_t start);

// ---- the reports ------------------------------------------------------------------------------------------------------------

// Exports, segments and the function inventory of every code segment.
std::string map_report(const formats::ne::Module& module, const SharedOrdinals& ordinals, const fixups::Objdump& objdump = fixups::run_objdump);
std::string list_report(const formats::ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, const fixups::Objdump& objdump = fixups::run_objdump);
std::string function_listing(const formats::ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t address,
                             const fixups::Objdump& objdump = fixups::run_objdump);
// A C skeleton for a window or dialog procedure: the dispatch switch (each case with its handler offset and resolved calls)
// and a documented-partial template for every reachable internal function. Names are never invented: unresolved ordinals
// stay raw.
std::string skeleton(const formats::ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t start,
                     const fixups::Objdump& objdump = fixups::run_objdump);
// The window procedures of the code segments (all of them, or `only_segment`): functions that dispatch two or more messages,
// found even where the export entry is a MOVEABLE thunk.
std::string wndprocs_report(const formats::ne::Module& module, const SharedOrdinals& ordinals, std::optional<std::size_t> only_segment = std::nullopt,
                            const fixups::Objdump& objdump = fixups::run_objdump);
// The call graph from `start`: the external API surface it reaches, then each function.
std::string trace_report(const formats::ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t start,
                         const fixups::Objdump& objdump = fixups::run_objdump);

// ---- the command ------------------------------------------------------------------------------------------------------------

// The arguments of the usage above; the report goes to stdout. Returns the exit code (1 when a traced ordinal is not exported).
int main(std::span<const std::string> args, const fixups::Objdump& objdump = fixups::run_objdump);

}  // namespace cb::source::ne_asm::segment
