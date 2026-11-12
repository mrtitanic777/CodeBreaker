// cb/emulate/sm83.hpp -- the Game Boy's CPU (Sharp SM83), to run functions on.
//
//     sm83::Bus bus;                                   // the Z80 module's: 64 KB, device ranges handed to callbacks
//     sm83::Cpu cpu(bus);
//     cpu.step();                                      // one instruction
//     sm83::RunResult r = sm83::run(bus, entry, {.regs = {.a = 3}});   // call a function: run to its RET
//     cb::emulate::Run record = sm83::record(r);       // the one run record the judge compares
//
// Every instruction of cb/decompile/sm83's table, each pattern compiled once into a small program. Flags as the SM83
// keeps them: Z N H C in bits 7-4 of F, bits 3-0 always zero; 16-bit ADD sets H and C from bits 11 and 15; ADD SP,e and
// LD HL,SP+e set H and C from the low byte's unsigned add and clear Z; DAA works from N H C as the Z80's does but leaves
// H clear. The I/O registers at $FF00-$FF7F and $FFFF are the machine's: a Bus hands them to callbacks and logs each
// access (the SM83 has no I/O ports: IN and OUT do not exist).
//
// A Bus, a Cpu and a run belong to one thread; the compiled programs are shared.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/decompile/sm83.hpp"
#include "cb/emulate/run.hpp"
#include "cb/emulate/z80.hpp"

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

struct Program;   // an instruction pattern compiled (sm83.cpp)

class Cpu {
public:
    explicit Cpu(Bus& bus);
    ~Cpu();
    Cpu(const Cpu&) = delete;
    Cpu& operator=(const Cpu&) = delete;

    Registers regs;
    std::uint64_t steps = 0;   // instructions executed

    // Execute the instruction at pc. nullopt: it ran. A Fault: it did not (an earlier HALT ran, the bytes are no
    // instruction) or it was STOP (pc is already past it and the step is not counted). The instruction is decoded once
    // per address and decoded again if the bytes under it change.
    std::optional<Fault> step();

private:
    struct Decoded {
        cb::decompile::sm83::Insn insn;
        const Program* program = nullptr;
    };
    Bus* bus_;
    std::unordered_map<std::uint16_t, Decoded> decoded_;
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

// Call the function at `entry` as CALL would and run it until it returns (RET to kReturnAddress), stops or takes
// `max_steps` instructions. What the function wrote is in the bus's memory.
RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options = {});

// The run as the one run record: the registers A F BC DE HL SP as results, the bus's accesses as io, truncated when it
// hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner (platforms/gb).
cb::emulate::Run recorded(Bus& bus, std::uint16_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::sm83
