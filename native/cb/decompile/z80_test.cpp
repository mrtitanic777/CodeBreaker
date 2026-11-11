#include "cb/decompile/z80.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace z = cb::decompile::z80;
using cb::Bytes;
using std::string;

// DIVERGES from the Python module (the old harness, 25,854 cases over every table entry, decode, text, assemble and
// value, agreed on all else; the expectations below came from the original and were read through by hand):
//  - value() is a parser, not eval(): the operators are + - * / (floor) & | ^ ~ << >> and parentheses. Python's `**`
//    and comparisons are not there; tokens that were silently dropped (`!`, `#`, `,`, `=`, a lone `%` or `'`) or glued
//    together (`1 2` was 12, `%12` was 12) are a ParseError; a number with leading zeros (`007`) reads as decimal
//    where Python raised SyntaxError; a result past 64 bits is a ParseError instead of a big integer;
//  - an unknown label is a cb::decompile::z80::UnknownLabel (Python: KeyError), the rest of the failures are ParseError;
//  - decode() takes an unsigned offset (Python indexed from the end for a negative one);
//  - an Insn holds at most two values and four bytes (the table's maximum), not lists.

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

const cb::decompile::NameLabels& labels() {
    static const cb::decompile::NameLabels l = {{"X", 5}, {"loop", 0x1234}, {"sub_0150", 0x150}, {"back", 0x10},
                                                {"main", 0x0100}, {"table", 0x0180}};
    return l;
}

cb::decompile::AddressLabels address_labels() {
    cb::decompile::AddressLabels l;
    l[0x150] = "sub_0150";
    l[0x1234] = "loop";
    l[0x10] = "back";
    l[0x100] = "main";
    l[0x180] = "table";
    return l;
}

string assembled(const string& line, std::uint32_t pc) {
    try {
        return to_hex(z::assemble(line, pc, labels()));
    } catch (const z::UnknownLabel&) {
        return "KeyError";
    } catch (const cb::ParseError&) {
        return "ValueError";
    }
}

}  // namespace

CB_TEST("decompile.z80.assemble", [] {
    struct Case {
        const char* line;
        std::uint32_t pc;
        const char* expected;   // hex, or the kind of error
    };
    const Case cases[] = {
        {"NOP", 4096, "00"}, {"LD A,B", 4096, "78"}, {"ld a,3", 0, "3e03"}, {"LD A,(HL)", 0, "7e"}, {"LD (HL),A", 0, "77"},
        {"LD A,($C000)", 0, "3a00c0"}, {"LD ($FFFF),A", 0, "32ffff"}, {"LD HL,table", 0, "218001"}, {"LD HL,($C000)", 0, "2a00c0"},
        {"LD ($C000),HL", 0, "2200c0"}, {"LD BC,15", 0, "010f00"}, {"LD SP,$DFF0", 0, "31f0df"}, {"LD (IX+3),$55", 0, "dd360355"},
        {"LD A,(IX-1)", 0, "dd7eff"}, {"LD (IY+0),B", 0, "fd7000"}, {"LD IXH,5", 0, "dd2605"}, {"LD IXL,IXH", 0, "dd6c"},
        {"INC IYL", 0, "fd2c"}, {"ADD A,$01", 0, "c601"}, {"ADD A,B", 0, "80"}, {"SUB 1", 0, "d601"}, {"SBC A,(HL)", 0, "9e"},
        {"AND $0F", 0, "e60f"}, {"XOR A", 0, "af"}, {"CP 'a'", 0, "fe61"}, {"ADD HL,BC", 0, "09"}, {"ADD IX,DE", 0, "dd19"},
        {"ADC HL,SP", 0, "ed7a"}, {"SBC HL,HL", 0, "ed62"}, {"INC (IX+5)", 0, "dd3405"}, {"DEC (IY-2)", 0, "fd35fe"},
        {"JP main", 0, "c30001"}, {"JP NZ,loop", 0, "c23412"}, {"JP (HL)", 0, "e9"}, {"JP (IX)", 0, "dde9"},
        {"JR back", 32, "18ee"}, {"JR NZ,$1002", 4096, "2000"}, {"JR $", 4096, "18fe"}, {"JR C,$1081", 4096, "387f"},
        {"JR C,$1082", 4096, "ValueError"}, {"JR $0F82", 4096, "1880"}, {"JR $0F81", 4096, "ValueError"}, {"DJNZ $", 8192, "10fe"},
        {"CALL sub_0150", 0, "cd5001"}, {"CALL Z,sub_0150", 0, "cc5001"}, {"RET", 0, "c9"}, {"RET NZ", 0, "c0"},
        {"RST $38", 0, "ff"}, {"RST 8", 0, "cf"}, {"RST 38h", 0, "ff"}, {"PUSH AF", 0, "f5"}, {"POP IX", 0, "dde1"},
        {"EX AF,AF'", 0, "08"}, {"EX DE,HL", 0, "eb"}, {"EX (SP),HL", 0, "e3"}, {"EX (SP),IY", 0, "fde3"}, {"EXX", 0, "d9"},
        {"IN A,($BF)", 0, "dbbf"}, {"OUT ($BE),A", 0, "d3be"}, {"IN B,(C)", 0, "ed40"}, {"IN (C)", 0, "ed70"},
        {"OUT (C),D", 0, "ed51"}, {"OUT (C),0", 0, "ed71"}, {"OUT (C), 0", 0, "ed71"}, {"IM 0", 0, "ed46"}, {"IM 1", 0, "ed56"},
        {"IM 2", 0, "ed5e"}, {"DI", 0, "f3"}, {"EI", 0, "fb"}, {"HALT", 0, "76"}, {"NEG", 0, "ed44"}, {"RETN", 0, "ed45"},
        {"RETI", 0, "ed4d"}, {"LD I,A", 0, "ed47"}, {"LD A,R", 0, "ed5f"}, {"RRD", 0, "ed67"}, {"LDIR", 0, "edb0"},
        {"CPDR", 0, "edb9"}, {"OTDR", 0, "edbb"}, {"INI", 0, "eda2"}, {"RLC B", 0, "cb00"}, {"SRL (HL)", 0, "cb3e"},
        {"SLL A", 0, "cb37"}, {"BIT 0,B", 0, "cb40"}, {"BIT 7,(HL)", 0, "cb7e"}, {"RES 3,C", 0, "cb99"},
        {"SET 2,(IX+3)", 0, "ddcb03d6"}, {"SET 0,(IX+5),A", 0, "ddcb05c7"}, {"RLC (IY-1),B", 0, "fdcbff00"},
        {"BIT 1,(IX+2)", 0, "ddcb0248"}, {"RL (IX+0)", 0, "ddcb0016"}, {"DAA", 0, "27"}, {"CPL", 0, "2f"}, {"SCF", 0, "37"},
        {"CCF", 0, "3f"}, {"RLCA", 0, "07"}, {"RRA", 0, "1f"}, {"LD SP,HL", 0, "f9"}, {"LD SP,IX", 0, "ddf9"},
        {"LD A,<$1234", 0, "3e34"}, {"LD A,>$1234", 0, "3e12"}, {"LD HL,main+2", 0, "210201"}, {"LD A,(1+2)*3", 0, "3e09"},
        {"LD A,-1", 0, "3eff"}, {"LD A,$100", 0, "3e00"}, {"ld hl,($C000)", 0, "2a00c0"}, {"Ld A , ( HL )", 0, "7e"},
        {"LD  A,B", 0, "78"}, {"LD A,unknown_label", 0, "KeyError"}, {"FOO", 0, "ValueError"}, {"LD A", 0, "ValueError"},
        {"", 0, "ValueError"}, {"LD A,B,C", 0, "ValueError"}, {"BIT 8,B", 0, "ValueError"}, {"IM 3", 0, "ValueError"},
        {"RST 7", 0, "ValueError"}, {"LD (IX+200),A", 0, "dd77c8"}, {"JP", 0, "ValueError"}, {"OUT (C),1", 0, "ValueError"},
        {"LD (BC),B", 0, "ValueError"}, {"PUSH SP", 0, "ValueError"}, {"EX AF,HL", 0, "ValueError"},
    };
    for (const Case& c : cases) CB_CHECK_EQ(assembled(c.line, c.pc), c.expected);
});

CB_TEST("decompile.z80.decode_and_text", [] {
    struct Case {
        const char* code;
        std::uint32_t pc;
        const char* pattern;   // null: the code ends inside the instruction
        int size;
        std::vector<int> values;
        int target;            // -1: none
        const char* flow;
        const char* text;
        const char* text_with_labels;
    };
    const Case cases[] = {
        {"00", 4096, "NOP", 1, {}, -1, "", "NOP", "NOP"},
        {"3e05", 0, "LD A,n", 2, {5}, -1, "", "LD A,$05", "LD A,$05"},
        {"3a00c0", 0, "LD A,(nn)", 3, {49152}, -1, "", "LD A,($C000)", "LD A,($C000)"},
        {"21 8001", 0, "LD HL,nn", 3, {384}, -1, "", "LD HL,$0180", "LD HL,table"},
        {"18fe", 4096, "JR e", 2, {4096}, 4096, "jump", "JR $1000", "JR $1000"},
        {"1802", 65534, "JR e", 2, {2}, 2, "jump", "JR $0002", "JR $0002"},                  // the target wraps at 16 bits
        {"18ee", 32, "JR e", 2, {16}, 16, "jump", "JR $0010", "JR back"},
        {"20f0", 3, "JR NZ,e", 2, {65525}, 65525, "branch", "JR NZ,$FFF5", "JR NZ,$FFF5"},
        {"10fe", 8192, "DJNZ e", 2, {8192}, 8192, "branch", "DJNZ $2000", "DJNZ $2000"},
        {"c30001", 0, "JP nn", 3, {256}, 256, "jump", "JP $0100", "JP main"},
        {"c23412", 0, "JP NZ,nn", 3, {4660}, 4660, "branch", "JP NZ,$1234", "JP NZ,loop"},
        {"cd5001", 0, "CALL nn", 3, {336}, 336, "call", "CALL $0150", "CALL sub_0150"},
        {"cc5001", 0, "CALL Z,nn", 3, {336}, 336, "ccall", "CALL Z,$0150", "CALL Z,sub_0150"},
        {"c9", 0, "RET", 1, {}, -1, "ret", "RET", "RET"},
        {"c0", 0, "RET NZ", 1, {}, -1, "cret", "RET NZ", "RET NZ"},
        {"ff", 0, "RST $38", 1, {}, 56, "call", "RST $38", "RST $38"},
        {"c7", 0, "RST $00", 1, {}, 0, "call", "RST $00", "RST $00"},
        {"e9", 0, "JP (HL)", 1, {}, -1, "indirect", "JP (HL)", "JP (HL)"},
        {"dde9", 0, "JP (IX)", 2, {}, -1, "indirect", "JP (IX)", "JP (IX)"},
        {"fde9", 0, "JP (IY)", 2, {}, -1, "indirect", "JP (IY)", "JP (IY)"},
        {"76", 0, "HALT", 1, {}, -1, "halt", "HALT", "HALT"},
        {"ed4d", 0, "RETI", 2, {}, -1, "ret", "RETI", "RETI"},
        {"ed45", 0, "RETN", 2, {}, -1, "ret", "RETN", "RETN"},
        // the undocumented mirrors are written as their bytes, the instruction in a comment, so the source rebuilds the same
        {"ed4c", 0, "NEG", 2, {}, -1, "", ".db $ED,$4C ; NEG", ".db $ED,$4C ; NEG"},
        {"ed55", 0, "RETN", 2, {}, -1, "ret", ".db $ED,$55 ; RETN", ".db $ED,$55 ; RETN"},
        {"ed4e", 0, "IM 0", 2, {}, -1, "", ".db $ED,$4E ; IM 0", ".db $ED,$4E ; IM 0"},
        {"ed6b3412", 0, "LD HL,(nn)", 4, {4660}, -1, "", ".db $ED,$6B,$34,$12 ; LD HL,($1234)", ".db $ED,$6B,$34,$12 ; LD HL,(loop)"},
        {"ed77", 0, "NOP", 2, {}, -1, "", ".db $ED,$77 ; NOP", ".db $ED,$77 ; NOP"},
        {"ed00", 0, ".db $ED,$00", 2, {}, -1, "", ".db $ED,$00", ".db $ED,$00"},                // an ED opcode that does nothing
        {"edff", 0, ".db $ED,$FF", 2, {}, -1, "", ".db $ED,$FF", ".db $ED,$FF"},
        {"ed71", 0, "OUT (C),0", 2, {}, -1, "", "OUT (C),0", "OUT (C),0"},
        {"ed70", 0, "IN (C)", 2, {}, -1, "", "IN (C)", "IN (C)"},
        {"edb0", 0, "LDIR", 2, {}, -1, "", "LDIR", "LDIR"},
        {"cb36", 0, "SLL (HL)", 2, {}, -1, "", "SLL (HL)", "SLL (HL)"},
        {"cb7e", 0, "BIT 7,(HL)", 2, {}, -1, "", "BIT 7,(HL)", "BIT 7,(HL)"},
        {"ddcb03d6", 0, "SET 2,(IX+d)", 4, {3}, -1, "", "SET 2,(IX+$03)", "SET 2,(IX+$03)"},
        {"ddcb05c7", 0, "SET 0,(IX+d),A", 4, {5}, -1, "", "SET 0,(IX+$05),A", "SET 0,(IX+$05),A"},
        {"fdcbff00", 0, "RLC (IY+d),B", 4, {-1}, -1, "", "RLC (IY-$01),B", "RLC (IY-$01),B"},
        {"ddcb0246", 0, "BIT 0,(IX+d)", 4, {2}, -1, "", ".db $DD,$CB,$02,$46 ; BIT 0,(IX+$02)", ".db $DD,$CB,$02,$46 ; BIT 0,(IX+$02)"},
        {"ddcb807e", 0, "BIT 7,(IX+d)", 4, {-128}, -1, "", ".db $DD,$CB,$80,$7E ; BIT 7,(IX-$80)", ".db $DD,$CB,$80,$7E ; BIT 7,(IX-$80)"},
        {"dd2605", 0, "LD IXH,n", 3, {5}, -1, "", "LD IXH,$05", "LD IXH,$05"},
        {"dd6c", 0, "LD IXL,IXH", 2, {}, -1, "", "LD IXL,IXH", "LD IXL,IXH"},
        {"dd7eff", 0, "LD A,(IX+d)", 3, {-1}, -1, "", "LD A,(IX-$01)", "LD A,(IX-$01)"},
        {"dd360355", 0, "LD (IX+d),n", 4, {3, 85}, -1, "", "LD (IX+$03),$55", "LD (IX+$03),$55"},
        {"dd2a3412", 0, "LD IX,(nn)", 4, {4660}, -1, "", "LD IX,($1234)", "LD IX,(loop)"},
        {"dd21 0001", 0, "LD IX,nn", 4, {256}, -1, "", "LD IX,$0100", "LD IX,main"},
        {"ddf9", 0, "LD SP,IX", 2, {}, -1, "", "LD SP,IX", "LD SP,IX"},
        {"dde3", 0, "EX (SP),IX", 2, {}, -1, "", "EX (SP),IX", "EX (SP),IX"},
        // a prefix the next opcode does not use does nothing: a one-byte instruction of its own
        {"dd00", 0, ".db $DD", 1, {}, -1, "", ".db $DD", ".db $DD"},
        {"dddd21", 0, ".db $DD", 1, {}, -1, "", ".db $DD", ".db $DD"},
        {"ddfd00", 0, ".db $DD", 1, {}, -1, "", ".db $DD", ".db $DD"},
        {"dded4d", 0, ".db $DD", 1, {}, -1, "", ".db $DD", ".db $DD"},
        // code that ends inside an instruction
        {"dd", 0, nullptr, 0, {}, -1, "", "", ""}, {"fd", 0, nullptr, 0, {}, -1, "", "", ""}, {"ddcb", 0, nullptr, 0, {}, -1, "", "", ""},
        {"ddcb01", 0, nullptr, 0, {}, -1, "", "", ""}, {"dd36", 0, nullptr, 0, {}, -1, "", "", ""},
        {"dd2a34", 0, nullptr, 0, {}, -1, "", "", ""}, {"cb", 0, nullptr, 0, {}, -1, "", "", ""}, {"ed", 0, nullptr, 0, {}, -1, "", "", ""},
        {"3e", 0, nullptr, 0, {}, -1, "", "", ""}, {"3a00", 0, nullptr, 0, {}, -1, "", "", ""}, {"", 0, nullptr, 0, {}, -1, "", "", ""},
        {"d3be", 0, "OUT (n),A", 2, {190}, -1, "", "OUT ($BE),A", "OUT ($BE),A"},
        {"dbbf", 0, "IN A,(n)", 2, {191}, -1, "", "IN A,($BF)", "IN A,($BF)"},
        {"08", 0, "EX AF,AF'", 1, {}, -1, "", "EX AF,AF'", "EX AF,AF'"},
        {"ed730010", 0, "LD (nn),SP", 4, {4096}, -1, "", "LD ($1000),SP", "LD ($1000),SP"},
        {"cdffff", 16, "CALL nn", 3, {65535}, 65535, "call", "CALL $FFFF", "CALL $FFFF"},
    };
    for (const Case& c : cases) {
        const Bytes code = hex(c.code);
        auto insn = z::decode(code, 0, c.pc);
        if (!c.pattern) {
            CB_CHECK(!insn.has_value());
            continue;
        }
        CB_CHECK(insn.has_value());
        if (!insn) continue;
        CB_CHECK_EQ(insn->pattern, c.pattern);
        CB_CHECK_EQ(static_cast<int>(insn->size), c.size);
        CB_CHECK(std::vector<int>(insn->operands().begin(), insn->operands().end()) == c.values);
        CB_CHECK_EQ(insn->target ? static_cast<int>(*insn->target) : -1, c.target);
        CB_CHECK_EQ(z::flow_name(insn->flow), c.flow);
        CB_CHECK_EQ(insn->pc, c.pc);
        CB_CHECK_EQ(to_hex(insn->bytes()), to_hex(cb::ByteView(code).first(static_cast<std::size_t>(c.size))));
        CB_CHECK_EQ(z::text(*insn), c.text);
        CB_CHECK_EQ(z::text(*insn, address_labels()), c.text_with_labels);
    }
    // decoding in the middle of a buffer: the offset, not the start, is where the instruction is
    auto mid = z::decode(hex("00 00 3e 07 00"), 2, 0x8002);
    CB_CHECK(mid && mid->pattern == "LD A,n" && mid->values[0] == 7 && mid->pc == 0x8002);
    CB_CHECK(!z::decode(hex("00"), 1, 0));    // an offset at the end
    CB_CHECK(!z::decode(hex("00"), 5, 0));    // or past it
});

// Every opcode of every page, written as text and assembled again: the same bytes, or an instruction that decodes to the
// same text (the aliases: `IM 0` at ED 46 and ED 4E ...).
CB_TEST("decompile.z80.table_round_trip", [] {
    struct Page {
        z::Page page;
        std::vector<std::uint8_t> lead;     // the bytes before the opcode
        std::vector<std::uint8_t> tail;     // and after it
    };
    const std::vector<Page> pages = {{z::Page::Main, {}, {0x34, 0x12, 0x56}}, {z::Page::CB, {0xCB}, {0, 0}},
                                     {z::Page::ED, {0xED}, {0x34, 0x12}}, {z::Page::DD, {0xDD}, {0x05, 0x12, 0x56}},
                                     {z::Page::FD, {0xFD}, {0xFB, 0x12, 0x56}}, {z::Page::DDCB, {0xDD, 0xCB, 0x05}, {}},
                                     {z::Page::FDCB, {0xFD, 0xCB, 0xFB}, {}}};
    int checked = 0;
    for (const Page& p : pages) {
        for (int op = 0; op < 256; ++op) {
            Bytes code = p.lead;
            code.push_back(static_cast<std::uint8_t>(op));
            code.insert(code.end(), p.tail.begin(), p.tail.end());
            code.resize(code.size() + 4);
            auto insn = z::decode(code, 0, 0x1000);
            if (!insn || insn->pattern.starts_with(".")) continue;
            if (insn->pattern == "NOP" && (code[0] == 0xDD || code[0] == 0xFD)) continue;
            const string text = z::text(*insn);
            if (text.starts_with(".db ")) {   // bytes written as data (a mirror encoding): the bytes themselves
                string listed = text.substr(0, text.find(';'));
                Bytes bytes;
                const string data = listed.substr(4);   // (named: the pieces are views into it)
                for (std::string_view piece : cb::text::split(cb::text::trim(data), ',')) {
                    bytes.push_back(static_cast<std::uint8_t>(std::stoi(string(piece.substr(1)), nullptr, 16)));
                }
                CB_CHECK_EQ(to_hex(bytes), to_hex(cb::ByteView(code).first(insn->size)));
                ++checked;
                continue;
            }
            Bytes built;
            try {
                built = z::assemble(text, 0x1000);
            } catch (const cb::Error&) {
                CB_CHECK(false && "assemble failed on decoded text");
                continue;
            }
            if (to_hex(built) != to_hex(cb::ByteView(code).first(insn->size))) {
                built.resize(built.size() + 4);
                auto again = z::decode(built, 0, 0x1000);
                CB_CHECK(again.has_value() && z::text(*again) == text);
            }
            ++checked;
        }
    }
    CB_CHECK(checked > 1200);
    CB_CHECK_EQ(z::text(*z::decode(hex("dde9"), 0, 0)), "JP (IX)");
    CB_CHECK_EQ(z::text(*z::decode(hex("ddcb05c7"), 0, 0)), "SET 0,(IX+$05),A");
});

CB_TEST("decompile.z80.pattern_table", [] {
    CB_CHECK_EQ(z::pattern_of(z::Page::Main, 0x00), "NOP");
    CB_CHECK_EQ(z::pattern_of(z::Page::Main, 0x76), "HALT");
    CB_CHECK_EQ(z::pattern_of(z::Page::Main, 0x41), "LD B,C");
    CB_CHECK_EQ(z::pattern_of(z::Page::Main, 0xC3), "JP nn");
    CB_CHECK_EQ(z::pattern_of(z::Page::Main, 0xFF), "RST $38");
    for (int prefix : {0xCB, 0xDD, 0xED, 0xFD}) CB_CHECK_EQ(z::pattern_of(z::Page::Main, static_cast<std::uint8_t>(prefix)), "");
    CB_CHECK_EQ(z::pattern_of(z::Page::CB, 0x00), "RLC B");
    CB_CHECK_EQ(z::pattern_of(z::Page::CB, 0x46), "BIT 0,(HL)");
    CB_CHECK_EQ(z::pattern_of(z::Page::ED, 0x44), "NEG");
    CB_CHECK_EQ(z::pattern_of(z::Page::ED, 0x00), "");       // does nothing
    CB_CHECK_EQ(z::pattern_of(z::Page::ED, 0xB0), "LDIR");
    CB_CHECK_EQ(z::pattern_of(z::Page::DD, 0x21), "LD IX,nn");
    CB_CHECK_EQ(z::pattern_of(z::Page::FD, 0x36), "LD (IY+d),n");
    CB_CHECK_EQ(z::pattern_of(z::Page::DD, 0x00), "");       // a prefix that changes nothing
    CB_CHECK_EQ(z::pattern_of(z::Page::DD, 0xE9), "JP (IX)");
    CB_CHECK_EQ(z::pattern_of(z::Page::DDCB, 0x46), "BIT 0,(IX+d)");
    CB_CHECK_EQ(z::pattern_of(z::Page::DDCB, 0x00), "RLC (IX+d),B");
    CB_CHECK_EQ(z::pattern_of(z::Page::FDCB, 0xC6), "SET 0,(IY+d)");
    // the count of defined opcodes per page (the old harness's TABLE had 1270 entries besides the ED nothings)
    std::size_t defined = 0;
    for (z::Page page : {z::Page::Main, z::Page::CB, z::Page::ED, z::Page::DD, z::Page::FD, z::Page::DDCB, z::Page::FDCB}) {
        for (int op = 0; op < 256; ++op) defined += !z::pattern_of(page, static_cast<std::uint8_t>(op)).empty();
    }
    CB_CHECK_EQ(defined, 1270u);
    CB_CHECK_EQ(z::flow_of("JR e"), z::Flow::Jump);
    CB_CHECK_EQ(z::flow_of("JR NZ,e"), z::Flow::Branch);
    CB_CHECK_EQ(z::flow_of("JP (HL)"), z::Flow::Indirect);
    CB_CHECK_EQ(z::flow_of("RET"), z::Flow::Return);
    CB_CHECK_EQ(z::flow_of("RET PE"), z::Flow::ConditionalReturn);
    CB_CHECK_EQ(z::flow_of("CALL C,nn"), z::Flow::ConditionalCall);
    CB_CHECK_EQ(z::flow_of("LD A,B"), z::Flow::None);
    CB_CHECK_EQ(z::kConditions[6], "P");
    CB_CHECK_EQ(z::kRotates[6], "SLL");
});

CB_TEST("decompile.z80.value", [] {
    auto v = [](const char* e, std::uint32_t pc = 0) { return z::value(e, labels(), pc); };
    CB_CHECK_EQ(v("1+2*3"), 7);
    CB_CHECK_EQ(v("(1+2)*3"), 9);
    CB_CHECK_EQ(v("-5/2"), -3);              // floor division, as Python's // had it
    CB_CHECK_EQ(v("7/2"), 3);
    CB_CHECK_EQ(v("7/-2"), -4);
    CB_CHECK_EQ(v("1<<4|1"), 17);            // shifts bind tighter than |
    CB_CHECK_EQ(v("6&3^1"), 3);              // & binds tighter than ^
    CB_CHECK_EQ(v("1|2^3&4"), 3);
    CB_CHECK_EQ(v("~0"), -1);
    CB_CHECK_EQ(v("$FF&~$0F"), 240);
    CB_CHECK_EQ(v("- - 3"), 3);
    CB_CHECK_EQ(v("2 - -3"), 5);
    CB_CHECK_EQ(v("256>>4"), 16);
    CB_CHECK_EQ(v("-1>>1"), -1);
    CB_CHECK_EQ(v("0x10+1Fh+%101+31+'a'"), 180);
    CB_CHECK_EQ(v("$1F"), 31);
    CB_CHECK_EQ(v("0FFh"), 255);
    CB_CHECK_EQ(v("10h"), 16);
    CB_CHECK_EQ(v("%0101"), 5);
    CB_CHECK_EQ(v("0x1f"), 31);
    CB_CHECK_EQ(v("$"), 0);
    CB_CHECK_EQ(v("$+2", 0x1000), 0x1002);
    CB_CHECK_EQ(v("  X * 2 "), 10);
    CB_CHECK_EQ(v("loop-main"), 0x1134);
    CB_CHECK_EQ(v("<$1234"), 0x34);
    CB_CHECK_EQ(v(">$1234"), 0x12);
    CB_CHECK_EQ(v(">$1234+$100"), 0x13);     // the byte of the whole expression after it
    CB_CHECK_EQ(v("'\xC3\xA9'"), 233);       // a character literal reads a whole UTF-8 character
    cb::decompile::NameLabels special = {{".local", 7}, {"@v", 9}, {"X\xC3\xA9", 11}, {"_a1", 1}};
    CB_CHECK_EQ(z::value(".local+@v+X\xC3\xA9+_a1", special), 28);
    // errors: the label's type, the rest are ParseError
    CB_CHECK_THROWS_AS(z::UnknownLabel, (void)v("nope+1"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("nope+1"));   // (an UnknownLabel is a ParseError)
    try {
        (void)v("1 + nope");
    } catch (const z::UnknownLabel& e) {
        CB_CHECK_EQ(e.name(), "nope");
    }
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v(""));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1+"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("(1"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1)"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1 2"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1 # 2"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("*3"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1/0"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1<<64000"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1<<-1"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("9223372036854775807+1"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("99999999999999999999"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("3000000000*3000000000*3000000000"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v("1 < 2"));
    // try_value reports instead of throwing
    CB_CHECK(!z::try_value("1/0", labels()).ok());
    auto unknown = z::try_value("nope", labels());
    CB_CHECK(!unknown.ok() && std::string(unknown.error().kind()) == "UnknownLabel");
    CB_CHECK_EQ(z::try_value("3*4", labels()).value(), 12);
});

CB_TEST("decompile.z80.operand_helpers", [] {
    CB_CHECK(z::split_operands("A,B") == std::vector<string>({"A", "B"}));
    CB_CHECK(z::split_operands("(IX+1), 5") == std::vector<string>({"(IX+1)", "5"}));
    CB_CHECK(z::split_operands("(a,b),c").size() == 2u);        // a comma inside parentheses does not split
    CB_CHECK(z::split_operands("A,").size() == 1u);             // an empty last piece is dropped
    CB_CHECK(z::split_operands("A,,B") == std::vector<string>({"A", "", "B"}));
    CB_CHECK(z::split_operands("").empty());
    CB_CHECK(z::split_operands("   ").empty());
    CB_CHECK(z::balanced("") && z::balanced("(a)(b)") && z::balanced("((a))"));
    CB_CHECK(!z::balanced("(a") && !z::balanced("a)") && !z::balanced(")("));
});

// The real program tests/data/sms/demo.asm (a Master System program written by hand for the SMS lane): assembled with
// labels from a two-pass pass of the same assembler, and read back by the disassembler.
CB_TEST("decompile.z80.sms_demo_program", [] {
    const string source = cb::fs::read_text(cb::testing::data_path("decompile/testdata/z80_demo.asm"));
    struct Line {
        std::uint32_t address;
        string text;
    };
    std::vector<Line> program;
    cb::decompile::NameLabels names;
    std::uint32_t pc = 0;
    // pass 1 reads the layout: `.ORG` sets the address, a `name:` defines a label, an instruction takes its size from
    // assembling it with every label far away
    auto strip = [](std::string_view s) { return string(cb::text::trim(s.substr(0, s.find(';')))); };
    for (std::string_view raw : cb::text::lines(source)) {
        string line = strip(raw);
        if (line.empty() || line.starts_with(".MEMORYMAP") || line.starts_with("SLOT") || line.starts_with(".ENDME") ||
            line.starts_with(".ROMBANKMAP") || line.starts_with("BANK") || line.starts_with(".ENDRO") || line.starts_with(".BANK")) {
            continue;
        }
        if (line.starts_with(".ORG")) {
            pc = static_cast<std::uint32_t>(z::value(line.substr(4), {}, 0));
            continue;
        }
        if (line.starts_with(".db") || line.starts_with(".DB")) break;     // the table and the header: data
        const std::size_t colon = line.find(':');
        if (colon != string::npos && line.find('"') == string::npos) {
            names[line.substr(0, colon)] = pc;
            line = strip(line.substr(colon + 1));
        }
        if (line.empty()) continue;
        if (line.starts_with(".db") || line.starts_with(".DB")) break;
        program.push_back({pc, line});
        cb::decompile::NameLabels far_away = names;
        for (const char* n : {"main", "vbl", "loop", "fill", "idle", "table"}) far_away.emplace(n, 0xFFFF);
        pc += static_cast<std::uint32_t>(z::assemble(line, pc, far_away).size());
    }
    names["table"] = pc;   // the data after the last instruction
    CB_CHECK_EQ(program.size(), 36u);   // the instructions of demo.asm, counted by hand
    // pass 2 and a disassembly of what came out, against the text the program should read as
    const std::vector<string> expected = {
        "DI", "IM 1", "LD SP,$DFF0", "JP main", "PUSH AF", "IN A,($BF)", "CALL vbl", "POP AF", "EI", "RETI", "RETN",
        "LD A,$03", "LD ($FFFF),A", "LD HL,table", "LD B,$04", "LD A,(HL)", "OUT ($BE),A", "INC HL", "DJNZ loop", "CALL fill", "EI",
        "HALT", "JR idle", "LD A,($C000)", "INC A", "LD ($C000),A", "RET", "LD IX,$C100", "LD (IX+$03),$55", "SET 2,(IX+$03)",
        "LD HL,$C200", "LD DE,$C201", "LD BC,$000F", "LD (HL),$00", "LDIR", "RET"};
    cb::decompile::AddressLabels by_address;
    for (const auto& [name, address] : names) by_address[static_cast<std::uint32_t>(address)] = name;
    std::size_t index = 0;
    for (const Line& l : program) {
        Bytes b = z::assemble(l.text, l.address, names);
        auto insn = z::decode(b, 0, l.address);
        CB_CHECK(insn.has_value() && insn->size == b.size());
        if (!insn) continue;
        if (index < expected.size()) CB_CHECK_EQ(z::text(*insn, by_address), expected[index]);
        ++index;
    }
    CB_CHECK_EQ(names.at("main"), 0x100);
    CB_CHECK_EQ(names.at("vbl"), names.at("idle") + 3);   // halt (1) + jr (2)
});

CB_TEST("decompile.z80.assemble_range_and_forms", [] {
    // a relative jump reaches -126..+129 from its own address
    CB_CHECK_EQ(to_hex(z::assemble("JR $1081", 0x1000)), "187f");
    CB_CHECK_THROWS_AS(cb::ParseError, (void)z::assemble("JR $1082", 0x1000));
    CB_CHECK_EQ(to_hex(z::assemble("JR $0F82", 0x1000)), "1880");
    CB_CHECK_THROWS_AS(cb::ParseError, (void)z::assemble("JR $0F81", 0x1000));
    // the case of mnemonic and registers does not matter; labels are case-sensitive
    CB_CHECK_EQ(to_hex(z::assemble("ld hl , ( $c000 )")), "2a00c0");
    CB_CHECK_THROWS_AS(z::UnknownLabel, (void)z::assemble("jp MAIN", 0, labels()));
    // an unknown label is reported as one, not as a bad instruction
    try {
        (void)z::assemble("JP nowhere", 0, labels());
    } catch (const z::UnknownLabel& e) {
        CB_CHECK_EQ(e.name(), "nowhere");
    }
    // a value that does not fit a byte is its low byte (the assembler's habit), a negative one its two's complement
    CB_CHECK_EQ(to_hex(z::assemble("LD A,$1FF")), "3eff");
    CB_CHECK_EQ(to_hex(z::assemble("LD A,-128")), "3e80");
});
