// codebreaker/emulate/z80.hpp -- typed C++ port of codebreaker.emulate.z80.
//
// z80.py -- a Z80, to run functions on (Unicorn has none).
//
//     CPU cpu(bus);                               // bus: a Bus, or a 64 KB bytearray
//     cpu.step();                                 // one instruction
//     run(bus, entry, ...);                       // call a function: run to its RET; the run record
//
// Every instruction of the table in decompile/z80.py, the undocumented ones included (IXH/IXL,
// SLL, the DDCB forms that copy their result to a register, OUT (C),0), read from that table: each
// pattern ("LD (IX+d),n", "ADC HL,DE", ...) is compiled once into a function. Flags as the CPU
// sets them -- S Z H P/V N C, and the two undocumented bits (5 and 3, copied from the result, or
// from the operand where the CPU takes them from there: CP, BIT, the block instructions) -- so
// that a rebuilt program and its original leave the same F. R counts instruction fetches (its low
// seven bits); interrupts are not raised (a function is run on its own).
//
// What a function touches outside memory is the machine's business: the Z80 has its own I/O
// ports (IN, OUT, and the block forms), which go to the Bus's `inp`/`outp` and are logged with the
// memory ranges a Bus hands to callbacks -- the effects the judge compares.
//
// Notes on the port:
//   * the Python's `memory` is "a Bus or a 64 KB bytearray" (duck typing: both answer m[a] and
//     m[a] = v; only a Bus has inp/outp, which `hasattr` tests). Memory is the std::variant of the
//     two, and Bus is a py::ref<Bus> because the CPU, the caller and run() all share the one object;
//   * a Bus's callbacks are std::functions; "no callback" (None) is an empty one;
//   * the dict run() returns (fixed keys) is the struct RunRecord, its **regs keywords the struct Regs
//     (a field is set only when given), the other keywords RunOptions;
//   * a handler (what the Python compiles a pattern into) is a std::function over (CPU, Insn);
//   * Python's ints are unbounded; int64_t is the type (registers are kept in range by the code,
//     only what a caller stores into them can leave it).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "codebreaker/decompile/z80.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::z80 {

namespace table = ::codebreaker::decompile::z80;   // (the Python's `table`)
namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FS = 0x80, FZ = 0x40, FY = 0x20, FH = 0x10, FX = 0x08, FP = 0x04, FN = 0x02, FC = 0x01;
inline constexpr int64_t RETURN = 0xFFFF;          // the return address run() pushes: reaching it ends the run

class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

// (kind "r", "w", "in" or "out", address or port, value) of one I/O access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t)>;
using IoWrite = std::function<void(int64_t, int64_t)>;
using PortIn = std::function<int64_t(int64_t)>;
using PortOut = std::function<void(int64_t, int64_t)>;
// (lo, hi, read, write): an empty std::function is None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// 64 KB of memory and 256 ports (the Z80 puts the full 16-bit BC on the bus; most machines
// decode the low byte). io ranges of memory go to callbacks: map(lo, hi, read, write).
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;                        // (kind, address or port, value)

    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt, const PortIn& inp = PortIn(),
                 const PortOut& outp = PortOut());
    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite());
    int64_t getitem(int64_t a);                    // bus[a]
    void setitem(int64_t a, int64_t v);            // bus[a] = v
    int64_t inp(int64_t port);
    void outp(int64_t port, int64_t v);

  private:
    PortIn _inp;
    PortOut _outp;
};

// A Bus or a 64 KB bytearray.
using Memory = std::variant<py::bytearray, py::ref<Bus>>;

class CPU;
// a compiled pattern
using Handler = std::function<void(CPU&, const table::Insn&)>;
using ReadFn = std::function<int64_t(CPU&, const table::Insn&)>;                 // empty: None
using WriteFn = std::function<void(CPU&, const table::Insn&, int64_t)>;          // empty: None

// An instruction as decoded, with the bytes it was decoded from (to tell if memory changed under it)
struct Decoded {
    table::Insn insn;
    py::bytes bytes;
};

class CPU {
  public:
    Memory m;
    int64_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
    int64_t a_ = 0, f_ = 0, b_ = 0, c_ = 0, d_ = 0, e_ = 0, h_ = 0, l_ = 0;
    int64_t ix = 0xFFFF, iy = 0xFFFF;
    int64_t sp = 0xFFFF, pc = 0, i = 0, r = 0;
    bool iff1 = false, iff2 = false;
    int64_t im = 0;
    bool halted = false;
    int64_t steps = 0;
    py::dict<int64_t, Decoded> _decoded;

    explicit CPU(const Memory& memory);

    // ---- memory
    int64_t rd(int64_t a);
    void wr(int64_t a, int64_t v);
    int64_t rd16(int64_t a);
    void wr16(int64_t a, int64_t v);
    void push(int64_t v);
    int64_t pop();

    // ---- register pairs and halves
    int64_t get(const py::str& name);
    void set(const py::str& name, int64_t v);
    bool cond(const py::str& cc);

    // ---- one instruction
    void step();

    py::bytes _bytes_at(int64_t pc, int64_t n);

    // the plain register a name stands for (KeyError for any other name)
    int64_t& field(const py::str& name);
};

// ---- operands: (read, write, consumes a value?) for each written form
std::tuple<ReadFn, WriteFn, bool> _operand(const py::str& form, int64_t k);
// (mnemonic, [(form, read, write)])
struct Operand {
    py::str form;
    ReadFn read;
    WriteFn write;
};
std::tuple<py::str, py::list<Operand>> _operands(const py::str& pattern);

// ---- arithmetic with flags
int64_t _add8(CPU& c, int64_t a, int64_t b, int64_t carry = 0);
int64_t _sub8(CPU& c, int64_t a, int64_t b, int64_t carry = 0, bool keep = true);
void _alu(CPU& c, const py::str& op, int64_t v);
int64_t _rot(CPU& c, const py::str& op, int64_t v);
int64_t _inc8(CPU& c, int64_t v);
int64_t _dec8(CPU& c, int64_t v);
int64_t _add16(CPU& c, int64_t a, int64_t b);
int64_t _adc16(CPU& c, int64_t a, int64_t b);
int64_t _sbc16(CPU& c, int64_t a, int64_t b);
int64_t _parity(int64_t v);

// ---- the handlers
Handler _handler(const py::str& pattern);
Handler _compile(const py::str& pattern);

// the registers run() sets before it starts (the Python's **regs: a field only when given)
struct Regs {
    std::optional<int64_t> a, f, b, c, d, e, h, l, ix, iy;
};

// run's record: {end: "return" | "stopped: <why>" | "limit", a, f, bc, de, hl, ix, iy, sp, steps, io}
struct RunRecord {
    py::str end;
    int64_t a = 0, f = 0, bc = 0, de = 0, hl = 0, ix = 0, iy = 0, sp = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as CALL would and run it until it returns.
// `memory` is a Bus or a 64 KB bytearray (changed in place).
RunRecord run(const Memory& memory, int64_t entry, int64_t steps = MAX_STEPS, int64_t sp = 0xDFF0,
              const Regs& regs = Regs());

// run's record as the one run record (emulate/run.py): the registers (A F BC DE HL IX IY SP) its results,
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

}  // namespace codebreaker::emulate::z80
