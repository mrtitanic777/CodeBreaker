// source/sega32x_writer.cpp -- a Sega 32X cartridge as source that builds back into it (sega32x_writer.hpp).

#include "source/sega32x_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "formats/genesis.hpp"
#include "source/genesis_writer.hpp"
#include "source/isa_sh2.hpp"
#include "source/sh2_writer.hpp"

namespace cb::source::sega32x_writer {

namespace iw = image_writer;
namespace f32 = formats::sega32x;

const std::map<std::uint32_t, std::string>& hardware() {
    // the 32X's own registers (config/knowledge/sega32x/registers.json) and the SH-2's (sh2/registers.json), which name an address both do
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t = knowledge::standard().hardware_names("sega32x", "hardware");
        for (const auto& [a, n] : sh2_writer::chip_registers()) t[a] = n;
        return t;
    }();
    return table;
}

iw::Written write(const f32::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();

    // the 68000 side: the Genesis writer's, over the whole ROM (the SH-2 program is data in it)
    formats::genesis::Rom genesis = formats::genesis::parse(rom.data, ".32x");
    iw::Written gen = genesis_writer::write(genesis, file_name, folder);

    iw::Written r;
    r.source = gen.source;
    r.functions = gen.functions;
    r.notes = gen.notes;
    r.linked = gen.linked;
    r.code_bytes = gen.code_bytes;
    r.image_bytes = rom.data.size();
    r.same = false;

    Bytes rebuilt = rom.data;   // the ROM as the two sources build it
    if (gen.built && gen.linked) rebuilt = fs::read_file(*gen.built);
    if (rebuilt.size() != rom.data.size()) rebuilt.resize(rom.data.size(), 0);

    if (rom.has_sh2_program()) {
        const ByteView program = rom.sh2_program();
        sh2_writer::Image image;
        image.bytes.assign(program.begin(), program.end());
        image.parts = {{"sh2_program", 0, static_cast<std::uint32_t>(program.size()), f32::kSdram, true, true}};
        const auto start_of = [&](std::uint32_t a) { return a & 0x1FFFFFFF; };
        if (rom.in_program(rom.header.master_start)) image.names[start_of(rom.header.master_start)] = "master_entry";
        if (rom.in_program(rom.header.slave_start) && !image.names.contains(start_of(rom.header.slave_start))) image.names[start_of(rom.header.slave_start)] = "slave_entry";
        for (const std::uint32_t a : rom.functions(isa::sh2_decoder())) image.entries.push_back(a);
        image.hardware = &hardware();
        const std::uint32_t program_end = f32::kSdram + static_cast<std::uint32_t>(program.size());
        image.is_variable = [program_end](std::uint32_t a) {
            return (a >= f32::kSdram && a < f32::kSdram + f32::kSdramSize && a >= program_end) || (a >= f32::kFrameBuffer && a < f32::kFrameBuffer + f32::kFrameBufferSize);
        };
        image.module = std::string(file_name);
        image.head = {std::format("; {}.sh2.s -- the SH-2 program of {}, written by CodeBreaker", stem, base_name),
                      std::format("; \"{}\": {} bytes from ROM offset 0x{:X}, copied to SDRAM at 0x{:08X}; master starts at 0x{:08X}, slave at 0x{:08X}",
                                  rom.header.module_name, program.size(), rom.header.sh2_offset, f32::kSdram, rom.header.master_start, rom.header.slave_start),
                      std::format("; the 68000 side is {}.asm", stem)};
        iw::Written sh = sh2_writer::write(image, stem + ".sh2", folder, stem + ".sh2.built.bin");
        r.source = sh.source;
        for (const auto& [a, n] : sh.functions) r.functions[a] = n;
        r.code_bytes += sh.code_bytes;
        for (const std::string& n : sh.notes) r.notes.push_back("SH-2 program: " + n);
        r.linked = r.linked && sh.linked;
        if (sh.built && sh.linked) {
            const Bytes built = fs::read_file(*sh.built);
            if (built.size() == program.size()) std::copy(built.begin(), built.end(), rebuilt.begin() + rom.header.sh2_offset);
        }
    } else {
        r.notes.push_back("no SH-2 program in the ROM (the header names none that lies in the ROM and in SDRAM)");
    }

    const std::filesystem::path built_path = folder / (stem + ".built.32x");
    fs::write_file(built_path, rebuilt);
    r.built = built_path;
    r.same = r.linked && rebuilt == rom.data;
    if (r.linked && !r.same) r.notes.push_back("the ROM built from the sources differs from the original");
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(f32::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::sega32x_writer
