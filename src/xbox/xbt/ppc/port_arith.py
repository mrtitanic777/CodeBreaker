# -*- coding: utf-8 -*-
# ============================================================================
# Faithful port of RexGlue codegen builders:
#   rexcodegen/builders/arithmetic.cpp   (build_* for ARITHMETIC)
#   rexcodegen/builders/comparison.cpp   (build_* for COMPARISON)
#   (staged at /tmp/cg/builders/arithmetic.cpp, /tmp/cg/builders/comparison.cpp)
# Emission helpers referenced: /tmp/cg/builders/context.cpp,
#   /tmp/cg/builders/helpers.h, /tmp/cg/builders/builder_context.h
# Opcode->builder map: /tmp/cg/instruction_dispatch.cpp
#
# Emission rules followed (per PORT_BRIEF.md + task):
#   - ctx.r(i)  -> "ctx.rN"   (the C++ local-variable optimization in
#     BuilderContext::r/f/v/cr is ignored; ALWAYS ctx.rN).
#   - ctx.xer() -> "ctx.xer".  ctx.cr(i) -> "ctx.crN".
#   - ctx.temp() emits the token "temp" (a function-local PPCRegister the
#     integrator declares as `PPCRegister temp{};`). So we emit "temp.u8" /
#     "temp.s64" / "temp.u32" exactly where the C++ uses ctx.temp().
#   - The leading '\t' in the C++ println() format strings is dropped (the
#     recompiler framework re-indents). One C++ statement per list item.
#
# RECORD FORMS (important): EVERY record-capable builder in these two files
#   updates CR0 by calling emitRecordFormCompare(ctx) (helpers.h), which emits
#   exactly:
#       ctx.cr0.compare<int32_t>(ctx.rD.s32, 0, ctx.xer);
#   That is the GENERIC record-form wrapper the integrator already provides
#   (it strips the trailing '.' and appends that very line). Per the task, we
#   therefore emit ONLY THE BASE OP here and let the integrator's wrapper add
#   the CR0 compare -- we do NOT emit the compare ourselves (doing so would
#   double-emit it). We still strip a trailing '.' defensively so a dotted
#   mnemonic ("add.", "subfc.", ...) resolves to its base handler if the
#   wrapper passes it through unstripped.
#
#   Builders that DO carry a record form (all route through the generic
#   wrapper): add, adde, addic, addze, addme, addc, divd, divdu, divw, divwu,
#   mulhw, mulhwu, mulld, mullw, mulhd, mulhdu, neg, subf, subfc, subfe,
#   subfze, subfme.
#   Builders WITHOUT a record form (no emitRecordFormCompare call): addi,
#   addis, mulli, subfic, and all comparisons.
#
# Comparison builders use the emitCompareRegister / emitCompareImmediate
# helpers (helpers.h): crD.compare<T>(rA.field, rB.field|imm, ctx.xer).
# Operand layout matches the C++ decode: operands[0]=CR field (BF),
# operands[1]=rA, operands[2]=rB or the immediate.  (If the disassembler ever
# omits an implicit cr0 operand, that is an upstream _split/_cr concern shared
# by every CR-consuming instruction; this port assumes ops[0] is the CR field,
# exactly as the C++ reads ctx.insn.operands[0].)
# ============================================================================


def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):

    def i32(x):
        x &= 0xFFFFFFFF
        return x - 0x100000000 if x & 0x80000000 else x

    def u32(x):
        return x & 0xFFFFFFFF

    # Record-form '.' is handled by the integrator's generic wrapper; strip it
    # defensively and NEVER emit the CR0 compare here (see header note).
    base_mn = mn[:-1] if mn.endswith(".") else mn

    # =======================================================================
    # ARITHMETIC (arithmetic.cpp)
    # =======================================================================

    # ---- Addition --------------------------------------------------------
    if base_mn == "add":  # build_add
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64;"]

    if base_mn == "adde":  # build_adde
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"temp.u8 = (ctx.r{a}.u32 + ctx.r{b}.u32 < ctx.r{a}.u32) | "
            f"(ctx.r{a}.u32 + ctx.r{b}.u32 + ctx.xer.ca < ctx.xer.ca);",
            f"ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64 + ctx.xer.ca;",
            "ctx.xer.ca = temp.u8;",
        ]

    if base_mn == "addi":  # build_addi  (no record form)
        d, a = _reg(ops[0]), _reg(ops[1])
        simm = i32(_imm(ops[2]))
        lhs = f"ctx.r{a}.s64 + " if a != 0 else ""
        return [f"ctx.r{d}.s64 = {lhs}{simm};"]

    if base_mn == "addic":  # build_addic
        d, a = _reg(ops[0]), _reg(ops[1])
        simm = i32(_imm(ops[2]))
        notimm = (~simm) & 0xFFFFFFFF          # C++: ~operands[2] (uint32_t, unsigned print)
        return [
            f"ctx.xer.ca = ctx.r{a}.u32 > {notimm};",
            f"ctx.r{d}.s64 = ctx.r{a}.s64 + {simm};",
        ]

    if base_mn == "addis":  # build_addis  (no record form)
        d, a = _reg(ops[0]), _reg(ops[1])
        imm = i32(_imm(ops[2]) << 16)          # C++: static_cast<int32_t>(operands[2] << 16)
        lhs = f"ctx.r{a}.s64 + " if a != 0 else ""
        return [f"ctx.r{d}.s64 = {lhs}{imm};"]

    if base_mn == "addze":  # build_addze
        d, a = _reg(ops[0]), _reg(ops[1])
        return [
            f"temp.s64 = ctx.r{a}.s64 + ctx.xer.ca;",
            f"ctx.xer.ca = temp.u32 < ctx.r{a}.u32;",
            f"ctx.r{d}.s64 = temp.s64;",
        ]

    if base_mn == "addme":  # build_addme
        d, a = _reg(ops[0]), _reg(ops[1])
        return [
            f"temp.u8 = (ctx.r{a}.u32 + 0xFFFFFFFFu < ctx.r{a}.u32) | "
            f"(ctx.r{a}.u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);",
            f"ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;",
            "ctx.xer.ca = temp.u8;",
        ]

    if base_mn == "addc":  # build_addc
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.xer.ca = ctx.r{a}.u32 + ctx.r{b}.u32 < ctx.r{a}.u32;",
            f"ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64;",
        ]

    # ---- Division (safe: 0 on div-by-zero / INT_MIN/-1) ------------------
    if base_mn == "divd":  # build_divd
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.r{d}.s64 = (ctx.r{b}.s64 && !(ctx.r{a}.s64 == INT64_MIN && "
            f"ctx.r{b}.s64 == -1)) ? ctx.r{a}.s64 / ctx.r{b}.s64 : 0;"
        ]

    if base_mn == "divdu":  # build_divdu
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ctx.r{b}.u64 ? ctx.r{a}.u64 / ctx.r{b}.u64 : 0;"]

    if base_mn == "divw":  # build_divw
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.r{d}.u64 = uint32_t((ctx.r{b}.s32 && !(ctx.r{a}.s32 == INT32_MIN && "
            f"ctx.r{b}.s32 == -1)) ? ctx.r{a}.s32 / ctx.r{b}.s32 : 0);"
        ]

    if base_mn == "divwu":  # build_divwu
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = uint32_t(ctx.r{b}.u32 ? ctx.r{a}.u32 / ctx.r{b}.u32 : 0);"]

    # ---- Multiplication --------------------------------------------------
    if base_mn == "mulhw":  # build_mulhw
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.s64 = (int64_t(ctx.r{a}.s32) * int64_t(ctx.r{b}.s32)) >> 32;"]

    if base_mn == "mulhwu":  # build_mulhwu
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = (uint64_t(ctx.r{a}.u32) * uint64_t(ctx.r{b}.u32)) >> 32;"]

    if base_mn == "mulld":  # build_mulld
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.s64 = static_cast<int64_t>(ctx.r{a}.u64 * ctx.r{b}.u64);"]

    if base_mn == "mulli":  # build_mulli  (no record form)
        d, a = _reg(ops[0]), _reg(ops[1])
        simm = i32(_imm(ops[2]))
        return [
            f"ctx.r{d}.s64 = static_cast<int64_t>(ctx.r{a}.u64 * "
            f"static_cast<uint64_t>({simm}));"
        ]

    if base_mn == "mullw":  # build_mullw
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.s64 = int64_t(ctx.r{a}.s32) * int64_t(ctx.r{b}.s32);"]

    if base_mn == "mulhd":  # build_mulhd
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.r{d}.s64 = static_cast<int64_t>((static_cast<__int128>("
            f"static_cast<int64_t>(ctx.r{a}.s64)) * static_cast<__int128>("
            f"static_cast<int64_t>(ctx.r{b}.s64))) >> 64);"
        ]

    if base_mn == "mulhdu":  # build_mulhdu
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.r{d}.u64 = static_cast<uint64_t>((static_cast<__uint128_t>(ctx.r{a}.u64) * "
            f"static_cast<__uint128_t>(ctx.r{b}.u64)) >> 64);"
        ]

    # ---- Negation --------------------------------------------------------
    if base_mn == "neg":  # build_neg
        d, a = _reg(ops[0]), _reg(ops[1])
        return [f"ctx.r{d}.s64 = static_cast<int64_t>(-ctx.r{a}.u64);"]

    # ---- Subtraction (subtract-from: rD = rB - rA) -----------------------
    if base_mn == "subf":  # build_subf
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.r{d}.u64 = ctx.r{b}.u64 - ctx.r{a}.u64;"]

    if base_mn == "subfc":  # build_subfc
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"ctx.xer.ca = ctx.r{b}.u32 >= ctx.r{a}.u32;",
            f"ctx.r{d}.u64 = ctx.r{b}.u64 - ctx.r{a}.u64;",
        ]

    if base_mn == "subfe":  # build_subfe
        d, a, b = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"temp.u8 = (~ctx.r{a}.u32 + ctx.r{b}.u32 < ~ctx.r{a}.u32) | "
            f"(~ctx.r{a}.u32 + ctx.r{b}.u32 + ctx.xer.ca < ctx.xer.ca);",
            f"ctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.r{b}.u64 + ctx.xer.ca;",
            "ctx.xer.ca = temp.u8;",
        ]

    if base_mn == "subfic":  # build_subfic  (no record form)
        d, a = _reg(ops[0]), _reg(ops[1])
        simm = i32(_imm(ops[2]))
        return [
            f"ctx.xer.ca = ctx.r{a}.u32 <= {u32(simm)};",      # C++: <= operands[2] (uint32_t)
            f"ctx.r{d}.u64 = static_cast<uint64_t>({simm}) - ctx.r{a}.u64;",
        ]

    if base_mn == "subfze":  # build_subfze
        d, a = _reg(ops[0]), _reg(ops[1])
        return [
            f"temp.u8 = ~ctx.r{a}.u32 + ctx.xer.ca < ~ctx.r{a}.u32;",
            f"ctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.xer.ca;",
            "ctx.xer.ca = temp.u8;",
        ]

    if base_mn == "subfme":  # build_subfme
        d, a = _reg(ops[0]), _reg(ops[1])
        return [
            f"temp.u8 = (~ctx.r{a}.u32 + 0xFFFFFFFFu < ~ctx.r{a}.u32) | "
            f"(~ctx.r{a}.u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);",
            f"ctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;",
            "ctx.xer.ca = temp.u8;",
        ]

    # =======================================================================
    # COMPARISON (comparison.cpp)  -- crD.compare<T>(rA.field, rB.field|imm, xer)
    # =======================================================================

    # emitCompareRegister(ctx, type, field): reg-to-reg
    _CMP_REG = {
        "cmpd":  ("int64_t",  "s64"),
        "cmpld": ("uint64_t", "u64"),
        "cmplw": ("uint32_t", "u32"),
        "cmpw":  ("int32_t",  "s32"),
    }
    if base_mn in _CMP_REG:
        typ, fld = _CMP_REG[base_mn]
        cr, a, b = _cr(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"ctx.cr{cr}.compare<{typ}>(ctx.r{a}.{fld}, ctx.r{b}.{fld}, ctx.xer);"]

    # emitCompareImmediate(ctx, type, field, sign_extend): reg-to-imm
    #   sign_extend True  -> imm = static_cast<int32_t>(operands[2])  (signed print)
    #   sign_extend False -> imm = operands[2]                        (uint32_t print)
    _CMP_IMM = {
        "cmpdi":  ("int64_t",  "s64", True),
        "cmpldi": ("uint64_t", "u64", False),
        "cmplwi": ("uint32_t", "u32", False),
        "cmpwi":  ("int32_t",  "s32", True),
    }
    if base_mn in _CMP_IMM:
        typ, fld, sx = _CMP_IMM[base_mn]
        cr, a = _cr(ops[0]), _reg(ops[1])
        imm = i32(_imm(ops[2])) if sx else u32(_imm(ops[2]))
        return [f"ctx.cr{cr}.compare<{typ}>(ctx.r{a}.{fld}, {imm}, ctx.xer);"]

    return None


# ---------------------------------------------------------------------------
# No non-simde runtime helpers are needed: arithmetic.cpp / comparison.cpp
# emit only plain C++ arithmetic plus the CRRegister::compare<T> method and the
# INT64_MIN / INT32_MIN macros (all provided by the recompiler runtime /
# <cstdint>).  __int128 / __uint128_t are compiler builtins.
# ---------------------------------------------------------------------------
RUNTIME_HELPERS = ""


# Every mnemonic handled above (base forms; the integrator's generic record-form
# wrapper appends the CR0 compare for the '.' variants of the record-capable
# ones, so add./adde./addc./addze./addme./adde./divd./.../subfme. resolve here
# via the trailing-'.' strip).
MNEMONICS = [
    # addition
    "add", "adde", "addi", "addic", "addis", "addze", "addme", "addc",
    # division
    "divd", "divdu", "divw", "divwu",
    # multiplication
    "mulhw", "mulhwu", "mulld", "mulli", "mullw", "mulhd", "mulhdu",
    # negation
    "neg",
    # subtraction
    "subf", "subfc", "subfe", "subfic", "subfze", "subfme",
    # comparison (reg-reg)
    "cmpd", "cmpld", "cmplw", "cmpw",
    # comparison (reg-imm)
    "cmpdi", "cmpldi", "cmplwi", "cmpwi",
]


# ---------------------------------------------------------------------------
# Not ported (with reason).
# ---------------------------------------------------------------------------
UNPORTED = [
    # Every build_* in arithmetic.cpp and comparison.cpp IS ported faithfully.
    # The two notes below record deliberate/structural omissions, not failures.
    "record-form CR0 update line -- deliberately NOT emitted here. Every "
    "record-capable builder calls emitRecordFormCompare(ctx) which emits exactly "
    "`ctx.cr0.compare<int32_t>(ctx.rD.s32, 0, ctx.xer);`. That is the integrator's "
    "generic '.'-stripping wrapper (it appends that very line), so emitting it here "
    "too would double it. The BASE op of every '.' form IS ported; only the "
    "wrapper-supplied CR0 line is omitted.",
    "overflow-enabled OE forms (trailing 'o'/'o.': addo, addco, addeo, addmeo, "
    "addzeo, subfo, subfco, subfeo, subfmeo, subfzeo, nego, divdo, divduo, divwo, "
    "divwuo, mulldo, mullwo, ...) -- no build_* exists in arithmetic.cpp and no "
    "PPC_INST_*O -> build_* entry exists in instruction_dispatch.cpp (only the base "
    "opcodes are registered). The source never emits XER.OV for these, so there is "
    "nothing to port; the integrator strips only '.', not 'o', so 'addo' correctly "
    "does not fold onto 'add'.",
]
