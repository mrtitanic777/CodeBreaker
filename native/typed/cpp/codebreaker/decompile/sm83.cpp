// codebreaker/decompile/sm83.cpp -- C++ port of codebreaker.decompile.sm83 (see sm83.hpp).
//
// The Python takes `value`, `_split_operands` and `_balanced` from decompile/z80.py; here they are the typed
// port of that module (z80.hpp).

#include "codebreaker/decompile/sm83.hpp"

namespace codebreaker::decompile::sm83 {

const py::list<py::str> R = {py::str("B"), py::str("C"), py::str("D"), py::str("E"), py::str("H"), py::str("L"), py::str("(HL)"), py::str("A")};
const py::list<py::str> RP = {py::str("BC"), py::str("DE"), py::str("HL"), py::str("SP")};
const py::list<py::str> RP2 = {py::str("BC"), py::str("DE"), py::str("HL"), py::str("AF")};
const py::list<py::str> CC = {py::str("NZ"), py::str("Z"), py::str("NC"), py::str("C")};
const py::list<py::str> ALU = {py::str("ADD A,"), py::str("ADC A,"), py::str("SUB "), py::str("SBC A,"), py::str("AND "), py::str("XOR "), py::str("OR "), py::str("CP ")};
const py::list<py::str> ROT = {py::str("RLC"), py::str("RRC"), py::str("RL"), py::str("RR"), py::str("SLA"), py::str("SRA"), py::str("SWAP"), py::str("SRL")};
const py::set<int64_t> ILLEGAL = {0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD};

namespace {
py::bytes byte_string(int64_t b) { return py::bytes(std::string(1, static_cast<char>(b))); }
const py::bytes P_NONE = py::bytes();
const py::bytes P_CB = byte_string(0xCB);

// The `...][i]` of the Python's list literals.
py::str pick(const py::list<py::str>& names, int64_t i) { return names[i]; }
}  // namespace

// The unprefixed opcode `op` as a pattern ("LD B,n", "JR NZ,e", ...), None if the opcode does not exist.
std::optional<py::str> _main(int64_t op) {
    int64_t x = op >> 6, y = (op >> 3) & 7, z = op & 7, p = (op >> 4) & 3, q = (op >> 3) & 1;
    if (ILLEGAL.contains(op)) {
        return std::nullopt;
    }
    if (x == 0) {
        if (z == 0) {
            return pick({py::str("NOP"), py::str("LD (nn),SP"), py::str("STOP"), py::str("JR e"), py::str("JR NZ,e"), py::str("JR Z,e"), py::str("JR NC,e"), py::str("JR C,e")}, y);
        }
        if (z == 1) {
            return q ? py::percent("ADD HL,%s", RP[p]) : py::percent("LD %s,nn", RP[p]);
        }
        if (z == 2) {
            if (q) {
                return pick({py::str("LD A,(BC)"), py::str("LD A,(DE)"), py::str("LD A,(HL+)"), py::str("LD A,(HL-)")}, p);
            }
            return pick({py::str("LD (BC),A"), py::str("LD (DE),A"), py::str("LD (HL+),A"), py::str("LD (HL-),A")}, p);
        }
        if (z == 3) {
            return py::percent(q ? "DEC %s" : "INC %s", RP[p]);
        }
        if (z == 4) {
            return py::percent("INC %s", R[y]);
        }
        if (z == 5) {
            return py::percent("DEC %s", R[y]);
        }
        if (z == 6) {
            return py::percent("LD %s,n", R[y]);
        }
        return pick({py::str("RLCA"), py::str("RRCA"), py::str("RLA"), py::str("RRA"), py::str("DAA"), py::str("CPL"), py::str("SCF"), py::str("CCF")}, y);
    }
    if (x == 1) {
        return op == 0x76 ? py::str("HALT") : py::percent("LD %s,%s", R[y], R[z]);
    }
    if (x == 2) {
        return ALU[y] + R[z];
    }
    if (z == 0) {
        return pick({py::str("RET NZ"), py::str("RET Z"), py::str("RET NC"), py::str("RET C"), py::str("LDH (h),A"), py::str("ADD SP,s"), py::str("LDH A,(h)"), py::str("LD HL,SP+s")}, y);
    }
    if (z == 1) {
        return !q ? py::percent("POP %s", RP2[p]) : pick({py::str("RET"), py::str("RETI"), py::str("JP HL"), py::str("LD SP,HL")}, p);
    }
    if (z == 2) {
        return pick({py::str("JP NZ,nn"), py::str("JP Z,nn"), py::str("JP NC,nn"), py::str("JP C,nn"), py::str("LD (C),A"), py::str("LD (nn),A"), py::str("LD A,(C)"), py::str("LD A,(nn)")}, y);
    }
    if (z == 3) {
        // {0: "JP nn", 6: "DI", 7: "EI"}.get(y)
        if (y == 0) return py::str("JP nn");
        if (y == 6) return py::str("DI");
        if (y == 7) return py::str("EI");
        return std::nullopt;
    }
    if (z == 4) {
        if (y < 4) return py::percent("CALL %s,nn", CC[y]);
        return std::nullopt;
    }
    if (z == 5) {
        if (!q) return py::percent("PUSH %s", RP2[p]);
        if (p == 0) return py::str("CALL nn");
        return std::nullopt;
    }
    if (z == 6) {
        return ALU[y] + py::str("n");
    }
    return py::percent("RST $%02X", y * 8);
}

// The CB-page opcode `op` as a pattern ("RLC B", "BIT 3,(HL)", ...).
py::str _cb(int64_t op) {
    int64_t x = op >> 6, y = (op >> 3) & 7, z = op & 7;
    if (x == 0) {
        return py::percent("%s %s", ROT[y], R[z]);
    }
    return py::percent("%s %d,%s", pick({py::str(""), py::str("BIT"), py::str("RES"), py::str("SET")}, x), y, R[z]);
}

static Table build_table() {
    Table table;
    for (int64_t op = 0; op < 256; ++op) {
        if (op != 0xCB) {
            table.set(TableKey(P_NONE, op), _main(op));
        }
        table.set(TableKey(P_CB, op), std::optional<py::str>(_cb(op)));
    }
    return table;
}

const Table TABLE = build_table();
const py::dict<py::str, int64_t> _PLACE = {{py::str("n"), 1}, {py::str("nn"), 2}, {py::str("e"), 1}, {py::str("s"), 1}, {py::str("h"), 1}};

py::str Insn::text() const { return sm83::text(*this); }
py::str Insn::repr() const { return py::percent("Insn(%s)", sm83::text(*this)); }

// How control leaves the instruction: "jump" "branch" "call" "ccall" "ret" "cret" "indirect"
// "halt" or "" (falls through).
py::str _flow(const py::str& pattern) {
    py::str m = py::split(pattern, py::str(" "))[0];
    if (m == "JP" || m == "JR") {
        if (pattern == "JP HL") {
            return "indirect";
        }
        return pattern.contains(py::str(",")) ? py::str("branch") : py::str("jump");
    }
    if (m == "CALL") {
        return pattern.contains(py::str(",")) ? py::str("ccall") : py::str("call");
    }
    if (m == "RST") {
        return "call";
    }
    if (m == "RET") {
        return pattern.contains(py::str(" ")) ? py::str("cret") : py::str("ret");
    }
    if (m == "RETI") {
        return "ret";
    }
    if (m == "HALT" || m == "STOP") {
        return "halt";
    }
    return "";
}

// The placeholders in a pattern, in order.
py::list<py::str> _fields(const py::str& pattern) {
    static const py::re::Pattern fields = py::re::compile(py::str("\\b(nn|n|e|s|h)\\b"));
    return fields.findall(pattern);
}

// The instruction at code[at:] (it runs at address `pc`), None if code ends inside it or the
// opcode does not exist.
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    if (at >= py::len(code)) {
        return std::nullopt;
    }
    py::bytes prefix;
    int64_t i = at;
    if (code[i] == 0xCB) {
        prefix = P_CB;
        i += 1;
        if (i >= py::len(code)) {
            return std::nullopt;
        }
    }
    std::optional<std::optional<py::str>> got = TABLE.get(TableKey(prefix, code[i]));
    std::optional<py::str> pattern = got.has_value() ? *got : std::nullopt;
    if (!pattern.has_value()) {
        return std::nullopt;
    }
    i += 1;
    if (*pattern == "STOP") {  // STOP is two bytes ($10 $00)
        i += 1;
    }
    py::list<int64_t> values;
    for (const py::str& f : _fields(*pattern)) {
        int64_t size = _PLACE.at(f);
        if (i + size > py::len(code)) {
            return std::nullopt;
        }
        int64_t v = size == 1 ? code[i] : (code[i] | (code[i + 1] << 8));
        if ((f == "e" || f == "s") && v >= 128) {
            v -= 256;
        }
        if (f == "h") {
            v += 0xFF00;
        }
        values.append(v);
        i += size;
    }
    if (i > py::len(code)) {
        return std::nullopt;
    }
    int64_t size = i - at;
    py::str flow = _flow(*pattern);
    std::optional<int64_t> target;
    if (_fields(*pattern).contains(py::str("e"))) {
        target = (pc + size + values[-1]) & 0xFFFF;
        values[-1] = *target;
    } else if (pattern->startswith(py::str("RST"))) {
        target = py::int_from_str(py::split(*pattern, py::str("$"))[1], 16);
    } else if (flow == "jump" || flow == "branch" || flow == "call" || flow == "ccall") {
        target = values[-1];
    }
    return Insn(*pattern, size, values, target, flow, pc, code.slice(at, i));
}

// The instruction as source; `labels` {address: name} names jump targets and absolute words.
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    if (insn.pattern == "STOP") {
        py::bytes second = insn.bytes.slice(1, 2);
        if (!(second == py::bytes("\0", 1) || second == py::bytes())) {
            return py::percent("DB $10,$%02X", insn.bytes[1]);  // STOP with a second byte assemblers do not write
        }
    }
    py::list<py::str> out;
    int64_t k = 0;
    static const py::re::Pattern places = py::re::compile(py::str("(\\bnn\\b|\\bn\\b|\\be\\b|\\bs\\b|\\bh\\b)"));
    for (const py::str& part : places.split(insn.pattern)) {
        if (_PLACE.contains(part)) {
            int64_t v = insn.values[k];
            k += 1;
            if (part == "e" || (part == "nn" && labels.contains(v))) {
                out.append(labels.get(v, py::percent("$%04X", v)));
            } else if (part == "nn" || part == "h") {
                out.append(part == "h" && labels.contains(v) ? labels.get(v, py::percent("$%04X", v)) : py::percent("$%04X", v));
            } else if (part == "s") {
                out.append(v >= 0 ? py::percent("$%02X", v) : py::percent("-$%02X", py::checked_neg(v)));
            } else {
                out.append(py::percent("$%02X", v));
            }
        } else {
            out.append(part);
        }
    }
    return py::join(py::str(""), out).replace(py::str("SP+-"), py::str("SP-"));
}

const py::set<py::str> _WORDS = [] {
    py::set<py::str> words;
    for (const py::list<py::str>* l : {&R, &RP, &RP2, &CC}) {
        for (const py::str& w : *l) {
            words.add(w);
        }
    }
    for (const char* w : {"(BC)", "(DE)", "(C)", "(HL+)", "(HL-)", "(HLI)", "(HLD)", "HL"}) {
        words.add(py::str(w));
    }
    return words;
}();

// {pattern: prefix + opcode bytes} for the assembler, built the first time it is needed (the Python's _ASM,
// None until then).
static std::optional<py::dict<py::str, py::bytes>> ASM_;
static const py::dict<py::str, py::bytes>& asm_table() {
    if (!ASM_.has_value()) {
        py::dict<py::str, py::bytes> table;
        for (const auto& item : TABLE.items()) {
            const py::bytes& prefix = std::get<0>(std::get<0>(item));
            int64_t op = std::get<1>(std::get<0>(item));
            const std::optional<py::str>& pattern = std::get<1>(item);
            if (pattern.has_value() && !pattern->empty()) {
                table.setdefault(*pattern, prefix + byte_string(op));
            }
        }
        ASM_ = table;
    }
    return *ASM_;
}

// The bytes of one instruction (RGBDS syntax).
py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    const py::dict<py::str, py::bytes>& asm_ = asm_table();
    auto [mnemonic0, sep, rest] = line.strip().partition(py::str(" "));
    py::str mnemonic = mnemonic0.upper();
    py::list<py::str> ops = !rest.strip().empty() ? z80::_split_operands(rest) : py::list<py::str>();
    py::list<py::str> shape, exprs;
    static const py::re::Pattern whitespace = py::re::compile(py::str("\\s+"));
    static const py::re::Pattern sp_form = py::re::compile(py::str("^SP([+-].+)$"), py::re::I);
    for (const py::str& o : ops) {
        py::str u = whitespace.sub(py::str(""), o).upper().replace(py::str("(HLI)"), py::str("(HL+)")).replace(py::str("(HLD)"), py::str("(HL-)"));
        if (_WORDS.contains(u)) {
            shape.append(u);
            continue;
        }
        std::optional<py::re::Match> m = sp_form.match(whitespace.sub(py::str(""), o));
        if (m.has_value() && mnemonic == "LD") {
            shape.append(py::str("SP+?"));
            exprs.append(m->group(1));
            continue;
        }
        if (mnemonic == "RST") {
            shape.append(py::percent("$%02X", z80::value(o, labels, pc)));
            continue;
        }
        if ((mnemonic == "BIT" || mnemonic == "SET" || mnemonic == "RES") && shape.empty()) {
            shape.append(py::to_str(z80::value(o, labels, pc)));
            continue;
        }
        if (o.startswith(py::str("(")) && o.endswith(py::str(")")) && z80::_balanced(o.slice(1, -1))) {
            shape.append(py::str("(?)"));
            exprs.append(o.slice(1, -1));
        } else {
            shape.append(py::str("?"));
            exprs.append(o);
        }
    }
    int64_t count = 0;
    for (const py::str& s : shape) {
        for (const py::str& c : s) {
            if (c == "?") {
                count += 1;
            }
        }
    }
    py::str pattern;
    bool found = false;
    for (const py::list<py::str>& kinds : _choices(count)) {
        int64_t next = 0;
        py::list<py::str> parts;
        for (const py::str& s : shape) {
            if (s.contains(py::str("?"))) {
                parts.append(s.replace(py::str("?"), kinds[next]));
                next += 1;
            } else {
                parts.append(s);
            }
        }
        pattern = mnemonic + (!shape.empty() ? py::str(" ") + py::join(py::str(","), parts) : py::str(""));
        if (asm_.contains(pattern)) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw py::ValueError(py::percent("not an instruction: %r", line));
    }
    std::string code = asm_.at(pattern).raw();
    if (pattern == "STOP") {
        code.push_back('\0');
    }
    py::list<py::str> fields = _fields(pattern);
    int64_t size = static_cast<int64_t>(code.size());
    for (const py::str& f : fields) {
        size += _PLACE.at(f);
    }
    for (int64_t n = 0; n < std::min(py::len(fields), py::len(exprs)); ++n) {
        const py::str& f = fields[n];
        int64_t v = z80::value(exprs[n], labels, pc);
        if (f == "e") {
            v = py::checked_sub(v, pc + size);
            if (!(-128 <= v && v <= 127)) {
                throw py::ValueError(py::percent("relative jump out of range: %r", line));
            }
            code.push_back(static_cast<char>(v & 0xFF));
        } else if (f == "h") {
            if (!(0xFF00 <= v && v <= 0xFFFF)) {
                throw py::ValueError(py::percent("LDH address outside $FF00-$FFFF: %r", line));
            }
            code.push_back(static_cast<char>(v & 0xFF));
        } else if (f == "n" || f == "s") {
            code.push_back(static_cast<char>(v & 0xFF));
        } else {
            code.push_back(static_cast<char>(v & 0xFF));
            code.push_back(static_cast<char>((v >> 8) & 0xFF));
        }
    }
    return py::bytes(code);
}

// Every way `count` value operands can be read, as lists of "n" "nn" "e" "s" "h" (the first operand varying slowest).
py::list<py::list<py::str>> _choices(int64_t count) {
    py::list<py::list<py::str>> out;
    if (count == 0) {
        out.append(py::list<py::str>());
        return out;
    }
    for (const char* first : {"n", "nn", "e", "s", "h"}) {
        for (const py::list<py::str>& rest : _choices(count - 1)) {
            py::list<py::str> kinds = {py::str(first)};
            kinds.extend(rest);
            out.append(kinds);
        }
    }
    return out;
}

}  // namespace codebreaker::decompile::sm83
