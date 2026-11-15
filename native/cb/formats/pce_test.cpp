// PC Engine HuCard images: the header, the banks, the vectors, the probe and the chips' register names.
#include "cb/formats/pce.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/pce_test_image.hpp"

namespace pce = cb::formats::pce;
using cb::Bytes;

CB_TEST("formats.pce.a_card_is_its_banks_and_its_vectors", [] {
    const pce::Rom rom = pce::parse(cb::formats::pce_test_image::cartridge());
    CB_CHECK(rom.header.empty());
    CB_CHECK_EQ(rom.bank_count(), 2u);
    CB_CHECK_EQ(rom.vectors.reset, 0xE000);
    CB_CHECK_EQ(rom.vectors.nmi, 0xE040);
    CB_CHECK_EQ(rom.vectors.irq1, 0xE041);
    CB_CHECK_EQ(rom.vectors.irq2, 0xE042);
    CB_CHECK_EQ(rom.vectors.timer, 0xE042);
    CB_CHECK_EQ(rom.at(0, 0).value(), 0x78);
    CB_CHECK_EQ(rom.at(1, 0).value(), 0xA9);
    CB_CHECK(!rom.at(0, 0x2000).has_value());
    CB_CHECK_EQ(rom.rom_bank(0).value(), 0u);
    CB_CHECK_EQ(rom.rom_bank(3).value(), 1u);   // two banks repeat
    CB_CHECK(!rom.rom_bank(0x80).has_value());
});

CB_TEST("formats.pce.a_copier_header_is_kept", [] {
    const Bytes file = cb::formats::pce_test_image::cartridge(true);
    const pce::Rom rom = pce::parse(file);
    CB_CHECK_EQ(rom.header.size(), 512u);
    CB_CHECK_EQ(rom.header[0], 0x40);
    CB_CHECK_EQ(rom.bank_count(), 2u);
    CB_CHECK_EQ(rom.vectors.reset, 0xE000);
});

CB_TEST("formats.pce.a_size_that_is_not_banks_or_a_reset_outside_the_top_page_is_refused", [] {
    Bytes file = cb::formats::pce_test_image::cartridge();
    file.push_back(0);
    CB_CHECK_THROWS_AS(cb::FormatError, pce::parse(file));
    Bytes bad_reset = cb::formats::pce_test_image::cartridge();
    bad_reset[0x1FFF] = 0x40;   // RESET $4000
    CB_CHECK_THROWS_AS(cb::FormatError, pce::parse(bad_reset));
    CB_CHECK_THROWS_AS(cb::FormatError, pce::parse(Bytes{}));
});

CB_TEST("formats.pce.a_file_is_probed_by_its_extension_size_and_reset_vector", [] {
    cb::fs::ScopedTempDir dir("cb_pce_probe");
    cb::fs::write_file(dir.path() / "a.pce", cb::formats::pce_test_image::cartridge());
    cb::fs::write_file(dir.path() / "b.PCE", cb::formats::pce_test_image::cartridge(true));
    cb::fs::write_file(dir.path() / "c.tg16", cb::formats::pce_test_image::cartridge());
    cb::fs::write_file(dir.path() / "d.bin", cb::formats::pce_test_image::cartridge());
    Bytes odd = cb::formats::pce_test_image::cartridge();
    odd.resize(odd.size() - 1);
    cb::fs::write_file(dir.path() / "e.pce", odd);
    Bytes low_reset = cb::formats::pce_test_image::cartridge();
    low_reset[0x1FFF] = 0x10;
    cb::fs::write_file(dir.path() / "f.pce", low_reset);
    CB_CHECK(pce::probe(dir.path() / "a.pce"));
    CB_CHECK(pce::probe(dir.path() / "b.PCE"));
    CB_CHECK(pce::probe(dir.path() / "c.tg16"));
    CB_CHECK(!pce::probe(dir.path() / "d.bin"));
    CB_CHECK(!pce::probe(dir.path() / "e.pce"));
    CB_CHECK(!pce::probe(dir.path() / "f.pce"));
    CB_CHECK(!pce::probe(dir.path() / "missing.pce"));
    CB_CHECK_EQ(pce::read(dir.path() / "b.PCE").bank_count(), 2u);
});

CB_TEST("formats.pce.the_chips_registers_have_their_hardware_names", [] {
    CB_CHECK_EQ(pce::register_name(0x0000, false), "VDC_STATUS");
    CB_CHECK_EQ(pce::register_name(0x0000, true), "VDC_SELECT");
    CB_CHECK_EQ(pce::register_name(0x0002, true), "VDC_DATA_L");
    CB_CHECK_EQ(pce::register_name(0x0007, true), "VDC_DATA_H");   // the VDC repeats every four bytes
    CB_CHECK_EQ(pce::register_name(0x0400, true), "VCE_CR");
    CB_CHECK_EQ(pce::register_name(0x0405, true), "VCE_CTD_H");
    CB_CHECK_EQ(pce::register_name(0x0802, true), "PSG_FREQ_L");
    CB_CHECK_EQ(pce::register_name(0x0C00, false), "TIMER_COUNTER");
    CB_CHECK_EQ(pce::register_name(0x0C00, true), "TIMER_RELOAD");
    CB_CHECK_EQ(pce::register_name(0x1000, false), "IO_PORT");
    CB_CHECK_EQ(pce::register_name(0x1403, true), "IRQ_ACK");
    CB_CHECK_EQ(pce::register_name(0x1801, false), "IO_1801");
    CB_CHECK_EQ(pce::vdc_register_name(5), "CR");
    CB_CHECK_EQ(pce::vdc_register_name(0x13), "SATB");
    CB_CHECK_EQ(pce::vdc_register_name(0x1F), "R31");
});
