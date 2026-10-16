#!/usr/bin/env python3
"""symx.py - do two stretches of 8086 code without a jump in them compute the same?

Used by newcomp.py where no habit of the later compiler it knows explains a difference. Both
stretches are run on symbols from the same start (every register holds the same unknown value in
both); they are the same if they push the same values, store the same values at the same places in
the same order, and leave the same values in the frame's temporaries - which may be other
temporaries, each of the one for one of the other. Sums, products and ORs are compared without
regard to the order of their terms, a shift to the left as a product. The one jump allowed is the
compiler's way to a Boolean's value, mov R,0 / jcc over / inc R. What it does not know it refuses.
"""
import re
R16 = ("ax", "cx", "dx", "bx", "si", "di")
LO = {"al": "ax", "cl": "cx", "dl": "dx", "bl": "bx"}; HI = {"ah": "ax", "ch": "cx", "dh": "dx", "bh": "bx"}
COMM = {"add": "+", "or": "|", "and": "&", "xor": "^"}
UNIT = {"+": 0, "*": 1, "|": 0, "&": 0xFFFF, "^": 0}

class No(Exception): pass

def C(n): return ("c", n & 0xFFFF)
def isc(x): return x[0] in ("c", "f")
def fold(tag, a, b):
    if tag == "+": return a + b
    if tag == "*": return a * b
    if tag == "|": return a | b
    if tag == "&": return a & b
    return a ^ b
def nary(tag, xs):
    out = []; k = None
    for x in xs:
        for y in (x[1] if x[0] == tag else [x]):
            if isc(y):
                if k is None: k = y
                elif "f" in (k[0], y[0]): k = ("f",)
                else: k = C(fold(tag, k[1], y[1]))
            else: out.append(y)
    if k is not None and k != C(UNIT[tag]): out.append(k)
    if not out: return C(UNIT[tag])
    return out[0] if len(out) == 1 else (tag, sorted(out, key=canon))
def neg(x): return C(-x[1]) if x[0] == "c" else ("neg", x)
def canon(x):
    if isinstance(x, tuple): return "K" if isc(x) else "(" + " ".join(canon(y) for y in x) + ")"
    if isinstance(x, list): return "[" + " ".join(canon(y) for y in x) + "]"
    return str(x)
def unify(a, b):
    if a == b: return True
    if isinstance(a, tuple) and isinstance(b, tuple):
        if a and b and isc(a) and isc(b): return "f" in (a[0], b[0])
        return len(a) == len(b) and all(unify(x, y) for x, y in zip(a, b))
    if isinstance(a, list) and isinstance(b, list): return len(a) == len(b) and all(unify(x, y) for x, y in zip(a, b))
    return False

def run(ins, fixed):
    """ins: [(text, bytes)]; fixed: {index of an instruction: the addresses in it a linker fills in} -> the state after"""
    reg = {r: ("r", r) for r in R16}; pushes = []; writes = []; slots = {}; flags = [None]
    def num(t, fx):
        v = int(t, 0)
        return ("f",) if fx and v in fx else C(v)
    def addr(t, fx):
        terms = []; sign = 1
        for tok in t.split():
            if tok == "+": sign = 1
            elif tok == "-": sign = -1
            elif tok in R16: terms.append(reg[tok])
            elif tok == "bp": terms.append(("r", "bp"))
            elif re.fullmatch(r"0x[0-9a-f]+|\d+", tok): terms.append(num(tok, fx) if sign > 0 else neg(num(tok, fx)))
            else: raise No(t)
        return nary("+", terms)
    def slot(a):
        if a == ("r", "bp"): return 0
        if a[0] == "+" and len(a[1]) == 2 and ("r", "bp") in a[1]:
            k = [x for x in a[1] if x != ("r", "bp")][0]
            if k[0] == "c": return k[1]
        return None
    def parse(o):
        m = re.fullmatch(r"(word|byte) ptr \[(.+)\]", o)
        if m: return ("M", m.group(1), m.group(2))
        m = re.fullmatch(r"\[(.+)\]", o)
        if m: return ("A", m.group(1))
        if o in R16 or o in LO or o in HI: return ("R", o)
        if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", o): return ("I", o)
        raise No(o)
    def get(o, fx):
        p = parse(o)
        if p[0] == "I": return num(p[1], fx)
        if p[0] == "A": return addr(p[1], fx)
        if p[0] == "R":
            if p[1] in R16: return reg[p[1]]
            if p[1] in LO: return ("lo", reg[LO[p[1]]])
            raise No(o)
        a = addr(p[2], fx); s = slot(a)
        if s is not None:
            if s in slots:
                if slots[s][0] != p[1]: raise No("a temporary read in another size")
                return slots[s][1]
            return ("m", a, p[1], 0)
        return ("m", a, p[1], len(writes))
    def put(o, v, fx):
        p = parse(o)
        if p[0] == "R":
            if p[1] in R16: reg[p[1]] = v
            elif p[1] in LO: reg[LO[p[1]]] = ("lohi", v, reg[LO[p[1]]])
            else: raise No(o)
            return
        if p[0] != "M": raise No(o)
        a = addr(p[2], fx); s = slot(a)
        if s is not None: slots[s] = (p[1], v)
        else: writes.append((a, p[1], v))
    k = 0
    while k < len(ins):
        t, b = ins[k]; fx = fixed.get(k); m = re.fullmatch(r"(\w+)(?: (.+))?", t)
        if not m: raise No(t)
        op = m.group(1); a = [x.strip() for x in m.group(2).split(", ")] if m.group(2) else []
        if op == "mov" and len(a) == 2 and a[1] == "0" and a[0] in R16 and k + 2 < len(ins) and 0x70 <= ins[k + 1][1][0] <= 0x7F and ins[k + 1][1][1] == 1 \
           and ins[k + 2][0] == "inc " + a[0]:
            reg[a[0]] = ("ord", ins[k + 1][1][0], flags[0]); k += 3; continue
        if op == "mov": put(a[0], get(a[1], fx), fx)
        elif op == "lea": put(a[0], get(a[1], fx), fx)
        elif op == "xchg":
            x, y = get(a[0], fx), get(a[1], fx); put(a[0], y, fx); put(a[1], x, fx)
        elif op == "push": pushes.append(get(a[0], fx))
        elif op in ("xor", "sub") and a[0] == a[1] and a[0] in R16: reg[a[0]] = C(0); flags[0] = ("res", C(0))
        elif op == "xor" and a[0] == a[1] and a[0] in HI:
            r = HI[a[0]]; v = reg[r]
            reg[r] = ("zx", v[1]) if v[0] == "lohi" else v if v[0] == "zx" else ("zx", ("lo", v))
        elif op == "and" and a[1] == "0xff" and a[0] in R16:
            v = reg[a[0]]; reg[a[0]] = v if v[0] == "zx" else ("zx", ("lo", v)); flags[0] = ("res", reg[a[0]])
        elif op in ("or", "and") and a[0] == a[1]: flags[0] = ("res", get(a[0], fx))
        elif op in COMM: v = nary(COMM[op], [get(a[0], fx), get(a[1], fx)]); put(a[0], v, fx); flags[0] = ("res", v)
        elif op == "sub": v = nary("+", [get(a[0], fx), neg(get(a[1], fx))]); put(a[0], v, fx); flags[0] = ("res", v)
        elif op == "cmp": flags[0] = ("cmp", get(a[0], fx), get(a[1], fx))
        elif op in ("inc", "dec"): v = nary("+", [get(a[0], fx), C(1 if op == "inc" else -1)]); put(a[0], v, fx); flags[0] = ("res", v)
        elif op in ("shl", "shr") and a[1] in ("1", "cl"):
            n = 1
            if a[1] == "cl":
                if reg["cx"][0] != "c": raise No("a shift by an unknown count")
                n = reg["cx"][1]
            x = get(a[0], fx)
            if op == "shl": v = nary("*", [x, C(1 << n)])
            else: v = ("shr", x[1], x[2] + n) if x[0] == "shr" else ("shr", x, n)
            put(a[0], v, fx); flags[0] = ("res", v)
        elif op == "mul":
            v = nary("*", [reg["ax"], get(a[0], fx)]); reg["ax"] = v; reg["dx"] = ("high", v); flags[0] = ("res", v)
        elif op == "div":
            x = (reg["dx"], reg["ax"], get(a[0], fx)); reg["ax"] = ("div",) + x; reg["dx"] = ("mod",) + x; flags[0] = None
        else: raise No(t)
        k += 1
    return reg, pushes, writes, slots, flags[0]

def same(w, g, fixed, regs_too=False, flags_too=False):
    """w, g: [(text, bytes)] -> (True, '') or (False, why)"""
    try: rw, pw, ww, sw, fw = run(w, {}); rg, pg, wg, sg, fg = run(g, fixed)
    except No as e: return False, "not known: %s" % e
    if not unify(pw, pg): return False, "other values pushed: %s / %s" % (pw, pg)
    if not unify(ww, wg): return False, "other values stored"
    a = sorted((v for v in sw.values()), key=canon); b = sorted((v for v in sg.values()), key=canon)
    if not unify(a, b): return False, "other values left in the frame"
    if regs_too and not all(unify(rw[r], rg[r]) for r in R16): return False, "other values left in the registers"
    if flags_too and not unify(fw, fg): return False, "another condition"
    return True, ""
