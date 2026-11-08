// codebreaker/decompile/m68k.cpp -- C++ port of codebreaker.decompile.m68k (see m68k.hpp).
//
// The Python takes `value` from decompile/z80.py; here it is the typed port of that module (z80.hpp).

#include "codebreaker/decompile/m68k.hpp"

namespace codebreaker::decompile::m68k {

const py::dict<int64_t, py::str> SIZES = {{0, py::str("B")}, {1, py::str("W")}, {2, py::str("L")}};
const py::list<py::str> CC = {py::str("T"), py::str("F"), py::str("HI"), py::str("LS"), py::str("CC"), py::str("CS"), py::str("NE"), py::str("EQ"),
                              py::str("VC"), py::str("VS"), py::str("PL"), py::str("MI"), py::str("GE"), py::str("LT"), py::str("GT"), py::str("LE")};
const py::list<py::str> BCC = py::list<py::str>{py::str("RA"), py::str("SR")} + CC.slice(2, py::len(CC));

std::optional<int64_t> _category(int64_t mode, int64_t reg) {
    if (mode == 0) {
        return DATA | ALTERABLE;
    }
    if (mode == 1) {
        return ALTERABLE;
    }
    if (mode == 2 || mode == 5 || mode == 6) {
        return DATA | MEMORY | CONTROL | ALTERABLE;
    }
    if (mode == 3 || mode == 4) {
        return DATA | MEMORY | ALTERABLE;
    }
    if (reg == 0 || reg == 1) {
        return DATA | MEMORY | CONTROL | ALTERABLE;
    }
    if (reg == 2 || reg == 3) {
        return DATA | MEMORY | CONTROL;
    }
    if (reg == 4) {
        return DATA | MEMORY;
    }
    return std::nullopt;
}

bool _ok(int64_t mode, int64_t reg, int64_t need, bool no_an, bool no_imm) {
    std::optional<int64_t> c = _category(mode, reg);
    if (!c.has_value() || (*c & need) != need) {
        return false;
    }
    if (no_an && mode == 1) {
        return false;
    }
    if (no_imm && mode == 7 && reg == 4) {
        return false;
    }
    return true;
}

int64_t Op::num() const {
    if (value_none) {
        throw py::TypeError("unsupported operand type(s) for NoneType");
    }
    return value;
}

bool Op::size_is(const char* s) const {
    const py::str* p = std::get_if<py::str>(&size);
    return p != nullptr && *p == s;
}

py::str Op::key() const {
    if (kind == "D" || kind == "A" || kind == "ind" || kind == "post" || kind == "pre" || kind == "disp") {
        return py::percent("%s%d", kind, reg);
    }
    if (kind == "index") {                  // (the index register and its size are the extension word's)
        return py::percent("index%d", reg);
    }
    if (kind == "pcindex") {
        return py::str("pcindex");
    }
    if (kind == "quick") {
        return py::percent("#%d", num());
    }
    if (kind == "label8") {
        return py::percent("d8:%d", num());
    }
    return kind;
}

py::str Insn::shape() const {
    py::list<py::str> keys;
    for (const Op& o : operands) {
        keys.append(o.key());
    }
    return py::percent("%s%s %s", mnemonic, !size.empty() ? py::str(".") + size : py::str(""), py::join(py::str(","), keys));
}

py::str Insn::repr() const { return py::percent("Insn(%s)", m68k::text(*this)); }

int64_t _Reader::word() {
    if (at + 2 > py::len(code)) {
        throw py::IndexError("");
    }
    int64_t v = code[at] << 8 | code[at + 1];
    at += 2;
    return v;
}

int64_t _Reader::long_() {
    int64_t hi = word() << 16;
    return hi | word();
}

int64_t _s16(int64_t v) { return (v & 0x8000) ? v - 0x10000 : v; }

int64_t _s8(int64_t v) { return (v & 0x80) ? v - 0x100 : v; }

// The operand of effective address (mode, reg), reading its extension words; None if invalid.
std::optional<Op> _ea(_Reader& r, int64_t mode, int64_t reg, const py::str& size, const std::function<int64_t(int64_t)>& pc_of_ext) {
    if (mode == 0) {
        return Op("D", reg);
    }
    if (mode == 1) {
        return Op("A", reg);
    }
    if (mode == 2) {
        return Op("ind", reg);
    }
    if (mode == 3) {
        return Op("post", reg);
    }
    if (mode == 4) {
        return Op("pre", reg);
    }
    if (mode == 5) {
        return Op("disp", reg, _s16(r.word()));
    }
    if (mode == 6 || (mode == 7 && reg == 3)) {
        int64_t ext_at = r.at;
        int64_t ext = r.word();
        if (ext & 0x0700) {                 // the 68000 has only the brief format, bits 8-10 zero
            return std::nullopt;
        }
        XReg xreg(py::str((ext & 0x8000) ? "A" : "D"), (ext >> 12) & 7);
        py::str xsize((ext & 0x0800) ? "L" : "W");
        if (mode == 6) {
            return Op("index", reg, _s8(ext & 0xFF), xreg, xsize);
        }
        return Op("pcindex", 0, (pc_of_ext(ext_at) + _s8(ext & 0xFF)) & 0xFFFFFFFF, xreg, xsize);
    }
    if (reg == 0) {
        return Op("absw", 0, _s16(r.word()) & 0xFFFFFFFF);
    }
    if (reg == 1) {
        return Op("absl", 0, r.long_());
    }
    if (reg == 2) {
        int64_t ext_at = r.at;
        int64_t ext = pc_of_ext(ext_at);
        return Op("pcdisp", 0, (ext + _s16(r.word())) & 0xFFFFFFFF);
    }
    if (reg == 4) {
        if (size == "B") {
            int64_t v = r.word();
            if (v & 0xFF00) {
                return std::nullopt;
            }
            Op o("imm", 0, v);
            o.size = py::str("B");
            return o;
        }
        if (size == "W") {
            Op o("imm", 0, r.word());
            o.size = py::str("W");
            return o;
        }
        Op o("imm", 0, r.long_());
        o.size = py::str("L");
        return o;
    }
    return std::nullopt;
}

Op _Ctx::ea(int64_t m, int64_t rg, const py::str& size, int64_t need, bool no_an, bool no_imm) {
    if (!_ok(m, rg, need, no_an, no_imm)) {
        throw _Not();
    }
    std::optional<Op> o = _ea(r, m, rg, size, [this](int64_t ext_at) { return ext_pc(ext_at); });
    if (!o.has_value()) {
        throw _Not();
    }
    return *o;
}

Insn _Ctx::done(const py::str& mn, const py::str& size, const py::list<Op>& operands, const py::str& flow, const std::optional<int64_t>& target) {
    Insn insn(mn, size, operands, r.at - at, pc, r.code.slice(at, r.at), w);
    insn.flow = flow;
    insn.target = target;
    return insn;
}

// The instruction at code[at:] (running at address `pc`), or None if it is not one (or code
// ends inside it).
std::optional<Insn> decode(const py::bytes& code, int64_t at, int64_t pc) {
    try {
        return _decode(code, at, pc);
    } catch (const py::IndexError&) {
        return std::nullopt;
    }
}

std::optional<Insn> _decode(const py::bytes& code, int64_t at, int64_t pc) {
    _Ctx c(code, at, pc);
    _Reader& r = c.r;
    int64_t w = r.word();
    c.w = w;
    int64_t line = w >> 12;
    int64_t mode = (w >> 3) & 7, reg = w & 7;
    int64_t hi = (w >> 9) & 7;
    try {
        if (line == 0) {
            if (w == 0x003C || w == 0x007C || w == 0x023C || w == 0x027C || w == 0x0A3C || w == 0x0A7C) {
                int64_t hb = w >> 8;
                py::str mn(hb == 0x00 ? "ORI" : hb == 0x02 ? "ANDI" : "EORI");
                bool to_sr = (w & 0x40) != 0;
                int64_t v = r.word();
                if (!to_sr && (v & 0xFF00)) {
                    throw _Not();
                }
                Op imm("imm", 0, v);
                imm.size = py::str(to_sr ? "W" : "B");
                return c.done(mn, to_sr ? "W" : "B", {imm, Op(to_sr ? "sr" : "ccr")});
            }
            if (w & 0x0100) {
                if (mode == 1) {                          // MOVEP
                    int64_t opm = (w >> 6) & 3;
                    py::str size((opm & 1) ? "L" : "W");
                    Op d("disp", reg, _s16(r.word()));
                    py::list<Op> pair = opm < 2 ? py::list<Op>{d, Op("D", hi)} : py::list<Op>{Op("D", hi), d};
                    return c.done("MOVEP", size, pair);
                }
                int64_t t = (w >> 6) & 3;
                py::str mn = py::list<py::str>{py::str("BTST"), py::str("BCHG"), py::str("BCLR"), py::str("BSET")}[t];
                int64_t need = t == 0 ? DATA : DATA | ALTERABLE;
                Op dst = c.ea(mode, reg, "B", need);
                return c.done(mn, mode == 0 ? "L" : "B", {Op("D", hi), dst});
            }
            int64_t top = (w >> 9) & 7;
            if (top == 4) {                               // static bit operations
                int64_t t = (w >> 6) & 3;
                py::str mn = py::list<py::str>{py::str("BTST"), py::str("BCHG"), py::str("BCLR"), py::str("BSET")}[t];
                int64_t v = r.word();
                if (v & 0xFF00) {
                    throw _Not();
                }
                int64_t need = t == 0 ? DATA : DATA | ALTERABLE;
                Op dst = c.ea(mode, reg, "B", need, false, true);
                Op imm("imm", 0, v);
                imm.size = py::str("B");
                return c.done(mn, mode == 0 ? "L" : "B", {imm, dst});
            }
            const char* iname = top == 0 ? "ORI" : top == 1 ? "ANDI" : top == 2 ? "SUBI" : top == 3 ? "ADDI" : top == 5 ? "EORI" : top == 6 ? "CMPI" : nullptr;
            if (iname != nullptr && ((w >> 6) & 3) != 3) {
                py::str size = SIZES.at((w >> 6) & 3);
                std::optional<Op> imm = _ea(r, 7, 4, size, [&c](int64_t ext_at) { return c.ext_pc(ext_at); });
                if (!imm.has_value()) {
                    throw _Not();
                }
                Op dst = c.ea(mode, reg, size, DATA | ALTERABLE);
                return c.done(iname, size, {*imm, dst});
            }
            throw _Not();
        }
        if (line == 1 || line == 2 || line == 3) {
            py::str size(line == 1 ? "B" : line == 3 ? "W" : "L");
            Op src = c.ea(mode, reg, size, 0, size == "B");
            int64_t dmode = (w >> 6) & 7, dreg = hi;
            if (dmode == 1) {
                if (size == "B") {
                    throw _Not();
                }
                return c.done("MOVEA", size, {src, Op("A", dreg)});
            }
            Op dst = c.ea(dmode, dreg, size, DATA | ALTERABLE);
            return c.done("MOVE", size, {src, dst});
        }
        if (line == 4) {
            return _line4(c, mode, reg, hi);
        }
        if (line == 5) {
            int64_t ss = (w >> 6) & 3;
            if (ss == 3) {
                py::str cond = CC[(w >> 8) & 0xF];
                if (mode == 1) {
                    int64_t disp_at = r.at;
                    int64_t base = c.ext_pc(disp_at);
                    int64_t t = (base + _s16(r.word())) & 0xFFFFFFFF;
                    Op lab("label", 0, t);
                    return c.done(py::str("DB") + cond, "", {Op("D", reg), lab}, "branch", t);
                }
                Op dst = c.ea(mode, reg, "B", DATA | ALTERABLE);
                return c.done(py::str("S") + cond, "", {dst});
            }
            int64_t q = hi ? hi : 8;
            py::str size = SIZES.at(ss);
            if (mode == 1 && size == "B") {
                throw _Not();
            }
            Op dst = c.ea(mode, reg, size, ALTERABLE);
            return c.done((w & 0x0100) ? "SUBQ" : "ADDQ", size, {Op("quick", 0, q), dst});
        }
        if (line == 6) {
            int64_t cc = (w >> 8) & 0xF;
            int64_t d8 = w & 0xFF;
            py::str mn = py::str("B") + BCC[cc];
            Op op;
            py::str size;
            int64_t t;
            if (d8 == 0) {
                int64_t disp_at = r.at;
                int64_t base = c.ext_pc(disp_at);
                t = (base + _s16(r.word())) & 0xFFFFFFFF;
                op = Op("label", 0, t);
                size = py::str("W");
            } else {
                t = (pc + 2 + _s8(d8)) & 0xFFFFFFFF;
                op = Op("label8", 0, d8);
                op.size = t;
                size = py::str("S");
            }
            const char* flow = cc == 0 ? "jump" : cc == 1 ? "call" : "branch";
            return c.done(mn, size, {op}, flow, t);
        }
        if (line == 7) {
            if (w & 0x0100) {
                throw _Not();
            }
            return c.done("MOVEQ", "", {Op("quick", 0, _s8(w & 0xFF)), Op("D", hi)});
        }
        if (line == 8 || line == 9 || line == 0xB || line == 0xC || line == 0xD) {
            return _arith(c, line, mode, reg, hi);
        }
        if (line == 0xE) {
            int64_t ss = (w >> 6) & 3;
            py::list<py::str> names = {py::str("AS"), py::str("LS"), py::str("ROX"), py::str("RO")};
            if (ss == 3) {
                if (hi & 4) {
                    throw _Not();
                }
                Op dst = c.ea(mode, reg, "W", MEMORY | ALTERABLE);
                return c.done(names[hi & 3] + ((w & 0x0100) ? "L" : "R"), "W", {dst});
            }
            int64_t t = (w >> 3) & 3;
            Op count = (w & 0x20) ? Op("D", hi) : Op("quick", 0, hi ? hi : 8);
            return c.done(names[t] + ((w & 0x0100) ? "L" : "R"), SIZES.at(ss), {count, Op("D", reg)});
        }
        return std::nullopt;                                    // line A and line F: no 68000 instruction
    } catch (const _Not&) {
        return std::nullopt;
    }
}

Insn _line4(_Ctx& c, int64_t mode, int64_t reg, int64_t hi) {
    int64_t w = c.w;
    _Reader& r = c.r;
    if (w == 0x4AFC) {
        return c.done("ILLEGAL", "", {});
    }
    if ((w & 0xFFF0) == 0x4E40) {
        return c.done("TRAP", "", {Op("quick", 0, w & 0xF)}, "call");
    }
    if ((w & 0xFFF8) == 0x4E50) {
        Op imm("imm", 0, r.word());
        imm.size = py::str("W");
        return c.done("LINK", "", {Op("A", reg), imm});
    }
    if ((w & 0xFFF8) == 0x4E58) {
        return c.done("UNLK", "", {Op("A", reg)});
    }
    if ((w & 0xFFF8) == 0x4E60) {
        return c.done("MOVE", "L", {Op("A", reg), Op("usp")});
    }
    if ((w & 0xFFF8) == 0x4E68) {
        return c.done("MOVE", "L", {Op("usp"), Op("A", reg)});
    }
    const char* simple = w == 0x4E70 ? "RESET" : w == 0x4E71 ? "NOP" : w == 0x4E73 ? "RTE" : w == 0x4E75 ? "RTS" : w == 0x4E76 ? "TRAPV" : w == 0x4E77 ? "RTR" : nullptr;
    if (simple != nullptr) {
        return c.done(simple, "", {}, (w == 0x4E73 || w == 0x4E75 || w == 0x4E77) ? "ret" : "");
    }
    if (w == 0x4E72) {
        Op imm("imm", 0, r.word());
        imm.size = py::str("W");
        return c.done("STOP", "", {imm}, "halt");
    }
    if ((w & 0xFFC0) == 0x4E80) {
        Op dst = c.ea(mode, reg, "L", CONTROL);
        return c.done("JSR", "", {dst}, "call", _absolute(dst));
    }
    if ((w & 0xFFC0) == 0x4EC0) {
        Op dst = c.ea(mode, reg, "L", CONTROL);
        std::optional<int64_t> t = _absolute(dst);
        return c.done("JMP", "", {dst}, t.has_value() ? "jump" : "indirect", t);
    }
    if ((w & 0xF1C0) == 0x41C0) {
        Op src = c.ea(mode, reg, "L", CONTROL);
        return c.done("LEA", "", {src, Op("A", hi)});
    }
    if ((w & 0xF1C0) == 0x4180) {
        Op src = c.ea(mode, reg, "W", DATA);
        return c.done("CHK", "W", {src, Op("D", hi)});
    }
    if ((w & 0xFFF8) == 0x4840) {
        return c.done("SWAP", "", {Op("D", reg)});
    }
    if ((w & 0xFFF8) == 0x4880) {
        return c.done("EXT", "W", {Op("D", reg)});
    }
    if ((w & 0xFFF8) == 0x48C0) {
        return c.done("EXT", "L", {Op("D", reg)});
    }
    if ((w & 0xFFC0) == 0x4840) {
        return c.done("PEA", "", {c.ea(mode, reg, "L", CONTROL)});
    }
    if ((w & 0xFB80) == 0x4880) {                              // MOVEM
        py::str size((w & 0x40) ? "L" : "W");
        int64_t mask = r.word();
        if (w & 0x0400) {                                      // memory to registers
            if (!(_ok(mode, reg, CONTROL) || mode == 3)) {
                throw _Not();
            }
            Op src = mode == 3 ? c.ea(mode, reg, size, 0) : c.ea(mode, reg, size, CONTROL);
            return c.done("MOVEM", size, {src, Op("regs", 0, mask)});
        }
        if (!(_ok(mode, reg, CONTROL | ALTERABLE) || mode == 4)) {
            throw _Not();
        }
        Op dst = mode == 4 ? c.ea(mode, reg, size, 0) : c.ea(mode, reg, size, CONTROL | ALTERABLE);
        return c.done("MOVEM", size, {Op("regs", 0, mask, std::nullopt, py::str(mode == 4 ? "pre" : "")), dst});
    }
    if ((w & 0xFFC0) == 0x4800) {
        return c.done("NBCD", "", {c.ea(mode, reg, "B", DATA | ALTERABLE)});
    }
    if ((w & 0xFFC0) == 0x40C0) {
        Op dst = c.ea(mode, reg, "W", DATA | ALTERABLE);
        return c.done("MOVE", "W", {Op("sr"), dst});
    }
    if ((w & 0xFFC0) == 0x44C0) {
        Op src = c.ea(mode, reg, "W", DATA);
        return c.done("MOVE", "W", {src, Op("ccr")});
    }
    if ((w & 0xFFC0) == 0x46C0) {
        Op src = c.ea(mode, reg, "W", DATA);
        return c.done("MOVE", "W", {src, Op("sr")});
    }
    if ((w & 0xFFC0) == 0x4AC0) {
        return c.done("TAS", "", {c.ea(mode, reg, "B", DATA | ALTERABLE)});
    }
    int64_t ss = (w >> 6) & 3;
    if (ss != 3) {
        int64_t hb = w >> 8;
        const char* mn = hb == 0x40 ? "NEGX" : hb == 0x42 ? "CLR" : hb == 0x44 ? "NEG" : hb == 0x46 ? "NOT" : hb == 0x4A ? "TST" : nullptr;
        if (mn != nullptr) {
            py::str size = SIZES.at(ss);
            return c.done(mn, size, {c.ea(mode, reg, size, DATA | ALTERABLE)});
        }
    }
    throw _Not();
}

std::optional<int64_t> _absolute(const Op& op) {
    if (op.kind == "absw" || op.kind == "absl" || op.kind == "pcdisp") {
        return op.num();
    }
    return std::nullopt;
}

Insn _arith(_Ctx& c, int64_t line, int64_t mode, int64_t reg, int64_t hi) {
    int64_t w = c.w;
    int64_t opm = (w >> 6) & 7;
    int64_t ss = opm & 3;
    if ((line == 8 || line == 0xC) && ss == 3) {               // DIVU DIVS / MULU MULS
        py::str mn = py::str(line == 8 ? "DIV" : "MUL") + ((w & 0x0100) ? "S" : "U");
        Op src = c.ea(mode, reg, "W", DATA);
        return c.done(mn, "W", {src, Op("D", hi)});
    }
    if ((line == 9 || line == 0xB || line == 0xD) && ss == 3) {  // SUBA CMPA ADDA
        py::str size((w & 0x0100) ? "L" : "W");
        py::str mn(line == 9 ? "SUBA" : line == 0xB ? "CMPA" : "ADDA");
        Op src = c.ea(mode, reg, size, 0);
        return c.done(mn, size, {src, Op("A", hi)});
    }
    py::str size = SIZES.at(ss);
    bool to_ea = (w & 0x0100) != 0;
    if (to_ea && (mode == 0 || mode == 1)) {
        if ((line == 8 || line == 0xC) && ss == 0) {           // SBCD ABCD
            py::str mn(line == 8 ? "SBCD" : "ABCD");
            py::list<Op> pair = mode ? py::list<Op>{Op("pre", reg), Op("pre", hi)} : py::list<Op>{Op("D", reg), Op("D", hi)};
            return c.done(mn, "", pair);
        }
        if (line == 0xC) {                                     // EXG
            const char* k0 = nullptr;
            const char* k1 = nullptr;
            if (ss == 1 && mode == 0) { k0 = "D"; k1 = "D"; }
            else if (ss == 1 && mode == 1) { k0 = "A"; k1 = "A"; }
            else if (ss == 2 && mode == 1) { k0 = "D"; k1 = "A"; }
            if (k0 == nullptr) {
                throw _Not();
            }
            return c.done("EXG", "", {Op(k0, hi), Op(k1, reg)});
        }
        if (line == 9 || line == 0xD) {                        // SUBX ADDX
            py::str mn(line == 9 ? "SUBX" : "ADDX");
            py::list<Op> pair = mode ? py::list<Op>{Op("pre", reg), Op("pre", hi)} : py::list<Op>{Op("D", reg), Op("D", hi)};
            return c.done(mn, size, pair);
        }
        if (line == 0xB && mode == 1) {                        // CMPM
            return c.done("CMPM", size, {Op("post", reg), Op("post", hi)});
        }
        if (line == 0xB) {                                     // EOR Dn,Dn
            return c.done("EOR", size, {Op("D", hi), Op("D", reg)});
        }
        throw _Not();
    }
    py::str mn(line == 8 ? "OR" : line == 9 ? "SUB" : line == 0xB ? (!to_ea ? "CMP" : "EOR") : line == 0xC ? "AND" : "ADD");
    if (to_ea) {
        int64_t need = mn == "EOR" ? DATA | ALTERABLE : MEMORY | ALTERABLE;
        Op dst = c.ea(mode, reg, size, need);
        return c.done(mn, size, {Op("D", hi), dst});
    }
    bool no_an = mn == "OR" || mn == "AND" || size == "B";
    Op src = c.ea(mode, reg, size, 0, no_an);
    return c.done(mn, size, {src, Op("D", hi)});
}

// ---- text ------------------------------------------------------------------------------------------

py::str _reg(const py::str& kind, int64_t n) {
    if (kind == "A" && n == 7) {
        return py::str("SP");
    }
    return py::percent("%s%d", kind, n);
}

py::str _hex(int64_t v, int64_t digits) {
    if (v < 0) {
        return py::percent("-$%X", py::checked_neg(v));
    }
    if (digits == 0) {
        return py::percent("$%X", v);
    }
    return py::percent(py::str("$%0") + py::to_str(digits) + "X", v);
}

namespace {
// int("{:016b}".format(mask)[::-1], 2): the binary digits (at least 16) read backwards.
int64_t reversed_bits(int64_t mask) {
    if (mask < 0) {
        throw py::ValueError("invalid literal for int() with base 2");
    }
    std::string bits;
    for (int64_t m = mask; m > 0; m >>= 1) {
        bits.push_back(static_cast<char>('0' + (m & 1)));      // least significant digit first: the reversed string
    }
    while (bits.size() < 16) {
        bits.push_back('0');
    }
    int64_t rev = 0;
    for (char ch : bits) {
        rev = py::checked_add(py::checked_mul(rev, 2), ch - '0');
    }
    return rev;
}
}  // namespace

py::str _regs(int64_t mask, bool predec) {
    if (predec) {
        mask = reversed_bits(mask);
    }
    py::list<py::str> names;
    for (int64_t i = 0; i < 8; ++i) {
        names.append(py::percent("D%d", i));
    }
    for (int64_t i = 0; i < 8; ++i) {
        names.append(py::percent("A%d", i));
    }
    py::list<py::str> parts;
    int64_t i = 0;
    while (i < 16) {
        if (mask & (int64_t(1) << i)) {
            int64_t j = i;
            while (j + 1 < 16 && (mask & (int64_t(1) << (j + 1))) && (j + 1) / 8 == i / 8) {
                j += 1;
            }
            parts.append(i == j ? names[i] : py::percent("%s-%s", names[i], names[j]));
            i = j + 1;
        } else {
            i += 1;
        }
    }
    py::list<py::str> shown;
    for (const py::str& p : parts) {
        shown.append(p.replace(py::str("A7"), py::str("SP")));
    }
    py::str joined = py::join(py::str("/"), shown);
    return !joined.empty() ? joined : py::str("0");
}

namespace {
// `labels.get(key) or fallback`
py::str label_or(const py::dict<int64_t, py::str>& labels, int64_t key, const py::str& fallback) {
    if (labels.contains(key)) {
        const py::str& name = labels.at(key);
        if (!name.empty()) {
            return name;
        }
    }
    return fallback;
}
py::str xreg_text(const std::optional<XReg>& x) {
    if (!x.has_value()) {
        throw py::TypeError("Op.xreg is None");
    }
    return _reg(std::get<0>(*x), std::get<1>(*x));
}
}  // namespace

py::str operand_text(const Op& o, const py::dict<int64_t, py::str>& labels) {
    const py::str& k = o.kind;
    if (k == "D" || k == "A") {
        return _reg(k, o.reg);
    }
    if (k == "ind") {
        return py::percent("(%s)", _reg(py::str("A"), o.reg));
    }
    if (k == "post") {
        return py::percent("(%s)+", _reg(py::str("A"), o.reg));
    }
    if (k == "pre") {
        return py::percent("-(%s)", _reg(py::str("A"), o.reg));
    }
    if (k == "disp") {
        return py::percent("%s(%s)", _hex(o.num()), _reg(py::str("A"), o.reg));
    }
    if (k == "index") {
        return py::percent("%s(%s,%s.%s)", _hex(o.num()), _reg(py::str("A"), o.reg), xreg_text(o.xreg), o.xsize);
    }
    if (k == "absw") {
        int64_t v = o.num();
        return py::percent("(%s).W", label_or(labels, v, _hex(v < 0x8000 ? (v & 0xFFFF) : v, 4)));
    }
    if (k == "absl") {
        int64_t v = o.num();
        return py::percent("(%s).L", label_or(labels, v, _hex(v, 8)));
    }
    if (k == "pcdisp") {
        int64_t v = o.num();
        return py::percent("%s(PC)", label_or(labels, v, _hex(v)));
    }
    if (k == "pcindex") {
        int64_t v = o.num();
        return py::percent("%s(PC,%s.%s)", label_or(labels, v, _hex(v)), xreg_text(o.xreg), o.xsize);
    }
    if (k == "imm") {
        int64_t v = o.num();
        return py::percent("#%s", o.size_is("L") && labels.contains(v) ? labels.at(v) : _hex(v));
    }
    if (k == "quick") {
        return py::percent("#%d", o.num());
    }
    if (k == "label") {
        int64_t v = o.num();
        return label_or(labels, v, _hex(v));
    }
    if (k == "label8") {
        const int64_t* target = std::get_if<int64_t>(&o.size);
        if (target == nullptr) {
            throw py::TypeError("label8 without a target");
        }
        return label_or(labels, *target, _hex(*target));
    }
    if (k == "regs") {
        return _regs(o.num(), o.xsize == "pre");
    }
    if (k == "ccr") {
        return py::str("CCR");
    }
    if (k == "sr") {
        return py::str("SR");
    }
    if (k == "usp") {
        return py::str("USP");
    }
    throw py::KeyError(k);
}

// The instruction as source; `labels` {address: name} names jump targets and absolute words.
py::str text(const Insn& insn, const py::dict<int64_t, py::str>& labels) {
    py::str m = insn.mnemonic + (!insn.size.empty() ? py::str(".") + insn.size : py::str(""));
    if (insn.operands.empty()) {
        return m;
    }
    py::list<py::str> shown;
    for (const Op& o : insn.operands) {
        shown.append(operand_text(o, labels));
    }
    return py::percent("%s %s", m, py::join(py::str(","), shown));
}

// ---- the assembler -----------------------------------------------------------------------------------

static std::optional<py::dict<py::str, int64_t>> SHAPES_;

const py::dict<py::str, int64_t>& _shapes() {
    if (!SHAPES_.has_value()) {
        py::dict<py::str, int64_t> shapes;
        py::bytes filler(std::string(8, '\0'));
        for (int64_t w = 0; w < 0x10000; ++w) {
            std::string two;
            two.push_back(static_cast<char>(w >> 8));
            two.push_back(static_cast<char>(w & 0xFF));
            std::optional<Insn> insn = decode(py::bytes(two) + filler, 0, 0);
            if (insn.has_value()) {
                shapes.setdefault(insn->shape(), w);
            }
        }
        SHAPES_ = shapes;
    }
    return *SHAPES_;
}

int64_t _value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc) {
    return z80::value(expr.strip() == "*" ? expr.replace(py::str("*"), py::str("$")) : expr, labels, pc);
}

std::optional<XReg> _parse_reg(const py::str& s_in) {
    py::str s = s_in.strip().upper();
    if (s == "SP") {
        return XReg(py::str("A"), 7);
    }
    static const py::re::Pattern reg_re = py::re::compile(py::str("^([DA])([0-7])$"));
    std::optional<py::re::Match> m = reg_re.match(s);
    if (m.has_value()) {
        return XReg(m->group(1), py::int_from_str(m->group(2)));
    }
    return std::nullopt;
}

std::optional<int64_t> _parse_regs(const py::str& s) {
    int64_t mask = 0;
    for (const py::str& part : py::split(s.upper(), py::str("/"))) {
        auto [a, sep, b] = part.partition(py::str("-"));
        std::optional<XReg> ra = _parse_reg(a);
        std::optional<XReg> rb = _parse_reg(!b.empty() ? b : a);
        if (!ra.has_value() || !rb.has_value()) {
            return std::nullopt;
        }
        int64_t lo = std::get<1>(*ra) + (std::get<0>(*ra) == "A" ? 8 : 0);
        int64_t hi = std::get<1>(*rb) + (std::get<0>(*rb) == "A" ? 8 : 0) + 1;
        for (int64_t i = lo; i < hi; ++i) {
            mask |= int64_t(1) << i;
        }
    }
    return mask;
}

namespace {
[[noreturn]] void none_subscript() { throw py::TypeError("'NoneType' object is not subscriptable"); }
}  // namespace

// -> Op for one operand's text.
Op _parse_operand(const py::str& s, const py::dict<py::str, int64_t>& labels, int64_t pc, const py::str& size) {
    py::str t = s.strip();
    py::str u = t.upper();
    if (u == "CCR" || u == "SR" || u == "USP") {
        return Op(u.lower());
    }
    std::optional<XReg> rr = _parse_reg(t);
    if (rr.has_value()) {
        return Op(std::get<0>(*rr), std::get<1>(*rr));
    }
    if (t.startswith(py::str("#"))) {
        Op o("imm", 0, _value(t.slice_from(1), labels, pc));
        o.size = size;
        return o;
    }
    static const py::re::Pattern post_re = py::re::compile(py::str(R"re(^\((\w+)\)(\+?)$)re"));
    static const py::re::Pattern pre_re = py::re::compile(py::str(R"re(^-\((\w+)\)$)re"));
    static const py::re::Pattern index_re = py::re::compile(py::str(R"re(^(.*)\((\w+)\s*,\s*(\w+)(?:\.([WL]))?\)$)re"), py::re::I);
    static const py::re::Pattern disp_re = py::re::compile(py::str(R"re(^(.*)\((\w+)\)$)re"));
    static const py::re::Pattern abs_paren_re = py::re::compile(py::str(R"re(^\((.+)\)\.([WL])$)re"), py::re::I);
    static const py::re::Pattern abs_re = py::re::compile(py::str(R"re(^(.+)\.([WL])$)re"), py::re::I);
    static const py::re::Pattern range_re = py::re::compile(py::str("^[DA][0-7]-[DA][0-7]$"));
    std::optional<py::re::Match> m = post_re.match(t);
    if (m.has_value()) {
        std::optional<XReg> a = _parse_reg(m->group(1));
        if (a.has_value() && std::get<0>(*a) == "A") {
            return Op(!m->group(2).empty() ? "post" : "ind", std::get<1>(*a));
        }
    }
    m = pre_re.match(t);
    if (m.has_value()) {
        std::optional<XReg> a = _parse_reg(m->group(1));
        if (a.has_value()) {
            return Op("pre", std::get<1>(*a));
        }
    }
    m = index_re.match(t);
    if (m.has_value()) {
        py::str base = m->group(2).upper();
        std::optional<XReg> x = _parse_reg(m->group(3));
        int64_t disp = !m->group(1).strip().empty() ? _value(m->group(1), labels, pc) : 0;
        std::optional<py::str> g4 = m->group_opt(4);
        py::str xs = (g4.has_value() && !g4->empty() ? *g4 : py::str("W")).upper();
        if (base == "PC") {
            return Op("pcindex", 0, disp, x, xs);
        }
        std::optional<XReg> b = _parse_reg(base);
        if (!b.has_value()) {
            none_subscript();
        }
        return Op("index", std::get<1>(*b), disp, x, xs);
    }
    m = disp_re.match(t);
    if (m.has_value() && !m->group(1).strip().empty()) {
        bool ok = m->group(2).upper() == "PC";
        if (!ok) {
            ok = _parse_reg(m->group(2)).has_value();
        }
        if (ok) {
            int64_t v = _value(m->group(1), labels, pc);
            if (m->group(2).upper() == "PC") {
                return Op("pcdisp", 0, v);
            }
            std::optional<XReg> b = _parse_reg(m->group(2));
            if (!b.has_value()) {
                none_subscript();
            }
            return Op("disp", std::get<1>(*b), v);
        }
    }
    m = abs_paren_re.match(t);
    if (!m.has_value()) {
        m = abs_re.match(t);
    }
    if (m.has_value()) {
        int64_t v = _value(m->group(1), labels, pc);
        return Op(m->group(2).upper() == "W" ? "absw" : "absl", 0, v);
    }
    if (t.contains(py::str("/")) || range_re.match(u).has_value()) {
        std::optional<int64_t> mask = _parse_regs(t);
        Op o("regs", 0, mask.has_value() ? *mask : 0);
        o.value_none = !mask.has_value();
        return o;
    }
    return Op("expr", 0, _value(t, labels, pc));
}

py::list<py::str> _split(const py::str& s) {
    py::list<py::str> out;
    int64_t depth = 0;
    py::str cur = "";
    for (const py::str& c : s) {
        if (c == "(") {
            depth += 1;
        }
        if (c == ")") {
            depth -= 1;
        }
        if (c == "," && depth == 0) {
            out.append(cur);
            cur = "";
        } else {
            cur += c;
        }
    }
    if (!cur.strip().empty()) {
        out.append(cur);
    }
    return out;
}

// The bytes of one instruction (Motorola syntax); KeyError for an unknown label.
py::bytes assemble(const py::str& line_in, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    py::str line = line_in.strip();
    auto [head, sep, rest] = line.partition(py::str(" "));
    auto [mn, sep2, size] = head.upper().partition(py::str("."));
    py::list<Op> ops;
    if (!rest.strip().empty()) {
        for (const py::str& o : _split(rest)) {
            ops.append(_parse_operand(o, labels, pc, !size.empty() ? size : py::str("W")));
        }
    }
    const py::dict<py::str, int64_t>& shapes = _shapes();
    for (const Candidate& cand : _candidates(mn, size, ops, pc)) {
        const py::str& cmn = std::get<0>(cand);
        const py::str& csize = std::get<1>(cand);
        const py::list<Op>& cops = std::get<2>(cand);
        py::list<py::str> keys;
        for (const Op& o : cops) {
            keys.append(o.key());
        }
        py::str key = py::percent("%s%s %s", cmn, !csize.empty() ? py::str(".") + csize : py::str(""), py::join(py::str(","), keys));
        if (shapes.contains(key)) {
            return _encode(shapes.at(key), cmn, csize, cops, pc);
        }
    }
    throw py::ValueError(py::percent("not an instruction: %r", line));
}

// The ways a parsed line can be read: branches by their size, values as absolute words or longs,
// quick values, and so on.
py::list<Candidate> _candidates(const py::str& mn, const py::str& size, const py::list<Op>& ops_in, int64_t pc) {
    py::list<Candidate> out;
    py::list<Op> ops = ops_in;
    bool branch = false;
    for (const py::str& c : BCC) {
        if (mn == py::str("B") + c) {
            branch = true;
        }
    }
    if (branch) {
        int64_t target = ops[0].num();
        int64_t d = _wrap(py::checked_sub(target, pc + 2));
        if (size == "S" || size == "B") {              // a short branch as written: never made long
            if (!(-128 <= d && d <= 127) || d == 0) {
                throw py::ValueError("short branch out of range");
            }
            Op o("label8", 0, d & 0xFF);
            o.size = target;
            out.append(Candidate(mn, py::str("S"), py::list<Op>{o}));
            return out;
        }
        if (size == "" && -128 <= d && d <= 127 && d != 0) {
            Op o("label8", 0, d & 0xFF);
            o.size = target;
            out.append(Candidate(mn, py::str("S"), py::list<Op>{o}));
            return out;
        }
        out.append(Candidate(mn, py::str("W"), py::list<Op>{Op("label", 0, target)}));
        return out;
    }
    if (mn.startswith(py::str("DB"))) {
        Op first = ops[0];
        Op second = ops[1];
        Op lab("label", 0, second.value);
        lab.value_none = second.value_none;
        out.append(Candidate(mn, py::str(""), py::list<Op>{first, lab}));
        return out;
    }
    if (mn == "JMP" || mn == "JSR") {
        Op o = ops[0];
        if (o.kind == "expr") {
            out.append(Candidate(mn, py::str(""), py::list<Op>{Op("absl", 0, o.num())}));
            return out;
        }
    }
    if (mn == "MOVEM") {
        py::list<Op> rebuilt;
        for (int64_t i = 0; i < py::len(ops); ++i) {
            const Op& o = ops[i];
            bool plain = o.kind == "D" || o.kind == "A" || o.kind == "regs";
            if (plain) {
                const py::str& last = ops[-1].kind;
                bool last_plain = last == "D" || last == "A" || last == "regs";
                if (last_plain ? i == 1 : i == 0) {
                    Op n("regs");
                    if (o.kind == "regs") {
                        n.value = o.value;
                        n.value_none = o.value_none;
                    } else {
                        n.value = int64_t(1) << (o.reg + (o.kind == "A" ? 8 : 0));
                    }
                    n.xsize = ops[-1].kind == "pre" && i == 0 ? py::str("pre") : py::str("");
                    rebuilt.append(n);
                    continue;
                }
            }
            rebuilt.append(o);
        }
        ops = rebuilt;
        if (ops[0].kind == "regs" && ops[1].kind == "pre") {
            ops[0].value = reversed_bits(ops[0].num());
        }
        out.append(Candidate(mn, size, ops));
        return out;
    }
    // quick forms take the value in the opcode word
    bool shift = mn.slice(0, 2) == "AS" || mn.slice(0, 2) == "LS" || mn.slice(0, 2) == "RO";
    if (mn == "ADDQ" || mn == "SUBQ" || mn == "MOVEQ" || mn == "TRAP" || (shift && !ops.empty() && ops[0].kind == "imm")) {
        py::list<Op> q;
        for (int64_t i = 0; i < py::len(ops); ++i) {
            const Op& o = ops[i];
            if (o.kind == "imm" && i == 0) {
                q.append(Op("quick", 0, mn == "MOVEQ" ? _s8(o.num() & 0xFF) : o.num()));
            } else {
                q.append(o);
            }
        }
        out.append(Candidate(mn, mn == "MOVEQ" || mn == "TRAP" ? py::str("") : size, q));
        return out;
    }
    // a bare value is an absolute address: word if it fits, else long
    py::list<py::list<Op>> variants = {py::list<Op>()};
    for (const Op& o : ops) {
        if (o.kind == "expr") {
            int64_t v = o.num() & 0xFFFFFFFF;
            py::list<Op> choices = {Op("absl", 0, v)};
            if (v <= 0x7FFF || v >= 0xFFFF8000) {
                choices.insert(0, Op("absw", 0, v));
            }
            py::list<py::list<Op>> next;
            for (const py::list<Op>& x : variants) {
                for (const Op& ch : choices) {
                    next.append(x + py::list<Op>{ch});
                }
            }
            variants = next;
        } else {
            py::list<py::list<Op>> next;
            for (const py::list<Op>& x : variants) {
                next.append(x + py::list<Op>{o});
            }
            variants = next;
        }
    }
    for (const py::list<Op>& v : variants) {
        out.append(Candidate(mn, size, v));
        if ((mn == "MOVE" || mn == "ADD" || mn == "SUB" || mn == "CMP") && py::len(v) == 2 && v[1].kind == "A") {
            out.append(Candidate(mn + "A", size, v));
        }
        if ((mn == "ADD" || mn == "SUB" || mn == "AND" || mn == "OR" || mn == "EOR" || mn == "CMP") && !v.empty() && v[0].kind == "imm") {
            out.append(Candidate(mn + "I", size, v));
        }
        if ((mn == "BTST" || mn == "BCHG" || mn == "BCLR" || mn == "BSET") && py::len(v) == 2) {
            out.append(Candidate(mn, v[1].kind == "D" ? py::str("L") : py::str("B"), v));
        }
        if ((mn == "MOVE" || mn == "ORI" || mn == "ANDI" || mn == "EORI") && py::len(v) == 2 && v[1].kind == "ccr") {
            out.append(Candidate(mn, mn != "MOVE" ? py::str("B") : py::str("W"), py::list<Op>{v[0], v[1]}));
        }
        if ((mn == "MOVE" || mn == "ORI" || mn == "ANDI" || mn == "EORI") && py::len(v) == 2 && (v[0].kind == "sr" || v[1].kind == "sr")) {
            out.append(Candidate(mn, py::str("W"), v));
        }
    }
    return out;
}

py::bytes _encode(int64_t word, const py::str& mn, const py::str& size, const py::list<Op>& ops, int64_t pc) {
    std::string out;
    out.push_back(static_cast<char>(word >> 8));
    out.push_back(static_cast<char>(word & 0xFF));

    auto put16 = [&out](int64_t v) {
        out.push_back(static_cast<char>((v >> 8) & 0xFF));
        out.push_back(static_cast<char>(v & 0xFF));
    };
    auto put32 = [&put16](int64_t v) {
        put16((v >> 16) & 0xFFFF);
        put16(v & 0xFFFF);
    };
    if (mn == "MOVEM") {
        bool found = false;
        for (const Op& o : ops) {
            if (o.kind == "regs") {
                put16(o.num());
                found = true;
                break;
            }
        }
        if (!found) {
            throw py::StopIteration();
        }
    }
    for (const Op& o : ops) {
        const py::str& k = o.kind;
        if (k == "imm") {
            bool is_l, is_b;
            if (mn == "LINK" || mn == "STOP") {
                is_l = false;
                is_b = false;
            } else {
                is_l = o.size_is("L");
                is_b = o.size_is("B");
            }
            if ((mn == "ORI" || mn == "ANDI" || mn == "EORI") && ops[-1].kind == "ccr") {
                is_l = false;
                is_b = true;
            }
            if (mn == "BTST" || mn == "BCHG" || mn == "BCLR" || mn == "BSET") {
                is_l = false;
                is_b = true;
            }
            int64_t v = o.num();
            if (is_l) {
                put32(v & 0xFFFFFFFF);
            } else {
                put16(v & (is_b ? 0xFF : 0xFFFF));
            }
        } else if (k == "disp") {
            put16(o.num() & 0xFFFF);
        } else if (k == "index") {
            put16(_brief(o) | (o.num() & 0xFF));
        } else if (k == "absw") {
            put16(o.num() & 0xFFFF);
        } else if (k == "absl") {
            put32(o.num() & 0xFFFFFFFF);
        } else if (k == "pcdisp") {
            int64_t d = _wrap(py::checked_sub(o.num(), pc + static_cast<int64_t>(out.size())));
            if (!(-0x8000 <= d && d <= 0x7FFF)) {
                throw py::ValueError("PC-relative operand out of range");
            }
            put16(d & 0xFFFF);
        } else if (k == "pcindex") {
            int64_t d = _wrap(py::checked_sub(o.num(), pc + static_cast<int64_t>(out.size())));
            if (!(-0x80 <= d && d <= 0x7F)) {
                throw py::ValueError("PC-relative operand out of range");
            }
            put16(_brief(o) | (d & 0xFF));
        } else if (k == "label") {
            int64_t d = _wrap(py::checked_sub(o.num(), pc + static_cast<int64_t>(out.size())));
            if (!(-0x8000 <= d && d <= 0x7FFF)) {
                throw py::ValueError("branch out of range");
            }
            put16(d & 0xFFFF);
        }
    }
    return py::bytes(out);
}

// A difference of 32-bit addresses as a signed number (the address space wraps).
int64_t _wrap(int64_t v) {
    return ((py::checked_add(v, 0x80000000) & 0xFFFFFFFF)) - 0x80000000;
}

int64_t _brief(const Op& o) {
    if (!o.xreg.has_value()) {
        throw py::TypeError("'NoneType' object is not subscriptable");
    }
    return (std::get<0>(*o.xreg) == "A" ? 0x8000 : 0) | (std::get<1>(*o.xreg) << 12) | (o.xsize == "L" ? 0x0800 : 0);
}

}  // namespace codebreaker::decompile::m68k
