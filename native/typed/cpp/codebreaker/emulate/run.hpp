// codebreaker/emulate/run.hpp -- C++ port of codebreaker.emulate.run.
//
// run.py -- one record of a run, whichever emulator made it.
//
// Every platform's emulator slot (codebreaker/platforms: p.emulator.run) returns a Run: the same
// fields under the same names, whether the run was the 32-bit tracer's, the 16-bit one's, a
// driver's, the PowerPC interpreter's or one of the console machines'. A judge (verify/lanes/*.py)
// turns it into what verify/compare.py compares -- through lane_common.fingerprint, lane_common.left
// and lane_common.calls -- and gives only what is its own: how it names a call, which places it
// leaves out, what of the result is a result.
//
//     run.exit        how it ended, in the emulator's own words ("ret", "exception: ...", "return")
//     run.ended       the same as the comparer says it: "ret", "fault", "truncated", or the word
//                     before a colon (ended())
//     run.results     {register: value} the result registers as the run left them ("EAX", "AX" and
//                     "DX:AX", "r3" and "f1", a machine's A X Y ...)
//     run.writes      [Write] the memory it wrote, in order (address, size, value, what the bytes
//                     held just before -- None when not known --, harness: written by the emulator
//                     for the run, not by the code: a stand-in's return through zeros)
//     run.capped      more writes were made than were recorded: the writes are not all there
//     run.calls       [Call] the calls out of the code, in order: name, args (None when their
//                     number is not known), detail (the emulator's own note of it)
//     run.io          [IO] accesses to devices, in order: step, kind ("in", "out", ...), port,
//                     value, size (a machine's bus log: its own tuples)
//     run.stack       how far the stack pointer moved from entry to the end (None: not known)
//     run.preserved   {register: (on entry, at the end)} the registers a function must hand back,
//                     None when they were not read
//     run.path        the addresses of the instructions that ran (in order where the emulator keeps
//                     the order)
//     run.steps       instructions it took
//     run.truncated   cut off at the step limit (or another of the emulator's caps)
//     run.endian      how a written value's bytes lie in memory ("little", "big")
//     run.raw         the emulator's own record, for what only one lane asks of it
//
// A Run is built by its emulator's `record(...)` from the record the emulator always made (those
// stay what they were: other tools read them).
//
// Typing notes (where the Python mixes kinds in one slot):
//   Register   a result register holds an int, a float (PowerPC f1) or None (a driver's missing EAX).
//   Detail     a Call's detail is None, a text, a (device, service) pair, or the 16-bit tracer's
//              {"api": ..., "n": ...} note.
//   raw        the emulator's own record: an opaque handle, never looked into here.
#pragma once

#include "py.hpp"

namespace codebreaker::emulate::run {

using Register = std::variant<py::none_t, int64_t, double>;
using Word = std::variant<py::str, int64_t>;
using Detail = std::variant<py::none_t, py::str, std::tuple<std::optional<py::str>, Word>, py::dict<py::str, Word>>;

// Write = namedtuple("Write", "address size value old harness")
struct Write {
    int64_t address = 0;
    int64_t size = 0;
    int64_t value = 0;
    std::optional<int64_t> old;
    bool harness = false;
};

// Call = namedtuple("Call", "name args detail")
struct Call {
    std::optional<py::str> name;
    std::optional<py::list<int64_t>> args;
    Detail detail;
};

// IO = namedtuple("IO", "step kind port value size")
struct IO {
    std::optional<int64_t> step;
    py::str kind;
    int64_t port = 0;
    int64_t value = 0;
    std::optional<int64_t> size;
};

using Preserved = py::dict<py::str, std::tuple<std::optional<int64_t>, std::optional<int64_t>>>;

// How a run ended, as the comparer says it: "none" (no word), "ret", "fault" (an exception),
// else the word before any colon ("truncated", "wild", "int:21" -> "int").
py::str ended(const std::optional<py::str>& e);

class Run {
  public:
    std::optional<py::str> exit;
    py::dict<py::str, Register> results;
    py::list<Write> writes;
    bool capped = false;
    py::list<Call> calls;
    py::list<IO> io;
    std::optional<int64_t> stack;
    std::optional<Preserved> preserved;
    py::list<int64_t> path;
    std::optional<int64_t> steps;
    bool truncated = false;
    py::str endian;
    std::shared_ptr<void> raw;   // the emulator's own record (opaque); Python's default is {}

    Run(const std::optional<py::str>& exit_, const std::optional<py::dict<py::str, Register>>& results_ = std::nullopt,
        const py::list<Write>& writes_ = py::list<Write>(), bool capped_ = false,
        const py::list<Call>& calls_ = py::list<Call>(), const py::list<IO>& io_ = py::list<IO>(),
        std::optional<int64_t> stack_ = std::nullopt, const std::optional<Preserved>& preserved_ = std::nullopt,
        const py::list<int64_t>& path_ = py::list<int64_t>(), std::optional<int64_t> steps_ = std::nullopt,
        bool truncated_ = false, const py::str& endian_ = py::str("little"), std::shared_ptr<void> raw_ = nullptr);

    py::str ended() const;

    // Whether the registers a function must hand back came back as they went in (those of
    // `names`, else every one recorded); False when they were not read.
    bool kept(const std::optional<py::list<py::str>>& names = std::nullopt) const;

    // Python's __repr__
    py::str repr() const;
};

}  // namespace codebreaker::emulate::run
