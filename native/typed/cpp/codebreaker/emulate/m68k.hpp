// codebreaker/emulate/m68k.hpp -- typed C++ port of codebreaker.emulate.m68k.
//
// m68k.py -- a 68000, to run functions on.
//
//     Bus bus(...);                           // 16 MB, big-endian; map(lo, hi, read, write) hands ranges to callbacks
//     run(bus, entry, ...);                   // call a function as JSR would: run to its RTS; the run record
//
// Every instruction decompile/m68k.py decodes, read from it (each decoded instruction is cached by
// address). Flags as the 68000 sets them -- X N Z V C, with X following C where the manual says,
// ADDX/SUBX/NEGX/ABCD/SBCD/NBCD leaving Z set only if it was and the result is zero, DIVU/DIVS
// overflow leaving the destination alone and setting V -- and addresses 24 bits wide. A byte pushed
// or popped through A7 moves it by two. Exceptions (TRAP, CHK, a division by zero, ILLEGAL, line
// A/F) end the run with the reason: a function is run on its own, without the program's handlers.
//
// Notes on the port:
//   * the handler table (_HANDLERS) mixes plain functions, lambdas and factory closures (_arith("ADD")
//     ...): all of them are a Handler, a std::function over (CPU, Insn, size, operands); the prefixed
//     families (_PREFIXED) take the condition as well: a PrefixedHandler;
//   * an address (CPU.address) is None for a register or immediate operand: std::optional<int64_t>;
//     using a None address as a number is the TypeError Python raises;
//   * setf(x=, n=, z=, v=, c=) is five optional bools (None: the flag is left alone);
//   * modify's callback returns None for "do not write back": std::optional<int64_t>;
//   * the Bus's callbacks are std::functions, an empty one being None; a Bus is a py::ref<Bus> (the CPU,
//     the caller and run() share the one object); its `memory` is a py::bytearray (a shared handle, so
//     the caller's bytearray is the Bus's memory, as in Python);
//   * the dict run() returns (fixed keys) is the struct RunRecord;
//   * Python's ints are unbounded; int64_t is the type (registers are kept in range by the code, only
//     what a caller stores into them or a device callback returns can leave it).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/decompile/m68k.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::m68k {

namespace table = ::codebreaker::decompile::m68k;   // (the Python's `table`)
namespace emu_run = ::codebreaker::emulate::run;    // (this namespace has a function run(), which hides the name)

using Op = table::Op;
using Insn = table::Insn;

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FX = 0x10, FN = 0x08, FZ = 0x04, FV = 0x02, FC = 0x01;
// the return address run() pushes (even, outside a cartridge's code): reaching it ends the run
inline constexpr int64_t RETURN = 0x00FFFFF0;
extern const py::dict<py::str, int64_t> MASK;
extern const py::dict<py::str, int64_t> SIGN;
extern const py::dict<py::str, int64_t> BYTES;

class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

// (kind "r1" "r2" "r4" "w1" "w2" "w4", address, value) of one device access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t, int64_t)>;            // (address, size in bytes); empty: None
using IoWrite = std::function<void(int64_t, int64_t, int64_t)>;     // (address, size in bytes, value); empty: None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// Memory, big-endian, 24-bit addresses; ranges handed to callbacks (each access logged).
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;

    // memory: the bytearray to use (shared, not copied), else `size` bytes of zeros
    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt, int64_t size = 0x1000000);
    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite());
    int64_t read(int64_t a, int64_t n);
    void write(int64_t a, int64_t n, int64_t v);

  private:
    // the (read, write) of the first range holding `a`, or None
    std::optional<std::tuple<IoRead, IoWrite>> _io(int64_t a);
};

int64_t _sx(int64_t v, const py::str& size);

class CPU;
// a compiled instruction handler: (cpu, insn, size, operands)
using Handler = std::function<void(CPU&, const Insn&, const py::str&, const py::list<Op>&)>;
// the conditional families' handlers: (cpu, insn, condition, size, operands)
using PrefixedHandler = std::function<void(CPU&, const Insn&, const py::str&, const py::str&, const py::list<Op>&)>;
// CPU.modify's callback: the new value, or None to leave the operand alone
using ModifyFn = std::function<std::optional<int64_t>(int64_t)>;

class CPU {
  public:
    py::ref<Bus> bus;
    py::list<int64_t> d, a;
    int64_t pc = 0;
    int64_t sr = 0x2700;
    int64_t usp = 0;
    int64_t steps = 0;
    py::dict<int64_t, Insn> _cache;

    explicit CPU(const py::ref<Bus>& bus);

    // ---- flags
    int64_t flag(int64_t f) const;
    // each argument: None leaves the flag alone, else it is set or cleared
    void setf(std::optional<bool> x = std::nullopt, std::optional<bool> n = std::nullopt, std::optional<bool> z = std::nullopt,
              std::optional<bool> v = std::nullopt, std::optional<bool> c = std::nullopt);
    void nz(int64_t r, const py::str& size);
    void logic(int64_t r, const py::str& size);
    bool cond(const py::str& cc) const;

    // ---- memory
    int64_t rd(int64_t a, const py::str& size);
    void wr(int64_t a, const py::str& size, int64_t v);
    void push(int64_t v, const py::str& size = py::str("L"));
    int64_t pop(const py::str& size = py::str("L"));

    // ---- operands
    // The address an operand names (applying -(An) / (An)+), or None for a register or immediate.
    std::optional<int64_t> address(const Op& o, const py::str& size);
    int64_t get(const Op& o, const py::str& size, const std::optional<int64_t>& at = std::nullopt);
    void put(const Op& o, const py::str& size, int64_t v, const std::optional<int64_t>& at = std::nullopt);
    // Read-modify-write one operand (its address worked out once).
    void modify(const Op& o, const py::str& size, const ModifyFn& fn);

    // ---- one instruction
    void step();
};

int64_t _add(CPU& cpu, int64_t a, int64_t b, const py::str& size, int64_t x = 0, bool keep_z = false);
// a - b - x.
int64_t _sub(CPU& cpu, int64_t a, int64_t b, const py::str& size, int64_t x = 0, bool keep_z = false, bool set_x = true);
int64_t _bcd_add(CPU& cpu, int64_t a, int64_t b);
// a - b - X in BCD.
int64_t _bcd_sub(CPU& cpu, int64_t a, int64_t b);
int64_t _shift(CPU& cpu, const py::str& name, int64_t v, int64_t count, const py::str& size);

void _execute(CPU& cpu, const Insn& insn);

// ---- the handlers (a plain function, a factory's closure, or a lambda: all a Handler)
void _move(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _movea(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _moveq(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _movem(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _movep(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _lea(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _pea(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _clr(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _tst(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _tas(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _neg(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _negx(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _not(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _ext(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _swap(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _exg(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
Handler _arith(const py::str& name);
Handler _addr_arith(const py::str& name);
Handler _quick(const py::str& name);
Handler _extended(const py::str& name);
void _cmpm(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
Handler _mul(bool signed_);
Handler _div(bool signed_);
Handler _bcd(const py::str& name);
void _shift_handler(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
Handler _bit(const py::str& name);
void _branch(CPU& cpu, const Insn& insn, const py::str& cc, const py::str& size, const py::list<Op>& ops);
void _dbcc(CPU& cpu, const Insn& insn, const py::str& cc, const py::str& size, const py::list<Op>& ops);
void _scc(CPU& cpu, const Insn& insn, const py::str& cc, const py::str& size, const py::list<Op>& ops);
void _jmp(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _jsr(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _rts(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _rtr(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _rte(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _link(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _unlk(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
void _chk(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);
Handler _stop_run(const py::str& why);
void _trapv(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops);

// mnemonic -> handler
extern const py::dict<py::str, Handler> _HANDLERS;
// conditional families, by prefix: B<cc> (BRA, BSR too), DB<cc>, S<cc>
extern const py::list<std::tuple<py::str, PrefixedHandler>> _PREFIXED;
extern const py::set<py::str> _NOT_PREFIXED;

// run's record: {end: "return" | "stopped: <why>" | "limit", d: [8], a: [8], sr, steps, io}
struct RunRecord {
    py::str end;
    py::list<int64_t> d, a;
    int64_t sr = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as JSR would and run it until it returns.
// d, a: the registers to start from (None, or an empty list: all zero).
RunRecord run(const py::ref<Bus>& bus, int64_t entry, int64_t steps = MAX_STEPS,
              const std::optional<py::list<int64_t>>& d = std::nullopt, const std::optional<py::list<int64_t>>& a = std::nullopt,
              int64_t sp = 0x00FFFF00, int64_t sr = 0x2700);

// run's record as the one run record (emulate/run.py): the registers (D0-D7 A0-A7 SR) its results,
// the bus's log its device accesses. What it wrote is in the memory it was given, which its
// lane reads (verify/lanes/image_lane.py).
emu_run::Run record(const RunRecord& out);

// run(...)'s keywords
struct RunOptions {
    int64_t steps = MAX_STEPS;
    std::optional<py::list<int64_t>> d, a;
    int64_t sp = 0x00FFFF00;
    int64_t sr = 0x2700;
};

// The platform's runner (codebreaker/platforms): run(...) as a Run.
emu_run::Run recorded(const py::ref<Bus>& bus, int64_t entry, const RunOptions& kw = RunOptions());

}  // namespace codebreaker::emulate::m68k
