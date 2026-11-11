// codebreaker/verify/authored/search16.hpp -- typed C++ of codebreaker.verify.authored.search16
//
// search16.py -- the last bytes of a near miss, found by trying small rewrites of its source.
//
//     python -m codebreaker match <16-bit module> --search ROUNDS
//
// A function a few bytes from the original under Microsoft C (verify/authored/match16.py) is often one the
// compiler would make exactly from a source written a little differently: its locals declared in
// another order (the compiler lays the frame out by declaration, so every [bp-N] moves), or one of
// them unsigned where the decompiler guessed signed (a compare, a shift, a widening). Each round
// takes every near miss's best text so far, rewrites it one small way, compiles all of them (one
// emulator session for each compiler and switches), and keeps a rewrite that comes nearer. A rewrite
// that comes out identical is source: the compiler says so, byte for byte.
//
// Typing notes:
//   * The Python draws its choices from random.Random(seed): `Random` below is the runtime's py::random::Random
//     (CPython's Mersenne Twister, bit for bit); the same seed gives the same rewrites as the Python.
//   * A verdict is the tuple (word, differing bytes, total bytes) that verify/authored/match16.py makes with
//     match.distance(...)[:3] (or (V.NO_COMPILE, 0, 0)); a group is the tuple (compiler, switches, winner).
//   * compile_group and score are the caller's functions (std::function); `log` is the Python's `log=print`:
//     an empty std::function is None (no log), the overload without it prints to stdout.
//   * Python int(num, 0) of a constant of a comparison: a decimal with a leading zero is a ValueError; a value
//     past int64 is outside this port (no real source gets near it).
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"
#include "py_random.hpp"

namespace codebreaker::verify::authored::search16 {

// random.Random(seed): the runtime's CPython-exact Mersenne Twister
using Random = py::random::Random;

const py::re::Pattern& _DECL();
const py::re::Pattern& _COMPARE();
const py::re::Pattern& _LEFT();
const py::re::Pattern& _IF();
const py::re::Pattern& _STMT();
const py::re::Pattern& _SELF();
const py::re::Pattern& _COMPOUND();
const py::re::Pattern& _COMMUTE();
const py::dict<py::str, py::str>& _SIGN();              // int1 <-> uint1 ...

// the indices of the body's leading declarations (not the K&R parameter declarations)
py::list<int64_t> _locals(const py::list<py::str>& lines);

// A comparison with a constant, as the decompiler normalizes it (`x <= 0x19`) and as a source may
// have had it (`x < 0x1a`): the compiler writes the constant given and the jump that goes with it.
py::str _compare(const py::re::Match& m);

// Ghidra puts the constant first (`0x19 < iVar1`); a source had the variable first, and the
// comparison it meant may be the other side of the constant (`iVar1 >= 0x1a`).
py::str _left(const py::re::Match& m);

// [(if line, else line, end line)] of each `if (c) { ... } else { ... }` written a block to a line
py::list<std::tuple<int64_t, int64_t, int64_t>> _if_else(const py::list<py::str>& lines);

// `cond` said the other way: a comparison turned around, anything else under a !
py::str _negate(const py::str& cond);

// [k] -- adjacent simple statements k, k+1 of one block (neither a declaration nor a control line)
py::list<int64_t> _statement_swaps(const py::list<py::str>& lines, int64_t body);

// one small rewrite of `text`, or None when there is nothing of the kind to rewrite
std::optional<py::str> rewrite(const py::str& text, Random& rng);

using Verdict = std::tuple<py::str, int64_t, int64_t>;      // (word, differing bytes, total bytes)
using Group = std::tuple<py::str, py::str, py::str>;        // (compiler, switches, winner)
using Near = std::tuple<py::str, Verdict, Group>;           // (text, verdict, group)
using CompileGroup = std::function<py::dict<py::str, Verdict>(const Group&, const py::dict<py::str, py::str>&)>;
using Score = std::function<double(const Verdict&)>;
using Log = std::function<void(const py::str&)>;            // empty: None

// near: {fn: (text, verdict, group)} -- each near miss's best text, its verdict and the compiler and switches it
// is judged under; compile_group(group, {fn: text}) -> {fn: verdict}; score(verdict) -> number, higher nearer.
// -> {fn: (text, verdict)} the best found.
py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                       const Score& score, int64_t rounds, int64_t seed, const Log& log);
// log=print, seed=1995
py::dict<py::str, std::tuple<py::str, Verdict>> search(const py::dict<py::str, Near>& near, const CompileGroup& compile_group,
                                                       const Score& score, int64_t rounds);

}  // namespace codebreaker::verify::authored::search16
