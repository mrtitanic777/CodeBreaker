#include "cb/decompile/m68k.hpp"

#include <algorithm>
#include <format>
#include <string>
#include <unordered_set>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace m = cb::decompile::m68k;
using cb::Bytes;
using std::string;

// DIVERGES from the Python module (the old harness: every one of the 65,536 opcode words decoded with random extension
// words at 8 start addresses, 4,000 truncations, 3,000 assembler lines and 1,000 mutated ones, agreed on all else; the
// expectations below came from the original and were read through by hand):
//  - decode() takes an unsigned offset (Python indexed from the end of the code for a negative one);
//  - the assembler refuses text the Python took and assembled to the wrong thing:
//      `-(D3)`, `4(D0)`, `(D0,D1.W)`: a data register where an address register belongs (Python used the number as An);
//      `BRA D0`, `BRA (A0)`: a branch to something that is not an address (Python branched to 0);
//      `MOVE.B 300(A0,D0.W),D0`: a displacement that does not fit its field (Python cut it to the low 8 bits; for a 16-bit
//        displacement the cut is to -32768..65535, so `$FFF0(A0)` still means -16);
//      `BRA.L x` and an unknown size such as `.X`: Python read a branch's unknown size as `.W`;
//  - a tab separates the mnemonic from the operands as a blank does (Python took "MOVE.L\tD0,D1" for a mnemonic);
//  - a label with an empty name is no label, in an immediate as everywhere else (Python wrote `#` and nothing);
//  - an Insn holds at most two operands and ten bytes (the 68000's maximum), not lists.

namespace {

Bytes hex(string s) {
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
    Bytes out;
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(s.substr(i, 2), nullptr, 16)));
    return out;
}

string to_hex(cb::ByteView b) {
    string out;
    for (std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

const cb::decompile::AddressLabels& address_labels() {
    static const cb::decompile::AddressLabels l = {{0x1002, "loop"}, {0x2000, "sub"}, {0xC00004, "VDP_CTRL"}};
    return l;
}

const cb::decompile::NameLabels& name_labels() {
    static const cb::decompile::NameLabels l = {{"loop", 0x1002}, {"sub", 0x2000}, {"near", 0x1010}, {"far", 0x1000000}};
    return l;
}

// "text | length flow target" of the instruction at the start of the hex, or "none".
string describe(const string& h, std::uint32_t pc = 0, std::size_t at = 0) {
    const Bytes code = hex(h);
    const auto insn = m::decode(code, at, pc);
    if (!insn) return "none";
    return m::text(*insn, address_labels());
}

string assemble(const string& line, std::uint32_t pc = 0x1000) { return to_hex(m::assemble(line, pc, name_labels())); }

}  // namespace

CB_TEST("decompile.m68k.decode.common_instructions", [] {
    CB_CHECK_EQ(describe("4e75"), "RTS");
    CB_CHECK_EQ(describe("4e71"), "NOP");
    CB_CHECK_EQ(describe("2018"), "MOVE.L (A0)+,D0");
    CB_CHECK_EQ(describe("303c1234"), "MOVE.W #$1234,D0");
    CB_CHECK_EQ(describe("43e8fff0"), "LEA -$10(A0),A1");
    CB_CHECK_EQ(describe("7005"), "MOVEQ #5,D0");
    CB_CHECK_EQ(describe("4afc"), "ILLEGAL");
    CB_CHECK_EQ(describe("4e4f"), "TRAP #15");
    CB_CHECK_EQ(describe("4e5e"), "UNLK A6");
    CB_CHECK_EQ(describe("e348"), "LSL.W #1,D0");
    CB_CHECK_EQ(describe("48c0"), "EXT.L D0");
    CB_CHECK_EQ(describe("4840"), "SWAP D0");
    CB_CHECK_EQ(describe("4e60"), "MOVE.L A0,USP");
    CB_CHECK_EQ(describe("40c0"), "MOVE.W SR,D0");
    CB_CHECK_EQ(describe("46fc2700"), "MOVE.W #$2700,SR");
    CB_CHECK_EQ(describe("027cf8ff"), "ANDI.W #$F8FF,SR");
    CB_CHECK_EQ(describe("003c0010"), "ORI.B #$10,CCR");
    CB_CHECK_EQ(describe("4a10"), "TST.B (A0)");
    CB_CHECK_EQ(describe("0c4000ff"), "CMPI.W #$FF,D0");
    CB_CHECK_EQ(describe("d3fc00001000"), "ADDA.L #$1000,A1");
    CB_CHECK_EQ(describe("20bc00000010"), "MOVE.L #$10,(A0)");
    CB_CHECK_EQ(describe("30301004"), "MOVE.W $4(A0,D1.W),D0");
    CB_CHECK_EQ(describe("01080004"), "MOVEP.W $4(A0),D0");
    CB_CHECK_EQ(describe("b308"), "CMPM.B (A0)+,(A1)+");
    CB_CHECK_EQ(describe("c140"), "EXG D0,D0");
    CB_CHECK_EQ(describe("c188"), "EXG D0,A0");
});

CB_TEST("decompile.m68k.decode.addresses_flow_and_labels", [] {
    const Bytes bne = hex("6610");
    const auto insn = m::decode(bne, 0, 0x1000);
    CB_CHECK(insn.has_value());
    CB_CHECK_EQ(m::text(*insn), "BNE.S $1012");
    CB_CHECK_EQ(*insn->target, 0x1012u);
    CB_CHECK(insn->flow == cb::decompile::z80::Flow::Branch);
    CB_CHECK_EQ(static_cast<int>(insn->length), 2);
    CB_CHECK_EQ(insn->word, 0x6610);
    CB_CHECK_EQ(insn->pc, 0x1000u);
    CB_CHECK_EQ(insn->shape(), "BNE.S d8:16");
    CB_CHECK_EQ(describe("60000230"), "BRA.W $232");
    CB_CHECK_EQ(describe("4eb900002000"), "JSR (sub).L");
    CB_CHECK_EQ(describe("0839000400c00004"), "BTST.B #$4,(VDP_CTRL).L");
    CB_CHECK_EQ(describe("13fc00ff00c00004"), "MOVE.B #$FF,(VDP_CTRL).L");
    CB_CHECK_EQ(describe("51c8fffe", 0x1000), "DBF D0,$1000");
    CB_CHECK_EQ(describe("41fa0010", 0x1000), "LEA $1012(PC),A0");
    CB_CHECK_EQ(describe("43fb0004", 0x1000), "LEA $1006(PC,D0.W),A1");
    // a long absolute, a sign-extended word (shown whole once it is negative), and a PC-relative address that has a label
    CB_CHECK_EQ(describe("3039ffff8000"), "MOVE.W ($FFFF8000).L,D0");
    CB_CHECK_EQ(describe("31c0ff8000"), "MOVE.W D0,($FFFFFF80).W");
    // the same instruction by flow
    const Bytes rts = hex("4e75"), jsr = hex("4eb900002000"), jmp = hex("4ef0 0000");
    CB_CHECK(m::decode(rts, 0, 0)->flow == cb::decompile::z80::Flow::Return);
    CB_CHECK(m::decode(jsr, 0, 0)->flow == cb::decompile::z80::Flow::Call);
    CB_CHECK_EQ(*m::decode(jsr, 0, 0)->target, 0x2000u);
    CB_CHECK(m::decode(jmp, 0, 0)->flow == cb::decompile::z80::Flow::Indirect);
    CB_CHECK(!m::decode(jmp, 0, 0)->target.has_value());
});

CB_TEST("decompile.m68k.decode.movem_lists", [] {
    CB_CHECK_EQ(describe("48e7fffe"), "MOVEM.L D0-D7/A0-A6,-(SP)");
    CB_CHECK_EQ(describe("4cdf7fff"), "MOVEM.L (SP)+,D0-D7/A0-A6");
    CB_CHECK_EQ(describe("48a7a000"), "MOVEM.W D0/D2,-(SP)");   // (the mask is reversed for -(An))
    CB_CHECK_EQ(describe("48900000"), "MOVEM.W 0,(A0)");
    CB_CHECK_EQ(describe("48908000"), "MOVEM.W SP,(A0)");   // (bit 15 is A7)
    CB_CHECK_EQ(describe("48900080"), "MOVEM.W D7,(A0)");
});

CB_TEST("decompile.m68k.decode.not_instructions", [] {
    CB_CHECK_EQ(describe("a000"), "none");        // line A
    CB_CHECK_EQ(describe("f000"), "none");        // line F
    CB_CHECK_EQ(describe("e0e8"), "none");        // memory shift with no extension word: the code ends inside it
    CB_CHECK_EQ(describe("57c8"), "none");        // DBEQ with its displacement word missing
    CB_CHECK_EQ(describe("303c12"), "none");      // an immediate cut short
    CB_CHECK_EQ(describe("4e72"), "none");        // STOP without its operand
    CB_CHECK_EQ(describe("4e75", 0, 1), "none");  // started at the second byte, one byte is left
    CB_CHECK_EQ(describe(""), "none");
    CB_CHECK_EQ(describe("4e75", 0, 2), "none");  // an offset at the end of the code
    CB_CHECK_EQ(describe("4e75", 0, 100), "none");
    // the 68020's index word (bits 8-10 set) is no 68000 operand
    CB_CHECK_EQ(describe("30300100"), "none");
    // an immediate byte with a dirty high byte is not one
    CB_CHECK_EQ(describe("003c0110"), "none");
});

CB_TEST("decompile.m68k.text.registers_and_numbers", [] {
    using K = m::OpKind;
    m::Op o;
    o.kind = K::Areg;
    o.reg = 7;
    CB_CHECK_EQ(m::operand_text(o), "SP");
    o.kind = K::Disp;
    o.reg = 3;
    o.value = -16;
    CB_CHECK_EQ(m::operand_text(o), "-$10(A3)");
    o.value = 0x7FFF;
    CB_CHECK_EQ(m::operand_text(o), "$7FFF(A3)");
    o.kind = K::Imm;
    o.size = m::Size::Long;
    o.value = 0x2000;
    CB_CHECK_EQ(m::operand_text(o), "#$2000");
    CB_CHECK_EQ(m::operand_text(o, address_labels()), "#sub");   // a long immediate that is a label names it
    o.size = m::Size::Word;
    CB_CHECK_EQ(m::operand_text(o, address_labels()), "#$2000");   // a word one does not
    o.size = m::Size::Long;
    const cb::decompile::AddressLabels empty_name = {{0x2000, ""}};
    CB_CHECK_EQ(m::operand_text(o, empty_name), "#$2000");   // (an empty name is no label)
    o.kind = K::Regs;
    o.value = 0x0000;
    CB_CHECK_EQ(m::operand_text(o), "0");
    o.value = 0x0F0F;
    CB_CHECK_EQ(m::operand_text(o), "D0-D3/A0-A3");
    o.value = 0x8080;
    CB_CHECK_EQ(m::operand_text(o), "D7/SP");
    o.predecrement = true;
    o.value = 0x0001;
    CB_CHECK_EQ(m::operand_text(o), "SP");
    o.kind = K::Quick;
    o.value = -3;
    CB_CHECK_EQ(m::operand_text(o), "#-3");
});

CB_TEST("decompile.m68k.assemble.lines", [] {
    CB_CHECK_EQ(assemble("MOVE.L D0,D1"), "2200");
    CB_CHECK_EQ(assemble("move.l d0,d1"), "2200");
    CB_CHECK_EQ(assemble("MOVE.W #$1234,(A0)+"), "30fc1234");
    CB_CHECK_EQ(assemble("MOVE.B 2(A0,D1.W),D2"), "14301002");
    CB_CHECK_EQ(assemble("LEA $10(A0),A1"), "43e80010");
    CB_CHECK_EQ(assemble("JSR (sub).L"), "4eb900002000");
    CB_CHECK_EQ(assemble("JMP loop"), "4ef900001002");
    CB_CHECK_EQ(assemble("MOVEM.L D0-D7/A0-A6,-(SP)"), "48e7fffe");
    CB_CHECK_EQ(assemble("MOVEM.L (SP)+,D0-D7/A0-A6"), "4cdf7fff");
    CB_CHECK_EQ(assemble("MOVEM.W D0/D2,-(A7)"), "48a7a000");
    CB_CHECK_EQ(assemble("MOVEQ #-1,D3"), "76ff");
    CB_CHECK_EQ(assemble("ADDQ.W #3,D1"), "5641");
    CB_CHECK_EQ(assemble("LSL.W #2,D0"), "e548");
    CB_CHECK_EQ(assemble("TRAP #15"), "4e4f");
    CB_CHECK_EQ(assemble("ADD.L #5,A0"), "d1fc00000005");   // (ADD to An is ADDA)
    CB_CHECK_EQ(assemble("ANDI #$F8FF,SR"), "027cf8ff");
    CB_CHECK_EQ(assemble("MOVE.W ($FFFF8000).W,D0"), "30388000");
    CB_CHECK_EQ(assemble("MOVE.L loop(PC),D0"), "203a0000");
    CB_CHECK_EQ(assemble("MOVE.W 40000(A0),D0"), "30289c40");   // (a 16-bit displacement written unsigned)
    CB_CHECK_EQ(assemble("RTS"), "4e75");
    CB_CHECK_EQ(assemble("rts"), "4e75");
    CB_CHECK_EQ(assemble("  MOVE.L  D0 , D1  "), "2200");
    CB_CHECK_EQ(assemble("MOVE.L\tD0,D1"), "2200");
});

CB_TEST("decompile.m68k.assemble.branches", [] {
    // BRA to the next instruction's address needs the word form (a displacement of 0 means "word follows")
    CB_CHECK_EQ(assemble("BRA loop"), "60000000");
    CB_CHECK_EQ(assemble("BRA.W loop"), "60000000");
    CB_CHECK_EQ(assemble("BNE.S near"), "660e");   // $1010 - ($1000 + 2)
    CB_CHECK_EQ(assemble("BNE near"), "660e");     // unsized: short when it fits
    CB_CHECK_EQ(assemble("DBF D0,loop"), "51c80000");
    CB_CHECK_EQ(assemble("BRA.B near"), "600e");
    CB_CHECK_EQ(assemble("BEQ.S $+4"), "6702");
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BRA.S loop", 0x1000, name_labels()));   // 0: needs the word form
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BEQ.S far", 0x1000, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BEQ.S $+2", 0x1000, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BEQ.W $+40000", 0x1000, name_labels()));   // a word displacement does not reach
});

CB_TEST("decompile.m68k.assemble.errors", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("FOO D0", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.X D0,D1", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.B A0,D0", 0, name_labels()));   // no byte move from An
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.B far(PC,D0.W),D0", 0x1000, name_labels()));   // out of reach
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BRA", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVEM.L", 0, name_labels()));
    // an unknown label is its own type, still a ParseError
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, (void)m::assemble("MOVE.L X,D0", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.L X,D0", 0, name_labels()));
    // the text the Python assembled to the wrong thing (DIVERGES above)
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.L -(D3),D0", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.L 4(D0),D1", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BRA D0", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("BRA.L loop", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.B 300(A0,D0.W),D0", 0, name_labels()));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::assemble("MOVE.W 70000(A0),D0", 0, name_labels()));
    CB_CHECK_EQ(assemble("MOVE.W $FFFF(A0),D0"), "3028ffff");
    CB_CHECK_EQ(assemble("MOVE.B 255(A0,D0.W),D0"), "103000ff");
});

CB_TEST("decompile.m68k.value", [] {
    CB_CHECK_EQ(m::value("*", {}, 0x1234), 0x1234);
    CB_CHECK_EQ(m::value(" * ", {}, 0x1234), 0x1234);
    CB_CHECK_EQ(m::value("$+4", {}, 0x1000), 0x1004);
    CB_CHECK_EQ(m::value("loop+2", name_labels()), 0x1004);
    CB_CHECK_THROWS_AS(cb::ParseError, (void)m::value("1 +", {}));
});

// What is written as an instruction assembles back to the same bytes: all 65,536 opcode words, each with a few extension
// words, decoded, written as text with no labels, assembled at the address they ran at.
CB_TEST("decompile.m68k.round_trip_of_every_opcode_word", [] {
    std::size_t decoded = 0, mismatched = 0;
    std::vector<string> examples;
    const std::vector<Bytes> tails = {hex("00121234567800ab"), hex("fffe8000000000ff"), hex("0070ff80ffff0000"), hex("f8007fff00000000"),
                                      hex("08400000deadbeef")};
    for (unsigned w = 0; w < 0x10000; ++w) {
        Bytes code = {static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w & 0xFF)};
        const Bytes& tail = tails[w % tails.size()];
        code.insert(code.end(), tail.begin(), tail.end());
        const std::uint32_t pc = 0x1000;
        const auto insn = m::decode(code, 0, pc);
        if (!insn) continue;
        ++decoded;
        const string line = m::text(*insn);
        string got;
        try {
            got = to_hex(m::assemble(line, pc));
        } catch (const cb::Error& e) {
            got = string("error: ") + e.what();
        }
        if (got != to_hex(insn->bytes())) {
            ++mismatched;
            if (examples.size() < 12) examples.push_back(std::format("{:04x} {} -> {} (was {})", w, line, got, to_hex(insn->bytes())));
        }
    }
    CB_CHECK(decoded > 40000);
    // The CPU ignores a few bits an assembler never writes; the decoder takes only the encodings an assembler writes, so
    // nothing should differ. (Printed when it does.)
    for (const string& e : examples) cb::testing::record_failure(__FILE__, __LINE__, "round trip: " + e);
    CB_CHECK_EQ(mismatched, 0u);
});

CB_TEST("decompile.m68k.the_shapes_are_the_originals", [] {
    // 45,816 distinct shapes in the 65,536 opcode words (the original's count; several words share a shape, as EXG D0,D1
    // does in two spellings).
    std::unordered_set<string> shapes;
    for (unsigned w = 0; w < 0x10000; ++w) {
        const Bytes code = {static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w & 0xFF), 0, 0, 0, 0, 0, 0, 0, 0};
        if (const auto insn = m::decode(code, 0, 0)) shapes.insert(insn->shape());
    }
    CB_CHECK_EQ(shapes.size(), 45816u);
});

CB_TEST("decompile.m68k.truncated_at_every_length", [] {
    // an instruction whose extension words are all there decodes; cut at any byte before its end, it is nullopt
    const std::vector<string> full = {"13fc00ff00c00004", "0839000400c00004", "4eb900002000", "303c1234", "41fa0010", "43fb0004", "30301004", "48e7fffe", "60000230"};
    for (const string& h : full) {
        const Bytes code = hex(h);
        CB_CHECK(m::decode(code, 0, 0).has_value());
        for (std::size_t n = 0; n < code.size(); ++n) {
            const Bytes cut(code.begin(), code.begin() + static_cast<std::ptrdiff_t>(n));
            CB_CHECK(!m::decode(cut, 0, 0).has_value());
        }
        // and with more code after it, the length is the same
        Bytes more = code;
        more.insert(more.end(), 6, 0x4e);
        CB_CHECK_EQ(static_cast<std::size_t>(m::decode(more, 0, 0)->length), code.size());
    }
});

CB_TEST("decompile.m68k.pc_wraps_at_32_bits", [] {
    // a branch near the top of the address space reaches the bottom
    const Bytes bra = hex("6010");
    CB_CHECK_EQ(*m::decode(bra, 0, 0xFFFFFFF0u)->target, 0x2u);
    CB_CHECK_EQ(describe("41fa0010", 0xFFFFFFFEu), "LEA $10(PC),A0");   // $FFFFFFFE + 2 + $10 wraps to $10
});
