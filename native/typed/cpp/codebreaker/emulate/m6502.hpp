// codebreaker/emulate/m6502.hpp -- typed C++ port of codebreaker.emulate.m6502.
//
// m6502.py -- a 6502, to run functions on (Unicorn has none).
//
//     cpu = CPU(memory);                          // memory: a 64 KB bytearray (or a Bus)
//     cpu.step();                                 // one instruction
//     run(memory, entry, regs);                   // call a function: run to its RTS; the run record
//
// The NMOS 6502 as documented, every one of its 151 instructions: the flags each sets (N V Z C,
// with V from signed overflow), the carry as "no borrow" in subtraction, JSR pushing the address
// of its own last byte and RTS adding one to what it pops, BRK pushing the address two past
// itself with B set, PHP pushing B and bit 5 set, PLP and RTI ignoring both, the indirect JMP
// that fetches the high byte from the start of the same page when the pointer ends a page
// ($xxFF), and zero-page indexing and pointers that wrap within page zero. `decimal` (default
// off: the NES's 2A03 has no decimal mode) makes ADC and SBC follow D as a stock 6502 does.
//
// What a function touches outside its own RAM is the machine's business: a Bus routes
// reads and writes of chosen ranges (the NES's PPU and APU registers) to callbacks, and the run
// record lists them in order -- the effects the judge compares.
//
// Notes on the port:
//   * the Python's `memory` is "a Bus or a 64 KB bytearray" (duck typing: both answer m[a] and
//     m[a] = v). Memory is the std::variant of the two, and Bus is a py::ref<Bus> because the
//     CPU, the caller and run() all share the one object;
//   * a Bus's callbacks are std::functions; "no callback" (None) is an empty one. Its log is a list of
//     (kind, address, value) tuples;
//   * the run record `run()` returns, a dict with fixed keys, is the struct RunRecord;
//     record() puts its io tuples into the run record's IO structs as (kind, port = address, value),
//     with no step and no size -- Run.io is typed as that struct, the Python's plain 3-tuple is
//     the same three fields;
//   * `recorded(memory, entry, **kw)` takes the keywords as the struct RunOptions;
//   * Python's ints are unbounded; int64_t is the type (CPU registers are kept in 0..255 by the code,
//     only what a caller stores into them can leave that range).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::m6502 {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t _N = 0x80;
inline constexpr int64_t _V = 0x40;
inline constexpr int64_t _U = 0x20;
inline constexpr int64_t _B = 0x10;
inline constexpr int64_t _D = 0x08;
inline constexpr int64_t _I = 0x04;
inline constexpr int64_t _Z = 0x02;
inline constexpr int64_t _C = 0x01;

class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

// (kind "r" or "w", address, value) of one I/O access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t)>;
using IoWrite = std::function<void(int64_t, int64_t)>;
// (lo, hi, read, write): an empty std::function is None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// 64 KB of memory, with ranges handed to callbacks: io[(lo, hi)] = (read(addr), write(addr, v)).
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;                        // (kind, address, value) of every I/O access

    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt);
    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite());
    int64_t getitem(int64_t a);                    // bus[a]
    void setitem(int64_t a, int64_t v);            // bus[a] = v
};

// A Bus or a 64 KB bytearray.
using Memory = std::variant<py::bytearray, py::ref<Bus>>;

class CPU {
  public:
    Memory m;
    int64_t a = 0, x = 0, y = 0;
    int64_t s = 0xFD, p = _U | _I, pc = 0;
    bool decimal = false;
    int64_t steps = 0;

    explicit CPU(const Memory& memory, bool decimal = false);

    // ---- memory helpers
    int64_t rd(int64_t a);
    void wr(int64_t a, int64_t v);
    int64_t rd16(int64_t a);
    int64_t rd16_zp(int64_t a);                    // a pointer in page zero wraps within it
    void push(int64_t v);
    int64_t pull();
    void flag(int64_t f, bool on);
    int64_t nz(int64_t v);

    // ---- operands
    int64_t address(const py::str& mode);
    int64_t load(const py::str& mode);
    void store_back(const py::str& mode, std::optional<int64_t> addr, int64_t v);

    // ---- arithmetic
    void adc(int64_t v);
    void sbc(int64_t v);
    void compare(int64_t reg, int64_t v);

    // ---- one instruction
    void step();

    // {"N": .., "V": .., "B": .., "D": .., "I": .., "Z": .., "C": ..}
    py::dict<py::str, int64_t> flags() const;
};

inline constexpr int64_t RETURN = 0xFFF0;          // where a called function's RTS lands

// run's record: {end: "return" | "stopped: <why>" | "limit", a, x, y, p, s, steps, io: [(kind, address, value)]}
struct RunRecord {
    py::str end;
    int64_t a = 0, x = 0, y = 0, p = 0, s = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as JSR would and run it until it returns.
// `memory` is a Bus or a 64 KB bytearray (changed in place: what the function wrote is in it).
RunRecord run(const Memory& memory, int64_t entry, int64_t a = 0, int64_t x = 0, int64_t y = 0, int64_t p = _U | _I,
              int64_t steps = MAX_STEPS, bool decimal = false);

// run's record as the one run record (emulate/run.py): the registers (A X Y P S) its results,
// the bus's log its device accesses. What it wrote is in the memory it was given, which its
// lane reads (verify/lanes/image_lane.py).
emu_run::Run record(const RunRecord& out);

// run(...)'s keywords
struct RunOptions {
    int64_t a = 0, x = 0, y = 0, p = _U | _I;
    int64_t steps = MAX_STEPS;
    bool decimal = false;
};

// The platform's runner (codebreaker/platforms): run(...) as a Run.
emu_run::Run recorded(const Memory& memory, int64_t entry, const RunOptions& kw = RunOptions());

}  // namespace codebreaker::emulate::m6502
