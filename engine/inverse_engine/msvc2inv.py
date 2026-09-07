#!/usr/bin/env python3
r"""msvc2inv.py - INVERSE of msvc2model: MSVC 2.0 (win95 C2.EXE) bytes -> byte-exact C.

The 32-bit sibling of mc2inv.py. Recognises the codegen templates msvc2model emits and recovers
the source expression, then SELF-VERIFIES: a candidate is accepted only if msvc2model.emit(node)
reproduces the input bytes exactly (relocation-free leaf functions), so every returned C is
byte-exact by construction -- never a plausible-but-wrong guess. Out-of-class byte sequences are
cleanly DECLINED (return None), exactly like the 16-bit engine.

Scope = the modeled forward class (standard /Oy- frame): single-`return` __stdcall leaf fns over
int args/consts -- const, arg, arg<op>const, arg<op>arg, multiplicative / bitwise / subtractive
chains, additive chains, mixed +/- three-operand, product +/- leaf, two products, comparisons,
pointer deref / index, global read/write/inc, extern & indirect calls.

    wt inverse msvc2inv                 # round-trip battery: bytes -> C -> (oracle) -> bytes
"""
import re
import sys
from engine.inverse_engine import msvc2model as M

try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    _MD = Cs(CS_ARCH_X86, CS_MODE_32)
except Exception:
    _MD = None


def _dis(code):
    return [(i.mnemonic, i.op_str) for i in _MD.disasm(bytes(code), 0)]


def _argk(opstr):
    """arg index from a stack mem operand: framed `[ebp + 4 + 4k]`, or frameless (/Oy)
    `[esp + 4k]` (arg1 at [esp+4]).  Base register disambiguates the two conventions."""
    m = re.search(r"\[(e[sb]p) \+ (0x[0-9a-f]+|\d+)\]", opstr)
    if not m:
        return None
    base, off = m.group(1), int(m.group(2), 0)
    if base == "esp":                                # frameless: arg1 at [esp+4], argK at [esp+4K]
        return off // 4 if (off >= 4 and off % 4 == 0) else None
    return (off - 4) // 4 if (off >= 8 and (off - 4) % 4 == 0) else None   # framed: [ebp+4+4K]


def _sconst(s):
    """Parse an immediate string to a signed 32-bit int."""
    v = int(s, 0)
    return v - 0x100000000 if v > 0x7fffffff else v


def _cst(s):
    """('const', signed int) from an immediate string, or None if not a number."""
    try:
        return ("const", _sconst(s))
    except ValueError:
        return None


def _nargs_from_ret(ins):
    mn, op = ins[-1]
    if mn == "ret" and op:                       # ret imm  -> nargs = imm/4
        return int(op, 0) // 4
    return 0


_ALU_MN = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}


def _leaf_ops(ins):
    """Strip frame scaffolding (push/mov ebp,esp/pop ebp/ret); return the compute instrs."""
    out = []
    for mn, op in ins:
        if (mn, op) in (("push", "ebp"), ("mov", "ebp, esp"), ("pop", "ebp")):
            continue
        if mn == "ret":
            continue
        out.append((mn, op))
    return out


def _maxarg(e):
    """Largest arg index referenced in an expr (register args carry no ret-imm, so nargs must be
    inferred from usage: ecx=arg1, edx=arg2)."""
    if e[0] == "arg":
        return e[1]
    if e[0] in ("deref", "index"):
        return _maxarg(e[1])
    if e[0] == "binop":
        return max(_maxarg(e[2]), _maxarg(e[3]))
    return 0


def _fastcall_candidates(ops, _nargs):
    """Register-convention (__fastcall/__thiscall) leaf shapes: ecx=arg1, edx=arg2, no frame,
    result in eax.  Yields nodes tagged cc='fastcall' (emit-verified by the caller)."""
    REG = {"ecx": 1, "edx": 2}
    if not ops:
        return

    def fc(expr):
        return {"nargs": _maxarg(expr), "cc": "fastcall", "expr": expr}

    m0, o0 = ops[0]
    if len(ops) == 1 and m0 == "mov" and o0 in ("eax, ecx", "eax, edx"):     # return arg1/arg2
        yield fc(("arg", REG[o0.split(", ")[1]]))
        return
    if len(ops) == 1 and m0 == "lea" and o0 == "eax, [edx + ecx]":           # arg1 + arg2
        yield fc(("binop", "+", ("arg", 1), ("arg", 2)))
        return
    mm = re.match(r"eax, (?:dword ptr )?\[(ecx|edx)(?: \+ (0x[0-9a-f]+|\d+))?\]$", o0) if m0 == "mov" else None
    if mm:
        k = REG[mm.group(1)]
        d = int(mm.group(2), 0) if mm.group(2) else 0
        if len(ops) == 1:
            if d == 0:                                                       # *arg
                yield fc(("deref", ("arg", k)))
            if d % 4 == 0:                                                   # arg[k]
                yield fc(("index", ("arg", k), ("const", d // 4)))
            return
        if len(ops) == 2 and d == 0 and k == 1:                             # (*arg1) <op> arg2
            m1, o1 = ops[1]
            if m1 in _ALU_MN and o1 == "eax, edx":
                yield fc(("binop", _ALU_MN[m1], ("deref", ("arg", 1)), ("arg", 2)))
            return
    return


def _candidates(code):
    """Yield plausible (nargs, expr) nodes for the byte sequence -- each is emit-verified by the
    caller, so over-generation is safe. Mirrors the standard-/Oy- msvc2model templates."""
    ins = _dis(code)
    if not ins:
        return
    nargs = _nargs_from_ret(ins)
    ops = _leaf_ops(ins)

    def loadk(reg):
        return next((_argk(o) for m, o in ops if m == "mov" and o.startswith(reg + ",")
                     and _argk(o) is not None), None)

    # register-convention (__fastcall/__thiscall): first compute reads ecx/edx as an incoming
    # register arg (not loaded from the stack) -- a distinct convention, detect and route.
    if (ops and ops[0][0] in ("mov", "lea") and ops[0][1].startswith("eax,")
            and re.search(r"\b(ecx|edx)\b", ops[0][1])
            and "esp" not in ops[0][1] and "ebp" not in ops[0][1]):
        yield from _fastcall_candidates(ops, nargs)
        return

    # --- constants (no frame): xor eax,eax / push imm;pop eax / mov eax,imm32 ---
    if len(ops) == 1 and ops[0] == ("xor", "eax, eax"):
        yield {"nargs": nargs, "expr": ("const", 0)}
        return
    if len(ops) == 2 and ops[0][0] == "push" and ops[1] == ("pop", "eax"):
        try:
            yield {"nargs": nargs, "expr": ("const", _sconst(ops[0][1]))}
        except ValueError:
            pass
        return
    if (len(ops) == 1 and ops[0][0] == "mov" and ops[0][1].startswith("eax, ")
            and "[" not in ops[0][1] and "ptr" not in ops[0][1]):
        try:
            yield {"nargs": nargs, "expr": ("const", _sconst(ops[0][1].split(", ")[1]))}
        except ValueError:
            pass
        return

    # --- call an extern: tail-jmp (no args) or push args; call (rel32 / indirect [P]) ---
    if len(ins) == 1 and ins[0][0] == "jmp":
        yield {"nargs": nargs, "expr": ("call", "__stdcall", [])}
        return
    if any(m == "call" for m, o in ops):
        ci = next(i for i, (m, o) in enumerate(ops) if m == "call")
        callop = ops[ci][1]
        pushes = [_argk(o) for m, o in ops if m == "push" and o.startswith("dword")]
        if pushes and all(k is not None for k in pushes):
            conv = "__cdecl" if ("pop", "ecx") in ops else "__stdcall"   # cdecl cleans via pop ecx
            args = [("arg", k) for k in reversed(pushes)]                 # push order = reverse of source
            kind_ = "icall" if "[" in callop else "call"
            yield {"nargs": nargs, "expr": (kind_, conv, args)}
            # callbin: exactly one op on eax AFTER a direct __stdcall  ->  g(args) <op> {arg|const}
            tail = ops[ci + 1:]
            if kind_ == "call" and conv == "__stdcall" and len(tail) == 1:
                tm, to = tail[0]
                op = other = None
                if tm == "inc":
                    op, other = "+", ("const", 1)
                elif tm == "dec":
                    op, other = "-", ("const", 1)
                elif tm == "lea" and re.match(r"eax, \[eax \+ eax\*4\]$", to):
                    op, other = "*", ("const", 5)
                elif tm == "imul":
                    k = _argk(to)
                    op, other = "*", (("arg", k) if k is not None else _cst(to.split(", ")[-1]))
                elif tm in _ALU_MN:
                    k = _argk(to)
                    op, other = _ALU_MN[tm], (("arg", k) if k is not None else _cst(to.split(", ")[1]))
                if op is not None and other is not None:
                    yield {"nargs": nargs, "expr": ("callbin", op, conv, args, other)}
        return

    # --- globals: read (mov eax,[abs]), write imm/arg, inc/dec [abs] ---
    _ABS = r"(?:dword ptr )?\[(0x[0-9a-f]+|\d+)\]"
    if len(ops) == 1 and ops[0][0] == "mov" and re.match(r"eax, " + _ABS + r"$", ops[0][1]):
        yield {"nargs": nargs, "expr": ("gread", "G")}
        return
    gw = next((o for m, o in ops if m == "mov" and re.match(_ABS + r", ", o)), None)
    if gw:
        rhs = re.match(_ABS + r", (.+)$", gw).group(2)
        if rhs == "eax":
            ak = loadk("eax")
            if ak is not None:
                yield {"nargs": nargs, "expr": ("gwrite", "G", ("arg", ak))}
        else:
            try:
                yield {"nargs": nargs, "expr": ("gwrite", "G", ("const", _sconst(rhs)))}
            except ValueError:
                pass
        return
    idc = next(((m, o) for m, o in ops if m in ("inc", "dec") and re.match(_ABS + r"$", o)), None)
    if idc:
        yield {"nargs": nargs, "expr": ("gincdec", "G", 1 if idc[0] == "inc" else -1)}
        return

    # --- store *p = value :  mov ecx,[p]; mov eax,[v]; mov [ecx],eax  (or and[eax],0 / mov[eax],imm) ---
    if any(m == "mov" and re.match(r"(?:dword ptr )?\[ecx\], eax$", o) for m, o in ops):
        pk, vk = loadk("ecx"), loadk("eax")
        if None not in (pk, vk):
            yield {"nargs": nargs, "expr": ("store", ("arg", pk), ("arg", vk))}
        return
    if any(m == "and" and re.match(r"(?:dword ptr )?\[eax\], 0$", o) for m, o in ops):
        pk = loadk("eax")
        if pk is not None:
            yield {"nargs": nargs, "expr": ("store", ("arg", pk), ("const", 0))}
        return
    stc = next((o for m, o in ops if m == "mov" and re.match(r"(?:dword ptr )?\[eax\], ", o)), None)
    if stc:
        pk = loadk("eax")
        rhs = re.match(r"(?:dword ptr )?\[eax\], (.+)$", stc).group(1)
        if pk is not None:
            try:
                yield {"nargs": nargs, "expr": ("store", ("arg", pk), ("const", _sconst(rhs)))}
            except ValueError:
                pass
        return

    # --- indexed load p[i] :  mov eax,[i]; mov ecx,[p]; mov eax,[ecx + eax*4] ---
    if any(m == "mov" and o.startswith("eax,") and "eax*4]" in o for m, o in ops):
        ik, pk = loadk("eax"), loadk("ecx")
        if None not in (ik, pk):
            yield {"nargs": nargs, "expr": ("index", ("arg", pk), ("arg", ik))}
        return

    # --- comparison -> 0/1 :  mov ecx,[L]; xor eax,eax; cmp ecx,[R]; setcc al ---
    setcc = next((m for m, o in ops if m.startswith("set")), None)
    if setcc:
        Lk = loadk("ecx")
        Rk = next((_argk(o) for m, o in ops if m == "cmp" and _argk(o) is not None), None)
        SET = {"setl": "<", "setle": "<=", "setg": ">", "setge": ">=", "sete": "==", "setne": "!="}
        if None not in (Lk, Rk) and setcc in SET:
            yield {"nargs": nargs, "expr": ("cmp", SET[setcc], ("arg", Lk), ("arg", Rk))}
        elif setcc == "sete" and Rk is not None:             # !a : xor eax,eax; cmp [a],eax; sete al
            yield {"nargs": nargs, "expr": ("unary", "lognot", ("arg", Rk))}
        return

    if not ops:                                              # empty body: void f(...) {}
        yield {"nargs": nargs, "expr": ("empty",)}
        return

    # the first instruction is a load into eax or ecx
    first_reg, k0 = None, None
    if ops[0][0] == "mov":
        rr = ops[0][1].split(",")[0].strip()
        if rr in ("eax", "ecx"):
            kk = _argk(ops[0][1])
            if kk is not None:
                first_reg, k0 = rr, kk

    n_imul = sum(1 for m, o in ops if m == "imul")
    has_addec = ("add", "eax, ecx") in ops
    has_subec = ("sub", "eax, ecx") in ops

    # --- two-register combines: eax=..., ecx=..., add/sub eax,ecx ---
    if first_reg == "eax" and (has_addec or has_subec):
        if n_imul == 2:                                       # (a*b) +/- (c*d)
            b, d = k0, loadk("ecx")
            a = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("eax,") and _argk(o) is not None), None)
            c = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("ecx,") and _argk(o) is not None), None)
            if None not in (a, b, c, d):
                op = "+" if has_addec else "-"
                yield {"nargs": nargs, "expr": ("binop", op,
                        ("binop", "*", ("arg", a), ("arg", b)),
                        ("binop", "*", ("arg", c), ("arg", d)))}
            return
        if has_addec and n_imul == 1:                        # (a+b) * c
            b, a = k0, loadk("ecx")
            c = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("eax,") and _argk(o) is not None), None)
            if None not in (a, b, c):
                yield {"nargs": nargs, "expr": ("binop", "*",
                        ("binop", "+", ("arg", a), ("arg", b)), ("arg", c))}
            return
        if has_addec:                                        # a+b(+c..): eax=last; ecx=2nd-last; +rest
            R, L = k0, loadk("ecx")
            extra = [_argk(o) for m, o in ops if m == "add" and o.startswith("eax,")
                     and o != "eax, ecx" and _argk(o) is not None]
            if L is not None:
                leaves = list(reversed(extra)) + [L, R]
                e = ("arg", leaves[0])
                for i in leaves[1:]:
                    e = ("binop", "+", e, ("arg", i))
                yield {"nargs": nargs, "expr": e}
            return
        return

    # --- product + leaf :  eax=pR; imul eax,[pL]; add eax,[leaf] ---
    if (first_reg == "eax" and any(m == "imul" and o.startswith("eax,") for m, o in ops)
            and any(m == "add" and o.startswith("eax,") and _argk(o) is not None for m, o in ops)):
        pR = k0
        pL = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("eax,") and _argk(o) is not None), None)
        leaf = next((_argk(o) for m, o in ops if m == "add" and o.startswith("eax,") and _argk(o) is not None), None)
        if None not in (pL, leaf):
            P = ("binop", "*", ("arg", pL), ("arg", pR))
            yield {"nargs": nargs, "expr": ("binop", "+", P, ("arg", leaf))}
            yield {"nargs": nargs, "expr": ("binop", "+", ("arg", leaf), P)}
        return

    # --- mixed +/- three-operand :  eax=P; sub eax,[M]; add eax,[H]  ->  (H + P) - M ---
    if (first_reg == "eax" and len(ops) == 3
            and ops[1][0] == "sub" and _argk(ops[1][1]) is not None
            and ops[2][0] == "add" and _argk(ops[2][1]) is not None):
        P, Mk, H = k0, _argk(ops[1][1]), _argk(ops[2][1])
        yield {"nargs": nargs, "expr": ("binop", "-",
                ("binop", "+", ("arg", H), ("arg", P)), ("arg", Mk))}
        return

    if first_reg != "eax":
        return

    # --- single-accumulator eax path: arg / deref / index-const / arg<op>{arg,const} / chains ---
    if len(ops) == 2 and ops[1][0] == "mov":                 # pointer deref / const-index
        m = re.match(r"eax, (?:dword ptr )?\[eax(?: \+ (0x[0-9a-f]+|\d+))?\]$", ops[1][1])
        if m:
            d = int(m.group(1), 0) if m.group(1) else 0
            if d == 0:
                yield {"nargs": nargs, "expr": ("deref", ("arg", k0))}
            if d % 4 == 0:
                yield {"nargs": nargs, "expr": ("index", ("arg", k0), ("const", d // 4))}
            return

    if len(ops) == 2 and ops[1][0] in ("neg", "not") and ops[1][1] == "eax":   # -a / ~a
        yield {"nargs": nargs, "expr": ("unary", ops[1][0], ("arg", k0))}
        return
    if len(ops) == 2 and ops[1] == ("imul", "eax, eax"):     # a*a  (compiler squares the loaded value)
        yield {"nargs": nargs, "expr": ("binop", "*", ("arg", k0), ("arg", k0))}
        return

    steps = []                                               # (op_char, kind, val) applied to eax
    ok = True
    for mn, op in ops[1:]:
        if mn == "inc":
            steps.append(("+", "const", 1))
        elif mn == "dec":
            steps.append(("-", "const", 1))
        elif mn == "lea" and re.match(r"eax, \[eax \+ eax\*4\]$", op):
            steps.append(("*", "const", 5))                  # lea [eax+eax*4] == *5
        elif mn == "imul":
            k = _argk(op)
            if k is not None:
                steps.append(("*", "arg", k))
            else:
                try:
                    steps.append(("*", "const", _sconst(op.split(", ")[-1])))
                except ValueError:
                    ok = False
                    break
        elif mn in _ALU_MN:
            k = _argk(op)
            if k is not None:
                steps.append((_ALU_MN[mn], "arg", k))
            else:
                try:
                    steps.append((_ALU_MN[mn], "const", _sconst(op.split(", ")[1])))
                except ValueError:
                    ok = False
                    break
        else:
            ok = False
            break
    if not ok:
        return
    if not steps:                                            # bare arg
        yield {"nargs": nargs, "expr": ("arg", k0)}
    elif len(steps) == 1:                                    # arg <op> {arg|const}
        o, kind, v = steps[0]
        R = ("arg", v) if kind == "arg" else ("const", v)
        yield {"nargs": nargs, "expr": ("binop", o, ("arg", k0), R)}
        if kind == "arg":                                    # commutative: try the other order too
            yield {"nargs": nargs, "expr": ("binop", o, R, ("arg", k0))}
    elif (all(kind == "arg" for _, kind, _ in steps)
          and len({o for o, _, _ in steps}) == 1 and steps[0][0] in ("*", "&", "|", "^")):
        o0 = steps[0][0]                                     # commutative chain (*, &, |, ^)
        idxs = list(reversed([k0] + [v for _, _, v in steps]))
        e = ("arg", idxs[0])
        for i in idxs[1:]:
            e = ("binop", o0, e, ("arg", i))
        yield {"nargs": nargs, "expr": e}
    elif all(o == "-" and kind == "arg" for o, kind, _ in steps):   # subtraction chain
        byte_order = [v for _, _, v in steps]                # subtrahends in byte order
        src = [byte_order[-1]] + byte_order[:-1]             # un-rotate: emit moved the 1st to last
        e = ("arg", k0)
        for i in src:
            e = ("binop", "-", e, ("arg", i))
        yield {"nargs": nargs, "expr": e}
    else:                                                    # generic left-fold
        e = ("arg", k0)
        for o, kind, v in steps:
            e = ("binop", o, e, ("arg", v) if kind == "arg" else ("const", v))
        yield {"nargs": nargs, "expr": e}
    return


def _match(pred, code, mask):
    if pred is None or len(pred) != len(code):
        return False
    return all(pred[i] == code[i] for i in range(len(code)) if i not in mask)


def _reloc_mask(code):
    """Byte positions in `code` that are relocation slots (a call/jmp rel32 target, or an
    absolute [disp32] memory operand) -- their bytes differ between our 0-placeholder and the
    shipped linked address, so they are ignored in the byte comparison."""
    m = set()
    for ins in _MD.disasm(bytes(code), 0):
        b, a = ins.bytes, ins.address
        if ins.mnemonic in ("call", "jmp"):
            if b[:1] in (b"\xe8", b"\xe9") and len(b) == 5:       # rel32 call/jmp (detect by opcode:
                m.update(range(a + 1, a + 5))                     # a 0-placeholder target prints as
            elif "]" in ins.op_str and len(b) >= 6:               # decimal '5', not '0x5', so key off e8/e9)
                m.update(range(a + len(b) - 4, a + len(b)))       # indirect [disp32] ff15/ff25
        elif b[:1] in (b"\xa1", b"\xa3"):                         # mov eax,[disp32] / mov [disp32],eax
            m.update(range(a + 1, a + 5))
        elif b[:2] == b"\xc7\x05":                                # mov dword[disp32], imm32
            m.update(range(a + 2, a + 6))
        elif b[:2] in (b"\xff\x05", b"\xff\x0d"):                 # inc/dec dword[disp32]
            m.update(range(a + 2, a + 6))
    return m


_FPO_KINDS = ("arg", "binop", "unary", "deref", "index")   # shapes whose bytes differ framed vs /Oy


def _is_frameless(ins):
    """True when the function has no `push ebp; mov ebp,esp` prologue (a /Oy leaf)."""
    return not (len(ins) >= 2 and ins[0] == ("push", "ebp") and ins[1] == ("mov", "ebp, esp"))


def invert(code, mask=None):
    """bytes -> {nargs, expr, c} byte-exact, or None if out of the modeled class. Relocation slots
    (call targets / global addrs) are auto-detected from each candidate and ignored, so this works
    directly on real LINKED code; pass `mask` to override.  Frameless (/Oy) leaves are recognised
    too -- an arg-using node from a no-frame function is emitted in its frameless form."""
    code = bytes(code)
    fpo = _is_frameless(_dis(code))
    for node in _candidates(code):
        try:
            n = node
            if fpo and node["expr"] and node["expr"][0] in _FPO_KINDS:
                n = dict(node)
                n["fpo"] = True
            pred = M.emit(n)
            if pred is None:
                continue
            m = set(mask) if mask is not None else _reloc_mask(pred)
            if _match(pred, code, m):
                return {"nargs": n["nargs"], "expr": n["expr"], "c": M.to_c(n)}
        except Exception:
            continue
    return None


def selftest(show=False):
    """Round-trip: oracle-compile each modeled shape -> bytes -> invert -> C -> re-check by
    msvc2model.emit (== the oracle bytes we already validate). True bytes->C->bytes."""
    cc32 = M._oracle()
    ok = tot = 0
    for name, node in M.battery():
        tot += 1
        r = cc32.compile_c(M.to_c(node), flags="/c /O1 /Oy-")
        fn = next((v for k, v in cc32.Obj(r["obj"]).functions().items() if k.startswith("_f")), None) if "obj" in r else None
        if not fn:
            print("  %-8s ORACLE-FAIL" % name)
            continue
        real = bytes(fn[0])
        mask = set(fn[1])
        inv = invert(real, mask)
        good = inv is not None and _match(M.emit({"nargs": inv["nargs"], "expr": inv["expr"]}), real, mask)
        ok += good
        if show or not good:
            print("  %-8s %s  %s" % (name, "OK " if good else "MISS",
                                     inv["c"] if inv else "(declined)"))
    print("\nmsvc2inv round-trip: %d/%d bytes->C->byte-exact  (MSVC 2.0 /O1 /Oy-)" % (ok, tot))
    return ok == tot


def main():
    sys.exit(0 if selftest(show="--show" in sys.argv[1:]) else 1)


if __name__ == "__main__":
    main()
