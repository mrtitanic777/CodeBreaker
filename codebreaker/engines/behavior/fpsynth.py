#!/usr/bin/env python3
r"""fpsynth.py -- floating-point reconstruction (x87), the FP counterpart to synth.

Every other engine is integer-only. A function that computes with the x87 FPU takes its float args
on the stack and returns in st(0) -- invisible to the integer synth (which reads eax). This detects
x87 usage, traces the function with float arguments (their IEEE-754 bit patterns on the stack),
reads the st(0) return the tracer now captures, and fits a small float-expression grammar
(x+y, x*c, sqrt(x), 1/x, ...) to the samples, verified with FP tolerance.
"""
import math
import struct
from codebreaker.engines.behavior import tracer

_X87 = ("fld", "fst", "fstp", "fadd", "fsub", "fmul", "fdiv", "fild", "fistp", "faddp",
        "fmulp", "fsubp", "fdivp", "fsqrt", "fabs", "fchs", "fsin", "fcos", "fscale")
_IN = [1.0, 2.0, 3.5, 0.5, -1.0, 10.0, 0.25, 4.0]


def _f2u(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def _pack_args(vals, width):
    """Pack float args as stack dwords: width 4 = float (1 dword each), 8 = double (2 dwords each)."""
    out = []
    for v in vals:
        if width == 8:
            lo, hi = struct.unpack("<II", struct.pack("<d", v))
            out += [lo, hi]
        else:
            out.append(_f2u(v))
    return out


def is_fp(module, va):
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        fend = next((f["end"] for o, f in sorted(funcs.items()) if o == off), None)
        if fend is None:
            nx = sorted(o for o in funcs if o > off); fend = nx[0] if nx else off + 0x200
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        return any(ins.mnemonic in _X87 for ins in md.disasm(code[off:fend], va))
    except Exception:
        return False


def _sample(module, va, vals, width):
    tr = tracer.trace_function(module, func=hex(va), args=_pack_args(vals, width), insn_cap=8000)
    if tr.get("exit") != "ret" or "st0" not in tr:
        return None
    v = tr["st0"]
    return v if (isinstance(v, float) and not math.isnan(v) and abs(v) < 1e30) else None


def _fit(samples, fn):
    for a, r in samples:
        try:
            e = fn(a)
        except Exception:
            return False
        if e is None or abs(e - r) > 1e-3 * max(1.0, abs(r)):
            return False
    return True


_UNARY = [
    ("x", "x", lambda x: x),
    ("-x", "-x", lambda x: -x),
    ("x * x", "x * x", lambda x: x * x),
    ("sqrt(x)", "sqrtf(x)", lambda x: math.sqrt(x) if x >= 0 else None),
    ("fabs(x)", "fabsf(x)", abs),
    ("1.0 / x", "1.0f / x", lambda x: 1.0 / x if x else None),
    ("x + 1", "x + 1.0f", lambda x: x + 1.0),
    ("x * 2", "x * 2.0f", lambda x: x * 2.0),
    ("x * 0.5", "x * 0.5f", lambda x: x * 0.5),
]
_BINARY = [
    ("x + y", "x + y", lambda x, y: x + y),
    ("x - y", "x - y", lambda x, y: x - y),
    ("y - x", "y - x", lambda x, y: y - x),
    ("x * y", "x * y", lambda x, y: x * y),
    ("x / y", "x / y", lambda x, y: x / y if y else None),
    ("y / x", "y / x", lambda x, y: y / x if x else None),
]


def synthesize(module, func=None, arity=2):
    va = int(func, 0) if isinstance(func, str) else int(func)
    if not is_fp(module, va):
        return {"reconstructed": False, "reason": "no x87 float ops"}
    for width, ty in ((8, "double"), (4, "float")):          # double first (Win95 FP is mostly double)
        # unary: vary arg0
        s1 = []
        for x in _IN:
            r = _sample(module, va, [x], width)
            if r is None:
                break
            s1.append((x, r))
        if len(s1) == len(_IN):
            for desc, c, fn in _UNARY:
                if _fit(s1, fn):
                    return {"reconstructed": True, "verified": True, "arity": 1, "width": width,
                            "expr": desc, "emitted_c": "%s sub_%x(%s x)\n{\n    return %s;\n}\n" % (ty, va, ty, c)}
        # binary
        s2 = []
        for x in _IN[:5]:
            for y in (1.0, 2.0, 0.5, 3.0):
                r = _sample(module, va, [x, y], width)
                if r is None:
                    s2 = None; break
                s2.append((x, y, r))
            if s2 is None:
                break
        if s2:
            for desc, c, fn in _BINARY:
                ok = True
                for x, y, r in s2:
                    try:
                        e = fn(x, y)
                    except Exception:
                        ok = False; break
                    if e is None or abs(e - r) > 1e-3 * max(1.0, abs(r)):
                        ok = False; break
                if ok:
                    return {"reconstructed": True, "verified": True, "arity": 2, "width": width,
                            "expr": desc, "emitted_c": "%s sub_%x(%s x, %s y)\n{\n    return %s;\n}\n" % (ty, va, ty, ty, c)}
    return {"reconstructed": False, "reason": "float behavior not in grammar"}
