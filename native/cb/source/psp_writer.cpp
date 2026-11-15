// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/source/psp_writer.hpp"

#include <cctype>
#include <format>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/source/isa_mips_le.hpp"
#include "cb/source/mips_elf_writer.hpp"

namespace cb::source::psp_writer {

namespace {

// RAM and video memory as the program sees them (kernel, user and uncached views), and the scratchpad.
bool psp_ram(std::uint32_t a) {
    const std::uint32_t p = a >= 0x80000000u && a < 0xC0000000u ? a & 0x1FFFFFFFu : a >= 0x40000000u && a < 0x60000000u ? a - 0x40000000u : a;
    return (p >= formats::psp::kRamBase && p < formats::psp::kRamBase + formats::psp::kRamSize) || (p >= formats::psp::kVram && p < formats::psp::kVram + formats::psp::kVramSize) ||
           (p >= formats::psp::kScratchpad && p < formats::psp::kScratchpad + formats::psp::kScratchpadSize);
}

std::string clean(std::string_view s) {
    std::string out;
    for (const char c : s) out += (std::isalnum(static_cast<unsigned char>(c)) || c == '_') ? c : '_';
    return out;
}

}  // namespace

image_writer::Written write(const formats::psp::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    mips_elf_writer::Machine m;
    m.variant = decompile::mips::Variant::Allegrex;
    m.encoder = &isa::allegrex_encoder();
    m.description = std::string("PSP ") + (program.kind == formats::psp::Kind::Pbp ? "EBOOT.PBP" : program.prx() ? "module (PRX)" : "executable (ELF)") + " (Allegrex)";
    if (!program.title.empty()) m.description += ": " + program.title;
    m.hardware = [](std::uint32_t a) { return formats::psp::hardware_name(a); };
    m.in_ram = psp_ram;
    m.extension = program.kind == formats::psp::Kind::Pbp ? ".pbp" : program.prx() ? ".prx" : ".elf";
    std::unordered_set<std::string> used;
    const auto unique = [&](std::string name, std::uint32_t address) {
        if (!used.insert(name).second) name = std::format("{}_{:08X}", name, address);
        used.insert(name);
        return name;
    };
    for (const formats::psp::Export& e : formats::psp::exports(program.elf)) {
        if (e.variable || e.address == 0) continue;
        const std::string name = e.name.empty() ? std::format("export_{}_{:08X}", e.library.empty() ? "system" : clean(e.library), e.nid) : e.name;
        if (!m.names.contains(e.address)) m.names[e.address] = unique(name, e.address);
    }
    for (const formats::psp::Import& i : formats::psp::imports(program.elf)) {
        if (i.variable) continue;
        const std::string name = i.name.empty() ? std::format("{}_{:08X}", clean(i.library), i.nid) : i.name;
        if (!m.names.contains(i.stub)) m.names[i.stub] = unique(name, i.stub);
    }
    return mips_elf_writer::write(m, program.file, program.elf, program.elf_offset, file_name, folder);
}

image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    const std::optional<formats::psp::Program> program = formats::psp::read(file);
    if (!program) throw FormatError("psp", 0, "not an unencrypted PSP program (an ELF, a PRX, or an EBOOT.PBP holding one)");
    return write(*program, file.filename().string(), folder);
}

}  // namespace cb::source::psp_writer
