#include "cb/formats/genesis.hpp"

#include <cstring>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::genesis {
namespace {

void put_long(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

void put_text(Bytes& b, std::size_t at, std::string_view s, std::size_t width) {
    std::memset(b.data() + at, ' ', width);
    std::memcpy(b.data() + at, s.data(), s.size());
}

// A cartridge of `size` bytes: the vectors (stack, reset $200), a header, and 0x4E71 (nop) everywhere else.
Bytes image(std::size_t size = 0x600) {
    Bytes b(size);
    for (std::size_t i = 0; i + 1 < size; i += 2) { b[i] = 0x4E; b[i + 1] = 0x71; }
    std::memset(b.data(), 0, 0x100);
    put_long(b, 0, 0x00FF0000);
    put_long(b, 4, 0x200);
    put_text(b, 0x100, "SEGA MEGA DRIVE", 16);
    put_text(b, 0x110, "(C)TEST 2026.JAN", 16);
    put_text(b, 0x120, "TEST GAME", 48);
    put_text(b, 0x150, "TEST GAME OVERSEAS", 48);
    put_text(b, 0x180, "GM 00000000-00", 14);
    b[0x18E] = 0xAB; b[0x18F] = 0xCD;
    put_text(b, 0x190, "J6", 16);
    put_long(b, 0x1A0, 0);
    put_long(b, 0x1A4, static_cast<std::uint32_t>(size - 1));
    put_long(b, 0x1A8, 0x00FF0000);
    put_long(b, 0x1AC, 0x00FFFFFF);
    put_text(b, 0x1F0, "JUE", 3);
    return b;
}

CB_TEST("formats.genesis.header_fields", [] {
    const Rom rom = parse(image());
    CB_CHECK(!rom.smd);
    CB_CHECK_EQ(rom.header.system, "SEGA MEGA DRIVE");
    CB_CHECK_EQ(rom.header.copyright, "(C)TEST 2026.JAN");
    CB_CHECK_EQ(rom.header.domestic_name, "TEST GAME");
    CB_CHECK_EQ(rom.header.overseas_name, "TEST GAME OVERSEAS");
    CB_CHECK_EQ(rom.header.serial, "GM 00000000-00");
    CB_CHECK_EQ(rom.header.checksum, 0xABCD);
    CB_CHECK_EQ(rom.header.devices, "J6");
    CB_CHECK_EQ(rom.header.rom_start, 0u);
    CB_CHECK_EQ(rom.header.rom_end, 0x5FFu);
    CB_CHECK_EQ(rom.header.ram_start, 0x00FF0000u);
    CB_CHECK_EQ(rom.header.ram_end, 0x00FFFFFFu);
    CB_CHECK_EQ(rom.header.region, "JUE");
    CB_CHECK_EQ(rom.initial_ssp, 0x00FF0000u);
    CB_CHECK_EQ(rom.reset, 0x200u);
});

CB_TEST("formats.genesis.vectors_keep_even_in_rom_addresses_from_200", [] {
    Bytes b = image();
    put_long(b, 4 * 30, 0x400);   // vblank
    put_long(b, 4 * 28, 0x401);   // hblank at an odd address: dropped
    put_long(b, 4 * 3, 0x1FE);    // address error below $200: dropped
    put_long(b, 4 * 2, 0x100000); // bus error outside the ROM: dropped
    put_long(b, 4 * 32, 0x210);   // trap0
    put_long(b, 4 * 47, 0x220);   // trap15
    put_long(b, 4 * 12, 0x230);   // vector 12 has no name: ignored
    const Rom rom = parse(b);
    std::vector<std::pair<std::string, std::uint32_t>> got;
    for (const Vector& v : rom.vectors) got.emplace_back(v.name, v.address);
    const std::vector<std::pair<std::string, std::uint32_t>> want = {{"vblank", 0x400}, {"trap0", 0x210}, {"trap15", 0x220}};
    CB_CHECK((got) == (want));   // in vector-number order
});

CB_TEST("formats.genesis.memory_and_checksum", [] {
    Bytes b = image(0x204);
    b[0x200] = 0x12; b[0x201] = 0x34; b[0x202] = 0x00; b[0x203] = 0x01;
    const Rom rom = parse(b);
    CB_CHECK_EQ(rom.computed_checksum(), 0x1235);   // 0x1234 + 0x0001
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem.size(), 0x1000000u);
    CB_CHECK_EQ(mem[0x200], 0x12);
    CB_CHECK_EQ(mem[0x204], 0);
    // an odd length pairs the last byte with a zero
    Bytes odd = image(0x201);
    odd[0x200] = 0x05;
    CB_CHECK_EQ(parse(odd).computed_checksum(), 0x0500);
    // the sum wraps at 16 bits
    Bytes wrap = image(0x204);
    wrap[0x200] = 0xFF; wrap[0x201] = 0xFF; wrap[0x202] = 0x00; wrap[0x203] = 0x02;
    CB_CHECK_EQ(parse(wrap).computed_checksum(), 0x0001);
});

CB_TEST("formats.genesis.too_short", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x1FF, 0)));
    CB_CHECK_NOTHROW((void)parse(Bytes(0x200, 0)));
});

// The .smd form of a plain image: 512 header bytes (the third byte pair $AA $BB at 8), then 16 KB blocks each
// holding the odd bytes of its 16 KB first, then the even bytes.
Bytes interleave(const Bytes& plain) {
    Bytes out(512, 0);
    out[8] = 0xAA; out[9] = 0xBB;
    for (std::size_t b = 0; b < plain.size(); b += 0x4000) {
        const std::size_t n = std::min<std::size_t>(0x4000, plain.size() - b);
        Bytes block(n);
        for (std::size_t i = 0; i < n / 2; ++i) block[i] = plain[b + 2 * i + 1];
        for (std::size_t i = 0; i < n - n / 2; ++i) block[n / 2 + i] = plain[b + 2 * i];
        out.insert(out.end(), block.begin(), block.end());
    }
    return out;
}

CB_TEST("formats.genesis.smd_is_deinterleaved", [] {
    Bytes plain = image(0x8000);
    for (std::size_t i = 0x200; i < plain.size(); ++i) plain[i] = static_cast<std::uint8_t>(i * 7 + (i >> 8));
    const Bytes smd = interleave(plain);
    CB_CHECK_EQ(smd.size() % 0x4000, 512u);
    CB_CHECK(deinterleave_smd(smd) == plain);
    const Rom rom = parse(smd);
    CB_CHECK(rom.smd);
    CB_CHECK(rom.data == plain);
    CB_CHECK_EQ(rom.header.system, "SEGA MEGA DRIVE");
    // with only the extension as the hint (the $AA $BB marker missing)
    Bytes unmarked = smd;
    unmarked[8] = 0; unmarked[9] = 0;
    CB_CHECK(!parse(unmarked).smd);   // not recognised without a marker...
    CB_CHECK(parse(unmarked, ".smd").smd);   // ...but the extension says so
    CB_CHECK(parse(unmarked, ".SMD").data == plain);
});

CB_TEST("formats.genesis.a_plain_image_of_smd_size_is_not_an_smd", [] {
    // 512 bytes over a block boundary but with SEGA at $100 and no marker: plain, not interleaved
    Bytes b = image(0x4000 + 512);
    const Rom rom = parse(b);
    CB_CHECK(!rom.smd);
    CB_CHECK_EQ(rom.data.size(), b.size());
});

CB_TEST("formats.genesis.functions", [] {
    // A toy 68000: 4E71 nop, 4E75 rts, 6100 hhll bsr (4 bytes) with the absolute target after the opcode word.
    const Decoder decode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
        if (mem[pc] == 0x4E && mem[pc + 1] == 0x71) return DecodedInsn{2, Flow::Next, std::nullopt};
        if (mem[pc] == 0x4E && mem[pc + 1] == 0x75) return DecodedInsn{2, Flow::Return, std::nullopt};
        if (mem[pc] == 0x61 && mem[pc + 1] == 0x00) {
            return DecodedInsn{4, Flow::Call, static_cast<std::uint32_t>(mem[pc + 2] << 8 | mem[pc + 3])};
        }
        return std::nullopt;
    };
    Bytes b = image();
    auto at = [&](std::size_t a, std::initializer_list<std::uint8_t> bytes) { std::copy(bytes.begin(), bytes.end(), b.begin() + static_cast<std::ptrdiff_t>(a)); };
    at(0x200, {0x61, 0x00, 0x03, 0x00, 0x61, 0x00, 0x03, 0x01, 0x4E, 0x75});   // reset: bsr $300; bsr $301 (odd: ignored); rts
    at(0x300, {0x4E, 0x75});
    put_long(b, 4 * 30, 0x400);   // vblank handler
    at(0x400, {0x4E, 0x75});
    put_long(b, 4 * 28, 0x500);   // hblank handler is nops to the end
    CB_CHECK((parse(b).functions(decode)) == ((std::vector<std::uint32_t>{0x200, 0x300, 0x400, 0x500})));
});

CB_TEST("formats.genesis.probe", [] {
    fs::ScopedTempDir dir("genesis-test");
    fs::write_file(dir.path() / "a.bin", image());
    Bytes shifted(0x600, 0);
    std::memcpy(shifted.data() + 0x101, "SEGA", 4);
    fs::write_file(dir.path() / "shifted.bin", shifted);
    fs::write_file(dir.path() / "bare.md", Bytes(0x300, 0));
    fs::write_file(dir.path() / "bare.bin", Bytes(0x300, 0));
    fs::write_file(dir.path() / "tiny.md", Bytes(0x100, 0));
    CB_CHECK(probe(dir.path() / "a.bin"));
    CB_CHECK(probe(dir.path() / "shifted.bin"));
    CB_CHECK(probe(dir.path() / "bare.md"));
    CB_CHECK(!probe(dir.path() / "bare.bin"));
    CB_CHECK(!probe(dir.path() / "tiny.md"));
    CB_CHECK(!probe(dir.path() / "missing.md"));
    CB_CHECK_EQ(read(dir.path() / "a.bin").header.region, "JUE");
});

}  // namespace
}  // namespace cb::formats::genesis
