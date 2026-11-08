// codebreaker/exact/habits/symx.hpp -- C++ from codebreaker.exact.habits.symx.
//
// symx.py - do two stretches of 8086 code without a jump in them compute the same?
//
// Used by newcomp.py where no habit of the later compiler it knows explains a difference. Both
// stretches are run on symbols from the same start (every register holds the same unknown value in
// both); they are the same if they push the same values, store the same values at the same places in
// the same order, and leave the same values in the frame's temporaries - which may be other
// temporaries, each of the one for one of the other. Sums, products and ORs are compared without
// regard to the order of their terms, a shift to the left as a product. The one jump allowed is the
// compiler's way to a Boolean's value, mov R,0 / jcc over / inc R. What it does not know it refuses.
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::habits::symx {

// class No(Exception): what the symbolic run does not know
struct No : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "No"; }
};

extern const py::list<py::str> R16;
extern const py::dict<py::str, py::str> LO;
extern const py::dict<py::str, py::str> HI;
extern const py::dict<py::str, py::str> COMM;
extern const py::dict<py::str, int64_t> UNIT;

// A symbolic value. In the Python it is a tuple whose first item names its kind and whose other
// items depend on the kind (the expressions are nested tuples, a kind's terms in a list):
//
//   ("c", n)                  a constant              ("f",)               a constant a linker fills in
//   ("r", name)               the unknown a register held at the start ("bp" too)
//   ("neg", x)                -x                      ("lo", x)            the low byte of x
//   ("lohi", x, old)          old with its low byte replaced by x         ("zx", x)  x zero-extended
//   ("+" | "*" | "|" | "&" | "^", [terms])   a sum, product, OR, AND, XOR (terms in canonical order)
//   ("m", address, size, k)   a read of memory ("word" or "byte") after k writes
//   ("ord", opcode, flags)    a Boolean from a conditional jump (opcode) on the flags (or None)
//   ("shr", x, n)             x >> n                  ("high", x)          the high word of a product
//   ("div", dx, ax, d)        the quotient            ("mod", dx, ax, d)   the remainder
//   ("res", x)                the flags of a result   ("cmp", a, b)        the flags of a comparison
//
// Here each kind keeps its items in the same fields: n (the int item), name (the str item), args
// (the items that are values, or the list of terms; for "ord" the flags are args[0], absent for None).
struct Expr {
    py::str tag;
    int64_t n = 0;
    py::str name;
    py::list<Expr> args;
};
bool operator==(const Expr& a, const Expr& b);
inline bool operator!=(const Expr& a, const Expr& b) { return !(a == b); }

// one instruction: (text, its bytes)
using Ins = std::tuple<py::str, py::bytes>;

// a frame temporary: (size, value)
struct Slot {
    py::str size;
    Expr value;
};
// a store to memory: (address, size, value)
struct Write {
    Expr addr;
    py::str size;
    Expr value;
};

Expr C(int64_t n);
bool isc(const Expr& x);
int64_t fold(const py::str& tag, int64_t a, int64_t b);
Expr nary(const py::str& tag, const py::list<Expr>& xs);
Expr neg(const Expr& x);
py::str canon(const Expr& x);
py::str canon(const py::list<Expr>& x);
py::str canon(const Slot& x);
bool unify(const Expr& a, const Expr& b);
bool unify(const py::list<Expr>& a, const py::list<Expr>& b);
bool unify(const Write& a, const Write& b);
bool unify(const py::list<Write>& a, const py::list<Write>& b);
bool unify(const Slot& a, const Slot& b);
bool unify(const py::list<Slot>& a, const py::list<Slot>& b);
bool unify(const std::optional<Expr>& a, const std::optional<Expr>& b);

// Python's repr of the value (the tuples and lists the Python builds); what "same" puts in its reasons.
py::str show(const Expr& x);
py::str show(const py::list<Expr>& x);

// the state after run: registers, pushes, stores, frame temporaries by offset from bp, last flags
struct RunResult {
    py::dict<py::str, Expr> reg;
    py::list<Expr> pushes;
    py::list<Write> writes;
    py::dict<int64_t, Slot> slots;
    std::optional<Expr> flags;
};

// ins: [(text, bytes)]; fixed: {index of an instruction: the addresses in it a linker fills in} -> the state after
RunResult run(const py::list<Ins>& ins, const py::dict<int64_t, py::set<int64_t>>& fixed);

// w, g: [(text, bytes)] -> (True, '') or (False, why)
std::tuple<bool, py::str> same(const py::list<Ins>& w, const py::list<Ins>& g, const py::dict<int64_t, py::set<int64_t>>& fixed,
                               bool regs_too = false, bool flags_too = false);

}  // namespace codebreaker::exact::habits::symx
