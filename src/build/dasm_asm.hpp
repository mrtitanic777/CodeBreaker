// build/dasm_asm.hpp -- assemble 6502 source in dasm syntax, the assembler of Atari 2600 and 7800 homebrew.
//
//     Assembler a(encoder);                          // the 6502 instruction syntax (decompile/m6502)
//     Bytes image = a.build(source_text, folder);    // the bytes `dasm game.asm -f3` writes
//
// The subset the Atari lanes write (source/atari2600_writer, source/atari7800_writer), so a rebuild needs no dasm on the machine; the
// same files assemble with dasm. Understood:
//
//     processor 6502          (ignored, as are `seg` and `echo`)
//     ORG $F000               the output position: the image starts at the first ORG, a later ORG moves forward in it (a gap is zero)
//     RORG $F000              the address the code runs at, from here until the next ORG
//     NAME = $0002            a constant (also `NAME equ value`)
//     label                   a label at the start of a line, with the line's content after it if any
//     .byte 1, $02, "text"    bytes and strings (`byte`, `dc.b`)           .word $1234, label   little-endian words (`word`, `dc.w`)
//     ds.b 32,$FF             32 bytes of $FF (`ds`, `ds.b`; the fill is 0 without one)
//     hex 0102 03             bytes written as hex digits
//     incbin "file"           a file's bytes, from `folder`
//     LDA.w $0080             an instruction; `.w` forces the absolute form of a zero-page-sized address
//     ; ...                   a comment
//
// Numbers are `$hex`, `%binary` or decimal; an operand is one number or label (and `<` / `>` for the low / high byte in an immediate).
#pragma once

#include <filesystem>
#include <string_view>

#include "build/m6502_asm.hpp"

namespace cb::build::dasm_asm {

class Assembler {
public:
    explicit Assembler(const m6502_asm::Encoder6502& encoder) : isa_(&encoder) {}

    // The image. AsmError, with the line, for source that does not assemble.
    Bytes build(std::string_view source, const std::filesystem::path& folder = ".") const;

private:
    const m6502_asm::Encoder6502* isa_;
};

}  // namespace cb::build::dasm_asm
