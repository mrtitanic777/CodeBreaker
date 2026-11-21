// engine/pcode_cpu.hpp -- the lanes' 6502 run on CodeBreaker's one emulator (decompile/engine/pcode_emulator), the p-code
// interpreter of the processor's SLEIGH description, in place of the lane's own CPU core (emulate/m6502).
//
//     RunResult r = engine::pcode_cpu::call(bus, entry, options);   // as emulate::m6502::run, on the core chosen by CB_PCODE_EMU
//
// The machine stays the lane's: the Bus (memory map, address mask, devices, the log of device accesses) is the emulator's
// read and write hook, so every data access of the program goes through Bus::read and Bus::write exactly as the own core
// makes it. What the emulator reads instructions from is its page map, a copy of the bus's memory as the processor sees it
// (the address mask's mirrors included), kept current: after an instruction that touched the bus the copy is compared with
// the bus's memory and what changed (a bank switch, a store into RAM that code runs from) is written to every mirror.
//
// The same run and the same result as emulate::m6502::run: a function is called as JSR calls it (return address pushed,
// returning when the pc reaches kReturnAddress), the registers A X Y P S as the run left them, the device accesses of the
// run in order, an instruction the NMOS 6502 documents nothing for stops the run with the same fault. Differences from the
// own core, which the description decides: ADC and SBC honour the D flag (the own core's `decimal` is off by default);
// `Variant::Cmos` runs 65c02.sla.
//
// call() uses the emulator by default; CB_PCODE_EMU=0 (or force(false)) makes it use the own core (emulate/m6502, emulate/z80). Every 6502-family
// and Z80-family lane (Atari 2600, 5200, 7800, NES, Lynx, Master System, SG-1000, MSX, ColecoVision, ZX Spectrum, Amstrad CPC) calls it.
#pragma once

#include <optional>

#include "emulate/m6502.hpp"
#include "emulate/m68k.hpp"
#include "emulate/arm.hpp"
#include "emulate/mips.hpp"
#include "emulate/r4300.hpp"
#include "emulate/powerpc.hpp"
#include "emulate/sh2.hpp"
#include "emulate/z80.hpp"

namespace cb::engine::pcode_cpu {

// The CPU families with an adapter. A family's default (default_on) is the emulator only where its lanes' homebrew is judged with no verdict
// differing from the own core's and no slower; the others run the own core unless CB_PCODE_EMU=1.
enum class Core : std::uint8_t { Mos6502, Z80, M68000, Mips, R4300, Arm, SuperH, PowerPc };
bool default_on(Core core);

// Whether call() runs on the emulator: force() if set, else CB_PCODE_EMU ("0", "false", "no": off for every core; "1", "true", "yes": on for every
// core), else the core's default.
bool enabled(Core core = Core::Mos6502);
// Override the environment (tests flip it); std::nullopt goes back to the environment.
void force(std::optional<bool> on);

// The run on the emulator, always.
emulate::m6502::RunResult run(emulate::m6502::Bus& bus, std::uint16_t entry, const emulate::m6502::RunOptions& options = {});

// emulate::m6502::run or run(), by enabled().
emulate::m6502::RunResult call(emulate::m6502::Bus& bus, std::uint16_t entry, const emulate::m6502::RunOptions& options = {});

// The same for the lanes' Z80 (emulate::z80::Bus), on z80.sla: the Bus is the emulator's read/write hook for the "ram" space and its
// "io" space (IN and OUT; the port is all sixteen lines, as the description puts A or B on the high ones), the 64 KB of bus memory the
// page map instructions are decoded from, kept current. The call is made as CALL makes it (return address pushed, kReturnAddress ends the
// run), HALT stops the run with the Halt fault, an instruction the description lacks with Undecodable (a DD/FD in front of an instruction that
// takes no index is skipped, most of the ED page is NOP; the description has every opcode, the undocumented ones too). R is counted by the adapter as the own core counts it; the interrupt flip-flops and mode
// are tracked from the description's user operations (disable/enableMaskableInterrupts, setInterruptMode, getIFF2, halt).
emulate::z80::RunResult run(emulate::z80::Bus& bus, std::uint16_t entry, const emulate::z80::RunOptions& options = {});
emulate::z80::RunResult call(emulate::z80::Bus& bus, std::uint16_t entry, const emulate::z80::RunOptions& options = {});

// The same for the 68000 lanes (emulate::m68k::Bus), on 68020.sla (a superset of the 68000). Every data access goes through the Bus's read/write (the
// device ranges and their log are the bus's), the 16 MB of bus memory is what instructions are decoded from, a page at a time current from the last run.
// The call is made as JSR makes it; TRAP, CHK, TRAPV and STOP end the run with the fault the own core gives.
emulate::m68k::RunResult run(emulate::m68k::Bus& bus, std::uint32_t entry, const emulate::m68k::RunOptions& options = {});
emulate::m68k::RunResult call(emulate::m68k::Bus& bus, std::uint32_t entry, const emulate::m68k::RunOptions& options = {});

// The PlayStation's R3000A (emulate::mips::Bus), on mips32le.sla: the adapter carries the BIOS calls, the load delay slot, the faults, coprocessor 0 and the
// GTE (the own core's). A branch in a branch's delay slot is the one thing the description cannot decode.
emulate::mips::RunResult run(emulate::mips::Bus& bus, std::uint32_t entry, const emulate::mips::RunOptions& options = {});
emulate::mips::RunResult call(emulate::mips::Bus& bus, std::uint32_t entry, const emulate::mips::RunOptions& options = {});

// The Nintendo 64's R4300i (emulate::r4300::Bus), on mips64be.sla: the adapter carries the integer machine, loads and stores, branches (likely ones too),
// faults, coprocessor 0 and the FPU.
emulate::r4300::RunResult run(emulate::r4300::Bus& bus, std::uint32_t entry, const emulate::r4300::RunOptions& options = {});
emulate::r4300::RunResult call(emulate::r4300::Bus& bus, std::uint32_t entry, const emulate::r4300::RunOptions& options = {});

// The Game Boy Advance's ARM7TDMI (emulate::arm::Bus of System::Gba), on ARM4t_le.sla: ARM and Thumb state, the BIOS calls and bus errors are carried by the
// adapter. call() gives every other machine (the DS, the 3DS, the Vita) to the own core.
emulate::arm::RunResult run(emulate::arm::Bus& bus, std::uint32_t entry, const emulate::arm::RunOptions& options = {});
emulate::arm::RunResult call(emulate::arm::Bus& bus, std::uint32_t entry, const emulate::arm::RunOptions& options = {});

// The SuperH lanes' SH-2 (the Saturn, the 32X; emulate::sh2::Bus) on sh-2.sla and SH-4 (the Dreamcast) on SuperH4_le.sla: a delayed branch and its slot are one step of
// the emulator, the faults, the on-chip registers and the bus's devices are the adapter's and the bus's; what the descriptions leave out (the SH-4's FPU, banks and
// cache instructions) runs as the own core runs it.
emulate::sh2::RunResult run(emulate::sh2::Bus& bus, std::uint32_t entry, const emulate::sh2::RunOptions& options = {});
emulate::sh2::RunResult call(emulate::sh2::Bus& bus, std::uint32_t entry, const emulate::sh2::RunOptions& options = {});

// The PowerPC lanes' (the GameCube, Wii, Wii U, PlayStation 3) function runs, on ppc_64_isa_altivec_be.sla: the integer, branch and condition-register instructions on the
// description with the Machine as the memory, the rest (floating point and paired singles, vector, system calls, reservations, special registers) as the interpreter runs
// it. call() gives a run that wants calls out, a path, a branch log or a shadow (the lane judge's) to the interpreter.
emulate::powerpc::RunResult run(std::span<const emulate::powerpc::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args = {},
                                const emulate::powerpc::RunOptions& options = {});
emulate::powerpc::RunResult call(std::span<const emulate::powerpc::Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args = {},
                                 const emulate::powerpc::RunOptions& options = {});

}  // namespace cb::engine::pcode_cpu
