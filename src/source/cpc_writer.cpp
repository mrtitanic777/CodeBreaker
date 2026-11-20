#include "source/cpc_writer.hpp"

#include <format>

namespace cb::source::cpc_writer {

const z80_writer::Machine& machine() {
    static const z80_writer::Machine kCpc{.ports = {}, .names = formats::cpc::firmware_names()};
    return kCpc;
}

image_writer::Written write(const formats::cpc::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    z80_writer::Block block{.image = program.image, .origin = program.origin, .entries = {}, .header = {}};
    for (const formats::cpc::Entry& e : program.entries) block.entries[e.address] = e.name;
    using Kind = formats::cpc::Kind;
    const char* what = program.kind == Kind::Snapshot ? "snapshot" : program.kind == Kind::Disk ? "disk" : "binary";
    block.header.push_back(std::format("Amstrad CPC {} {}{}; {} bytes at ${:04X}", what, file_name, program.kind == Kind::Disk ? " (file " + program.name + ")" : std::string(),
                                       program.image.size(), program.origin));
    for (const std::string& f : program.files) block.header.push_back("disk file: " + f);
    if (program.registers) {
        const formats::cpc::Registers& r = *program.registers;
        block.header.push_back(std::format("registers at the snapshot: A={:02X} B={:02X} C={:02X} D={:02X} E={:02X} F={:02X} H={:02X} I={:02X} IM={:02X} IX={:04X} IY={:04X} L={:02X} SP={:04X} PC={:04X}",
                                           r.a, r.b, r.c, r.d, r.e, r.f, r.h, r.i, r.im, r.ix, r.iy, r.l, r.sp, r.pc));
    }
    const std::filesystem::path name(file_name);
    return z80_writer::write(block, machine(), folder, name.stem().string());
}

image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::cpc::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::cpc_writer
