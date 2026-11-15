// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/source/ps2_writer.hpp"

#include "cb/core/error.hpp"
#include "cb/source/isa_mips_le.hpp"
#include "cb/source/mips_elf_writer.hpp"

namespace cb::source::ps2_writer {

namespace {

// RAM as the program sees it: the first 32 MB (any of the views the kernel gives), and the scratchpad.
bool ee_ram(std::uint32_t a) {
    const std::uint32_t p = a >= 0x80000000u && a < 0xC0000000u ? a & 0x1FFFFFFFu : a >= 0x20000000u && a < 0x22000000u ? a - 0x20000000u : a;
    return p < formats::ps2::kRamSize || (a >= formats::ps2::kScratchpad && a < formats::ps2::kScratchpad + formats::ps2::kScratchpadSize);
}

}  // namespace

image_writer::Written write(const formats::ps2::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    mips_elf_writer::Machine m;
    m.variant = decompile::mips::Variant::R5900;
    m.encoder = &isa::r5900_encoder();
    m.description = program.kind == formats::ps2::Kind::Disc ? "PlayStation 2 disc: the BOOT2 program (Emotion Engine R5900)" : "PlayStation 2 ELF (Emotion Engine R5900)";
    m.hardware = [](std::uint32_t a) { return formats::ps2::hardware_name(a); };
    m.in_ram = ee_ram;
    m.extension = ".elf";
    return mips_elf_writer::write(m, program.elf.data, program.elf, 0, file_name, folder);
}

image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    const std::optional<formats::ps2::Program> program = formats::ps2::read(file);
    if (!program) throw FormatError("ps2", 0, "not a PlayStation 2 program (an R5900 ELF, or a disc image whose BOOT2 file is one)");
    // a disc's program is written under the disc's name
    const std::string name = program->kind == formats::ps2::Kind::Disc ? file.stem().string() + ".elf" : file.filename().string();
    return write(*program, name, folder);
}

}  // namespace cb::source::ps2_writer
