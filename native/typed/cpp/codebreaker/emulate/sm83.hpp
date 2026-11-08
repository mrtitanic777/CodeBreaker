// codebreaker/emulate/sm83.hpp -- typed C++ port of codebreaker.emulate.sm83.
//
// sm83.py -- the Game Boy's CPU, to run functions on.
//
//     CPU cpu(bus);                               // bus: a Bus, or a 64 KB bytearray
//     cpu.step();                                 // one instruction
//     run(bus, entry, ...);                       // call a function: run to its RET; the run record
//
// Every instruction of decompile/sm83.py's table, each pattern compiled once into a function.
// Flags as the SM83 keeps them: Z N H C in bits 7-4 of F, bits 3-0 always zero; 16-bit ADD sets H
// and C from bits 11 and 15; ADD SP,e and LD HL,SP+e set H and C from the low byte's unsigned add
// and clear Z; DAA works from N H C as the Z80's does but leaves H clear. The I/O registers at
// $FF00-$FF7F and $FFFF are the machine's: a Bus hands them to callbacks and logs each access.
//
// Notes on the port:
//   * `from codebreaker.emulate.z80 import Bus, Stop` is the typed port of emulate/z80 (Bus and Stop are
//     those very classes here, as sm83.Bus is z80.Bus in Python);
//   * the Python's `memory` is "a Bus or a 64 KB bytearray" (duck typing: both answer m[a] and m[a] = v);
//     Memory is the std::variant of the two, as in the z80 port, and a Bus is a py::ref<Bus> (the CPU, the
//     caller and run() share the one object);
//   * the dict run() returns (fixed keys) is the struct RunRecord, its **regs keywords the struct Regs
//     (a field is set only when given), the other keywords RunOptions;
//   * a handler (what the Python compiles a pattern into) is a std::function over (CPU, Insn); a missing
//     reader or writer (None) is an empty one, and calling it is the TypeError Python raises;
//   * CPU.get / CPU.set take the register names the Python's getattr/setattr reach: the pairs AF BC DE HL,
//     SP, and any attribute name in either case: a f b c d e h l sp pc steps ime halted. A name that is no
//     attribute is the AttributeError of get; set of one stores it (Python adds an attribute) and a later
//     get of it reads it back. ime and halted are bools (set stores v & 0xFF != 0, get gives 0 or 1); the
//     attributes that are objects (m, _decoded, the methods) are not values and so not reachable by get;
//   * Python's ints are unbounded; int64_t is the type (registers are kept in range by the code, only
//     what a caller stores into them can leave it).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/decompile/sm83.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/z80.hpp"

namespace codebreaker::emulate::sm83 {

namespace table = ::codebreaker::decompile::sm83;   // (the Python's `table`)
namespace emu_run = ::codebreaker::emulate::run;    // (this namespace has a function run(), which hides the name)

using Bus = ::codebreaker::emulate::z80::Bus;       // sm83.Bus: the same 64 KB bus with mapped ranges
using Stop = ::codebreaker::emulate::z80::Stop;
using Memory = ::codebreaker::emulate::z80::Memory; // a Bus or a 64 KB bytearray

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FZ = 0x80, FN = 0x40, FH = 0x20, FC = 0x10;
inline constexpr int64_t RETURN = 0xFFFE;   // pushed by run(): reaching it ends the run (an address no program returns to: high RAM's end)

class CPU;
// a compiled pattern
using Handler = std::function<void(CPU&, const table::Insn&)>;
using ReadFn = std::function<int64_t(CPU&, const table::Insn&)>;                 // empty: None
using WriteFn = std::function<void(CPU&, const table::Insn&, int64_t)>;          // empty: None

class CPU {
  public:
    Memory m;
    int64_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
    int64_t sp = 0xFFFE, pc = 0x0100;
    bool ime = false;
    bool halted = false;
    int64_t steps = 0;
    py::dict<int64_t, table::Insn> _decoded;
    py::dict<py::str, int64_t> _extra;   // attributes set() added by a name that is none of the above

    explicit CPU(const Memory& memory);

    int64_t rd(int64_t a);
    void wr(int64_t a, int64_t v);
    void push(int64_t v);
    int64_t pop();

    int64_t get(const py::str& name);
    void set(const py::str& name, int64_t v);
    bool cond(const py::str& cc);

    void step();
};

// (register pairs: name -> (high, low))
extern const py::dict<py::str, std::tuple<py::str, py::str>> _PAIRS;
extern const py::set<py::str> _REG8;

// ---- operands: (read, write, consumes a value?) for each written form
std::tuple<ReadFn, WriteFn, bool> _operand(const py::str& form, int64_t k);

// ---- the handlers (a pattern compiled once)
Handler _handler(const py::str& pattern);

void _alu(CPU& c, const py::str& op, int64_t v);
int64_t _rot(CPU& c, const py::str& op, int64_t v);
int64_t _sp_plus(CPU& c, int64_t e);

Handler _compile(const py::str& pattern);

// the registers run() sets before it starts (the Python's **regs: a field only when given)
struct Regs {
    std::optional<int64_t> a, f, b, c, d, e, h, l;
    std::optional<bool> ime, halted;
};

// io of one run: (kind "r" or "w", address, value) of each access to a mapped range
using IoAccess = ::codebreaker::emulate::z80::IoAccess;

// run's record: {end: "return" | "stopped: <why>" | "limit", a, f, bc, de, hl, sp, steps, io}
struct RunRecord {
    py::str end;
    int64_t a = 0, f = 0, bc = 0, de = 0, hl = 0, sp = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as CALL would and run it until it returns.
RunRecord run(const Memory& memory, int64_t entry, int64_t steps = MAX_STEPS, int64_t sp = 0xDFF0, const Regs& regs = Regs());

// run's record as the one run record (emulate/run.py): the registers (A F BC DE HL SP) its results,
// the bus's log its device accesses. What it wrote is in the memory it was given, which its
// lane reads (verify/lanes/image_lane.py).
emu_run::Run record(const RunRecord& out);

// run(...)'s keywords
struct RunOptions {
    int64_t steps = MAX_STEPS;
    int64_t sp = 0xDFF0;
    Regs regs;
};

// The platform's runner (codebreaker/platforms): run(...) as a Run.
emu_run::Run recorded(const Memory& memory, int64_t entry, const RunOptions& kw = RunOptions());

}  // namespace codebreaker::emulate::sm83
