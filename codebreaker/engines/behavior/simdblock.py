#!/usr/bin/env python3
r"""simdblock.py -- recover MMX / SSE vector expressions statically, per basic block.

The integer/x87 engines miss packed-SIMD code. This models the MMX (mm0-7) and SSE (xmm0-7)
register files across a basic block -- a load into a register, packed arithmetic between registers,
a store back to memory -- and emits the authored intrinsic form (`xmm0 = _mm_mul_ps(xmm0, xmm1);`,
`mm0 = _mm_add_pi32(mm0, mm2);`). Register-based (SIMD has no push/pop stack), and conservative:
an unmodelled SIMD op or an unnamable operand emits nothing.

Era note: this exists for completeness and for later multimedia/driver corpora. Win95 (1995)
predates MMX (Pentium MMX, 1997) and SSE (Pentium III, 1999), so its OS binaries contain
essentially no MMX and no SSE -- expected yield on the Win95 corpus itself is ~zero.
"""
from codebreaker.engines.behavior import dataflow

# packed-arithmetic mnemonic -> MSVC/Intel intrinsic name (the authored form)
_INTRIN = {
    "paddb": "_mm_add_pi8", "paddw": "_mm_add_pi16", "paddd": "_mm_add_pi32",
    "psubb": "_mm_sub_pi8", "psubw": "_mm_sub_pi16", "psubd": "_mm_sub_pi32",
    "pmullw": "_mm_mullo_pi16", "pmulhw": "_mm_mulhi_pi16", "pmaddwd": "_mm_madd_pi16",
    "pand": "_mm_and_si64", "por": "_mm_or_si64", "pxor": "_mm_xor_si64",
    "addps": "_mm_add_ps", "subps": "_mm_sub_ps", "mulps": "_mm_mul_ps", "divps": "_mm_div_ps",
    "addss": "_mm_add_ss", "subss": "_mm_sub_ss", "mulss": "_mm_mul_ss", "divss": "_mm_div_ss",
    "andps": "_mm_and_ps", "orps": "_mm_or_ps", "xorps": "_mm_xor_ps",
    "minps": "_mm_min_ps", "maxps": "_mm_max_ps",
    "addpd": "_mm_add_pd", "subpd": "_mm_sub_pd", "mulpd": "_mm_mul_pd", "divpd": "_mm_div_pd",
    "paddq": "_mm_add_si64",
}
_MOVS = {"movq", "movd", "movdqa", "movdqu", "movaps", "movups", "movss", "movsd", "movlps", "movhps"}
_REG = ("mm", "xmm")


def _isreg(tok):
    tok = tok.strip()
    return tok.startswith(_REG) and tok[2:].lstrip("m").isdigit() if tok.startswith(_REG) else False


def _operand(tok):
    tok = tok.strip()
    if _isreg(tok):
        return tok
    if "[" in tok:
        return dataflow._mem_name(tok) or tok
    return tok


def emit(rows):
    """Emit SIMD assignment statements for a block's rows [(addr,mn,ops,sz)], or []."""
    reg = {}                                            # mm/xmm register -> current expression
    out = []
    for _a, mn, ops, _sz in rows:
        parts = [p.strip() for p in ops.split(",")] if "," in ops else [ops.strip()]
        try:
            if mn in _MOVS and len(parts) == 2:
                dst, src = parts
                if _isreg(dst):
                    reg[dst] = reg.get(src, _operand(src)) if _isreg(src) else _operand(src)
                elif "[" in dst and _isreg(src):        # store register to memory
                    out.append("%s = %s;" % (_operand(dst), reg.get(src, src)))
                else:
                    continue
            elif mn in _INTRIN and len(parts) == 2:
                dst, src = parts
                if not _isreg(dst):
                    return []
                a = reg.get(dst, dst)
                b = reg.get(src, _operand(src)) if _isreg(src) else _operand(src)
                reg[dst] = "%s(%s, %s)" % (_INTRIN[mn], a, b)
            elif mn.startswith(("p", "add", "sub", "mul", "div", "min", "max", "and", "or", "xor",
                                "movl", "movh", "unpck", "shuf", "cvt", "sqrtp", "sqrts")):
                if mn not in _MOVS and mn not in _INTRIN:
                    return []                            # an unmodelled SIMD op -> bail
        except Exception:
            return []
    return out
