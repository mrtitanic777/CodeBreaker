// cb-unity: alone
#include "decompile/m6809.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <vector>

#include "core/text.hpp"

namespace cb::decompile::m6809 {

namespace {

using Table = std::array<std::optional<OpcodeEntry>, 256>;

struct Tables {
    Table page[3];
};

void put(Table& t, unsigned op, std::string_view mnemonic, Mode mode) { t[op] = OpcodeEntry{mnemonic, mode}; }

// The eight read-modify-write instructions of the 6809, by their low nibble: the first page's $00, $40 (A), $50 (B), $60 (indexed), $70 (extended).
struct Rmw {
    unsigned low;
    std::string_view name;
};
constexpr Rmw kRmw[] = {{0x0, "NEG"}, {0x3, "COM"}, {0x4, "LSR"}, {0x6, "ROR"}, {0x7, "ASR"}, {0x8, "LSL"}, {0x9, "ROL"}, {0xA, "DEC"}, {0xC, "INC"}, {0xD, "TST"}, {0xF, "CLR"}};

constexpr std::string_view kBranches[] = {"BRA", "BRN", "BHI", "BLS", "BCC", "BCS", "BNE", "BEQ", "BVC", "BVS", "BPL", "BMI", "BGE", "BLT", "BGT", "BLE"};
constexpr std::string_view kLongBranches[] = {"LBRA", "LBRN", "LBHI", "LBLS", "LBCC", "LBCS", "LBNE", "LBEQ", "LBVC", "LBVS", "LBPL", "LBMI", "LBGE", "LBLT", "LBGT", "LBLE"};

// The A and B groups: row = the opcode's high nibble group (0x80 immediate, 0x90 direct, 0xA0 indexed, 0xB0 extended), column = low nibble.
void groups(Table& t, unsigned base, const std::string_view (&names)[16], bool a_group) {
    for (unsigned col = 0; col < 16; ++col) {
        if (names[col].empty()) continue;
        for (unsigned row = 0; row < 4; ++row) {
            const unsigned op = base + row * 0x10 + col;
            Mode mode = row == 0 ? Mode::Imm8 : row == 1 ? Mode::Dir : row == 2 ? Mode::Idx : Mode::Ext;
            if (row == 0 && (names[col] == "SUBD" || names[col] == "ADDD" || names[col] == "CMPX" || names[col] == "LDX" || names[col] == "LDD" || names[col] == "LDU")) mode = Mode::Imm16;
            if (names[col] == "BSR/JSR") {
                if (row == 0) put(t, op, "BSR", Mode::Rel8);
                else put(t, op, "JSR", mode);
                continue;
            }
            if (row == 0 && (names[col] == "STA" || names[col] == "STB" || names[col] == "STX" || names[col] == "STD" || names[col] == "STU")) continue;   // (no immediate store)
            put(t, op, names[col], mode);
        }
    }
    (void)a_group;
}

Tables build() {
    Tables out;
    Table& p0 = out.page[0];
    for (const Rmw& r : kRmw) {
        put(p0, 0x00 + r.low, r.name, Mode::Dir);
        put(p0, 0x60 + r.low, r.name, Mode::Idx);
        put(p0, 0x70 + r.low, r.name, Mode::Ext);
        put(p0, 0x40 + r.low, std::string_view(), Mode::Inh);
    }
    // the accumulator forms: NEGA ... CLRA and NEGB ... CLRB (static names)
    static const std::vector<std::string> acc_names = [] {
        std::vector<std::string> v;
        for (const Rmw& r : kRmw) {
            v.push_back(std::string(r.name) + "A");
            v.push_back(std::string(r.name) + "B");
        }
        return v;
    }();
    for (std::size_t i = 0; i < std::size(kRmw); ++i) {
        put(p0, 0x40 + kRmw[i].low, acc_names[2 * i], Mode::Inh);
        put(p0, 0x50 + kRmw[i].low, acc_names[2 * i + 1], Mode::Inh);
    }
    put(p0, 0x0E, "JMP", Mode::Dir);
    put(p0, 0x6E, "JMP", Mode::Idx);
    put(p0, 0x7E, "JMP", Mode::Ext);
    // (0x0E replaces nothing in kRmw: JMP has no RMW slot)
    put(p0, 0x12, "NOP", Mode::Inh);
    put(p0, 0x13, "SYNC", Mode::Inh);
    put(p0, 0x16, "LBRA", Mode::Rel16);
    put(p0, 0x17, "LBSR", Mode::Rel16);
    put(p0, 0x19, "DAA", Mode::Inh);
    put(p0, 0x1A, "ORCC", Mode::Imm8);
    put(p0, 0x1C, "ANDCC", Mode::Imm8);
    put(p0, 0x1D, "SEX", Mode::Inh);
    put(p0, 0x1E, "EXG", Mode::Pair);
    put(p0, 0x1F, "TFR", Mode::Pair);
    for (unsigned i = 0; i < 16; ++i) put(p0, 0x20 + i, kBranches[i], Mode::Rel8);
    put(p0, 0x30, "LEAX", Mode::Idx);
    put(p0, 0x31, "LEAY", Mode::Idx);
    put(p0, 0x32, "LEAS", Mode::Idx);
    put(p0, 0x33, "LEAU", Mode::Idx);
    put(p0, 0x34, "PSHS", Mode::Regs);
    put(p0, 0x35, "PULS", Mode::Regs);
    put(p0, 0x36, "PSHU", Mode::Regs);
    put(p0, 0x37, "PULU", Mode::Regs);
    put(p0, 0x39, "RTS", Mode::Inh);
    put(p0, 0x3A, "ABX", Mode::Inh);
    put(p0, 0x3B, "RTI", Mode::Inh);
    put(p0, 0x3C, "CWAI", Mode::Imm8);
    put(p0, 0x3D, "MUL", Mode::Inh);
    put(p0, 0x3F, "SWI", Mode::Inh);
    static const std::string_view a_names[16] = {"SUBA", "CMPA", "SBCA", "SUBD", "ANDA", "BITA", "LDA", "STA", "EORA", "ADCA", "ORA", "ADDA", "CMPX", "BSR/JSR", "LDX", "STX"};
    static const std::string_view b_names[16] = {"SUBB", "CMPB", "SBCB", "ADDD", "ANDB", "BITB", "LDB", "STB", "EORB", "ADCB", "ORB", "ADDB", "LDD", "STD", "LDU", "STU"};
    groups(p0, 0x80, a_names, true);
    groups(p0, 0xC0, b_names, false);
    // the second page ($10): long branches, SWI2, and the 16-bit instructions on Y and S
    Table& p1 = out.page[1];
    for (unsigned i = 1; i < 16; ++i) put(p1, 0x20 + i, kLongBranches[i], Mode::Rel16);
    put(p1, 0x3F, "SWI2", Mode::Inh);
    const auto four = [](Table& t, unsigned imm, std::string_view name, bool store) {
        if (!store) put(t, imm, name, Mode::Imm16);
        put(t, imm + 0x10, name, Mode::Dir);
        put(t, imm + 0x20, name, Mode::Idx);
        put(t, imm + 0x30, name, Mode::Ext);
    };
    four(p1, 0x83, "CMPD", false);
    four(p1, 0x8C, "CMPY", false);
    four(p1, 0x8E, "LDY", false);
    four(p1, 0x8F, "STY", true);
    four(p1, 0xCE, "LDS", false);
    four(p1, 0xCF, "STS", true);
    Table& p2 = out.page[2];
    put(p2, 0x3F, "SWI3", Mode::Inh);
    four(p2, 0x83, "CMPU", false);
    four(p2, 0x8C, "CMPS", false);
    return out;
}

const Tables& tables() {
    static const Tables t = build();
    return t;
}

constexpr std::string_view kIndexRegs[4] = {"X", "Y", "U", "S"};

// The registers of PSHS and PSHU in mask order (bit 6 is the other stack pointer: U for PSHS, S for PSHU).
std::string regs_text(unsigned mask, bool user_stack) {
    static const char* names[8] = {"CC", "A", "B", "DP", "X", "Y", "U", "PC"};
    std::string out;
    for (unsigned i = 0; i < 8; ++i) {
        if (!(mask & (1u << i))) continue;
        if (!out.empty()) out += ",";
        out += (i == 6 && user_stack) ? "S" : names[i];
    }
    return out;
}

std::optional<unsigned> pair_register(std::string_view name) {
    static const struct {
        const char* name;
        unsigned code;
    } regs[] = {{"D", 0}, {"X", 1}, {"Y", 2}, {"U", 3}, {"S", 4}, {"PC", 5}, {"A", 8}, {"B", 9}, {"CC", 10}, {"DP", 11}};
    for (const auto& r : regs) {
        if (name == r.name) return r.code;
    }
    return std::nullopt;
}

std::string pair_name(unsigned code) {
    static const char* names[16] = {"D", "X", "Y", "U", "S", "PC", "?", "?", "A", "B", "CC", "DP", "?", "?", "?", "?"};
    return names[code & 15];
}

bool valid_pair(unsigned post) {
    const unsigned a = post >> 4, b = post & 15;
    const auto ok = [](unsigned c) { return c <= 5 || (c >= 8 && c <= 11); };
    return ok(a) && ok(b) && ((a < 8) == (b < 8));   // (the registers of an exchange are of one size)
}

std::int32_t signed8(std::uint8_t v) { return static_cast<std::int8_t>(v); }
std::int32_t signed16(std::uint16_t v) { return static_cast<std::int16_t>(v); }

// The indexed operand at code[at:] (the postbyte first): decoded, and how many bytes it took.
std::optional<std::pair<Index, unsigned>> decode_index(ByteView code, std::size_t at) {
    if (at >= code.size()) return std::nullopt;
    Index ix;
    const std::uint8_t post = code[at];
    ix.postbyte = post;
    ix.reg = (post >> 5) & 3;
    if (!(post & 0x80)) {
        ix.kind = Index::Kind::Offset5;
        ix.offset = (post & 0x10) ? (post & 0x1F) - 32 : (post & 0x1F);
        return std::pair{ix, 1u};
    }
    ix.indirect = (post & 0x10) != 0;
    switch (post & 0x0F) {
        case 0x0: ix.kind = Index::Kind::Inc1; if (ix.indirect) return std::nullopt; return std::pair{ix, 1u};
        case 0x1: ix.kind = Index::Kind::Inc2; return std::pair{ix, 1u};
        case 0x2: ix.kind = Index::Kind::Dec1; if (ix.indirect) return std::nullopt; return std::pair{ix, 1u};
        case 0x3: ix.kind = Index::Kind::Dec2; return std::pair{ix, 1u};
        case 0x4: ix.kind = Index::Kind::Zero; return std::pair{ix, 1u};
        case 0x5: ix.kind = Index::Kind::B; return std::pair{ix, 1u};
        case 0x6: ix.kind = Index::Kind::A; return std::pair{ix, 1u};
        case 0x8:
            if (at + 2 > code.size()) return std::nullopt;
            ix.kind = Index::Kind::Offset8;
            ix.offset = signed8(code[at + 1]);
            return std::pair{ix, 2u};
        case 0x9:
            if (at + 3 > code.size()) return std::nullopt;
            ix.kind = Index::Kind::Offset16;
            ix.offset = signed16(static_cast<std::uint16_t>(code[at + 1] << 8 | code[at + 2]));
            return std::pair{ix, 3u};
        case 0xB: ix.kind = Index::Kind::D; return std::pair{ix, 1u};
        case 0xC:
            if (at + 2 > code.size() || ix.reg != 0) return std::nullopt;   // (the register bits mean nothing here: such a postbyte is not read as code)
            ix.kind = Index::Kind::Pc8;
            ix.offset = signed8(code[at + 1]);
            return std::pair{ix, 2u};
        case 0xD:
            if (at + 3 > code.size() || ix.reg != 0) return std::nullopt;
            ix.kind = Index::Kind::Pc16;
            ix.offset = signed16(static_cast<std::uint16_t>(code[at + 1] << 8 | code[at + 2]));
            return std::pair{ix, 3u};
        case 0xF:
            if (!ix.indirect || at + 3 > code.size() || ix.reg != 0) return std::nullopt;
            ix.kind = Index::Kind::Extended;
            ix.offset = static_cast<std::int32_t>(code[at + 1] << 8 | code[at + 2]);
            return std::pair{ix, 3u};
        default: return std::nullopt;
    }
}

std::string index_text(const Index& ix) {
    const std::string reg(kIndexRegs[ix.reg]);
    std::string s;
    using K = Index::Kind;
    switch (ix.kind) {
        case K::Offset5: s = std::format("{},{}", ix.offset, reg); break;
        case K::Offset8: s = std::format("<{},{}", ix.offset, reg); break;
        case K::Offset16: s = std::format(">{},{}", ix.offset, reg); break;
        case K::A: s = "A," + reg; break;
        case K::B: s = "B," + reg; break;
        case K::D: s = "D," + reg; break;
        case K::Inc1: s = "," + reg + "+"; break;
        case K::Inc2: s = "," + reg + "++"; break;
        case K::Dec1: s = ",-" + reg; break;
        case K::Dec2: s = ",--" + reg; break;
        case K::Zero: s = "," + reg; break;
        case K::Pc8: s = std::format("<{},PCR", ix.offset); break;
        case K::Pc16: s = std::format(">{},PCR", ix.offset); break;
        case K::Extended: s = std::format("${:04X}", ix.offset); break;
    }
    return ix.indirect ? "[" + s + "]" : s;
}

}  // namespace

const std::array<std::optional<OpcodeEntry>, 256>& opcode_table(unsigned page) { return tables().page[page > 2 ? 0 : page]; }

std::optional<Opcode> opcode_of(std::string_view mnemonic, Mode mode) {
    for (unsigned page = 0; page < 3; ++page) {
        for (unsigned op = 0; op < 256; ++op) {
            const auto& e = tables().page[page][op];
            if (e && e->mnemonic == mnemonic && e->mode == mode) return Opcode{page, static_cast<std::uint8_t>(op)};
        }
    }
    return std::nullopt;
}

bool is_mnemonic(std::string_view mnemonic) {
    for (unsigned page = 0; page < 3; ++page) {
        for (const auto& e : tables().page[page]) {
            if (e && e->mnemonic == mnemonic) return true;
        }
    }
    return false;
}

bool is_branch(std::string_view m) {
    return std::ranges::find(kBranches, m) != std::end(kBranches) || std::ranges::find(kLongBranches, m) != std::end(kLongBranches) || m == "BSR" || m == "LBSR";
}

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset >= code.size()) return std::nullopt;
    Insn insn;
    insn.at = pc;
    std::size_t at = offset;
    unsigned page = 0;
    if (code[at] == 0x10 || code[at] == 0x11) {
        page = code[at] - 0x0F;
        if (++at >= code.size()) return std::nullopt;
    }
    const std::uint8_t op = code[at++];
    const auto& entry = tables().page[page][op];
    if (!entry) return std::nullopt;
    insn.page = page;
    insn.op = op;
    insn.mnemonic = entry->mnemonic;
    insn.mode = entry->mode;
    const auto byte = [&](std::size_t i) { return code[i]; };
    switch (entry->mode) {
        case Mode::Inh: break;
        case Mode::Imm8:
        case Mode::Dir:
        case Mode::Regs:
            if (at + 1 > code.size()) return std::nullopt;
            insn.value = byte(at++);
            break;
        case Mode::Pair:
            if (at + 1 > code.size()) return std::nullopt;
            insn.value = byte(at++);
            if (!valid_pair(insn.value)) return std::nullopt;
            break;
        case Mode::Imm16:
        case Mode::Ext:
        case Mode::Rel16:
            if (at + 2 > code.size()) return std::nullopt;
            insn.value = static_cast<std::uint32_t>(byte(at) << 8 | byte(at + 1));
            at += 2;
            break;
        case Mode::Rel8:
            if (at + 1 > code.size()) return std::nullopt;
            insn.value = byte(at++);
            break;
        case Mode::Idx: {
            const auto ix = decode_index(code, at);
            if (!ix) return std::nullopt;
            insn.index = ix->first;
            at += ix->second;
            break;
        }
    }
    insn.size = static_cast<std::uint8_t>(at - offset);
    if (insn.size > insn.raw.size()) return std::nullopt;
    std::copy_n(code.begin() + static_cast<std::ptrdiff_t>(offset), insn.size, insn.raw.begin());
    const std::string_view m = insn.mnemonic;
    const std::uint32_t next = pc + insn.size;
    if (insn.mode == Mode::Rel8) insn.target = (next + static_cast<std::uint32_t>(signed8(static_cast<std::uint8_t>(insn.value)))) & 0xFFFF;
    if (insn.mode == Mode::Rel16) insn.target = (next + static_cast<std::uint32_t>(signed16(static_cast<std::uint16_t>(insn.value)))) & 0xFFFF;
    if (m == "BRA" || m == "LBRA") insn.flow = Flow::Jump;
    else if (m == "BSR" || m == "LBSR") insn.flow = Flow::Call;
    else if (is_branch(m)) insn.flow = Flow::Branch;
    else if (m == "JMP") {
        if (insn.mode == Mode::Ext) {
            insn.target = insn.value;
            insn.flow = Flow::Jump;
        } else {
            insn.flow = Flow::Indirect;
        }
    } else if (m == "JSR") {
        if (insn.mode == Mode::Ext) insn.target = insn.value;
        insn.flow = Flow::Call;
    } else if (m == "RTS" || m == "RTI") insn.flow = Flow::Return;
    else if (m == "PULS" && (insn.value & 0x80)) insn.flow = Flow::Return;   // (a pull of the pc returns)
    else if (m == "PULU" && (insn.value & 0x80)) insn.flow = Flow::Return;
    else if (m == "TFR" && (insn.value & 15) == 5) insn.flow = Flow::Indirect;   // (a transfer into the pc jumps)
    return insn;
}

std::string text(const Insn& i, const AddressLabels& labels) {
    const auto name_of = [&](std::uint32_t addr, std::string fallback) {
        const auto it = labels.find(addr);
        return it != labels.end() && !it->second.empty() ? it->second : std::move(fallback);
    };
    std::string m(i.mnemonic);
    switch (i.mode) {
        case Mode::Inh: return m;
        case Mode::Imm8: return std::format("{} #${:02X}", m, i.value);
        case Mode::Imm16: return std::format("{} #{}", m, name_of(i.value, std::format("${:04X}", i.value)));
        case Mode::Dir: return std::format("{} <${:02X}", m, i.value);
        case Mode::Ext: return m + " " + name_of(i.value, std::format("${:04X}", i.value));
        case Mode::Rel8:
        case Mode::Rel16: return m + " " + name_of(*i.target, std::format("${:04X}", *i.target));
        case Mode::Regs: return i.value == 0 ? m + " #$00" : m + " " + regs_text(i.value, m == "PSHU" || m == "PULU");   // (an empty list has no register to name)
        case Mode::Pair: return m + " " + pair_name(i.value >> 4) + "," + pair_name(i.value & 15);
        case Mode::Idx: return m + " " + index_text(i.index);
    }
    return m;
}

// ---- the assembler --------------------------------------------------------------------------------------------------

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc, bool lenient) {
    const std::string_view e = text::trim(expr);
    if (e.empty()) throw ParseError("", 0, 0, "an operand is missing");
    // sum of terms, left to right
    std::int64_t total = 0;
    std::size_t i = 0;
    int sign = 1;
    while (i < e.size()) {
        if (e[i] == ' ') { ++i; continue; }
        if (e[i] == '+' || e[i] == '-') {
            sign = e[i] == '-' ? -1 : 1;
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < e.size() && e[j] != '+' && !(e[j] == '-' && j != i)) ++j;
        std::string_view term = text::trim(e.substr(i, j - i));
        std::int64_t v = 0;
        bool byte_low = false, byte_high = false;
        if (term.starts_with('<')) { byte_low = true; term.remove_prefix(1); }
        else if (term.starts_with('>')) { byte_high = true; term.remove_prefix(1); }
        if (term == "*") v = pc;
        else if (term.starts_with('$')) v = text::parse_int<std::int64_t>(term.substr(1), 16).value_or(0);
        else if (term.starts_with("0x") || term.starts_with("0X")) v = text::parse_int<std::int64_t>(term.substr(2), 16).value_or(0);
        else if (term.starts_with('%')) v = text::parse_int<std::int64_t>(term.substr(1), 2).value_or(0);
        else if (!term.empty() && std::isdigit(static_cast<unsigned char>(term[0]))) v = text::parse_int<std::int64_t>(term, 10).value_or(0);
        else {
            const auto it = labels.find(term);
            if (it != labels.end()) v = it->second;
            else if (lenient) v = 0;
            else throw ParseError("", 0, 0, "unknown name '" + std::string(term) + "'");
        }
        if (byte_low) v &= 0xFF;
        if (byte_high) v = (v >> 8) & 0xFF;
        total += sign * v;
        sign = 1;
        i = j;
    }
    return total;
}

namespace {

[[noreturn]] void fail(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

// "A,B,X" as a PSH / PUL mask.
std::uint32_t parse_regs(std::string_view list, bool user_stack, std::string_view line) {
    std::uint32_t mask = 0;
    for (const std::string_view item : text::split_trim(list, ',')) {
        std::string reg = text::upper(item);
        static const char* names[8] = {"CC", "A", "B", "DP", "X", "Y", "U", "PC"};
        bool found = false;
        for (unsigned i = 0; i < 8; ++i) {
            if (reg == names[i] || (i == 6 && user_stack && reg == "S") ) {
                if (i == 6 && user_stack && reg == "U") continue;
                mask |= 1u << i;
                found = true;
                break;
            }
        }
        if (!found && reg == "D") { mask |= 0x06; found = true; }
        if (!found) fail(line, "unknown register " + reg);
    }
    return mask;
}

// An indexed operand as source: the postbyte and what follows it.
Bytes encode_index(std::string_view operand, std::uint32_t pc, const NameLabels& labels, bool lenient, std::string_view line) {
    std::string_view s = text::trim(operand);
    bool indirect = false;
    if (s.size() >= 2 && s.front() == '[' && s.back() == ']') {
        indirect = true;
        s = text::trim(s.substr(1, s.size() - 2));
    }
    const std::uint8_t ind = indirect ? 0x10 : 0;
    const std::size_t comma = s.find(',');
    if (comma == std::string_view::npos) {   // [address]
        if (!indirect) fail(line, "an indexed operand needs a comma");
        const std::int64_t v = value(s, labels, pc, lenient);
        return {0x9F, static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)};
    }
    std::string_view offset = text::trim(s.substr(0, comma));
    const std::string base = text::upper(text::trim(s.substr(comma + 1)));
    const auto reg_code = [&](std::string_view r) -> int {
        if (r == "X") return 0;
        if (r == "Y") return 1;
        if (r == "U") return 2;
        if (r == "S") return 3;
        return -1;
    };
    std::string r = base;
    unsigned kind = 99;   // the low nibble of the postbyte
    int reg = -1;
    if (r == "PCR" || r == "PC") {
        std::uint8_t force = 0;   // 0 shortest, 8, 16
        if (offset.starts_with('<')) { force = 8; offset.remove_prefix(1); }
        else if (offset.starts_with('>')) { force = 16; offset.remove_prefix(1); }
        const std::int64_t v = value(offset, labels, pc, lenient);   // (the offset as written: this assembler does not work it out from a label)
        const bool wide = force == 16 || (force == 0 && (v < -128 || v > 127));
        if (!wide) return {static_cast<std::uint8_t>(0x8C | ind), static_cast<std::uint8_t>(v)};
        return {static_cast<std::uint8_t>(0x8D | ind), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)};
    }
    if (offset.empty()) {   // ,R  ,R+  ,R++  ,-R  ,--R
        std::string_view b = r;
        if (b.starts_with("--")) { kind = 3; reg = reg_code(b.substr(2)); }
        else if (b.starts_with('-')) { kind = 2; reg = reg_code(b.substr(1)); }
        else if (b.ends_with("++")) { kind = 1; reg = reg_code(b.substr(0, b.size() - 2)); }
        else if (b.ends_with('+')) { kind = 0; reg = reg_code(b.substr(0, b.size() - 1)); }
        else { kind = 4; reg = reg_code(b); }
        if (reg < 0) fail(line, "unknown index register");
        if (indirect && (kind == 0 || kind == 2)) fail(line, "a single increment cannot be indirect");
        return {static_cast<std::uint8_t>(0x80 | (reg << 5) | ind | kind)};
    }
    reg = reg_code(r);
    if (reg < 0) fail(line, "unknown index register");
    const std::string up = text::upper(offset);
    if (up == "A" || up == "B" || up == "D") {
        kind = up == "A" ? 6 : up == "B" ? 5 : 0xB;
        return {static_cast<std::uint8_t>(0x80 | (reg << 5) | ind | kind)};
    }
    std::uint8_t force = 0;
    if (offset.starts_with('<')) { force = 8; offset.remove_prefix(1); }
    else if (offset.starts_with('>')) { force = 16; offset.remove_prefix(1); }
    const std::int64_t v = value(offset, labels, pc, lenient);
    if (force == 0 && !indirect && v >= -16 && v <= 15) return {static_cast<std::uint8_t>((reg << 5) | (v & 0x1F))};
    const bool wide = force == 16 || (force == 0 && (v < -128 || v > 127));
    if (!wide) return {static_cast<std::uint8_t>(0x88 | (reg << 5) | ind), static_cast<std::uint8_t>(v)};
    return {static_cast<std::uint8_t>(0x89 | (reg << 5) | ind), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)};
}

}  // namespace

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    const std::string_view line = text::trim(line_in);
    const std::size_t space = line.find_first_of(" \t");
    std::string m = text::upper(space == std::string_view::npos ? line : line.substr(0, space));
    for (const auto& [alias, real] : {std::pair<std::string_view, std::string_view>{"ASLA", "LSLA"}, {"ASLB", "LSLB"}, {"ASL", "LSL"}, {"BHS", "BCC"}, {"BLO", "BCS"}, {"LBHS", "LBCC"}, {"LBLO", "LBCS"}}) {
        if (m == alias) m = real;
    }
    const std::string_view operand = space == std::string_view::npos ? std::string_view() : text::trim(line.substr(space + 1));
    if (!is_mnemonic(m)) fail(line, "unknown mnemonic");
    Bytes out;
    const auto emit_op = [&](const Opcode& o) {
        if (o.page != 0) out.push_back(static_cast<std::uint8_t>(0x0F + o.page));
        out.push_back(o.op);
    };
    const auto word = [&](std::int64_t v) {
        out.push_back(static_cast<std::uint8_t>(v >> 8));
        out.push_back(static_cast<std::uint8_t>(v));
    };
    if (operand.empty()) {
        const auto o = opcode_of(m, Mode::Inh);
        if (!o) fail(line, "needs an operand");
        emit_op(*o);
        return out;
    }
    if (const auto o = opcode_of(m, Mode::Regs)) {
        emit_op(*o);
        out.push_back(operand.front() == '#' ? static_cast<std::uint8_t>(value(operand.substr(1), labels, pc, lenient))
                                              : static_cast<std::uint8_t>(parse_regs(operand, m == "PSHU" || m == "PULU", line)));
        return out;
    }
    if (const auto o = opcode_of(m, Mode::Pair)) {
        const std::vector<std::string_view> pair = text::split_trim(operand, ',');
        if (pair.size() != 2) fail(line, "two registers");
        const auto a = pair_register(text::upper(pair[0])), b = pair_register(text::upper(pair[1]));
        if (!a || !b) fail(line, "unknown register");
        emit_op(*o);
        out.push_back(static_cast<std::uint8_t>(*a << 4 | *b));
        return out;
    }
    for (const Mode rel : {Mode::Rel8, Mode::Rel16}) {
        const auto o = opcode_of(m, rel);
        if (!o) continue;
        const std::int64_t target = value(operand, labels, pc, lenient);
        emit_op(*o);
        const std::int64_t from = pc + out.size() + (rel == Mode::Rel8 ? 1 : 2);
        const std::int64_t d = target - from;
        if (rel == Mode::Rel8) {
            if (!lenient && (d < -128 || d > 127)) fail(line, "the branch is out of reach");
            out.push_back(static_cast<std::uint8_t>(d));
        } else {
            word(d);
        }
        return out;
    }
    if (operand.front() == '#') {
        const std::string_view v = operand.substr(1);
        if (const auto o = opcode_of(m, Mode::Imm8)) {
            emit_op(*o);
            out.push_back(static_cast<std::uint8_t>(value(v, labels, pc, lenient)));
            return out;
        }
        if (const auto o = opcode_of(m, Mode::Imm16)) {
            emit_op(*o);
            word(value(v, labels, pc, lenient));
            return out;
        }
        fail(line, "no immediate form");
    }
    const bool indexed = operand.find(',') != std::string_view::npos || operand.front() == '[';
    if (indexed) {
        const auto o = opcode_of(m, Mode::Idx);
        if (!o) fail(line, "no indexed form");
        emit_op(*o);
        const Bytes ix = encode_index(operand, pc, labels, lenient, line);
        out.insert(out.end(), ix.begin(), ix.end());
        return out;
    }
    if (operand.front() == '<') {
        const auto o = opcode_of(m, Mode::Dir);
        if (!o) fail(line, "no direct form");
        emit_op(*o);
        out.push_back(static_cast<std::uint8_t>(value(operand.substr(1), labels, pc, lenient)));
        return out;
    }
    const std::string_view v = operand.front() == '>' ? operand.substr(1) : operand;
    const auto o = opcode_of(m, Mode::Ext);
    if (!o) fail(line, "no extended form");
    emit_op(*o);
    word(value(v, labels, pc, lenient));
    return out;
}

}  // namespace cb::decompile::m6809
