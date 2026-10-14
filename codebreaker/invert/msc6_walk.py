r"""msc6_walk.py -- generalized whole-function MSC 6.0a inverse: a symbol-map-driven
recursive statement walker (generalizing msc6fn.invert_cleanup beyond one idiom).

Walks a function's code as a STREAM of modeled statements -- returns, if-blocks (recursed),
far calls, long/word global stores -- emitting C, and DECLINES (returns None) at the first
unmodeled byte, so it never mis-reconstructs. Verified byte-exact by recompiling with the
oracle (relocs masked via msc6fn._mask).

Inputs: code bytes, a DGROUP symbol map {lo_off:(name,'long'|'word')}, an import map
{} (names come from the reloc table upstream; here a far call is `CALL_<seg:off>` unless
resolved). This is the validation battery over the functions we've proven byte-exact.
"""
import sys
from codebreaker.invert.msc6_function import _mask, PROLOGUE, EPI

JCC_INV = {0x74: "!=", 0x75: "==", 0x7c: ">=", 0x7d: "<", 0x7e: ">", 0x7f: "<="}


def _u16(b, i): return b[i] | (b[i+1] << 8)


class Decl:
    def __init__(self): self.globs = {}; self.imports = set()


def _pushes(b, p, end, syms, d):
    """Parse a pascal arg-push run; return (arg_strings, new_p) or (None,p)."""
    args = []
    while p < end:
        if b[p] == 0x6a:                                    # push imm8
            v = b[p+1]; args.append(str(v - 0x100 if v >= 0x80 else v)); p += 2
        elif b[p] == 0x68:                                  # push imm16
            args.append(str(_u16(b, p+1))); p += 3
        elif b[p:p+2] == b"\xff\x76":                       # push [bp+NN] param
            args.append("a%d" % ((b[p+2]-6)//2)); p += 3
        elif b[p:p+2] == b"\xff\x36":                       # push [off]: word global / long lane
            off = _u16(b, p+2)
            nm = syms.get(off); lo = syms.get(off-2)
            if nm and nm[1] == 'long':                       # off = LOW word of a long
                d.globs[nm[0]] = 'long'; args.append("(WORD)%s" % nm[0])
            elif lo and lo[1] == 'long':                     # off = HIGH word (lo+2) of a long
                d.globs[lo[0]] = 'long'; args.append("(WORD)(%s >> 16)" % lo[0])
            elif nm:                                         # a plain word global
                d.globs[nm[0]] = 'word'; args.append(nm[0])
            else:
                g = "gW%x" % off; d.globs[g] = 'word'; args.append(g)
            p += 4
        else:
            break
    return args, p


def _stmt(b, p, end, syms, d):
    """Recognize ONE statement at b[p:] -> (c_lines, new_p) or (None, p, reason)."""
    # far call: [pushes] 9a <reloc4>
    q0 = p
    args, q = _pushes(b, p, end, syms, d)
    if q < end and b[q] == 0x9a:
        d.imports.add("CALLEE"); p = q + 5
        return (["CALLEE(%s);" % ", ".join(args)], p)
    # long global store zero: 2b c0 ; a3 hi ; a3 lo
    if b[p:p+2] == b"\x2b\xc0" and b[p+2] == 0xa3 and b[p+5] == 0xa3:
        lo = _u16(b, p+6); nm = syms.get(lo)
        if nm and nm[1] == 'long':
            d.globs[nm[0]] = 'long'; return (["%s = 0L;" % nm[0]], p+8)
    # word global store const: c7 06 off imm
    if b[p:p+2] == b"\xc7\x06":
        off = _u16(b, p+2); imm = _u16(b, p+4)
        nm = (syms.get(off) or ("gW%x" % off, 'word')); d.globs[nm[0]] = 'word'
        return (["%s = %d;" % (nm[0], imm)], p+6)
    # return const/param/global just before the epilogue
    if b[p:p+1] == b"\xb8" and b[p+3:p+3+len(EPI)] == EPI:
        imm = _u16(b, p+1); return (["return %d;" % (imm-0x10000 if imm >= 0x8000 else imm)], p+3)
    if b[p:p+2] == b"\x8b\x46" and b[p+3:p+3+len(EPI)] == EPI:
        return (["return a%d;" % ((b[p+2]-6)//2)], p+3)
    # long-global non-zero test  a1 hi ; 0b06 lo ; je skip   -> if (name != 0L) { then }
    if b[p:p+1] == b"\xa1" and b[p+3:p+5] == b"\x0b\x06" and b[p+7] == 0x74:
        lo = _u16(b, p+5); nm = syms.get(lo)
        if nm and nm[1] == 'long':
            d.globs[nm[0]] = 'long'
            skip = p + 9 + b[p+8]
            then, tp = _walk(b, p+9, skip, syms, d)
            if then is None:
                return (None, p, "then-block unmodeled")
            return (["if (%s != 0L) {" % nm[0]] + ["    " + t for t in then] + ["}"], skip)
    return (None, p, "unmodeled @+%d: %s" % (p-q0, b[p:p+6].hex()))


def _walk(b, p, end, syms, d):
    out = []
    while p < end:
        r = _stmt(b, p, end, syms, d)
        if r[0] is None:
            return None, r[2] if len(r) > 2 else "decline"
        lines, p = r
        out += lines
    return out, p


def invert(code, syms, callee="G"):
    b = bytes(code)
    if not b.startswith(PROLOGUE):
        return None, "prologue miss"
    p = len(PROLOGUE)
    if b[p:p+2] == b"\x83\xec":
        p += 3
    ei = b.rfind(EPI)
    if ei < 0:
        return None, "epilogue miss"
    d = Decl()
    lines, why = _walk(b, p, ei, syms, d)
    if lines is None:
        return None, why
    decls = "typedef unsigned int WORD;\n"
    if d.imports:
        decls += "extern WORD far pascal %s(WORD);\n" % callee
    for g, t in d.globs.items():
        decls += "extern %s %s;\n" % ("long" if t == 'long' else "int", g)
    body = "".join("    %s\n" % ln.replace("CALLEE", callee) for ln in lines)
    src = "%s\nvoid far F(void)\n{\n%s}" % (decls, body)
    return src, {"stmts": len(lines), "globals": dict(d.globs)}


if __name__ == "__main__":
    # regression: Cleanup must invert + verify through the generalized walker
    from codebreaker.invert import compilers as ora
    code = bytes.fromhex(
        "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a344"
        "00a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
        "009affff00002bc0a35600a354004d4d8be51f5d4dcb90")
    syms = {0x44: ("gpA", 'long'), 0x48: ("gpB", 'long'), 0x54: ("gpC", 'long')}
    src, meta = invert(code, syms, callee="GlobalFree")
    if src is None:
        print("DECLINED:", meta); sys.exit(1)
    print(src)
    rc = ora.mc6_compile(src.replace("void far F", "void far Cleanup"), "/AM /G2 /Gsw /Ox")
    print("\n// generalized walker: masked-equal = %s (%d stmts)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), meta["stmts"]))
