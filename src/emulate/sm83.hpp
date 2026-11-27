// emulate/sm83.hpp -- the Game Boy's CPU (Sharp SM83): the machine's side of a run (the bus, the registers, the run's options and result, and the run record the
// judge compares). The CPU itself runs on the one emulator, the p-code interpreter of the SM83's SLEIGH description (engine/pcode_cpu_sm83: run() calls a function,
// Sm83Cpu steps it).
//
//     sm83::Bus bus;                                   // the Z80 module's: 64 KB, device ranges handed to callbacks
//     sm83::RunResult r = engine::pcode_cpu::run(bus, entry, sm83::RunOptions{.regs = {.a = 3}});   // call a function: run to its RET
//     cb::emulate::Run record = sm83::record(r);       // the one run record the judge compares
//
// Flags as the SM83 keeps them: Z N H C in bits 7-4 of F, bits 3-0 always zero. The I/O registers at $FF00-$FF7F and $FFFF are the machine's: a Bus hands them to
// callbacks and logs each access (the SM83 has no I/O ports: IN and OUT do not exist).
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "emulate/run.hpp"
#include "emulate/z80.hpp"

namespace cb::emulate::sm83 {

// The Game Boy's bus is the Z80's: 64 KB and mapped ranges, every access to a range logged in order.
using Bus = z80::Bus;
using BusAccess = z80::BusAccess;

inline constexpr std::uint64_t kMaxSteps = 200000;
// The return address run() pushes: reaching it ends the run (an address no program returns to: the end of high RAM).
inline constexpr std::uint16_t kReturnAddress = 0xFFFE;

// The flag bits of F.
namespace flag {
inline constexpr std::uint8_t Z = 0x80, N = 0x40, H = 0x20, C = 0x10;
}

struct Registers {
    std::uint8_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
    std::uint16_t sp = 0xFFFE, pc = 0x0100;
    bool ime = false;
    bool halted = false;

    std::uint16_t af() const { return static_cast<std::uint16_t>(a << 8 | f); }
    std::uint16_t bc() const { return static_cast<std::uint16_t>(b << 8 | c); }
    std::uint16_t de() const { return static_cast<std::uint16_t>(d << 8 | e); }
    std::uint16_t hl() const { return static_cast<std::uint16_t>(h << 8 | l); }
};

// Why the CPU cannot go on.
struct Fault {
    enum class Kind : std::uint8_t { Halt, Stop, Undecodable };
    Kind kind = Kind::Halt;
    std::uint16_t pc = 0;
    std::string text() const;   // "HALT" | "STOP" | "no instruction at 8000"
};

// The registers run() sets before it starts: a field only when given. F keeps only its high nibble.
struct RegisterInit {
    std::optional<std::uint8_t> a, f, b, c, d, e, h, l;
    std::optional<bool> ime, halted;
};

struct RunOptions {
    std::uint64_t max_steps = kMaxSteps;
    std::uint16_t sp = 0xDFF0;
    RegisterInit regs;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                // why it stopped
    Registers regs;                            // as the run left them
    std::uint64_t steps = 0;
    std::vector<BusAccess> io;                 // the device accesses of this run, in order
    std::string exit() const;                  // "return" | "stopped: <why>" | "limit"
};

// The run as the one run record: the registers A F BC DE HL SP as results, the bus's accesses as io, truncated when it
// hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

}  // namespace cb::emulate::sm83
