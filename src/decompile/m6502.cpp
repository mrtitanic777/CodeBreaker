#include "decompile/m6502.hpp"

#include <cctype>
#include <format>

#include "core/error.hpp"
#include "core/text.hpp"

namespace cb::decompile::m6502 {

namespace {

constexpr std::size_t kModeCount = 15;

struct Form {
    Mode mode;
    std::uint8_t op;
};
struct Group {
    std::string_view mnemonic;
    std::array<Form, 8> forms;   // unused entries have op 0 and are cut off by `count`
    std::size_t count;
};

// mnemonic -> {mode: opcode}, the documented set, in the order the manuals list it.
constexpr Group kGroups[] = {
    {"ADC", {{{Mode::Imm, 0x69}, {Mode::Zp, 0x65}, {Mode::Zpx, 0x75}, {Mode::Abs, 0x6D}, {Mode::Abx, 0x7D}, {Mode::Aby, 0x79}, {Mode::Izx, 0x61}, {Mode::Izy, 0x71}}}, 8},
    {"AND", {{{Mode::Imm, 0x29}, {Mode::Zp, 0x25}, {Mode::Zpx, 0x35}, {Mode::Abs, 0x2D}, {Mode::Abx, 0x3D}, {Mode::Aby, 0x39}, {Mode::Izx, 0x21}, {Mode::Izy, 0x31}}}, 8},
    {"ASL", {{{Mode::Acc, 0x0A}, {Mode::Zp, 0x06}, {Mode::Zpx, 0x16}, {Mode::Abs, 0x0E}, {Mode::Abx, 0x1E}}}, 5},
    {"BCC", {{{Mode::Rel, 0x90}}}, 1}, {"BCS", {{{Mode::Rel, 0xB0}}}, 1}, {"BEQ", {{{Mode::Rel, 0xF0}}}, 1},
    {"BMI", {{{Mode::Rel, 0x30}}}, 1}, {"BNE", {{{Mode::Rel, 0xD0}}}, 1}, {"BPL", {{{Mode::Rel, 0x10}}}, 1},
    {"BVC", {{{Mode::Rel, 0x50}}}, 1}, {"BVS", {{{Mode::Rel, 0x70}}}, 1},
    {"BIT", {{{Mode::Zp, 0x24}, {Mode::Abs, 0x2C}}}, 2},
    {"BRK", {{{Mode::Imp, 0x00}}}, 1},
    {"CLC", {{{Mode::Imp, 0x18}}}, 1}, {"CLD", {{{Mode::Imp, 0xD8}}}, 1}, {"CLI", {{{Mode::Imp, 0x58}}}, 1},
    {"CLV", {{{Mode::Imp, 0xB8}}}, 1},
    {"CMP", {{{Mode::Imm, 0xC9}, {Mode::Zp, 0xC5}, {Mode::Zpx, 0xD5}, {Mode::Abs, 0xCD}, {Mode::Abx, 0xDD}, {Mode::Aby, 0xD9}, {Mode::Izx, 0xC1}, {Mode::Izy, 0xD1}}}, 8},
    {"CPX", {{{Mode::Imm, 0xE0}, {Mode::Zp, 0xE4}, {Mode::Abs, 0xEC}}}, 3},
    {"CPY", {{{Mode::Imm, 0xC0}, {Mode::Zp, 0xC4}, {Mode::Abs, 0xCC}}}, 3},
    {"DEC", {{{Mode::Zp, 0xC6}, {Mode::Zpx, 0xD6}, {Mode::Abs, 0xCE}, {Mode::Abx, 0xDE}}}, 4},
    {"DEX", {{{Mode::Imp, 0xCA}}}, 1}, {"DEY", {{{Mode::Imp, 0x88}}}, 1},
    {"EOR", {{{Mode::Imm, 0x49}, {Mode::Zp, 0x45}, {Mode::Zpx, 0x55}, {Mode::Abs, 0x4D}, {Mode::Abx, 0x5D}, {Mode::Aby, 0x59}, {Mode::Izx, 0x41}, {Mode::Izy, 0x51}}}, 8},
    {"INC", {{{Mode::Zp, 0xE6}, {Mode::Zpx, 0xF6}, {Mode::Abs, 0xEE}, {Mode::Abx, 0xFE}}}, 4},
    {"INX", {{{Mode::Imp, 0xE8}}}, 1}, {"INY", {{{Mode::Imp, 0xC8}}}, 1},
    {"JMP", {{{Mode::Abs, 0x4C}, {Mode::Ind, 0x6C}}}, 2},
    {"JSR", {{{Mode::Abs, 0x20}}}, 1},
    {"LDA", {{{Mode::Imm, 0xA9}, {Mode::Zp, 0xA5}, {Mode::Zpx, 0xB5}, {Mode::Abs, 0xAD}, {Mode::Abx, 0xBD}, {Mode::Aby, 0xB9}, {Mode::Izx, 0xA1}, {Mode::Izy, 0xB1}}}, 8},
    {"LDX", {{{Mode::Imm, 0xA2}, {Mode::Zp, 0xA6}, {Mode::Zpy, 0xB6}, {Mode::Abs, 0xAE}, {Mode::Aby, 0xBE}}}, 5},
    {"LDY", {{{Mode::Imm, 0xA0}, {Mode::Zp, 0xA4}, {Mode::Zpx, 0xB4}, {Mode::Abs, 0xAC}, {Mode::Abx, 0xBC}}}, 5},
    {"LSR", {{{Mode::Acc, 0x4A}, {Mode::Zp, 0x46}, {Mode::Zpx, 0x56}, {Mode::Abs, 0x4E}, {Mode::Abx, 0x5E}}}, 5},
    {"NOP", {{{Mode::Imp, 0xEA}}}, 1},
    {"ORA", {{{Mode::Imm, 0x09}, {Mode::Zp, 0x05}, {Mode::Zpx, 0x15}, {Mode::Abs, 0x0D}, {Mode::Abx, 0x1D}, {Mode::Aby, 0x19}, {Mode::Izx, 0x01}, {Mode::Izy, 0x11}}}, 8},
    {"PHA", {{{Mode::Imp, 0x48}}}, 1}, {"PHP", {{{Mode::Imp, 0x08}}}, 1}, {"PLA", {{{Mode::Imp, 0x68}}}, 1},
    {"PLP", {{{Mode::Imp, 0x28}}}, 1},
    {"ROL", {{{Mode::Acc, 0x2A}, {Mode::Zp, 0x26}, {Mode::Zpx, 0x36}, {Mode::Abs, 0x2E}, {Mode::Abx, 0x3E}}}, 5},
    {"ROR", {{{Mode::Acc, 0x6A}, {Mode::Zp, 0x66}, {Mode::Zpx, 0x76}, {Mode::Abs, 0x6E}, {Mode::Abx, 0x7E}}}, 5},
    {"RTI", {{{Mode::Imp, 0x40}}}, 1}, {"RTS", {{{Mode::Imp, 0x60}}}, 1},
    {"SBC", {{{Mode::Imm, 0xE9}, {Mode::Zp, 0xE5}, {Mode::Zpx, 0xF5}, {Mode::Abs, 0xED}, {Mode::Abx, 0xFD}, {Mode::Aby, 0xF9}, {Mode::Izx, 0xE1}, {Mode::Izy, 0xF1}}}, 8},
    {"SEC", {{{Mode::Imp, 0x38}}}, 1}, {"SED", {{{Mode::Imp, 0xF8}}}, 1}, {"SEI", {{{Mode::Imp, 0x78}}}, 1},
    {"STA", {{{Mode::Zp, 0x85}, {Mode::Zpx, 0x95}, {Mode::Abs, 0x8D}, {Mode::Abx, 0x9D}, {Mode::Aby, 0x99}, {Mode::Izx, 0x81}, {Mode::Izy, 0x91}}}, 7},
    {"STX", {{{Mode::Zp, 0x86}, {Mode::Zpy, 0x96}, {Mode::Abs, 0x8E}}}, 3},
    {"STY", {{{Mode::Zp, 0x84}, {Mode::Zpx, 0x94}, {Mode::Abs, 0x8C}}}, 3},
    {"TAX", {{{Mode::Imp, 0xAA}}}, 1}, {"TAY", {{{Mode::Imp, 0xA8}}}, 1}, {"TSX", {{{Mode::Imp, 0xBA}}}, 1},
    {"TXA", {{{Mode::Imp, 0x8A}}}, 1}, {"TXS", {{{Mode::Imp, 0x9A}}}, 1}, {"TYA", {{{Mode::Imp, 0x98}}}, 1},
};

// What the 65SC02 adds to the NMOS set: a form of an existing mnemonic, or a new mnemonic.
struct Extra {
    std::string_view mnemonic;
    Mode mode;
    std::uint8_t op;
};
constexpr Extra kCmosExtra[] = {
    {"ADC", Mode::Izp, 0x72}, {"AND", Mode::Izp, 0x32}, {"CMP", Mode::Izp, 0xD2}, {"EOR", Mode::Izp, 0x52},
    {"LDA", Mode::Izp, 0xB2}, {"ORA", Mode::Izp, 0x12}, {"SBC", Mode::Izp, 0xF2}, {"STA", Mode::Izp, 0x92},
    {"BIT", Mode::Imm, 0x89}, {"BIT", Mode::Zpx, 0x34}, {"BIT", Mode::Abx, 0x3C}, {"BRA", Mode::Rel, 0x80},
    {"DEC", Mode::Acc, 0x3A}, {"INC", Mode::Acc, 0x1A}, {"JMP", Mode::Iax, 0x7C},
    {"PHX", Mode::Imp, 0xDA}, {"PHY", Mode::Imp, 0x5A}, {"PLX", Mode::Imp, 0xFA}, {"PLY", Mode::Imp, 0x7A},
    {"STZ", Mode::Zp, 0x64},  {"STZ", Mode::Zpx, 0x74}, {"STZ", Mode::Abs, 0x9C}, {"STZ", Mode::Abx, 0x9E},
    {"TRB", Mode::Zp, 0x14},  {"TRB", Mode::Abs, 0x1C}, {"TSB", Mode::Zp, 0x04},  {"TSB", Mode::Abs, 0x0C},
};

struct Tables {
    std::array<std::optional<OpcodeEntry>, 256> by_opcode;
    // mnemonic -> opcode per mode (-1: no such form); lookup only
    std::unordered_map<std::string_view, std::array<int, kModeCount>> by_mnemonic;
    explicit Tables(Variant variant) {
        for (const Group& g : kGroups) {
            std::array<int, kModeCount> forms;
            forms.fill(-1);
            for (std::size_t i = 0; i < g.count; ++i) {
                by_opcode[g.forms[i].op] = OpcodeEntry{g.mnemonic, g.forms[i].mode};
                forms[static_cast<std::size_t>(g.forms[i].mode)] = g.forms[i].op;
            }
            by_mnemonic.emplace(g.mnemonic, forms);
        }
        if (variant == Variant::Cmos) {
            for (const Extra& e : kCmosExtra) {
                by_opcode[e.op] = OpcodeEntry{e.mnemonic, e.mode};
                auto [it, added] = by_mnemonic.try_emplace(e.mnemonic);
                if (added) it->second.fill(-1);
                it->second[static_cast<std::size_t>(e.mode)] = e.op;
            }
        }
    }
};

const Tables& tables(Variant variant = Variant::Nmos) {
    static const Tables nmos(Variant::Nmos);
    static const Tables cmos(Variant::Cmos);
    return variant == Variant::Cmos ? cmos : nmos;
}

[[noreturn]] void bad(std::string_view line, const std::string& what) {
    throw ParseError("", 0, 0, std::format("{} ({})", what, line));
}

// A number or label as the assembler reads it: its value, and whether the way it is written forces an
// absolute address (an `a:` prefix, four hex digits, or a value past one byte).
struct Value {
    std::int64_t v;
    bool wide;
};

Value value_of(std::string_view s, const NameLabels& labels, std::string_view line) {
    s = text::trim(s);
    const bool wide = s.starts_with("a:");
    if (wide) s.remove_prefix(2);
    if (s.starts_with("$")) {
        auto v = text::parse_int<std::int64_t>(s.substr(1), 16);
        if (!v) bad(line, "bad hex number");
        return {*v, wide || s.size() > 3};
    }
    if (auto it = labels.find(s); it != labels.end()) return {it->second, wide || it->second > 0xFF};
    auto v = text::parse_int<std::int64_t>(s, 0);
    if (!v) bad(line, "unknown label or number " + std::string(s));
    return {*v, wide || *v > 0xFF};
}

}  // namespace

std::string_view mode_name(Mode mode) {
    static constexpr std::string_view kNames[kModeCount] = {"imp", "acc", "imm", "zp",  "zpx", "zpy", "abs",
                                                           "abx", "aby", "ind", "izx", "izy", "rel", "izp", "iax"};
    return kNames[static_cast<std::size_t>(mode)];
}

const std::array<std::optional<OpcodeEntry>, 256>& opcode_table(Variant variant) { return tables(variant).by_opcode; }

std::optional<std::uint8_t> opcode_of(std::string_view mnemonic, Mode mode, Variant variant) {
    const Tables& t = tables(variant);
    auto it = t.by_mnemonic.find(mnemonic);
    if (it == t.by_mnemonic.end()) return std::nullopt;
    int op = it->second[static_cast<std::size_t>(mode)];
    if (op < 0) return std::nullopt;
    return static_cast<std::uint8_t>(op);
}

bool is_mnemonic(std::string_view mnemonic, Variant variant) { return tables(variant).by_mnemonic.contains(mnemonic); }

bool is_branch(std::string_view m) {
    return m == "BCC" || m == "BCS" || m == "BEQ" || m == "BMI" || m == "BNE" || m == "BPL" || m == "BVC" || m == "BVS" || m == "BRA";
}

bool ends_flow(std::string_view m) { return m == "JMP" || m == "RTS" || m == "RTI" || m == "BRK" || m == "BRA"; }

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc, Variant variant) {
    if (offset >= code.size()) return std::nullopt;
    const std::optional<OpcodeEntry>& entry = tables(variant).by_opcode[code[offset]];
    if (!entry) return std::nullopt;
    Insn insn;
    insn.at = pc;
    insn.op = code[offset];
    insn.mnemonic = entry->mnemonic;
    insn.mode = entry->mode;
    insn.size = instruction_size(entry->mode);
    if (offset + insn.size > code.size()) return std::nullopt;
    if (insn.size == 2) {
        insn.operand = code[offset + 1];
    } else if (insn.size == 3) {
        insn.operand = static_cast<std::uint16_t>(code[offset + 1] | (code[offset + 2] << 8));
    }
    if (insn.mode == Mode::Rel) {
        const int displacement = static_cast<std::int8_t>(*insn.operand);   // one signed byte
        insn.target = static_cast<std::uint16_t>((pc + 2 + static_cast<std::uint32_t>(displacement)) & 0xFFFF);
    } else if ((insn.mnemonic == "JMP" || insn.mnemonic == "JSR") && insn.mode == Mode::Abs) {
        insn.target = insn.operand;
    }
    return insn;
}

std::string text(const Insn& insn, const AddressLabels& labels) {
    const std::string m(insn.mnemonic);
    // a label's name, else the address in `width` hex digits; `a:` keeps a zero-page-sized label absolute
    auto addr = [&](std::uint16_t value, int width) {
        auto it = labels.find(value);
        if (it != labels.end() && !it->second.empty()) {
            return (width == 4 && value < 0x100) ? "a:" + it->second : it->second;
        }
        return width == 4 ? std::format("${:04X}", value) : std::format("${:02X}", value);
    };
    switch (insn.mode) {
        case Mode::Imp: return m;
        case Mode::Acc: return m + " a";
        case Mode::Imm: return std::format("{} #${:02X}", m, *insn.operand);
        case Mode::Rel: return m + " " + addr(*insn.target, 4);
        default: break;
    }
    const std::uint16_t v = *insn.operand;
    switch (insn.mode) {
        case Mode::Zp: return m + " " + addr(v, 2);
        case Mode::Zpx: return m + " " + addr(v, 2) + ",x";
        case Mode::Zpy: return m + " " + addr(v, 2) + ",y";
        case Mode::Abs: return m + " " + addr(v, 4);
        case Mode::Abx: return m + " " + addr(v, 4) + ",x";
        case Mode::Aby: return m + " " + addr(v, 4) + ",y";
        case Mode::Ind: return m + " (" + addr(v, 4) + ")";
        case Mode::Iax: return m + " (" + addr(v, 4) + ",x)";
        case Mode::Izp: return m + " (" + addr(v, 2) + ")";
        case Mode::Izx: return m + " (" + addr(v, 2) + ",x)";
        default: return m + " (" + addr(v, 2) + "),y";   // Izy
    }
}

std::string dasm_text(const Insn& insn, const AddressLabels& labels) {
    std::string m(insn.mnemonic);
    auto addr = [&](std::uint16_t value, int width) {
        auto it = labels.find(value);
        if (it != labels.end() && !it->second.empty()) return it->second;
        return width == 4 ? std::format("${:04X}", value) : std::format("${:02X}", value);
    };
    switch (insn.mode) {
        case Mode::Imp: return m;
        case Mode::Acc: return m + " A";
        case Mode::Imm: return std::format("{} #${:02X}", m, *insn.operand);
        case Mode::Rel: return m + " " + addr(*insn.target, 4);
        default: break;
    }
    const std::uint16_t v = *insn.operand;
    const bool wide = v < 0x100 && insn.mnemonic != "JMP" && insn.mnemonic != "JSR";   // an absolute form of a zero-page address: dasm's `.w`
    switch (insn.mode) {
        case Mode::Zp: return m + " " + addr(v, 2);
        case Mode::Zpx: return m + " " + addr(v, 2) + ",X";
        case Mode::Zpy: return m + " " + addr(v, 2) + ",Y";
        case Mode::Abs: return m + (wide ? ".w " : " ") + addr(v, 4);
        case Mode::Abx: return m + (wide ? ".w " : " ") + addr(v, 4) + ",X";
        case Mode::Aby: return m + (wide ? ".w " : " ") + addr(v, 4) + ",Y";
        case Mode::Ind: return m + " (" + addr(v, 4) + ")";
        case Mode::Iax: return m + " (" + addr(v, 4) + ",X)";
        case Mode::Izp: return m + " (" + addr(v, 2) + ")";
        case Mode::Izx: return m + " (" + addr(v, 2) + ",X)";
        default: return m + " (" + addr(v, 2) + "),Y";   // Izy
    }
}

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, Variant variant) {
    // `  MNM  argument  ; comment`: three letters, then whitespace and the argument, or nothing
    std::string_view rest = text::ltrim(line);
    const bool three_letters = rest.size() >= 3 && std::isalpha(static_cast<unsigned char>(rest[0])) &&
                               std::isalpha(static_cast<unsigned char>(rest[1])) &&
                               std::isalpha(static_cast<unsigned char>(rest[2]));
    if (!three_letters || (rest.size() > 3 && !text::is_space(rest[3]) && rest[3] != ';')) {
        bad(line, "not an instruction");
    }
    const std::string mnemonic = text::upper(rest.substr(0, 3));
    rest.remove_prefix(3);
    const std::size_t comment = rest.find(';');
    if (comment != std::string_view::npos) rest = rest.substr(0, comment);
    const std::string_view arg = text::trim(rest);

    auto it = tables(variant).by_mnemonic.find(mnemonic);
    if (it == tables(variant).by_mnemonic.end()) bad(line, "unknown instruction " + mnemonic);
    const std::array<int, kModeCount>& forms = it->second;
    auto value = [&](std::string_view s) { return value_of(s, labels, line); };

    Mode mode;
    std::int64_t operand = 0;
    const std::string low_arg = text::lower(arg);
    std::string low;   // lower case, spaces removed
    for (char c : low_arg) {
        if (c != ' ') low += c;
    }
    if (arg.empty()) {
        mode = Mode::Imp;
    } else if (low_arg == "a") {
        mode = Mode::Acc;
    } else if (arg.starts_with("#")) {
        std::string body;   // `#<$12` / `#>$12`: the byte-select marks are dropped, the whole value is used
        for (char c : arg.substr(1)) {
            if (c != '<' && c != '>') body += c;
        }
        mode = Mode::Imm;
        operand = value(body).v;
    } else if (is_branch(mnemonic)) {
        const std::int64_t target = value(arg).v;
        const std::int64_t d = target - (static_cast<std::int64_t>(pc) + 2);
        if (d < -128 || d > 127) bad(line, "branch out of reach");
        mode = Mode::Rel;
        operand = d & 0xFF;
    } else if (low.starts_with("(") && low.ends_with(",x)")) {
        const std::size_t cut = low_arg.rfind(",x");
        if (cut == std::string::npos) bad(line, "bad indexed-indirect operand");
        const Value v = value(arg.substr(1, cut - 1));
        mode = (forms[static_cast<std::size_t>(Mode::Izx)] < 0 && forms[static_cast<std::size_t>(Mode::Iax)] >= 0) ? Mode::Iax : Mode::Izx;
        operand = v.v;
    } else if (low.starts_with("(") && low.ends_with("),y")) {
        const std::size_t cut = arg.find(')');
        mode = Mode::Izy;
        operand = value(arg.substr(1, cut - 1)).v;
    } else if (low.starts_with("(")) {
        const std::size_t cut = arg.rfind(')');
        if (cut == std::string_view::npos) bad(line, "unclosed parenthesis");
        const Value v = value(arg.substr(1, cut - 1));
        mode = (!v.wide && forms[static_cast<std::size_t>(Mode::Izp)] >= 0) ? Mode::Izp : Mode::Ind;
        operand = v.v;
    } else {
        const bool x = low.ends_with(",x"), y = low.ends_with(",y");
        const Value v = value(x || y ? arg.substr(0, arg.size() - 2) : arg);
        const Mode zero_page = x ? Mode::Zpx : y ? Mode::Zpy : Mode::Zp;
        const Mode absolute = x ? Mode::Abx : y ? Mode::Aby : Mode::Abs;
        mode = (!v.wide && forms[static_cast<std::size_t>(zero_page)] >= 0) ? zero_page : absolute;
        operand = v.v;
    }
    if (forms[static_cast<std::size_t>(mode)] < 0) bad(line, mnemonic + " has no " + std::string(mode_name(mode)) + " form");
    Bytes out{static_cast<std::uint8_t>(forms[static_cast<std::size_t>(mode)])};
    const std::uint8_t size = instruction_size(mode);
    if (size >= 2) out.push_back(static_cast<std::uint8_t>(operand & 0xFF));
    if (size == 3) out.push_back(static_cast<std::uint8_t>((operand >> 8) & 0xFF));
    return out;
}

}  // namespace cb::decompile::m6502
