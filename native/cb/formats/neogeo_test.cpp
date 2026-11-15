#include "cb/formats/neogeo.hpp"

#include <cstring>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/detect.hpp"
#include "cb/source/isa.hpp"

namespace cb::formats::neogeo {
namespace {

void put_long(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

void put_word(Bytes& b, std::size_t at, std::uint16_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 8);
    b[at + 1] = static_cast<std::uint8_t>(v);
}

void put_bytes(Bytes& b, std::size_t at, std::initializer_list<std::uint8_t> bytes) { std::copy(bytes.begin(), bytes.end(), b.begin() + static_cast<std::ptrdiff_t>(at)); }

// A P-ROM: the vectors (the stack, the BIOS's reset, a vertical blank of its own at $500), the header with four callbacks, and
//   $400 JSR ($C00450).L ; MOVE.W #7,($3C0000).L ; RTS          (CREDIT_CHECK, then the LSPC's address register)
//   $410 NOP ; RTS
//   $420 BSR $400 ; RTS
//   $500 RTE
Bytes prom(std::size_t size = 0x1000) {
    Bytes b(size, 0xFF);
    std::fill(b.begin(), b.begin() + 0x200, std::uint8_t{0});
    put_long(b, 0, 0x0010F300);
    put_long(b, 4, 0x00C00402);
    put_long(b, 4 * 25, 0x500);
    put_long(b, 4 * 26, 0x00C00426);   // the raster timer is the BIOS's
    std::memcpy(b.data() + 0x100, "NEO-GEO", 7);
    put_word(b, 0x108, 0x0123);
    put_long(b, 0x10A, static_cast<std::uint32_t>(size));
    put_long(b, 0x10E, 0x0010E000);
    put_word(b, 0x112, 0x0100);
    b[0x114] = 1;
    b[0x115] = 2;
    put_long(b, 0x116, 0x00000200);
    put_long(b, 0x11A, 0x00000210);
    put_long(b, 0x11E, 0x00000220);
    put_bytes(b, 0x122, {0x4E, 0xF9, 0, 0, 0x04, 0x00});   // JMP $400
    put_bytes(b, 0x128, {0x4E, 0xF9, 0, 0, 0x04, 0x10});
    put_bytes(b, 0x12E, {0x4E, 0xF9, 0, 0, 0x04, 0x20});
    put_bytes(b, 0x134, {0x4E, 0xF9, 0, 0, 0x04, 0x20});
    put_bytes(b, 0x400, {0x4E, 0xB9, 0x00, 0xC0, 0x04, 0x50, 0x33, 0xFC, 0x00, 0x07, 0x00, 0x3C, 0x00, 0x00, 0x4E, 0x75});
    put_bytes(b, 0x410, {0x4E, 0x71, 0x4E, 0x75});
    put_bytes(b, 0x420, {0x61, 0x00, 0xFF, 0xDE, 0x4E, 0x75});
    put_bytes(b, 0x500, {0x4E, 0x73});
    return b;
}

// A `.neo` file: the 4 KB header, then P, S and M.
Bytes neo_file(const Bytes& p, const Bytes& s, const Bytes& m) {
    Bytes b(kNeoHeader, 0);
    put_bytes(b, 0, {'N', 'E', 'O', 1});
    const auto le32 = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    le32(4, static_cast<std::uint32_t>(p.size()));
    le32(8, static_cast<std::uint32_t>(s.size()));
    le32(12, static_cast<std::uint32_t>(m.size()));
    le32(28, 2026);
    le32(40, 0x0123);
    std::memcpy(b.data() + 44, "Test Game", 9);
    std::memcpy(b.data() + 77, "Test Maker", 10);
    b.insert(b.end(), p.begin(), p.end());
    b.insert(b.end(), s.begin(), s.end());
    b.insert(b.end(), m.begin(), m.end());
    return b;
}

CB_TEST("formats.neogeo.header_and_vectors", [] {
    const Rom rom = parse(prom());
    CB_CHECK(rom.layout == Layout::Plain);
    CB_CHECK_EQ(rom.header.ngh, 0x0123);
    CB_CHECK_EQ(rom.header.program_size, 0x1000u);
    CB_CHECK_EQ(rom.header.backup_ram, 0x0010E000u);
    CB_CHECK_EQ(rom.header.save_size, 0x0100);
    CB_CHECK_EQ(rom.header.eye_catcher, 1);
    CB_CHECK_EQ(rom.header.eye_catcher_bank, 2);
    CB_CHECK((rom.header.settings == std::array<std::uint32_t, 3>{0x200, 0x210, 0x220}));
    CB_CHECK((rom.header.callbacks == std::array<std::uint32_t, 4>{0x400, 0x410, 0x420, 0x420}));
    CB_CHECK_EQ(rom.initial_ssp, 0x0010F300u);
    CB_CHECK_EQ(rom.reset, 0x00C00402u);   // the BIOS's
    // only the vector that points into the P-ROM is kept: the raster timer is the BIOS's
    CB_CHECK_EQ(rom.vectors.size(), 1u);
    CB_CHECK_EQ(rom.vectors[0].name, "vblank");
    CB_CHECK_EQ(rom.vectors[0].address, 0x500u);
    CB_CHECK_EQ(vector_name(26), "timer");
    CB_CHECK_EQ(vector_name(33), "trap1");
    CB_CHECK_EQ(vector_name(12), "");
});

CB_TEST("formats.neogeo.functions_start_where_the_callbacks_jump_and_the_vblank_is", [] {
    const Rom rom = parse(prom());
    const std::vector<std::uint32_t> got = rom.functions(source::isa::m68k_decoder());
    CB_CHECK((got == std::vector<std::uint32_t>{0x400, 0x410, 0x420, 0x500}));   // $420 calls $400 and the vblank handler is its own start
    CB_CHECK((rom.callback_jumps() == std::vector<std::uint32_t>{0x122, 0x128, 0x12E, 0x134}));
    Bytes none = prom();
    put_bytes(none, 0x128, {0, 0, 0, 0, 0, 0});   // no JMP at $128: that callback is not set
    const Rom without = parse(none);
    CB_CHECK_EQ(without.header.callbacks[1], 0u);
    CB_CHECK((without.callback_jumps() == std::vector<std::uint32_t>{0x122, 0x12E, 0x134}));
});

CB_TEST("formats.neogeo.the_security_code_the_header_points_to_is_a_function", [] {
    Bytes b = prom();
    CB_CHECK_EQ(parse(b).header.security, 0u);
    put_long(b, 0x182, 0x300);
    put_bytes(b, 0x300, {0x4E, 0x75});   // RTS
    const Rom rom = parse(b);
    CB_CHECK_EQ(rom.header.security, 0x300u);
    CB_CHECK((rom.functions(source::isa::m68k_decoder()) == std::vector<std::uint32_t>{0x300, 0x400, 0x410, 0x420, 0x500}));
    put_long(b, 0x182, 0x301);   // an odd address is not code
    CB_CHECK((parse(b).functions(source::isa::m68k_decoder()) == std::vector<std::uint32_t>{0x400, 0x410, 0x420, 0x500}));
});

CB_TEST("formats.neogeo.a_word_swapped_prom_is_put_in_the_68000s_order", [] {
    const Bytes plain = prom();
    const Bytes swapped = swap_words(plain);
    CB_CHECK(swapped != plain);
    CB_CHECK_EQ(swapped[0x100], 'E');   // "EN-OEG": the header's text with each pair reversed
    const Rom rom = parse(swapped);
    CB_CHECK(rom.layout == Layout::WordSwapped);
    CB_CHECK(rom.data == plain);
    CB_CHECK_EQ(rom.header.ngh, 0x0123);
    CB_CHECK((rom.header.callbacks == std::array<std::uint32_t, 4>{0x400, 0x410, 0x420, 0x420}));
});

CB_TEST("formats.neogeo.two_halves_join_into_the_prom", [] {
    const Bytes plain = prom();
    Bytes even, odd;
    for (std::size_t i = 0; i < plain.size(); i += 2) {
        even.push_back(plain[i]);
        odd.push_back(plain[i + 1]);
    }
    CB_CHECK(join_halves(even, odd) == plain);
    odd.pop_back();
    CB_CHECK_THROWS_AS(FormatError, (void)join_halves(even, odd));
});

CB_TEST("formats.neogeo.a_neo_container_gives_the_prom_and_the_sound_program", [] {
    const Bytes m = {0xF3, 0xC3, 0x00, 0x01, 0xAA, 0xBB, 0xCC, 0xDD};
    const Rom rom = parse(neo_file(prom(), Bytes(0x20, 7), m), ".neo");
    CB_CHECK(rom.layout == Layout::Neo);
    CB_CHECK(rom.data == prom());
    CB_CHECK(rom.sound == m);
    CB_CHECK(rom.neo.has_value());
    if (!rom.neo) return;
    CB_CHECK_EQ(rom.neo->p_size, 0x1000u);
    CB_CHECK_EQ(rom.neo->s_size, 0x20u);
    CB_CHECK_EQ(rom.neo->m_size, 8u);
    CB_CHECK_EQ(rom.neo->year, 2026u);
    CB_CHECK_EQ(rom.neo->ngh, 0x0123u);
    CB_CHECK_EQ(rom.neo->name, "Test Game");
    CB_CHECK_EQ(rom.neo->manufacturer, "Test Maker");
    // a .neo whose P-ROM is word-swapped is put in order as well, and stays a .neo
    const Rom swapped = parse(neo_file(swap_words(prom()), {}, {}), ".neo");
    CB_CHECK(swapped.layout == Layout::Neo);
    CB_CHECK(swapped.data == prom());
    // a P section that runs past the file is a format error
    Bytes cut = neo_file(prom(), {}, {});
    cut.resize(kNeoHeader + 0x100);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(cut, ".neo"));
});

CB_TEST("formats.neogeo.the_second_megabyte_is_in_the_bank_window", [] {
    Bytes big = prom(0x180000);
    big[0x100000] = 0xA5;
    big[0x17FFFF] = 0x5A;
    const Rom rom = parse(big);
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem.size(), 0x1000000u);
    CB_CHECK_EQ(mem[0x400], 0x4E);
    CB_CHECK_EQ(mem[0x200000], 0xA5);
    CB_CHECK_EQ(mem[0x27FFFF], 0x5A);
    CB_CHECK_EQ(mem[0x280000], 0);   // past the end of the P-ROM
    CB_CHECK_EQ(Rom::address_of(0x0FFFFF), 0x0FFFFFu);
    CB_CHECK_EQ(Rom::address_of(0x100000), 0x200000u);
    CB_CHECK_EQ(rom.offset_of(0x200004), 0x100004u);
    CB_CHECK_EQ(rom.offset_of(0x300000), rom.data.size());
    CB_CHECK_EQ(rom.offset_of(0x1000), 0x1000u);
});

CB_TEST("formats.neogeo.what_is_not_a_prom_is_refused", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x100, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x400, 0)));   // no header
});

CB_TEST("formats.neogeo.probe_reads_the_header", [] {
    fs::ScopedTempDir dir("cb_neogeo");
    fs::write_file(dir.path() / "game.p1", prom());
    fs::write_file(dir.path() / "swapped.bin", swap_words(prom()));
    fs::write_file(dir.path() / "game.neo", neo_file(prom(), {}, {}));
    fs::write_file(dir.path() / "other.p1", Bytes(0x400, 0));
    fs::write_file(dir.path() / "short.p1", Bytes(0x10, 0));
    CB_CHECK(probe(dir.path() / "game.p1"));
    CB_CHECK(probe(dir.path() / "swapped.bin"));
    CB_CHECK(probe(dir.path() / "game.neo"));
    CB_CHECK(!probe(dir.path() / "other.p1"));
    CB_CHECK(!probe(dir.path() / "short.p1"));
    CB_CHECK(!probe(dir.path() / "missing.p1"));
    CB_CHECK(read(dir.path() / "swapped.bin").data == prom());
});

CB_TEST("formats.neogeo.identify_names_the_cartridge_and_the_neo_file", [] {
    const auto plain = detect::identify(prom(), "game-p1.p1");
    CB_CHECK_EQ(plain.size(), 1u);
    CB_CHECK_EQ(plain[0].format, "Neo Geo cartridge (P-ROM)");
    CB_CHECK_EQ(plain[0].detail, "NGH 0123, 4 KB");
    CB_CHECK_EQ(plain[0].family, "neogeo");
    CB_CHECK_EQ(plain[0].command, "reach");
    CB_CHECK_EQ(detect::identify(swap_words(prom()), "game-p1.p1")[0].detail, "NGH 0123, 4 KB, words swapped");
    const auto neo = detect::identify(neo_file(prom(), Bytes(0x400, 0), Bytes(0x800, 0)), "game.neo");
    CB_CHECK_EQ(neo.size(), 1u);
    CB_CHECK_EQ(neo[0].format, "Neo Geo .neo file");
    CB_CHECK_EQ(neo[0].detail, "4 KB P-ROM, 2 KB sound program, 0 KB sprites");
    CB_CHECK(detect::identify(Bytes(0x400, 0), "other.bin").empty());
});

}  // namespace
}  // namespace cb::formats::neogeo
