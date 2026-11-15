// cb/source/pce_writer.hpp -- a PC Engine / TurboGrafx-16 HuCard as assembly source that builds back into it.
//
//     image_writer::Written r = pce_writer::reconstruct("game.pce", "out");
//     r.source, r.config        game.s (ca65 syntax, `.setcpu "HuC6280"`) and game.cfg (ld65)
//     r.same                    the source, built, is the card file byte for byte (a copier header included)
//     r.functions               {address: name}; an address is `bank << 16 | logical address`, the bank being the ROM bank's number
//
// Source syntax: ca65. The HuC6280 is a 65C02 and ca65 (cc65's assembler) has it as a CPU (`.setcpu "HuC6280"`: the block transfers, BBRn, TST, TAM,
// ST0 ...), reads the same `$` numbers, `a:` and `z:` size prefixes and `.byte` / `.word` / `.segment` that the NES, Atari Lynx and SNES writers write
// (one assembler for every 65xx console, so the 65xx core -- table, encoder, directive layer, ld65 configuration reader -- is shared), and has a
// linker that places each 8 KB bank at the address the program maps it to. pceas (the assembler of the HuC toolchain) has the same instructions but
// its banks are a `.bank`/`.org` pair per section; ca65's segments and the ld65 configuration say the same thing, and make a bank's logical address a
// fact of the configuration instead of a directive in the source.
//
// Each 8 KB bank is a segment `BANKn`, linked at the address the code that maps it expects (bank 0 at $E000; another bank at the page the flow found it
// mapped into, `LDA #n` and `TAM #mask` being tracked through the code, else $4000). Code is what flow reaches from the vectors (RESET, NMI, IRQ1, IRQ2,
// TIMER); a jump into a page whose mapping is not known from the code that reaches it is not followed, so a bank only the mapping routines of
// another bank select is data unless something in code that is reached names the bank. Every other byte is data. Function starts are `reset`,
// `nmi`, `irq1`, `irq2`, `timer` or `sub_<bank><address>`; jump targets `L_<bank><address>`, and a data address an instruction names `D_<bank><address>`. The
// chips' registers carry the hardware names (VDC_SELECT, VDC_DATA_L, VCE_CR, PSG_FREQ_L, TIMER_CTRL, IO_PORT, IRQ_DISABLE ...), and the zero page (the
// RAM at $2000) the names `zp_20`. The ten vector bytes are `.word irq2, irq1, timer, nmi, reset`.
//
// An instruction whose text does not assemble back into its own bytes (an operand that is the address of a bank mapped at two places) is written as
// `.byte` data, so the source always is the file. It builds with CodeBreaker's own assembler (build/m6502_asm with the HuC6280 encoder) or the cc65 suite
// (`ca65 game.s` and `ld65 -C game.cfg -o game.pce game.o`; the memory areas are in file order, each filled to its size).
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

#include "cb/formats/pce.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::pce_writer {

image_writer::Written write(const formats::pce::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::pce_writer
