// cb-unity: alone
#include "decompile/cp1610.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "core/text.hpp"

namespace cb::decompile::cp1610 {

namespace {

constexpr std::array<std::string_view, 16> kBranchNames = {"B", "BC", "BOV", "BPL", "BEQ", "BLT", "BLE", "BUSC", "NOPP", "BNC", "BNOV", "BMI", "BNEQ", "BGE", "BGT", "BESC"};
constexpr std::array<std::string_view, 5> kRegNames = {"INCR", "DECR", "COMR", "NEGR", "ADCR"};
constexpr std::array<std::string_view, 8> kShiftNames = {"SWAP", "SLL", "RLC", "SLLC", "SLR", "SAR", "RRC", "SARC"};
constexpr std::array<std::string_view, 6> kRrNames = {"MOVR", "ADDR", "SUBR", "CMPR", "ANDR", "XORR"};
constexpr std::array<std::string_view, 7> kMemNames = {"MVO", "MVI", "ADD", "SUB", "CMP", "AND", "XOR"};

std::string number_text(std::uint32_t v) { return v < 10 ? std::to_string(v) : std::format("${:X}", v); }

}  // namespace

// ---- decoding -----------------------------------------------------------------------------------------------------------

std::optional<Insn> decode(std::span<const std::uint16_t> code, std::size_t offset, std::uint32_t pc, bool sdbd) {
    if (offset >= code.size()) return std::nullopt;
    const unsigned w = code[offset] & 0x3FF;   // (a ROM is 10 bits wide: what is above is not read; the instruction's text cannot say it, so the writer keeps such words as data lines)
    Insn i;
    i.at = pc;
    i.size = 1;
    i.words[0] = code[offset];
    // operand decle k (k = 1, 2): a word that is a decle, when the code has it
    const auto operand = [&](std::size_t k) -> std::optional<unsigned> {
        if (offset + k >= code.size()) return std::nullopt;
        i.words[k] = code[offset + k];
        return code[offset + k] & 0x3FF;
    };
    if (w < 8) {
        switch (w) {
            case 0: i.op = Op::Hlt; i.flow = Flow::Halt; return i;
            case 1: i.op = Op::Sdbd; return i;
            case 2: i.op = Op::Eis; return i;
            case 3: i.op = Op::Dis; return i;
            case 5: i.op = Op::Tci; return i;
            case 6: i.op = Op::Clrc; return i;
            case 7: i.op = Op::Setc; return i;
            default: break;
        }
        const std::optional<unsigned> w1 = operand(1), w2 = operand(2);   // the jumps: J, JE, JD, JSR, JSRE, JSRD
        if (!w1 || !w2) return std::nullopt;
        const unsigned rr = (*w1 >> 8) & 3, ff = *w1 & 3;
        if (ff == 3) return std::nullopt;
        i.op = Op::Jump;
        i.size = 3;
        i.cond = static_cast<std::uint8_t>(rr == 3 ? 7 : 4 + rr);
        i.flag = static_cast<std::uint8_t>(ff);
        i.value = (((*w1 >> 2) & 0x3F) << 10) | *w2;
        i.target = i.value;
        i.flow = rr == 3 ? Flow::Jump : Flow::Call;
        return i;
    }
    if (w < 0x30) {
        const unsigned group = (w >> 3) - 1;   // 0 Incr ... 4 Adcr
        i.op = static_cast<Op>(static_cast<unsigned>(Op::Incr) + group);
        i.r2 = w & 7;
        return i;
    }
    if (w < 0x34) { i.op = Op::Gswd; i.r2 = w & 3; return i; }
    if (w < 0x36) { i.op = Op::Nop; i.cond = w & 1; return i; }
    if (w < 0x38) { i.op = Op::Sin; i.cond = w & 1; return i; }
    if (w < 0x40) { i.op = Op::Rswd; i.r2 = w & 7; return i; }
    if (w < 0x80) {
        i.op = static_cast<Op>(static_cast<unsigned>(Op::Swap) + ((w >> 3) & 7));
        i.cond = static_cast<std::uint8_t>(((w >> 2) & 1) + 1);
        i.r2 = w & 3;
        return i;
    }
    if (w < 0x200) {
        i.op = static_cast<Op>(static_cast<unsigned>(Op::Movr) + ((w >> 6) - 2));
        i.r1 = (w >> 3) & 7;
        i.r2 = w & 7;
        if (i.r2 == 7 && i.op != Op::Cmpr) i.flow = (i.op == Op::Movr && (i.r1 == 4 || i.r1 == 5)) ? Flow::Return : Flow::Indirect;
        return i;
    }
    if (w < 0x240) {
        const std::optional<unsigned> d = operand(1);
        if (!d) return std::nullopt;
        const bool ext = (w >> 4) & 1;
        i.size = 2;
        i.backward = (w >> 5) & 1;
        i.cond = w & 15;
        i.op = ext ? Op::Bext : Op::Branch;
        i.value = *d;
        i.target = (i.backward ? pc + 1 - *d : pc + 2 + *d) & 0xFFFF;
        i.flow = ext || (i.cond != 0 && i.cond != 8) ? Flow::Branch : (i.cond == 0 ? Flow::Jump : Flow::None);
        return i;
    }
    // MVO MVI ADD SUB CMP AND XOR
    i.op = static_cast<Op>(static_cast<unsigned>(Op::Mvo) + ((w >> 6) - 9));
    const unsigned mmm = (w >> 3) & 7;
    i.r2 = w & 7;
    i.r1 = static_cast<std::uint8_t>(mmm);
    if (mmm == 0) {
        const std::optional<unsigned> a = operand(1);
        if (!a) return std::nullopt;
        i.access = Access::Direct;
        i.size = 2;
        i.value = *a;
    } else if (mmm < 7) {
        i.access = Access::Indirect;
    } else {
        if (i.op == Op::Mvo) return std::nullopt;
        const std::optional<unsigned> a = operand(1);
        if (!a) return std::nullopt;
        i.access = Access::Immediate;
        i.size = 2;
        i.sdbd = sdbd;
        i.value = *a;
        if (sdbd) {
            const std::optional<unsigned> b = operand(2);
            if (!b || *a > 0xFF || *b > 0xFF) return std::nullopt;
            i.size = 3;
            i.value = *a | (*b << 8);
        }
    }
    if (i.op != Op::Mvo && i.op != Op::Cmp && i.r2 == 7) {   // a write to the program counter
        if (i.op == Op::Mvi && i.access == Access::Immediate) { i.flow = Flow::Jump; i.target = i.value; }
        else i.flow = i.op == Op::Mvi && i.access == Access::Indirect && mmm == 6 ? Flow::Return : Flow::Indirect;
    }
    return i;
}

// ---- text ---------------------------------------------------------------------------------------------------------------

std::string mnemonic(const Insn& i) {
    switch (i.op) {
        case Op::Hlt: return "HLT";
        case Op::Sdbd: return "SDBD";
        case Op::Eis: return "EIS";
        case Op::Dis: return "DIS";
        case Op::Tci: return "TCI";
        case Op::Clrc: return "CLRC";
        case Op::Setc: return "SETC";
        case Op::Nop: return "NOP";
        case Op::Sin: return "SIN";
        case Op::Gswd: return "GSWD";
        case Op::Rswd: return "RSWD";
        case Op::Bext: return "BEXT";
        case Op::Branch: return std::string(kBranchNames[i.cond]);
        case Op::Jump: return std::string(i.cond == 7 ? "J" : "JSR") + (i.flag == 1 ? "E" : i.flag == 2 ? "D" : "");
        default: break;
    }
    const unsigned o = static_cast<unsigned>(i.op);
    if (o >= static_cast<unsigned>(Op::Incr) && o <= static_cast<unsigned>(Op::Adcr)) return std::string(kRegNames[o - static_cast<unsigned>(Op::Incr)]);
    if (o >= static_cast<unsigned>(Op::Swap) && o <= static_cast<unsigned>(Op::Sarc)) return std::string(kShiftNames[o - static_cast<unsigned>(Op::Swap)]);
    if (o >= static_cast<unsigned>(Op::Movr) && o <= static_cast<unsigned>(Op::Xorr)) return std::string(kRrNames[o - static_cast<unsigned>(Op::Movr)]);
    std::string m(kMemNames[o - static_cast<unsigned>(Op::Mvo)]);
    return i.access == Access::Indirect ? m + "@" : i.access == Access::Immediate ? m + "I" : m;
}

std::string text(const Insn& i, const AddressLabels& labels, bool label_immediates) {
    const std::string m = mnemonic(i);
    const auto reg = [](unsigned r) { return "R" + std::to_string(r); };
    const auto address = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && !it->second.empty() ? it->second : std::format("${:04X}", a);
    };
    switch (i.op) {
        case Op::Hlt: case Op::Sdbd: case Op::Eis: case Op::Dis: case Op::Tci: case Op::Clrc: case Op::Setc: return m;
        case Op::Nop: case Op::Sin: return i.cond ? m + " 1" : m;
        case Op::Incr: case Op::Decr: case Op::Comr: case Op::Negr: case Op::Adcr: case Op::Gswd: case Op::Rswd: return m + " " + reg(i.r2);
        case Op::Swap: case Op::Sll: case Op::Rlc: case Op::Sllc: case Op::Slr: case Op::Sar: case Op::Rrc: case Op::Sarc:
            return m + " " + reg(i.r2) + (i.cond == 2 ? ",2" : "");
        case Op::Movr: case Op::Addr: case Op::Subr: case Op::Cmpr: case Op::Andr: case Op::Xorr: return m + " " + reg(i.r1) + "," + reg(i.r2);
        case Op::Branch: return m + " " + address(*i.target);
        case Op::Bext: return m + " " + std::to_string(i.cond) + "," + address(*i.target);
        case Op::Jump: return i.cond == 7 ? m + " " + address(i.value) : m + " " + reg(i.cond) + "," + address(i.value);
        default: break;
    }
    // the memory instructions
    const bool store = i.op == Op::Mvo;
    switch (i.access) {
        case Access::Direct: return store ? m + " " + reg(i.r2) + "," + address(i.value) : m + " " + address(i.value) + "," + reg(i.r2);
        case Access::Indirect: return store ? m + " " + reg(i.r2) + "," + reg(i.r1) : m + " " + reg(i.r1) + "," + reg(i.r2);
        default: {
            const auto it = labels.find(i.value);
            const std::string v = label_immediates && it != labels.end() && !it->second.empty() ? it->second : number_text(i.value);
            return m + " #" + v + "," + reg(i.r2);
        }
    }
}

// ---- the assembler ------------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void fail(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

bool is_ident_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.'; }

std::optional<unsigned> parse_reg(std::string_view s) {
    const std::string r = text::upper(text::trim(s));
    if (r == "SP") return 6;
    if (r == "PC") return 7;
    if (r.size() == 2 && r[0] == 'R' && r[1] >= '0' && r[1] <= '7') return static_cast<unsigned>(r[1] - '0');
    return std::nullopt;
}

}  // namespace

std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient) {
    const std::string_view e = text::trim(expr);
    if (e.empty()) throw ParseError("", 0, 0, "an operand is missing");
    std::int64_t total = 0;
    std::size_t i = 0;
    int sign = 1;
    bool expect_term = true;
    while (i < e.size()) {
        if (e[i] == ' ' || e[i] == '\t') { ++i; continue; }
        if (e[i] == '+' || e[i] == '-') {
            if (e[i] == '-') sign = -sign;
            ++i;
            expect_term = true;
            continue;
        }
        if (!expect_term) throw ParseError("", 0, 0, "cannot read '" + std::string(e) + "' as a value");
        std::size_t j = i;
        while (j < e.size() && (is_ident_char(e[j]) || e[j] == '$')) ++j;
        const std::string_view term = e.substr(i, j - i);
        if (term.empty()) throw ParseError("", 0, 0, "cannot read '" + std::string(e) + "' as a value");
        std::int64_t v = 0;
        if (term[0] == '$') {
            const auto p = text::parse_int<std::int64_t>(term.substr(1), 16);
            if (!p) throw ParseError("", 0, 0, "cannot read '" + std::string(term) + "' as a number");
            v = *p;
        } else if (term.size() > 2 && term[0] == '0' && (term[1] == 'x' || term[1] == 'X')) {
            const auto p = text::parse_int<std::int64_t>(term.substr(2), 16);
            if (!p) throw ParseError("", 0, 0, "cannot read '" + std::string(term) + "' as a number");
            v = *p;
        } else if (std::isdigit(static_cast<unsigned char>(term[0]))) {
            const auto p = text::parse_int<std::int64_t>(term, 10);
            if (!p) throw ParseError("", 0, 0, "cannot read '" + std::string(term) + "' as a number");
            v = *p;
        } else if (const auto it = labels.find(term); it != labels.end()) {
            v = it->second;
        } else if (lenient) {
            v = 0;
        } else {
            throw ParseError("", 0, 0, "unknown name '" + std::string(term) + "'");
        }
        total += sign * v;
        sign = 1;
        expect_term = false;
        i = j;
    }
    return total;
}

std::vector<std::uint16_t> assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, bool lenient, bool sdbd) {
    const std::string_view line = text::trim(line_in);
    const std::size_t space = line.find_first_of(" \t");
    const std::string m = text::upper(space == std::string_view::npos ? line : line.substr(0, space));
    const std::string_view operand_text = space == std::string_view::npos ? std::string_view() : text::trim(line.substr(space + 1));
    std::vector<std::string_view> ops;
    if (!operand_text.empty()) ops = text::split_trim(operand_text, ',');
    const auto number = [&](std::string_view e) { return value(e, labels, lenient); };
    const auto want = [&](std::size_t n) {
        if (ops.size() != n) fail(line, std::format("{} operand{} expected", n, n == 1 ? "" : "s"));
    };
    const auto reg = [&](std::string_view s) {
        const std::optional<unsigned> r = parse_reg(s);
        if (!r) fail(line, "'" + std::string(s) + "' is not a register");
        return *r;
    };
    const auto in_range = [&](std::int64_t v, std::int64_t lo, std::int64_t hi, const char* what) {
        if (!lenient && (v < lo || v > hi)) fail(line, std::string(what) + " is out of range");
    };
    using Words = std::vector<std::uint16_t>;
    const auto word = [](unsigned v) { return static_cast<std::uint16_t>(v); };

    static const std::pair<std::string_view, unsigned> inherent[] = {{"HLT", 0}, {"SDBD", 1}, {"EIS", 2}, {"DIS", 3}, {"TCI", 5}, {"CLRC", 6}, {"SETC", 7}};
    for (const auto& [name, code] : inherent) {
        if (m == name) {
            want(0);
            return {word(code)};
        }
    }
    if (m == "NOP" || m == "SIN") {
        unsigned flag = 0;
        if (!ops.empty()) {
            want(1);
            const std::int64_t v = number(ops[0]);
            if (v != 0 && v != 1) fail(line, "NOP and SIN take 1 or nothing");
            flag = static_cast<unsigned>(v);
        }
        return {word((m == "NOP" ? 0x34 : 0x36) | flag)};
    }
    for (unsigned k = 0; k < kRegNames.size(); ++k) {
        if (m == kRegNames[k]) {
            want(1);
            return {word((k + 1) * 8 + reg(ops[0]))};
        }
    }
    if (m == "GSWD" || m == "RSWD") {
        want(1);
        const unsigned r = reg(ops[0]);
        if (m == "GSWD" && r > 3) fail(line, "GSWD takes R0 to R3");
        return {word((m == "GSWD" ? 0x30 : 0x38) + r)};
    }
    for (unsigned k = 0; k < kShiftNames.size(); ++k) {
        if (m == kShiftNames[k]) {
            if (ops.size() != 1 && ops.size() != 2) fail(line, "a register and an optional count");
            const unsigned r = reg(ops[0]);
            if (r > 3) fail(line, "a shift takes R0 to R3");
            unsigned n = 1;
            if (ops.size() == 2) {
                const std::int64_t v = number(ops[1]);
                if (v != 1 && v != 2) fail(line, "a shift count is 1 or 2");
                n = static_cast<unsigned>(v);
            }
            return {word(0x40 + k * 8 + (n == 2 ? 4 : 0) + r)};
        }
    }
    for (unsigned k = 0; k < kRrNames.size(); ++k) {
        if (m == kRrNames[k]) {
            want(2);
            return {word(0x80 + k * 0x40 + (reg(ops[0]) << 3) + reg(ops[1]))};
        }
    }
    // branches
    std::string branch = m == "BNE" ? "BNEQ" : m;
    for (unsigned c = 0; c < kBranchNames.size(); ++c) {
        if (branch == kBranchNames[c] || (m == "BEXT")) {
            const bool ext = m == "BEXT";
            if (ext) {
                want(2);
                c = static_cast<unsigned>(number(ops[0]));
                in_range(c, 0, 15, "the external condition");
                c &= 15;
            } else {
                want(1);
            }
            const std::int64_t target = number(ops[ext ? 1 : 0]);
            const bool forward = target >= static_cast<std::int64_t>(pc) + 2;
            const std::int64_t d = forward ? target - (pc + 2) : static_cast<std::int64_t>(pc) + 1 - target;
            in_range(d, 0, 0x3FF, "the branch distance");
            return {word(0x200 | (forward ? 0 : 0x20) | (ext ? 0x10 : 0) | c), word(static_cast<unsigned>(d) & 0x3FF)};
        }
    }
    // jumps
    for (const auto& [name, link, flag] : {std::tuple<std::string_view, bool, unsigned>{"J", false, 0}, {"JE", false, 1}, {"JD", false, 2}, {"JSR", true, 0}, {"JSRE", true, 1}, {"JSRD", true, 2}}) {
        if (m != name) continue;
        want(link ? 2 : 1);
        unsigned rr = 3;
        if (link) {
            const unsigned r = reg(ops[0]);
            if (r < 4 || r > 6) fail(line, "a link register is R4, R5 or R6");
            rr = r - 4;
        }
        const std::int64_t a = number(ops[link ? 1 : 0]);
        in_range(a, 0, 0xFFFF, "the address");
        const unsigned ua = static_cast<unsigned>(a) & 0xFFFF;
        return {word(4), word(rr << 8 | ((ua >> 10) & 0x3F) << 2 | flag), word(ua & 0x3FF)};
    }
    // memory instructions: MVO MVI ADD SUB CMP AND XOR, with @ (indirect) or a trailing I (immediate)
    std::string base = m;
    bool indirect = false, immediate = false;
    if (!base.empty() && base.back() == '@') {
        indirect = true;
        base.pop_back();
    } else if (base.size() == 4 && base.back() == 'I' && base != "MVOI") {
        immediate = true;
        base.pop_back();
    }
    for (unsigned k = 0; k < kMemNames.size(); ++k) {
        if (base != kMemNames[k]) continue;
        const bool store = k == 0;
        if (immediate && store) break;
        want(2);
        const unsigned opcode = 0x240 + k * 0x40;
        if (store) {
            const unsigned r = reg(ops[0]);
            if (indirect) {
                const unsigned p = reg(ops[1]);
                if (p == 0 || p == 7) fail(line, "an indirect access goes through R1 to R6");
                return {word(opcode | p << 3 | r)};
            }
            const std::int64_t a = number(ops[1]);
            in_range(a, 0, 0x3FF, "a direct address");
            return {word(opcode | r), word(static_cast<unsigned>(a) & 0x3FF)};
        }
        const unsigned r = reg(ops[1]);
        if (indirect) {
            const unsigned p = reg(ops[0]);
            if (p == 0 || p == 7) fail(line, "an indirect access goes through R1 to R6");
            return {word(opcode | p << 3 | r)};
        }
        if (immediate) {
            std::string_view v = ops[0];
            if (!v.empty() && v.front() == '#') v.remove_prefix(1);
            const std::int64_t n = number(v);
            if (sdbd) {
                in_range(n, -32768, 65535, "a 16-bit immediate");
                return {word(opcode | 7 << 3 | r), word(static_cast<unsigned>(n) & 0xFF), word((static_cast<unsigned>(n) >> 8) & 0xFF)};
            }
            in_range(n, 0, 0x3FF, "a 10-bit immediate");
            return {word(opcode | 7 << 3 | r), word(static_cast<unsigned>(n) & 0x3FF)};
        }
        const std::int64_t a = number(ops[0]);
        in_range(a, 0, 0x3FF, "a direct address");
        return {word(opcode | r), word(static_cast<unsigned>(a) & 0x3FF)};
    }
    fail(line, "unknown mnemonic");
}

}  // namespace cb::decompile::cp1610
