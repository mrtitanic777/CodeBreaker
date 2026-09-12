#!/usr/bin/env python3
r"""msc6inv.py -- Compiler-Inverse for Microsoft C 6.00a (medium model, optimized),
the compiler that built the Win95 16-bit NE DLLs. Bytes -> byte-exact C by recognizing
the exact codegen TEMPLATES (not search), verified by recompiling with msc6_oracle.

Parallel to mc2inv.py (MS C 2.03). This is the SCAFFOLD + the first proven template
class; MSC 6.0a's optimized medium-model codegen (register allocation, /Ox peepholes,
the far calling conventions) is modeled one template at a time, each added only after it
round-trips byte-exact against the oracle -- probe -> model -> invert -> verify. Anything
outside a modeled template is cleanly DECLINED (returns None), never mis-reconstructed.

Templates modeled so far:
  T0  loadds far prologue/epilogue frame  (1e 58 90 45 55 8bec 1e 8ed8 [83 ec NN] ...
                                           4d 4d 8be5 1f 5d 4d ca NN NN | cb  [90])
  T1  leaf return-constant                (b8 <imm16>  -> `return <imm>;`; absent -> no
                                           return; an unused `int i` accounts for /Ox's
                                           SUB SP,2 + trailing NOP)

Usage:  python msc6inv.py <hexbytes>            # invert a function's code bytes -> C
        python msc6inv.py <hexbytes> --verify   # ... and recompile-verify via the oracle
"""
import re, sys

# fixed loadds far prologue MSC 6.0a emits under /Gsw (push ds;pop ax;nop;inc bp;push bp;
# mov bp,sp;push ds;mov ds,ax). The `1e 58 90` is the patchable MOV AX,DGROUP site.
PROLOGUE = bytes.fromhex("1e589045558bec1e8ed8")
# epilogue tail: dec bp;dec bp;mov sp,bp;pop ds;pop bp;dec bp  then retf n / retf
EPI = bytes.fromhex("4d4d8be51f5d4d")

# C condition == the INVERSE of the jcc that SKIPS the then-block (signed/unsigned)
JCC_INV = {0x74: "!=", 0x75: "==", 0x7c: ">=", 0x7d: "<", 0x7e: ">", 0x7f: "<=",
           0x72: ">=", 0x73: "<", 0x76: ">", 0x77: "<="}


def _leaf_stmt(bb):
    """A single terminal statement's C, or None. Shares the T1-T4 body templates."""
    if bb[:1] == b"\xb8" and len(bb) == 3:
        imm = bb[1] | (bb[2] << 8); return "return %d;" % (imm - 0x10000 if imm >= 0x8000 else imm)
    if bb[:2] == b"\x8b\x46" and len(bb) == 3:
        return "return a%d;" % ((bb[2] - 6) // 2)
    if bb[:1] == b"\xa1" and len(bb) == 3:
        return "return gG;"
    return None


def _recover(b):
    """Recover a straight-line/if block of bytes -> list of C statement strings, or None.
    Models the `cmp <lval>,imm ; jcc skip ; THEN ; jmp end ; skip: REST` if-pattern with
    leaf then/rest, recursively. Anything else declines (returns None)."""
    # cmp word [bp+NN], imm8   (83 7e NN VV)  then jcc rel8
    if b[:2] == b"\x83\x7e" and len(b) >= 5 and b[4] in JCC_INV:
        lval = "a%d" % ((b[2] - 6) // 2)
        val = b[3] if b[3] < 0x80 else b[3] - 0x100
        cc = JCC_INV[b[4]]; skip = 5 + b[5] if False else None
        rel = b[5]; skip_off = 6 + (rel - 0x100 if rel >= 0x80 else rel)  # target from after jcc(=6)
        then = b[6:skip_off]
        # then usually ends with `eb XX` (jmp to end); rest = b[skip_off:]
        if then[-2:-1] == b"\xeb":
            then_body = then[:-2]; rest = b[skip_off:]
        else:
            then_body = then; rest = b[skip_off:]
        ts = _leaf_stmt(then_body)
        if ts is None:
            return None
        out = ["if (%s %s %d)" % (lval, cc, val), "    " + ts]
        if rest:
            rs = _recover(rest)
            if rs is None:
                return None
            out += rs
        return out
    # a terminal leaf statement
    ls = _leaf_stmt(b)
    return [ls] if ls else None


def invert(code):
    """code:bytes -> (c_source, meta) for a modeled template, or (None, reason).

    Body templates (single-statement leaf class):
      empty  + frame + NOP  -> `{ int i; }`         (unused local == the frame)
      b8 imm16              -> `return <imm>;`       (T1 return-const)
      8b 46 NN              -> `return a<k>;`        (T2 return param at bp+NN)
      a1 <off16>           -> `return gG;`          (T3 return global; reloc masked)
      c7 06 <off16> <imm16> -> `gG = <imm>;` (void)  (T4 store-const to global)
    """
    b = bytes(code)
    if not b.startswith(PROLOGUE):
        return None, "prologue not the /Gsw loadds template (T0 miss)"
    p = len(PROLOGUE)
    frame = 0
    if b[p:p+2] == b"\x83\xec":                       # SUB SP,imm8 = /Ox working frame
        frame = b[p+2]; p += 3
    ei = b.find(EPI, p)
    if ei < 0:
        return None, "epilogue (T0 tail) not found"
    body = b[p:ei]
    q = ei + len(EPI)
    if b[q:q+1] == b"\xca":                            # retf n (pascal, n arg bytes)
        argbytes = b[q+1] | (b[q+2] << 8); q += 3
    elif b[q:q+1] == b"\xcb":                          # retf (0-arg pascal)
        argbytes = 0; q += 1
    else:
        return None, "no far return (ca/cb) after epilogue"
    trailing_nop = (q < len(b) and b[q] == 0x90)
    nargs = argbytes // 2

    # ---- recognize the body ----
    globals_used = {}; stmt = None; rtype = "int"; unused_local = False
    if body == b"":
        if frame and trailing_nop:
            unused_local = True                        # `{ int i; }` -- the frame IS the local
    elif body[:1] == b"\xb8" and len(body) == 3:       # T1
        imm = body[1] | (body[2] << 8)
        stmt = "return %d;" % (imm - 0x10000 if imm >= 0x8000 else imm)
    elif body[:2] == b"\x8b\x46" and len(body) == 3:   # T2  mov ax,[bp+NN]
        k = (body[2] - 6) // 2
        stmt = "return a%d;" % k
    elif body[:1] == b"\xa1" and len(body) == 3:       # T3  mov ax,[global]
        stmt = "return gG;"; globals_used["gG"] = "int"
    elif body[:2] == b"\xc7\x06" and len(body) == 6:   # T4  mov word[global],imm
        imm = body[4] | (body[5] << 8)
        stmt = "gG = %d;" % imm; globals_used["gG"] = "int"; rtype = "void"
    elif b"\x9a" in body:                              # T5  far call: [pushes] CALLF
        callidx = body.index(b"\x9a")
        if len(body) - callidx != 5:                   # exactly one CALLF, at the tail
            return None, "T5: code after the CALLF not modeled (%s)" % body.hex()
        pushes, i, ok = [], 0, True
        while i < callidx:
            if body[i] == 0x6a:                        # push imm8 (sign-extended)
                v = body[i+1]; pushes.append(str(v - 0x100 if v >= 0x80 else v)); i += 2
            elif body[i] == 0x68:                      # push imm16
                pushes.append(str(body[i+1] | (body[i+2] << 8))); i += 3
            elif body[i:i+2] == b"\xff\x76":           # push word [bp+NN] (a param)
                pushes.append("a%d" % ((body[i+2] - 6) // 2)); i += 3
            else:
                ok = False; break
        if not ok:
            return None, "T5: arg-push form not modeled (%s)" % body[:callidx].hex()
        na = len(pushes)
        gproto = "extern void far pascal G(%s);\n" % (", ".join(["int"] * na) or "void")
        stmt = "G(%s);" % ", ".join(pushes)
        decls_extra = gproto; rtype = "void"
        params = ", ".join("int a%d" % i for i in range(nargs)) or "void"
        decls = "".join("%s %s;\n" % (t, g) for g, t in globals_used.items())
        inner = "    %s\n" % stmt
        src = "%s%svoid far pascal F(%s)\n{\n%s}" % (decls, decls_extra, params, inner)
        return src, {"nargs": nargs, "call_args": pushes, "template": "T5 far-call"}
    elif body[:2] == b"\x83\x7e" and len(body) >= 5 and body[4] in JCC_INV:  # T6 if-branch
        lines = _recover(body)
        if lines is None:
            return None, "T6: branch structure not fully modeled (%s)" % body.hex()
        params = ", ".join("int a%d" % i for i in range(nargs)) or "void"
        inner = "".join("    %s\n" % ln for ln in lines)
        src = "int far pascal F(%s)\n{\n%s}" % (params, inner)
        return src, {"nargs": nargs, "template": "T6 if-branch", "lines": lines}
    else:
        return None, "body not a modeled leaf template (%d bytes: %s)" % (len(body), body.hex())

    params = ", ".join("int a%d" % i for i in range(nargs)) or "void"
    decls = "".join("%s %s;\n" % (t, g) for g, t in globals_used.items())
    inner = "    int i;\n" if unused_local else ("    %s\n" % stmt if stmt else "")
    src = "%s%s far pascal F(%s)\n{\n%s}" % (decls, rtype, params, inner)
    return src, {"nargs": nargs, "frame": frame, "trailing_nop": trailing_nop,
                 "rtype": rtype, "globals": list(globals_used), "template": stmt or ("int i" if unused_local else "empty")}


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        print(__doc__); sys.exit(2)
    code = bytes.fromhex(args[0])
    src, meta = invert(code)
    if src is None:
        print("DECLINED: %s" % meta); sys.exit(1)
    print(src)
    if "--verify" in sys.argv:
        from codebreaker.engines.inverse import msc6_oracle as ora
        for flags in ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os"):
            rc = ora.mc6_compile(src, flags)               # compile the emitted source as-is
            if rc == code:
                print("// VERIFIED byte-exact (%s): recompiles to the identical %d bytes" % (flags, len(code)))
                return
        print("// NOT byte-exact under the tried flags -- template incomplete for this function")


if __name__ == "__main__":
    import os
    main()
