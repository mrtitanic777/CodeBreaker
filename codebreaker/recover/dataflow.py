#!/usr/bin/env python3
r"""dataflow.py -- intra-block data-flow (SSA-lite): name a register's value by where it came from.

cfgstruct recovers predicates as raw registers (`if (eax == ecx)`). This resolves each operand to
its SOURCE by scanning backward for the last def within the block: a parameter (`[ebp+8]` -> a0), a
local (`[ebp-N]` -> local_N), a struct field (`[reg+N]` -> reg->f_N), an immediate, a copy (follow
it), or a call result (eax after a call). The predicate then reads `if (a0 == 0)` / `if (p->f_4 != 0)`
-- the readable form. Bounded, best-effort; unresolved operands keep the register name.
"""
import re

_REG = r"e?[abcd]x|e?[sd]i|e?bp|e?sp|[abcd][lh]"
_MEM = re.compile(r"\[(e[a-z]{2})\s*([+\-])\s*(0x[0-9a-f]+|\d+)\]")
_MEMBP = re.compile(r"\[ebp\s*([+\-])\s*(0x[0-9a-f]+|\d+)\]")


def _mem_name(ops):
    """Name a memory operand: [ebp+8]->a0/param, [ebp-N]->local_N, [reg+N]->reg->f_N."""
    m = _MEMBP.search(ops)
    if m:
        sign, num = m.group(1), int(m.group(2), 0)
        if sign == "+":
            if num >= 8 and num % 4 == 0:
                return "a%d" % ((num - 8) // 4)            # [ebp+8]=a0, [ebp+0xc]=a1, ...
            return "param_%x" % num
        return "local_%x" % num                             # [ebp-N]
    m = re.search(r"\[esp\s*\+\s*(0x[0-9a-f]+|\d+)\]", ops)   # stack slot
    if m:
        return "sp_%x" % int(m.group(1), 0)
    m = _MEM.search(ops)
    if m:
        reg, sign, num = m.group(1), m.group(2), int(m.group(3), 0)
        if reg not in ("ebp", "esp"):
            return "%s->f_%x" % (reg, num) if sign == "+" else "%s[-0x%x]" % (reg, num)
    m = re.search(r"\[(e[a-z]{2})\]", ops)                    # bare deref [reg]
    if m and m.group(1) not in ("ebp", "esp"):
        return "*%s" % m.group(1)
    return None


def source_of(rows, idx, reg, depth=0):
    """Best-effort source expression for `reg` just before rows[idx]. rows: [(addr,mn,ops,sz)]."""
    if depth > 4:
        return reg
    base = reg[-2:] if reg.startswith("e") else reg       # normalize ax<-eax etc. loosely
    for j in range(idx - 1, -1, -1):
        _, mn, ops, _ = rows[j]
        if mn == "call":
            # eax holds the call result
            if reg in ("eax", "ax", "al"):
                return "result"
            continue
        parts = [p.strip() for p in ops.split(",")]
        if len(parts) != 2:
            continue
        dst = parts[0]
        if dst != reg and dst != base and not (reg == "eax" and dst == "ax"):
            continue
        src = parts[1]
        if mn == "mov":
            if src.startswith("0x") or src.isdigit():
                return src
            nm = _mem_name(ops)
            if nm:
                return nm
            if re.fullmatch(_REG, src):
                return source_of(rows, j, src, depth + 1)
            return src
        if mn == "lea":
            nm = _mem_name("[" + src.split("[", 1)[1] if "[" in src else src)
            return ("&" + nm) if nm else reg
        if mn in ("xor",) and parts[0] == parts[1]:
            return "0"
        return reg                                          # some other def (add/or/...) -> opaque
    return reg
