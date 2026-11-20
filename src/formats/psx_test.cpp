#include "formats/psx.hpp"

#include <cstring>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/mips.hpp"

namespace cb::formats::psx {
namespace {

void put_word(Bytes& b, std::size_t at, std::uint32_t v) {
    for (std::size_t i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}

// An executable: the header, then `program` (words) at $80010000; pc0 is its first word.
Bytes image(const std::vector<std::uint32_t>& program, std::uint32_t pc0 = 0x80010000, std::uint32_t t_addr = 0x80010000) {
    Bytes b(kHeaderSize + program.size() * 4, 0);
    std::memcpy(b.data(), "PS-X EXE", 8);
    put_word(b, 0x10, pc0);
    put_word(b, 0x14, 0x80018000);
    put_word(b, 0x18, t_addr);
    put_word(b, 0x1C, static_cast<std::uint32_t>(program.size() * 4));
    put_word(b, 0x30, 0x801FFFF0);
    const std::string_view marker = "Sony Computer Entertainment Inc. for North America area";
    std::memcpy(b.data() + 0x4C, marker.data(), marker.size());
    for (std::size_t i = 0; i < program.size(); ++i) put_word(b, kHeaderSize + i * 4, program[i]);
    return b;
}

CB_TEST("formats.psx.header_fields", [] {
    const Exe exe = parse(image({0x03E00008, 0}));
    CB_CHECK_EQ(exe.header.pc0, 0x80010000u);
    CB_CHECK_EQ(exe.header.gp0, 0x80018000u);
    CB_CHECK_EQ(exe.header.t_addr, 0x80010000u);
    CB_CHECK_EQ(exe.header.t_size, 8u);
    CB_CHECK_EQ(exe.header.sp_base, 0x801FFFF0u);
    CB_CHECK_EQ(exe.header.sp_offset, 0u);
    CB_CHECK_EQ(exe.header.marker, "Sony Computer Entertainment Inc. for North America area");
    CB_CHECK_EQ(exe.text().size(), std::size_t{8});
    CB_CHECK_EQ(exe.text_end(), 0x80010008u);
    CB_CHECK_EQ(exe.region(), 0x80000000u);
});

CB_TEST("formats.psx.memory_is_the_ram_with_the_text_at_t_addr", [] {
    const Exe exe = parse(image({0x03E00008, 0x12345678}));
    const Bytes ram = exe.memory();
    CB_CHECK_EQ(ram.size(), kRamSize);
    CB_CHECK_EQ(ram[0x10000], std::uint8_t{0x08});
    CB_CHECK_EQ(ram[0x10003], std::uint8_t{0x03});
    CB_CHECK_EQ(ram[0x10004], std::uint8_t{0x78});
    CB_CHECK_EQ(ram[0x1000F], std::uint8_t{0});
    CB_CHECK_EQ(ram[0], std::uint8_t{0});
    // a program linked in KUSEG (no region bits)
    const Exe low = parse(image({0x03E00008, 0}, 0x00010000, 0x00010000));
    CB_CHECK_EQ(low.region(), 0u);
    CB_CHECK_EQ(low.memory()[0x10000], std::uint8_t{0x08});
});

// The decoder the code scan asks for: the R3000A as decompile/mips knows it (a jump, return or indirect jump ends a run).
const Decoder kDecode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
    const auto insn = decompile::mips::decode(mem, pc, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out{4, Flow::Next, insn->target};
    switch (insn->flow) {
        case decompile::mips::Flow::Jump: out.flow = Flow::Jump; break;
        case decompile::mips::Flow::Branch: out.flow = Flow::Branch; break;
        case decompile::mips::Flow::Call: out.flow = Flow::Call; break;
        case decompile::mips::Flow::Return: out.flow = Flow::Return; break;
        case decompile::mips::Flow::Indirect: out.flow = Flow::Indirect; break;
        default: break;
    }
    return out;
};

CB_TEST("formats.psx.functions_are_the_entry_and_what_it_calls", [] {
    // 0000: jal $80010010; nop; jr $ra; nop      0010: addiu $sp,$sp,-8; jr $ra; nop
    const Exe exe = parse(image({0x0C004004, 0, 0x03E00008, 0, 0x27BDFFF8, 0x03E00008, 0}));
    // (the program has 7 words; the jal's target is word 4 = $80010010)
    const std::vector<std::uint32_t> found = exe.functions(kDecode);
    CB_CHECK((found == std::vector<std::uint32_t>{0x80010000, 0x80010010}));
    // a call out of the text (a BIOS call at $A0) is not a function of the program
    const Exe bios = parse(image({0x0C000028, 0, 0x03E00008, 0}));
    CB_CHECK((bios.functions(kDecode) == std::vector<std::uint32_t>{0x80010000}));
});

CB_TEST("formats.psx.parse_refuses_what_is_not_an_executable", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, parse(Bytes(0x900, 0)));
    Bytes short_file(0x100, 0);
    std::memcpy(short_file.data(), "PS-X EXE", 8);
    CB_CHECK_THROWS_AS(cb::FormatError, parse(short_file));
    CB_CHECK_THROWS_AS(cb::FormatError, parse(Bytes{}));
    // a header and no program is an executable with nothing in it
    CB_CHECK_EQ(parse(image({})).text().size(), std::size_t{0});
});

CB_TEST("formats.psx.probe_reads_the_magic", [] {
    fs::ScopedTempDir dir("cb_psx");
    fs::write_file(dir.path() / "game.exe", image({0, 0}));
    fs::write_file(dir.path() / "SLUS_000.00", image({0, 0}));
    fs::write_file(dir.path() / "other.exe", Bytes(0x900, 0x4D));
    CB_CHECK(probe(dir.path() / "game.exe"));
    CB_CHECK(probe(dir.path() / "SLUS_000.00"));   // (a disc's executable has no extension)
    CB_CHECK(!probe(dir.path() / "other.exe"));
    CB_CHECK(!probe(dir.path() / "missing.exe"));
    CB_CHECK_EQ(read(dir.path() / "game.exe").header.t_size, 8u);
});

}  // namespace
}  // namespace cb::formats::psx
