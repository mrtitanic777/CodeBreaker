// cb-unity: alone
// source/xboxone_writer.cpp -- an Xbox One / Series PE32+ as NASM source that builds back into it (xboxone_writer.hpp): the 64-bit Windows writer
// (source/pe64_writer) with the package's identity in the head.

#include "source/xboxone_writer.hpp"

#include <format>

#include "source/pe64_writer.hpp"

namespace cb::source::xboxone_writer {

namespace iw = image_writer;

iw::Written write(const formats::xboxone::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    pe64_writer::Options options;
    options.name = std::filesystem::path(program.member.empty() ? std::string(file_name) : program.member).stem().string();
    options.kind = std::format("{} for the Xbox One / Series{}{}", program.container, program.identity.empty() ? "" : ", " + program.identity,
                               program.version.empty() ? "" : " " + program.version);
    const std::string from = program.member.empty() ? std::filesystem::path(std::string(file_name)).filename().string() : std::string(file_name) + ":" + program.member;
    return pe64_writer::write(program.file, from, folder, options);
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const std::string& member) {
    return write(formats::xboxone::read(file, member), file.filename().string(), folder);
}

}  // namespace cb::source::xboxone_writer
