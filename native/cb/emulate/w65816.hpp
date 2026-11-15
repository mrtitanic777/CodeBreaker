// cb/emulate/w65816.hpp -- a Super Nintendo cartridge machine, to run functions on (Unicorn has no 65C816).
//
//     w65816::Machine machine(rom);                                // the cartridge's ROM, the console's 128 KB of RAM, the chips' registers
//     w65816::Run r = w65816::run(machine, 0x808123, {.a = 3});    // call a function: run to its RTS or RTL
//
// The CPU is the whole 65C816: emulation and native mode (XCE), the accumulator 8 or 16 bits wide (M) and X and Y 8 or 16 (X) by REP and SEP, the direct page
// register D, the data and program banks DBR and PBR, long addressing, the stack-relative modes, BRK and COP with their vectors, the block moves, binary
// and decimal ADC and SBC at either width. WAI and STP end a run ("stopped"). Time is not modelled.
//
// Memory is the 5A22's view of the cartridge (formats/snes.hpp): the ROM by the map the header says (LoROM or HiROM, mirrored by size), the 128 KB of RAM at
// $7E:0000-$7F:FFFF and in the first 8 KB of banks $00-$3F and $80-$BF, no cartridge RAM (reads 0, writes are lost). The chips' registers answer as an idle
// console would and every access to them is logged by name (INIDISP, VMDATAL, APUIO0, DMAP0, NMITIMEN, MDMAEN ...): the APU ports read the sound CPU's
// ready signature $AA, $BB; RDNMI and HVBJOY show vertical blank on every other read; the joypads read "no button"; the multiplier and divider work.
// A write to MDMAEN ($420B) runs the DMA channels it names -- each byte of the transfer is a read or a write of its register, so a DMA to VRAM is a run of
// VMDATAL / VMDATAH writes in the log -- and HDMA (HDMAEN) is logged and not run.
//
// A Machine and a run belong to one thread.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/snes.hpp"

namespace cb::emulate::w65816 {

inline constexpr std::uint64_t kMaxSteps = 200000;
inline constexpr std::uint16_t kReturnAddress = 0xFFD0;   // where a called function's RTS or RTL lands (inside the header, which no program runs)

namespace status {
inline constexpr std::uint8_t N = 0x80, V = 0x40, M = 0x20, X = 0x10, D = 0x08, I = 0x04, Z = 0x02, C = 0x01;
}

// One access to a chip's register.
struct Access {
    bool write = false;
    std::uint16_t address = 0;
    std::uint8_t value = 0;
    std::string text() const;   // "w INIDISP 8f", "r RDNMI 80"
};

class Machine {
public:
    // The ROM must outlive the machine.
    explicit Machine(const formats::snes::Rom& rom);

    std::array<std::uint8_t, formats::snes::kWramSize> wram{};

    std::uint8_t read(std::uint32_t address);
    void write(std::uint32_t address, std::uint8_t value);

    const std::vector<Access>& log() const { return log_; }

private:
    std::uint8_t hardware_read(std::uint16_t address);
    void hardware_write(std::uint16_t address, std::uint8_t value);
    void dma(std::uint8_t channels);

    const formats::snes::Rom& rom_;
    std::vector<Access> log_;
    std::array<std::uint8_t, 0x80> dma_regs_{};   // $4300-$437F
    std::uint32_t wram_address_ = 0;
    unsigned nmi_reads_ = 0, status_reads_ = 0;
    std::uint16_t product_ = 0, quotient_ = 0, remainder_ = 0;
    std::uint8_t multiplicand_ = 0xFF;
    std::uint16_t dividend_ = 0xFFFF;
};

struct Registers {
    std::uint16_t a = 0, x = 0, y = 0, s = 0x01FF, d = 0, pc = 0;
    std::uint8_t dbr = 0, pbr = 0;
    std::uint8_t p = status::M | status::X | status::I;
    bool e = false;   // emulation mode
};

struct Fault {
    std::string what;   // "WAI", "STP"
    std::uint32_t pc = 0;
    std::string text() const;   // "WAI at 808123"
};

struct Options {
    std::uint16_t a = 0, x = 0, y = 0;
    bool m16 = false;          // the accumulator starts 16 bits wide (native mode)
    bool x16 = false;          // the index registers start 16 bits wide
    bool native = true;        // false: emulation mode (both 8 bits)
    std::uint64_t max_steps = kMaxSteps;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct Run {
    End end = End::Limit;
    std::optional<Fault> fault;
    Registers regs;
    std::uint64_t steps = 0;
    std::vector<Access> io;
    std::string exit() const;   // "return" | "stopped: <why>" | "limit"
};

// Call the function at the 24-bit `entry` as JSL would and run it until it returns (RTS or RTL to kReturnAddress), stops or takes `max_steps` instructions.
Run run(Machine& machine, std::uint32_t entry, const Options& options = {});

}  // namespace cb::emulate::w65816
