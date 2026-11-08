// codebreaker/decompile/m6502.cpp -- C++ port of codebreaker.decompile.m6502 (see m6502.hpp).

#include "codebreaker/decompile/m6502.hpp"

namespace codebreaker::decompile::m6502 {

namespace {
using S = py::str;
ModeTable mt(std::initializer_list<std::pair<S, int64_t>> xs) { return ModeTable(xs); }

// str.rindex: ValueError when the substring is absent.
int64_t rindex_(const py::str& s, const py::str& sub) {
    int64_t i = s.rfind(sub);
    if (i < 0) throw py::ValueError("substring not found");
    return i;
}
}  // namespace

const py::dict<py::str, int64_t> SIZE = {
    {S("imp"), 1}, {S("acc"), 1}, {S("imm"), 2}, {S("zp"), 2}, {S("zpx"), 2}, {S("zpy"), 2}, {S("izx"), 2},
    {S("izy"), 2}, {S("rel"), 2}, {S("abs"), 3}, {S("abx"), 3}, {S("aby"), 3}, {S("ind"), 3}};

// mnemonic -> {mode: opcode}
const py::dict<py::str, ModeTable> _GROUPS = {
    {S("ADC"), mt({{S("imm"), 0x69}, {S("zp"), 0x65}, {S("zpx"), 0x75}, {S("abs"), 0x6D}, {S("abx"), 0x7D}, {S("aby"), 0x79}, {S("izx"), 0x61}, {S("izy"), 0x71}})},
    {S("AND"), mt({{S("imm"), 0x29}, {S("zp"), 0x25}, {S("zpx"), 0x35}, {S("abs"), 0x2D}, {S("abx"), 0x3D}, {S("aby"), 0x39}, {S("izx"), 0x21}, {S("izy"), 0x31}})},
    {S("ASL"), mt({{S("acc"), 0x0A}, {S("zp"), 0x06}, {S("zpx"), 0x16}, {S("abs"), 0x0E}, {S("abx"), 0x1E}})},
    {S("BCC"), mt({{S("rel"), 0x90}})}, {S("BCS"), mt({{S("rel"), 0xB0}})}, {S("BEQ"), mt({{S("rel"), 0xF0}})}, {S("BMI"), mt({{S("rel"), 0x30}})},
    {S("BNE"), mt({{S("rel"), 0xD0}})}, {S("BPL"), mt({{S("rel"), 0x10}})}, {S("BVC"), mt({{S("rel"), 0x50}})}, {S("BVS"), mt({{S("rel"), 0x70}})},
    {S("BIT"), mt({{S("zp"), 0x24}, {S("abs"), 0x2C}})},
    {S("BRK"), mt({{S("imp"), 0x00}})},
    {S("CLC"), mt({{S("imp"), 0x18}})}, {S("CLD"), mt({{S("imp"), 0xD8}})}, {S("CLI"), mt({{S("imp"), 0x58}})}, {S("CLV"), mt({{S("imp"), 0xB8}})},
    {S("CMP"), mt({{S("imm"), 0xC9}, {S("zp"), 0xC5}, {S("zpx"), 0xD5}, {S("abs"), 0xCD}, {S("abx"), 0xDD}, {S("aby"), 0xD9}, {S("izx"), 0xC1}, {S("izy"), 0xD1}})},
    {S("CPX"), mt({{S("imm"), 0xE0}, {S("zp"), 0xE4}, {S("abs"), 0xEC}})},
    {S("CPY"), mt({{S("imm"), 0xC0}, {S("zp"), 0xC4}, {S("abs"), 0xCC}})},
    {S("DEC"), mt({{S("zp"), 0xC6}, {S("zpx"), 0xD6}, {S("abs"), 0xCE}, {S("abx"), 0xDE}})},
    {S("DEX"), mt({{S("imp"), 0xCA}})}, {S("DEY"), mt({{S("imp"), 0x88}})},
    {S("EOR"), mt({{S("imm"), 0x49}, {S("zp"), 0x45}, {S("zpx"), 0x55}, {S("abs"), 0x4D}, {S("abx"), 0x5D}, {S("aby"), 0x59}, {S("izx"), 0x41}, {S("izy"), 0x51}})},
    {S("INC"), mt({{S("zp"), 0xE6}, {S("zpx"), 0xF6}, {S("abs"), 0xEE}, {S("abx"), 0xFE}})},
    {S("INX"), mt({{S("imp"), 0xE8}})}, {S("INY"), mt({{S("imp"), 0xC8}})},
    {S("JMP"), mt({{S("abs"), 0x4C}, {S("ind"), 0x6C}})},
    {S("JSR"), mt({{S("abs"), 0x20}})},
    {S("LDA"), mt({{S("imm"), 0xA9}, {S("zp"), 0xA5}, {S("zpx"), 0xB5}, {S("abs"), 0xAD}, {S("abx"), 0xBD}, {S("aby"), 0xB9}, {S("izx"), 0xA1}, {S("izy"), 0xB1}})},
    {S("LDX"), mt({{S("imm"), 0xA2}, {S("zp"), 0xA6}, {S("zpy"), 0xB6}, {S("abs"), 0xAE}, {S("aby"), 0xBE}})},
    {S("LDY"), mt({{S("imm"), 0xA0}, {S("zp"), 0xA4}, {S("zpx"), 0xB4}, {S("abs"), 0xAC}, {S("abx"), 0xBC}})},
    {S("LSR"), mt({{S("acc"), 0x4A}, {S("zp"), 0x46}, {S("zpx"), 0x56}, {S("abs"), 0x4E}, {S("abx"), 0x5E}})},
    {S("NOP"), mt({{S("imp"), 0xEA}})},
    {S("ORA"), mt({{S("imm"), 0x09}, {S("zp"), 0x05}, {S("zpx"), 0x15}, {S("abs"), 0x0D}, {S("abx"), 0x1D}, {S("aby"), 0x19}, {S("izx"), 0x01}, {S("izy"), 0x11}})},
    {S("PHA"), mt({{S("imp"), 0x48}})}, {S("PHP"), mt({{S("imp"), 0x08}})}, {S("PLA"), mt({{S("imp"), 0x68}})}, {S("PLP"), mt({{S("imp"), 0x28}})},
    {S("ROL"), mt({{S("acc"), 0x2A}, {S("zp"), 0x26}, {S("zpx"), 0x36}, {S("abs"), 0x2E}, {S("abx"), 0x3E}})},
    {S("ROR"), mt({{S("acc"), 0x6A}, {S("zp"), 0x66}, {S("zpx"), 0x76}, {S("abs"), 0x6E}, {S("abx"), 0x7E}})},
    {S("RTI"), mt({{S("imp"), 0x40}})}, {S("RTS"), mt({{S("imp"), 0x60}})},
    {S("SBC"), mt({{S("imm"), 0xE9}, {S("zp"), 0xE5}, {S("zpx"), 0xF5}, {S("abs"), 0xED}, {S("abx"), 0xFD}, {S("aby"), 0xF9}, {S("izx"), 0xE1}, {S("izy"), 0xF1}})},
    {S("SEC"), mt({{S("imp"), 0x38}})}, {S("SED"), mt({{S("imp"), 0xF8}})}, {S("SEI"), mt({{S("imp"), 0x78}})},
    {S("STA"), mt({{S("zp"), 0x85}, {S("zpx"), 0x95}, {S("abs"), 0x8D}, {S("abx"), 0x9D}, {S("aby"), 0x99}, {S("izx"), 0x81}, {S("izy"), 0x91}})},
    {S("STX"), mt({{S("zp"), 0x86}, {S("zpy"), 0x96}, {S("abs"), 0x8E}})},
    {S("STY"), mt({{S("zp"), 0x84}, {S("zpx"), 0x94}, {S("abs"), 0x8C}})},
    {S("TAX"), mt({{S("imp"), 0xAA}})}, {S("TAY"), mt({{S("imp"), 0xA8}})}, {S("TSX"), mt({{S("imp"), 0xBA}})}, {S("TXA"), mt({{S("imp"), 0x8A}})},
    {S("TXS"), mt({{S("imp"), 0x9A}})}, {S("TYA"), mt({{S("imp"), 0x98}})},
};

namespace {
py::dict<int64_t, std::tuple<py::str, py::str>> build_opcodes() {
    py::dict<int64_t, std::tuple<py::str, py::str>> out;
    for (const auto& [m, ms] : _GROUPS.items()) {
        for (const auto& [mode, op] : ms.items()) {
            out.set(op, std::make_tuple(m, mode));
        }
    }
    return out;
}
}  // namespace

const py::dict<int64_t, std::tuple<py::str, py::str>> OPCODES = build_opcodes();
const py::set<py::str> BRANCHES = {S("BCC"), S("BCS"), S("BEQ"), S("BMI"), S("BNE"), S("BPL"), S("BVC"), S("BVS")};
const py::set<py::str> ENDS = {S("JMP"), S("RTS"), S("RTI"), S("BRK")};  // flow does not fall through

py::str Insn::repr() const { return py::percent("<%04X %s>", at, text(*this)); }

std::optional<Insn> decode(const py::bytes& code, int64_t offset, int64_t pc) {
    if (offset >= py::len(code) || !OPCODES.contains(code[offset])) {
        return std::nullopt;
    }
    int64_t op = code[offset];
    const auto& [mnemonic, mode] = OPCODES.at(op);
    int64_t size = SIZE.at(mode);
    if (offset + size > py::len(code)) {
        return std::nullopt;
    }
    std::optional<int64_t> operand;
    if (size == 2) {
        operand = code[offset + 1];
    } else if (size == 3) {
        operand = code[offset + 1] | (code[offset + 2] << 8);
    }
    std::optional<int64_t> target;
    if (mode == "rel") {
        target = (pc + 2 + ((*operand & 0x80) ? *operand - 256 : *operand)) & 0xFFFF;
    } else if ((mnemonic == "JMP" || mnemonic == "JSR") && mode == "abs") {
        target = operand;
    }
    return Insn(pc, op, mnemonic, mode, operand, size, target);
}

py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    const py::str& m = insn.mnemonic;
    const py::str& mode = insn.mode;
    auto operand = [&]() -> int64_t {
        if (!insn.operand) throw py::TypeError("unsupported operand type(s) for formatting: 'NoneType'");
        return *insn.operand;
    };
    auto addr = [&](int64_t value, int64_t width) -> py::str {
        std::optional<py::str> name = labels.get(value);
        if (name && py::truthy(*name)) {
            return (width == 4 && value < 0x100) ? py::str("a:") + *name : *name;
        }
        return width == 4 ? py::percent("$%04X", value) : py::percent("$%02X", value);
    };
    if (mode == "imp") {
        return m;
    }
    if (mode == "acc") {
        return m + py::str(" a");
    }
    if (mode == "imm") {
        return py::percent("%s #$%02X", m, operand());
    }
    if (mode == "rel") {
        if (!insn.target) throw py::TypeError("unsupported operand type(s) for formatting: 'NoneType'");
        return py::percent("%s %s", m, addr(*insn.target, 4));
    }
    static const py::dict<py::str, py::str> form = {
        {py::str("zp"), py::str("%s")},    {py::str("zpx"), py::str("%s,x")},    {py::str("zpy"), py::str("%s,y")},
        {py::str("abs"), py::str("%s")},   {py::str("abx"), py::str("%s,x")},    {py::str("aby"), py::str("%s,y")},
        {py::str("ind"), py::str("(%s)")}, {py::str("izx"), py::str("(%s,x)")},  {py::str("izy"), py::str("(%s),y")}};
    bool zero_page = mode == "zp" || mode == "zpx" || mode == "zpy" || mode == "izx" || mode == "izy";
    return py::percent("%s %s", m, py::percent(form.at(mode), addr(operand(), zero_page ? 2 : 4)));
}

const py::re::Pattern _LINE = py::re::compile(py::str("^\\s*([A-Za-z]{3})(?:\\s+(.*?))?\\s*(?:;.*)?$"));

py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    std::optional<py::re::Match> m = _LINE.match(line);
    if (!m) {
        throw py::ValueError(py::percent("not an instruction: %r", line));
    }
    py::str mnemonic = m->group(1).upper();
    py::str arg = m->group_opt(2).value_or(py::str("")).strip();
    if (!_GROUPS.contains(mnemonic)) {
        throw py::ValueError(py::percent("unknown instruction %s", mnemonic));
    }
    const ModeTable modes = _GROUPS.at(mnemonic);

    // value(s) -> (the number, whether it was written wide)
    auto value = [&](const py::str& s_in) -> std::tuple<int64_t, bool> {
        py::str s = s_in.strip();
        bool wide = s.startswith(py::str("a:"));
        if (wide) s = s.slice(2, py::len(s));
        if (s.startswith(py::str("$"))) {
            return {py::int_parse(s.slice(1, py::len(s)), 16), wide || py::len(s) > 3};
        }
        if (labels.contains(s)) {
            return {labels.at(s), wide || labels.at(s) > 0xFF};
        }
        int64_t v = py::int_parse(s, 0);
        return {v, wide || v > 0xFF};
    };

    py::str mode;
    std::optional<int64_t> operand;
    py::str low = arg.lower().replace(py::str(" "), py::str(""));
    if (!py::truthy(arg)) {
        mode = py::str("imp");
    } else if (arg.lower() == "a") {
        mode = py::str("acc");
    } else if (arg.startswith(py::str("#"))) {
        mode = py::str("imm");
        operand = std::get<0>(value(arg.slice(1, py::len(arg)).replace(py::str("<"), py::str("")).replace(py::str(">"), py::str(""))));
    } else if (BRANCHES.contains(mnemonic)) {
        int64_t target = std::get<0>(value(arg));
        int64_t d = target - (pc + 2);
        if (!(-128 <= d && d <= 127)) {
            throw py::ValueError(py::percent("branch out of reach: %s", line));
        }
        mode = py::str("rel");
        operand = d & 0xFF;
    } else if (low.startswith(py::str("(")) && low.endswith(py::str(",x)"))) {
        mode = py::str("izx");
        operand = std::get<0>(value(arg.slice(1, rindex_(arg.lower(), py::str(",x")))));
    } else if (low.startswith(py::str("(")) && low.endswith(py::str("),y"))) {
        mode = py::str("izy");
        operand = std::get<0>(value(arg.slice(1, arg.index(py::str(")")))));
    } else if (low.startswith(py::str("("))) {
        mode = py::str("ind");
        operand = std::get<0>(value(arg.slice(1, rindex_(arg, py::str(")")))));
    } else {
        py::str index = (low.endswith(py::str(",x")) || low.endswith(py::str(",y"))) ? low.slice(py::len(low) - 2, py::len(low)) : py::str("");
        auto [v, wide] = value(index != "" ? arg.slice(0, py::len(arg) - 2) : arg);
        py::str zpmode = index == "" ? py::str("zp") : index == ",x" ? py::str("zpx") : py::str("zpy");
        if (!wide && modes.contains(zpmode)) {
            mode = zpmode;
        } else {
            mode = index == "" ? py::str("abs") : index == ",x" ? py::str("abx") : py::str("aby");
        }
        operand = v;
    }
    if (!modes.contains(mode)) {
        throw py::ValueError(py::percent("%s has no %s form", mnemonic, mode));
    }
    int64_t size = SIZE.at(mode);
    std::string out(1, static_cast<char>(modes.at(mode)));
    if (size == 2) {
        out.push_back(static_cast<char>(*operand & 0xFF));
    } else if (size == 3) {
        out.push_back(static_cast<char>(*operand & 0xFF));
        out.push_back(static_cast<char>((*operand >> 8) & 0xFF));
    }
    return py::bytes(out);
}

}  // namespace codebreaker::decompile::m6502
