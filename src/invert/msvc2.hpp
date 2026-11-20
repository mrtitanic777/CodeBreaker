// invert/msvc2.hpp -- the Microsoft C/C++ 2.0 (32-bit, the Windows 95 build's C2.EXE) code generator, forward and inverse.
//
//     emit      a construct (cb/invert/term.hpp) -> the exact bytes `/O1 /Oy-` gives for it, or none for a construct outside the modelled class
//     to_c      the C a construct stands for (what the self-test hands the compiler)
//     invert    the bytes of a function -> the construct and C that give them back: the inverse proposes candidates from the codegen templates and
//               accepts one only when `emit` reproduces the bytes (the places a linker fills in -- call targets, global addresses -- left out), so
//               every answer is byte-exact by construction -- never a plausible-but-wrong guess; a function outside the class is declined (none)
//     interp    the straight-line register-dataflow walk of a framed function: its statement-list constructs, each still to be checked by `emit`
//     frame_layout  the stack frame the compiler lays out for a list of locals
//
// The class: single-`return` __stdcall leaves over int arguments and constants (chains, mixed precedence, products, comparisons, deref / index),
// globals, stores, extern and indirect calls, call results combined with an operand, the esi-parked `callL <op> callR`, the frameless (/Oy) and
// __fastcall forms, and straight-line statement lists (void calls, if-guards, stores, a return).
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "invert/term.hpp"

namespace cb::invert::msvc2 {

std::optional<Bytes> emit(const Term& node);
std::string to_c(const Term& node);

struct Reading {
    long long nargs = 0;
    Term expr;           // the returned expression; none for a statement list
    std::string c;
    Term node;           // the construct that gave the bytes back
};

// `mask`: the byte positions to ignore (default: each candidate's own relocation slots).
std::optional<Reading> invert(ByteView code, const std::set<long long>* mask = nullptr);

// The constructs `code` (a framed function of `nargs` arguments) may be, or none where it steps outside the cut.
std::optional<Term> interp(ByteView code, long long nargs);

struct Local {
    std::uint32_t size = 0;
    bool is_array = false;
};
struct FrameLayout {
    std::uint32_t frame_size = 0;
    // (index in declaration order, ebp offset) in the order the compiler lays them out: scalars first, then arrays, each in declaration order; each
    // size rounded up to 4; the offset is minus the bytes laid out through that local.
    std::vector<std::pair<std::size_t, std::int64_t>> offsets;
};
FrameLayout frame_layout(std::span<const Local> locals);

}  // namespace cb::invert::msvc2
