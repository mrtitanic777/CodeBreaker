// cb-unity: alone
#include "decompile/v810.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <unordered_map>
#include <vector>

#include "core/text.hpp"

namespace cb::decompile::v810 {

namespace {

using Table = std::array<std::optional<OpcodeEntry>, 64>;

Table build_main() {
    Table t{};
    const auto put = [&](unsigned op, std::string_view m, Mode mode) { t[op] = OpcodeEntry{m, mode}; };
    put(0x00, "mov", Mode::R1R2);   put(0x01, "add", Mode::R1R2);   put(0x02, "sub", Mode::R1R2);   put(0x03, "cmp", Mode::R1R2);
    put(0x04, "shl", Mode::R1R2);   put(0x05, "shr", Mode::R1R2);   put(0x06, "jmp", Mode::Jmp);     put(0x07, "sar", Mode::R1R2);
    put(0x08, "mul", Mode::R1R2);   put(0x09, "div", Mode::R1R2);   put(0x0A, "mulu", Mode::R1R2);   put(0x0B, "divu", Mode::R1R2);
    put(0x0C, "or", Mode::R1R2);    put(0x0D, "and", Mode::R1R2);   put(0x0E, "xor", Mode::R1R2);    put(0x0F, "not", Mode::R1R2);
    put(0x10, "mov", Mode::S5R2);   put(0x11, "add", Mode::S5R2);   put(0x12, "setf", Mode::U5R2);   put(0x13, "cmp", Mode::S5R2);
    put(0x14, "shl", Mode::U5R2);   put(0x15, "shr", Mode::U5R2);   put(0x16, "cli", Mode::None);    put(0x17, "sar", Mode::U5R2);
    put(0x18, "trap", Mode::Trap);  put(0x19, "reti", Mode::None);  put(0x1A, "halt", Mode::None);
    put(0x1C, "ldsr", Mode::LdSr);  put(0x1D, "stsr", Mode::StSr);  put(0x1E, "sei", Mode::None);
    put(0x1F, "", Mode::None);   // (the bit-string instructions: the sub-table names them)
    put(0x28, "movea", Mode::Imm16S); put(0x29, "addi", Mode::Imm16S); put(0x2A, "jr", Mode::Jump26); put(0x2B, "jal", Mode::Jump26);
    put(0x2C, "ori", Mode::Imm16U); put(0x2D, "andi", Mode::Imm16U); put(0x2E, "xori", Mode::Imm16U); put(0x2F, "movhi", Mode::Imm16U);
    put(0x30, "ld.b", Mode::Load);  put(0x31, "ld.h", Mode::Load);  put(0x33, "ld.w", Mode::Load);
    put(0x34, "st.b", Mode::Store); put(0x35, "st.h", Mode::Store); put(0x37, "st.w", Mode::Store);
    put(0x38, "in.b", Mode::Load);  put(0x39, "in.h", Mode::Load);  put(0x3A, "caxi", Mode::Load);   put(0x3B, "in.w", Mode::Load);
    put(0x3C, "out.b", Mode::Store); put(0x3D, "out.h", Mode::Store); put(0x3F, "out.w", Mode::Store);
    put(0x3E, "", Mode::None);   // (the extended instructions)
    return t;
}

std::array<std::optional<OpcodeEntry>, 64> build_extended() {
    std::array<std::optional<OpcodeEntry>, 64> t{};
    t[0x00] = OpcodeEntry{"cmpf.s", Mode::R1R2};  t[0x02] = OpcodeEntry{"cvt.ws", Mode::R1R2};  t[0x03] = OpcodeEntry{"cvt.sw", Mode::R1R2};
    t[0x04] = OpcodeEntry{"addf.s", Mode::R1R2};  t[0x05] = OpcodeEntry{"subf.s", Mode::R1R2};  t[0x06] = OpcodeEntry{"mulf.s", Mode::R1R2};
    t[0x07] = OpcodeEntry{"divf.s", Mode::R1R2};  t[0x08] = OpcodeEntry{"xb", Mode::R2};       t[0x09] = OpcodeEntry{"xh", Mode::R2};
    t[0x0A] = OpcodeEntry{"rev", Mode::R1R2};     t[0x0B] = OpcodeEntry{"trnc.sw", Mode::R1R2}; t[0x0C] = OpcodeEntry{"mpyhw", Mode::R1R2};
    return t;
}

std::array<std::optional<OpcodeEntry>, 32> build_bitstring() {
    std::array<std::optional<OpcodeEntry>, 32> t{};
    const std::string_view names[16] = {"sch0bsu", "sch0bsd", "sch1bsu", "sch1bsd", "", "", "", "", "orbsu", "andbsu", "xorbsu", "movbsu", "ornbsu", "andnbsu", "xornbsu", "notbsu"};
    for (unsigned i = 0; i < 16; ++i) {
        if (!names[i].empty()) t[i] = OpcodeEntry{names[i], Mode::None};
    }
    return t;
}

std::int32_t sext(std::uint32_t v, unsigned bits) {
    const std::uint32_t m = 1u << (bits - 1);
    return static_cast<std::int32_t>((v ^ m) - m);
}

std::string number_text(std::int64_t v) {
    if (v >= -9 && v <= 9) return std::to_string(v);
    return v < 0 ? std::format("-0x{:X}", static_cast<std::uint64_t>(-v)) : std::format("0x{:X}", static_cast<std::uint64_t>(v));
}

std::string reg_text(unsigned r) { return "r" + std::to_string(r); }

}  // namespace

const std::array<std::optional<OpcodeEntry>, 64>& opcode_table() {
    static const Table t = build_main();
    return t;
}
const std::array<std::optional<OpcodeEntry>, 64>& extended_table() {
    static const std::array<std::optional<OpcodeEntry>, 64> t = build_extended();
    return t;
}
const std::array<std::optional<OpcodeEntry>, 32>& bitstring_table() {
    static const std::array<std::optional<OpcodeEntry>, 32> t = build_bitstring();
    return t;
}

std::string_view system_register_name(unsigned n) {
    switch (n) {
        case 0: return "eipc";
        case 1: return "eipsw";
        case 2: return "fepc";
        case 3: return "fepsw";
        case 4: return "ecr";
        case 5: return "psw";
        case 6: return "pir";
        case 7: return "tkcw";
        case 24: return "chcw";
        case 25: return "adtre";
        default: return {};
    }
}

// ---- the mnemonics, for the assembler -----------------------------------------------------------------------------------

namespace {

struct Candidate {
    unsigned id;
    Mode mode;
    unsigned cond = 0;
};

const std::unordered_map<std::string, std::vector<Candidate>>& by_mnemonic() {
    static const std::unordered_map<std::string, std::vector<Candidate>> table = [] {
        std::unordered_map<std::string, std::vector<Candidate>> m;
        for (unsigned op = 0; op < 64; ++op) {
            const auto& e = opcode_table()[op];
            if (!e || e->mnemonic.empty()) continue;
            m[std::string(e->mnemonic)].push_back({op, e->mode});
        }
        for (unsigned s = 0; s < 64; ++s) {
            if (const auto& e = extended_table()[s]) m[std::string(e->mnemonic)].push_back({id::kExtendedBase + s, e->mode});
        }
        for (unsigned s = 0; s < 32; ++s) {
            if (const auto& e = bitstring_table()[s]) m[std::string(e->mnemonic)].push_back({id::kBitStringBase + s, Mode::None});
        }
        for (unsigned c = 0; c < 16; ++c) m["b" + std::string(kConditions[c])].push_back({id::kBranch, Mode::Branch, c});
        for (const auto& [alias, c] : {std::pair<const char*, unsigned>{"bc", 1}, {"bz", 2}, {"bnc", 9}, {"bnz", 10}}) m[alias].push_back({id::kBranch, Mode::Branch, c});
        m["nop"].push_back({id::kMov, Mode::None});
        return m;
    }();
    return table;
}

}  // namespace

bool is_mnemonic(std::string_view mnemonic) { return by_mnemonic().contains(text::lower(mnemonic)); }

// ---- decoding -----------------------------------------------------------------------------------------------------------

std::optional<Insn> decode(ByteView code, std::size_t offset, std::uint32_t pc) {
    if (offset + 2 > code.size()) return std::nullopt;
    const std::uint32_t hw0 = code[offset] | code[offset + 1] << 8;
    Insn i;
    i.at = pc;
    i.raw[0] = code[offset];
    i.raw[1] = code[offset + 1];
    i.r1 = hw0 & 31;
    i.r2 = (hw0 >> 5) & 31;
    if ((hw0 >> 13) == 4) {   // a branch
        const std::uint32_t d = hw0 & 0x1FF;
        if (d & 1) return std::nullopt;
        i.size = 2;
        i.id = id::kBranch;
        i.cond = (hw0 >> 9) & 15;
        i.mnemonic = {};   // (set below: needs static storage)
        i.mode = Mode::Branch;
        i.imm = sext(d, 9);
        i.target = pc + static_cast<std::uint32_t>(i.imm);
        i.flow = i.cond == 5 ? Flow::Jump : i.cond == 13 ? Flow::None : Flow::Branch;
        i.r1 = i.r2 = 0;
        static const std::array<std::string, 16> names = [] {
            std::array<std::string, 16> n;
            for (unsigned c = 0; c < 16; ++c) n[c] = "b" + std::string(kConditions[c]);
            return n;
        }();
        i.mnemonic = names[i.cond];
        return i;
    }
    const unsigned opcode = hw0 >> 10;
    const auto& entry = opcode_table()[opcode];
    if (!entry) return std::nullopt;
    i.size = opcode >= 0x28 ? 4 : 2;
    if (offset + i.size > code.size()) return std::nullopt;
    std::uint32_t hw1 = 0;
    if (i.size == 4) {
        i.raw[2] = code[offset + 2];
        i.raw[3] = code[offset + 3];
        hw1 = code[offset + 2] | code[offset + 3] << 8;
    }
    i.id = opcode;
    i.mnemonic = entry->mnemonic;
    i.mode = entry->mode;
    switch (opcode) {
        case id::kBitString: {
            const auto& b = bitstring_table()[i.r1];
            if (!b || i.r2 != 0) return std::nullopt;
            i.id = id::kBitStringBase + i.r1;
            i.mnemonic = b->mnemonic;
            i.imm = i.r1;
            i.r1 = 0;
            return i;
        }
        case id::kExtended: {
            const unsigned sub = hw1 >> 10;
            const auto& e = extended_table()[sub];
            if (!e || (hw1 & 0x3FF) != 0) return std::nullopt;
            if (e->mode == Mode::R2 && i.r1 != 0) return std::nullopt;
            i.id = id::kExtendedBase + sub;
            i.mnemonic = e->mnemonic;
            i.mode = e->mode;
            return i;
        }
        default: break;
    }
    switch (i.mode) {
        case Mode::None:
            if (i.r1 != 0 || i.r2 != 0) return std::nullopt;
            if (opcode == id::kReti) i.flow = Flow::Return;
            if (opcode == id::kHalt) i.flow = Flow::Halt;
            break;
        case Mode::R1R2: break;
        case Mode::Jmp:
            if (i.r2 != 0) return std::nullopt;
            i.flow = i.r1 == 31 ? Flow::Return : Flow::Indirect;
            break;
        case Mode::S5R2: i.imm = sext(i.r1, 5); i.r1 = 0; break;
        case Mode::U5R2: i.imm = i.r1; i.r1 = 0; break;
        case Mode::Trap:
            if (i.r2 != 0) return std::nullopt;
            i.imm = i.r1;
            i.r1 = 0;
            break;
        case Mode::LdSr:
        case Mode::StSr: i.imm = i.r1; i.r1 = 0; break;
        case Mode::Jump26: {
            const std::uint32_t d = ((hw0 & 0x3FF) << 16) | hw1;
            if (d & 1) return std::nullopt;
            i.imm = sext(d, 26);
            i.target = pc + static_cast<std::uint32_t>(i.imm);
            i.flow = opcode == id::kJal ? Flow::Call : Flow::Jump;
            i.r1 = i.r2 = 0;
            break;
        }
        case Mode::Imm16S: i.imm = sext(hw1, 16); break;
        case Mode::Imm16U: i.imm = static_cast<std::int32_t>(hw1); break;
        case Mode::Load:
        case Mode::Store: i.imm = sext(hw1, 16); break;
        default: return std::nullopt;
    }
    return i;
}

// ---- text ---------------------------------------------------------------------------------------------------------------

std::string text(const Insn& i, const AddressLabels& labels, std::string_view immediate) {
    const std::string m(i.mnemonic);
    const auto address = [&](std::uint32_t a) {
        const auto it = labels.find(a);
        return it != labels.end() && !it->second.empty() ? it->second : std::format("0x{:08X}", a);
    };
    const auto imm = [&] { return immediate.empty() ? number_text(i.imm) : std::string(immediate); };
    switch (i.mode) {
        case Mode::None: return m;
        case Mode::R1R2: return m + " " + reg_text(i.r1) + "," + reg_text(i.r2);
        case Mode::R2: return m + " " + reg_text(i.r2);
        case Mode::Jmp: return m + " [" + reg_text(i.r1) + "]";
        case Mode::S5R2:
        case Mode::U5R2: return m + " " + imm() + "," + reg_text(i.r2);
        case Mode::Trap: return m + " " + imm();
        case Mode::LdSr:
        case Mode::StSr: {
            const std::string_view name = system_register_name(static_cast<unsigned>(i.imm));
            const std::string sys = name.empty() ? std::to_string(i.imm) : std::string(name);
            return i.mode == Mode::LdSr ? m + " " + reg_text(i.r2) + "," + sys : m + " " + sys + "," + reg_text(i.r2);
        }
        case Mode::Branch:
        case Mode::Jump26: return m + " " + (immediate.empty() ? address(*i.target) : std::string(immediate));
        case Mode::Imm16S:
        case Mode::Imm16U: return m + " " + imm() + "," + reg_text(i.r1) + "," + reg_text(i.r2);
        case Mode::Load: return m + " " + imm() + "[" + reg_text(i.r1) + "]," + reg_text(i.r2);
        case Mode::Store: return m + " " + reg_text(i.r2) + "," + imm() + "[" + reg_text(i.r1) + "]";
    }
    return m;
}

// ---- the assembler ------------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void fail(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

bool is_ident_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$'; }

class Expr {
public:
    Expr(std::string_view s, const NameLabels& labels, bool lenient) : s_(s), labels_(labels), lenient_(lenient) {}

    std::int64_t parse() {
        const std::int64_t v = sum();
        skip();
        if (i_ != s_.size()) throw ParseError("", 0, 0, "cannot read '" + std::string(s_) + "' as a value");
        return v;
    }

private:
    void skip() {
        while (i_ < s_.size() && (s_[i_] == ' ' || s_[i_] == '\t')) ++i_;
    }
    std::int64_t sum() {
        std::int64_t total = unary();
        for (;;) {
            skip();
            if (i_ < s_.size() && (s_[i_] == '+' || s_[i_] == '-')) {
                const bool minus = s_[i_++] == '-';
                const std::int64_t v = unary();
                total += minus ? -v : v;
            } else {
                return total;
            }
        }
    }
    std::int64_t unary() {
        skip();
        if (i_ < s_.size() && s_[i_] == '-') {
            ++i_;
            return -unary();
        }
        if (i_ < s_.size() && s_[i_] == '+') {
            ++i_;
            return unary();
        }
        return atom();
    }
    std::int64_t atom() {
        skip();
        if (i_ >= s_.size()) throw ParseError("", 0, 0, "an operand is missing");
        if (s_[i_] == '(') {
            ++i_;
            const std::int64_t v = sum();
            skip();
            if (i_ >= s_.size() || s_[i_] != ')') throw ParseError("", 0, 0, "a parenthesis is not closed");
            ++i_;
            return v;
        }
        const std::size_t begin = i_;
        while (i_ < s_.size() && is_ident_char(s_[i_])) ++i_;
        const std::string_view word = s_.substr(begin, i_ - begin);
        if (word.empty()) throw ParseError("", 0, 0, "cannot read '" + std::string(s_) + "' as a value");
        if ((word == "hi" || word == "lo") && i_ < s_.size() && s_[i_] == '(') {
            ++i_;
            const std::int64_t v = sum();
            skip();
            if (i_ >= s_.size() || s_[i_] != ')') throw ParseError("", 0, 0, "a parenthesis is not closed");
            ++i_;
            return word == "hi" ? ((v + 0x8000) >> 16) & 0xFFFF : v & 0xFFFF;
        }
        if (word.size() > 1 && word[0] == '$') {   // $1F, as the data lines of the source writers spell a byte
            const std::optional<std::int64_t> v = text::parse_int<std::int64_t>(word.substr(1), 16);
            if (!v) throw ParseError("", 0, 0, "cannot read '" + std::string(word) + "' as a number");
            return *v;
        }
        if (std::isdigit(static_cast<unsigned char>(word[0]))) {
            const std::optional<std::int64_t> v = word.size() > 2 && word[0] == '0' && (word[1] == 'x' || word[1] == 'X') ? text::parse_int<std::int64_t>(word.substr(2), 16)
                                                                                                                               : text::parse_int<std::int64_t>(word, 10);
            if (!v) throw ParseError("", 0, 0, "cannot read '" + std::string(word) + "' as a number");
            return *v;
        }
        if (const auto it = labels_.find(word); it != labels_.end()) return it->second;
        if (lenient_) return 0;
        throw ParseError("", 0, 0, "unknown name '" + std::string(word) + "'");
    }

    std::string_view s_;
    const NameLabels& labels_;
    bool lenient_;
    std::size_t i_ = 0;
};

std::optional<unsigned> parse_reg(std::string_view s) {
    const std::string r = text::lower(text::trim(s));
    if (r == "hp") return 2;
    if (r == "sp") return 3;
    if (r == "gp") return 4;
    if (r == "tp") return 5;
    if (r == "lp") return 31;
    if (r.size() >= 2 && r[0] == 'r' && std::isdigit(static_cast<unsigned char>(r[1]))) {
        const std::optional<unsigned> n = text::parse_int<unsigned>(std::string_view(r).substr(1), 10);
        if (n && *n < 32) return n;
    }
    return std::nullopt;
}

unsigned need_reg(std::string_view s, std::string_view line) {
    const std::optional<unsigned> r = parse_reg(s);
    if (!r) fail(line, "'" + std::string(s) + "' is not a register");
    return *r;
}

// `disp[rN]` -> (disp expression, register)
std::pair<std::string_view, unsigned> parse_memory(std::string_view s, std::string_view line) {
    s = text::trim(s);
    const std::size_t open = s.find('[');
    if (open == std::string_view::npos || s.back() != ']') fail(line, "a memory operand is disp[reg]");
    const std::string_view disp = text::trim(s.substr(0, open));
    return {disp.empty() ? std::string_view("0") : disp, need_reg(s.substr(open + 1, s.size() - open - 2), line)};
}

void put16(Bytes& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}

}  // namespace

std::int64_t value(std::string_view expr, const NameLabels& labels, bool lenient) { return Expr(expr, labels, lenient).parse(); }

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, bool lenient) {
    const std::string_view line = text::trim(line_in);
    const std::size_t space = line.find_first_of(" \t");
    const std::string m = text::lower(space == std::string_view::npos ? line : line.substr(0, space));
    const std::string_view operand_text = space == std::string_view::npos ? std::string_view() : text::trim(line.substr(space + 1));
    const auto found = by_mnemonic().find(m);
    if (found == by_mnemonic().end()) fail(line, "unknown mnemonic");
    std::vector<std::string_view> ops;
    if (!operand_text.empty()) ops = text::split_trim(operand_text, ',');
    const auto number = [&](std::string_view e) { return value(e, labels, lenient); };
    const auto range = [&](std::int64_t v, std::int64_t lo, std::int64_t hi, const char* what) {
        if (!lenient && (v < lo || v > hi)) fail(line, std::string(what) + " is out of range");
    };

    // Pick the candidate the operands fit: a register first operand means the register form of mov add cmp shl shr sar.
    const Candidate* c = nullptr;
    for (const Candidate& cand : found->second) {
        const bool reg_form = cand.mode == Mode::R1R2;
        const bool imm_form = cand.mode == Mode::S5R2 || cand.mode == Mode::U5R2;
        if (reg_form || imm_form) {
            if (ops.size() != 2) continue;
            if (parse_reg(ops[0]).has_value() != reg_form) continue;
        }
        c = &cand;
        break;
    }
    if (c == nullptr) fail(line, "no such form");
    Bytes out;
    const auto want = [&](std::size_t n) {
        if (ops.size() != n) fail(line, std::format("{} operand{} expected", n, n == 1 ? "" : "s"));
    };
    const auto first = [&](unsigned opcode, unsigned r2, unsigned r1) { return static_cast<std::uint32_t>(opcode << 10 | r2 << 5 | r1); };
    switch (c->mode) {
        case Mode::None:
            want(0);
            if (c->id >= id::kBitStringBase && c->id < id::kExtendedBase) put16(out, first(id::kBitString, 0, c->id - id::kBitStringBase));
            else put16(out, first(c->id, 0, 0));
            break;
        case Mode::R1R2:
            want(2);
            if (c->id >= id::kExtendedBase) {
                put16(out, first(id::kExtended, need_reg(ops[1], line), need_reg(ops[0], line)));
                put16(out, (c->id - id::kExtendedBase) << 10);
            } else {
                put16(out, first(c->id, need_reg(ops[1], line), need_reg(ops[0], line)));
            }
            break;
        case Mode::R2:
            want(1);
            put16(out, first(id::kExtended, need_reg(ops[0], line), 0));
            put16(out, (c->id - id::kExtendedBase) << 10);
            break;
        case Mode::Jmp: {
            want(1);
            std::string_view r = ops[0];
            if (r.size() < 3 || r.front() != '[' || r.back() != ']') fail(line, "jmp [reg]");
            put16(out, first(id::kJmp, 0, need_reg(r.substr(1, r.size() - 2), line)));
            break;
        }
        case Mode::S5R2: {
            want(2);
            const std::int64_t v = number(ops[0]);
            range(v, -16, 15, "the 5-bit immediate");
            put16(out, first(c->id, need_reg(ops[1], line), static_cast<unsigned>(v & 31)));
            break;
        }
        case Mode::U5R2: {
            want(2);
            const std::int64_t v = number(ops[0]);
            range(v, 0, 31, "the 5-bit immediate");
            put16(out, first(c->id, need_reg(ops[1], line), static_cast<unsigned>(v & 31)));
            break;
        }
        case Mode::Trap: {
            want(1);
            const std::int64_t v = number(ops[0]);
            range(v, 0, 31, "the trap number");
            put16(out, first(c->id, 0, static_cast<unsigned>(v & 31)));
            break;
        }
        case Mode::LdSr:
        case Mode::StSr: {
            want(2);
            const std::string_view reg = c->mode == Mode::LdSr ? ops[0] : ops[1];
            const std::string_view sys = c->mode == Mode::LdSr ? ops[1] : ops[0];
            unsigned n = 99;
            const std::string name = text::lower(sys);
            for (unsigned k = 0; k < 32; ++k) {
                if (system_register_name(k) == name) n = k;
            }
            if (n == 99) {
                const std::int64_t v = number(sys);
                range(v, 0, 31, "the system register number");
                n = static_cast<unsigned>(v & 31);
            }
            put16(out, first(c->id, need_reg(reg, line), n));
            break;
        }
        case Mode::Branch: {
            want(1);
            const std::int64_t d = number(ops[0]) - static_cast<std::int64_t>(pc);
            if (!lenient && (d & 1)) fail(line, "a branch target is on an even address");
            range(d, -256, 254, "the branch displacement");
            put16(out, 0x8000u | c->cond << 9 | static_cast<std::uint32_t>(d & 0x1FF));
            break;
        }
        case Mode::Jump26: {
            want(1);
            const std::int64_t d = number(ops[0]) - static_cast<std::int64_t>(pc);
            if (!lenient && (d & 1)) fail(line, "a jump target is on an even address");
            range(d, -(1 << 25), (1 << 25) - 2, "the jump displacement");
            put16(out, first(c->id, 0, 0) | static_cast<std::uint32_t>((d >> 16) & 0x3FF));
            put16(out, static_cast<std::uint32_t>(d & 0xFFFF));
            break;
        }
        case Mode::Imm16S:
        case Mode::Imm16U: {
            want(3);
            const std::int64_t v = number(ops[0]);
            range(v, c->mode == Mode::Imm16S ? -32768 : 0, 65535, "the 16-bit immediate");
            put16(out, first(c->id, need_reg(ops[2], line), need_reg(ops[1], line)));
            put16(out, static_cast<std::uint32_t>(v & 0xFFFF));
            break;
        }
        case Mode::Load: {
            want(2);
            const auto [disp, base] = parse_memory(ops[0], line);
            const std::int64_t v = number(disp);
            range(v, -32768, 65535, "the displacement");
            put16(out, first(c->id, need_reg(ops[1], line), base));
            put16(out, static_cast<std::uint32_t>(v & 0xFFFF));
            break;
        }
        case Mode::Store: {
            want(2);
            const auto [disp, base] = parse_memory(ops[1], line);
            const std::int64_t v = number(disp);
            range(v, -32768, 65535, "the displacement");
            put16(out, first(c->id, need_reg(ops[0], line), base));
            put16(out, static_cast<std::uint32_t>(v & 0xFFFF));
            break;
        }
    }
    return out;
}

}  // namespace cb::decompile::v810
