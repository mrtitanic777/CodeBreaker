// emulate/m6502.hpp -- the 6502 machine a function is run on: its bus (64 KB, device ranges handed to callbacks and logged), its registers, the
// options and record of a run. The instructions run on CodeBreaker's one emulator (engine/pcode_cpu: run() a call at a time, M6502Cpu a step at a time,
// on 6502.sla and 65sc02.sla); the interpreter that was here was retired when the emulator matched it.
//
//     m6502::Bus bus;                                  // 64 KB, with device ranges handed to callbacks
//     m6502::RunResult r = engine::pcode_cpu::run(bus, entry, {.a = 3});    // call a function: run to its RTS
//     cb::emulate::Run record = m6502::record(r);      // the one run record the judge compares
//
// `decimal` (default off: the NES's 2A03 has no decimal mode) makes ADC and SBC follow D as a stock 6502 does. An opcode the NMOS 6502 documents nothing
// for stops the run ("stopped: ..."). What a function touches outside its own RAM is the machine's business: a Bus routes reads and writes of chosen
// ranges (the NES's PPU and APU registers) to callbacks, and logs them in order -- the effects the judge compares. A Bus and a run belong to one thread.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/m6502.hpp"
#include "emulate/run.hpp"

namespace cb::emulate::m6502 {

// The instruction set the CPU runs (decompile/m6502): the NMOS 6502, or the Lynx's 65SC02 (see the end of this comment).
using Variant = cb::decompile::m6502::Variant;

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::size_t kMemorySize = 0x10000;
inline constexpr std::uint16_t kReturnAddress = 0xFFF0;   // where a called function's RTS lands

// The processor status bits.
namespace status {
inline constexpr std::uint8_t N = 0x80, V = 0x40, U = 0x20, B = 0x10, D = 0x08, I = 0x04, Z = 0x02, C = 0x01;
}

// One access to a device range of the bus.
struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint16_t address = 0;
    std::uint8_t value = 0;
};

// 64 KB of memory, with ranges handed to callbacks. The first range that holds an address answers it; every access
// to a range is logged. Memory the Bus owns starts as zeros (or as the image given); Bus::over() uses the caller's
// 64 KB in place instead, which must outlive the Bus.
class Bus {
public:
    using ReadFn = std::function<std::uint8_t(std::uint16_t address)>;
    using WriteFn = std::function<void(std::uint16_t address, std::uint8_t value)>;

    Bus();
    // A copy of `image` placed at address 0 (UsageError past 64 KB), the rest zeros.
    explicit Bus(ByteView image);
    static Bus over(std::span<std::uint8_t> ram);
    Bus(Bus&&) noexcept = default;
    Bus& operator=(Bus&&) noexcept = default;

    // Addresses lo..hi (inclusive) go to the callbacks: a range with no read callback reads 0, with no write
    // callback ignores the write.
    void map(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {});
    // The same, with the accesses not logged: for memory behind a callback (the 2600's 128 bytes of RAM), which is state and not an effect.
    void map_quiet(std::uint16_t lo, std::uint16_t hi, ReadFn read = {}, WriteFn write = {});
    // A range that only writes are handed to the callback (logged): a read of it is the memory behind it (a cartridge's bank-select register over its ROM).
    void map_writes(std::uint16_t lo, std::uint16_t hi, WriteFn write);
    // The address lines the chip has: every address is ANDed with `mask` before anything else (the 2600's 6507 has 13, mask 0x1FFF).
    void set_address_mask(std::uint16_t mask) { mask_ = mask; }
    std::uint16_t address_mask() const { return mask_; }

    std::uint8_t read(std::uint16_t address);
    void write(std::uint16_t address, std::uint8_t value);
    // What an instruction fetched from `address` would find, without touching a device or logging: the quiet memory behind a callback (RAM the
    // machine keeps in its own state), else the Bus's memory (a logged device range's address is its memory too: reading a device is an effect).
    std::uint8_t peek(std::uint16_t address) const;

    // The address ranges whose reads are logged (a device: reading it is an effect, and an instruction fetched from it is logged like any other read).
    std::vector<std::pair<std::uint16_t, std::uint16_t>> logged_reads() const;

    std::span<std::uint8_t> memory() { return ram_; }
    std::span<const std::uint8_t> memory() const { return ram_; }
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint16_t lo, hi;
        ReadFn read;
        WriteFn write;
        bool logged;
        bool reads = true;   // false: the range answers writes only
    };
    Bytes owned_;
    std::span<std::uint8_t> ram_;
    std::vector<Range> ranges_;
    std::vector<DeviceAccess> log_;
    std::uint16_t mask_ = 0xFFFF;
};

struct Registers {
    std::uint8_t a = 0, x = 0, y = 0, s = 0xFD;
    std::uint8_t p = status::U | status::I;
    std::uint16_t pc = 0;
};

// An opcode the 6502 documents nothing for: the run cannot go on.
struct Fault {
    std::uint8_t opcode = 0;
    std::uint16_t pc = 0;
    std::string text() const;   // "undocumented opcode 02 at 8002"
};


struct RunOptions {
    std::uint8_t a = 0, x = 0, y = 0;
    std::uint8_t p = status::U | status::I;
    std::uint64_t max_steps = kMaxSteps;
    bool decimal = false;
    Variant variant = Variant::Nmos;   // Cmos: the 65SC02 (BRK clears D, the indirect JMP does not wrap, decimal results set N and Z)
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;                // why it stopped
    Registers regs;                            // as the run left them (pc: where it stopped)
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;              // the device accesses of this run, in order
    std::string exit() const;                  // "return" | "stopped: <why>" | "limit"
};

// The run as the one run record: the registers A X Y P S as results, the bus's device accesses as io, truncated when
// it hit the step limit; the RunResult is the record's `raw`.
cb::emulate::Run record(const RunResult& result);

}  // namespace cb::emulate::m6502
