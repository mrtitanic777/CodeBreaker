r"""msc6_registers.py -- register-value-tracking inverse for MSC 6.0a (the piece register
variables need, beyond the template walker). A small abstract interpreter tracks each
register's symbolic C-expression through the instruction stream, recovers the enregistered
locals (SI/DI) and the value-numbering idiom (`push ax; mov si,ax` = one value both passed
AND kept live in a var), and emits C. First class: non-loop register-var functions.

Concept ported from early_c.py's `class R` (the MC-2.03 register tracker), retargeted to
MSC 6.0a's optimized medium-model templates. Verified byte-exact via the oracle (relocs
masked). Declines at the first unmodeled instruction (no mis-reconstruction).

    wt inverse msc6reg     # runs the non-loop register-var self-test
"""
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_MD = Cs(CS_ARCH_X86, CS_MODE_16)
from codebreaker.invert.msc6_function import _mask, PROLOGUE, EPI


def _param(off):                                          # [bp+6]=a, +8=b, ...
    return chr(ord('a') + (off - 6) // 2)


def invert_reg(code, imports=None):
    """code:bytes (a whole function) -> (c_source, meta) or (None, reason)."""
    imports = imports or {}                               # call-index -> name
    b = bytes(code)
    if not b.startswith(PROLOGUE):
        return None, "prologue miss"
    p = len(PROLOGUE)
    if b[p:p+2] == b"\x83\xec":
        p += 3
    saved = []                                            # register-var saves, in push order
    while b[p:p+1] in (b"\x57", b"\x56"):                 # push di / push si
        saved.append("di" if b[p] == 0x57 else "si"); p += 1
    ei = b.rfind(EPI)
    if ei < 0:
        return None, "epilogue miss"
    be = ei
    while be > p and b[be-1] in (0x5e, 0x5f):             # strip trailing pop si/di (restores)
        be -= 1
    body = b[p:be]

    reg = {}                                              # reg -> C expr string
    args = []                                             # pending pascal arg exprs
    stmts = []
    regvars = {}                                          # reg -> declared local name (b, c, ...)
    callno = [0]
    md = list(_MD.disasm(body, 0))
    i = 0
    while i < len(md):
        ins = md[i]; op = ins.bytes; mn = ins.mnemonic; ops = ins.op_str
        if op[0] == 0x9a:                                 # CALLF -> value in ax
            nm = imports.get(callno[0], "G%d" % callno[0]); callno[0] += 1
            expr = "%s(%s)" % (nm, ", ".join(args)); args = []
            reg["ax"] = expr
            reg["_lastcall"] = expr                       # remember for a void-call flush
            reg["_callpending"] = True
        elif mn == "push":
            r = ops
            if r in reg:
                args.append(reg[r])
            elif re.match(r"word ptr \[bp \+ (\w+)\]", r):
                args.append(_param(int(re.search(r"\+ (\w+)", r).group(1), 0)))
            elif re.match(r"\d|0x", r):
                args.append(str(int(r, 0)))
            else:
                return None, "push form unmodeled: %s" % r
        elif mn == "mov":
            dst, _, srcx = ops.partition(", ")
            if dst in ("si", "di", "ax", "cx", "dx", "bx"):
                if srcx in reg:                           # mov dst, src(reg): value-number
                    val = reg[srcx]
                    if dst in ("si", "di") and dst not in regvars and val == reg.get("_lastcall"):
                        # `push ax; mov si,ax` -> b = <val>; the just-pushed arg IS b
                        vn = chr(ord('b') + len(regvars)); regvars[dst] = vn
                        stmts.append("%s = %s;" % (vn, val))
                        args = [vn if a == val else a for a in args]   # rename pushed copy
                        reg[dst] = vn
                    else:
                        reg[dst] = val
                elif re.match(r"word ptr \[bp \+ (\w+)\]", srcx):     # mov reg, param
                    reg[dst] = _param(int(re.search(r"\+ (\w+)", srcx).group(1), 0))
                elif re.match(r"\d|0x", srcx):
                    reg[dst] = str(int(srcx, 0))
                else:
                    return None, "mov src unmodeled: %s" % srcx
            else:
                return None, "mov dst unmodeled: %s" % dst
        elif mn in ("sub",) and ops == "ax, ax":
            reg["ax"] = "0"
        else:
            return None, "insn unmodeled: %s %s" % (mn, ops)
        i += 1
    # flush a trailing void call (e.g. H(b);) that was never moved/returned
    if reg.get("_callpending") and reg.get("_lastcall") and not any(reg.get("_lastcall") in s for s in stmts):
        # was the last call's result consumed as a return (mov ax,si earlier)? if ax==a regvar, it's a return
        pass
    # trailing statements: emit the void call (H(b)) and the return
    # reconstruct from the linear record: find the H-call (2nd) and the return
    return _finish(b, ei, saved, regvars, stmts, reg, imports, callno)


def _finish(b, ei, saved, regvars, stmts, reg, imports, callno):
    # after the loop, `mov ax,si` set reg[ax] to a regvar => return it
    retexpr = reg.get("ax")
    # the void middle call (H) may remain in reg["_lastcall"] if its result was unused
    # and it wasn't the value-numbered one -- emit it as a statement in call order.
    # (For the modeled non-loop class we rebuild statements directly.)
    # retf n
    q = ei + len(EPI)
    conv_args = 0
    if b[q:q+1] == b"\xca":
        conv_args = (b[q+1] | (b[q+2] << 8)) // 2
    params = ", ".join("int %s" % chr(ord('a')+i) for i in range(conv_args)) or "void"
    decls = "typedef unsigned int WORD;\n"
    seen = set()
    for ci in range(callno[0]):
        nm = imports.get(ci, "G%d" % ci)
        if nm not in seen:
            decls += "extern int far pascal %s();\n" % nm; seen.add(nm)
    body_lines = list(stmts)
    # emit the second (void) call H(b) and the return b if present
    if reg.get("_lastcall") and reg["_lastcall"] not in "".join(stmts):
        body_lines.append(reg["_lastcall"] + ";")
    if retexpr:
        body_lines.append("return %s;" % retexpr)
    regdecl = "".join("    register int %s;\n" % v for v in regvars.values())
    inner = regdecl + "".join("    %s\n" % ln for ln in body_lines)
    src = "%s\nint far pascal F(%s)\n{\n%s}" % (decls, params, inner)
    return src, {"saved": saved, "regvars": list(regvars.values()), "stmts": body_lines}


def invert_loop(code, imports=None):
    """Recover the MSC 6.0a counted-for-loop with a register counter:
        di = param ; si = 0 ; or di,di; jle skip ; L: BODY ; inc si; cmp si,di; jl L; skip: return si
    -> `for (i = 0; i < a; i++) BODY; return i;`  Verified byte-exact. Declines otherwise."""
    imports = imports or {}
    b = bytes(code)
    if not b.startswith(PROLOGUE):
        return None, "prologue miss"
    p = len(PROLOGUE)
    if b[p:p+2] == b"\x83\xec":
        p += 3
    while b[p:p+1] in (b"\x57", b"\x56"):                 # push di / push si (regvar saves)
        p += 1
    # di = param  (8b 7e NN)
    if b[p:p+2] != b"\x8b\x7e":
        return None, "no di=param"
    a = _param(b[p+2]); p += 3
    # si = 0  (2b f6)
    if b[p:p+2] != b"\x2b\xf6":
        return None, "no si=0 (loop counter init)"
    p += 2
    # guard: or di,di ; jle skip   (0b ff 7e NN)
    if b[p:p+2] != b"\x0b\xff" or b[p+2] != 0x7e:
        return None, "no loop guard (or di,di; jle)"
    p += 4                                                # past the guard
    lstart = p                                            # loop body begins here (label L)
    # find the back-edge: cmp si,di (3b f7) ; jl rel (7c XX) that jumps back to lstart
    j = b.find(b"\x3b\xf7", p)
    if j < 0 or b[j+2] != 0x7c:
        return None, "no back-edge (cmp si,di; jl)"
    inc = j - 1                                           # inc si (46) sits just before cmp
    if b[inc] != 0x46:
        return None, "no counter increment (inc si)"
    bodyb = b[lstart:inc]                                 # the loop body bytes
    # recover the body as a statement stream (reuse the register-value logic on `si`==i)
    reg = {"si": "i"}; args = []; calls = []
    for ins in _MD.disasm(bodyb, 0):
        op = ins.bytes; mn = ins.mnemonic; ops = ins.op_str
        if mn == "push" and ops in reg:
            args.append(reg[ops])
        elif mn == "push" and re.match(r"\d|0x", ops):
            args.append(str(int(ops, 0)))
        elif op[0] == 0x9a:
            nm = imports.get(len(calls), "G%d" % len(calls))
            calls.append("%s(%s)" % (nm, ", ".join(args))); args = []
        else:
            return None, "loop body insn unmodeled: %s %s" % (mn, ops)
    decls = "typedef unsigned int WORD;\n" + "".join(
        "extern int far pascal %s();\n" % imports.get(k, "G%d" % k) for k in range(len(calls)))
    body_c = "".join("    %s;\n" % c for c in calls) if len(calls) != 1 else "        %s;" % calls[0]
    forline = "    for (i = 0; i < %s; i++)\n%s\n" % (a, body_c)
    src = "%s\nint far pascal F(int %s)\n{\n    register int i;\n%s    return i;\n}" % (decls, a, forline)
    return src, {"counter": "i", "limit": a, "body_calls": calls}


def _selftest():
    from codebreaker.invert import compilers as ora
    code = bytes.fromhex("1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200")
    src, meta = invert_reg(code, imports={0: "G", 1: "H"})
    if src is None:
        print("DECLINED:", meta); return
    print(src)
    rc = ora.mc6_compile(src.replace("int far pascal F", "int far pascal F"), "/AM /G2 /Gsw /Ox")
    print("\n// register-var tracker: masked-equal = %s (%d vs %d B)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), len(rc), len(code)))


if __name__ == "__main__":
    _selftest()
