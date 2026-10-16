"""draft.py -- functions written by an inverse model of the compiler, kept only when the compiler proves them.

    python -m codebreaker exact PROJECT draft [--all] [--limit N] [-v]

For every function carve finds that has no src/fADDR.c (--all: every function, the written ones
left as they are), the toolchain profile's inverse model ("inverse": a module of
codebreaker.invert with recover(code) and to_c(...)) reads the function's bytes back into C. The
text is made a source file of the project's kind:

  - the places it names get the engine's names: data g_XXXX (DS:XXXX), functions fXXXXX (their
    image offset), the library's routines by their names (the library stage's), each declared
  - what the model leaves of the machine (a register it could not fold into an expression) makes
    the draft unfit: it is not compiled

and compiled by the project's toolchain and held to the program (exact/compare.py). Identical:
written as src/fADDR.c. Otherwise it is kept in src/drafts/fADDR.c with what was wrong, for a
person or a later model - the ledger never counts a draft.

    draft.run(project) -> {address: verdict}
"""
import importlib
import os
import re
import struct

from codebreaker.exact import carve, compare, library, omf

REGS = r"\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\b"


def _names(prj):
    """{image offset: name} of the code the program has names for (SYMBOLS, the library's modules)"""
    out = {}
    mp = library.locate(prj)
    for p in mp.coded + mp.lone:
        for m in [p.m] + p.also:
            for n, (s, off) in m.pubs.items():
                if s == m.code[0][0]:
                    out.setdefault(p.code + off, n.lstrip("_"))
    for n, (t, a) in prj.symbols().items():
        if t == "C" and ":" not in n:
            out.setdefault(a, n.lstrip("_"))
    return out


WREG = r"(ax|bx|cx|dx|si|di)"


def compare_values(text):
    """if (C) { r = 1; } else { r = 0; } - a comparison's value (the compiler's 1, then 0 unless it holds): r = C"""
    rx = re.compile(r"(?m)^(\s*)if \((.*)\) \{\n\s*(ax|al|bx|cx|dx) = ([01]);\n\s*\} else \{\n\s*\3 = ([01]);\n\s*\}$")

    def one(m):
        ind, c, r, a, b = m.groups()
        if (a, b) == ("1", "0"):
            return "%s%s = %s;" % (ind, r, c if c.startswith("(") else "(%s)" % c)
        if (a, b) == ("0", "1"):
            return "%s%s = !(%s);" % (ind, r, c)
        return m.group(0)
    return rx.sub(one, text)


def _strip(c):
    """((X)) -> (X): the parentheses a test is printed in, once"""
    while c.startswith("((") and c.endswith("))"):
        depth = 0
        for k, ch in enumerate(c[1:-1]):
            depth += (ch == "(") - (ch == ")")
            if depth < 0:
                return c
        if depth:
            return c
        c = c[1:-1]
    return c


def _truth(c):
    """a test as the value it tests: (X == 0) -> !X, (X != 0) -> X"""
    c = _strip(c)
    m = re.fullmatch(r"\((.*) != 0\)", c)
    if m:
        return m.group(1)
    m = re.fullmatch(r"\((.*) == 0\)", c)
    if m:
        return "!%s" % m.group(1) if re.fullmatch(r"[\w.\->\[\]()]+", m.group(1)) else "!(%s)" % m.group(1)
    return c


def _not(c):
    c = _strip(c)
    m = re.fullmatch(r"\((.*?) (==|!=|<=|>=|<|>) (.*)\)", c)
    if m and m.group(2) and "(" not in m.group(3).replace("(", "", 0) and m.group(1).count("(") == m.group(1).count(")"):
        flip = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[m.group(2)]
        if m.group(3) != "0":
            return "(%s %s %s)" % (m.group(1), flip, m.group(3))
    m = re.fullmatch(r"\((.*) == 0\)", c)
    if m:
        return m.group(1)
    m = re.fullmatch(r"\((.*) != 0\)", c)
    if m:
        return "!%s" % m.group(1)
    return "!%s" % c


def logical_values(text):
    """a || or && taken as a value - the compiler's 1/0 behind the tests - read back:
        if (A0) { if (B) { L: r = 1; } else { r = 0; } } else { goto L; }   ->  r = !A0 || B
        if (A) { if (B) { r = 1; } else { r = 0; } } else { r = 0; }        ->  r = A && B"""
    one = r"\s*"
    rx_or = re.compile(r"(?m)^(\s*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(L_\w+):\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n"
                       r"\s*\} else \{\n\s*goto \4;\n\s*\}$")
    text = rx_or.sub(lambda m: "%s%s = %s || %s;" % (m.group(1), m.group(5), _not(m.group(2)), _truth(m.group(3))), text)
    rx_and = re.compile(r"(?m)^(\s*)if (\(.*\)) \{\n((?:\s*(?:ax|bx|cx|dx|si|di) = [^;\n]+;\n)*)\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n"
                        r"\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*\5 = 0;\n\s*\}$")

    def and_(m):
        b = m.group(4)
        for t in re.findall(r"(\w+) = ([^;\n]+);", m.group(3)):     # (the temporaries the second test reads)
            b = re.sub(r"\b%s\b" % t[0], lambda x: "(%s)" % t[1] if not re.fullmatch(r"\w+", t[1]) else t[1], b)
        return "%s%s = %s && %s;" % (m.group(1), m.group(5), _truth(m.group(2)), _truth(b))
    text = rx_and.sub(and_, text)
    return text


def conditional_values(text):
    """a register given one value or another by a test, then read once by the next statement - C's ?: read back:
        if (C) { r = A; } else { r = B; } X = ..r..;   ->   X = ..(C ? A : B)..;
    (the arms give it nothing else, and the statement after reads it no more)"""
    rx = re.compile(r"(?m)^(\s*)if (\(.*\)) \{\n\s*%s = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\s*\}\n(\s*)(.*)$" % (r"(ax|bx|cx|dx|si|di)"))

    def one(m):
        ind, c, r, a, b, ind2, nxt = m.groups()
        if ind2 != ind or len(re.findall(r"\b%s\b" % r, nxt)) != 1 or re.match(r"(?:if|while|for|switch|\}|L_\w+:)", nxt):
            return m.group(0)
        rest = m.string[m.end():]
        if re.search(r"\b%s\b" % r, re.split(r"\n\s*%s = (?![^;\n]*\b%s\b)" % (r, r), "\n" + rest)[0]) and r not in ("si", "di"):
            return m.group(0)                               # (read again later: not this way)
        if r in ("si", "di"):
            return m.group(0)                               # (a register variable: given a value it keeps)
        val = "%s ? %s : %s" % (_truth(c), a, b)
        return ind + re.sub(r"\b%s\b" % r, lambda x: "(%s)" % val, nxt, count=1)
    return rx.sub(one, text)


def branch_returns(text):
    """if (C) { r = E1; } else { ..; r = E2; } return r;  ->  if (C) return E1; ..; return E2; (each branch ends
    the function: the compiler jumps from the first to the end)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)if (\(.*\)) \{\s*$", l)
        if not m or k + 3 >= len(lines):
            continue
        ind = m.group(1)
        a = re.match(r"\s*(ax|al|bx|cx|dx|si|di) = (.*);\s*$", lines[k + 1])
        if not a or lines[k + 2] != ind + "} else {":
            continue
        reg = a.group(1)
        close = next((j for j in range(k + 3, len(lines)) if lines[j] == ind + "}"), None)
        if close is None or close + 1 >= len(lines) or not re.fullmatch(r"\s*return %s;\s*" % reg, lines[close + 1]):
            continue
        last = re.match(r"\s*%s = (.*);\s*$" % reg, lines[close - 1])
        if not last:
            continue
        body = [x[4:] if x.startswith(ind + "    ") else x for x in lines[k + 3:close - 1]]
        new = ["%sif %s" % (ind, m.group(2)), "%s    return %s;" % (ind, a.group(2))] + body + ["%sreturn %s;" % (ind, last.group(1))]
        return branch_returns("\n".join(lines[:k] + new + lines[close + 2:]))
    return text


def chain_returns(text):
    """if (A) { ..; r = E1; } else if (B) { ..; r = E2; } .. return r;  -> each arm returns its value (with no
    last else the function falls off its end there, as its source did)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)if \(.*\) \{\s*$", l)
        if not m:
            continue
        ind = m.group(1)
        arms_end = []                                       # (the closing line of each arm)
        j = k + 1
        while j < len(lines):
            x = lines[j]
            if x.startswith(ind + "} else if (") and x.endswith("{") or x == ind + "} else {":
                arms_end.append(j)
            elif x == ind + "}":
                arms_end.append(j)
                break
            elif x.startswith(ind) and not x.startswith(ind + " ") and x.strip():
                break
            j += 1
        if not arms_end or lines[arms_end[-1]] != ind + "}" or arms_end[-1] + 1 >= len(lines):
            continue
        ret = re.fullmatch(r"\s*return (ax|bx|cx|dx|si|di);\s*", lines[arms_end[-1] + 1])
        if not ret or len(arms_end) < 2:
            continue
        reg = ret.group(1)
        lasts = [e - 1 for e in arms_end]
        if not all(re.fullmatch(r"\s*%s = .+;\s*" % reg, lines[q]) for q in lasts):
            continue
        for q in lasts:
            lines[q] = re.sub(r"^(\s*)%s = (.+);\s*$" % reg, r"\1return \2;", lines[q])
        lines[arms_end[-1] + 1] = None
        return chain_returns("\n".join(x for x in lines if x is not None))
    return text


UCHAR = set()                                               # (bytes the code widens with zeros: unsigned char)
BYTEG = set()                                               # (globals the code reads or writes a byte of: char)


def zero_extends(text):
    """al = E; ah = 0;  - a byte widened with zeros: ax = E, E an unsigned char (a global so declared, a field of a
    struct so laid out)"""
    lines = text.split("\n")
    for k in range(len(lines) - 1):
        m = re.match(r"(\s*)al = (.+);\s*$", lines[k] or "")
        if m and re.fullmatch(r"\s*ah = 0;\s*", lines[k + 1] or ""):
            e = m.group(2)
            lines[k] = "%sax = %s;" % (m.group(1), e)
            lines[k + 1] = None
            g = re.fullmatch(r"(g_[0-9a-f]{1,4})", e)
            f = re.fullmatch(r"(\w+)->f(\d+)", e)
            if g:
                UCHAR.add("g_%04x" % int(g.group(1)[2:], 16))
            elif f:
                UCHAR.add((f.group(1), int(f.group(2))))
    return "\n".join(l for l in lines if l is not None)


def widen_bytes(text):
    """al = E; ... ax read before ax or al is given again, al never read: the byte widened for its use (cbw) -
    in C a char is an int where it is used: ax = E"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(al|bl|cl|dl) = (.*);\s*$", l)
        if not m:
            continue
        b8 = m.group(2)
        w = b8[0] + "x"
        for x in lines[k + 1:]:
            if re.search(r"\b%s\b" % b8, x):
                break
            if re.search(r"\b%s\b" % w, x):
                if not re.match(r"\s*%s = " % w, x):
                    lines[k] = "%s%s = %s;" % (m.group(1), w, m.group(3))
                break
    return "\n".join(lines)


def post_increments(text):
    """r = E(*p); p = p + n; X uses r once - the load and the step are *p++ in its place (n: the size it reads)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        l = lines[k]
        m = re.match(r"(\s*)(ax|bx|cx|dx|si|di|al|bl|cl|dl) = (.*);\s*$", l)    # (a byte register too: a char)
        if m and k + 2 < len(lines):
            ind, reg, e = m.groups()
            st = re.match(r"\s*(\w+) = \1 \+ (\d)\s*;\s*$", lines[k + 1])
            if st:
                p, n = st.group(1), int(st.group(2))
                deref = re.fullmatch(r"\*\((?:unsigned )?(char|int) \*\)%s|\*%s" % (p, p), e)
                size = 1 if deref and deref.group(1) == "char" else 2 if deref and deref.group(1) == "int" else None
                nxt = lines[k + 2]
                rest = "\n".join(lines[k + 3:])
                if deref and (size is None or size == n) and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
                        not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0]) and not re.search(r"\b%s\b" % p, nxt):
                    out.append(re.sub(r"\b%s\b" % reg, "%s++" % e, nxt, count=1))
                    k += 3
                    continue
        out.append(l)
        k += 1
    return "\n".join(out)


def copy_props(text):
    """R = X; (R a scratch register, X a variable): the reads of R that follow are X - until R or X is given
    again, a call (which leaves its value in ax), a label or a block's end; the copy then goes"""
    lines = text.split("\n")
    out = list(lines)
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(ax|bx|cx|dx) = ([A-Za-z_]\w*);\s*$", l)
        if not m:
            continue
        ind, r, x = m.groups()
        if x in ("ax", "bx", "cx", "dx", "si", "di"):
            continue
        depth = 0
        used = False
        for j in range(k + 1, len(lines)):
            y = out[j]
            depth += y.count("{") - y.count("}")
            if depth < 0 or re.match(r"\s*L_\w+:", y):
                break
            if re.fullmatch(r"\s*%s = %s;\s*" % (r, x), y):    # (the same copy again: no change)
                out[j] = None
                continue
            reads = len(re.findall(r"\b%s\b" % r, y))
            sets_r = re.match(r"\s*%s\s*=[^=]" % r, y)
            if reads and not sets_r:
                out[j] = re.sub(r"\b%s\b" % r, x, y)
                used = True
            if sets_r or re.search(r"\b%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*%s\b" % (x, x), y) or \
                    (r == "ax" and re.search(r"\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\s*\(", y)):
                break
        rest = "\n".join(y for y in out[k + 1:] if y is not None)
        if not re.search(r"\b%s\b" % r, re.split(r"\n\s*%s = " % r, "\n" + rest)[0]):
            out[k] = None                                   # (read no more before it is given again: gone)
    return "\n".join(x for x in out if x is not None)


def self_updates(text):
    """X = E; X = X op F; (F not reading X): one expression - X = E op F (the compiler kept the value in X's
    register between the two)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)([A-Za-z_]\w*) = ([^;]+);\s*$", lines[k])
        if m and k + 1 < len(lines):
            ind, x, e = m.groups()
            u = re.match(r"\s*%s = %s ([-+*&|^]|<<|>>) (.+);\s*$" % (x, x), lines[k + 1])
            if u and not re.search(r"\b%s\b" % x, e + u.group(2)) and not re.match(r"\s*(?:if|while|return)\b", lines[k]):
                f = u.group(2)
                out.append("%s%s = %s %s %s;" % (ind, x, e, u.group(1), f))
                k += 2
                continue
        out.append(lines[k])
        k += 1
    return "\n".join(out)


def post_steps(text):
    """R = X; X = X + 1 (or - 1); then R read once: the value before the step - X++ (X--) in its place"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(ax|bx|cx|dx|al|bl|cl|dl) = ([A-Za-z_]\w*);\s*$", lines[k])
        if m and k + 2 < len(lines):
            ind, reg, x = m.groups()
            st = re.match(r"\s*%s = %s ([+-]) 1;\s*$" % (x, x), lines[k + 1])
            nxt = lines[k + 2]
            rest = "\n".join(lines[k + 3:])
            if st and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and not re.search(r"\b%s\b" % x, nxt) and \
                    not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0]):
                out.append(re.sub(r"\b%s\b" % reg, x + ("++" if st.group(1) == "+" else "--"), nxt, count=1))
                k += 3
                continue
        out.append(lines[k])
        k += 1
    return "\n".join(out)


def test_first_loops(text):
    """for (;;) { if (C) break; ..} - the loop's test first: while (!C) { ..}"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)for \(;;\) \{\s*$", l) if l is not None else None
        if m and k + 1 < len(lines) and lines[k + 1] is not None:
            b = re.match(r"\s*if (\(.*\)) break;\s*$", lines[k + 1])
            if b:
                lines[k] = "%swhile (%s) {" % (m.group(1), _not(b.group(1)))
                lines[k + 1] = None
                continue
            s = re.match(r"\s*(\w+) = \1 ([+-]) 1;\s*$", lines[k + 1])     # (a step, then the test: --p in it)
            b = re.match(r"\s*if \(\((%s)\b(.*)\)\) break;\s*$" % s.group(1), lines[k + 2]) if s and k + 2 < len(lines) and lines[k + 2] else None
            if b:
                op = "++" if s.group(2) == "+" else "--"
                lines[k] = "%swhile (%s) {" % (m.group(1), _not("((%s%s%s))" % (op, b.group(1), b.group(2))))
                lines[k + 1] = lines[k + 2] = None
    return "\n".join(l for l in lines if l is not None)


def stored_calls(text):
    """r = f(..); LV = r; and r read no more: LV = f(..) (the call's value stored where it goes)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)%s = (.*\w\(.*\).*);\s*$" % WREG, lines[k])      # (a value with a call in it)
        if m and k + 1 < len(lines):
            ind, reg, call = m.groups()
            rd = "(?:%s|%sl)" % (reg, reg[0]) if reg in ("ax", "bx", "cx", "dx") else reg   # (or its low byte: a char)
            st = re.match(r"(\s*)(.+?) = %s;\s*$" % rd, lines[k + 1])
            rest = "\n" + "\n".join(lines[k + 2:])          # (from a line's start: the next value given it)
            later = not re.search(r"\b%s\b" % rd, re.split(r"\n\s*%s = (?![^;\n]*\b%s\b)" % (reg, rd), rest)[0])
            # (read before it is given a value that does not read it: still wanted)
            if st and not re.search(r"\b%s\b" % rd, st.group(2)) and later:
                out.append("%s%s = %s;" % (st.group(1), st.group(2), call))
                k += 2
                continue
            nxt = lines[k + 1]                              # (read once, anywhere in the next statement - which
            if later and len(re.findall(r"\b%s\b" % rd, nxt)) == 1 and not re.search(r"\w\(", nxt) and \
                    not re.match(r"\s*(?:\}|L_\w+:|else\b)", nxt) and not re.search(r"\b%s\s*=[^=]" % rd, nxt):
                out.append(re.sub(r"\b%s\b" % rd, lambda x: call, nxt, count=1))   # makes no call of its own:
                k += 2                                                               #  the order stays)
                continue
        out.append(lines[k])
        k += 1
    return "\n".join(out)


SIZES = {"char": 1, "int": 2, "unsigned": 2, "short": 2, "long": 4}


def frame_names(text, rule):
    """the locals renamed so that the compiler gives each the frame slot the program shows. rule (the toolchain
    profile's "frame_order"): {"mod": M} - the compiler gives the locals of a function (register ones too) their
    slots in the order of their names' buckets in its table of names, (the sum of the name's characters) mod M,
    ascending, the first next to BP. An auto the model names vN is at BP-N; a filler _pN after vN; a register
    local takes a free slot. A name is kept where its bucket falls in order, else a letter is put after it."""
    mod = rule.get("mod")
    if not mod or "{" not in text:
        return None
    head, _sep, body = split_body(text)
    lines = body.split("\n")
    decls = []
    for k, l in enumerate(lines[1:], 1):
        m = re.match(r"\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short)\b|struct\s+\w+)\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$", l)
        if not m:
            if l.strip():
                break
            continue
        reg, ty, star, name, cnt = m.groups()
        if ty.startswith("struct") and not star:
            return None                                     # (a struct's size the text does not say)
        size = 2 if star else SIZES.get(ty.split()[-1], 2) * int(cnt or 1)
        decls.append(dict(name=name, reg=bool(reg), size=max(size, 2) if not cnt else size + (size & 1)))
    autos = sorted((d for d in decls if not d["reg"] and re.fullmatch(r"v\d+", d["name"])), key=lambda d: int(d["name"][1:]))
    if not autos:
        return None
    fill = {d["name"]: d for d in decls if re.fullmatch(r"_p\d+", d["name"])}
    regs = [d for d in decls if d["reg"]]
    order = []
    drop = set()
    cum = 0
    for d in autos:                                         # (each auto at its place; what lies between: register
        n = int(d["name"][1:])                              #  locals, then a filler)
        gap = n - d["size"] - cum
        while gap >= 2 and regs:
            order.append(regs.pop(0))
            gap -= 2
        if "_p%d" % cum in fill:
            if gap <= 0:                                    # (the register locals take the room: no filler)
                drop.add("_p%d" % cum)
                fill.pop("_p%d" % cum)
            else:
                order.append(fill["_p%d" % cum])
        order.append(d)
        cum = n
    order += regs + [fill[f] for f in fill if fill[f] not in order]
    if len(order) > mod:
        return None
    taken = set(re.findall(r"\b[A-Za-z_]\w*\b", text))
    bucket = lambda s: sum(map(ord, s)) % mod
    new = {}
    prev = -1
    for k, d in enumerate(order):
        room = mod - (len(order) - k)                       # (the last bucket this one may take)
        nm = d["name"]
        if not (prev < bucket(nm) <= room):
            want = prev + 1
            nm = next((d["name"] + c for c in "abcdefghijklmnopqrstuvwxyz" if bucket(d["name"] + c) == want and d["name"] + c not in taken), None)
            if nm is None:
                return None
            new[d["name"]] = nm
        prev = bucket(nm)
    if not new and not drop:
        return None
    for f in drop:
        body = re.sub(r"(?m)^\s*char %s\[\d+\];\n" % f, "", body)
    for a, b in new.items():
        body = re.sub(r"\b%s\b" % a, b, body)
    return head + "{" + body


def pointer_flow(text):
    """the pointers a function's names are, from those it declares: a name compared with a pointer, given one's
    value, or giving its value to one is one too (a pointer compares without its sign: the code shows which)"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    decl = re.compile(r"(?m)^(\s*)(register\s+)?int\s+(\w+);$")
    ptr = set(re.findall(r"(?m)^\s*(?:register\s+)?char\s*\*\s*(\w+);", text))
    if not ptr:
        return text
    ints = set(m.group(3) for m in decl.finditer(text))
    changed = True
    while changed:
        changed = False
        for x in list(ints - ptr):
            for p in ptr:
                st = r"(?:\+\+|--)?"
                if re.search(r"\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\)|\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\)" % (st, x, st, p, st, p, st, x), body) or \
                        re.search(r"(?m)^\s*%s = %s\b|^\s*%s = %s\b" % (x, p, p, x), body):
                    ptr.add(x)
                    changed = True
                    break
    new = lambda m: "%s%schar *%s;" % (m.group(1), m.group(2) or "", m.group(3)) if m.group(3) in ptr else m.group(0)
    return decl.sub(new, head) + sep + decl.sub(new, body)


def char_param_variants(text, most=2):
    """the text with one of its int parameters a char (each, at most `most`): one compared with a byte, or given
    one, may be a char the caller widened (a variant: the compiler decides)"""
    head, sep, body = split_body(text)
    out = []
    for m in list(re.finditer(r"(?m)^(\s*)int ([\w, ]+);$", head))[:1]:
        names = [x.strip() for x in m.group(2).split(",")]
        for p in names[:most]:
            if not re.search(r"\b%s\b" % p, body):
                continue
            rest = [x for x in names if x != p]
            new = (m.group(1) + "int " + ", ".join(rest) + ";\n" if rest else "") + m.group(1) + "char %s;" % p
            out.append(head[:m.start()] + new + head[m.end():] + sep + body)
        used = [p for p in names if re.search(r"\b%s\b" % p, body)]
        if len(used) > 1:                                   # (and all of them chars)
            rest = [x for x in names if x not in used[:most]]
            new = (m.group(1) + "int " + ", ".join(rest) + ";\n" if rest else "") + m.group(1) + "char %s;" % ", ".join(used[:most])
            out.append(head[:m.start()] + new + head[m.end():] + sep + body)
    return out


def pointer_widths(text, most=2):
    """the text with its register pointers (`register char *p;`, at most `most` of them) made int * - each, then
    all: what the model read through them may be words (the text alone does not say)"""
    ps = re.findall(r"(?m)^\s*register char \*(\w+);", text)[:most]
    out = []
    for p in ps:
        out.append(re.sub(r"(?m)^(\s*)register char \*%s;" % p, r"\1register int *%s;" % p, text))
    if len(ps) > 1:
        out.append(re.sub(r"(?m)^(\s*)register char \*(%s);" % "|".join(ps), r"\1register int *\2;", text))
    return out


def pre_steps(text):
    """X--; (X++; X = X - 1;) then a test of X: the step in the test - if (--X ..) - a variant: the compiler
    tests the flags of the step there, not a compare"""
    lines = text.split("\n")
    changed = False
    k = 0
    while k + 1 < len(lines):
        m = re.match(r"\s*(\w+(?:->f\d+)?)(\+\+|--);\s*$", lines[k]) or re.match(r"\s*(\w+) = \1 ([+-]) 1;\s*$", lines[k])
        if m:
            x = m.group(1)
            op = m.group(2)
            op = "++" if op in ("++", "+") else "--"
            t = re.match(r"(\s*(?:if|while) \(\(?)(%s)\b(.*)$" % re.escape(x), lines[k + 1])
            if t:
                lines[k + 1] = t.group(1) + op + x + t.group(3)
                lines[k] = None
                changed = True
                k += 2
                continue
            nxt = lines[k + 1]                              # (or read once by the next statement: --x in place)
            if len(re.findall(r"\b%s\b" % re.escape(x), nxt)) == 1 and not re.match(r"\s*%s\s*=" % re.escape(x), nxt) \
                    and not re.match(r"\s*(?:\}|else\b|L_\w+:)", nxt):
                lines[k + 1] = re.sub(r"\b%s\b" % re.escape(x), op + x, nxt, count=1)
                lines[k] = None
                changed = True
                k += 2
                continue
        k += 1
    return "\n".join(l for l in lines if l is not None) if changed else None


PTRSIZE = {}                                                # (data a draft steps through by 2: a pointer to words)
ARRAYS = {}                                                 # (data a draft indexes: an array - name: element type)


def address_variants(text, dsize, most=2, lo=0x100):
    """a number a pointer is compared with, or given (`p < 14708`, `p = 14708`): the place of data there - an array
    at that offset, g_XXXX (a variant each: the compiler decides; a pointer compares without its sign)"""
    ptrs = set(re.findall(r"(?m)^\s*(?:register\s+)?(?:char|int|struct \w+)\s*\*\s*(\w+);", text))
    if not ptrs:
        return []
    nums = []
    for m in re.finditer(r"\b(%s)\s*(?:[<>]=?|==|!=|=)\s*(\d{3,5})\b" % "|".join(map(re.escape, ptrs)), text):
        n = int(m.group(2))
        if lo <= n < dsize and n not in nums:
            nums.append(n)
    out = []
    for n in nums[:most]:
        name = "g_%04x" % n
        out.append((name, re.sub(r"(?<![\w.])%d(?![\w.])" % n, name, text)))
    return out


def word_arrays(text, lo=0x40):
    """(E << 1)->fN, N a place in the data (not a field's small offset): an array of words there indexed by E -
    g_XXXX[E] (the model reads `mov bx,E / shl bx,1 / [bx+N]` as a field of a pointer)"""
    rx = re.compile(r"\(((?:[A-Za-z_]\w*|\d+)(?:\s*[-+]\s*\d+)?) << 1\)->f(\d+)\b")

    def one(m):
        n = int(m.group(2))
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "int"
        return "%s[%s]" % (name, m.group(1))
    t = rx.sub(one, text)
    # (I << 1)[E]: the model's word index into the words at E - ((int *)(E))[I]
    def word_index(m):                                      # (an offset in bytes: through a char *)
        i, e = m.group(1), m.group(2)
        o = re.fullmatch(r"([A-Za-z_]\w*) \+ (\d+)", e)
        return "((int *)((char *)%s + %s))[%s]" % (o.group(1), o.group(2), i) if o else "((int *)(%s))[%s]" % (e, i)
    t = re.sub(r"\(((?:[A-Za-z_]\w*|\d+)(?:\s*[-+]\s*\d+)?) << 1\)\[([^\[\]]+)\]", word_index, t)
    return t


def pre_increments(text):
    """X = X + n; then *X read once: *++X in its place - X a pointer to things of n bytes (a variant)"""
    lines = text.split("\n")
    changed = False
    for k in range(len(lines) - 1):
        m = re.match(r"\s*(\w+) = \1 \+ ([12]);\s*$", lines[k] or "")
        if not m:
            continue
        x, n = m.group(1), int(m.group(2))
        nxt = lines[k + 1]
        if nxt is None or len(re.findall(r"\b%s\b" % x, nxt)) != 1 or not re.search(r"\*%s\b(?!\s*(?:->|\[|\+\+|--))" % x, nxt):
            continue
        lines[k + 1] = re.sub(r"\*%s\b" % x, "*++%s" % x, nxt, count=1)
        lines[k] = None
        if n == 2:
            PTRSIZE[x] = 2
            lines = [re.sub(r"^(\s*register )char \*%s;" % x, r"\1int *%s;" % x, l) if l else l for l in lines]
        changed = True
    return "\n".join(l for l in lines if l is not None) if changed else None


FILEPTR = {}                                                # (a stream a library macro reads: name -> its type)


_LIBDATA = {}
_HEADERS = {}
_TABLES = {}


def header_text(prj, header):
    """a header of the kit, as text ('' when the kit has none)"""
    from codebreaker.exact import toolchain as tcm
    try:
        root = tcm.kit_dir(prj.toolchain().p["kit"])
    except Exception:
        return ""
    for d, _, fs in os.walk(root):
        for f in fs:
            if f.lower() == header.lower():
                return open(os.path.join(d, f), encoding="latin1").read()
    return ""


def table_macros(prj, text, specs):
    """the macros of a header that read a table of the library - ctype.h's isdigit(c) .. ( (_ctype_+1)[c] & _DIGIT ) -
    read back where the model has the table's place: (c->fN & M), N the table's place + its offset, M a macro's
    mask (the profile's "table_macros": [{"header": "ctype.h"}]: the macros and masks are the kit's header's, the
    table's place the library stage's)"""
    for sp in specs or []:
        key = (prj.dir, sp["header"])
        if key not in _TABLES:
            h = header_text(prj, sp["header"])
            masks = {n: int(v, 0) for n, v in re.findall(r"#define\s+(_[A-Z]+)\s+(0x[0-9a-fA-F]+|\d+)", h)}
            macs = []
            for name, arg, tab, off, expr in re.findall(r"#define\s+(\w+)\((\w)\)\s*\(\s*\((\w+)\+(\d+)\)\[\2\]\s*&\s*([^)]*\)?)\s*\)", h):
                bits = re.findall(r"_[A-Z]+", expr)
                if bits and all(b in masks for b in bits):
                    m = 0
                    for b in bits:
                        m |= masks[b]
                    macs.append((name, tab, int(off), m))
            if prj.dir not in _LIBDATA:
                data_macros(prj, "", [{"base": "-", "size": 1, "names": []}])
            _TABLES[key] = macs
        hit = False
        for name, tab, off, mask in _TABLES[key]:
            base = _LIBDATA.get(prj.dir, {}).get("_" + tab) or _LIBDATA.get(prj.dir, {}).get(tab)
            if base is None:
                continue
            n = base + off
            for form, fmt in ((r"\(\((\w+)->f%d & %d\) != 0\)" % (n, mask), "%s(%%s)" % name),
                              (r"\(\((\w+)->f%d & %d\) == 0\)" % (n, mask), "!%s(%%s)" % name),
                              (r"\((\w+)->f%d & %d\)" % (n, mask), "%s(%%s)" % name)):
                new = re.sub(form, lambda m: fmt % m.group(1), text)
                if new != text:
                    hit, text = True, new
        if hit:
            FILEPTR.setdefault("#include", sp["header"])
            FILEPTR["#include2"] = sp["header"]
    return text


def header_names(prj, header):
    """the functions and macros a header of the kit declares (stdio.h's fopen, freopen, getc ..): those come from
    the header, the draft does not declare them again"""
    key = (prj.toolchain().name, header.lower())
    if key not in _HEADERS:
        from codebreaker.exact import toolchain as tcm
        names = set()
        try:
            root = tcm.kit_dir(prj.toolchain().p["kit"])
            for d, _, fs in os.walk(root):
                for f in fs:
                    if f.lower() == header.lower():
                        t = open(os.path.join(d, f), encoding="latin1").read()
                        names |= set(re.findall(r"\b([A-Za-z_]\w*)\s*\(\s*\)\s*[;,]", t))
                        names |= set(re.findall(r"#define\s+([A-Za-z_]\w*)\s*\(", t))
                        break
                if names:
                    break
        except Exception:
            pass
        _HEADERS[key] = names
    return _HEADERS[key]


def data_macros(prj, text, specs):
    """a library's data a macro names - stdio's streams: stdin, stdout, stderr .. the elements of _iob, fileno(f)
    a byte of one - read back where the model has their places (g_XXXX, or a number passed: a stream's place)
    (the profile's "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"},
    "include": "stdio.h"}]; the base's place from the library stage)"""
    if not specs:
        return text
    if prj.dir not in _LIBDATA:
        try:
            from codebreaker.exact import library
            mp = library.locate(prj)
            places = {n: a for n, (t, a) in mp.allrefs.items() if t == "D"}
            for p in mp.coded + mp.donly:                   # (and the placed modules' own data names: a table
                if p.data is None:                          #  the macros read in line, which no code calls)
                    continue
                for n, s, off in p.m.o.pubs:
                    if s and s.upper() in ("_DATA", "DATA", "CONST"):
                        places.setdefault(n, p.data + off)
            _LIBDATA[prj.dir] = places
        except Exception:
            _LIBDATA[prj.dir] = {}
    places = _LIBDATA[prj.dir]
    for sp in specs:
        base = places.get(sp["base"])
        if base is None:
            continue
        n, size = len(sp["names"]), sp["size"]
        hit = [False]

        def field(m):
            a = int(m.group(1), 16)
            k, off = divmod(a - base, size)
            if 0 <= k < n and str(off) in sp.get("fields", {}):
                hit[0] = True
                return sp["fields"][str(off)] % sp["names"][k]
            return m.group(0)
        text = re.sub(r"\bg_([0-9a-f]{4})\b", field, text)

        def stream(m):                                      # (a number passed that is a stream's place)
            a = int(m.group(2))
            k, off = divmod(a - base, size)
            if 0 <= k < n and off == 0:
                hit[0] = True
                return m.group(1) + sp["names"][k]
            return m.group(0)
        text = re.sub(r"([(,]\s*)(\d{3,5})(?=\s*[,)])", stream, text)
        if hit[0] and sp.get("include"):
            FILEPTR["#include"] = sp["include"]
    return text


def unused_locals(text):
    """the locals declared in the function's body that nothing in it names any more, left out"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    lines = body.split("\n")
    drop = set()
    for k, l in enumerate(lines):
        m = re.match(r"\s*(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)\s*\**\s*(\w+)\s*(?:\[\d+\])?;\s*$", l)
        if m and not re.search(r"\b%s\b" % m.group(1), "\n".join(lines[:k] + lines[k + 1:])):
            drop.add(k)
    return head + sep + "\n".join(l for k, l in enumerate(lines) if k not in drop)


def library_macros(text, macros):
    """a library macro the compiler expanded in line - stdio's getc(f) and putc(c, f): the count of the stream's
    buffer stepped, then its pointer, else the routine that fills or empties the buffer (the profile's "macros":
    [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h"}]) - read back as the macro: from
    the line that steps the stream's count to the routine's call, the region is the macro's value"""
    if not macros:
        return text
    for mac in macros:
        rx = re.compile(r"\b%s\(([^()]*)\)" % re.escape(mac["call"]))
        while True:
            lines = text.split("\n")
            hit = next(((k, m) for k, l in enumerate(lines) for m in [rx.search(l)] if m), None)
            if not hit:
                break
            k1, m = hit
            args = [x.strip() for x in m.group(1).split(",")]
            f = args[-1]
            k0 = next((j for j in range(k1, -1, -1) if re.search(r"\b%s->f2(?:--| = %s->f2 - 1)" % (re.escape(f), re.escape(f)), lines[j])), None)
            if k0 is None:
                break
            val = mac["as"] % tuple(args)
            k2 = k1
            depth = sum(l.count("{") - l.count("}") for l in lines[k0:k2 + 1])
            while depth > 0 and k2 + 1 < len(lines):        # (to the region's own closing brace)
                k2 += 1
                depth += lines[k2].count("{") - lines[k2].count("}")
            ind = re.match(r"\s*", lines[k0]).group(0)
            r = re.match(r"\s*return\b", lines[k1])
            new = ["%sreturn %s;" % (ind, val)] if r else ["%sax = %s;" % (ind, val)]
            if depth < 0:
                new.append(ind + "}")
            lines[k0:k2 + 1] = new
            text = "\n".join(lines)
            FILEPTR[f] = mac.get("type", "FILE")
            FILEPTR["#include"] = mac.get("include")
    return text


def and_returns(text):
    """if (A) return E; return 0;  ->  return A && E;  (a variant: the compiler makes one test chain of the &&)"""
    rx = re.compile(r"(?m)^(\s*)if (\(.*\))(?: \{)?\n\s*return (.+);\n(?:\s*\}\n)?\s*return 0;$")

    def one(m):
        e = m.group(3)
        if re.search(r"\|\|", e) and not (e.startswith("(") and e.endswith(")")):
            e = "(%s)" % e
        return "%sreturn %s && %s;" % (m.group(1), _truth(m.group(2)), e)
    t = rx.sub(one, text)
    return t if t != text else None


def word_constants(text):
    """a word the model reads as 32768..65535: a decimal constant over 32767 is a long to a 16-bit compiler (the
    compare or the sum goes to 32 bits) - the int it was, negative (65535 is -1)"""
    return re.sub(r"(?<![\w.])(3276[89]|327[7-9]\d|32[89]\d\d|3[3-9]\d{3}|[4-5]\d{4}|6[0-4]\d{3}|65[0-4]\d\d|655[0-2]\d|6553[0-5])(?![\w.])",
                  lambda m: str(int(m.group(1)) - 65536), text)


def unsigned_globals(text, most=2):
    """the text with one of its int globals unsigned (each): a variable compared, the compiler's code says how"""
    out = []
    m = re.search(r"(?m)^extern int ([\w, ]+);$", text)
    if not m:
        return out
    names = [x.strip() for x in m.group(1).split(",")]
    for g in names[:most]:
        rest = [x for x in names if x != g]
        new = ("extern int %s;\n" % ", ".join(rest) if rest else "") + "extern unsigned %s;" % g
        out.append(text[:m.start()] + new + text[m.end():])
    return out


def assign_in_tests(text):
    """X = E; then a test of X: the assignment in the test - if ((X = E) == K) - a variant: the compiler
    compares the value it has, not X again"""
    lines = text.split("\n")
    changed = False
    for k in range(len(lines) - 1):
        m = re.match(r"(\s*)([A-Za-z_]\w*(?:\[[^\]]*\])?) = ([^;]+);\s*$", lines[k] or "")
        if not m or re.match(r"\s*(?:if|while|return)\b", lines[k]):
            continue
        x, e = m.group(2), m.group(3)
        t = re.match(r"(\s*(?:if|while) \(\(?)(%s)\b(.*)$" % re.escape(x), lines[k + 1] or "")
        if t and not re.search(r"\b%s\b" % re.escape(x), e):
            lines[k + 1] = "%s(%s = %s)%s" % (t.group(1), x, e, t.group(3))
            lines[k] = None
            changed = True
    return "\n".join(l for l in lines if l is not None) if changed else None


def char_params(text):
    """a parameter the model only reads into a byte register (al = b) is a char: declared so (K&R widens it
    in the call; the function reads its low byte)"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    params = re.findall(r"\b\w+\(([\w, ]*)\)", head)
    params = [p.strip() for p in (params[-1].split(",") if params else []) if p.strip()]
    chars = [p for p in params if re.search(r"\b(?:al|bl|cl|dl) = %s;" % p, body) and
             not re.search(r"\b(?:ax|bx|cx|dx|si|di) = %s;" % p, body) and not re.search(r"\b%s\s*(?:->|\[)" % p, body)]
    if not chars:
        return text
    lines = head.split("\n")
    for j, l in enumerate(lines):
        d = re.match(r"(\s*)int (.*);$", l)
        if d:
            names = [x.strip() for x in d.group(2).split(",")]
            keep = [x for x in names if x not in chars]
            lines[j] = (d.group(1) + "int " + ", ".join(keep) + ";\n" if keep else "") + \
                d.group(1) + "char " + ", ".join(x for x in names if x in chars) + ";" if any(x in chars for x in names) else l
    return "\n".join(lines) + sep + body


def _block_end(lines, k):
    """the index of the line closing the block line k opens (by indentation: the first line back at its indent)"""
    ind = len(lines[k]) - len(lines[k].lstrip())
    for j in range(k + 1, len(lines)):
        if lines[j].strip() and len(lines[j]) - len(lines[j].lstrip()) <= ind:
            return j
    return None


def for_loops(text):
    """a while loop whose body ends by stepping a variable - `X = X + K;`, or `else { X = X + K; continue; }`
    after a test whose other arm leaves - a for loop: for (INIT; C; X = X + K), its first statement the init when
    the line before gives a value to what C tests (a variant: the compiler lays a for out its own way - the step
    where a continue goes)"""
    lines = text.split("\n")
    changed = False
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)while \((.*)\) \{$", lines[k])
        e = _block_end(lines, k) if m else None
        if not m or e is None or lines[e].strip() != "}":
            k += 1
            continue
        ind = m.group(1)
        i4 = ind + "    "
        body = lines[k + 1:e]
        own = [l for l in body if l.startswith(i4) and not l.startswith(i4 + " ")]
        step = None
        st = r"([A-Za-z_]\w*) = \1 [+-] \d+;|[A-Za-z_]\w*(?:\+\+|--);"
        if body and re.fullmatch(i4 + "(?:%s)" % st, body[-1]) and i4 + "continue;" not in own:
            step = body[-1].strip()
            body = body[:-1]
        elif len(body) >= 4 and body[-4:-3] == [i4 + "} else {"] and re.fullmatch(i4 + "    (?:%s)" % st, body[-3]) and \
                body[-2] == i4 + "    continue;" and body[-1] == i4 + "}" and i4 + "continue;" not in own and \
                re.match(r"\s*(?:break|return)\b", body[-5] if len(body) >= 5 else ""):
            step = body[-3].strip()
            body = body[:-4] + [i4 + "}"]
        if not step:
            k += 1
            continue
        init = ""
        p = k - 1
        im = re.fullmatch(re.escape(ind) + r"([A-Za-z_]\w*) = ([^;]+);", lines[p]) if p >= 0 else None
        if im and re.search(r"\b%s\b" % im.group(1), m.group(2)):
            init = "%s = %s" % (im.group(1), im.group(2))
        head = "%sfor (%s; %s; %s) {" % (ind, init, _truth("(" + m.group(2) + ")"), step.rstrip(";"))
        new = [head] + body + [lines[e]]
        if init:
            lines[p:e + 1] = new
            k = p + 1
        else:
            lines[k:e + 1] = new
            k += 1
        changed = True
    return "\n".join(lines) if changed else None


def case_falls(text):
    """a case that goes on into the next (`goto L;` to the label the next case starts with): it falls into it -
    an arm's last `goto L;` is dropped, and `if (C) { X } else { goto L; } break;` is `if (C) { X break; }`"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(L_\w+):$", lines[k])
        c = k
        while c > 0 and re.match(r"\s*(?:case [^:]+|default):$", lines[c - 1]):
            c -= 1
        if not m or c == k:
            k += 1
            continue
        lab, ind = m.group(2), re.match(r"\s*", lines[c]).group(0)
        i4 = ind + "    "
        if lines[c - 1] == i4 + "goto %s;" % lab:
            del lines[c - 1]
            k -= 1
        elif c >= 5 and lines[c - 4:c] == [i4 + "} else {", i4 + "    goto %s;" % lab, i4 + "}", i4 + "break;"] and \
                not re.match(r"\s*(?:return|break|continue|goto)\b", lines[c - 5]):
            lines[c - 4:c] = [i4 + "    break;", i4 + "}"]
            k -= 2
        else:
            k += 1
            continue
        if not re.search(r"\bgoto %s;" % lab, "\n".join(lines)):
            del lines[k]
        k += 1
    return "\n".join(lines)


def inner_registers(text):
    """a register variable first given a value after other statements: declared in a block from there (to
    before the function's last return) - a variant: before the block the compiler has the register free for
    its own use"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    decls = [(j, m) for j, l in enumerate(lines) for m in [re.match(r"    register ([\w ]+?\**) ?(\w+);$", l)] if m]
    for j, m in decls:
        x = m.group(2)
        first = next((k for k, l in enumerate(lines) if k > j and re.search(r"\b%s\b" % x, l)), None)
        if first is None or not re.match(r"    %s = " % x, lines[first]):
            continue
        stmts = [k for k in range(j + 1, first) if lines[k].strip() and not re.match(r"\s*(?:register |int |char |unsigned |long |struct )", lines[k])]
        if not stmts:
            continue
        end = max((k for k, l in enumerate(lines) if re.match(r"    return\b", l)), default=None)
        if end is None or end <= first:
            continue
        new = lines[:j] + lines[j + 1:first] + ["    {", "        register %s%s;" % (m.group(1) + " " if not m.group(1).endswith("*") else m.group(1), x)] + \
            ["    " + l if l.strip() else l for l in lines[first:end]] + ["    }"] + lines[end:]
        return head + sep + "\n".join(new)
    return None


def char_stores(text):
    """a byte register stored into a local (vN = al) is a char local; a test of the register at once is a test of
    the store - if ((vN = al) != K): the compiler compares what it has - and the register read later, before it is
    given another value, is the local's value. A constant 128..255 it is compared with: (char)0xNN, a byte's"""
    lines = text.split("\n")
    chars = set()
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(v\d+) = ([abcd]l);\s*$", lines[k])
        if not m:
            k += 1
            continue
        v, r = m.group(2), m.group(3)
        w = r[0] + "x"
        nx = next((j for j in range(k + 1, len(lines)) if lines[j].strip()), None)
        if nx is None:
            break
        if re.match(r"\s*(?:if|while) \(.*\b%s\b" % r, lines[nx]):
            lines[nx] = re.sub(r"\b%s\b" % r, "(%s = %s)" % (v, r), lines[nx], count=1)
            del lines[k]
            nx -= 1
        else:
            k += 1
            nx = k
        chars.add(v)
        for j in range(nx + 1, len(lines)):                 # (read again before it changes: the local)
            if re.match(r"\s*L_\w+:", lines[j]) or re.search(r"\b(?:%s|%s) = |\w\(" % (r, w), lines[j]):
                break
            lines[j] = re.sub(r"\b%s\b" % r, v, lines[j])
    if not chars:
        return text
    text = "\n".join(lines)
    for v in chars:
        text = re.sub(r"(?m)^(\s*)int %s;" % v, r"\1char %s;" % v, text)
        text = re.sub(r"(\(%s = [abcd]l\)|\b%s) ([!=]=|[<>]=?) (1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b" % (v, v),
                      lambda m: "%s %s (char)0x%02x" % (m.group(1), m.group(2), int(m.group(3))), text)
    return text


def store_increments(text):
    """*p = X; p = p + 1; (X not reading p) - the store and the step as one: *p++ = X (a variant: the compiler
    makes other code of the two)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)\*(\w+) = (.*);\s*$", lines[k])
        if m and k + 1 < len(lines) and re.match(r"\s*%s = %s \+ 1;\s*$" % (m.group(2), m.group(2)), lines[k + 1]) \
                and not re.search(r"\b%s\b" % m.group(2), m.group(3)):
            out.append("%s*%s++ = %s;" % m.groups())
            k += 2
            continue
        out.append(lines[k])
        k += 1
    t = "\n".join(out)
    return t if t != text else None


def fold_registers(text):
    """what a model leaves of the machine's registers, folded where the text alone shows how: a register given a
    value no later statement reads (a call's: the call; a load: nothing), a register returned right after it was
    given, a call the model wrote twice (once alone, once into a register)"""
    lines = text.split("\n")
    out = []
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(ax|al|bx|cx|dx|si|di) = (.*);\s*$", l)
        if m:
            ind, reg, expr = m.groups()
            rest = "\n".join(lines[k + 1:])
            nxt = next((x for x in lines[k + 1:] if x.strip()), "")
            later = "\n".join(lines[lines.index(nxt, k + 1) + 1:]) if nxt in lines[k + 1:] else ""
            pure = re.fullmatch(r"[A-Za-z_]\w*|-?\d+|[A-Za-z_]\w*->f\d+|\*[A-Za-z_]\w*|[A-Za-z_]\w*\[[A-Za-z_0-9]+\]", expr) or (
                not re.search(r"\w\s*\(|\+\+|--|[^=!<>]=[^=]", expr) and expr.startswith("(") and expr.endswith(")"))
            # (a value with no effect: a name, a field, an element, or no call and no store in it)
            names = re.findall(r"[A-Za-z_]\w*", expr)
            if pure and reg not in ("si", "di") and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
               not any(re.search(r"\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b" % (n, n), nxt) for n in names) and \
               not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, later)[0]) and not re.search(
                   r"\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b" % (re.escape(expr), re.escape(expr)), nxt):
                j = lines.index(nxt, k + 1)                 # (a plain value read once, at once: in its place)
                lines[j] = re.sub(r"\b%s\b" % reg, lambda m: expr, nxt)
                continue
            if re.fullmatch(r"\s*return %s;\s*" % reg, nxt):
                out.append("%sreturn %s;" % (ind, expr))
                lines[lines.index(nxt, k + 1)] = "\x00"
                continue
            used = re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0]) or \
                reg in ("si", "di")                         # (a register variable: a loop may read it on its next pass)
            if not used:
                call = re.search(r"\b(?!if\b|while\b|sizeof\b)[A-Za-z_]\w*\s*\(", expr)    # (a call: a name, then
                if call and out and out[-1].strip() == expr + ";":                       #  its arguments)
                    continue                                # (the call written twice)
                if call:
                    out.append("%s%s;" % (ind, expr))
                continue                                    # (a load nothing reads)
        if l != "\x00":
            out.append(l)
    return "\n".join(out)


def shapes(text):
    """the shapes a compiler gives a statement, read back: a loop whose test it moved to the bottom (`goto L;
    do {..} while (c);` with no L - the jump goes to the test) is `while (c) {..}`; a step then a read one back
    (`p = p + 1; .. p[-1]`) is `*p++`"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        l = lines[k]
        m = re.match(r"(\s*)goto (L_\w+);\s*$", l)
        if m and k + 1 < len(lines) and lines[k + 1].strip() == "do {" and not re.search(r"^\s*%s:" % m.group(2), text, re.M):
            depth, close = 0, None
            for j in range(k + 1, len(lines)):              # (the do's own closing line)
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0:
                    close = j
                    break
            mm = re.match(r"(\s*)\} while \((.*)\);\s*$", lines[close]) if close is not None else None
            if mm:
                out.append("%swhile (%s) {" % (m.group(1), mm.group(2)))
                out += lines[k + 2:close]
                out.append("%s}" % m.group(1))
                k = close + 1
                continue
        out.append(l)
        k += 1
    text = "\n".join(out)
    # after a return, what stands in the same block before its end (no label) is never reached
    lines = text.split("\n")
    keep = []
    dead = None
    for l in lines:
        ind = len(l) - len(l.lstrip(" "))
        if dead is not None:
            if ind < dead or re.match(r"\s*L_\w+:", l):    # (out of the return's block: a closing brace, or
                                                            #  the next statement after a braceless if's)
                dead = None
            else:
                continue
        keep.append(l)
        if re.match(r"\s*return\b.*;\s*$", l):
            dead = ind
    text = "\n".join(keep)
    # a step, then the value one back read: the post-increment (in a do's test, or the next statement)
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"\s*(\w+) = \1 \+ 1;\s*$", l)
        if not m:
            continue
        v = m.group(1)
        for j in range(k + 1, min(k + 4, len(lines))):
            if re.search(r"\b%s\[-1\]" % v, lines[j]):
                lines[j] = re.sub(r"\b%s\[-1\]" % v, "*%s++" % v, lines[j], count=1)
                lines[k] = None
                break
            if lines[j].strip() not in ("}", "") and not lines[j].strip().startswith("} while"):
                break
    return "\n".join(l for l in lines if l is not None)


def structs(text, widths):
    """the pointers the text goes through to fields (`p->fN`, N its offset) declared pointers to a struct of
    those fields: a char where the code reads a byte (widths: {(pointer, offset)} of the byte ones), else an
    int, and a pad between"""
    text = re.sub(r"&(\w+)->f(\d+)\b", r"(\1 + \2)", text)  # (lea ax,[p+N] - the model's &p->fN - is p + N)
    text = re.sub(r"&(\w+)\[(-?)(\d+)\]", lambda m: "(%s %s %s)" % (m.group(1), "-" if m.group(2) else "+", m.group(3)), text)
    uses = {}
    for p, off in re.findall(r"\b(\w+)->f(\d+)\b", text):
        uses.setdefault(p, set()).add(int(off))
    if not uses:
        return text
    for p in uses:                                          # (*p of a struct pointer: its first field)
        whole = re.compile(r"(?<![\w>)\]])\*%s\b(?!\s*(?:->|\[|\+\+|--|\.))" % p)
        decl = re.compile(r"\s*(?:extern\s+|register\s+|static\s+)*(?:unsigned\s+)?(?:char|int|long|short|struct\s+\w+)\b")
        lines = text.split("\n")
        if any(whole.search(l) for l in lines if not decl.match(l)):
            text = "\n".join(l if decl.match(l) else whole.sub("%s->f0" % p, l) for l in lines)
            uses[p].add(0)
    defs = []
    for p, offs in sorted(uses.items()):
        fields = []
        pos = 0
        selfref = re.search(r"\b%s = %s->f0;" % (p, p), text)  # (p = p->f0: a list - the field points on)
        for off in sorted(offs):
            if off < pos:
                continue                                    # (overlaps the field before: not this way)
            if off > pos:
                fields.append("char _%d[%d];" % (pos, off - pos))
            uch = any(isinstance(u, tuple) and u[1] == off for u in UCHAR)   # (widened with zeros: unsigned)
            byte = (p, off) in widths or ("*", off) in widths or uch
            deref = re.search(r"(?<![\w>)\]])\*%s->f%d\b(?!\s*(?:->|\[|\.))" % (p, off), text)   # (*p->fN: a pointer)
            fields.append("%s f%d;" % ("struct s_%s *" % p if off == 0 and selfref else "unsigned char" if uch else
                                       "char" if byte else "int *" if deref else "int", off))
            pos = off + (1 if byte else 2)
        tag = "s_" + p
        steps = {int(n) for n in re.findall(r"\b%s = %s [+-] (\d+);" % (p, p), text)}
        if len(steps) == 1:                                 # (stepped through an array of them: the step is the
            n = steps.pop()                                 #  struct's size, and in C a step of one)
            if n >= pos and n < 0x100:
                if n > pos:
                    fields.append("char _%d[%d];" % (pos, n - pos))
                text = re.sub(r"\b%s = %s ([+-]) %d;" % (p, p, n), r"%s = %s \1 1;" % (p, p), text)
        defs.append("struct %s { %s };" % (tag, " ".join(fields)))
        # the declarations of p: a parameter, a register variable, a local, or data (declared by localize)
        text = re.sub(r"(?m)^(\s*)(register )?(?:int|char \*) ?%s;" % p, r"\1\2struct %s *%s;" % (tag, p), text)
        text = re.sub(r"(?m)^(\s*)int ((?:\w+, )*)%s((?:, \w+)*);" % p,
                      lambda m: "%sint %s;\n%sstruct %s *%s;" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", "), m.group(1), tag, p)
                      if (m.group(2) + m.group(3)).strip(", ") else "%sstruct %s *%s;" % (m.group(1), tag, p), text)
        STRUCTPTR[p] = tag
    return "\n".join(defs) + "\n" + text


STRUCTPTR = {}                                              # (data the draft declares a struct pointer: localize)


def candidates(prj, text, at, names, regs, regparam, widths=()):
    """[(source, registers left)] - the model's text as the project's source, then the variants the compiler
    is to decide between: as read, without its last return (a function of no value), with the locals whose
    address it takes as the buffers they are - and both"""
    base = [text] + [v for v in [void_variant(text)] if v]
    out = []
    for t in base + [v for v in (buffers(t) for t in base) if v]:
        STRUCTPTR.clear()
        PTRSIZE.clear()
        ARRAYS.clear()
        UCHAR.clear()
        BYTEG.clear()
        BYTEG.update(n for k, n in widths if k == "g")
        FILEPTR.clear()
        t = zero_extends(copy_props(table_macros(prj, case_falls(char_stores(char_params(t))), prj.toolchain().p.get("table_macros"))))
        t = re.sub(r"&(\w+)\[(-?)(\d+)\]", lambda m: "(%s %s %s)" % (m.group(1), "-" if m.group(2) else "+", m.group(3)), t)
        t = re.sub(r"&(\w+)->f(\d+)\b", r"(\1 + \2)", t)  # (lea: &p[N], &p->fN - the place, p + N)
        t = fold_registers(stored_calls(post_increments(widen_bytes(compare_values(chain_returns(branch_returns(conditional_values(logical_values(t)))))))))
        t = word_arrays(test_first_loops(post_steps(t)))
        t = self_updates(t)
        t = structs(pointer_flow(local_pointers(register_variables(shapes(t), regs, regparam))), set(widths))
        t = fill_frame(t, FRAME[0], bool(prj.toolchain().p.get("frame_order")))
        variants = [t] + [v for v in [store_increments(t), pre_steps(t), pre_increments(t), assign_in_tests(t), and_returns(t), inner_registers(t), for_loops(t)] if v]
        both = pre_steps(t)
        both = pre_increments(both) if both else None
        if both:
            variants.append(both)
        for v in list(variants):                            # (a pointer register variable through which words
            for w in pointer_widths(v):                     #  are read: int *, the compiler decides)
                variants.append(w)
        if re.search(r"& 0xff\)|\b[a-z] [<>=!]=? (?:3[2-9]|[4-9]\d|1[01]\d|12[0-6])\b", t):   # (a byte compared, or
            variants += char_param_variants(t)                                              #  a char: a char param)
        rule = prj.toolchain().p.get("frame_order")
        for v in list(variants):                            # (the names the frame's order asks for)
            fn = frame_names(v, rule) if rule else None
            variants.insert(variants.index(v) + 1, fn) if fn else None
        dsize = len(prj.image) - prj.dgroup
        for v in list(variants)[:2]:                        # (a number that is a place in the data)
            for name, av in address_variants(v, dsize):
                ARRAYS.setdefault(name, "char")
                variants.append(av)
        loc = [localize(prj, word_constants(v), at, names) for v in variants]
        for src, left in loc[:3]:                           # (a global compared: unsigned, a variant)
            if not left and re.search(r"[<>=]=?\s*-?\d", src):
                loc += [(u, left) for u in unsigned_globals(src)]
        out += loc
    return out


def byte_fields(inv, code):
    """{(pointer, offset)} of the fields the code reads or writes a byte of: the model's checker mode marks
    byte-wide memory as BYTE(x)"""
    if not hasattr(inv, "MARK"):
        return set()
    old = inv.MARK
    try:
        inv.MARK = True
        fs, params, stmts, pt, loc = inv.recover(code)
        t = inv.to_c("f", fs, params, stmts, pt, loc)
    except Exception:
        return set()
    finally:
        inv.MARK = old
    out = {(p, int(o)) for p, o in re.findall(r"BYTE\(\s*\(?(\w+)->f(\d+)\)?\s*\)", t)}
    out |= {(p, 0) for p in re.findall(r"BYTE\(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)?\)", t)}   # (*p a byte: a char *)
    from codebreaker.exact import x86                       # (and from the code itself: a byte read or written
    for i in x86.disasm(code, 0, len(code), False):         #  through a register at an offset - whatever the
        m = re.search(r"byte ptr \[(?:bx|si|di) \+ (0x[0-9a-f]+|\d+)\]", i.op_str)    # draft names the pointer)
        if m and int(m.group(1), 0) < 0x100:
            out.add(("*", int(m.group(1), 0)))
        g = re.search(r"byte ptr \[(0x[0-9a-f]+|\d+)\]", i.op_str)     # (a global read or written a byte at a time)
        if g:
            out.add(("g", "g_%04x" % int(g.group(1), 0)))
    return out


def buffers(text):
    """the locals whose address the text takes (`&vN`) as char arrays, each as long as the frame has room before
    the next local (vN is at bp-N); a store into one goes through an int"""
    locs = sorted({int(n) for n in re.findall(r"(?m)^\s*int v(\d+);", text)})
    taken = [n for n in locs if re.search(r"&v%d\b" % n, text)]
    if not taken:
        return None
    for n in taken:
        lower = [m for m in locs if m < n]
        size = n - (max(lower) if lower else 0)
        text = re.sub(r"(?m)^(\s*)int v%d;" % n, r"\1char v%d[%d];" % (n, size), text)
        text = re.sub(r"&v%d\b" % n, "v%d" % n, text)
        text = re.sub(r"(?m)^(\s*)v%d = " % n, r"\1*(int *)v%d = " % n, text)
    return text


def local_pointers(text):
    """a register given the place of a local (`r = bp - N`, the model's lea of [bp-N]): a char pointer into it,
    `r = (char *)&vN;`, its fields the bytes it reaches (`r->fK` is r[K])"""
    for reg, n in re.findall(r"(?m)^\s*(\w+) = bp - (\d+);", text):
        if not re.search(r"(?m)^\s*int v%s;" % n, text):
            continue
        text = re.sub(r"(?m)^(\s*)%s = bp - %s;" % (reg, n), r"\1%s = (char *)&v%s;" % (reg, n), text)
        text = re.sub(r"\b%s->f(\d+)\b" % reg, r"%s[\1]" % reg, text)
        text = re.sub(r"(?m)^(\s*)(register )?int %s;" % reg, r"\1\2char *%s;" % reg, text)
    return text


def void_variant(text):
    """the draft without its last `return X;` (a function of no value whose last call's result the model kept)"""
    m = list(re.finditer(r"(?m)^(\s*)return (.+);\s*$", text))
    if not m:
        return None
    x = m[-1]
    return text[:x.start()] + ("%s%s;" % (x.group(1), x.group(2)) if "(" in x.group(2) else "") + text[x.end():]


def register_variables(text, regs=("si", "di"), regparam=None):
    """the registers a compiler keeps its register variables in (MS C 3.00: SI, then DI), made those variables:
    declared in that order, pointers where the text goes through them; a register a parameter is loaded into
    at the start (regparam {register: parameter index}) is that parameter, declared register"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    rdecl = []                                              # (declared in the order of their registers: the
    for r, ix in sorted((regparam or {}).items(), key=lambda kv: list(regs).index(kv[0]) if kv[0] in regs else 9):
        p = chr(ord("a") + ix)                              #  compiler gives the first declared the first)
        body = re.sub(r"\b%s\b" % r, p, body)
        ptr = re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*\[" % (p, p), body)
        lines = head.split("\n")
        for j, l in enumerate(lines):                       # (out of the plain declaration of the parameters)
            m = re.match(r"(\s*)int (.*);$", l)
            if m and re.search(r"\b%s\b" % p, m.group(2)):
                rest = [x.strip() for x in m.group(2).split(",") if x.strip() != p]
                lines[j] = m.group(1) + "int " + ", ".join(rest) + ";" if rest else None
        head = "\n".join(l for l in lines if l is not None)
        rdecl.append("register %s%s;" % ("char *" if ptr else "int ", p))
    if rdecl:
        head = head.rstrip("\n") + "\n" + "\n".join(rdecl) + "\n"
        sig = re.search(r"(?m)^(\w+)\(([\w, ]*)\)\s*$", head)    # (the parameters in the signature, all of them)
        if sig:
            have = [x.strip() for x in sig.group(2).split(",") if x.strip()]
            want = sorted(set(have) | {chr(ord("a") + ix) for ix in (regparam or {}).values()})
            if want != have:
                head = head[:sig.start()] + "%s(%s)" % (sig.group(1), ", ".join(want)) + head[sig.end():]
    used = [r for r in regs if re.search(r"\b%s\b" % r, body)]
    if not used:
        return head + sep + body
    decls = []
    pnames = set(re.findall(r"(?m)^\s*(?:register\s+)?char\s*\*\s*(\w+);", head))     # (pointer parameters)
    for r in used:
        ptr = re.search(r"\*\s*%s\b|\b%s\s*\[" % (r, r), body) or \
            any(re.search(r"(?m)^\s*%s = %s;" % (r, p), body) for p in pnames)       # (given a pointer's value)
        decls.append("    register %s%s;" % ("char *" if ptr else "int ", "r_" + r))
        body = re.sub(r"\b%s\b" % r, "r_" + r, body)
    return head + "{\n" + "\n".join(decls) + body


def string_variants(prj, text):
    """the drafts where a number passed or given is the place of a string in the data: that string (the model
    reads a literal's address as the number it is); the compiler decides"""
    ds = prj.image[prj.dgroup:]
    nums = sorted(set(int(x) for x in re.findall(r"(?<![\w.])(\d{2,5})(?![\w.])", text.split("{", 1)[-1])))
    cands = []
    for v in nums:
        if 0 < v < len(ds) - 1 and 32 <= ds[v] < 127 and (v == 0 or ds[v - 1] == 0):
            e = ds.find(b"\0", v)
            s = ds[v:e]
            if 0 < len(s) < 200 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
                from codebreaker.exact.data import cstr
                cands.append((v, cstr(s)))
    out = []
    for v, lit in cands:                                    # (each alone, then all together)
        out.append(re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, text))
    if len(cands) > 1:
        t = text
        for v, lit in cands:
            t = re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, t)
        out.append(t)
    return out


def localize(prj, text, at, names):
    """the model's text with the engine's names and the declarations it needs; None if a machine register is left"""
    def call(m):
        rel = int(m.group(1), 16)
        if rel >= 0x80000000:
            rel -= 1 << 32
        t = (at + rel) & 0xFFFFF
        return (names.get(t) or "f%05x" % t) + "("
    text = re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, text)
    text = re.sub(r"\bg_([0-9a-f]{1,4})\b", lambda m: "g_%04x" % int(m.group(1), 16), text)
    macros = prj.toolchain().p.get("macros")
    text = library_macros(text, macros)                     # (the routines named: the macros they show)
    text = data_macros(prj, text, prj.toolchain().p.get("data_macros"))
    if FILEPTR:                                             # (the stream's fields the struct pass read: the macro's now)
        norm = lambda n: "g_%04x" % int(n[2:], 16) if re.fullmatch(r"g_[0-9a-f]{1,4}", n) else n
        streams = {norm(k) for k in FILEPTR if not k.startswith("#")}
        text = re.sub(r"(?m)^struct s_(\w+) \{[^}]*\};\n", lambda m: "" if norm(m.group(1)) in streams else m.group(0), text)
        text = fold_registers(stored_calls(text))
        text = unused_locals(text)
    macnames = {re.match(r"\w+", m["as"]).group(0) for m in (macros or [])} | \
        {re.match(r"\w+", f).group(0) for sp in (prj.toolchain().p.get("data_macros") or []) for f in sp.get("fields", {}).values()}
    body = text.split("{", 1)[1] if "{" in text else text
    # registers the model left: a draft for a person, not for the compiler
    left = sorted(set(re.findall(REGS, re.sub(r'"(?:[^"\\]|\\.)*"', "", body))))
    data = sorted(set(re.findall(r"\bg_[0-9a-f]{4}\b", text)))
    head = text.split("(", 1)[0].strip()
    funcs = sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body)) - {head, "if", "while", "for", "switch", "return", "sizeof"} - macnames)
    decl = []
    fptr = [d for d in data if re.search(r"\(\s*\*\s*%s\s*\)\s*\(" % d, body)]     # (data called through)
    for d in fptr:                                          # (a number given it that is a function's place: it)
        text = re.sub(r"(?m)^(\s*%s = )(\d+);" % d, lambda m: m.group(1) + (names.get(int(m.group(2))) or
                                                                             "f%05x" % int(m.group(2))) + ";", text)
        body = text.split("{", 1)[1] if "{" in text else text
    funcs = sorted(set(funcs) | {w for d in fptr for w in re.findall(r"(?m)^\s*%s = ([A-Za-z_]\w*);" % d, body)})
    arrs = [d for d in data if d in ARRAYS and d not in fptr]   # (indexed in place: an array, not a pointer)
    pvars = set(re.findall(r"(?m)^\s*(?:register\s+)?char\s*\*\s*(\w+);", text))   # (pointers the draft declares)
    flows = lambda d: any(re.search(r"(?m)^\s*%s = %s\b|^\s*%s = %s\b" % (p, d, d, p), body) for p in pvars)
    ptrs = [d for d in data if d not in fptr and d not in arrs and (re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:->|\[)" % (d, d), body)
                                                                  or flows(d))]   # (or given a pointer's value, or giving one)
    ints = [d for d in data if d not in ptrs and d not in fptr and d not in arrs]
    if arrs:
        decl += ["extern %s %s[];" % (ARRAYS[d], d) for d in arrs]
    if fptr:
        decl.append("extern int %s;" % ", ".join("(*%s)()" % d for d in fptr))
    streams = [d for d in data if d in FILEPTR]             # (a stream a library macro reads)
    ptrs = [d for d in ptrs if d not in streams]
    ints = [d for d in ints if d not in streams]
    decl += ["extern %s *%s;" % (FILEPTR[d], d) for d in streams]
    head2, sep2, body2 = text.partition("{")
    for p in [n for n in FILEPTR if not n.startswith("#") and n not in data]:   # (a parameter that is one)
        head2 = re.sub(r"(?m)^(\s*)(?:register )?(?:int|char \*|struct \w+ \*) ?%s;$" % p, r"\1%s *%s;" % (FILEPTR[p], p), head2)
        head2 = re.sub(r"(?m)^(\s*)int ((?:\w+, )*)%s((?:, \w+)*);$" % p,
                       lambda m: ("%sint %s;\n" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", ")) if (m.group(2) + m.group(3)).strip(", ") else "") +
                       "%s%s *%s;" % (m.group(1), FILEPTR[p], p), head2)
    text = head2 + sep2 + body2
    incs = list(dict.fromkeys(FILEPTR[k] for k in ("#include", "#include2") if FILEPTR.get(k)))
    for h in reversed(incs):
        decl.insert(0, "#include <%s>" % h)
    if ptrs:                                                # (data the draft goes through: pointers)
        for d in ptrs:
            decl.append("extern %s *%s;" % ("struct " + STRUCTPTR[d] if d in STRUCTPTR else "char *" if PTRSIZE.get(d) == 2 and re.search(r"\*\+\+%s\b" % d, body) else "char", d))
    uch = [d for d in ints if d in UCHAR]
    chs = [d for d in ints if d in BYTEG and d not in UCHAR]
    ints = [d for d in ints if d not in UCHAR and d not in BYTEG]
    if uch:
        decl.append("extern unsigned char %s;" % ", ".join(uch))
    if chs:
        decl.append("extern char %s;" % ", ".join(chs))
    if ints:
        decl.append("extern int %s;" % ", ".join(ints))
    for h in incs:                                          # (what an included header declares: its)
        funcs = [f for f in funcs if f not in header_names(prj, h)]
    if funcs:
        decl.append("extern %s;" % ", ".join("%s()" % f for f in funcs))
    return ("\n".join(decl) + "\n\n" if decl else "") + text, left


ERRLINE = re.compile(r"\((\d+)\) : (?:error \w+: )?(.*)$", re.M)


def _declare_pointer(src, x, ty):
    """the source with x (an int parameter or local) declared a pointer to ty, or None"""
    head, sep, body = split_body(src)
    for part in ("head", "body"):
        t = head if part == "head" else body
        m = re.search(r"(?m)^(\s*)(register\s+)?int ((?:\w+, )*)%s((?:, \w+)*);$" % re.escape(x), t)
        if m:
            rest = (m.group(3) + m.group(4).lstrip(", ")).strip(", ")
            new = ("%s%sint %s;\n" % (m.group(1), m.group(2) or "", rest) if rest else "") + "%s%s%s *%s;" % (m.group(1), m.group(2) or "", ty, x)
            t = t[:m.start()] + new + t[m.end():]
            out = (t + sep + body) if part == "head" else (head + sep + t)
            if ty == "int":                                 # (its byte offsets, now in words: x + N/2)
                out = re.sub(r"\(\(int \*\)\(\(char \*\)%s \+ (\d+)\)\)" % re.escape(x),
                             lambda m2: "(%s + %d)" % (x, int(m2.group(1)) // 2) if int(m2.group(1)) % 2 == 0 else m2.group(0), out)
                out = re.sub(r"\b%s\[\((.+?) << 1\) \+ (\d+)\]" % re.escape(x),     # (an index the code doubles)
                             lambda m2: "%s[%s + %d]" % (x, m2.group(1), int(m2.group(2)) // 2) if int(m2.group(2)) % 2 == 0 else m2.group(0), out)
                out = re.sub(r"\b%s\[\((.+?) << 1\)\]" % re.escape(x), r"%s[\1]" % x, out)
            return out
    return None


def compile_fixes(src, con):
    """mendings of the line the compiler stopped at, as the error says ([] when none):
        left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
        illegal indirection                             *X     -> X an int * (or a char *); else *(char *)X
        subscript on non-array                          X[I]   -> X an int *; else ((int *)X)[I]
    (the model's expressions carry no types: what the code shows is given them - each way, the bytes decide)"""
    errs = [(int(m.group(1)), m.group(2)) for m in ERRLINE.finditer(con) if "warning" not in m.group(2)]
    if not errs:
        return []
    n, msg = errs[0]
    lines = src.split("\n")
    if not 0 < n <= len(lines):
        return []
    l = lines[n - 1]
    out = []
    m = re.search(r"(?<![\w)\]])\*(?!\()\s*([A-Za-z_]\w*)", l) if "illegal indirection" in msg else \
        re.search(r"\b([A-Za-z_]\w*)\[", l) if "subscript on non-array" in msg else None
    if m:
        for ty in ("char", "int") if (m.group(1), 0) in BYTEPTR else ("int", "char"):   # (read a byte at a time: char)
            d = _declare_pointer(src, m.group(1), ty)
            if d:
                out.append(d)
    one = compile_fix(src, con)
    if one:
        out.append(one)
    return out


def compile_fix(src, con):
    """the source with the line the compiler stopped at mended, as the error says, or None:
        left of `->fN' must have a struct/union type   E->fN  -> *(int *)((char *)(E) + N)
        illegal indirection                             *X     -> *(char *)X
        subscript on non-array                          X[I]   -> ((int *)X)[I]
    (the model's expressions carry no types; a cast gives the compiler the one the code shows)"""
    errs = [(int(m.group(1)), m.group(2)) for m in ERRLINE.finditer(con) if "warning" not in m.group(2)]
    if not errs:
        return None
    n, msg = errs[0]
    lines = src.split("\n")
    if not 0 < n <= len(lines):
        return None
    l = lines[n - 1]
    m = re.search(r"left of .->f(\d+). must have a struct", msg)
    if m:
        f = m.group(1)
        new = re.sub(r"(\((?:[^()]|\([^()]*\))*\)|[A-Za-z_]\w*(?:\[[^\]]*\])?)->f%s\b" % f,
                     lambda x: "*(int *)((char *)%s + %s)" % (x.group(1), f), l, count=1)
    elif "illegal indirection" in msg:
        new = re.sub(r"(?<![\w)\]])\*(?!\()(\s*[A-Za-z_]\w*)", r"*(char *)\1", l, count=1)
    elif "subscript on non-array" in msg:
        new = re.sub(r"\b([A-Za-z_]\w*)\[", r"((int *)\1)[", l, count=1)
    else:
        return None
    if new == l:
        return None
    lines[n - 1] = new
    return "\n".join(lines)


def _prove(prj, tc, src, a, ln, tries=4, budget=10):
    """the verdict on src - mended where the compiler says it cannot be compiled, each way the error offers, and
    again (a few rounds, a few compiles): the best of what compiles. _prove.src is the source the verdict is of"""
    comp = lambda s: tc.compile(s.encode("latin1"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    frontier = [src]
    best = None
    first_con = None
    for depth in range(tries + 1):
        nxt = []
        for s in frontier:
            if budget <= 0:
                break
            budget -= 1
            raw, con = comp(s)
            if raw is None:
                first_con = first_con or (s, con)
                nxt += compile_fixes(s, con)[:3] if depth < tries else []
                continue
            v = _verdict(prj, raw, a, ln)
            if v == "identical":
                _prove.src = s
                return v
            if best is None or _rank(v) < _rank(best[1]):
                best = (s, v)
        if not nxt or budget <= 0:
            break
        frontier = nxt
    if best:
        _prove.src = best[0]
        return best[1]
    s, con = first_con if first_con else (src, "")
    _prove.src = s
    last = [l for l in con.strip().splitlines() if "(the pass" not in l]
    return "does not compile: %s" % (last[-1][:100] if last else "")


def _verdict(prj, raw, a, ln):
    o = omf.read(raw)
    segs = omf.code_segments(o)
    fns = omf.functions(o, segs[0]) if segs else []
    if not fns:
        return "no function compiled"
    name, off, l2 = fns[0]
    bad, notes = compare.function(o, prj.image, name, off, l2, a, prj, dict(prj.symbols()), {}, segs[0])
    if compare.identical(bad, notes) and l2 == ln:
        return "identical"
    return "differs: %d bytes%s" % (len(bad), ", length %d not %d" % (l2, ln) if l2 != ln else "")


def _rank(v):
    """how near a verdict is: one that differs (the right length first, then by fewer bytes), before one that
    does not compile, before one with registers left"""
    import re as _re
    m = _re.match(r"differs: (\d+) bytes(?:, length (\d+) not (\d+))?", v)
    if m:
        gap = abs(int(m.group(2)) - int(m.group(3))) if m.group(2) else 0
        return (0, 1 if gap else 0, int(m.group(1)) + gap)  # (what is missing counts as differing)
    return (1, 0, 0) if v.startswith("does not") else (2, 0, 0)


def prove_texts(prj, tc, inv, texts, a, ln, names):
    """-> (source, verdict): of each of the model's texts [(text, register parameters)] and their variants, the
    first the compiler proves identical; else the best of them (one that differs before one that does not
    compile, before one with registers left)"""
    best = None
    for text, regparam in texts:
        src, v = prove_best(prj, tc, inv, text, a, ln, names, regparam)
        if v == "identical":
            return src, v
        rank = _rank(v)
        if best is None or rank < best[0]:
            best = (rank, src, v)
    return best[1], best[2]


MAXCANDS = 48
BYTEPTR = set()                                             # (the byte-wide places the code reads: (pointer, offset))


def prove_best(prj, tc, inv, text, a, ln, names, regparam=None):
    """-> (source, verdict): of the model's text and its variants, the first the compiler proves identical; else
    the first that compiles (or the first)"""
    regs = tc.p.get("carve", {}).get("register_variables", ["si", "di"])
    if regparam is None:
        regparam = dict(getattr(inv, "LAST", {}).get("regparam") or {})
    widths = byte_fields(inv, bytes(prj.image[a:a + ln]))
    BYTEPTR.clear()
    BYTEPTR.update(widths)
    cands = candidates(prj, text, a, names, regs, regparam, widths)
    seen = set()
    uniq = []
    for src, left in cands:                                 # (each text once; the first readings first, and at
        if src not in seen:                                 #  most MAXCANDS of them: a bench must end)
            seen.add(src)
            uniq.append((src, left))
    cands = uniq[:MAXCANDS]
    best = None
    for src, left in cands:
        if left:
            v = "registers left: %s" % " ".join(left)
        else:
            v = _prove(prj, tc, src, a, ln)
            src = _prove.src                                # (as the compiler's errors mended it)
            if v != "identical":
                for alt in string_variants(prj, src):       # (a number that is a string's place: the string)
                    if _prove(prj, tc, alt, a, ln) == "identical":
                        src, v = _prove.src, "identical"
                        break
        if v == "identical":
            return src, v
        rank = _rank(v)
        if best is None or rank < best[0]:
            best = (rank, src, v)
    return best[1], best[2]


def table_hook(code):
    """SWITCH(address) for the model: the places a near jump through a table goes (`jmp cs:[bx + T]`, after
    `cmp R, N`: N + 1 words at T) - addresses in the function's own code, from 0. T is the program's offset of
    the table, so the function's own offset is found where its code begins being the program's"""
    from codebreaker.exact import x86
    ins = list(x86.disasm(code, 0, len(code), False))

    def hook(addr):
        k = next((j for j, i in enumerate(ins) if i.address == addr), None)
        if k is None:
            return None
        m = re.search(r"cs:\[bx \+ (0x[0-9a-f]+)\]", ins[k].op_str)
        n = next((int(x.group(1), 0) for i in reversed(ins[max(0, k - 8):k])
                  for x in [re.fullmatch(r"\w+, (0x[0-9a-f]+|\d+)", i.op_str)] if i.mnemonic == "cmp" and x), None)
        if not m or n is None:
            return None
        t = int(m.group(1), 16) - hook.base
        if not (0 <= t and t + 2 * (n + 1) <= len(code)):
            return None
        return [struct.unpack_from("<H", code, t + 2 * j)[0] - hook.base for j in range(n + 1)]
    hook.base = 0
    return hook


FRAME = [0]


def split_body(text):
    """(head, "{", body) at the function body's own brace - a line of "{" alone, after its signature - not a
    struct's that a pass declared before it"""
    m = re.search(r"(?m)^\{\s*$", text)
    if not m:
        return text.partition("{")
    return text[:m.start()], "{", text[m.start() + 1:]


def fill_frame(text, fs, regs_count=True):
    """locals declared for the part of the frame no declared local takes (a local the source declared and never
    used still has its place; MS C 3.00 gives a register local one too): a filler `char _pf[n]` at the end"""
    if not fs or "{" not in text:
        return text
    head, sep, body = split_body(text)
    size = 0
    lines = body.split("\n")
    last = 0
    for k, l in enumerate(lines[1:], 1):
        m = re.match(r"\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short|struct\s+\w+))\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$", l)
        if not m:
            if l.strip():
                break
            continue
        reg, ty, star, name, cnt = m.groups()
        last = k
        if reg:
            size += 2 if regs_count else 0
            continue
        if ty.startswith("struct") and not star:
            return text                                     # (a struct's size the text does not say)
        one = 2 if star else SIZES.get(ty.split()[-1], 2)
        n = one * int(cnt or 1)
        size += n + (n & 1)
    if fs <= size or re.search(r"(?m)^\s+(?:int|char|long|short)\s+v\d+", body):
        return text                                         # (the model's autos: the frame model lays those out)
    lines.insert(last + 1, "    char _pf[%d];" % (fs - size))
    return head + sep + "\n".join(lines)


def model_texts(inv, code, name, base=None):
    """[(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk;
    a structured control-flow graph, where the model has one): the compiler decides between them"""
    out = []
    fs, params, stmts, pt, loc = inv.recover(code)
    FRAME[0] = fs                                           # (the frame's size: fill_frame)
    regparam = dict(getattr(inv, "LAST", {}).get("regparam") or {})
    out.append((inv.to_c(name, fs, params, stmts, pt, loc), regparam))
    if hasattr(inv, "recover_cfg"):
        from codebreaker.invert import flow16
        flow16.CMPSWITCH = True                             # (switches by compares read as switches)
        old = getattr(inv, "SWITCH", None)
        if base is not None and hasattr(inv, "SWITCH"):     # (and by a table: where it sends each value)
            hook = table_hook(code)
            hook.base = base
            inv.SWITCH = hook
        try:
            fs, params, stmts, pt, loc = inv.recover_cfg(code)
        except Exception:
            return out
        finally:
            flow16.CMPSWITCH = False
            if hasattr(inv, "SWITCH"):
                inv.SWITCH = old
        stmts = [s for s in stmts if not s.startswith("/* register variables")]
        for reg, ix in regparam.items():                    # (a register parameter's load is not a statement)
            p = chr(ord("a") + ix)
            stmts = [s for k, s in enumerate(stmts) if not (k < 3 and re.fullmatch(r"%s = %s;" % (reg, p), s.strip()))]
            if p not in params:
                params = sorted(set(params) | {p})
        t = inv.to_c(name, fs, params, stmts, pt, loc)
        if t != out[0][0]:
            out.append((t, regparam))
    return out


def run(prj, every=False, limit=None, verbose=False):
    tc = prj.toolchain()
    model = tc.p.get("inverse")
    if not model:
        raise SystemExit("the toolchain %s names no inverse model (its profile's \"inverse\")" % tc.name)
    inv = importlib.import_module("codebreaker.invert." + model)
    names = _names(prj)
    img = prj.image
    have = {a for a, p in prj.sources()}
    out = {}
    os.makedirs(prj.path("drafts"), exist_ok=True)
    todo = [(a, ln) for a, ln, w in carve.functions(prj) if every or a not in have]
    for a, ln in todo[:limit]:
        if a in have:
            continue
        try:
            texts = model_texts(inv, bytes(img[a:a + ln]), "f%05x" % a, a)
        except Exception as e:                              # (the model declines: it says why)
            out[a] = "declined: %s" % str(e).splitlines()[0][:100] if str(e) else "declined"
            continue
        try:
            src, verdict = prove_texts(prj, tc, inv, texts, a, ln, names)
        except Exception as e:                              # (a pass that fails on one function: noted, not fatal)
            out[a] = "failed: %s: %s" % (type(e).__name__, str(e)[:100])
            continue
        out[a] = verdict
        if verdict == "identical":
            with open(prj.path("f%05x.c" % a), "w", newline="\n") as f:
                f.write("/* drafted by codebreaker exact draft (%s), identical */\n%s" % (model, src))
        else:
            with open(prj.path(os.path.join("drafts", "f%05x.c" % a)), "w", newline="\n") as f:
                f.write("/* draft (%s): %s */\n%s" % (model, verdict, src))
        if verbose:
            print("%05x %5d  %s" % (a, ln, verdict))
    return out


def main(prj, argv):
    lim = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    res = run(prj, "--all" in argv, lim, "-v" in argv)
    kinds = {}
    for v in res.values():
        k = v.split(":")[0]
        kinds[k] = kinds.get(k, 0) + 1
    print("%d functions drafted: %s" % (len(res), ", ".join("%d %s" % (n, k) for k, n in sorted(kinds.items(), key=lambda kv: -kv[1]))))
    return 0
