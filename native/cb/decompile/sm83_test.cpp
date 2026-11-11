#include "cb/decompile/sm83.hpp"

#include <algorithm>
#include <format>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace s = cb::decompile::sm83;
using cb::Bytes;
using std::string;

// DIVERGES from the Python module (the old harness, 19,700 cases over the table, decode, text and assemble, agreed on
// everything else; the expectations below were produced by the original and read through by hand):
//  - an unknown label is a cb::decompile::z80::UnknownLabel (Python: KeyError), the other failures are ParseError
//    (ValueError); the expression language is the Z80 module's (see z80_test: no `**`, no comparisons);
//  - decode() takes an unsigned offset (Python indexed from the end for a negative one);
//  - an Insn holds at most one operand value and three bytes (the table's maximum), not lists;
//  - a label with an empty name counts as no label (Python wrote the empty string).

namespace {

Bytes hex(string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    Bytes out;
    for (std::size_t i = 0; i + 1 < str.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(str.substr(i, 2), nullptr, 16)));
    return out;
}

string to_hex(cb::ByteView b) {
    string out;
    for (std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

const cb::decompile::NameLabels& names() {
    static const cb::decompile::NameLabels l = {{"X", 5}, {"loop", 0x1234}, {"back", 0x10}, {"hram", 0xFF80}, {"rLY", 0xFF44}, {"main", 0x100}};
    return l;
}

string assembled(const string& line, std::uint32_t pc) {
    try {
        return to_hex(s::assemble(line, pc, names()));
    } catch (const cb::decompile::z80::UnknownLabel&) {
        return "KeyError";
    } catch (const cb::ParseError&) {
        return "ValueError";
    }
}

}  // namespace

// testdata/sm83_patterns.golden: the pattern of every opcode of both pages ("M 3E LD A,n", "C 30 SWAP B", "-" = not an
// instruction). Written by the original table generator and checked here against the Sharp SM83 opcode map: the eleven
// holes, LD (HL+)/(HL-) at $22 $2A $32 $3A, LDH at $E0 $F0, LD (C) at $E2 $F2, ADD SP,s at $E8, LD HL,SP+s at $F8, RETI at
// $D9, STOP at $10, SWAP at CB 30-37, and CB 30..37 being SWAP and not SLL.
CB_TEST("decompile.sm83.table", [] {
    const string golden = cb::fs::read_text(cb::testing::data_path("decompile/testdata/sm83_patterns.golden"));
    int rows = 0;
    for (std::string_view line : cb::text::lines(golden)) {
        if (line.empty()) continue;
        const auto parts = cb::text::split(line, ' ');
        CB_CHECK(parts.size() >= 3);
        if (parts.size() < 3) continue;
        const unsigned op = cb::text::parse_int<unsigned>(parts[1], 16).value();
        const string want(line.substr(line.find(' ', 2) + 1));
        const auto page = parts[0] == "M" ? s::Page::Main : s::Page::CB;
        const string got(s::pattern_of(page, static_cast<std::uint8_t>(op)));
        CB_CHECK_EQ(got.empty() ? string("-") : got, want);
        ++rows;
    }
    CB_CHECK_EQ(rows, 512);
    for (std::uint8_t op : s::kIllegalOpcodes) CB_CHECK(s::pattern_of(s::Page::Main, op).empty());
    CB_CHECK(s::pattern_of(s::Page::Main, 0xCB).empty());   // the prefix is not an instruction
});

CB_TEST("decompile.sm83.decode_and_text", [] {
    struct Case {
        const char* code;
        const char* pattern;   // null: no instruction (code ends inside it, or a hole of the map)
        int size;
        int value;             // -999999: none
        int target;            // -1: none
        const char* flow;
        const char* text;
        const char* text_with_labels;   // labels: $1234 far, $FF44 ly, the value lbl, the target tgt
    };
    constexpr int kNone = -999999;
    const Case cases[] = {
        {"00", "NOP", 1, kNone, -1, "", "NOP", "NOP"},
        {"3e05", "LD A,n", 2, 5, -1, "", "LD A,$05", "LD A,$05"},
        {"3a00c0", "LD A,(HL-)", 1, kNone, -1, "", "LD A,(HL-)", "LD A,(HL-)"},
        {"fa3412", "LD A,(nn)", 3, 0x1234, -1, "", "LD A,($1234)", "LD A,(lbl)"},
        {"18fe", "JR e", 2, 0x1000, 0x1000, "jump", "JR $1000", "JR tgt"},
        {"20f0", "JR NZ,e", 2, 0x0FF2, 0x0FF2, "branch", "JR NZ,$0FF2", "JR NZ,tgt"},
        {"1802", "JR e", 2, 0x1004, 0x1004, "jump", "JR $1004", "JR tgt"},
        {"c30001", "JP nn", 3, 0x100, 0x100, "jump", "JP $0100", "JP tgt"},
        {"c20010", "JP NZ,nn", 3, 0x1000, 0x1000, "branch", "JP NZ,$1000", "JP NZ,tgt"},
        {"cd5001", "CALL nn", 3, 0x150, 0x150, "call", "CALL $0150", "CALL tgt"},
        {"c45001", "CALL NZ,nn", 3, 0x150, 0x150, "ccall", "CALL NZ,$0150", "CALL NZ,tgt"},
        {"c9", "RET", 1, kNone, -1, "ret", "RET", "RET"},
        {"d9", "RETI", 1, kNone, -1, "ret", "RETI", "RETI"},
        {"c0", "RET NZ", 1, kNone, -1, "cret", "RET NZ", "RET NZ"},
        {"ff", "RST $38", 1, kNone, 0x38, "call", "RST $38", "RST $38"},
        {"c7", "RST $00", 1, kNone, 0, "call", "RST $00", "RST $00"},
        {"e9", "JP HL", 1, kNone, -1, "indirect", "JP HL", "JP HL"},
        {"76", "HALT", 1, kNone, -1, "halt", "HALT", "HALT"},
        {"1000", "STOP", 2, kNone, -1, "halt", "STOP", "STOP"},
        {"1001", "STOP", 2, kNone, -1, "halt", "DB $10,$01", "DB $10,$01"},   // a second byte an assembler cannot write
        {"f044", "LDH A,(h)", 2, 0xFF44, -1, "", "LDH A,($FF44)", "LDH A,(lbl)"},
        {"e044", "LDH (h),A", 2, 0xFF44, -1, "", "LDH ($FF44),A", "LDH (lbl),A"},
        {"e244", "LD (C),A", 1, kNone, -1, "", "LD (C),A", "LD (C),A"},
        {"f244", "LD A,(C)", 1, kNone, -1, "", "LD A,(C)", "LD A,(C)"},
        {"e8fb", "ADD SP,s", 2, -5, -1, "", "ADD SP,-$05", "ADD SP,-$05"},
        {"e805", "ADD SP,s", 2, 5, -1, "", "ADD SP,$05", "ADD SP,$05"},
        {"f805", "LD HL,SP+s", 2, 5, -1, "", "LD HL,SP+$05", "LD HL,SP+$05"},
        {"f8fb", "LD HL,SP+s", 2, -5, -1, "", "LD HL,SP-$05", "LD HL,SP-$05"},   // SP+-$05 reads SP-$05
        {"083412", "LD (nn),SP", 3, 0x1234, -1, "", "LD ($1234),SP", "LD (lbl),SP"},
        {"22", "LD (HL+),A", 1, kNone, -1, "", "LD (HL+),A", "LD (HL+),A"},
        {"cb36", "SWAP (HL)", 2, kNone, -1, "", "SWAP (HL)", "SWAP (HL)"},
        {"cb7e", "BIT 7,(HL)", 2, kNone, -1, "", "BIT 7,(HL)", "BIT 7,(HL)"},
        {"cbc7", "SET 0,A", 2, kNone, -1, "", "SET 0,A", "SET 0,A"},
        // holes of the map, and code that ends inside an instruction
        {"d3", nullptr, 0, 0, 0, "", "", ""}, {"fc", nullptr, 0, 0, 0, "", "", ""}, {"cb", nullptr, 0, 0, 0, "", "", ""},
        {"", nullptr, 0, 0, 0, "", "", ""}, {"3e", nullptr, 0, 0, 0, "", "", ""}, {"3a00", "LD A,(HL-)", 1, kNone, -1, "", "LD A,(HL-)", "LD A,(HL-)"},
        {"e0", nullptr, 0, 0, 0, "", "", ""}, {"e8", nullptr, 0, 0, 0, "", "", ""}, {"10", nullptr, 0, 0, 0, "", "", ""},
        {"f8", nullptr, 0, 0, 0, "", "", ""},
    };
    for (const Case& c : cases) {
        const Bytes code = hex(c.code);
        auto insn = s::decode(code, 0, 0x1000);
        if (!c.pattern) {
            CB_CHECK(!insn.has_value());
            continue;
        }
        CB_CHECK(insn.has_value());
        if (!insn) continue;
        CB_CHECK_EQ(insn->pattern, c.pattern);
        CB_CHECK_EQ(static_cast<int>(insn->size), c.size);
        CB_CHECK_EQ(insn->value ? static_cast<int>(*insn->value) : kNone, c.value);
        CB_CHECK_EQ(insn->target ? static_cast<int>(*insn->target) : -1, c.target);
        CB_CHECK_EQ(string(cb::decompile::z80::flow_name(insn->flow)), c.flow);
        CB_CHECK_EQ(to_hex(insn->bytes()), to_hex(Bytes(code.begin(), code.begin() + c.size)));
        CB_CHECK_EQ(insn->pc, 0x1000u);
        CB_CHECK_EQ(s::text(*insn), c.text);
        cb::decompile::AddressLabels labels = {{0x1234, "far"}, {0xFF44, "ly"}};
        if (insn->value) labels[static_cast<std::uint32_t>(*insn->value)] = "lbl";
        if (insn->target) labels[*insn->target] = "tgt";
        CB_CHECK_EQ(s::text(*insn, labels), c.text_with_labels);
    }
    // decode at an offset into the buffer; at the end of the buffer there is nothing
    const Bytes buf = hex("00 3e05 18fe");
    CB_CHECK_EQ(s::decode(buf, 1, 0x200)->pattern, "LD A,n");
    CB_CHECK_EQ(s::decode(buf, 3, 0x202)->target.value(), 0x202);
    CB_CHECK(!s::decode(buf, 5, 0).has_value());
    CB_CHECK(!s::decode(buf, 100, 0).has_value());
    // an empty label name is no label
    CB_CHECK_EQ(s::text(*s::decode(hex("c30001"), 0, 0), {{0x100, ""}}), "JP $0100");
});

CB_TEST("decompile.sm83.assemble", [] {
    struct Case {
        const char* line;
        const char* expected;   // hex, or the kind of error
    };
    // pc is $1000; names: X=5 loop=$1234 back=$10 hram=$FF80 rLY=$FF44 main=$100
    const Case cases[] = {
        {"NOP", "00"}, {"LD A,B", "78"}, {"ld a,3", "3e03"}, {"LD A,(HL)", "7e"}, {"LD A,(HL+)", "2a"}, {"LD A,(HLI)", "2a"},
        {"LD (hld),A", "32"}, {"LD A,(HL-)", "3a"}, {"LD ($C000),A", "ea00c0"}, {"LD A,($C000)", "fa00c0"},
        {"LD ($FF44),SP", "0844ff"}, {"LD (C),A", "e2"}, {"LD A,(C)", "f2"}, {"LDH A,(rLY)", "f044"}, {"LDH (hram),A", "e080"},
        {"LDH ($FF40),A", "e040"}, {"LDH ($0040),A", "ValueError"}, {"LDH ($FE00),A", "ValueError"}, {"LDH (C),A", "ValueError"},
        {"LD HL,SP+5", "f805"}, {"LD HL,SP-5", "f8fb"}, {"LD HL,SP + 5", "f805"}, {"ADD SP,-5", "e8fb"}, {"ADD SP,X", "e805"},
        {"ADD HL,SP", "39"}, {"LD SP,HL", "f9"}, {"JP HL", "e9"}, {"JP (HL)", "ValueError"}, {"JP main", "c30001"},
        {"JP NZ,loop", "c23412"}, {"JR back", "ValueError"}, {"JR NZ,$1002", "2000"}, {"JR $", "18fe"}, {"JR $1081", "187f"},
        {"JR $1082", "ValueError"}, {"JR $0F82", "1880"}, {"JR $0F81", "ValueError"}, {"CALL main", "cd0001"},
        {"CALL C,main", "dc0001"}, {"RET", "c9"}, {"RET NC", "d0"}, {"RETI", "d9"}, {"RST $38", "ff"}, {"RST 8", "cf"},
        {"RST 7", "ValueError"}, {"STOP", "1000"}, {"HALT", "76"}, {"SWAP B", "cb30"}, {"SWAP (HL)", "cb36"}, {"SLA A", "cb27"},
        {"SRL C", "cb39"}, {"BIT 7,H", "cb7c"}, {"RES 0,(HL)", "cb86"}, {"SET 3,A", "cbdf"}, {"BIT 8,A", "ValueError"},
        {"PUSH AF", "f5"}, {"POP BC", "c1"}, {"PUSH SP", "ValueError"}, {"DI", "f3"}, {"EI", "fb"}, {"DAA", "27"}, {"CPL", "2f"},
        {"SCF", "37"}, {"CCF", "3f"}, {"ADD A,$01", "c601"}, {"ADD A,B", "80"}, {"SUB 1", "d601"}, {"SBC A,(HL)", "9e"},
        {"CP 'a'", "fe61"}, {"AND $0F", "e60f"}, {"XOR A", "af"}, {"LD A,<$1234", "3e34"}, {"LD A,>$1234", "3e12"},
        {"LD A,X+1", "3e06"}, {"LD A,-1", "3eff"}, {"LD A,$100", "3e00"}, {"LD HL,main+2", "210201"}, {"INC (HL)", "34"},
        {"DEC SP", "3b"}, {"LD B,B", "40"}, {"LD (HL),(HL)", "ValueError"}, {"LD A,unknown", "KeyError"}, {"FOO", "ValueError"},
        {"", "ValueError"}, {"LD A", "ValueError"}, {"LD A,B,C", "ValueError"}, {"EX DE,HL", "ValueError"}, {"DJNZ $", "ValueError"},
        {"IM 1", "ValueError"}, {"LD IX,5", "ValueError"}, {"LD A,(IX+1)", "KeyError"}, {"NEG", "ValueError"}, {"CB", "ValueError"},
        {"LD A,(DE)", "1a"}, {"LD (BC),A", "02"}, {"LD A,(BC)", "0a"}, {"ADD HL,BC", "09"}, {"INC BC", "03"}, {"DEC HL", "2b"},
        {"LD DE,$1234", "113412"}, {"LD DE,loop", "113412"}, {"LD (nn),SP", "KeyError"}, {"LD B,$12", "0612"},
        {"LD (HL),$34", "3634"}, {"  ld   a , ( hl )  ", "7e"}, {"LD HL,(X)", "ValueError"},
    };
    for (const Case& c : cases) CB_CHECK_EQ(assembled(c.line, 0x1000), c.expected);
});

// Every instruction of the map, disassembled and assembled again, gives its bytes back (at a pc where every relative
// jump the byte can encode is in range of its own target).
CB_TEST("decompile.sm83.round_trip", [] {
    int checked = 0;
    for (unsigned prefix = 0; prefix < 2; ++prefix) {
        for (unsigned op = 0; op < 256; ++op) {
            Bytes code = prefix ? Bytes{0xCB, static_cast<std::uint8_t>(op)} : Bytes{static_cast<std::uint8_t>(op)};
            for (const std::uint8_t operand : {std::uint8_t{0x34}, std::uint8_t{0x12}, std::uint8_t{0xFF}}) code.push_back(operand);
            if (prefix == 0 && op == 0x10) code[1] = 0;   // STOP's second byte is 0
            if (prefix == 0 && (op == 0xE0 || op == 0xF0)) code[1] = 0x44;
            const auto insn = s::decode(code, 0, 0x4000);
            if (!insn) {
                CB_CHECK(prefix == 0 && (op == 0xCB || std::find(s::kIllegalOpcodes.begin(), s::kIllegalOpcodes.end(), op) != s::kIllegalOpcodes.end()));
                continue;
            }
            const Bytes again = s::assemble(s::text(*insn), 0x4000);
            CB_CHECK_EQ(to_hex(again), to_hex(insn->bytes()));
            ++checked;
        }
    }
    CB_CHECK_EQ(checked, 245 + 256);   // the main page less the eleven holes ($CB decodes as a prefix), and the whole CB page
});
