// codebreaker/source/ne_asm/segment.hpp -- typed C++ of codebreaker.source.ne_asm.segment.
//
// trace.py -- Call-graph-tracing disassembler for the Windows 1.0 RE project.
//
// Where disasm.py dumps one segment and wndproc.py decodes one dispatch table,
// trace.py automates the tedious part of reconstruction: given an exported entry,
// it discovers *every* function reachable from it, carves each function's
// boundaries, disassembles it, and resolves *every* call to a real name --
// KERNEL/GDI/USER ordinals via ORDINALS.md, internal calls to their function
// label. One command replaces the dozens of hand-run disasm.py invocations and
// the manual call-graph chase that Level-2 reconstruction otherwise needs.
//
// Function discovery combines four signals: exported entry points, internal
// relocation targets (segN:0xNNNN fixups), intra-segment near-call targets, and
// `push bp`/`mov bp,sp` prologue scanning. Boundaries run start-to-next-start,
// trimming trailing padding.
//
// Usage:
//     python3 trace.py FILE --map                 # exports, segments, function inventory
//     python3 trace.py FILE --list [seg]          # every discovered function in a segment
//     python3 trace.py FILE --fn 0xADDR [seg]     # one function, annotated
//     python3 trace.py FILE --trace ORD           # an export + its whole reachable call graph
//     python3 trace.py FILE --trace 0xADDR [seg]  # same, from a raw address
// Add --ords PATH to point at a non-default ORDINALS.md.
//
// Typing notes:
//   * Seg keeps a reference to the NEFile it was made from (the Python keeps the object): the NEFile must
//     outlive the Seg.
//   * resolve_call's (kind, detail) pair, whose detail is a (label, name|None) pair, an int or a str by kind, is
//     the Resolved record; calls_in's kinds are the CallKind enum.
//   * The Python's `if __name__ == "__main__"` block is main(argv) here (argv is sys.argv[1:], the file first); the
//     return is the exit code: 0, or the 1 of the Python's sys.exit(1). The Python's stdout.reconfigure(utf-8)
//     is not needed: the runtime's print writes UTF-8.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::segment {

// Window message names (mirrors wndproc.py's table).
extern const py::dict<int64_t, py::str> WM;
// DOS INT 21h function names, for annotating the KERNEL wrappers' actual syscalls.
extern const py::dict<int64_t, py::str> DOS21;

// Parse ORDINALS.md tables into {'USER.39': 'BeginPaint', ...}.
//
// Retargeted for Windows 3.1: the module name is matched generically rather
// than against a fixed list. Windows 1.0 had nine system modules and they
// could be enumerated; 3.1 has 254 NE modules whose names are neither fixed
// nor unique, so anything hardcoded here would silently drop exports.
//
// (The Windows 1.0 ORDINALS_SDK.md merge is gone with it. In 3.1 the names
// come from the binaries' own export tables -- see genordinals.py -- so there
// is no second, more-authoritative source to layer underneath.)
py::dict<py::str, py::str> load_ordinals(const py::str& path);

// objdump the whole blob as 16-bit code; return [(offset, asm_text), ...].
// Uses a unique temp file so parallel invocations never collide.
py::list<std::tuple<int64_t, py::str>> disasm_whole(const py::bytes& code, int64_t base = 0);

// a function the carve found: [start, end) and its name
struct Func {
    int64_t start = 0, end = 0;
    py::str name;
};

// what a call/jmp resolves to: Sys (label, name or None: an imported routine), Int (target: a function of this segment),
// Far (label: another segment of the module), NotACall (not a call to anything known)
enum class CallKind { NotACall, Sys, Int, Far };
struct Resolved {
    CallKind kind = CallKind::NotACall;
    py::str label;                    // Sys, Far
    std::optional<py::str> name;      // Sys: the ordinal's name when known
    int64_t target = 0;               // Int
};

// One CODE segment: disassembled, with functions discovered and calls resolved.
class Seg {
  public:
    const formats::ne::NEFile& ne;
    int64_t si;
    formats::ne::Segment seg;
    py::dict<py::str, py::str> ordinals;
    // {offset:int -> real internal name}. Recovered exactly-matching names for
    // functions the export table leaves anonymous (see names.json produced by
    // the debug-symbol signature matcher). A pure label rename: it changes the
    // cProc name and every symbolic call/branch that resolves to it alike, so
    // the emitted bytes are unchanged. Export names still win at their offset.
    py::dict<int64_t, py::str> name_override;
    py::list<py::str> mods;
    py::dict<int64_t, py::str> fixups;   // site_off -> "MODULE.ord"/"segN:0xNNNN"
    py::bytes code;
    py::list<std::tuple<int64_t, py::str>> rows;
    py::dict<int64_t, int64_t> off2idx;
    py::set<int64_t> valid;
    py::dict<int64_t, py::str> export_names;
    py::set<int64_t> starts;
    py::dict<int64_t, Func> funcs;       // start_off -> Func

    Seg(const formats::ne::NEFile& ne, int64_t seg_index, const py::dict<py::str, py::str>& ordinals,
        const std::optional<py::dict<int64_t, py::str>>& name_override = std::nullopt);

    // -- names
    py::dict<int64_t, py::str> _ord2name() const;
    py::dict<int64_t, py::str> _export_names() const;

    // -- function discovery
    py::set<int64_t> _find_starts() const;
    py::dict<int64_t, Func> _carve() const;
    py::str name_at(int64_t off) const;

    // -- call resolution
    // Return what the call/jmp at `off` resolves to.
    Resolved resolve_call(int64_t off, const py::str& asm_) const;
    std::tuple<Func, py::list<std::tuple<int64_t, py::str>>> func_rows(int64_t start) const;
    // Distinct resolved calls made by a function, in order.
    py::list<std::tuple<CallKind, py::str>> calls_in(int64_t start) const;
    // Positive [bp+N] frame slots a function reads = its incoming PASCAL args.
    py::list<int64_t> args_used(int64_t start) const;
    // Read a little-endian WORD from the segment image, or None if OOB.
    std::optional<int64_t> word_at(int64_t off) const;

    // -- WndProc dispatch decode
    // If `start` is a WndProc, return [(msg, handler_off), ...]; else [].
    // Handles both the compare-chain form and the jump-table form
    // (`sub ax,BASE; cmp ax,N; jbe; add ax,ax; jmp WORD cs:[bx+TAB]`).
    py::list<std::tuple<int64_t, int64_t>> dispatch(int64_t start) const;
    // Resolved calls a single dispatch handler makes: walk from handler_off
    // until the next handler / a ret / a jump out to the shared epilogue.
    py::list<py::str> handler_calls(int64_t start, int64_t handler_off, const py::list<int64_t>& handler_starts) const;
    // Detect the `mov bx,IMM; push bx; jmp TARGET` MOVEABLE-entry thunk;
    // return TARGET (the shared dispatcher) or None.
    std::optional<int64_t> is_thunk(int64_t start) const;
};

// (segment, offset) of the entry with this ordinal, or None
std::optional<std::tuple<int64_t, int64_t>> find_seg_of_entry(const formats::ne::NEFile& ne, int64_t ordinal);

// ---- annotated printing
py::str annotate_row(const Seg& sg, int64_t off, const py::str& asm_);
void print_function(const Seg& sg, int64_t start, bool body = true);
// BFS over internal calls from `start`; return reachable func starts in discovery order.
py::list<int64_t> trace_callgraph(const Seg& sg, int64_t start);

// ---- commands
void cmd_map(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals);
void cmd_list(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index);
void cmd_fn(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t addr);
// Emit a ready-to-refine C skeleton for a window/dialog procedure: the
// dispatch switch (each case annotated with its handler offset and resolved
// calls) plus documented-partial templates for every reachable internal fn.
// Automates the mechanical part of Level-2 reconstruction; the writer fills
// logic and verifies. Never fabricates names (unresolved ordinals stay raw).
void cmd_cskel(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start);
// Scan code segments for real window procedures (functions that load the
// `message` arg and dispatch >=2 messages), decoding each dispatch table.
// Finds bodies even when the export entry is a MOVEABLE thunk.
void cmd_wndprocs(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, std::optional<int64_t> only_seg = std::nullopt);
void cmd_trace(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start);

// the script's command line: argv is sys.argv[1:]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::ne_asm::segment
