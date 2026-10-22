"""logical.py -- logical, rotate, shift, extend and immediate boolean operations, as C++ for the recompiler (recompile/powerpc.py).

Each `emit(mnemonic, operands, ...)` returns the C++ statements for one instruction, or None
when the mnemonic is not this file's. Registers are always written as the context's fields
(ctx.rN, ctx.fN, ctx.vN, ctx.crN, ctx.xer, ctx.ctr, ctx.lr); `ea` and `temp` are locals every
recompiled function declares.
Record forms emit only the base operation (powerpc.py appends the CR0 update), except andi. and
andis., which always set CR0 and emit it themselves. The CR-bit operations (crand ...) are
system.py's.

Derived from the rexglue SDK's code generator (rexcodegen/builders/logical.cpp; BSD-3-Clause,
Copyright (c) 2026 Tom Clay, with parts from Xenia -- see NOTICE).
"""
U64 = 0xFFFFFFFFFFFFFFFF


def _compute_mask(mstart, mstop):
    # the rotate mask from bit mstart to bit mstop (the code generator's compute_mask)
    mstart &= 0x3F
    mstop &= 0x3F
    value = (U64 >> mstart) ^ (0 if mstop >= 63 else (U64 >> (mstop + 1)))
    value &= U64
    return value if mstart <= mstop else ((~value) & U64)


def emit(mn, ops, _reg, _vreg, _freg, _imm, _cr, _mem, _rv, _base_expr, _addr, va, WORD):
    # ---- AND ----
    if mn == "and":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} & {_rv(c,'u64')};"]

    if mn == "andc":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} & ~{_rv(c,'u64')};"]

    if mn == "andi":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2])
        return [
            f"{_rv(a,'u64')} = {_rv(b,'u64')} & {imm};",
            f"ctx.cr0.compare<int32_t>({_rv(a,'s32')}, 0, ctx.xer);",
        ]

    if mn == "andis":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2])
        return [
            f"{_rv(a,'u64')} = {_rv(b,'u64')} & {imm << 16};",
            f"ctx.cr0.compare<int32_t>({_rv(a,'s32')}, 0, ctx.xer);",
        ]

    # ---- OR / NAND / NOR / NOT ----
    if mn == "nand":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = ~({_rv(b,'u64')} & {_rv(c,'u64')});"]

    if mn == "nor":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = ~({_rv(b,'u64')} | {_rv(c,'u64')});"]

    if mn == "not":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'u64')} = ~{_rv(b,'u64')};"]

    if mn == "or":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} | {_rv(c,'u64')};"]

    if mn == "orc":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} | ~{_rv(c,'u64')};"]

    if mn == "ori":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} | {imm};"]

    if mn == "oris":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2]) & 0xFFFFFFFF
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} | {imm << 16};"]

    # ---- XOR / EQV ----
    if mn == "xor":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} ^ {_rv(c,'u64')};"]

    if mn == "xori":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} ^ {imm};"]

    if mn == "xoris":
        a, b = _reg(ops[0]), _reg(ops[1])
        imm = _imm(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} ^ {imm << 16};"]

    if mn == "eqv":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = ~({_rv(b,'u64')} ^ {_rv(c,'u64')});"]

    # ---- Count Leading Zeros ----
    if mn == "cntlzd":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} == 0 ? 64 : __builtin_clzll({_rv(b,'u64')});"]

    if mn == "cntlzw":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'u64')} = {_rv(b,'u32')} == 0 ? 32 : __builtin_clz({_rv(b,'u32')});"]

    # ---- Sign Extension ----
    if mn == "extsb":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'s64')} = {_rv(b,'s8')};"]

    if mn == "extsh":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'s64')} = {_rv(b,'s16')};"]

    if mn == "extsw":
        a, b = _reg(ops[0]), _reg(ops[1])
        return [f"{_rv(a,'s64')} = {_rv(b,'s32')};"]

    # ---- Clear Left Word Immediate ----
    if mn == "clrlwi":
        a, b = _reg(ops[0]), _reg(ops[1])
        n = _imm(ops[2])
        mask = ((1 << (32 - n)) - 1) & U64
        return [f"{_rv(a,'u64')} = {_rv(b,'u32')} & 0x{mask:X};"]

    # ---- Rotate Left Double Word ----
    if mn == "rldicl":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(_imm(ops[3]), 63)
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {sh}) & 0x{mask:X};"]

    if mn == "rldicr":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(0, _imm(ops[3]))
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {sh}) & 0x{mask:X};"]

    if mn == "rldic":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(_imm(ops[3]), 63 - sh)
        if sh:
            return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {sh}) & 0x{mask:X};"]
        return [f"{_rv(a,'u64')} = {_rv(b,'u64')} & 0x{mask:X};"]

    if mn == "rldcl":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        mask = _compute_mask(_imm(ops[3]), 63)
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {_rv(c,'u8')} & 0x3F) & 0x{mask:X};"]

    if mn == "rldcr":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        mask = _compute_mask(0, _imm(ops[3]))
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {_rv(c,'u8')} & 0x3F) & 0x{mask:X};"]

    if mn == "rldimi":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(_imm(ops[3]), ~_imm(ops[2]))
        notmask = (~mask) & U64
        return [f"{_rv(a,'u64')} = (__builtin_rotateleft64({_rv(b,'u64')}, {sh}) & 0x{mask:X}) | ({_rv(a,'u64')} & 0x{notmask:X});"]

    if mn == "rotldi":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u64')}, {sh});"]

    # ---- Rotate Left Word ----
    if mn == "rlwimi":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(_imm(ops[3]) + 32, _imm(ops[4]) + 32)
        notmask = (~mask) & U64
        return [f"{_rv(a,'u64')} = (__builtin_rotateleft64({_rv(b,'u32')} | ({_rv(b,'u64')} << 32), {sh}) & 0x{mask:X}) | ({_rv(a,'u64')} & 0x{notmask:X});"]

    if mn == "rlwinm":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        mask = _compute_mask(_imm(ops[3]) + 32, _imm(ops[4]) + 32)
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u32')} | ({_rv(b,'u64')} << 32), {sh}) & 0x{mask:X};"]

    if mn == "rlwnm":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        mask = _compute_mask(_imm(ops[3]) + 32, _imm(ops[4]) + 32)
        return [f"{_rv(a,'u64')} = __builtin_rotateleft64({_rv(b,'u32')} | ({_rv(b,'u64')} << 32), {_rv(c,'u8')} & 0x1F) & 0x{mask:X};"]

    if mn == "rotlw":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = __builtin_rotateleft32({_rv(b,'u32')}, {_rv(c,'u8')} & 0x1F);"]

    if mn == "rotlwi":
        a, b = _reg(ops[0]), _reg(ops[1])
        sh = _imm(ops[2])
        return [f"{_rv(a,'u64')} = __builtin_rotateleft32({_rv(b,'u32')}, {sh});"]

    # ---- Shift Left ----
    if mn == "sld":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(c,'u8')} & 0x40 ? 0 : ({_rv(b,'u64')} << ({_rv(c,'u8')} & 0x7F));"]

    if mn == "slw":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(c,'u8')} & 0x20 ? 0 : ({_rv(b,'u32')} << ({_rv(c,'u8')} & 0x3F));"]

    # ---- Shift Right Algebraic (signed) ----
    if mn == "srad":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"temp.u64 = {_rv(c,'u64')} & 0x7F;",
            "if (temp.u64 > 0x3F) temp.u64 = 0x3F;",
            f"ctx.xer.ca = ({_rv(b,'s64')} < 0) & ((({_rv(b,'s64')} >> temp.u64) << temp.u64) != {_rv(b,'s64')});",
            f"{_rv(a,'s64')} = {_rv(b,'s64')} >> temp.u64;",
        ]

    if mn == "sradi":
        a, b = _reg(ops[0]), _reg(ops[1])
        n = _imm(ops[2])
        if n != 0:
            mask = _compute_mask(64 - n, 63)
            return [
                f"ctx.xer.ca = ({_rv(b,'s64')} < 0) & (({_rv(b,'u64')} & 0x{mask:X}) != 0);",
                f"{_rv(a,'s64')} = {_rv(b,'s64')} >> {n};",
            ]
        return [
            "ctx.xer.ca = 0;",
            f"{_rv(a,'s64')} = {_rv(b,'s64')};",
        ]

    if mn == "sraw":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [
            f"temp.u32 = {_rv(c,'u32')} & 0x3F;",
            "if (temp.u32 > 0x1F) temp.u32 = 0x1F;",
            f"ctx.xer.ca = ({_rv(b,'s32')} < 0) & ((({_rv(b,'s32')} >> temp.u32) << temp.u32) != {_rv(b,'s32')});",
            f"{_rv(a,'s64')} = {_rv(b,'s32')} >> temp.u32;",
        ]

    if mn == "srawi":
        a, b = _reg(ops[0]), _reg(ops[1])
        n = _imm(ops[2])
        if n != 0:
            mask = _compute_mask(64 - n, 63)
            return [
                f"ctx.xer.ca = ({_rv(b,'s32')} < 0) & (({_rv(b,'u32')} & 0x{mask:X}) != 0);",
                f"{_rv(a,'s64')} = {_rv(b,'s32')} >> {n};",
            ]
        return [
            "ctx.xer.ca = 0;",
            f"{_rv(a,'s64')} = {_rv(b,'s32')};",
        ]

    # ---- Shift Right (unsigned) ----
    if mn == "srd":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(c,'u8')} & 0x40 ? 0 : ({_rv(b,'u64')} >> ({_rv(c,'u8')} & 0x7F));"]

    if mn == "srw":
        a, b, c = _reg(ops[0]), _reg(ops[1]), _reg(ops[2])
        return [f"{_rv(a,'u64')} = {_rv(c,'u8')} & 0x20 ? 0 : ({_rv(b,'u32')} >> ({_rv(c,'u8')} & 0x3F));"]

    return None


RUNTIME_HELPERS = ""

MNEMONICS = [
    "and", "andc", "andi", "andis",
    "nand", "nor", "not", "or", "orc", "ori", "oris",
    "xor", "xori", "xoris", "eqv",
    "cntlzd", "cntlzw",
    "extsb", "extsh", "extsw",
    "clrlwi",
    "rldicl", "rldicr", "rldic", "rldcl", "rldcr", "rldimi", "rotldi",
    "rlwimi", "rlwinm", "rlwnm", "rotlw", "rotlwi",
    "sld", "slw",
    "srad", "sradi", "sraw", "srawi",
    "srd", "srw",
]

# Elsewhere: the CR-bit operations crand crandc creqv crnand crnor cror crorc crxor (system.py).
