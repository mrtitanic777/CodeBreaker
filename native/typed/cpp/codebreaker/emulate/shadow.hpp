// codebreaker/emulate/shadow.hpp -- interface of codebreaker.emulate.shadow (typed C++; implementation pending).
//
// shadow.py -- which values of a run are made from its inputs, and from which: a run's symbolic shadow, kept up beside
// the emulator instruction by instruction.
//
//     sh = shadow::Shadow16(lifter, segbase, inputs)    # a 16-bit run in Unicorn (windows16)
//     sh = shadow::Shadow32(lifter, inputs)             # a flat 32-bit run in Unicorn (emulator::trace_function)
//     sh = shadow::ShadowLE(lifter, inputs)             # a driver's flat ring-0 run in Unicorn (driver.run)
//     sh = ShadowPPC(lifter, inputs)                    # a PowerPC run in CodeBreaker's interpreter (powerpc.run)
//     ... the runner calls sh.enter(), sh.step() before each instruction, sh.after_call() ...
//     sh.path    [PathEntry]  the conditions the run's path went by (a jump's, an instruction's inner one, a pin)
//     sh.trail   [Step]       every conditional jump, in order
//     sh.vars    {name: Var}  the inputs the conditions are over
//     sh.lost    why the conditions may not be all there is (empty: they are)
//
// The x86 shadows are C++ already (native/cb/emulate/tracer shadow.cpp, through emulate/native_shadow.hpp): Shadow16, Shadow32 and
// ShadowLE are factory functions returning a native_shadow::Shadow.  `Shadow` here is the PowerPC run's (its View is
// PowerPCView: the interpreter's registers and memory, read without disturbing them).
//
// Typing notes:
//   * PathEntry / Step / Var / Place are native_shadow.hpp's (the same values from either shadow).
//   * The PowerPC machine `m` is emulate::powerpc::Machine; Python's `view` object (only PowerPCView exists) is the
//     concrete class PowerPCView.  `inputs(address)` and `segbase(machine, selector)` are std::function.
//   * Shadow16's `segbase` is windows16::selector_base or real_mode -- the only two it accepts (ValueError for any
//     other): a function pointer of type SegBase, compared by identity as in the Python.
//   * The Python's mutable attributes (reg, mem, touched, vars, path, trail, lost, pins, steps, off, pending, lifted)
//     are public data members, as in Python (the explorer reads path/trail/vars/lost/pins/steps/off).
//   * A register or memory byte's symbolic holder `(expr, k)` -- byte k of expr -- is ByteOf.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/powerpc.hpp"
#include "codebreaker/emulate/symbolic.hpp"
#include "py.hpp"

namespace codebreaker::emulate::shadow {

using core16::Uc;

using native_shadow::PathEntry;
using native_shadow::Place;
using native_shadow::Step;
using native_shadow::Var;
using native_shadow::Vars;

// conditions kept per run; past it the run is followed no further
inline constexpr int64_t _CAP_PATH = 400;

// The lift mode (emulate/pcode.py) for a platform's cpu slot (codebreaker/platforms: its SLEIGH language, bits, byte
// order), or None where there is no lift: "x86-16-real" (x86:LE:16), "x86-32-flat" (x86:LE:32), "ppc-be-64"
// (PowerPC:BE).  (`cpu` is str() of the slot.)  used by: verify.lanes.driver / powerpc / win32
std::optional<py::str> mode(const py::str& cpu);

// The view of a run in CodeBreaker's PowerPC interpreter (emulate/powerpc.py): r0-r31, LR, CTR, the condition
// register's eight fields, XER's bits, f0-f31 as the interpreter keeps them; memory big-endian, read without making
// pages.
class PowerPCView {
  public:
    py::str endian = py::str("big");
    int64_t mask = 0xFFFFFFFF;
    int64_t addresses = 0xFFFFFFFF;   // (64-bit registers, 32-bit addresses: the interpreter's M32)
    int64_t page = 0;

    PowerPCView();
    bool known(const py::str& name) const;
    uint64_t reg(const powerpc::Machine& m, const py::str& name) const;     // (f registers: their 64 raw bits)
    py::bytes mem(const powerpc::Machine& m, int64_t at, int64_t size) const;
    py::bytes code(const powerpc::Machine& m, int64_t address) const;
};

// A register byte or memory byte made from an input: byte k of an expression.
using ByteOf = std::tuple<symbolic::ExprP, int64_t>;

// Python's _Stop (raised inside _run to abandon an instruction; never leaves the class)
struct _Stop : py::Exception {
    _Stop() : py::Exception() {}
};

// The shadow of one run. `lifter`: emulate/pcode.py's for the processor; `view`: how the emulator's registers and
// memory are read (PowerPCView); `inputs(address)`: whether a byte of memory is one of the run's inputs until
// written; `span`: (low, high) addresses of the function's own code (its jumps are the path's); `segbase(machine,
// selector)`: the base a selector names (a flat program's are all 0). `result`: [(register, byte of it, byte of
// `ret`)] -- where an unmodelled call's result lands; `clobbered`: the registers such a call sets (anything else it
// leaves as it was); `ret_bits`: how wide `ret` is.
class Shadow {
  public:
    using SegBaseFn = std::function<int64_t(powerpc::Machine&, int64_t)>;
    using InputsFn = std::function<bool(int64_t)>;

    Shadow(std::shared_ptr<pcode::Lifter> lifter, const PowerPCView& view, const InputsFn& inputs,
           const std::optional<std::tuple<int64_t, int64_t>>& span = std::nullopt, const SegBaseFn& segbase = nullptr,
           const py::list<std::tuple<py::str, int64_t, int64_t>>& result = py::list<std::tuple<py::str, int64_t, int64_t>>(),
           const py::list<py::str>& clobbered = py::list<py::str>(), int64_t ret_bits = 32);
    virtual ~Shadow() = default;

    std::shared_ptr<pcode::Lifter> lifter;
    PowerPCView view;
    InputsFn inputs;
    std::optional<std::tuple<int64_t, int64_t>> span;
    py::list<std::tuple<py::str, int64_t, int64_t>> result;
    py::list<py::str> clobbered;
    int64_t ret_bits = 32;
    bool little = false;
    int64_t mask = 0;
    py::dict<int64_t, int64_t> bases;                       // selector -> base, as read this run (until a call)
    py::dict<int64_t, ByteOf> reg;                          // register byte -> (expr, k)
    py::dict<int64_t, ByteOf> mem;                          // byte of memory -> (expr, k)
    py::set<int64_t> touched;                               // bytes the run wrote (no longer inputs)
    Vars vars;
    py::list<PathEntry> path;
    py::list<Step> trail;
    py::set<py::str> lost;
    int64_t pins = 0;
    int64_t steps = 0;
    bool off = false;
    // a conditional jump awaiting its outcome: (site, target, fallthrough, the condition it makes in the path or None
    // (null), the direction the specification says it goes)
    struct Pending {
        int64_t site = 0, target = 0, fall = 0;
        symbolic::ExprP entry;
        bool guess = false;
    };
    std::optional<Pending> pending;
    py::dict<std::tuple<py::bytes, int64_t>, std::shared_ptr<const pcode::Lifted>> lifted;
    // register bytes -> (register name, shift): byte k of a register
    py::dict<int64_t, std::tuple<py::str, int64_t>> places;
    py::dict<py::str, int64_t> named;                       // register name -> its offset in the lifter's register file

    // The base `sel` names (read once, until the environment next answers a call).
    int64_t segbase(powerpc::Machine& m, int64_t sel);
    // The run is about to start: argument k, `bits` wide, is (where, value[, k]).
    void enter(const py::list<Place>& places, int64_t bits = 16);
    // Registers the environment set (not an instruction): no longer made from anything.
    void forget(const py::list<py::str>& names);
    // A call the environment answered: the `clobbered` registers were set, the result ones to `value` -- `ret`, an
    // input, when `symbolic_result`.  (Python's after_call(*a, **k) -> _safely(_after_call): nothing may change the run.)
    void after_call(powerpc::Machine& m, bool symbolic_result, int64_t value,
                    const std::optional<int64_t>& args_at = std::nullopt, int64_t nargs = 0,
                    const std::optional<py::str>& why = std::nullopt, int64_t word = 2);
    // Before the instruction at `address` (its segment based at `cs_base`) runs.
    void step(powerpc::Machine& m, int64_t address, int64_t cs_base = 0);
    // Memory the environment wrote (not an instruction): no longer an input, nor made from one.
    void written(int64_t at, int64_t size);

  private:
    symbolic::ExprP var(const py::str& name, int64_t bits, const py::str& kind,
                        const std::variant<py::none_t, int64_t, py::str>& where, int64_t value);
    py::list<int64_t> _bytes(int64_t at, int64_t size) const;   // the addresses of a value's bytes, least significant first
    void _forget_regs(const py::list<py::str>& names);
    void _after_call(powerpc::Machine& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at,
                     int64_t nargs, const std::optional<py::str>& why, int64_t word);
    void _step(powerpc::Machine& m, int64_t address, int64_t cs_base);
    void _unknown(const py::str& why);
    void _run(powerpc::Machine& m, int64_t address, int64_t cs_base, const pcode::Lifted& ins);
    // One p-code operation of values (concrete, symbolic or None) -> (concrete, symbolic or None).  (module-level _arith)
    std::pair<uint64_t, symbolic::ExprP> _arith(const std::string& opc, int64_t osize,
                                                const std::vector<std::pair<uint64_t, symbolic::ExprP>>& vals,
                                                const py::list<pcode::Input>& args);
    py::dict<py::str, symbolic::ExprP> sym_;                // Python: self._sym
    SegBaseFn segbase_fn_;                                  // Python: self._segbase
};

// The shadow of one PowerPC run (emulate/powerpc.py run, `shadow`): a call out of the code given returns `ret`
// (64 bits) in r3 and changes nothing else. Arguments: r3-r10 (enter).
class ShadowPPC : public Shadow {
  public:
    ShadowPPC(std::shared_ptr<pcode::Lifter> lifter, const InputsFn& inputs,
              const std::optional<std::tuple<int64_t, int64_t>>& span = std::nullopt);
};

// A selector's base in real mode (Shadow16's `segbase` for a real-mode run): sel * 16.
int64_t real_mode(Uc& uc, int64_t sel);

// A selector's base for Shadow16: windows16::selector_base (the GDT's) or real_mode -- the only two.
using SegBase = int64_t (*)(Uc&, int64_t);

// --- the x86 shadows: in C++ (native/cb/emulate/tracer shadow.cpp, through emulate/native_shadow), stepped by the recorder of the
// run they are handed to. `inputs`: [(low, high)] -- the memory bytes that are inputs of the run until written.
using Inputs = py::list<std::tuple<int64_t, int64_t>>;
using Span = std::optional<std::tuple<int64_t, int64_t>>;
// A 16-bit run's (windows16::trace_function16): `segbase` -- windows16::selector_base or real_mode (ValueError otherwise).
std::shared_ptr<native_shadow::Shadow> Shadow16(const std::shared_ptr<pcode::Lifter>& lifter, SegBase segbase,
                                                const Inputs& inputs, const Span& span = std::nullopt);
// A flat 32-bit run's (emulator::trace_function).
std::shared_ptr<native_shadow::Shadow> Shadow32(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs,
                                                const Span& span = std::nullopt);
// A driver's flat ring-0 run's (driver.run): a service call also sets the carry flag.
std::shared_ptr<native_shadow::Shadow> ShadowLE(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs,
                                                const Span& span = std::nullopt);

}  // namespace codebreaker::emulate::shadow
