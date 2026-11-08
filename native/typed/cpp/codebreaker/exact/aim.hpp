// codebreaker/exact/aim.hpp -- C++17 port of codebreaker.exact.aim.
//
// aim.py -- where a candidate's code differs from the program's, and how: what the beam rewrites first.
//
// The compiler's listing of a candidate (toolchain.compile(listing=True)) gives each instruction the line of the
// source it came from. The candidate's instructions are lined up with the program's (both read by the same
// disassembler, the numbers a link would change masked), and each stretch that differs is told by its kind:
//
//     frame    the same instructions, only the places in the frame ([bp - N]) other
//     width    a byte read where a word is, or the other way (byte ptr, cbw, sub ah,ah)
//     sign     a jump of the other signedness (jl / jb ..), sar / shr, imul / mul
//     order    the same instructions in another order or layout (the jumps, the blocks)
//     shape    something else: the code of a statement itself
//
// and by the lines of the candidate that stretch comes from. A diagnosis is one compile more (kept in the cache).
//
// The instructions are read by codebreaker::exact::x86 (its 16-bit decoder is installed with
// x86::set_decoder, as that header says); difflib.SequenceMatcher, which the runtime has none of, is
// ported here as a small local class.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/exact/x86.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::aim {

extern const std::tuple<py::str, py::str, py::str, py::str, py::str> KINDS;
extern const py::dict<py::str, py::str> SIGNED;

// One instruction of a listing: (offset, bytes, the source line before it or None)
using ListingEntry = std::tuple<int64_t, py::bytes, std::optional<int64_t>>;

// diagnose()'s result: {"kinds": {kind: n}, "lines": [source lines of the differing stretches, first first],
// "first": line}
struct Diagnosis {
    py::dict<py::str, int64_t> kinds;
    py::list<int64_t> lines;
    std::optional<int64_t> first;
};
template <class F> void for_each_field(const Diagnosis& r, F&& f) {
    f("kinds", r.kinds);
    f("lines", r.lines);
    f("first", r.first);
}
template <class F> void for_each_field_mut(Diagnosis& r, F&& f) {
    f("kinds", r.kinds, true);
    f("lines", r.lines, true);
    f("first", r.first, true);
}

// [(offset, bytes, line)] - the instructions of the listing's function (the first, or the one named), each
// with the source line before it ("; Line N")
py::list<ListingEntry> listing_code(const py::str& text, const std::optional<py::str>& name = std::nullopt);

// an instruction as compared: mnemonic and operands, the numbers a link changes (places of code and data,
// targets) masked, a frame's place and a small constant kept
py::str _norm(const x86::Insn& ins, int64_t base);

py::list<x86::Insn> _insns(const py::bytes& code);

// the kind of a differing stretch: a (the program's), b (the candidate's) - normalized instructions
py::str _kind(const py::list<py::str>& a, const py::list<py::str>& b);

// orig the program's bytes of the function, code the candidate's (from its listing), listing its listing_code
Diagnosis diagnose(const py::bytes& orig, const py::bytes& code, const py::list<ListingEntry>& listing);

// the candidate's function bytes, from its listing
py::bytes code_of(const py::list<ListingEntry>& listing);

// difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() for lists of strings:
// (tag, i1, i2, j1, j2) with tag "replace" / "delete" / "insert" / "equal"
using Opcode = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t>;
py::list<Opcode> _get_opcodes(const py::list<py::str>& a, const py::list<py::str>& b);

}  // namespace codebreaker::exact::aim
