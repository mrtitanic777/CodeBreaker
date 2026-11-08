// codebreaker/recompile/builders/logical.cpp -- C++17 port of codebreaker.recompile.builders.logical (see logical.hpp).

#include "codebreaker/recompile/builders/logical.hpp"

#include <cstdio>
#include <string>
#include <tuple>

namespace codebreaker::recompile::builders::logical {

namespace {

using Number = std::optional<int64_t>;

// f"{n}" for a number that is None when the operand token was not of the kind asked for
py::str num(const Number& n) { return n ? py::to_str(*n) : py::str("None"); }

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Number& n) {
    if (!n) throw py::TypeError("unsupported operand type(s): 'NoneType'");
    return *n;
}

// the Python's  n + k  /  k - n  on values that only feed a mask: done modulo 2**64, which the
// mask's "& 0x3F" cannot tell from the exact sum
int64_t wrap_add(int64_t a, int64_t b) { return static_cast<int64_t>(static_cast<uint64_t>(a) + static_cast<uint64_t>(b)); }
int64_t wrap_sub(int64_t a, int64_t b) { return static_cast<int64_t>(static_cast<uint64_t>(a) - static_cast<uint64_t>(b)); }

// f"0x{value:X}"
py::str hex_(uint64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%llX", static_cast<unsigned long long>(value));
    return py::str(buf);
}

// the decimal text of  v << 16  (a 64-bit imm shifted left by 16 does not always fit 64 bits, and the
// Python's integers do not overflow): the magnitude kept in four 32-bit limbs and divided by ten
py::str shl16_text(int64_t v) {
    bool neg = v < 0;
    uint64_t mag = neg ? static_cast<uint64_t>(0) - static_cast<uint64_t>(v) : static_cast<uint64_t>(v);
    uint32_t limb[4] = {static_cast<uint32_t>(mag << 16), static_cast<uint32_t>((mag << 16) >> 32),
                        static_cast<uint32_t>(mag >> 48), 0};   // least significant first
    std::string digits;
    for (;;) {
        bool zero = true;
        uint64_t rem = 0;
        for (int i = 3; i >= 0; --i) {
            uint64_t cur = (rem << 32) | limb[i];
            limb[i] = static_cast<uint32_t>(cur / 10);
            rem = cur % 10;
            if (limb[i]) zero = false;
        }
        digits.insert(digits.begin(), static_cast<char>('0' + static_cast<int>(rem)));
        if (zero) break;
    }
    if (digits == "0") return py::str("0");
    return py::str((neg ? "-" : "") + digits);
}

}  // namespace

uint64_t _compute_mask(int64_t mstart, int64_t mstop) {
    // the rotate mask from bit mstart to bit mstop (the code generator's compute_mask)
    mstart &= 0x3F;
    mstop &= 0x3F;
    uint64_t value = (U64 >> mstart) ^ (mstop >= 63 ? 0 : (U64 >> (mstop + 1)));
    value &= U64;
    return mstart <= mstop ? value : ((~value) & U64);
}

std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_vreg; (void)_freg; (void)_cr; (void)_mem; (void)_base_expr; (void)_addr; (void)va; (void)WORD;
    const std::string m = mn.utf8();

    // _rv(n, field)
    auto V = [&](const Number& n, const char* field) { return _rv(n, py::str(field)); };
    // a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
    auto reg3 = [&]() {
        Number a = _reg(ops[0]);
        Number b = _reg(ops[1]);
        Number c = _reg(ops[2]);
        return std::make_tuple(a, b, c);
    };
    // a, b = _reg(ops[0]), _reg(ops[1])
    auto reg2 = [&]() {
        Number a = _reg(ops[0]);
        Number b = _reg(ops[1]);
        return std::make_tuple(a, b);
    };
    auto lines = [](std::initializer_list<py::str> l) { return py::list<py::str>(l); };

    // ---- AND ----
    if (m == "and") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " & " + V(c, "u64") + ";"});
    }

    if (m == "andc") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " & ~" + V(c, "u64") + ";"});
    }

    if (m == "andi") {
        auto [a, b] = reg2();
        Number imm = _imm(ops[2]);
        return lines({
            V(a, "u64") + " = " + V(b, "u64") + " & " + num(imm) + ";",
            py::str("ctx.cr0.compare<int32_t>(") + V(a, "s32") + ", 0, ctx.xer);",
        });
    }

    if (m == "andis") {
        auto [a, b] = reg2();
        Number imm = _imm(ops[2]);
        py::str shifted = shl16_text(need(imm));
        return lines({
            V(a, "u64") + " = " + V(b, "u64") + " & " + shifted + ";",
            py::str("ctx.cr0.compare<int32_t>(") + V(a, "s32") + ", 0, ctx.xer);",
        });
    }

    // ---- OR / NAND / NOR / NOT ----
    if (m == "nand") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = ~(" + V(b, "u64") + " & " + V(c, "u64") + ");"});
    }

    if (m == "nor") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = ~(" + V(b, "u64") + " | " + V(c, "u64") + ");"});
    }

    if (m == "not") {
        auto [a, b] = reg2();
        return lines({V(a, "u64") + " = ~" + V(b, "u64") + ";"});
    }

    if (m == "or") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " | " + V(c, "u64") + ";"});
    }

    if (m == "orc") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " | ~" + V(c, "u64") + ";"});
    }

    if (m == "ori") {
        auto [a, b] = reg2();
        Number imm = _imm(ops[2]);
        return lines({V(a, "u64") + " = " + V(b, "u64") + " | " + num(imm) + ";"});
    }

    if (m == "oris") {
        auto [a, b] = reg2();
        int64_t imm = need(_imm(ops[2])) & 0xFFFFFFFF;
        return lines({V(a, "u64") + " = " + V(b, "u64") + " | " + py::to_str(imm << 16) + ";"});
    }

    // ---- XOR / EQV ----
    if (m == "xor") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " ^ " + V(c, "u64") + ";"});
    }

    if (m == "xori") {
        auto [a, b] = reg2();
        Number imm = _imm(ops[2]);
        return lines({V(a, "u64") + " = " + V(b, "u64") + " ^ " + num(imm) + ";"});
    }

    if (m == "xoris") {
        auto [a, b] = reg2();
        Number imm = _imm(ops[2]);
        py::str shifted = shl16_text(need(imm));
        return lines({V(a, "u64") + " = " + V(b, "u64") + " ^ " + shifted + ";"});
    }

    if (m == "eqv") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = ~(" + V(b, "u64") + " ^ " + V(c, "u64") + ");"});
    }

    // ---- Count Leading Zeros ----
    if (m == "cntlzd") {
        auto [a, b] = reg2();
        return lines({V(a, "u64") + " = " + V(b, "u64") + " == 0 ? 64 : __builtin_clzll(" + V(b, "u64") + ");"});
    }

    if (m == "cntlzw") {
        auto [a, b] = reg2();
        return lines({V(a, "u64") + " = " + V(b, "u32") + " == 0 ? 32 : __builtin_clz(" + V(b, "u32") + ");"});
    }

    // ---- Sign Extension ----
    if (m == "extsb") {
        auto [a, b] = reg2();
        return lines({V(a, "s64") + " = " + V(b, "s8") + ";"});
    }

    if (m == "extsh") {
        auto [a, b] = reg2();
        return lines({V(a, "s64") + " = " + V(b, "s16") + ";"});
    }

    if (m == "extsw") {
        auto [a, b] = reg2();
        return lines({V(a, "s64") + " = " + V(b, "s32") + ";"});
    }

    // ---- Clear Left Word Immediate ----
    if (m == "clrlwi") {
        auto [a, b] = reg2();
        int64_t n = need(_imm(ops[2]));
        // ((1 << (32 - n)) - 1) & U64: a negative shift count is the Python's ValueError, a count of 64 or more fills the mask
        if (n > 32) throw py::ValueError("negative shift count");
        uint64_t k = static_cast<uint64_t>(32) - static_cast<uint64_t>(n);   // 0 or more, and fits 64 bits
        uint64_t mask = k >= 64 ? U64 : ((static_cast<uint64_t>(1) << k) - 1) & U64;
        return lines({V(a, "u64") + " = " + V(b, "u32") + " & " + hex_(mask) + ";"});
    }

    // ---- Rotate Left Double Word ----
    if (m == "rldicl") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        uint64_t mask = _compute_mask(need(_imm(ops[3])), 63);
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + num(sh) + ") & " + hex_(mask) + ";"});
    }

    if (m == "rldicr") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        uint64_t mask = _compute_mask(0, need(_imm(ops[3])));
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + num(sh) + ") & " + hex_(mask) + ";"});
    }

    if (m == "rldic") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        Number mb = _imm(ops[3]);
        int64_t stop = wrap_sub(63, need(sh));
        uint64_t mask = _compute_mask(need(mb), stop);
        if (sh && *sh != 0)
            return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + num(sh) + ") & " + hex_(mask) + ";"});
        return lines({V(a, "u64") + " = " + V(b, "u64") + " & " + hex_(mask) + ";"});
    }

    if (m == "rldcl") {
        auto [a, b, c] = reg3();
        uint64_t mask = _compute_mask(need(_imm(ops[3])), 63);
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + V(c, "u8") + " & 0x3F) & " + hex_(mask) + ";"});
    }

    if (m == "rldcr") {
        auto [a, b, c] = reg3();
        uint64_t mask = _compute_mask(0, need(_imm(ops[3])));
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + V(c, "u8") + " & 0x3F) & " + hex_(mask) + ";"});
    }

    if (m == "rldimi") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        Number mb = _imm(ops[3]);
        int64_t stop = ~need(sh);
        uint64_t mask = _compute_mask(need(mb), stop);
        uint64_t notmask = (~mask) & U64;
        return lines({V(a, "u64") + " = (__builtin_rotateleft64(" + V(b, "u64") + ", " + num(sh) + ") & " + hex_(mask) + ") | (" + V(a, "u64") + " & " + hex_(notmask) + ");"});
    }

    if (m == "rotldi") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u64") + ", " + num(sh) + ");"});
    }

    // ---- Rotate Left Word ----
    if (m == "rlwimi") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        int64_t mstart = wrap_add(need(_imm(ops[3])), 32);
        int64_t mstop = wrap_add(need(_imm(ops[4])), 32);
        uint64_t mask = _compute_mask(mstart, mstop);
        uint64_t notmask = (~mask) & U64;
        return lines({V(a, "u64") + " = (__builtin_rotateleft64(" + V(b, "u32") + " | (" + V(b, "u64") + " << 32), " + num(sh) + ") & " + hex_(mask) + ") | (" + V(a, "u64") + " & " + hex_(notmask) + ");"});
    }

    if (m == "rlwinm") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        int64_t mstart = wrap_add(need(_imm(ops[3])), 32);
        int64_t mstop = wrap_add(need(_imm(ops[4])), 32);
        uint64_t mask = _compute_mask(mstart, mstop);
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u32") + " | (" + V(b, "u64") + " << 32), " + num(sh) + ") & " + hex_(mask) + ";"});
    }

    if (m == "rlwnm") {
        auto [a, b, c] = reg3();
        int64_t mstart = wrap_add(need(_imm(ops[3])), 32);
        int64_t mstop = wrap_add(need(_imm(ops[4])), 32);
        uint64_t mask = _compute_mask(mstart, mstop);
        return lines({V(a, "u64") + " = __builtin_rotateleft64(" + V(b, "u32") + " | (" + V(b, "u64") + " << 32), " + V(c, "u8") + " & 0x1F) & " + hex_(mask) + ";"});
    }

    if (m == "rotlw") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = __builtin_rotateleft32(" + V(b, "u32") + ", " + V(c, "u8") + " & 0x1F);"});
    }

    if (m == "rotlwi") {
        auto [a, b] = reg2();
        Number sh = _imm(ops[2]);
        return lines({V(a, "u64") + " = __builtin_rotateleft32(" + V(b, "u32") + ", " + num(sh) + ");"});
    }

    // ---- Shift Left ----
    if (m == "sld") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(c, "u8") + " & 0x40 ? 0 : (" + V(b, "u64") + " << (" + V(c, "u8") + " & 0x7F));"});
    }

    if (m == "slw") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(c, "u8") + " & 0x20 ? 0 : (" + V(b, "u32") + " << (" + V(c, "u8") + " & 0x3F));"});
    }

    // ---- Shift Right Algebraic (signed) ----
    if (m == "srad") {
        auto [a, b, c] = reg3();
        return lines({
            py::str("temp.u64 = ") + V(c, "u64") + " & 0x7F;",
            py::str("if (temp.u64 > 0x3F) temp.u64 = 0x3F;"),
            py::str("ctx.xer.ca = (") + V(b, "s64") + " < 0) & (((" + V(b, "s64") + " >> temp.u64) << temp.u64) != " + V(b, "s64") + ");",
            V(a, "s64") + " = " + V(b, "s64") + " >> temp.u64;",
        });
    }

    if (m == "sradi") {
        auto [a, b] = reg2();
        Number n = _imm(ops[2]);
        if (!n || *n != 0) {
            uint64_t mask = _compute_mask(wrap_sub(64, need(n)), 63);
            return lines({
                py::str("ctx.xer.ca = (") + V(b, "s64") + " < 0) & ((" + V(b, "u64") + " & " + hex_(mask) + ") != 0);",
                V(a, "s64") + " = " + V(b, "s64") + " >> " + num(n) + ";",
            });
        }
        return lines({
            py::str("ctx.xer.ca = 0;"),
            V(a, "s64") + " = " + V(b, "s64") + ";",
        });
    }

    if (m == "sraw") {
        auto [a, b, c] = reg3();
        return lines({
            py::str("temp.u32 = ") + V(c, "u32") + " & 0x3F;",
            py::str("if (temp.u32 > 0x1F) temp.u32 = 0x1F;"),
            py::str("ctx.xer.ca = (") + V(b, "s32") + " < 0) & (((" + V(b, "s32") + " >> temp.u32) << temp.u32) != " + V(b, "s32") + ");",
            V(a, "s64") + " = " + V(b, "s32") + " >> temp.u32;",
        });
    }

    if (m == "srawi") {
        auto [a, b] = reg2();
        Number n = _imm(ops[2]);
        if (!n || *n != 0) {
            uint64_t mask = _compute_mask(wrap_sub(64, need(n)), 63);
            return lines({
                py::str("ctx.xer.ca = (") + V(b, "s32") + " < 0) & ((" + V(b, "u32") + " & " + hex_(mask) + ") != 0);",
                V(a, "s64") + " = " + V(b, "s32") + " >> " + num(n) + ";",
            });
        }
        return lines({
            py::str("ctx.xer.ca = 0;"),
            V(a, "s64") + " = " + V(b, "s32") + ";",
        });
    }

    // ---- Shift Right (unsigned) ----
    if (m == "srd") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(c, "u8") + " & 0x40 ? 0 : (" + V(b, "u64") + " >> (" + V(c, "u8") + " & 0x7F));"});
    }

    if (m == "srw") {
        auto [a, b, c] = reg3();
        return lines({V(a, "u64") + " = " + V(c, "u8") + " & 0x20 ? 0 : (" + V(b, "u32") + " >> (" + V(c, "u8") + " & 0x3F));"});
    }

    return std::nullopt;
}

const py::str RUNTIME_HELPERS = py::str("");

const py::list<py::str> MNEMONICS = {
    py::str("and"), py::str("andc"), py::str("andi"), py::str("andis"),
    py::str("nand"), py::str("nor"), py::str("not"), py::str("or"), py::str("orc"), py::str("ori"), py::str("oris"),
    py::str("xor"), py::str("xori"), py::str("xoris"), py::str("eqv"),
    py::str("cntlzd"), py::str("cntlzw"),
    py::str("extsb"), py::str("extsh"), py::str("extsw"),
    py::str("clrlwi"),
    py::str("rldicl"), py::str("rldicr"), py::str("rldic"), py::str("rldcl"), py::str("rldcr"), py::str("rldimi"), py::str("rotldi"),
    py::str("rlwimi"), py::str("rlwinm"), py::str("rlwnm"), py::str("rotlw"), py::str("rotlwi"),
    py::str("sld"), py::str("slw"),
    py::str("srad"), py::str("sradi"), py::str("sraw"), py::str("srawi"),
    py::str("srd"), py::str("srw"),
};

}  // namespace codebreaker::recompile::builders::logical
