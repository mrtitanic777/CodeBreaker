// codebreaker/recompile/builders/system.cpp -- C++17 port of codebreaker.recompile.builders.system (see system.hpp).

#include "codebreaker/recompile/builders/system.hpp"

#include <cstdio>
#include <string>

namespace codebreaker::recompile::builders::system {

namespace {

using Number = std::optional<int64_t>;

// the number, or the TypeError Python raises for arithmetic on None
int64_t need(const Number& n) {
    if (!n) throw py::TypeError("unsupported operand type(s): 'NoneType'");
    return *n;
}

// f"{n}" for a number that is None when the operand token was not of the kind asked for
py::str num(const Number& n) { return n ? py::to_str(*n) : py::str("None"); }

// f"0x{value:X}"
py::str hex_(uint64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%llX", static_cast<unsigned long long>(value));
    return py::str(buf);
}

// f"0x{value:08X}"
py::str hex8(uint64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%08llX", static_cast<unsigned long long>(value));
    return py::str(buf);
}

// Python's  a // 4  and  a % 4  (floor semantics)
int64_t floordiv4(int64_t a) { return a >= 0 ? a / 4 : -((-(a + 1)) / 4) - 1; }
int64_t mod4(int64_t a) { return a - floordiv4(a) * 4; }

py::str crbit(int64_t index) { return _CRBIT[mod4(index)]; }

py::list<py::str> lines(std::initializer_list<py::str> l) { return py::list<py::str>(l); }

}  // namespace

const uint64_t U64 = 0xFFFFFFFFFFFFFFFFull;
const uint64_t U32 = 0xFFFFFFFFull;

const py::list<py::str> _CRBIT = {py::str("lt"), py::str("gt"), py::str("eq"), py::str("so")};

const py::set<py::str> _TWI = {
    py::str("twi"), py::str("twlgti"), py::str("twllti"), py::str("tweqi"), py::str("twlgei"), py::str("twlnli"), py::str("twllei"),
    py::str("twlngi"), py::str("twgti"), py::str("twgei"), py::str("twnli"), py::str("twlti"), py::str("twlei"), py::str("twngi"), py::str("twnei"),
};
const py::set<py::str> _TDI = {
    py::str("tdi"), py::str("tdlgti"), py::str("tdllti"), py::str("tdeqi"), py::str("tdlgei"), py::str("tdlnli"), py::str("tdllei"),
    py::str("tdlngi"), py::str("tdgti"), py::str("tdgei"), py::str("tdnli"), py::str("tdlti"), py::str("tdlei"), py::str("tdngi"), py::str("tdnei"),
};
const py::set<py::str> _TW = {
    py::str("tw"), py::str("twge"), py::str("twgt"), py::str("twle"), py::str("twlt"), py::str("tweq"), py::str("twne"),
    py::str("twlge"), py::str("twlgt"), py::str("twlle"), py::str("twllt"),
};
const py::set<py::str> _TD = {
    py::str("td"), py::str("tdge"), py::str("tdgt"), py::str("tdle"), py::str("tdlt"), py::str("tdeq"), py::str("tdne"),
    py::str("tdlge"), py::str("tdlgt"), py::str("tdlle"), py::str("tdllt"),
};

const py::set<py::str> _NOP_MNEMONICS = {
    py::str("nop"), py::str("attn"), py::str("sync"), py::str("lwsync"), py::str("eieio"), py::str("db16cyc"),
    py::str("cctpl"), py::str("cctpm"), py::str("cctph"),
    py::str("dcbf"), py::str("dcbt"), py::str("dcbtst"), py::str("dcbst"),
};

const py::dict<py::str, CrLogical> _CRLOGICAL = {
    {py::str("crand"), CrLogical(py::str("&"), false, false, false)},
    {py::str("crandc"), CrLogical(py::str("&"), false, true, false)},
    {py::str("creqv"), CrLogical(py::str("^"), false, false, true)},
    {py::str("crnand"), CrLogical(py::str("&"), false, false, true)},
    {py::str("crnor"), CrLogical(py::str("|"), false, false, true)},
    {py::str("cror"), CrLogical(py::str("|"), false, false, false)},
    {py::str("crorc"), CrLogical(py::str("|"), false, true, false)},
    {py::str("crxor"), CrLogical(py::str("^"), false, false, false)},
};

py::list<py::str> _emit_trap(int64_t to, const py::str& a_signed, const py::str& a_unsigned,
                             const py::str& b_signed, const py::str& b_unsigned) {
    // Port of emitTrap() (helpers.h).
    if (to == 0) return py::list<py::str>();
    if (to == 0x1F) return lines({py::str("ppc_trap(ctx, base, 0);")});
    py::list<py::str> parts;
    if (to & 0x10) parts.append(a_signed + " < " + b_signed);
    if (to & 0x08) parts.append(a_signed + " > " + b_signed);
    if (to & 0x04) parts.append(a_signed + " == " + b_signed);
    if (to & 0x02) parts.append(a_unsigned + " < " + b_unsigned);
    if (to & 0x01) parts.append(a_unsigned + " > " + b_unsigned);
    return lines({py::str("if (") + py::join(py::str(" || "), parts) + ") ppc_trap(ctx, base, 0);"});
}

py::list<py::str> _emit_crbit(const py::str& op, bool inv_a, bool inv_b, bool inv_res, const OperandNumber& _cr,
                              const py::list<py::str>& ops) {
    // Port of emitCRBitOperation() (helpers.h). operands are CR *bit* indices.
    Number crd = _cr(ops[0]);
    Number cra = _cr(ops[1]);
    Number crb = _cr(ops[2]);
    int64_t a = need(cra);
    py::str a_expr = py::str("ctx.cr") + py::to_str(floordiv4(a)) + "." + crbit(a);
    int64_t b = need(crb);
    py::str b_expr = py::str("ctx.cr") + py::to_str(floordiv4(b)) + "." + crbit(b);
    if (inv_a) a_expr = py::str("!(") + a_expr + ")";
    if (inv_b) b_expr = py::str("!(") + b_expr + ")";
    py::str expr = a_expr + " " + op + " " + b_expr;
    if (inv_res) expr = py::str("!(") + expr + ")";
    int64_t d = need(crd);
    return lines({py::str("ctx.cr") + py::to_str(floordiv4(d)) + "." + crbit(d) + " = " + expr + ";"});
}

std::optional<py::list<py::str>> emit(const py::str& mn, const py::list<py::str>& ops,
                                      const OperandNumber& _reg, const OperandNumber& _vreg, const OperandNumber& _freg,
                                      const OperandNumber& _imm, const OperandNumber& _cr, const MemOperand& _mem,
                                      const RegisterValue& _rv, const BaseExpression& _base_expr, const AddressExpression& _addr,
                                      int64_t va, int64_t WORD) {
    (void)_vreg; (void)_mem; (void)_rv; (void)_base_expr; (void)_addr; (void)va;
    // Record-form: only build_mr and build_clrldi honor the '.' bit (emit a CR0
    // compare). Strip a trailing '.' to get the base mnemonic; nothing else in
    // this category legitimately ends with '.'.
    bool rc = mn.endswith(py::str("."));
    py::str base_mn_s = rc ? mn.slice(0, mn.size() - 1) : mn;
    const std::string base_mn = base_mn_s.utf8();

    // -----------------------------------------------------------------------
    // No-ops / barriers / hints (emit nothing)
    // -----------------------------------------------------------------------
    if (_NOP_MNEMONICS.contains(base_mn_s)) return py::list<py::str>();

    // -----------------------------------------------------------------------
    // Trap instructions (decode from the raw word)
    // -----------------------------------------------------------------------
    if (_TWI.contains(base_mn_s)) {
        int64_t to = (WORD >> 21) & 0x1F;
        int64_t ra = (WORD >> 16) & 0x1F;
        int64_t simm = WORD & 0xFFFF;
        if (simm & 0x8000) simm -= 0x10000;   // sign-extend 16 -> 32
        // twi 31, r0, <imm> == unconditional service trap with code in imm
        if (to == 0x1F && ra == 0) {
            int64_t trap_type = simm & 0xFFFF;   // (uint16_t)simm
            return lines({py::str("ppc_trap(ctx, base, ") + py::to_str(trap_type) + ");"});
        }
        return _emit_trap(to,
                          py::str("ctx.r") + py::to_str(ra) + ".s32", py::str("ctx.r") + py::to_str(ra) + ".u32",
                          py::to_str(simm), py::to_str(static_cast<int64_t>(static_cast<uint64_t>(simm) & U32)) + "u");
    }

    if (_TDI.contains(base_mn_s)) {
        int64_t to = (WORD >> 21) & 0x1F;
        int64_t ra = (WORD >> 16) & 0x1F;
        int64_t simm = WORD & 0xFFFF;
        if (simm & 0x8000) simm -= 0x10000;   // sign-extend 16 -> 64
        // simm & U64 is the 64-bit two's complement: unsigned text
        return _emit_trap(to,
                          py::str("ctx.r") + py::to_str(ra) + ".s64", py::str("ctx.r") + py::to_str(ra) + ".u64",
                          py::to_str(simm) + "ll", py::str(std::to_string(static_cast<uint64_t>(simm) & U64)) + "ull");
    }

    if (_TW.contains(base_mn_s) || _TD.contains(base_mn_s)) {
        const char* w = _TW.contains(base_mn_s) ? "32" : "64";
        int64_t to = (WORD >> 21) & 0x1F;
        int64_t ra = (WORD >> 16) & 0x1F;
        int64_t rb = (WORD >> 11) & 0x1F;
        py::str sa = py::str("ctx.r") + py::to_str(ra), sb = py::str("ctx.r") + py::to_str(rb);
        return _emit_trap(to, sa + ".s" + w, sa + ".u" + w, sb + ".s" + w, sb + ".u" + w);
    }

    // -----------------------------------------------------------------------
    // Cache ops with a real effect: dcbz / dcbzl
    // -----------------------------------------------------------------------
    if (base_mn == "dcbz" || base_mn == "dcbzl") {
        Number a = _reg(ops[0]);
        Number b = _reg(ops[1]);
        py::str base_term = (!a || *a != 0) ? py::str("ctx.r") + num(a) + ".u32 + " : py::str("");
        if (base_mn == "dcbz")
            return lines({
                py::str("ea = (") + base_term + "ctx.r" + num(b) + ".u32) & ~31;",
                py::str("memset((void*)PPC_RAW_ADDR(ea), 0, 32);"),
            });
        return lines({
            py::str("ea = (") + base_term + "ctx.r" + num(b) + ".u32) & ~127;",
            py::str("memset((void*)PPC_RAW_ADDR(ea), 0, 128);"),
        });
    }

    // -----------------------------------------------------------------------
    // Move register
    // -----------------------------------------------------------------------
    if (base_mn == "mr") {
        Number d = _reg(ops[0]);
        Number s = _reg(ops[1]);
        py::list<py::str> out = lines({py::str("ctx.r") + num(d) + ".u64 = ctx.r" + num(s) + ".u64;"});
        if (rc) out.append(py::str("ctx.cr0.compare<int32_t>(ctx.r") + num(d) + ".s32, 0, ctx.xer);");
        return out;
    }

    // -----------------------------------------------------------------------
    // Move CR field
    // -----------------------------------------------------------------------
    if (base_mn == "mcrf") {
        Number d = _cr(ops[0]);
        Number a = _cr(ops[1]);
        return lines({py::str("ctx.cr") + num(d) + " = ctx.cr" + num(a) + ";"});
    }

    // -----------------------------------------------------------------------
    // Move From Special Registers
    // -----------------------------------------------------------------------
    if (base_mn == "mfxer") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".u64 = (ctx.xer.so << 31) | (ctx.xer.ov << 30) | (ctx.xer.ca << 29);"});
    }

    if (base_mn == "mfctr") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".u64 = ctx.ctr.u64;"});
    }

    if (base_mn == "mfcr") {
        Number d = _reg(ops[0]);
        py::list<py::str> out;
        for (int64_t i = 0; i < 32; ++i) {
            py::str op = i == 0 ? py::str("") : py::str("|");
            uint64_t mask = 1ull << (31 - i);
            out.append(py::str("ctx.r") + num(d) + ".u64 " + op + "= ctx.cr" + py::to_str(i / 4) + "." + _CRBIT[i % 4] +
                       " ? " + hex_(mask) + " : 0;");
        }
        return out;
    }

    if (base_mn == "mfocrf") {
        Number d = _reg(ops[0]);
        Number fxm_n = _imm(ops[1]);
        int64_t fxm = need(fxm_n);
        int64_t cr_field = 0;
        for (int64_t i = 0; i < 8; ++i) {
            if (fxm & (0x80 >> i)) {
                cr_field = i;
                break;
            }
        }
        int64_t bs = 28 - 4 * cr_field;
        py::str f = py::to_str(cr_field);
        return lines({
            py::str("ctx.r") + num(d) + ".u64 = (ctx.cr" + f + ".lt << " + py::to_str(bs + 3) + ") | " +
            "(ctx.cr" + f + ".gt << " + py::to_str(bs + 2) + ") | " +
            "(ctx.cr" + f + ".eq << " + py::to_str(bs + 1) + ") | " +
            "(ctx.cr" + f + ".so << " + py::to_str(bs) + ");"
        });
    }

    if (base_mn == "mflr") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".u64 = ctx.lr;"});
    }

    if (base_mn == "mfmsr") {
        Number d = _reg(ops[0]);
        return lines({
            py::str("std::atomic_thread_fence(std::memory_order_seq_cst);"),
            py::str("ctx.r") + num(d) + ".u64 = PPC_CHECK_GLOBAL_LOCK();",
        });
    }

    if (base_mn == "mffs") {
        Number d = _freg(ops[0]);
        return lines({py::str("ctx.f") + num(d) + ".u64 = ctx.fpscr.loadFromHost();"});
    }

    if (base_mn == "mftb") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".u64 = PPC_QUERY_TIMEBASE();"});
    }

    if (base_mn == "mftbu") {
        Number d = _reg(ops[0]);
        return lines({py::str("ctx.r") + num(d) + ".u64 = PPC_QUERY_TIMEBASE() >> 32;"});
    }

    // -----------------------------------------------------------------------
    // Move To Special Registers
    // -----------------------------------------------------------------------
    if (base_mn == "mtcr") {
        Number s = _reg(ops[0]);
        py::list<py::str> out;
        for (int64_t i = 0; i < 32; ++i) {
            uint64_t mask = 1ull << (31 - i);
            out.append(py::str("ctx.cr") + py::to_str(i / 4) + "." + _CRBIT[i % 4] + " = (ctx.r" + num(s) + ".u32 & " +
                       hex_(mask) + ") != 0;");
        }
        return out;
    }

    if (base_mn == "mtcrf" || base_mn == "mtocrf") {
        Number fxm_n = _imm(ops[0]);
        Number s = _reg(ops[1]);
        int64_t fxm = need(fxm_n);
        py::list<py::str> out;
        for (int64_t field = 0; field < 8; ++field) {
            if (fxm & (0x80 >> field)) {
                int64_t base_bit = 28 - 4 * field;
                for (int64_t b = 0; b < 4; ++b) {
                    uint64_t mask = 1ull << (base_bit + 3 - b);
                    out.append(py::str("ctx.cr") + py::to_str(field) + "." + _CRBIT[b] + " = (ctx.r" + num(s) + ".u32 & " +
                               hex_(mask) + ") != 0;");
                }
            }
        }
        return out;
    }

    if (base_mn == "mtctr") {
        Number s = _reg(ops[0]);
        return lines({py::str("ctx.ctr.u64 = ctx.r") + num(s) + ".u64;"});
    }

    if (base_mn == "mtlr") {
        Number s = _reg(ops[0]);
        return lines({py::str("ctx.lr = ctx.r") + num(s) + ".u64;"});
    }

    if (base_mn == "mtmsrd") {
        Number s = _reg(ops[0]);
        py::list<py::str> out = lines({
            py::str("std::atomic_thread_fence(std::memory_order_seq_cst);"),
            py::str("ctx.msr = (ctx.r") + num(s) + ".u32 & 0x8020) | (ctx.msr & ~0x8020);",
        });
        // R13 = enter global lock (disable interrupts); anything else = leave.
        out.append((s && *s == 13) ? py::str("PPC_ENTER_GLOBAL_LOCK();") : py::str("PPC_LEAVE_GLOBAL_LOCK();"));
        return out;
    }

    if (base_mn == "mtfsf") {
        Number fm_n = _imm(ops[0]);
        Number b = _freg(ops[1]);
        int64_t fm = need(fm_n);
        uint64_t mask = 0;
        for (int64_t j = 0; j < 8; ++j) {
            if (fm & (1ll << (7 - j))) mask |= 0xFull << (4 * j);
        }
        if (mask == 0xFFFFFFFFull) return lines({py::str("ctx.fpscr.storeFromGuest(ctx.f") + num(b) + ".u32);"});
        uint64_t nmask = (~mask) & U32;
        return lines({
            py::str("ctx.fpscr.storeFromGuest((ctx.fpscr.loadFromHost() & ") + hex8(nmask) + ") | " +
            "(ctx.f" + num(b) + ".u32 & " + hex8(mask) + "));"
        });
    }

    if (base_mn == "mtxer") {
        Number s = _reg(ops[0]);
        return lines({
            py::str("ctx.xer.so = (ctx.r") + num(s) + ".u64 & 0x80000000) != 0;",
            py::str("ctx.xer.ov = (ctx.r") + num(s) + ".u64 & 0x40000000) != 0;",
            py::str("ctx.xer.ca = (ctx.r") + num(s) + ".u64 & 0x20000000) != 0;",
        });
    }

    // -----------------------------------------------------------------------
    // Clear Left Doubleword Immediate (simplified rldicl)
    // -----------------------------------------------------------------------
    if (base_mn == "clrldi") {
        Number d = _reg(ops[0]);
        Number s = _reg(ops[1]);
        int64_t n = need(_imm(ops[2]));
        // mask = ((1 << (64 - n)) - 1) & U64
        uint64_t mask;
        if (n > 64) throw py::ValueError("negative shift count");
        else if (n <= 0) mask = U64;                       // 64 - n >= 64: every low bit is set
        else mask = (1ull << (64 - n)) - 1;                // n in 1..64 (n == 64 gives 0)
        py::list<py::str> out = lines({py::str("ctx.r") + num(d) + ".u64 = ctx.r" + num(s) + ".u64 & " + hex_(mask) + ";"});
        if (rc) out.append(py::str("ctx.cr0.compare<int32_t>(ctx.r") + num(d) + ".s32, 0, ctx.xer);");
        return out;
    }

    // -----------------------------------------------------------------------
    // CR-logical bit operations (from logical.cpp, this category)
    // -----------------------------------------------------------------------
    if (_CRLOGICAL.contains(base_mn_s)) {
        const CrLogical& c = _CRLOGICAL.at(base_mn_s);
        return _emit_crbit(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c), _cr, ops);
    }

    return std::nullopt;
}

const py::str RUNTIME_HELPERS = py::str("");

const py::list<py::str> MNEMONICS = {
    // No-ops / barriers / Xenon hints / cache hints
    py::str("nop"), py::str("attn"), py::str("sync"), py::str("lwsync"), py::str("eieio"), py::str("db16cyc"),
    py::str("cctpl"), py::str("cctpm"), py::str("cctph"),
    py::str("dcbf"), py::str("dcbt"), py::str("dcbtst"), py::str("dcbst"),
    // Cache ops with effect
    py::str("dcbz"), py::str("dcbzl"),
    // Move register / CR field
    py::str("mr"), py::str("mcrf"),
    // Move-from SPR
    py::str("mfxer"), py::str("mfctr"), py::str("mfcr"), py::str("mfocrf"), py::str("mflr"), py::str("mfmsr"), py::str("mffs"), py::str("mftb"), py::str("mftbu"),
    // Move-to SPR
    py::str("mtcr"), py::str("mtcrf"), py::str("mtocrf"), py::str("mtctr"), py::str("mtlr"), py::str("mtmsrd"), py::str("mtfsf"), py::str("mtxer"),
    // Clear-left doubleword immediate
    py::str("clrldi"),
    // CR-logical
    py::str("crand"), py::str("crandc"), py::str("creqv"), py::str("crnand"), py::str("crnor"), py::str("cror"), py::str("crorc"), py::str("crxor"),
    // Trap: base + all simplified conditional forms
    py::str("twi"), py::str("twlgti"), py::str("twllti"), py::str("tweqi"), py::str("twlgei"), py::str("twlnli"), py::str("twllei"),
    py::str("twlngi"), py::str("twgti"), py::str("twgei"), py::str("twnli"), py::str("twlti"), py::str("twlei"), py::str("twngi"), py::str("twnei"),
    py::str("tdi"), py::str("tdlgti"), py::str("tdllti"), py::str("tdeqi"), py::str("tdlgei"), py::str("tdlnli"), py::str("tdllei"),
    py::str("tdlngi"), py::str("tdgti"), py::str("tdgei"), py::str("tdnli"), py::str("tdlti"), py::str("tdlei"), py::str("tdngi"), py::str("tdnei"),
    py::str("tw"), py::str("twge"), py::str("twgt"), py::str("twle"), py::str("twlt"), py::str("tweq"), py::str("twne"),
    py::str("twlge"), py::str("twlgt"), py::str("twlle"), py::str("twllt"),
    py::str("td"), py::str("tdge"), py::str("tdgt"), py::str("tdle"), py::str("tdlt"), py::str("tdeq"), py::str("tdne"),
    py::str("tdlge"), py::str("tdlgt"), py::str("tdlle"), py::str("tdllt"),
};

}  // namespace codebreaker::recompile::builders::system
