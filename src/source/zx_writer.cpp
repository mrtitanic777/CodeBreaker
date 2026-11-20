#include "source/zx_writer.hpp"

#include <format>

namespace cb::source::zx_writer {

const z80_writer::Machine& machine() {
    static const z80_writer::Machine kSpectrum{
        .ports = {{0xFE, "ULA"}, {0x1F, "KEMPSTON"}},
        .names = {{0x0010, "PRINT_A"}, {0x03B5, "BEEPER"},    {0x04C2, "SA_BYTES"},  {0x0556, "LD_BYTES"}, {0x0D6B, "CLS"},    {0x0DAF, "CL_ALL"},
                  {0x1601, "CHAN_OPEN"}, {0x203C, "PR_STRING"}, {0x2D2B, "STACK_BC"}, {0x2DE3, "PRINT_FP"}, {0x5C08, "LAST_K"}, {0x5C36, "CHARS"},
                  {0x5C3B, "FLAGS"},   {0x5C48, "BORDCR"},    {0x5C4B, "VARS"},      {0x5C4F, "CHANS"},    {0x5C53, "PROG"},   {0x5C76, "SEED"},
                  {0x5C78, "FRAMES"},  {0x5C8D, "ATTR_P"},    {0x5CB2, "RAMTOP"}}};
    return kSpectrum;
}

image_writer::Written write(const formats::zx::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    z80_writer::Block block{.image = program.image, .origin = program.origin, .entries = {}, .header = {}};
    for (const formats::zx::Entry& e : program.entries) block.entries[e.address] = e.name;
    block.header.push_back(std::format("ZX Spectrum {} {}; {} bytes at ${:04X}", program.kind == formats::zx::Kind::Tape ? "tape" : "snapshot", file_name,
                                       program.image.size(), program.origin));
    if (program.registers) {
        const formats::zx::Registers& r = *program.registers;
        // (listed by name: a b c d e f h i im ix iy l sp)
        block.header.push_back(std::format("registers at the snapshot: A={:02X} B={:02X} C={:02X} D={:02X} E={:02X} F={:02X} H={:02X} I={:02X} IM={:02X} "
                                           "IX={:04X} IY={:04X} L={:02X} SP={:04X}",
                                           r.a, r.b, r.c, r.d, r.e, r.f, r.h, r.i, r.im, r.ix, r.iy, r.l, r.sp));
    }
    const std::filesystem::path name(file_name);
    return z80_writer::write(block, machine(), folder, name.stem().string());
}

image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(formats::zx::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::zx_writer
