// emulate/f8.hpp -- the Fairchild F8 machine a function is run on: its bus (64 KB of memory, ports through callbacks, every port access logged), its registers and the record of
// a run. The instructions run on CodeBreaker's one emulator (engine/pcode_cpu_f8: f8.sla and what the adapter adds); the interpreter that was here was retired when the emulator
// matched it.
//
//     f8::Bus bus;                                    // 64 KB of memory, ports through callbacks, every port access logged
//     bus.load(0x0800, rom);                          // the cartridge
//     f8::RunResult r = engine::pcode_cpu::run(bus, 0x0820, {.a = 1});    // call the function as PI would, run to its POP
//
// A run starts with PC1 holding kReturnAddress and ends when POP brings it back.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/f8.hpp"

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

}  // namespace cb::emulate::f8
