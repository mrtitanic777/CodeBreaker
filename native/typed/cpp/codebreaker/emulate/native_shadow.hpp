// codebreaker/emulate/native_shadow.hpp -- interface of codebreaker.emulate.native_shadow (typed C++; implementation pending).
//
// native_shadow.py -- the path explorer's symbolic shadow of an x86 run in C++ (native/tracer: cbtrace.dll,
// shadow.cc), through ctypes.
//
//     sh = native_shadow.Shadow(16 | 32 | 33, lifter, inputs)   # Shadow16 / Shadow32 / ShadowLE
//
// The same object emulate/shadow.py's x86 shadows were: enter(), step(), after_call(), written(), forget(), segbase();
// and after the run path, trail, vars, lost, pins, steps, off -- read back from the DLL as the same values (the
// conditions as emulate/symbolic.py's expressions). A recorder in the same DLL (emulator.trace_function,
// windows16.trace_function16, driver.run) is handed `handle` and steps it itself (handle_of()).
//
// Typing notes:
//   * The values a run's shadow hands back are records, shared with emulate/shadow.py's PowerPC shadow (shadow.hpp
//     includes this header): PathEntry (shadow.path's (site, taken, condition, kind) tuple), Var (shadow.vars' (kind,
//     where, bits, value) tuple), Step (shadow.trail's (site, taken)).  verify/concolic.hpp has the same three under
//     its own names (PathEntry, Vars, Trail) for the Observation a lane builds from them.
//   * Conditions are emulate::symbolic::ExprP (the DLL's node table is read back into the same expressions).
//   * `m`, the running machine, is the Unicorn engine `Uc` (core16_types.hpp); enter()'s places are a Place record
//     (the Python's (where, value[, k]) tuple: where an address or a register's name; k the argument's number or, for a
//     register input, its name).
//   * ctypes (_Config, _Result, the LIFT callback, the DLL's entry points) is private to the implementation: the
//     class holds the DLL's handle and the configuration as opaque pointers.
//   * Python's Shadow.__del__ frees the handle: ~Shadow().
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/symbolic.hpp"
#include "py.hpp"

namespace codebreaker::emulate::native_shadow {

using core16::Uc;

// shadow.path: one condition the run's path went by.  kind: "jump" (a conditional jump: taken or not), "inner" (one
// inside an instruction's p-code: a string instruction's count), "pin" (an address or selector held at its value).
struct PathEntry {
    int64_t site = 0;
    bool taken = false;
    symbolic::ExprP cond;
    py::str kind;
};
// shadow.trail: (site, taken) of every conditional jump, in order
using Step = std::tuple<int64_t, bool>;
// shadow.vars[name] = (kind, where, bits, value): kind "arg" (where: the argument's index), "reg" (where: the
// register's name), "mem" (where: the address), "ret" (what an unmodelled call returned; where None) or "carry".
struct Var {
    py::str kind;
    std::variant<py::none_t, int64_t, py::str> where;   // (None, an index/address, or a register name)
    int64_t bits = 0;
    int64_t value = 0;
};
using Vars = py::dict<py::str, Var>;

// One argument place for enter(): Python's `(where, value)` or `(where, value, k)`.
//   where  a memory address (int) or a register's name
//   k      the argument's number (an input "a<k>"), or a register input's own name; absent: the place's position
struct Place {
    std::variant<int64_t, py::str> where;
    int64_t value = 0;
    std::optional<std::variant<int64_t, py::str>> k;
};

// The shadow for this process: the DLL loads here (the Python hooks it replaced are gone;
// require() raises with why_not() when it does not).
bool enabled();
std::optional<py::str> why_not();

class Shadow;
// The DLL's handle of a shadow a recorder can step itself, or None (a Python shadow).  Python raised TypeError for a
// shadow that is not native_shadow.Shadow; here the type says so.  Marks the shadow's cached export stale (the
// recorder steps it from here on).  null = None.
void* handle_of(Shadow* shadow);

// One run's shadow in the DLL. kind 16 (a 16-bit run; selectors' bases read from the GDT), 32 (flat), 33 (a driver's
// flat ring-0 run: a service call also sets the carry flag to `carry`).
class Shadow {
  public:
    // lifter: emulate/pcode.py's for the processor; inputs: [(low, high)] -- the memory bytes that are inputs of the
    // run until written; span: (low, high) of the function's own code (its jumps are the path's)
    Shadow(int64_t kind, std::shared_ptr<pcode::Lifter> lifter, const py::list<std::tuple<int64_t, int64_t>>& inputs,
           const std::optional<std::tuple<int64_t, int64_t>>& span = std::nullopt, bool real_mode = false);
    Shadow(const Shadow&) = delete;
    Shadow& operator=(const Shadow&) = delete;
    ~Shadow();

    std::shared_ptr<pcode::Lifter> lifter;
    std::optional<std::tuple<int64_t, int64_t>> span;
    int64_t kind = 0;
    void* handle = nullptr;   // the DLL's handle (Python: self.handle)

    // --- what the runner calls ---------------------------------------------------------------------------
    // As shadow.Shadow.enter: argument k, `bits` wide, is (where, value[, k]).
    void enter(const py::list<Place>& places, int64_t bits = 16);
    void step(Uc& m, int64_t address, int64_t cs_base = 0);
    void after_call(Uc& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at = std::nullopt,
                    int64_t nargs = 0, const std::optional<py::str>& why = std::nullopt, int64_t word = 2,
                    int64_t carry = 0);
    void written(int64_t at, int64_t size);
    void forget(const py::list<py::str>& names);
    int64_t segbase(Uc& m, int64_t sel);

    // --- what the run made (read back from the DLL on first use after the run) ------------------------------
    py::list<PathEntry> path();
    py::list<Step> trail();
    Vars vars();
    py::set<py::str> lost();    // why the conditions may not be all there is (empty: they are)
    int64_t pins();
    int64_t steps();
    bool off();

  private:
    struct Export;                                  // (path, trail, vars, lost, pins, steps, off), read once
    const Export& _export();
    std::shared_ptr<Export> got_;                   // Python: self._got
    std::shared_ptr<void> config_;                  // the DLL's configuration (ctypes _Config) and what it points at
    std::function<int(const void*, int32_t, uint64_t, void*, int32_t)> lift_cb_;   // the LIFT callback (_lift)
    py::list<py::str> names_;                       // the lifter's register names
};

// The recorder is C++ only (the Python hooks it replaced are gone): raise (RuntimeError) when the DLL cannot
// be used here, saying why.
void require();

}  // namespace codebreaker::emulate::native_shadow
