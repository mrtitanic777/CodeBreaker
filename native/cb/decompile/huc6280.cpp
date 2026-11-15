#include "cb/decompile/huc6280.hpp"

#include <cctype>
#include <format>
#include <unordered_map>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/m6502.hpp"

namespace cb::decompile::huc6280 {

namespace m6502 = decompile::m6502;

namespace {

constexpr std::size_t kModeCount = 21;

struct Extra {
    std::string_view mnemonic;
    Mode mode;
    std::uint8_t op;
};

constexpr Extra kExtras[] = {
    {"SXY", Mode::Imp, 0x02}, {"SAX", Mode::Imp, 0x22}, {"SAY", Mode::Imp, 0x42},
    {"ST0", Mode::Imm, 0x03}, {"ST1", Mode::Imm, 0x13}, {"ST2", Mode::Imm, 0x23},
    {"TMA", Mode::Imm, 0x43}, {"TAM", Mode::Imm, 0x53}, {"BSR", Mode::Rel, 0x44},
    {"CSL", Mode::Imp, 0x54}, {"CSH", Mode::Imp, 0xD4}, {"CLA", Mode::Imp, 0x62},
    {"CLX", Mode::Imp, 0x82}, {"CLY", Mode::Imp, 0xC2}, {"SET", Mode::Imp, 0xF4},
    {"TII", Mode::Block, 0x73}, {"TDD", Mode::Block, 0xC3}, {"TIN", Mode::Block, 0xD3}, {"TIA", Mode::Block, 0xE3}, {"TAI", Mode::Block, 0xF3},
    {"TST", Mode::ImmZp, 0x83}, {"TST", Mode::ImmAbs, 0x93}, {"TST", Mode::ImmZpx, 0xA3}, {"TST", Mode::ImmAbx, 0xB3},
};

constexpr std::string_view kBitNames[4][8] = {
    {"RMB0", "RMB1", "RMB2", "RMB3", "RMB4", "RMB5", "RMB6", "RMB7"},
    {"SMB0", "SMB1", "SMB2", "SMB3", "SMB4", "SMB5", "SMB6", "SMB7"},
    {"BBR0", "BBR1", "BBR2", "BBR3", "BBR4", "BBR5", "BBR6", "BBR7"},
    {"BBS0", "BBS1", "BBS2", "BBS3", "BBS4", "BBS5", "BBS6", "BBS7"},
};

struct Tables {
    std::array<std::optional<OpcodeEntry>, 256> by_opcode;
    std::unordered_map<std::string_view, std::array<int, kModeCount>> by_mnemonic;

    void add(std::string_view mnemonic, Mode mode, std::uint8_t op) {
        by_opcode[op] = OpcodeEntry{mnemonic, mode};
        auto [it, added] = by_mnemonic.try_emplace(mnemonic);
        if (added) it->second.fill(-1);
        it->second[static_cast<std::size_t>(mode)] = op;
    }

    Tables() {
        const auto& base = m6502::opcode_table(m6502::Variant::Cmos);   // the 65C02 half
        for (std::size_t op = 0; op < 256; ++op) {
            if (base[op]) add(base[op]->mnemonic, static_cast<Mode>(base[op]->mode), static_cast<std::uint8_t>(op));
        }
        for (const Extra& e : kExtras) add(e.mnemonic, e.mode, e.op);
        for (std::uint8_t bit = 0; bit < 8; ++bit) {
            add(kBitNames[0][bit], Mode::Zp, static_cast<std::uint8_t>(0x07 + 0x10 * bit));
            add(kBitNames[1][bit], Mode::Zp, static_cast<std::uint8_t>(0x87 + 0x10 * bit));
            add(kBitNames[2][bit], Mode::ZpRel, static_cast<std::uint8_t>(0x0F + 0x10 * bit));
            add(kBitNames[3][bit], Mode::ZpRel, static_cast<std::uint8_t>(0x8F + 0x10 * bit));
        }
    }
};

const Tables& tables() {
    static const Tables t;
    return t;
}

bool is_bit_branch(std::string_view m) { return m.size() == 4 && (m.starts_with("BBR") || m.starts_with("BBS")); }

[[noreturn]] void bad(std::string_view line, const std::string& what) { throw ParseError("", 0, 0, std::format("{} ({})", what, line)); }

struct Value {
    std::int64_t v;
    bool wide;   // an `a:` prefix, four hex digits or a value past one byte
};

Value value_of(std::string_view s, const NameLabels& labels, std::string_view line) {
    s = text::trim(s);
    const bool forced = s.starts_with("a:");
    if (forced) s.remove_prefix(2);
    if (s.starts_with("z:")) s.remove_prefix(2);
    if (s.starts_with("$")) {
        const auto v = text::parse_int<std::int64_t>(s.substr(1), 16);
        if (!v) bad(line, "bad hex number");
        return {*v, forced || s.size() > 3};
    }
    if (const auto it = labels.find(s); it != labels.end()) return {it->second, forced || it->second > 0xFF};
    const auto v = text::parse_int<std::int64_t>(s, 0);
    if (!v) bad(line, "unknown label or number " + std::string(s));
    return {*v, forced || *v > 0xFF};
}

// The items of an operand list: split at commas outside parentheses.
std::vector<std::string_view> items_of(std::string_view args) {
    std::vector<std::string_view> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t i = 0; i <= args.size(); ++i) {
        if (i < args.size() && args[i] == '(') ++depth;
        if (i < args.size() && args[i] == ')') --depth;
        if (i == args.size() || (args[i] == ',' && depth == 0)) {
            out.push_back(text::trim(args.substr(from, i - from)));
            from = i + 1;
        }
    }
    if (out.size() == 1 && out[0].empty()) out.clear();
    return out;
}

}  // namespace

const std::array<std::optional<OpcodeEntry>, 256>& opcode_table() { return tables().by_opcode; }

std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode) {
    const auto it = tables().by_mnemonic.find(mnemonic);
    if (it == tables().by_mnemonic.end()) return std::nullopt;
    const int op = it->second[static_cast<std::size_t>(mode)];
    if (op < 0) return std::nullopt;
    return static_cast<std::uint8_t>(op);
}

bool is_mnemonic(std::string_view mnemonic) { return tables().by_mnemonic.contains(mnemonic); }

bool is_branch(std::string_view m) { return m6502::is_branch(m) || m == "BSR"; }

bool ends_flow(std::string_view m) { return m6502::ends_flow(m); }

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset >= code.size()) return std::nullopt;
    const std::optional<OpcodeEntry>& entry = tables().by_opcode[code[offset]];
    if (!entry) return std::nullopt;
    Insn insn;
    insn.at = pc;
    insn.op = code[offset];
    insn.mnemonic = entry->mnemonic;
    insn.mode = entry->mode;
    insn.size = instruction_size(entry->mode);
    if (offset + insn.size > code.size()) return std::nullopt;
    const auto byte = [&](std::size_t i) { return static_cast<std::uint16_t>(code[offset + i]); };
    const auto word = [&](std::size_t i) { return static_cast<std::uint16_t>(code[offset + i] | code[offset + i + 1] << 8); };
    switch (insn.mode) {
        case Mode::Imp:
        case Mode::Acc: break;
        case Mode::ZpRel:
            insn.operand = {byte(1), byte(2), 0};
            insn.operands = 2;
            insn.target = static_cast<std::uint16_t>((pc + 3 + static_cast<std::uint32_t>(static_cast<std::int8_t>(byte(2)))) & 0xFFFF);
            break;
        case Mode::ImmZp:
        case Mode::ImmZpx:
            insn.operand = {byte(1), byte(2), 0};
            insn.operands = 2;
            break;
        case Mode::ImmAbs:
        case Mode::ImmAbx:
            insn.operand = {byte(1), word(2), 0};
            insn.operands = 2;
            break;
        case Mode::Block:
            insn.operand = {word(1), word(3), word(5)};
            insn.operands = 3;
            break;
        default:
            insn.operand[0] = insn.size == 2 ? byte(1) : word(1);
            insn.operands = 1;
            break;
    }
    if (insn.mode == Mode::Rel) {
        insn.target = static_cast<std::uint16_t>((pc + 2 + static_cast<std::uint32_t>(static_cast<std::int8_t>(insn.operand[0]))) & 0xFFFF);
    } else if ((insn.mnemonic == "JMP" || insn.mnemonic == "JSR") && insn.mode == Mode::Abs) {
        insn.target = insn.operand[0];
    }
    return insn;
}

std::string text(const Insn& insn, const AddressLabels& labels, const AddressLabels& zero_page) {
    const std::string m(insn.mnemonic);
    const auto abs = [&](std::uint16_t v, bool small_is_absolute) -> std::string {
        if (const auto it = labels.find(v); it != labels.end() && !it->second.empty()) return small_is_absolute && v < 0x100 ? "a:" + it->second : it->second;
        return small_is_absolute && v < 0x100 ? std::format("a:${:04X}", v) : std::format("${:04X}", v);
    };
    const auto zp = [&](std::uint16_t v) -> std::string {
        if (const auto it = zero_page.find(v); it != zero_page.end() && !it->second.empty()) return it->second;
        return std::format("${:02X}", v);
    };
    const std::uint16_t v = insn.operand[0];
    switch (insn.mode) {
        case Mode::Imp: return m;
        case Mode::Acc: return m + " a";
        case Mode::Imm: return std::format("{} #${:02X}", m, v);
        case Mode::Rel: return m + " " + abs(*insn.target, false);
        case Mode::Zp: return m + " " + zp(v);
        case Mode::Zpx: return m + " " + zp(v) + ",x";
        case Mode::Zpy: return m + " " + zp(v) + ",y";
        case Mode::Abs: return m + " " + abs(v, true);
        case Mode::Abx: return m + " " + abs(v, true) + ",x";
        case Mode::Aby: return m + " " + abs(v, true) + ",y";
        case Mode::Ind: return m + " (" + abs(v, true) + ")";
        case Mode::Iax: return m + " (" + abs(v, true) + ",x)";
        case Mode::Izp: return m + " (" + zp(v) + ")";
        case Mode::Izx: return m + " (" + zp(v) + ",x)";
        case Mode::Izy: return m + " (" + zp(v) + "),y";
        case Mode::ZpRel: return m + " " + zp(v) + ", " + abs(*insn.target, false);
        case Mode::ImmZp: return std::format("{} #${:02X}, ", m, v) + zp(insn.operand[1]);
        case Mode::ImmZpx: return std::format("{} #${:02X}, ", m, v) + zp(insn.operand[1]) + ",x";
        case Mode::ImmAbs: return std::format("{} #${:02X}, ", m, v) + abs(insn.operand[1], true);
        case Mode::ImmAbx: return std::format("{} #${:02X}, ", m, v) + abs(insn.operand[1], true) + ",x";
        case Mode::Block: return m + " " + abs(v, false) + ", " + abs(insn.operand[1], false) + ", " + abs(insn.operand[2], false);
    }
    return m;
}

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels) {
    std::string_view rest = text::ltrim(line);
    std::size_t n = 0;
    while (n < rest.size() && std::isalnum(static_cast<unsigned char>(rest[n]))) ++n;
    if (n < 3 || n > 4 || (n < rest.size() && !text::is_space(rest[n]) && rest[n] != ';')) bad(line, "not an instruction");
    const std::string mnemonic = text::upper(rest.substr(0, n));
    rest.remove_prefix(n);
    if (const std::size_t comment = rest.find(';'); comment != std::string_view::npos) rest = rest.substr(0, comment);
    const std::string_view args = text::trim(rest);
    if (!is_mnemonic(mnemonic)) bad(line, "unknown instruction " + mnemonic);
    const auto value = [&](std::string_view s) { return value_of(s, labels, line); };
    const auto form = [&](Mode mode) -> std::uint8_t {
        const auto op = opcode_of(mnemonic, mode);
        if (!op) bad(line, mnemonic + " has no such form");
        return *op;
    };
    const auto word = [](Bytes& out, std::int64_t v) {
        out.push_back(static_cast<std::uint8_t>(v & 0xFF));
        out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
    };
    const auto displacement = [&](std::string_view target, std::uint32_t length) {
        const std::int64_t d = value(target).v - (static_cast<std::int64_t>(pc) + length);
        if (d < -128 || d > 127) bad(line, "branch out of reach");
        return static_cast<std::uint8_t>(d & 0xFF);
    };
    const std::vector<std::string_view> items = items_of(args);

    if (opcode_of(mnemonic, Mode::Block)) {   // TII src, dst, length
        if (items.size() != 3) bad(line, "a block transfer takes three operands");
        Bytes out{form(Mode::Block)};
        for (const std::string_view item : items) word(out, value(item).v);
        return out;
    }
    if (is_bit_branch(mnemonic)) {   // BBR0 zp, target
        if (items.size() != 2) bad(line, mnemonic + " takes a zero-page address and a target");
        const std::int64_t zp = value(items[0]).v;
        return {form(Mode::ZpRel), static_cast<std::uint8_t>(zp & 0xFF), displacement(items[1], 3)};
    }
    if (mnemonic == "TST") {   // TST #mask, zp | abs [,x]
        const bool indexed = items.size() == 3 && text::lower(items[2]) == "x";
        if (items.size() < 2 || items.size() > 3 || (items.size() == 3 && !indexed) || !items[0].starts_with("#")) bad(line, "TST takes #mask, an address and an optional x");
        const Value mask = value(items[0].substr(1));
        const Value at = value(items[1]);
        Bytes out;
        if (at.wide) {
            out = {form(indexed ? Mode::ImmAbx : Mode::ImmAbs), static_cast<std::uint8_t>(mask.v & 0xFF)};
            word(out, at.v);
        } else {
            out = {form(indexed ? Mode::ImmZpx : Mode::ImmZp), static_cast<std::uint8_t>(mask.v & 0xFF), static_cast<std::uint8_t>(at.v & 0xFF)};
        }
        return out;
    }
    if (mnemonic == "BSR") {
        if (items.size() != 1) bad(line, "BSR takes a target");
        return {form(Mode::Rel), displacement(items[0], 2)};
    }
    if (opcode_of(mnemonic, Mode::Imp) && items.empty() && !m6502::is_mnemonic(mnemonic, m6502::Variant::Cmos)) return {form(Mode::Imp)};
    if (mnemonic == "ST0" || mnemonic == "ST1" || mnemonic == "ST2" || mnemonic == "TAM" || mnemonic == "TMA") {
        if (items.size() != 1 || !items[0].starts_with("#")) bad(line, mnemonic + " takes an immediate");
        return {form(Mode::Imm), static_cast<std::uint8_t>(value(items[0].substr(1)).v & 0xFF)};
    }
    if (mnemonic.size() == 4 && (mnemonic.starts_with("RMB") || mnemonic.starts_with("SMB"))) {
        if (items.size() != 1) bad(line, mnemonic + " takes a zero-page address");
        return {form(Mode::Zp), static_cast<std::uint8_t>(value(items[0]).v & 0xFF)};
    }
    // The rest is the 65C02's: the shared assembler reads it.
    return m6502::assemble(line, pc, labels, m6502::Variant::Cmos);
}

}  // namespace cb::decompile::huc6280
