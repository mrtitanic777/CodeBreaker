#include "cb/formats/nds.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/source/isa.hpp"
#include "cb/source/nds_test_image.hpp"

namespace cb::formats::nds {
namespace {

CB_TEST("formats.nds.header_fields", [] {
    const Rom rom = parse(source::nds_test::test_rom());
    const Header& h = rom.header;
    CB_CHECK_EQ(h.title, "NDSTEST");
    CB_CHECK_EQ(h.game_code, "ATSE");
    CB_CHECK_EQ(h.maker_code, "01");
    CB_CHECK_EQ(h.version, std::uint8_t{1});
    CB_CHECK(h.logo_ok);
    CB_CHECK(h.header_crc_ok());
    CB_CHECK_EQ(h.arm9.rom_offset, 0x4000u);
    CB_CHECK_EQ(h.arm9.entry, 0x02000000u);
    CB_CHECK_EQ(h.arm9.ram_address, 0x02000000u);
    CB_CHECK_EQ(h.arm7.ram_address, 0x037F8000u);
    CB_CHECK(h.arm9.holds(0x02000010) && !h.arm9.holds(h.arm9.end()));
    CB_CHECK_EQ(rom.memory(Cpu::Arm9).size(), std::size_t{h.arm9.size});
    CB_CHECK(*rom.cpu_of(0x02000004) == Cpu::Arm9);
    CB_CHECK(*rom.cpu_of(0x037F8004) == Cpu::Arm7);
    CB_CHECK(!rom.cpu_of(0x08000000));
    // the CRC-16 of the console (MODBUS): "123456789" gives 0x4B37
    const std::string check = "123456789";
    CB_CHECK_EQ(crc16(ByteView(reinterpret_cast<const std::uint8_t*>(check.data()), check.size())), std::uint16_t{0x4B37});
    CB_CHECK_EQ(h.logo_crc, kLogoCrc);
});

CB_TEST("formats.nds.regions_in_file_order_with_the_name_of_each_file", [] {
    const Rom rom = parse(source::nds_test::test_rom());
    const std::vector<Region> regions = rom.regions();
    std::vector<Region::Kind> kinds;
    for (const Region& r : regions) kinds.push_back(r.kind);
    const std::vector<Region::Kind> expected = {Region::Kind::Header, Region::Kind::Arm9,   Region::Kind::Arm7, Region::Kind::Fnt,
                                                Region::Kind::Fat,    Region::Kind::Banner, Region::Kind::File};
    CB_CHECK(kinds == expected);
    CB_CHECK_EQ(regions.back().name, "hello.txt");
    CB_CHECK_EQ(regions.back().id, 0u);
    CB_CHECK_EQ(regions.back().size, 31u);
    for (std::size_t i = 1; i < regions.size(); ++i) CB_CHECK(regions[i - 1].end() <= regions[i].offset);
});

CB_TEST("formats.nds.functions_of_each_program_with_its_own_decoder", [] {
    const Rom rom = parse(source::nds_test::test_rom());
    const std::vector<std::uint32_t> nine = rom.functions(Cpu::Arm9, source::isa::arm9_decoder());
    const std::vector<std::uint32_t> seven = rom.functions(Cpu::Arm7, source::isa::arm_decoder());
    CB_CHECK(std::find(nine.begin(), nine.end(), 0x02000000u) != nine.end());
    CB_CHECK(nine.size() >= 3);   // the entry, `count`, the Thumb function a blx names
    CB_CHECK(std::find(seven.begin(), seven.end(), 0x037F8000u) != seven.end());
    CB_CHECK_EQ(seven.size(), std::size_t{2});
});

CB_TEST("formats.nds.damaged_files_are_refused", [] {
    const Bytes rom = source::nds_test::test_rom();
    CB_CHECK_THROWS_AS(FormatError, parse(ByteView(rom).subspan(0, 0x100)));
    Bytes no_crc = rom;
    no_crc[0x15C] = 0;
    CB_CHECK_THROWS_AS(FormatError, parse(no_crc));
    fs::ScopedTempDir dir("cb_nds_probe");
    fs::write_file(dir.path() / "a.nds", rom);
    fs::write_file(dir.path() / "b.nds", no_crc);
    CB_CHECK(probe(dir.path() / "a.nds"));
    CB_CHECK(!probe(dir.path() / "b.nds"));
    // with the FAT's size zero there are no files
    Bytes none = rom;
    none[0x4C] = 0;
    for (const Region& region : parse(none).regions()) CB_CHECK(region.kind != Region::Kind::File);
});

}  // namespace
}  // namespace cb::formats::nds
