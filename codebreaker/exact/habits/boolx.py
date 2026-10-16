#!/usr/bin/env python3
"""boolx.py - do two stretches of 8086 code decide a condition the same way?

Used by newcomp.py for MASM 3.00. Its compiler leaves a condition at the first term that settles
it; MS-Pascal 3.11 does so too, except where the terms share the record of a WITH and are grouped
so that AND THEN / OR ELSE cannot say it: there it works every term out to 0 or 1 (mov r,0 / jcc /
inc r), combines them with AND and OR and tests the result once. The two are the same condition if,
whatever the comparisons in them come out as, both go on to the same place.

Each stretch is run on every outcome of its comparisons: a comparison of the same two operands is
one unknown (less, equal or greater), a Boolean byte one (0 or 1), a word masked with a constant
one (zero or not). A stretch may load registers, compare, combine the 0/1 values and keep one in a
temporary of the frame; it leaves by falling through its end or by a jump out of it. Anything else
it refuses.
"""
import re

class No(Exception): pass
class Need(Exception):
    def __init__(self, key, dom): self.key = key; self.dom = dom

R8 = {"al": "ax", "cl": "cx", "dl": "dx", "bl": "bx"}
R16 = ("ax", "cx", "dx", "bx")
JCC = ("je", "jne", "jb", "jae", "ja", "jbe")
MEM = re.compile(r"(?:byte ptr |word ptr )?\[([^\]]+)\]")

def run(ins, lo, hi, assign, init=None):
    """ins: [(offset, text)] of the stretch at offsets lo..hi -> ('end',) or ('out', target offset)"""
    at = {o: k for k, (o, t) in enumerate(ins)}; regs = {}; temps = {}; base = dict(init or {"si": "si0", "di": "di0"}); flags = None; k = 0; steps = 0
    def atom(key, dom):
        if key not in assign: raise Need(key, dom)
        return assign[key]
    def memkey(inner):
        return re.sub(r"\b(si|di)\b", lambda m: "<%s>" % base[m.group(1)], inner)
    def val(op):
        """an operand as a value: an int, or ('mem', key)"""
        op = op.strip()
        if op in R16: return regs.get(op, ("reg", op))
        if op in R8: return regs.get(R8[op], ("reg", R8[op]))
        if re.fullmatch(r"0x[0-9a-f]+|\d+", op): return int(op, 0)
        m = MEM.fullmatch(op)
        if m:
            key = memkey(m.group(1))
            if key in temps: return temps[key]
            return ("mem", key)
        raise No("operand " + op)
    def boolean(v):
        if isinstance(v, int): return v & 1
        if v[0] == "mem": return atom(("bool", v[1]), (0, 1))
        raise No("not a Boolean: %r" % (v,))
    while k < len(ins):
        steps += 1
        if steps > 400: raise No("loop")
        o, t = ins[k]; m = re.match(r"(\w+)\s*(.*)", t); op, rest = m.group(1), m.group(2); a = [x.strip() for x in rest.split(",")] if rest else []
        if op in JCC or op == "jmp":
            tgt = int(a[0], 0)
            if op == "jmp": take = True
            else:
                if flags is None: raise No("jump without flags")
                if flags[0] == "cmp":
                    s = flags[1]; take = {"je": s == "eq", "jne": s != "eq", "jb": s == "lt", "jae": s != "lt", "ja": s == "gt", "jbe": s != "gt"}[op]
                else:
                    zf, cf = flags[1], flags[2]
                    if op in ("je", "jne"): take = zf if op == "je" else not zf
                    elif op in ("jb", "jae"):
                        if cf is None: raise No("carry not known")
                        take = cf if op == "jb" else not cf
                    else: raise No(op + " after a logical operation")
            if take:
                if tgt == hi: return ("end",)
                if lo <= tgt < hi:
                    if tgt not in at: raise No("jump into an instruction")
                    k = at[tgt]; continue
                return ("out", tgt)
            k += 1; continue
        if op == "mov":
            d, s = a
            if d in ("si", "di"): base[d] = memkey(MEM.fullmatch(s).group(1)) if MEM.fullmatch(s) else s; k += 1; continue
            if d in R16 or d in R8: regs[R8.get(d, d)] = val(s); k += 1; continue
            md = MEM.fullmatch(d)
            if md and md.group(1).startswith("bp - ") and (s in R16 or s in R8): temps[memkey(md.group(1))] = val(s); k += 1; continue
            raise No("store " + t)
        if op == "inc" and a[0] in R16:
            v = regs.get(a[0])
            if not isinstance(v, int): raise No("inc of an unknown")
            regs[a[0]] = v + 1; k += 1; continue
        if op == "xor" and a[0] in ("ah", "ch", "dh", "bh") and a[1] == a[0]: k += 1; continue
        if op == "xor" and a[0] in R8 and a[1] == "1":
            r = R8[a[0]]; regs[r] = 1 - boolean(regs.get(r, ("reg", r))); flags = ("log", regs[r] == 0, False); k += 1; continue
        if op in ("and", "or") and a[0] in R8:
            r = R8[a[0]]; x = boolean(regs.get(r, ("reg", r))); y = boolean(val(a[1]))
            regs[r] = (x & y) if op == "and" else (x | y); flags = ("log", regs[r] == 0, False); k += 1; continue
        if op == "and" and a[0] in R16 and re.fullmatch(r"0x[0-9a-f]+|\d+", a[1]):
            v = regs.get(a[0])
            if isinstance(v, tuple) and v[0] == "mem": regs[a[0]] = ("bits", atom(("bits", v[1], int(a[1], 0)), (0, 1))); flags = ("log", regs[a[0]][1] == 0, False); k += 1; continue
            raise No("and of " + repr(v))
        if op == "cmp":
            x, y = val(a[0]), val(a[1])
            if isinstance(x, int) and isinstance(y, int): flags = ("cmp", "lt" if x < y else "eq" if x == y else "gt")
            else: flags = ("cmp", atom(("cmp", repr(x), repr(y)), ("lt", "eq", "gt")))
            k += 1; continue
        if op == "test" and a[1] == "1" and MEM.fullmatch(a[0]):
            flags = ("log", boolean(val(a[0])) == 0, False); k += 1; continue
        if op == "shr" and a[0] in R16 and a[1] == "1":
            v = regs.get(a[0]); b = boolean(v) if v is not None else None
            if b is None: raise No("shr of an unknown")
            flags = ("log", None, b == 1); regs[a[0]] = 0; k += 1; continue
        raise No("instruction " + t)
    return ("end",)

def outcomes(ins, lo, hi, init=None):
    """-> {frozenset(assignment items): result}, every assignment the stretch asks for"""
    out = {}; todo = [{}]
    while todo:
        asg = todo.pop()
        try: out[frozenset(asg.items())] = run(ins, lo, hi, asg, init)
        except Need as n:
            for v in n.dom: d = dict(asg); d[n.key] = v; todo.append(d)
        if len(out) + len(todo) > 300000: raise No("too many cases")
    return out

def same(iw, lw, hw, ig, lg, hg, initw=None, initg=None):
    """-> (True, (else target of the original, else target of the compiled) or None) or (False, why)"""
    import itertools
    try:
        A = outcomes(iw, lw, hw, initw); B = outcomes(ig, lg, hg, initg)
    except No as e: return False, str(e)
    doms = {}
    for s in list(A) + list(B):
        for k, v in s: doms.setdefault(k, ("lt", "eq", "gt") if k[0] == "cmp" else (0, 1))
    ks = sorted(doms, key=repr); elses = set(); n = 1
    for k in ks: n *= len(doms[k])
    if not ks or len({r[0] for r in A.values()}) < 2: return False, "not a condition: it always goes the same way"
    if n > 300000: return False, "too many cases"
    try:
        for vals in itertools.product(*[doms[k] for k in ks]):
            asg = dict(zip(ks, vals)); ra = run(iw, lw, hw, asg, initw); rb = run(ig, lg, hg, asg, initg)
            if ra[0] != rb[0]: return False, "with %s the original %s, the compiled %s" % (", ".join("%s=%s" % (k[1:], v) for k, v in asg.items()), ra[0], rb[0])
            if ra[0] == "out": elses.add((ra[1], rb[1]))
    except (No, Need) as e: return False, "a case one of them does not have"
    if len({a for a, b in elses}) > 1 or len({b for a, b in elses}) > 1: return False, "more than one way out"
    return True, (next(iter(elses)) if elses else None)
