// cb/build/z80_asm.hpp -- CodeBreaker's own Z80 assembler, for the source the Master System, Game Gear, MSX and
// Spectrum lanes write.
//
//     Assembler z80(encoder);                      // the Z80 instruction syntax (decompile/z80)
//     Bytes rom = z80.build(source_text, folder);  // WLA-DX source -> the ROM image it describes
//     auto flat = z80.build_flat(source, folder);  // sjasmplus/pasmo source -> (start address, bytes)
//
// Two dialects, one directive layer over the instruction encoder:
//
// WLA-DX, the part source/sms_writer writes and Master System / Game Gear homebrew is commonly written in, so the
// source also builds with WLA-DX itself:
//
//     .MEMORYMAP ... .ENDME           the slots (only SLOTSIZE and SLOT n $addr are used)
//     .ROMBANKMAP ... .ENDRO          BANKSTOTAL, BANKSIZE, BANKS (the ROM's size)
//     .BANK n SLOT s                  where the following bytes go: ROM page n, seen at slot s's address
//     .ORG offset / .ORGA address     within the bank: from its start / at a CPU address
//     .DB / .BYTE  .DW / .WORD        bytes and little-endian words: numbers, labels, "strings"
//     .DSB n value                    n bytes of value
//     .INCBIN "file"                  a file's bytes
//     .DEFINE NAME value / NAME = value     constants
//     label:                          the address of the next byte
//     ; comment
//
// Numbers: $1F 1Fh %0101 31 'c'. Expressions + - * / & | ^ << >> ( ), and <x >x (low and high byte).
// Unwritten bytes of the ROM are $FF, as WLA-DX leaves them by default (.EMPTYFILL).
//
// The flat form (sjasmplus, pasmo, the Spectrum and MSX assemblers):
//
//     ORG $addr   DEFB / DB / DEFM, DEFW / DW   DEFS / DS n[,value]   NAME EQU value   INCBIN "file"   label:
//
// Source text is bytes (asm_text.hpp); `.INCLUDE` is not understood, `.INCBIN` resolves beside `folder`.
// An assembler holds no state between builds: labels live for one build.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "cb/build/asm_text.hpp"

namespace cb::build::z80_asm {

class Assembler {
public:
    // The encoder is borrowed and must outlive the assembler.
    explicit Assembler(const Encoder& encoder) : isa_(&encoder) {}

    // The ROM image `source` describes (WLA-DX dialect). AsmError, with the line, for anything that does not
    // assemble; IoError for an .INCBIN that cannot be read.
    Bytes build(std::string_view source, const std::filesystem::path& folder = ".") const;
    // build() of a file's bytes; `.INCBIN` resolves beside the file.
    Bytes build_file(const std::filesystem::path& path) const;

    // What a flat program writes: where the first byte goes and the bytes up to the last one written (gaps zero).
    struct Flat {
        std::uint32_t start = 0;
        Bytes bytes;
    };
    Flat build_flat(std::string_view source, const std::filesystem::path& folder = ".") const;

private:
    const Encoder* isa_;
};

// An instruction's size on the first pass, when a label it names is not defined yet: assembled with every
// unknown name taken as a value that fits each operand kind. The bytes are only meant for their count. The error
// is the encoder's own when the line fails for another reason. (Public because the SM83 and 68000 assemblers
// size their instructions the same way.)
Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc);

}  // namespace cb::build::z80_asm
