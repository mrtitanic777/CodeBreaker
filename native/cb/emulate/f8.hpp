// cb/emulate/f8.hpp -- the Fairchild F8 as an interpreter, to run functions on (small, deterministic, and it logs what a function does to the devices).
//
//     f8::Bus bus;                                    // 64 KB of memory, ports through callbacks, every port access logged
//     bus.load(0x0800, rom);                          // the cartridge
//     f8::RunResult r = f8::run(bus, 0x0820, {.a = 1});    // call the function as PI would, run to its POP
//
// Every instruction of decompile/f8: the accumulator and the status register W (S sign, set when the result is positive; C carry; Z zero; O overflow; ICB the interrupt bit),
// the 64-byte scratchpad with its direct registers 0-11 and the indirect S / I / D through ISAR (I and D count its low three bits after the access), the pairs K Q H and J W, the
// data counters DC0 / DC1 (LM, ST, AM, AMD, NM, OM, XM, CM, DCI, XDC, and ADC, which adds the accumulator to DC0), the program counters PC0 / PC1 (PI, PK, POP, JMP), the shifts, the binary and decimal adds (ASD and AMD adjust as
// the F8 does: the decimal carry and the low-digit carry are modelled), every branch, and the ports (IN, OUT, INS, OUTS), each access logged. PI and JMP overwrite the accumulator with
// the high byte of the address, as the F8 does. Interrupts are not modelled. A run starts with PC1 holding an address nothing is at and ends when POP brings it back.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/decompile/f8.hpp"

namespace cb::emulate::f8 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint16_t kReturnAddress = 0xFFF0;   // where a called function's POP lands

namespace w {
inline constexpr std::uint8_t S = 1, C = 2, Z = 4, O = 8, ICB = 16;
}

struct PortAccess {
    enum class Kind : std::uint8_t { In, Out };
    Kind kind = Kind::In;
    std::uint8_t port = 0;
    std::uint8_t value = 0;
};

class Bus {
public:
    using InFn = std::function<std::uint8_t(std::uint8_t port)>;
    using OutFn = std::function<void(std::uint8_t port, std::uint8_t value)>;

    Bus() : memory_(0x10000, 0), writable_(0x10000, false) {}

    // Put `bytes` in memory at `address` (UsageError past 64 KB); with `writable`, stores to it are kept (otherwise dropped).
    void load(std::uint16_t address, ByteView bytes, bool writable = false);
    void make_writable(std::uint16_t lo, std::uint16_t hi);
    void set_in(InFn f) { in_ = std::move(f); }
    void set_out(OutFn f) { out_ = std::move(f); }

    std::uint8_t read(std::uint16_t address) const { return memory_[address]; }
    void write(std::uint16_t address, std::uint8_t value) {
        if (writable_[address]) memory_[address] = value;
    }
    std::uint8_t port_in(std::uint8_t port);
    void port_out(std::uint8_t port, std::uint8_t value);
    ByteView memory() const { return memory_; }
    const std::vector<PortAccess>& log() const { return log_; }

private:
    Bytes memory_;
    std::vector<bool> writable_;
    InFn in_;
    OutFn out_;
    std::vector<PortAccess> log_;
};

struct Registers {
    std::uint8_t a = 0, status = 0, isar = 0;
    std::uint16_t pc0 = 0, pc1 = 0, dc0 = 0, dc1 = 0;
    std::array<std::uint8_t, 64> scratch{};
};

struct Fault {
    std::uint16_t pc = 0;
    std::string reason;   // "undefined opcode 2D at 0800"
    std::string text() const { return reason; }
};

class Cpu {
public:
    explicit Cpu(Bus& bus) : bus_(&bus) {}

    Registers regs;
    std::uint64_t steps = 0;

    std::optional<Fault> step();

private:
    Bus* bus_;
};

struct RunOptions {
    std::uint8_t a = 0;
    std::array<std::uint8_t, 12> r{};         // the starting scratchpad registers 0-11
    std::uint64_t max_steps = kMaxSteps;
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;
    std::uint64_t steps = 0;
    std::vector<PortAccess> io;
    std::string exit() const;   // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as PI would and run it until a POP reaches kReturnAddress, it faults or takes `max_steps` instructions.
RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::f8
