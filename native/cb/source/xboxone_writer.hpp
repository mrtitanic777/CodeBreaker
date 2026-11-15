// cb/source/xboxone_writer.hpp -- an Xbox One / Series program (the x64 PE32+ of a UWP app) as NASM source that builds back into it.
//
//     image_writer::Written r = xboxone_writer::reconstruct("app.msixbundle", "out");     // or a loose .exe / .dll
//     r.source           app.asm (NASM, `bits 64`)
//     r.same             the source, built with `nasm -f bin`, is the PE file byte for byte
//     r.functions        {RVA: name}
//
// The PE file (formats/xboxone gives it, out of its package when it is in one) is written whole by source/x64_writer, one NASM section per run -- the headers, each
// section's raw bytes at its RVA. Code is what flow reaches from the entry point, the exports and every function the exception directory (`.pdata`, the unwind
// table x64 code has for each function) begins; functions keep the names the exports give, and an import's address-table slot is `__imp_<name>` (a thunk that jumps
// through it is named for the function, so a call reads `call CreateFileW`). The headers are `db` rows under a comment that says what they are.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include "cb/formats/xboxone.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::xboxone_writer {

image_writer::Written write(const formats::xboxone::Program& program, std::string_view file_name, const std::filesystem::path& folder);
// `member` picks a PE file of a package (default: the app's executable).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const std::string& member = "");

}  // namespace cb::source::xboxone_writer
