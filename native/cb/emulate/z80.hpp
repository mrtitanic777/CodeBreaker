// cb/emulate/z80.hpp -- a Z80, to run functions on (Unicorn has none).
//
//     z80::Bus bus;                                    // 64 KB, 64K ports, device ranges handed to callbacks
//     z80::Cpu cpu(bus);
//     cpu.step();                                      // one instruction
//     z80::RunResult r = z80::run(bus, entry, {.regs = {.b = 3}});    // call a function: run to its RET
//     cb::emulate::Run record = z80::record(r);        // the one run record the judge compares
//
// Every instruction of the table in cb/decompile/z80, the undocumented ones included (IXH/IXL, SLL, the DDCB forms
// that copy their result to a register, OUT (C),0), read from that table: each pattern ("LD (IX+d),n", "ADC HL,DE", ...)
// is compiled once into a small program. Flags as the CPU sets them -- S Z H P/V N C, and the two undocumented bits
// (5 and 3, copied from the result, or from the operand where the CPU takes them from there: CP, BIT, the block
// instructions) -- so that a rebuilt program and its original leave the same F. R counts instruction fetches (its low
// seven bits); interrupts are not raised (a function is run on its own).
//
// What a function touches outside memory is the machine's business: the Z80 has its own I/O ports (IN, OUT, and the
// block forms), which go to the Bus's port callbacks and are logged with the memory ranges a Bus hands to callbacks --
// the effects the judge compares. A Bus, a Cpu and a run belong to one thread; the compiled programs are shared.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/decompile/z80.hpp"
#include "cb/emulate/run.hpp"

namespace cb::emulate::z80 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kMemorySize = 0x10000;
inline constexpr std::uint16_t kReturnAddress = 0xFFFF;   // the return address run() pushes: reaching it ends the run

// The flag bits of F.
namespace flag {
inline constexpr std::uint8_t S = 0x80, Z = 0x40, Y = 0x20, H = 0x10, X = 0x08, P = 0x04, N = 0x02, C = 0x01;
}

// One access to a device: a mapped memory range (Read, Write: the address) or a port (In, Out: the low byte of the port).
struct BusAccess {
    enum class Kind : std::uint8_t { Read, Write, In, Out };
    Kind kind = Kind::Read;
    std::uint16_t where = 0;
    std::uint8_t value = 0;
};
std::string_view to_text(BusAccess::Kind kind);   // "r", "w", "in", "out"

// 64 KB of memory and the ports (the Z80 puts the full 16-bit BC on the bus; most machines decode the low byte, and
// the callbacks get all 16 bits). Memory ranges go to callbacks: map(lo, hi, read, write); the first range that holds an
// address answers it. Every access to a range or a port is logged, in order. Memory the Bus owns starts as zeros (or
// the image given); Bus::over() uses the caller's 64 KB in place, which must outlive the Bus.
class Bus {
public:
    using ReadFn = std::function<std::uint8_t(std::uint16_t address)>;
    using WriteFn = std::function<void(std::uint16_t address, std::uint8_t value)>;
    using PortIn = std::function<std::uint8_t(std::uint16_t port)>;           // none: IN reads 0xFF
    using PortOut = std::function<void(std::uint16_t port, std::uint8_t value)>;

    explicit Bus(PortIn in = {}, PortOut out = {});
    // A copy of `image` placed at address 0 (UsageError past 64 KB), the rest zeros.
    explicit Bus(ByteView image, PortIn in = {}, PortOut out = {});
    static Bus over(ByteSpan ram, PortIn in = {}, PortOut out = {});
    // Memory only: no ports are wired at all, so IN reads 0xFF and OUT does nothing, and neither is logged.
    static Bus bare(ByteSpan ram);
    Bus(Bus&&) noexcept = default;
    Bus& operator=(Bus&&) noexcept = default;

    // Addresses lo..hi (inclusive) go to the callbacks: a range with no read callback reads 0, with no write callback
    // ignores the write.
    void map(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {});

    std::uint8_t read(std::uint16_t address);
    void write(std::uint16_t address, std::uint8_t value);
    std::uint8_t in(std::uint16_t port);
    void out(std::uint16_t port, std::uint8_t value);

    ByteSpan memory() { return ram_; }
    ByteView memory() const { return ram_; }
    const std::vector<BusAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint16_t lo, hi;
        ReadFn read;
        WriteFn write;
    };
    Bytes owned_;
    ByteSpan ram_;
    std::vector<Range> ranges_;
    std::vector<BusAccess> log_;
    PortIn port_in_;
    PortOut port_out_;
    bool ports_ = true;
};

struct Registers {
    std::uint8_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
    std::uint8_t a2 = 0, f2 = 0, b2 = 0, c2 = 0, d2 = 0, e2 = 0, h2 = 0, l2 = 0;   // the alternate set (EX AF,AF' and EXX)
    std::uint16_t ix = 0xFFFF, iy = 0xFFFF, sp = 0xFFFF, pc = 0;
    std::uint8_t i = 0, r = 0;
    bool iff1 = false, iff2 = false;
    std::uint8_t im = 0;
    bool halted = false;

    std::uint16_t af() const { return static_cast<std::uint16_t>(a << 8 | f); }
    std::uint16_t bc() const { return static_cast<std::uint16_t>(b << 8 | c); }
    std::uint16_t de() const { return static_cast<std::uint16_t>(d << 8 | e); }
    std::uint16_t hl() const { return static_cast<std::uint16_t>(h << 8 | l); }
};

// Why the CPU cannot go on.
struct Fault {
    enum class Kind : std::uint8_t { Halt, Undecodable };
    Kind kind = Kind::Halt;
    std::uint16_t pc = 0;
    std::string text() const;   // "HALT" | "undecodable at 8000"
};

struct Program;   // an instruction pattern compiled (z80.cpp)

class Cpu {
public:
    explicit Cpu(Bus& bus);
    ~Cpu();
    Cpu(const Cpu&) = delete;
    Cpu& operator=(const Cpu&) = delete;

    Registers regs;
    std::uint64_t steps = 0;   // instructions executed

    // Execute the instruction at pc. nullopt: it ran. A Fault: the CPU is halted (an earlier HALT ran), nothing ran.
    // The instruction is decoded once per address and decoded again if the bytes under it change.
    std::optional<Fault> step();

private:
    struct Decoded {
        cb::decompile::z80::Insn insn;
        const Program* program = nullptr;
    };
    Bus* bus_;
    std::unordered_map<std::uint16_t, Decoded> decoded_;
};

// The registers run() sets before it starts: a field only when given.
struct RegisterInit {
    std::optional<std::uint8_t> a, f, b, c, d, e, h, l;
    std::optional<std::uint16_t> ix, iy;
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

// The run as the one run record: the registers A F BC DE HL IX IY SP as results, the bus's accesses as io, truncated
// when it hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

// run + record: the platform's runner (platforms/sms, msx, zx).
cb::emulate::Run recorded(Bus& bus, std::uint16_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::z80
