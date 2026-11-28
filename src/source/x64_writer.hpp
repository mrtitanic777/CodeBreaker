// source/x64_writer.hpp -- a 64-bit x86 program file written whole as one NASM source that builds back into it: what the PlayStation 4 and 5 writers
// (source/console_orbis) and the Xbox One writer (source/console_xboxone) share. They know the file format; this knows the instructions.
//
//     x64_writer::Image image;                  // the file's bytes, which runs of it are mapped where, the entry points, the names
//     image_writer::Written r = x64_writer::write(image, "out");
//     r.source           <name>.asm (NASM, `bits 64`)
//     r.same             the source, built with `nasm -f bin`, is the file byte for byte
//     r.functions        {address: name}
//
// Code is what flow reaches from the entry points and from every call's target, with Capstone's 64-bit decoder; every other byte is data (`db` rows, strings
// and `times` runs). The file is written as one NASM section per run -- a mapped run with `vstart` at its address, so a label is the address the processor
// sees -- and each instruction as text in NASM's spelling: a direct branch names its label, a `[rip + disp]` operand is `[rel label]` (or a run's label and an
// offset where nothing in the file is at the target), and a register-to-register move the assembler would encode the other way round is written through the
// `mov_`-style macros at the head of the source. What NASM still does not give back byte for byte (a long form where a short one fits, a multi-byte `nop`, an
// instruction it has no spelling for) is found out in rounds against the assembler (source/flat_rounds) and written as `db` with the instruction beside it.
//
// x86asm, CodeBreaker's own assembler, builds the source, in this process.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "source/image_writer.hpp"

namespace cb::source::x64_writer {

// A stretch of the file: where it is in the file, and where the program sees it (when it is mapped at all).
struct Run {
    std::uint32_t offset = 0, size = 0;
    std::uint32_t address = 0;
    bool mapped = false;
    bool code = false;        // instructions may stand here
};

struct Image {
    std::string name;                                  // the source is <name>.asm
    Bytes file;
    std::vector<Run> runs;                             // ascending by offset, together the whole file
    std::vector<std::uint32_t> entries;                // addresses flow starts from (the entry point, exports, symbols)
    std::map<std::uint32_t, std::string> functions;    // names of known function starts, by address
    std::map<std::uint32_t, std::string> data;         // names of known data places (an import's slot), by address
    std::map<std::uint32_t, std::string> comments;     // by file offset: a line written above that offset (it also starts a new data row)
    std::string head;                                  // comment lines at the top of the source
    std::string first_entry_name = "start";           // what the first entry is called when it has no name of its own
};

image_writer::Written write(const Image& image, const std::filesystem::path& folder);

}  // namespace cb::source::x64_writer
