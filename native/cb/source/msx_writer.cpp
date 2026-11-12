#include "cb/source/msx_writer.hpp"

#include <format>

namespace cb::source::msx_writer {

const z80_writer::Machine& machine() {
    static const z80_writer::Machine kMsx{
        .ports = {{0x98, "VDP_DATA"}, {0x99, "VDP_CTRL"}, {0xA0, "PSG_ADDR"}, {0xA1, "PSG_WRITE"}, {0xA2, "PSG_READ"},
                  {0xA8, "PPI_A"},    {0xA9, "PPI_B"},    {0xAA, "PPI_C"},    {0xAB, "PPI_CTRL"}},
        .names = {{0x000C, "RDSLT"},  {0x0014, "WRSLT"},  {0x001C, "CALSLT"}, {0x0024, "ENASLT"}, {0x0030, "CALLF"},  {0x0041, "DISSCR"},
                  {0x0044, "ENASCR"}, {0x0047, "WRTVDP"}, {0x004A, "RDVRM"},  {0x004D, "WRTVRM"}, {0x0050, "SETRD"},  {0x0053, "SETWRT"},
                  {0x0056, "FILVRM"}, {0x0059, "LDIRMV"}, {0x005C, "LDIRVM"}, {0x005F, "CHGMOD"}, {0x0062, "CHGCLR"}, {0x0069, "CLRSPR"},
                  {0x006C, "INITXT"}, {0x006F, "INIT32"}, {0x0072, "INIGRP"}, {0x0075, "INIMLT"}, {0x0090, "GICINI"}, {0x0093, "WRTPSG"},
                  {0x0096, "RDPSG"},  {0x009C, "CHSNS"},  {0x009F, "CHGET"},  {0x00A2, "CHPUT"},  {0x00C3, "CLS"},    {0x00D5, "GTSTCK"},
                  {0x00D8, "GTTRIG"}, {0x0138, "RSLREG"}, {0x013B, "WSLREG"}, {0xF3AE, "LINL40"}, {0xF3DF, "RG0SAV"}, {0xFC9E, "JIFFY"},
                  {0xFCC1, "EXPTBL"}, {0xFD9F, "H_TIMI"}}};
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
