// Super Nintendo cartridge images: the internal header, LoROM and HiROM maps, the copier header, the probe and the chips' register names.
#include "cb/formats/snes.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/snes_test_image.hpp"

namespace snes = cb::formats::snes;
namespace img = cb::formats::snes_test_image;
using cb::Bytes;

CB_TEST("formats.snes.a_lorom_cartridge_is_found_by_its_internal_header", [] {
    const snes::Rom rom = snes::parse(img::cartridge());
    CB_CHECK(rom.mapping == snes::Mapping::LoRom);
    CB_CHECK_EQ(rom.header_offset, 0x7FC0u);
    CB_CHECK_EQ(rom.title, "CODEBREAKER TEST");
    CB_CHECK_EQ(rom.map_mode, 0x20);
    CB_CHECK_EQ(rom.rom_size_code, 5);
    CB_CHECK_EQ(rom.developer, 0x33);
    CB_CHECK(rom.checksum_ok);
    CB_CHECK(rom.copier_header.empty());
    CB_CHECK_EQ(rom.emulation.reset, 0x8000);
    CB_CHECK_EQ(rom.native.nmi, 0x8060);
    CB_CHECK_EQ(rom.native.irq, 0x8061);
    CB_CHECK_EQ(rom.native.cop, 0x8062);
    CB_CHECK_EQ(rom.native.brk, 0x8063);
    CB_CHECK_EQ(rom.bank_count(), 1u);
    CB_CHECK(!rom.fast());
});

CB_TEST("formats.snes.lorom_maps_the_upper_half_of_each_bank_and_mirrors_at_80", [] {
    const snes::Rom rom = snes::parse(img::cartridge());
    CB_CHECK_EQ(rom.offset(0x008000).value(), 0u);
    CB_CHECK_EQ(rom.offset(0x808000).value(), 0u);        // the $80 mirror
    CB_CHECK_EQ(rom.offset(0x009000).value(), 0x1000u);
    CB_CHECK_EQ(rom.offset(0x01801A).value(), 0x1Au);     // 32 KB repeats
    CB_CHECK(!rom.offset(0x007000).has_value());           // the lower half of a bank is not ROM
    CB_CHECK(!rom.offset(0x7E8000).has_value());           // the console's RAM
    CB_CHECK_EQ(rom.address_of(0x1000, false), 0x009000u);
    CB_CHECK_EQ(rom.address_of(0x1000, true), 0x809000u);
});

CB_TEST("formats.snes.a_hirom_cartridge_maps_whole_banks", [] {
    const snes::Rom rom = snes::parse(img::hirom());
    CB_CHECK(rom.mapping == snes::Mapping::HiRom);
    CB_CHECK_EQ(rom.header_offset, 0xFFC0u);
    CB_CHECK_EQ(rom.emulation.reset, 0x8000);
    CB_CHECK_EQ(rom.bank_count(), 1u);
    CB_CHECK_EQ(rom.offset(0xC00000).value(), 0u);
    CB_CHECK_EQ(rom.offset(0xC08020).value(), 0x8020u);
    CB_CHECK_EQ(rom.offset(0x008020).value(), 0x8020u);   // the upper half of bank 0 is the same bytes
    CB_CHECK_EQ(rom.offset(0x400000).value(), 0u);        // $40 is the full-bank mirror
    CB_CHECK(!rom.offset(0x000000).has_value());          // below $8000 in banks $00-$3F is not ROM
    CB_CHECK_EQ(rom.address_of(0x8020, false), 0xC08020u);
    CB_CHECK_EQ(rom.address_of(0x12345, true), 0xC12345u);
});

CB_TEST("formats.snes.a_copier_header_is_kept", [] {
    const Bytes file = img::cartridge(true);
    const snes::Rom rom = snes::parse(file);
    CB_CHECK_EQ(rom.copier_header.size(), 512u);
    CB_CHECK_EQ(rom.copier_header[0], 0x40);
    CB_CHECK_EQ(rom.data.size(), 0x8000u);
    CB_CHECK_EQ(rom.emulation.reset, 0x8000);
});

CB_TEST("formats.snes.what_is_not_a_cartridge_is_refused", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, snes::parse(Bytes(100, 0)));
    CB_CHECK_THROWS_AS(cb::FormatError, snes::parse(Bytes(0x8000, 0xFF)));    // a size, and no internal header
    CB_CHECK_THROWS_AS(cb::FormatError, snes::parse(Bytes(0x10000, 0x00)));
});

CB_TEST("formats.snes.a_file_is_probed_by_its_extension_and_header", [] {
    cb::fs::ScopedTempDir dir("cb_snes_probe");
    cb::fs::write_file(dir.path() / "a.sfc", img::cartridge());
    cb::fs::write_file(dir.path() / "b.SMC", img::cartridge(true));
    cb::fs::write_file(dir.path() / "c.bin", img::cartridge());
    cb::fs::write_file(dir.path() / "d.sfc", Bytes(0x8000, 0xFF));
    cb::fs::write_file(dir.path() / "e.sfc", img::hirom());
    cb::fs::write_file(dir.path() / "f.sfc", Bytes(100, 1));
    CB_CHECK(snes::probe(dir.path() / "a.sfc"));
    CB_CHECK(snes::probe(dir.path() / "b.SMC"));
    CB_CHECK(!snes::probe(dir.path() / "c.bin"));
    CB_CHECK(!snes::probe(dir.path() / "d.sfc"));
    CB_CHECK(snes::probe(dir.path() / "e.sfc"));
    CB_CHECK(!snes::probe(dir.path() / "f.sfc"));
    CB_CHECK(!snes::probe(dir.path() / "missing.sfc"));
    CB_CHECK(snes::read(dir.path() / "b.SMC").copier_header.size() == 512u);
});

CB_TEST("formats.snes.the_chips_registers_have_their_hardware_names", [] {
    CB_CHECK_EQ(snes::register_name(0x2100), "INIDISP");
    CB_CHECK_EQ(snes::register_name(0x2118), "VMDATAL");
    CB_CHECK_EQ(snes::register_name(0x2119), "VMDATAH");
    CB_CHECK_EQ(snes::register_name(0x213F), "STAT78");
    CB_CHECK_EQ(snes::register_name(0x2140), "APUIO0");
    CB_CHECK_EQ(snes::register_name(0x2143), "APUIO3");
    CB_CHECK_EQ(snes::register_name(0x2180), "WMDATA");
    CB_CHECK_EQ(snes::register_name(0x4016), "JOYSER0");
    CB_CHECK_EQ(snes::register_name(0x4200), "NMITIMEN");
    CB_CHECK_EQ(snes::register_name(0x420B), "MDMAEN");
    CB_CHECK_EQ(snes::register_name(0x420C), "HDMAEN");
    CB_CHECK_EQ(snes::register_name(0x4210), "RDNMI");
    CB_CHECK_EQ(snes::register_name(0x4218), "JOY1L");
    CB_CHECK_EQ(snes::register_name(0x4300), "DMAP0");
    CB_CHECK_EQ(snes::register_name(0x4301), "BBAD0");
    CB_CHECK_EQ(snes::register_name(0x4372), "A1T7L");
    CB_CHECK_EQ(snes::register_name(0x437A), "NTRL7");
    CB_CHECK_EQ(snes::register_name(0x802100), "INIDISP");   // the bank is ignored
    CB_CHECK_EQ(snes::register_name(0x1234), "");
    CB_CHECK_EQ(snes::register_name(0x420F), "");
});
