// source/pe64_writer.hpp -- a 64-bit Windows program (an x64 PE32+ .exe or .dll) as NASM source that builds back into it, byte for byte.
//
//     image_writer::Written r = pe64_writer::reconstruct("notepad.exe", "out");
//     r.source           notepad.asm (NASM, `bits 64`)
//     r.same             the source, built by x86asm (CodeBreaker's own assembler, as `nasm -f bin` would), is the PE file byte for byte
//     r.functions        {RVA: name}
//
// The PE file is written whole by source/x64_writer, one NASM section per run -- the headers, each section's raw bytes at its RVA. Code is what flow reaches
// from the entry point, the exports and every function the exception directory (`.pdata`, the unwind table x64 code has for each function) begins; functions
// keep the names the exports give, and an import's address-table slot is `__imp_<name>` (a thunk that jumps through it is named for the function, so a call
// reads `call CreateFileW`). The headers are `db` rows under a comment that says what they are.
//
// What is not yet done: pointers the base-relocation table names are written as numbers (the source rebuilds the original, but code moved by an edit leaves
// them pointing at the old places), and a switch's table of 32-bit RVAs is data.
//
// The Xbox One / Series writer (source/console_xboxone) is this writer with the package's identity in the head.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "source/image_writer.hpp"

namespace cb::source::pe64_writer {

struct Options {
    std::string name;   // the source is <name>.asm (default: the file name's stem)
    std::string kind;   // what the program is, for the source's head (default: "64-bit Windows program")
};

// `file` is the PE32+ file's bytes; `file_name` is what the head says it was written from.
image_writer::Written write(ByteView file, std::string_view file_name, const std::filesystem::path& folder, const Options& options = {});
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const Options& options = {});

}  // namespace cb::source::pe64_writer
