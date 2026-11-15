#include "cb/formats/atari7800.hpp"

#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::atari7800 {
namespace {

// A header (version 3, the title, the size and cartridge type fields) and `size` bytes of ROM whose bank number is its fill (so a bank is recognisable
// where it lands), with the vectors at its end.
Bytes image(std::size_t size, std::uint16_t type = 0, std::uint16_t nmi = 0xC010, std::uint16_t reset = 0xC000, std::uint16_t irq = 0xC020) {
    Bytes file(kHeaderSize, 0);
    file[0] = 3;
    const std::string magic = "ATARI7800";
    std::copy(magic.begin(), magic.end(), file.begin() + 1);
    const std::string title = "Test Game";
    std::copy(title.begin(), title.end(), file.begin() + 17);
    file[49] = static_cast<std::uint8_t>(size >> 24);
    file[50] = static_cast<std::uint8_t>(size >> 16);
    file[51] = static_cast<std::uint8_t>(size >> 8);
    file[52] = static_cast<std::uint8_t>(size);
    file[53] = static_cast<std::uint8_t>(type >> 8);
    file[54] = static_cast<std::uint8_t>(type);
    file[55] = 1;
    file[57] = 1;
    for (std::size_t i = 0; i < size; ++i) file.push_back(static_cast<std::uint8_t>(0x10 + i / kBankSize));
    const std::size_t end = file.size();
    file[end - 6] = nmi & 0xFF;
    file[end - 5] = nmi >> 8;
    file[end - 4] = reset & 0xFF;
    file[end - 3] = reset >> 8;
    file[end - 2] = irq & 0xFF;
    file[end - 1] = irq >> 8;
    return file;
}

}  // namespace

CB_TEST("formats.atari7800.header_fields", [] {
    const Rom rom = parse(image(0x4000));
    CB_CHECK_EQ(rom.version, 3);
    CB_CHECK_EQ(rom.title, "Test Game");
    CB_CHECK_EQ(rom.declared_size, 0x4000u);
    CB_CHECK_EQ(rom.cart_type, 0);
    CB_CHECK_EQ(rom.controller1, 1);
    CB_CHECK_EQ(rom.tv, 1);
    CB_CHECK_EQ(rom.header.size(), kHeaderSize);
    CB_CHECK_EQ(rom.data.size(), 0x4000u);
    CB_CHECK(!rom.supergame);
    CB_CHECK_EQ(rom.vectors.nmi, 0xC010);
    CB_CHECK_EQ(rom.vectors.reset, 0xC000);
    CB_CHECK_EQ(rom.vectors.irq, 0xC020);
});

CB_TEST("formats.atari7800.the_rom_ends_at_the_top_of_memory", [] {
    CB_CHECK_EQ(parse(image(0x4000)).base, 0xC000u);
    CB_CHECK_EQ(parse(image(0x8000, 0, 0x8010, 0x8000, 0x8020)).base, 0x8000u);
    CB_CHECK_EQ(parse(image(0xC000, 0, 0x4010, 0x4000, 0x4020)).base, 0x4000u);
    const Rom rom = parse(image(0x8000, 0, 0x8010, 0x8000, 0x8020));
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem[0x8000], 0x10);
    CB_CHECK_EQ(mem[0x7FFF], 0);
    CB_CHECK_EQ(rom.offset(0, 0x8000).value(), 0u);
    CB_CHECK_EQ(rom.offset(0, 0xFFFF).value(), 0x7FFFu);
    CB_CHECK(!rom.offset(0, 0x7FFF).has_value());
});

CB_TEST("formats.atari7800.supergame_banks", [] {
    const Rom rom = parse(image(0x20000, kTypeSuperGame, 0xC010, 0xC000, 0xC020));
    CB_CHECK(rom.supergame);
    CB_CHECK_EQ(rom.bank_count(), 8u);
    CB_CHECK_EQ(rom.fixed_bank(), 7u);
    const Bytes bank3 = rom.memory(3);
    CB_CHECK_EQ(bank3[0x8000], 0x13);   // the selected bank at $8000
    CB_CHECK_EQ(bank3[0xC000], 0x17);   // the last bank always at $C000
    CB_CHECK_EQ(rom.offset(3, 0x8000).value(), 3u * kBankSize);
    CB_CHECK_EQ(rom.offset(3, 0xC000).value(), 7u * kBankSize);
    CB_CHECK(!rom.offset(3, 0x4000).has_value());
});

CB_TEST("formats.atari7800.rejects_what_it_does_not_model", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(Bytes(200, 0)));      // no magic
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(Bytes(50, 0)));       // too short for a header
    Bytes cut = image(0x4000);
    cut.resize(cut.size() - 100);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(cut));                // the ROM is cut short
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(image(0x4001)));      // not a multiple of 4 KB
});

CB_TEST("formats.atari7800.functions_follow_code", [] {
    Bytes file = image(0x4000);
    // reset $C000: JSR $C020; RTS-style: JMP $C000. sub $C020: RTS. nmi $C010: RTI. irq $C020: (the sub)
    const Bytes code = {0x20, 0x20, 0xC0, 0x4C, 0x00, 0xC0};
    std::copy(code.begin(), code.end(), file.begin() + kHeaderSize);
    file[kHeaderSize + 0x10] = 0x40;
    file[kHeaderSize + 0x20] = 0x60;
    const Rom rom = parse(file);
    const Decoder decode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
        DecodedInsn d;
        switch (mem[pc]) {
            case 0x20: d.size = 3; d.flow = Flow::Call; d.target = mem[pc + 1] | mem[pc + 2] << 8; return d;
            case 0x4C: d.size = 3; d.flow = Flow::Jump; d.target = mem[pc + 1] | mem[pc + 2] << 8; return d;
            case 0x60:
            case 0x40: d.size = 1; d.flow = Flow::Return; return d;
            default: return std::nullopt;
        }
    };
    const std::vector<std::uint32_t> fs = rom.functions(decode);
    CB_CHECK_EQ(fs.size(), 3u);
    CB_CHECK_EQ(fs[0], 0xC000u);
    CB_CHECK_EQ(fs[1], 0xC010u);
    CB_CHECK_EQ(fs[2], 0xC020u);
});

CB_TEST("formats.atari7800.probe", [] {
    fs::ScopedTempDir dir("cb_a7800_probe");
    fs::write_file(dir.path() / "game.a78", image(0x4000));
    CB_CHECK(probe(dir.path() / "game.a78"));
    fs::write_file(dir.path() / "other.a78", Bytes(200, 0x11));
    CB_CHECK(!probe(dir.path() / "other.a78"));
    fs::write_file(dir.path() / "short.a78", Bytes(20, 0));
    CB_CHECK(!probe(dir.path() / "short.a78"));
    CB_CHECK(!probe(dir.path() / "missing.a78"));
});

}  // namespace cb::formats::atari7800
