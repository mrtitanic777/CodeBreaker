"""floating.py -- scalar floating point: arithmetic, multiply-add, sign, conversion, compare, select, as C++ for the recompiler (recompile/powerpc.py).

Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
when the mnemonic is not this file's. Registers are always written as the context's fields
(ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
recompiled function declares.
Every builder first emits `ctx.fpscr.disableFlushMode();`. Values are ctx.fN.f64, raw bits
ctx.fN.u64; single-precision results are rounded through double(float(...)). The floating-point
loads and stores are memory.py's, mffs/mtfsf system.py's.

Derived from the rexglue SDK's code generator (rexcodegen/builders/floating_point.cpp; BSD-3-Clause,
Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
"""
def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):
    # All scalar FP ops first disable flush mode (FPU mode: denormals preserved).
    FLUSH = "ctx.fpscr.disableFlushMode();"

    # ---- Sign Manipulation -------------------------------------------------
    if mn == "fabs":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.u64 = ctx.f{a}.u64 & ~0x8000000000000000;"]
    if mn == "fnabs":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.u64 = ctx.f{a}.u64 | 0x8000000000000000;"]
    if mn == "fneg":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.u64 = ctx.f{a}.u64 ^ 0x8000000000000000;"]

    # ---- Move and Conversion ----------------------------------------------
    if mn == "fmr":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64;"]
    if mn == "fcfid":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = double(ctx.f{a}.s64);"]
    if mn == "fctid":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH,
                f"ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x8000000000000000ULL) : "
                f"(ctx.f{a}.f64 > double(LLONG_MAX)) ? LLONG_MAX : "
                f"simde_mm_cvtsd_si64(simde_mm_load_sd(&ctx.f{a}.f64));"]
    if mn == "fctidz":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH,
                f"ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x8000000000000000ULL) : "
                f"(ctx.f{a}.f64 > double(LLONG_MAX)) ? LLONG_MAX : "
                f"simde_mm_cvttsd_si64(simde_mm_load_sd(&ctx.f{a}.f64));"]
    if mn == "fctiwz":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH,
                f"ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x80000000U) : "
                f"(ctx.f{a}.f64 > double(INT_MAX)) ? INT_MAX : "
                f"simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f{a}.f64));"]
    if mn == "frsp":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64));"]

    # ---- Comparison --------------------------------------------------------
    # fcmpu: operands[0]=crfD, operands[1]=fA, operands[2]=fB.
    # fcmpo is translated as fcmpu (the same statements).
    if mn == "fcmpu" or mn == "fcmpo":
        c, a, b = _cr(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.cr{c}.compare(ctx.f{a}.f64, ctx.f{b}.f64);"]

    # ---- Addition ----------------------------------------------------------
    if mn == "fadd":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 + ctx.f{b}.f64;"]
    if mn == "fadds":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 + ctx.f{b}.f64));"]

    # ---- Subtraction -------------------------------------------------------
    if mn == "fsub":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 - ctx.f{b}.f64;"]
    if mn == "fsubs":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 - ctx.f{b}.f64));"]

    # ---- Multiplication ----------------------------------------------------
    if mn == "fmul":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{b}.f64;"]
    if mn == "fmuls":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{b}.f64));"]

    # ---- Division ----------------------------------------------------------
    if mn == "fdiv":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 / ctx.f{b}.f64;"]
    if mn == "fdivs":
        d, a, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 / ctx.f{b}.f64));"]

    # ---- Fused Multiply-Add ------------------------------------------------
    # operands[1]*operands[2] (+/-) operands[3]  (index order reproduced verbatim)
    if mn == "fmadd":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64;"]
    if mn == "fmadds":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64));"]
    if mn == "fmsub":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64;"]
    if mn == "fmsubs":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64));"]
    if mn == "fnmadd":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = -(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64);"]
    if mn == "fnmadds":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(-(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64)));"]
    if mn == "fnmsub":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = -(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64);"]
    if mn == "fnmsubs":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(-(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64)));"]

    # ---- Reciprocal and Square Root ---------------------------------------
    if mn == "fres":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(1.0 / ctx.f{a}.f64));"]
    if mn == "frsqrte":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = 1.0 / sqrt(ctx.f{a}.f64);"]
    if mn == "fsqrt":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = sqrt(ctx.f{a}.f64);"]
    if mn == "fsqrts":
        d, a = _freg(ops[0]), _freg(ops[1])
        return [FLUSH, f"ctx.f{d}.f64 = double(float(sqrt(ctx.f{a}.f64)));"]

    # ---- Selection ---------------------------------------------------------
    # operands[1] >= 0.0 ? operands[2] : operands[3]
    if mn == "fsel":
        d, a, c, b = _freg(ops[0]), _freg(ops[1]), _freg(ops[2]), _freg(ops[3])
        return [FLUSH, f"ctx.f{d}.f64 = ctx.f{a}.f64 >= 0.0 ? ctx.f{c}.f64 : ctx.f{b}.f64;"]

    return None


# Standard headers relied on by the emitted C++ (isnan/sqrt from <cmath>,
# LLONG_MAX/INT_MAX from <climits>). simde_mm_* are standard simde intrinsics
# already available in the recomp prelude. No custom (ppc_vperm-style) helper
# is required for this category.
RUNTIME_HELPERS = r'''#include <cmath>
#include <climits>
'''


MNEMONICS = [
    "fabs", "fnabs", "fneg",
    "fmr", "fcfid", "fctid", "fctidz", "fctiwz", "frsp",
    "fcmpu", "fcmpo",
    "fadd", "fadds",
    "fsub", "fsubs",
    "fmul", "fmuls",
    "fdiv", "fdivs",
    "fmadd", "fmadds", "fmsub", "fmsubs",
    "fnmadd", "fnmadds", "fnmsub", "fnmsubs",
    "fres", "frsqrte", "fsqrt", "fsqrts",
    "fsel",
]


# Elsewhere: the floating-point loads and stores (memory.py), mffs/mtfsf (system.py).
# Not translated: fctiw (only fctiwz has a builder).
