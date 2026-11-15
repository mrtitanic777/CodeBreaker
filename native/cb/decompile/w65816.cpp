#include "cb/decompile/w65816.hpp"

#include <cctype>
#include <format>
#include <unordered_map>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::w65816 {

namespace {

constexpr std::size_t kModeCount = 26;

struct Form {
    std::uint8_t op;
    std::string_view mnemonic;
    Mode mode;
};

// The instructions that are not in the eight accumulator groups, by opcode within each row of the opcode map.
constexpr Form kForms[] = {
    {0x00, "BRK", Mode::Imm8}, {0x02, "COP", Mode::Imm8}, {0x04, "TSB", Mode::Dp}, {0x06, "ASL", Mode::Dp}, {0x08, "PHP", Mode::Imp}, {0x0A, "ASL", Mode::Acc},
    {0x0B, "PHD", Mode::Imp}, {0x0C, "TSB", Mode::Abs}, {0x0E, "ASL", Mode::Abs},
    {0x10, "BPL", Mode::Rel}, {0x14, "TRB", Mode::Dp}, {0x16, "ASL", Mode::DpX}, {0x18, "CLC", Mode::Imp}, {0x1A, "INC", Mode::Acc}, {0x1B, "TCS", Mode::Imp},
    {0x1C, "TRB", Mode::Abs}, {0x1E, "ASL", Mode::AbsX},
    {0x20, "JSR", Mode::Abs}, {0x22, "JSL", Mode::Long}, {0x24, "BIT", Mode::Dp}, {0x26, "ROL", Mode::Dp}, {0x28, "PLP", Mode::Imp}, {0x2A, "ROL", Mode::Acc},
    {0x2B, "PLD", Mode::Imp}, {0x2C, "BIT", Mode::Abs}, {0x2E, "ROL", Mode::Abs},
    {0x30, "BMI", Mode::Rel}, {0x34, "BIT", Mode::DpX}, {0x36, "ROL", Mode::DpX}, {0x38, "SEC", Mode::Imp}, {0x3A, "DEC", Mode::Acc}, {0x3B, "TSC", Mode::Imp},
    {0x3C, "BIT", Mode::AbsX}, {0x3E, "ROL", Mode::AbsX},
    {0x40, "RTI", Mode::Imp}, {0x42, "WDM", Mode::Imm8}, {0x44, "MVP", Mode::Block}, {0x46, "LSR", Mode::Dp}, {0x48, "PHA", Mode::Imp}, {0x4A, "LSR", Mode::Acc},
    {0x4B, "PHK", Mode::Imp}, {0x4C, "JMP", Mode::Abs}, {0x4E, "LSR", Mode::Abs},
    {0x50, "BVC", Mode::Rel}, {0x54, "MVN", Mode::Block}, {0x56, "LSR", Mode::DpX}, {0x58, "CLI", Mode::Imp}, {0x5A, "PHY", Mode::Imp}, {0x5B, "TCD", Mode::Imp},
    {0x5C, "JML", Mode::Long}, {0x5E, "LSR", Mode::AbsX},
    {0x60, "RTS", Mode::Imp}, {0x62, "PER", Mode::RelLong}, {0x64, "STZ", Mode::Dp}, {0x66, "ROR", Mode::Dp}, {0x68, "PLA", Mode::Imp}, {0x6A, "ROR", Mode::Acc},
    {0x6B, "RTL", Mode::Imp}, {0x6C, "JMP", Mode::AbsInd}, {0x6E, "ROR", Mode::Abs},
    {0x70, "BVS", Mode::Rel}, {0x74, "STZ", Mode::DpX}, {0x76, "ROR", Mode::DpX}, {0x78, "SEI", Mode::Imp}, {0x7A, "PLY", Mode::Imp}, {0x7B, "TDC", Mode::Imp},
    {0x7C, "JMP", Mode::AbsIndX}, {0x7E, "ROR", Mode::AbsX},
    {0x80, "BRA", Mode::Rel}, {0x82, "BRL", Mode::RelLong}, {0x84, "STY", Mode::Dp}, {0x86, "STX", Mode::Dp}, {0x88, "DEY", Mode::Imp}, {0x89, "BIT", Mode::ImmM},
    {0x8A, "TXA", Mode::Imp}, {0x8B, "PHB", Mode::Imp}, {0x8C, "STY", Mode::Abs}, {0x8E, "STX", Mode::Abs},
    {0x90, "BCC", Mode::Rel}, {0x94, "STY", Mode::DpX}, {0x96, "STX", Mode::DpY}, {0x98, "TYA", Mode::Imp}, {0x9A, "TXS", Mode::Imp}, {0x9B, "TXY", Mode::Imp},
    {0x9C, "STZ", Mode::Abs}, {0x9E, "STZ", Mode::AbsX},
    {0xA0, "LDY", Mode::ImmX}, {0xA2, "LDX", Mode::ImmX}, {0xA4, "LDY", Mode::Dp}, {0xA6, "LDX", Mode::Dp}, {0xA8, "TAY", Mode::Imp}, {0xAA, "TAX", Mode::Imp},
    {0xAB, "PLB", Mode::Imp}, {0xAC, "LDY", Mode::Abs}, {0xAE, "LDX", Mode::Abs},
    {0xB0, "BCS", Mode::Rel}, {0xB4, "LDY", Mode::DpX}, {0xB6, "LDX", Mode::DpY}, {0xB8, "CLV", Mode::Imp}, {0xBA, "TSX", Mode::Imp}, {0xBB, "TYX", Mode::Imp},
    {0xBC, "LDY", Mode::AbsX}, {0xBE, "LDX", Mode::AbsY},
    {0xC0, "CPY", Mode::ImmX}, {0xC2, "REP", Mode::Imm8}, {0xC4, "CPY", Mode::Dp}, {0xC6, "DEC", Mode::Dp}, {0xC8, "INY", Mode::Imp}, {0xCA, "DEX", Mode::Imp},
    {0xCB, "WAI", Mode::Imp}, {0xCC, "CPY", Mode::Abs}, {0xCE, "DEC", Mode::Abs},
    {0xD0, "BNE", Mode::Rel}, {0xD4, "PEI", Mode::DpInd}, {0xD6, "DEC", Mode::DpX}, {0xD8, "CLD", Mode::Imp}, {0xDA, "PHX", Mode::Imp}, {0xDB, "STP", Mode::Imp},
    {0xDC, "JML", Mode::AbsIndL}, {0xDE, "DEC", Mode::AbsX},
    {0xE0, "CPX", Mode::ImmX}, {0xE2, "SEP", Mode::Imm8}, {0xE4, "CPX", Mode::Dp}, {0xE6, "INC", Mode::Dp}, {0xE8, "INX", Mode::Imp}, {0xEA, "NOP", Mode::Imp},
    {0xEB, "XBA", Mode::Imp}, {0xEC, "CPX", Mode::Abs}, {0xEE, "INC", Mode::Abs},
    {0xF0, "BEQ", Mode::Rel}, {0xF4, "PEA", Mode::Abs}, {0xF6, "INC", Mode::DpX}, {0xF8, "SED", Mode::Imp}, {0xFA, "PLX", Mode::Imp}, {0xFB, "XCE", Mode::Imp},
    {0xFC, "JSR", Mode::AbsIndX}, {0xFE, "INC", Mode::AbsX},
};

// The eight accumulator groups: the base opcode of each, and the forms by their offset from it.
struct Group {
    std::string_view mnemonic;
    std::uint8_t base;
};
constexpr Group kGroups[] = {{"ORA", 0x00}, {"AND", 0x20}, {"EOR", 0x40}, {"ADC", 0x60}, {"STA", 0x80}, {"LDA", 0xA0}, {"CMP", 0xC0}, {"SBC", 0xE0}};
struct GroupForm {
    std::uint8_t offset;
    Mode mode;
};
constexpr GroupForm kGroupForms[] = {{0x01, Mode::DpIndX}, {0x03, Mode::Sr},      {0x05, Mode::Dp},   {0x07, Mode::DpIndL}, {0x09, Mode::ImmM},
                                     {0x0D, Mode::Abs},    {0x0F, Mode::Long},    {0x11, Mode::DpIndY}, {0x12, Mode::DpInd}, {0x13, Mode::SrIndY},
                                     {0x15, Mode::DpX},    {0x17, Mode::DpIndLY}, {0x19, Mode::AbsY}, {0x1D, Mode::AbsX},   {0x1F, Mode::LongX}};

struct Tables {
    std::array<OpcodeEntry, 256> by_opcode{};
    std::unordered_map<std::string_view, std::array<int, kModeCount>> by_mnemonic;

    void add(std::uint8_t op, std::string_view mnemonic, Mode mode) {
        by_opcode[op] = OpcodeEntry{mnemonic, mode};
        auto [it, added] = by_mnemonic.try_emplace(mnemonic);
        if (added) it->second.fill(-1);
        it->second[static_cast<std::size_t>(mode)] = op;
    }

    Tables() {
        for (const Form& f : kForms) add(f.op, f.mnemonic, f.mode);
        for (const Group& g : kGroups) {
            for (const GroupForm& f : kGroupForms) {
                if (g.mnemonic == "STA" && f.mode == Mode::ImmM) continue;   // (0x89 is BIT #)
                add(static_cast<std::uint8_t>(g.base + f.offset), g.mnemonic, f.mode);
            }
        }
    }
};

const Tables& tables() {
    static const Tables t;
    return t;
}

[[noreturn]] void bad(std::string_view line, const std::string& what) { throw ParseError("", 0, 0, std::format("{} ({})", what, line)); }

// A number or label as the assembler reads it: its value, and how the way it is written forces a size.
struct Value {
    std::int64_t v = 0;
    enum class Force : std::uint8_t { None, Dp, Abs, Long } force = Force::None;
};

Value value_of(std::string_view s, const NameLabels& labels, std::string_view line) {
    s = text::trim(s);
    Value out;
    if (s.starts_with("a:")) {
        out.force = Value::Force::Abs;
        s.remove_prefix(2);
    } else if (s.starts_with("z:")) {
        out.force = Value::Force::Dp;
        s.remove_prefix(2);
    } else if (s.starts_with("f:")) {
        out.force = Value::Force::Long;
        s.remove_prefix(2);
    }
    s = text::trim(s);
    bool low_word = false;
    if (s.starts_with(".loword(") && s.ends_with(")")) {
        low_word = true;
        s = text::trim(s.substr(8, s.size() - 9));
    }
    if (s.starts_with("$")) {
        const auto v = text::parse_int<std::int64_t>(s.substr(1), 16);
        if (!v) bad(line, "bad hex number");
        out.v = *v;
    } else if (const auto it = labels.find(s); it != labels.end()) {
        out.v = it->second;
    } else {
        const auto v = text::parse_int<std::int64_t>(s, 0);
        if (!v) bad(line, "unknown label or number " + std::string(s));
        out.v = *v;
    }
    if (low_word) out.v &= 0xFFFF;
    return out;
}

// The items of an operand list: split at commas outside brackets.
std::vector<std::string_view> items_of(std::string_view args) {
    std::vector<std::string_view> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t i = 0; i <= args.size(); ++i) {
        if (i < args.size() && (args[i] == '(' || args[i] == '[')) ++depth;
        if (i < args.size() && (args[i] == ')' || args[i] == ']')) --depth;
        if (i == args.size() || (args[i] == ',' && depth == 0)) {
            out.push_back(text::trim(args.substr(from, i - from)));
            from = i + 1;
        }
    }
    if (out.size() == 1 && out[0].empty()) out.clear();
    return out;
}

}  // namespace

const std::array<OpcodeEntry, 256>& opcode_table() { return tables().by_opcode; }

std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode) {
    const auto it = tables().by_mnemonic.find(mnemonic);
    if (it == tables().by_mnemonic.end()) return std::nullopt;
    const int op = it->second[static_cast<std::size_t>(mode)];
    if (op < 0) return std::nullopt;
    return static_cast<std::uint8_t>(op);
}

bool is_mnemonic(std::string_view mnemonic) { return tables().by_mnemonic.contains(mnemonic); }

bool is_branch(std::string_view m) {
    return m == "BCC" || m == "BCS" || m == "BEQ" || m == "BMI" || m == "BNE" || m == "BPL" || m == "BVC" || m == "BVS" || m == "BRA" || m == "BRL" || m == "PER";
}

bool ends_flow(std::string_view m) {
    return m == "JMP" || m == "JML" || m == "RTS" || m == "RTL" || m == "RTI" || m == "BRA" || m == "BRL" || m == "BRK" || m == "COP" || m == "STP";
}

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc, bool m16, bool x16) {
    if (offset >= code.size()) return std::nullopt;
    const OpcodeEntry& entry = tables().by_opcode[code[offset]];
    Insn insn;
    insn.at = pc;
    insn.op = code[offset];
    insn.mnemonic = entry.mnemonic;
    insn.mode = entry.mode;
    insn.m16 = m16;
    insn.x16 = x16;
    insn.size = instruction_size(entry.mode, m16, x16);
    if (offset + insn.size > code.size()) return std::nullopt;
    const auto byte = [&](std::size_t i) { return static_cast<std::uint32_t>(code[offset + i]); };
    if (insn.size == 2) insn.operand = byte(1);
    else if (insn.size == 3) insn.operand = byte(1) | byte(2) << 8;
    else if (insn.size == 4) insn.operand = byte(1) | byte(2) << 8 | byte(3) << 16;
    if (insn.mode == Mode::Block) {
        insn.operand = byte(1);    // the destination bank
        insn.operand2 = byte(2);   // the source bank
    }
    const std::uint32_t bank = pc & 0xFF0000;
    switch (insn.mode) {
        case Mode::Rel: insn.target = bank | ((pc + 2 + static_cast<std::uint32_t>(static_cast<std::int8_t>(insn.operand))) & 0xFFFF); break;
        case Mode::RelLong: insn.target = bank | ((pc + 3 + static_cast<std::uint32_t>(static_cast<std::int16_t>(insn.operand))) & 0xFFFF); break;
        case Mode::Abs:
            if (insn.mnemonic == "JMP" || insn.mnemonic == "JSR") insn.target = bank | insn.operand;
            break;
        case Mode::Long:
            if (insn.mnemonic == "JML" || insn.mnemonic == "JSL") insn.target = insn.operand;
            break;
        default: break;
    }
    return insn;
}

std::string text(const Insn& insn, const AddressLabels& labels, const AddressLabels& direct_page) {
    const std::string m(insn.mnemonic);
    const auto name_of = [&](const AddressLabels& table, std::uint32_t key) -> const std::string* {
        const auto it = table.find(key);
        return it != table.end() && !it->second.empty() ? &it->second : nullptr;
    };
    const auto dp = [&](std::uint32_t v) { return name_of(direct_page, v) ? *name_of(direct_page, v) : std::format("${:02X}", v); };
    // a 16-bit data address: a name or the number; `a:` keeps a value below $100 absolute
    const auto abs = [&](std::uint32_t v) -> std::string {
        if (const std::string* n = name_of(labels, v)) return v < 0x100 ? "a:" + *n : *n;
        return v < 0x100 ? std::format("a:${:04X}", v) : std::format("${:04X}", v);
    };
    const auto lng = [&](std::uint32_t v) -> std::string {
        if (const std::string* n = name_of(labels, v)) return "f:" + *n;   // (a name's size is not its value's: the first pass does not know it yet)
        return v < 0x10000 ? std::format("f:${:06X}", v) : std::format("${:06X}", v);
    };
    const auto jump = [&](std::uint32_t operand) -> std::string {   // JMP and JSR name their target's 24-bit address
        const std::uint32_t key = insn.target ? *insn.target : operand;
        if (const std::string* n = name_of(labels, key)) return *n;
        return std::format("${:04X}", operand);
    };
    const std::uint32_t v = insn.operand;
    switch (insn.mode) {
        case Mode::Imp: return m;
        case Mode::Acc: return m + " a";
        case Mode::Imm8: return std::format("{} #${:02X}", m, v);
        case Mode::ImmM: return insn.m16 ? std::format("{} #${:04X}", m, v) : std::format("{} #${:02X}", m, v);
        case Mode::ImmX: return insn.x16 ? std::format("{} #${:04X}", m, v) : std::format("{} #${:02X}", m, v);
        case Mode::Dp: return m + " " + dp(v);
        case Mode::DpX: return m + " " + dp(v) + ",x";
        case Mode::DpY: return m + " " + dp(v) + ",y";
        case Mode::Abs: return insn.target || insn.mnemonic == "PEA" ? m + " " + jump(v) : m + " " + abs(v);
        case Mode::AbsX: return m + " " + abs(v) + ",x";
        case Mode::AbsY: return m + " " + abs(v) + ",y";
        case Mode::Long:
            if (insn.target) {   // JSL and JML have only the long form: no `f:` to say so
                if (const std::string* n = name_of(labels, v)) return m + " " + *n;
                return std::format("{} ${:06X}", m, v);
            }
            return m + " " + lng(v);
        case Mode::LongX: return m + " " + lng(v) + ",x";
        case Mode::AbsInd: return m + " (" + abs(v) + ")";
        case Mode::AbsIndX: return m + " (" + abs(v) + ",x)";
        case Mode::AbsIndL: return m + " [" + abs(v) + "]";
        case Mode::DpInd: return m + " (" + dp(v) + ")";
        case Mode::DpIndX: return m + " (" + dp(v) + ",x)";
        case Mode::DpIndY: return m + " (" + dp(v) + "),y";
        case Mode::DpIndL: return m + " [" + dp(v) + "]";
        case Mode::DpIndLY: return m + " [" + dp(v) + "],y";
        case Mode::Rel:
        case Mode::RelLong: {
            const std::uint32_t at = *insn.target;
            if (const std::string* n = name_of(labels, at)) return m + " " + *n;
            return std::format("{} ${:06X}", m, at);
        }
        case Mode::Sr: return std::format("{} ${:02X},s", m, v);
        case Mode::SrIndY: return std::format("{} (${:02X},s),y", m, v);
        case Mode::Block: return std::format("{} ${:02X}, ${:02X}", m, insn.operand2, insn.operand);   // source, destination
    }
    return m;
}

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, bool a16, bool i16) {
    std::string_view rest = text::ltrim(line);
    const bool three_letters = rest.size() >= 3 && std::isalpha(static_cast<unsigned char>(rest[0])) && std::isalpha(static_cast<unsigned char>(rest[1])) &&
                               std::isalpha(static_cast<unsigned char>(rest[2]));
    if (!three_letters || (rest.size() > 3 && !text::is_space(rest[3]) && rest[3] != ';')) bad(line, "not an instruction");
    const std::string mnemonic = text::upper(rest.substr(0, 3));
    rest.remove_prefix(3);
    if (const std::size_t comment = rest.find(';'); comment != std::string_view::npos) rest = rest.substr(0, comment);
    const std::string_view args = text::trim(rest);
    const auto forms = tables().by_mnemonic.find(mnemonic);
    if (forms == tables().by_mnemonic.end()) bad(line, "unknown instruction " + mnemonic);
    const auto has = [&](Mode mode) { return forms->second[static_cast<std::size_t>(mode)] >= 0; };
    const auto op_of = [&](Mode mode) -> std::uint8_t {
        if (!has(mode)) bad(line, mnemonic + " has no " + std::to_string(static_cast<int>(mode)) + " form");
        return static_cast<std::uint8_t>(forms->second[static_cast<std::size_t>(mode)]);
    };
    const auto value = [&](std::string_view s) { return value_of(s, labels, line); };
    const auto fits = [&](std::int64_t v, std::int64_t max) {
        if (v < 0 || v > max) bad(line, std::format("the value {} does not fit", v));
    };
    Bytes out;
    const auto put = [&](std::uint8_t op, std::int64_t v, unsigned bytes) {
        out.push_back(op);
        for (unsigned i = 0; i < bytes; ++i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i) & 0xFF));
    };

    if (is_branch(mnemonic)) {
        const Mode mode = has(Mode::Rel) ? Mode::Rel : Mode::RelLong;
        const unsigned length = mode == Mode::Rel ? 2 : 3;
        const std::int64_t target = value(args).v;
        std::int64_t d = ((target & 0xFFFF) - ((static_cast<std::int64_t>(pc) + length) & 0xFFFF)) & 0xFFFF;   // within the bank
        if (d >= 0x8000) d -= 0x10000;
        if (mode == Mode::Rel && (d < -128 || d > 127)) bad(line, "branch out of reach");
        put(op_of(mode), d & (mode == Mode::Rel ? 0xFF : 0xFFFF), length - 1);
        return out;
    }
    if (has(Mode::Block)) {   // MVN src, dst: the opcode, the destination bank, the source bank
        const std::vector<std::string_view> items = items_of(args);
        if (items.size() != 2) bad(line, mnemonic + " takes a source and a destination bank");
        const std::int64_t src = value(items[0]).v, dst = value(items[1]).v;
        fits(src, 0xFF);
        fits(dst, 0xFF);
        return {op_of(Mode::Block), static_cast<std::uint8_t>(dst), static_cast<std::uint8_t>(src)};
    }
    const std::string low = text::lower(args);
    std::string squeezed;   // lower case without spaces
    for (const char c : low) {
        if (c != ' ' && c != '\t') squeezed += c;
    }
    if (args.empty()) {
        if (has(Mode::Imp)) return {op_of(Mode::Imp)};
        if (has(Mode::Acc)) return {op_of(Mode::Acc)};
        bad(line, mnemonic + " needs an operand");
    }
    if (squeezed == "a" && has(Mode::Acc)) return {op_of(Mode::Acc)};
    if (args.starts_with("#")) {
        const Value v = value(args.substr(1));
        if (has(Mode::Imm8)) {
            fits(v.v, 0xFF);
            put(op_of(Mode::Imm8), v.v, 1);
        } else if (has(Mode::ImmM) || has(Mode::ImmX)) {
            const bool wide = has(Mode::ImmM) ? a16 : i16;
            fits(v.v, wide ? 0xFFFF : 0xFF);
            put(op_of(has(Mode::ImmM) ? Mode::ImmM : Mode::ImmX), v.v, wide ? 2 : 1);
        } else {
            bad(line, mnemonic + " has no immediate form");
        }
        return out;
    }
    // the bracketed forms: the operand is what is inside
    const auto inside = [&](char open, char close) -> std::string_view {
        const std::size_t a = args.find(open), z = args.rfind(close);
        return a == std::string_view::npos || z == std::string_view::npos || z < a ? std::string_view() : text::trim(args.substr(a + 1, z - a - 1));
    };
    if (squeezed.starts_with("(") && squeezed.ends_with(",s),y")) {
        const std::string_view body = inside('(', ')');
        const std::size_t comma = body.rfind(',');
        const Value v = value(body.substr(0, comma));
        fits(v.v, 0xFF);
        put(op_of(Mode::SrIndY), v.v, 1);
        return out;
    }
    if (squeezed.ends_with(",s")) {
        const Value v = value(args.substr(0, args.rfind(',')));
        fits(v.v, 0xFF);
        put(op_of(Mode::Sr), v.v, 1);
        return out;
    }
    if (squeezed.starts_with("(") && squeezed.ends_with("),y")) {
        const Value v = value(inside('(', ')'));
        fits(v.v, 0xFF);
        put(op_of(Mode::DpIndY), v.v, 1);
        return out;
    }
    if (squeezed.starts_with("(") && squeezed.ends_with(",x)")) {
        const std::string_view body = inside('(', ')');
        const Value v = value(body.substr(0, body.rfind(',')));
        if (has(Mode::AbsIndX)) {
            fits(v.v, 0xFFFF);
            put(op_of(Mode::AbsIndX), v.v, 2);
        } else {
            fits(v.v, 0xFF);
            put(op_of(Mode::DpIndX), v.v, 1);
        }
        return out;
    }
    if (squeezed.starts_with("(") && squeezed.ends_with(")")) {
        const Value v = value(inside('(', ')'));
        if (has(Mode::AbsInd)) {
            fits(v.v, 0xFFFF);
            put(op_of(Mode::AbsInd), v.v, 2);
        } else {
            fits(v.v, 0xFF);
            put(op_of(Mode::DpInd), v.v, 1);
        }
        return out;
    }
    if (squeezed.starts_with("[") && squeezed.ends_with("],y")) {
        const Value v = value(inside('[', ']'));
        fits(v.v, 0xFF);
        put(op_of(Mode::DpIndLY), v.v, 1);
        return out;
    }
    if (squeezed.starts_with("[") && squeezed.ends_with("]")) {
        const Value v = value(inside('[', ']'));
        if (has(Mode::AbsIndL)) {
            fits(v.v, 0xFFFF);
            put(op_of(Mode::AbsIndL), v.v, 2);
        } else {
            fits(v.v, 0xFF);
            put(op_of(Mode::DpIndL), v.v, 1);
        }
        return out;
    }
    // an address, with an index register or none
    const bool x = squeezed.ends_with(",x"), y = squeezed.ends_with(",y");
    const Value v = value(x || y ? args.substr(0, args.rfind(',')) : args);
    const Mode dp = x ? Mode::DpX : y ? Mode::DpY : Mode::Dp;
    const Mode ab = x ? Mode::AbsX : y ? Mode::AbsY : Mode::Abs;
    const Mode lg = x ? Mode::LongX : Mode::Long;
    if (v.v < 0 || v.v > 0xFFFFFF) bad(line, "the value does not fit an address");
    Mode mode = ab;
    if (v.force == Value::Force::Long || v.v > 0xFFFF) mode = lg;
    else if (v.force == Value::Force::Abs) mode = ab;
    else if (v.force == Value::Force::Dp || v.v <= 0xFF) mode = has(dp) ? dp : ab;
    if (!has(mode)) {   // JSR and JMP have only the absolute form, JSL and JML only the long one
        if (mode == lg && !has(lg)) bad(line, mnemonic + " has no long form");
        if (mode == ab) mode = has(lg) && !has(ab) ? lg : mode;
        if (!has(mode)) bad(line, mnemonic + " has no such form");
    }
    const unsigned size = mode == dp ? 1 : mode == ab ? 2 : 3;
    fits(v.v, size == 1 ? 0xFF : size == 2 ? 0xFFFF : 0xFFFFFF);
    put(op_of(mode), v.v, size);
    return out;
}

}  // namespace cb::decompile::w65816
