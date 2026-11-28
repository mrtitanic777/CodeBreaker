// cb-unity: alone
// source/console_xboxone.cpp -- the executable shape's container `xboxone` (console_executable.cpp): an Xbox One / Series program (the x64 PE32+ of a UWP app) as NASM source that builds back into it
// (config/platforms/xboxone.json).
//
//     r.source           app.asm (NASM, `bits 64`)
//     r.same             the source, built with `nasm -f bin`, is the PE file byte for byte
//     r.functions        {RVA: name}
//
// The PE file (formats/xboxone gives it, out of its package when it is in one) is written whole by source/x64_writer, one NASM section per run -- the headers, each
// section's raw bytes at its RVA. Code is what flow reaches from the entry point, the exports and every function the exception directory (`.pdata`, the unwind
// table x64 code has for each function) begins; functions keep the names the exports give, and an import's address-table slot is `__imp_<name>` (a thunk that jumps
// through it is named for the function, so a call reads `call CreateFileW`). The headers are `db` rows under a comment that says what they are.
//
// The head (what the program is) is the description's.

#include "source/console_shapes.hpp"
#include "formats/xboxone.hpp"

#include <format>

#include "source/pe64_writer.hpp"

namespace cb::source::console_writer::xboxone_code {

namespace iw = image_writer;

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    const formats::xboxone::Program& program = contained_as<formats::xboxone::Program>(loaded);
    const std::string file_name = std::get<std::string>(values.at("file"));
    pe64_writer::Options options;
    options.name = std::filesystem::path(program.member.empty() ? file_name : program.member).stem().string();
    options.kind = fill(description.executable.description, values);
    const std::string from = program.member.empty() ? std::filesystem::path(file_name).filename().string() : file_name + ":" + program.member;
    return pe64_writer::write(program.file, from, folder, options);
}

}  // namespace cb::source::console_writer::xboxone_code
