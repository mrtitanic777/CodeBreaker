// emulate/m6809.hpp -- the Motorola 6809 machine a function is run on: its bus (64 KB; map(lo, hi, read, write) hands a range to callbacks, every access to it logged), its
// registers and the record of a run. The instructions run on CodeBreaker's one emulator (engine/pcode_cpu_m6809: 6809.sla); the interpreter that was here was retired when
// the emulator matched it.
//
//     m6809::Bus bus(image);
//     m6809::RunResult r = emulate::pcode_cpu::run(bus, 0x0030, {.a = 1});   // call the function as JSR would, run to its RTS
//
// SWI, SWI2, SWI3, CWAI and SYNC wait for an interrupt the model does not raise: a run reaching one stops with that reason. A run starts with the registers it is given and
// the stack pointer at `RunOptions::s`; a return address is pushed that nothing is at, and reaching it is the end.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/m6809.hpp"

namespace cb::emulate::m6809 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kMemorySize = 0x10000;
inline constexpr std::uint16_t kReturnAddress = 0xFFF0;   // where a called function's RTS lands

// The condition code register's bits.
namespace cc {
inline constexpr std::uint8_t C = 0x01, V = 0x02, Z = 0x04, N = 0x08, I = 0x10, H = 0x20, F = 0x40, E = 0x80;
}

// One access to a device range of the bus.
struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint16_t address = 0;
    std::uint8_t value = 0;
};

// 64 KB of memory, with ranges handed to callbacks. The first range that holds an address answers it; every access to a logged range is logged.
class Bus {
public:
    using ReadFn = std::function<std::uint8_t(std::uint16_t address)>;
    using WriteFn = std::function<void(std::uint16_t address, std::uint8_t value)>;

    Bus();
    // A copy of `image` placed at address 0 (UsageError past 64 KB), the rest zeros.
    explicit Bus(ByteView image);

    // Addresses lo..hi (inclusive) go to the callbacks (a range with no read callback reads 0, with no write callback ignores the write), every access logged.
    void map(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {});
    // The same, not logged: for memory behind a callback (the Vectrex's 1 KB of RAM seen twice), which is state and not an effect.
    void map_quiet(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {});

    std::uint8_t read(std::uint16_t address);
    void write(std::uint16_t address, std::uint8_t value);

    std::span<std::uint8_t> memory() { return ram_; }
    std::span<const std::uint8_t> memory() const { return ram_; }
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint16_t lo, hi;
        ReadFn read;
        WriteFn write;
        bool logged;
    };
    Bytes ram_;
    std::vector<Range> ranges_;
    std::vector<DeviceAccess> log_;
};

struct Registers {
    std::uint8_t a = 0, b = 0, dp = 0, cc = cc::I | cc::F;
    std::uint16_t x = 0, y = 0, u = 0, s = 0, pc = 0;
};

// Why the CPU cannot go on.
struct Fault {
    std::uint16_t pc = 0;
    std::string reason;   // "undefined opcode 01 at 0030" | "waits for an interrupt (SYNC) at 0042"
    std::string text() const { return reason; }
};

struct RunOptions {
    std::uint8_t a = 0, b = 0, dp = 0;
    std::uint16_t x = 0, y = 0, u = 0;
    std::uint16_t s = 0xCBFF;
    std::uint64_t max_steps = kMaxSteps;
    // Called with the registers before each instruction runs (the lane notes where the BIOS is entered).
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;                        // as the run left them (pc: where it stopped)
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;          // the device accesses of this run, in order
    std::string exit() const;              // "return" | "stopped: <why>" | "limit"
};

}  // namespace cb::emulate::m6809
