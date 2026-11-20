// cb-unity: alone
// `info` on the Atari 5200, Jaguar, WonderSwan and 3DO files: what formats::detect says of small files of each.
#include "core/testing.hpp"
#include "formats/vb_test_image.hpp"
#include "formats/intv_test_image.hpp"
#include "formats/detect.hpp"

namespace cb::formats::detect {
namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v, bool big) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (big ? 24 - 8 * i : 8 * i));
}

Detected first(const Bytes& file, const char* name) {
    const std::vector<Detected> found = identify(ByteView(file), name);
    return found.empty() ? Detected{} : found.front();
}

}  // namespace

CB_TEST("formats.detect.atari_5200_cartridge", [] {
    Bytes rom(0x4000, 0xEA);
    rom[0x3FFE] = 0x00;
    rom[0x3FFF] = 0x80;
    CB_CHECK_EQ(first(rom, "game.a52").family, "atari5200");
    CB_CHECK_EQ(first(rom, "game.bin").family, "atari5200");
    CB_CHECK(first(Bytes(0x4000, 0xFF), "game.bin").family != "atari5200");   // erased ROM at the start address: not a cartridge
});

CB_TEST("formats.detect.atari_jaguar_cartridge", [] {
    Bytes rom(0x4000, 0xFF);
    put32(rom, 0x400, 0x04040404, true);
    put32(rom, 0x404, 0x802000, true);
    const Detected d = first(rom, "game.j64");
    CB_CHECK_EQ(d.family, "jaguar");
    CB_CHECK_EQ(d.format, "Atari Jaguar cartridge");
    CB_CHECK_EQ(d.command, "reach");
});

CB_TEST("formats.detect.wonderswan_cartridge", [] {
    Bytes rom(0x10000, 0xFF);
    const Bytes footer = {0xEA, 0x00, 0x00, 0x00, 0xF0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
    std::copy(footer.begin(), footer.end(), rom.end() - 16);
    const Detected d = first(rom, "game.wsc");
    CB_CHECK_EQ(d.family, "wonderswan");
    CB_CHECK_EQ(d.format, "WonderSwan Color cartridge");
});

CB_TEST("formats.detect.threedo_aif", [] {
    Bytes aif(0x50, 0);
    put32(aif, 0, 0xE1A00000, true);
    put32(aif, 12, 0xEB00000B, true);
    put32(aif, 16, 0xEF000011, true);
    put32(aif, 20, 0x50, true);
    const Detected d = first(aif, "LaunchMe.aif");
    CB_CHECK_EQ(d.family, "threedo");
    CB_CHECK_EQ(d.format, "3DO AIF executable");
});

CB_TEST("formats.detect.cdi_disc", [] {
    Bytes disc(2048 * 3, 0);
    disc[0] = 1;
    const char label[] = "CD-I ";
    std::copy(label, label + 5, disc.begin() + 1);
    disc[6] = 1;
    const Bytes module = {0x4A, 0xFC, 0, 1, 0, 0, 0, 0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xB5, 0x32};   // the header: the sync and the size, the parity word last
    std::copy(module.begin(), module.end(), disc.begin() + 4096);
    const Detected d = first(disc, "game.iso");
    CB_CHECK_EQ(d.family, "cdi");
    CB_CHECK_EQ(d.format, "Philips CD-i disc");
});

CB_TEST("formats.detect.vectrex_cartridge", [] {
    Bytes rom(0x40, 0xFF);
    const Bytes header = {'g', ' ', 'G', 'C', 'E', ' ', '1', '9', '8', '2', 0x80, 0xFD, 0x0D, 0xF8, 0x50, 0x20, 0xD0, 'T', 'E', 'S', 'T', 0x80, 0x00};
    std::copy(header.begin(), header.end(), rom.begin());
    const Detected d = first(rom, "game.vec");
    CB_CHECK_EQ(d.family, "vectrex");
    CB_CHECK_EQ(d.format, "Vectrex cartridge");
});

CB_TEST("formats.detect.virtual_boy_cartridge", [] {
    const Detected d = first(vb::test_image::cartridge(), "game.vb");
    CB_CHECK_EQ(d.family, "vb");
    CB_CHECK_EQ(d.format, "Virtual Boy cartridge");
    CB_CHECK_EQ(first(vb::test_image::cartridge(), "game.bin").family == "vb", false);
});

CB_TEST("formats.detect.intellivision_cartridge", [] {
    const Detected d = first(intv::test_image::file(), "game.rom");
    CB_CHECK_EQ(d.family, "intv");
    CB_CHECK_EQ(d.format, "Intellivision cartridge (Intellicart .rom)");
    CB_CHECK_EQ(first(intv::test_image::file(), "game.bin").family == "intv", false);
});

}  // namespace cb::formats::detect
