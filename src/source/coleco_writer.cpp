#include "source/coleco_writer.hpp"

#include <format>

namespace cb::source::coleco_writer {

const z80_writer::Machine& machine() {
    static const z80_writer::Machine kColeco{
        .ports = {{0x80, "KEYPAD_SELECT"}, {0xC0, "JOYSTICK_SELECT"}, {0xBE, "VDP_DATA"}, {0xBF, "VDP_CTRL"}, {0xFC, "JOY1"}, {0xFF, "PSG_JOY2"}},
        .names = formats::coleco::bios_names()};
    return kColeco;
}

image_writer::Written write(const formats::coleco::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    const formats::coleco::Header& h = program.header;
    z80_writer::Block block{.image = program.image, .origin = formats::coleco::kOrigin, .entries = {}, .header = {}};
    for (const formats::coleco::Entry& e : program.entries) block.entries[e.address] = e.name;
    block.header.push_back(std::format("ColecoVision cartridge {}; {} KB at $8000; {}", file_name, program.image.size() / 1024,
                                       h.title_screen ? "$AA55: the BIOS shows its title screen first" : "$55AA: the game starts at once"));
    block.header.push_back(std::format("sprite names ${:04X} sprite order ${:04X} work buffer ${:04X} controllers ${:04X} start ${:04X}", h.sprite_names, h.sprite_order,
                                       h.work_buffer, h.controllers, h.start));
    if (!h.title.empty()) block.header.push_back("title: " + h.title);
    const std::filesystem::path name(file_name);
    return z80_writer::write(block, machine(), folder, name.stem().string());
}

image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::coleco::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::coleco_writer
