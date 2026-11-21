#include "source/msx_writer.hpp"
#include "knowledge/package.hpp"

#include <format>

namespace cb::source::msx_writer {

const z80_writer::Machine& machine() {
    static const z80_writer::Machine kMsx{
        .ports = knowledge::standard().hardware_names("msx", "ports"),   // (config/knowledge/msx/registers.json)
        .names = knowledge::standard().table_names("msx", "firmware")};
    return kMsx;
}

image_writer::Written write(const formats::msx::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    const formats::msx::Header& h = program.header;
    z80_writer::Block block{.image = program.image, .origin = program.origin, .entries = {}, .header = {}};
    for (const formats::msx::Entry& e : program.entries) block.entries[e.address] = std::string(e.name);
    block.header.push_back(std::format("MSX cartridge {}; {} KB at ${:04X}; INIT ${:04X} STATEMENT ${:04X} DEVICE ${:04X} TEXT ${:04X}",
                                       file_name, program.image.size() / 1024, program.origin, h.init, h.statement, h.device, h.text));
    const std::filesystem::path name(file_name);
    return z80_writer::write(block, machine(), folder, name.stem().string());
}

image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::msx::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::msx_writer
