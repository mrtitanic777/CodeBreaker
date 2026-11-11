// codebreaker/invert/msvc2_forward.hpp -- C++17 port of codebreaker.invert.msvc2_forward.
//
// msvc2_forward.py - FORWARD MODEL for the MSVC 2.0 (win95 C2.EXE) code generator, 32-bit.
//
// A deterministic `construct -> bytes` predictor of what MSVC 2.0's backend emits at the retail flags
// (`/O1 /Oy-`), each rule found by probing the real compiler and proven byte-exact against it; the
// first step toward msvc2.py's inverse (bytes -> byte-exact C).  The constructs: single-`return`
// __stdcall leaves over int args and constants (const, arg, arg<op>{arg,const}, chains, mixed
// precedence, products, comparisons, deref / index, unary), globals, stores, extern and indirect
// calls, call results combined with an operand, the esi-parked `callL <op> callR`, the frameless
// (/Oy) and __fastcall forms, and multi-statement bodies (void calls, if-guards, stores, a return).
//
// The model is C++ (native/invert/msvc2.cc: emit, to_c, frame_layout; docs/NATIVE.md); this calls its entry
// point (cbinv::msvc2_call) directly, and keeps the self-tests against the 1994 compiler.
//
//     wt inverse msvc2model                 # self-test: model vs oracle, byte-exact count
//     wt inverse msvc2model --show          # dump each case: predicted vs shipped
//     wt inverse msvc2model --frame         # the stack-frame layout against the compiler
//
// Out-of-class constructs are declined (emit returns None) -- never mis-modeled.
//
// Typing notes:
//   * The construct IR is the Python's nested tuples: ("const", V), ("arg", K), ("binop", op, L, R),
//     ("call", conv, [args]), ("if", cond, [stmts]) ...  A tuple is the record `Expr`: `tag` is its first
//     element and `args` the rest; an element is one of None, bool, int, str, a nested tuple (Expr) or a list
//     of tuples (the std::variant `Arg`, since the Python mixes them in one place).
//   * A node is the Python's dict, whose keys are nargs, expr, stmts, ret, leave, fpo and cc: the record
//     `Node`, a key the dict lacks being an empty optional.  (stmts, a statement-list node's body, is a list of
//     Expr.)
//   * main takes sys.argv (the program name first) and returns the process's exit status (the Python's sys.exit).
//   * The oracle is invert/compilers.msvc2() (settings: period_compiler).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/invert/compilers.hpp"
#include "py.hpp"

namespace codebreaker::invert::msvc2_forward {

struct Expr;
using ExprPtr = std::shared_ptr<Expr>;
// None | bool | int | str | a tuple | a list of tuples
using Arg = std::variant<std::monostate, bool, int64_t, py::str, ExprPtr, py::list<ExprPtr>>;

struct Expr {
    py::str tag;
    std::vector<Arg> args;
};

// {"nargs": N, "expr": E}  or a statement list {"nargs", "stmts", "ret", "leave"}; "fpo": True is the frameless
// form, "cc": "fastcall" the register form
struct Node {
    std::optional<int64_t> nargs;
    std::optional<ExprPtr> expr;                 // (absent or None: emit declines)
    bool expr_none = false;                      // the key "expr" is there, holding None (to_c then raises TypeError, not KeyError)
    std::optional<py::list<ExprPtr>> stmts;
    std::optional<ExprPtr> ret;
    bool ret_none = false;                       // the key "ret" is there, holding None
    std::optional<bool> leave;
    std::optional<bool> fpo;
    std::optional<py::str> cc;
};

extern const py::set<py::str> OPS;

// Forward model: construct -> exact MSVC-2.0 bytes, or None if out of class.  A node with
// "fpo":True is the frameless (/Oy) form; otherwise the standard /Oy- framed form.
std::optional<py::bytes> emit(const Node& node);

// The C the node stands for (what the self-test hands the compiler).
py::str to_c(const Node& node);

// --- the battery: every construct in the class ------------------------------
py::list<std::tuple<py::str, Node>> battery();

// Shapes the model must DECLINE (emit -> None): kept as guards against a wrong guess.
py::list<std::tuple<py::str, Node>> declined_battery();

// ===========================================================================
// 32-BIT FRAME-LAYOUT MODEL  (the early_c_frame.py analogue)
//   * frame:  push ebp; mov ebp,esp; sub esp,N   /   epilogue: mov esp,ebp; pop ebp; ret[4*nargs]
//   * each local's size rounds UP to 4 (char[10] -> 12).
//   * ORDERING: scalars first (top, nearest ebp), then arrays; within each class, DECLARATION order.
//   * offset(local) = -(cumulative rounded size through that local); framesize = that cumulative.
// ===========================================================================
// locals_ = [(size_bytes, is_array), ...] in DECLARATION order.
// Returns (framesize, {decl_index: ebp_offset}) -- offset negative, i.e. [ebp+offset].
std::tuple<int64_t, py::dict<int64_t, int64_t>> frame_layout(const py::list<std::tuple<int64_t, bool>>& locals_);

// (name, C source, locals-in-decl-order[(size,is_array)])
py::list<std::tuple<py::str, py::str, py::list<std::tuple<int64_t, bool>>>> _frame_battery();

// From compiled bytes: (framesize from `sub esp,N`, sorted set of distinct [ebp-X] offsets).
std::tuple<int64_t, py::list<int64_t>> _extract_frame(const py::bytes& code);

bool frame_selftest(bool show = false);

// The 1994 32-bit compiler the model is checked against (invert/compilers.py).
compilers::Msvc2 _oracle();

bool selftest(bool show = false);

// the module's `if __name__ == "__main__"` block; returns the exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::invert::msvc2_forward
