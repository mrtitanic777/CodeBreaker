// codebreaker/build/ppc_asm.cpp -- typed C++ port of codebreaker.build.ppc_asm.

#include "codebreaker/build/ppc_asm.hpp"

namespace codebreaker::build::ppc_asm {

const int64_t MASK32 = 0xFFFFFFFF;

namespace {

// ---------------------------------------------------------------------------------------------------------
// Python's int(text, base) for base 0 and 10, returning int64_t; a magnitude past int64 is reported by
// `saturated` (the value is then +-INT64_MAX) instead of growing. Rules as CPython's: blanks around, a sign,
// 0x / 0o / 0b prefixes in base 0, no leading zeros in base 0 unless the number is zero, single underscores
// between digits (or right after a prefix), Unicode decimal digits as their ASCII digit; anything else raises
// ValueError.
// ---------------------------------------------------------------------------------------------------------
int decimal_digit_value(char32_t c) {
    if (c >= U'0' && c <= U'9') {
        return static_cast<int>(c - U'0');
    }
    if (c < 0x80) {
        return -1;
    }
    constexpr size_t N = sizeof(py::unicode_::decimal_ranges) / sizeof(py::unicode_::decimal_ranges[0]);
    for (size_t i = 0; i < N; ++i) {
        if (c >= py::unicode_::decimal_ranges[i][0] && c <= py::unicode_::decimal_ranges[i][1]) {
            return static_cast<int>((c - py::unicode_::decimal_ranges[i][0]) % 10);
        }
    }
    return -1;
}

int digit_value(char32_t c) {
    int d = decimal_digit_value(c);
    if (d >= 0) {
        return d;
    }
    if (c >= U'a' && c <= U'z') {
        return static_cast<int>(c - U'a') + 10;
    }
    if (c >= U'A' && c <= U'Z') {
        return static_cast<int>(c - U'A') + 10;
    }
    return 99;
}

int64_t py_int(const py::str& text, int64_t base, bool& saturated) {
    saturated = false;
    const std::u32string s = text.strip().u32();
    auto bad = [&]() {
        return py::ValueError(py::percent(py::str("invalid literal for int() with base %d: %s"), base, py::repr(text)));
    };
    size_t i = 0;
    bool neg = false;
    if (i < s.size() && (s[i] == U'+' || s[i] == U'-')) {
        neg = s[i] == U'-';
        ++i;
    }
    bool leading_zero_decimal = false;
    if (base == 0) {
        if (i < s.size() && s[i] == U'0') {
            char32_t p = i + 1 < s.size() ? s[i + 1] : 0;
            if (p == U'x' || p == U'X') {
                base = 16;
                i += 2;
            } else if (p == U'o' || p == U'O') {
                base = 8;
                i += 2;
            } else if (p == U'b' || p == U'B') {
                base = 2;
                i += 2;
            } else {
                base = 10;
                leading_zero_decimal = true;  // "old" octal literal: only zero is valid
            }
            if (base != 10 && i < s.size() && s[i] == U'_') {
                ++i;  // one underscore is allowed after a prefix
            }
        } else {
            base = 10;
        }
    } else if (base == 16 || base == 8 || base == 2) {
        throw py::ValueError("unsupported base");
    }
    if (i >= s.size()) {
        throw bad();
    }
    uint64_t v = 0;
    bool any_nonzero = false;
    bool prev_underscore = true;  // a number may not start with an underscore (a prefix's one was taken)
    for (; i < s.size(); ++i) {
        if (s[i] == U'_') {
            if (prev_underscore) {
                throw bad();
            }
            prev_underscore = true;
            continue;
        }
        int d = digit_value(s[i]);
        if (d >= base) {
            throw bad();
        }
        prev_underscore = false;
        if (d != 0) {
            any_nonzero = true;
        }
        if (!saturated) {
            if (v > (static_cast<uint64_t>(INT64_MAX) - static_cast<uint64_t>(d)) / static_cast<uint64_t>(base)) {
                saturated = true;
            } else {
                v = v * static_cast<uint64_t>(base) + static_cast<uint64_t>(d);
            }
        }
    }
    if (prev_underscore) {
        throw bad();  // empty, or a trailing underscore
    }
    if (leading_zero_decimal && any_nonzero) {
        throw bad();
    }
    int64_t mag = saturated ? INT64_MAX : static_cast<int64_t>(v);
    return neg ? -mag : mag;
}

// int(text, 0) / int(text) where the value is used whole: past int64 is an OverflowError.
int64_t py_int_exact(const py::str& text, int64_t base) {
    bool sat = false;
    int64_t v = py_int(text, base, sat);
    if (sat) {
        throw py::OverflowError("int too large for int64");
    }
    return v;
}

}  // namespace

int64_t _reg(const py::str& tok) {
    py::str t = tok.strip().lower();
    if (t.startswith(py::str("r"))) {
        t = t.slice_from(1);
    }
    if (!t.isdigit()) {
        throw AsmError(py::percent(py::str("bad register: %s"), tok));
    }
    bool sat = false;
    int64_t n = py_int(t, 10, sat);  // (a digit that is not a decimal one, such as "²", is a ValueError here too)
    if (sat || !(0 <= n && n <= 31)) {
        throw AsmError(py::percent(py::str("bad register: %s"), tok));
    }
    return n;
}

int64_t _imm(const py::str& tok, int64_t bits, bool signed_) {
    py::str t = tok.strip();
    int64_t v = 0;
    bool sat = false;
    try {
        v = py_int(t, 0, sat);
    } catch (const py::ValueError&) {
        throw AsmError(py::percent(py::str("bad immediate: %s"), tok));
    }
    if (signed_) {
        int64_t lo = -(INT64_C(1) << (bits - 1));
        int64_t hi = (INT64_C(1) << (bits - 1)) - 1;
        if (sat || (!(lo <= v && v <= hi) && !(0 <= v && v <= (INT64_C(1) << bits) - 1))) {
            throw AsmError(py::percent(py::str("immediate out of range: %s"), tok));
        }
    } else if (sat) {
        throw py::OverflowError("int too large for int64");
    }
    return v & ((INT64_C(1) << bits) - 1);
}

// Parse `D(rA)` -> (disp, rA).
std::tuple<int64_t, int64_t> _mem(const py::str& tok) {
    std::optional<py::re::Match> m =
        py::re::match(py::str("\\s*(-?(?:0x)?[0-9a-fA-F]+)\\s*\\(\\s*r?(\\d+)\\s*\\)\\s*$"), tok);
    if (!m.has_value()) {
        throw AsmError(py::percent(py::str("bad memory operand: %s"), tok));
    }
    int64_t disp = py_int_exact(m->group(1), 0) & 0xFFFF;
    int64_t ra = py_int_exact(m->group(2), 10);
    return std::make_tuple(disp, ra);
}

int64_t _dform(int64_t op, int64_t rt, int64_t ra, int64_t imm) {
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | (imm & 0xFFFF);
}

int64_t _xform(int64_t op, int64_t rt, int64_t ra, int64_t rb, int64_t xo, int64_t rc) {
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | ((rb & 0x1F) << 11) |
           ((xo & 0x3FF) << 1) | (rc & 1);
}

int64_t _spr_enc(int64_t spr) { return ((spr & 0x1F) << 5) | ((spr >> 5) & 0x1F); }

const py::dict<py::str, int64_t> _SPR = {{py::str("xer"), 1}, {py::str("lr"), 8}, {py::str("ctr"), 9}};

const py::dict<py::str, int64_t> _XFORM3 = {
    {py::str("add"), 266},   {py::str("add."), 266},  {py::str("subf"), 40},    {py::str("subf."), 40},
    {py::str("sub"), 40},    {py::str("and"), 28},    {py::str("and."), 28},     {py::str("or"), 444},
    {py::str("or."), 444},   {py::str("xor"), 316},   {py::str("xor."), 316},    {py::str("nand"), 476},
    {py::str("nor"), 124},   {py::str("slw"), 24},    {py::str("srw"), 536},     {py::str("sraw"), 792},
    {py::str("mullw"), 235}, {py::str("mullw."), 235}, {py::str("divw"), 491},   {py::str("divwu"), 459},
    {py::str("andc"), 60},
};

const py::dict<py::str, std::tuple<int64_t, bool>> _DFORM = {
    {py::str("addi"), {14, true}},    {py::str("addis"), {15, true}},  {py::str("subi"), {14, true}},
    {py::str("ori"), {24, false}},    {py::str("oris"), {25, false}},  {py::str("xori"), {26, false}},
    {py::str("xoris"), {27, false}},  {py::str("andi."), {28, false}}, {py::str("andis."), {29, false}},
    {py::str("mulli"), {7, true}},    {py::str("cmpwi"), {11, true}},  {py::str("cmplwi"), {10, false}},
};

const py::dict<py::str, int64_t> _MEMOP = {
    {py::str("lwz"), 32}, {py::str("lwzu"), 33}, {py::str("lbz"), 34}, {py::str("lbzu"), 35}, {py::str("lhz"), 40},
    {py::str("lhzu"), 41}, {py::str("lha"), 42}, {py::str("stw"), 36}, {py::str("stwu"), 37}, {py::str("stb"), 38},
    {py::str("stbu"), 39}, {py::str("sth"), 44}, {py::str("sthu"), 45},
};

const py::dict<py::str, std::tuple<int64_t, int64_t>> _BCOND = {
    {py::str("beq"), {12, 2}},  {py::str("bne"), {4, 2}},  {py::str("blt"), {12, 0}}, {py::str("bge"), {4, 0}},
    {py::str("bgt"), {12, 1}},  {py::str("ble"), {4, 1}},  {py::str("bdnz"), {16, 0}}, {py::str("bdz"), {18, 0}},
    {py::str("bso"), {12, 3}},  {py::str("bns"), {4, 3}},
};

std::optional<int64_t> assemble_one(const py::str& line_in, int64_t addr, const py::dict<py::str, int64_t>& labels) {
    py::str line = py::split(py::split(line_in, py::str("#"), 1)[0], py::str(";"), 1)[0].strip();
    if (line.size() == 0 || line.endswith(py::str(":"))) {
        return std::nullopt;
    }
    py::list<py::str> parts = py::split(line.replace(py::str(","), py::str(" ")));
    py::str mn = parts[0].lower();
    py::list<py::str> ops = parts.slice_from(1);

    auto tgt = [&](const py::str& tok) -> int64_t {
        if (labels.contains(tok)) {
            return labels.at(tok);
        }
        return py_int_exact(tok, 0);
    };

    // ---- simplified / no-operand ------------------------------------------
    if (mn == py::str("nop")) {
        return 0x60000000;
    }
    if (mn == py::str("blr")) {
        return 0x4E800020;
    }
    if (mn == py::str("blrl")) {
        return 0x4E800021;
    }
    if (mn == py::str("bctr")) {
        return 0x4E800420;
    }
    if (mn == py::str("bctrl")) {
        return 0x4E800421;
    }
    if (mn == py::str("trap") || mn == py::str("tw")) {
        return 0x7FE00008;  // tw 31,r0,r0 (unconditional trap)
    }
    if (mn == py::str("mflr")) {
        int64_t e = _spr_enc(8);
        int64_t r = _reg(ops[0]);
        return (INT64_C(31) << 26) | (r << 21) | (e << 11) | (339 << 1);
    }
    if (mn == py::str("mtlr")) {
        int64_t rs = _reg(ops[0]);
        int64_t e = _spr_enc(8);
        return (INT64_C(31) << 26) | (rs << 21) | (e << 11) | (467 << 1);
    }
    if (mn == py::str("mfctr")) {
        int64_t e = _spr_enc(9);
        int64_t r = _reg(ops[0]);
        return (INT64_C(31) << 26) | (r << 21) | (e << 11) | (339 << 1);
    }
    if (mn == py::str("mtctr")) {
        int64_t e = _spr_enc(9);
        int64_t r = _reg(ops[0]);
        return (INT64_C(31) << 26) | (r << 21) | (e << 11) | (467 << 1);
    }
    if (mn == py::str("mfspr") || mn == py::str("mtspr")) {
        // mtspr spr, rS  /  mfspr rD, spr
        if (mn == py::str("mtspr")) {
            std::optional<int64_t> named = _SPR.get(ops[0].lower());
            int64_t spr = named.has_value() ? *named : _imm(ops[0], 10, false);
            int64_t rs = _reg(ops[1]);
            return (INT64_C(31) << 26) | (rs << 21) | (_spr_enc(spr) << 11) | (467 << 1);
        } else {
            int64_t rd = _reg(ops[0]);
            std::optional<int64_t> named = _SPR.get(ops[1].lower());
            int64_t spr = named.has_value() ? *named : _imm(ops[1], 10, false);
            return (INT64_C(31) << 26) | (rd << 21) | (_spr_enc(spr) << 11) | (339 << 1);
        }
    }
    if (mn == py::str("li")) {
        int64_t rt = _reg(ops[0]);
        int64_t imm = _imm(ops[1]);
        return _dform(14, rt, 0, imm);
    }
    if (mn == py::str("lis")) {
        int64_t rt = _reg(ops[0]);
        int64_t imm = _imm(ops[1]);
        return _dform(15, rt, 0, imm);
    }
    if (mn == py::str("mr") || mn == py::str("mr.")) {
        int64_t rs = _reg(ops[1]);
        int64_t ra = _reg(ops[0]);
        return _xform(31, rs, ra, rs, 444, mn.endswith(py::str(".")) ? 1 : 0);
    }
    if (mn == py::str("not")) {
        int64_t rs = _reg(ops[1]);
        int64_t ra = _reg(ops[0]);
        return _xform(31, rs, ra, rs, 124);  // nor rA,rS,rS
    }

    // ---- branches ---------------------------------------------------------
    if (mn == py::str("b") || mn == py::str("ba") || mn == py::str("bl") || mn == py::str("bla")) {
        int64_t aa = mn.slice_from(1).contains(py::str("a")) ? 1 : 0;
        int64_t lk = mn.endswith(py::str("l")) ? 1 : 0;
        int64_t dest = tgt(ops[0]);
        int64_t li = aa ? dest : (dest - addr);
        if (li & 3) {
            throw AsmError("branch target not 4-aligned");
        }
        return (INT64_C(18) << 26) | (li & 0x03FFFFFC) | (aa << 1) | lk;
    }
    if (_BCOND.contains(mn) || mn == py::str("bc") || mn == py::str("bcl")) {
        int64_t bo = 0, bi = 0, dest = 0, lk = 0;
        if (_BCOND.contains(mn)) {
            std::tuple<int64_t, int64_t> pair = _BCOND.at(mn);
            bo = std::get<0>(pair);
            bi = std::get<1>(pair);
            dest = tgt(ops[0]);
            lk = 0;
        } else {
            bo = _imm(ops[0], 5, false);
            bi = _imm(ops[1], 5, false);
            dest = tgt(ops[2]);
            lk = mn.endswith(py::str("l")) ? 1 : 0;
        }
        int64_t bd = dest - addr;
        if (bd & 3) {
            throw AsmError("branch target not 4-aligned");
        }
        return (INT64_C(16) << 26) | (bo << 21) | (bi << 16) | (bd & 0xFFFC) | lk;
    }

    // ---- D-form -----------------------------------------------------------
    if (_DFORM.contains(mn)) {
        std::tuple<int64_t, bool> spec = _DFORM.at(mn);
        int64_t op = std::get<0>(spec);
        bool signed_ = std::get<1>(spec);
        if (mn == py::str("subi")) {
            int64_t rt = _reg(ops[0]);
            int64_t ra = _reg(ops[1]);
            int64_t imm = (-_imm(ops[2], 16, false)) & 0xFFFF;
            return _dform(14, rt, ra, imm);
        }
        if (mn == py::str("cmpwi") || mn == py::str("cmplwi")) {
            int64_t ra = _reg(ops[0]);
            int64_t imm = _imm(ops[1], 16, signed_);
            return _dform(op, 0, ra, imm);
        }
        int64_t rt = _reg(ops[0]);
        int64_t ra = _reg(ops[1]);
        int64_t imm = ops.size() > 2 ? _imm(ops[2], 16, signed_) : 0;
        return _dform(op, rt, ra, imm);
    }
    if (_MEMOP.contains(mn)) {
        std::tuple<int64_t, int64_t> mem = _mem(ops[1]);
        int64_t disp = std::get<0>(mem);
        int64_t ra = std::get<1>(mem);
        int64_t rt = _reg(ops[0]);
        return _dform(_MEMOP.at(mn), rt, ra, disp);
    }

    // ---- X-form 3-register ------------------------------------------------
    if (_XFORM3.contains(mn)) {
        int64_t xo = _XFORM3.at(mn);
        int64_t rc = mn.endswith(py::str(".")) ? 1 : 0;
        // (the Python special-cases sub / subf / subf. with the very same encoding as the others:
        //  subf rD,rA,rB = rB - rA, the natural `sub rD,rB,rA` handled as subf)
        int64_t rt = _reg(ops[0]);
        int64_t ra = _reg(ops[1]);
        int64_t rb = _reg(ops[2]);
        return _xform(31, rt, ra, rb, xo, rc);
    }
    if (mn == py::str("cmpw") || mn == py::str("cmplw")) {
        int64_t xo = mn == py::str("cmpw") ? 0 : 32;
        int64_t ra = _reg(ops[0]);
        int64_t rb = _reg(ops[1]);
        return _xform(31, 0, ra, rb, xo);
    }
    if (mn == py::str("neg") || mn == py::str("neg.")) {
        int64_t rt = _reg(ops[0]);
        int64_t ra = _reg(ops[1]);
        return _xform(31, rt, ra, 0, 104, mn.endswith(py::str(".")) ? 1 : 0);
    }
    if (mn == py::str("extsb") || mn == py::str("extsh") || mn == py::str("extsb.") || mn == py::str("extsh.")) {
        int64_t xo = mn.contains(py::str("b")) ? 954 : 922;
        int64_t rt = _reg(ops[1]);
        int64_t ra = _reg(ops[0]);
        return _xform(31, rt, ra, 0, xo, mn.endswith(py::str(".")) ? 1 : 0);
    }

    throw AsmError(py::percent(py::str("unsupported mnemonic: %s"), mn));
}

py::bytes assemble(const py::str& text, int64_t base_addr) {
    py::list<py::str> lines = py::splitlines(text);
    py::dict<py::str, int64_t> labels;
    int64_t addr = base_addr;
    for (const py::str& ln : lines) {  // pass 1: labels
        py::str s = py::split(py::split(ln, py::str("#"), 1)[0], py::str(";"), 1)[0].strip();
        if (s.size() == 0) {
            continue;
        }
        if (s.endswith(py::str(":"))) {
            labels.set(s.slice(0, s.size() - 1).strip(), addr);
            continue;
        }
        addr += 4;
    }
    py::bytes out;
    addr = base_addr;
    for (const py::str& ln : lines) {  // pass 2: encode
        std::optional<int64_t> w = assemble_one(ln, addr, labels);
        if (!w.has_value()) {
            continue;
        }
        out = out + py::pack(">I", *w & MASK32);
        addr += 4;
    }
    return out;
}

}  // namespace codebreaker::build::ppc_asm
