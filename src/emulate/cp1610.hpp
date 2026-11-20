// emulate/cp1610.hpp -- the CP1610 as an interpreter, to run functions on (small, deterministic, and it logs what a function does to the devices).
//
//     cp1610::Bus bus;                                         // 64 K words
//     bus.memory(0x5000, 0x6FFF, rom_words, false);            // ROM (a write to it is dropped)
//     bus.device(0x0000, 0x003F, read, write, true, 0x3FFF);   // a device range: every access logged, values kept to 14 bits
//     cp1610::RunResult r = cp1610::run(bus, 0x5100, {.r = {1, 2}});   // call the function as `JSR R5` would, run to its `MOVR R5,R7`
//
// Every instruction of decompile/cp1610: the flags S Z OV C of the arithmetic (SUB, CMP and the others subtract by adding the complement, so C is set when there was no borrow),
// ADCR, NEGR, INCR / DECR / COMR (S and Z only), the shifts and rotates of R0-R3 by one or two (the overflow of SLLC and the sign of the byte operations are modelled loosely),
// GSWD / RSWD (the flags in bits 15-12 and 7-4), every addressing mode (direct, @R1-@R3, @R4 and @R5 counting up after the access, @R6 the stack: down before a read, up after a
// write, immediate), SDBD (the double-byte immediate, and a double read through @Rn that takes two words), the sixteen branch conditions (BEXT reads its external pins as low), J /
// JE / JD / JSR / JSRE / JSRD. Reading R7 gives the address of the next word. Timing and interrupts are not modelled; HLT stops a run. A run starts with the registers it is given
// and R5 and the top of the stack holding an address nothing is at; reaching it is the end.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "decompile/cp1610.hpp"

namespace cb::emulate::cp1610 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint16_t kReturnAddress = 0x4FF0;   // where a called function's return lands

// One access to a device range of the bus.
struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint16_t address = 0;
    std::uint16_t value = 0;
};

// 64 K words of address space: memory ranges (read-only or not, kept to a width) and device ranges handed to callbacks. The first range that holds an address answers it; an address
// outside every range reads 0 and ignores writes.
class Bus {
public:
    using ReadFn = std::function<std::uint16_t(std::uint16_t address)>;
    using WriteFn = std::function<void(std::uint16_t address, std::uint16_t value)>;

    // Addresses lo..hi are memory, starting as `initial` (zero padded); a write keeps `mask`'s bits only. With `logged`, every access is logged.
    void memory(std::uint16_t lo, std::uint16_t hi, std::span<const std::uint16_t> initial = {}, bool writable = true, bool logged = false, std::uint16_t mask = 0xFFFF);
    void device(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {}, bool logged = true, std::uint16_t mask = 0xFFFF);

    std::uint16_t read(std::uint16_t address);
    void write(std::uint16_t address, std::uint16_t value);

    // The words of the memory range that starts at `lo`.
    std::span<std::uint16_t> words(std::uint16_t lo);
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint16_t lo, hi;
        std::vector<std::uint16_t> data;
        bool is_memory, writable, logged;
        std::uint16_t mask;
        ReadFn read;
        WriteFn write;
    };
    std::vector<Range> ranges_;
    std::vector<DeviceAccess> log_;
};

struct Registers {
    std::array<std::uint16_t, 8> r{};     // r[7] is the program counter
    bool s = false, z = false, ov = false, c = false, i = false;
    std::uint16_t status() const { return static_cast<std::uint16_t>((s ? 8 : 0) | (z ? 4 : 0) | (ov ? 2 : 0) | (c ? 1 : 0) | (i ? 16 : 0)); }
};

struct Fault {
    std::uint16_t pc = 0;
    std::string reason;   // "undefined instruction at 5000" | "halt at 5000"
    std::string text() const { return reason; }
};

class Cpu {
public:
    explicit Cpu(Bus& bus) : bus_(&bus) {}

    Registers regs;
    bool sdbd = false;                    // the previous instruction was SDBD
    std::uint64_t steps = 0;

    // Execute the instruction at the program counter. nullopt: it ran. A Fault: it did not (nothing changed).
    std::optional<Fault> step();

private:
    Bus* bus_;
};

struct RunOptions {
    std::array<std::uint16_t, 5> r{};      // the starting R0..R4
    std::uint16_t sp = 0x02F0;             // where the stack starts
    std::uint64_t max_steps = kMaxSteps;
    std::function<void(const Registers&)> on_instruction;   // called with the registers before each instruction runs
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;
    std::string exit() const;              // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JSR R5 would and run it until it returns (to kReturnAddress through R5 or the stack), faults or takes `max_steps` instructions.
RunResult run(Bus& bus, std::uint16_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::cp1610
