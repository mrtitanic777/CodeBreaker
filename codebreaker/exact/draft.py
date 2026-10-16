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
    rx = re.compile(r"(?m)^([ \t]*)if \((.*)\) \{\n\s*(ax|al|bx|cx|dx) = ([01]);\n\s*\} else \{\n\s*\3 = ([01]);\n\s*\}$")

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


def _balanced(s):
    depth = 0
    for ch in s:
        depth += (ch == "(") - (ch == ")")
        if depth < 0:
            return False
    return depth == 0


def _top_split(c, op):
    """c split at op (&& or ||) outside parentheses, or [c]"""
    out, depth, cur, k = [], 0, "", 0
    while k < len(c):
        ch = c[k]
        depth += (ch == "(") - (ch == ")")
        if depth == 0 and c.startswith(" %s " % op, k):
            out.append(cur)
            cur = ""
            k += len(op) + 2
            continue
        cur += ch
        k += 1
    return out + [cur]


def _not(c):
    c = _strip(c)
    inner = c[1:-1] if c.startswith("(") and c.endswith(")") and _balanced(c[1:-1]) else c
    m = re.fullmatch(r"!\((.*)\)", inner)                   # (!(X): X)
    if m and _balanced(m.group(1)):
        return "(%s)" % m.group(1)
    if re.fullmatch(r"![\w\->\[\].]+", inner):              # (!x: x)
        return inner[1:]
    for op, other in (("&&", "||"), ("||", "&&")):          # (De Morgan, a level down)
        parts = _top_split(inner, op)
        if len(parts) > 1:
            return "(%s)" % (" %s " % other).join(_truth(_not("(" + p + ")")) for p in parts)
    m = re.fullmatch(r"\((.*?) (==|!=|<=|>=|<|>) (.*)\)", c)
    if m and m.group(2) and "(" not in m.group(3).replace("(", "", 0) and m.group(1).count("(") == m.group(1).count(")"):
        flip = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[m.group(2)]
        if m.group(3) != "0" or m.group(2) not in ("==", "!="):   # (x == 0: !x below; a relation to 0 flips)
            return "(%s %s %s)" % (m.group(1), flip, m.group(3))
    m = re.fullmatch(r"\((.*) == 0\)", c)
    if m:
        return m.group(1)
    m = re.fullmatch(r"\((.*) != 0\)", c)
    if m:
        return "!%s" % m.group(1)
    if re.fullmatch(r"[\w\->\[\].]+", inner):
        return "!%s" % inner
    return "!%s" % c


def logical_values(text):
    """a || or && taken as a value - the compiler's 1/0 behind the tests - read back:
        if (A0) { if (B) { L: r = 1; } else { r = 0; } } else { goto L; }   ->  r = !A0 || B
        if (A) { if (B) { r = 1; } else { r = 0; } } else { r = 0; }        ->  r = A && B"""
    one = r"\s*"
    rx_or = re.compile(r"(?m)^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(L_\w+):\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n"
                       r"\s*\} else \{\n\s*goto \4;\n\s*\}$")
    text = rx_or.sub(lambda m: "%s%s = %s || %s;" % (m.group(1), m.group(5), _not(m.group(2)), _truth(m.group(3))), text)
    # (if (A) { if (B) { r = 1; } else { L: r = 0; } } else { goto L; }  ->  r = A && B)
    rx_and2 = re.compile(r"(?m)^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*(L_\w+):\n\s*\4 = 0;\n\s*\}\n"
                         r"\s*\} else \{\n\s*goto \5;\n\s*\}$")
    text = rx_and2.sub(lambda m: "%s%s = %s && %s;" % (m.group(1), m.group(4), _truth(m.group(2)), _truth(m.group(3))), text)
    rx_and = re.compile(r"(?m)^([ \t]*)if (\(.*\)) \{\n((?:\s*(?:ax|bx|cx|dx|si|di) = [^;\n]+;\n)*)\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n"
                        r"\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*\5 = 0;\n\s*\}$")

    def and_(m):
        b = m.group(4)
        for t in re.findall(r"(\w+) = ([^;\n]+);", m.group(3)):     # (the temporaries the second test reads)
            b = re.sub(r"\b%s\b" % t[0], lambda x: "(%s)" % t[1] if not re.fullmatch(r"\w+", t[1]) else t[1], b)
        return "%s%s = %s && %s;" % (m.group(1), m.group(5), _truth(m.group(2)), _truth(b))
    text = rx_and.sub(and_, text)
    return text


def _ternary(c, a, b):
    """C ? A : B - one test's two values; a value that is itself a test of the same two values joins the test
    (C ? (C2 ? X : Y) : X is (!C || C2) ? X : Y; C ? X : (C2 ? X : Y) is (C || C2) ? X : Y)"""
    for inner, other, join in ((a, b, "or-not"), (b, a, "or")):
        m = re.fullmatch(r"\((.*) \? (.*) : (.*)\)", inner)
        if m and _balanced(m.group(1)) and _balanced(m.group(2)) and m.group(2) == other:
            first = _truth(_not("(" + c + ")")) if join == "or-not" else _truth("(" + c + ")")
            return "%s ? %s : %s" % (_conj("||", first, _truth("(" + m.group(1) + ")")), m.group(2), m.group(3))
        if m and _balanced(m.group(1)) and _balanced(m.group(2)) and m.group(3) == other:   # (C ? (C2 ? X : Y) : Y:
            first = _truth("(" + c + ")") if join == "or-not" else _truth(_not("(" + c + ")"))   #  C && C2 ? X : Y)
            return "%s ? %s : %s" % (_conj("&&", first, _truth("(" + m.group(1) + ")")), m.group(2), m.group(3))
    return "%s ? %s : %s" % (_truth(c), a, b)


def step_tests(text):
    """a register stepped to test it - inc al; jne - read back as the compare it is: R = (X + K); if ((R..) == 0)
    -> if ((X == -K)) (a byte's: & 0xff)"""
    rx = re.compile(r"(?m)^([ \t]*)([abcd][lx]) = \((.+) ([+-]) (\d+)\);\n([ \t]*(?:if|while) \(\()\(?\2(?: & 0xff)?\)? (==|!=) 0\)\)(.*)$")

    def one(m):
        ind, r, x, op, k, head, rel, tail = m.groups()
        v = -int(k) if op == "+" else int(k)
        return "%s%s%s %s %d))%s" % (head, "" if True else "", x, rel, v, tail)
    return rx.sub(one, text)


def conditional_values(text):
    """a register given one value or another by a test, then read once by the next statement - C's ?: read back:
        if (C) { r = A; } else { r = B; } X = ..r..;   ->   X = ..(C ? A : B)..;
    (the arms give it nothing else, and the statement after reads it no more)"""
    rx = re.compile(r"(?m)^([ \t]*)if (\(.*\)) \{\n\s*%s = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\s*\}\n(\s*)(.*)$" % (r"(ax|bx|cx|dx|si|di|al|bl|cl|dl)"))

    def one(m):
        ind, c, r, a, b, ind2, nxt = m.groups()
        if ind2 != ind or len(re.findall(r"\b%s\b" % r, nxt)) != 1 or re.match(r"(?:if|while|for|switch|\}|L_\w+:)", nxt):
            return m.group(0)
        rest = m.string[m.end():]
        if re.search(r"\b%s\b" % r, re.split(r"\n\s*%s = (?![^;\n]*\b%s\b)" % (r, r), "\n" + rest)[0]) and r not in ("si", "di"):
            return m.group(0)                               # (read again later: not this way)
        if r in ("si", "di"):
            return m.group(0)                               # (a register variable: given a value it keeps)
        val = _ternary(c, a, b)
        if r.endswith("l"):                                 # (a byte register's: the value a char)
            return ind + re.sub(r"\b%s\b" % r, lambda x: "(char)(%s)" % val, nxt, count=1)
        return ind + re.sub(r"\b%s\b" % r, lambda x: "(%s)" % val, nxt, count=1)
    # (the value given first and given again when a test holds - r = A; if (C) { r = B; }: C ? B : A)
    pre = re.compile(r"(?m)^([ \t]*)(ax|bx|cx|dx) = ([^;\n]+);\n\1if (\(.*\)) \{\n\s*\2 = ([^;\n]+);\n\1\}\n\1(.*)$")

    def first(m):
        ind, r, a, c, b, nxt = m.groups()
        if len(re.findall(r"\b%s\b" % r, nxt)) != 1 or re.search(r"\b%s\b" % r, c) or \
                re.match(r"(?:if|while|for|switch|\}|L_\w+:)", nxt) or _live_after(m.string[m.end():].split("\n"), -1, r):
            return m.group(0)
        return ind + re.sub(r"\b%s\b" % r, lambda x: "(%s)" % _ternary(c, b, a), nxt, count=1)
    text = pre.sub(first, text)
    # (nested: an arm that is itself such a test - `else { if (C2) { r = B; } else { r = D; } }` - one value
    #  first, `r = (C2 ? B : D);`, then the outer test)
    inner = re.compile(r"(?m)^([ \t]*)if (\(.*\)) \{\n\s*(ax|bx|cx|dx) = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\1\}$")
    while True:
        t = inner.sub(lambda m: "%s%s = (%s);" % (m.group(1), m.group(3), _ternary(m.group(2), m.group(4), m.group(5))), text)
        t = rx.sub(one, t)
        if t == text:
            return t
        text = t


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


def range_switches(text):
    """a switch whose cases are one run of values - LO..HI - the compiler tests as a range (cmp LO, jb; cmp HI,
    jbe): R = E; if ((R >= LO)) { if ((R > HI)) { D } else { C } } else { D }  ->
    switch (E) { default: D case LO: .. case HI: C }"""
    lines = text.split("\n")
    for k in range(len(lines) - 1):
        a = re.fullmatch(r"([ \t]*)(ax|bx|cx|dx) = (.+);", lines[k])
        v = None
        if not a:                                           # (or a variable the code loads for the tests: RANGES)
            v = re.fullmatch(r"([ \t]*)if \(\(([\w\->]+) >= (\d+)\)\) \{", lines[k])
            if not v or not any(lo == int(v.group(3)) for lo, hi, _cf in RANGES):
                continue
            a = type("M", (), {"group": lambda s, i, v=v: {1: v.group(1), 2: v.group(2), 3: v.group(2)}[i]})()
        p = _if_parts(lines, k + 1 if not v else k) if a else None
        if not p or p[2] is None:
            continue
        c1 = re.fullmatch(r"\(\(%s >= (\d+)\)\)" % a.group(2), "(" + p[0] + ")")
        if c1 and p[1] and re.fullmatch(r"\s*(?:ax|bx|cx|dx) = %s;" % re.escape(a.group(2)), p[1][0]):
            p = (p[0], p[1][1:], p[2], p[3])                # (the selector loaded again for the next test: nothing)
        q = _if_parts(p[1], 0) if c1 and p[1] else None
        if not q or q[3] != len(p[1]):
            continue
        if q[2] is None:                                    # (the range's own arm empty: only a break)
            q = (q[0], q[1], [], q[3])
        c2 = re.fullmatch(r"\(\(%s (>|<=) (\d+)\)\)" % a.group(2), "(" + q[0] + ")")
        dd = lambda ls: [l[4:] if l.startswith("    ") else l for l in ls]
        if c2 and c2.group(1) == "<=":                      # (the range's own arm first: the same, turned about)
            q = (q[0], q[2], q[1], q[3])
        g_ = re.fullmatch(r"\s*goto (L_\w+);", p[2][0]) if c2 and len(p[2]) == 1 else None
        if g_ and dd(q[1]) and dd(q[1])[0].strip() == g_.group(1) + ":":   # (the other way out a goto into it:
            body_ = [l for l in dd(q[1]) if l.strip() != g_.group(1) + ":"]   #  the same code)
            if not re.search(r"\bgoto %s;" % g_.group(1), "\n".join(lines[:k] + lines[p[3]:])):
                p = (p[0], p[1], body_, p[3])
                q = (q[0], ["    " + l if l.strip() else l for l in body_], q[2], q[3])
        if not c2 or dd(q[1]) != p[2]:
            continue
        c2 = type("M", (), {"group": lambda s, i, c=c2: c.group(2)})()
        lo, hi = int(c1.group(1)), int(c2.group(1))
        if not 0 <= lo <= hi or hi - lo > 16:
            continue
        ind = a.group(1)
        dflt = [ind + "default:"] + p[2] + [ind + "    break;"]
        cases = ["%scase %d:" % (ind, v) for v in range(lo, hi + 1)] + dd(q[2]) + [ind + "    break;"]
        first = any(r[:2] == (lo, hi) and r[2] for r in RANGES)   # (the code shows the cases' arm first: no
        ends = [l for l in dd(q[2]) if l.strip()]                 #  default - what the rest do follows the switch,
        after = next((l for l in lines[p[3]:] if l.strip()), "")  #  the cases leaving)
        r_ = re.fullmatch(r"\s*return (\w+);", after)
        if first and r_ and ends and re.fullmatch(r"\s*%s = [^;]+;" % r_.group(1), ends[-1]) and \
                all(re.fullmatch(r"\s*(?:%s = [^;]+;|)" % r_.group(1), l) or not l.strip() for l in p[2][-1:]):
            sub_ = lambda ls: [re.sub(r"^(\s*)%s = (.*);$" % r_.group(1), r"\1return \2;", l) for l in ls]
            new = ["%sswitch (%s) {" % (ind, a.group(3))] + sub_(cases[:-1]) + [ind + "}"] + sub_([l[4:] if l.startswith("    ") else l for l in p[2]])
            lines[k:p[3]] = new
            k2 = next(j for j in range(k + len(new), len(lines)) if lines[j].strip())
            del lines[k2]                                   # (the return after: each way returns its own)
            return range_switches("\n".join(lines))
        if first and ends and re.match(r"\s*return\b", ends[-1]):
            new = ["%sswitch (%s) {" % (ind, a.group(3))] + cases[:-1] + [ind + "}"] + [l[4:] if l.startswith("    ") else l for l in p[2]]
        else:
            new = ["%sswitch (%s) {" % (ind, a.group(3))] + (cases + dflt if first else dflt + cases) + [ind + "}"]
        lines[k:p[3]] = new
        return range_switches("\n".join(lines))
    return text


def push_returns(text):
    """`return r;` at the end, every way to it giving r a value last (an if's arms, nested): each of those
    a return of its value, the last return gone - the compiler's jumps to the one exit read back"""
    lines = text.split("\n")
    for k in range(len(lines) - 2):                         # (an if's arm ending r = ax, r a register variable: r's)
        m = re.fullmatch(r"([ \t]*)ax = ([^;\n]+);", lines[k])
        if not m or lines[k + 1] != "%s%s = ax;" % (m.group(1), "si") and lines[k + 1] != "%s%s = ax;" % (m.group(1), "di") or \
                not re.fullmatch(r"[ \t]*\}(?: else \{)?", lines[k + 2]):
            continue
        ind = len(m.group(1)) - 4
        op = next((j for j in range(k - 1, -1, -1) if lines[j].strip() and len(lines[j]) - len(lines[j].lstrip()) == ind), None)
        if op is None or not re.match(r"[ \t]*(?:if \(.*\) \{|\} else \{|else \{)$", lines[op]):
            continue
        r = lines[k + 1].strip()[:2]
        lines[k], lines[k + 1] = "%s%s = %s;" % (m.group(1), r, m.group(2)), "%sax = %s;" % (m.group(1), r)
    lines = _else_ifs(lines)
    end = max((k for k, l in enumerate(lines) if re.fullmatch(r"    return (ax|bx|cx|dx|si|di);", l)), default=None)
    if end is None or end + 1 >= len(lines) or lines[end + 1] != "}":
        return text
    reg = re.fullmatch(r"\s*return (\w+);", lines[end]).group(1)

    def tail(lo, hi):                                       # (the statements lines[lo:hi] end giving reg a value:
        k = hi - 1                                          #  [indexes of those assignments], or None)
        while k >= lo and not lines[k].strip():
            k -= 1
        if k < lo:
            return None
        if re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[k]):
            return [k]
        if lines[k].strip() != "}":
            return None
        start = next((j for j in range(k - 1, lo - 1, -1) if re.match(r"\s*if \(.*\) \{$", lines[j]) and
                      len(lines[j]) - len(lines[j].lstrip()) == len(lines[k]) - len(lines[k].lstrip())), None)
        if start is None:
            return None
        p = _if_parts(lines, start)
        if not p or p[3] != k + 1 or p[2] is None:
            return None
        then_lo = start + 1
        then_hi = then_lo + len(p[1])
        a = tail(then_lo, then_hi)
        b = tail(then_hi + 1, k)
        return a + b if a is not None and b is not None else None
    ks = tail(0, end)
    if not ks:                                              # (a switch last, each arm giving it: case ..: return X;)
        k = end - 1
        if lines[k].strip() == "}":
            ind = lines[k][:len(lines[k]) - len(lines[k].lstrip())]
            st = next((j for j in range(k - 1, -1, -1) if lines[j].startswith(ind + "switch (") and lines[j].endswith("{")), None)
            if st is not None:
                arm, ok, rets = [], True, []
                for j in range(st + 1, k + 1):
                    l = lines[j]
                    if re.match(r"\s*(?:case [^:]+|default):$", l) or j == k:
                        if arm:
                            idx = [x for x in arm if lines[x].strip()]
                            if len(idx) >= 2 and lines[idx[-1]].strip() == "break;" and re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[idx[-2]]):
                                rets.append((idx[-2], idx[-1]))
                            elif idx and re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[idx[-1]]) and j == k:
                                rets.append((idx[-1], None))
                            elif idx and j != k and not re.match(r"\s*(?:break|continue|return|goto)\b", lines[idx[-1]]) and \
                                    not re.search(r"\b%s\b" % reg, " ".join(lines[x] for x in idx)):
                                pass                        # (an arm falling into the next: that one returns)
                            else:
                                ok = False
                        arm = []
                    else:
                        arm.append(j)
                if ok and rets:
                    drop = set()
                    for a_, b_ in rets:
                        lines[a_] = re.sub(r"^(\s*)%s = (.*);$" % reg, r"\1return \2;", lines[a_])
                        if b_ is not None:
                            drop.add(b_)
                    lines = [l for j, l in enumerate(lines) if j not in drop and j != end]
                    return "\n".join(lines)
                # (each `r = V; break;` anywhere in the arms, outside loops of their own, a return of V; when no
                #  break is left and every arm ends in a return or falls on, the last return is gone)
                inner = [j for j in range(st + 1, k) if re.match(r"\s*(?:while|for|do|switch)\b.*\{$", lines[j])]
                spans = [(j, _block_end(lines, j)) for j in inner]
                pairs = [j for j in range(st + 1, k - 1) if re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[j]) and
                         lines[j + 1].strip() == "break;" and not any(a < j < (b or k) for a, b in spans)]
                if pairs:
                    new = list(lines)
                    for j in pairs:
                        new[j] = re.sub(r"^(\s*)%s = (.*);$" % reg, r"\1return \2;", new[j])
                        new[j + 1] = None
                    rest = [x for x in new[st + 1:k] if x is not None]
                    left = any(x.strip() == "break;" for x in rest) or re.search(r"\b%s\b" % reg, "\n".join(rest))
                    last = next((x for x in reversed(rest) if x.strip()), "")
                    if not left and re.match(r"\s*return\b", last):
                        new[end] = None
                        return "\n".join(x for x in new if x is not None)
        return text
    if len(ks) < 2:
        return text
    for k in ks:
        lines[k] = re.sub(r"^(\s*)%s = (.*);$" % reg, r"\1return \2;", lines[k])
    del lines[end]
    return "\n".join(lines)


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
        m = re.match(r"(\s*)([abcd])[lx] = (.+);\s*$", lines[k] or "")   # (al, bl.. - or the word the model wrote)
        if m and re.fullmatch(r"\s*%sh = 0;\s*" % m.group(2), lines[k + 1] or ""):
            e = m.group(3)
            mk = re.fullmatch(r"\(%sl & (\d+)\)" % m.group(2), e)    # (a byte masked first: al = X; al = (al & K))
            if mk and k > 0 and lines[k - 1] is not None:
                p_ = re.fullmatch(r"\s*%sl = (.+);" % m.group(2), lines[k - 1])
                if p_:
                    e = "(%s & %s)" % (p_.group(1), mk.group(1))
                    lines[k - 1] = None
                    f0 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", p_.group(1))
                    if f0:
                        UCHAR.add((f0.group(1), int(f0.group(2))))
            lines[k] = "%s%sx = %s;" % (m.group(1), m.group(2), e)
            lines[k + 1] = None
            g = re.fullmatch(r"(g_[0-9a-f]{1,4})", e)
            f = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", e)       # (through a field too: its own struct's)
            if g:
                UCHAR.add("g_%04x" % int(g.group(1)[2:], 16))
            elif f:
                UCHAR.add((f.group(1), int(f.group(2))))
    text = "\n".join(l for l in lines if l is not None)

    def chained(m):                                         # (a byte stored, then widened with zeros and stored
        ind, x, e, r, y = m.groups()                        #  again: y = x = e, x an unsigned char)
        g = re.fullmatch(r"g_([0-9a-f]{1,4})", x)
        f = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", x)
        if g:
            UCHAR.add("g_%04x" % int(g.group(1), 16))
        elif f:
            UCHAR.add((f.group(1), int(f.group(2))))
        return "%s%s = %s = %s;" % (ind, y, x, e)
    return re.sub(r"(?m)^([ \t]*)([^=;\n]+?) = ([^;\n]+);\n[ \t]*([abcd])h = 0;\n[ \t]*([^=;\n]+?) = \4x;$", chained, text)


def high_zero(text):
    """al given a byte while ah is still the 0 an earlier widening left (no call, no ah between): ax read then
    is that byte widened - al = X; R = ax  ->  R = X (X then an unsigned char)"""
    lines = text.split("\n")
    zero = False
    for k, l in enumerate(lines):
        if l is None:
            continue
        if re.fullmatch(r"\s*ah = 0;", l):
            zero = True
            continue
        if re.search(r"\w\(|\bah\b|^\s*ax = |\b(?:if|while|for|switch)\b|[{}]|L_\w+:", l):
            zero = False if not re.fullmatch(r"\s*\w+ = ax;", l) else zero
            if re.search(r"\w\(|\bah\b|^\s*ax = |[{}]|L_\w+:", l):
                zero = False
            continue
        m = re.fullmatch(r"(\s*)al = (.+);", l)
        if zero and m and k + 1 < len(lines):
            n = re.fullmatch(r"(\s*)(\w+) = ax;", lines[k + 1])
            if n:
                lines[k] = "%s%s = %s;" % (n.group(1), n.group(2), m.group(2))
                lines[k + 1] = None
                f = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", m.group(2))
                if f:
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
        m = re.match(r"(\s*)(ax|bx|cx|dx|si|di|al|bl|cl|dl|t\d+) = (.*);\s*$", l)    # (a byte register too: a char;
                                                                                    #  the model's temporary)
        if m and k + 2 < len(lines):
            ind, reg, e = m.groups()
            st = re.match(r"\s*(\w+) = \1 \+ (\d)\s*;\s*$", lines[k + 1]) or re.match(r"\s*(\w+)\+\+()\s*;\s*$", lines[k + 1])
            if st:
                p, n = st.group(1), int(st.group(2) or 1)   # (p++: a step of one)
                deref = re.fullmatch(r"\*\((?:unsigned )?(char|int) \*\)%s|\*%s" % (p, p), e)
                size = 1 if deref and deref.group(1) == "char" else 2 if deref and deref.group(1) == "int" else None
                nxt = lines[k + 2]
                rest = "\n".join(lines[k + 3:])
                if deref and (size is None or size == n) and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
                        not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0]) and not re.search(r"\b%s\b" % p, nxt):
                    out.append(re.sub(r"\b%s\b" % reg, "%s++" % e, nxt, count=1))
                    k += 3
                    continue
                # (r = p; p = p + 1; X reads r once: p's old value - an index stepped, a[n++])
                if e == p and n == 1 and (reg in ("ax", "bx", "cx", "dx") or reg.startswith("t")) and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
                        not re.search(r"\b%s\b" % p, nxt) and not _live_after(lines, k + 2, reg):
                    out.append(re.sub(r"\b%s\b" % reg, "%s++" % p, nxt, count=1))
                    k += 3
                    continue
            sd = re.match(r"\s*(\w+) = \1 - 1\s*;\s*$", lines[k + 1]) or re.match(r"\s*(\w+)--\s*;\s*$", lines[k + 1])
            if sd and e == sd.group(1) and (reg in ("ax", "bx", "cx", "dx") or reg.startswith("t")):   # (and p--: the
                nxt = lines[k + 2]                                                                    #  old value)
                if len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and not re.search(r"\b%s\b" % e, nxt) and not _live_after(lines, k + 2, reg):
                    out.append(re.sub(r"\b%s\b" % reg, "%s--" % e, nxt, count=1))
                    k += 3
                    continue
        out.append(l)
        k += 1
    return "\n".join(out)


def fall_gotos(text):
    """goto L; where what comes after it, past the ends of ifs and a switch's case labels, is L: itself - the
    goto only falls there (a case falling into the next): gone, and L with it when nothing else goes there"""
    lines = text.split("\n")

    def opener(j):                                          # (the line opening the block line j closes)
        ind = len(lines[j]) - len(lines[j].lstrip())
        return next((q for q in range(j - 1, -1, -1) if lines[q] is not None and lines[q].strip() and
                     len(lines[q]) - len(lines[q].lstrip()) == ind and lines[q].rstrip().endswith("{")), None)
    changed = True
    while changed:
        changed = False
        for k, l in enumerate(lines):
            m = re.fullmatch(r"[ \t]*goto (L_\w+);", l or "")
            if not m:
                continue
            j = k + 1
            while j < len(lines):
                x = (lines[j] or "").strip()
                if lines[j] is None or not x:
                    j += 1
                    continue
                if x == "}":
                    q = opener(j)
                    if q is None or not re.match(r"[ \t]*(?:if \(.*\)|\} else|else(?: if \(.*\))?) \{$", lines[q]):
                        break
                    j += 1
                    continue
                if re.fullmatch(r"case [^:]+:|default:", x):
                    j += 1
                    continue
                break
            if j < len(lines) and (lines[j] or "").strip() == m.group(1) + ":":
                lines[k] = None
                changed = True
                break
            # (if (C) { goto L; } else { S } with L just after: if (!C) { S }, falling into L)
            c = re.fullmatch(r"([ \t]*)if (\(.*\)) \{", lines[k - 1] or "") if k else None
            if c and lines[k + 1:k + 2] == [c.group(1) + "} else {"]:
                e = _block_end([x if x is not None else "" for x in lines], k + 1)
                if e is not None and lines[e].strip() == "}":
                    j = e + 1
                    while j < len(lines) and re.fullmatch(r"[ \t]*(?:case [^:]+|default):", lines[j] or ""):
                        j += 1
                    if j < len(lines) and (lines[j] or "").strip() == m.group(1) + ":":
                        lines[k - 1] = "%sif (%s) {" % (c.group(1), _strip(_not(c.group(2))) if _strip(_not(c.group(2))).startswith("(") else "(%s)" % _not(c.group(2)))
                        lines[k] = lines[k + 1] = None
                        changed = True
                        break
    used = set(re.findall(r"goto (L_\w+);", "\n".join(x for x in lines if x is not None)))
    return "\n".join(x for x in lines if x is not None and not (re.fullmatch(r"[ \t]*(L_\w+):", x) and x.strip()[:-1] not in used))


def empty_loops(text):
    """for (;;) { [x = E;] if (C1) break; .. if (Cn) { break; } [else { continue; }] } - a loop of tests alone
    (its value given first): while ((x = E) && !C1 .. && !Cn) ; - the first test x's zero"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = re.match(r"([ \t]*)for \(;;\) \{$", lines[k])
        e = _block_end(lines, k) if m else None
        if not m or e is None or lines[e].strip() != "}":
            k += 1
            continue
        body = [x.strip() for x in lines[k + 1:e]]
        conds, first, q = [], None, 0
        a = re.fullmatch(r"(\w+) = ([^;=]+);", body[0]) if body else None
        if a:
            first, q = a.groups(), 1
        ok = True
        while q < len(body):
            t = re.fullmatch(r"if (\(.*\)) break;", body[q])
            if t:
                conds.append(t.group(1)); q += 1
                continue
            t = re.fullmatch(r"if (\(.*\)) \{", body[q])
            if t and body[q + 1:q + 3] == ["break;", "}"]:
                conds.append(t.group(1)); q += 3
                continue
            if t and body[q + 1:q + 5] == ["break;", "} else {", "continue;", "}"]:
                conds.append(t.group(1)); q += 5
                continue
            if body[q] == "continue;" and q == len(body) - 1:
                q += 1
                continue
            ok = False
            break
        if not ok and first and len(body) > 2 and re.fullmatch(r"if \(\((%s == 0)\)\) break;|if \(!%s\) break;" % (first[0], first[0]), body[1]):
            # (for (;;) { x = E; if (!x) break; S } - the value tested first, a body after: while ((x = E)) { S })
            lines[k:k + 3] = ["%swhile ((%s = %s)) {" % (m.group(1), first[0], first[1])]
            k += 1
            continue
        if not ok or not conds or any(not _balanced(c) for c in conds):
            k += 1
            continue
        gs = [_truth(_not(c)) for c in conds]
        if first:
            x, v = first
            if _strip(conds[0]) in ("(%s == 0)" % x, "!%s" % x):
                gs[0] = "(%s = %s)" % (x, v)
            else:
                k += 1
                continue
        g = " && ".join(c if re.fullmatch(r"[\w\->.\[\]]+|\(.*\)", c) and _balanced(c[1:-1] if c.startswith("(") else c) else "(%s)" % c for c in gs)
        lines[k:e + 1] = ["%swhile (%s)" % (m.group(1), g), "%s    ;" % m.group(1)]
        k += 2
    return "\n".join(lines)


def stored_reads(text):
    """M = E; x = M; - a value stored and read straight back from where it went (E a call's: no other way to
    have it twice): x = M = E, the compiler's chained store (the value kept in the register)"""
    return re.sub(r"(?m)^([ \t]*)(\w+(?:->f\d+)+) = ([^;\n]*\w\([^;\n]*\));\n\1(\w+) = \2;$",
                  lambda m: "%s%s = %s = %s;" % (m.group(1), m.group(4), m.group(2), m.group(3)), text)


def register_chains(text):
    """r = E; A = r; B = r; - r given once and stored twice at once, read no more: the value given twice,
    B = A = E (A stored first, as the compiler stores a chain from the right)"""
    lines = text.split("\n")
    for k in range(len(lines) - 2):
        m = re.match(r"([ \t]*)(ax|bx|cx|dx) = ([^;]+);$", lines[k] or "")
        if not m:
            continue
        ind, r, e = m.groups()
        a = re.match(r"%s([^;=]+?) = %s;$" % (ind, r), lines[k + 1] or "")
        b = re.match(r"%s([^;=]+?) = %s;$" % (ind, r), lines[k + 2] or "")
        if not a or not b or re.search(r"\b%s\b" % r, a.group(1) + b.group(1) + e) or _live_after(lines, k + 2, r):
            continue
        lines[k] = "%s%s = %s = %s;" % (ind, b.group(1), a.group(1), e)
        lines[k + 1] = lines[k + 2] = None
    return "\n".join(x for x in lines if x is not None)


def store_loops(text):
    """do { r = E; [p = p + 1;] M = r; } while (r != 0) - the value stored is what the loop tests: an empty loop
    on the assignment, while (M = E) ; (a string copied, while (*d++ = *s++) ;) - E's pointer stepped as *p++"""
    rx = re.compile(r"(?m)^([ \t]*)do \{\n[ \t]*(ax|al|bx|bl|cx|cl|dx|dl|t\d+) = ([^;\n]+);\n(?:[ \t]*(\w+) = \4 \+ 1;\n)?"
                    r"[ \t]*([^;\n=]+?) = \2;\n[ \t]*\} while \(\(?\2(?: != 0)?\)?\);")

    def one(m):
        ind, reg, e, p, dst = m.groups()
        if p:
            d = re.fullmatch(r"\*\((?:unsigned )?char \*\)%s|\*%s" % (p, p), e)
            if not d or re.search(r"\b%s\b" % p, dst):
                return m.group(0)
            e = "*%s++" % p
        if re.search(r"\b%s\b" % reg, dst + e):
            return m.group(0)
        return "%swhile (%s = %s)\n%s    ;" % (ind, dst, e, ind)
    return rx.sub(one, text)


def _live_after(lines, k, reg, read=None):
    """is reg (or what `read` matches: its byte too) read after line k before it is given a new value - on any
    path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing; one
    given at k's own depth, in a bare block, or out of k's block does (lines None: dropped)"""
    rx = re.compile(read or r"\b%s\b" % reg)
    kill = re.compile(r"\s*%s = (?![^;]*\b%s\b)" % (reg, reg))
    stack = []                                              # (the blocks open since k: True a conditional one)
    skip = None                                             # (the depth of an arm beside k's, not on its path)
    killed = None                                           # (the depth of a block that gave it a new value)
    for l in lines[k + 1:]:
        if l is None:
            continue
        opens, closes = l.count("{"), l.count("}")
        if skip is not None:
            skip += opens - closes
            if skip < 0:
                skip = None                                 # (the other arm closed: past the whole if)
            continue
        if re.match(r"\s*\} else\b.*\{\s*$", l) and not stack:   # (leaving k's arm into the other: skipped)
            skip = 0
            continue
        if not stack and re.match(r"\s*return\b", l) and not rx.search(l):
            return False                                    # (the path ends)
        if not stack and re.match(r"\s*(?:break|continue);", l):
            skip = 0                                        # (to the end of the block it leaves: the rest of
            continue                                        #  it - a switch's other cases - not on its path)
        if killed is not None and len(stack) < killed:
            killed = None                                   # (out of the block that gave it a new value)
        if kill.match(l):
            if not any(stack):
                return bool(rx.search(l.split("=", 1)[1]))
            if killed is None and rx.search(l.split("=", 1)[1]):   # (given a value on some paths: read only in it)
                return True
            killed = len(stack) if killed is None else min(killed, len(stack))
        elif rx.search(l) and killed is None:
            return True
        for _ in range(closes):
            if stack:
                stack.pop()                                 # (out of k's block: what follows is on its path)
        stack += [l.strip() != "{"] * opens
    return False


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
        if x in ("ax", "bx", "cx", "dx"):                  # (si, di: register variables - copied as variables)
            continue
        depth = 0
        used = False
        for j in range(k + 1, len(lines)):
            y = out[j]
            if y is None:                                   # (a copy dropped before: nothing)
                continue
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
        if not _live_after(out, k, r):
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
                if re.search(r" [-+*/%&|^<>]{1,2} ", e) and not (e.startswith("(") and e.endswith(")") and _balanced(e[1:-1])):
                    e = "(%s)" % e                          # (E an expression of its own: kept one, whatever op)
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


def copy_tests(text):
    """a byte copied and the copy tested (the compiler tests the register it moved it through):
        [s = s - 1;] *d = *s; [d = d + 1;] if (((*s & 0xff) == 0)) break;  ->  if ((*d++ = *--s) == 0) break;
    (the steps written where they go: --s before, d++ after)"""
    rx = re.compile(r"(?m)^([ \t]*)(?:(\w+) = \2 - 1;\n\s*)?\*(\w+) = \*(\w+);\n(?:\s*(\3) = \3 \+ 1;\n)?\s*if \(\(\(\*(\w+) & 0xff\) == 0\)\) break;$")

    def one(m):
        ind, pre, d, s, post, t = m.groups()
        if t != s or (pre and pre != s):
            return m.group(0)
        src = "*--%s" % s if pre else "*%s" % s
        dst = "*%s++" % d if post else "*%s" % d
        return "%sif (((%s = %s) == 0)) break;" % (ind, dst, src)
    return rx.sub(one, text)


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
            s = re.match(r"\s*(\w+) = \1 ([+-]) ([12]);\s*$", lines[k + 1]) or \
                re.match(r"\s*(\w+)(\+|-)[+-]()\s*;\s*$", lines[k + 1])          # (a step, then the test: --p in it;
            b = re.match(r"\s*if (\(.*\)) break;\s*$", lines[k + 2]) if s and k + 2 < len(lines) and lines[k + 2] else None   # by 2: words)
            if b and len(re.findall(r"\b%s\b" % s.group(1), b.group(1))) == 1:
                op = "++" if s.group(2) == "+" else "--"
                c = re.sub(r"\b%s\b" % s.group(1), op + s.group(1), b.group(1))
                lines[k] = "%swhile (%s) {" % (m.group(1), _truth(_not(c)))
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
            a_ = re.match(r"(\s*%s = )(.*)$" % reg, lines[k + 1])   # (its reads there gone - not a value it is given)
            stripped = a_.group(1) + re.sub(r"\b%s\b" % rd, "", a_.group(2)) if a_ else re.sub(r"\b%s\b" % rd, "", lines[k + 1])
            later = not _live_after(lines[:k + 1] + [stripped] + lines[k + 2:], k, reg, r"\b%s\b" % rd)
            # (read before it is given a value that does not read it, on any path: still wanted)
            if st and not re.search(r"\b%s\b" % rd, st.group(2)) and later:
                out.append("%s%s = %s;" % (st.group(1), st.group(2), call))
                k += 2
                continue
            if st and not re.search(r"\b%s\b" % rd, st.group(2)) and re.fullmatch(r"\w+", st.group(2)):
                lv = st.group(2)                            # (read again after: the variable it was stored in -
                j = k + 2                                   #  until it is given another value, a call, a label)
                fixed = list(lines)
                while j < len(fixed) and not re.match(r"\s*(?:%s|%s) = |\s*L_\w+:" % (reg, rd), fixed[j]) and \
                        not re.search(r"\w\(", fixed[j]) and not re.search(r"\b%s\s*=[^=]" % lv, fixed[j]):
                    fixed[j] = re.sub(r"\b%s\b" % rd, lv, fixed[j])
                    j += 1
                if not _live_after(fixed, k + 1, reg, r"\b%s\b" % rd):
                    lines = fixed
                    out.append("%s%s = %s;" % (st.group(1), lv, call))
                    k += 2
                    continue
            nxt = lines[k + 1]                              # (read once, anywhere in the next statement - which
            if reg in ("si", "di"):                         #  (a register variable's value: kept in it)
                later = False
            if later and len(re.findall(r"\b%s\b" % rd, nxt)) == 1 and not re.search(r"\w\(", nxt) and \
                    not re.match(r"\s*(?:\}|L_\w+:|else\b)", nxt) and not re.search(r"\b%s\s*=[^=]" % rd, nxt):
                out.append(re.sub(r"\b%s\b" % rd, lambda x: call, nxt, count=1))   # makes no call of its own:
                k += 2                                                               #  the order stays)
                continue
            calls = re.findall(r"\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\(", nxt)
            ca = re.search(r"\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", nxt)
            outside = re.sub(r"^\s*%s = " % reg, "", nxt[:ca.start()] + nxt[ca.end():]) if ca else ""
            if later and len(calls) == 1 and ca and len(re.findall(r"\b%s\b" % rd, ca.group(1))) == 1 and \
                    not re.search(r"\b%s\b" % rd, outside) and not re.match(r"\s*(?:\}|L_\w+:|else\b)", nxt):
                byte = re.search(r"\b%sl\b" % reg[0], ca.group(1)) and reg != "%sl" % reg[0]   # (an argument of the one
                v = "(char)(%s)" % call if byte else call                                    #  call there: its value
                arg = re.sub(r"\b%s\b" % rd, lambda x: v, ca.group(1), count=1)              #  goes first anyway; its
                out.append(nxt[:ca.start(1)] + arg + nxt[ca.end(1):])                        #  low byte, a char)
                k += 2
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
        body = re.sub(r"(?m)^[ \t]*char %s\[\d+\];\n" % f, "", body)
    for a, b in new.items():
        body = re.sub(r"\b%s\b" % a, b, body)
    return head + "{" + body


def pointer_flow(text):
    """the pointers a function's names are, from those it declares: a name compared with a pointer, given one's
    value, or giving its value to one is one too (a pointer compares without its sign: the code shows which)"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    decl = re.compile(r"(?m)^([ \t]*)(register\s+)?int\s+(\w+);$")
    ptr = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);", text))
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
                        re.search(r"(?m)^[ \t]*%s = %s\b|^\s*%s = %s\b" % (x, p, p, x), body):
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
    for m in list(re.finditer(r"(?m)^([ \t]*)int ([\w, ]+);$", head))[:1]:
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
    ps = re.findall(r"(?m)^[ \t]*register char \*(\w+);", text)[:most]
    out = []

    def words(t, p):                                        # (its steps in bytes now in words)
        t = re.sub(r"(?m)^([ \t]*)register char \*%s;" % p, r"\1register int *%s;" % p, t)
        return re.sub(r"\b%s = %s ([+-]) (\d+);" % (p, p), lambda m: "%s = %s %s %d;" % (p, p, m.group(1), int(m.group(2)) // 2)
                      if int(m.group(2)) % 2 == 0 else m.group(0), t)
    for p in ps:
        out.append(words(text, p))
    if len(ps) > 1:
        t = text
        for p in ps:
            t = words(t, p)
        out.append(t)
    return out


def pre_steps(text):
    """X--; (X++; X = X - 1;) then a test of X: the step in the test - if (--X ..) - a variant: the compiler
    tests the flags of the step there, not a compare"""
    lines = text.split("\n")
    changed = False
    k = 0
    while k + 1 < len(lines):
        m = re.match(r"\s*(\w+(?:->f\d+)?)(\+\+|--);\s*$", lines[k]) or re.match(r"\s*(\w+) = \1 ([+-]) [12];\s*$", lines[k])
        # (by 2: a pointer to words stepped once - declared so where the compiler asks)
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
STRUCTARR = {}                                              # (data an array of structs: name -> (size, first field's type))
ARRAYS = {}                                                 # (data a draft indexes: an array - name: element type)


def address_variants(text, dsize, most=2, lo=0x100):
    """a number a pointer is compared with, or given (`p < 14708`, `p = 14708`): the place of data there - an array
    at that offset, g_XXXX (a variant each: the compiler decides; a pointer compares without its sign)"""
    ptrs = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?(?:char|int|struct \w+)\s*\*\s*(\w+);", text))
    if not ptrs:
        return []
    nums = []
    for m in re.finditer(r"\b(%s)\s*(?:[<>]=?|==|!=|=)\s*(-?\d{3,5})\b" % "|".join(map(re.escape, ptrs)), text):
        n = int(m.group(2)) & 0xFFFF                        # (a place above 32K the model writes negative)
        if lo <= n < dsize and (n, m.group(2)) not in nums:
            nums.append((n, m.group(2)))
    out = []
    for n, lit in nums[:most]:
        name = "g_%04x" % n
        out.append((name, re.sub(r"(?<![\w.])%s(?![\w.])" % re.escape(lit), name, text)))
    return out


def word_arrays(text, lo=0x40):
    """(E << 1)->fN, N a place in the data (not a field's small offset): an array of words there indexed by E -
    g_XXXX[E] (the model reads `mov bx,E / shl bx,1 / [bx+N]` as a field of a pointer)"""
    rx = re.compile(r"\(((?:(?:\+\+|--)?[A-Za-z_]\w*(?:->f\d+)*(?:\+\+|--)?|\d+)(?:\s*[-+]\s*\d+)?) << 1\)->f(\d+)\b")

    def one(m):
        n = int(m.group(2))
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "int"
        return "%s[%s]" % (name, m.group(1))
    t = rx.sub(one, text)

    def paren(m):                                           # (an expression in parentheses the index: itself)
        n = int(m.group(2))
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "int"
        return "%s[%s]" % (name, m.group(1)[1:-1])
    t = re.sub(r"\((\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)) << 1\)->f(\d+)\b", paren, t)

    def byte_index(m):                                      # (a scratch register's value plus a place in the data:
        n = int(m.group(2))                                 #  an array of bytes there, the value its index)
        if n < 0x100:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "char" if ("data", n) in BYTEPTR else "int"
        return "%s[%s]" % (name, m.group(1))
    t = re.sub(r"\b(ax|bx|cx|dx)->f(\d+)\b", byte_index, t)
    for g in set(re.findall(r"\bg_([0-9a-f]+)\[", t)):     # (an element of the data the model named: an array)
        if int(g, 16) >= 0x40:
            ARRAYS.setdefault("g_%04x" % int(g, 16), "char" if ("data", int(g, 16)) in BYTEPTR or ("*", int(g, 16)) in BYTEPTR else "int")
    t = re.sub(r"\(((?:\+\+|--)?[A-Za-z_]\w*(?:\+\+|--)?)\)->f(\d+)\b", byte_index, t)   # (a variable's value -
                                                                                       #  stepped too - the index)
    # (a register given K * E, then an element at a place in the data through it: an array of structs of K
    #  bytes - g_N[E].f0, the struct laid out in localize)
    def struct_index(m):
        ind, k, e, rest = m.group(1), int(m.group(2)), m.group(3), m.group(4)
        g = re.search(r"\b(g_[0-9a-f]{4})\[bx\]", rest)
        if not g or k < 2 or k > 0x400:
            return m.group(0)
        STRUCTARR[g.group(1)] = (k, ARRAYS.get(g.group(1), "int"))
        ARRAYS[g.group(1)] = "struct s_%s" % g.group(1)
        return "%s%s" % (ind, rest.replace("%s[bx]" % g.group(1), "%s[%s].f0" % (g.group(1), e), 1))
    t = re.sub(r"(?m)^([ \t]*)bx = \((\d+) \* (.+?)\);\n[ \t]*(.*\bg_[0-9a-f]{4}\[bx\].*)$", struct_index, t)

    def struct_place(m):                                    # (K * E + N, N a place in the data: the address of
        k, e, n = int(m.group(1)), m.group(2), int(m.group(3))   #  the element E of an array of K-byte structs)
        if n < 0x100 or k < 3 or k > 0x400:
            return m.group(0)
        name = "g_%04x" % n
        STRUCTARR.setdefault(name, (k, "int"))
        ARRAYS[name] = "struct s_%s" % name
        return "&%s[%s]" % (name, e)
    t = re.sub(r"\(\((\d+) \* ([\w\->.]+)\) \+ (\d+)\)(?!->)", struct_place, t)

    def struct_product(m):                                  # (the product folded in place: (K * E)->fN)
        k, e, n = int(m.group(1)), m.group(2), int(m.group(3))
        if n < 0x100 or k < 2 or k > 0x400:
            return m.group(0)
        name = "g_%04x" % n
        STRUCTARR.setdefault(name, (k, "char" if ("data", n) in BYTEPTR else "int"))
        ARRAYS[name] = "struct s_%s" % name
        return "%s[%s].f0" % (name, e)
    t = re.sub(r"\((\d+) \* ([A-Za-z_][\w\->]*)\)->f(\d+)\b", struct_product, t)
    # (the compiler keeps bx on: later elements through it, until it is given again - and the product itself
    #  read as a pointer to a field, (K * E)->fN, an element at N)
    lines = t.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"[ \t]*(\w+)\[(.+?)\]\.f0\b", l) if "].f0" in l else None
        prod = re.search(r"\b(g_[0-9a-f]{4})\[(.+?)\]\.f0", l)
        if not prod or prod.group(1) not in STRUCTARR:
            continue
        kk, e = STRUCTARR[prod.group(1)][0], prod.group(2)
        for j in range(k + 1, len(lines)):
            if re.match(r"[ \t]*bx = ", lines[j]) or re.search(r"\b%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*%s\b" % (re.escape(e), re.escape(e)), lines[j]):
                break
            def elem(m2):
                STRUCTARR.setdefault(m2.group(1), (kk, ARRAYS.get(m2.group(1), "int") if not str(ARRAYS.get(m2.group(1), "")).startswith("struct") else "int"))
                ARRAYS[m2.group(1)] = "struct s_%s" % m2.group(1)
                return "%s[%s].f0" % (m2.group(1), e)
            lines[j] = re.sub(r"\b(g_[0-9a-f]{4})\[bx\]", elem, lines[j])
            lines[j] = re.sub(r"\(%d \* %s\)->f(\d+)\b" % (kk, re.escape(e)), lambda m2: elem(type("M", (), {"group": lambda s, i: "g_%04x" % int(m2.group(1))})()), lines[j])
    t = "\n".join(lines)
    # (I << 1)[E]: the model's word index into the words at E - ((int *)(E))[I]
    def word_index(m):                                      # (an offset in bytes: through a char *)
        i, e = m.group(1), m.group(2)
        o = re.fullmatch(r"([A-Za-z_]\w*) \+ (\d+)", e)
        return "((int *)((char *)%s + %s))[%s]" % (o.group(1), o.group(2), i) if o else "((int *)(%s))[%s]" % (e, i)
    t = re.sub(r"\(((?:[A-Za-z_][\w\->]*|\d+|\((?:[^()]|\([^()]*\))*\))(?:\s*[-+]\s*\d+)?) << 1\)\[([^\[\]]+)\]", word_index, t)
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


def _top_commas(s):
    """s split at its commas outside parentheses and brackets"""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([":
            depth += 1
        elif ch in ")]":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    return out + [cur]


def library_macros(text, macros):
    """a library macro the compiler expanded in line - stdio's getc(f) and putc(c, f): the count of the stream's
    buffer stepped, then its pointer, else the routine that fills or empties the buffer (the profile's "macros":
    [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h"}]) - read back as the macro: from
    the line that steps the stream's count to the routine's call, the region is the macro's value"""
    if not macros:
        return text
    for mac in macros:
        rx = re.compile(r"\b%s\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)" % re.escape(mac["call"]))
        while True:
            lines = text.split("\n")
            hit = next(((k, m) for k, l in enumerate(lines) for m in [rx.search(l)] if m), None)
            if not hit:
                break
            k1, m = hit
            args = [x.strip() for x in _top_commas(m.group(1))]
            f = args[-1]
            k0 = next((j for j in range(k1, -1, -1) if re.search(r"\b%s->f2(?:--| = %s->f2 - 1)" % (re.escape(f), re.escape(f)), lines[j])), None)
            if k0 is None:
                break
            for ix in mac.get("byte_args", []):             # (an argument the macro stores as a byte - putc's c:
                p_ = re.fullmatch(r"\*(\w+)", args[ix]) if ix < len(args) else None   # *p of an int *p read
                if p_ and re.search(r"(?m)^[ \t]*(?:register )?int \*%s;$" % p_.group(1), text):   # a byte at a time)
                    args[ix] = "*(char *)%s" % p_.group(1)
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
    rx = re.compile(r"(?m)^([ \t]*)if (\(.*\))(?: \{)?\n\s*return (.+);\n(?:\s*\}\n)?\s*return 0;$")

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


def loop_gotos(text):
    """the gotos back to a loop's top: a move shared at the label (`ax = E; L: for (;;) { r = ax;` - every
    `ax = V; goto L;` too) given in each place (r = E; r = V), a for (;;) left holding one while and a break that
    while, and a goto to the top of the loop it is in a continue"""
    for _ in range(4):
        lines = text.split("\n")
        done = False
        for k, l in enumerate(lines):
            m = re.match(r"([ \t]*)(L_\w+):$", l)
            if not m:
                continue
            lab = m.group(2)
            gos = [j for j, x in enumerate(lines) if x.strip() == "goto %s;" % lab]
            # (the move shared at the label: into each place that comes to it)
            j = k + 1
            if j < len(lines) and lines[j].strip() == "for (;;) {":
                j += 1
            mv = re.match(r"[ \t]*(\w+) = ax;$", lines[j]) if j < len(lines) else None
            if mv and k and re.match(r"[ \t]*ax = [^;]+;$", lines[k - 1]) and gos and \
                    all(g and re.match(r"[ \t]*ax = [^;]+;$", lines[g - 1]) for g in gos) and \
                    not _live_after(lines, j, "ax"):
                r = mv.group(1)
                for g in [k] + gos:
                    lines[g - 1] = re.sub(r"^([ \t]*)ax = ", r"\1%s = " % r, lines[g - 1])
                lines[j] = None
                text = "\n".join(x for x in lines if x is not None)
                done = True
                break
            # (a for (;;) holding one while and a break: the while)
            if j < len(lines) and lines[k + 1].strip() == "for (;;) {":
                e = _block_end(lines, k + 1)
                w = k + 2
                if e and re.match(r"[ \t]*while \(.*\) \{$", lines[w]):
                    we = _block_end(lines, w)
                    if we and we + 2 == e and lines[we + 1].strip() == "break;":
                        lines[w:we + 1] = [x[4:] if x.startswith("    ") else x for x in lines[w:we + 1]]
                        lines[k + 1] = None
                        lines[we + 1] = None
                        lines[e] = None
                        text = "\n".join(x for x in lines if x is not None)
                        done = True
                        break
            # (a goto to the top of the loop it is in: a continue)
            if k + 1 < len(lines) and re.match(r"[ \t]*(?:while \(.*\)|for \(.*\)) \{$", lines[k + 1]) and gos:
                e = _block_end(lines, k + 1)
                if e and all(k + 1 < g < e for g in gos):
                    ok = True
                    for g in gos:                           # (not inside a loop of its own)
                        for q in range(k + 2, g):
                            x = lines[q]
                            if re.match(r"[ \t]*(?:while \(.*\)|for \(.*\)|do) \{$", x):
                                qe = _block_end(lines, q)
                                if qe and q < g < qe:
                                    ok = False
                    if ok:
                        for g in gos:
                            lines[g] = lines[g].replace("goto %s;" % lab, "continue;")
                        lines[k] = None
                        text = "\n".join(x for x in lines if x is not None)
                        done = True
                        break
        if not done:
            break
    return text


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
        else:                                               # (the other way about: if (C) { X = X + K; continue; }
            j0 = max((j for j, l in enumerate(body) if l.startswith(i4 + "if (")), default=None)   #  else { ..leaves.. })
            q = _if_parts(body, j0) if j0 is not None else None
            if q and q[3] == len(body) and q[2] and len(q[1]) == 2 and re.fullmatch(i4 + "    (?:%s)" % st, q[1][0]) and \
                    q[1][1] == i4 + "    continue;" and re.match(r"\s*(?:break|return)\b", q[2][-1]) and \
                    i4 + "continue;" not in own:
                step = q[1][0].strip()
                body = body[:j0] + ["%sif (%s) {" % (i4, _truth(_not("(" + q[0] + ")")))] + q[2] + [i4 + "}"]
        if not step:
            k += 1
            continue
        init = ""
        p = k - 1                                           # (the init: the line before, or one a few before
        while p >= 0 and p > k - 4 and re.fullmatch(re.escape(ind) + r"[A-Za-z_]\w* = [^;]+;", lines[p]):   # with
            im = re.fullmatch(re.escape(ind) + r"([A-Za-z_]\w*) = ([^;]+);", lines[p])          # nothing between
            sv = re.match(r"[A-Za-z_]\w*", step).group(0)  # (the variable stepped)
            if im.group(1) == sv and not any(re.search(r"\b%s\b" % sv, lines[j]) for j in range(p + 1, k)):   # reading it)
                init = "%s = %s" % (im.group(1), im.group(2))
                break
            p -= 1
        head = "%sfor (%s; %s; %s) {" % (ind, init, _truth("(" + m.group(2) + ")"), step.rstrip(";"))
        new = [head] + body + [lines[e]]
        if init:
            lines[k:e + 1] = new
            del lines[p]
            k += 0
        else:
            lines[k:e + 1] = new
            k += 1
        changed = True
    return "\n".join(lines) if changed else None


def temporaries(text):
    """the model's temporaries (tN: a value it had to keep while it read another - an argument pushed first)
    read once, by the next statement: the value in its place (C's own order of evaluation gives it back); one
    read more often declared an int"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(t\d+) = (.+);$", lines[k])
        if m:
            x = m.group(2)
            uses = [j for j in range(len(lines)) if j != k and re.search(r"\b%s\b" % x, lines[j])]
            if uses == [k + 1] and len(re.findall(r"\b%s\b" % x, lines[k + 1])) == 1:
                e = m.group(3)
                lines[k + 1] = re.sub(r"\b%s\b" % x, lambda _: e if re.fullmatch(r"[\w\[\]\->.]+", e) else "(%s)" % e, lines[k + 1])
                del lines[k]
                continue
        k += 1
    text = "\n".join(lines)
    left = sorted(set(re.findall(r"\b(t\d+)\b", text.split("{", 1)[-1])))
    if left:
        head, sep, body = split_body(text)
        if sep:
            text = head + sep + "\n    int %s;" % ", ".join(left) + body
    return text


def default_last(text):
    """a switch's default arm, written first, moved to the end (a variant: the compiler puts the default's code
    after the tests wherever it is written, so the code alone does not say where)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"(\s*)switch \(.*\) \{", l)
        if not m or k + 1 >= len(lines) or lines[k + 1] != m.group(1) + "default:":
            continue
        ind = m.group(1)
        nxt = next((j for j in range(k + 2, len(lines)) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])), None)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if nxt is None or end is None or nxt > end:
            continue
        arm = lines[k + 1:nxt]
        if not re.match(r"\s*(?:break;|return\b|goto\b)", [x for x in arm if x.strip()][-1]):
            continue                                        # (it falls into the next: not movable)
        return "\n".join(lines[:k + 1] + lines[nxt:end] + arm + lines[end:])
    return None


def empty_cases_first(text):
    """a switch's arms that only break (case K: break;) moved to its top (a variant: such an arm has no code of
    its own - where it is written decides the layout of the others)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"(\s*)switch \(.*\) \{", l)
        if not m:
            continue
        ind = m.group(1)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if end is None:
            continue
        moved, rest, j = [], [], k + 1
        while j < end:
            if re.fullmatch(r"%scase [^:]+:" % re.escape(ind), lines[j]) and j + 1 < end and lines[j + 1].strip() == "break;" and \
                    (j == k + 1 or re.match(r"\s*(?:break;|return\b|goto\b|continue;)", lines[j - 1]) or lines[j - 1].strip().endswith("}")) and \
                    j > k + 1:
                moved += lines[j:j + 2]
                j += 2
                continue
            rest.append(lines[j])
            j += 1
        if moved:
            return "\n".join(lines[:k + 1] + moved + rest + lines[end:])
    return None


def default_after(text):
    """switch (..) { .. default: S; case K: return V; } - the default falling into a case that only returns: the
    case written first, S after the switch with its own return V (a variant: the source's layout of both)"""
    for m in re.finditer(r"(?m)^([ \t]*)switch \(.*\) \{\n", text):
        ind = m.group(1)
        end = next((j for j in re.finditer(r"(?m)^%s\}$" % re.escape(ind), text) if j.start() > m.end()), None)
        if end is None:
            continue
        body = text[m.end():end.start()]
        d = re.search(r"(?m)^%sdefault:\n((?:%s    [^\n]*\n)+)%s(case [^:\n]+:)\n%s    (return [^;\n]*;)\n$" %
                      (re.escape(ind), re.escape(ind), re.escape(ind), re.escape(ind)), body)
        if d and not re.search(r"(?m)^%s    (?:break|return|goto)\b" % re.escape(ind), d.group(1)):
            s_ = "".join(x[4:] + "\n" for x in d.group(1).rstrip("\n").split("\n"))
            new_body = "%s%s\n%s    %s\n" % (ind, d.group(2), ind, d.group(3)) + body[:d.start()]
            return text[:m.end()] + new_body + "%s}\n" % ind + s_ + "%s%s" % (ind, d.group(3)) + text[end.end():]
    return None


def push_tails(text):
    """if (C) { ..; r = A; } else { ..; r = B; } S(r); - the statement after, taking what each arm leaves in r, in
    each arm with its value: the compiler's one copy of a tail both arms ended with (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None or p[3] >= len(lines):
            continue
        def tail(arm):                                      # (the arm's last run of r = V; - {r: V}, its start)
            got, j = {}, len(arm) - 1
            while j >= 0 and not arm[j].strip():
                j -= 1
            while j >= 0:
                m_ = re.fullmatch(r"(\s*)(ax|bx|cx|dx|t\d+) = (.+);", arm[j])   # (or the model's temporary)
                if not m_ or m_.group(2) in got:
                    break
                got[m_.group(2)] = (m_.group(3), m_.group(1))
                j -= 1
            return got, j + 1
        ga, ja = tail(p[1])
        gb, jb = tail(p[2])
        nxt = lines[p[3]]
        regs = [r for r in ga if r in gb]
        if not regs or set(ga) != set(gb) or re.match(r"\s*(?:if|while|for|switch|\}|L_\w+:)", nxt) or \
                any(len(re.findall(r"\b%s\b" % r, nxt)) != 1 or _live_after(lines, p[3], r) for r in regs):
            continue
        pv = lambda v: v if re.fullmatch(r"[\w\->.]+", v) else "(%s)" % v

        def put(got):
            s_ = nxt.strip()
            for r in regs:
                s_ = re.sub(r"\b%s\b" % r, lambda _: pv(got[r][0]), s_, count=1)
            return got[regs[0]][1] + s_
        then = p[1][:ja] + [put(ga)]
        els = p[2][:jb] + [put(gb)]
        i0 = re.match(r"\s*", lines[k]).group(0)
        new = [lines[k]] + then + [i0 + "} else {"] + els + [i0 + "}"]
        t = "\n".join(lines[:k] + new + lines[p[3] + 1:])
        for r in regs:
            if r.startswith("t"):
                t = _drop_decl(t, r)
        return t
    return None


def split_ternaries(text):
    """return (C ? A : B); - as the two returns it may have been: if (C) return A; return B; (a variant: the
    compiler keeps one copy of a tail both share either way)"""
    def one(m):
        ind, inner = m.group(1), m.group(2)
        depth, q = 0, None
        for i, ch in enumerate(inner):                      # (the top-level ? and :)
            depth += (ch == "(") - (ch == ")")
            if ch == "?" and depth == 0 and q is None:
                q = i
            if ch == ":" and depth == 0 and q is not None:
                c, a, b = inner[:q].strip(), inner[q + 1:i].strip(), inner[i + 1:].strip()
                return "%sif (%s) {\n%s    return %s;\n%s}\n%sreturn %s;" % (ind, c if c.startswith("(") else "(%s)" % c, ind, a, ind, ind, b)
        return m.group(0)
    t = re.sub(r"(?m)^([ \t]*)return \((.*)\);$", one, text)
    return t if t != text else None


def store_reloads(text):
    """M = A; .. r = M; - a register the model reloads from the place a plain value was just stored in (its
    store-forwarding, written out before the pointer moves): the value itself, r = A"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)(ax|dx|bx|cx) = (\*\w+|\w+->f\d+);", lines[k])
        if not m:
            continue
        place = m.group(3)
        base = re.match(r"\*?(\w+)", place).group(1)
        for j in range(k - 1, max(k - 6, -1), -1):          # (the store just before, nothing between it and
            s = re.fullmatch(r"[ \t]*(.+?) = ([A-Za-z_]\w*|-?\d+);", lines[j])   #  here giving its value or
            if not s:                                                         #  base another)
                break
            if s.group(1) == place:
                if s.group(2) != base and not any(re.match(r"[ \t]*(?:%s|%s) = " % (re.escape(s.group(2)), base), lines[q]) for q in range(j + 1, k)):
                    lines[k] = "%s%s = %s;" % (m.group(1), m.group(2), s.group(2))
                break
    return "\n".join(lines)


def fn_pointers(text):
    """(*x)(..) - x called through: a pointer to a function, int (*x)(); (a parameter's or a local's int)"""
    for x in sorted(set(re.findall(r"\(\*([A-Za-z_]\w*)\)\s*\(", text))):
        if re.search(r"(?m)^[ \t]*(?:register\s+)?int (?:\w+, )*%s\b" % x, text):
            t = _redeclare(text, x, "int (*%s)()" % x)
            text = t or text
    return text


def scaled_index(text):
    """bx = p + (i << 1); .. *bx - a word of p's at i: p[i], p an int's pointer (the scaled index the model left
    as an address; a copy of the doubled index the code made on the way, `ax = (i << 1);`, nothing)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.match(r"([ \t]*)bx = ([A-Za-z_]\w*) \+ \((\w+|\((?:[^()]|\([^()]*\))+\)) << 1\);$", lines[k] or "")
        if not m:
            continue
        p, i = m.group(2), m.group(3)
        if i.startswith("(") and _balanced(i[1:-1]):
            i = i[1:-1]                                     # (an expression the index: as it is)
        j = k + 1
        while j < len(lines) and re.fullmatch(r"[ \t]*ax = \(%s << 1\);" % re.escape(i), lines[j] or "") and not _live_after(lines, j, "ax"):
            j += 1
        if j >= len(lines) or len(re.findall(r"(?<![\w)])\*bx\b(?!\+\+|--)", lines[j])) != 1 or \
                len(re.findall(r"\bbx\b", lines[j])) != 1 or _live_after(lines, j, "bx") or \
                any(re.search(r"\b%s\s*=[^=]" % v, lines[j]) for v in re.findall(r"[A-Za-z_]\w*", i)):
            continue
        lines[j] = re.sub(r"(?<![\w)])\*bx\b", "%s[%s]" % (p, i), lines[j])
        lines[k + 1:j] = [None] * (j - k - 1)
        lines[k] = None
        t = "\n".join(x for x in lines if x is not None)
        return _declare_pointer(t, p, "int") or t
    return text


def guard_returns(text):
    """if (C) { S } return X; last in the function: if (!C) return X; S return X; (a variant: a guard returning
    at once is laid out its own way)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        if not lines[k].startswith("    if (") or not lines[k].endswith("{"):
            continue
        p = _if_parts(lines, k)
        if not p or p[2] is not None:
            continue
        rest = [x for x in lines[p[3]:] if x.strip()]
        if len(rest) != 2 or not re.fullmatch(r"    return [^;]+;", rest[0]) or rest[1] != "}":
            continue
        neg = _not("(%s)" % p[0])
        new = ["    if %s %s" % (neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg, rest[0].strip())] + \
            [x[4:] if x.startswith("        ") else x for x in p[1]]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    return None


def bool_trees(text):
    """an if-tree whose every way ends giving r 1 or 0 (a goto to a labelled part of it the same part again):
    if (E) { r = 1; } else { r = 0; } - E the && / || of its tests, a part both ways share factored out
    (if (c) { X && T } else { T }: (!c || X) && T) - the compiler's one copy of the tests read back"""
    lines = _else_ifs(text.split("\n"))
    labels = {}

    def stmts(block):                                       # (the statements of a block: [(kind, data)])
        out, j = [], 0
        while j < len(block):
            l = block[j]
            if not l.strip():
                j += 1
                continue
            m = re.fullmatch(r"[ \t]*(L_\w+):", l)
            if m:
                out.append(("label", m.group(1)))
                j += 1
                continue
            p = _if_parts(block, j)
            if p:
                out.append(("if", p))
                j = p[3]
                continue
            out.append(("line", l.strip()))
            j += 1
        return out

    def node(block, reg):                                   # (the expression a block gives reg, or None)
        ss = stmts(block)
        lab = None
        if ss and ss[0][0] == "label":
            lab = ss[0][1]
            ss = ss[1:]
        if len(ss) != 1:
            return None
        kind, d = ss[0]
        if kind == "line":
            m = re.fullmatch(r"%s = ([01]);" % reg, d)
            if m:
                e = m.group(1)
            else:
                g = re.fullmatch(r"goto (L_\w+);", d)
                if not g:
                    return None
                e = ("goto", g.group(1))
        else:
            c, th, el = d[0], d[1], d[2]
            if el is None:
                return None
            a, b = node(th, reg), node(el, reg)
            if a is None or b is None:
                return None
            e = ("if", c, a, b)
        if lab:
            labels[lab] = e
        return e

    def text_of(e):
        if isinstance(e, str):
            return e
        if e[0] == "goto":
            return text_of(labels[e[1]]) if e[1] in labels and labels[e[1]] != e else None
        c, a, b = "(%s)" % _strip("(%s)" % e[1]) if not e[1].startswith("(") else e[1], e[2], e[3]
        a_, b_ = text_of(a), text_of(b)
        if a_ is None or b_ is None:
            return None
        t, f = _truth(c), _truth(_not(c))
        if b_ == "0":
            return t if a_ == "1" else "0" if a_ == "0" else _conj("&&", t, a_)
        if a_ == "1":
            return _conj("||", t, b_)
        if a_ == "0":
            return f if b_ == "1" else _conj("&&", f, b_)
        if b_ == "1":
            return _conj("||", f, a_)
        parts = [x.strip() for x in _top_split(a_, "&&")]
        tail = [x.strip() for x in _top_split(b_, "&&")]
        if len(parts) > len(tail) and parts[-len(tail):] == tail:   # (if (c) { X && T } else { T }: (!c || X) && T)
            x = " && ".join(parts[:-len(tail)])
            return _conj("&&", "(%s)" % _conj("||", f, x), b_)
        return None

    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            continue
        nxt = next((x for x in lines[p[3]:] if x.strip()), "")
        r = re.fullmatch(r"[ \t]*return (\w+);", nxt)
        if not r:
            continue
        labels.clear()
        e = node(lines[k:p[3]], r.group(1))
        if e is None or isinstance(e, str) or e[0] != "if" or all(isinstance(x, str) for x in e[2:]):
            continue                                        # (one test alone: as written - its sense is the code's)
        x = text_of(e)
        x = text_of(e) if x is None else x                  # (a goto to a label met later: again)
        if x is None or x in ("0", "1") or "goto" in x:
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        reg = r.group(1)
        new = ["%sif %s {" % (ind, x if x.startswith("(") and x.endswith(")") and _balanced(x[1:-1]) else "(%s)" % x), "%s    %s = 1;" % (ind, reg),
               "%s} else {" % ind, "%s    %s = 0;" % (ind, reg), "%s}" % ind]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    return text


def shared_tails(text):
    """if (A) { X; if (B) { T } } else { T } at the function's end - the compiler's one copy of T both ways
    reach: if (A) { X; if (!B) return; } T (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None or not p[1]:
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        rest = [x for x in lines[p[3]:] if x.strip()]
        if not rest or not (rest[0] == "}" or re.fullmatch(r"[ \t]*return( \w+)?;", rest[0]) and len(rest) > 1 and rest[1] == "}"):
            continue
        then = p[1]
        q = next((j for j in range(len(then)) if then[j].startswith(ind + "    if (") and then[j].endswith("{") and
                  len(then[j]) - len(then[j].lstrip()) == len(ind) + 4), None)
        while q is not None:                                # (the last if of the then-arm, nothing after it)
            pi = _if_parts(then, q)
            if pi and pi[2] is None and pi[3] == len(then):
                break
            q = next((j for j in range(q + 1, len(then)) if then[j].startswith(ind + "    if (") and then[j].endswith("{") and
                      len(then[j]) - len(then[j].lstrip()) == len(ind) + 4), None)
        if q is None:
            continue
        inner = [x[4:] for x in pi[1]]
        if [x.rstrip() for x in inner if x.strip()] != [x.rstrip() for x in p[2] if x.strip()]:
            continue
        neg = _not("(%s)" % pi[0])
        new = [lines[k]] + then[:q] + [ind + "    if (%s) return;" % (neg if neg.startswith("(") else "(%s)" % neg), ind + "}"] + \
            [x[4:] for x in p[2]]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    return None


def scaled_places(text):
    """(i << 1) + K, K a place in the data: the address of a word there, &g_K[i] (a variant)"""
    def one(m):
        k = int(m.group(2)) & 0xFFFF
        if k < 0x100:
            return m.group(0)
        name = "g_%04x" % k
        ARRAYS.setdefault(name, "int")
        return "&%s[%s]" % (name, m.group(1))
    t = re.sub(r"\(\(([\w\->.]+) << 1\) \+ (-?\d+)\)", one, text)
    for g in sorted(set(re.findall(r"&(g_[0-9a-f]{4})\[", t))):   # (declared, when the text is a whole source)
        if re.search(r"(?m)^\w+\(.*\)$", t) and not re.search(r"(?m)^(?:extern|struct)[^\n]*\b%s\b" % g, t):
            t = "extern int %s[];\n" % g + t
    return t if t != text else None


def lead_filler(text):
    """the frame's first local vN (bp-N) with room between it and bp: a filler declared before it (a variant:
    unused locals the source declared first, the model's layout taking the first one for read)"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    m = re.search(r"(?m)^([ \t]*)((?:unsigned\s+)?(int|char|long|short)|(?:\w+ )?\w+ \*) ?v(\d+);$", body)
    first = re.search(r"(?m)^[ \t]*(?!register\b)(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;$", body)
    if not m or not first or first.start() != m.start():
        return None
    size = 4 if m.group(3) == "long" else 1 if m.group(3) == "char" else 2
    gap = int(m.group(4)) - size
    if gap <= 0:
        return None
    fill = "".join("%sint _q%d;\n" % (m.group(1), 2 * j) for j in range(gap // 2)) + \
        ("%schar _q%d;\n" % (m.group(1), gap) if gap % 2 else "")      # (words: an array goes elsewhere in the frame)
    return head + sep + body[:m.start()] + fill + body[m.start():]


def register_copies(text):
    """r = x = E; with the register variable r given nothing else: x itself where r is read (a variant: the
    compiler went on through the value it still held - the register its own scratch, not a variable)"""
    for m in re.finditer(r"(?m)^([ \t]*)(r_\w+) = (\w+) = ([^;\n]+);$", text):
        r, x = m.group(2), m.group(3)
        rest = text[m.end():]
        if re.search(r"(?<![\w>.*])%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)%s\b" % (r, r), rest) or \
                re.search(r"(?<![\w>.*])%s\s*(?:=[^=]|\+\+|--)" % x, rest):
            continue
        t = text[:m.start()] + "%s%s = %s;" % (m.group(1), x, m.group(4)) + re.sub(r"\b%s\b" % r, x, rest)
        d = re.search(r"(?m)^[ \t]*register [^;\n]*\b%s;\n" % r, t)
        if d:
            t = t[:d.start()] + t[d.end():]
        if (r[2:] + "w", 0) in BYTEPTR and (r[2:] + "*", 0) not in BYTEPTR:   # (words read through the register:
            t = _redeclare(t, x, "int *%s" % x) or t                        #  an int's pointer)
        return t
    return None


def unsigned_registers(text):
    """the text with its int register locals unsigned (a variant: compared, the compiler's jumps say which)"""
    t = re.sub(r"(?m)^([ \t]*)register int (r_\w+);$", r"\1register unsigned \2;", text)
    return t if t != text else None


def no_default(text):
    """switch (..) { .. default: return X; } - a switch with no default, the return after it (a variant: the
    compiler's default jump goes to the code after the switch either way, laid out after the cases)"""
    t = re.sub(r"(?m)^([ \t]*)default:\n[ \t]*(return [^;\n]*;)\n([ \t]*)\}$",
               lambda m: "%s}\n%s%s" % (m.group(3), m.group(3), m.group(2)) if len(m.group(1)) == len(m.group(3)) else m.group(0), text)
    return t if t != text else None


def unmasked(text):
    """(A == (B & 0xff)) - a byte the model masked compared with another: the bytes compared (a variant: the
    compiler compares bytes of two chars as they are)"""
    t = re.sub(r"(==|!=) \(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\)", r"\1 \2", text)
    t = re.sub(r"\(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\) (==|!=)", r"\1 \2", t)
    flip = {"==": "!=", "!=": "=="}
    t = re.sub(r"!\(([\w\->.*\[\]]+) (==|!=) ([\w\->.*\[\]]+)\)", lambda m: "(%s %s %s)" % (m.group(1), flip[m.group(2)], m.group(3)), t)
    u = re.sub(r"\b(\w+->f\d+|\*\w+) (==|!=) (12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b",     # (a byte compared with
               lambda m: "%s %s (char)0x%x" % (m.group(1), m.group(2), int(m.group(3))), t)   #  128..255: the char's)
    return u if u != text else None


def high_words(text):
    """f(a, b): a never read, b only masked (b & K) - a long's high word tested alone: long a; (a & 0xK0000L)
    (a variant: the compiler tests just the high word of a long masked so)"""
    h = re.search(r"(?m)^(\w+)\(([\w, ]+)\)\n", text)
    if not h:
        return None
    ps = [p.strip() for p in h.group(2).split(",")]
    body = text[h.end():]
    for p, q in zip(ps, ps[1:]):
        uses = re.findall(r"(?<![\w>.])%s(?![\w(])" % q, body)
        masks = re.findall(r"\(%s & (-?\d+|0x[0-9a-f]+)\)" % q, body)
        decl = len(re.findall(r"(?m)^[ \t]*(?:register )?\w+ [^;\n]*\b%s\b[^;\n]*;$" % q, body))
        if re.search(r"(?<![\w>.])%s(?![\w(])" % p, re.sub(r"(?m)^[ \t]*(?:register )?\w+ [\w, *]+;$", "", body)) or \
                not masks or len(uses) != len(masks) + decl:
            continue
        def mask(m):
            k = int(m.group(1), 0) & 0xffff
            return "(%s & 0x%x0000L)" % (p, k)
        t = re.sub(r"\(%s & (-?\d+|0x[0-9a-f]+)\)" % q, mask, body)
        t = _drop_decl(t, q)
        t = _redeclare(t, p, "long %s" % p)
        if t is None:
            return None
        return text[:h.start()] + "%s(%s)\n" % (h.group(1), ", ".join(x for x in ps if x != q)) + t
    return None


def assign_in_args(text):
    """X = E; f(..X..); - the value passed from the register it was stored from: f(..(X = E)..) (a variant)"""
    def one(m):
        ind, x, e, call = m.groups()
        if len(re.findall(r"(?<![\w>.])%s(?![\w(])" % re.escape(x), call)) != 1 or \
                re.search(r"&\s*%s(?![\w(])" % re.escape(x), call):   # (its address passed: no value there)
            return m.group(0)
        return ind + re.sub(r"(?<![\w>.])%s(?![\w(])" % re.escape(x), lambda _: "(%s = %s)" % (x, e), call, count=1)
    t = re.sub(r"(?m)^([ \t]*)([A-Za-z_][\w\->\[\]]*) = ([^;\n=]+);\n[ \t]*((?:return )?[A-Za-z_]\w*\(.*\);)$", one, text)
    return t if t != text else None


def return_sources(text):
    """X = Y; return X; - the value still in the register it was stored from: return Y (a variant: a reload of X
    is other code)"""
    t = re.sub(r"(?m)^([ \t]*)([^=;\n]+?) = ([A-Za-z_]\w*);\n([ \t]*)return \2;$", r"\1\2 = \3;\n\4return \3;", text)
    return t if t != text else None


def compound_assigns(text):
    """X = X op E; - the compiler's `op [X], reg` - as C's compound assignment X op= E (a variant: MS C gives the
    two different code)"""
    rx = re.compile(r"(?m)^([ \t]*)([A-Za-z_*][\w\->\[\]*]*) = \2 ([-+*/%&|^]|<<|>>) (.+);$")

    def one(m):
        ind, x, op, e = m.groups()
        if e.startswith("(") and e.endswith(")") and _strip("(" + e + ")") == e:
            e = e[1:-1]
        return "%s%s %s= %s;" % (ind, x, op, e)
    t = rx.sub(one, text)
    return t if t != text else None


def chain_assigns(text):
    """A = E; B = A; - the value given twice from the register it is in: B = A = E (a variant: a reload of A
    reads the same)"""
    rx = re.compile(r"(?m)^([ \t]*)([^\s=;][^=;]*?) = ([^;\n]+);\n\1([^\s=;][^=;]*?) = \2;$")

    def one(m):
        ind, a, e, b = m.groups()
        if b == a or re.search(r"\b%s\b" % re.escape(b), e) or re.search(r"\+\+|--|\w\(", a + b):
            return m.group(0)
        return "%s%s = %s = %s;" % (ind, b, a, e)
    t = rx.sub(one, text)
    return t if t != text else None


def _else_ifs(lines):
    """`} else if (C) {` chains written out as nested ifs, each inside the else before it"""
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)\} else if \((.*)\) \{$", lines[k])
        if not m:
            k += 1
            continue
        ind = m.group(1)
        e = k + 1                                           # (the chain's end: the `}` at the if's indent that
        while e < len(lines) and not (lines[e] == ind + "}"):   #  closes the whole chain)
            e += 1
        if e >= len(lines):
            return lines
        inner = [ind + "    if (%s) {" % m.group(2)] + ["    " + l if l.strip() else l for l in lines[k + 1:e]] + [ind + "    }"]
        lines[k:e + 1] = [ind + "} else {"] + inner + [ind + "}"]
        k += 1
    return lines


def _if_parts(lines, k):
    """(cond, then, else, end) of the if at line k - `if (C) {`, its arms' lines, the line after it - or None"""
    m = re.match(r"(\s*)if \((.*)\) \{$", lines[k])
    if not m:
        return None
    ind = m.group(1)
    j = k + 1
    while j < len(lines) and not lines[j].startswith(ind + "}"):
        if lines[j].strip() and not lines[j].startswith(ind + " "):
            return None
        j += 1
    if j >= len(lines):
        return None
    then = lines[k + 1:j]
    if lines[j] == ind + "}":
        return m.group(2), then, None, j + 1
    if lines[j] != ind + "} else {":
        return None
    e = j + 1
    while e < len(lines) and not lines[e].startswith(ind + "}"):
        if lines[e].strip() and not lines[e].startswith(ind + " "):
            return None
        e += 1
    if e >= len(lines) or lines[e] != ind + "}":
        return None
    return m.group(2), then, lines[j + 1:e], e + 1


def _unlabelled(arm, other=None):
    """arm without the label the other arm's goto went to (the only label it loses: others are wanted)"""
    g = re.match(r"\s*goto (L_\w+);$", other[-1]) if other else None
    if other is not None and not g:
        return arm
    return [l for l in arm if not re.match(r"\s*L_\w+:$", l) or (g and l.strip() != g.group(1) + ":")]


def _conj(op, a, b):
    """a op b, an operand that is the other kind of join parenthesized (|| inside &&)"""
    wrap = lambda x: "(%s)" % x if op == "&&" and len(_top_split(_strip(x) if x.startswith("((") else x, "||")) > 1 else x
    return "%s %s %s" % (wrap(a), op, wrap(b))


def _same_arm(x, y):
    """do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label
    there (the compiler's one copy of a tail both reach) - and the label then wanted by none else"""
    if x == y:
        return True
    if not x or not re.match(r"\s*goto (L_\w+);$", x[-1]):
        return False
    lab = re.match(r"\s*goto (L_\w+);$", x[-1]).group(1)
    at = next((j for j, l in enumerate(y) if l.strip() == lab + ":"), None)
    return at is not None and x[:-1] == y[:at] and _unlabelled(x[:-1]) == x[:-1]


def guard_gotos(text):
    """a goto past statements under a test - the compiler's || of a test that skips them:
        if (C1) { if (C2) goto L; } S.. L:   ->   if (!C1 || !C2) { S.. }
        if (C) goto L; S.. L:                ->   if (!C) { S.. }
    (L then wanted by none else: dropped)"""
    lines = text.split("\n")
    changed = True
    while changed:
        changed = False
        for k in range(len(lines)):
            m = re.match(r"(\s*)if \((.*)\) \{$", lines[k])
            g = re.match(r"(\s*)if \((.*)\) goto (L_\w+);$", lines[k + 1]) if m and k + 2 < len(lines) else None
            one = re.match(r"(\s*)if \((.*)\) goto (L_\w+);$", lines[k])
            if m and g and lines[k + 2] == m.group(1) + "}":
                ind, lab = m.group(1), g.group(3)
                cond = "%s || %s" % (_truth(_not("(" + m.group(2) + ")")), _truth(_not("(" + g.group(2) + ")")))
                start = k + 3
            elif one:
                ind, lab = one.group(1), one.group(3)
                cond = _truth(_not("(" + one.group(2) + ")"))
                start = k + 1
            else:
                continue
            end = next((j for j in range(start, len(lines)) if lines[j] == ind + lab + ":"), None)
            if end is None or end == start:
                continue
            body = lines[start:end]
            if any(l.strip() and not l.startswith(ind) for l in body) or any(re.match(r"\s*L_\w+:", l) for l in body):
                continue
            depth = 0
            ok = True
            for l in body:                                  # (the statements between: whole ones)
                depth += l.count("{") - l.count("}")
                if depth < 0:
                    ok = False
            if not ok or depth:
                continue
            new = ["%sif (%s) {" % (ind, cond)] + ["    " + l if l.strip() else l for l in body] + [ind + "}"]
            rest = lines[:k] + new + lines[end + 1:]
            if re.search(r"\bgoto %s;" % lab, "\n".join(rest)):
                rest = lines[:k] + new + lines[end:]        # (wanted elsewhere: kept)
            lines = rest
            changed = True
            break
    return "\n".join(lines)


def merge_conditions(text):
    """ifs nested as the compiler's || and && leave them, joined again:
        if (A) { if (B) { P } else { Q } } else { Q }   ->  if (A && B) { P } else { Q }
        if (A) { if (B) { P } else { Q } } else { P }   ->  if (!A || B) { P } else { Q }
        if (A) { if (B) { P } }                         ->  if (A && B) { P }
    (the arms the model wrote twice, where both ways go)"""
    lines = _else_ifs(text.split("\n"))
    changed = True
    while changed:
        changed = False
        for k in range(len(lines)):
            p = _if_parts(lines, k)
            if not p:
                continue
            c1, then, els, end = p
            ind = re.match(r"\s*", lines[k]).group(0)
            body = [l for l in then if l.strip()]
            if not body:
                continue
            q = _if_parts(then, next(j for j, l in enumerate(then) if l.strip()))
            if not q or q[3] != len(then) and any(l.strip() for l in then[q[3]:]):
                continue
            c2, p2, q2, _ = q
            dd = lambda ls: [l[4:] if l.startswith("    ") else l for l in ls]
            new = None
            if els is None and q2 is None:
                new = ["%sif (%s) {" % (ind, _conj("&&", _truth("(" + c1 + ")"), _truth("(" + c2 + ")")))] + dd(p2) + [ind + "}"]
            elif els is not None and q2 is not None and _same_arm(els, dd(q2)):
                new = ["%sif (%s) {" % (ind, _conj("&&", _truth("(" + c1 + ")"), _truth("(" + c2 + ")")))] + dd(p2) + [ind + "} else {"] + _unlabelled(dd(q2), els) + [ind + "}"]
            elif els is not None and q2 is not None and _same_arm(els, dd(p2)):
                new = ["%sif (%s) {" % (ind, _conj("||", _truth(_not("(" + c1 + ")")), _truth("(" + c2 + ")")))] + _unlabelled(dd(p2), els) + [ind + "} else {"] + dd(q2) + [ind + "}"]
            if new:
                cand = lines[:k] + new + lines[end:]
                whole = "\n".join(cand)                     # (a label another goto still wants: kept, else not this way)
                if any(not re.search(r"(?m)^[ \t]*%s:$" % g, whole) for g in re.findall(r"\bgoto (L_\w+);", whole)):
                    continue
                lines = cand
                changed = True
                break
    return "\n".join(lines)


def register_temps(text):
    """a register variable given a call's value and read once, by the next statement, and never again: the
    compiler's own keeping of a value it computed first (strlen(b) of strlen(a) + strlen(b)) - the call in its
    place, the variable gone (a variant)"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    changed = False
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(\w+) = (\w+\((?:[^()]|\([^()]*\))*\));$", l)
        if not m or not re.search(r"(?m)^[ \t]*register [\w ]+\**\s*%s;$" % m.group(2), body):
            continue
        x = m.group(2)
        rest = [j for j in range(len(lines)) if j != k and re.search(r"\b%s\b" % x, lines[j]) and not re.match(r"\s*register\b", lines[j])]
        if len(rest) != 1 or rest[0] != k + 1 or len(re.findall(r"\b%s\b" % x, lines[k + 1])) != 1:
            continue
        lines[k + 1] = re.sub(r"\b%s\b" % x, lambda _: m.group(3), lines[k + 1])
        lines[k] = None
        lines = [y for y in lines if y is None or not re.match(r"\s*register [\w ]+\**\s*%s;$" % x, y)]
        changed = True
        break
    if not changed:
        return None
    return head + sep + "\n".join(l for l in lines if l is not None)


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
    # (an arm's last goto to another case's start, not the next: the statements there to their break, copied -
    #  the compiler's one copy of a tail both cases end with)
    for _ in range(4):
        hit = False
        for j, l in enumerate(lines):
            g = re.fullmatch(r"(\s*)goto (L_\w+);", l)
            if not g or j + 1 >= len(lines) or not re.match(r"\s*(?:case [^:]+|default):$|\s*\}$", lines[j + 1]):
                continue
            at = next((i for i, x in enumerate(lines) if x.strip() == g.group(2) + ":"), None)
            if at is None or at <= j:
                continue
            end = next((i for i in range(at + 1, len(lines)) if re.match(r"\s*(?:break;|return\b)", lines[i])), None)
            if end is None or any(re.match(r"\s*(?:case [^:]+|default):$|\s*L_\w+:", x) for x in lines[at + 1:end]):
                continue
            src_ind = len(lines[at + 1]) - len(lines[at + 1].lstrip()) if at + 1 <= end else 0
            lines[j:j + 1] = [g.group(1) + x[src_ind:] if len(x) - len(x.lstrip()) >= src_ind else x for x in lines[at + 1:end + 1]]
            if not re.search(r"\bgoto %s;" % g.group(2), "\n".join(lines)):
                lines = [x for x in lines if x.strip() != g.group(2) + ":"]
            hit = True
            break
        if not hit:
            break
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
        text = re.sub(r"(?m)^([ \t]*)int %s;" % v, r"\1char %s;" % v, text)
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


def register_values(text):
    """r = E; (E a variable, a field, *p) read more than once after, nothing between changing E or calling: E at
    each read - the compiler kept E in the register and read it there (r a machine register left over)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"([ \t]*)(ax|al|bx|bl|cx|cl|dx|dl) = ([A-Za-z_]\w*(?:->f\d+)*|\*[A-Za-z_]\w*);", l)
        if not m:
            continue
        reg, e = m.group(2), m.group(3)
        names = re.findall(r"[A-Za-z_]\w*", e)
        depth, uses, end = 0, [], len(lines)
        for j in range(k + 1, len(lines)):
            x = lines[j]
            depth += x.count("{") - x.count("}")
            if depth < 0 or re.match(r"[ \t]*%s = " % reg, x) or re.match(r"[ \t]*L_\w+:", x):
                end = j
                break
            if re.search(r"\w\s*\(", re.sub(r"\b(?:if|while|for|switch|return)\s*\(", "", x)) or \
                    any(re.search(r"(?<![\w>.])%s\s*(?:=[^=]|\+\+|--)" % re.escape(n), x) for n in names) or \
                    re.search(r"%s\s*=[^=]" % re.escape(e), x):
                end = None
                break
            if re.search(r"\b%s\b" % reg, x):
                uses.append(j)
        if end is None or len(uses) < 2 or _live_after(lines, (end - 1) if end else len(lines) - 1, reg):
            continue
        for j in uses:
            lines[j] = re.sub(r"\b%s\b" % reg, e, lines[j])
        lines[k] = None
        return "\n".join(x for x in lines if x is not None)
    return text


def _plain_store(l, reg):
    """`X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store"""
    m = re.fullmatch(r"\s*([^=;]+?) = ([^;]+);\s*", l)
    return bool(m) and not re.search(r"\b%s\b" % reg, m.group(1)) and not re.search(r"\+\+|--|[^=!<>]=[^=]", m.group(2)) \
        and not re.search(r"\w\s*\(", m.group(2) + m.group(1))


def fold_registers(text):
    """what a model leaves of the machine's registers, folded where the text alone shows how: a register given a
    value no later statement reads (a call's: the call; a load: nothing), a register returned right after it was
    given, a call the model wrote twice (once alone, once into a register)"""
    lines = text.split("\n")
    out = []
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(ax|al|bx|cx|dx|si|di|bl|cl|dl|ah|bh|ch|dh) = (.*);\s*$", l)
        if m:
            ind, reg, expr = m.groups()
            rest = "\n".join(lines[k + 1:])
            nxt = next((x for x in lines[k + 1:] if x.strip()), "")
            later = "\n".join(lines[lines.index(nxt, k + 1) + 1:]) if nxt in lines[k + 1:] else ""
            pure = re.fullmatch(r"[A-Za-z_]\w*|-?\d+|[A-Za-z_]\w*(?:->f\d+)+|\*[A-Za-z_]\w*|[A-Za-z_]\w*\[[A-Za-z_0-9]+\]", expr) or (
                not re.search(r"\w\s*\(|\+\+|--|[^=!<>]=[^=]", expr) and expr.startswith("(") and expr.endswith(")"))
            if not pure and re.fullmatch(r"[A-Za-z_][\w\->.]* (?:[-+&|^*]|<<|>>) (?:[A-Za-z_][\w\->.]*|\d+)", expr):
                expr, pure = "(%s)" % expr, True            # (a plain operation: in parentheses where it goes)
            # (a value with no effect: a name, a field, an element, or no call and no store in it)
            names = re.findall(r"[A-Za-z_]\w*", expr)
            if pure and reg not in ("si", "di") and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
               (re.fullmatch(r"\s*[A-Za-z_]\w* = %s;\s*" % reg, nxt) or _plain_store(nxt, reg) or not any(re.search(r"\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b" % (n, n), nxt) for n in names)) and \
               not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, later)[0]) and (_plain_store(nxt, reg) or not re.search(
                   r"\b%s\s*(=[^=]|\+\+|--)|(\+\+|--)\s*%s\b" % (re.escape(expr), re.escape(expr)), nxt)):
                j = lines.index(nxt, k + 1)                 # (a plain value read once, at once: in its place)
                bk = re.fullmatch(r"(\s*)%s = (\(%s [-+&|^] [^;]+\));\s*" % (re.escape(expr), reg), nxt)
                if bk and reg in ("al", "bl", "cl", "dl"):  # (a byte read, changed in a register, put back: the
                    nxt = "%s%s = (char)%s;" % (bk.group(1), expr, bk.group(2))   # value cast - else done in place)
                lines[j] = re.sub(r"\b%s\b" % reg, lambda m: expr, nxt)
                continue
            if re.fullmatch(r"\s*return %s;\s*" % reg, nxt) and (reg not in ("si", "di") or not re.search(r"\w\s*\(", expr)):
                out.append("%sreturn %s;" % (ind, expr))
                lines[lines.index(nxt, k + 1)] = "\x00"
                continue
            used = _live_after(lines, k, reg, r"\b%s\b" % ("(?:ax|al)" if reg == "ax" else reg)) or \
                reg in ("si", "di")                         # (a register variable: a loop may read it on its next pass)
            if not used:
                call = re.search(r"\b(?!if\b|while\b|sizeof\b)[A-Za-z_]\w*\s*\(|\(\*[^()]+\)\s*\(", expr)   # (a call: a name, or
                                                                                                            #  a pointer's, then
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
    rets = {}                                               # (a routine's value gone through to a field: a pointer to a
    callrx = r"\b((?!if\b|while\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\([^()]*\))*)\)->f(\d+)\b"   # struct it returns - cast)
    for f_, a_, off in re.findall(callrx, text):
        rets.setdefault(f_, set()).add(int(off))
    pre = []
    for f_, offs in sorted(rets.items()):
        fl, pos = [], 0
        for o in sorted(offs):
            if o > pos:
                fl.append("char _%d[%d];" % (pos, o - pos))
            b_ = ("bx*", o) in widths and ("bxw", o) not in widths
            fl.append("%s f%d;" % ("char" if b_ else "int", o))
            pos = o + (1 if b_ else 2)
        pre.append("struct s_r_%s { %s };" % (f_, " ".join(fl)))
    if rets:
        text = re.sub(callrx, lambda m: "((struct s_r_%s *)%s(%s))->f%s" % (m.group(1), m.group(1), m.group(2), m.group(3)), text)
        text = "\n".join(pre) + "\n" + text
    uses = {}
    for p, off in re.findall(r"\b(\w+)->f(\d+)\b", text):
        if p.startswith("s_r_"):
            continue
        uses.setdefault(p, set()).add(int(off))
    for p in re.findall(r"\*\(\*(\w+)\)|\*\*(\w+)", text):  # (a pointer read through twice: a struct whose first
        p = p[0] or p[1]                                    #  field points on - a list)
        if re.search(r"(?m)^[ \t]*(?:register )?int %s;|^[ \t]*int (?:\w+, )*%s\b" % (p, p), text):
            text = re.sub(r"\*\(\*%s\)|\*\*%s\b" % (p, p), "(%s->f0)->f0" % p, text)
            uses.setdefault(p, set()).add(0)
    if not uses:
        return text
    for p in list(uses):                                    # (p + N, the place of a field - lea - is &p->fN)
        for n in set(re.findall(r"\(%s \+ (\d+)\)" % p, text)):
            if int(n) < 0x100:
                uses[p].add(int(n))
                text = text.replace("(%s + %s)" % (p, n), "&%s->f%s" % (p, n))
    for p in uses:                                          # (*p of a struct pointer: its first field)
        whole = re.compile(r"(?<![\w>)\]])\*%s\b(?!\s*(?:->|\[|\+\+|--|\.))" % p)
        decl = re.compile(r"\s*(?:extern\s+|register\s+|static\s+)*(?:unsigned\s+)?(?:char|int|long|short|struct\s+\w+)\b")
        lines = text.split("\n")
        if any(whole.search(l) for l in lines if not decl.match(l)):
            text = "\n".join(l if decl.match(l) else whole.sub("%s->f0" % p, l) for l in lines)
            uses[p].add(0)
    for p, offs in list(uses.items()):                      # (a byte read inside a word read: no struct - the
        byte = lambda o: (p, o) in widths or ("*", o) in widths or \
            p.startswith("r_") and (p[2:] + "*", o) in widths and (p[2:] + "w", o) not in widths   # (a register variable's
                                                                    #  bytes: its register's) words through an int *, the bytes through
        words = {o for o in offs if not byte(o)}                     # a char * of it)
        if any(o % 2 and o - 1 in words for o in offs if byte(o)) and all(o % 2 == 0 for o in words):
            t2 = re.sub(r"\b%s->f(\d+)\b" % p, lambda m: "((char *)%s)[%s]" % (p, m.group(1)) if byte(int(m.group(1))) and
                        int(m.group(1)) % 2 else ("*%s" % p if m.group(1) == "0" else "%s[%d]" % (p, int(m.group(1)) // 2)), text)
            t3 = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % p, r"\1\2int *%s;" % p, t2)
            t2 = t3 if t3 != t2 else _declare_pointer(t2, p, "int")
            if t2:
                text = t2
                del uses[p]
                continue
        steps = set(re.findall(r"\b%s = %s [+-] (\d+);" % (p, p), text)) | \
            ({"1"} if re.search(r"(?<![\w>.])%s(?:\+\+|--)|(?:\+\+|--)%s\b" % (p, p), text) else set())   # (stepped by one, bytes alone read through
        if steps == {"1"} and all(byte(o) for o in offs) and max(offs) < 0x10 and \
                not re.search(r"\b%s->f\d+->" % p, text):                 #  it: a char *, its bytes p[N])
            t2 = re.sub(r"\b%s->f(\d+)\b" % p, lambda m: "*%s" % p if m.group(1) == "0" else "%s[%s]" % (p, m.group(1)), text)
            if not re.search(r"(?m)^[ \t]*(?:register\s+)?char \*%s;$" % p, t2):
                t2 = _declare_pointer(t2, p, "char")
            if t2:
                text = t2
                del uses[p]
    # (a field through which the code goes on - p->fN->fM, (p->fN)->fM, *(p->fN): a pointer to a struct of its
    #  own; the same struct when what it reaches are fields p has too - a list, a tree)
    inner = {}
    text = re.sub(r"\*\((\w+)->f(\d+)\)(?!\s*(?:->|\[))", r"(\1->f\2)->f0", text)
    for p, n, m_ in re.findall(r"\b(\w+)->f(\d+)\)?->f(\d+)\b", text):
        if p in uses:
            inner.setdefault((p, int(n)), set()).add(int(m_))
    text = re.sub(r"\((\w+->f\d+)\)->", r"\1->", text)
    # (a whole deref of a field chain that goes on as a pointer elsewhere - *p->fN, *p->fN->fM: its first field)
    for _ in range(3):
        for p, chain in set(re.findall(r"(?<![\w>)\]])\*(\w+)((?:->f\d+)+)\b(?!\s*(?:->|\[|\.))", text)):
            n = int(re.findall(r"->f(\d+)", chain)[-1])
            if p in uses and (re.search(r"\b%s%s->f\d+" % (p, re.escape(chain)), text) or (len(chain.split("->")) > 2)):
                text = re.sub(r"(?<![\w>)\]])\*%s%s\b(?!\s*(?:->|\[|\.))" % (p, re.escape(chain)), "%s%s->f0" % (p, chain), text)
                if len(chain.split("->")) == 2:
                    inner.setdefault((p, n), set()).add(0)
    ptrfield = {}
    defs = []
    deeper = {}                                             # (what chains reach further down: p->fA->fB->fC..)
    for p, chain in re.findall(r"\b(\w+)((?:->f\d+){3,})", text):
        if p in uses:
            deeper.setdefault(p, set()).update(int(o) for o in re.findall(r"->f(\d+)", chain)[2:])
    for (p, n), subs in sorted(inner.items()):
        bytes_ = {o for o in subs if ("*", o) in widths}    # (a byte the code reads where p has a word: another struct)
        # (what bx - the register the model goes on through - reads at an offset, beside what the others read
        #  there: a byte through one and a word through the other is two structs)
        width = lambda regs, o: {("b" if (r + "*", o) in widths else "") + ("w" if (r + "w", o) in widths else "") for r in regs} - {""}
        both = {o for o in subs if width(["bx"], o) and width(["si", "di"], o) and width(["bx"], o) != width(["si", "di"], o)}
        bytes_ = {o for o in subs if "b" in "".join(width(["bx"], o)) or (not width(["bx"], o) and o in bytes_)}
        if (subs <= uses[p] or (p, n) == (p, 0) and subs <= uses[p] | {0}) and not both and \
                not (bytes_ & ({n} | set(k for (q, k) in inner if q == p))):
            ptrfield[(p, n)] = "struct s_%s *" % p
            uses[p] |= deeper.get(p, set())                 # (p's own struct: what the chains reach further too)
            continue
        fl, pos = [], 0
        for o in sorted(subs):
            if o > pos:
                fl.append("char _%d[%d];" % (pos, o - pos))
            b_ = o in bytes_                     # (a byte read: a char - an
            pg = re.search(r"\b%s->f%d->f%d = (\w+);" % (p, n, o), text)   # (given a pointer variable: a pointer)
            pty = re.search(r"(?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$" % pg.group(1), text) if pg else None
            deep = sorted(set(int(x) for x in re.findall(r"\b%s->f%d->f%d->f(\d+)\b" % (p, n, o), text)))
            if deep and not b_:                             # (gone through once more: a pointer to a struct of its own)
                dl, dp = [], 0
                for x in deep:
                    if x < dp:
                        continue
                    if x > dp:
                        dl.append("char _%d[%d];" % (dp, x - dp))
                    dl.append("%s f%d;" % ("long" if x in LONGINFO.get("fields", ()) and x + 2 not in deep else "int", x))
                    dp = x + (4 if dl[-1].startswith("long") else 2)
                defs.append("struct s_%s_%d_%d { %s };" % (p, n, o, " ".join(dl)))
                pty = None
            lng_ = not b_ and not deep and not pty and o in LONGINFO.get("fields", ()) and o + 2 not in subs   # (a long one)
            fl.append("%s f%d;" % ("unsigned char" if any(isinstance(u, tuple) and u[1] == o for u in UCHAR) and b_
                                   else "char" if b_ else "struct s_%s_%d_%d *" % (p, n, o) if deep else
                                   "%s *" % pty.group(1) if pty else "long" if lng_ else "int", o))   #  unsigned one widened with zeros)
            pos = o + (1 if b_ else 4 if lng_ else 2)
        defs.append("struct s_%s_%d { %s };" % (p, n, " ".join(fl)))
        ptrfield[(p, n)] = "struct s_%s_%d *" % (p, n)
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
            if byte and not uch and (p, off) not in widths and p.startswith("g_") and ("bxw", off) in widths and \
                    ("bx*", off) not in widths:
                byte = False                                # (data's pointer, gone through in bx: bx's width there)
            uch = uch or byte and re.search(r"\b%s->f%d \S{1,2} (?:12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b" % (p, off), text)   # (a byte
                                                                                    #  compared with 128..255: unsigned)
            deref = re.search(r"(?<![\w>)\]])\*%s->f%d\b(?!\s*(?:->|\[|\.))" % (p, off), text)   # (*p->fN: a pointer)
            lng = off in LONGINFO.get("fields", ()) and off + 2 not in offs   # (a long field: two words given as one)
            pg = re.search(r"\b%s->f%d = (\w+);" % (p, off), text)          # (given a pointer variable: a pointer)
            pty = re.search(r"(?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$" % pg.group(1), text) if pg else None
            deref = deref or (pty and pty.group(1) == "int" and not byte and not lng)
            if pty and pty.group(1) != "int" and not byte and not lng and (p, off) not in ptrfield and not (off == 0 and selfref):
                ptrfield[(p, off)] = "%s *" % pty.group(1)
            fields.append("%s f%d;" % ("struct s_%s *" % p if off == 0 and selfref else ptrfield[(p, off)] if (p, off) in ptrfield
                                       else "unsigned char" if uch else
                                       "char" if byte else "int *" if deref else "long" if lng else "int", off))
            pos = off + (2 if (p, off) in ptrfield else 1 if byte else 4 if lng else 2)
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
        text = re.sub(r"(?m)^([ \t]*)(register )?(?:int|char \*) ?%s;" % p, r"\1\2struct %s *%s;" % (tag, p), text)
        text = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);" % p,
                      lambda m: "%sint %s;\n%sstruct %s *%s;" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", "), m.group(1), tag, p)
                      if (m.group(2) + m.group(3)).strip(", ") else "%sstruct %s *%s;" % (m.group(1), tag, p), text)
        STRUCTPTR[p] = tag
        for (q, n_), ty_ in ptrfield.items():               # (a struct pointer field plus bytes: through a char *)
            if q == p:
                text = re.sub(r"(?<![\w>)])%s->f%d \+ (\d+)\b" % (p, n_), r"(char *)%s->f%d + \1" % (p, n_), text)
        for f_ in re.findall(r"\bchar f(\d+);", defs[-1]):  # (a byte field masked to a byte: itself)
            text = re.sub(r"\(%s->f%s & 0xff\)" % (p, f_), "%s->f%s" % (p, f_), text)
        if ("*", 0) in widths or any(w[0].endswith("*") for w in widths if isinstance(w, tuple) and len(w) == 2 and isinstance(w[0], str)):
            text = re.sub(r"(?<![\w>.])%s\[" % p, "((char *)%s)[" % p, text)   # (indexed by bytes: through a char *)
    return "\n".join(defs) + "\n" + text


STRUCTPTR = {}                                              # (data the draft declares a struct pointer: localize)


def candidates(prj, text, at, names, regs, regparam, widths=()):
    """[(source, registers left)] - the model's text as the project's source, then the variants the compiler
    is to decide between: as read, without its last return (a function of no value), with the locals whose
    address it takes as the buffers they are - and both"""
    base = [text] + [v for v in [void_variant(text)] if v]
    out = []
    REGSLOTS[0] = bool(prj.toolchain().p.get("frame_order"))
    for t in base + [v for v in (buffers(t) for t in base) if v]:
        STRUCTPTR.clear()
        PTRSIZE.clear()
        ARRAYS.clear()
        STRUCTARR.clear()
        UCHAR.clear()
        BYTEG.clear()
        BYTEG.update(n for k, n in widths if k == "g")
        FILEPTR.clear()
        t = zero_extends(high_zero(copy_props(table_macros(prj, case_falls(char_stores(char_params(long_pairs(long_returns(store_reloads(fall_gotos(t))))))), prj.toolchain().p.get("table_macros")))))
        t = re.sub(r"&(\w+)\[(-?)(\d+)\]", lambda m: "(%s %s %s)" % (m.group(1), "-" if m.group(2) else "+", m.group(3)), t)
        t = re.sub(r"&(\w+)->f(\d+)\b", r"(\1 + \2)", t)  # (lea: &p[N], &p->fN - the place, p + N)
        t = fold_registers(stored_calls(store_loops(post_increments(widen_bytes(compare_values(chain_returns(branch_returns(push_returns(conditional_values(logical_values(step_tests(bool_trees(range_switches(loop_gotos(empty_loops(fall_gotos(t)))))))))))))))))
        t = stored_calls(fold_registers(push_returns(t)))   # (returns again, as the passes left the arms)
        t = stored_reads(scaled_index(register_chains(word_arrays(test_first_loops(copy_tests(post_steps(t)))))))
        t = fold_registers(conditional_values(t))           # (the ?: values again, the elements folded)
        t = fold_registers(push_returns(temporaries(empty_loops(merge_conditions(guard_gotos(self_updates(t)))))))
        wd = set(widths)                                    # (a parameter kept in a register: the bytes read through
        for r_, i_ in (regparam or {}).items():             #  that register its own)
            if isinstance(i_, int) and 0 <= i_ < 26:
                wd |= {(chr(97 + i_), o) for (q, o) in widths if q in (r_, r_ + "*") and (r_ + "w", o) not in widths}
        t = structs(pointer_flow(local_pointers(register_variables(shapes(long_pairs(t)), regs, regparam))), wd)
        t = re.sub(r"(?m)^[ \t]*(\w+) = \1;\n", "", t)         # (a register variable given itself: nothing)
        t = register_values(long_ladders(long_pairs(t)))                     # (the longs again, as the passes left them)
        t = frame_chars(fn_pointers(t), widths)
        t = fill_frame(t, FRAME[0], bool(prj.toolchain().p.get("frame_order")))
        variants = [t] + [v for v in [store_increments(t), pre_steps(t), pre_increments(t), assign_in_tests(t), and_returns(t), inner_registers(t), for_loops(t), register_temps(t), compound_assigns(t), return_sources(t), assign_in_args(t), split_ternaries(t), default_last(t), push_tails(t), high_words(t), unmasked(t), no_default(t), unsigned_registers(t), empty_cases_first(t), register_slots(t), register_copies(t), lead_filler(t), scaled_places(t), shared_tails(t), guard_returns(t), default_after(t)] if v]
        both = pre_steps(t)
        both = pre_increments(both) if both else None
        if both:
            variants.append(both)
        for v in list(variants)[:4]:                        # (a value given twice: chained)
            c = chain_assigns(v)
            if c:
                variants.insert(variants.index(v) + 1, c)
        for v in list(variants):                            # (a pointer register variable through which words
            for w in pointer_widths(v):                     #  are read: int *, the compiler decides)
                variants.append(w)
        if re.search(r"& 0xff\)|\b[a-z] [<>=!]=? (?:3[2-9]|[4-9]\d|1[01]\d|12[0-6])\b", t):   # (a byte compared, or
            variants += char_param_variants(t)                                              #  a char: a char param)
        rule = prj.toolchain().p.get("frame_order")
        for v in list(variants):                            # (the names the frame's order asks for)
            fn = frame_names(v, rule) if rule else None
            variants.insert(variants.index(v) + 1, fn) if fn else None
        dsize = 0x10000                                     # (the data and the BSS after it: any place a pointer is compared with)
        for v in list(variants)[:2]:                        # (a number that is a place in the data)
            for name, av in address_variants(v, dsize):
                ARRAYS.setdefault(name, "char")
                variants.append(av)
                variants += pointer_widths(av)[:1]          # (and its pointer stepping words through it)
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
    FRAMEW.clear()
    RANGES.clear()
    insns = list(x86.disasm(code, 0, len(code), False))
    for k in range(len(insns) - 4):                         # (a register loaded and tested against a range: a switch's)
        a_, b_, c_, d_ = insns[k + 1:k + 5]
        lo_ = "0" if a_.mnemonic == "or" and a_.op_str == "ax, ax" else a_.op_str[4:] if a_.mnemonic == "cmp" else None
        if insns[k].mnemonic == "mov" and insns[k].op_str.startswith("ax, ") and lo_ is not None and \
                c_.mnemonic == "cmp" and (a_.op_str.startswith("ax, ")) and c_.op_str.startswith("ax, ") and \
                b_.mnemonic in ("jl", "jb") and d_.mnemonic in ("jg", "ja", "jle", "jbe"):
            try:                                            # (jg/ja to the default: the cases' code first)
                RANGES.add((int(lo_, 0), int(c_.op_str[4:], 0), d_.mnemonic in ("jg", "ja")))
            except ValueError:
                pass
    for k, i in enumerate(insns):                           # (a slot's byte loaded and widened with zeros: unsigned)
        z = re.fullmatch(r"al, byte ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\]", i.op_str) if i.mnemonic == "mov" else None
        if z and any(x.op_str in ("ah, ah", "ah, 0") and x.mnemonic in ("sub", "xor", "mov") for x in insns[k + 1:k + 4]):
            FRAMEW.add(((1 if z.group(1) == "+" else -1) * int(z.group(2), 0), "zx"))
    for i in insns:         #  through a register at an offset - whatever the
        m = re.search(r"byte ptr \[(?:bx|si|di) \+ (0x[0-9a-f]+|\d+)\]", i.op_str)    # draft names the pointer)
        if m and int(m.group(1), 0) < 0x100:
            out.add(("*", int(m.group(1), 0)))
        elif m:
            out.add(("data", int(m.group(1), 0)))           # (a byte in the data indexed by a register)
        for wd, r_, o_ in re.findall(r"(byte|word) ptr \[(bx|si|di)(?: \+ (0x[0-9a-f]+|\d+))?\]", i.op_str):
            o_ = int(o_ or "0", 0)                          # (each width at each offset through each register:
            if o_ < 0x100:                                  #  an offset read both ways is two structs')
                out.add((r_ + ("*" if wd == "byte" else "w"), o_))
        g = re.search(r"byte ptr \[(0x[0-9a-f]+|\d+)\]", i.op_str)     # (a global read or written a byte at a time -
        if g and not any("word ptr [%s]" % g.group(1) in x.op_str for x in insns):   #  never a word: a word's low byte)
            out.add(("g", "g_%04x" % int(g.group(1), 0)))
        f = re.search(r"(byte|word) ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\]", i.op_str)   # (a frame slot by its width:
        if f:                                                                        #  FRAMEW: (+-N, width))
            FRAMEW.add(((1 if f.group(2) == "+" else -1) * int(f.group(3), 0), f.group(1)))
    return out


FRAMEW = set()                                              # (the frame slots the code reads or writes, by width)
RANGES = set()                                              # (the ranges a loaded register is tested against: (lo, hi))


def frame_chars(text, widths):
    """the locals and parameters the code only ever reads or writes a byte of (its frame slots, by width): chars"""
    slots = {}
    for off, w in FRAMEW:
        slots.setdefault(off, set()).add(w)
    chars = []
    for off, ws in slots.items():
        if ws - {"zx"} != {"byte"}:
            continue
        name = "v%d" % -off if off < 0 else chr(ord("a") + (off - 4) // 2) if off >= 4 and off % 2 == 0 else None
        if name:                                            # (widened with zeros where it is read: unsigned)
            chars.append((name, "unsigned char" if "zx" in ws else "char"))
    for x, ty in chars:
        text = re.sub(r"(?m)^([ \t]*)int %s;$" % x, r"\1%s %s;" % (ty, x), text)
        text = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$" % x, lambda m: "%sint %s;\n%s%s %s;" % (
            m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", "), m.group(1), ty, x), text)
        text = re.sub(r"(?m)^([ \t]*)char %s;$" % x, r"\1%s %s;" % (ty, x), text) if ty != "char" else text
    return text


def buffers(text):
    """the locals whose address the text takes (`&vN`) as char arrays, each as long as the frame has room before
    the next local (vN is at bp-N); a store into one goes through an int"""
    locs = sorted({int(n) for n in re.findall(r"(?m)^[ \t]*int v(\d+);", text)})
    taken = [n for n in locs if re.search(r"&v%d\b" % n, text)]
    if not taken:
        return None
    for n in taken:
        lower = [m for m in locs if m < n]
        size = n - (max(lower) if lower else 0)
        text = re.sub(r"(?m)^([ \t]*)int v%d;" % n, r"\1char v%d[%d];" % (n, size), text)
        text = re.sub(r"&v%d\b" % n, "v%d" % n, text)
        text = re.sub(r"(?m)^([ \t]*)v%d = " % n, r"\1*(int *)v%d = " % n, text)
    return text


def long_returns(text):
    """a function whose every return gives dx too (the high half: dx:ax) returns a long: `dx = vM; return vN;`
    (M = N - 2, the two halves of one local) is `return vN;` of a long vN; `dx = (0 - (E >> 15)); return E;` (the
    sign of E spread: cwd) and `dx = 0; return E;` the value widened - C does that itself"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    rets = list(re.finditer(r"(?m)^([ \t]*)return ([^;]+);$", body))
    if not rets or not all(re.search(r"(?m)^[ \t]*dx = [^;]+;\n\s*\Z", body[:r.start()]) for r in rets):
        return text
    longs = set()

    def one(m):
        ind, hi, lo = m.group(1), m.group(2), m.group(3)
        h, l = re.fullmatch(r"v(\d+)", hi), re.fullmatch(r"v(\d+)", lo)
        if h and l and int(h.group(1)) == int(l.group(1)) - 2:
            longs.add((lo, hi))
            return "%sreturn %s;" % (ind, lo)
        if hi == "(0 - (%s >> 15))" % lo or hi == "(0 - (%s >> 15))" % lo.strip("()") or hi == "0":
            return "%sreturn %s;" % (ind, lo)
        return m.group(0)
    body = re.sub(r"(?m)^([ \t]*)dx = ([^;\n]+);\n\s*return ([^;\n]+);$", one, body)
    for lo, hi in longs:
        body = re.sub(r"(?m)^([ \t]*)int %s;\n" % hi, "", body)
        body = re.sub(r"(?m)^([ \t]*)int %s;" % lo, r"\1long %s;" % lo, body)
    m = re.search(r"(?m)^(\w+\()", head)
    if m:
        head = head[:m.start()] + "long\n" + head[m.start():]
    return head + sep + body


def long_pairs(text):
    """two words the code takes as one long - `*(long *)&x`, `LONG(y, x)`, `(x | y)` tested - x the low word: a
    parameter and the next (a, b) or a local and the one above it (vN, vN-2): x declared long, y gone, the
    pair written x. A function whose code widens its value last (cwd: FPINFO longret) returns a long"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    pairs = set(re.findall(r"\*\(long \*\)&(\w+)\b(?!->|\[)", body))   # (a variable's own two words - not a field's)
    for hi, lo in re.findall(r"\bLONG\((\w+), (\w+)\)", body):
        pairs.add(lo)
    for x, y in re.findall(r"\((\w+) \| (\w+)\)", body):
        pairs.add(x)
    hi_of = lambda x: chr(ord(x) + 1) if re.fullmatch(r"[a-y]", x) else "v%d" % (int(x[1:]) - 2) if re.fullmatch(r"v\d+", x) else \
        "g_%x" % (int(x[2:], 16) + 2) if re.fullmatch(r"g_[0-9a-f]+", x) else None   # (a word of the data: the one after)
    # (a pair given a pair: `p->fN = x; p->fN+2 = y;` - a long field given a long - written as one store)
    def pair_store(m):
        ind, p, n, x, p2, n2, y = m.groups()
        byte = any(o in BYTEPTR for o in (("*", int(n)), ("*", int(n2)), (p, int(n)), (p, int(n2))))
        alone = [l for l in body.split("\n") if re.search(r"\b%s->f%s\b" % (p, n), l) and
                 not re.search(r"\b%s->f%s\b" % (p, n2), l) and not re.fullmatch(r"[ \t]*%s->f%s = %s;" % (p, n, x), l)]
        if alone and not any(re.search(r"\b%s->f%s\b" % (p, n2), l) for l in body.split("\n")
                             if not re.fullmatch(r"[ \t]*%s->f%s = %s;" % (p, n2, y), l)):
            return m.group(0)                               # (the low word read alone, the high never: two ints)
        if p2 == p and int(n2) == int(n) + 2 and hi_of(x) == y and not byte:   # (words both: a byte is no long's)
            pairs.add(x)
            LONGINFO.setdefault("fields", set()).add(int(n))
            return "%s%s->f%s = %s;" % (ind, p, n, x)
        return m.group(0)
    body = re.sub(r"(?m)^([ \t]*)\*(\w+) = (\w+);\n(\s*)\2->f2 = (\w+);$",           # (*p the first word: p->f0)
                  lambda m: "%s%s->f0 = %s;\n%s%s->f2 = %s;" % (m.group(1), m.group(2), m.group(3), m.group(4), m.group(2), m.group(5))
                  if hi_of(m.group(3)) == m.group(5) else m.group(0), body)
    body = _line_pairs(body, r"([ \t]*)(\w+)->f(\d+) = (\w+);\n\s*(\w+)->f(\d+) = (\w+);", pair_store)

    def call_store(m):                                      # (a routine's long value, dx:ax, kept in a pair:
        ind, lo, call, hi = m.groups()[:4]                      #  the routine returns a long)
        if hi_of(lo) != hi:
            return m.group(0)
        pairs.add(lo)
        LONGINFO.setdefault("funcs", set()).add(re.match(r"\w+", call).group(0))
        nxt = m.group(5) or ""
        return "%s%s = %s;%s" % (ind, lo, call, nxt.replace("(ax | dx)", lo))   # (the pair tested at once: the long)
    def call_args(m):                                       # (a long passed: its two words pushed - the pair, or
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []   # an int widened)
        out, i = [], 0
        while i < len(args):
            x = args[i]
            y = args[i + 1] if i + 1 < len(args) else None
            if y is not None and re.fullmatch(r"\w+", x) and y == hi_of(x) and x in pairs:   # (a long the code
                out.append(x)                                                            #  shows elsewhere)
                i += 2
                continue
            if y is not None and y in ("(0 - (%s >> 15))" % x, "(0 - ((%s) >> 15))" % x):
                out.append("(long)%s" % x if re.fullmatch(r"\w+", x) else "(long)(%s)" % x)
                i += 2
                continue
            out.append(x)
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))

    bl = body.split("\n")                                   # (the model's temporaries holding an int and its sign
    for k in range(len(bl)):                                #  spread, read once by a call a line or two on: in their
        m = re.fullmatch(r"[ \t]*(t\d+) = (\(0 - \((\w+) >> 15\)\)|(\w+));", bl[k] or "")   # places - the call
        if not m:                                                                        #  passes a long)
            continue
        t, e, x = m.group(1), m.group(2), m.group(3) or m.group(4)
        uses = [j for j in range(k + 1, min(k + 4, len(bl))) if bl[j] is not None and re.search(r"\b%s\b" % t, bl[j])]
        if len(uses) != 1 or any(re.search(r"\b%s\b" % t, bl[j] or "") for j in range(uses[0] + 1, len(bl))) or \
                len(re.findall(r"\b%s\b" % t, bl[uses[0]])) != 1 or not re.search(r"\w\(", bl[uses[0]]) or \
                any(re.match(r"[ \t]*%s = " % x, bl[j] or "") for j in range(k + 1, uses[0])) or \
                any("{" in (bl[j] or "") or "}" in (bl[j] or "") for j in range(k + 1, uses[0] + 1)):   # (one block)
            continue
        bl[uses[0]] = re.sub(r"\b%s\b" % t, lambda _: e, bl[uses[0]])
        bl[k] = None
    body = "\n".join(x for x in bl if x is not None)
    for t in set(re.findall(r"(?m)^[ \t]*int ((?:t\d+, )*t\d+);$", body)):
        for t1 in t.split(", "):
            if not re.search(r"\b%s\b" % t1, body.split("int " + t, 1)[-1].split(";", 1)[-1]):
                body = _drop_decl(body, t1)
    body = re.sub(r"\bLONG\(\(0 - \((\w+) >> 15\)\), \1\)", r"(long)\1", body)   # (an int's sign spread: widened)
    body = re.sub(r"\bLONG\(0, (\w+)\)", r"(long)(unsigned)\1", body)

    def pair_copy(m):                                       # (a pair given a pair: x = y, the high words too)
        ind, x, y, hx, hy = m.groups()
        ghi = lambda g: "g_%x" % (int(g[2:], 16) + 2) if re.fullmatch(r"g_[0-9a-f]+", g) else None
        if hi_of(x) == hx and hi_of(y) == hy:
            pairs.add(x)
            pairs.add(y)
            return "%s%s = %s;" % (ind, x, y)
        if (hi_of(x) == hx or ghi(x) and int(hx[2:] if hx.startswith("g_") else "0", 16) == int(x[2:], 16) + 2) and \
                re.fullmatch(r"g_[0-9a-f]+", y) and re.fullmatch(r"g_[0-9a-f]+", hy) and int(hy[2:], 16) == int(y[2:], 16) + 2:
            pairs.add(x)                                    # (a long in the data copied: one long)
            LONGINFO.setdefault("globals", set()).add("g_%04x" % int(y[2:], 16))
            if x.startswith("g_"):
                LONGINFO["globals"].add("g_%04x" % int(x[2:], 16))
            return "%s%s = %s;" % (ind, x, y)
        return m.group(0)
    body = _line_pairs(body, r"([ \t]*)(\w+) = (\w+);\n\s*(\w+) = (\w+);", pair_copy)

    def const_pair(m):                                      # (a pair given two numbers: one long number)
        ind, x, k1, hx, k2 = m.groups()
        if hi_of(x) != hx:
            return m.group(0)
        pairs.add(x)
        v = (int(k2) & 0xFFFF) << 16 | (int(k1) & 0xFFFF)
        v = v - (1 << 32) if v & 0x80000000 else v
        return "%s%s = %s;" % (ind, x, "%dL" % v if abs(v) > 0x7FFF else str(v))
    body = _line_pairs(body, r"([ \t]*)(\w+) = (-?\d+);\n\s*(\w+) = (-?\d+);", const_pair)

    def long_of(lo, hi):                                    # (two words' expressions, the low and the high, as the
        lo, hi = _strip(lo.strip()), _strip(hi.strip())     #  one long expression they are, or None: names, fields
        def bare(x):                                        #  and data in pairs, ~, & | ^, a number masking a half)
            return x[1:-1] if x.startswith("(") and x.endswith(")") and _balanced(x[1:-1]) else x
        lo, hi = bare(lo), bare(hi)
        fl, fh = re.fullmatch(r"(\w+)->f(\d+)", lo), re.fullmatch(r"(\w+)->f(\d+)", hi)
        if fl and fh and fl.group(1) == fh.group(1) and int(fh.group(2)) == int(fl.group(2)) + 2:
            LONGINFO.setdefault("fields", set()).add(int(fl.group(2)))
            return lo
        if re.fullmatch(r"g_[0-9a-f]+", lo) and re.fullmatch(r"g_[0-9a-f]+", hi) and int(hi[2:], 16) == int(lo[2:], 16) + 2:
            LONGINFO.setdefault("globals", set()).add("g_%04x" % int(lo[2:], 16))
            return lo
        if re.fullmatch(r"\w+", lo) and hi_of(lo) == hi:
            pairs.add(lo)
            return lo
        if lo.startswith("~") and hi.startswith("~"):
            x = long_of(lo[1:], hi[1:])
            return "(~%s)" % x if x else None
        def flat(x, op):                                    # (a & (b & c): a, b, c)
            out = []
            for p_ in _top_split(x, op):
                p_ = p_.strip()
                b_ = bare(p_)
                out += flat(b_, op) if b_ != p_ and len(_top_split(b_, op)) > 1 else [p_]
            return out
        for op in ("&", "|", "^"):
            pl, ph = flat(lo, op), flat(hi, op)
            if len(ph) > 1 and len(ph) == len(pl) + 1 and re.fullmatch(r"\d+|0x[0-9a-f]+", ph[-1].strip()) and op == "&":
                x = long_of(" & ".join(pl) if len(pl) > 1 else pl[0], " & ".join(ph[:-1]) if len(ph) > 2 else ph[0])
                k = int(ph[-1].strip(), 0) & 0xFFFF             # (the high word masked alone: the low word whole)
                return "(%s & 0x%04xffffL)" % (x, k) if x else None
            if len(pl) > 1 and len(pl) == len(ph):
                xs = [long_of(a, b) for a, b in zip(pl, ph)]
                return "(%s)" % (" %s " % op).join(xs) if all(xs) else None
        return None

    def long_test(m):                                       # ((lo) | (hi)) != 0: a long tested
        inner = m.group(1)[1:-1]
        parts = _top_split(inner, "|") if _balanced(inner) else []
        if len(parts) != 2 or not all(_balanced(p) for p in parts):
            return m.group(0)
        x = long_of(parts[0], parts[1])
        return "(%s %s 0)" % (x, m.group(2)) if x else m.group(0)
    body = re.sub(r"\((\((?:[^()]|\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\))*\)) (!=|==) 0\)", long_test, body)

    def lfield(p, n):                                       # (a long field: offset n of what p points to)
        LONGINFO.setdefault("fields", set()).add(int(n))
        return "%s->f%s" % (p, n)
    body = re.sub(r"(?m)^([ \t]*)(\w+)->f(\d+) = \(~\2->f\3\);\n[ \t]*\2->f(\d+) = \(~\2->f\4\);$",
                  lambda m: "%s%s = ~%s;" % (m.group(1), lfield(m.group(2), m.group(3)), lfield(m.group(2), m.group(3)))
                  if int(m.group(4)) == int(m.group(3)) + 2 else m.group(0), body)        # (both words complemented)
    body = re.sub(r"(?m)^([ \t]*)(\w+)->f(\d+) = (\w+);\n[ \t]*\2->f(\d+) = \(0 - \(\4 >> 15\)\);$",
                  lambda m: "%s%s = %s;" % (m.group(1), lfield(m.group(2), m.group(3)), m.group(4))
                  if int(m.group(5)) == int(m.group(3)) + 2 else m.group(0), body)        # (an int widened into it)
    body = re.sub(r"\((\w+(?:->f\d+)*)->f(\d+) \| \1->f(\d+)\)", lambda m: lfield(m.group(1), m.group(2))
                  if int(m.group(3)) == int(m.group(2)) + 2 else m.group(0), body)        # (its words tested together)
    body = re.sub(r"\*\(long \*\)&(\w+(?:->f\d+)*)->f(\d+)\b", lambda m: lfield(m.group(1), m.group(2)), body)   # (read
                                                                                                             #  whole)

    def field_args(m):                                      # (a long field's two words pushed together: the field)
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []
        out, i = [], 0
        while i < len(args):
            a1 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[i])
            a2 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[i + 1]) if i + 1 < len(args) else None
            together = a1 and i + 1 < len(args) and len(re.findall(r"%s\b(?!->)" % re.escape(args[i + 1]), body)) == \
                len(re.findall(r"%s, %s\b" % (re.escape(args[i]), re.escape(args[i + 1])), body))   # (the high word never alone)
            if a1 and a2 and a1.group(1) == a2.group(1) and int(a2.group(2)) == int(a1.group(2)) + 2 and together and \
                    (a1.group(1), int(a1.group(2))) not in BYTEPTR and int(a1.group(2)) in LONGINFO.get("fields", ()):
                out.append(lfield(a1.group(1), a1.group(2)))
                i += 2
                continue
            out.append(args[i])
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", field_args, body)

    def masked_test(m):                                     # (a long and-ed with a long field and tested: the high
        ind, hi, p, n2, rest, lo, n1 = m.groups()           #  words in dx first, then the low words or-ed in)
        if hi_of(lo) != hi or int(n2) != int(n1) + 2:
            return m.group(0)
        pairs.add(lo)
        LONGINFO.setdefault("fields", set()).add(int(n1))
        return rest.replace("((%s & %s->f%s) | dx)" % (lo, p, n1), "(%s & %s->f%s)" % (lo, p, n1))
    body = re.sub(r"(?m)^([ \t]*)dx = (\w+) & (\w+)->f(\d+);\n(.*\(\((\w+) & \3->f(\d+)\) \| dx\).*)$", masked_test, body)

    def gpair_op(m):                                        # (a long in the data and-ed, or-ed, xor-ed with a long:
        ind, g, op, x, g2, op2, x2 = m.groups()             #  a word at a time, the high words after - one op)
        if op != op2 or int(g2[2:], 16) != int(g[2:], 16) + 2:
            return m.group(0)
        lx = long_of(x, x2)
        if not lx:
            return m.group(0)
        LONGINFO.setdefault("globals", set()).add("g_%04x" % int(g[2:], 16))
        return "%s%s = %s %s %s;" % (ind, g, g, op, lx)
    body = _line_pairs(body, r"([ \t]*)(g_[0-9a-f]+) = \2 ([|&^]) ([^;\n]+);\n\s*(g_[0-9a-f]+) = \5 ([|&^]) ([^;\n]+);", gpair_op)

    def sign_pair(m):                                       # (a pair given an int and its sign spread: the int
        ind, x, e, hx, e2 = m.groups()                      #  widened to the long)
        if hi_of(x) != hx or e2.strip() not in (e, "(%s)" % e):
            return m.group(0)
        pairs.add(x)
        if re.fullmatch(r"[A-Za-z_]\w*", e):
            widened[x] = e
        return "%s%s = %s;" % (ind, x, e)
    widened = {}                                            # (a long given an int: its low word that int)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = ([^;\n]+);\n[ \t]*(\w+) = \(0 - \((.+) >> 15\)\);$", sign_pair, body)
    for x, e in widened.items():                            # (its one word tested - the int's, still in a register)
        if not re.search(r"(?m)^[ \t]*(?:%s|%s) = " % (x, e), body.split("%s = %s;" % (x, e), 1)[-1]):
            body = re.sub(r"\(%s (==|!=) 0\)" % x, r"(%s \1 0)" % e, body)

    def zero_pair(m):                                       # (both halves given 0, or -1: the long 0, -1)
        ind, a, v, b = m.groups()
        lo, hi = (b, a) if hi_of(b) == a else (a, b) if hi_of(a) == b else (None, None)
        if not lo:
            return m.group(0)
        pairs.add(lo)
        return "%s%s = %s;" % (ind, lo, v)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = (0|-1);\n\s*(\w+) = \2;$", zero_pair, body)

    def call_long(m):                                       # (a routine's dx:ax read as one long by the next
        LONGINFO.setdefault("funcs", set()).add(re.match(r"\w+", m.group(2)).group(0))   # statement)
        return m.group(3).replace("LONG(dx, ax)", m.group(2))
    body = re.sub(r"(?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n(.*\bLONG\(dx, ax\).*)$", call_long, body)

    def halves(m):                                          # (a long given to a pair a half at a time: one store)
        ind, t, e, lo, hi = m.groups()
        if hi_of(lo) != hi:
            return m.group(0)
        pairs.add(lo)
        return "%s%s = %s;" % (ind, lo, e)
    body = re.sub(r"(?m)^([ \t]*)(t\d+) = (.+);\n\s*(\w+) = LO\(\2\);\n\s*(\w+) = HI\(\2\);$", halves, body)
    body = re.sub(r"(?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = LO\((.+)\);\n[ \t]*\2->f(\d+) = HI\(\4\);$",
                  lambda m: "%s%s = %s;" % (m.group(1), lfield(m.group(2), m.group(3)), m.group(4))
                  if int(m.group(5)) == int(m.group(3)) + 2 else m.group(0), body)   # (a long field given a long)
    body = re.sub(r"(?m)^([ \t]*)()(\w+) = LO\((.+)\);\n\s*(\w+) = HI\(\4\);$",
                  lambda m: halves(type("M", (), {"groups": lambda s: (m.group(1), "", m.group(4), m.group(3), m.group(5)),
                                                  "group": lambda s, i=0: m.group(0)})()), body)
    body = re.sub(r"(?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = ax;\n(?=\s*\w+ = dx;)",
                  lambda m: "%s%s = %s;\n" % (m.group(1), m.group(3), m.group(2)), body)   # (through ax: the same -
                                                                                           #  when dx goes with it)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = dx;(\n.*)?$", call_store, body)
    for g in [x for x in pairs if re.fullmatch(r"g_[0-9a-f]+", x)]:   # (data a pair is made of: a long there)
        LONGINFO.setdefault("globals", set()).add("g_%04x" % int(g[2:], 16))

    def data_args(m):                                       # (two words of the data pushed together, the high one
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []   #  first: a long there -
        out, i = [], 0                                                                     #  the same pushes)
        while i < len(args):
            if i + 1 < len(args) and re.fullmatch(r"g_[0-9a-f]+", args[i]) and args[i + 1] == hi_of(args[i]):
                LONGINFO.setdefault("globals", set()).add("g_%04x" % int(args[i][2:], 16))
                out.append(args[i])
                i += 2
                continue
            out.append(args[i])
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", call_args, body)
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", data_args, body)
    params = re.findall(r"(?m)^\w+\(([\w, ]*)\)", head)
    plist = [p.strip() for p in (params[-1].split(",") if params else []) if p.strip()]
    done = []
    for lo in sorted(pairs):
        hi = hi_of(lo)
        if not hi:
            continue
        isparam = lo in plist and hi in plist
        islocal = re.search(r"(?m)^[ \t]*(?:int|long) (?:\w+, )*%s\b" % lo, body) and (
            re.search(r"(?m)^[ \t]*int (?:\w+, )*%s\b" % hi, body) or re.search(r"(?m)^[ \t]*long %s;" % lo, body))
        if not isparam and not islocal:
            continue
        uns = "unsigned long" if lo in LONGINFO.get("unsigned", ()) else "long"
        body = re.sub(r"\*\(long \*\)&%s\b" % lo, lo, body)
        body = re.sub(r"\bLONG\(%s, %s\)" % (hi, lo), lo, body)
        body = re.sub(r"\(%s \| %s\)|\(%s \| %s\)" % (lo, hi, hi, lo), lo, body)
        body = re.sub(r"\(%s (<|>=) 0\)" % hi, r"(%s \1 0)" % lo, body)   # (the high word's sign: the long's)
        body = re.sub(r"\(%s & (-?\d+)\)" % hi, lambda m: "(%s & 0x%xL)" % (lo, (int(m.group(1)) & 0xFFFF) << 16), body)
        if re.search(r"\b%s\b" % hi, body.split("\n", 1)[-1] if islocal else body):
            pass                                            # (the high word read alone: kept as it is)
        if isparam:
            plist = [p for p in plist if p != hi]
            head = re.sub(r"(?m)^(\w+)\([\w, ]*\)", lambda m: "%s(%s)" % (m.group(1), ", ".join(plist)), head, count=1)
            head = _drop_decl(head, hi)
            head = _redeclare(head, lo, "%s %s" % (uns, lo)) or head
        else:
            body = _drop_decl(body, hi)
            l2 = _redeclare(body, lo, "%s %s" % (uns, lo))
            body = l2 or body
        done.append(lo)
    lrets = [x for x in re.findall(r"(?m)^[ \t]*return (\w+);$", body)
             if re.search(r"(?m)^[ \t]*(?:unsigned )?long (?:\w+, )*%s\b" % x, body)]
    if lrets and not re.search(r"(?m)^[ \t]*return [^;]*\w\(", body):
        LONGINFO["ret"] = True                              # (a long local returned: the function's a long)
    if LONGINFO.get("ret"):
        m = re.search(r"(?m)^\w+\(", head)
        if m and not re.search(r"(?m)^(?:unsigned )?long\s*$", head):
            head = head[:m.start()] + "long\n" + head[m.start():]
        for n in re.findall(r"(?m)^[ \t]*return \w+(?:->f\d+)*->f(\d+);$", body):   # (a field returned whole: a long one)
            LONGINFO.setdefault("fields", set()).add(int(n))
    return head + sep + body


LONGINFO = {}                                               # (the function's longs the code shows: unsigned ones, its value)


def _line_pairs(text, rx, fn):
    """text with each two lines in a row that rx (over both) matches as fn makes them - tried at every line,
    a pair that does not change left for the next (no overlapping match lost)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        if k + 1 < len(lines):
            two = lines[k] + "\n" + lines[k + 1]
            m = re.fullmatch(rx, two)
            if m:
                new = fn(m)
                if new != two:
                    out.append(new)
                    k += 2
                    continue
        out.append(lines[k])
        k += 1
    return "\n".join(out)


def long_ladders(text):
    """two longs compared the compiler's way - the high words, then the low ones - left as ifs of the halves:
        if ((H <= H2)) { if ((H < H2) || (L < L2)) { A } else { L_x: B } } else { goto L_x; }  ->  if ((L < L2)) A else B
    (L, L2 longs - declared so - H, H2 their high words; the same for >= and >)"""
    longs = set(re.findall(r"(?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);", text))
    if not longs:
        return text
    hi = lambda x: chr(ord(x) + 1) if re.fullmatch(r"[a-y]", x) else "v%d" % (int(x[1:]) - 2) if re.fullmatch(r"v\d+", x) else None
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            continue
        m = re.fullmatch(r"\((\w+) ([<>])= (\w+)\)", _strip("(" + p[0] + ")"))
        q = _if_parts(p[1], 0) if p[1] else None
        if not m or not q or q[3] != len(p[1]) or q[2] is None:
            continue
        n = re.fullmatch(r"\(?\((\w+) ([<>]) (\w+)\) \|\| \((\w+) ([<>]=?) (\w+)\)\)?", q[0])
        if not n or (n.group(1), n.group(2), n.group(3)) != (m.group(1), m.group(2), m.group(3)):
            continue
        L, L2 = n.group(4), n.group(6)
        if L not in longs or hi(L) != m.group(1) or (L2 in longs and hi(L2) != m.group(3)) or n.group(5)[0] != m.group(2):
            continue
        dd = lambda ls: [l[4:] if l.startswith("    ") else l for l in ls]
        if not _same_arm(p[2], dd(q[2])):
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        new = ["%sif ((%s %s %s)) {" % (ind, L, n.group(5), L2)] + dd(q[1]) + [ind + "} else {"] + dd(q[2]) + [ind + "}"]
        cand = lines[:k] + new + lines[p[3]:]
        whole = "\n".join(cand)
        cand = [l for l in cand if not (re.fullmatch(r"\s*L_\w+:", l) and not re.search(r"\bgoto %s;" % l.strip()[:-1], whole))]
        return long_ladders("\n".join(cand))
    return text


def _drop_decl(t, x):
    """t without x in its `int a, x, b;` declaration (the line gone if x was all of it)"""
    def one(m):
        rest = (m.group(2) + m.group(3).lstrip(", ")).strip(", ")
        return "%sint %s;\n" % (m.group(1), rest) if rest else ""
    return re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);\n" % re.escape(x), one, t, count=1)


def local_pointers(text):
    """a register given the place of a local (`r = bp - N`, the model's lea of [bp-N]): a char pointer into it,
    `r = (char *)&vN;`, its fields the bytes it reaches (`r->fK` is r[K])"""
    text = re.sub(r"(?m)^([ \t]*)(\w+) = bp;\n\s*\2 = \2 - (\d+);$", r"\1\2 = bp - \3;", text)   # (mov r,bp; sub r,N)
    for reg, n in re.findall(r"(?m)^[ \t]*(\w+) = bp - (\d+);", text):
        buf = next(((k_, c_) for k_, c_ in re.findall(r"(?m)^[ \t]*char _p(\d+)\[(\d+)\];", text) if int(k_) + int(c_) == int(n)), None)
        if buf:                                             # (the start of a filler the frame keeps: a buffer there)
            text = re.sub(r"(?m)^([ \t]*)char _p%s\[%s\];" % buf, r"\1char v%s[%s];" % (n, buf[1]), text)
            text = re.sub(r"(?m)^([ \t]*)%s = bp - %s;" % (reg, n), r"\1%s = v%s;" % (reg, n), text)
            text = re.sub(r"(?m)^([ \t]*)(register )?int %s;" % reg, r"\1\2char *%s;" % reg, text)
            continue
        if not re.search(r"(?m)^[ \t]*(?:int|long) v%s;" % n, text):
            continue
        text = re.sub(r"(?m)^([ \t]*)%s = bp - %s;" % (reg, n), r"\1%s = (char *)&v%s;" % (reg, n), text)
        text = re.sub(r"\b%s->f(\d+)\b" % reg, r"%s[\1]" % reg, text)
        text = re.sub(r"(?m)^([ \t]*)(register )?int %s;" % reg, r"\1\2char *%s;" % reg, text)
    return text


def void_variant(text):
    """the draft without its last `return X;` (a function of no value whose last call's result the model kept)"""
    m = list(re.finditer(r"(?m)^([ \t]*)return (.+);\s*$", text))
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
        pp = re.search(r"\*\(\*%s\)|\*\*%s\b" % (p, p), body) and not re.search(r"\b%s->" % p, body)   # (read through
                                                                                                    #  twice: a pointer to pointers)
        rdecl.append("register %s%s;" % ("char **" if pp else "char *" if ptr else "int ", p))
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
    pnames = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);", head))     # (pointer parameters)
    for r in used:
        ptr = re.search(r"\*\s*%s\b|\b%s\s*\[" % (r, r), body) or \
            any(re.search(r"(?m)^[ \t]*%s = %s;" % (r, p), body) for p in pnames)       # (given a pointer's value)
        pp = re.search(r"\*\(\*%s\)|\*\*%s\b" % (r, r), body) and not re.search(r"\b%s->" % r, body)
        decls.append("    register %s%s;" % ("char **" if pp else "char *" if ptr else "int ", "r_" + r))
        body = re.sub(r"\b%s\b" % r, "r_" + r, body)
    return head + "{\n" + "\n".join(decls) + body


def register_slots(text):
    """the model's trailing frame filler less two bytes for each register local (a variant: MS C 3.00 gives some
    register locals a slot in the frame - which ones the compiler alone says)"""
    if not REGSLOTS[0]:
        return None
    n_regs = len(re.findall(r"(?m)^[ \t]*register [^;(]*\br_\w+;$", text))
    fl = list(re.finditer(r"(?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n", text))
    nxt = re.match(r"[ \t]*(\S+)", text[fl[-1].end():]) if fl else None
    if not n_regs or not fl or not nxt or re.match(r"(?:register|int|char|long|unsigned|struct|short)\b", nxt.group(1)):
        return None
    m = fl[-1]                                              # (the trailing one, after the last local)
    n = int(m.group(3)) - 2 * n_regs
    return text[:m.start()] + ("%schar %s[%d];\n" % (m.group(1), m.group(2), n) if n > 0 else "") + text[m.end():]


REGSLOTS = [False]                                          # (the toolchain gives a register local a frame slot)


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
        out.append(_string_decls(re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, text)))
    if len(cands) > 1:
        t = text
        for v, lit in cands:
            t = re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, t)
        out.append(_string_decls(t))
    return out


def _string_decls(text):
    """data given a string (`g = "..";`, `g[i] = "..";`) declared int: a char * (an array of them)"""
    for g, ix in re.findall(r"\b(g_[0-9a-f]{4})(\[[^\]]*\])? = \"", text):
        text = re.sub(r"(?m)^extern int ((?:\w+(?:\[\])?, )*)%s(\[\])?((?:, \w+(?:\[\])?)*);$" % g, lambda d: (
            ("extern int %s;\n" % (d.group(1) + d.group(3).lstrip(", ")).strip(", ") if (d.group(1) + d.group(3)).strip(", ") else "") +
            "extern char *%s%s;" % (g, d.group(2) or "")), text)
    return text


def localize(prj, text, at, names):
    """the model's text with the engine's names and the declarations it needs; None if a machine register is left"""
    def call(m):
        rel = int(m.group(1), 16)
        if rel >= 0x80000000:
            rel -= 1 << 32
        t = (at + rel) & 0xFFFFF if at + rel >= 0 else at + rel + 0x10000   # (a near call wraps in its 64K: the segment's)
        if t not in names:                                  # (the 64K it wraps in: where a routine is)
            t = next((x for x in (t - 0x10000, t + 0x10000) if x in names), t)
        return (names.get(t) or "f%05x" % t) + "("
    longf = {re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, f + "(")[:-1] for f in LONGINFO.get("funcs", ())}   # (routines of a long value)
    text = re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, text)
    text = re.sub(r"\bg_([0-9a-f]{1,4})\b", lambda m: "g_%04x" % int(m.group(1), 16), text)
    text = re.sub(r"(?m)^([ \t]*)(L_\w+:)\n(?=[ \t]*\})", r"\1\2\n\1;\n", text)   # (a label last in its block: on an
    macros = prj.toolchain().p.get("macros")                                         #  empty statement)
    text = re.sub(r"\*\((\w+)(\+\+|--)\)", r"*\1\2", text)   # (*(p++): *p++ - the same, and mendable)
    text = fp_conversions(text, prj.toolchain().p.get("fp_conversions"))
    text = long_helpers(text, prj.toolchain().p.get("long_helpers"))
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
    head = text.split("(", 1)[0].split()[-1] if text.split("(", 1)[0].split() else ""   # (its own name: after a type)
    funcs = sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body)) - {head, "if", "while", "for", "switch", "return", "sizeof"} - macnames)
    decl = []
    fptr = [d for d in data if re.search(r"\(\s*\*\s*%s\s*\)\s*\(" % d, body)]     # (data called through)
    for d in fptr:                                          # (a number given it that is a function's place: it)
        text = re.sub(r"(?m)^(\s*%s = )(\d+);" % d, lambda m: m.group(1) + (names.get(int(m.group(2))) or
                                                                             "f%05x" % int(m.group(2))) + ";", text)
        body = text.split("{", 1)[1] if "{" in text else text
    funcs = sorted(set(funcs) | {w for d in fptr for w in re.findall(r"(?m)^[ \t]*%s = ([A-Za-z_]\w*);" % d, body)})
    for a_ in [d for d in data if ARRAYS.get(d) == "int"]:  # (an array of words named whole, and the words just
        base = int(a_[2:], 16)                              #  after it by their own places: its elements)
        near = [d for d in data if d != a_ and d not in ARRAYS and 0 < int(d[2:], 16) - base < 0x40 and (int(d[2:], 16) - base) % 2 == 0]
        if re.search(r"\b%s\b(?!\[)" % a_, body) or near:
            hd, sp_, bd = text.partition("{")
            bd = re.sub(r"\b%s\b(?!\[)" % a_, "%s[0]" % a_, bd)
            for d in near:
                bd = re.sub(r"\b%s\b" % d, "%s[%d]" % (a_, (int(d[2:], 16) - base) // 2), bd)
            text = hd + sp_ + bd
            body = bd
            data = [d for d in data if d not in near]
    arrs = [d for d in data if d in ARRAYS and d not in fptr]   # (indexed in place: an array, not a pointer)
    pvars = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?(?:unsigned\s+)?(?:char|int)\s*\*\s*(\w+);", text))   # (pointers the draft declares)
    sptrs = dict((n, t_) for t_, n in re.findall(r"(?m)^[ \t]*(?:register\s+)?(struct \w+)\s*\*\s*(\w+);", text))
    pvars |= set(sptrs)
    flows = lambda d: any(re.search(r"(?m)^[ \t]*%s = %s\b|^\s*%s = %s\b" % (p, d, d, p), body) for p in pvars)
    for d in data:                                          # (data given a struct pointer's value: a pointer to it)
        src_ = next((p for p in sptrs if re.search(r"(?m)^[ \t]*(?:\w+ = )*%s = (?:\w+ = )*%s;$" % (d, p), body)), None)
        if src_ and d not in STRUCTPTR:
            STRUCTPTR[d] = sptrs[src_].split()[1]
    ptrs = [d for d in data if d not in fptr and d not in arrs and (re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:->|\[)" % (d, d), body)
                                                                  or flows(d))]   # (or given a pointer's value, or giving one)
    ints = [d for d in data if d not in ptrs and d not in fptr and d not in arrs]
    for nb, x_, y_ in re.findall(r"\*\(struct _b(\d+) \*\)(\w+) = \*\(struct _b\1 \*\)(\w+);", text):
        for v_ in (x_, y_):                                 # (the pointers copied through: pointers to the block)
            t_ = _redeclare(text, v_, "struct _b%s *%s" % (nb, v_))
            if t_ and not re.search(r"\b%s\s*(?:->|\[)" % v_, text):
                text = t_.replace("*(struct _b%s *)%s" % (nb, v_), "*%s" % v_)
    for nb in sorted(set(re.findall(r"\bstruct _b(\d+) \*", text)), key=int):   # (a block of bytes copied whole)
        decl.insert(0, "struct _b%s { char _c[%s]; };" % (nb, nb))
    if arrs:
        # (arrays of structs of one size whose places lie within one element: one array, its fields at the
        #  places' distances from the first)
        sa = sorted((int(d[2:], 16), d) for d in arrs if d in STRUCTARR)
        merged = {}
        for b, d in sa:
            base = next((x for x in merged if STRUCTARR[merged[x][0]][0] == STRUCTARR[d][0] and 0 < b - x < STRUCTARR[d][0]), None)
            if base is None:
                merged[b] = [d]
            else:
                merged[base].append(d)
        for base, ds in merged.items():
            if len(ds) < 2:
                continue
            bn = ds[0]
            for d in ds[1:]:
                off = int(d[2:], 16) - base
                text = re.sub(r"\b%s\[([^\[\]]+)\]\.f0\b" % d, lambda m, o=off: "%s[%s].f%d" % (bn, m.group(1), o), text)
                arrs = [x for x in arrs if x != d]
            STRUCTARR[bn] = (STRUCTARR[bn][0], [(int(d[2:], 16) - base, STRUCTARR[d][1] if not str(STRUCTARR[d][1]).startswith("struct") else "int") for d in ds])
        for d in arrs:                                      # (an array of structs: its element laid out - the field
            if d in STRUCTARR:                              #  the code reads first, the rest of its size after)
                k, ty = STRUCTARR[d]
                fl, pos = [], 0
                for off, t_ in (ty if isinstance(ty, list) else [(0, ty)]):
                    if off > pos:
                        fl.append("char _%d[%d];" % (pos, off - pos))
                    fl.append("%s f%d;" % (t_, off))
                    pos = off + (1 if t_ == "char" else 2)
                if k > pos:
                    fl.append("char _%d[%d];" % (pos, k - pos))
                decl.append("struct s_%s { %s };" % (d, " ".join(fl)))
        decl += ["extern %s %s[];" % (ARRAYS[d], d) for d in arrs]
    if fptr:
        decl.append("extern int %s;" % ", ".join("(*%s)()" % d for d in fptr))
    streams = [d for d in data if d in FILEPTR]             # (a stream a library macro reads)
    ptrs = [d for d in ptrs if d not in streams]
    ints = [d for d in ints if d not in streams]
    decl += ["extern %s *%s;" % (FILEPTR[d], d) for d in streams]
    head2, sep2, body2 = text.partition("{")
    for p in [n for n in FILEPTR if not n.startswith("#") and n not in data]:   # (a parameter that is one)
        head2 = re.sub(r"(?m)^([ \t]*)(?:register )?(?:int|char \*|struct \w+ \*) ?%s;$" % p, r"\1%s *%s;" % (FILEPTR[p], p), head2)
        head2 = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$" % p,
                       lambda m: ("%sint %s;\n" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", ")) if (m.group(2) + m.group(3)).strip(", ") else "") +
                       "%s%s *%s;" % (m.group(1), FILEPTR[p], p), head2)
    text = head2 + sep2 + body2
    incs = list(dict.fromkeys(FILEPTR[k] for k in ("#include", "#include2") if FILEPTR.get(k)))
    for h in reversed(incs):
        decl.insert(0, "#include <%s>" % h)
    if ptrs:                                                # (data the draft goes through: pointers)
        sp_ = {("g_%04x" % int(k[2:], 16) if re.fullmatch(r"g_[0-9a-f]{1,4}", k) else k): v for k, v in STRUCTPTR.items()}
        for d in ptrs:                                      # (a word read through it, never a byte: an int *)
            word = any((r + "w", 0) in BYTEPTR for r in ("bx", "si", "di")) and \
                not any((r + "*", 0) in BYTEPTR for r in ("bx", "si", "di")) and re.search(r"\*%s\b" % d, body)
            decl.append("extern %s *%s;" % ("struct " + sp_[d] if d in sp_ else "char *" if PTRSIZE.get(d) == 2 and re.search(r"\*\+\+%s\b" % d, body)
                                            else "int" if word else "char", d))
    uch = [d for d in ints if d in UCHAR]
    chs = [d for d in ints if d in BYTEG and d not in UCHAR]
    ints = [d for d in ints if d not in UCHAR and d not in BYTEG]
    if uch:
        decl.append("extern unsigned char %s;" % ", ".join(uch))
    if chs:
        decl.append("extern char %s;" % ", ".join(chs))
    lngs = [d for d in ints if d in LONGINFO.get("globals", ())]   # (a long in the data: its two words one)
    ints = [d for d in ints if d not in lngs]
    if lngs:
        decl.append("extern long %s;" % ", ".join(lngs))
    if ints:
        decl.append("extern int %s;" % ", ".join(ints))
    for h in incs:                                          # (what an included header declares: its)
        funcs = [f for f in funcs if f not in header_names(prj, h)]
    if [f for f in funcs if f in longf]:
        decl.append("extern long %s;" % ", ".join("%s()" % f for f in funcs if f in longf))
    funcs = [f for f in funcs if f not in longf]
    if funcs:
        decl.append("extern %s;" % ", ".join("%s()" % f for f in funcs))
    text = fp_types(text)
    # (a byte through a char pointer compared with 128..255: the compiler compares bytes only of an unsigned char)
    for p in set(re.findall(r"\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)? [!=]= (?:1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b", text)):
        text = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % p, r"\1\2unsigned char *%s;" % p, text)
    for p_, k_, e_, n_ in re.findall(r"(?m)^[ \t]*(\w+) = \((\d+) \* ([^()]+?)\) \+ (\d+);$", text):
        tag = re.search(r"(?m)^[ \t]*(?:register )?struct (\w+) \*%s;$" % p_, text)   # (a struct pointer given
        dm = re.search(r"(?m)^struct %s \{([^}]*)\};$" % tag.group(1), text) if tag else None   # K * E + N: the
        if dm and int(n_) >= 0x100:                                                       # address of an element
            k, n = int(k_), int(n_)                                                       # of an array there)
            fl = re.findall(r"(char|int|long|\*) ?_?f?(\d+)(?:\[(\d+)\])?;", dm.group(1))
            size = max((int(o) + (int(c) if c else 4 if t == "long" else 1 if t == "char" else 2) for t, o, c in fl), default=0)
            if size <= k:
                body_ = dm.group(1) + (" char _%d[%d];" % (size, k - size) if size < k else "")
                name = "g_%04x" % n
                text = text.replace(dm.group(0), "struct %s {%s };\nextern struct %s %s[];" % (tag.group(1), body_.rstrip(), tag.group(1), name))
                text = re.sub(r"(?m)^([ \t]*)%s = \(%s \* %s\) \+ %s;$" % (p_, k_, re.escape(e_), n_), r"\1%s = &%s[%s];" % (p_, name, e_), text)
    for ty, x in set(re.findall(r"\*\((char|int|unsigned char) \*\)(\w+)\b", text)):   # (only ever read through one
        others = re.findall(r"\*\((?:char|int|unsigned char) \*\)%s\b" % x, text)       #  cast: a pointer to it)
        if all(o == "*(%s *)%s" % (ty, x) for o in others) and not re.search(r"(?<![)\w])\*\s*%s\b|\b%s\s*(?:->|\[)" % (x, x), text):
            t2 = _redeclare(text, x, "%s *%s" % (ty, x)) if re.search(r"(?m)^[ \t]*(?:register )?int (?:\w+, )*%s\b" % x, text) else None
            if t2:
                text = t2.replace("*(%s *)%s" % (ty, x), "*%s" % x)
    out = ("\n".join(decl) + "\n\n" if decl else "") + text
    dn = _data_names(prj)                                   # (data the program has names for: those, by place - a
    if dn:                                                  #  name of the form g_XXXX may be another place's)
        out = re.sub(r"\bg_([0-9a-f]{4})\b", lambda m: dn.get(int(m.group(1), 16), m.group(0)), out)
    return out, left


def _data_names(prj):
    """{data offset: name} of the data SYMBOLS names (where a name differs from the place's own g_XXXX)"""
    if getattr(prj, "_dnames", None) is None:
        prj._dnames = {a: n.lstrip("_") for n, (t, a) in prj.symbols().items()
                       if t == "D" and ":" not in n and re.fullmatch(r"_?\w+", n) and n.lstrip("_") != "g_%04x" % a}
    return prj._dnames


def long_helpers(text, helpers):
    """the library's long arithmetic (the profile's "long_helpers": routine -> C operator) as the operator:
    lmul(X, Y) is (X * Y); an assigning one, almul(&X, Y), is X *= Y"""
    if not helpers:
        return text
    rx = re.compile(r"\b(%s)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)" % "|".join(re.escape(h) for h in helpers))

    uns = set()

    def one(m):
        op = helpers[m.group(1)]
        args = [x.strip() for x in _top_commas(m.group(2))]
        if len(args) != 2:
            return m.group(0)
        if re.match(r"a?ul", m.group(1)) and re.fullmatch(r"&?\w+", args[0]):   # (an unsigned one's operand:
            uns.add(args[0].lstrip("&"))                                         #  an unsigned long)
        if op.endswith("=") and op not in ("==", "<=", ">=", "!="):
            if not args[0].startswith("&"):
                return m.group(0)
            return "(%s %s %s)" % (args[0][1:], op, args[1])
        v = "(%s %s %s)" % (args[0], op, args[1])
        before, after = m.string[:m.start()].rstrip(), m.string[m.end():].lstrip()
        if re.search(r"\w\($|,$", before) and after[:1] in (")", ","):   # (an argument pushed as one word: the
            return "(int)%s" % v                                            #  long's low word - an int)
        return v
    t = rx.sub(one, text)
    for x in uns:
        t = re.sub(r"(?m)^([ \t]*)(register )?long %s;$" % x, r"\1\2unsigned long %s;" % x, t)
    t = re.sub(r"(?m)^([ \t]*)\((.+ [*/%<>+-]{1,2}= .+)\);$", r"\1\2;", t)   # (a statement of its own: no parens)
    # (the long it leaves in dx:ax tested at once - `ax = (ax | dx)`: the assignment's value)
    t2 = re.sub(r"(?m)^([ \t]*)(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n\s*ax = \(ax \| dx\);$", r"\1ax = (\2);", t)
    t2 = re.sub(r"(?m)^[ \t]*(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n(\s*)\} while \(\(\(ax \| dx\) != 0\)\);$",
                r"\2} while (((\1) != 0));", t2)              # (a do's test of it)
    return fold_registers(t2) if t2 != t else t


def fp_conversions(text, routines):
    """the library's conversion of the 8087's top to an integer (the profile's "fp_conversions": its names - ftol)
    is C's own conversion in an assignment: `X = ftol(E);` is `X = E;`, and `p->f0 = ftol(E); p->f2 = dx;` (the
    high half too) `*p = E;` of a long *p"""
    if not routines:
        return text
    rx = "|".join(re.escape(r) for r in routines)
    arg = r"((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)"
    def long_(m):
        FPINFO.setdefault("types", {})["*" + m.group(2)] = "long"
        return "%s*%s = %s;" % (m.group(1), m.group(2), m.group(3))
    text = re.sub(r"(?m)^([ \t]*)(\w+)->f0 = (?:%s)\(%s\);\n\s*\2->f2 = dx;$" % (rx, arg), lambda m: long_(m) if True else "", text)
    text = re.sub(r"(?m)^([ \t]*)(.+?) = (?:%s)\(%s\);$" % (rx, arg), r"\1\2 = \3;", text)
    return text


def _redeclare(t, x, new):
    """t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None"""
    d = re.search(r"(?m)^([ \t]*)(register\s+)?(struct \w+|\w+)\s*\*?\s*((?:\w+, )*)%s((?:, \w+)*);$" % re.escape(x), t)
    if not d or d.group(3) in ("return", "goto", "extern"):
        return None
    rest = (d.group(4) + d.group(5).lstrip(", ")).strip(", ")
    rep = ("%s%s%s %s;\n" % (d.group(1), d.group(2) or "", d.group(3), rest) if rest else "") + \
        "%s%s%s;" % (d.group(1), d.group(2) or "", new)
    return t[:d.start()] + rep + t[d.end():]


def fp_types(text):
    """the types the 8087's operands showed (FPINFO): `*p` a float/double/long - p a pointer to one; vN or a
    parameter one itself; data declared so; a function giving a double back declared double"""
    types = FPINFO.get("types") or {}
    if not types and not FPINFO.get("ret"):
        return text
    head, sep, body = split_body(text)
    if not sep:
        return text
    pre, fn = "", head
    m = re.search(r"(?m)^(?:\w+\s*\n)?\w+\(", head)         # (the declarations before the function: data's)
    if m:
        pre, fn = head[:m.start()], head[m.start():]
    for nm, ty in types.items():
        m = re.fullmatch(r"\*\(?(\w+)\)?", nm)
        x, ptr = (m.group(1), True) if m else (nm, False)
        if not re.fullmatch(r"\w+", x):
            continue
        new = "%s %s%s" % (ty, "*" if ptr else "", x)
        f2 = _redeclare(fn, x, new)
        if f2:
            fn = f2
            continue
        lines = body.split("\n")                            # (a local: among the declarations at the top)
        for k, l in enumerate(lines):
            if l.strip() and not re.match(r"\s*(?:register\s+)?(?:int|char|long|unsigned|short|struct|float|double)\b", l):
                break
            l2 = _redeclare(l, x, new) if l.strip() else None
            if l2:
                lines[k] = l2
                break
        else:
            lines = None
        if lines is not None:
            body = "\n".join(lines)
        if x.startswith("g_"):
            pre = re.sub(r"(?m)^extern int ((?:\w+, )*)%s((?:, \w+)*);$" % x, lambda d: (
                ("extern int %s;\n" % (d.group(1) + d.group(2).lstrip(", ")).strip(", ") if (d.group(1) + d.group(2)).strip(", ") else "") +
                "extern %s;" % new), pre)
    if FPINFO.get("ret"):
        m = re.search(r"(?m)^\w+\(", fn)
        if m and not re.search(r"(?m)^double\s*$", fn):
            fn = fn[:m.start()] + FPINFO["ret"] + "\n" + fn[m.start():]
    return pre + fn + sep + body


ERRLINE = re.compile(r"\((\d+)\) : (?:error \w+: )?(.*)$", re.M)


def _declare_pointer(src, x, ty):
    """the source with x (an int parameter or local) declared a pointer to ty, or None"""
    head, sep, body = split_body(src)
    for part in ("head", "body"):
        t = head if part == "head" else body
        m = re.search(r"(?m)^([ \t]*)(register\s+)?int ((?:\w+, )*)%s((?:, \w+)*);$" % re.escape(x), t)
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
    m = re.search(r"(?<![\w)\]])\*(?!\()\s*(?:\+\+|--)?([A-Za-z_]\w*)", l) if "illegal indirection" in msg else \
        re.search(r"\b([A-Za-z_]\w*)\[", l) if "subscript on non-array" in msg else None
    if m:
        for ty in ("char", "int") if (m.group(1), 0) in BYTEPTR else ("int", "char"):   # (read a byte at a time: char)
            d = _declare_pointer(src, m.group(1), ty)
            if d:
                out.append(d)
    if m and "illegal indirection" in msg and (m.group(1), 0) not in BYTEPTR:   # (or a word through a cast in place:
        new = re.sub(r"(?<![\w)\]])\*(?!\()(\s*%s)\b" % re.escape(m.group(1)), r"*(int *)\1", l, count=1)   # the
        if new != l:                                                                                   # variable
            lines2 = list(lines)                                                                       # stays an int)
            lines2[n - 1] = new
            out.append("\n".join(lines2))
    if "bad left operand" in msg or "bad right operand" in msg:   # (a pointer in arithmetic only ints take: an int)
        for x in dict.fromkeys(re.findall(r"\b([A-Za-z_]\w*)\b", l)):
            d = re.sub(r"(?m)^([ \t]*)(register )?(?:unsigned )?char \*%s;$" % x, r"\1\2int %s;" % x, src)
            if d != src:
                out.append(d)
    if "2 pointers" in msg:                                 # (two pointers added: one of them an int - each way)
        for x in dict.fromkeys(re.findall(r"\b([A-Za-z_]\w*)\b", l)):
            d = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % x, r"\1\2int %s;" % x, src)
            if d != src:
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
        new = re.sub(r"((?:\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)|[A-Za-z_]\w*(?:\[[^\]]*\])?)(?:(?:\.|->)f\d+)*)->f%s\b" % f,
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
            _prove.spent = getattr(_prove, "spent", 0) + 1
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
    errs_ = [l for l in last if "warning" not in l]       # (the error, not a warning before it)
    last = errs_[:1] or last
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
    texts = list(texts)
    for text, regparam in list(texts):                      # (a register loaded from a parameter and then given
        given = {r for r in (regparam or {}) if re.search(r"(?m)^[ \t]*%s = " % r, text)}   # a new value: maybe a
        for g_ in sorted(given):                                                             # local the parameter
            texts.append((text, {r: i for r, i in regparam.items() if r != g_}))             # started - the compiler says)
        if len(given) > 1:
            texts.append((text, {r: i for r, i in regparam.items() if r not in given}))
    for text, regparam in texts:
        src, v = prove_best(prj, tc, inv, text, a, ln, names, regparam)
        if v == "identical":
            return src, v
        rank = _rank(v)
        if best is None or rank < best[0]:
            best = (rank, src, v)
    return best[1], best[2]


MAXCANDS = 48
FUNC_BUDGET = 150                                           # (compiles a function's candidates may take in all)
BYTEPTR = set()                                             # (the byte-wide places the code reads: (pointer, offset))


def _probes(code):
    """True: the prologue calls the stack probe (`mov ax,N` or `xor ax,ax`, then a call, after `mov bp,sp`);
    False: a frame without one (`sub sp,N`, or none and no call there); None: not a frame the rule knows"""
    from codebreaker.exact import x86
    ins = list(x86.disasm(bytes(code), 0, min(len(code), 16), False))
    if len(ins) < 2 or ins[0].mnemonic != "push" or ins[0].op_str != "bp" or ins[1].op_str != "bp, sp":
        return None
    if len(ins) > 3 and (ins[2].op_str.startswith("ax, ") and ins[2].mnemonic in ("mov", "xor")) and ins[3].mnemonic == "call":
        return True
    return False


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
    fpsw, fsw, gs = tc.p.get("fp_switch"), tc.p.get("file_switches"), tc.p.get("probe_switch")
    sw = prj.switches if prj.switches is not None else tc.p.get("switches", "")
    want = sw.split()
    if FPINFO.get("fp") and fpsw and fpsw not in want:      # (code of the 8087's: compiled for it - the profile's
        want.append(fpsw)                                   #  "fp_switch")
    if gs:                                                  # (a stack probe in the prologue, or none: the switch
        probe = _probes(bytes(prj.image[a:a + ln]))         #  that leaves it out - "probe_switch" - as it shows)
        if probe and gs in want:
            want.remove(gs)
        elif probe is False and gs not in want:
            want.append(gs)
    if fsw and want != sw.split():
        cands = [("/* %s: %s */\n" % (fsw, " ".join(want)) + src, left) for src, left in cands]
    best = None
    _prove.spent = 0
    for src, left in cands:
        if left:
            v = "registers left: %s" % " ".join(left)
        elif _prove.spent >= FUNC_BUDGET and best is not None:
            break                                           # (compiled enough for one function: the best so far)
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
    if best and best[2].startswith("differs"):              # (the best that compiles, rewritten a way at a time:
        return refine(prj, tc, a, ln, best)                 #  each kept that brings it nearer - combinations
    return best[1], best[2]                                 #  the variants alone do not make)


def _place_variants(prj, src):
    """the numbers a pointer is given or compared with in a localized source as the places in the data they are"""
    out = []
    for name, t in address_variants(src, 0x10000, most=3):
        if not re.search(r"(?m)^extern .*\b%s\b" % name, t):
            t = "extern char %s[];\n" % name + t
        out.append(t)
    return out


REFINERS = [lambda prj, s: [assign_in_args(s)], lambda prj, s: [return_sources(s)], lambda prj, s: [compound_assigns(s)],
            lambda prj, s: [chain_assigns(s)], lambda prj, s: [for_loops(s)], lambda prj, s: [store_increments(s)],
            lambda prj, s: [pre_steps(s)], lambda prj, s: [pre_increments(s)], lambda prj, s: [assign_in_tests(s)],
            lambda prj, s: [and_returns(s)], lambda prj, s: [register_temps(s)], lambda prj, s: [void_variant(s)],
            lambda prj, s: pointer_widths(s), _place_variants, lambda prj, s: string_variants(prj, s),
            lambda prj, s: [split_ternaries(s)], lambda prj, s: [default_last(s)], lambda prj, s: [push_tails(s)],
            lambda prj, s: [unsigned_registers(s)], lambda prj, s: [unmasked(s)], lambda prj, s: [no_default(s)],
            lambda prj, s: [empty_cases_first(s)], lambda prj, s: [register_slots(s)], lambda prj, s: [register_copies(s)],
            lambda prj, s: [lead_filler(s)], lambda prj, s: [scaled_places(s)], lambda prj, s: [shared_tails(s)],
            lambda prj, s: [guard_returns(s)], lambda prj, s: [default_after(s)],
            lambda prj, s: [re.sub(r"\b0, 0\)", "0L)", s)]]     # (two zero words pushed: a long 0)


def refine(prj, tc, a, ln, best, rounds=2, budget=40):
    """-> (source, verdict): best (rank, source, verdict) rewritten by REFINERS one at a time, each rewriting
    kept when the compiler finds the result nearer - a greedy climb, bounded"""
    rank, src, v = best
    spent = 0
    for _ in range(rounds):
        moved = False
        for f in REFINERS:
            try:
                alts = [x for x in f(prj, src) if x and x != src]
            except Exception:
                continue
            for alt in alts:
                if spent >= budget:
                    return src, v
                spent += 1
                v2 = _prove(prj, tc, alt, a, ln, tries=2, budget=3)
                if v2 == "identical":
                    return _prove.src, v2
                r2 = _rank(v2)
                if r2 < rank:
                    rank, src, v, moved = r2, _prove.src, v2, True
                    break
        if not moved:
            break
    return src, v


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
        m = re.search(r"cs:\[bx ([+-]) (0x[0-9a-f]+)\]", ins[k].op_str)
        n = next((int(x.group(1), 0) for i in reversed(ins[max(0, k - 8):k])
                  for x in [re.fullmatch(r"\w+, (0x[0-9a-f]+|\d+)", i.op_str)] if i.mnemonic == "cmp" and x), None)
        if not m or n is None:
            return None
        t = ((int(m.group(2), 16) if m.group(1) == "+" else -int(m.group(2), 16)) & 0xFFFF) - hook.base   # (above 32K:
                                                                                                         #  printed less)
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
    gaps = []                                               # ((line, bytes): a filler before a local vN the
    for k, l in enumerate(lines[1:], 1):                    #  declarations above would put nearer bp than N)
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
        v = re.fullmatch(r"v(\d+)", name)
        if v and not star and not cnt and ty.split()[-1] == "long" and size + n < int(v.group(1)):
            gaps.append((k, int(v.group(1)) - size - n))    # (a long the pair made of vN and the word above it:
            size = int(v.group(1)) - n                      #  where vN was)
        size += n + (n & 1)
    for k, g in reversed(gaps):
        lines.insert(k, "    char _pg%d[%d];" % (k, g))
        last += 1
    if gaps and fs > size:
        lines.insert(last + 1, "    char _pf[%d];" % (fs - size))
        return head + sep + "\n".join(lines)
    if fs <= size or re.search(r"(?m)^\s+(?:int|char|long|short)\s+v\d+", body):
        return text if not gaps else head + sep + "\n".join(lines)   # (the model's autos: the frame model lays those out)
    lines.insert(last + 1, "    char _pf[%d];" % (fs - size))
    return head + sep + "\n".join(lines)


def fp_native(code):
    """the code with the floating-point emulator's calls (int 34h..3Bh: an 8087 instruction, its first byte
    D8h + n; int 3Ch: one with a segment; int 3Dh: fwait) written as the 8087's own - each as long as it was
    (a wait before it), so the model reads them as the instructions they stand for"""
    from codebreaker.exact import x86
    b = bytearray(code)
    if not re.search(rb"\xcd[\x34-\x3d]", bytes(code)):
        return bytes(code)
    for i in x86.disasm(bytes(code), 0, len(code), True):
        raw = bytes(i.bytes)
        if raw[:1] != b"\xcd" or len(raw) < 2 or not 0x34 <= raw[1] <= 0x3D:
            continue
        a = i.address
        if raw[1] <= 0x3B:
            b[a:a + 2] = bytes([0x9B, 0xD8 + raw[1] - 0x34])
        elif raw[1] == 0x3D:
            b[a:a + 2] = b"\x9b\x90"
        elif len(raw) >= 3:
            seg = {0x00: 0x26, 0x40: 0x2E, 0x80: 0x36, 0xC0: 0x3E}[raw[2] & 0xC0]
            b[a:a + 3] = bytes([0x9B, seg, 0xD8 | (raw[2] & 7)])
    return bytes(b)


def inline_goto_arms(lines):
    """an arm that is only `goto L;`, L the start of statements to the end of their block: those statements in
    its place (the compiler's one copy of a tail two arms share); L dropped when nothing goes to it then"""
    lines = list(lines)
    for _ in range(8):
        hit = None
        for k, l in enumerate(lines):
            g = re.fullmatch(r"(\s*)goto (L_\w+);", l)
            if not g or k == 0 or not re.search(r"\{\s*$", lines[k - 1]) or k + 1 >= len(lines) or not lines[k + 1].strip().startswith("}"):
                continue
            at = next((j for j, x in enumerate(lines) if x.strip() == g.group(2) + ":"), None)
            if at is None:
                continue
            ind = len(lines[at]) - len(lines[at].lstrip())
            j, depth = at + 1, 0
            while j < len(lines):                           # (to the end of the label's block)
                if depth == 0 and lines[j].strip().startswith("}"):
                    break
                depth += lines[j].count("{") - lines[j].count("}")
                j += 1
            tail = lines[at + 1:j]
            if not tail or len(tail) > 40 or any(re.fullmatch(r"\s*L_\w+:", x) for x in tail) or at < k < j:
                continue
            hit = (k, g.group(1), [g.group(1) + x[ind:] if x.strip() else x for x in tail], g.group(2), at)
            break
        if not hit:
            break
        k, gi, body, lab, at = hit
        lines[k:k + 1] = body
        if not any(re.search(r"\bgoto %s;" % lab, x) for x in lines):
            lines = [x for x in lines if x.strip() != lab + ":"]
    return lines


def model_texts(inv, code, name, base=None):
    """[(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk;
    a structured control-flow graph, where the model has one): the compiler decides between them"""
    raw, code = code, fp_native(code)
    if hasattr(inv, "FRAME_AFTER_BP"):                      # (a draft is of 3.00 or later code: its -Gs frames,
        inv.FRAME_AFTER_BP = True                           #  its register variables)
    if hasattr(inv, "REGVARS_EAGER"):
        inv.REGVARS_EAGER = True
    out = []
    try:
        fs, params, stmts, pt, loc = inv.recover(code)
    except Exception:
        if raw == code:
            raise
        fs, params, stmts, pt, loc = inv.recover(raw)       # (the linear walk lost in the 8087's code: as it was)
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
            _fpinfo(inv, raw, code)
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
        # (a long compared the compiler's way - the high words, then the low words without sign - three nested
        #  ifs: read again with the unsigned compares marked, and the ladders folded into one long comparison)
        from codebreaker.exact import x86 as _x86          # (or a compare without sign: the text with it marked)
        _ins = list(_x86.disasm(bytes(code), 0, len(code), False))
        unsigned = any(a_.mnemonic == "cmp" and b_.mnemonic in ("jb", "jbe", "ja", "jae") and "bp" not in a_.op_str.split(",")[1]
                       for a_, b_ in zip(_ins, _ins[1:]))
        if hasattr(inv, "_fold_ladders") and (unsigned or re.search(r"if \(\((\w+) [<>]= [^)]*\)\) \{\n\s*if \(\(\1 [<>]= ", t)):
            olds = (inv.UNSIGNED_CAST, getattr(inv, "SWITCH", None))
            try:
                inv.UNSIGNED_CAST = True
                flow16.CMPSWITCH = True
                fs2, params2, stmts2, pt2, loc2 = inv.recover_cfg(code)
                stmts2 = [s for s in inv._fold_ladders(inline_goto_arms(stmts2)) if not s.startswith("/* register variables")]
                for reg, ix in regparam.items():
                    p = chr(ord("a") + ix)
                    stmts2 = [s for k, s in enumerate(stmts2) if not (k < 3 and re.fullmatch(r"%s = %s;" % (reg, p), s.strip()))]
                t2 = inv.to_c(name, fs2, params, stmts2, pt2, loc2)
                if t2 != t:
                    out.append((t2, regparam))
            except Exception:
                pass
            finally:
                inv.UNSIGNED_CAST = olds[0]
                flow16.CMPSWITCH = False
    _fpinfo(inv, raw, code)
    for reg, ix in regparam.items():                        # (a register parameter's register given the parameter:
        p = chr(ord("a") + ix)                              #  itself - the model's reload at a join, no statement)
        out = [(re.sub(r"(?m)^[ \t]*%s = %s;\n" % (reg, p), "", t), rp) for t, rp in out]
    return out


FPINFO = {}                                               # (the function's floating point: types, its value, any)


def _fpinfo(inv, raw, code):
    last = getattr(inv, "LAST", {})                         # (what the model read of the 8087's values: localize)
    FPINFO.clear()
    FPINFO.update(types=dict(last.get("fptypes") or {}), ret=last.get("fpret"),
                  fp=raw != code or bool(re.search(rb"\x9b[\xd8-\xdf]", bytes(raw))))
    LONGINFO.clear()                                        # (the longs: shifted without sign; a value widened last)
    LONGINFO["unsigned"] = set(last.get("unsigned_longs") or ())
    from codebreaker.exact import x86
    ins = list(x86.disasm(bytes(code), 0, len(code), False))
    j = len(ins) - 1
    if ins and ins[j].mnemonic in ("ret", "retf"):
        j -= 1
        while j >= 0 and (ins[j].mnemonic == "pop" or ins[j].op_str == "sp, bp"):
            j -= 1
        epi = ins[j + 1].address if j + 1 < len(ins) else None
        gives_dx = lambda x: x.mnemonic in ("cwd", "cdq") or (x.mnemonic in ("mov", "xor", "sub") and x.op_str.startswith("dx, "))
        ends = [j] + [k - 1 for k, x in enumerate(ins) if x.mnemonic == "jmp" and x.op_str in ("0x%x" % epi, str(epi)) and k > 0] if epi is not None else [j]
        LONGINFO["ret"] = j >= 0 and all(e >= 0 and gives_dx(ins[e]) for e in ends)   # (dx given before every way out)


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
