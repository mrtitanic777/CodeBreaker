#!/usr/bin/env python3
r"""constprop.py -- forward constant propagation through registers.

ssa resolves an operand to the SOURCE expression that defined it; this resolves it to a concrete
VALUE where one is known. A forward pass folds `mov reg,imm`, reg-to-reg copies, `xor reg,reg`->0,
and arithmetic against a known constant, so by a given instruction we know which registers hold
constants. That pins call arguments the tracer under-discovered, and -- when a register holds a
constant pointer -- lets datarec resolve the string/data it points at. Intra-block and
conservative: a call clobbers the caller-saved registers, a memory load makes a register unknown.
"""
import re
from codebreaker.engines.behavior import tracer

_GPR = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")
_CLOBBER = ("eax", "ecx", "edx")                             # caller-saved: unknown after a call
_MASK = 0xFFFFFFFF


def _imm(tok):
    tok = tok.strip()
    try:
        return int(tok, 0) & _MASK
    except ValueError:
        return None


def propagate(rows):
    """Yield the register->constant state (a dict) AFTER each row. rows: [(addr,mn,ops,sz)]."""
    st = {}
    states = []
    for _a, mn, ops, _sz in rows:
        parts = [p.strip() for p in ops.split(",")]
        dst = parts[0] if parts else ""
        if mn == "call":
            for r in _CLOBBER:
                st.pop(r, None)
        elif mn == "xor" and len(parts) == 2 and parts[0] == parts[1] and parts[0] in _GPR:
            st[parts[0]] = 0
        elif mn == "mov" and len(parts) == 2 and dst in _GPR:
            v = _imm(parts[1])
            if v is not None:
                st[dst] = v
            elif parts[1] in st:
                st[dst] = st[parts[1]]
            else:
                st.pop(dst, None)                            # loaded from memory / unknown reg
        elif mn == "lea" and len(parts) == 2 and dst in _GPR:
            m = re.match(r"\[(e[a-z]{2})\s*([+-])\s*(0x[0-9a-f]+|\d+)\]$", parts[1])
            if m and m.group(1) in st:
                d = int(m.group(3), 0) * (-1 if m.group(2) == "-" else 1)
                st[dst] = (st[m.group(1)] + d) & _MASK
            else:
                st.pop(dst, None)
        elif mn in ("add", "sub", "and", "or", "xor", "shl", "shr", "sar", "imul") \
                and len(parts) == 2 and dst in _GPR and dst in st:
            v = _imm(parts[1])
            if v is None:
                st.pop(dst, None)
            else:
                a = st[dst]
                st[dst] = {"add": a + v, "sub": a - v, "and": a & v, "or": a | v, "xor": a ^ v,
                           "shl": a << v, "shr": a >> v, "sar": a >> v, "imul": a * v}[mn] & _MASK
        elif mn in ("inc", "dec") and dst in _GPR and dst in st:
            st[dst] = (st[dst] + (1 if mn == "inc" else -1)) & _MASK
        elif dst in _GPR:
            st.pop(dst, None)                                # any other write -> unknown
        states.append(dict(st))
    return states


def const_at(rows, idx, reg):
    """The constant value of `reg` just before rows[idx], or None if not known-constant."""
    if idx <= 0:
        return None
    return propagate(rows[:idx])[-1].get(reg) if idx else None


def const_args_at_call(rows, call_idx, regs=("ecx", "edx")):
    """Known-constant register args at a call (thiscall/fastcall) -- {reg: value}."""
    st = propagate(rows[:call_idx])
    last = st[-1] if st else {}
    return {r: last[r] for r in regs if r in last}


def analyze(module, va):
    """Constant-register state across a function -- {'nconst': how many instructions pin >=1 reg}."""
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    states = propagate(rows)
    return {"ok": True, "ninsns": len(rows), "nconst": sum(1 for s in states if s), "rows": rows,
            "states": states}
