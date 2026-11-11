#include "cb/decompile/sm83.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::decompile::sm83 {

namespace {

constexpr std::uint8_t kPrefixCB = 0xCB;
constexpr std::uint8_t kStopOpcode = 0x10;

bool is_illegal(unsigned op) { return std::find(kIllegalOpcodes.begin(), kIllegalOpcodes.end(), op) != kIllegalOpcodes.end(); }

// The unprefixed opcode as a pattern, or nullopt for an opcode that does not exist. The fields of an opcode are the CPU's
// own: x y z in bits 7-6, 5-3, 2-0, p = y >> 1 and q = y & 1 ("Decoding Z80 opcodes", z80.info; the SM83 reads the same
// bits to a different map).
std::optional<std::string> main_pattern(unsigned op) {
    const unsigned x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    const auto r = [](unsigned i) { return std::string(kRegisters[i]); };
    const auto rp = [](unsigned i) { return std::string(kRegisterPairs[i]); };
    const auto rp2 = [](unsigned i) { return std::string(kRegisterPairsAf[i]); };
    if (is_illegal(op)) return std::nullopt;
    if (x == 0) {
        switch (z) {
            case 0: {
                static constexpr std::array<std::string_view, 8> names = {"NOP", "LD (nn),SP", "STOP", "JR e", "JR NZ,e", "JR Z,e", "JR NC,e", "JR C,e"};
                return std::string(names[y]);
            }
            case 1: return q ? "ADD HL," + rp(p) : "LD " + rp(p) + ",nn";
            case 2: {
                static constexpr std::array<std::string_view, 4> store = {"LD (BC),A", "LD (DE),A", "LD (HL+),A", "LD (HL-),A"};
                static constexpr std::array<std::string_view, 4> load = {"LD A,(BC)", "LD A,(DE)", "LD A,(HL+)", "LD A,(HL-)"};
                return std::string(q ? load[p] : store[p]);
            }
            case 3: return (q ? "DEC " : "INC ") + rp(p);
            case 4: return "INC " + r(y);
            case 5: return "DEC " + r(y);
            case 6: return "LD " + r(y) + ",n";
            default: {
                static constexpr std::array<std::string_view, 8> names = {"RLCA", "RRCA", "RLA", "RRA", "DAA", "CPL", "SCF", "CCF"};
                return std::string(names[y]);
            }
        }
    }
    if (x == 1) return op == 0x76 ? "HALT" : "LD " + r(y) + "," + r(z);
    if (x == 2) return std::string(kAlu[y]) + r(z);
    switch (z) {   // x == 3
        case 0: {
            static constexpr std::array<std::string_view, 8> names = {"RET NZ", "RET Z", "RET NC", "RET C", "LDH (h),A", "ADD SP,s", "LDH A,(h)", "LD HL,SP+s"};
            return std::string(names[y]);
        }
        case 1: {
            static constexpr std::array<std::string_view, 4> names = {"RET", "RETI", "JP HL", "LD SP,HL"};
            return q ? std::string(names[p]) : "POP " + rp2(p);
        }
        case 2: {
            static constexpr std::array<std::string_view, 8> names = {"JP NZ,nn", "JP Z,nn", "JP NC,nn", "JP C,nn",
                                                                       "LD (C),A", "LD (nn),A", "LD A,(C)", "LD A,(nn)"};
            return std::string(names[y]);
        }
        case 3: {
            if (y == 0) return "JP nn";
            if (y == 6) return "DI";
            if (y == 7) return "EI";
            return std::nullopt;   // $CB is the prefix; the others do not exist
        }
        case 4:
            if (y < 4) return "CALL " + std::string(kConditions[y]) + ",nn";
            return std::nullopt;
        case 5:
            if (!q) return "PUSH " + rp2(p);
            return p == 0 ? std::optional<std::string>("CALL nn") : std::nullopt;
        case 6: return std::string(kAlu[y]) + "n";
        default: return std::format("RST ${:02X}", y * 8);
    }
}

std::string cb_pattern(unsigned op) {
    const unsigned x = op >> 6, y = (op >> 3) & 7, z = op & 7;
    if (x == 0) return std::string(kRotates[y]) + " " + std::string(kRegisters[z]);
    static constexpr std::array<std::string_view, 4> names = {"", "BIT", "RES", "SET"};
    return std::format("{} {},{}", names[x], y, kRegisters[z]);
}

// The placeholder words of a pattern (\b(nn|n|e|s|h)\b): which operand field the instruction carries.
enum class Field : std::uint8_t { N, NN, E, S, H };
constexpr std::uint8_t field_size(Field f) { return f == Field::NN ? 2 : 1; }

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

std::optional<Field> field_of_word(std::string_view w) {
    if (w == "nn") return Field::NN;
    if (w == "n") return Field::N;
    if (w == "e") return Field::E;
    if (w == "s") return Field::S;
    if (w == "h") return Field::H;
    return std::nullopt;
}

// The field of a pattern (no pattern has two).
std::optional<Field> field_in(std::string_view pattern) {
    for (std::size_t i = 0; i < pattern.size();) {
        if (!is_word_char(pattern[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < pattern.size() && is_word_char(pattern[end])) ++end;
        if (auto f = field_of_word(pattern.substr(i, end - i))) return f;
        i = end;
    }
    return std::nullopt;
}

struct Entry {
    std::string pattern;
    std::optional<Field> field;
    Flow flow = Flow::None;
};

// What the assembler needs per pattern: the opcode bytes before the operand.
struct AsmEntry {
    std::array<std::uint8_t, 2> code{};
    std::uint8_t code_size = 0;
};

struct Tables {
    std::array<std::array<std::optional<Entry>, 256>, 2> pages;
    std::unordered_map<std::string, AsmEntry> assembler;

    void set(Page page, unsigned op, std::optional<std::string> pattern) {
        if (!pattern) return;
        Entry e;
        e.field = field_in(*pattern);
        e.flow = flow_of(*pattern);
        e.pattern = std::move(*pattern);
        pages[static_cast<std::size_t>(page)][op] = std::move(e);
    }

    Tables() {
        for (unsigned op = 0; op < 256; ++op) {
            if (op != kPrefixCB) set(Page::Main, op, main_pattern(op));
            set(Page::CB, op, cb_pattern(op));
        }
        // the assembler's view: the first opcode that has a pattern is the one it writes (the main page first)
        for (unsigned op = 0; op < 256; ++op) {
            for (Page page : {Page::Main, Page::CB}) {
                const auto& e = pages[static_cast<std::size_t>(page)][op];
                if (!e) continue;
                AsmEntry a;
                if (page == Page::CB) {
                    a.code = {kPrefixCB, static_cast<std::uint8_t>(op)};
                    a.code_size = 2;
                } else {
                    a.code = {static_cast<std::uint8_t>(op), 0};
                    a.code_size = 1;
                }
                assembler.try_emplace(e->pattern, a);
            }
        }
    }
};

const Tables& tables() {
    static const Tables t;
    return t;
}

const Entry* entry_of(Page page, unsigned op) {
    const auto& e = tables().pages[static_cast<std::size_t>(page)][op];
    return e ? &*e : nullptr;
}

}  // namespace

std::string_view pattern_of(Page page, std::uint8_t opcode) {
    const Entry* e = entry_of(page, opcode);
    return e ? std::string_view(e->pattern) : std::string_view();
}

Flow flow_of(std::string_view pattern) {
    const std::string_view m = pattern.substr(0, pattern.find(' '));
    if (m == "JP" || m == "JR") {
        if (pattern == "JP HL") return Flow::Indirect;
        return pattern.find(',') != std::string_view::npos ? Flow::Branch : Flow::Jump;
    }
    if (m == "CALL") return pattern.find(',') != std::string_view::npos ? Flow::ConditionalCall : Flow::Call;
    if (m == "RST") return Flow::Call;
    if (m == "RET") return pattern.find(' ') != std::string_view::npos ? Flow::ConditionalReturn : Flow::Return;
    if (m == "RETI") return Flow::Return;
    if (m == "HALT" || m == "STOP") return Flow::Halt;
    return Flow::None;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc) {
    if (at >= code.size()) return std::nullopt;
    Page page = Page::Main;
    std::size_t i = at;
    if (code[i] == kPrefixCB) {
        page = Page::CB;
        if (++i >= code.size()) return std::nullopt;
    }
    const Entry* e = entry_of(page, code[i]);
    if (!e) return std::nullopt;
    const bool is_stop = code[i] == kStopOpcode && page == Page::Main;
    ++i;
    if (is_stop) ++i;   // STOP is two bytes ($10 $00)
    Insn insn;
    insn.pattern = e->pattern;
    insn.flow = e->flow;
    insn.pc = pc;
    if (e->field) {
        const std::size_t size = field_size(*e->field);
        if (i + size > code.size()) return std::nullopt;
        std::int32_t v = size == 1 ? code[i] : code[i] | (code[i + 1] << 8);
        if (*e->field == Field::E || *e->field == Field::S) {
            if (v >= 128) v -= 256;
        }
        if (*e->field == Field::H) v += 0xFF00;
        insn.value = v;
        i += size;
    }
    if (i > code.size()) return std::nullopt;
    CB_ASSERT(i - at <= insn.raw.size(), "an SM83 instruction is at most three bytes");
    insn.size = static_cast<std::uint8_t>(i - at);
    std::copy_n(code.begin() + static_cast<std::ptrdiff_t>(at), insn.size, insn.raw.begin());
    if (e->field == Field::E) {
        insn.target = static_cast<std::uint16_t>((pc + insn.size + *insn.value) & 0xFFFF);
        insn.value = *insn.target;
    } else if (insn.pattern.starts_with("RST")) {
        insn.target = static_cast<std::uint16_t>(text::parse_int<unsigned>(std::string_view(insn.pattern).substr(5), 16).value_or(0));
    } else if (e->flow == Flow::Jump || e->flow == Flow::Branch || e->flow == Flow::Call || e->flow == Flow::ConditionalCall) {
        insn.target = static_cast<std::uint16_t>(*insn.value);
    }
    return insn;
}

std::string text(const Insn& insn, const AddressLabels& labels) {
    if (insn.pattern == "STOP" && insn.size > 1 && insn.raw[1] != 0) {
        return std::format("DB $10,${:02X}", insn.raw[1]);   // STOP with a second byte assemblers do not write
    }
    auto named = [&](std::int32_t v) -> const std::string* {
        auto it = labels.find(static_cast<std::uint32_t>(v));
        return it != labels.end() && !it->second.empty() ? &it->second : nullptr;
    };
    std::string out;
    for (std::size_t i = 0; i < insn.pattern.size();) {
        if (!is_word_char(insn.pattern[i])) {
            out += insn.pattern[i++];
            continue;
        }
        std::size_t end = i;
        while (end < insn.pattern.size() && is_word_char(insn.pattern[end])) ++end;
        const std::string_view w = std::string_view(insn.pattern).substr(i, end - i);
        i = end;
        const std::optional<Field> f = field_of_word(w);
        if (!f) {
            out += w;
            continue;
        }
        CB_ASSERT(insn.value.has_value(), "an Insn has a value for the field of its pattern");
        const std::int32_t v = *insn.value;
        switch (*f) {
            case Field::E:
            case Field::NN:
            case Field::H:
                if (const std::string* label = named(v)) out += *label;
                else out += std::format("${:04X}", v);
                break;
            case Field::S: out += v >= 0 ? std::format("${:02X}", v) : std::format("-${:02X}", -v); break;
            case Field::N: out += std::format("${:02X}", v); break;
        }
    }
    return text::replace_all(out, "SP+-", "SP-");
}

namespace {

// The operand words an assembler reads as themselves (registers, pairs, conditions, the (HL+)-style indirect forms).
bool is_fixed_word(std::string_view u) {
    static const std::unordered_set<std::string_view> words = [] {
        std::unordered_set<std::string_view> w;
        for (auto n : kRegisters) w.insert(n);
        for (auto n : kRegisterPairs) w.insert(n);
        for (auto n : kRegisterPairsAf) w.insert(n);
        for (auto n : kConditions) w.insert(n);
        for (std::string_view n : {"(BC)", "(DE)", "(C)", "(HL+)", "(HL-)", "(HLI)", "(HLD)", "HL"}) w.insert(n);
        return w;
    }();
    return words.contains(u);
}

std::string squeezed(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (!text::is_space(c)) out += c;
    }
    return out;
}

}  // namespace

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels) {
    const std::string_view line = text::trim(line_in);
    const std::size_t space = line.find(' ');
    const std::string mnemonic = text::upper(line.substr(0, space));
    const std::string_view rest = space == std::string_view::npos ? std::string_view() : line.substr(space + 1);
    std::vector<std::string> ops;
    if (!text::trim(rest).empty()) ops = z80::split_operands(rest);

    std::vector<std::string> shape;   // each operand with its value hole as '?'
    std::vector<std::string> exprs;   // the hole's expression, in operand order
    for (const std::string& o : ops) {
        std::string u = text::upper(squeezed(o));
        u = text::replace_all(text::replace_all(u, "(HLI)", "(HL+)"), "(HLD)", "(HL-)");
        if (is_fixed_word(u)) {
            shape.push_back(u);
            continue;
        }
        const std::string tight = squeezed(o);
        if (mnemonic == "LD" && tight.size() > 3 && text::iequals(std::string_view(tight).substr(0, 2), "SP") &&
            (tight[2] == '+' || tight[2] == '-')) {
            shape.push_back("SP+?");   // LD HL,SP+e: the sign belongs to the expression
            exprs.push_back(tight.substr(2));
            continue;
        }
        if (mnemonic == "RST") {
            shape.push_back(std::format("${:02X}", z80::value(o, labels, pc)));
            continue;
        }
        if ((mnemonic == "BIT" || mnemonic == "SET" || mnemonic == "RES") && shape.empty()) {
            shape.push_back(std::to_string(z80::value(o, labels, pc)));
            continue;
        }
        if (o.starts_with("(") && o.ends_with(")") && z80::balanced(std::string_view(o).substr(1, o.size() - 2))) {
            shape.push_back("(?)");
            exprs.push_back(o.substr(1, o.size() - 2));
        } else {
            shape.push_back("?");
            exprs.push_back(o);
        }
    }

    // try each way the value operands can be read: n, nn, e, s, h for each, the first operand changing slowest
    static constexpr std::array<std::string_view, 5> kKinds = {"n", "nn", "e", "s", "h"};
    const std::size_t holes = exprs.size();
    std::vector<std::size_t> choice(holes, 0);
    const AsmEntry* found = nullptr;
    std::string pattern;
    for (;;) {
        std::size_t k = 0;
        pattern = mnemonic;
        for (std::size_t s = 0; s < shape.size(); ++s) {
            pattern += s == 0 ? " " : ",";
            const std::size_t q = shape[s].find('?');
            pattern += q == std::string::npos ? shape[s] : shape[s].substr(0, q) + std::string(kKinds[choice[k++]]) + shape[s].substr(q + 1);
        }
        auto it = tables().assembler.find(pattern);
        if (it != tables().assembler.end()) {
            found = &it->second;
            break;
        }
        // the next combination (an odometer, the last digit fastest)
        std::size_t digit = holes;
        while (digit > 0 && ++choice[digit - 1] == kKinds.size()) choice[--digit] = 0;
        if (digit == 0) break;
    }
    if (!found) throw ParseError("", 0, 0, "not an instruction: " + std::string(line));

    Bytes data(found->code.begin(), found->code.begin() + found->code_size);
    if (pattern == "STOP") data.push_back(0);
    const std::optional<Field> field = field_in(pattern);
    if (field && !exprs.empty()) {
        const std::size_t size = data.size() + field_size(*field);
        const std::int64_t v = z80::value(exprs[0], labels, pc);
        switch (*field) {
            case Field::E: {
                const std::int64_t d = v - static_cast<std::int64_t>(pc) - static_cast<std::int64_t>(size);
                if (d < -128 || d > 127) throw ParseError("", 0, 0, "relative jump out of range: " + std::string(line));
                data.push_back(static_cast<std::uint8_t>(d & 0xFF));
                break;
            }
            case Field::H:
                if (v < 0xFF00 || v > 0xFFFF) throw ParseError("", 0, 0, "LDH address outside $FF00-$FFFF: " + std::string(line));
                data.push_back(static_cast<std::uint8_t>(v & 0xFF));
                break;
            case Field::N:
            case Field::S: data.push_back(static_cast<std::uint8_t>(v & 0xFF)); break;
            case Field::NN:
                data.push_back(static_cast<std::uint8_t>(v & 0xFF));
                data.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
                break;
        }
    }
    return data;
}

}  // namespace cb::decompile::sm83
