// emulate/huc6280.hpp -- a PC Engine / TurboGrafx-16 HuCard machine, to run functions on (Unicorn has no HuC6280).
//
//     huc6280::Machine machine(rom.data);          // the card's ROM in 8 KB banks; RAM, the mapping registers and the chips' registers
//     huc6280::Run r = huc6280::run(machine, 0xE010, {.a = 3});    // call a function: run to its RTS
//
// The CPU is the 65C02 of emulate/m6502 (the Cmos variant, decimal mode on) with page zero and the stack moved to $2000 and $2100, and what the
// HuC6280 adds: the register swaps SXY SAX SAY, CLA CLX CLY, CSL CSH (no effect here), SET (the next ORA, AND, EOR or ADC works on memory at zero
// page,X), ST0 ST1 ST2 (stores to the VDC, whatever the mapping), TAM and TMA (the mapping registers), TST, BSR, RMBn SMBn BBRn BBSn, the five block
// transfers (TII TDD TIN TIA TAI, a length of 0 being 65536), and BRK taking its vector at $FFF6. Time is not modelled.
//
// Memory is the eight mapping registers' view of the physical banks (formats/pce.hpp): ROM banks $00-$7F, RAM $F8 (8 KB; $F9-$FB repeat it, as on
// the PC Engine), the hardware page $FF. Anywhere else reads $FF and ignores writes. The hardware page's chips answer as an idle console would: the
// VDC's status shows the vertical-blank bit on every other read, the joypad port reads "no button", the other registers 0; every access to the page is
// logged with its register's name -- and, for the VDC's data port, the register ST0 or a store to $0000 selected.
//
// A Machine and a run belong to one thread.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "emulate/m6502.hpp"

namespace cb::emulate::huc6280 {

inline constexpr std::size_t kBankSize = 0x2000;
inline constexpr std::uint16_t kReturnAddress = 0xFFF0;   // where a called function's RTS lands

// One access to the hardware page.
struct Access {
    bool write = false;
    std::uint16_t offset = 0;    // in the hardware page
    std::uint8_t value = 0;
    std::uint8_t vdc = 0xFF;     // the VDC register the data port reached (0xFF: not the data port)
    std::string text() const;    // "w VDC_DATA_L(CR) 80", "r IO_PORT ff"
};

class Machine {
public:
    // The card's ROM: whole 8 KB banks. The mapping registers start as a card's reset and the usual start-up leave them: $FF in MPR0 (the chips), $F8
    // in MPR1 (RAM), $00 in MPR7 (the first bank at $E000), the pages between mapping banks 2 to 6 in turn.
    explicit Machine(ByteView rom);

    std::array<std::uint8_t, 8> mpr{};
    std::array<std::uint8_t, kBankSize> ram{};

    std::uint8_t read(std::uint16_t logical);
    void write(std::uint16_t logical, std::uint8_t value);
    // The byte of the hardware page at `offset`, with the access logged; ST0 ST1 ST2 write here directly.
    std::uint8_t hardware_read(std::uint16_t offset);
    void hardware_write(std::uint16_t offset, std::uint8_t value);

    // The physical bank mapped at a logical address.
    std::uint8_t bank_at(std::uint16_t logical) const { return mpr[logical >> 13]; }

    const std::vector<Access>& log() const { return log_; }
    ByteView rom() const { return rom_; }

private:
    ByteView rom_;
    std::vector<Access> log_;
    std::uint8_t vdc_select_ = 0;
    unsigned vdc_status_reads_ = 0;
};

struct Fault {
    std::uint8_t opcode = 0;
    std::uint16_t pc = 0;
    std::string text() const;   // "no instruction for opcode 02 at e000"
};

struct Options {
    std::uint8_t a = 0, x = 0, y = 0;
    std::uint64_t max_steps = m6502::kMaxSteps;
};

enum class End : std::uint8_t { Return, Stopped, Limit };

struct Run {
    End end = End::Limit;
    std::optional<Fault> fault;
    m6502::Registers regs;
    std::uint64_t steps = 0;
    std::vector<Access> io;
    std::string exit() const;   // "return" | "stopped: <why>" | "limit"
};

// Call the function at `entry` as JSR would and run it until it returns (RTS to kReturnAddress), faults or takes `max_steps` instructions.
Run run(Machine& machine, std::uint16_t entry, const Options& options = {});

}  // namespace cb::emulate::huc6280
