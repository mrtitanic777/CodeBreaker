#include "cb/formats/gba.hpp"

#include <cstring>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/arm.hpp"

namespace cb::formats::gba {
namespace {

void put_word(Bytes& b, std::size_t at, std::uint32_t v) {
    for (std::size_t i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}

// A cartridge: the header (the entry branch to $C0, the logo, "TESTROM", AGB-TEST, maker "01", the checksum), then `program` (words) at $C0.
Bytes image(const std::vector<std::uint32_t>& program) {
    Bytes b(kHeaderSize + program.size() * 4, 0);
    put_word(b, 0, 0xEA00002E);   // b $C0: 8 + 4 * 0x2E
    std::copy(kLogo.begin(), kLogo.end(), b.begin() + 4);
    std::memcpy(b.data() + 0xA0, "TESTROM", 7);
    std::memcpy(b.data() + 0xAC, "ATST", 4);
    std::memcpy(b.data() + 0xB0, "01", 2);
    b[0xB2] = kFixedValue;
    b[0xBC] = 2;
    b[0xBD] = header_complement(b);
    for (std::size_t i = 0; i < program.size(); ++i) put_word(b, kHeaderSize + i * 4, program[i]);
    return b;
}

CB_TEST("formats.gba.header_fields", [] {
    const Rom rom = parse(image({0xE12FFF1E}));
    CB_CHECK_EQ(rom.header.entry_word, 0xEA00002Eu);
    CB_CHECK(rom.header.entry.has_value());
    CB_CHECK_EQ(*rom.header.entry, 0x080000C0u);
    CB_CHECK(rom.header.logo_ok);
    CB_CHECK_EQ(rom.header.title, "TESTROM");
    CB_CHECK_EQ(rom.header.game_code, "ATST");
    CB_CHECK_EQ(rom.header.maker_code, "01");
    CB_CHECK_EQ(rom.header.version, std::uint8_t{2});
    CB_CHECK(rom.header.complement_ok());
    CB_CHECK_EQ(rom.end(), 0x080000C4u);
    // the checksum, worked by hand: the bytes $A0..$BC are "TESTROM" (84+69+83+84+82+79+77 = 558), "ATST" (65+84+83+84 = 316),
    // "01" (48+49 = 97), 0x96 (150) and the version 2; 558 + 316 + 97 + 150 + 2 + 25 = 1148 = 0x47C; the check byte is -0x47C = 0x84 (mod 256)
    CB_CHECK_EQ(rom.data[0xBD], std::uint8_t{0x84});
    Bytes bad = image({0});
    bad[0xBD] ^= 1;
    CB_CHECK(!parse(bad).header.complement_ok());
    Bytes wrong_logo = image({0});
    wrong_logo[10] ^= 0xFF;
    CB_CHECK(!parse(wrong_logo).header.logo_ok);
    // a backward entry branch: b to itself is 0xEAFFFFFE: 0x08000000
    Bytes back = image({0});
    put_word(back, 0, 0xEAFFFFFE);
    CB_CHECK_EQ(*parse(back).header.entry, 0x08000000u);
    // an entry that is not a branch has no entry address
    Bytes none = image({0});
    put_word(none, 0, 0xE3A00000);
    CB_CHECK(!parse(none).header.entry.has_value());
});

CB_TEST("formats.gba.memory_is_the_rom_as_the_console_maps_it", [] {
    const Rom rom = parse(image({0x12345678}));
    CB_CHECK_EQ(rom.memory().size(), std::size_t{0xC4});
    CB_CHECK_EQ(rom.memory()[0xC0], std::uint8_t{0x78});
    CB_CHECK_EQ(rom.memory()[0xC3], std::uint8_t{0x12});
});

// The decoder the code scan asks for: the ARM7TDMI as decompile/arm knows it (the state is the address's lowest bit).
const Decoder kDecode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
    const auto insn = decompile::arm::decode(mem, pc & ~1u, pc);
    if (!insn) return std::nullopt;
    DecodedInsn out{insn->length, Flow::Next, insn->target};
    switch (insn->flow) {
        case decompile::arm::Flow::Jump: out.flow = Flow::Jump; break;
        case decompile::arm::Flow::Branch: out.flow = Flow::Branch; break;
        case decompile::arm::Flow::Call: out.flow = Flow::Call; break;
        case decompile::arm::Flow::Return: out.flow = Flow::Return; break;
        case decompile::arm::Flow::Indirect: out.flow = Flow::Indirect; break;
        default: break;
    }
    return out;
};

CB_TEST("formats.gba.functions_are_the_entry_and_what_it_calls", [] {
    // $C0: bl $C8; b $C4 (itself); $C8: bx lr
    const Rom rom = parse(image({0xEB000000, 0xEAFFFFFE, 0xE12FFF1E}));
    CB_CHECK((rom.functions(kDecode) == std::vector<std::uint32_t>{0x08000000, 0x080000C0, 0x080000C8}));
    // a Thumb function reached by a Thumb call: an odd address ($C0: bl +0, which is $C4; $C4: bx lr in Thumb after the pair)
    //   ARM $C0: ldr-free: `b $C8` is not needed; put the Thumb call in an ARM-reachable place instead: the code scan follows ARM only
    //   from the entry, so a Thumb function is found only when ARM code branches into it: not possible without bx. The scan finds the ARM ones.
    const Rom thumb_only = parse(image({0xE12FFF1E}));
    CB_CHECK((thumb_only.functions(kDecode) == std::vector<std::uint32_t>{0x08000000, 0x080000C0}));
});

CB_TEST("formats.gba.parse_refuses_what_is_not_a_cartridge", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, parse(Bytes(0x100, 0)));   // no $96
    CB_CHECK_THROWS_AS(cb::FormatError, parse(Bytes(0x20, 0)));    // shorter than the header
    CB_CHECK_THROWS_AS(cb::FormatError, parse(Bytes{}));
    // a header and no program is a cartridge with nothing in it
    CB_CHECK_EQ(parse(image({})).data.size(), std::size_t{0xC0});
});

CB_TEST("formats.gba.probe_reads_the_mark_and_the_entry_branch", [] {
    fs::ScopedTempDir dir("cb_gba");
    fs::write_file(dir.path() / "game.gba", image({0, 0}));
    fs::write_file(dir.path() / "game.bin", image({0, 0}));   // (the extension says nothing)
    Bytes not_a_branch = image({0, 0});
    put_word(not_a_branch, 0, 0xE3A00000);
    fs::write_file(dir.path() / "entry.gba", not_a_branch);
    Bytes no_mark = image({0, 0});
    no_mark[0xB2] = 0;
    fs::write_file(dir.path() / "mark.gba", no_mark);
    fs::write_file(dir.path() / "short.gba", Bytes(0x40, 0));
    CB_CHECK(probe(dir.path() / "game.gba"));
    CB_CHECK(probe(dir.path() / "game.bin"));
    CB_CHECK(!probe(dir.path() / "entry.gba"));
    CB_CHECK(!probe(dir.path() / "mark.gba"));
    CB_CHECK(!probe(dir.path() / "short.gba"));
    CB_CHECK(!probe(dir.path() / "missing.gba"));
    CB_CHECK_EQ(read(dir.path() / "game.gba").header.title, "TESTROM");
});

}  // namespace
}  // namespace cb::formats::gba
