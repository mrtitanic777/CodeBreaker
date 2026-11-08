// codebreaker/verify/lane_common.hpp -- C++17 port of codebreaker.verify.lane_common.
//
// lane_common.py -- what every kind's judge does the same way.
//
// A kind of module (verify/lanes/win32.py, win16.py, powerpc.py, driver.py) knows how to run its
// code -- its emulator, its calling convention, which addresses mean what -- and turns a run (the
// one run record every emulator returns, emulate/run.py) into a fingerprint for verify/compare.py. What is left over is the same for all of them, and lives here
// so that it stays the same:
//
//     the switches     CB_NO_FP_CACHE, CB_NO_GUIDED and CB_PLAIN, read in one place
//     remembered()     a fingerprint kept across runs, under a key naming everything the run depends on
//     call(), calls()  a recorded call as the text two builds of the same code agree on
//     argument()       one argument of such a call
//     writes()         the memory a run left changed, as the digest a fingerprint carries
//     fingerprint()    what compare.py compares, made from an emulator's run record (emulate/run.py):
//                      how it ended, the result, the calls, the memory, the stack, the saved registers
//     changed(), left() the memory a run record says was left changed, as a lane names its places
//     the inputs       the argument patterns the kinds' tables of inputs are made of
//     guided()         branch-guided inputs (verify/guided.py), with the limits every kind uses
//     explored()       the same found path by path (verify/concolic.py), where the kind can follow
//                      a run's values symbolically; and the limits a verdict then carries
//     judged()         one function's verdict, whatever goes wrong while reaching it
//
// Typing notes:
//   * The fingerprint is compare::Fingerprint (a record: see compare.hpp), not a dict: fingerprint()'s `**more` is
//     the one field a lane adds that the comparer reads, `steps`.
//   * A place a lane names a byte of memory by is an address (int) or a (tag, offset) pair: `Place`; what a place
//     ends up holding is a byte or a token standing for a whole word ("S"): `Held`.  `changed()` returns them as a
//     dict.  Its `place(address)` answers None (leave the write out), a key (the write's bytes follow it), or a
//     function naming each byte: `Where`.
//   * `calls()` takes the calls as `Called` (name, arguments or None); called() makes them from a run record's Calls.
//     `name` and `skip` are functions of the call's name (None is a name, as in the Python).
//   * argument()'s `image` is None, a (low, high) range, or a test: `Image`; `frame` holds ranges, or None for a
//     frame that is not there.
//   * guided() and explored() are the front ends of verify/guided.py and verify/concolic.py (see there for the
//     input and observation records); explored<V> is a template over the lane's input type as concolic::explore is.
//   * judged(): "Anything that goes wrong on the way" is any py::Exception other than ImportError (a std::exception that
//     is not one is judged "Exception").
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/emulate/run.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/guided.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::lane_common {

// --- the switches -----------------------------------------------------------------------------
// False when CB_NO_FP_CACHE is set: every run is made afresh, none is kept (the test suites
// run so, since a verdict must then reflect the code as it is).
bool fp_cache_on();

// False when CB_NO_GUIDED is set: no branch-guided inputs (to measure what they add).
bool guided_on();

// False when CB_NO_CONCOLIC is set: the branch-guided inputs are guessed (verify/guided.py)
// rather than solved for along each path (verify/concolic.py) -- to compare the two.
bool concolic_on();

// CB_PLAIN=1: one instruction at a time on a fresh machine each run -- the slow way the fast one
// must agree with (the 32-bit emulator; read once, when the judge is loaded)
extern const bool PLAIN;

// --- fingerprints kept across runs --------------------------------------------------------------
// run()'s fingerprint, or the one it gave before under `key`.
//
// A run is a pure function of what its key names -- the module's content, the function, the
// input, whatever else the kind varies -- so a fingerprint made once can be used again: by the
// next judgement of the same original, by the next sweep. A key of None (the module has no
// content hash) is never kept. A run that was not an observation ("err") is never kept either,
// nor one `keep(fp)` turns down.
compare::Fingerprint remembered(const std::optional<py::str>& key, const std::function<compare::Fingerprint()>& run,
                                const std::function<bool(const compare::Fingerprint&)>& keep = nullptr);

// --- calls, as two builds agree on them ---------------------------------------------------------
// One call as a fingerprint records it: `name(a,b,...)`, or just `name` when how many
// arguments it takes is not known -- compare.py counts those (argc-unknown), since two calls
// alike by name alone may still differ in what they were given.
py::str call(const py::str& name, const std::optional<py::list<py::str>>& args = std::nullopt);

// A recorded call: its name (None for one that has none) and its arguments (None when their number is not known).
struct Called {
    std::optional<py::str> name;
    std::optional<py::list<int64_t>> args;
};
// the calls of a run record (emulate/run.py Call) as `Called`
py::list<Called> called(const py::list<emulate::run::Call>& seq);

// The calls a run made, in order, as a fingerprint records them. `seq` is the calls of a run
// record or pairs --, args None when their number is not known; each name goes through `name`
// (default: str) and each argument through `argument(value)`. Calls `skip(name)` says are not the
// function's own are left out. Past `cap` calls the rest are one "..." (compare.py: calls-capped).
py::list<py::str> calls(const py::list<Called>& seq, const std::function<py::str(int64_t)>& argument,
                        const std::function<py::str(const std::optional<py::str>&)>& name = nullptr,
                        const std::function<bool(const std::optional<py::str>&)>& skip = nullptr,
                        const std::optional<int64_t>& cap = std::nullopt);

// A range of the addresses of a frame (None: the kind has no such place)
using Span = std::optional<std::tuple<int64_t, int64_t>>;
// The addresses of this build's own image: a (low, high) range, or a test
using Image = std::variant<std::monostate, std::tuple<int64_t, int64_t>, std::function<bool(int64_t)>>;

// One argument value in a form two builds of the same code agree on. In this order:
//
//     "a2"     it is one of the function's own inputs (the third), when it is above `above`
//              (a small input is as likely to be a constant that happens to equal it)
//     "-1"     with `small`, a small constant, as a signed number
//     "S"      an address in one of the `frame` ranges: the stack, or anything else that is
//              "somewhere in this call's frame" and lies elsewhere in each build
//     "P"      an address in this build's own image (`image`: a (low, high) range or a test),
//              where the two builds do not have the same layout
//     "1f3a"   otherwise the value itself
//
// A kind with forms of its own (a selector, the contents of what a pointer points at) tests
// for those first and hands the rest to this.
py::str argument(int64_t v, const py::list<int64_t>& inputs = py::list<int64_t>(), const std::optional<int64_t>& above = std::nullopt,
                 bool small = false, const py::list<Span>& frame = py::list<Span>(), const Image& image = std::monostate{});

// A byte of memory a lane names: an address, or (tag, offset)
using Place = std::variant<int64_t, std::tuple<py::str, int64_t>>;
// What a place held at the end: a byte, or the token that stands for the whole word it was part of
using Held = std::variant<int64_t, py::str>;
using State = py::dict<Place, Held>;

// The memory a run left changed -- {place: final value}, holding only places that ended up
// with a value other than the one they had -- as a fingerprint carries it: how many, and a
// digest of which and what. "" when nothing changed.  (TypeError: places of both kinds cannot be sorted.)
py::str writes(const State& state);

// --- from a run record (emulate/run.py) to a fingerprint ----------------------------------------
// What `place(address)` names a write by: a key for its first byte (the next ones following it) or a function
// naming each byte (i -> key or None)
using ByteNamer = std::function<std::optional<Place>(int64_t)>;
using Where = std::variant<int64_t, std::tuple<py::str, int64_t>, ByteNamer>;

// {place: final value} of the bytes a run left with a value other than they held before it
// (run.writes, emulate/run.py), as a lane names its places. `place(address)` is asked of each
// write's first byte: None leaves the write out; a key -- an address, or (tag, offset) -- names
// its first byte, the next ones following it; a function names each byte (i -> key or None).
// Default: the address itself. `word(value)`: what a 4-byte value stands for in every one of
// its bytes when it is not a number to compare ("S", a stack address), else None. At most
// `most` bytes of a write are taken. What a byte held before is what the first write to it
// found there -- the first of every write (`everything`), the harness's and those left out
// too, or the first of those kept.
State changed(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place = nullptr,
              const std::function<std::optional<py::str>(int64_t)>& word = nullptr, int64_t most = 8, bool everything = true);

// The memory a run left changed (changed()) as a fingerprint carries it (writes()); None when
// the emulator recorded fewer writes than were made (compare.py: writes-capped).
std::optional<py::str> left(const emulate::run::Run& run, const std::function<std::optional<Where>(int64_t)>& place = nullptr,
                            const std::function<std::optional<py::str>(int64_t)>& word = nullptr, int64_t most = 8,
                            bool everything = true);

// What verify/compare.py compares, made from a run record: how it ended, `ret` (the result,
// or None for one that is not), `deps` (its calls and effects out, as text in order: calls()),
// `writes` (left()), the stack's move and `saved` (the preserved registers handed back: a bool,
// or None where a lane does not compare them) only for a run that returned; `cov`, `reach`: the
// function's own instructions run and every one run; `steps`: the lane's own field the comparer reads.
compare::Fingerprint fingerprint(const emulate::run::Run& run, const py::list<py::str>& deps,
                                 const std::optional<int64_t>& ret = std::nullopt, const std::optional<py::str>& writes = py::str(""),
                                 const std::optional<bool>& saved = std::nullopt, const py::list<int64_t>& cov = py::list<int64_t>(),
                                 const py::list<int64_t>& reach = py::list<int64_t>(),
                                 const std::optional<int64_t>& steps = std::nullopt);

// --- inputs -------------------------------------------------------------------------------------
// The argument patterns the kinds' inputs are made of. Every argument gets a DIFFERENT value
// wherever the pattern allows: with one value in every slot, a body that uses its parameters in
// the wrong order behaves the same as the right one.

// first, first + step, ...: distinct, small, increasing.
py::list<int64_t> counting(int64_t n, int64_t first = 1, int64_t step = 1);

// top, top - 1, ...: large, and negative when read as signed.
py::list<int64_t> descending(int64_t n, int64_t top);

// A value, or a function of the argument's position
using Pattern = std::variant<int64_t, std::function<int64_t(int64_t)>>;

// Arguments 0, 2, 4 ... from `even` and 1, 3, 5 ... from `odd`, each a value or a function
// of the argument's position (a pointer in one place, an integer in the next).
py::list<int64_t> alternating(int64_t n, const Pattern& even, const Pattern& odd);

constexpr int64_t MAX_GUIDED = 16;       // branch-guided inputs kept per function (verify/guided.py)
constexpr int64_t GUIDED_BUDGET = 96;    // runs of the original it may spend finding them

// Inputs that take branches of the original no seed took (verify/guided.py guided_inputs,
// with these limits; `byteorder` and `word` say how wide a word is and which way round its bytes go), or
// none when CB_NO_GUIDED is set. `run(input)` traces the original with its branches logged.
py::list<guided::Input> guided(const guided::Run& run, const py::list<guided::Input>& seeds, int64_t seed,
                               const py::str& byteorder = py::str("little"), int64_t word = 4);

constexpr int64_t PATH_BUDGET = 32;      // runs of the original the path explorer may spend (verify/concolic.py)
constexpr int64_t MAX_PATHS = 24;        // inputs it keeps, one per new path
constexpr int64_t GUIDED_REST = 32;      // runs then left to guessing, where the paths found are not all

// Inputs for the paths of the original no seed took (verify/concolic.py explore, with these
// limits): -> its Exploration, whose .kept are the inputs. `steps`: the solver's limit for one
// question, in its own steps, where not concolic.SOLVER_STEPS; `more`: runs beyond PATH_BUDGET
// (for seeds whose paths are not known beforehand, each run again with its shadow).
template <class V>
concolic::Exploration<V> explored(const std::function<std::optional<concolic::Observation>(V&)>& run, const py::list<V>& seeds,
                                  const std::function<std::optional<V>(const V&, const concolic::Values&, const concolic::Vars&)>& derive,
                                  const std::function<bool(int64_t)>& own,
                                  const py::dict<int64_t, concolic::Trail>& known = py::dict<int64_t, concolic::Trail>(),
                                  const std::optional<int64_t>& steps = std::nullopt, int64_t more = 0) {
    return concolic::explore<V>(run, seeds, derive, own, PATH_BUDGET + more, MAX_PATHS, known,
                                (steps && *steps != 0) ? *steps : concolic::SOLVER_STEPS);
}

// What an exploration adds to a verdict's limits: `paths=N`, the distinct paths the original
// took on the inputs, and `paths-all` when those are all it has (concolic.Exploration.complete).
// (None: no exploration.)
py::list<py::str> path_limits(const concolic::ExplorationStats* got);

// --- the verdict --------------------------------------------------------------------------------
// (verdict, reason, limits, detail) for one function. compare_one(info) makes the comparison
// (compare.compare, handed `info`) and returns its (verdict, reason, limits). Anything that goes
// wrong on the way makes this function inconclusive -- one function must not sink the rest --
// except a missing module: a broken setup is not an observation.
std::tuple<py::str, py::str, py::list<py::str>, py::str> judged(
    const std::function<std::tuple<py::str, py::str, py::list<py::str>>(compare::Info&)>& compare_one);

}  // namespace codebreaker::verify::lane_common
