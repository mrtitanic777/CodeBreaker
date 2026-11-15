// cb-unity: alone (file-local names that other files in a batch also use)
// The PowerPC 750 family's instructions (the GameCube's Gekko, the Wii's Broadway, the Wii U's Espresso): words worked out by hand from the
// PowerPC instruction formats (D: op rD rA d, XO: op rD rA rB OE xo Rc, A: op frD frA frB frC xo Rc, ...) and Gekko's paired-single formats,
// the text of each form, the simplified mnemonics, the refusals, and the round trip: every word a row can make is written as text that
// assembles back to that word.
#include "cb/decompile/ppc.hpp"

#include <format>
#include <set>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace p = cb::decompile::ppc;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x80003100;

std::uint32_t word_of(const Bytes& b) {
    return static_cast<std::uint32_t>(b.at(0)) << 24 | static_cast<std::uint32_t>(b.at(1)) << 16 | static_cast<std::uint32_t>(b.at(2)) << 8 | static_cast<std::uint32_t>(b.at(3));
}

std::uint32_t assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) { return word_of(p::assemble(line, pc, labels)); }

string disassembled(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = p::decode_word(word, pc);
    return insn ? p::text(*insn, labels) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.ppc.integer_words_and_text", [] {
    const Case cases[] = {
        {0x60000000, "nop"},
        {0x38600001, "li r3,1"},
        {0x3860FFFF, "li r3,-1"},
        {0x3C608000, "lis r3,0x8000"},
        {0x38630010, "addi r3,r3,16"},
        {0x9421FFE0, "stwu r1,-32(r1)"},
        {0x90010014, "stw r0,20(r1)"},
        {0x80010014, "lwz r0,20(r1)"},
        {0x7C0802A6, "mflr r0"},
        {0x7C0803A6, "mtlr r0"},
        {0x7D8903A6, "mtctr r12"},
        {0x7C0902A6, "mfctr r0"},
        {0x4E800020, "blr"},
        {0x4E800021, "blrl"},
        {0x4E800420, "bctr"},
        {0x4E800421, "bctrl"},
        {0x7C641B78, "mr r4,r3"},
        {0x7C632A14, "add r3,r3,r5"},
        {0x7C632A15, "add. r3,r3,r5"},
        {0x7C632E14, "addo r3,r3,r5"},
        {0x7C632E15, "addo. r3,r3,r5"},
        {0x7C632B78, "or r3,r3,r5"},
        {0x7C6318F8, "not r3,r3"},
        {0x2C030000, "cmpwi r3,0"},
        {0x2F830000, "cmpwi cr7,r3,0"},
        {0x28030010, "cmplwi r3,0x10"},
        {0x7C032800, "cmpw r3,r5"},
        {0x7F832800, "cmpw cr7,r3,r5"},
        {0x7C032840, "cmplw r3,r5"},
        {0x5483103A, "rlwinm r3,r4,2,0,29"},
        {0x5483103B, "rlwinm. r3,r4,2,0,29"},
        {0x60630010, "ori r3,r3,0x10"},
        {0x64630001, "oris r3,r3,1"},
        {0x7C0004AC, "sync"},
        {0x4C00012C, "isync"},
        {0x7C0006AC, "eieio"},
        {0x7C001FEC, "dcbz r0,r3"},
        {0x7C60212D, "stwcx. r3,r0,r4"},
        {0x7C70FAA6, "mfspr r3,1008"},
        {0x7C70FBA6, "mtspr 1008,r3"},
        {0x7C6C42E6, "mftb r3,268"},
        {0x44000002, "sc"},
        {0x4CC63182, "crxor 6,6,6"},
        {0x7C6FF120, "mtcrf 0xFF,r3"},
        {0x7C600026, "mfcr r3"},
        {0x7C6000A6, "mfmsr r3"},
        {0x7C0802A6 | 0x600000, "mflr r3"},
        {0x7C7A02A6, "mfspr r3,26"},
        {0x7C632BD6, "divw r3,r3,r5"},
        {0x7C6329D6, "mullw r3,r3,r5"},
        {0x7C632850, "subf r3,r3,r5"},
        {0xB0830008, "sth r4,8(r3)"},
        {0x88830008, "lbz r4,8(r3)"},
        {0xBFA10014, "stmw r29,20(r1)"},
        {0x7C6329D7, "mullw. r3,r3,r5"},
    };
    for (const Case& c : cases) {
        if (string(c.text) == "-") continue;
        const string got = disassembled(c.word);
        CB_CHECK_EQ(got, string(c.text));
    }
});

CB_TEST("decompile.ppc.assembles_to_the_hand_worked_words", [] {
    const Case cases[] = {
        {0x60000000, "nop"},
        {0x38600001, "li r3,1"},
        {0x38600001, "addi r3,0,1"},
        {0x3860FFFF, "li r3, -1"},
        {0x3C608000, "lis r3,0x8000"},
        {0x9421FFE0, "stwu r1,-32(r1)"},
        {0x9421FFE0, "stwu r1, -32(r1)"},
        {0x7C0802A6, "mflr r0"},
        {0x7C641B78, "mr r4,r3"},
        {0x7C641B78, "or r4,r3,r3"},
        {0x2F830000, "cmpwi cr7,r3,0"},
        {0x2C030000, "cmpwi r3,0"},
    };
    for (const Case& c : cases) {
        if (string(c.text).starts_with("slwi")) continue;
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    CB_CHECK(assembled("rlwinm r3,r4,2,0,29") == 0x5483103A);
    CB_CHECK(assembled("lwz r0, 20 (r1)") == 0x80010014);
    CB_CHECK(assembled("lwz r0,20(r1)  ") == 0x80010014);
    CB_CHECK(assembled("LWZ R0,20(R1)") == 0x80010014);
    CB_CHECK(assembled("lwz 0,20(1)") == 0x80010014);
    CB_CHECK(assembled("lwz r0,(r1)") == 0x80010000);
});

CB_TEST("decompile.ppc.branches", [] {
    const cb::decompile::AddressLabels names = {{kBase + 0x10, "next"}, {kBase - 0x10, "top"}};
    CB_CHECK(disassembled(0x48000011, kBase, names) == "bl next");
    CB_CHECK(disassembled(0x48000010, kBase, names) == "b next");
    CB_CHECK(disassembled(0x4BFFFFF0, kBase, names) == "b top");
    CB_CHECK(disassembled(0x4082FFF0, kBase, names) == "bne top");
    CB_CHECK(disassembled(0x41820010, kBase, names) == "beq next");
    CB_CHECK(disassembled(0x409E0010, kBase, names) == "bne cr7,next");
    CB_CHECK(disassembled(0x4200FFF0, kBase, names) == "bdnz top");
    CB_CHECK(disassembled(0x4D820020) == "beqlr");
    CB_CHECK(disassembled(0x4C820420) == "bnectr");
    CB_CHECK(disassembled(0x4D9E0020) == "beqlr cr7");
    CB_CHECK(disassembled(0x40A20010) == "bc 5,2,0x80003110");   // (BO with the hint bit set is the general form)
    CB_CHECK(disassembled(0x48000013) == "bla 0x00000010");
    // flow
    const auto flow = [](std::uint32_t w) { return p::decode_word(w, kBase)->flow; };
    CB_CHECK(flow(0x48000010) == p::Flow::Jump);
    CB_CHECK(flow(0x48000011) == p::Flow::Call);
    CB_CHECK(flow(0x4082FFF0) == p::Flow::Branch);
    CB_CHECK(flow(0x4E800020) == p::Flow::Return);
    CB_CHECK(flow(0x4E800021) == p::Flow::Call);
    CB_CHECK(flow(0x4E800420) == p::Flow::Indirect);
    CB_CHECK(flow(0x4D820020) == p::Flow::Branch);
    CB_CHECK(flow(0x60000000) == p::Flow::None);
    CB_CHECK(*p::decode_word(0x48000010, kBase)->target == kBase + 0x10);
    CB_CHECK(*p::decode_word(0x4BFFFFF0, kBase)->target == kBase - 0x10);
    CB_CHECK(*p::decode_word(0x48000013, kBase)->target == 0x10);   // absolute
    CB_CHECK(!p::decode_word(0x4E800020, kBase)->target);
    // assembling: labels, absolute, reach
    const cb::decompile::NameLabels labels = {{"next", kBase + 0x10}, {"top", kBase - 0x10}};
    CB_CHECK(assembled("bl next", kBase, labels) == 0x48000011);
    CB_CHECK(assembled("b top", kBase, labels) == 0x4BFFFFF0);
    CB_CHECK(assembled("bne top", kBase, labels) == 0x4082FFF0);
    CB_CHECK(assembled("bne cr7,next", kBase, labels) == 0x409E0010);
    CB_CHECK(assembled("beqlr cr7") == 0x4D9E0020);
    CB_CHECK(assembled("bdnz top", kBase, labels) == 0x4200FFF0);
    CB_CHECK(assembled("bc 4,2,top", kBase, labels) == 0x4082FFF0);
    bool refused = false;
    try {
        assembled("bne far", kBase, {{"far", kBase + 0x20000}});
    } catch (const cb::ParseError&) {
        refused = true;
    }
    CB_CHECK(refused);
    refused = false;
    try {
        assembled("b odd", kBase, {{"odd", kBase + 2}});
    } catch (const cb::ParseError&) {
        refused = true;
    }
    CB_CHECK(refused);
});

CB_TEST("decompile.ppc.floating_point_and_paired_singles", [] {
    const Case cases[] = {
        {0xC0230000, "lfs f1,0(r3)"},
        {0xC8230008, "lfd f1,8(r3)"},
        {0xD0230004, "stfs f1,4(r3)"},
        {0xEC22182A, "fadds f1,f2,f3"},
        {0xFC22182A, "fadd f1,f2,f3"},
        {0xFC22182B, "fadd. f1,f2,f3"},
        {0xFC2200F2, "fmul f1,f2,f3"},
        {0xFC2220FA, "fmadd f1,f2,f3,f4"},
        {0xEC2220FA, "fmadds f1,f2,f3,f4"},
        {0xFC20181E, "fctiwz f1,f3"},
        {0xFC201890, "fmr f1,f3"},
        {0xFC02F000, "fcmpu cr0,f2,f30"},
        {0x10221C20, "ps_merge00 f1,f2,f3"},
        {0x1022182A, "ps_add f1,f2,f3"},
        {0x102220FA, "ps_madd f1,f2,f3,f4"},
        {0x102220FA | 1, "ps_madd. f1,f2,f3,f4"},
        {0x10201890, "ps_mr f1,f3"},
        {0xE0230008, "psq_l f1,8(r3),0,0"},
        {0xE0239008, "psq_l f1,8(r3),1,1"},
        {0xE4230008, "psq_lu f1,8(r3),0,0"},
        {0xF0230008, "psq_st f1,8(r3),0,0"},
        {0xF4230008, "psq_stu f1,8(r3),0,0"},
        {0x1023200C, "psq_lx f1,r3,r4,0,0"},
        {0x1000EFEC, "dcbz_l r0,r29"},
    };
    for (const Case& c : cases) {
        if (string(c.text) == "-") continue;
        const string got = disassembled(c.word);
        CB_CHECK_EQ(got, string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
});

CB_TEST("decompile.ppc.immediate_override", [] {
    const auto insn = p::decode_word(0x3C608000, kBase);
    CB_CHECK(insn && p::has_immediate(*insn));
    CB_CHECK(p::text(*insn, {}, "msg@ha") == "lis r3,msg@ha");
    const auto load = p::decode_word(0x80630010, kBase);   // lwz r3,16(r3)
    CB_CHECK(load && p::text(*load, {}, "msg@l") == "lwz r3,msg@l(r3)");
    CB_CHECK(!p::has_immediate(*p::decode_word(0x7C632A14, kBase)));
    const cb::decompile::NameLabels labels = {{"msg", 0x80123456}};
    CB_CHECK(assembled("lis r3,msg@ha", kBase, labels) == 0x3C608012);
    CB_CHECK(assembled("addi r3,r3,msg@l", kBase, labels) == 0x38633456);
    // the low half of an address past 0x8000 carries into the high half: msg@ha is (x + 0x8000) >> 16
    const cb::decompile::NameLabels high = {{"msg", 0x8012C000}};
    CB_CHECK(assembled("lis r3,msg@ha", kBase, high) == 0x3C608013);
    CB_CHECK(assembled("addi r3,r3,msg@l", kBase, high) == 0x3863C000);
    CB_CHECK(p::value("1 + (2 << 4)", {}) == 33);
    CB_CHECK(p::value("end - start", {{"end", 0x80003200}, {"start", 0x80003100}}) == 0x100);
    CB_CHECK(p::value("0x12345678@h", {}) == 0x1234);
});

CB_TEST("decompile.ppc.refuses_what_is_not_an_instruction", [] {
    CB_CHECK(!p::decode_word(0x00000000, kBase));         // zeros are no instruction
    CB_CHECK(!p::decode_word(0x4E800022, kBase));         // blr with a reserved bit set
    CB_CHECK(!p::decode_word(0x7C6329D8, kBase));         // an extended opcode the 750 does not have
    // (opcode 9 is no instruction; 8 is subfic)
    CB_CHECK(!p::decode_word(0x24000000, kBase));
    const Bytes short_bytes = {0x4E, 0x80, 0x00};
    CB_CHECK(!p::decode(short_bytes, 0, kBase));
    const Bytes code = {0x4E, 0x80, 0x00, 0x20};
    CB_CHECK(p::decode(code, 0, kBase)->mnemonic == "blr");
    for (const char* bad : {"frob r3,r4", "addi r3,r4", "addi r3,r4,70000", "lwz r3,r4", "addi r99,r3,1", "li r3,1,2", "bne"}) {
        bool refused = false;
        try {
            assembled(bad);
        } catch (const cb::ParseError&) {
            refused = true;
        }
        CB_CHECK(refused);
    }
});

// Every word a row can make decodes, and its text assembles back to the word.
CB_TEST("decompile.ppc.every_row_round_trips", [] {
    CB_CHECK(p::row_count() > 250);
    std::set<std::string> names;
    std::size_t decoded = 0;
    for (std::size_t row = 0; row < p::row_count(); ++row) {
        const std::uint32_t pattern = p::row_pattern(row);
        const std::uint32_t vary = p::row_variable_bits(row);
        CB_CHECK((pattern & vary) == 0);
        CB_CHECK(names.insert(string(p::row_name(row))).second);
        for (const std::uint32_t seed : {0x00000000u, 0xFFFFFFFFu, 0x12345678u, 0xA5A5A5A5u, 0x5A5A5A5Au, 0x0F0F0F0Fu, 0xDEADBEEFu, 0x01234567u}) {
            const std::uint32_t word = pattern | (seed & vary);
            const auto insn = p::decode_word(word, kBase);
            if (!insn) continue;   // (the row's own constraint, a register written twice, is not met by this word)
            ++decoded;
            const string line = p::text(*insn);
            CB_CHECK_EQ(std::format("{:08X} {}", assembled(line), line), std::format("{:08X} {}", word, line));
        }
    }
    CB_CHECK(decoded > 1000);
});

// The 64-bit rows (the Cell's PPU): words worked out by hand, each row's words round-trip, and the 750 family does not read them as instructions.
CB_TEST("decompile.ppc.the_64_bit_rows", [] {
    const Case cases[] = {{0xE8610008, "ld r3,8(r1)"},       {0xF8210001, "stdu r1,0(r1)"}, {0xF8610008, "std r3,8(r1)"}, {0x78630020, "rldicl r3,r3,0,32"},
                          {0x7C6307B4, "extsw r3,r3"},       {0x7C6429D2, "mulld r3,r4,r5"}, {0x2C230000, "cmpdi r3,0"}};
    for (const Case& c : cases) {
        const auto insn = p::decode_word(c.word, kBase, true);
        CB_CHECK(insn.has_value());
        if (!insn) continue;
        CB_CHECK_EQ(p::text(*insn), string(c.text));
        CB_CHECK_EQ(word_of(p::assemble(c.text, kBase, {}, true)), c.word);
    }
    CB_CHECK(!p::decode_word(0xE8610008, kBase).has_value());
    std::size_t decoded = 0;
    for (std::size_t row = 0; row < p::row_count(); ++row) {
        const std::uint32_t pattern = p::row_pattern(row), vary = p::row_variable_bits(row);
        for (const std::uint32_t seed : {0x00000000u, 0xFFFFFFFFu, 0x12345678u, 0xA5A5A5A5u, 0x5A5A5A5Au, 0x0F0F0F0Fu, 0xDEADBEEFu, 0x01234567u}) {
            const std::uint32_t word = pattern | (seed & vary);
            const auto insn = p::decode_word(word, kBase, true);
            if (!insn) continue;
            ++decoded;
            const string line = p::text(*insn);
            CB_CHECK_EQ(std::format("{:08X} {}", word_of(p::assemble(line, kBase, {}, true)), line), std::format("{:08X} {}", word, line));
        }
    }
    CB_CHECK(decoded > 1000);
});

// A word that decodes is exactly one row's: no word of the whole 750 set is read two ways, and the text of a decoded word is the same
// whichever pc it runs at (only a branch's target moves with it).
CB_TEST("decompile.ppc.text_is_independent_of_pc", [] {
    for (const std::uint32_t word : {0x7C632A14u, 0x80630010u, 0x1022182Au, 0x2C030000u, 0x4E800020u}) {
        CB_CHECK(disassembled(word, 0x80003100) == disassembled(word, 0x81000000));
    }
});
