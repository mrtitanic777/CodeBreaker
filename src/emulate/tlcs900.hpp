// emulate/tlcs900.hpp -- the Toshiba TLCS-900/H's machine as the Neo Geo Pocket lane runs functions on it: the Bus (memory and device ranges, every device access
// logged), the registers (the four banks, SR, F', the DMA control registers), the options of a run and its result. The processor is CodeBreaker's one emulator on
// tlcs900exact.sla (engine/pcode_cpu_tlcs900: pcode_cpu::run(bus, entry, options)); the interpreter that was here is retired.
//
//     tlcs900::Bus bus;                                           // 24 address lines
//     bus.memory(0x004000, 0x006BFF, {}, true);                    // work RAM
//     bus.device(0x008000, 0x008FFF);                              // a device range: every access logged
//     tlcs900::RunResult r = emulate::pcode_cpu::run(bus, entry, {.xwa = 1});   // call the function as `call` would, run to its `ret`
//
// `swi n` is the BIOS: it is stubbed -- it logs the number and the register state (the function number is RW3) in `RunResult::swis` and goes on, with `RunOptions::on_swi`
// free to supply a result. Timing, interrupts and the other on-chip devices are not modelled. `halt`, an undefined bit pattern and a run past `max_steps` stop a run. A run
// starts with the registers it is given and a return address pushed that nothing is at; reaching it is the end.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "core/bytes.hpp"

namespace cb::emulate::tlcs900 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint32_t kReturnAddress = 0xFFFFE0;   // where a called function's `ret` lands (inside the BIOS ROM area)

// F's bits.
namespace flag {
inline constexpr std::uint8_t S = 0x80, Z = 0x40, H = 0x10, V = 0x04, N = 0x02, C = 0x01;
}

struct DeviceAccess {
    enum class Kind : std::uint8_t { Read, Write };
    Kind kind = Kind::Read;
    std::uint32_t address = 0;
    std::uint8_t size = 1;   // bytes: 1, 2 or 4
    std::uint32_t value = 0;
};

// An address space: memory ranges (read-only or not, mirrored over `size`) and device ranges handed to callbacks. The first range that holds an address answers it; an address
// outside every range reads 0 and ignores writes. Addresses are masked to 24 bits. Reads and writes are little-endian and may be unaligned.
class Bus {
public:
    using ReadFn = std::function<std::uint32_t(std::uint32_t address, unsigned size)>;
    using WriteFn = std::function<void(std::uint32_t address, unsigned size, std::uint32_t value)>;

    // Addresses lo..hi are memory: `initial` (zero padded to `size`, which is `hi - lo + 1` unless given) repeated over the range. With `logged`, every access is logged; without
    // `writable`, a write is dropped.
    void memory(std::uint32_t lo, std::uint32_t hi, ByteView initial = {}, bool writable = true, bool logged = false, std::size_t size = 0);
    // Addresses lo..hi go to the callbacks, every access logged unless `logged` is false.
    void device(std::uint32_t lo, std::uint32_t hi, ReadFn read = {}, WriteFn write = {}, bool logged = true);

    std::uint32_t read(std::uint32_t address, unsigned size);
    void write(std::uint32_t address, unsigned size, std::uint32_t value);

    // The bytes of the memory range that starts at `lo`.
    std::span<std::uint8_t> bytes(std::uint32_t lo);
    const std::vector<DeviceAccess>& log() const { return log_; }

private:
    struct Range {
        std::uint32_t lo, hi;
        Bytes data;          // memory; empty for a device
        bool writable;
        bool logged;
        ReadFn read;
        WriteFn write;
    };
    Range* find(std::uint32_t address);
    std::vector<Range> ranges_;
    std::vector<DeviceAccess> log_;
};

struct Registers {
    std::array<std::array<std::uint32_t, 4>, 4> bank{};   // [RFP][XWA XBC XDE XHL]
    std::uint32_t xix = 0, xiy = 0, xiz = 0, xsp = 0;
    std::uint32_t pc = 0;
    std::uint16_t sr = 0;                                  // the flags F in the low byte, the high byte: SYSM, IFF, MAX, RFP
    std::uint8_t f_alt = 0;                                // F'
    std::array<std::uint32_t, 4> dmas{}, dmad{};
    std::array<std::uint16_t, 4> dmac{};
    std::array<std::uint8_t, 4> dmam{};
    std::uint16_t intnest = 0;

    unsigned rfp() const { return (sr >> 8) & 3u; }
    std::uint8_t f() const { return static_cast<std::uint8_t>(sr); }
    // A register by its code (see decompile/tlcs900.hpp: rcode_name) and size.
    std::uint32_t read(std::uint8_t code, unsigned size) const;
    void write(std::uint8_t code, unsigned size, std::uint32_t value);
    std::uint32_t& xwa() { return bank[rfp()][0]; }
    std::uint32_t& xbc() { return bank[rfp()][1]; }
    std::uint32_t& xde() { return bank[rfp()][2]; }
    std::uint32_t& xhl() { return bank[rfp()][3]; }
    std::uint32_t xwa() const { return bank[rfp()][0]; }
};

// One BIOS call (`swi n`).
struct SoftwareInterrupt {
    std::uint8_t number = 0;
    std::uint32_t pc = 0;                                  // the swi's address
    std::uint8_t function = 0;                             // RW3 (register bank 3's W): the BIOS function the call asks for
    std::array<std::uint32_t, 4> regs{};                   // XWA XBC XDE XHL of the current bank when it was made
};

// Why the CPU cannot go on.
struct Fault {
    std::uint32_t pc = 0;
    std::string reason;   // "undefined instruction at 200120" | "halt at ..."
    std::string text() const { return reason; }
};

struct RunOptions {
    std::uint32_t xwa = 0, xbc = 0, xde = 0, xhl = 0;      // the current bank's registers
    std::uint32_t xix = 0, xiy = 0, xiz = 0;
    std::vector<std::uint16_t> args;                       // words pushed for the callee in order: the last one ends up next to the return address, at (XSP+4)
    std::uint32_t sp = 0x006C00;
    std::uint16_t sr = 0xF300;                             // SYSM, IFF 7, register bank 3
    std::uint64_t max_steps = kMaxSteps;
    std::function<void(Registers&, const SoftwareInterrupt&)> on_swi;
    // Called with the registers before each instruction runs.
    std::function<void(const Registers&)> on_instruction;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct RunResult {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;                        // as the run left them (pc: where it stopped)
    std::uint64_t steps = 0;
    std::vector<DeviceAccess> io;          // the device accesses of this run, in order
    std::vector<SoftwareInterrupt> swis;   // the BIOS calls of this run, in order
    std::string exit() const;              // "return" | "stopped: <why>" | "limit"
};

}  // namespace cb::emulate::tlcs900
