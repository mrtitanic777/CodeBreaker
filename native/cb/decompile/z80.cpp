#include "cb/decompile/z80.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/intmath.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::z80 {

namespace {

// ---- the table: (page, opcode) -> pattern ------------------------------------------------------------

constexpr std::uint8_t kPrefixCB = 0xCB, kPrefixDD = 0xDD, kPrefixED = 0xED, kPrefixFD = 0xFD;

bool is_prefix_byte(unsigned op) { return op == kPrefixCB || op == kPrefixDD || op == kPrefixED || op == kPrefixFD; }

std::optional<std::string> main_pattern(unsigned op) {
    const unsigned x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    auto rp = kRegisterPairs[p];
    auto rp2 = kRegisterPairsAf[p];
    auto r = kRegisters[y];
    if (x == 0) {
        switch (z) {
            case 0: {
                static constexpr std::array<std::string_view, 8> k = {"NOP",    "EX AF,AF'", "DJNZ e",  "JR e",
                                                                      "JR NZ,e", "JR Z,e",  "JR NC,e", "JR C,e"};
                return std::string(k[y]);
            }
            case 1: return q ? "ADD HL," + std::string(rp) : "LD " + std::string(rp) + ",nn";
            case 2: {
                static constexpr std::array<std::array<std::string_view, 4>, 2> k = {
                    {{"LD (BC),A", "LD (DE),A", "LD (nn),HL", "LD (nn),A"}, {"LD A,(BC)", "LD A,(DE)", "LD HL,(nn)", "LD A,(nn)"}}};
                return std::string(k[q][p]);
            }
            case 3: return (q ? "DEC " : "INC ") + std::string(rp);
            case 4: return "INC " + std::string(r);
            case 5: return "DEC " + std::string(r);
            case 6: return "LD " + std::string(r) + ",n";
            default: {
                static constexpr std::array<std::string_view, 8> k = {"RLCA", "RRCA", "RLA", "RRA", "DAA", "CPL", "SCF", "CCF"};
                return std::string(k[y]);
            }
        }
    }
    if (x == 1) return op == 0x76 ? std::string("HALT") : "LD " + std::string(r) + "," + std::string(kRegisters[z]);
    if (x == 2) return std::string(kAlu[y]) + std::string(kRegisters[z]);
    switch (z) {
        case 0: return "RET " + std::string(kConditions[y]);
        case 1: {
            if (!q) return "POP " + std::string(rp2);
            static constexpr std::array<std::string_view, 4> k = {"RET", "EXX", "JP (HL)", "LD SP,HL"};
            return std::string(k[p]);
        }
        case 2: return "JP " + std::string(kConditions[y]) + ",nn";
        case 3: {
            static constexpr std::array<std::string_view, 8> k = {"JP nn", "", "OUT (n),A", "IN A,(n)", "EX (SP),HL", "EX DE,HL", "DI", "EI"};
            if (k[y].empty()) return std::nullopt;   // CB
            return std::string(k[y]);
        }
        case 4: return "CALL " + std::string(kConditions[y]) + ",nn";
        case 5:
            if (!q) return "PUSH " + std::string(rp2);
            if (p == 0) return std::string("CALL nn");
            return std::nullopt;                      // DD, ED, FD
        case 6: return std::string(kAlu[y]) + "n";
        default: return std::format("RST ${:02X}", y * 8);
    }
}

std::string cb_pattern(unsigned op, std::string_view target = "(HL)") {
    const unsigned x = op >> 6, y = (op >> 3) & 7, z = op & 7;
    const std::string_view reg = target == "(HL)" ? kRegisters[z] : target;
    if (x == 0) return std::string(kRotates[y]) + " " + std::string(reg);
    static constexpr std::array<std::string_view, 4> k = {"", "BIT", "RES", "SET"};
    return std::format("{} {},{}", k[x], y, reg);
}

std::optional<std::string> ed_pattern(unsigned op) {
    const unsigned x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    auto rp = std::string(kRegisterPairs[p]);
    if (x == 1) {
        switch (z) {
            case 0: return y == 6 ? std::string("IN (C)") : "IN " + std::string(kRegisters[y]) + ",(C)";
            case 1: return y == 6 ? std::string("OUT (C),0") : "OUT (C)," + std::string(kRegisters[y]);
            case 2: return (q ? "ADC HL," : "SBC HL,") + rp;
            case 3: return q ? "LD " + rp + ",(nn)" : "LD (nn)," + rp;
            case 4: return std::string("NEG");
            case 5: return std::string(y == 1 ? "RETI" : "RETN");
            case 6: return "IM " + std::string(kInterruptModes[y]);
            default: {
                static constexpr std::array<std::string_view, 8> k = {"LD I,A", "LD R,A", "LD A,I", "LD A,R", "RRD", "RLD", "NOP", "NOP"};
                return std::string(k[y]);
            }
        }
    }
    if (x == 2 && z <= 3 && y >= 4) {
        // rows 4..7 (y) are the I, D, IR and DR forms of the four block operations (z)
        static constexpr std::array<std::array<std::string_view, 4>, 4> rows = {{{"LDI", "CPI", "INI", "OUTI"},
                                                                                 {"LDD", "CPD", "IND", "OUTD"},
                                                                                 {"LDIR", "CPIR", "INIR", "OTIR"},
                                                                                 {"LDDR", "CPDR", "INDR", "OTDR"}}};
        return std::string(rows[y - 4][z]);
    }
    return std::nullopt;   // an ED opcode that does nothing (a two-byte NOP)
}

std::vector<std::string_view> split_pattern(std::string_view pattern) {
    // the pieces of re.split(r"([ ,()+])", pattern): the words and each separator on its own
    std::vector<std::string_view> out;
    std::size_t start = 0;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        const char c = pattern[i];
        if (c == ' ' || c == ',' || c == '(' || c == ')' || c == '+') {
            out.push_back(pattern.substr(start, i - start));
            out.push_back(pattern.substr(i, 1));
            start = i + 1;
        }
    }
    out.push_back(pattern.substr(start));
    return out;
}

// An unprefixed pattern as the DD/FD page has it: HL -> IX, H/L -> IXH/IXL, (HL) -> (IX+d) (and then H/L stay H/L), or
// nullopt when the prefix changes nothing (the opcode runs as unprefixed).
std::optional<std::string> index_pattern(const std::optional<std::string>& pattern, std::string_view ix) {
    if (!pattern || *pattern == "EX DE,HL" || *pattern == "EXX" || pattern->starts_with("RST")) return std::nullopt;
    if (*pattern == "JP (HL)") return "JP (" + std::string(ix) + ")";   // a jump to HL's value, not through memory
    if (pattern->find("(HL)") != std::string::npos) return text::replace_all(*pattern, "(HL)", "(" + std::string(ix) + "+d)");
    const std::vector<std::string_view> words = split_pattern(*pattern);
    auto has = [&](std::string_view w) { return std::find(words.begin(), words.end(), w) != words.end(); };
    std::string out;
    if (has("HL")) {
        for (std::string_view w : words) out += w == "HL" ? ix : w;
        return out;
    }
    if (has("H") || has("L")) {
        for (std::string_view w : words) {
            if (w == "H" || w == "L") out += ix;
            out += w;
        }
        return out;
    }
    return std::nullopt;
}

// The placeholder fields of a pattern, in order: n (1 byte), nn (2), d (1, signed), e (1, signed, a relative jump).
enum class Field : std::uint8_t { N, NN, D, E };
constexpr std::uint8_t field_size(Field f) { return f == Field::NN ? 2 : 1; }

struct Words {
    std::array<Field, 2> fields{};
    std::uint8_t count = 0;
};

// The words of the pattern that are placeholders (\b(nn|n|d|e)\b).
Words fields_of(std::string_view pattern) {
    Words out;
    for (std::size_t i = 0; i < pattern.size();) {
        auto word_char = [](char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; };
        if (!word_char(pattern[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < pattern.size() && word_char(pattern[end])) ++end;
        const std::string_view w = pattern.substr(i, end - i);
        std::optional<Field> f;
        if (w == "nn") f = Field::NN;
        else if (w == "n") f = Field::N;
        else if (w == "d") f = Field::D;
        else if (w == "e") f = Field::E;
        if (f) {
            CB_ASSERT(out.count < out.fields.size(), "a Z80 pattern has at most two operands");
            out.fields[out.count++] = *f;
        }
        i = end;
    }
    return out;
}

struct Entry {
    std::string pattern;
    Words words;
    Flow flow = Flow::None;
};

// What the assembler needs per pattern: the bytes before the operands.
struct AsmEntry {
    std::array<std::uint8_t, 2> code{};
    std::uint8_t code_size = 0;
    std::uint8_t cb_opcode = 0;          // DDCB/FDCB: the opcode that follows the displacement
    bool displacement_first = false;
};

struct Tables {
    std::array<std::array<std::optional<Entry>, 256>, 7> pages;
    std::unordered_map<std::string, AsmEntry> assembler;

    void set(Page page, unsigned op, std::optional<std::string> pattern) {
        if (!pattern) return;
        Entry e;
        e.words = fields_of(*pattern);
        e.flow = flow_of(*pattern);
        e.pattern = std::move(*pattern);
        pages[static_cast<std::size_t>(page)][op] = std::move(e);
    }

    Tables() {
        for (unsigned op = 0; op < 256; ++op) {
            set(Page::CB, op, cb_pattern(op));
            set(Page::ED, op, ed_pattern(op));
            for (Page prefix : {Page::DD, Page::FD}) {
                const std::string_view ix = prefix == Page::DD ? "IX" : "IY";
                if (!is_prefix_byte(op)) set(prefix, op, index_pattern(main_pattern(op), ix));
                // DDCB d op: the operation on (IX+d); the undocumented forms also copy the result to a register
                std::string base = cb_pattern(op, "(" + std::string(ix) + "+d)");
                if ((op & 7) != 6 && (op >> 6) != 1) base += "," + std::string(kRegisters[op & 7]);
                set(prefix == Page::DD ? Page::DDCB : Page::FDCB, op, std::move(base));
            }
            if (!is_prefix_byte(op)) set(Page::Main, op, main_pattern(op));
        }
        // the assembler's view: the first opcode (in this order) that has a pattern is the one it writes
        for (unsigned op = 0; op < 256; ++op) {
            for (Page page : {Page::CB, Page::ED, Page::DD, Page::DDCB, Page::FD, Page::FDCB, Page::Main}) {
                const auto& e = pages[static_cast<std::size_t>(page)][op];
                if (!e) continue;
                AsmEntry a;
                switch (page) {
                    case Page::DDCB:
                    case Page::FDCB:
                        a.code = {page == Page::DDCB ? kPrefixDD : kPrefixFD, kPrefixCB};
                        a.code_size = 2;
                        a.cb_opcode = static_cast<std::uint8_t>(op);
                        a.displacement_first = true;
                        break;
                    case Page::Main:
                        a.code = {static_cast<std::uint8_t>(op), 0};
                        a.code_size = 1;
                        break;
                    default: {
                        const std::uint8_t prefix = page == Page::CB ? kPrefixCB : page == Page::ED ? kPrefixED
                                                    : page == Page::DD ? kPrefixDD : kPrefixFD;
                        a.code = {prefix, static_cast<std::uint8_t>(op)};
                        a.code_size = 2;
                    }
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

// A DD or FD the next opcode does not use: the CPU ignores it (a one-byte instruction that does nothing). It is written as
// its byte (an assembler would write NOP, 00, for "NOP").
Insn lone_prefix(ByteView code, std::size_t at, std::uint32_t pc) {
    Insn i;
    i.pattern = std::format(".db ${:02X}", code[at]);
    i.size = 1;
    i.pc = pc;
    i.raw[0] = code[at];
    return i;
}

Insn make_insn(std::string pattern, ByteView code, std::size_t at, std::size_t end, std::uint32_t pc) {
    Insn i;
    i.pattern = std::move(pattern);
    i.size = static_cast<std::uint8_t>(end - at);
    i.pc = pc;
    std::copy_n(code.begin() + static_cast<std::ptrdiff_t>(at), i.size, i.raw.begin());
    return i;
}

}  // namespace

std::string_view pattern_of(Page page, std::uint8_t opcode) {
    const Entry* e = entry_of(page, opcode);
    return e ? std::string_view(e->pattern) : std::string_view();
}

std::string_view flow_name(Flow flow) {
    switch (flow) {
        case Flow::None: return "";
        case Flow::Jump: return "jump";
        case Flow::Branch: return "branch";
        case Flow::Call: return "call";
        case Flow::ConditionalCall: return "ccall";
        case Flow::Return: return "ret";
        case Flow::ConditionalReturn: return "cret";
        case Flow::Indirect: return "indirect";
        case Flow::Halt: return "halt";
    }
    return "";
}

Flow flow_of(std::string_view pattern) {
    const std::string_view m = pattern.substr(0, pattern.find(' '));
    if (m == "JP" || m == "JR") {
        if (pattern.find('(') != std::string_view::npos) return Flow::Indirect;
        return pattern.find(',') != std::string_view::npos ? Flow::Branch : Flow::Jump;
    }
    if (m == "DJNZ") return Flow::Branch;
    if (m == "CALL") return pattern.find(',') != std::string_view::npos ? Flow::ConditionalCall : Flow::Call;
    if (m == "RST") return Flow::Call;
    if (m == "RET") return pattern.find(' ') != std::string_view::npos ? Flow::ConditionalReturn : Flow::Return;
    if (m == "RETI" || m == "RETN") return Flow::Return;
    if (m == "HALT") return Flow::Halt;
    return Flow::None;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc) {
    // prefixes: one DD/FD, which a second one makes a one-byte instruction of its own
    Page page = Page::Main;
    std::size_t i = at;
    while (i < code.size() && (code[i] == kPrefixDD || code[i] == kPrefixFD)) {
        if (page == Page::DD || page == Page::FD) return lone_prefix(code, at, pc);   // the first one does nothing
        page = code[i] == kPrefixDD ? Page::DD : Page::FD;
        ++i;
    }
    const bool indexed = page == Page::DD || page == Page::FD;
    if (i >= code.size()) return std::nullopt;
    unsigned op = code[i];
    if ((op == kPrefixCB || op == kPrefixED) && !indexed) {
        page = op == kPrefixCB ? Page::CB : Page::ED;
        if (++i >= code.size()) return std::nullopt;
        op = code[i];
    } else if (op == kPrefixCB && indexed) {
        // DDCB d op: the displacement comes before the opcode
        if (i + 2 >= code.size()) return std::nullopt;
        const unsigned d = code[i + 1];
        op = code[i + 2];
        const Entry* e = entry_of(page == Page::DD ? Page::DDCB : Page::FDCB, op);
        CB_ASSERT(e != nullptr, "every DDCB/FDCB opcode has a pattern");
        Insn insn = make_insn(e->pattern, code, at, i + 3, pc);
        insn.values[0] = d >= 128 ? static_cast<std::int32_t>(d) - 256 : static_cast<std::int32_t>(d);
        insn.value_count = 1;
        return insn;
    } else if (op == kPrefixED && indexed) {
        return lone_prefix(code, at, pc);   // DD ED: the DD is ignored
    }
    const Entry* e = entry_of(page, op);
    if (!e && indexed) return lone_prefix(code, at, pc);   // DD before an unindexed op
    if (!e) {
        if (page != Page::ED) return std::nullopt;
        return make_insn(std::format(".db $ED,${:02X}", op), code, at, i + 1, pc);   // an ED opcode that does nothing
    }
    ++i;
    Insn insn;
    for (std::uint8_t k = 0; k < e->words.count; ++k) {
        const Field f = e->words.fields[k];
        const std::size_t size = field_size(f);
        if (i + size > code.size()) return std::nullopt;
        std::int32_t v = size == 1 ? code[i] : code[i] | (code[i + 1] << 8);
        if ((f == Field::D || f == Field::E) && v >= 128) v -= 256;
        insn.values[k] = v;
        i += size;
    }
    insn.value_count = e->words.count;
    const std::size_t size = i - at;
    insn.pattern = e->pattern;
    insn.size = static_cast<std::uint8_t>(size);
    insn.pc = pc;
    insn.flow = e->flow;
    std::copy_n(code.begin() + static_cast<std::ptrdiff_t>(at), size, insn.raw.begin());
    const bool relative = e->words.count > 0 && e->words.fields[e->words.count - 1] == Field::E;
    if (relative) {
        insn.target = static_cast<std::uint16_t>((pc + size + insn.values[insn.value_count - 1]) & 0xFFFF);
        insn.values[insn.value_count - 1] = *insn.target;
    } else if (e->flow == Flow::Call && e->pattern.starts_with("RST")) {
        insn.target = static_cast<std::uint16_t>(text::parse_int<unsigned>(std::string_view(e->pattern).substr(5), 16).value_or(0));
    } else if (e->flow == Flow::Jump || e->flow == Flow::Branch || e->flow == Flow::Call || e->flow == Flow::ConditionalCall) {
        insn.target = static_cast<std::uint16_t>(insn.values[insn.value_count - 1]);
    }
    return insn;
}

std::string plain_text(const Insn& insn, const AddressLabels& labels) {
    std::string out;
    std::size_t k = 0;
    const std::string& pattern = insn.pattern;
    auto word_char = [](char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; };
    for (std::size_t i = 0; i < pattern.size();) {
        if (!word_char(pattern[i])) {
            out += pattern[i++];
            continue;
        }
        std::size_t end = i;
        while (end < pattern.size() && word_char(pattern[end])) ++end;
        const std::string_view w = std::string_view(pattern).substr(i, end - i);
        i = end;
        if (w != "nn" && w != "n" && w != "d" && w != "e") {
            out += w;
            continue;
        }
        CB_ASSERT(k < insn.value_count, "an Insn has a value for each placeholder of its pattern");
        const std::int32_t v = insn.values[k++];
        if (w == "e" || (w == "nn" && labels.contains(static_cast<std::uint32_t>(v)))) {
            auto named = labels.find(static_cast<std::uint32_t>(v));
            out += named != labels.end() ? named->second : std::format("${:04X}", v);
        } else if (w == "nn") {
            out += std::format("${:04X}", v);
        } else if (w == "d") {
            out += v >= 0 ? std::format("+${:02X}", v) : std::format("-${:02X}", -v);
        } else {
            out += std::format("${:02X}", v);
        }
    }
    return text::replace_all(text::replace_all(out, "++", "+"), "+-", "-");
}

bool is_canonical(const Insn& insn) {
    auto it = tables().assembler.find(insn.pattern);
    if (it == tables().assembler.end()) return true;
    const AsmEntry& a = it->second;
    const ByteView b = insn.bytes();
    if (a.displacement_first) {   // DDCB d op
        return b.size() >= 4 && b[0] == a.code[0] && b[1] == a.code[1] && b[3] == a.cb_opcode;
    }
    return b.size() >= a.code_size && std::equal(a.code.begin(), a.code.begin() + a.code_size, b.begin());
}

std::string text(const Insn& insn, const AddressLabels& labels) {
    if (!insn.pattern.starts_with(".") && !is_canonical(insn)) {
        std::string bytes;
        for (std::uint8_t b : insn.bytes()) bytes += (bytes.empty() ? "" : ",") + std::format("${:02X}", b);
        return ".db " + bytes + " ; " + plain_text(insn, labels);
    }
    return plain_text(insn, labels);
}

// ---- the assembler -----------------------------------------------------------------------------------

namespace {

bool is_register_word(std::string_view u) {
    static const std::unordered_set<std::string_view> words = [] {
        std::unordered_set<std::string_view> w;
        for (auto n : kRegisters) w.insert(n);
        for (auto n : kRegisterPairs) w.insert(n);
        for (auto n : kRegisterPairsAf) w.insert(n);
        for (auto n : kConditions) w.insert(n);
        for (std::string_view n : {"IX", "IY", "IXH", "IXL", "IYH", "IYL", "I", "R", "AF'", "(C)", "(SP)", "(BC)", "(DE)", "(IX)", "(IY)"}) w.insert(n);
        return w;
    }();
    return words.contains(u);
}

[[noreturn]] void bad_expression(std::string_view expr, const char* why) {
    throw ParseError("", 0, 0, std::format("{} in \"{}\"", why, expr));
}

bool is_name_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || c == '.' || c == '@'; }
// (the bytes of a multi-byte UTF-8 character count as letters inside a name: `Xé`)
bool is_name_char(char c) { return is_name_start(c) || (c >= '0' && c <= '9') || static_cast<unsigned char>(c) >= 0x80; }
bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_dec_digit(char c) { return c >= '0' && c <= '9'; }

struct Token {
    bool is_number = false;
    std::int64_t number = 0;
    std::string_view op;      // for an operator: "-" "+" "*" "/" "(" ")" "&" "|" "^" "~" "<" ">" "<<" ">>"
};

// Tokens of an expression; a label is resolved as it is read (so an unknown one is reported before a syntax error).
Result<std::vector<Token>> tokenize(std::string_view expr, const NameLabels& labels, std::uint32_t pc) {
    std::vector<Token> out;
    auto number = [&](std::string_view digits, int base) -> bool {
        auto v = text::parse_int<std::int64_t>(digits, base);
        if (!v) return false;
        out.push_back({true, *v, {}});
        return true;
    };
    for (std::size_t i = 0; i < expr.size();) {
        const char c = expr[i];
        if (text::is_space(c)) {
            ++i;
            continue;
        }
        const std::string_view rest = expr.substr(i);
        std::size_t len = 0;
        if (c == '$') {
            while (1 + len < rest.size() && is_hex_digit(rest[1 + len])) ++len;
            if (len == 0) {   // `$` alone: here
                out.push_back({true, static_cast<std::int64_t>(pc), {}});
                ++i;
                continue;
            }
            if (!number(rest.substr(1, len), 16)) return ParseError("", 0, 0, "number too large in \"" + std::string(expr) + "\"");
            i += 1 + len;
            continue;
        }
        if (c == '0' && rest.size() > 2 && rest[1] == 'x' && is_hex_digit(rest[2])) {
            len = 2;
            while (len < rest.size() && is_hex_digit(rest[len])) ++len;
            if (!number(rest.substr(2, len - 2), 16)) return ParseError("", 0, 0, "number too large in \"" + std::string(expr) + "\"");
            i += len;
            continue;
        }
        if (is_dec_digit(c)) {
            // 1Fh: a digit, hex digits, h, and no word character after
            std::size_t h = 1;
            while (h < rest.size() && is_hex_digit(rest[h])) ++h;
            if (h < rest.size() && (rest[h] == 'h' || rest[h] == 'H') && (h + 1 == rest.size() || !is_name_char(rest[h + 1]))) {
                if (!number(rest.substr(0, h), 16)) return ParseError("", 0, 0, "number too large in \"" + std::string(expr) + "\"");
                i += h + 1;
                continue;
            }
            len = 1;
            while (len < rest.size() && is_dec_digit(rest[len])) ++len;
            if (!number(rest.substr(0, len), 10)) return ParseError("", 0, 0, "number too large in \"" + std::string(expr) + "\"");
            i += len;
            continue;
        }
        if (c == '%' && rest.size() > 1 && (rest[1] == '0' || rest[1] == '1')) {
            len = 1;
            while (len < rest.size() && (rest[len] == '0' || rest[len] == '1')) ++len;
            if (!number(rest.substr(1, len - 1), 2)) return ParseError("", 0, 0, "number too large in \"" + std::string(expr) + "\"");
            i += len;
            continue;
        }
        if (c == '\'' && rest.size() >= 3) {   // 'c': the character's code (a UTF-8 character is read whole)
            const auto lead = static_cast<unsigned char>(rest[1]);
            const std::size_t n = lead < 0x80 ? 1 : lead >= 0xF0 ? 4 : lead >= 0xE0 ? 3 : lead >= 0xC0 ? 2 : 0;
            if (n != 0 && rest.size() > 1 + n && rest[1 + n] == '\'') {
                std::int64_t code = n == 1 ? lead : lead & (0xFF >> (n + 1));
                for (std::size_t k = 2; k < 1 + n; ++k) code = (code << 6) | (static_cast<unsigned char>(rest[k]) & 0x3F);
                out.push_back({true, code, {}});
                i += 2 + n;
                continue;
            }
        }
        if (is_name_start(c)) {
            len = 1;
            while (len < rest.size() && is_name_char(rest[len])) ++len;
            auto it = labels.find(rest.substr(0, len));
            if (it == labels.end()) return UnknownLabel(std::string(rest.substr(0, len)));
            out.push_back({true, it->second, {}});
            i += len;
            continue;
        }
        if (rest.starts_with("<<") || rest.starts_with(">>")) {
            out.push_back({false, 0, rest.substr(0, 2)});
            i += 2;
            continue;
        }
        if (std::string_view("-+*/()&|^~<>").find(c) != std::string_view::npos) {
            out.push_back({false, 0, rest.substr(0, 1)});
            ++i;
            continue;
        }
        return ParseError("", 0, 0, std::format("unexpected '{}' in \"{}\"", c, expr));
    }
    return out;
}

// A recursive-descent evaluator over the tokens, with Python's integer operators and their precedence:
// | then ^ then & then << >> then + - then * / (floor division) then unary + - ~.
class Evaluator {
public:
    Evaluator(std::span<const Token> tokens, std::string_view expr) : tokens_(tokens), expr_(expr) {}

    std::int64_t parse() {
        if (tokens_.empty()) bad_expression(expr_, "empty expression");
        std::int64_t v = bit_or();
        if (at_ != tokens_.size()) bad_expression(expr_, "unexpected token");
        return v;
    }

private:
    bool accept(std::string_view op) {
        if (at_ < tokens_.size() && !tokens_[at_].is_number && tokens_[at_].op == op) {
            ++at_;
            return true;
        }
        return false;
    }
    std::int64_t checked(std::optional<std::int64_t> v) const {
        if (!v) bad_expression(expr_, "arithmetic overflow");
        return *v;
    }

    std::int64_t bit_or() {
        std::int64_t v = bit_xor();
        while (accept("|")) v |= bit_xor();
        return v;
    }
    std::int64_t bit_xor() {
        std::int64_t v = bit_and();
        while (accept("^")) v ^= bit_and();
        return v;
    }
    std::int64_t bit_and() {
        std::int64_t v = shift();
        while (accept("&")) v &= shift();
        return v;
    }
    std::int64_t shift() {
        std::int64_t v = additive();
        for (;;) {
            const bool left = accept("<<");
            if (!left && !accept(">>")) return v;
            const std::int64_t n = additive();
            if (n < 0) bad_expression(expr_, "negative shift count");
            if (left) {
                if (v != 0 && (n >= 63 || ((v << n) >> n) != v)) bad_expression(expr_, "arithmetic overflow");
                v = n >= 64 ? 0 : v << n;
            } else {
                v = n >= 64 ? (v < 0 ? -1 : 0) : v >> n;
            }
        }
    }
    std::int64_t additive() {
        std::int64_t v = multiplicative();
        for (;;) {
            if (accept("+")) v = checked(checked_add(v, multiplicative()));
            else if (accept("-")) {
                const std::int64_t r = multiplicative();
                if (r == std::numeric_limits<std::int64_t>::min()) bad_expression(expr_, "arithmetic overflow");
                v = checked(checked_add(v, -r));
            } else return v;
        }
    }
    std::int64_t multiplicative() {
        std::int64_t v = unary();
        for (;;) {
            if (accept("*")) v = checked(checked_mul(v, unary()));
            else if (accept("/")) {
                const std::int64_t d = unary();
                if (d == 0) bad_expression(expr_, "division by zero");
                if (v == std::numeric_limits<std::int64_t>::min() && d == -1) bad_expression(expr_, "arithmetic overflow");
                v = floor_div(v, d);
            } else return v;
        }
    }
    std::int64_t unary() {
        if (accept("-")) {
            const std::int64_t v = unary();
            if (v == std::numeric_limits<std::int64_t>::min()) bad_expression(expr_, "arithmetic overflow");
            return -v;
        }
        if (accept("+")) return unary();
        if (accept("~")) return ~unary();
        return primary();
    }
    std::int64_t primary() {
        if (at_ >= tokens_.size()) bad_expression(expr_, "incomplete expression");
        const Token& t = tokens_[at_];
        if (t.is_number) {
            ++at_;
            return t.number;
        }
        if (accept("(")) {
            const std::int64_t v = bit_or();
            if (!accept(")")) bad_expression(expr_, "missing )");
            return v;
        }
        bad_expression(expr_, "unexpected operator");
    }

    std::span<const Token> tokens_;
    std::string_view expr_;
    std::size_t at_ = 0;
};

}  // namespace

Result<std::int64_t> try_value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) {
    expr = text::trim(expr);
    CB_TRY_ASSIGN(std::vector<Token> tokens, tokenize(expr, labels, pc));
    try {
        std::span<const Token> all(tokens);
        // `<x` / `>x`: the low / high byte of what follows (WLA-DX)
        if (!tokens.empty() && !tokens[0].is_number && (tokens[0].op == "<" || tokens[0].op == ">")) {
            const bool low = tokens[0].op == "<";
            const std::int64_t v = Evaluator(all.subspan(1), expr).parse();
            return low ? (v & 0xFF) : ((v >> 8) & 0xFF);
        }
        return Evaluator(all, expr).parse();
    } catch (const ParseError& e) {   // (the evaluator reports malformed expressions by throwing; this is where they become a Result)
        return e;
    }
}

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) {
    return try_value(expr, labels, pc).value();
}

std::vector<std::string> split_operands(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (char c : s) {
        if (c == '(') ++depth;
        else if (c == ')') --depth;
        if (c == ',' && depth == 0) {
            out.emplace_back(text::trim(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!text::trim(cur).empty()) out.emplace_back(text::trim(cur));
    return out;
}

bool balanced(std::string_view s) {
    int depth = 0;
    for (char c : s) {
        depth += c == '(';
        depth -= c == ')';
        if (depth < 0) return false;
    }
    return depth == 0;
}

namespace {

std::string without_space_upper(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (!text::is_space(c)) out += c;
    }
    return text::upper(out);
}

}  // namespace

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels) {
    const std::string_view line = text::trim(line_in);
    const std::size_t space = line.find(' ');
    const std::string mnemonic = text::upper(line.substr(0, space));
    const std::string_view rest = space == std::string_view::npos ? std::string_view() : line.substr(space + 1);
    std::vector<std::string> ops;
    if (!text::trim(rest).empty()) ops = split_operands(rest);

    if (mnemonic == "OUT" && ops.size() == 2 && without_space_upper(ops[0]) == "(C)" && without_space_upper(ops[1]) == "0") {
        return {0xED, 0x71};   // OUT (C),0 (undocumented): 0 is part of the instruction
    }
    std::vector<std::string> shape;
    std::vector<std::string> exprs;   // the operand expressions, in operand order (a `d` and the `?`s alike)
    for (const std::string& o : ops) {
        const std::string u = without_space_upper(o);
        if (is_register_word(u)) {
            shape.push_back(u);
            continue;
        }
        // (IX+d) / (IY-d): a displacement from an index register
        std::string squeezed;
        for (char c : o) {
            if (!text::is_space(c)) squeezed += c;
        }
        if (squeezed.size() >= 5 && squeezed.front() == '(' && squeezed.back() == ')' &&
            (text::iequals(std::string_view(squeezed).substr(1, 2), "IX") || text::iequals(std::string_view(squeezed).substr(1, 2), "IY")) &&
            (squeezed[3] == '+' || squeezed[3] == '-')) {
            shape.push_back("(" + text::upper(std::string_view(squeezed).substr(1, 2)) + "+d)");
            exprs.push_back(squeezed.substr(3, squeezed.size() - 4));
            continue;
        }
        if (mnemonic == "RST") {
            shape.push_back(std::format("${:02X}", value(o, labels, pc)));
            continue;
        }
        if (mnemonic == "IM" || ((mnemonic == "BIT" || mnemonic == "SET" || mnemonic == "RES") && shape.empty())) {
            shape.push_back(std::to_string(value(o, labels, pc)));
            continue;
        }
        if (o.starts_with("(") && o.ends_with(")") && balanced(std::string_view(o).substr(1, o.size() - 2))) {
            shape.push_back("(?)");
            exprs.push_back(o.substr(1, o.size() - 2));
        } else {
            shape.push_back("?");
            exprs.push_back(o);
        }
    }

    // try each way the value operands can be read: n, nn, e, d for each, the first operand changing slowest
    static constexpr std::array<std::string_view, 4> kKinds = {"n", "nn", "e", "d"};
    std::size_t holes = 0;
    for (const std::string& s : shape) holes += s.find('?') != std::string::npos;
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

    const Words words = fields_of(pattern);
    Bytes data(found->code.begin(), found->code.begin() + found->code_size);
    if (found->displacement_first) {
        if (exprs.empty()) throw ParseError("", 0, 0, "not an instruction: " + std::string(line));
        const std::int64_t d = value(exprs[0], labels, pc);
        data.push_back(static_cast<std::uint8_t>(d & 0xFF));
        data.push_back(found->cb_opcode);
        return data;
    }
    std::size_t size = found->code_size;
    for (std::uint8_t k = 0; k < words.count; ++k) size += field_size(words.fields[k]);
    for (std::size_t k = 0; k < words.count && k < exprs.size(); ++k) {
        std::int64_t v = value(exprs[k], labels, pc);
        switch (words.fields[k]) {
            case Field::E:
                v -= static_cast<std::int64_t>(pc) + static_cast<std::int64_t>(size);
                if (v < -128 || v > 127) throw ParseError("", 0, 0, "relative jump out of range: " + std::string(line));
                data.push_back(static_cast<std::uint8_t>(v & 0xFF));
                break;
            case Field::N:
            case Field::D: data.push_back(static_cast<std::uint8_t>(v & 0xFF)); break;
            case Field::NN:
                data.push_back(static_cast<std::uint8_t>(v & 0xFF));
                data.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
                break;
        }
    }
    return data;
}

}  // namespace cb::decompile::z80
