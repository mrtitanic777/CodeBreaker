r"""msvc2_forward.py - FORWARD MODEL for the MSVC 2.0 (win95 C2.EXE) code generator, 32-bit.

A deterministic `construct -> bytes` predictor of what MSVC 2.0's backend emits at the retail flags
(`/O1 /Oy-`), each rule found by probing the real compiler and proven byte-exact against it; the
first step toward msvc2.py's inverse (bytes -> byte-exact C).  The constructs: single-`return`
__stdcall leaves over int args and constants (const, arg, arg<op>{arg,const}, chains, mixed
precedence, products, comparisons, deref / index, unary), globals, stores, extern and indirect
calls, call results combined with an operand, the esi-parked `callL <op> callR`, the frameless
(/Oy) and __fastcall forms, and multi-statement bodies (void calls, if-guards, stores, a return).

The model is C++ (native/invert/msvc2.cc: emit, to_c, frame_layout; docs/NATIVE.md); this keeps
its API and the self-tests against the 1994 compiler.

    wt inverse msvc2model                 # self-test: model vs oracle, byte-exact count
    wt inverse msvc2model --show          # dump each case: predicted vs shipped
    wt inverse msvc2model --frame         # the stack-frame layout against the compiler

Out-of-class constructs are declined (emit returns None) -- never mis-modeled.
"""
import sys

from codebreaker.invert import native_invert

# ---- construct IR (tiny; the "quad" analogue for this class) ---------------
# node = {"nargs": N, "expr": E}  (or a statement list: {"nargs", "stmts", "ret", "leave"})
#   E = ("const", V) | ("arg", K) | ("binop", op, L, R)   with L,R = ("arg",K)|("const",V)
OPS = {"+", "-", "&", "|", "^", "*"}


def emit(node):
    """Forward model: construct -> exact MSVC-2.0 bytes, or None if out of class.  A node with
    "fpo":True is the frameless (/Oy) form; otherwise the standard /Oy- framed form."""
    return native_invert.msvc2_emit(node)


def to_c(node):
    """The C the node stands for (what the self-test hands the compiler)."""
    return native_invert.msvc2_to_c(node)


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

    def _chain(op, idxs):
        e = ("arg", idxs[0])
        for i in idxs[1:]:
            e = ("binop", op, e, ("arg", i))
        return e
    cases.append(("a*b*c", {"nargs": 3, "expr": _chain("*", [1, 2, 3])}))
    cases.append(("a*b*c*d", {"nargs": 4, "expr": _chain("*", [1, 2, 3, 4])}))
    A = lambda i: ("arg", i)                                   # noqa: E731
    cases.append(("a+b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a+b-c", {"nargs": 3, "expr": ("binop", "-", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a-b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "-", A(1), A(2)), A(3))}))
    cases.append(("a*b+c", {"nargs": 3, "expr": ("binop", "+", ("binop", "*", A(1), A(2)), A(3))}))
    cases.append(("a+b*c", {"nargs": 3, "expr": ("binop", "+", A(1), ("binop", "*", A(2), A(3)))}))
    cases.append(("*p", {"nargs": 1, "expr": ("deref", A(1))}))
    cases.append(("p[2]", {"nargs": 1, "expr": ("index", A(1), ("const", 2))}))
    cases.append(("p[0]", {"nargs": 1, "expr": ("index", A(1), ("const", 0))}))
    for o in ("<", "<=", ">", ">=", "==", "!="):
        cases.append(("a%sb" % o, {"nargs": 2, "expr": ("cmp", o, A(1), A(2))}))
    cases.append(("a+1", {"nargs": 1, "expr": ("binop", "+", A(1), ("const", 1))}))
    cases.append(("a-1", {"nargs": 1, "expr": ("binop", "-", A(1), ("const", 1))}))
    cases.append(("store", {"nargs": 2, "expr": ("store", A(1), A(2))}))
    cases.append(("store0", {"nargs": 1, "expr": ("store", A(1), ("const", 0))}))

    def _sub(idxs):
        e = A(idxs[0])
        for i in idxs[1:]:
            e = ("binop", "-", e, A(i))
        return e
    cases.append(("a-b-c", {"nargs": 3, "expr": _sub([1, 2, 3])}))
    cases.append(("a-b-c-d", {"nargs": 4, "expr": _sub([1, 2, 3, 4])}))
    cases.append(("a-b-c-d-e", {"nargs": 5, "expr": _sub([1, 2, 3, 4, 5])}))
    cases.append(("p[i]", {"nargs": 2, "expr": ("index", A(1), A(2))}))
    cases.append(("a+b+c+d", {"nargs": 4, "expr": _chain("+", [1, 2, 3, 4])}))
    cases.append(("a+b+c+d+e", {"nargs": 5, "expr": _chain("+", [1, 2, 3, 4, 5])}))
    cases.append(("(a+b)*c", {"nargs": 3, "expr": ("binop", "*", ("binop", "+", A(1), A(2)), A(3))}))
    cases.append(("a*b+c*d", {"nargs": 4, "expr": ("binop", "+", ("binop", "*", A(1), A(2)),
                                                   ("binop", "*", A(3), A(4)))}))
    cases.append(("a&b&c", {"nargs": 3, "expr": _chain("&", [1, 2, 3])}))
    cases.append(("a*b-c*d", {"nargs": 4, "expr": ("binop", "-", ("binop", "*", A(1), A(2)),
                                                   ("binop", "*", A(3), A(4)))}))
    cases.append(("g()", {"nargs": 0, "expr": ("call", "__stdcall", [])}))
    cases.append(("g(a)", {"nargs": 1, "expr": ("call", "__stdcall", [A(1)])}))
    cases.append(("g(a,b)", {"nargs": 2, "expr": ("call", "__stdcall", [A(1), A(2)])}))
    cases.append(("g(a,b,c)", {"nargs": 3, "expr": ("call", "__stdcall", [A(1), A(2), A(3)])}))
    cases.append(("g_cdecl(a)", {"nargs": 1, "expr": ("call", "__cdecl", [A(1)])}))
    cases.append(("getG", {"nargs": 0, "expr": ("gread", "G")}))
    cases.append(("setGc", {"nargs": 0, "expr": ("gwrite", "G", ("const", 5))}))
    cases.append(("setG", {"nargs": 1, "expr": ("gwrite", "G", A(1))}))
    cases.append(("incG", {"nargs": 0, "expr": ("gincdec", "G", 1)}))
    cases.append(("P(a)", {"nargs": 1, "expr": ("icall", "__stdcall", [A(1)])}))
    cases.append(("P(a,b)", {"nargs": 2, "expr": ("icall", "__stdcall", [A(1), A(2)])}))
    cases.append(("empty0", {"nargs": 0, "expr": ("empty",)}))
    cases.append(("empty1", {"nargs": 1, "expr": ("empty",)}))
    cases.append(("empty2", {"nargs": 2, "expr": ("empty",)}))
    cases.append(("neg", {"nargs": 1, "expr": ("unary", "neg", A(1))}))
    cases.append(("not", {"nargs": 1, "expr": ("unary", "not", A(1))}))
    cases.append(("lognot", {"nargs": 1, "expr": ("unary", "lognot", A(1))}))
    cases.append(("a*a", {"nargs": 1, "expr": ("binop", "*", A(1), A(1))}))
    cases.append(("g(a)+b", {"nargs": 2, "expr": ("callbin", "+", "__stdcall", [A(1)], A(2))}))
    cases.append(("g(a)+c", {"nargs": 1, "expr": ("callbin", "+", "__stdcall", [A(1)], ("const", 1))}))
    cases.append(("g(a,b)&c", {"nargs": 2, "expr": ("callbin", "&", "__stdcall", [A(1), A(2)], ("const", 7))}))
    return cases


def declined_battery():
    """Shapes the model must DECLINE (emit -> None): kept as guards against a wrong guess."""
    A = lambda i: ("arg", i)                                   # noqa: E731
    return [
        ("a/b", {"nargs": 2, "expr": ("binop", "/", A(1), A(2))}),          # division not modeled
        ("a<<b", {"nargs": 2, "expr": ("binop", "<<", A(1), A(2))}),        # shifts not modeled
    ]


# ===========================================================================
# 32-BIT FRAME-LAYOUT MODEL  (the early_c_frame.py analogue)
#   * frame:  push ebp; mov ebp,esp; sub esp,N   /   epilogue: mov esp,ebp; pop ebp; ret[4*nargs]
#   * each local's size rounds UP to 4 (char[10] -> 12).
#   * ORDERING: scalars first (top, nearest ebp), then arrays; within each class, DECLARATION order.
#   * offset(local) = -(cumulative rounded size through that local); framesize = that cumulative.
# ===========================================================================
def frame_layout(locals_):
    """locals_ = [(size_bytes, is_array), ...] in DECLARATION order.
    Returns (framesize, {decl_index: ebp_offset}) -- offset negative, i.e. [ebp+offset]."""
    return native_invert.msvc2_frame_layout(locals_)


def _frame_battery():
    # (name, C source, locals-in-decl-order[(size,is_array)])
    G = "extern void __stdcall g(void*); "
    return [
        ("1int",   G + "int __stdcall f(void){ int a; g(&a); return a; }", [(4, False)]),
        ("2int",   G + "int __stdcall f(void){ int a,b; g(&a); g(&b); return a+b; }", [(4, False), (4, False)]),
        ("3int",   G + "int __stdcall f(void){ int a,b,c; g(&a); g(&b); g(&c); return a+b+c; }",
         [(4, False), (4, False), (4, False)]),
        ("2int_rev", G + "int __stdcall f(void){ int a,b; g(&b); g(&a); return a+b; }", [(4, False), (4, False)]),
        ("buf8",   G + "int __stdcall f(void){ char buf[8]; g(buf); return 0; }", [(8, True)]),
        ("buf10",  G + "int __stdcall f(void){ char buf[10]; g(buf); return 0; }", [(10, True)]),
        ("int_buf", G + "int __stdcall f(void){ int a; char buf[8]; g(&a); g(buf); return a; }",
         [(4, False), (8, True)]),
        ("buf_int", G + "int __stdcall f(void){ char buf[6]; int a; g(buf); g(&a); return a; }",
         [(6, True), (4, False)]),
    ]


def _extract_frame(code):
    """From compiled bytes: (framesize from `sub esp,N`, sorted set of distinct [ebp-X] offsets)."""
    import re
    from codebreaker import platforms
    fs = 0
    offs = set()
    for ins in platforms.decoder(32).disasm(code, 0):
        if ins.mnemonic == "sub" and ins.op_str.startswith("esp,"):
            fs = int(ins.op_str.split(",")[1], 0)
        for x in re.findall(r"\[ebp - (0x[0-9a-f]+|\d+)\]", ins.op_str):
            offs.add(-int(x, 0))
    return fs, sorted(offs)


def frame_selftest(show=False):
    cc32 = _oracle()
    ok = tot = 0
    for name, src, locals_ in _frame_battery():
        tot += 1
        pred_fs, pred_off = frame_layout(locals_)
        pred_set = sorted(set(pred_off.values()))
        r = cc32.compile_c(src, flags="/c /O1 /Oy-")
        if "obj" not in r:
            print("  %-8s ORACLE-FAIL" % name)
            continue
        fns = cc32.Obj(r["obj"]).functions()
        fn = next((v for k, v in fns.items() if k.startswith("_f")), None)
        real_fs, real_off = _extract_frame(bytes(fn[0])) if fn else (-1, [])
        match = (pred_fs == real_fs) and (pred_set == real_off)
        ok += match
        if show or not match:
            print("  %-8s %s  pred(fs=%d,off=%s) real(fs=%d,off=%s)" %
                  (name, "OK " if match else "MISS", pred_fs, pred_set, real_fs, real_off))
    print("\nmsvc2 frame-layout self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    return ok == tot


def _oracle():
    """The 1994 32-bit compiler the model is checked against (invert/compilers.py)."""
    from codebreaker.invert import compilers
    return compilers.msvc2()


def selftest(show=False):
    cc32 = _oracle()
    ok = tot = 0
    for name, node in battery():
        pred = emit(node)
        tot += 1
        r = cc32.compile_c(to_c(node), flags="/c /O1 /Oy-")
        if "obj" not in r:
            print("  %-8s ORACLE-FAIL %s" % (name, str(r.get("err", ""))[:80]))
            continue
        fns = cc32.Obj(r["obj"]).functions()
        fn = next((v for k, v in fns.items() if k.startswith("_f")), None)
        real = bytes(fn[0]) if fn else b""
        mask = set(fn[1]) if fn else set()               # reloc byte positions -> ignore
        match = pred is not None and len(pred) == len(real) and all(
            pred[i] == real[i] for i in range(len(real)) if i not in mask)
        ok += match
        if show or not match:
            tag = "OK " if match else "MISS"
            print("  %-8s %s  pred=%s  real=%s" % (name, tag,
                  pred.hex(" ") if pred else "(declined)", real.hex(" ")))
    print("\nmsvc2model self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    dec_ok = dec_tot = 0
    for name, node in declined_battery():
        dec_tot += 1
        d = emit(node) is None
        dec_ok += d
        if show or not d:
            print("  declined %-8s %s" % (name, "OK (None)" if d else "LEAK: emitted bytes!"))
    print("declined-correctly: %d/%d  (unmodeled shapes return None, never mis-modeled)"
          % (dec_ok, dec_tot))
    return ok == tot and dec_ok == dec_tot


def main():
    args = sys.argv[1:]
    show = "--show" in args
    ok = frame_selftest(show=show) if "--frame" in args else selftest(show=show)
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
