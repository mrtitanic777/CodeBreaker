// source/console_sega32x.cpp -- the executable shape's container `sega32x` (console_executable.cpp): a Sega 32X cartridge: its 68000 side through the console writer's Genesis, its SH-2 program through source/sh2_writer, the two built back into the ROM
// (config/platforms/sega32x.json).
//
//     r.source                  game.sh2.s (the SH-2 program, GNU as syntax); the 68000 side is game.asm
//     r.same                    both sources, built, put the cartridge back byte for byte
//     r.functions               {address: name}: the SH-2s' at $06000000 and up, the 68000's below $400000
//
// A 32X cartridge is two programs in one ROM (formats/sega32x): the Genesis's 68000 code, which source/console_writer writes as Motorola
// syntax (`<stem>.asm`, the whole ROM: the SH-2 program is data in it), and the SH-2 program the add-on's two processors run, which
// source/sh2_writer writes as GNU as source (`<stem>.sh2.s`): the vector tables, then code and data at $06000000, where the 32X copies it.
// Each is assembled by CodeBreaker's own assemblers and compared with its bytes; the ROM is put back together from the two (the 68000
// side's rebuilt ROM with the SH-2 program's rebuilt bytes over the program's place) and compared with the file: `<stem>.built.32x`.
//
// The two SH-2s share the program: the master starts at the header's first address, the slave at the second. Functions are `master_entry`
// and `slave_entry` for the starts, `sub_<address>`, and the 32X's registers (the system registers, the communication ports, PWM, the VDP's)
// and the SH7604's own (the divide unit, the DMA controller, the timer ...) are written by name.
//
// The head and the rebuilt file's name are the description's.

#include "source/console_shapes.hpp"
#include "formats/sega32x.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "formats/genesis.hpp"
#include "source/console_writer.hpp"
#include "source/isa_sh2.hpp"
#include "source/sh2_writer.hpp"

namespace cb::source::console_writer::sega32x_code {

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

iw::Written write(const Description& description, const Loaded& loaded, const Values& values, const std::filesystem::path& folder) {
    const f32::Rom& rom = contained_as<f32::Rom>(loaded);
    fs::make_dirs(folder);
    const std::string file_name = std::get<std::string>(values.at("file"));
    const std::string stem = std::get<std::string>(values.at("stem"));

    // the 68000 side: the Genesis's (the console writer), over the whole ROM (the SH-2 program is data in it)
    formats::genesis::Rom genesis = formats::genesis::parse(rom.data, ".32x");
    iw::Written gen = console_writer::write(describe("genesis"), load_genesis(genesis), file_name, folder);

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
        image.head = head_comment(description, values);
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

    const std::filesystem::path built_path = folder / fill(description.executable.built, values);
    fs::write_file(built_path, rebuilt);
    r.built = built_path;
    r.same = r.linked && rebuilt == rom.data;
    if (r.linked && !r.same) r.notes.push_back("the ROM built from the sources differs from the original");
    return r;
}


}  // namespace cb::source::console_writer::sega32x_code
