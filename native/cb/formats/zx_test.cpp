#include "cb/formats/zx.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::zx {
namespace {

void le16(Bytes& b, std::size_t at, unsigned v) {
    b[at] = static_cast<std::uint8_t>(v & 0xFF);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

// ---- .sna ----------------------------------------------------------------------------------------------------

// A 48K .sna: the 27-byte header, then 48 KB of RAM where byte n of RAM is (n & 0xFF) except what the test sets.
Bytes sna48(std::uint16_t sp, std::uint16_t pc_on_stack, std::uint8_t im = 1, std::uint8_t i = 0x3F) {
    Bytes b(27 + 0xC000);
    for (std::size_t n = 0; n < 0xC000; ++n) b[27 + n] = static_cast<std::uint8_t>(n & 0xFF);
    b[0] = i;
    for (std::size_t k = 1; k <= 14; ++k) b[k] = static_cast<std::uint8_t>(0x10 + k);   // l' h' e' d' c' b' f' a' l h e d c b
    le16(b, 15, 0xAAAA);    // iy
    le16(b, 17, 0xBBBB);    // ix
    b[20] = 0x77;           // r
    b[21] = 0x66;           // f
    b[22] = 0x55;           // a
    le16(b, 23, sp);
    b[25] = im;
    if (sp >= 0x4000) le16(b, 27 + sp - 0x4000, pc_on_stack);
    return b;
}

CB_TEST("formats.zx.sna48_pops_the_pc_off_the_stack", [] {
    const Program p = parse(sna48(0x8000, 0x8123), Format::Sna);
    CB_CHECK(p.kind == Kind::Snapshot);
    CB_CHECK(p.registers.has_value());
    const Registers& r = *p.registers;
    CB_CHECK_EQ(r.sp, 0x8002);          // RETN pops it
    CB_CHECK_EQ(r.i, 0x3F);
    CB_CHECK_EQ(r.ix, 0xBBBB);
    CB_CHECK_EQ(r.iy, 0xAAAA);
    CB_CHECK_EQ(r.r, 0x77);
    CB_CHECK_EQ(r.f, 0x66);
    CB_CHECK_EQ(r.a, 0x55);
    CB_CHECK_EQ(r.l, 0x19);
    CB_CHECK_EQ(r.b, 0x1E);
    CB_CHECK(r.shadow.has_value());
    CB_CHECK_EQ(r.shadow->l, 0x11);
    CB_CHECK_EQ(r.shadow->a, 0x18);
    CB_CHECK_EQ(r.im, 1);
    CB_CHECK_EQ(p.memory.size(), kMemorySize);
    CB_CHECK_EQ(p.memory[0x4000], 0x00);
    CB_CHECK_EQ(p.memory[0x4001], 0x01);
    CB_CHECK_EQ(p.memory[0x0100], 0);   // ROM area stays empty
    CB_CHECK_EQ(p.origin, kProgramStart);
    CB_CHECK_EQ(p.image.size(), kMemorySize - kProgramStart);
    CB_CHECK_EQ(p.image.front(), p.memory[kProgramStart]);
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 0x8123);
    CB_CHECK_EQ(p.entries[0].name, "start");
});

CB_TEST("formats.zx.sna128_has_the_pc_after_the_ram", [] {
    Bytes b = sna48(0x8000, 0x1111);
    b.insert(b.end(), {0x34, 0x92, 0x10, 0x00});   // pc $9234, port $7FFD, TR-DOS flag
    const Program p = parse(b, Format::Sna);
    CB_CHECK_EQ(p.registers->sp, 0x8000);        // nothing popped
    CB_CHECK_EQ(p.entries[0].address, 0x9234);
});

CB_TEST("formats.zx.a_pc_below_the_program_block_is_no_entry", [] {
    const Program p = parse(sna48(0x8000, 0x4000), Format::Sna);   // pc in the screen
    CB_CHECK(p.entries.empty());
});

CB_TEST("formats.zx.im2_handler_is_an_entry", [] {
    Bytes b = sna48(0x8000, 0x8123, /*im=*/2, /*i=*/0x80);
    le16(b, 27 + 0x80FF - 0x4000, 0x9000);   // the table's last entry, read at I*256 + $FF
    const Program p = parse(b, Format::Sna);
    CB_CHECK_EQ(p.entries.size(), 2u);
    CB_CHECK_EQ(p.entries[1].address, 0x9000);
    CB_CHECK_EQ(p.entries[1].name, "interrupt");
    // the same address as the start keeps the name "start"
    le16(b, 27 + 0x80FF - 0x4000, 0x8123);
    CB_CHECK_EQ(parse(b, Format::Sna).entries.size(), 1u);
});

CB_TEST("formats.zx.sna_too_short", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(27 + 0xC000 - 1), Format::Sna));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}, Format::Sna));
});

// ---- .z80 ----------------------------------------------------------------------------------------------------

Bytes z80_header(std::uint16_t pc, std::uint8_t flags12 = 0) {
    Bytes h(30);
    h[0] = 0xA1;  // a
    h[1] = 0xF1;  // f
    h[2] = 0xC1;  // c
    h[3] = 0xB1;  // b
    h[4] = 0x11;  // l
    h[5] = 0x22;  // h
    le16(h, 6, pc);
    le16(h, 8, 0xFF00);  // sp
    h[10] = 0x3F;        // i
    h[11] = 0x05;        // r low
    h[12] = flags12;
    h[13] = 0xE1;        // e
    h[14] = 0xD1;        // d
    le16(h, 23, 0x1234); // iy
    le16(h, 25, 0x5678); // ix
    h[29] = 0x06;        // im: 6 & 3 = 2
    return h;
}

CB_TEST("formats.zx.z80_v1_uncompressed", [] {
    Bytes b = z80_header(0x8000, 0x00);
    for (std::size_t n = 0; n < 0xC000; ++n) b.push_back(static_cast<std::uint8_t>(n ^ 0x5A));
    const Program p = parse(b, Format::Z80);
    const Registers& r = *p.registers;
    CB_CHECK_EQ(r.a, 0xA1);
    CB_CHECK_EQ(r.f, 0xF1);
    CB_CHECK_EQ(r.b, 0xB1);
    CB_CHECK_EQ(r.c, 0xC1);
    CB_CHECK_EQ(r.d, 0xD1);
    CB_CHECK_EQ(r.e, 0xE1);
    CB_CHECK_EQ(r.h, 0x22);
    CB_CHECK_EQ(r.l, 0x11);
    CB_CHECK_EQ(r.sp, 0xFF00);
    CB_CHECK_EQ(r.i, 0x3F);
    CB_CHECK_EQ(r.r, 0x05);
    CB_CHECK_EQ(r.ix, 0x5678);
    CB_CHECK_EQ(r.iy, 0x1234);
    CB_CHECK_EQ(r.im, 2);
    CB_CHECK(!r.shadow.has_value());
    CB_CHECK_EQ(p.memory[0x4000], 0x5A);
    CB_CHECK_EQ(p.memory[0xFFFF], static_cast<std::uint8_t>(0xBFFF ^ 0x5A));
    CB_CHECK_EQ(p.entries[0].address, 0x8000);
});

CB_TEST("formats.zx.z80_r_register_takes_bit_7_from_byte_12", [] {
    Bytes b = z80_header(0x8000, 0x01);   // bit 0: R's bit 7 (and not compressed)
    b.insert(b.end(), 0xC000, 0);
    CB_CHECK_EQ(parse(b, Format::Z80).registers->r, 0x85);
});

CB_TEST("formats.zx.z80_v1_compressed_with_the_end_marker", [] {
    Bytes b = z80_header(0x8000, 0x20);
    // ED ED 0x10 0x07: sixteen sevens; a lone ED is a literal; a run covers the rest of RAM
    b.insert(b.end(), {0xED, 0xED, 0x10, 0x07, 0xED, 0x41});
    b.insert(b.end(), {0xED, 0xED, 0xFF, 0x00});   // 255 zeros
    b.insert(b.end(), {0x00, 0xED, 0xED, 0x00});   // the end marker
    const Program p = parse(b, Format::Z80);
    CB_CHECK_EQ(p.memory[0x4000], 7);
    CB_CHECK_EQ(p.memory[0x400F], 7);
    CB_CHECK_EQ(p.memory[0x4010], 0xED);
    CB_CHECK_EQ(p.memory[0x4011], 0x41);
    CB_CHECK_EQ(p.memory[0x4012], 0);
});

CB_TEST("formats.zx.z80_a_truncated_run_is_an_error", [] {
    Bytes b = z80_header(0x8000, 0x20);
    b.insert(b.end(), {0x01, 0xED, 0xED, 0x05});   // ED ED n with no byte after n
    CB_CHECK_THROWS_AS(TruncatedError, (void)parse(b, Format::Z80));
});

CB_TEST("formats.zx.z80_header_too_short", [] {
    CB_CHECK_THROWS_AS(TruncatedError, (void)parse(Bytes(29), Format::Z80));
    CB_CHECK_THROWS_AS(TruncatedError, (void)parse(z80_header(0), Format::Z80));   // v2/v3: the extra header is missing
});

// A v2/v3 file: pc = 0 in the base header, a 23-byte extra header, then pages (length, page, data).
Bytes z80_v2(std::uint8_t hardware, std::uint8_t port7ffd, std::uint16_t pc) {
    Bytes b = z80_header(0, 0);
    b.resize(30 + 2 + 23);
    le16(b, 30, 23);
    le16(b, 32, pc);
    b[34] = hardware;
    b[35] = port7ffd;
    return b;
}

// One page: length, page number, data. Compressed: a single run (ED ED FF fill) of 255 bytes.
void add_page(Bytes& b, std::uint8_t page, std::uint8_t fill, bool compressed) {
    if (compressed) {
        b.insert(b.end(), {0x04, 0x00, page, 0xED, 0xED, 0xFF, fill});
    } else {
        b.insert(b.end(), {0xFF, 0xFF, page});   // length $FFFF: 16 KB stored as it is
        b.insert(b.end(), 0x4000, fill);
    }
}

CB_TEST("formats.zx.z80_v2_48k_pages", [] {
    Bytes b = z80_v2(/*hardware=*/0, 0, 0x8000);
    add_page(b, 8, 0x81, false);   // $4000
    add_page(b, 4, 0x82, false);   // $8000
    add_page(b, 5, 0x83, true);    // $C000, compressed: 255 bytes then nothing
    const Program p = parse(b, Format::Z80);
    CB_CHECK_EQ(p.memory[0x4000], 0x81);
    CB_CHECK_EQ(p.memory[0x7FFF], 0x81);
    CB_CHECK_EQ(p.memory[0x8000], 0x82);
    CB_CHECK_EQ(p.memory[0xC000], 0x83);
    CB_CHECK_EQ(p.memory[0xC0FE], 0x83);
    CB_CHECK_EQ(p.memory[0xC0FF], 0);
    CB_CHECK_EQ(p.entries[0].address, 0x8000);
});

CB_TEST("formats.zx.z80_v2_128k_maps_the_paged_bank_at_c000", [] {
    Bytes b = z80_v2(/*hardware=*/3, /*port 7FFD: bank 0 paged in*/ 0x00, 0x8000);
    add_page(b, 8, 0x91, false);    // bank 5 -> $4000
    add_page(b, 5, 0x92, false);    // bank 2 -> $8000
    add_page(b, 3, 0x93, false);    // bank 0 -> $C000
    const Program p = parse(b, Format::Z80);
    CB_CHECK_EQ(p.memory[0x4000], 0x91);
    CB_CHECK_EQ(p.memory[0x8000], 0x92);
    CB_CHECK_EQ(p.memory[0xC000], 0x93);
});

CB_TEST("formats.zx.z80_v2_128k_with_bank_5_paged_in_shows_it_twice", [] {
    // Bank 5 is fixed at $4000 and, when also paged in, appears at $C000 as well: the same bytes in both places.
    Bytes b = z80_v2(/*hardware=*/4, /*bank 5 paged*/ 0x05, 0x8000);
    add_page(b, 8, 0x91, false);
    add_page(b, 5, 0x92, false);
    const Program p = parse(b, Format::Z80);
    CB_CHECK_EQ(p.memory[0x4000], 0x91);
    CB_CHECK_EQ(p.memory[0x8000], 0x92);
    CB_CHECK_EQ(p.memory[0xC000], 0x91);
    // and bank 2 paged in: it shows at $8000 and $C000
    Bytes c = z80_v2(4, 0x02, 0x8000);
    add_page(c, 8, 0x91, false);
    add_page(c, 5, 0x92, false);
    const Program q = parse(c, Format::Z80);
    CB_CHECK_EQ(q.memory[0x8000], 0x92);
    CB_CHECK_EQ(q.memory[0xC000], 0x92);
});

CB_TEST("formats.zx.z80_hardware_64_is_not_128k", [] {
    Bytes b = z80_v2(/*hardware=*/64, 0x00, 0x8000);   // 48K-compatible hardware code
    add_page(b, 8, 0x81, false);
    add_page(b, 4, 0x82, false);
    CB_CHECK_EQ(parse(b, Format::Z80).memory[0x8000], 0x82);
});

CB_TEST("formats.zx.z80_a_cut_off_last_page_is_what_is_there", [] {
    Bytes b = z80_v2(0, 0, 0x8000);
    add_page(b, 8, 0x81, false);
    b.resize(b.size() - 100);
    const Program p = parse(b, Format::Z80);
    CB_CHECK_EQ(p.memory[0x4000], 0x81);
    CB_CHECK_EQ(p.memory[0x4000 + 0x4000 - 101], 0x81);
    CB_CHECK_EQ(p.memory[0x4000 + 0x4000 - 100], 0);
});

// ---- .tap ----------------------------------------------------------------------------------------------------

void add_block(Bytes& tape, const Bytes& block) {
    tape.push_back(static_cast<std::uint8_t>(block.size() & 0xFF));
    tape.push_back(static_cast<std::uint8_t>(block.size() >> 8));
    tape.insert(tape.end(), block.begin(), block.end());
}

// A header block: flag 00, type, name, length, param1, param2, checksum (19 bytes).
Bytes tape_header(std::uint8_t type, std::uint16_t length, std::uint16_t p1) {
    Bytes h(19, 0x20);
    h[0] = 0x00;
    h[1] = type;
    le16(h, 12, length);
    le16(h, 14, p1);
    le16(h, 16, 0x8000);
    h[18] = 0;
    return h;
}

Bytes data_block(const Bytes& payload) {
    Bytes b = {0xFF};
    b.insert(b.end(), payload.begin(), payload.end());
    b.push_back(0);   // checksum
    return b;
}

// BASIC: `RANDOMIZE USR n` -- the token $C0, the number as text, $0E and the five-byte form (00 00 lo hi 00).
Bytes basic_usr(std::uint16_t n) {
    Bytes b = {0xC0};
    for (char c : std::to_string(n)) b.push_back(static_cast<std::uint8_t>(c));
    b.insert(b.end(), {0x0E, 0x00, 0x00, static_cast<std::uint8_t>(n & 0xFF), static_cast<std::uint8_t>(n >> 8), 0x00, 0x0D});
    return b;
}

CB_TEST("formats.zx.tape_with_a_basic_loader", [] {
    Bytes tape;
    const Bytes basic = basic_usr(32768);
    add_block(tape, tape_header(0, static_cast<std::uint16_t>(basic.size()), 10));
    add_block(tape, data_block(basic));
    const Bytes code(100, 0xC9);
    add_block(tape, tape_header(3, 100, 32768));
    add_block(tape, data_block(code));
    const Program p = parse(tape, Format::Tap);
    CB_CHECK(p.kind == Kind::Tape);
    CB_CHECK(!p.registers.has_value());
    CB_CHECK_EQ(p.origin, 32768u);
    CB_CHECK_EQ(p.image.size(), 100u);
    CB_CHECK_EQ(p.memory.size(), kMemorySize);
    CB_CHECK_EQ(p.memory[32768], 0xC9);
    CB_CHECK_EQ(p.memory[32868], 0);
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 32768);
    CB_CHECK_EQ(p.entries[0].name, "start");
});

CB_TEST("formats.zx.tape_usr_calls_name_the_entries", [] {
    Bytes tape;
    Bytes basic = basic_usr(32768);
    const std::uint16_t calls[] = {32800, 32768, 40000 /* outside the block */, 32830};
    for (std::uint16_t n : calls) {
        const Bytes more = basic_usr(n);
        basic.insert(basic.end(), more.begin(), more.end());
    }
    add_block(tape, tape_header(0, static_cast<std::uint16_t>(basic.size()), 10));
    add_block(tape, data_block(basic));
    add_block(tape, tape_header(3, 100, 32768));
    add_block(tape, data_block(Bytes(100, 0)));
    const Program p = parse(tape, Format::Tap);
    // The first call is "start". A later call of the same address does not rename it (a loader that calls USR 32768
    // twice still starts there).
    CB_CHECK_EQ(p.entries.size(), 3u);
    CB_CHECK_EQ(p.entries[0].address, 32768);
    CB_CHECK_EQ(p.entries[0].name, "start");
    CB_CHECK_EQ(p.entries[1].address, 32800);
    CB_CHECK_EQ(p.entries[1].name, "usr_8020");
    CB_CHECK_EQ(p.entries[2].address, 32830);
    CB_CHECK_EQ(p.entries[2].name, "usr_803E");
});

CB_TEST("formats.zx.tape_without_basic_starts_at_the_code", [] {
    Bytes tape;
    add_block(tape, tape_header(3, 10, 30000));
    add_block(tape, data_block(Bytes(10, 1)));
    const Program p = parse(tape, Format::Tap);
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 30000);
    CB_CHECK_EQ(p.entries[0].name, "start");
});

CB_TEST("formats.zx.tape_takes_the_biggest_code_block", [] {
    Bytes tape;
    add_block(tape, tape_header(3, 10, 30000));
    add_block(tape, data_block(Bytes(10, 1)));
    add_block(tape, tape_header(3, 50, 40000));
    add_block(tape, data_block(Bytes(50, 2)));
    add_block(tape, tape_header(3, 20, 50000));
    add_block(tape, data_block(Bytes(20, 3)));
    const Program p = parse(tape, Format::Tap);
    CB_CHECK_EQ(p.origin, 40000u);
    CB_CHECK_EQ(p.image.size(), 50u);
    CB_CHECK_EQ(p.memory[40000], 2);
});

CB_TEST("formats.zx.tape_code_that_runs_off_the_end_of_memory_is_cut", [] {
    Bytes tape;
    add_block(tape, tape_header(3, 100, 65500));
    add_block(tape, data_block(Bytes(100, 7)));
    const Program p = parse(tape, Format::Tap);
    CB_CHECK_EQ(p.image.size(), 36u);
    CB_CHECK_EQ(p.memory[0xFFFF], 7);
});

CB_TEST("formats.zx.tape_errors", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}, Format::Tap));            // no blocks, no code
    Bytes basic_only;
    add_block(basic_only, tape_header(0, 4, 10));
    add_block(basic_only, data_block(Bytes(4, 0x20)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(basic_only, Format::Tap));
    // a data block with no header before it is ignored
    Bytes orphan;
    add_block(orphan, data_block(Bytes(10, 1)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(orphan, Format::Tap));
});

// ---- files ---------------------------------------------------------------------------------------------------

CB_TEST("formats.zx.format_follows_the_extension", [] {
    CB_CHECK(format_of("a.SNA") == Format::Sna);
    CB_CHECK(format_of("dir/a.z80") == Format::Z80);
    CB_CHECK(format_of("a.tap") == Format::Tap);
    CB_CHECK(!format_of("a.tzx").has_value());
    CB_CHECK(!format_of("a").has_value());
});

CB_TEST("formats.zx.read_and_probe_a_file", [] {
    fs::ScopedTempDir dir("zx-test");
    fs::write_file(dir.path() / "game.sna", sna48(0x8000, 0x8123));
    fs::write_file(dir.path() / "broken.sna", Bytes(100));
    fs::write_file(dir.path() / "game.bin", sna48(0x8000, 0x8123));
    CB_CHECK(probe(dir.path() / "game.sna"));
    CB_CHECK(!probe(dir.path() / "broken.sna"));
    CB_CHECK(!probe(dir.path() / "game.bin"));
    CB_CHECK(!probe(dir.path() / "missing.sna"));
    CB_CHECK_EQ(read(dir.path() / "game.sna").entries[0].address, 0x8123);
    CB_CHECK_THROWS_AS(UnsupportedError, (void)read(dir.path() / "game.bin"));
    CB_CHECK_THROWS_AS(FormatError, (void)read(dir.path() / "broken.sna"));
    CB_CHECK(!try_parse(Bytes(100), Format::Sna).ok());
});

}  // namespace
}  // namespace cb::formats::zx
