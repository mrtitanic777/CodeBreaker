// Tests of the Saturn disc reader over a small disc made in the test (source/sh2_test_programs.hpp): IP.BIN's header, the root directory and the
// first program, the memory they make, the function starts, raw and cooked images and a .cue sheet, and the probe.
#include "cb/formats/saturn.hpp"

#include <algorithm>
#include <array>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/source/isa_sh2.hpp"
#include "cb/source/sh2_test_programs.hpp"

namespace cb::formats::saturn {
namespace {

namespace t = source::sh2_test;

// The same disc as raw 2352-byte sectors: a sync pattern, the sector's address, mode 1, the data, and zeros for the error codes.
Bytes raw_sectors(const Bytes& cooked) {
    Bytes out;
    for (std::size_t s = 0; s * kSector < cooked.size(); ++s) {
        const std::array<std::uint8_t, 16> header = {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 2, static_cast<std::uint8_t>(s), 1};
        out.insert(out.end(), header.begin(), header.end());
        out.insert(out.end(), cooked.begin() + static_cast<std::ptrdiff_t>(s * kSector), cooked.begin() + static_cast<std::ptrdiff_t>((s + 1) * kSector));
        out.insert(out.end(), kRawSector - 16 - kSector, 0);
    }
    return out;
}

CB_TEST("formats.saturn.the_header_and_the_first_program", [] {
    const Bytes cooked = t::disc_saturn();
    const Disc disc = parse(cooked);
    CB_CHECK_EQ(disc.header.hardware_id, "SEGA SEGASATURN");
    CB_CHECK_EQ(disc.header.maker, "SEGA TP T-0001");
    CB_CHECK_EQ(disc.header.product, "T-0001");
    CB_CHECK_EQ(disc.header.version, "V1.000");
    CB_CHECK_EQ(disc.header.release_date, "20261115");
    CB_CHECK_EQ(disc.header.device, "CD-1/1");
    CB_CHECK_EQ(disc.header.areas, "JTUE");
    CB_CHECK_EQ(disc.header.title, "SH-2 TEST DISC");
    CB_CHECK_EQ(disc.header.ip_size, 0x1000u);
    CB_CHECK_EQ(disc.header.master_stack, 0x06004000u);
    CB_CHECK_EQ(disc.header.first_read_address, 0x06004000u);
    CB_CHECK_EQ(disc.ip_length(), 0x1000u);
    CB_CHECK_EQ(disc.load_address(), 0x06004000u);
    CB_CHECK_EQ(disc.system.size(), kSystemBytes);
    // the first program is the root's first file that is not a licence text
    CB_CHECK_EQ(disc.first_read_name, "0.BIN");
    const Bytes program = t::assemble(t::kSaturnProgram);
    CB_CHECK(disc.first_read == program);
    // memory: IP.BIN at $06002000, the program at its load address
    const Bytes mem = disc.memory();
    CB_CHECK_EQ(mem.size(), kHighRamSize);
    CB_CHECK(std::equal(disc.system.begin(), disc.system.begin() + 0x1000, mem.begin() + 0x2000));
    CB_CHECK(std::equal(program.begin(), program.end(), mem.begin() + 0x4000));
});

CB_TEST("formats.saturn.function_starts", [] {
    const Disc disc = parse(t::disc_saturn());
    const std::vector<std::uint32_t> starts = disc.functions(source::isa::sh2_decoder());
    // the four area-code entries, the initial program they run into, the first program's start and its BSR target
    const std::vector<std::uint32_t> expected = {0x06002E00, 0x06002E20, 0x06002E40, 0x06002E60, 0x06002E80, 0x06004000};
    CB_CHECK(starts.size() == expected.size() + 1);
    for (const std::uint32_t a : expected) CB_CHECK(std::find(starts.begin(), starts.end(), a) != starts.end());
});

CB_TEST("formats.saturn.raw_and_cooked_images_and_cue_sheets", [] {
    fs::ScopedTempDir dir("saturn-probe");
    const Bytes cooked = t::disc_saturn();
    const Bytes raw = raw_sectors(cooked);
    fs::write_file(dir.path() / "game.iso", cooked);
    fs::write_file(dir.path() / "game.bin", raw);
    fs::write_text(dir.path() / "game.cue", "FILE \"game.bin\" BINARY\n  TRACK 01 MODE1/2352\n    INDEX 01 00:00:00\n");
    fs::write_text(dir.path() / "cooked.cue", "FILE \"game.iso\" BINARY\r\n  TRACK 01 MODE1/2048\r\n    INDEX 01 00:00:00\r\n");
    fs::write_text(dir.path() / "none.cue", "TRACK 01 MODE1/2352\n");
    fs::write_file(dir.path() / "other.bin", Bytes(0x9000, 0));
    for (const char* name : {"game.iso", "game.bin", "game.cue", "cooked.cue"}) CB_CHECK(probe(dir.path() / name));
    for (const char* name : {"other.bin", "none.cue", "missing.iso"}) CB_CHECK(!probe(dir.path() / name));
    const Disc a = read(dir.path() / "game.iso");
    const Disc b = read(dir.path() / "game.bin");
    const Disc c = read(dir.path() / "game.cue");
    CB_CHECK(a.system == b.system && b.system == c.system);
    CB_CHECK(a.first_read == b.first_read && b.first_read == c.first_read);
    CB_CHECK_EQ(c.first_read_name, "0.BIN");
    CB_CHECK_THROWS_AS(FormatError, read(dir.path() / "other.bin"));
    CB_CHECK_THROWS_AS(FormatError, read(dir.path() / "none.cue"));
    // a Saturn disc is not a Genesis cartridge, and not a Sega CD disc
    CB_CHECK(!genesis::probe(dir.path() / "game.iso"));
});

CB_TEST("formats.saturn.a_disc_without_a_volume_has_no_first_program", [] {
    Bytes cooked = t::disc_saturn();
    cooked[16 * kSector + 1] = 'X';   // the volume descriptor's identifier is damaged
    const Disc disc = parse(cooked);
    CB_CHECK(disc.first_read_name.empty());
    CB_CHECK(disc.first_read.empty());
    CB_CHECK_EQ(disc.header.title, "SH-2 TEST DISC");
    // the system area alone is still read: its functions are IP.BIN's
    CB_CHECK(!disc.functions(source::isa::sh2_decoder()).empty());
});

}  // namespace
}  // namespace cb::formats::saturn
