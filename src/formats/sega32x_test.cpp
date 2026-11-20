// Tests of the 32X cartridge reader over a small cartridge made in the test (source/sh2_test_programs.hpp): the header, the SH-2 program and
// where it lies, the function starts, the probe (and that the Genesis probe leaves a 32X cartridge to it).
#include "formats/sega32x.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/genesis.hpp"
#include "source/isa_sh2.hpp"
#include "source/sh2_test_programs.hpp"

namespace cb::formats::sega32x {
namespace {

namespace t = source::sh2_test;

CB_TEST("formats.sega32x.the_header_names_the_sh2_program", [] {
    const Bytes image = t::cartridge_32x();
    const Rom rom = parse(image);
    CB_CHECK_EQ(rom.header.system, "SEGA 32X");
    CB_CHECK_EQ(rom.header.domestic_name, "SH-2 TEST CARTRIDGE");
    CB_CHECK_EQ(rom.header.serial, "GM 00000000-00");
    CB_CHECK_EQ(rom.header.region, "JUE");
    CB_CHECK_EQ(rom.header.module_name, "TESTROM32X");
    CB_CHECK_EQ(rom.header.sh2_offset, 0x1000u);
    CB_CHECK_EQ(rom.header.master_start, 0x06000040u);
    CB_CHECK_EQ(rom.header.slave_start, 0x06000040u);
    CB_CHECK_EQ(rom.header.master_vbr, 0x06000000u);
    CB_CHECK(rom.has_sh2_program());
    const Bytes program = t::assemble(t::kSdramProgram);
    CB_CHECK_EQ(rom.header.sh2_size, static_cast<std::uint32_t>(program.size()));
    CB_CHECK(rom.sh2_program().size() == program.size());
    CB_CHECK(std::equal(program.begin(), program.end(), rom.sh2_program().begin()));
    // where the program lies: SDRAM, in any cache view
    CB_CHECK(rom.in_program(0x06000040));
    CB_CHECK(rom.in_program(0x26000040));
    CB_CHECK(!rom.in_program(0x06000000 + static_cast<std::uint32_t>(program.size())));
    CB_CHECK(!rom.in_program(0x02000040));
    const Bytes sdram = rom.sdram();
    CB_CHECK_EQ(sdram.size(), kSdramSize);
    CB_CHECK(std::equal(program.begin(), program.end(), sdram.begin()));
    CB_CHECK_THROWS_AS(FormatError, parse(Bytes(0x100, 0)));
});

CB_TEST("formats.sega32x.function_starts_follow_the_code_and_the_vectors", [] {
    const Rom rom = parse(t::cartridge_32x());
    const Decoder& decode = source::isa::sh2_decoder();
    // the vectors are read from the vector base on, 256 longwords: the entry (twice), not the stack pointers; the longwords after the table that hold
    // addresses of code count too (the table is followed at once by the constants and pointers of the program)
    const std::vector<std::uint32_t> vectors = rom.vector_targets(decode);
    CB_CHECK(std::find(vectors.begin(), vectors.end(), 0x06000040u) != vectors.end());
    CB_CHECK(std::find(vectors.begin(), vectors.end(), 0x0603FFFCu) == vectors.end());
    const std::vector<std::uint32_t> starts = rom.functions(decode);
    // the entry, `tally` (a BSR target) and `helper` (a pointer in the program's table)
    CB_CHECK(starts.size() == 3);
    CB_CHECK_EQ(starts.front(), 0x06000040u);
    CB_CHECK(starts.back() > 0x06000040u);
});

CB_TEST("formats.sega32x.a_program_that_does_not_fit_has_none", [] {
    Bytes image = t::cartridge_32x();
    image[0x3DC] = 0x7F;   // a length far past the ROM and SDRAM
    const Rom rom = parse(image);
    CB_CHECK(!rom.has_sh2_program());
    CB_CHECK(rom.functions(source::isa::sh2_decoder()).empty());
});

CB_TEST("formats.sega32x.probe_tells_a_32x_cartridge_from_a_genesis_one", [] {
    fs::ScopedTempDir dir("sega32x-probe");
    const Bytes image = t::cartridge_32x();
    fs::write_file(dir.path() / "game.32x", image);
    fs::write_file(dir.path() / "game.bin", image);   // (the header says "SEGA 32X": the extension is not needed)
    Bytes ssf = image;                                  // a ROM that says SEGA SSF is a 32X cartridge by its extension and its start addresses
    for (std::size_t i = 0; i < 16; ++i) ssf[0x100 + i] = i < 8 ? "SEGA SSF"[i] : ' ';
    fs::write_file(dir.path() / "ssf.32x", ssf);
    fs::write_file(dir.path() / "ssf.bin", ssf);
    Bytes genesis(0x4000, 0);
    for (std::size_t i = 0; i < 4; ++i) genesis[0x100 + i] = "SEGA"[i];
    fs::write_file(dir.path() / "genesis.md", genesis);
    CB_CHECK(probe(dir.path() / "game.32x"));
    CB_CHECK(probe(dir.path() / "game.bin"));
    CB_CHECK(probe(dir.path() / "ssf.32x"));
    CB_CHECK(!probe(dir.path() / "ssf.bin"));
    CB_CHECK(!probe(dir.path() / "genesis.md"));
    CB_CHECK(!probe(dir.path() / "missing.32x"));
    CB_CHECK_EQ(read(dir.path() / "game.32x").header.master_start, 0x06000040u);
    // the Genesis reader leaves a 32X cartridge to this one
    CB_CHECK(!genesis::probe(dir.path() / "game.32x"));
    CB_CHECK(!genesis::probe(dir.path() / "ssf.32x"));
    CB_CHECK(genesis::probe(dir.path() / "genesis.md"));
});

}  // namespace
}  // namespace cb::formats::sega32x
