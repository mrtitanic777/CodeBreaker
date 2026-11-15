#include "cb/formats/segacd.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/formats/segacd_test_image.hpp"
#include "cb/source/isa.hpp"

namespace cb::formats::segacd {
namespace {

namespace img = segacd_test_image;

CB_TEST("formats.segacd.the_boot_header_names_the_two_programs", [] {
    const Disc disc = parse(img::system_area());
    CB_CHECK_EQ(disc.sector_size, 2048u);
    CB_CHECK_EQ(disc.boot.size(), 0x8000u);
    CB_CHECK_EQ(disc.header.disc_id, "SEGADISCSYSTEM");
    CB_CHECK_EQ(disc.header.volume, "TESTCDGAME");
    CB_CHECK_EQ(disc.header.system_type, "SEGA MEGA DRIVE");
    CB_CHECK_EQ(disc.header.domestic_name, "TEST DISC");
    CB_CHECK_EQ(disc.header.serial, "GM 00000000-00");
    CB_CHECK_EQ(disc.header.region, "E");
    CB_CHECK_EQ(disc.header.ip.start, 0x800u);
    CB_CHECK_EQ(disc.header.ip.size, 0x200u);
    CB_CHECK_EQ(disc.header.sp.start, 0x1000u);
    CB_CHECK_EQ(disc.header.sp.size, 0x400u);
    // the main program is the security code and the IP: from $200 to the IP's end
    CB_CHECK_EQ(disc.main_begin, 0x200u);
    CB_CHECK_EQ(disc.main_end, 0xA00u);
    CB_CHECK_EQ(disc.main_address(0x800), 0xFF0600u);
    CB_CHECK(disc.has_sub());
    CB_CHECK_EQ(disc.sub_begin, 0x1000u);
    CB_CHECK_EQ(disc.sub_end, 0x1400u);
    CB_CHECK_EQ(disc.sub_address(0x1028), 0x6028u);
    CB_CHECK(Disc::is_sub(0x6028));
    CB_CHECK(!Disc::is_sub(0xFF0600));
});

CB_TEST("formats.segacd.a_raw_image_gives_the_same_system_area", [] {
    const Bytes cooked = img::system_area();
    const Disc raw = parse(img::raw(cooked));
    CB_CHECK_EQ(raw.sector_size, 2352u);
    CB_CHECK(raw.boot == cooked);
    CB_CHECK_EQ(raw.header.ip.start, 0x800u);
    CB_CHECK_EQ(raw.sub_end, 0x1400u);
});

CB_TEST("formats.segacd.memory_has_each_program_where_it_runs", [] {
    const Disc disc = parse(img::system_area());
    const Bytes main = disc.main_memory();
    CB_CHECK_EQ(main.size(), 0x1000000u);
    CB_CHECK_EQ(main[0xFF0000], 0x60);   // the security code
    CB_CHECK_EQ(main[0xFF0600], 0x13);   // the IP
    CB_CHECK_EQ(main[0x6028], 0);        // not the sub CPU's
    const Bytes sub = disc.sub_memory();
    CB_CHECK_EQ(sub[0x6028], 0x70);
    CB_CHECK_EQ(sub[0x6000], 'M');
    CB_CHECK_EQ(sub[0xFF0000], 0);
});

CB_TEST("formats.segacd.the_sp_jump_table_gives_its_entries", [] {
    const Disc disc = parse(img::system_area());
    const std::array<std::uint32_t, 4> entries = disc.sub_entries();
    CB_CHECK((entries == std::array<std::uint32_t, 4>{0x6028, 0x6032, 0x6034, 0}));   // offsets from the table's start ($6020); the user call is none
    // a table that is outside the SP gives none
    Bytes broken = img::system_area();
    img::put_long(broken, 0x1018, 0x7000);
    const std::array<std::uint32_t, 4> none = parse(broken).sub_entries();
    CB_CHECK((none == std::array<std::uint32_t, 4>{}));
});

CB_TEST("formats.segacd.functions_of_both_programs", [] {
    const Disc disc = parse(img::system_area());
    const std::vector<std::uint32_t> got = disc.functions(source::isa::m68k_decoder());
    // sub CPU: init, main, int2. main CPU: the security code, the IP's entry, the IP's BSR target.
    CB_CHECK((got == std::vector<std::uint32_t>{0x6028, 0x6032, 0x6034, 0xFF0000, 0xFF0600, 0xFF0610}));
});

CB_TEST("formats.segacd.a_disc_without_an_sp_or_with_an_ip_that_is_outside_the_area_still_reads", [] {
    Bytes no_sp = img::system_area();
    img::put_long(no_sp, 0x44, 0);
    const Disc a = parse(no_sp);
    CB_CHECK(!a.has_sub());
    CB_CHECK(a.sub_entries() == std::array<std::uint32_t, 4>{});
    CB_CHECK(a.functions(source::isa::m68k_decoder()).size() == 3u);
    Bytes wild = img::system_area();
    img::put_long(wild, 0x30, 0xFFFFFF00);   // an IP that lies nowhere
    const Disc b = parse(wild);
    CB_CHECK_EQ(b.main_end, 0x800u);         // the main program is the first part of the area
    Bytes overlapping = img::system_area();
    img::put_long(overlapping, 0x40, 0x400);   // an SP that starts inside the main program
    CB_CHECK(!parse(overlapping).has_sub());
});

CB_TEST("formats.segacd.what_is_not_a_disc_is_refused", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x8000, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}));
    Bytes short_disc = img::system_area();
    short_disc.resize(0x100);   // the ID, but not the boot header
    CB_CHECK_THROWS_AS(FormatError, (void)parse(short_disc));
});

CB_TEST("formats.segacd.probe_tells_a_disc_from_a_cartridge", [] {
    fs::ScopedTempDir dir("cb_segacd");
    const Bytes cooked = img::system_area();
    fs::write_file(dir.path() / "game.iso", cooked);
    fs::write_file(dir.path() / "game.bin", img::raw(cooked));
    fs::write_file(dir.path() / "cart.bin", Bytes(0x400, 0));
    CB_CHECK(probe(dir.path() / "game.iso"));
    CB_CHECK(probe(dir.path() / "game.bin"));
    CB_CHECK(!probe(dir.path() / "cart.bin"));
    CB_CHECK(!probe(dir.path() / "missing.iso"));
    CB_CHECK(read(dir.path() / "game.bin").boot == cooked);
    // the disc's header at $100 reads "SEGA", but the Genesis does not take the disc for a cartridge
    CB_CHECK(!genesis::probe(dir.path() / "game.iso"));
    CB_CHECK(!genesis::probe(dir.path() / "game.bin"));
});

CB_TEST("formats.segacd.identify_names_the_disc_in_either_sector_size", [] {
    const Bytes cooked = img::system_area();
    for (const Bytes& image : {cooked, img::raw(cooked)}) {
        const auto found = detect::identify(image, "game.bin");
        CB_CHECK_EQ(found.size(), 1u);
        CB_CHECK_EQ(found[0].format, "Sega CD disc");
        CB_CHECK_EQ(found[0].family, "segacd");
        CB_CHECK_EQ(found[0].command, "reach");
        CB_CHECK(found[0].detail.find("IP 512 bytes at 0x800, SP 1024 bytes at 0x1000") != std::string::npos);
    }
    CB_CHECK_EQ(detect::identify(cooked, "game.iso")[0].detail.rfind("\"TEST DISC\", 2048-byte sectors", 0), 0u);
});

}  // namespace
}  // namespace cb::formats::segacd
