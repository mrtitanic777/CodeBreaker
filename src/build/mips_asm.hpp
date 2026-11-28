// build/mips_asm.hpp -- CodeBreaker's own MIPS assembler, for the source the PlayStation lane (R3000A, little-endian) and the Nintendo 64
// lane (R4300i, big-endian) write.
//
//     Assembler mips(encoder);                     // the MIPS instruction syntax (decompile/mips)
//     Assembler n64(encoder, ByteOrder::Big);      // the same, writing data directives big-endian (the encoder makes big-endian instructions)
//     Bytes file = mips.build(source_text);        // the bytes written, in the order they were written
//
// It reads the part of GNU as's language (the assembler of the PlayStation homebrew toolchains: mipsel-none-elf-as, PSn00bSDK)
// that source/console_writer writes for the PlayStation and the Nintendo 64, so the source's instructions also build with it:
//
//     .word / .4byte / .long    words (in the byte order): numbers, labels, expressions
//     .dword / .8byte / .quad   doublewords
//     .half / .short / .2byte   halfwords;  .byte  bytes
//     .ascii / .asciz / .asciiz text (with \n \t \r \0 \\ \" and \ooo octal), without and with the terminating zero
//     .space n[, fill]          n bytes (zero, or fill);  .fill n, size, value   n items of `size` bytes
//     .align n                  zeros up to a multiple of 2^n;  .p2align n likewise
//     .equ name, value          constants (also `.set name, value`; `.set noreorder` and the other `.set` options are accepted and
//                               mean nothing: delay slots are always written out)
//     name = value
//     label:                    the address of the next byte
//     .org address              the address the next byte has (see below)
//     # comment
//
// The file is the bytes in the order they are written. `.org` does not move the file: it says what address the next byte is
// seen at, which is what labels, branches and jumps use -- a PS-X EXE's header is written at address 0, then `.org 0x80010000`
// begins the program at the address the BIOS loads it to. `.globl .text .data .rdata .bss .section .type .size .ent .end .frame
// .mask .fmask .extern .local .comm .file` are accepted and ignored (a flat image has no sections or symbols). An instruction
// must be at a multiple of four.
//
// The directive layer sits over an `Encoder`, so it is tested without the decompiler's instruction tables. An assembler holds no
// state between builds.
#pragma once

#include <cstdint>
#include <string_view>

#include "build/asm_text.hpp"

namespace cb::build::mips_asm {

// How the data directives (.word .half .fill ...) lay out a value; an instruction's bytes are the encoder's.
enum class ByteOrder : std::uint8_t { Little, Big };

class Assembler {
public:
    // The encoder is borrowed and must outlive the assembler.
    explicit Assembler(const Encoder& encoder, ByteOrder order = ByteOrder::Little) : isa_(&encoder), order_(order) {}
    // `alignment`: what an instruction's address must be a multiple of (4 for the fixed-width processors this layer was written
    // for; 1 for a processor whose instructions are any number of bytes, such as one of the SLEIGH descriptions)
    // `comment`: the character a comment starts with ('#' as GNU as writes MIPS; ';' where '#' marks an immediate, as the SLEIGH
    // descriptions of the 6502, 68000 and ARM write it)
    Assembler(const Encoder& encoder, ByteOrder order, unsigned alignment, char comment = '#')
        : isa_(&encoder), order_(order), alignment_(alignment == 0 ? 1 : alignment), comment_(comment) {}

    // The file `source` describes. AsmError, with the line, for anything that does not assemble (including an instruction whose size
    // differs between the two passes, and a file past 16 MiB).
    Bytes build(std::string_view source) const;

private:
    const Encoder* isa_;
    ByteOrder order_;
    unsigned alignment_ = 4;
    char comment_ = '#';
};

// `line` with its `#` comment removed (not one inside a string), trimmed at the end.
std::string_view strip(std::string_view line, char comment = '#');

// An instruction's size on the first pass, when a label it names is not defined yet: assembled with every unknown name standing for
// a nearby address, so the bytes have the size the instruction will have. (Public for the tests.)
Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc, const Labels& labels);

}  // namespace cb::build::mips_asm
