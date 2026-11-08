// codebreaker/verify/guided.hpp -- typed C++ port of codebreaker.verify.guided.
//
// guided.py -- inputs aimed at the branches no input has taken yet.
//
// A function verified on inputs that ran under half of its instructions is "verified (weak)": the
// rest was never compared. The fixed inputs (verify/lanes/win32.py _vectors, _confirm_vectors) and the
// directed ones (constants and globals read off the code) stop at the first test they do not
// happen to satisfy. This generator watches each conditional jump of the ORIGINAL function -- which
// way it went, and what the comparison before it compared -- and makes inputs for the side not
// taken yet:
//
//   * input-to-state: when a value the comparison saw is also in the input (an argument, the value
//     imported calls return, a word of the memory handed in), that place gets the value it was
//     compared with, one either side of it, or for a zero test the opposite of zero;
//   * otherwise the compared-with value is tried in each argument, and a few fixed changes
//     (interesting integers, what imported calls return) are tried on the inputs kept so far.
//
// An input is kept when the original then takes a branch direction, or runs an instruction, that
// no input before it did. Kept inputs are run on both sides by compare.compare like any other.
//
//     guided_inputs(run, seeds, budget=96, keep=16) -> [input]
//
// `run(input)` traces the original and returns an Observation (the Python's {"branches": [(jump,
// taken, compare)], "reach": set of addresses, "args_at": where the first argument is on the stack,
// "stack": (low, high) of the stack}) or None when it cannot be observed (compare: see
// emulate/emulator.py _branch_hook). An input is the Python's dict with "args" (a list of words) and
// optionally "mem_init" [(address, bytes)] and "stub_ret"; here an Input record. Nothing here knows
// the processor: verify/lanes/win32.py, win16.py and powerpc.py supply `run`.
//
// Types the Python leaves implicit:
//   * Input: the dict's three known keys are fields; any other key a lane puts in the dict (win32.py
//     adds "ecx", "edx", "given") travels in `extra` untouched, as the Python's dict(v) copy carries it.
//     An extra is an int or a list of ints (std::variant, said here because the Python is untyped).
//   * Compare: the comparison tuple (mnemonic, size, a, b[, address of a, address of b]). `at` holds
//     the two optional addresses; both None for the four-element form.
//   * words are int64_t (the Python's ints are unbounded). Exact for words up to 4 bytes (all lanes use
//     2 or 4); an 8-byte word with values at or above 2**63 would differ.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::verify::guided {

// A value of an Input's other keys: an int or a list of ints.
using Extra = std::variant<int64_t, py::list<int64_t>>;

// An input: {"args": [...], "mem_init": [(address, bytes)], "stub_ret": int, ...other keys}.
struct Input {
    py::list<int64_t> args;
    std::optional<py::list<std::tuple<int64_t, py::bytes>>> mem_init;
    std::optional<int64_t> stub_ret;
    py::dict<py::str, Extra> extra;
};

// What a comparison before a jump compared: (mnemonic, size, a, b[, a's address, b's address]).
struct Compare {
    py::str mn;
    int64_t size = 0;
    int64_t a = 0;
    int64_t b = 0;
    std::optional<int64_t> at_a;
    std::optional<int64_t> at_b;
    bool has_at = false;          // the six-element form (len(cmp) > 4)
};

// One trace of the original: its conditional jumps, the addresses it ran, and where the stack is.
struct Observation {
    py::list<std::tuple<int64_t, bool, std::optional<Compare>>> branches;   // (jump, taken, compare)
    py::set<int64_t> reach;
    std::optional<int64_t> args_at;
    std::optional<std::tuple<int64_t, int64_t>> stack;    // (low, high)
};

// What to change in an input: where a value lives.
enum class PlaceKind { arg, addr, stub_ret, mem };
struct Place {
    PlaceKind kind = PlaceKind::arg;
    int64_t a = 0;     // arg: word index; addr: the address; mem: index into mem_init
    int64_t b = 0;     // arg: byte within the word; mem: byte offset in that block
    friend bool operator==(const Place& x, const Place& y) { return x.kind == y.kind && x.a == y.a && x.b == y.b; }
};

using Run = std::function<std::optional<Observation>(const Input&)>;

extern const py::list<int64_t> INTERESTING;

// {(jump address, direction taken)} of an observation
py::set<std::tuple<int64_t, bool>> _edges(const Observation& obs);

// an independent copy of an input (its lists are not shared)
Input _copy(const Input& v);

// (operand, value seen, value wanted): changes to one operand that would send a comparison
// the other way.
py::list<std::tuple<int64_t, int64_t, int64_t>> _flips(const Compare& cmp);

struct _Machine {
    py::str order;       // "little" or "big"
    int64_t word;        // bytes in an argument
    uint64_t wmask;
    _Machine(const py::str& byteorder, int64_t word_);
};

// Places in input `v` to put a new value for operand `k` of comparison `cmp`: first where
// that operand was read from (an argument's stack slot -- also one past those given -- or any
// other memory the input can set), then every place in the input that holds the value it had.
py::list<Place> _places(const Input& v, const Observation& obs, const Compare& cmp, int64_t k, int64_t value,
                        int64_t size, const _Machine& mach);

Input _put(const Input& v, const Place& place, int64_t value, int64_t size, const _Machine& mach);

// Inputs (beyond `seeds`) that take branch directions the seeds did not. See the module text.
// `byteorder` and `word` (bytes in an argument) are the processor's. `observed`: [(input, its
// observation)] of inputs already run, taken as seeds without running them again.
py::list<Input> guided_inputs(const Run& run, const py::list<Input>& seeds, int64_t budget = 96, int64_t keep = 16,
                              int64_t seed = 0, const py::str& byteorder = py::str("little"), int64_t word = 4,
                              const py::list<std::tuple<Input, Observation>>& observed = py::list<std::tuple<Input, Observation>>());

}  // namespace codebreaker::verify::guided
