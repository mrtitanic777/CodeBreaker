#include "cb/formats/nes.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::nes {
namespace {

// An iNES image: the 16-byte header, an optional trainer, `prg_units` 16 KB banks whose bytes are the bank number
// (so a bank is recognisable where it lands), `chr_units` 8 KB of CHR.
Bytes image(unsigned prg_units, unsigned chr_units, std::uint8_t f6 = 0, std::uint8_t f7 = 0, bool trainer = false,
            std::uint8_t b8 = 0, std::uint8_t b9 = 0) {
    Bytes out = {'N', 'E', 'S', 0x1A, static_cast<std::uint8_t>(prg_units & 0xFF), static_cast<std::uint8_t>(chr_units & 0xFF),
                 f6, f7, b8, b9, 0, 0, 0, 0, 0, 0};
    if (trainer) out.insert(out.end(), kTrainerSize, 0xEE);
    for (unsigned b = 0; b < prg_units; ++b) out.insert(out.end(), kPrgUnit, static_cast<std::uint8_t>(0x10 + b));
    out.insert(out.end(), chr_units * kChrUnit, 0xCC);
    return out;
}

void set_vectors(Bytes& img, std::size_t prg_end, std::uint16_t nmi, std::uint16_t reset, std::uint16_t irq) {
    // The vectors are the last six bytes of the last PRG bank.
    const std::size_t at = prg_end - 6;
    img[at] = nmi & 0xFF;
    img[at + 1] = nmi >> 8;
    img[at + 2] = reset & 0xFF;
    img[at + 3] = reset >> 8;
    img[at + 4] = irq & 0xFF;
    img[at + 5] = irq >> 8;
}

CB_TEST("formats.nes.header_fields", [] {
    Bytes img = image(2, 1, /*f6=*/0x43 /*mapper low nibble 4, battery, vertical*/, /*f7=*/0x20);
    const Rom rom = parse(img);
    CB_CHECK(!rom.nes2);
    CB_CHECK_EQ(rom.mapper, 0x24);
    CB_CHECK(rom.battery);
    CB_CHECK_EQ(rom.mirroring, Mirroring::Vertical);
    CB_CHECK_EQ(rom.prg.size(), 2 * kPrgUnit);
    CB_CHECK_EQ(rom.chr.size(), kChrUnit);
    CB_CHECK(rom.trainer.empty());
    CB_CHECK(rom.extra.empty());
    CB_CHECK_EQ(rom.header.size(), kHeaderSize);
    CB_CHECK_EQ(rom.code_at, 0x8000u);
});

CB_TEST("formats.nes.mirroring_names", [] {
    CB_CHECK_EQ(to_string(parse(image(1, 0, 0)).mirroring), "horizontal");
    CB_CHECK_EQ(to_string(parse(image(1, 0, 1)).mirroring), "vertical");
    CB_CHECK_EQ(to_string(parse(image(1, 0, 8 | 1)).mirroring), "four-screen");   // four-screen wins over the bit
});

CB_TEST("formats.nes.nes2_widens_mapper_and_sizes", [] {
    // NES 2.0 (f7 bits 2-3 = 2): mapper bits 8-11 and the submapper from byte 8, size high nibbles from byte 9.
    Bytes img = image(0, 0, 0x10, 0x28, false, /*b8=*/0x35, /*b9=*/0x00);
    const Rom rom = parse(img);
    CB_CHECK(rom.nes2);
    CB_CHECK_EQ(rom.mapper, 0x521);   // 1 from f6's high nibble, 2 from f7's, 5 from byte 8's low nibble
    CB_CHECK_EQ(rom.submapper, 3);
    // prg high nibble 1 -> 0x100 + 0 units of 16 KB: the file is cut short
    img[9] = 0x01;
    CB_CHECK_THROWS_AS(FormatError, (void)parse(img));
});

CB_TEST("formats.nes.trainer_is_skipped", [] {
    const Rom rom = parse(image(1, 1, 0x04, 0, /*trainer=*/true));
    CB_CHECK_EQ(rom.trainer.size(), kTrainerSize);
    CB_CHECK_EQ(rom.trainer.front(), 0xEE);
    CB_CHECK_EQ(rom.prg.front(), 0x10);
    CB_CHECK_EQ(rom.chr.front(), 0xCC);
});

CB_TEST("formats.nes.trailing_bytes_are_extra", [] {
    Bytes img = image(1, 0);
    for (char c : std::string("TITLE")) img.push_back(static_cast<std::uint8_t>(c));
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.extra.size(), 5u);
    CB_CHECK_EQ(rom.extra.front(), 'T');
});

CB_TEST("formats.nes.sixteen_kb_prg_is_mirrored", [] {
    Bytes img = image(1, 0);
    set_vectors(img, kHeaderSize + kPrgUnit, 0xC111, 0xC222, 0xC333);
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.code_at, 0xC000u);
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem.size(), kAddressSpace);
    CB_CHECK_EQ(mem[0x7FFF], 0);
    CB_CHECK_EQ(mem[0x8000], 0x10);
    CB_CHECK_EQ(mem[0xC000], 0x10);
    CB_CHECK_EQ(mem[0xBFFF], mem[0xFFFF]);   // the same 16 KB twice
    CB_CHECK_EQ(rom.vectors.nmi, 0xC111);
    CB_CHECK_EQ(rom.vectors.reset, 0xC222);
    CB_CHECK_EQ(rom.vectors.irq, 0xC333);
    CB_CHECK_EQ(*rom.mapped(0xC000), 0u);
    CB_CHECK_EQ(*rom.mapped(0x8005), 5u);
    CB_CHECK_EQ(*rom.mapped(0xFFFF), 0x3FFFu);
    CB_CHECK(!rom.mapped(0x7FFF));
});

CB_TEST("formats.nes.thirty_two_kb_fills_the_window", [] {
    Bytes img = image(2, 0);
    const Rom rom = parse(img);
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem[0x8000], 0x10);
    CB_CHECK_EQ(mem[0xBFFF], 0x10);
    CB_CHECK_EQ(mem[0xC000], 0x11);
    CB_CHECK_EQ(*rom.mapped(0xC000), kPrgUnit);
    CB_CHECK_EQ(rom.bank_count(), 2u);
    CB_CHECK_EQ(rom.bank(1).front(), 0x11);
});

CB_TEST("formats.nes.banked_boards_fix_first_and_last", [] {
    Bytes img = image(8, 0, /*f6=*/0x10);   // mapper 1, 128 KB
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.mapper, 1);
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem[0x8000], 0x10);          // bank 0 at $8000
    CB_CHECK_EQ(mem[0xC000], 0x17);          // the last bank at $C000
    CB_CHECK_EQ(*rom.mapped(0x9000), 0x1000u);
    CB_CHECK_EQ(*rom.mapped(0xC010), 7 * kPrgUnit + 0x10);
});

CB_TEST("formats.nes.nrom_prg_up_to_32kb_maps_from_8000_even_with_a_mapper", [] {
    const Rom rom = parse(image(2, 0, 0x30));   // mapper 3 (CNROM), 32 KB of PRG: no PRG banking
    CB_CHECK_EQ(rom.mapper, 3);
    CB_CHECK_EQ(rom.memory()[0xC000], 0x11);
    CB_CHECK_EQ(*rom.mapped(0xC000), kPrgUnit);
});

CB_TEST("formats.nes.errors", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{'N', 'E', 'S', 0x00, 1, 1}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{'N', 'E', 'S', 0x1A, 1, 1}));   // header cut
    Bytes img = image(2, 0);
    img.resize(img.size() - 1);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(img));                                // PRG cut
    Bytes trainer = image(1, 0, 0x04, 0, false);                                         // trainer flag, no trainer
    CB_CHECK_THROWS_AS(FormatError, (void)parse(trainer));
});

CB_TEST("formats.nes.a_trimmed_chr_is_read_as_far_as_it_goes", [] {
    Bytes img = image(1, 2);
    img.resize(img.size() - 100);
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.chr.size(), 2 * kChrUnit - 100);
});

CB_TEST("formats.nes.functions_follow_vectors_and_jsr", [] {
    // A toy decoder: opcode 0x20 is "call" with a 16-bit little-endian target, 0x60 returns, 0xEA is a nop.
    const Decoder decode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
        switch (mem[pc]) {
            case 0xEA: return DecodedInsn{1, Flow::Next, std::nullopt};
            case 0x60: return DecodedInsn{1, Flow::Return, std::nullopt};
            case 0x20: return DecodedInsn{3, Flow::Call, static_cast<std::uint32_t>(mem[pc + 1] | (mem[pc + 2] << 8))};
            default: return std::nullopt;
        }
    };
    Bytes img = image(1, 0);
    set_vectors(img, kHeaderSize + kPrgUnit, 0xC000, 0xC010, 0x0000);   // irq below $8000 is not a start
    auto at = [&](std::uint32_t address) { return kHeaderSize + (address & 0x3FFF); };
    img[at(0xC000)] = 0x60;                                                        // nmi: rts
    const Bytes reset = {0x20, 0x30, 0xC0, 0x20, 0x00, 0x10, 0x60};                // jsr $C030; jsr $1000 (not code); rts
    std::copy(reset.begin(), reset.end(), img.begin() + static_cast<std::ptrdiff_t>(at(0xC010)));
    img[at(0xC030)] = 0xEA;
    img[at(0xC031)] = 0x60;
    const Rom rom = parse(img);
    CB_CHECK((rom.functions(decode)) == ((std::vector<std::uint32_t>{0xC000, 0xC010, 0xC030})));
});

CB_TEST("formats.nes.read_and_probe_a_file", [] {
    fs::ScopedTempDir dir("nes-test");
    const auto path = dir.path() / "demo.nes";
    fs::write_file(path, image(1, 1));
    CB_CHECK(probe(path));
    CB_CHECK_EQ(read(path).prg.size(), kPrgUnit);
    const auto junk = dir.path() / "junk.nes";
    fs::write_file(junk, Bytes{1, 2, 3, 4, 5});
    CB_CHECK(!probe(junk));
    CB_CHECK(!probe(dir.path() / "missing.nes"));
    CB_CHECK_THROWS_AS(NotFoundError, (void)read(dir.path() / "missing.nes"));
});

}  // namespace
}  // namespace cb::formats::nes
