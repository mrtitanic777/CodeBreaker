// codebreaker/build/asm16.hpp -- interface of codebreaker.build.asm16 (typed C++; implementation pending).
//
// asm16.py -- a 16-bit function the C lane cannot give as assembly that links into the rebuild.
//
//     Function f(nef, seg, start, end, public_, symbol_for);
//     f.text, f.imports, f.why          (f.text is None when it cannot be written so; f.why says why)
//     asm16::check(obj_bytes, f)        -> "" or what differs
//
// The 16-bit system modules were largely written in assembly; when the decompiler's C for a function does not compile,
// this writes it as its own instructions in a form that links into the rebuilt module wherever the linker puts it:
// every place the loader fixes is written as the symbol it refers to; a near call or jump leaving the function is a
// `call`/`jmp` to the target's symbol; everything else is the original's bytes.  A function that refers to its own
// segment by number is refused (f.text None).
//
// Typing notes:
//   * `symbol_for(kind, *what)` is `SymbolFor`: its call is a `Reference` record (kind "function" seg, off | "segment" seg |
//     "data" seg | "import" module, which [ordinal or name]); the answer is a symbol or None.
//   * `public` is a symbol, or [(offset, symbol)] for each function of a run: variant `Publics`.
//   * The Python Function carries the public record the build reads (text, imports, why, externs, missing, tables,
//     far_missing, reaches, raw_far_entries, raw_hole_calls, holes ...) and an `assembled` slot build.module sets after
//     assembling it (the why-text it was assembled with, or None).
//   * __file__ (hashed into build.module's cache stamp) is `module_file()`.
//   * Capstone is the implementation's concern: `ins` holds each instruction's (bytes, mnemonic, operands, label target).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::build::asm16 {

// the path of build/asm16.py (the Python's __file__)  -- used by: build.module
py::str module_file();

// an import is named by its ordinal (int) or its name (str)
using Which = std::variant<int64_t, py::str>;

// One call of symbol_for: kind is "function" (seg, off), "segment" (seg), "data" (seg: a data segment's first byte)
// or "import" (module, which).  Unused fields stay at their defaults.
struct Reference {
    py::str kind;
    int64_t seg = 0;
    int64_t off = 0;
    py::str module;
    Which which = int64_t(0);
};
using SymbolFor = std::function<std::optional<py::str>(const Reference&)>;

// `public`: the symbol to define, or [(offset, symbol)] for each function of a run
using Publics = std::variant<py::str, py::list<std::tuple<int64_t, py::str>>>;

class Function {
  public:
    // `seg` the segment number (1-based), [start, end) the function in it -- or the run of functions, written together as
    // their source file had them.  cs_ok: offsets of the segment below it may be read through CS (the lead-in).
    // extra_entries: other places of the run called into from outside.  whole: the run is a whole segment (any place may
    // be read through CS by its number).  holes: [(start, end, symbol)] of functions inside it rebuilt from C, which the
    // build places after it.
    Function(const formats::ne::NEFile& nef, int64_t seg, int64_t start, int64_t end, const Publics& public_,
             const SymbolFor& symbol_for, int64_t cs_ok = 0, const py::list<int64_t>& extra_entries = py::list<int64_t>(),
             bool whole = false, const py::list<std::tuple<int64_t, int64_t, py::str>>& holes =
                                     py::list<std::tuple<int64_t, int64_t, py::str>>());

    // --- the result
    std::optional<py::str> text;                          // the assembly, None when it cannot be written so
    py::dict<py::str, std::tuple<py::str, Which>> imports;   // symbol -> (module, ordinal or name)
    py::str why;                                          // why text is None
    py::dict<py::str, py::str> externs;                   // symbol -> "far" | "near" | "byte"
    py::set<int64_t> missing;                             // near-call targets without a symbol
    py::set<int64_t> tables;                              // jump-table places
    py::set<std::tuple<int64_t, int64_t>> far_missing;    // (segment, offset) a far call or jump reaches mid-run
    std::optional<int64_t> reaches;                       // refused for jumping into the middle of this offset's function
    py::list<std::tuple<int64_t, int64_t, py::str>> holes;   // sorted
    py::dict<int64_t, std::tuple<py::str, int64_t>> raw_hole_calls;   // offset -> (symbol, opcode)
    py::set<std::tuple<int64_t, int64_t>> raw_far_entries;   // (segment, offset) a raw far call or pushed pointer goes to
    py::list<std::tuple<int64_t, py::str>> publics;       // (offset, symbol), sorted
    py::dict<int64_t, int64_t> cs_labels;                 // offset of an instruction -> the place in the run it reads through CS
    // set by build.module after it assembled the function: the text it says (why) was assembled with, or None
    std::optional<py::str> assembled;

    // --- what the build and check read (set once the code is followed)
    int64_t bits = 16;                                    // 32 for a use32 segment (NE flag 0x2000)
    int64_t start = 0, end = 0;
    py::bytes blob;                                       // the segment's bytes as the loader has them
    py::dict<int64_t, int64_t> code_segments;             // segment -> bytes of code
    py::dict<int64_t, std::tuple<int64_t, py::str>> sites;      // offset -> (width, directive text)
    py::dict<int64_t, std::tuple<py::bytes, py::str, py::str, std::optional<int64_t>>> ins;   // offset -> (bytes, mnemonic, operands, label target)
    py::dict<int64_t, std::tuple<int64_t, py::str>> symbolic;   // offset -> (size, text): an instruction whose fixup has a name
    py::dict<int64_t, std::tuple<int64_t, py::str>> calls;      // offset -> (size, text): near calls/jumps leaving the run
    py::set<int64_t> labels;
    py::str module;
    int64_t seg = 0;
    py::str public_;                                      // the first public symbol

    // the mnemonic text of the instruction at `at`
    py::str mnemonic(int64_t at) const;
    // {offset: (text, bytes, mask)} of the instructions the assembler's spelling is probed for
    py::dict<int64_t, std::tuple<py::str, py::bytes, py::bytes>> candidates() const;
    // {name: offset} of each function rebuilt from C (a hole) that a near call or jump in the part of the frame NOT followed as
    // code -- written out as its bytes -- goes to (also fills raw_hole_calls)
    py::dict<py::str, int64_t> raw_calls_into_holes();

    // The source, with the instructions in `good` (offsets) as mnemonics and the rest as bytes (None: every instruction).
    // With lo, hi: only that piece of it.
    py::str write(const std::optional<py::set<int64_t>>& good, const std::optional<int64_t>& lo = std::nullopt,
                  const std::optional<int64_t>& hi = std::nullopt);
};

// A piece of a frame (see Function::write) against the original. -> "" or what differs.
py::str check_piece(const py::bytes& obj, Function& f, int64_t lo, int64_t hi);

// The assembled object against the original: every byte the same but those the loader or the linker sets. -> "" or where.
py::str check(const py::bytes& obj, const Function& f);

// The offsets (in the segment) where the assembled bytes are not the original's, the places the loader or the linker sets aside.
py::list<int64_t> differences(const py::bytes& code, const py::bytes& mask, const Function& f);

}  // namespace codebreaker::build::asm16
