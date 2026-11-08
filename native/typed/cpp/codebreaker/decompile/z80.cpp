// codebreaker/decompile/z80.cpp -- C++ port of codebreaker.decompile.z80 (see z80.hpp).

#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::decompile::z80 {

namespace {
// the prefix byte strings the table is keyed by
py::bytes byte_string(int64_t b) { return py::bytes(std::string(1, static_cast<char>(b))); }
const py::bytes P_NONE = py::bytes();
const py::bytes P_CB = byte_string(0xCB);
const py::bytes P_ED = byte_string(0xED);
const py::bytes P_DD = byte_string(0xDD);
const py::bytes P_FD = byte_string(0xFD);
const py::bytes P_DDCB = P_DD + P_CB;
const py::bytes P_FDCB = P_FD + P_CB;
}  // namespace

const py::list<py::str> R = {py::str("B"), py::str("C"), py::str("D"), py::str("E"), py::str("H"), py::str("L"), py::str("(HL)"), py::str("A")};
const py::list<py::str> RP = {py::str("BC"), py::str("DE"), py::str("HL"), py::str("SP")};
const py::list<py::str> RP2 = {py::str("BC"), py::str("DE"), py::str("HL"), py::str("AF")};
const py::list<py::str> CC = {py::str("NZ"), py::str("Z"), py::str("NC"), py::str("C"), py::str("PO"), py::str("PE"), py::str("P"), py::str("M")};
const py::list<py::str> ALU = {py::str("ADD A,"), py::str("ADC A,"), py::str("SUB "), py::str("SBC A,"), py::str("AND "), py::str("XOR "), py::str("OR "), py::str("CP ")};
const py::list<py::str> ROT = {py::str("RLC"), py::str("RRC"), py::str("RL"), py::str("RR"), py::str("SLA"), py::str("SRA"), py::str("SLL"), py::str("SRL")};
const py::list<py::str> IM = {py::str("0"), py::str("0"), py::str("1"), py::str("2"), py::str("0"), py::str("0"), py::str("1"), py::str("2")};
const py::dict<std::tuple<int64_t, int64_t>, py::str> BLOCK = {
    {{4, 0}, py::str("LDI")},  {{4, 1}, py::str("CPI")},  {{4, 2}, py::str("INI")},  {{4, 3}, py::str("OUTI")},
    {{5, 0}, py::str("LDD")},  {{5, 1}, py::str("CPD")},  {{5, 2}, py::str("IND")},  {{5, 3}, py::str("OUTD")},
    {{6, 0}, py::str("LDIR")}, {{6, 1}, py::str("CPIR")}, {{6, 2}, py::str("INIR")}, {{6, 3}, py::str("OTIR")},
    {{7, 0}, py::str("LDDR")}, {{7, 1}, py::str("CPDR")}, {{7, 2}, py::str("INDR")}, {{7, 3}, py::str("OTDR")}};

// The unprefixed opcode `op` as a pattern ("LD B,n", "JR NZ,e", ...).
std::optional<py::str> _main(int64_t op) {
    int64_t x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    typedef std::optional<py::str> Pat;
    if (x == 0) {
        if (z == 0) {
            static const py::list<py::str> names = {py::str("NOP"), py::str("EX AF,AF'"), py::str("DJNZ e"), py::str("JR e"),
                                                    py::str("JR NZ,e"), py::str("JR Z,e"), py::str("JR NC,e"), py::str("JR C,e")};
            return Pat(names[y]);
        }
        if (z == 1) {
            return Pat(q ? py::percent("ADD HL,%s", RP[p]) : py::percent("LD %s,nn", RP[p]));
        }
        if (z == 2) {
            static const py::list<py::list<py::str>> names = {
                py::list<py::str>{py::str("LD (BC),A"), py::str("LD (DE),A"), py::str("LD (nn),HL"), py::str("LD (nn),A")},
                py::list<py::str>{py::str("LD A,(BC)"), py::str("LD A,(DE)"), py::str("LD HL,(nn)"), py::str("LD A,(nn)")}};
            return Pat(names[q][p]);
        }
        if (z == 3) {
            return Pat(q ? py::percent("DEC %s", RP[p]) : py::percent("INC %s", RP[p]));
        }
        if (z == 4) {
            return Pat(py::percent("INC %s", R[y]));
        }
        if (z == 5) {
            return Pat(py::percent("DEC %s", R[y]));
        }
        if (z == 6) {
            return Pat(py::percent("LD %s,n", R[y]));
        }
        static const py::list<py::str> names = {py::str("RLCA"), py::str("RRCA"), py::str("RLA"), py::str("RRA"),
                                                py::str("DAA"), py::str("CPL"), py::str("SCF"), py::str("CCF")};
        return Pat(names[y]);
    }
    if (x == 1) {
        return Pat(op == 0x76 ? py::str("HALT") : py::percent("LD %s,%s", R[y], R[z]));
    }
    if (x == 2) {
        return Pat(ALU[y] + R[z]);
    }
    if (z == 0) {
        return Pat(py::percent("RET %s", CC[y]));
    }
    if (z == 1) {
        if (!q) {
            return Pat(py::percent("POP %s", RP2[p]));
        }
        static const py::list<py::str> names = {py::str("RET"), py::str("EXX"), py::str("JP (HL)"), py::str("LD SP,HL")};
        return Pat(names[p]);
    }
    if (z == 2) {
        return Pat(py::percent("JP %s,nn", CC[y]));
    }
    if (z == 3) {
        static const py::list<Pat> names = {Pat(py::str("JP nn")), Pat(), Pat(py::str("OUT (n),A")), Pat(py::str("IN A,(n)")),
                                            Pat(py::str("EX (SP),HL")), Pat(py::str("EX DE,HL")), Pat(py::str("DI")), Pat(py::str("EI"))};
        return names[y];
    }
    if (z == 4) {
        return Pat(py::percent("CALL %s,nn", CC[y]));
    }
    if (z == 5) {
        if (!q) {
            return Pat(py::percent("PUSH %s", RP2[p]));
        }
        static const py::list<Pat> names = {Pat(py::str("CALL nn")), Pat(), Pat(), Pat()};
        return names[p];
    }
    if (z == 6) {
        return Pat(ALU[y] + py::str("n"));
    }
    return Pat(py::percent("RST $%02X", y * 8));
}

py::str _cb(int64_t op, const py::str& target) {
    int64_t x = op >> 6, y = (op >> 3) & 7, z = op & 7;
    py::str reg = target == "(HL)" ? R[z] : target;
    if (x == 0) {
        return py::percent("%s %s", ROT[y], reg);
    }
    static const py::list<py::str> names = {py::str(""), py::str("BIT"), py::str("RES"), py::str("SET")};
    return py::percent("%s %d,%s", names[x], y, reg);
}

std::optional<py::str> _ed(int64_t op) {
    int64_t x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    typedef std::optional<py::str> Pat;
    if (x == 1) {
        if (z == 0) {
            return Pat(y == 6 ? py::str("IN (C)") : py::percent("IN %s,(C)", R[y]));
        }
        if (z == 1) {
            return Pat(y == 6 ? py::str("OUT (C),0") : py::percent("OUT (C),%s", R[y]));
        }
        if (z == 2) {
            return Pat(q ? py::percent("ADC HL,%s", RP[p]) : py::percent("SBC HL,%s", RP[p]));
        }
        if (z == 3) {
            return Pat(q ? py::percent("LD %s,(nn)", RP[p]) : py::percent("LD (nn),%s", RP[p]));
        }
        if (z == 4) {
            return Pat(py::str("NEG"));
        }
        if (z == 5) {
            return Pat(y == 1 ? py::str("RETI") : py::str("RETN"));
        }
        if (z == 6) {
            return Pat(py::percent("IM %s", IM[y]));
        }
        static const py::list<py::str> names = {py::str("LD I,A"), py::str("LD R,A"), py::str("LD A,I"), py::str("LD A,R"),
                                                py::str("RRD"), py::str("RLD"), py::str("NOP"), py::str("NOP")};
        return Pat(names[y]);
    }
    if (x == 2 && z <= 3 && y >= 4) {
        return Pat(BLOCK.at(std::make_tuple(y, z)));
    }
    return Pat();  // an ED opcode that does nothing (a two-byte NOP)
}

// An unprefixed pattern as the DD/FD page has it: HL -> IX, H/L -> IXH/IXL, (HL) -> (IX+d) (and
// then H/L stay H/L), or None when the prefix changes nothing (the opcode runs as unprefixed).
std::optional<py::str> _index(const std::optional<py::str>& pattern, const py::str& ix) {
    if (!pattern.has_value() || (*pattern == "EX DE,HL" || *pattern == "EXX") || (*pattern).startswith(py::str("RST"))) {
        return std::nullopt;
    }
    py::list<py::str> words = py::re::split(py::str("([ ,()+])"), *pattern);
    if (*pattern == "JP (HL)") {  // a jump to HL's value, not through memory
        return py::percent("JP (%s)", ix);
    }
    py::str out;
    if ((*pattern).contains(py::str("(HL)"))) {
        out = (*pattern).replace(py::str("(HL)"), py::percent("(%s+d)", ix));
    } else if (words.contains(py::str("HL"))) {
        py::list<py::str> items;
        for (const py::str& w : words) {
            items.append(w == "HL" ? ix : w);
        }
        out = py::join(py::str(""), items);
    } else if (words.contains(py::str("H")) || words.contains(py::str("L"))) {
        py::list<py::str> items;
        for (const py::str& w : words) {
            items.append((w == "H" || w == "L") ? ix + w : w);
        }
        out = py::join(py::str(""), items);
    } else {
        return std::nullopt;
    }
    return out;
}

// ---- the table: (prefix bytes, opcode) -> pattern ----------------------------------------------

Table _build() {
    Table table;
    for (int64_t op = 0; op < 256; ++op) {
        table.set(TableKey(P_CB, op), _cb(op));
        table.set(TableKey(P_ED, op), _ed(op));
        const std::pair<py::bytes, const char*> pages[] = {{P_DD, "IX"}, {P_FD, "IY"}};
        for (const auto& page : pages) {
            const py::bytes& prefix = page.first;
            py::str ix = page.second;
            if (!(op == 0xCB || op == 0xDD || op == 0xED || op == 0xFD)) {
                std::optional<py::str> pat = _index(_main(op), ix);
                if (pat.has_value() && !pat->empty()) {
                    table.set(TableKey(prefix, op), pat);
                }
            }
            // DDCB d op: the operation on (IX+d); the undocumented forms also copy the result to a register
            int64_t z = op & 7;
            py::str base = _cb(op, py::percent("(%s+d)", ix));
            if (z != 6 && (op >> 6) != 1) {
                base += py::percent(",%s", R[z]);
            }
            table.set(TableKey(prefix + P_CB, op), base);
        }
        if (!(op == 0xCB || op == 0xDD || op == 0xED || op == 0xFD)) {
            table.set(TableKey(P_NONE, op), _main(op));
        }
    }
    return table;
}

const Table TABLE = _build();
// placeholder sizes
const py::dict<py::str, int64_t> _PLACE = {{py::str("n"), 1}, {py::str("nn"), 2}, {py::str("d"), 1}, {py::str("e"), 1}};

py::str Insn::text() const { return z80::text(*this); }
py::str Insn::repr() const { return py::percent("Insn(%s)", z80::text(*this)); }

// How control leaves the instruction: "jump" "branch" "call" "ccall" "ret" "cret" "indirect"
// "halt" or "" (falls through).
py::str _flow(const py::str& pattern) {
    py::str m = py::split(pattern, py::str(" "))[0];
    if (m == "JP" || m == "JR") {
        if (pattern.contains(py::str("("))) {
            return "indirect";
        }
        return pattern.contains(py::str(",")) ? "branch" : "jump";
    }
    if (m == "DJNZ") {
        return "branch";
    }
    if (m == "CALL") {
        return pattern.contains(py::str(",")) ? "ccall" : "call";
    }
    if (m == "RST") {
        return "call";
    }
    if (m == "RET") {
        return pattern.contains(py::str(" ")) ? "cret" : "ret";
    }
    if (m == "RETI" || m == "RETN") {
        return "ret";
    }
    if (m == "HALT") {
        return "halt";
    }
    return "";
}

py::list<py::str> _fields(const py::str& pattern) {
    return py::re::findall(py::str("\\b(nn|n|d|e)\\b"), pattern);
}

// A DD or FD the next opcode does not use: the CPU ignores it (a one-byte instruction that does nothing). It is
// written as its byte (an assembler would write NOP, 00, for "NOP").
std::optional<Insn> _lone_prefix(const py::bytes& code, int64_t at, int64_t pc) {
    return Insn(py::percent(".db $%02X", code[at]), 1, py::list<int64_t>(), std::nullopt, py::str(""), pc, code.slice(at, at + 1));
}

// The instruction at code[at:] (it runs at address `pc`), or None if code ends inside it.
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    py::bytes prefix;
    int64_t i = at;
    while (i < py::len(code) && (code[i] == 0xDD || code[i] == 0xFD) && prefix.slice(-1, py::none) != P_ED) {
        if (prefix == P_DD || prefix == P_FD) {
            // a prefix after a prefix: the first one does nothing (a one-byte instruction of its own)
            return _lone_prefix(code, at, pc);
        }
        prefix = byte_string(code[i]);
        i += 1;
    }
    if (i >= py::len(code)) {
        return std::nullopt;
    }
    int64_t op = code[i];
    if ((op == 0xCB || op == 0xED) && prefix.empty()) {
        prefix = byte_string(op);
        i += 1;
        if (i >= py::len(code)) {
            return std::nullopt;
        }
        op = code[i];
    } else if (op == 0xCB && !prefix.empty()) {
        // DDCB d op: the displacement comes before the opcode
        if (i + 2 >= py::len(code)) {
            return std::nullopt;
        }
        int64_t d = code[i + 1];
        op = code[i + 2];
        py::str pattern = TABLE.at(TableKey(prefix + P_CB, op)).value();
        py::list<int64_t> values = {d >= 128 ? d - 256 : d};
        return Insn(pattern, i + 3 - at, values, std::nullopt, py::str(""), pc, code.slice(at, i + 3));
    } else if (op == 0xED && !prefix.empty()) {
        return _lone_prefix(code, at, pc);  // DD ED: the DD is ignored
    }
    std::optional<std::optional<py::str>> got = TABLE.get(TableKey(prefix, op));
    std::optional<py::str> pattern = got.has_value() ? *got : std::nullopt;
    if (!pattern.has_value() && (prefix == P_DD || prefix == P_FD)) {
        return _lone_prefix(code, at, pc);  // DD before an unindexed op
    }
    if (!pattern.has_value()) {
        if (prefix != P_ED) {
            return std::nullopt;
        }
        return Insn(py::percent(".db $ED,$%02X", op), i + 1 - at, py::list<int64_t>(), std::nullopt, py::str(""), pc, code.slice(at, i + 1));
    }
    i += 1;
    py::list<int64_t> values;
    for (const py::str& f : _fields(*pattern)) {
        int64_t size = _PLACE.at(f);
        if (i + size > py::len(code)) {
            return std::nullopt;
        }
        int64_t v = size == 1 ? code[i] : (code[i] | (code[i + 1] << 8));
        if ((f == "d" || f == "e") && v >= 128) {
            v -= 256;
        }
        values.append(v);
        i += size;
    }
    int64_t size = i - at;
    py::str flow = _flow(*pattern);
    std::optional<int64_t> target;
    bool flows = flow == "jump" || flow == "branch" || flow == "call" || flow == "ccall";
    if (_fields(*pattern).contains(py::str("e"))) {
        target = (pc + size + values[-1]) & 0xFFFF;
        values[-1] = *target;
    } else if (flows && pattern->startswith(py::str("RST"))) {
        target = py::int_from_str(py::split(*pattern, py::str("$"))[1], 16);
    } else if (flows) {
        target = values[-1];
    }
    return Insn(*pattern, size, values, target, flow, pc, code.slice(at, i));
}

// The assembler's table, built the first time it is needed (the Python's _ASM, None until then).
static std::optional<AsmTable> ASM_;
static const AsmTable& asm_table() {
    if (!ASM_.has_value()) {
        ASM_ = _patterns();
    }
    return *ASM_;
}

// The instruction as source; `labels` {address: name} names jump targets and absolute words. An encoding that is
// not the one an assembler writes for its instruction (the undocumented mirrors: NEG at ED 4C ... 7C, RETN, IM,
// LD HL,(nn) at ED 6B) is written as its bytes, the instruction in a comment, so the source rebuilds the same.
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    if (!insn.pattern.startswith(py::str(".")) && !_canonical(insn)) {
        py::list<py::str> hexes;
        for (int64_t b : insn.bytes) {
            hexes.append(py::percent("$%02X", b));
        }
        return py::percent(".db %s ; %s", py::join(py::str(","), hexes), _plain_text(insn, labels));
    }
    return _plain_text(insn, labels);
}

// Whether the instruction's prefix and opcode are those its pattern assembles to.
bool _canonical(const Insn& insn) {
    std::optional<AsmEntry> entry = asm_table().get(insn.pattern);
    if (!entry.has_value()) {
        return true;
    }
    const py::bytes& code = entry->code;
    if (entry->displacement_first) {  // DDCB d op
        return insn.bytes.slice(0, 2) == code && insn.bytes.slice(3, 4) == py::bytes::from_list(py::list<int64_t>{*entry->cbop});
    }
    return insn.bytes.slice(0, py::len(code)) == code;
}

py::str _plain_text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    py::list<py::str> out;
    int64_t k = 0;
    for (const py::str& part : py::re::split(py::str("(\\bnn\\b|\\bn\\b|\\bd\\b|\\be\\b)"), insn.pattern)) {
        if (_PLACE.contains(part)) {
            int64_t v = insn.values[k];
            k += 1;
            if (part == "e" || (part == "nn" && labels.contains(v))) {
                out.append(labels.get(v, py::percent("$%04X", v)));
            } else if (part == "nn") {
                out.append(py::percent("$%04X", v));
            } else if (part == "d") {
                out.append(v >= 0 ? py::percent("+$%02X", v) : py::percent("-$%02X", py::checked_neg(v)));
            } else {
                out.append(py::percent("$%02X", v));
            }
        } else {
            out.append(part);
        }
    }
    py::str s = py::join(py::str(""), out).replace(py::str("++"), py::str("+")).replace(py::str("+-"), py::str("-"));
    return s;
}

// ---- the assembler -------------------------------------------------------------------------------

// {normalised pattern: (prefix + opcode bytes, displacement-first?)} for the assembler.
AsmTable _patterns() {
    AsmTable out;
    for (const auto& item : TABLE.items()) {
        const py::bytes& prefix = std::get<0>(std::get<0>(item));
        int64_t op = std::get<1>(std::get<0>(item));
        const std::optional<py::str>& pattern = std::get<1>(item);
        if (!pattern.has_value() || pattern->empty() || pattern->startswith(py::str(".db"))) {
            continue;
        }
        const py::str& key = *pattern;
        if (prefix == P_DDCB || prefix == P_FDCB) {
            out.setdefault(key, AsmEntry{prefix, op, true});
        } else {
            out.setdefault(key, AsmEntry{prefix + byte_string(op), std::nullopt, false});
        }
    }
    return out;
}

const py::re::Pattern _NUMBER = py::re::compile(py::str("^(\\$[0-9A-Fa-f]+|0x[0-9A-Fa-f]+|[0-9][0-9A-Fa-f]*[hH]|%[01]+|[0-9]+)$"));
const py::set<py::str> _WORDS = py::set<py::str>::from(
    R + RP + RP2 + CC +
    py::list<py::str>{py::str("IX"), py::str("IY"), py::str("IXH"), py::str("IXL"), py::str("IYH"), py::str("IYL"), py::str("I"),
                      py::str("R"), py::str("AF'"), py::str("(C)"), py::str("(SP)"), py::str("(BC)"), py::str("(DE)"),
                      py::str("(IX)"), py::str("(IY)")});

// ---- value(): the Python's eval() of the expression, which here is a small Python-expression evaluator -----
//
// value() turns the operands of an expression into decimal text (numbers, labels' values, `$` as pc) and
// hands the joined text to Python's eval. The text is therefore Python's own expression grammar, so
// quirks come with it: adjacent numbers join into one ("1 2" is 12), `**` is a power, a negative label
// substitutes as "-5" (so "X**2" with X=-5 is -25), `2**-1` is the float 0.5 (int() makes it 0), a leading
// zero is a SyntaxError, `<` and `>` in the middle compare, "(1)(2)" is a call (TypeError).
// The grammar parsed here: or, xor, and, shifts, + -, * //, unary + - ~, **, calls, "()" and comparison chains.
// Unbounded Python ints are int64_t (OverflowError past it); a complex result of a negative base to a
// fractional power is raised as the TypeError int(complex) would end in.
namespace {

struct Val {
    enum Kind { Int, Float, Tuple } kind = Int;
    int64_t i = 0;
    double f = 0.0;
    static Val integer(int64_t v) { Val r; r.kind = Int; r.i = v; return r; }
    static Val real(double v) { Val r; r.kind = Float; r.f = v; return r; }
    static Val tuple() { Val r; r.kind = Tuple; return r; }
    double as_double() const { return kind == Float ? f : static_cast<double>(i); }
};

struct Node;
using NodeP = std::shared_ptr<Node>;
struct Node {
    enum Kind { Lit, EmptyTuple, Unary, Binary, Compare, Call } kind = Lit;
    int64_t lit = 0;
    std::string op;
    NodeP a, b;                    // Unary: a; Binary: a, b; Call: callee a, argument b (or none)
    std::vector<NodeP> operands;   // Compare: the operands
    std::string cmps;              // Compare: '<' or '>' between them
};

class Parser {
  public:
    explicit Parser(const std::string& s) : s_(s) {}
    NodeP parse() {
        NodeP n = comparison();
        if (pos_ != s_.size()) fail();
        return n;
    }

  private:
    const std::string& s_;
    size_t pos_ = 0;

    [[noreturn]] void fail() const { throw SyntaxError("invalid syntax"); }
    bool at(const char* op) const { return s_.compare(pos_, std::strlen(op), op) == 0; }
    bool end() const { return pos_ >= s_.size(); }
    void expect(const char* op) {
        if (!at(op)) fail();
        pos_ += std::strlen(op);
    }
    static NodeP make(Node::Kind k, const char* op, NodeP a, NodeP b) {
        NodeP n = std::make_shared<Node>();
        n->kind = k;
        n->op = op;
        n->a = std::move(a);
        n->b = std::move(b);
        return n;
    }

    NodeP comparison() {
        NodeP first = bor();
        std::vector<NodeP> operands = {first};
        std::string cmps;
        while ((at("<") && !at("<<")) || (at(">") && !at(">>"))) {
            cmps.push_back(s_[pos_]);
            pos_ += 1;
            operands.push_back(bor());
        }
        if (cmps.empty()) return first;
        NodeP n = std::make_shared<Node>();
        n->kind = Node::Compare;
        n->operands = operands;
        n->cmps = cmps;
        return n;
    }
    NodeP bor() {
        NodeP n = bxor();
        while (at("|")) { pos_ += 1; n = make(Node::Binary, "|", n, bxor()); }
        return n;
    }
    NodeP bxor() {
        NodeP n = band();
        while (at("^")) { pos_ += 1; n = make(Node::Binary, "^", n, band()); }
        return n;
    }
    NodeP band() {
        NodeP n = shift();
        while (at("&")) { pos_ += 1; n = make(Node::Binary, "&", n, shift()); }
        return n;
    }
    NodeP shift() {
        NodeP n = arith();
        while (at("<<") || at(">>")) {
            const char* op = at("<<") ? "<<" : ">>";
            pos_ += 2;
            n = make(Node::Binary, op, n, arith());
        }
        return n;
    }
    NodeP arith() {
        NodeP n = term();
        while (at("+") || at("-")) {
            const char* op = at("+") ? "+" : "-";
            pos_ += 1;
            n = make(Node::Binary, op, n, term());
        }
        return n;
    }
    NodeP term() {
        NodeP n = factor();
        while ((at("*") && !at("**")) || at("//")) {
            const char* op = at("//") ? "//" : "*";
            pos_ += std::strlen(op);
            n = make(Node::Binary, op, n, factor());
        }
        return n;
    }
    NodeP factor() {
        if (at("+") || at("-") || at("~")) {
            const char* op = at("+") ? "+" : at("-") ? "-" : "~";
            pos_ += 1;
            return make(Node::Unary, op, factor(), nullptr);
        }
        return power();
    }
    NodeP power() {
        NodeP n = primary();
        if (at("**")) {
            pos_ += 2;
            n = make(Node::Binary, "**", n, factor());
        }
        return n;
    }
    NodeP primary() {
        NodeP n = atom();
        while (at("(")) {  // a call
            pos_ += 1;
            NodeP arg;
            if (!at(")")) arg = comparison();
            expect(")");
            n = make(Node::Call, "", n, arg);
        }
        return n;
    }
    NodeP atom() {
        if (end()) fail();
        char c = s_[pos_];
        if (c >= '0' && c <= '9') {
            size_t start = pos_;
            while (pos_ < s_.size() && s_[pos_] >= '0' && s_[pos_] <= '9') pos_ += 1;
            std::string digits = s_.substr(start, pos_ - start);
            bool nonzero = digits.find_first_not_of('0') != std::string::npos;
            if (digits[0] == '0' && nonzero) {
                throw SyntaxError("leading zeros in decimal integer literals are not permitted");
            }
            int64_t v = 0;
            for (char d : digits) {
                v = py::checked_add(py::checked_mul(v, 10), d - '0');
            }
            NodeP n = std::make_shared<Node>();
            n->kind = Node::Lit;
            n->lit = v;
            return n;
        }
        if (c == '(') {
            pos_ += 1;
            if (at(")")) {
                pos_ += 1;
                NodeP n = std::make_shared<Node>();
                n->kind = Node::EmptyTuple;
                return n;
            }
            NodeP n = comparison();
            expect(")");
            return n;
        }
        fail();
    }
};

[[noreturn]] void type_error(const char* what) { throw py::TypeError(what); }

Val arithmetic(const std::string& op, const Val& l, const Val& r) {
    if (l.kind == Val::Tuple || r.kind == Val::Tuple) {
        if (op == "+" && l.kind == Val::Tuple && r.kind == Val::Tuple) return Val::tuple();
        if (op == "*" && ((l.kind == Val::Tuple && r.kind == Val::Int) || (l.kind == Val::Int && r.kind == Val::Tuple))) return Val::tuple();
        type_error("unsupported operand type(s) for a tuple");
    }
    if (l.kind == Val::Float || r.kind == Val::Float) {
        double a = l.as_double(), b = r.as_double();
        if (op == "+") return Val::real(a + b);
        if (op == "-") return Val::real(a - b);
        if (op == "*") return Val::real(a * b);
        if (op == "//") return Val::real(py::floordiv(a, b));
        if (op == "**") {
            if (a == 0.0 && b < 0) throw py::ZeroDivisionError("0.0 cannot be raised to a negative power");
            if (a < 0 && b != std::floor(b) && !std::isinf(b)) type_error("int() argument must be a string, a bytes-like object or a real number, not 'complex'");
            double v = std::pow(a, b);
            if (std::isinf(v) && !std::isinf(a) && !std::isinf(b)) throw py::OverflowError("(34, 'Numerical result out of range')");
            return Val::real(v);
        }
        type_error("unsupported operand type(s) for a float");
    }
    int64_t a = l.i, b = r.i;
    if (op == "+") return Val::integer(py::checked_add(a, b));
    if (op == "-") return Val::integer(py::checked_sub(a, b));
    if (op == "*") return Val::integer(py::checked_mul(a, b));
    if (op == "//") return Val::integer(py::floordiv(a, b));
    if (op == "**") {
        if (b >= 0) return Val::integer(py::ipow(a, b));
        if (a == 0) throw py::ZeroDivisionError("0.0 cannot be raised to a negative power");
        return Val::real(std::pow(static_cast<double>(a), static_cast<double>(b)));
    }
    if (op == "<<") return Val::integer(py::shl(a, b));
    if (op == ">>") return Val::integer(py::shr(a, b));
    if (op == "&") return Val::integer(a & b);
    if (op == "|") return Val::integer(a | b);
    return Val::integer(a ^ b);  // "^"
}

bool less(const Val& a, const Val& b) {
    if (a.kind == Val::Tuple && b.kind == Val::Tuple) return false;  // () < ()
    if (a.kind == Val::Tuple || b.kind == Val::Tuple) type_error("'<' not supported between a tuple and a number");
    if (a.kind == Val::Int && b.kind == Val::Int) return a.i < b.i;
    long double x = a.kind == Val::Float ? static_cast<long double>(a.f) : static_cast<long double>(a.i);
    long double y = b.kind == Val::Float ? static_cast<long double>(b.f) : static_cast<long double>(b.i);
    return x < y;
}

Val evaluate(const Node& n) {
    switch (n.kind) {
        case Node::Lit:
            return Val::integer(n.lit);
        case Node::EmptyTuple:
            return Val::tuple();
        case Node::Unary: {
            Val v = evaluate(*n.a);
            if (v.kind == Val::Tuple) type_error("bad operand type for a unary operator: 'tuple'");
            if (n.op == "-") return v.kind == Val::Float ? Val::real(-v.f) : Val::integer(py::checked_neg(v.i));
            if (n.op == "+") return v;
            if (v.kind == Val::Float) type_error("bad operand type for unary ~: 'float'");
            return Val::integer(~v.i);
        }
        case Node::Binary: {
            Val l = evaluate(*n.a);
            Val r = evaluate(*n.b);
            return arithmetic(n.op, l, r);
        }
        case Node::Compare: {
            Val left = evaluate(*n.operands[0]);
            for (size_t k = 0; k < n.cmps.size(); ++k) {
                Val right = evaluate(*n.operands[k + 1]);
                bool ok = n.cmps[k] == '<' ? less(left, right) : less(right, left);
                if (!ok) return Val::integer(0);
                left = right;
            }
            return Val::integer(1);
        }
        case Node::Call:
        default: {
            evaluate(*n.a);
            if (n.b) evaluate(*n.b);
            type_error("object is not callable");
        }
    }
}

// int(eval(text))
int64_t eval_int(const std::string& text) {
    Parser parser(text);
    NodeP tree = parser.parse();
    Val v = evaluate(*tree);
    if (v.kind == Val::Tuple) type_error("int() argument must be a string, a bytes-like object or a real number, not 'tuple'");
    if (v.kind == Val::Float) return py::int_from_float(v.f);
    return v.i;
}

// int(digits, base) for the digit strings value() hands it
int64_t parse_digits(const py::str& digits, int64_t base) {
    int64_t v = 0;
    for (char32_t c : digits.u32()) {
        int64_t d = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : c - 'A' + 10;
        v = py::checked_add(py::checked_mul(v, base), d);
    }
    return v;
}

const char* const TOKEN_PATTERN =
    R"re(\$[0-9A-Fa-f]+|0x[0-9A-Fa-f]+|[0-9][0-9A-Fa-f]*[hH]\b|%[01]+|[0-9]+|'.'|[A-Za-z_.@][\w.@]*|\$|<<|>>|[-+*/()&|^~<>])re";

}  // namespace

// A numeric expression: numbers ($1F 1Fh %0101 31 'c'), labels, $ (here), + - * / and parentheses.
int64_t value(const py::str& expr_in, const py::dict<py::str, int64_t>& labels, int64_t pc) {
    py::str expr = expr_in.strip();
    py::list<py::str> tokens = py::re::findall(py::str(TOKEN_PATTERN), expr);
    py::list<py::str> pieces;
    for (const py::str& t : tokens) {
        const std::u32string& u = t.u32();
        if (t == "$") {
            pieces.append(py::to_str(pc));
        } else if (t.startswith(py::str("$"))) {
            pieces.append(py::to_str(parse_digits(t.slice_from(1), 16)));
        } else if (t.lower().startswith(py::str("0x"))) {
            pieces.append(py::to_str(parse_digits(t.slice_from(2), 16)));
        } else if (u[0] >= '0' && u[0] <= '9' && (u.back() == 'h' || u.back() == 'H')) {
            pieces.append(py::to_str(parse_digits(t.slice(0, -1), 16)));
        } else if (t.startswith(py::str("%")) && py::len(t) > 1) {
            pieces.append(py::to_str(parse_digits(t.slice_from(1), 2)));
        } else if (t.startswith(py::str("'"))) {
            pieces.append(py::to_str(static_cast<int64_t>(u[1])));
        } else if (u[0] >= '0' && u[0] <= '9') {
            pieces.append(t);
        } else if ((u[0] >= 'A' && u[0] <= 'Z') || (u[0] >= 'a' && u[0] <= 'z') || u[0] == '_' || u[0] == '.' || u[0] == '@') {
            if (!labels.contains(t)) {
                throw py::KeyError(t);
            }
            pieces.append(py::to_str(labels.at(t)));
        } else if (t == "<") {
            pieces.append(py::str("<"));
        } else if (t == ">") {
            pieces.append(py::str(">"));
        } else {
            pieces.append(t.replace(py::str("/"), py::str("//")));
        }
    }
    if (pieces.empty()) {
        throw py::ValueError(expr);
    }
    if (pieces[0] == "<" || pieces[0] == ">") {  // <x low byte, >x high byte (WLA-DX)
        int64_t v = eval_int(py::join(py::str(""), pieces.slice_from(1)).utf8());
        return pieces[0] == "<" ? (v & 0xFF) : ((v >> 8) & 0xFF);
    }
    return eval_int(py::join(py::str(""), pieces).utf8());
}

py::list<py::str> _split_operands(const py::str& s) {
    py::list<py::str> out;
    int64_t depth = 0;
    py::str cur = "";
    for (const py::str& c : s) {
        if (c == "(") {
            depth += 1;
        } else if (c == ")") {
            depth -= 1;
        }
        if (c == "," && depth == 0) {
            out.append(cur.strip());
            cur = "";
        } else {
            cur += c;
        }
    }
    if (!cur.strip().empty()) {
        out.append(cur.strip());
    }
    return out;
}

// The bytes of one instruction (`labels` {name: address}); KeyError for an unknown label,
// ValueError for something that is not an instruction.
py::bytes assemble(const py::str& line_in, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    const AsmTable& asm_ = asm_table();
    py::str line = line_in.strip();
    auto [mnemonic0, sep, rest] = line.partition(py::str(" "));
    py::str mnemonic = mnemonic0.upper();
    py::list<py::str> ops = !rest.strip().empty() ? _split_operands(rest) : py::list<py::str>();
    py::list<py::str> shape;
    py::list<std::tuple<py::str, py::str>> exprs;
    static const py::re::Pattern whitespace = py::re::compile(py::str("\\s+"));
    static const py::re::Pattern index_form = py::re::compile(py::str("^\\((I[XY])([+-].*)\\)$"), py::re::I);
    auto squash = [&](const py::str& o) { return whitespace.sub(py::str(""), o); };
    if (mnemonic == "OUT") {
        py::list<py::str> squashed;
        for (const py::str& o : ops) {
            squashed.append(squash(o).upper());
        }
        if (squashed == py::list<py::str>{py::str("(C)"), py::str("0")}) {
            return py::bytes::from_list(py::list<int64_t>{0xED, 0x71});  // OUT (C),0 (undocumented): 0 is part of the instruction
        }
    }
    for (const py::str& o : ops) {
        py::str u = squash(o).upper();
        if (_WORDS.contains(u) || u == "AF'") {
            shape.append(u);
            continue;
        }
        std::optional<py::re::Match> m = index_form.match(squash(o));
        if (m.has_value()) {
            shape.append(py::percent("(%s+d)", m->group(1).upper()));
            exprs.append(std::make_tuple(py::str("d"), m->group(2)));
            continue;
        }
        if (mnemonic == "RST") {
            shape.append(py::percent("$%02X", value(o, labels, pc)));
            continue;
        }
        if (mnemonic == "IM" || ((mnemonic == "BIT" || mnemonic == "SET" || mnemonic == "RES") && shape.empty())) {
            shape.append(py::to_str(value(o, labels, pc)));
            continue;
        }
        if (o.startswith(py::str("(")) && o.endswith(py::str(")")) && _balanced(o.slice(1, -1))) {
            shape.append(py::str("(?)"));
            exprs.append(std::make_tuple(py::str("?"), o.slice(1, -1)));
        } else {
            shape.append(py::str("?"));
            exprs.append(std::make_tuple(py::str("?"), o));
        }
    }
    // try each way the value operands can be read
    int64_t unknown = 0;
    for (const py::str& s : shape) {
        if (s.contains(py::str("?"))) {
            unknown += 1;
        }
    }
    py::str pattern;
    bool found = false;
    for (const py::list<py::str>& kinds : _choices(unknown)) {
        int64_t k = 0;
        py::list<py::str> candidate;
        for (const py::str& s : shape) {
            if (s.contains(py::str("?"))) {
                candidate.append(s.replace(py::str("?"), kinds[k]));
                k += 1;
            } else {
                candidate.append(s);
            }
        }
        pattern = mnemonic + (!candidate.empty() ? py::str(" ") + py::join(py::str(","), candidate) : py::str(""));
        if (asm_.contains(pattern)) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw py::ValueError(py::percent("not an instruction: %r", line));
    }
    AsmEntry entry = asm_.at(pattern);
    const py::bytes& code = entry.code;
    py::list<py::str> fields = _fields(pattern);
    py::list<py::str> vals;
    for (const auto& e : exprs) {
        vals.append(std::get<1>(e));
    }
    int64_t size = py::len(code) + (entry.displacement_first ? 1 : 0);
    for (const py::str& f : fields) {
        size += _PLACE.at(f);
    }
    std::string data;
    if (entry.displacement_first) {
        int64_t d = value(vals[0], labels, pc);
        return code + py::bytes::from_list(py::list<int64_t>{d & 0xFF, *entry.cbop});
    }
    data += code.raw();
    for (int64_t n = 0; n < std::min(py::len(fields), py::len(vals)); ++n) {
        const py::str& f = fields[n];
        int64_t v = value(vals[n], labels, pc);
        if (f == "e") {
            v = py::checked_sub(v, pc + size);
            if (!(-128 <= v && v <= 127)) {
                throw py::ValueError(py::percent("relative jump out of range: %r", line));
            }
            data.push_back(static_cast<char>(v & 0xFF));
        } else if (f == "n" || f == "d") {
            data.push_back(static_cast<char>(v & 0xFF));
        } else {
            data.push_back(static_cast<char>(v & 0xFF));
            data.push_back(static_cast<char>((v >> 8) & 0xFF));
        }
    }
    return py::bytes(data);
}

bool _balanced(const py::str& s) {
    int64_t depth = 0;
    for (const py::str& c : s) {
        depth += static_cast<int64_t>(c == "(");
        depth -= static_cast<int64_t>(c == ")");
        if (depth < 0) {
            return false;
        }
    }
    return depth == 0;
}

// Every way `count` value operands can be read, as lists of "n" "nn" "e" "d" (the first operand varying slowest).
py::list<py::list<py::str>> _choices(int64_t count) {
    py::list<py::list<py::str>> out;
    if (count == 0) {
        out.append(py::list<py::str>());
        return out;
    }
    for (const char* first : {"n", "nn", "e", "d"}) {
        for (const py::list<py::str>& rest : _choices(count - 1)) {
            py::list<py::str> kinds = {py::str(first)};
            kinds.extend(rest);
            out.append(kinds);
        }
    }
    return out;
}

}  // namespace codebreaker::decompile::z80
