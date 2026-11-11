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
//   * The Python draws its choices from random.Random(seed).  The runtime has no `random`, so the Mersenne
//     Twister of CPython (seeding by init_by_array, getrandbits, random, choice, sample) is `Random` below,
//     bit for bit; the same seed gives the same rewrites as the Python.
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

namespace codebreaker::verify::authored::search16 {

// random.Random(seed): CPython's Mersenne Twister, the calls search16 makes
class Random {
  public:
    explicit Random(int64_t seed);
    uint32_t genrand_uint32();
    double random();                                    // [0.0, 1.0)
    int64_t getrandbits(int64_t k);                     // 0 <= k <= 63
    int64_t _randbelow(int64_t n);
    template <class T> T choice(const py::list<T>& seq) {
        if (seq.empty()) throw py::IndexError("Cannot choose from an empty sequence");
        return seq[_randbelow(py::len(seq))];
    }
    // random.sample(population, k) (a list)
    template <class T> py::list<T> sample(const py::list<T>& population, int64_t k) {
        int64_t n = py::len(population);
        if (k < 0 || k > n) throw py::ValueError("Sample larger than population or is negative");
        std::vector<T> result;
        int64_t setsize = 21;
        if (k > 5) setsize += sample_extra_(k);
        if (n <= setsize) {
            std::vector<T> pool;
            for (int64_t i = 0; i < n; i++) pool.push_back(population[i]);
            for (int64_t i = 0; i < k; i++) {
                int64_t j = _randbelow(n - i);
                result.push_back(pool[static_cast<size_t>(j)]);
                pool[static_cast<size_t>(j)] = pool[static_cast<size_t>(n - i - 1)];
            }
        } else {
            std::vector<int64_t> selected;
            for (int64_t i = 0; i < k; i++) {
                int64_t j = _randbelow(n);
                while (std::find(selected.begin(), selected.end(), j) != selected.end()) j = _randbelow(n);
                selected.push_back(j);
                result.push_back(population[j]);
            }
        }
        return py::list<T>(result);
    }

  private:
    static int64_t sample_extra_(int64_t k);            // 4 ** ceil(log(k * 3, 4))
    uint32_t mt_[624];
    int mti_ = 625;
    void init_genrand_(uint32_t s);
};

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
