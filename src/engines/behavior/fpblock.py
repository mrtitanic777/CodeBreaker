#!/usr/bin/env python3
r"""fpblock.py -- recover x87 float expressions statically, per basic block.

fpsynth reconstructs a whole LEAF float function by tracing st(0). But on Win95 the x87 code is
usually embedded inside a bigger integer function -- a few `fld/fmul/fstp` in one block of a
larger CFG -- so the leaf tracer never sees it. This recovers those expressions statically: it
models the x87 register stack symbolically across a block's instructions (push on fld, combine on
fadd/fmulp/..., store on fst/fstp) and emits the resulting `dst = a * b + c;` assignments. It is
deliberately conservative -- on any unmodelled instruction, stack underflow, or unnamable operand
it returns nothing, because a wrong float statement is worse than none.
"""
from src.engines.behavior import dataflow

_ARITH = {"fadd": "+", "fsub": "-", "fmul": "*", "fdiv": "/"}
_REV = {"fsubr": "-", "fdivr": "/"}                     # reversed operand order
_UNARY = {"fsqrt": "sqrt(%s)", "fabs": "fabs(%s)", "fchs": "-(%s)"}
_LOADS = {"fld", "fild"}


def _operand(tok):
    """Name a float operand: a memory ref via dataflow, a register/immediate as-is."""
    tok = tok.strip()
    if "[" in tok:
        return dataflow._mem_name(tok) or tok
    return tok


def _sti(tok):
    """st(i) -> i, or None."""
    tok = tok.strip()
    if tok.startswith("st(") and tok.endswith(")"):
        try:
            return int(tok[3:-1])
        except ValueError:
            return None
    if tok == "st":
        return 0
    return None


def emit(rows):
    """Emit float assignment statements for a block's instruction rows [(addr,mn,ops,sz)], or []."""
    st = []                                             # symbolic x87 stack; st[-1] is st(0)
    out = []

    def paren(e):
        return e if (e.replace(".", "").replace("_", "").isalnum() or e.startswith(("(", "-"))) else "(%s)" % e

    for _a, mn, ops, _sz in rows:
        try:
            if mn in _LOADS:
                i = _sti(ops)
                st.append(st[-1 - i] if i is not None else _operand(ops))
            elif mn == "fld1":
                st.append("1.0")
            elif mn == "fldz":
                st.append("0.0")
            elif mn in _UNARY:
                if not st:
                    return []
                st[-1] = _UNARY[mn] % st[-1]
            elif mn in _ARITH or mn in _REV:
                op = _ARITH.get(mn) or _REV[mn]
                rev = mn in _REV
                if ops and "," not in ops and _sti(ops) is None:   # `fadd m` : st0 = st0 op m
                    if not st:
                        return []
                    m = _operand(ops)
                    a, b = (m, st[-1]) if rev else (st[-1], m)
                    st[-1] = "%s %s %s" % (paren(a), op, paren(b))
                else:                                              # register form `fadd st(i),st`
                    if len(st) < 1:
                        return []
                    parts = [p.strip() for p in ops.split(",")] if "," in ops else ["st(1)", "st"]
                    di = _sti(parts[0]) or 0
                    si = _sti(parts[1]) if len(parts) > 1 else 0
                    if di >= len(st) or si >= len(st):
                        return []
                    d, s = st[-1 - di], st[-1 - si]
                    a, b = (s, d) if rev else (d, s)
                    st[-1 - di] = "%s %s %s" % (paren(a), op, paren(b))
            elif mn in ("faddp", "fsubp", "fmulp", "fdivp", "fsubrp", "fdivrp"):
                if len(st) < 2:
                    return []
                base = mn[:-1]
                op = _ARITH.get(base) or _REV.get(base)
                if op is None:
                    return []
                s0 = st.pop(); s1 = st[-1]
                a, b = (s0, s1) if base in _REV else (s1, s0)
                st[-1] = "%s %s %s" % (paren(a), op, paren(b))
            elif mn in ("fst", "fstp", "fistp", "fist"):
                if not st:
                    return []
                dst = _operand(ops)
                cast = "(int)" if mn in ("fistp", "fist") else ""
                out.append("%s = %s%s;" % (dst, cast, st[-1]))
                if mn.endswith("p"):
                    st.pop()
            elif mn in ("fxch",):
                if len(st) >= 2:
                    st[-1], st[-2] = st[-2], st[-1]
            elif mn in ("fnstsw", "fstsw", "fnstcw", "fldcw", "fwait", "fclex", "fnclex",
                        "fcom", "fcomp", "fcompp", "fucom", "fucomp", "fucompp",
                        "fcomi", "fcomip", "ftst"):
                pass                                               # control / compare: no value effect
            else:
                if mn.startswith("f"):                             # an x87 op we don't model -> bail
                    return []
        except Exception:
            return []
    return out
