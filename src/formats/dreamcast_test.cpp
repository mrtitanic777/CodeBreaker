// cb-unity: alone (file-local names that other files in a batch also use)
// formats/dreamcast_test.cpp -- Dreamcast discs: IP.BIN's header field by field, 1ST_READ.BIN found through the volume (cooked, raw, a .cdi searched
// for its boot track, a .gdi's last data track whose volume is numbered from the start of the disc), the scramble and its detection, and what is refused.
#include "formats/dreamcast.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/dreamcast_test_programs.hpp"
#include "source/isa_sh2.hpp"

namespace dc = cb::formats::dreamcast;
namespace t = cb::source::dreamcast_test;
using cb::Bytes;

namespace {

Bytes pseudo_random(std::size_t n) {
    Bytes b(n);
    std::uint32_t x = 12345;
    for (std::size_t i = 0; i < n; ++i) {
        x = x * 1103515245u + 12345u;
        b[i] = static_cast<std::uint8_t>(x >> 16);
    }
    return b;
}

}  // namespace

CB_TEST("formats.dreamcast.the_scramble_is_an_exact_inverse", [] {
    for (const std::size_t n : {std::size_t{0}, std::size_t{31}, std::size_t{32}, std::size_t{100}, std::size_t{4096}, std::size_t{2 * 1024 * 1024 + 777},
                                std::size_t{4 * 1024 * 1024 + 64}}) {
        const Bytes plain = pseudo_random(n);
        const Bytes scrambled = dc::scramble(plain);
        CB_CHECK_EQ(scrambled.size(), n);
        CB_CHECK(dc::descramble(scrambled) == plain);
        if (n >= 4096) CB_CHECK(scrambled != plain);
        // the slices are moved, not changed: the multiset of bytes stays
        if (n == 4096) {
            Bytes a = plain, b = scrambled;
            std::sort(a.begin(), a.end());
            std::sort(b.begin(), b.end());
            CB_CHECK(a == b);
        }
    }
    // the bytes after the last whole slice of a chunk stay where they are
    const Bytes odd = pseudo_random(100);
    const Bytes s = dc::scramble(odd);
    CB_CHECK(std::equal(odd.begin() + 96, odd.end(), s.begin() + 96));
});

CB_TEST("formats.dreamcast.the_scramble_is_found_by_what_it_does_to_delay_slots", [] {
    const Bytes program = t::assemble(t::big_source());
    CB_CHECK(!dc::looks_scrambled(program));
    const Bytes scrambled = dc::scramble(program);
    CB_CHECK(scrambled != program);
    CB_CHECK(dc::looks_scrambled(scrambled));
    CB_CHECK(!dc::looks_scrambled(dc::descramble(scrambled)));
    CB_CHECK(!dc::looks_scrambled(Bytes(10, 0)));
});

CB_TEST("formats.dreamcast.an_iso_gives_ip_bin_field_by_field_and_the_first_program", [] {
    const Bytes disc = t::iso(false);
    CB_CHECK(dc::is_disc_start(disc));
    const dc::Disc d = dc::parse(disc);
    CB_CHECK(d.container == dc::Container::Iso);
    CB_CHECK_EQ(d.header.hardware_id, std::string("SEGA SEGAKATANA"));
    CB_CHECK_EQ(d.header.maker, std::string("SEGA ENTERPRISES"));
    CB_CHECK_EQ(d.header.device, std::string("0000 GD-ROM1/1"));
    CB_CHECK_EQ(d.header.areas, std::string("JUE"));
    CB_CHECK_EQ(d.header.peripherals, std::string("E000F10"));
    CB_CHECK_EQ(d.header.product, std::string("T-00000"));
    CB_CHECK_EQ(d.header.version, std::string("V1.000"));
    CB_CHECK_EQ(d.header.release_date, std::string("20261115"));
    CB_CHECK_EQ(d.header.boot_file, std::string("1ST_READ.BIN"));
    CB_CHECK_EQ(d.header.software_maker, std::string("CODEBREAKER TEST"));
    CB_CHECK_EQ(d.header.title, std::string("DREAMCAST TEST DISC"));
    CB_CHECK_EQ(d.first_read_name, std::string("1ST_READ.BIN"));
    CB_CHECK(!d.scrambled);
    CB_CHECK(d.program == t::assemble(t::kProgram));
    CB_CHECK_EQ(d.system.size(), dc::kSystemBytes);
    CB_CHECK_EQ(d.ip_length(), 0x8000u);
    CB_CHECK_EQ(d.load_address(), dc::kFirstReadBase);
    const Bytes mem = d.memory();   // IP.BIN, then the program exactly where its 32 KB end
    CB_CHECK_EQ(mem.size(), dc::kSystemBytes + d.program.size());
    CB_CHECK(std::equal(d.program.begin(), d.program.end(), mem.begin() + dc::kSystemBytes));
    const std::vector<std::uint32_t> functions = d.functions(cb::source::isa::sh4_decoder());
    CB_CHECK(std::find(functions.begin(), functions.end(), 0x0C008300u) != functions.end());   // the bootstrap
    CB_CHECK(std::find(functions.begin(), functions.end(), 0x0C010000u) != functions.end());   // the program's start
    CB_CHECK(functions.size() >= 3u);                                                           // and the one it calls
});

CB_TEST("formats.dreamcast.a_scrambled_first_program_is_read_descrambled", [] {
    const Bytes program = t::assemble(t::big_source());
    const dc::Disc d = dc::parse(t::iso(program, true));
    CB_CHECK(d.scrambled);
    CB_CHECK(d.program == program);
    CB_CHECK(d.first_read == dc::scramble(program));
    CB_CHECK(!dc::parse(t::iso(program, false)).scrambled);
});

CB_TEST("formats.dreamcast.raw_sectors_a_cdi_and_a_gdi_are_read_too", [] {
    const Bytes program = t::assemble(t::kProgram);
    const Bytes cooked = t::iso(false);
    // raw 2352-byte sectors (mode 2 form 1)
    const Bytes raw = t::raw_sectors(cooked);
    CB_CHECK(dc::is_disc_start(raw));
    const dc::Disc from_raw = dc::parse(raw);
    CB_CHECK(from_raw.container == dc::Container::Raw);
    CB_CHECK(from_raw.program == program);
    // a .cdi: other tracks first, then the boot track, then the descriptor: found by its content
    Bytes cdi = pseudo_random(5 * 2352 + 100);
    cdi.insert(cdi.end(), raw.begin(), raw.end());
    const Bytes trailer = pseudo_random(300);
    cdi.insert(cdi.end(), trailer.begin(), trailer.end());
    const dc::Disc from_cdi = dc::parse(cdi);
    CB_CHECK(from_cdi.container == dc::Container::Cdi);
    CB_CHECK(from_cdi.program == program);
    CB_CHECK_EQ(from_cdi.header.title, std::string("DREAMCAST TEST DISC"));
    // the same boot track stored as 2048-byte sectors inside a .cdi
    Bytes cdi_cooked = pseudo_random(7777);
    cdi_cooked.insert(cdi_cooked.end(), cooked.begin(), cooked.end());
    CB_CHECK(dc::parse(cdi_cooked).program == program);

    cb::fs::ScopedTempDir dir("cb_dc_formats");
    cb::fs::write_file(dir.path() / "game.cdi", cdi);
    CB_CHECK(dc::probe(dir.path() / "game.cdi"));
    CB_CHECK(dc::read(dir.path() / "game.cdi").container == dc::Container::Cdi);
    cb::fs::write_file(dir.path() / "game.iso", cooked);
    CB_CHECK(dc::probe(dir.path() / "game.iso"));
    CB_CHECK(dc::read(dir.path() / "game.iso").program == program);
    cb::fs::write_file(dir.path() / "game.bin", raw);
    CB_CHECK(dc::probe(dir.path() / "game.bin"));

    // a GD-ROM: track 3 is the high-density area's data, its volume numbered from sector 45000
    cb::fs::write_file(dir.path() / "track01.bin", pseudo_random(2352 * 4));
    cb::fs::write_file(dir.path() / "track02.raw", pseudo_random(2352 * 4));
    cb::fs::write_file(dir.path() / "track03.bin", t::raw_sectors(t::iso(program, false, 45000)));
    cb::fs::write_text(dir.path() / "disc.gdi", "3\n1 0 4 2352 track01.bin 0\n2 756 0 2352 track02.raw 0\n3 45000 4 2352 \"track03.bin\" 0\n");
    CB_CHECK(dc::probe(dir.path() / "disc.gdi"));
    const dc::Disc from_gdi = dc::read(dir.path() / "disc.gdi");
    CB_CHECK(from_gdi.container == dc::Container::Gdi);
    CB_CHECK_EQ(from_gdi.tracks, std::size_t{3});
    CB_CHECK(from_gdi.program == program);
    CB_CHECK_EQ(from_gdi.first_read_name, std::string("1ST_READ.BIN"));
    // a multi-session iso: the same numbering inside one file
    CB_CHECK(dc::parse(t::iso(program, false, 11702)).program == program);
});

CB_TEST("formats.dreamcast.what_is_not_a_disc_is_refused", [] {
    CB_CHECK(!dc::is_disc_start(Bytes(64, 0)));
    CB_CHECK_THROWS_AS(cb::FormatError, dc::parse(Bytes(100000, 0)));
    // a Saturn disc's ID is not a Dreamcast's
    Bytes saturn(0x9000, 0);
    const std::string id = "SEGA SEGASATURN ";
    std::copy(id.begin(), id.end(), saturn.begin());
    CB_CHECK(!dc::is_disc_start(saturn));
    CB_CHECK_THROWS_AS(cb::FormatError, dc::parse(saturn));
    // the ID without a volume behind it is not enough
    Bytes bare = t::ip_bin();
    bare.resize(0x9000, 0);
    CB_CHECK_THROWS_AS(cb::FormatError, dc::parse(bare));
    cb::fs::ScopedTempDir dir("cb_dc_formats");
    cb::fs::write_file(dir.path() / "no.gdi", Bytes{'x', 'y'});
    CB_CHECK(!dc::probe(dir.path() / "no.gdi"));
    CB_CHECK(!dc::probe(dir.path() / "missing.iso"));
});
