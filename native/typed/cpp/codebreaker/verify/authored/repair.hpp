// codebreaker/verify/authored/repair.hpp -- typed C++ of codebreaker.verify.authored.repair.
//
// repair.py -- recover the authored source by shrinking the byte distance to the original.
//
//     python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--json FILE]
//
// The behavioural pipeline (verify/) proves a reconstruction MEANS the same as the original, but
// many different sources mean the same thing, and the decompiler's C is only one of them -- and
// not usually the one that was written. The byte distance to the original, when it is compiled
// with a compiler of its time (verify/authored/match.py), is the gradient that tells which of the many
// behaviourally-equal sources is closest to what was written: a register variable rather than a
// stack one, declarations in the order that lays the frame out the same way, an unused local the
// author left in, the operands of a sum in the order they were typed.
//
// This is a repair, not a fresh decompile. It starts from the behaviourally-verified candidate and
// changes one thing at a time -- a KNOB -- keeping a change when it brings the bytes closer:
//
//     pad       add an unused local the size of the frame the original reserves but the draft does not
//     order     the order the local variables are declared in (the compiler lays the frame out in
//               declaration order)
//     swap      the operands of a commutative operator (a + b, a * b, a & b, ... and == / !=)
//
// Every knob is a rewrite that cannot change what the function does -- an unused local, a
// reordering of declarations, a commutative swap -- so a variant always still behaves like the
// original: the search moves toward the authored bytes without ever risking a plausible-but-wrong
// result, which was the old inverse engine's worst failure. The variants, their distances and the
// knob each changed are kept in the store (functions.variant); the closest is the reconstruction
// offered as the authored source.
//
// Byte-exactness is not the goal and not the measure -- behaviour is. This is the signal that
// chooses, among sources that all behave right, the one nearest to what a person wrote.
//
// Typing notes:
//   * `bits` (the Python: 0, 16, 32 or "ppc") is an int64_t: 0, 16, 32 or BITS_PPC.
//   * A function's compiled code (the Python's (bytes, mask)) is match::Masked: the mask of an x86 function is bytes, a PowerPC one's
//     the list of bits left out of each word.  `instructions()` gives the code's instructions as `Insns`: byte strings (x86) or words (PowerPC).
//   * The Python's generators (_tests, _knobs) are lists: they are pure, and the search walks them to their end or stops at its limits, as the
//     Python's does.
//   * repair(compile, ...): `compile` is a std::function text -> Masked or nullopt (build/period.py's compilers).  The Python's `profile` of
//     truth() (units(), compile(text, work), bits) is `Profile`; build/period.py has no typed port, so main()'s --truth takes how to find a
//     profile as `Periods` (its names() and get()).
//   * run() / run_powerpc() read the lanes' prepared modules directly (verify/lanes/win32, powerpc); `log` is the Python's `log=print`: an empty
//     std::function is None (nothing logged).  The functions are repaired on threads of this process (ThreadPoolExecutor), one folder each.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "py.hpp"

namespace codebreaker::verify::authored::repair {

using match::Masked;
using Log = std::function<void(const py::str&)>;
void log_print(const py::str& line);   // print
using FunctionStore = engine::native_engine::FunctionStore;

inline constexpr int64_t MAX_LOCALS = 6;   // the leading locals whose order and storage are tried
inline constexpr int64_t MAX_SITES = 6;    // places in the body each kind of rewrite is tried at
inline constexpr int64_t MAX_WORK = 300;   // compiles per function, a ceiling on the search
inline constexpr int64_t BEAM = 3;         // the sources the search goes on from, each round
inline constexpr int64_t PATIENCE = 2;     // rounds without a better source before the search stops
inline constexpr int64_t BITS_PPC = -1;    // bits="ppc"

// (everything up to and including the opening brace, the declaration lines, the rest).
std::tuple<py::str, py::list<py::str>, py::str> _split(const py::str& source);

// The bytes a function reserves for locals: the N of `sub esp, N` (`sub sp, N`) in its prologue (PowerPC: of `stwu r1,-N(r1)`), 0 when there is none.
int64_t _frame(const py::bytes& code, int64_t bits = 32);

// A function's code as its instructions, each the bytes of one with the linker's bytes blanked -- what two versions of a function are lined up by.
// (PowerPC: each word, with the bits the linker fills in cleared.)
using Insns = std::variant<py::list<py::bytes>, py::list<int64_t>>;
Insns instructions(const Masked& built, int64_t bits);

// ((the instructions that have no counterpart, the bytes that differ), (verdict, bytes that differ, total)): how far a compiled function is from
// the original, smaller the nearer.
using Distance = std::tuple<py::str, int64_t, int64_t>;
using Score = std::tuple<std::tuple<int64_t, int64_t>, Distance>;
Score score(const Masked& built, const Masked& original, int64_t bits = 0, const std::optional<Insns>& target = std::nullopt);

// The text with its strings, characters and comments blanked, the same length.
py::str _blank(const py::str& text);
int64_t _depth(const py::str& shadow, int64_t at);

// (start, end, rewritten) for each `if (A op B)` / `while (A op B)` whose whole condition is one comparison of two things that do nothing but have
// a value: (B flipped-op A).
py::list<std::tuple<int64_t, int64_t, py::str>> _tests(const py::str& shadow, const py::str& body);

std::optional<py::str> _toggle_register(const py::str& line);

// (name of the knob, rewritten source). Each rewrite preserves what the function does.
py::list<std::tuple<py::str, py::str>> _knobs(const py::str& source, int64_t target_frame, const std::optional<int64_t>& draft_frame);

// What the search found for one function.
struct Repaired {
    py::str source;
    py::str verdict;
    int64_t distance = 0;
    int64_t total = 0;
    py::str knobs;
    int64_t tried = 0;
};

using Compile = std::function<std::optional<Masked>(const py::str& text)>;

// Search behaviour-preserving rewrites of one function's source for the bytes closest to the original's. `bits`: the processor mode of the code: with
// it the search lines the two up by instruction and reads the size of the locals. nullopt when the starting source does not compile.
std::optional<Repaired> repair(const Compile& compile, const py::str& source, const Masked& original, int64_t bits = 0);

// repair() with Microsoft's 32-bit compiler of 1994 (verify/authored/match.py): the setting the function is compiled in is chosen first.
std::optional<Repaired> repair_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                        const py::str& name, const Masked& original, const py::str& work, const py::str& flags);

// A period compiler's unit: a source known to be the authored one, and the bytes it compiled to.
struct Unit {
    py::str name;
    py::str source;
    Masked original;
};
// What build/period.py's get() gives: the units, a compile of a text (in a work folder) and the processor mode of its code.
struct Profile {
    std::function<py::list<Unit>()> units;
    std::function<std::optional<Masked>(const py::str& text, const py::str& work)> compile;
    int64_t bits = 0;
};
// build/period.py's names() and get(*args), which have no typed port
struct Periods {
    std::function<py::list<py::str>()> names;
    std::function<Profile(const py::list<py::str>& args)> get;
};

// Test the search on sources KNOWN to be the authored ones. -> (units, identical as they stand, knocked away, brought back).
std::tuple<int64_t, int64_t, int64_t, int64_t> truth(const Profile& profile, int64_t limit = 0, const Log& log = log_print);

// settings.state("repair") -- the folder repairs are worked in
py::str _scratch();

// The compiler switches the module was most likely built with: the set under which most of a sample of its functions come out as the original's
// bytes, or at least the same length.
py::str best_flags(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::dict<py::str, py::str>& texts,
                   const py::list<py::str>& order, const py::dict<int64_t, match::ByteMasked>& originals, const py::dict<py::str, int64_t>& address,
                   const py::str& work, int64_t sample = 60);

// (functions, identical, same size, different)
using Counts = std::tuple<int64_t, int64_t, int64_t, int64_t>;

// run() for a PowerPC image, with the Xbox 360 compiler (verify/authored/match.py).
Counts run_powerpc(const py::str& module_path, const std::optional<py::str>& flags, const py::str& work,
                   const std::shared_ptr<FunctionStore>& fs = nullptr, const Log& log = log_print, int64_t limit = 0);

// Repair every function of a module and record the variants. `flags` None: the switches are found first (best_flags).
Counts run(const py::str& module_path, const std::optional<py::str>& flags, const py::str& work, const std::shared_ptr<FunctionStore>& fs = nullptr,
           const Log& log = log_print, int64_t limit = 0);

extern const py::str DOC;
// python -m codebreaker repair <module> [--flags "/O2 /Gz"] [--first N]   (--truth <compiler> [target])
int64_t main(const py::list<py::str>& argv, const Periods& periods = Periods());

}  // namespace codebreaker::verify::authored::repair
