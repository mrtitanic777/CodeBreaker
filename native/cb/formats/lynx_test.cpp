#include "cb/formats/lynx.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/lynx_test_image.hpp"

namespace cb::formats::lynx {
namespace {

// A program for $0400: LDA #1 / STA SPRCTL0 / JSR $040C / JMP $0408 (a loop), a stray byte, then STZ SPRGO / INC A / RTS.
const Bytes kProgram = {0xA9, 0x01, 0x8D, 0x80, 0xFC, 0x20, 0x0C, 0x04, 0x4C, 0x08, 0x04, 0xEA, 0x9C, 0x91, 0xFC, 0x1A, 0x60};

Bytes bs93(std::uint16_t load, ByteView body, std::size_t trailer = 0) {
    const std::size_t length = body.size() + kBs93Header;
    Bytes out = {0x80, 0x08, static_cast<std::uint8_t>(load >> 8), static_cast<std::uint8_t>(load), static_cast<std::uint8_t>(length >> 8),
                 static_cast<std::uint8_t>(length), 'B', 'S', '9', '3'};
    out.insert(out.end(), body.begin(), body.end());
    out.insert(out.end(), trailer, 0x00);
    return out;
}

CB_TEST("formats.lynx.header_and_first_stage", [] {
    const Bytes file = lynx_test_image::cartridge(kProgram, "Test Cart");
    CB_CHECK(is_cartridge(file) && !is_bs93(file));
    const Program p = parse(file);
    CB_CHECK(p.kind == Kind::Cartridge);
    CB_CHECK_EQ(p.name, "Test Cart");
    CB_CHECK_EQ(p.bank0_block, 1024);
    CB_CHECK_EQ(p.version, 1);
    CB_CHECK_EQ(p.encrypted_blocks, 1u);
    // the first stage decrypts to the 16 bytes of code the helper encrypted, then zeros
    CB_CHECK_EQ(p.stage1.size(), 50u);
    CB_CHECK_EQ(p.stage1[0], 0xA2);
    CB_CHECK_EQ(p.stage1[3], 0xB2);
    CB_CHECK_EQ(p.stage1[10], static_cast<std::uint8_t>(kProgram.size()));
    CB_CHECK_EQ(p.stage1[15], 0x04);
    CB_CHECK_EQ(p.stage1[49], 0);
    // memory(): the first stage at $0200
    const Bytes mem = p.memory();
    CB_CHECK_EQ(mem[0x200], 0xA2);
    CB_CHECK_EQ(mem[0x400], 0);   // nothing loaded until a boot says so
});

CB_TEST("formats.lynx.a_damaged_first_stage_is_not_decrypted", [] {
    Bytes file = lynx_test_image::cartridge(kProgram);
    file[64 + 20] ^= 0x55;   // a bit of the encrypted block
    const Program p = parse(file);
    CB_CHECK(p.stage1.empty());
    CB_CHECK_EQ(p.encrypted_blocks, 0u);
    CB_CHECK(decrypt_blocks(ByteView(file).subspan(65), 6).empty());   // more blocks than the ROM reads
});

CB_TEST("formats.lynx.bs93_object", [] {
    const Bytes file = bs93(0x0400, kProgram, 3);
    CB_CHECK(is_bs93(file) && !is_cartridge(file));
    const Program p = parse(file);
    CB_CHECK(p.kind == Kind::Bs93);
    CB_CHECK_EQ(p.load_address, 0x0400);
    CB_CHECK_EQ(p.block_length, kProgram.size() + kBs93Header);
    CB_CHECK((p.segments == std::vector<Segment>{{kBs93Header, 0x0400, static_cast<std::uint32_t>(kProgram.size())}}));
    CB_CHECK((p.entries == std::vector<std::uint32_t>{0x0400}));
    CB_CHECK_EQ(p.memory()[0x400], 0xA9);
    CB_CHECK_EQ(p.memory()[0x400 + 16], 0x60);
    CB_CHECK(p.address_of(kBs93Header + 2) == std::optional<std::uint32_t>(0x0402));
    CB_CHECK(!p.address_of(0));   // the header is not loaded
    // a block longer than the file is not an object
    Bytes cut = file;
    cut.resize(15);
    CB_CHECK(!is_bs93(cut));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(cut));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)parse(Bytes(100, 0x55)));
});

CB_TEST("formats.lynx.probe_reads_the_file", [] {
    fs::ScopedTempDir dir("cb_lynx_probe");
    const auto put = [&](const char* name, const Bytes& data) {
        fs::write_file(dir.path() / name, data);
        return dir.path() / name;
    };
    CB_CHECK(probe(put("a.lnx", lynx_test_image::cartridge(kProgram))));
    CB_CHECK(probe(put("b.o", bs93(0x0200, kProgram))));
    CB_CHECK(!probe(put("c.bin", Bytes(300, 0x11))));
    CB_CHECK(!probe(dir.path() / "missing.lnx"));
});

CB_TEST("formats.lynx.functions_follow_calls_from_the_entries", [] {
    Program p = parse(bs93(0x0400, kProgram));
    const Decoder decode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
        // just enough decoder for the program: LDA #, STA abs, JSR, JMP, STZ abs, INC A, RTS, NOP
        switch (mem[pc]) {
            case 0xA9: return DecodedInsn{2, Flow::Next, std::nullopt};
            case 0x8D:
            case 0x9C: return DecodedInsn{3, Flow::Next, std::nullopt};
            case 0x20: return DecodedInsn{3, Flow::Call, static_cast<std::uint32_t>(mem[pc + 1] | (mem[pc + 2] << 8))};
            case 0x4C: return DecodedInsn{3, Flow::Jump, static_cast<std::uint32_t>(mem[pc + 1] | (mem[pc + 2] << 8))};
            case 0x1A:
            case 0xEA: return DecodedInsn{1, Flow::Next, std::nullopt};
            case 0x60: return DecodedInsn{1, Flow::Return, std::nullopt};
            default: return std::nullopt;
        }
    };
    CB_CHECK((p.functions(decode) == std::vector<std::uint32_t>{0x0400, 0x040C}));
});

CB_TEST("formats.lynx.registers_have_their_documented_names", [] {
    const auto& r = registers();
    CB_CHECK_EQ(r.at(0xFC00), "TMPADRL");
    CB_CHECK_EQ(r.at(0xFC01), "TMPADRH");
    CB_CHECK_EQ(r.at(0xFC2E), "PROCADRL");
    CB_CHECK_EQ(r.at(0xFC80), "SPRCTL0");
    CB_CHECK_EQ(r.at(0xFC91), "SPRGO");
    CB_CHECK_EQ(r.at(0xFCB0), "JOYSTICK");
    CB_CHECK_EQ(r.at(0xFCB2), "RCART0");
    CB_CHECK_EQ(r.at(0xFD00), "TIM0BKUP");
    CB_CHECK_EQ(r.at(0xFD1F), "TIM7CTLB");
    CB_CHECK_EQ(r.at(0xFD20), "AUD0VOL");
    CB_CHECK_EQ(r.at(0xFD38), "AUD3VOL");
    CB_CHECK_EQ(r.at(0xFD87), "SYSCTL1");
    CB_CHECK_EQ(r.at(0xFD92), "DISPCTL");
    CB_CHECK_EQ(r.at(0xFDA0), "GREEN0");
    CB_CHECK_EQ(r.at(0xFDBF), "BLUEREDF");
    CB_CHECK_EQ(r.at(0xFFF9), "MAPCTL");
});

}  // namespace
}  // namespace cb::formats::lynx
