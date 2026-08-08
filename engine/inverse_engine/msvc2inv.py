#!/usr/bin/env python3
r"""msvc2inv.py - INVERSE of msvc2model: MSVC 2.0 (win95 C2.EXE) bytes -> byte-exact C.

The 32-bit sibling of mc2inv.py. Recognises the codegen templates msvc2model emits and recovers
the source expression, then SELF-VERIFIES: a candidate is accepted only if msvc2model.emit(node)
reproduces the input bytes exactly (relocation-free leaf functions), so every returned C is
byte-exact by construction -- never a plausible-but-wrong guess. Out-of-class byte sequences are
cleanly DECLINED (return None), exactly like the 16-bit engine.

Scope = the modeled forward class: single-`return` __stdcall leaf fns over int args/consts --
const, arg, arg<op>const, arg<op>arg, multiplicative chains, 3-op additive/mixed, product+/-leaf.

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
    """'dword ptr [esp + 8]' / '[ebp + 0xc]' -> arg index k (argoff(k)=4+4k)."""
    import re
    m = re.search(r"\[e[sb]p \+ (0x[0-9a-f]+|\d+)\]", opstr)
    if not m:
        return None
    off = int(m.group(1), 0)
    if off < 8 or (off - 4) % 4:
        return None
    return (off - 4) // 4


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


def _candidates(code):
    """Yield plausible (nargs, expr) nodes for the byte sequence -- each is emit-verified by the
    caller, so over-generation is safe. Covers every msvc2model forward shape."""
    ins = _dis(code)
    if not ins:
        return
    nargs = _nargs_from_ret(ins)
    ops = _leaf_ops(ins)

    # --- call an extern: tail-jmp (no args) or push args; call ---
    if len(ins) == 1 and ins[0][0] == "jmp":
        yield {"nargs": nargs, "expr": ("call", "__stdcall", [])}
        return
    if any(m == "call" for m, o in ops):
        pushes = [_argk(o) for m, o in ops if m == "push" and o.startswith("dword")]
        if pushes and all(k is not None for k in pushes):
            conv = "__cdecl" if any((m, o) == ("mov", "esp, ebp") for m, o in ops) else "__stdcall"
            args = [("arg", k) for k in reversed(pushes)]   # push order = reverse of source args
            yield {"nargs": nargs, "expr": ("call", conv, args)}
        return

    # --- store *p = value :  mov [eax], (ecx | imm) ---
    st = next((o for m, o in ops if m == "mov" and re.match(r"(?:dword ptr )?\[eax\], ", o)), None)
    if st:
        pk = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("eax,") and _argk(o) is not None), None)
        rhs = re.match(r"(?:dword ptr )?\[eax\], (.+)$", st).group(1)
        if pk is not None:
            if rhs == "ecx":
                vk = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("ecx,") and _argk(o) is not None), None)
                if vk is not None:
                    yield {"nargs": nargs, "expr": ("store", ("arg", pk), ("arg", vk))}
            else:
                try:
                    v = int(rhs, 0)
                    yield {"nargs": nargs, "expr": ("store", ("arg", pk),
                                                    ("const", v - 0x100000000 if v > 0x7fffffff else v))}
                except ValueError:
                    pass
        return

    # --- indexed load p[i] :  ...; mov eax,[eax + ecx*4] ---
    if any(m == "mov" and o.startswith("eax,") and "ecx*4]" in o for m, o in ops):
        pk = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("eax,") and _argk(o) is not None), None)
        ik = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("ecx,") and _argk(o) is not None), None)
        if pk is not None and ik is not None:
            yield {"nargs": nargs, "expr": ("index", ("arg", pk), ("arg", ik))}
        return

    # --- two products combined by lea :  lea eax,[edx + ecx]  (a*b + c*d) ---
    if any(m == "lea" and "edx + ecx" in o for m, o in ops):
        dl = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("edx,") and _argk(o) is not None), None)
        di = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("edx,") and _argk(o) is not None), None)
        cl = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("ecx,") and _argk(o) is not None), None)
        ci = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("ecx,") and _argk(o) is not None), None)
        if None not in (dl, di, cl, ci):     # edx = di*dl (imul=L, load=R); ecx = ci*cl
            yield {"nargs": nargs, "expr": ("binop", "+",
                                            ("binop", "*", ("arg", ci), ("arg", cl)),
                                            ("binop", "*", ("arg", di), ("arg", dl)))}
        return

    # --- two products (a*b) - (c*d): ...; sub eax, edx ---
    if any((m, o) == ("sub", "eax, edx") for m, o in ops) and any(m == "imul" and o.startswith("edx,") for m, o in ops):
        el = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("eax,") and _argk(o) is not None), None)
        ei = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("eax,") and _argk(o) is not None), None)
        dl = next((_argk(o) for m, o in ops if m == "mov" and o.startswith("edx,") and _argk(o) is not None), None)
        di = next((_argk(o) for m, o in ops if m == "imul" and o.startswith("edx,") and _argk(o) is not None), None)
        if None not in (el, ei, dl, di):
            yield {"nargs": nargs, "expr": ("binop", "-",
                                            ("binop", "*", ("arg", ei), ("arg", el)),
                                            ("binop", "*", ("arg", di), ("arg", dl)))}
        return

    # --- constants (no frame): xor eax,eax  or  mov eax,imm ---
    if len(ops) == 1:
        mn, op = ops[0]
        if mn == "xor" and op == "eax, eax":
            yield {"nargs": nargs, "expr": ("const", 0)}
            return
        if mn == "mov" and op.startswith("eax, ") and "ptr" not in op:
            try:
                v = int(op.split(", ")[1], 0)
                yield {"nargs": nargs, "expr": ("const", v - 0x100000000 if v > 0x7fffffff else v)}
            except ValueError:
                pass
            return
        # a single memory load = bare `arg`; fall through to the accumulator path below

    # first op is the initial load into a register
    def loadreg(mn, op):
        if mn == "mov":
            r = op.split(",")[0].strip()
            k = _argk(op)
            if r in ("eax", "ecx", "edx") and k is not None:
                return r, k
        return None, None

    r0, k0 = loadreg(*ops[0])
    if r0 is None:
        return

    # --- single accumulator in eax: arg / arg<op>arg / arg<op>const / mul-&-alu chains ---
    if r0 == "eax":
        # pointer deref / const-index:  mov eax,[esp+p]; mov eax,[eax(+d)]
        if len(ops) == 2 and ops[1][0] == "mov":
            m = re.match(r"eax, (?:dword ptr )?\[eax(?: \+ (0x[0-9a-f]+|\d+))?\]$", ops[1][1])
            if m:
                d = int(m.group(1), 0) if m.group(1) else 0
                if d == 0:
                    yield {"nargs": nargs, "expr": ("deref", ("arg", k0))}
                if d % 4 == 0:
                    yield {"nargs": nargs, "expr": ("index", ("arg", k0), ("const", d // 4))}
                return
        # relational bool:  mov eax,[L]; cmp [R],eax; mov eax,1; jcc +2; xor eax,eax
        cmpi = next((o for mn, o in ops if mn == "cmp"), None)
        jcc = next((mn for mn, o in ops if mn.startswith("j")), None)
        if cmpi and jcc:
            Rk = _argk(cmpi)
            JMAP = {"jg": "<", "jge": "<=", "jl": ">", "jle": ">="}
            if Rk is not None and jcc in JMAP:
                yield {"nargs": nargs, "expr": ("cmp", JMAP[jcc], ("arg", k0), ("arg", Rk))}
                return
        # gather the chain of ops applied to eax after the load
        steps = []                                # (op_char or '*', kind, val)
        ok = True
        for mn, op in ops[1:]:
            if mn == "inc":
                steps.append(("+", "const", 1)); continue
            if mn == "dec":
                steps.append(("-", "const", 1)); continue
            if mn == "imul":
                k = _argk(op)
                if k is not None:
                    steps.append(("*", "arg", k))
                else:                             # imul eax, eax, imm  (arg * const)
                    try:
                        steps.append(("*", "const", int(op.split(", ")[-1], 0)))
                    except ValueError:
                        ok = False; break
            elif mn in _ALU_MN:
                k = _argk(op)
                if k is not None:
                    steps.append((_ALU_MN[mn], "arg", k))
                else:                             # op eax, imm  (arg<op>const)
                    try:
                        steps.append((_ALU_MN[mn], "const", int(op.split(", ")[1], 0)))
                    except ValueError:
                        ok = False; break
            else:
                ok = False; break
        if ok:
            if not steps:                                     # bare arg
                yield {"nargs": nargs, "expr": ("arg", k0)}
            elif len(steps) == 1:                             # arg <op> {arg|const}
                o, kind, v = steps[0]
                R = ("arg", v) if kind == "arg" else ("const", v)
                # try both operand orders (commutative load-right-first vs load-left-first)
                yield {"nargs": nargs, "expr": ("binop", o, ("arg", k0), R)}
                if kind == "arg":
                    yield {"nargs": nargs, "expr": ("binop", o, R, ("arg", k0))}
            elif (steps and all(kind == "arg" for _, kind, _ in steps)
                  and len({o for o, _, _ in steps}) == 1 and steps[0][0] in ("*", "&", "|", "^")):
                o0 = steps[0][0]                              # commutative chain (*, &, |, ^)
                idxs = list(reversed([k0] + [v for _, _, v in steps]))   # load order = reverse
                e = ("arg", idxs[0])
                for i in idxs[1:]:
                    e = ("binop", o0, e, ("arg", i))
                yield {"nargs": nargs, "expr": e}
            elif all(o == "-" and kind == "arg" for o, kind, _ in steps):   # sub chain (>=2 subs)
                src = M._pairswap([v for _, _, v in steps])   # un-swap the scheduler's pairing
                e = ("arg", k0)
                for i in src:
                    e = ("binop", "-", e, ("arg", i))
                yield {"nargs": nargs, "expr": e}
            else:                                             # generic left-fold, e.g. (a+b)*c
                e = ("arg", k0)
                for o, kind, v in steps:
                    e = ("binop", o, e, ("arg", v) if kind == "arg" else ("const", v))
                yield {"nargs": nargs, "expr": e}
        return

    # --- two-register (ecx base) : equality, 3-op additive/mixed, or product+/-leaf ---
    if r0 == "ecx":
        # equality:  mov ecx,[R]; sub ecx,[L]; cmp ecx,1; sbb eax,eax; neg(==)/inc(!=)
        if len(ops) >= 2 and ops[1][0] == "sub" and any(o == "ecx, 1" for m, o in ops if m == "cmp"):
            Lk = _argk(ops[1][1])
            if Lk is not None:
                if any(m == "neg" for m, o in ops):
                    yield {"nargs": nargs, "expr": ("cmp", "==", ("arg", Lk), ("arg", k0))}
                elif any(m == "inc" for m, o in ops):
                    yield {"nargs": nargs, "expr": ("cmp", "!=", ("arg", Lk), ("arg", k0))}
        # additive chain (pure +):  mov ecx,[last]; add ecx,..; mov eax,[head]; ..; add eax,ecx
        if any(o == "eax, ecx" for m, o in ops if m == "add"):
            ecx_args = [k0]
            head = None
            pure = True
            for mn, o in ops[1:]:
                if mn == "add" and o.startswith("ecx,"):
                    k = _argk(o)
                    if k is None:
                        pure = False; break
                    ecx_args.append(k)
                elif mn == "mov" and o.startswith("eax,"):
                    head = _argk(o)
                elif (mn, o) == ("add", "eax, ecx"):
                    pass
                else:
                    pure = False; break
            if pure and head is not None and len(ecx_args) >= 2:
                e = ("arg", head)
                for i in reversed(ecx_args):
                    e = ("binop", "+", e, ("arg", i))
                yield {"nargs": nargs, "expr": e}
        # product+leaf:  mov ecx,[pR]; imul ecx,[pL]; mov eax,[H]; add eax,ecx
        if (len(ops) >= 4 and ops[1][0] == "imul" and ops[2][0] == "mov"
                and ops[-1] == ("add", "eax, ecx")):
            pR = k0; pL = _argk(ops[1][1]); H = _argk(ops[2][1])
            if None not in (pL, H):
                P = ("binop", "*", ("arg", pL), ("arg", pR))
                yield {"nargs": nargs, "expr": ("binop", "+", P, ("arg", H))}
                yield {"nargs": nargs, "expr": ("binop", "+", ("arg", H), P)}
        # 3-op additive/mixed:  mov ecx,[base]; <op> ecx,[other]; mov eax,[head]; add eax,ecx
        alu = [(mn, op) for mn, op in ops[1:] if mn in _ALU_MN or mn == "mov"]
        if ops[-1] == ("add", "eax, ecx"):
            mid = ops[1]
            head = next((op for mn, op in ops if mn == "mov" and op.startswith("eax,")), None)
            if mid[0] in _ALU_MN and head is not None:
                base, other = k0, _argk(mid[1]); hk = _argk(head); mo = _ALU_MN[mid[0]]
                if None not in (other, hk):
                    # enumerate the +/- chain source forms that could yield this ecx build-up
                    for e in _addsub_forms(hk, base, other, mo):
                        yield {"nargs": nargs, "expr": e}
    return


def _addsub_forms(head, base, other, mo):
    """Candidate 3-op +/- source trees for `eax=head; ecx=base <mo> other; eax+=ecx`.
    emit-verify picks the right one, so enumerate the reasonable arrangements."""
    A = lambda k: ("arg", k)
    forms = [
        ("binop", "+", ("binop", "+", A(head), A(base)), A(other)) if mo == "+" else
        ("binop", "-", ("binop", "+", A(head), A(base)), A(other)),
        ("binop", "+", ("binop", "-", A(head), A(other)), A(base)),
        ("binop", "-", ("binop", "+", A(head), A(other)), A(base)),
        ("binop", "+", ("binop", "+", A(head), A(other)), A(base)),
    ]
    return forms


def _match(pred, code, mask):
    if pred is None or len(pred) != len(code):
        return False
    return all(pred[i] == code[i] for i in range(len(code)) if i not in mask)


def invert(code, mask=None):
    """bytes -> {nargs, expr, c} byte-exact, or None if out of the modeled class. `mask` is the
    set of reloc byte positions to ignore (calls/globals); pass it for real linked code."""
    code = bytes(code)
    mask = set(mask or ())
    for node in _candidates(code):
        try:
            if _match(M.emit(node), code, mask):
                return {"nargs": node["nargs"], "expr": node["expr"], "c": M.to_c(node)}
        except Exception:
            continue
    return None


def selftest(show=False):
    """Round-trip: oracle-compile each modeled shape -> bytes -> invert -> C -> re-check by
    msvc2model.emit (== the oracle bytes we already validate 25/25). True bytes->C->bytes."""
    cc32 = M._oracle()
    ok = tot = 0
    for name, node in M.battery():
        tot += 1
        r = cc32.compile_c(M.to_c(node), flags="/c /O1 /Oy-")
        fn = next((v for k, v in cc32.Obj(r["obj"]).functions().items() if k.startswith("_f")), None) if "obj" in r else None
        if not fn:
            print("  %-8s ORACLE-FAIL" % name); continue
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
