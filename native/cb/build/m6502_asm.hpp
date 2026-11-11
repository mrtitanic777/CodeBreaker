// cb/build/m6502_asm.hpp -- assemble and link 6502 source in ca65 syntax, with an ld65 configuration.
//
//     Assembler a(encoder);                                  // the 6502 instruction syntax (decompile/m6502)
//     Bytes image = a.build(source_text, config_text, folder);   // the bytes ld65 would write
//
// The subset the NES lane writes (source/nes_writer), enough that it rebuilds without the cc65 suite on the
// machine; the same files build with ca65 and ld65 (`ca65 game.s` and `ld65 -C game.cfg game.o`). Understood:
//
//     .segment "NAME"         switch to a segment (the configuration says where it loads)
//     name:                   a label at the current address
//     NAME = $2000            a constant
//     .byte 1, $02, "text"    bytes and strings          .word $1234, label    little-endian words
//     .incbin "file"          a file's bytes, from `folder`
//     LDA $0200,x ...         an instruction (the encoder)
//     ; ...                   a comment
//
// The configuration's MEMORY areas, in order, are the output file: each starts at its `start`, holds the
// segments that `load` into it, and is filled to its `size` with `fill` value 0 (fill=yes), as ld65 does.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/build/asm_text.hpp"

namespace cb::build::m6502_asm {

// The 6502's instruction syntax, as the directive layer needs it.
class Encoder6502 {
public:
    virtual ~Encoder6502() = default;
    // The bytes of one instruction line at address `pc`. A name not in `labels` is an UndefinedSymbolError.
    virtual Result<Bytes> assemble(std::string_view line, std::uint32_t pc, const Labels& labels) const = 0;
    // Whether the (upper-case) mnemonic is a relative branch, always two bytes: the first pass sizes it so.
    virtual bool is_branch(std::string_view mnemonic) const = 0;
};

// One MEMORY area of the configuration.
struct MemoryArea {
    std::string name;
    std::uint32_t start = 0;
    std::uint32_t size = 0;
    bool fill = false;   // fill=yes: padded with zeros to `size`
};

// What the configuration says: the areas in file order, and the area each segment loads into.
struct LinkConfig {
    std::vector<MemoryArea> areas;
    std::unordered_map<std::string, std::string> segment_area;   // looked up only
};

// Read the MEMORY and SEGMENTS blocks of an ld65 configuration (`#` comments). AsmError for a number or a
// segment entry that is not understood.
LinkConfig parse_config(std::string_view text);

class Assembler {
public:
    explicit Assembler(const Encoder6502& encoder) : isa_(&encoder) {}

    // The linked image. AsmError, with the line, for source that does not assemble; an area holding more than
    // its size is an AsmError too.
    Bytes build(std::string_view source, std::string_view config_text,
                const std::filesystem::path& folder = ".") const;

private:
    const Encoder6502* isa_;
};

}  // namespace cb::build::m6502_asm
