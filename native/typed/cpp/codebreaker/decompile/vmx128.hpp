// codebreaker/decompile/vmx128.hpp -- typed C++ of codebreaker.decompile.vmx128
//
// VMX128 -- the Xbox 360 (Xenon) 128-register extension of AltiVec.
//
//     vmx128::main({"0x102320C3", ...})        decode instruction words
//     vmx128::main({"--coverage", file})       vector decode coverage over .text
//     vmx128::main({"--sleigh", out})          write the SLEIGH (vmx128.sinc)
//
// Typing notes: `coverage` takes the formats::powerpc_image::Image the Python's `img` is; `sleigh`'s
// `declared` is a set of strings; a decoded word is std::optional<std::tuple<mnemonic, operands>>.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::decompile::vmx128 {

extern const py::str DOC;   // the module docstring

int64_t vd128(int64_t w);
int64_t va128(int64_t w);
int64_t vb128(int64_t w);
int64_t vc128(int64_t w);

// (mnemonic, pattern, mask, form, semantics): the 77 rows
using Row = std::tuple<py::str, int64_t, int64_t, py::str, py::str>;
extern const py::list<Row> ROWS;
// (mnemonic, extended opcode, form, the user operation the SLEIGH calls)
using X31Row = std::tuple<py::str, int64_t, py::str, py::str>;
extern const py::list<X31Row> X31;

// (mnemonic, operand text) for a VMX128 instruction word (or one of X31's), else None.
std::optional<std::tuple<py::str, py::str>> decode(int64_t word);
py::str _operands(const py::str& form, int64_t w);
std::optional<py::str> text(int64_t word);

// {'vector words', 'vmx128', 'other'} over a PowerPC image's code section
py::dict<py::str, int64_t> coverage(const formats::powerpc_image::Image& img);

// Contiguous runs (lo, hi) of the set bits.
py::list<std::tuple<int64_t, int64_t>> _runs(int64_t bits);
// The text of languages/src/ppc/vmx128.sinc; `declared`: the user operations the other PowerPC .sinc
// files already define (a second definition is an error).
py::str sleigh(const py::set<py::str>& declared = py::set<py::str>());
// (display operands, pattern operands, p-code lines) for one row.
std::tuple<py::str, py::list<py::str>, py::list<py::str>> _constructor(const py::str& form, const py::str& sem);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::decompile::vmx128
