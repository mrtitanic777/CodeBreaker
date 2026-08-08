#!/usr/bin/env python3
r"""msvc2model.py - FORWARD MODEL for the MSVC 2.0 (win95 C2.EXE) code generator, 32-bit.

The 32-bit sibling of mc2model.py: a deterministic `construct -> bytes` predictor of what
MSVC 2.0's backend emits at the retail flags (`/O1 /Oy-`), built the proven way -- probe the
real compiler, find the exact rule, encode it, prove it round-trips byte-exact. This is the
load-bearing first step toward an MSVC-2.0 *inverse* (bytes -> byte-exact C) that reaches the
below-C residual (scheduling / stack-displacement / register choice) which C-shape search
provably cannot -- see win95 docs/reports/MSVC20_ALLOCATOR_MODEL.md.

FIRST CLASS MODELED (this milestone): a single-`return` __stdcall leaf function over int
args/consts --  `int __stdcall f(<n int args>) { return EXPR; }`  where EXPR is a constant, an
argument, `arg <op> const`, or `arg <op> arg`  (op in + - & | ^ *).

Measured MSVC-2.0 rules encoded here (each verified byte-exact against the oracle):
  * const 0 -> `xor eax,eax` (33 c0); other const -> `mov eax,imm32` (b8 id); no frame.
  * any arg use sets up the /Oy- frame: `push ebp`(55) ... `mov ebp,esp`(8b ec) ... `pop ebp`
    `ret 4*nargs` (5d c2 iw).  `mov ebp,esp` sits right AFTER the first load, EXCEPT for imul,
    which defers it to AFTER the multiply.
  * arg is loaded to eax from [esp + 4 + 4*K] (K = 1-based arg index; the +4 is the pushed ebp).
  * arg<op>const:  add 83/0, sub 83/5, and 83/4 (eax,ib);  or/xor use the 2-byte AL form
    0c/34 ib (upper bytes pass through, so the byte op is exact + shorter);  mul 6b c0 ib.
  * arg<op>arg:  add/sub load the LEFT operand first (03 / 2b eax,[right]);  and/or/xor/mul
    load the RIGHT first then op with the left (23 / 0b / 33 / 0f af  eax,[left]) -- MSVC's
    commutative-operand canonicalization.

    wt inverse msvc2model                 # self-test: model vs oracle, byte-exact count
    wt inverse msvc2model --show          # dump each case: predicted vs shipped

Out-of-class constructs are declined (emit returns None) -- never mis-modeled.
"""
import struct
import sys

# ---- construct IR (tiny; the "quad" analogue for this class) ---------------
# node = {"nargs": N, "expr": E}
#   E = ("const", V) | ("arg", K) | ("binop", op, L, R)   with L,R = ("arg",K)|("const",V)
OPS = {"+", "-", "&", "|", "^", "*"}


def _imm32(v):
    return struct.pack("<i", v)


def _argoff(k):
    """Frame offset of the k-th int arg (1-based) once ebp is pushed: [esp+4+4k]."""
    return 4 + 4 * k


def _load_eax(k):
    """mov eax, [esp + off]  ->  8b 44 24 ib   (off always < 0x80 for our arg range)."""
    return bytes([0x8B, 0x44, 0x24, _argoff(k)])


# arg <op> const  -> the byte sequence for the op (operand already in eax)
def _op_const(op, c):
    if op == "+":
        return bytes([0x83, 0xC0, c & 0xFF])            # add eax, ib
    if op == "-":
        return bytes([0x83, 0xE8, c & 0xFF])            # sub eax, ib
    if op == "&":
        return bytes([0x83, 0xE0, c & 0xFF])            # and eax, ib
    if op == "|":
        return bytes([0x0C, c & 0xFF])                  # or  al, ib
    if op == "^":
        return bytes([0x34, c & 0xFF])                  # xor al, ib
    if op == "*":
        return bytes([0x6B, 0xC0, c & 0xFF])            # imul eax, eax, ib
    return None


# arg <op> arg  -> (which operand to LOAD first, the op-with-memory bytes vs the other operand)
_MEMOP = {"+": 0x03, "-": 0x2B, "&": 0x23, "|": 0x0B, "^": 0x33}   # op eax, [esp+off]
def _binargs_plan(op, li, ri):
    """Return (load_idx, other_idx, op_encoder). add/sub load LEFT first; commutative
    (and/or/xor/mul) load RIGHT first, then op against the LEFT."""
    if op in ("+", "-"):
        load, other = li, ri
    else:                                                # & | ^ *  -> right first
        load, other = ri, li
    def enc(off):
        if op == "*":
            return bytes([0x0F, 0xAF, 0x44, 0x24, off])  # imul eax, [esp+off]
        return bytes([_MEMOP[op], 0x44, 0x24, off])      # op eax, [esp+off]
    return load, other, enc


def emit(node):
    """Forward model: construct -> exact MSVC-2.0 /O1 /Oy- bytes, or None if out of class."""
    try:
        nargs, E = node["nargs"], node["expr"]
    except (KeyError, TypeError):
        return None
    kind = E[0]

    # --- pure constant: no frame -------------------------------------------
    if kind == "const":
        v = E[1]
        body = bytes([0x33, 0xC0]) if v == 0 else (bytes([0xB8]) + _imm32(v))
        ret = bytes([0xC3]) if nargs == 0 else bytes([0xC2]) + struct.pack("<H", 4 * nargs)
        return body + ret

    # --- everything else uses an arg -> /Oy- frame -------------------------
    setframe = bytes([0x8B, 0xEC])                       # mov ebp, esp
    epi = bytes([0x5D]) + bytes([0xC2]) + struct.pack("<H", 4 * nargs)   # pop ebp; ret 4n
    pro = bytes([0x55])                                  # push ebp

    if kind == "arg":
        seq = _load_eax(E[1]) + setframe                # load; mov ebp,esp
        return pro + seq + epi

    if kind == "binop":
        op, L, R = E[1], E[2], E[3]
        if op not in OPS:
            return None
        # arg <op> const
        if L[0] == "arg" and R[0] == "const":
            k, c = L[1], R[1]
            opb = _op_const(op, c)
            if opb is None or not (-128 <= c <= 127):
                return None                              # imm8-range only, this milestone
            if op == "*":                                # imul defers mov ebp,esp past the op
                return pro + _load_eax(k) + opb + setframe + epi
            return pro + _load_eax(k) + setframe + opb + epi
        # arg <op> arg
        if L[0] == "arg" and R[0] == "arg":
            load, other, enc = _binargs_plan(op, L[1], R[1])
            opb = enc(_argoff(other))
            if op == "*":
                return pro + _load_eax(load) + opb + setframe + epi
            return pro + _load_eax(load) + setframe + opb + epi
        return None
    return None


# --- C-source renderer (for oracle validation) ------------------------------
def _c_operand(o):
    return ("arg%d" % o[1]) if o[0] == "arg" else str(o[1])


def to_c(node):
    nargs, E = node["nargs"], node["expr"]
    params = "void" if nargs == 0 else ", ".join("int arg%d" % (i + 1) for i in range(nargs))
    if E[0] == "const":
        expr = str(E[1])
    elif E[0] == "arg":
        expr = "arg%d" % E[1]
    else:
        expr = "%s %s %s" % (_c_operand(E[2]), E[1], _c_operand(E[3]))
    return "int __stdcall f(%s) { return %s; }" % (params, expr)


# --- the battery: every construct in the class ------------------------------
def battery():
    cases = []
    cases.append(("ret0", {"nargs": 0, "expr": ("const", 0)}))
    cases.append(("ret1", {"nargs": 0, "expr": ("const", 1)}))
    cases.append(("retBig", {"nargs": 0, "expr": ("const", 0x1234)}))
    cases.append(("retNeg", {"nargs": 0, "expr": ("const", -7)}))
    cases.append(("arg1", {"nargs": 1, "expr": ("arg", 1)}))
    cases.append(("arg2of2", {"nargs": 2, "expr": ("arg", 2)}))
    for op in ("+", "-", "&", "|", "^", "*"):
        cases.append(("a%sc" % op, {"nargs": 1, "expr": ("binop", op, ("arg", 1), ("const", 5))}))
        cases.append(("a%sb" % op, {"nargs": 2, "expr": ("binop", op, ("arg", 1), ("arg", 2))}))
    return cases


def _oracle():
    """Wire MSVC-2.0 cc32 via the win95 target. cc32._vc_root() finds the vendored msvc20
    toolchain under the win95 DATA root's docs/reference/toolchain -- and it keys off cwd, so
    chdir there (data lives outside the tool repo; point at it with WT_DATA_ROOT)."""
    import os
    from engine.foundation import cores
    here = os.path.dirname(os.path.abspath(__file__))
    cb = os.path.dirname(os.path.dirname(here))
    if cores._active_dir is None:
        cores.set_target(os.path.join(cb, "engine", "targets", "win95"))
    # locate the win95 data root (has docs/reference/toolchain/msvc20/VC/BIN/CL.EXE)
    roots = []
    if os.environ.get("WT_DATA_ROOT"):
        roots.append(os.path.join(os.environ["WT_DATA_ROOT"], "win95"))
    roots += [os.path.join(os.path.dirname(cb), "win95"), os.getcwd()]
    for r in roots:
        if os.path.exists(os.path.join(r, "docs", "reference", "toolchain",
                                       "msvc20", "VC", "BIN", "CL.EXE")):
            os.chdir(r)
            break
    return cores.get("cc32")


def selftest(show=False):
    cc32 = _oracle()
    ok = tot = 0
    for name, node in battery():
        pred = emit(node)
        tot += 1
        r = cc32.compile_c(to_c(node), flags="/c /O1 /Oy-")
        if "obj" not in r:
            print("  %-8s ORACLE-FAIL %s" % (name, str(r.get("err", ""))[:80])); continue
        fns = cc32.Obj(r["obj"]).functions()
        fn = next((v for k, v in fns.items() if k.startswith("_f")), None)
        real = bytes(fn[0]) if fn else b""
        match = pred == real
        ok += match
        if show or not match:
            tag = "OK " if match else "MISS"
            print("  %-8s %s  pred=%s  real=%s" % (name, tag,
                  pred.hex(" ") if pred else "(declined)", real.hex(" ")))
    print("\nmsvc2model self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    return ok == tot


def main():
    show = "--show" in sys.argv[1:]
    ok = selftest(show=show)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
