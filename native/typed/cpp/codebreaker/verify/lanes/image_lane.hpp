// codebreaker/verify/lanes/image_lane.hpp -- typed C++ port of codebreaker.verify.lanes.image_lane.
//
// image_lane.py -- the lane of a program image written back as its assembly source: a cartridge, a tape
// block, a ROM. The NES, Master System, Game Boy, Genesis, ZX Spectrum and MSX lanes are each one of these
// (verify/lanes/nes.py ...), giving only what is particular to their machine.
//
//     LANE = Lane("gb", 1, STATES, effects, WHAT, start)
//     LANE.export(globals())            # the module is then a kind reconstruct.py works with
//
// The program's source is written by its platform's writer (the packager slot of
// codebreaker/platforms/<machine>.py: source/<machine>_writer.py) and assembled; a program that comes back
// identical IS the original, so each of its functions does what the original's does. When it is not
// identical -- the source was edited -- each function is run in both, from the same starting states, and what
// it did is compared: effects(subject, entry, state) is a tuple of what the run showed (how it ended, the
// registers, the memory it left, the hardware it touched), and WHAT names each place in it, so a difference
// says which.
//
// Typing notes:
//   * A starting state is a dict of integers (State: {"a": 0, "x": 0, "fill": 1}); its repr is part of the
//     profile, written as Python writes the dict.
//   * effects() returns the tuple of what the run showed as a vector of Effect: an integer, text, bytes, or
//     a list of text (a lane with a richer item, the hardware accesses, gives it as its lines of text).
//     Two runs differ where their Effects differ; tuples of different length are different (the first place
//     that differs among the WHAT names is then the Python's `next(...)` over zip, whose StopIteration is
//     raised when all the zipped places agree).
//   * The platform's packager (no port) is the `platform` function given to the constructor: it answers the
//     writer, as Lane.platform().packager.writer is in the Python. Written is its dict: source / functions
//     (address -> function with a name) / linked / notes / same / built; a key it does not have is an empty
//     optional (`r["source"]`, `r["functions"]` and `r["same"]` are KeyError then).
//   * subjects(module_path, written) -> (original, rebuilt) is an optional function (default: the original's
//     path and the built file's).
//   * not ported: export(namespace) -- it fills a Python module's namespace with the four functions of a
//     kind (and renames them for the worker processes that import them by name); a C++ lane is used through
//     its object.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "py.hpp"

namespace codebreaker::verify::lanes::image_lane {

extern const py::str SAME;   // "the cartridge built from this source is the original, byte for byte"

using State = py::dict<py::str, int64_t>;
using Effect = std::variant<int64_t, py::str, py::bytes, py::list<py::str>>;
using Effects = std::vector<Effect>;

struct Function {
    py::str name;
};

// what the packager's writer returns (a dict)
struct Written {
    std::optional<py::str> source;                          // the assembly source file written
    std::optional<py::dict<int64_t, Function>> functions;   // address -> function
    std::optional<bool> linked;                             // the program was assembled and linked
    std::optional<py::list<py::str>> notes;                 // what the build said, the last line last
    std::optional<bool> same;                               // the built program is the original, byte for byte
    std::optional<py::str> built;                           // the built file
};

using Writer = std::function<Written(const py::str& module_path, const py::str& work)>;

struct Prepared {
    Written written;
    py::str header;                                    // the source text (read as latin-1)
    py::list<py::str> order;                           // function names, by address
    py::dict<py::str, py::str> key;                    // name -> its address as the store's keys write it
    py::list<py::str> all_keys;
    py::dict<py::str, py::str> texts;
    py::dict<py::str, int64_t> by_name;                // name -> address
    py::str sig;
};

// what judge() is given for the whole program: (same, original, rebuilt)
struct Context {
    bool same;
    py::str original;
    py::str rebuilt;
};
// a function's item: (its key, its address)
using Item = std::tuple<py::str, int64_t>;

struct Built {
    std::optional<Context> context;                    // None: it did not build
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> missing;                // {name: why it is not in the build}
};

// (verdict, what differs, [], text)
using Verdict = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

class Lane {
  public:
    using EffectsFn = std::function<Effects(const py::str& subject, int64_t entry, const State& state)>;
    using StartFn = std::function<py::str(const State& state)>;
    using SubjectsFn = std::function<std::tuple<py::str, py::str>(const py::str& module_path, const Written& written)>;

    // kind: the name results are filed under (its platform's: the writer is that platform's packager); rev:
    // bump when what a verdict depends on changes; states: where each run starts (part of every verdict's
    // profile); effects(subject, entry, state) -> a tuple; what: a name for each place in that tuple;
    // start(state) -> the state, as a verdict's detail says it; platform(): the packager's writer.
    Lane(const py::str& kind, int64_t rev, const py::list<State>& states, const EffectsFn& effects,
         const py::list<py::str>& what, const StartFn& start, const std::function<Writer()>& platform,
         const SubjectsFn& subjects = nullptr, const py::str& key = py::str("%04x"), const py::str& same = image_lane::SAME);

    py::str KIND;
    int64_t REV;
    py::str SAME;

    Writer writer() const;

    // (a digest of the settings a verdict depends on, the text it is made from)
    std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt) const;

    Prepared prepare(const py::str& module_path, const py::str& work) const;

    Built build(const Prepared& m, const py::str& module_path, const py::str& work) const;

    Verdict judge(const std::optional<Context>& context, const Item& item) const;

  private:
    py::list<State> states_;
    EffectsFn effects_;
    py::list<py::str> what_;
    StartFn start_;
    std::function<Writer()> platform_;
    SubjectsFn subjects_;
    py::str key_;
};

}  // namespace codebreaker::verify::lanes::image_lane
