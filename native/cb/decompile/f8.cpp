// cb-unity: alone
#include "cb/decompile/f8.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/decompile/asm_value.hpp"

namespace cb::decompile::f8 {

namespace {

using Table = std::array<std::optional<OpcodeEntry>, 256>;

std::string reg_name(unsigned r) { return r < 12 ? "R" + std::to_string(r) : r == 12 ? "S" : r == 13 ? "I" : "D"; }

std::string number_text(std::uint32_t v) { return v < 10 ? std::to_string(v) : std::format("${:X}", v); }

Table build() {
    Table t{};
    const auto put = [&](unsigned op, std::string text, Kind kind = Kind::None) { t[op] = OpcodeEntry{std::move(text), kind}; };
    const char* fixed[] = {"LR A,KU", "LR A,KL", "LR A,QU", "LR A,QL", "LR KU,A", "LR KL,A", "LR QU,A", "LR QL,A", "LR K,P", "LR P,K", "LR A,IS", "LR IS,A", "PK", "LR P0,Q", "LR Q,DC", "LR DC,Q",
                           "LR DC,H", "LR H,DC", "SR 1", "SL 1", "SR 4", "SL 4", "LM", "ST", "COM", "LNK", "DI", "EI", "POP", "LR W,J", "LR J,W", "INC"};
    for (unsigned op = 0; op < 32; ++op) put(op, fixed[op]);
    put(0x20, "LI ", Kind::Imm8);
    put(0x21, "NI ", Kind::Imm8);
    put(0x22, "OI ", Kind::Imm8);
    put(0x23, "XI ", Kind::Imm8);
    put(0x24, "AI ", Kind::Imm8);
    put(0x25, "CI ", Kind::Imm8);
    put(0x26, "IN ", Kind::Port8);
    put(0x27, "OUT ", Kind::Port8);
    put(0x28, "PI ", Kind::Addr16);
    put(0x29, "JMP ", Kind::Addr16);
    put(0x2A, "DCI ", Kind::Imm16);
    put(0x2B, "NOP");
    put(0x2C, "XDC");
    for (unsigned r = 0; r < 15; ++r) {
        put(0x30 + r, "DS " + reg_name(r));
        put(0x40 + r, "LR A," + reg_name(r));
        put(0x50 + r, "LR " + reg_name(r) + ",A");
        put(0xC0 + r, "AS " + reg_name(r));
        put(0xD0 + r, "ASD " + reg_name(r));
        put(0xE0 + r, "XS " + reg_name(r));
        put(0xF0 + r, "NS " + reg_name(r));
    }
    put(0x60, "LISU ", Kind::Low3);
    put(0x68, "LISL ", Kind::Low3);
    put(0x70, "LIS ", Kind::Low4);
    put(0xA0, "INS ", Kind::Low4);
    put(0xB0, "OUTS ", Kind::Low4);
    for (unsigned n = 0; n < 8; ++n) put(0x80 + n, "BT " + std::to_string(n) + ",", Kind::Rel8);
    const char* memory[] = {"AM", "AMD", "NM", "OM", "XM", "CM", "ADC"};
    for (unsigned k = 0; k < 7; ++k) put(0x88 + k, memory[k]);
    put(0x8F, "BR7 ", Kind::Rel8);
    put(0x90, "BR ", Kind::Rel8);
    for (unsigned n = 1; n < 16; ++n) put(0x90 + n, "BF " + std::to_string(n) + ",", Kind::Rel8);
    return t;
}

std::string no_space(std::string_view s) {
    std::string out;
    for (const char c : s) {
        if (c != ' ' && c != '\t') out.push_back(c);
    }
    return out;
}

struct Index {
    std::vector<std::pair<std::string, unsigned>> exact;
    std::vector<std::pair<std::string, unsigned>> operand;
};

const Index& index() {
    static const Index idx = [] {
        Index x;
        for (unsigned op = 0; op < 256; ++op) {
            const auto& e = opcode_table()[op];
            if (!e) continue;
            const std::string key = text::upper(no_space(e->text));
            if (e->kind == Kind::None) x.exact.emplace_back(key, op);
            else x.operand.emplace_back(key, op);
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
    unsigned op = code[offset];
    // the families whose low bits are an operand are in the table under their base opcode
    unsigned base = op, low = 0;
    if (!opcode_table()[op]) {
        if (op >= 0x60 && op < 0x68) { base = 0x60; low = op & 7; }
        else if (op >= 0x68 && op < 0x70) { base = 0x68; low = op & 7; }
        else if (op >= 0x70 && op < 0x80) { base = 0x70; low = op & 15; }
        else if (op >= 0xA0 && op < 0xB0) { base = 0xA0; low = op & 15; }
        else if (op >= 0xB0 && op < 0xC0) { base = 0xB0; low = op & 15; }
    }
    const auto& e = opcode_table()[base];
    if (!e) return std::nullopt;
    Insn i;
    i.at = pc;
    i.op = static_cast<std::uint8_t>(op);
    i.entry = &*e;
    i.raw[0] = static_cast<std::uint8_t>(op);
    switch (e->kind) {
        case Kind::None: i.size = 1; break;
        case Kind::Low3:
        case Kind::Low4: i.size = 1; i.value = low; break;
        case Kind::Imm8:
        case Kind::Port8:
        case Kind::Rel8: i.size = 2; break;
        default: i.size = 3; break;
    }
    if (offset + i.size > code.size()) return std::nullopt;
    for (unsigned k = 1; k < i.size; ++k) i.raw[k] = code[offset + k];
    switch (e->kind) {
        case Kind::Imm8:
        case Kind::Port8: i.value = i.raw[1]; break;
        case Kind::Rel8:
            i.value = i.raw[1];
            i.target = (pc + 1 + static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(i.raw[1])))) & 0xFFFF;
            i.flow = op == 0x90 ? Flow::Jump : op == 0x80 ? Flow::None : Flow::Branch;
            break;
        case Kind::Addr16:
        case Kind::Imm16:
            i.value = static_cast<std::uint32_t>(i.raw[1] << 8 | i.raw[2]);
            if (e->kind == Kind::Addr16) {
                i.target = i.value;
                i.flow = op == 0x28 ? Flow::Call : Flow::Jump;
            }
            break;
        default: break;
    }
    if (op == 0x1C) i.flow = Flow::Return;
    if (op == 0x0D) i.flow = Flow::Indirect;
    return i;
}

std::string text(const Insn& i, const AddressLabels& labels, bool label_constant) {
    const auto address = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && !it->second.empty() ? it->second : std::format("${:04X}", a);
    };
    const std::string& t = i.entry->text;
    switch (i.entry->kind) {
        case Kind::None: return t;
        case Kind::Imm8:
        case Kind::Port8: return t + std::format("${:02X}", i.value);
        case Kind::Low3:
        case Kind::Low4: return t + number_text(i.value);
        case Kind::Rel8:
        case Kind::Addr16: return t + address(*i.target);
        default: {
            const auto it = labels.find(i.value);
            return t + (label_constant && it != labels.end() && !it->second.empty() ? it->second : std::format("${:04X}", i.value));
        }
    }
}

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    const std::string_view line = text::trim(line_in);
    const std::string raw = no_space(line), key = text::upper(raw);
    const Index& idx = index();
    const auto fail = [&](const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); };
    for (const auto& [name, op] : idx.exact) {
        if (key == name) return {static_cast<std::uint8_t>(op)};
    }
    for (const auto& [prefix, op] : idx.operand) {
        if (key.size() <= prefix.size() || !key.starts_with(prefix)) continue;
        const auto& e = *opcode_table()[op];
        const std::int64_t v = asm_value::eval(std::string_view(raw).substr(prefix.size()), labels, lenient);
        switch (e.kind) {
            case Kind::Imm8:
                if (!lenient && (v < -128 || v > 255)) fail("the immediate is a byte");
                return {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(v)};
            case Kind::Port8:
                if (!lenient && (v < 0 || v > 255)) fail("a port is 0 to 255");
                return {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(v)};
            case Kind::Low3:
            case Kind::Low4: {
                const std::int64_t max = e.kind == Kind::Low3 ? 7 : 15;
                if (!lenient && (v < 0 || v > max)) fail(std::format("the value is 0 to {}", max));
                return {static_cast<std::uint8_t>(op + (v & max))};
            }
            case Kind::Rel8: {
                const std::int64_t d = v - (static_cast<std::int64_t>(pc) + 1);
                if (!lenient && (d < -128 || d > 127)) fail("the branch is out of reach");
                return {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(d)};
            }
            default:
                if (!lenient && (v < 0 || v > 0xFFFF)) fail("an address is 16 bits");
                return {static_cast<std::uint8_t>(op), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)};
        }
    }
    fail("unknown instruction");
    return {};
}

}  // namespace cb::decompile::f8
