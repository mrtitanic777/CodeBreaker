// cb/source/nds_writer.hpp -- a Nintendo DS cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = nds_writer::reconstruct("game.nds", "out");
//     r.source                  game.s (GNU as syntax, `.syntax unified`, as devkitARM's gcc produces it)
//     r.same                    the source, built, is the ROM byte for byte
//     r.functions               {address: name}, both processors' functions
//
// A DS cartridge holds a program for each of the console's two processors (formats/nds.hpp): the ARM9 (ARMv5TE) and the ARM7 (ARMv4T), loaded
// at different RAM addresses. They come back as ONE source file with a part for each, in the order the ROM holds them, and not as two projects:
// the ROM is one image whose header, tables and files name the two programs by their file offset, RAM address and size, and the source says so
// with labels (`arm9_rom`, `arm9`, `arm9_end`; the header's words are `arm9_end - arm9`), so the one file shows how the pieces fit and rebuilds
// the one ROM. Each program has its own `.cpu` line (`.cpu arm946e-s`, `.cpu arm7tdmi`), which selects the instruction set the assembler reads
// (build/arm_asm), and its own names: `arm9_arm_<address>` and `arm9_thumb_<address>` for functions, `L9_<address>` for jump targets,
// `D9_<address>` for data (`arm7_...`, `L7_`, `D7_` for the other), `arm9_entry` and `arm7_entry` for where each starts.
//
// The programs are written as the GBA lane writes a cartridge (source/gba_writer.hpp): every function in the order it lies, `.arm` or `.thumb`
// before the instructions of the state it runs in, literal pools as `.word` lines, jump tables and pointer tables as labels, and each
// instruction as text only when the text assembles back to the instruction's bits (otherwise `.hword`/`.word` with the instruction as a
// comment). What differs is the instruction set (BLX with a label or a register is followed as a call that changes state; CLZ, the saturating
// and 16-bit multiplies, LDRD/STRD, PLD and the coprocessor 15 moves of the ARM9) and where the registers are: the console's names
// (libnds's REG_DISPCNT, REG_IME, REG_IPC_FIFO_TX ..., `hardware()`), the other RAM addresses `var_<address>` equates.
//
// Everything that is not code is data and comes back as such: the header field by field (title, game code, the two programs' offsets, entry points,
// RAM addresses and sizes, the tables' offsets, the banner's offset, the Nintendo logo, the checksums), the file name table (`fnt`), the file
// allocation table (`fat`, one `.word file_N, file_N_end` pair per file), the overlay tables, the banner (the icon, its palette, the titles in
// UTF-16) and each file (`file_N`, with its path from the name table in a comment). Bytes the header does not name are data in place.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/nds.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::nds_writer {

// The console's registers, by address ($04000000-$04001FFF and the FIFO at $04100000), by the names libnds gives them. The ARM7's differ from the
// ARM9's in the sound, touch screen and real-time clock registers.
const std::map<std::uint32_t, std::string>& hardware(formats::nds::Cpu cpu);

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::nds::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a DS ROM).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::nds_writer
