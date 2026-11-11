#include "cb/formats/gb.hpp"

#include <cstring>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::gb {
namespace {

// A cartridge of `size` bytes with a valid header: the logo, `title`, the Color flag, SGB flag, cartridge type, and
// the header checksum the boot ROM computes.
Bytes image(std::size_t size = 0x8000, std::string_view title = "TESTGAME", std::uint8_t cgb = 0, std::uint8_t sgb = 0,
            std::uint8_t type = 0) {
    Bytes out(size, 0xFF);
    std::copy(kLogo.begin(), kLogo.end(), out.begin() + 0x104);
    std::fill(out.begin() + 0x134, out.begin() + 0x144, std::uint8_t{0});
    std::memcpy(out.data() + 0x134, title.data(), title.size());
    out[0x143] = cgb;
    out[0x146] = sgb;
    out[0x147] = type;
    std::uint8_t x = 0;
    for (std::size_t i = 0x134; i < 0x14D; ++i) x = static_cast<std::uint8_t>(x - out[i] - 1);
    out[0x14D] = x;
    return out;
}

CB_TEST("formats.gb.header", [] {
    const Rom rom = parse(image());
    CB_CHECK(rom.logo_ok);
    CB_CHECK_EQ(rom.title, "TESTGAME");
    CB_CHECK(rom.color == ColorSupport::None);
    CB_CHECK(!rom.sgb);
    CB_CHECK_EQ(rom.mbc(), "ROM");
    CB_CHECK(rom.header_checksum_ok);
    CB_CHECK_EQ(rom.bank_count(), 2u);
    CB_CHECK_EQ(rom.bank(1).size(), kBank);
});

CB_TEST("formats.gb.checksum_and_logo_are_checked", [] {
    Bytes bad = image();
    bad[0x14D] ^= 1;
    CB_CHECK(!parse(bad).header_checksum_ok);
    Bytes logo = image();
    logo[0x110] ^= 1;
    const Rom rom = parse(logo);
    CB_CHECK(!rom.logo_ok);
    CB_CHECK(rom.header_checksum_ok);   // the checksum covers $0134-$014C, not the logo
});

CB_TEST("formats.gb.color_flag_shortens_the_title", [] {
    // 15 characters of title, then the Color flag at $0143: a Color cartridge keeps only 11 characters.
    const std::string long_title = "ABCDEFGHIJKLMNO";
    CB_CHECK_EQ(parse(image(0x8000, long_title, 0x00)).title, long_title);
    CB_CHECK_EQ(parse(image(0x8000, long_title, 0x80)).title, "ABCDEFGHIJK");
    CB_CHECK(parse(image(0x8000, long_title, 0x80)).color == ColorSupport::Also);
    CB_CHECK(parse(image(0x8000, long_title, 0xC0)).color == ColorSupport::Only);
    CB_CHECK(parse(image(0x8000, long_title, 0x7F)).color == ColorSupport::None);
});

CB_TEST("formats.gb.title_ends_at_nul_and_is_trimmed", [] {
    Bytes img = image(0x8000, "POKE   ");
    CB_CHECK_EQ(parse(img).title, "POKE");
    img[0x137] = 0;   // "POK\0..." cuts earlier
    CB_CHECK_EQ(parse(img).title, "POK");
    // latin-1 bytes decode to UTF-8
    Bytes latin = image();
    latin[0x134] = 0xE9;   // e acute
    CB_CHECK_EQ(parse(latin).title.substr(0, 2), "\xC3\xA9");
});

CB_TEST("formats.gb.cartridge_types", [] {
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0x01)).mbc(), "MBC1");
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0x13)).mbc(), "MBC3+RAM+BATTERY");
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0x1E)).mbc(), "MBC5+RUMBLE+RAM+BATTERY");
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0xFC)).mbc(), "POCKET CAMERA");
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0xAA)).mbc(), "type $AA");
    CB_CHECK_EQ(parse(image(0x8000, "T", 0, 0, 0x04)).mbc(), "type $04");   // 4 does not exist
    CB_CHECK(parse(image(0x8000, "T", 0, 0x03)).sgb);
});

CB_TEST("formats.gb.too_short", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x14F, 0)));
    CB_CHECK_NOTHROW((void)parse(Bytes(0x150, 0)));
});

CB_TEST("formats.gb.power_on_memory", [] {
    Bytes img = image(0x10000);
    img[0x4000] = 0x41;
    img[0x8000] = 0x99;   // bank 2: not mapped
    const Rom rom = parse(img);
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem.size(), 0x10000u);
    CB_CHECK_EQ(mem[0x4000], 0x41);
    CB_CHECK_EQ(mem[0x8000], 0);
    CB_CHECK_EQ(parse(image(0x4000 + 0x150)).memory()[0x4000], 0xFF);
});

// A toy SM83: C9 ret, CD lo hi call, 00 nop, 10 stop, C3 lo hi jump.
std::optional<DecodedInsn> toy(ByteView mem, std::uint32_t pc) {
    const auto word = [&] { return static_cast<std::uint32_t>(mem[pc + 1] | (mem[pc + 2] << 8)); };
    switch (mem[pc]) {
        case 0x00: return DecodedInsn{1, Flow::Next, std::nullopt};
        case 0x10: return DecodedInsn{1, Flow::Halt, std::nullopt};
        case 0xC9: return DecodedInsn{1, Flow::Return, std::nullopt};
        case 0xCD: return DecodedInsn{3, Flow::Call, word()};
        case 0xC3: return DecodedInsn{3, Flow::Jump, word()};
        default: return std::nullopt;
    }
}

CB_TEST("formats.gb.functions_from_entry_and_vectors", [] {
    Bytes img = image();
    img[0x100] = 0xC3; img[0x101] = 0x50; img[0x102] = 0x01;   // entry: jp $0150
    img[0x150] = 0xCD; img[0x151] = 0x00; img[0x152] = 0x02;   // call $0200
    img[0x153] = 0x10;                                         // stop
    img[0x200] = 0xC9;
    img[0x40] = 0xC9;      // vblank: code
    // stat ($48) is $FF, timer ($50) is $00: both unused; serial ($58) holds what does not decode
    img[0x50] = 0x00;
    img[0x58] = 0x77;
    img[0x60] = 0xCD; img[0x61] = 0x00; img[0x62] = 0x03; img[0x63] = 0xC9;   // joypad: calls $0300
    img[0x300] = 0xC9;
    CB_CHECK((parse(img).functions(toy)) == ((std::vector<std::uint32_t>{0x40, 0x60, 0x100, 0x200, 0x300})));
});

CB_TEST("formats.gb.the_header_is_data_not_code", [] {
    Bytes img = image();
    img[0x100] = 0x00; img[0x101] = 0x00; img[0x102] = 0x00; img[0x103] = 0x00;   // a sled into the logo
    // what follows the header would call $0400 -- never reached, because $0104 ends the run
    img[0x150] = 0xCD; img[0x151] = 0x00; img[0x152] = 0x04; img[0x153] = 0xC9;
    img[0x400] = 0xC9;
    CB_CHECK((parse(img).functions(toy)) == ((std::vector<std::uint32_t>{0x100})));
});

CB_TEST("formats.gb.calls_beyond_the_two_banks_are_not_followed", [] {
    Bytes img = image();
    img[0x100] = 0xCD; img[0x101] = 0x00; img[0x102] = 0x90; img[0x103] = 0xC9;
    CB_CHECK((parse(img).functions(toy)) == ((std::vector<std::uint32_t>{0x100})));
});

CB_TEST("formats.gb.probe", [] {
    fs::ScopedTempDir dir("gb-test");
    fs::write_file(dir.path() / "a.gb", image());
    fs::write_file(dir.path() / "small.gb", Bytes(0x4000, 0));
    fs::write_file(dir.path() / "renamed.bin", image());                  // no extension, but the logo is there
    fs::write_file(dir.path() / "other.bin", Bytes(0x8000, 0));
    CB_CHECK(probe(dir.path() / "a.gb"));
    CB_CHECK(!probe(dir.path() / "small.gb"));
    CB_CHECK(probe(dir.path() / "renamed.bin"));
    CB_CHECK(!probe(dir.path() / "other.bin"));
    CB_CHECK(!probe(dir.path() / "missing.gbc"));
    CB_CHECK_EQ(read(dir.path() / "a.gb").title, "TESTGAME");
});

}  // namespace
}  // namespace cb::formats::gb
