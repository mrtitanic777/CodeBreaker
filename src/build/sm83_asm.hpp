// build/sm83_asm.hpp -- CodeBreaker's own Game Boy assembler, for the source the Game Boy lane writes.
//
//     Bytes rom = sm83_asm::build(source_text, size);     // the ROM image (size: its bytes; unwritten ones $FF)
//
// It reads the part of RGBDS's language that source/gb_writer writes, and that Game Boy homebrew is commonly written in, so
// the source also builds with RGBDS (rgbasm, rgblink -n / rgbfix):
//
//     SECTION "name", ROM0[$addr]                 bank 0, at a fixed address
//     SECTION "name", ROMX[$addr], BANK[n]        bank n (seen at $4000-$7FFF)
//     db / dw                                     bytes, little-endian words: numbers, labels, "strings"
//     ds n, value                                 n bytes of value
//     INCBIN "file"
//     DEF NAME EQU value  /  NAME EQU value       constants
//     label:  /  .local:                          the address of the next byte
//     ; comment
//
// The source is read in two passes. The first collects the labels and constants (an instruction that names one that is not
// defined yet is given the size it will have: a label it does not know counts as an address, `ldh` as $FF80); the second
// writes the bytes, and is the one in which a name that is still undefined is an error. A label may be defined
// again and keeps its last value. Line ends are \n or \r\n; text (a string in db, a file name) is taken as it is, and a string's
// characters are written as Latin-1 (a character beyond it becomes `?`).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string_view>

#include "build/asm_text.hpp"
#include "core/bytes.hpp"

namespace cb::build::sm83_asm {

// Bank 1 and up are seen at $4000-$7FFF; a bank is this many bytes of the ROM.
inline constexpr std::int64_t kBank = 0x4000;

// The ROM `source` assembles to: `size` bytes, the unwritten ones $FF; INCBIN files are read from `folder`. AsmError
// ("line N: <text> (<reason>)" in its message, the line in line()) for a line the assembler refuses and for bytes that
// fall outside the ROM; IoError for an INCBIN file that cannot be read.
Bytes build(std::string_view source, std::size_t size, const std::filesystem::path& folder = ".");

}  // namespace cb::build::sm83_asm
