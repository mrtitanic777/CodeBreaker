// emulate/i8048.hpp -- the Intel 8048 machine a function is run on: its bus (4 KB of program memory, ports and external data through callbacks, every access logged), its
// registers and the record of a run. The instructions run on CodeBreaker's one emulator (engine/pcode_cpu_i8048: 8048.sla and what the adapter adds); the interpreter that
// was here was retired when the emulator matched it.
//
//     i8048::Bus bus;                                  // 4 KB of program memory, ports and external data through callbacks, every access logged
//     bus.load(0x400, rom);                            // the cartridge at $0400
//     i8048::RunResult r = engine::pcode_cpu::run(bus, 0x0420, {.a = 1});     // call the function as CALL would, run to its RET
//
// Interrupts, the timer and the counter are not modelled. Reaching the return address a run was given is its end; an undefined opcode stops it.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/i8048.hpp"

namespace cb::emulate::i8048 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint16_t kReturnAddress = 0x0FFE;   // where a called function's RET lands

// The places a device access names: the data space (MOVX), a port or expander port, a test pin.
enum class Place : std::uint8_t { Data, Bus, P1, P2, Dbb, P4, P5, P6, P7, T0, T1, Int };

struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    Place place = Place::Data;
    std::uint8_t address = 0;   // the data-space address (Place::Data); 0 otherwise
    std::uint8_t value = 0;
};

class Bus {
public:
    using ReadFn = std::function<std::uint8_t(Place place, std::uint8_t address)>;
    using WriteFn = std::function<void(Place place, std::uint8_t address, std::uint8_t value)>;

    Bus() : program_(4096, 0) {}

    // Put `bytes` in program memory at `address` (UsageError past 4 KB).
    void load(std::uint16_t address, ByteView bytes);
    void set_read(ReadFn f) { read_ = std::move(f); }
    void set_write(WriteFn f) { write_ = std::move(f); }

    std::uint8_t program(std::uint16_t address) const { return program_[address & 0xFFF]; }
    std::uint8_t read(Place place, std::uint8_t address = 0);
    void write(Place place, std::uint8_t value, std::uint8_t address = 0);
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    Bytes program_;
    ReadFn read_;
    WriteFn write_;
    std::vector<DeviceAccess> log_;
};

struct Registers {
    std::uint8_t a = 0;
    std::uint16_t pc = 0;
    bool c = false, ac = false, f0 = false, f1 = false, rb1 = false;   // the carry, auxiliary carry, flags 0 and 1, register bank 1 selected
    bool mb1 = false;                                                    // memory bank 1 selected (SEL MB1)
    std::uint8_t sp = 0;                                                 // the stack pointer, 0-7
    std::uint8_t timer = 0;
    bool timer_flag = false, timer_on = false, counter_on = false, ext_int = false, timer_int = false;
    std::uint8_t p1 = 0xFF, p2 = 0xFF;                                   // the port output latches
    std::array<std::uint8_t, 64> ram{};
    std::uint8_t psw() const { return static_cast<std::uint8_t>((c ? 0x80 : 0) | (ac ? 0x40 : 0) | (f0 ? 0x20 : 0) | (rb1 ? 0x10 : 0) | 0x08 | sp); }
};

struct Fault {
    std::uint16_t pc = 0;
    std::string reason;   // "undefined opcode 01 at 0420"
    std::string text() const { return reason; }
};

struct RunOptions {
    std::uint8_t a = 0;
    std::array<std::uint8_t, 8> r{};          // the starting R0..R7 of bank 0
    std::uint64_t max_steps = kMaxSteps;
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;
    std::string exit() const;   // "return" | "stopped: <why>" | "limit"
};

}  // namespace cb::emulate::i8048
