// cb-unity: alone
#include "cb/decompile/i8048.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/decompile/asm_value.hpp"

namespace cb::decompile::i8048 {

namespace {

using Table = std::array<std::optional<OpcodeEntry>, 256>;

Table build() {
    Table t{};
    const auto put = [&](unsigned op, std::string text, Kind kind = Kind::None) { t[op] = OpcodeEntry{std::move(text), kind}; };
    const auto r = [](unsigned n) { return "R" + std::to_string(n); };
    const auto i = [](unsigned n) { return "@R" + std::to_string(n); };
    put(0x00, "NOP");
    put(0x02, "OUTL BUS,A");
    put(0x03, "ADD A,#", Kind::Imm);
    put(0x05, "EN I");
    put(0x07, "DEC A");
    put(0x08, "INS A,BUS");
    put(0x09, "IN A,P1");
    put(0x0A, "IN A,P2");
    for (unsigned n = 0; n < 4; ++n) {
        put(0x0C + n, "MOVD A,P" + std::to_string(4 + n));
        put(0x3C + n, "MOVD P" + std::to_string(4 + n) + ",A");
        put(0x8C + n, "ORLD P" + std::to_string(4 + n) + ",A");
        put(0x9C + n, "ANLD P" + std::to_string(4 + n) + ",A");
    }
    put(0x13, "ADDC A,#", Kind::Imm);
    put(0x15, "DIS I");
    put(0x16, "JTF", Kind::Addr8);
    put(0x17, "INC A");
    put(0x22, "IN A,DBB");
    put(0x23, "MOV A,#", Kind::Imm);
    put(0x25, "EN TCNTI");
    put(0x26, "JNT0", Kind::Addr8);
    put(0x27, "CLR A");
    put(0x35, "DIS TCNTI");
    put(0x36, "JT0", Kind::Addr8);
    put(0x37, "CPL A");
    put(0x39, "OUTL P1,A");
    put(0x3A, "OUTL P2,A");
    put(0x42, "MOV A,T");
    put(0x43, "ORL A,#", Kind::Imm);
    put(0x45, "STRT CNT");
    put(0x46, "JNT1", Kind::Addr8);
    put(0x47, "SWAP A");
    put(0x53, "ANL A,#", Kind::Imm);
    put(0x55, "STRT T");
    put(0x56, "JT1", Kind::Addr8);
    put(0x57, "DA A");
    put(0x62, "MOV T,A");
    put(0x65, "STOP TCNT");
    put(0x67, "RRC A");
    put(0x75, "ENT0 CLK");
    put(0x76, "JF1", Kind::Addr8);
    put(0x77, "RR A");
    put(0x83, "RET");
    put(0x85, "CLR F0");
    put(0x86, "JNI", Kind::Addr8);
    put(0x88, "ORL BUS,#", Kind::Imm);
    put(0x89, "ORL P1,#", Kind::Imm);
    put(0x8A, "ORL P2,#", Kind::Imm);
    put(0x93, "RETR");
    put(0x95, "CPL F0");
    put(0x96, "JNZ", Kind::Addr8);
    put(0x97, "CLR C");
    put(0x98, "ANL BUS,#", Kind::Imm);
    put(0x99, "ANL P1,#", Kind::Imm);
    put(0x9A, "ANL P2,#", Kind::Imm);
    put(0xA3, "MOVP A,@A");
    put(0xA5, "CLR F1");
    put(0xA7, "CPL C");
    put(0xB3, "JMPP @A");
    put(0xB5, "CPL F1");
    put(0xB6, "JF0", Kind::Addr8);
    put(0xC5, "SEL RB0");
    put(0xC6, "JZ", Kind::Addr8);
    put(0xC7, "MOV A,PSW");
    put(0xD3, "XRL A,#", Kind::Imm);
    put(0xD5, "SEL RB1");
    put(0xD7, "MOV PSW,A");
    put(0xE3, "MOVP3 A,@A");
    put(0xE5, "SEL MB0");
    put(0xE6, "JNC", Kind::Addr8);
    put(0xE7, "RL A");
    put(0xF5, "SEL MB1");
    put(0xF6, "JC", Kind::Addr8);
    put(0xF7, "RLC A");
    for (unsigned n = 0; n < 2; ++n) {   // @R0 / @R1
        put(0x10 + n, "INC " + i(n));
        put(0x20 + n, "XCH A," + i(n));
        put(0x30 + n, "XCHD A," + i(n));
        put(0x40 + n, "ORL A," + i(n));
        put(0x50 + n, "ANL A," + i(n));
        put(0x60 + n, "ADD A," + i(n));
        put(0x70 + n, "ADDC A," + i(n));
        put(0x80 + n, "MOVX A," + i(n));
        put(0x90 + n, "MOVX " + i(n) + ",A");
        put(0xA0 + n, "MOV " + i(n) + ",A");
        put(0xB0 + n, "MOV " + i(n) + ",#", Kind::Imm);
        put(0xD0 + n, "XRL A," + i(n));
        put(0xF0 + n, "MOV A," + i(n));
    }
    for (unsigned n = 0; n < 8; ++n) {   // R0..R7
        put(0x18 + n, "INC " + r(n));
        put(0x28 + n, "XCH A," + r(n));
        put(0x48 + n, "ORL A," + r(n));
        put(0x58 + n, "ANL A," + r(n));
        put(0x68 + n, "ADD A," + r(n));
        put(0x78 + n, "ADDC A," + r(n));
        put(0xA8 + n, "MOV " + r(n) + ",A");
        put(0xB8 + n, "MOV " + r(n) + ",#", Kind::Imm);
        put(0xC8 + n, "DEC " + r(n));
        put(0xD8 + n, "XRL A," + r(n));
        put(0xE8 + n, "DJNZ " + r(n) + ",", Kind::Djnz);
        put(0xF8 + n, "MOV A," + r(n));
    }
    for (unsigned n = 0; n < 8; ++n) {   // the page bits of JMP and CALL, the bit of JBn
        put(0x04 + 0x20 * n, "JMP", Kind::Addr11);
        put(0x14 + 0x20 * n, "CALL", Kind::Addr11);
        put(0x12 + 0x20 * n, "JB" + std::to_string(n), Kind::Addr8);
    }
    return t;
}

std::string no_space(std::string_view s) {
    std::string out;
    for (const char c : s) {
        if (c != ' ' && c != '\t') out.push_back(c);
    }
    return out;
}

std::string upper_of(std::string_view s) { return text::upper(s); }

struct Index {
    std::vector<std::pair<std::string, unsigned>> exact;       // normalised text -> opcode, for instructions without an operand byte
    std::vector<std::pair<std::string, unsigned>> operand;     // normalised prefix -> opcode, with the longest tried first
};

const Index& index() {
    static const Index idx = [] {
        Index x;
        for (unsigned op = 0; op < 256; ++op) {
            const auto& e = opcode_table()[op];
            if (!e) continue;
            const std::string key = upper_of(no_space(e->text));
            if (e->kind == Kind::None) x.exact.emplace_back(key, op);
            else if (e->kind == Kind::Imm || e->kind == Kind::Djnz) x.operand.emplace_back(key, op);
            else if (e->kind == Kind::Addr8 || e->kind == Kind::Addr11) {
                if (std::none_of(x.operand.begin(), x.operand.end(), [&](const auto& p) { return p.first == key && (e->kind == Kind::Addr11); })) x.operand.emplace_back(key, op);
            }
        }
        std::stable_sort(x.operand.begin(), x.operand.end(), [](const auto& a, const auto& b) { return a.first.size() > b.first.size(); });
        return x;
    }();
    return idx;
}

}  // namespace

const std::array<std::optional<OpcodeEntry>, 256>& opcode_table() {
    static const Table t = build();
    return t;
}

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset >= code.size()) return std::nullopt;
    const std::uint8_t op = code[offset];
    const auto& e = opcode_table()[op];
    if (!e) return std::nullopt;
    Insn i;
    i.at = pc;
    i.op = op;
    i.entry = &*e;
    i.raw[0] = op;
    i.size = e->kind == Kind::None ? 1 : 2;
    if (i.size == 2) {
        if (offset + 1 >= code.size()) return std::nullopt;
        i.operand = code[offset + 1];
        i.raw[1] = i.operand;
    }
    switch (e->kind) {
        case Kind::Addr11: i.target = (pc & 0x800) | (static_cast<std::uint32_t>(op >> 5) << 8) | i.operand; i.flow = (op & 0x1F) == 0x14 ? Flow::Call : Flow::Jump; break;
        case Kind::Addr8:
        case Kind::Djnz: i.target = ((pc + 1) & 0xF00) | i.operand; i.flow = Flow::Branch; break;
        default: break;
    }
    if (op == 0x83 || op == 0x93) i.flow = Flow::Return;
    if (op == 0xB3) i.flow = Flow::Indirect;
    return i;
}

std::string text(const Insn& i, const AddressLabels& labels) {
    const auto address = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && !it->second.empty() ? it->second : std::format("${:03X}", a);
    };
    switch (i.entry->kind) {
        case Kind::None: return i.entry->text;
        case Kind::Imm: return i.entry->text + std::format("${:02X}", i.operand);
        case Kind::Djnz: return i.entry->text + address(*i.target);
        default: return i.entry->text + " " + address(*i.target);
    }
}

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    const std::string_view line = text::trim(line_in);
    const std::string raw = no_space(line), key = upper_of(raw);
    const Index& idx = index();
    const auto fail = [&](const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); };
    for (const auto& [name, op] : idx.exact) {
        if (key == name) return {static_cast<std::uint8_t>(op)};
    }
    for (const auto& [prefix, base] : idx.operand) {
        if (key.size() <= prefix.size() || !key.starts_with(prefix)) continue;
        const auto& e = *opcode_table()[base];
        const std::string_view operand = std::string_view(raw).substr(prefix.size());
        const std::int64_t v = asm_value::eval(operand, labels, lenient);
        switch (e.kind) {
            case Kind::Imm:
                if (!lenient && (v < -128 || v > 255)) fail("the immediate is a byte");
                return {static_cast<std::uint8_t>(base), static_cast<std::uint8_t>(v)};
            case Kind::Addr11:
                if (!lenient && (v < 0 || v > 0xFFF)) fail("the address is 12 bits");
                return {static_cast<std::uint8_t>(0x04 + (e.text == "CALL" ? 0x10 : 0) + (((static_cast<unsigned>(v) >> 8) & 7) << 5)), static_cast<std::uint8_t>(v)};
            default:
                if (!lenient && (static_cast<std::uint32_t>(v) & 0xF00) != ((pc + 1) & 0xF00)) fail("a conditional jump stays in its page");
                return {static_cast<std::uint8_t>(base), static_cast<std::uint8_t>(v)};
        }
    }
    fail("unknown instruction");
    return {};
}

}  // namespace cb::decompile::i8048
