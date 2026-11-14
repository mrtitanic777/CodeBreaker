// Tests of the Game Boy assembler: the opcodes of the instructions the Game Boy lane writes (bytes checked against the
// Game Boy CPU manual), the directives (SECTION and its banks, db / dw / ds, INCBIN, EQU), labels and the two passes,
// comments and line ends, every way a line is refused, and the 102 sources an old harness was run on (the demo program at three ROM sizes, every directive and instruction form,
// and the broken ones), with what it made -- the ROM or the line it refused.
//
// Notes (the cases of testdata/sm83_asm/cases.json marked "diverges", and the notes below):
//   * A line is ended by \n, \r\n or a lone \r. Other Unicode line separators (, , -, U+0085, U+2028, U+2029; case 72) do not end a line, and only ASCII spaces
//     are stripped (case 100: U+2003 around an instruction). Neither is source anyone writes.
//   * An expression that is wrong -- a division by zero, `7//2`, `(1, 2)` as an operand -- is an AsmError naming the line
//     (cases 66, 67, 68, 77).  `db (1, 2)` is refused too (case 77).
//   * An INCBIN file that is not there is an IoError (NotFoundError).
//   * The characters of a db string are written as Latin-1 (a character beyond it becomes `?`). Text is otherwise taken as the bytes it is.
//   * A `ds` count past the ROM is refused at once (a count of a few billion never makes the bytes).
//   * The messages are the AsmError's own: the line is its line() and the text of the line follows.
#include "cb/build/sm83_asm.hpp"

#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace stdfs = std::filesystem;
using cb::Bytes;
using cb::build::AsmError;
namespace gb = cb::build::sm83_asm;

constexpr std::size_t kSmall = 0x100;

Bytes rom(std::string_view source, std::size_t size = kSmall, const stdfs::path& folder = ".") { return gb::build(source, size, folder); }

std::string hex(const Bytes& b, std::size_t from = 0, std::size_t count = std::string::npos) {
    std::string out;
    for (std::size_t i = from; i < b.size() && i - from < count; ++i) out += std::format("{:02x}", b[i]);
    return out;
}

// The bytes at [from, from + count) of the ROM `source` makes, as hex.
std::string at(std::string_view source, std::size_t from, std::size_t count, std::size_t size = kSmall) {
    return hex(rom(source, size), from, count);
}

// The ROM from its start up to the last byte that is not $FF (a test names what was written, not the fill).
std::string written(std::string_view source, std::size_t size = kSmall) {
    Bytes b = rom(source, size);
    while (!b.empty() && b.back() == 0xFF) b.pop_back();
    return hex(b);
}

stdfs::path data_folder() { return cb::testing::data_path("build/testdata/sm83_asm/bin"); }

// What a refused source says: the AsmError's line, or 0 when it was not refused (or by something else).
std::uint32_t refused_at(std::string_view source, std::size_t size = kSmall) {
    try {
        (void)rom(source, size);
    } catch (const AsmError& e) {
        return e.line();
    }
    return 0;
}

constexpr std::string_view kRom0 = "SECTION \"a\", ROM0[$0000]\n";

}  // namespace

// ---- the instructions --------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.the_simple_instructions_have_their_opcodes", [] {
    // nop, halt, stop (two bytes: 10 00), ret, reti, di, ei
    CB_CHECK_EQ(written(std::string(kRom0) + "nop\nhalt\nstop\nret\nreti\ndi\nei"), std::string("0076") + "1000" + "c9d9f3fb");
});

CB_TEST("build.sm83_asm.loads_have_their_opcodes", [] {
    // ld a,b = 78; ld b,a = 47; ld (hl),a = 77; ld a,(hl) = 7E; ld a,(hl+) = 2A; ld (hl+),a = 22; ld a,(hl-) = 3A; ld (hl-),a = 32
    CB_CHECK_EQ(written(std::string(kRom0) + " ld a,b\n ld b,a\n ld (hl),a\n ld a,(hl)\n ld a,(hl+)\n ld (hli),a\n ld a,(hld)\n ld (hl-),a"),
                std::string("78") + "47" + "77" + "7e" + "2a" + "22" + "3a" + "32");
});

CB_TEST("build.sm83_asm.immediate_and_bit_instructions_have_their_opcodes", [] {
    // ld a,$12 = 3E 12; ld hl,$1234 = 21 34 12; ld sp,$C000 = 31 00 C0; add a,$12 = C6 12; and $0F = E6 0F; cp $20 = FE 20;
    // add sp,-2 = E8 FE; ld hl,sp+5 = F8 05
    CB_CHECK_EQ(written(std::string(kRom0) + " ld a,$12\n ld hl,$1234\n ld sp,$C000\n add a,$12\n and $0F\n cp $20\n add sp,-2\n ld hl,sp+5"),
                std::string("3e12") + "213412" + "3100c0" + "c612" + "e60f" + "fe20" + "e8fe" + "f805");
    // bit 3,a = CB 5F; set 7,(hl) = CB FE; res 0,b = CB 80; swap a = CB 37; rst $38 = FF; push bc = C5; pop af = F1
    CB_CHECK_EQ(written(std::string(kRom0) + " bit 3,a\n set 7,(hl)\n res 0,b\n swap a\n rst $38\n push bc\n pop af"),
                std::string("cb5f") + "cbfe" + "cb80" + "cb37" + "ff" + "c5" + "f1");
});

CB_TEST("build.sm83_asm.the_high_page_and_the_absolute_loads_have_their_opcodes", [] {
    // ldh ($FF40),a = E0 40; ldh a,($FF80) = F0 80; ld ($C000),sp = 08 00 C0; ld ($D000),a = EA 00 D0; ld a,($D000) = FA 00 D0;
    // ld (c),a = E2; ld a,(c) = F2
    CB_CHECK_EQ(written(std::string(kRom0) + " ldh ($FF40),a\n ldh a,($FF80)\n ld ($C000),sp\n ld ($D000),a\n ld a,($D000)\n ld (c),a\n ld a,(c)"),
                std::string("e040") + "f080" + "0800c0" + "ea00d0" + "fa00d0" + "e2" + "f2");
});

CB_TEST("build.sm83_asm.instructions_are_written_in_any_case_and_spacing", [] {
    CB_CHECK_EQ(written(std::string(kRom0) + "NOP\nLd C , D\n  Ld   a,\t$12 \n"), std::string("00") + "4a" + "3e12");
});

// ---- labels and the two passes --------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.a_label_is_the_address_of_the_next_byte", [] {
    // at $0010: label: nop; ld a,label (3E 10); ld hl,label (21 10 00); jp label (C3 10 00); call label (CD 10 00)
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0010]\nlabel: nop\nld a,label\nld hl,label\njp label\ncall label", 0x10, 12, 0x40),
                std::string("00") + "3e10" + "211000" + "c31000" + "cd1000");
});

CB_TEST("build.sm83_asm.a_label_may_be_used_before_it_is_defined", [] {
    // jp fwd (C3 16 00) at $10, call fwd (CD 16 00) at $13, fwd: ret (C9) at $16
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0010]\nstart:\n  jp fwd\n  call fwd\nfwd: ret", 0x10, 7, 0x40), std::string("c31600") + "cd1600" + "c9");
    // relative jumps both ways: jr fwd (18 01) skips a nop; jr nz,fwd (20 FD) goes back 3; jr c,back (38 00)
    CB_CHECK_EQ(at(std::string(kRom0) + " jr fwd\n nop\nfwd: nop\n jr nz,fwd\n jr c,back\nback: nop", 0, 9),
                std::string("1801") + "00" + "00" + "20fd" + "3800" + "00");
    // a constant defined after its use: ldh (fwd),a with fwd EQU $FF44 is E0 44
    CB_CHECK_EQ(at(std::string(kRom0) + " ldh (fwd),a\n ldh a,(fwd)\nfwd EQU $FF44\n ldh ($FF40),a", 0, 6), std::string("e044") + "f044" + "e040");
    // an instruction that names a label further on is sized right: the second one is at $0003
    CB_CHECK_EQ(at(std::string(kRom0) + " ld a,(fwd)\n ld a,1\nfwd EQU $C000", 0, 5), std::string("fa00c0") + "3e01");
});

CB_TEST("build.sm83_asm.local_labels_and_double_colons", [] {
    // start:: at 0; jr start (18 FE); .loc: at 2; jr .loc (18 FE); next:: nop at 4
    CB_CHECK_EQ(at(std::string(kRom0) + "start::\n  jr start\n.loc: jr .loc\nnext:: nop", 0, 5), std::string("18fe") + "18fe" + "00");
    // a label alone on its line, and one before an instruction
    CB_CHECK_EQ(at(std::string(kRom0) + "l1: nop\nl3:   \nl4:: ld a,l1\n ld b,l3\n ld c,l4", 0, 7), std::string("00") + "3e00" + "0601" + "0e01");
    // only one label to a line: the rest is taken as an instruction
    CB_CHECK_EQ(refused_at(std::string(kRom0) + "l1: l2: nop"), 2u);
});

CB_TEST("build.sm83_asm.a_label_defined_twice_has_the_value_last_defined_before_each_use", [] {
    // each definition sets the label as the pass reaches it: `ld a,L` sees L = 0, `ld b,L` sees L = 2
    CB_CHECK_EQ(at(std::string(kRom0) + "LBL:\n ld a,LBL\nLBL: nop\n ld b,LBL", 0, 5), std::string("3e00") + "00" + "0602");
});

// ---- constants -------------------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.constants_with_and_without_def", [] {
    // DEF X EQU 5; x2 EQU X+1 -> 6; ld a,X (3E 05); ld b,x2 (06 06); ld c,Y (0E 09) with DEF Y EQU 9 defined later
    CB_CHECK_EQ(at("DEF X EQU 5\nx2 EQU X+1\nSECTION \"a\", ROM0[$0]\n ld a,X\n ld b,x2\n ld c,Y\nDEF Y EQU 9", 0, 6), std::string("3e05") + "0606" + "0e09");
    // constants may be defined from later ones: a EQU b+1 with b EQU 3 gives a = 4
    CB_CHECK_EQ(at("a EQU b+1\nb EQU 3\nSECTION \"a\", ROM0[$0]\n db a,b", 0, 2), std::string("0403"));
    // a constant is usable as a section address and `$` is the address of the line
    CB_CHECK_EQ(at("NAME EQU 4\nSECTION \"a\", ROM0[NAME*2]\n db NAME", 8, 1, 0x20), std::string("04"));
    CB_CHECK_EQ(at("NAME EQU $\nSECTION \"a\", ROM0[$3]\nN2 EQU $\n db NAME,N2", 3, 2, 0x20), std::string("0003"));
});

// ---- data ----------------------------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.db_and_dw_take_numbers_labels_expressions_and_strings", [] {
    // db $12,$34,%1010,0x56,7Fh,'A',-1,300,256,255; dw $1234,$ABCD,-2,65536,70000 (the low 8 / 16 bits)
    CB_CHECK_EQ(written("SECTION \"a\", ROM0[$0]\n db $12,$34,%1010,0x56,7Fh,'A',-1,300,256,255\n dw $1234,$ABCD,-2,65536,70000"),
                std::string("12340a567f41ff2c00ff") + "3412" + "cdab" + "feff" + "0000" + "7011");
    // strings are written as they are, with `,` and `;` inside them; a string may be empty
    CB_CHECK_EQ(written("SECTION \"a\", ROM0[$0]\n db \"HELLO\", 0, \"a,b\", 1\n db \"\"\n db \"x\" , 2"), std::string("48454c4c4f00") + "612c62" + "01" + "78" + "02");
    CB_CHECK_EQ(written("SECTION \"a\", ROM0[$0]\n db 1 , 2 ,  3\n db ( 1 + 2 )"), std::string("010203") + "03");
    // expressions: ( ) * / & | ^ << >> ~ < > and a label further on
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n dw lbl,lbl+1\nlbl: db (1+2)*3, 10/3, 7&3, 1|6, 5^1, 1<<4, 256>>2, ~1", 0, 12),
                std::string("0400") + "0500" + "09" + "03" + "03" + "07" + "04" + "10" + "40" + "fe");
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db <$1234, >$1234, <lbl, >lbl\nlbl: nop", 0, 4), std::string("34120400"));
});

CB_TEST("build.sm83_asm.dollar_is_the_address_of_the_byte_being_written", [] {
    // db $ (00), dw $ (01 00 at 1), nop (3), db $ (04)
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db $\n dw $\n nop\n db $", 0, 5, 0x20), std::string("00") + "0100" + "00" + "04");
});

CB_TEST("build.sm83_asm.ds_reserves_bytes_and_a_negative_count_moves_back", [] {
    CB_CHECK_EQ(at(std::string(kRom0) + " ds 4\n ds 3,$AA\n ds 2,$1AB\n ds 0\n db 9", 0, 10), std::string("00000000") + "aaaaaa" + "abab" + "09");
    // `ds -3,1` writes nothing and the next byte goes three back: over the last of the $AA
    CB_CHECK_EQ(at(std::string(kRom0) + " ds 4\n ds 3,$AA\n ds 2,$1AB\n ds 0\n ds -3,1\n db 9", 0, 9), std::string("00000000") + "aaaa" + "09" + "abab");
    CB_CHECK_EQ(at(std::string(kRom0) + " ds 2\n db 1", 0, 3), std::string("000001"));   // (no value: zeros)
});

CB_TEST("build.sm83_asm.ds_needs_its_count_at_once", [] {
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds N, V\nN EQU 3\nV EQU 7"), 2u);   // (the first pass cannot know N)
});

CB_TEST("build.sm83_asm.what_falls_outside_the_rom_is_refused", [] {
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds 257"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds 1000"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds 4000000000000000000"), 2u);   // refused, not allocated
    CB_CHECK_EQ(rom(std::string(kRom0) + " ds 256").size(), std::size_t{256});      // exactly the ROM
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[$FF]\n db 1,2"), 2u);
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[$FE]\n ld a,1"), 0u);
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[$FF]\n ld a,1"), 2u);
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[-5]\n db 1"), 2u);   // before the ROM
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[$0]\n nop", 0), 2u);  // a ROM of no bytes holds nothing
    // the message says where: bank, offset
    try {
        (void)rom(std::string(kRom0) + "\n db 1\n ds 300");
        CB_CHECK(false);
    } catch (const AsmError& e) {
        CB_CHECK(std::string(e.what()).find("bytes outside the ROM (bank 0, offset $1)") != std::string::npos);
    }
});

CB_TEST("build.sm83_asm.incbin_reads_a_file_beside_the_source", [] {
    // data.bin is the bytes 1..39; the case-insensitive directive, a label before it, anything after the quote ignored
    const Bytes r = rom("SECTION \"a\", ROM0[$0]\nINCBIN \"data.bin\"\n db 1", kSmall, data_folder());
    CB_CHECK_EQ(hex(r, 0, 4), std::string("01020304"));
    CB_CHECK_EQ(r[38], std::uint8_t{39});
    CB_CHECK_EQ(r[39], std::uint8_t{1});   // what follows it
    const Bytes two = rom("SECTION \"a\", ROM0[$0]\nincbin \"data.bin\"\nlbl: incbin \"big.bin\" ; the file of $AB", kSmall + 0x100, data_folder());
    CB_CHECK_EQ(two[39], std::uint8_t{0xAB});
    CB_CHECK_EQ(two[39 + 299], std::uint8_t{0xAB});
});

CB_TEST("build.sm83_asm.incbin_errors", [] {
    CB_CHECK_THROWS_AS(cb::IoError, (void)rom("SECTION \"a\", ROM0[$0]\nINCBIN \"nothere.bin\"", kSmall, data_folder()));
    CB_CHECK_THROWS_AS(cb::IoError, (void)rom("SECTION \"a\", ROM0[$0]\nINCBIN \"data.bin\"", kSmall, data_folder() / "nodir"));
    // a file that does not fit is the line's fault: 300 bytes in a 256-byte ROM
    try {
        (void)gb::build("SECTION \"a\", ROM0[$0]\nINCBIN \"big.bin\"", kSmall, data_folder());
        CB_CHECK(false);
    } catch (const AsmError& e) {
        CB_CHECK_EQ(e.line(), 2u);
    }
    // the file name has to be in quotes
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROM0[$0]\nINCBIN data.bin"), 2u);
});

// ---- sections and banks -------------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.sections_put_bytes_at_their_addresses", [] {
    // a second section at $10: db 1 at 0, db 2 at 0x10
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db 1\nSECTION \"b\", ROM0[$10]\n db 2", 0, 17), std::string("01") + std::string(30, 'f') + "02");
    // two sections at one address: the later one overwrites
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db 1\nSECTION \"b\", ROM0[$0]\n db 2", 0, 1), std::string("02"));
});

CB_TEST("build.sm83_asm.banks_are_16k_each_and_seen_at_4000", [] {
    // bank 1 is at ROM offset $4000, bank 2 at $8000; ROMX defaults to bank 1; ROM0 with BANK[0] is bank 0
    const Bytes r = rom("SECTION \"a\", ROMX[$4000], BANK[1]\n db 1,2,3\nSECTION \"b\", ROMX[$4010], BANK[2]\n db 4\nSECTION \"c\", ROM0[$0]\n db 5", 0xC000);
    CB_CHECK_EQ(hex(r, 0x4000, 3), std::string("010203"));
    CB_CHECK_EQ(r[0x8000 + 0x10], std::uint8_t{4});   // bank 2, offset $10 into the window ($4010 - $4000)
    CB_CHECK_EQ(r[0], std::uint8_t{5});
    // `l1` is $4001 in a ROMX section: ld a,l1 is 3E 01 (the low byte)
    CB_CHECK_EQ(hex(rom("SECTION \"a\", ROMX[$4000]\n db 1\nl1: ld a,l1", 0x8000), 0x4000, 3), std::string("013e01"));
    // case, spacing, and the bank given by a constant
    CB_CHECK_EQ(rom("section \"a\", romx [ $4005 ] , bank [ 1 ] \n db 1", 0x8000)[0x4005], std::uint8_t{1});
    CB_CHECK_EQ(rom("BASE EQU $4000\nB EQU 1\nSECTION \"a\", ROMX[BASE], BANK[B]\n db 1", 0x8000)[0x4000], std::uint8_t{1});
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROMX[BASE], BANK[B]\nBASE EQU $4000\nB EQU 1\n db 1", 0x8000), 1u);   // (a section needs its numbers at once)
    CB_CHECK_EQ(rom("SECTION \"a\", ROMX[$4000],BANK[ 3 ]\nlp: jp lp\n db lp", 0x10000)[0xC000], std::uint8_t{0xC3});
    // a bank that does not fit the ROM
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROMX[$4000], BANK[2]\n db 1", 0x8000), 2u);
    // an address below the window is bank 1 at offset -$1000: the byte lands at $3000 of the ROM
    CB_CHECK_EQ(rom("SECTION \"a\", ROMX[$3000], BANK[1]\n db 1", 0x8000)[0x3000], std::uint8_t{1});
});

// ---- comments, line ends, case --------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.comments_and_blank_lines_are_nothing", [] {
    CB_CHECK_EQ(rom("").size(), kSmall);
    CB_CHECK_EQ(written("; just a comment\n\n   ; another\n"), std::string());
    CB_CHECK_EQ(written("\tSECTION \"a\", ROM0[$0]   ; sect\n\t\tnop ; one\n  db \"a;b\" ; two\n  db \";\""), std::string("00") + "613b62" + "3b");
});

CB_TEST("build.sm83_asm.line_ends_are_lf_crlf_or_cr", [] {
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\r\n nop\r\n db 1\r\nlp: jp lp\r\n", 0, 5, 0x20), std::string("00") + "01" + "c30200");   // lp is 2
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\r nop\r db 1\r", 0, 2), std::string("0001"));
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n nop\n db 1\n", 0, 2), std::string("0001"));
});

CB_TEST("build.sm83_asm.directives_are_case_insensitive", [] {
    CB_CHECK_EQ(at("section \"a\", rom0[$0]\nDB 1\nDw $1234\nDS 2\nNOP\nLD A,B", 0, 7), std::string("01") + "3412" + "0000" + "00" + "78");
});

// ---- the refused lines ---------------------------------------------------------------------------------------------------------------

CB_TEST("build.sm83_asm.a_refused_line_is_named_with_its_number", [] {
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " nop\n bogus a,b"), 3u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " nop\n ld a"), 3u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ld a,undefined"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " db undefined"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " dw undefined, 1"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " x EQU undefined"), 2u);
    CB_CHECK_EQ(refused_at("SECTION \"a\", ROMX[nope]\n nop"), 1u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds nope"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ds 1, nope"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ldh ($1234),a"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + "\n\n   \nA B C"), 5u);   // (blank lines count)
    CB_CHECK_EQ(refused_at(std::string(kRom0) + "db"), 2u);              // a directive with nothing to write
    CB_CHECK_EQ(refused_at(std::string(kRom0) + "ds"), 2u);
});

CB_TEST("build.sm83_asm.the_message_has_the_text_after_the_label_and_the_reason", [] {
    try {
        (void)rom(std::string(kRom0) + "start: ld a,nowhere");
        CB_CHECK(false);
    } catch (const AsmError& e) {
        const std::string what = e.what();
        CB_CHECK(what.find("ld a,nowhere") != std::string::npos);
        CB_CHECK(what.find("start:") == std::string::npos);   // (what follows the label is the line's text)
        CB_CHECK(what.find("nowhere") != std::string::npos);
    }
});

CB_TEST("build.sm83_asm.a_wrong_expression_is_an_error_not_a_crash", [] {
    // a division by zero and a syntax error are refused
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " db 1/0"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " db 7//2"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " ld a,1/0"), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " db (1, 2)"), 2u);
});

CB_TEST("build.sm83_asm.relative_jumps_have_a_reach", [] {
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " jr far\n ds 127\nfar: nop", 0x400), 0u);   // +127: the longest forward jump
    CB_CHECK_EQ(refused_at(std::string(kRom0) + " jr far\n ds 128\nfar: nop", 0x400), 2u);
    CB_CHECK_EQ(refused_at(std::string(kRom0) + "far: ds 129\n jr far", 0x400), 3u);
});

CB_TEST("build.sm83_asm.a_string_is_written_as_latin1", [] {
    // Design notes: "café" is 63 61 66 E9; a character beyond Latin-1 is `?` here, an error there
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db \"caf\xC3\xA9\"", 0, 4), std::string("636166e9"));
    CB_CHECK_EQ(at("SECTION \"a\", ROM0[$0]\n db \"a\xE2\x82\xAC\"", 0, 2), std::string("613f"));
});

// ---- the 102 sources of the old harness --------------------------------------------------------------------------------------------------

// A check with a message: which case of the harness.
#define CHECK_CASE(cond, message)                                                             do {                                                                                          ::cb::testing::count_check();                                                             if (!(cond)) ::cb::testing::record_failure(__FILE__, __LINE__, (message));            } while (false)

namespace {

// The ROM the harness wrote down: hex, with <N> for a run of N bytes of $FF.
Bytes unpack(const std::string& packed, std::size_t size) {
    Bytes out;
    for (std::size_t i = 0; i < packed.size();) {
        if (packed[i] == '<') {
            const std::size_t close = packed.find('>', i);
            out.insert(out.end(), static_cast<std::size_t>(std::stoull(packed.substr(i + 1, close - i - 1))), 0xFF);
            i = close + 1;
        } else if (packed[i] == '-') {
            ++i;
        } else {
            out.push_back(static_cast<std::uint8_t>(std::stoi(packed.substr(i, 2), nullptr, 16)));
            i += 2;
        }
    }
    CB_CHECK(out.size() <= size);
    out.resize(size, 0xFF);   // (the written ROM ends at its last byte that is not $FF: the harness's own sizes are `size`)
    return out;
}

}  // namespace

// Each case: what is made of a source (its ROM, or the line it refused) at a size and with INCBIN files from a folder.
// The ROM is compared byte for byte; a refusal by the line and the kind of error. Reviewed: the cases are the ones of the
// harness's source (demo program, every directive and instruction form, every kind of fault), and each case that differs from
// the recorded answer is marked "diverges" with what it answers now.
CB_TEST("build.sm83_asm.the_old_harness_sources_make_what_is_recorded", [] {
    const auto cases = cb::json::load(cb::testing::data_path("build/testdata/sm83_asm/cases.json"));
    CB_CHECK_EQ(cases.size(), std::size_t{102});
    std::size_t made = 0, refused = 0, diverging = 0;
    for (std::size_t i = 0; i < cases.size(); ++i) {
        const auto& c = cases[i];
        const std::string source = c["source"].get<std::string>();
        const std::int64_t size = c["size"].get<std::int64_t>();
        const stdfs::path folder = c["folder"].get<std::string>() == "." ? stdfs::path(".") : data_folder().parent_path() / c["folder"].get<std::string>();
        if (c.contains("diverges")) ++diverging;
        if (c.contains("rom")) {
            ++made;
            Bytes got;
            try {
                got = gb::build(source, static_cast<std::size_t>(std::max<std::int64_t>(size, 0)), folder);
            } catch (const std::exception& e) {
                CHECK_CASE(false, std::format("case {} was refused: {}", i, e.what()));
                continue;
            }
            const Bytes want = unpack(c["rom"].get<std::string>(), static_cast<std::size_t>(std::max<std::int64_t>(size, 0)));
            CHECK_CASE(got == want, std::format("case {}: the ROM differs", i));
            continue;
        }
        ++refused;
        const std::string kind = c["error"].get<std::string>();
        try {
            (void)gb::build(source, static_cast<std::size_t>(std::max<std::int64_t>(size, 0)), folder);
            CHECK_CASE(false, std::format("case {} should have been refused", i));
        } catch (const AsmError& e) {
            CHECK_CASE(kind == "AsmError", std::format("case {}: refused as {}", i, kind));
            if (c.contains("line")) CHECK_CASE(e.line() == c["line"].get<std::uint32_t>(), std::format("case {}: refused at line {}", i, e.line()));
        } catch (const cb::IoError&) {
            CHECK_CASE(kind == "IoError", std::format("case {}: an IoError", i));
        }
    }
    CB_CHECK_EQ(made + refused, std::size_t{102});
    CB_CHECK_EQ(diverging, std::size_t{6});
    CB_CHECK_EQ(made, std::size_t{53});
    CB_CHECK_EQ(refused, std::size_t{49});
});
