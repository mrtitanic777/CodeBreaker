#include "cb/formats/atari2600.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::atari2600 {
namespace {

// A cartridge of `banks` 4 KB banks (or one of 2 KB): each bank's bytes are 0x10 + its number, the vectors at its end, reset at `reset`.
Bytes image(std::size_t size, std::uint16_t reset = 0xF000, std::uint16_t irq = 0xF010) {
    const std::size_t bank_size = size == 0x800 ? 0x800 : 0x1000;
    Bytes out;
    for (std::size_t b = 0; b < size / bank_size; ++b) {
        Bytes bank(bank_size, static_cast<std::uint8_t>(0x10 + b));
        bank[bank_size - 4] = reset & 0xFF;
        bank[bank_size - 3] = reset >> 8;
        bank[bank_size - 2] = irq & 0xFF;
        bank[bank_size - 1] = irq >> 8;
        out.insert(out.end(), bank.begin(), bank.end());
    }
    return out;
}

}  // namespace

CB_TEST("formats.atari2600.schemes_by_size", [] {
    CB_CHECK_EQ(to_string(parse(image(0x800, 0xF800)).scheme), "none");
    CB_CHECK_EQ(to_string(parse(image(0x1000)).scheme), "none");
    CB_CHECK_EQ(to_string(parse(image(0x2000)).scheme), "F8");
    CB_CHECK_EQ(to_string(parse(image(0x4000)).scheme), "F6");
    CB_CHECK_EQ(to_string(parse(image(0x8000)).scheme), "F4");
    CB_CHECK_EQ(parse(image(0x4000)).bank_count(), 4u);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(Bytes(3000, 0)));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(image(0x1000, 0x0200)));   // reset not in the window
});

CB_TEST("formats.atari2600.window_and_vectors", [] {
    const Rom f = parse(image(0x1000, 0xF123, 0xF456));
    CB_CHECK_EQ(f.window, 0xF000u);
    CB_CHECK_EQ(f.vectors[0].reset, 0xF123);
    CB_CHECK_EQ(f.vectors[0].irq, 0xF456);
    CB_CHECK_EQ(parse(image(0x1000, 0x1000, 0x1000)).window, 0x1000u);   // a cartridge assembled for $1000
    const Rom two = parse(image(0x800, 0xF800));
    CB_CHECK_EQ(two.window, 0xF800u);
    CB_CHECK_EQ(two.bank_size, 0x800u);
});

CB_TEST("formats.atari2600.memory_mirrors_and_fold", [] {
    const Rom two = parse(image(0x800, 0xF800));
    const Bytes mem = two.memory(0);
    CB_CHECK_EQ(mem[0xF000], 0x10);   // a 2 KB cartridge twice in each 4 KB
    CB_CHECK_EQ(mem[0xF800], 0x10);
    CB_CHECK_EQ(mem[0x1000], 0x10);
    CB_CHECK_EQ(mem[0x0200], 0);      // nothing below the window
    CB_CHECK_EQ(two.fold(0xF023).value(), 0xF823u);
    CB_CHECK_EQ(two.fold(0x1FFC).value(), 0xFFFCu);
    CB_CHECK(!two.fold(0x0200).has_value());
    const Rom f8 = parse(image(0x2000));
    CB_CHECK_EQ(f8.memory(1)[0xF000], 0x11);
    CB_CHECK_EQ(f8.fold(0x1ABC).value(), 0xFABCu);
    CB_CHECK_EQ(f8.offset(1, 0xF010), 0x1010u);
});

CB_TEST("formats.atari2600.hotspots", [] {
    const Rom f8 = parse(image(0x2000));
    CB_CHECK_EQ(f8.bank_of_hotspot(0x1FF8).value(), 0u);
    CB_CHECK_EQ(f8.bank_of_hotspot(0xFFF9).value(), 1u);
    CB_CHECK(!f8.bank_of_hotspot(0x1FFA).has_value());
    CB_CHECK(!f8.bank_of_hotspot(0x0FF8).has_value());   // not in the cartridge's half
    const Rom f4 = parse(image(0x8000));
    CB_CHECK_EQ(f4.bank_of_hotspot(0x1FFB).value(), 7u);
    CB_CHECK_EQ(f4.bank_of_hotspot(0x1FF4).value(), 0u);
    CB_CHECK(!parse(image(0x1000)).bank_of_hotspot(0x1FF8).has_value());
});

CB_TEST("formats.atari2600.functions_follow_code", [] {
    // reset at $F000: JSR $F020; JMP $F000. The sub at $F020: RTS.
    Bytes img = image(0x1000, 0xF000, 0xF030);
    const Bytes code = {0x20, 0x20, 0xF0, 0x4C, 0x00, 0xF0};
    std::copy(code.begin(), code.end(), img.begin());
    img[0x20] = 0x60;
    img[0x30] = 0x40;   // RTI
    const Rom rom = parse(img);
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
    CB_CHECK_EQ(fs[0], 0xF000u);
    CB_CHECK_EQ(fs[1], 0xF020u);
    CB_CHECK_EQ(fs[2], 0xF030u);
});

CB_TEST("formats.atari2600.probe", [] {
    fs::ScopedTempDir scoped("cb_a2600_probe");
    const std::filesystem::path dir = scoped.path();
    Bytes img = image(0x1000);
    img[0] = 0x78;   // SEI at the reset
    fs::write_file(dir / "game.a26", img);
    fs::write_file(dir / "game.bin", img);
    CB_CHECK(probe(dir / "game.a26"));
    CB_CHECK(probe(dir / "game.bin"));
    Bytes odd = image(0x1000);
    odd[0] = 0x02;   // an opcode no start-up uses
    fs::write_file(dir / "odd.bin", odd);
    fs::write_file(dir / "odd.a26", odd);
    CB_CHECK(!probe(dir / "odd.bin"));
    CB_CHECK(probe(dir / "odd.a26"));
    fs::write_file(dir / "short.a26", Bytes(100, 0));
    CB_CHECK(!probe(dir / "short.a26"));
    fs::write_file(dir / "game.nes", img);
    CB_CHECK(!probe(dir / "game.nes"));
    CB_CHECK(!probe(dir / "missing.a26"));
});

}  // namespace cb::formats::atari2600
