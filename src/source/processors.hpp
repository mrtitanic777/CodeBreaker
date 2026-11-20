// source/processors.hpp -- every processor CodeBreaker reads and writes, in one table: the one disassembler and the one assembler.
//
// CodeBreaker has one disassembler and one assembler, not one per platform: a processor is a row here (its name, the names it
// is also known by, how its instructions read and how its source assembles), and `codebreaker disassemble` / `assemble` are
// the same command for every row. A new processor is a new row; nothing else changes.
//
//     const Processor& z80 = processors::find("z80");
//     for (const Line& line : z80.disassemble(rom, 0x0000, 32)) ...       // 32 instructions from $0000
//     Image image = z80.assemble(source, folder);                           // the bytes `source` assembles to
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::source::processors {

// One instruction as the disassembler reads it.
struct Line {
    std::uint64_t address = 0;
    Bytes bytes;                  // its bytes, as they are in the file
    std::string text;             // in the syntax the processor's assembler reads back (".db $FF" for a byte that is no instruction)
};

// What source assembles to: the bytes, and the address of the first (0 when the assembler does not say).
struct Image {
    std::uint64_t base = 0;
    Bytes data;
};

struct Processor {
    std::string_view name;                       // "z80", "x86-32", "m68k" ...
    std::vector<std::string_view> aliases;       // the other names it is asked for by ("gbz80", "6502", "i386" ...)
    std::string_view summary;                    // what it is and where it runs
    std::string_view syntax;                     // the assembler syntax both directions use
    // `count` instructions (0: all) of `code`, the first at `base`. Bytes that are no instruction come out as data lines.
    std::function<std::vector<Line>(ByteView code, std::uint64_t base, std::size_t count)> disassemble;
    // The image `source` assembles to; `folder` is where an include or INCBIN is read from. AsmError (or the assembler's own
    // error) for source that does not assemble. Empty for a processor CodeBreaker reads but does not yet assemble.
    std::function<Image(std::string_view source, const std::filesystem::path& folder)> assemble;
    // Its SLEIGH description (decompile/engine/languages/<sla>), the one the decompiler core reads, and the context variables it
    // starts from; empty for a processor that has none yet. `read_described` reads through it.
    std::string_view sla;
    std::vector<std::pair<std::string_view, int>> context;
    bool big_endian = false;   // how its data directives write a word
};

// The instructions as the processor's SLEIGH description reads them (the decompiler core's own reader, in this program): the
// same arguments as `disassemble`, the text in the description's syntax. UsageError for a processor with no description.
std::vector<Line> read_described(const Processor& processor, ByteView code, std::uint64_t base, std::size_t count);

// The image `source` assembles to through the processor's SLEIGH description: the one assembler (decompile/engine/
// sleigh_assembler) under the one directive layer (build/mips_asm: labels, .org, data). The instructions are in the description's
// syntax, as read_described writes them. UsageError for a processor with no description; AsmError for source that does not assemble.
Image assemble_described(const Processor& processor, std::string_view source, const std::filesystem::path& folder);

// Source for `code` that assembles back to exactly `code` through assemble_described: each instruction as read_described writes it,
// except where its text assembles to other bytes (an encoding the text cannot tell apart: a 68000 immediate's unused byte, an ARM
// constant's other rotation), which stays as its bytes, with the instruction in a comment.
std::string source_described(const Processor& processor, ByteView code, std::uint64_t base);

// Every processor, in a fixed order.
std::span<const Processor> all();
// The processor a name or alias names (case-blind), or nullptr.
const Processor* lookup(std::string_view name);
// The same; UsageError listing the names when there is none.
const Processor& find(std::string_view name);

}  // namespace cb::source::processors
