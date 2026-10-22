"""system.py -- special registers, traps, barriers, the CR-bit logical operations and the other system instructions, as C++ for the recompiler (recompile/powerpc.py).

Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
when the mnemonic is not this file's. Registers are always written as the context's fields
(ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
recompiled function declares.
Traps (tdi/twi/td/tw and every simplified form, tweqi ...) decode TO, RA, RB and SIMM from the
raw instruction word (WORD), so all their spellings reach one builder. Nops, barriers (the host's
ordering is strong) and cache hints emit nothing. The runtime provides what the statements call:
ppc_trap, PPC_QUERY_TIMEBASE, PPC_{CHECK,ENTER,LEAVE}_GLOBAL_LOCK, PPC_RAW_ADDR.

Derived from the rexglue SDK's code generator (rexcodegen/builders/system.cpp, logical.cpp's CR-bit operations; BSD-3-Clause,
Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
"""
U64 = 0xFFFFFFFFFFFFFFFF
U32 = 0xFFFFFFFF

# CR bit-field member names, indexed by (bit index % 4). Matches crBitName().
_CRBIT = ("lt", "gt", "eq", "so")

# Trap mnemonic groups -> shared builder (build_twi/tdi/tw/td). All decode from
# the raw word, so the exact simplified form does not matter.
_TWI = {
    "twi", "twlgti", "twllti", "tweqi", "twlgei", "twlnli", "twllei",
    "twlngi", "twgti", "twgei", "twnli", "twlti", "twlei", "twngi", "twnei",
}
_TDI = {
    "tdi", "tdlgti", "tdllti", "tdeqi", "tdlgei", "tdlnli", "tdllei",
    "tdlngi", "tdgti", "tdgei", "tdnli", "tdlti", "tdlei", "tdngi", "tdnei",
}
_TW = {
    "tw", "twge", "twgt", "twle", "twlt", "tweq", "twne",
    "twlge", "twlgt", "twlle", "twllt",
}
_TD = {
    "td", "tdge", "tdgt", "tdle", "tdlt", "tdeq", "tdne",
    "tdlge", "tdlgt", "tdlle", "tdllt",
}

# No-effect instructions (builder returns true, emits nothing): canonical nops,
# memory barriers (x86 strong ordering), Xenon-specific hints, and cache hints.
_NOP_MNEMONICS = {
    "nop", "attn", "sync", "lwsync", "eieio", "db16cyc",
    "cctpl", "cctpm", "cctph",
    "dcbf", "dcbt", "dcbtst", "dcbst",
}

# CR-logical: mnemonic -> (op, invertA, invertB, invertResult) for emitCRBitOperation.
_CRLOGICAL = {
    "crand":  ("&", False, False, False),
    "crandc": ("&", False, True,  False),
    "creqv":  ("^", False, False, True),
    "crnand": ("&", False, False, True),
    "crnor":  ("|", False, False, True),
    "cror":   ("|", False, False, False),
    "crorc":  ("|", False, True,  False),
    "crxor":  ("^", False, False, False),
}


def _emit_trap(to, a_signed, a_unsigned, b_signed, b_unsigned):
    """Port of emitTrap() (helpers.h)."""
    if to == 0:
        return []
    if to == 0x1F:
        return ["ppc_trap(ctx, base, 0);"]
    parts = []
    if to & 0x10:
        parts.append(f"{a_signed} < {b_signed}")
    if to & 0x08:
        parts.append(f"{a_signed} > {b_signed}")
    if to & 0x04:
        parts.append(f"{a_signed} == {b_signed}")
    if to & 0x02:
        parts.append(f"{a_unsigned} < {b_unsigned}")
    if to & 0x01:
        parts.append(f"{a_unsigned} > {b_unsigned}")
    cond = " || ".join(parts)
    return [f"if ({cond}) ppc_trap(ctx, base, 0);"]


def _emit_crbit(op, inv_a, inv_b, inv_res, _cr, ops):
    """Port of emitCRBitOperation() (helpers.h). operands are CR *bit* indices."""
    crd = _cr(ops[0])
    cra = _cr(ops[1])
    crb = _cr(ops[2])
    a_expr = f"ctx.cr{cra // 4}.{_CRBIT[cra % 4]}"
    b_expr = f"ctx.cr{crb // 4}.{_CRBIT[crb % 4]}"
    if inv_a:
        a_expr = "!(" + a_expr + ")"
    if inv_b:
        b_expr = "!(" + b_expr + ")"
    expr = f"{a_expr} {op} {b_expr}"
    if inv_res:
        expr = "!(" + expr + ")"
    return [f"ctx.cr{crd // 4}.{_CRBIT[crd % 4]} = {expr};"]


def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):
    # Record-form: only build_mr and build_clrldi honor the '.' bit (emit a CR0
    # compare). Strip a trailing '.' to get the base mnemonic; nothing else in
    # this category legitimately ends with '.'.
    rc = mn.endswith(".")
    base_mn = mn[:-1] if rc else mn

    # -----------------------------------------------------------------------
    # No-ops / barriers / hints (emit nothing)
    # -----------------------------------------------------------------------
    if base_mn in _NOP_MNEMONICS:
        return []

    # -----------------------------------------------------------------------
    # Trap instructions (decode from the raw word)
    # -----------------------------------------------------------------------
    if base_mn in _TWI:
        to = (WORD >> 21) & 0x1F
        ra = (WORD >> 16) & 0x1F
        simm = WORD & 0xFFFF
        if simm & 0x8000:               # sign-extend 16 -> 32
            simm -= 0x10000
        # twi 31, r0, <imm> == unconditional service trap with code in imm
        if to == 0x1F and ra == 0:
            trap_type = simm & 0xFFFF   # (uint16_t)simm
            return [f"ppc_trap(ctx, base, {trap_type});"]
        return _emit_trap(
            to,
            f"ctx.r{ra}.s32", f"ctx.r{ra}.u32",
            f"{simm}", f"{simm & U32}u",
        )

    if base_mn in _TDI:
        to = (WORD >> 21) & 0x1F
        ra = (WORD >> 16) & 0x1F
        simm = WORD & 0xFFFF
        if simm & 0x8000:               # sign-extend 16 -> 64
            simm -= 0x10000
        return _emit_trap(
            to,
            f"ctx.r{ra}.s64", f"ctx.r{ra}.u64",
            f"{simm}ll", f"{simm & U64}ull",
        )

    if base_mn in _TW:
        to = (WORD >> 21) & 0x1F
        ra = (WORD >> 16) & 0x1F
        rb = (WORD >> 11) & 0x1F
        return _emit_trap(
            to,
            f"ctx.r{ra}.s32", f"ctx.r{ra}.u32",
            f"ctx.r{rb}.s32", f"ctx.r{rb}.u32",
        )

    if base_mn in _TD:
        to = (WORD >> 21) & 0x1F
        ra = (WORD >> 16) & 0x1F
        rb = (WORD >> 11) & 0x1F
        return _emit_trap(
            to,
            f"ctx.r{ra}.s64", f"ctx.r{ra}.u64",
            f"ctx.r{rb}.s64", f"ctx.r{rb}.u64",
        )

    # -----------------------------------------------------------------------
    # Cache ops with a real effect: dcbz / dcbzl
    # -----------------------------------------------------------------------
    if base_mn == "dcbz":
        a = _reg(ops[0])
        b = _reg(ops[1])
        base_term = f"ctx.r{a}.u32 + " if a != 0 else ""
        return [
            f"ea = ({base_term}ctx.r{b}.u32) & ~31;",
            "memset((void*)PPC_RAW_ADDR(ea), 0, 32);",
        ]

    if base_mn == "dcbzl":
        a = _reg(ops[0])
        b = _reg(ops[1])
        base_term = f"ctx.r{a}.u32 + " if a != 0 else ""
        return [
            f"ea = ({base_term}ctx.r{b}.u32) & ~127;",
            "memset((void*)PPC_RAW_ADDR(ea), 0, 128);",
        ]

    # -----------------------------------------------------------------------
    # Move register
    # -----------------------------------------------------------------------
    if base_mn == "mr":
        d = _reg(ops[0])
        s = _reg(ops[1])
        out = [f"ctx.r{d}.u64 = ctx.r{s}.u64;"]
        if rc:
            out.append(f"ctx.cr0.compare<int32_t>(ctx.r{d}.s32, 0, ctx.xer);")
        return out

    # -----------------------------------------------------------------------
    # Move CR field
    # -----------------------------------------------------------------------
    if base_mn == "mcrf":
        d = _cr(ops[0])
        a = _cr(ops[1])
        return [f"ctx.cr{d} = ctx.cr{a};"]

    # -----------------------------------------------------------------------
    # Move From Special Registers
    # -----------------------------------------------------------------------
    if base_mn == "mfxer":
        d = _reg(ops[0])
        return [f"ctx.r{d}.u64 = (ctx.xer.so << 31) | (ctx.xer.ov << 30) | (ctx.xer.ca << 29);"]

    if base_mn == "mfctr":
        d = _reg(ops[0])
        return [f"ctx.r{d}.u64 = ctx.ctr.u64;"]

    if base_mn == "mfcr":
        d = _reg(ops[0])
        out = []
        for i in range(32):
            op = "" if i == 0 else "|"
            mask = 1 << (31 - i)
            out.append(f"ctx.r{d}.u64 {op}= ctx.cr{i // 4}.{_CRBIT[i % 4]} ? 0x{mask:X} : 0;")
        return out

    if base_mn == "mfocrf":
        d = _reg(ops[0])
        fxm = _imm(ops[1])
        cr_field = 0
        for i in range(8):
            if fxm & (0x80 >> i):
                cr_field = i
                break
        bs = 28 - 4 * cr_field
        return [
            f"ctx.r{d}.u64 = (ctx.cr{cr_field}.lt << {bs + 3}) | "
            f"(ctx.cr{cr_field}.gt << {bs + 2}) | "
            f"(ctx.cr{cr_field}.eq << {bs + 1}) | "
            f"(ctx.cr{cr_field}.so << {bs});"
        ]

    if base_mn == "mflr":
        d = _reg(ops[0])
        return [f"ctx.r{d}.u64 = ctx.lr;"]

    if base_mn == "mfmsr":
        d = _reg(ops[0])
        return [
            "std::atomic_thread_fence(std::memory_order_seq_cst);",
            f"ctx.r{d}.u64 = PPC_CHECK_GLOBAL_LOCK();",
        ]

    if base_mn == "mffs":
        d = _freg(ops[0])
        return [f"ctx.f{d}.u64 = ctx.fpscr.loadFromHost();"]

    if base_mn == "mftb":
        d = _reg(ops[0])
        return [f"ctx.r{d}.u64 = PPC_QUERY_TIMEBASE();"]

    if base_mn == "mftbu":
        d = _reg(ops[0])
        return [f"ctx.r{d}.u64 = PPC_QUERY_TIMEBASE() >> 32;"]

    # -----------------------------------------------------------------------
    # Move To Special Registers
    # -----------------------------------------------------------------------
    if base_mn == "mtcr":
        s = _reg(ops[0])
        out = []
        for i in range(32):
            mask = 1 << (31 - i)
            out.append(f"ctx.cr{i // 4}.{_CRBIT[i % 4]} = (ctx.r{s}.u32 & 0x{mask:X}) != 0;")
        return out

    if base_mn in ("mtcrf", "mtocrf"):
        fxm = _imm(ops[0])
        s = _reg(ops[1])
        out = []
        for field in range(8):
            if fxm & (0x80 >> field):
                base_bit = 28 - 4 * field
                for b in range(4):
                    mask = 1 << (base_bit + 3 - b)
                    out.append(
                        f"ctx.cr{field}.{_CRBIT[b]} = (ctx.r{s}.u32 & 0x{mask:X}) != 0;"
                    )
        return out

    if base_mn == "mtctr":
        s = _reg(ops[0])
        return [f"ctx.ctr.u64 = ctx.r{s}.u64;"]

    if base_mn == "mtlr":
        s = _reg(ops[0])
        return [f"ctx.lr = ctx.r{s}.u64;"]

    if base_mn == "mtmsrd":
        s = _reg(ops[0])
        out = [
            "std::atomic_thread_fence(std::memory_order_seq_cst);",
            f"ctx.msr = (ctx.r{s}.u32 & 0x8020) | (ctx.msr & ~0x8020);",
        ]
        # R13 = enter global lock (disable interrupts); anything else = leave.
        out.append("PPC_ENTER_GLOBAL_LOCK();" if s == 13 else "PPC_LEAVE_GLOBAL_LOCK();")
        return out

    if base_mn == "mtfsf":
        fm = _imm(ops[0])
        b = _freg(ops[1])
        mask = 0
        for j in range(8):
            if fm & (1 << (7 - j)):
                mask |= 0xF << (4 * j)
        if mask == 0xFFFFFFFF:
            return [f"ctx.fpscr.storeFromGuest(ctx.f{b}.u32);"]
        nmask = (~mask) & U32
        return [
            f"ctx.fpscr.storeFromGuest((ctx.fpscr.loadFromHost() & 0x{nmask:08X}) | "
            f"(ctx.f{b}.u32 & 0x{mask:08X}));"
        ]

    if base_mn == "mtxer":
        s = _reg(ops[0])
        return [
            f"ctx.xer.so = (ctx.r{s}.u64 & 0x80000000) != 0;",
            f"ctx.xer.ov = (ctx.r{s}.u64 & 0x40000000) != 0;",
            f"ctx.xer.ca = (ctx.r{s}.u64 & 0x20000000) != 0;",
        ]

    # -----------------------------------------------------------------------
    # Clear Left Doubleword Immediate (simplified rldicl)
    # -----------------------------------------------------------------------
    if base_mn == "clrldi":
        d = _reg(ops[0])
        s = _reg(ops[1])
        n = _imm(ops[2])
        mask = ((1 << (64 - n)) - 1) & U64
        out = [f"ctx.r{d}.u64 = ctx.r{s}.u64 & 0x{mask:X};"]
        if rc:
            out.append(f"ctx.cr0.compare<int32_t>(ctx.r{d}.s32, 0, ctx.xer);")
        return out

    # -----------------------------------------------------------------------
    # CR-logical bit operations (from logical.cpp, this category)
    # -----------------------------------------------------------------------
    if base_mn in _CRLOGICAL:
        op, inv_a, inv_b, inv_res = _CRLOGICAL[base_mn]
        return _emit_crbit(op, inv_a, inv_b, inv_res, _cr, ops)

    return None


RUNTIME_HELPERS = r""""""


MNEMONICS = [
    # No-ops / barriers / Xenon hints / cache hints
    "nop", "attn", "sync", "lwsync", "eieio", "db16cyc",
    "cctpl", "cctpm", "cctph",
    "dcbf", "dcbt", "dcbtst", "dcbst",
    # Cache ops with effect
    "dcbz", "dcbzl",
    # Move register / CR field
    "mr", "mcrf",
    # Move-from SPR
    "mfxer", "mfctr", "mfcr", "mfocrf", "mflr", "mfmsr", "mffs", "mftb", "mftbu",
    # Move-to SPR
    "mtcr", "mtcrf", "mtocrf", "mtctr", "mtlr", "mtmsrd", "mtfsf", "mtxer",
    # Clear-left doubleword immediate
    "clrldi",
    # CR-logical
    "crand", "crandc", "creqv", "crnand", "crnor", "cror", "crorc", "crxor",
    # Trap: base + all simplified conditional forms
    "twi", "twlgti", "twllti", "tweqi", "twlgei", "twlnli", "twllei",
    "twlngi", "twgti", "twgei", "twnli", "twlti", "twlei", "twngi", "twnei",
    "tdi", "tdlgti", "tdllti", "tdeqi", "tdlgei", "tdlnli", "tdllei",
    "tdlngi", "tdgti", "tdgei", "tdnli", "tdlti", "tdlei", "tdngi", "tdnei",
    "tw", "twge", "twgt", "twle", "twlt", "tweq", "twne",
    "twlge", "twlgt", "twlle", "twllt",
    "td", "tdge", "tdgt", "tdle", "tdlt", "tdeq", "tdne",
    "tdlge", "tdlgt", "tdlle", "tdllt",
]


# Not translated: isel; mfspr/mtspr other than their simplified forms (mfxer mfctr mflr mftb ...
# and mtxer mtctr mtlr); isync, sc, ptesync, tlbsync. The code generator has no builders for them.
