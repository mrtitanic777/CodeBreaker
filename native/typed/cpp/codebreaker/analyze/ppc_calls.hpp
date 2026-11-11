// codebreaker/analyze/ppc_calls.hpp -- COMPAT SHIM over cb/analyze/ppc_calls.hpp (docs/CPP_DESIGN.md 6.3), for the old
// callers (__main__'s ppc-calls command). Deleted with its last py:: consumer.
//
// The passes (arity, clobber, variadic, the TSV) are native/cb/analyze/ppc_calls.hpp, over a Module of listings. Making a
// Module from a file needs function discovery and the disassembler's listing, which are the old recompile/discover port
// until it is refactored (COMPAT(py): remove when recompile/discover and formats/powerpc_image are refactored); the
// shim's .cpp does that, hands the Module to the new passes, and converts the answer to the old shapes.
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/analyze/ppc_calls.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::ppc_calls {

inline constexpr int64_t LOOKBACK = ::cb::analyze::ppc_calls::kLookback;
extern const int64_t VOLATILE_MASK;
extern const int64_t FP_VOLATILE_MASK;
extern const py::list<int64_t> ARG_REGS;    // range(3, 11): the argument registers
extern const py::list<int64_t> VOLATILE;    // (0, 4 .. 12): r3 is the return value, always changed

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

// ({start: body} over discovered functions, the image)
std::tuple<py::dict<int64_t, Body>, py::ref<formats::powerpc_image::Image>> _functions(const py::str& path);

Result analyse(const py::str& path);
void write_tsv(const Result& r, const py::str& out);
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::analyze::ppc_calls
