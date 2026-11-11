// codebreaker/analyze/observe.hpp -- typed C++ port of codebreaker.analyze.observe.
//
// observe.py -- what running a function shows that reading it does not.
//
//     auto img = image::load("comctl32.dll");
//     auto seen = observe::run(*img, 400);                  // {function: observation}
//
// Each function is run in the emulator (emulate/emulator.py) with every argument pointing at a
// buffer of its own, filled so that any value read from it says which buffer and which offset it
// came from; and once more with only the arguments the static analysis took for pointers. From
// those runs:
//
//     fields      {argument: {offset: [size, reads, writes]}}   what it touches through each pointer
//     nested      {(argument, offset): {offset: [size, reads, writes]}}   ... and through the
//                 pointers it finds there
//     indirect    [(call site, function reached)]   where `call [reg+8]` and the like really went
//                 (addresses of the module's own functions the static call graph cannot have)
//     passes      [(import, parameter, argument, offset, "address" | "value")]   a field, or the
//                 address of one, handed to a system function -- which says what the field is
//     removed     bytes of arguments the function removed on return (None when it did not return)
//     returns     ("argument", k) or ("field", k, offset) when the result is one of those
//     idiom       "strlen", "memcpy", ... when the function behaves as that one on test inputs
//
// The idiom check runs the small functions that loop and call nothing on a battery of strings and
// buffers and compares what they return and leave behind with what strlen, wcslen, strcpy, strcat,
// strcmp, stricmp, strchr, strrchr, strncpy, memcpy, memset, memcmp, memicmp and strncmp would. A function that passes every case of one of them
// behaves as it on those cases; it is evidence for a name, not a proof.
//
// 32-bit modules only: the 16-bit emulator is entered with far pointers and a data segment, and
// nothing here drives it yet. Runs go through the job pool (a native crash loses one function,
// not the analysis), at most `jobs` at a time.
//
// Typing notes:
//   * An observation is the struct Observation (the Python's dict of eight keys); a function the pool lost (None) is an empty
//     std::optional<Observation>. The memo of run() is Image::memo<...> under the key "observe:<limit>" ("observe:None" for no limit).
//   * A cell [size, reads, writes] is std::array<int64_t, 3>.
//   * analyse() returns the struct Analysis: counts, facts (a list of two kinds of record: ObservedFact, StructureObservedFact) and
//     text. A fact's keys that are only sometimes there are std::optional.
//   * The workers are the pool's threads of this process (as the pool's port says), not processes.
//   * `_trace` of a run the emulator could not make: the Trace with exit "error: <exception type>" and nothing else; an ImportError
//     is raised.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/analyze/image.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "py.hpp"

namespace codebreaker::analyze::observe {

inline constexpr int64_t BUFFERS = 0x40000000, BUFFER_SIZE = 0x1000;     // argument k points at BUFFERS + k * 0x10000
inline constexpr int64_t MARK = 0x50000000;                              // the dword at offset o of buffer k is MARK + (k << 24) + (o << 12)
inline constexpr int64_t TEXT_A = 0x41000000, TEXT_B = 0x41010000;       // the idiom battery's two buffers
inline constexpr int64_t MAX_ARGS = 8, INSNS = 2500;

using Cell = std::array<int64_t, 3>;                                     // [size, reads, writes]
using Cells = py::dict<int64_t, Cell>;                                   // offset -> cell

// ("argument", k) or ("field", k, offset): the result is one of those
struct Returns {
    py::str kind;
    int64_t k = 0;
    std::optional<int64_t> offset;
};

struct Observation {
    py::dict<int64_t, Cells> fields;                                     // argument -> its cells
    py::dict<std::tuple<int64_t, int64_t>, Cells> nested;                // (argument, offset) -> cells
    py::list<std::tuple<int64_t, int64_t>> indirect;                     // (call site, function reached)
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t, py::str>> passes;   // (import, parameter, argument, offset, "address" | "value")
    std::optional<py::str> exit;                                         // "ret" | "cut" | "fault"
    std::optional<int64_t> removed;
    std::optional<Returns> returns;
    std::optional<py::str> idiom;
};

// The dword at offset o of buffer k says which buffer and offset it came from.
py::bytes _fill(int64_t k);

// the argument count of an imported function
std::optional<int64_t> _argc(const py::str& dll, const py::str& name);

// one run of the function (see the typing notes)
emulate::emulator::Trace _trace(const py::str& path, int64_t va, const py::list<int64_t>& args,
                                const py::list<std::tuple<int64_t, py::bytes>>& mem);

// What an address in one of our buffers is: ("arg", k, offset) or ("field", k, o, offset).
struct Where {
    py::str kind;                          // "arg" | "field"
    int64_t k = 0;
    int64_t o = 0;                         // "arg": the offset; "field": the offset in the buffer
    int64_t offset = 0;                    // "field": the offset in the field's dword
};
std::optional<Where> _where(int64_t addr);

// the run's record taken into the observation
void _digest(const emulate::emulator::Trace& rec, Observation& out);

// ---- behaving like a library function
// Buffer contents after a run: the initial bytes with the run's writes applied.
py::bytes _after(const emulate::emulator::Trace& rec, int64_t base, const py::bytes& initial);
int64_t _s32(int64_t v);
int64_t _sign(int64_t x);

// (result or None when it did not return, A afterwards, B afterwards)
std::tuple<std::optional<int64_t>, py::bytes, py::bytes> _case(const py::str& path, int64_t va, const py::list<int64_t>& args, const py::bytes& a,
                                                               const py::bytes& b);

// The library function `va` behaves as on the battery, or None.
std::optional<py::str> _idiom(const py::str& path, int64_t va, const std::optional<int64_t>& nargs);

// Worker: one function. item = (address, argument count, arguments known to be pointers, try the idiom battery).
using Item = std::tuple<int64_t, std::optional<int64_t>, py::list<int64_t>, bool>;
Observation _observe(const py::str& context, const Item& item);

// {function: observation} for up to `limit` functions (None: all of them); a function the pool lost is None
using Seen = py::dict<int64_t, std::optional<Observation>>;
Seen _run(image::Image& img, const std::optional<int64_t>& limit, int64_t jobs);
std::shared_ptr<Seen> run(image::Image& img, const std::optional<int64_t>& limit = 400, int64_t jobs = 4);

// ---- the analysis
struct ObservedFact {                                                    // kind "observed"
    int64_t addr = 0;
    py::str kind = py::str("observed");
    std::optional<py::str> exit;
    std::optional<py::dict<py::str, py::dict<py::str, Cell>>> fields;    // "argument 1" -> {"+8": cell}
    std::optional<py::dict<py::str, py::dict<py::str, Cell>>> nested;    // "argument 1 +8" -> {"+4": cell}
    std::optional<py::list<py::str>> passes;
    std::optional<py::list<py::str>> indirect_calls;
    std::optional<int64_t> removed;
    std::optional<Returns> returns;
    std::optional<py::str> behaves_like;
};
struct StructureObservedFact {                                           // kind "structure_observed"; addr None
    py::str kind = py::str("structure_observed");
    py::str name;
    py::list<py::str> fields_confirmed;
    py::list<py::str> fields_added;
    int64_t fields_not_reached = 0;
};
using Fact = std::variant<ObservedFact, StructureObservedFact>;

struct Analysis {
    py::dict<py::str, int64_t> counts;
    py::list<Fact> facts;
    py::list<py::str> text;
};

Analysis analyse(image::Image& img, const std::optional<int64_t>& limit = 400, int64_t jobs = 4);

}  // namespace codebreaker::analyze::observe
