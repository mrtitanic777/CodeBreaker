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
};

// Every processor, in a fixed order.
std::span<const Processor> all();
// The processor a name or alias names (case-blind), or nullptr.
const Processor* lookup(std::string_view name);
// The same; UsageError listing the names when there is none.
const Processor& find(std::string_view name);

}  // namespace cb::source::processors
