// codebreaker/analyze/ppc_calls.hpp -- typed C++ port of codebreaker.analyze.ppc_calls.
//
// ppc_calls.py -- what a PowerPC program's calls show about its functions, from the binary alone.
//
//     python -m codebreaker ppc-calls <module.xex or image> [-o calls.tsv]
//     Result r = ppc_calls::analyse(path);
//     r.arity        {address: argument registers r3.. its callers set up}   (internal functions)
//     r.imports      {name: argument registers}                            (imported functions)
//     r.clobber      {address: mask of r0, r4-r12 it may change}           (only those that keep some)
//     r.fpclobber    {address: mask of f0-f13 it may change}               (only those that keep some)
//     r.variadic     {address: fixed arguments before the variable ones}
//
// Each is evidence, never a guess:
//
//   * Arity: before a `bl`, the argument registers r3, r4, ... set within the 14 instructions back
//     (stopping at an earlier call), counted while they run on without a gap. A function's arity is
//     the count most of its call sites agree on: at least 3 sites, 60 % agreeing (85 % for 8, which
//     a longer argument list also gives), and 1-7 (0 is a pass-through as often as no arguments). An
//     import is a stub with no body to pass through, so 0 and 8 count there; it needs 3 sites and
//     85 %, and the printf family, whose count varies by call, is left out.
//   * Clobber: the volatile registers a function may change -- those it writes, and those every
//     function it calls may change, to a fixed point over the call graph. A call to an import, through
//     a pointer or out of the known functions may change them all. The compiler's register save and
//     restore runs (__savegprlr_N ...) change nothing the caller sees, and neither does a register
//     reloaded from where the function stored it on its frame. What is reported is what a function
//     provably keeps: a caller may read those registers across the call.
//   * Variadic: a function that stores r8, r9 and r10 to its caller's frame before its own `stwu`
//     is homing the va_list area; its fixed arguments are the registers below the lowest one homed.
//
// These passes come from the clean-room xam decompile (../xam, tools/infer_*.py), where they turned
// thousands of refused functions into faithful C; here they run on CodeBreaker's own listing
// (recompile/disassemble.py) and function discovery (recompile/discover.py), for any module.
//
// Typing notes:
//   * Result is a struct of the five dicts and the function count (the Python's class with those attributes).
//   * A decoded body is `Body`, a list of (address, mnemonic, operands) rows.
//   * `_reg` answers None as std::nullopt; `_target` likewise. gpr_writes / fpr_writes answer the Python's sets.
//   * `_consensus` takes the call-site counts as a list and answers None as std::nullopt.
//   * Needs Capstone through recompile/disassemble: hand the DLL to disassemble::set_capstone_dll() first.
//   * main(argv) takes sys.argv[1:] and returns the exit code; it prints with py::print.
//   * not ported: the `__name__ == "__main__"` block (`sys.exit(main(sys.argv[1:]))`).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::ppc_calls {

inline constexpr int64_t LOOKBACK = 14;
extern const int64_t VOLATILE_MASK;
extern const int64_t FP_VOLATILE_MASK;
// range(3, 11): the argument registers
extern const py::list<int64_t> ARG_REGS;
// (0, 4, 5, 6, 7, 8, 9, 10, 11, 12): r3 is the return value, always changed
extern const py::list<int64_t> VOLATILE;

// What analyse() answers.
struct Result {
    py::dict<int64_t, int64_t> arity;
    py::dict<py::str, int64_t> imports;
    py::dict<int64_t, int64_t> clobber;
    py::dict<int64_t, int64_t> fpclobber;
    py::dict<int64_t, int64_t> variadic;
    int64_t functions = 0;
};

// One instruction of a function: (address, mnemonic, operands).
using Row = std::tuple<int64_t, py::str, py::list<py::str>>;
using Body = py::list<Row>;

// `[t.strip() for t in text.split(",")] if text.strip() else []`
py::list<py::str> _ops(const py::str& text);

// The register number of a token `r<n>` (or, with `fp`, `f<n>` / `fr<n>`), else None.
std::optional<int64_t> _reg(const std::optional<py::str>& tok, bool fp = false);

// int(ops[0], 16) when the first operand starts with 0x, else None.
std::optional<int64_t> _target(const py::list<py::str>& ops);

// The general registers an instruction writes (an over-approximation).
py::set<int64_t> gpr_writes(const py::str& mnem, const py::list<py::str>& ops);

py::set<int64_t> fpr_writes(const py::str& mnem, const py::list<py::str>& ops);

// ({start: body} over discovered functions, the image)
std::tuple<py::dict<int64_t, Body>, py::ref<formats::powerpc_image::Image>> _functions(const py::str& path);

// How many argument registers (r3, r4 ...) the instructions before body[i] set up for its call.
int64_t _site_arity(const Body& body, int64_t i);

// The count most call sites agree on, or None.
std::optional<int64_t> _consensus(const py::list<int64_t>& arities, bool stub);

Result analyse(const py::str& path);

void write_tsv(const Result& r, const py::str& out);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::analyze::ppc_calls
