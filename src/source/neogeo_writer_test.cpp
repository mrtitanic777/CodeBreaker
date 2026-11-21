// Tests of the Neo Geo writer over a small P-ROM of its own: the vector table and the header with its four callbacks, a user routine that calls a BIOS
// entry point and writes the LSPC, a demo-end routine that calls it, a vertical-blank handler and padding. The program is machine code written out byte by
// byte; the writer is held to what it is for -- the source it writes, built again, is the P-ROM it came from -- and to the text it writes, a golden file
// that was read through for correctness (testdata/writers/neogeo.golden.asm).

#include "source/neogeo_writer.hpp"

#include <cstring>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::neogeo_writer {
namespace {

void put_long(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

void put_bytes(Bytes& b, std::size_t at, std::initializer_list<std::uint8_t> bytes) { std::copy(bytes.begin(), bytes.end(), b.begin() + static_cast<std::ptrdiff_t>(at)); }

// $400 JSR ($C00450).L ; MOVE.W #7,($3C0000).L ; RTS     $410 NOP ; RTS     $420 BSR $400 ; RTS     $500 RTE
Bytes prom(std::size_t size = 0x1000) {
    Bytes b(size, 0xFF);
    std::fill(b.begin(), b.begin() + 0x200, std::uint8_t{0});
    put_long(b, 0, 0x0010F300);
    put_long(b, 4, 0x00C00402);
    put_long(b, 4 * 25, 0x500);
    put_long(b, 4 * 26, 0x00C00426);
    std::memcpy(b.data() + 0x100, "NEO-GEO", 7);
    b[0x109] = 0x23;
    b[0x108] = 0x01;
    put_long(b, 0x10A, static_cast<std::uint32_t>(size));
    b[0x114] = 1;
    put_long(b, 0x116, 0x200);
    put_bytes(b, 0x122, {0x4E, 0xF9, 0, 0, 0x04, 0x00});
    put_bytes(b, 0x128, {0x4E, 0xF9, 0, 0, 0x04, 0x10});
    put_bytes(b, 0x12E, {0x4E, 0xF9, 0, 0, 0x04, 0x20});
    put_bytes(b, 0x134, {0x4E, 0xF9, 0, 0, 0x04, 0x20});
    put_long(b, 0x182, 0x300);
    put_bytes(b, 0x300, {0x4E, 0x75});   // the security code: RTS
    put_bytes(b, 0x400, {0x4E, 0xB9, 0x00, 0xC0, 0x04, 0x50, 0x33, 0xFC, 0x00, 0x07, 0x00, 0x3C, 0x00, 0x00, 0x4E, 0x75});
    put_bytes(b, 0x410, {0x4E, 0x71, 0x4E, 0x75});
    put_bytes(b, 0x420, {0x61, 0x00, 0xFF, 0xDE, 0x4E, 0x75});
    put_bytes(b, 0x500, {0x4E, 0x73});
    return b;
}

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.neogeo_writer.the_prom_comes_back_as_source_that_builds_the_same_prom", [] {
    fs::ScopedTempDir dir("cb_ng");
    const Bytes image = prom();
    const image_writer::Written r = write(formats::neogeo::parse(image), "demo-p1.p1", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    // where the callbacks jump to are the functions (the header's JMPs are code, not functions); the callbacks that share a place take the first name
    CB_CHECK((r.functions == image_writer::Functions{{0x300, "security"}, {0x400, "user"}, {0x410, "player_start"}, {0x420, "demo_end"}, {0x500, "vblank"}}));
    CB_CHECK_EQ(r.code_bytes, 54u);   // four JMPs (24), RTS (2), JSR + MOVE.W + RTS (16), NOP + RTS (4), BSR + RTS (6), RTE (2)
    CB_CHECK_EQ(r.image_bytes, 0x1000u);
    CB_CHECK(r.source == dir.path() / "demo-p1.asm");
    CB_CHECK(r.built && *r.built == dir.path() / "demo-p1.built.p1" && fs::read_file(*r.built) == image);
    check_golden("neogeo.golden.asm", fs::read_text(r.source));
});

CB_TEST("source.neogeo_writer.the_golden_names_what_the_code_touches", [] {
    // Read for what it says: the LSPC's port and the BIOS's call by name, the vector table naming its handlers, the header's fields as data with their names,
    // the callbacks' jumps in the header.
    const std::string text = golden("neogeo.golden.asm");
    for (const char* line :
         {"REG_VRAMADDR         equ $3C0000", "CREDIT_CHECK         equ $C00450", "        org $0", "        dc.l $0010F300  ; initial stack pointer",
          "        dc.l $00C00402  ; reset", "        dc.l vblank  ; vblank", "        dc.b \"NEO-GEO\"  ; magic", "        dc.w $0123  ; NGH number",
          "        dc.l $00000200  ; Japan settings", "        JMP (user).L", "        JMP (demo_end).L", "user:", "        JSR (CREDIT_CHECK).L", "        MOVE.W #$7,(REG_VRAMADDR).L",
          "security:", "player_start:", "demo_end:", "        BSR.W user", "vblank:", "        RTE"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    std::size_t longs = 0;
    for (const std::string_view line : text::lines(text)) longs += line.starts_with("        dc.l ") ? 1 : 0;
    CB_CHECK_EQ(longs, 64u + 6u);   // the vector table, and the header's program size, backup RAM, settings (three) and security code
});

CB_TEST("source.neogeo_writer.a_word_swapped_prom_is_written_from_the_68000s_order", [] {
    fs::ScopedTempDir dir("cb_ng");
    const Bytes plain = prom();
    const image_writer::Written r = write(formats::neogeo::parse(formats::neogeo::swap_words(plain)), "swapped.p1", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(r.built);   // (a failed rebuild fails here, not in the read below)
    CB_CHECK(fs::read_file(*r.built) == plain);   // what it builds back is the P-ROM in the 68000's order
    CB_CHECK(fs::read_text(r.source).find("(P-ROM, words swapped)") != std::string::npos);
    CB_CHECK_EQ(r.functions.size(), 5u);
});

CB_TEST("source.neogeo_writer.the_second_megabyte_is_written_at_the_bank_window", [] {
    fs::ScopedTempDir dir("cb_ng");
    Bytes big = prom(0x180000);
    for (std::size_t i = 0x100000; i < 0x100040; ++i) big[i] = static_cast<std::uint8_t>(i * 5);   // data in the bank
    const image_writer::Written r = write(formats::neogeo::parse(big), "big-p1.p1", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK_EQ(r.image_bytes, 0x180000u);
    CB_CHECK(fs::read_text(r.source).find("        org $200000") != std::string::npos);
    CB_CHECK(r.built);   // (a failed rebuild fails here, not in the read below)
    CB_CHECK(fs::read_file(*r.built) == big);
});

CB_TEST("source.neogeo_writer.banks_past_the_second_megabyte_are_said_not_to_be_written", [] {
    fs::ScopedTempDir dir("cb_ng");
    const image_writer::Written r = write(formats::neogeo::parse(prom(0x280000)), "huge-p1.p1", dir.path());
    CB_CHECK(!r.same);
    CB_CHECK_EQ(r.notes.size(), 1u);
    CB_CHECK(r.notes[0].find("banks after the second megabyte") != std::string::npos);
});

CB_TEST("source.neogeo_writer.a_shared_handler_is_the_catch_all_and_a_missing_callback_is_data", [] {
    Bytes rom = prom();
    put_long(rom, 4 * 2, 0x500);   // bus error, address error and the vblank all go where vblank goes
    put_long(rom, 4 * 3, 0x500);
    put_bytes(rom, 0x128, {0, 0, 0, 0, 0, 0});   // no player_start
    fs::ScopedTempDir dir("cb_ng");
    const image_writer::Written r = write(formats::neogeo::parse(rom), "shared-p1.p1", dir.path());
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.at(0x500), "exception");
    CB_CHECK(!r.functions.contains(0x410) || r.functions.at(0x410) != "player_start");
    CB_CHECK(fs::read_text(r.source).find("        dc.l exception  ; vblank") != std::string::npos);
});

CB_TEST("source.neogeo_writer.a_file_with_no_header_is_refused", [] {
    fs::ScopedTempDir dir("cb_ng");
    fs::write_file(dir.path() / "short.p1", Bytes(0x100, 0));
    fs::write_file(dir.path() / "none.p1", Bytes(0x400, 0));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "short.p1", dir.path() / "out"));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "none.p1", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "missing.p1", dir.path() / "out"));
});

CB_TEST("source.neogeo_writer.reconstruct_reads_the_file", [] {
    fs::ScopedTempDir dir("cb_ng");
    fs::write_file(dir.path() / "game-p1.p1", prom());
    const image_writer::Written r = reconstruct(dir.path() / "game-p1.p1", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game-p1.asm");
});

// A Z80 sound program: $0000 DI ; JP $0100     $0038 EI ; RETI     $0066 RETN     $0100 IN A,(0) ; OUT ($0C),A ; LD A,7 ; OUT (4),A ; RET
Bytes sound_program() {
    Bytes m(0x400, 0xFF);
    put_bytes(m, 0x0000, {0xF3, 0xC3, 0x00, 0x01});
    put_bytes(m, 0x0038, {0xFB, 0xED, 0x4D});
    put_bytes(m, 0x0066, {0xED, 0x45});
    put_bytes(m, 0x0100, {0xDB, 0x00, 0xD3, 0x0C, 0x3E, 0x07, 0xD3, 0x04, 0xC9});
    return m;
}

CB_TEST("source.neogeo_writer.the_sound_program_is_written_with_the_sound_ports_by_name", [] {
    fs::ScopedTempDir dir("cb_ng");
    const Bytes m = sound_program();
    const image_writer::Written r = write_sound(m, "game-m1.m1", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x0000, "reset"}, {0x0038, "irq"}, {0x0066, "nmi"}}));
    CB_CHECK(r.source == dir.path() / "game-m1.m1.asm");
    const std::string text = fs::read_text(r.source);
    for (const char* name : {"SOUND_COMMAND", "SOUND_REPLY", "YM2610_ADDR_A"}) CB_CHECK(text.find(name) != std::string::npos);
    // a program that is more than the fixed 32 KB is written for the first 32 KB and says so
    Bytes big = m;
    big.resize(0x20000, 0);
    const image_writer::Written banked = write_sound(big, "big.m1", dir.path());
    CB_CHECK(!banked.same);
    CB_CHECK_EQ(banked.notes.size(), 1u);
});

CB_TEST("source.neogeo_writer.a_neo_container_is_written_with_its_sound_program_beside_it", [] {
    fs::ScopedTempDir dir("cb_ng");
    Bytes file(4096, 0);
    put_bytes(file, 0, {'N', 'E', 'O', 1});
    const Bytes p = prom();
    const Bytes m = sound_program();
    const auto le32 = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) file[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    le32(4, static_cast<std::uint32_t>(p.size()));
    le32(12, static_cast<std::uint32_t>(m.size()));
    file.insert(file.end(), p.begin(), p.end());
    file.insert(file.end(), m.begin(), m.end());
    fs::write_file(dir.path() / "game.neo", file);
    const image_writer::Written r = reconstruct(dir.path() / "game.neo", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "out" / "game.asm"));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "out" / "game.m1.asm"));
    // a raw M-ROM file on its own
    fs::write_file(dir.path() / "driver.m1", m);
    CB_CHECK(reconstruct_sound(dir.path() / "driver.m1", dir.path() / "out").same);
});

CB_TEST("source.neogeo_writer.the_registers_and_bios_calls_are_the_documented_ones", [] {
    CB_CHECK_EQ(hardware().at(0x3C0002), "REG_VRAMRW");
    CB_CHECK_EQ(hardware().at(0x3C0006), "REG_LSPCMODE");
    CB_CHECK_EQ(hardware().at(0xC0044A), "SYSTEM_IO");
    CB_CHECK_EQ(hardware().at(0xC004C2), "FIX_CLEAR");
    CB_CHECK_EQ(hardware().at(0x10FDAE), "BIOS_USER_REQUEST");
});

}  // namespace
}  // namespace cb::source::neogeo_writer
