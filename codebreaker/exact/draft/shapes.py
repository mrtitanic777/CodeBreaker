"""shapes.py -- the control flow a compiler gives a statement, read back into the statement: a comparison's
value, the returns, the loops and their tests, a switch's arms, the gotos the model leaves."""
import re

from codebreaker.exact.draft.text import (_balanced, _block_end, _conj, _else_ifs, _if_parts, _in_loop, _live_after,
                                          _not, _opener, _openers, _plain_store, _strip, _top_split, _truth, LONGINFO,
                                          PTRSIZE, RANGES, split_body, UCHAR, WREG)


def compare_values(text):
    """if (C) { r = 1; } else { r = 0; } - a comparison's value (the compiler's 1, then 0 unless it holds): r = C"""
    rx = re.compile(r"(?m)^([ \t]*)if \((.*)\) \{\n\s*(ax|al|bx|cx|dx) = ([01]);\n\s*\} else \{\n\s*\3 = ([01]);\n\s*\}$")
    # (a byte given one or the other, then widened with zeros: a char's ?: - an int's would be given in ax)
    text = re.sub(r"(?m)^([ \t]*)if \((.*)\) \{\n\s*al = (-?\d+);\n\s*\} else \{\n\s*al = (-?\d+);\n\s*\}\n\s*ah = 0;$",
                  lambda m: "%sax = ((unsigned char)(%s ? %s : %s));" % (m.group(1), _strip("(%s)" % m.group(2)), m.group(3), m.group(4)), text)

    def one(m):
        ind, c, r, a, b = m.groups()
        if (a, b) == ("1", "0"):
            return "%s%s = %s;" % (ind, r, c if c.startswith("(") else "(%s)" % c)
        if (a, b) == ("0", "1"):
            return "%s%s = !(%s);" % (ind, r, c)
        return m.group(0)
    return rx.sub(one, text)


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
    breaks = set()                                          # (a switch's breaks after the values become returns)
    leaves = [0]

    def tail(lo, hi):                                       # (the statements lines[lo:hi] end giving reg a value:
        k = hi - 1                                          #  [indexes of those assignments], or None)
        while k >= lo and not lines[k].strip():
            k -= 1
        if k < lo:
            return None
        if re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[k]):
            return [k]
        if re.fullmatch(r"\s*return\b[^;]*;", lines[k]) and k != end:
            leaves[0] += 1                                  # (an arm returning its own value: nothing to give)
            return []
        if lines[k].strip() != "}":
            return None
        sw = next((j for j in range(k - 1, lo - 1, -1) if lines[j].strip() and not re.match(r"\s*(?:case [^:]+|default):$", lines[j]) and
                   len(lines[j]) - len(lines[j].lstrip()) == len(lines[k]) - len(lines[k].lstrip())), None)
        if sw is not None and re.match(r"\s*switch \(.*\) \{$", lines[sw]):   # (a switch each arm of which gives
            got, arm, ok_ = [], [], True                                    #  reg and breaks, a default among them)
            dflt = False
            for j in range(sw + 1, k + 1):
                if re.match(r"\s*(?:case [^:]+|default):$", lines[j]) or j == k:
                    if arm:
                        idx = [x for x in arm if lines[x].strip()]
                        sub = tail(idx[0], idx[-1]) if len(idx) >= 2 and lines[idx[-1]].strip() == "break;" and \
                            lines[idx[-2]].strip() == "}" else None  # (an arm ending in an if-tree giving it)
                        if len(idx) >= 2 and lines[idx[-1]].strip() == "break;" and re.fullmatch(r"\s*%s = [^;]+;" % reg, lines[idx[-2]]):
                            got.append(idx[-2])
                            breaks.add(idx[-1])
                        elif sub:
                            got += sub
                            breaks.add(idx[-1])
                        else:
                            ok_ = False
                    arm = []
                    dflt = dflt or lines[j].strip() == "default:"
                else:
                    arm.append(j)
            return got if ok_ and got else None             # (no default: the function falls off its end there,
                                                            #  as the source does)
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
    if len(ks) + leaves[0] < 2:
        return text
    for k in ks:
        lines[k] = re.sub(r"^(\s*)%s = (.*);$" % reg, r"\1return \2;", lines[k])
    lines[end] = None
    for b_ in breaks:
        lines[b_] = None
    return "\n".join(x for x in lines if x is not None)


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


def loop_registers(text):
    """r = E0; for (;;) { v = r; .. r = E1; continue; .. } - a register carried round a loop into a variable at its
    top (the model's phi): v itself - v = E0 before, v = E1 at each way round, the tests of r tests of v"""
    lines = text.split("\n")
    for k in range(1, len(lines)):
        m = re.fullmatch(r"([ \t]*)(?:for \(;;\)|while \(1\)) \{", lines[k])
        if not m:
            continue
        e = _block_end(lines, k)
        f = re.fullmatch(r"[ \t]*(\w+) = (ax|bx|cx|dx);", lines[k + 1] or "") if e else None
        pre = re.fullmatch(r"([ \t]*)(ax|bx|cx|dx) = ([^;]+);", lines[k - 1])
        if not f or not pre or pre.group(2) != f.group(2):
            continue
        v, r = f.group(1), f.group(2)
        body = lines[k + 2:e]
        ok = True
        for j, x in enumerate(body):                        # (r read only as v's value; given only before going round)
            if re.match(r"[ \t]*%s = " % r, x):
                if not (j + 1 < len(body) and body[j + 1].strip() == "continue;"):
                    ok = False
        if not ok or _live_after(lines, e, r):
            continue
        new = [pre.group(1) + "%s = %s;" % (v, pre.group(3)), lines[k]] + \
            [re.sub(r"\b%s\b" % r, v, x) for x in body] + lines[e:e + 1]
        return loop_registers("\n".join(lines[:k - 1] + new + lines[e + 1:]))
    return text


def const_tests(text):
    """x = K; if/while (.. x ..) at once - the test of what x was just given: of K (a variant: the compiler
    compares with the register it still holds K in)"""
    def one(m):
        ind, x, k, kw, test = m.groups()
        if len(re.findall(r"\b%s\b" % re.escape(x), test)) != 1:
            return m.group(0)
        return "%s%s = %s;\n%s%s (%s) {" % (ind, x, k, ind, kw, re.sub(r"\b%s\b" % re.escape(x), k, test))
    t = re.sub(r"(?m)^([ \t]*)(\w+) = (-?\d+);\n\1(if|while) \((.*)\) \{$", one, text)
    lines = t.split("\n")                                   # (and on past statements not giving it, to the first
    for k, l in enumerate(lines):                           #  test of it in the block or one inside)
        m = re.fullmatch(r"([ \t]*)(\w+) = (-?\d+);", l)
        if not m:
            continue
        x, kv = m.group(2), m.group(3)
        ind = len(m.group(1))
        for j in range(k + 1, len(lines)):
            y = lines[j]
            if y.strip() and len(y) - len(y.lstrip()) < ind or re.match(r"\s*(?:L_\w+:|\}\s*else)", y):
                break
            if re.search(r"(?<![\w>.])%s\s*(?:=[^=]|\+\+|--)" % x, y) or re.match(r"\s*(?:while|for|do)\b", y):
                break
            tm = re.fullmatch(r"(\s*)if \((.*)\) \{", y)
            if tm and re.search(r"\b%s\b" % x, tm.group(2)):
                if len(re.findall(r"\b%s\b" % x, tm.group(2))) == 1:
                    lines[j] = "%sif (%s) {" % (tm.group(1), re.sub(r"\b%s\b" % x, kv, tm.group(2)))
                break
            if re.search(r"\b%s\b" % x, y):
                break
    t = "\n".join(lines)
    return t if t != text else None


def loop_guards(text):
    """in a loop: if (C) { S; continue; } else { R } (R leaving: return, break) last in its body - the leaving
    test first: if (!C) { R } S (a variant, with its for when S ends by a step)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)(?:while \(.*\)|for \(.*\)) \{", lines[k])
        e = _block_end(lines, k) if m else None
        if e is None:
            continue
        i4 = m.group(1) + "    "
        j0 = max((j for j in range(k + 1, e) if lines[j].startswith(i4 + "if (") and lines[j].endswith("{")), default=None)
        p = _if_parts(lines, j0) if j0 is not None else None
        if not p or p[2] is None or p[3] != e:
            continue
        th = [x for x in p[1] if x.strip()]
        el = [x for x in p[2] if x.strip()]
        if not th or th[-1].strip() != "continue;" or not el or not re.match(r"\s*(?:return\b|break;)", el[-1]):
            continue
        neg = _truth(_not("(%s)" % p[0]))
        new = ["%sif (%s) {" % (i4, neg)] + p[2] + [i4 + "}"] + [x[4:] for x in th[:-1]]
        t = "\n".join(lines[:j0] + new + lines[e:])
        return for_loops(t) or t
    return None


def first_breaks(text):
    """for (;;) { if (C) break; S } - the loop's test first: while (!C) { S }"""
    lines = text.split("\n")
    for k in range(len(lines) - 1):
        m = re.fullmatch(r"([ \t]*)for \(;;\) \{", lines[k])
        b = re.fullmatch(r"[ \t]*if (\(.*\)) break;", lines[k + 1]) if m else None
        if not b or not _balanced(b.group(1)):
            continue
        cond = _truth(_not(b.group(1)))
        lines[k:k + 2] = ["%swhile %s {" % (m.group(1), cond if cond.startswith("(") and _balanced(cond[1:-1]) else "(%s)" % cond)]
        return first_breaks("\n".join(lines))
    return text


def loop_tests(text):
    """while (C) { if (D) { S } else { break; } } - the loop's own test the two: while (C && D) { S } (and the
    other way about, if (D) { break; } S: while (C && !D) { S })"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)while (\(.*\)) \{", lines[k])
        if not m:
            continue
        e = _block_end(lines, k)
        if e is None or lines[e].strip() != "}":
            continue
        p = _if_parts(lines, k + 1)
        if not p or p[3] != e:
            continue
        ind = m.group(1)
        if p[2] is not None and [x.strip() for x in p[2] if x.strip()] == ["break;"]:
            body, d = p[1], "(%s)" % p[0] if not p[0].startswith("(") else p[0]
        elif p[2] is None and [x.strip() for x in p[1] if x.strip()] == ["break;"]:
            continue                                        # (if (D) break; and nothing after: no body to keep)
        else:
            continue
        cond = _conj("&&", _truth(m.group(2)), _truth(d))
        new = ["%swhile (%s) {" % (ind, cond)] + [x[4:] if x.startswith(ind + "        ") else x for x in body] + ["%s}" % ind]
        t = "\n".join(lines[:k] + new + lines[e + 1:])
        return for_loops(t) or t                            # (and its step a for's, when it has one)
    return None


def goto_tails(text, most=3):
    """goto L; with L: in another arm, a few plain statements to that arm's end after it - the compiler's one
    copy of a tail two arms share: the statements in place of the goto (the value it carried given just before)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"([ \t]*)goto (L_\w+);", l)
        if not m:
            continue
        at = next((j for j, x in enumerate(lines) if x.strip() == m.group(2) + ":"), None)
        if at is None or at > k and all(not x.strip() or x.strip().endswith(":") for x in lines[k + 1:at]):
            continue                                        # (a goto that only falls there: fall_gotos')
        ind = len(lines[at]) - len(lines[at].lstrip())
        tail, j = [], at + 1
        while j < len(lines) and lines[j].strip() and len(lines[j]) - len(lines[j].lstrip()) == ind:
            x = lines[j].strip()
            if x.endswith("{") or x.endswith(":") and not re.fullmatch(r"L_\w+:", x) or re.match(r"(?:case |default)", x):
                tail = None
                break
            if re.fullmatch(r"L_\w+:", x):
                j += 1
                continue
            tail.append(x)
            j += 1
        if not tail or len(tail) > most or j >= len(lines) or len(lines[j]) - len(lines[j].lstrip()) >= ind or \
                any(re.match(r"(?:goto|break|continue|return)\b", x) for x in tail):
            continue
        lines[k:k + 1] = [m.group(1) + x for x in tail]
        t = "\n".join(lines)
        used = set(re.findall(r"goto (L_\w+);", t))
        return "\n".join(x for x in t.split("\n") if not (re.fullmatch(r"[ \t]*(L_\w+):", x) and x.strip()[:-1] not in used))
    return None


def call_tests(text):
    """ax = E; r = ax; switch (ax) - the selector an assignment: switch (r = E); and F(..); if (.. ax ..) - a call
    whose value the next test reads: the call in the test; ((x + 1) == 0) as (x == -1)"""
    text = re.sub(r"(?m)^([ \t]*)ax = ([^;\n]+);\n\1(\w+) = ax;\n\1switch \(ax\) \{$",
                  lambda m: "%sswitch (%s = %s) {" % (m.group(1), m.group(3), m.group(2)), text)

    def one(m):
        ind, call, test = m.group(1), m.group(2), m.group(3)
        if len(re.findall(r"\bax\b", test)) != 1:
            return m.group(0)
        return "%sif %s {" % (ind, re.sub(r"\bax\b", lambda _: call, test))
    text = re.sub(r"(?m)^([ \t]*)([A-Za-z_]\w*\([^;\n]*\));\n\1if (\(.*\)) \{$", one, text)
    return re.sub(r"\(\(([^()]+|[A-Za-z_]\w*\([^()]*\)) \+ 1\) (==|!=) 0\)", r"(\1 \2 -1)", text)


def fall_dups(text):
    """a switch arm ending with what the next arm is, word for word, and leaves (return, break): that tail gone -
    it falls into the next (the compiler's one copy, the model's two)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"([ \t]*)switch \(.*\) \{", l)
        if not m:
            continue
        ind = m.group(1)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if end is None:
            continue
        heads = [j for j in range(k + 1, end) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])]
        arms = []
        for i, h in enumerate(heads):
            stop = heads[i + 1] if i + 1 < len(heads) else end
            j = h
            while j + 1 < stop and re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j + 1]):
                j += 1
            if j + 1 < stop:
                arms.append((j + 1, stop))
        for (a0, a1), (b0, b1) in zip(arms, arms[1:]):
            nb = b1 - b0
            body_b = lines[b0:b1]
            if not body_b or not re.match(r"\s*(?:return\b|break;)", body_b[-1]) or a1 - a0 <= nb or lines[a1 - nb:a1] != body_b:
                continue
            if any(re.fullmatch(r"\s*L_\w+:", x) for x in body_b):
                continue
            del lines[a1 - nb:a1]
            return fall_dups("\n".join(lines))
    return text


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
            def after(q):                                   # (where the flow goes from line q on: past ends of ifs
                while q < len(lines):                       #  and the else arms beside them, to a statement)
                    x = (lines[q] or "").strip()
                    if lines[q] is None or not x or re.fullmatch(r"L_\w+:", x):
                        q += 1
                    elif x == "}":
                        o = opener(q)
                        if o is None or not re.match(r"[ \t]*(?:if \(.*\)|\} else|else) \{$", lines[o]):
                            return None
                        q += 1
                    elif x == "} else {":
                        q = _block_end([y if y is not None else "" for y in lines], q) or len(lines)
                    else:
                        return q
                return None
            at = next((q for q, x in enumerate(lines) if x is not None and x.strip() == m.group(1) + ":"), None)
            if at is not None and after(k + 1) is not None and after(k + 1) == after(at + 1):
                lines[k] = None                             # (the goto goes where its own way goes anyway)
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


def tested_zero(text):
    """if ((f(..) != 0)) { .. return ..; } then the arm's end, the function's value read after: ax is that call's
    value, 0 there - ax = 0; written (the model's value left in the register)"""
    lines = text.split("\n")
    if not re.search(r"(?m)^[ \t]*return ax;$", text):
        return text
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)if \(\(([A-Za-z_]\w*\(.*\)) != 0\)\) \{", lines[k])
        p = _if_parts(lines, k) if m else None
        if not p or p[2] is not None or p[3] >= len(lines):
            continue
        th = [x for x in p[1] if x.strip()]
        if not th or not re.match(r"\s*return\b", th[-1]) or not re.fullmatch(r"[ \t]*\}(?: else \{)?", lines[p[3]]):
            continue
        lines.insert(p[3], m.group(1) + "ax = 0;")
        return tested_zero("\n".join(lines))
    return text


def continue_loops(text):
    """for (;;) { [x = E;] if (C1) continue; .. if (Cn) { leave } [else { continue; }] } - a loop of tests going
    round while one fails: while (!((x = E) .. && !C1 && .. Cn)) ; then leave (a return of a number, a break)"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = re.fullmatch(r"([ \t]*)for \(;;\) \{", lines[k])
        e = _block_end(lines, k) if m else None
        if not m or e is None or lines[e].strip() != "}":
            k += 1
            continue
        body = [x.strip() for x in lines[k + 1:e] if x.strip()]
        first = re.fullmatch(r"(\w+) = ([^;=]+);", body[0]) if body else None
        q = 1 if first else 0
        parts, leave, ok = [], None, True
        while q < len(body):
            c = re.fullmatch(r"if (\(.*\)) \{", body[q])
            c1 = re.fullmatch(r"if (\(.*\)) continue;", body[q])
            if c1:
                parts.append(_truth(_not(c1.group(1)))); q += 1
                continue
            if c and body[q + 1:q + 3] == ["continue;", "}"]:
                parts.append(_truth(_not(c.group(1)))); q += 3
                continue
            if c and q + 2 < len(body) and re.fullmatch(r"(?:return -?\d+|break);", body[q + 1]) and \
                    (body[q + 2:] == ["}"] or body[q + 2:] == ["} else {", "continue;", "}"]):
                parts.append(_truth(c.group(1)))
                leave = body[q + 1]
                q = len(body)
                break
            ok = False
            break
        if not ok or leave is None or len(parts) < 2:
            k += 1
            continue
        if first:
            x, v = first.groups()
            p0 = parts[0]
            if len(re.findall(r"\b%s\b" % x, p0)) != 1:
                k += 1
                continue
            parts[0] = re.sub(r"\b%s\b" % x, lambda _: "(%s = %s)" % (x, v), p0, count=1)
        cond = " && ".join(p if re.fullmatch(r"!?[\w\->.\[\]]+|!?\(.*\)", p) and _balanced(p) else "(%s)" % p for p in parts)
        ind = m.group(1)
        new = ["%swhile (!(%s))" % (ind, cond), "%s    ;" % ind] + (["%s%s" % (ind, leave)] if leave != "break;" else [])
        lines[k:e + 1] = new
        k += len(new)
    return "\n".join(lines)


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


def stored_tests(text):
    """r = E; x = r; if ((r != 0)) - a value stored, then tested in the register still holding it: the
    assignment tested, if ((x = E)) (== 0: if (!(x = E))) - r read no more"""
    lines = text.split("\n")
    for k in range(len(lines) - 2):
        m = re.match(r"([ \t]*)(ax|al|bx|cx|dx) = ([^;]+);$", lines[k] or "")
        if not m:
            continue
        ind, r, e = m.groups()
        a = re.match(r"%s(\w+(?:->f\d+)*|\*\w+) = %s;$" % (ind, r), lines[k + 1] or "")
        t = re.match(r"%s(if|while) \(\(%s ([!=]=) 0\)\)(.*)$" % (ind, r), lines[k + 2] or "")
        if not a or not t or re.search(r"\b%s\b" % r, a.group(1) + e + t.group(3)) or _live_after(lines, k + 2, r):
            continue
        v = "(%s = %s)" % (a.group(1), e)
        lines[k] = "%s%s (%s)%s" % (ind, t.group(1), "(%s)" % v[1:-1] if t.group(2) == "!=" else "!" + v, t.group(3))
        lines[k + 1] = lines[k + 2] = None
    return "\n".join(x for x in lines if x is not None)


def flat_returns(text):
    """the returns as a source with early exits writes them (a variant): an else after an arm that returns
    written after the if; a switch's default arm first, every path of it returning and every other arm too,
    written after the switch; a return reached at once again by an arm's end (through ifs' ends only) gone"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    seen = list(lines)
    changed = True
    while changed:
        changed = False
        for k, l in enumerate(lines):                       # (an else after an arm that returns)
            if l.strip() != "} else {":
                continue
            prev = next((x for x in reversed(lines[:k]) if x.strip()), "")
            ind = l[:len(l) - len(l.lstrip())]
            if not re.fullmatch(r"%s    return\b[^;]*;" % re.escape(ind), prev):
                continue
            op = _opener(lines, k)
            if op is None or not re.match(r"\s*if \(.*\) \{$", lines[op]):
                continue
            p = _if_parts(lines, op)
            if not p or p[2] is None:
                continue
            lines = lines[:k] + [ind + "}"] + [x[4:] if x.startswith(ind + "    ") else x for x in p[2]] + lines[p[3]:]
            changed = True
            break
        if changed:
            continue
        for k, l in enumerate(lines):                       # (a default arm first, every path returning)
            m = re.match(r"(\s*)switch \(.*\) \{$", l)
            if not m:
                continue
            ind = m.group(1)
            end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
            if end is None or lines[k + 1] != ind + "default:":
                continue
            labels = [j for j in range(k + 1, end) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])]
            arms = [(a, b) for a, b in zip(labels, labels[1:] + [end])]
            ends = [next((x for x in reversed(lines[a + 1:b]) if x.strip()), "") for a, b in arms]
            if len(arms) < 2 or not ends[0] or not all(re.fullmatch(r"%s    return\b[^;]*;" % re.escape(ind), e) for e in ends if e) or \
               any(re.match(r"\s*(?:break|continue);", x) for x in lines[k + 1:end]):
                continue
            d0, d1 = arms[0]
            lines = lines[:k + 1] + lines[d1:end + 1] + [x[4:] if x.startswith(ind + "    ") else x for x in lines[d0 + 1:d1]] + lines[end + 1:]
            changed = True
            break
        if changed:
            continue
        for k, l in enumerate(lines):                       # (a return the arm's end reaches at once again)
            m = re.fullmatch(r"\s*(return\b[^;]*;)", l)
            if not m:
                continue
            j = k + 1
            ok = True
            while j < len(lines) and lines[j].strip() == "}":
                op = _opener(lines, j)
                if op is None or not re.match(r"\s*(?:if \(.*\) \{|\} else \{)$", lines[op]):
                    ok = False
                    break
                j += 1
            if ok and j > k + 1 and j < len(lines) and lines[j].strip() == m.group(1) and \
               lines[k - 1].strip() not in ("{", "} else {") and not re.match(r"\s*(?:if|else)\b.*\{$", lines[k - 1]):
                lines = lines[:k] + lines[k + 1:]
                changed = True
                break
    if lines == seen:
        return None
    return head + sep + "\n".join(lines)


def switch_follow(text):
    """switch (x) { default: L: S .. case K: .. goto L; .. } - the default arm the code after the switch (every
    way through S leaving, the other arms jumping to it): default: break; the gotos breaks, S after the switch"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)switch \(.*\) \{$", l)
        if not m or k + 2 >= len(lines) or lines[k + 1] != m.group(1) + "default:":
            continue
        ind = m.group(1)
        lab = re.fullmatch(r"%s    (L_\w+):" % re.escape(ind), lines[k + 2])
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if not lab or end is None:
            continue
        nxt = next((j for j in range(k + 2, end) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])), None)
        if nxt is None:
            continue
        body = lines[k + 3:nxt]
        last = next((x for x in reversed(body) if x.strip()), "")
        gotos = [j for j in range(nxt, end) if lines[j].strip() == "goto %s;" % lab.group(1)]
        if not gotos or not re.fullmatch(r"%s    return\b[^;]*;" % re.escape(ind), last) or \
           any(re.fullmatch(r"%s    break;" % re.escape(ind), x) for x in body) or any(_in_loop(lines, k, j, len(ind)) for j in gotos):
            continue
        new = list(lines)
        for j in gotos:
            new[j] = new[j].replace("goto %s;" % lab.group(1), "break;")
        rest = new[nxt:end + 1]
        after = [x[4:] if x.startswith(ind + "    ") else x for x in body]
        out = new[:k + 1] + [ind + "default:", ind + "    break;"] + rest + after + new[end + 1:]
        t = "\n".join(out)
        if re.search(r"\bgoto %s;" % lab.group(1), t):
            t = "\n".join(new[:k + 1] + [ind + "default:", ind + "    break;"] + rest + [ind + lab.group(1) + ":"] + after + new[end + 1:])
        return switch_follow(t)
    return text


def return_cases_first(text):
    """switch (..) { .. case K: return V; } - the arms that only return written first (a variant: the compiler
    lays a switch's arms out from the source's order its own way)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)switch \(.*\) \{$", l)
        if not m:
            continue
        ind = m.group(1)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if end is None:
            continue
        labels = [j for j in range(k + 1, end) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])]
        if not labels or labels[0] != k + 1:
            continue
        groups, j = [], 0                                   # (arms: their labels together, then the statements)
        while j < len(labels):
            a = j
            while j + 1 < len(labels) and labels[j + 1] == labels[j] + 1:
                j += 1
            b = labels[j + 1] if j + 1 < len(labels) else end
            groups.append((labels[a], b))
            j += 1
        rets = [g for g in groups if [x.strip() for x in lines[g[0]:g[1]] if x.strip() and not re.fullmatch(r"\s*(?:case [^:]+|default):", x)][:1] and
                all(re.fullmatch(r"\s*(?:case [^:]+|default):|%s    return(?: -?\d+)?;" % re.escape(ind), x) for x in lines[g[0]:g[1]] if x.strip())]
        if not rets or rets[0] == groups[0] or len(rets) == len(groups):
            continue
        prev = {g: lines[groups[i - 1][0]:groups[i - 1][1]] for i, g in enumerate(groups) if i}
        if any(not re.match(r"\s*(?:break|continue|return\b|goto\b)", next((x for x in reversed(prev[g]) if x.strip()), "")) for g in rets if g in prev):
            continue                                        # (an arm falling into it: not moved)
        order = rets + [g for g in groups if g not in rets]
        body = [x for g in order for x in lines[g[0]:g[1]]]
        return "\n".join(lines[:k + 1] + body + lines[end:])
    return None


def selector_reads(text):
    """switch (x) { case K: .. f(ax) .. } - an arm reading the register the selector was compared in before
    anything gives it a value (an arm only the dispatch enters: the first, or after one that leaves): x itself"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)switch \((.+)\) \{$", l)
        if not m or re.search(r"\w\s*\(|\+\+|--|[^=!<>]=[^=]", m.group(2)) or re.search(r"\b(?:ax|al|ah)\b", m.group(2)):
            continue
        ind, sel = m.group(1), m.group(2)
        j, entered = k + 1, True                            # (entered: only the dispatch reaches this arm)
        while j < len(lines) and lines[j] != ind + "}":
            x = lines[j]
            if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), x):
                prev = next((p for p in reversed(lines[k + 1:j]) if p.strip()), lines[k])
                entered = entered if re.fullmatch(r"\s*(?:case [^:]+|default):", prev) else bool(
                    prev == lines[k] or re.fullmatch(r"\s*(?:break|continue|return\b.*|goto \w+);", prev))
                ok = entered
                j += 1
                bsel = re.fullmatch(r"\*(\w+)", sel)              # (a byte the selector: its al)
                while ok and j < len(lines) and not re.fullmatch(r"%s(?:case [^:]+|default):|%s\}" % (re.escape(ind), re.escape(ind)), lines[j]):
                    y = lines[j]
                    if bsel and re.fullmatch(r"\s*%s = %s \+ 1;" % (bsel.group(1), bsel.group(1)), y) and j + 1 < len(lines) and \
                       len(re.findall(r"\bal\b", lines[j + 1])) == 1 and not re.search(r"\b%s\b" % bsel.group(1), lines[j + 1]):
                        lines[j + 1] = re.sub(r"\bal\b", "*%s++" % bsel.group(1), lines[j + 1])   # (read, then stepped:
                        lines[j] = "\x00"                                                   #  *p++)
                        break
                    if bsel and len(re.findall(r"\bal\b", y)) == 1 and not re.search(r"\b(?:ax|ah)\b|\bal = ", y):
                        lines[j] = re.sub(r"\bal\b", sel, y)
                        break
                    if re.match(r"\s*L_\w+:", y) or re.search(r"\b(?:al|ah)\b", y):
                        break
                    if re.search(r"\bax\b", y):
                        w = re.match(r"(\s*ax = )(.*)$", y)
                        if w and not re.search(r"\bax\b", w.group(2)):
                            break                           # (given a value first: not the selector's)
                        if w:
                            lines[j] = w.group(1) + re.sub(r"\bax\b", sel, w.group(2))
                        elif re.match(r"\s*(?:if|while|switch|for)\b", y) and not re.search(r"\w\s*\(", y):
                            break
                        else:
                            lines[j] = re.sub(r"\bax\b", sel, y)
                        break
                    if re.search(r"\w\s*\(|[{}]", y):
                        break                               # (a call, a block: the register may hold anything)
                    j += 1
                continue
            j += 1
    return "\n".join(l for l in lines if l != "\x00")


def unsigned_fields(text, most=3):
    """[the text with one int field of a struct unsigned] - a variant each, for the fields shifted right or
    compared (<, <=, >, >=): the compiler's shr and unsigned jumps say unsigned"""
    out = []
    fields = [(s.group(1), s.start() + f.start(), s.start() + f.end(), f.group(1))
              for s in re.finditer(r"struct (\w+) \{[^}]*\}", text) for f in re.finditer(r"(?<![\w ])int f(\d+);|(?<=[{;] )int f(\d+);", s.group(0))]
    for tag, a, b, n in fields[:16]:
        n = n or re.search(r"\d+", text[a:b]).group(0)
        ptrs = set(re.findall(r"struct %s \*\s*(\w+)" % tag, text)) | set(re.findall(r"struct %s (\w+)\[" % tag, text))
        if not any(re.search(r"\b%s(?:\[[^\]]*\])?(?:->|\.)f%s\b\)? (?:>>|[<>]=?(?![<>]))" % (re.escape(p), n), text) for p in ptrs):
            continue
        out.append(text[:a] + "unsigned f%s;" % n + text[b:])
        if len(out) >= most:
            break
    return out


def adjacent_swaps(text, most=4):
    """[the text with two next statements swapped] - assignments neither of which reads or gives what the other
    gives, no call in either (a variant each: the order the model wrote two independent stores in may not be
    the source's)"""
    lines = text.split("\n")
    out = []
    rx = re.compile(r"([ \t]*)([\w\->.\[\]*]+) = ([^;]+);$")
    for k in range(len(lines) - 1):
        a, b = rx.fullmatch(lines[k]), rx.fullmatch(lines[k + 1])
        if not a or not b or a.group(1) != b.group(1) or re.search(r"\w\s*\(", a.group(3) + b.group(3)):
            continue
        la, lb = re.sub(r"->.*|\[.*", "", a.group(2)).lstrip("*"), re.sub(r"->.*|\[.*", "", b.group(2)).lstrip("*")
        if re.search(r"\b%s\b" % re.escape(la), lines[k + 1]) or re.search(r"\b%s\b" % re.escape(lb), lines[k]):
            continue
        new = list(lines)
        new[k], new[k + 1] = lines[k + 1], lines[k]
        out.append("\n".join(new))
        if len(out) >= most:
            break
    return out


def word_offsets(text):
    """((int *)((char *)p + N))[E] - words from N bytes into what an int *p points to: p[E + N/2] (a variant: the
    compiler folds the constant into the place, [bx+si+N])"""
    def one(m):
        p, n, e = m.group(1), int(m.group(2)), m.group(3)
        if n % 2:
            return m.group(0)
        if re.search(r"(?m)^[ \t]*(?:register )?int \*%s;" % re.escape(p), text):
            return "%s[%s + %d]" % (p, e, n // 2)
        return "((int *)%s)[%s + %d]" % (p, e, n // 2)       # (another pointer: through an int * of it)
    return re.sub(r"\(\(int \*\)\(\(char \*\)(\w+) \+ (\d+)\)\)\[([^\[\]]+)\]", one, text)


def byte_heads(text):
    """*(int *)buf = E; - a buffer's first word given: its first byte (a variant: the store's width the model
    left to the type it guessed)"""
    return re.sub(r"(?m)^([ \t]*)\*\(int \*\)(\w+) = ([^;]+);$", r"\1\2[0] = \3;", text)


def common_tails(text):
    """if (C) { A; T } else { T } - both arms ending in the same statements (the compiler's one copy of them,
    jumped to): if (C) { A } T (the else's alone the same: if (!C) { B } T)"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            k += 1
            continue
        a, b = [x for x in p[1]], [x for x in p[2]]
        n = 0
        while n < min(len(a), len(b)) and a[-1 - n] == b[-1 - n] and a[-1 - n].strip():
            n += 1
        if not n or any(re.match(r"\s*(?:L_\w+:|case\b|default:|\}|else\b)", x) or x.rstrip().endswith("{") for x in a[len(a) - n:]):
            k += 1
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        tail = [x[4:] if x.startswith(ind + "    ") else x for x in a[len(a) - n:]]
        ra, rb = a[:len(a) - n], b[:len(b) - n]
        if not any(x.strip() for x in ra) and not any(x.strip() for x in rb):
            k += 1
            continue
        if not any(x.strip() for x in rb):
            new = [lines[k]] + ra + [ind + "}"]
        elif not any(x.strip() for x in ra):
            neg = _not("(%s)" % p[0])
            new = ["%sif %s {" % (ind, neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg)] + rb + [ind + "}"]
        else:
            new = [lines[k]] + ra + [ind + "} else {"] + rb + [ind + "}"]
        lines[k:p[3]] = new + tail
        k += 1
    return "\n".join(lines)


def common_heads(text):
    """if (C) { S; A } else { S; B } - both arms beginning with the same statement, no call in it and nothing C
    reads given by it: S; if (C) { A } else { B } (the model's copy of what the compiler did before the test)"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            k += 1
            continue
        a = [x for x in p[1] if x.strip()]
        b = [x for x in p[2] if x.strip()]
        m = re.fullmatch(r"\s*([\w\->.\[\]*]+) = ([^;]+);", a[0]) if a and b and a[0].strip() == b[0].strip() else None
        if not m or re.search(r"\w\s*\(|\+\+|--", a[0]) or \
           re.search(r"\b%s\b" % re.escape(re.sub(r"->.*|\[.*", "", m.group(1)).lstrip("*")), p[0]):
            k += 1
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        ia, ib = p[1].index(a[0]), p[2].index(b[0])
        lines[k:p[3]] = [ind + a[0].strip(), lines[k]] + p[1][:ia] + p[1][ia + 1:] + [ind + "} else {"] + \
            p[2][:ib] + p[2][ib + 1:] + [ind + "}"]
        k += 2
    return "\n".join(lines)


def call_pairs(text):
    """t = A; v = F(B); .. (G(t) op v) - two calls an operator joins, the compiler's order: the right one
    first into a temporary, the left one's argument kept from before it: (G(A) op F(B)) - a variant"""
    def one(m):
        ind, t, a, v, f, rest = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5), m.group(6)
        g = re.search(r"\b(\w+)\(%s\) ([-+*&|^]) %s\b" % (t, v), rest)
        if not g or len(re.findall(r"\b%s\b" % v, text)) != 3 and len(re.findall(r"\b%s\b" % v, text)) != 2:
            return m.group(0)
        new = rest[:g.start()] + "%s(%s) %s %s" % (g.group(1), a, g.group(2), f) + rest[g.end():]
        return ind + new
    t = re.sub(r"(?m)^([ \t]*)(t\d+) = ([^;\n]+);\n[ \t]*(v\d+) = (\w+\([^;\n]*\));\n[ \t]*(.*)$", one, text)
    if t == text:
        return None
    for v in re.findall(r"(?m)^[ \t]*int (v\d+);$", t):     # (the temporary's local: gone)
        if len(re.findall(r"\b%s\b" % v, t)) == 1:
            t = re.sub(r"(?m)^[ \t]*int %s;\n" % v, "", t)
    for x in re.findall(r"(?m)^[ \t]*int ((?:t\d+, )*t\d+);$", t):
        left = [y for y in x.split(", ") if len(re.findall(r"\b%s\b" % y, t)) > 1]
        t = re.sub(r"(?m)^([ \t]*)int %s;$" % re.escape(x), (r"\1int %s;" % ", ".join(left)) if left else "", t)
    return re.sub(r"\n\n+", "\n", t)


def lvalue_choices(text):
    """*(C ? A : B) = E; - one store the compiler shares between two ways to its place: if (C) *A = E; else
    *B = E; (A, B: `*X` read as the field X->f0 when they end in one)"""
    def one(m):
        ind, inner, e = m.group(1), m.group(2), m.group(3)
        parts = _top_split(inner, "?")
        if len(parts) != 2:
            return m.group(0)
        c = parts[0].strip()
        ab = _top_split(parts[1], ":")
        if len(ab) != 2 or re.search(r"\w\s*\(|\+\+|--", e):
            return m.group(0)
        a, b = ab[0].strip(), ab[1].strip()
        c = c if c.startswith("(") else "(%s)" % c
        return "%sif %s {\n%s    *%s = %s;\n%s} else {\n%s    *%s = %s;\n%s}" % (ind, c, ind, a, e, ind, ind, b, e, ind)
    return re.sub(r"(?m)^([ \t]*)\*\((.+\?.+:.+)\) = ([^;]+);$", one, text)


def label_sinks(text):
    """L: S; goto H; inside a loop H: while (..) { .. } - the code a goto in the loop jumps to, the loop's own
    last statements (S then the next test): S at the end of the loop's body, `goto L;` where it was (a
    variant: the source's `nomatch: ;` before the step, the model's copy at the first jump to it)"""
    lines = text.split("\n")
    for i, l in enumerate(lines):
        m = re.fullmatch(r"(\s*)(L_\w+):", l)
        if not m:
            continue
        ind, lab = m.group(1), m.group(2)
        j = i + 1
        body = []
        while j < len(lines) and lines[j].startswith(ind) and lines[j].strip() and not lines[j].startswith(ind + "}") \
                and not re.fullmatch(r"\s*L_\w+:", lines[j]):
            body.append(lines[j])
            j += 1
        if len(ind) < 8 or not body or j >= len(lines) or not lines[j].startswith(ind[:-4] + "}"):
            continue                                        # (the block's last statements, inside an arm)
        g = re.fullmatch(r"\s*goto (L_\w+);", body[-1])
        if not g or any(x.rstrip().endswith("{") or x.strip().startswith("}") for x in body):
            continue
        h = g.group(1)
        hj = next((k for k in range(i) if lines[k].strip() == h + ":"), None)
        if hj is None or not re.match(r"\s*while \(.*\) \{$", lines[hj + 1]):
            continue
        hind = re.match(r"\s*", lines[hj + 1]).group(0)
        end = next((k for k in range(hj + 2, len(lines)) if lines[k] == hind + "}"), None)
        if end is None or not hj + 1 < i < end:
            continue
        moved = [hind + "    " + lab + ":"] + [hind + "    " + x.strip() for x in body[:-1]]
        new = lines[:i] + [ind + "goto %s;" % lab] + lines[j:end] + moved + lines[end:]
        t = "\n".join(new)
        if not re.search(r"\bgoto %s;" % h, t):
            t = re.sub(r"(?m)^[ \t]*%s:\n" % h, "", t)
        return t
    return None


def shared_exit(text):
    """a function ending in a loop that never ends, returning a variable from inside it: the return at the end,
    jumped to (goto out; .. out: return x;) - a variant: the code the compiler lays after the loop"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    last = next((k for k in range(len(lines) - 1, -1, -1) if lines[k].strip()), None)
    if last is None or lines[last].strip() != "}":
        return None
    prev = next((k for k in range(last - 1, -1, -1) if lines[k].strip()), None)
    if prev is None or re.match(r"\s*return\b", lines[prev]):
        return None
    rets = [k for k, l in enumerate(lines) if re.fullmatch(r"\s+return [a-z]\w*;", l) and len(l) - len(l.lstrip()) > 4]
    if not rets:
        return None
    k = rets[-1]
    v = lines[k].strip()
    lines[k] = re.sub(r"return [a-z]\w*;", "goto out_;", lines[k])
    lines[last:last] = ["out_:", "    " + v]
    return head + sep + "\n".join(lines)


def pointer_index(text):
    """bx = E; bx = (bx << 1) + P; .. *bx .. - a word element through a pointer P held in the data (not an
    array there): *(P + E), P a pointer to words"""
    def one(m):
        ind, e, p, rest = m.group(1), m.group(2), m.group(3), m.group(4)
        if len(re.findall(r"\*bx\b", rest)) != 1 or re.search(r"\bbx\b", rest.replace("*bx", "")):
            return m.group(0)
        PTRSIZE["g_%04x" % int(p[2:], 16) if p.startswith("g_") else p] = 2
        return ind + rest.replace("*bx", "*(%s + %s)" % (p, e if re.fullmatch(r"[\w\->.]+", e) else "(%s)" % e))
    return re.sub(r"(?m)^([ \t]*)bx = (.+);\n[ \t]*bx = \(bx << 1\) \+ (g_[0-9a-f]+|[a-z]|v\d+);\n[ \t]*(.*)$", one, text)


def case_guards(text):
    """[switch (x) { .. case K: return V; .. } with that case an if before it] - `if (x == K) return V;` the
    compiler compares first and merges into the switch's tests (variants: as it returns, and a bare return)"""
    lines = text.split("\n")
    out = []
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)switch \((.+)\) \{$", l)
        if not m:
            continue
        ind, sel = m.group(1), m.group(2)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if end is None:
            continue
        for j in range(k + 1, end - 1):
            c = re.fullmatch(r"%scase (-?\w+):" % re.escape(ind), lines[j])
            r = re.fullmatch(r"%s    (return(?: [^;]+)?;)" % re.escape(ind), lines[j + 1])
            prev = lines[j - 1]
            if not c or not r or re.fullmatch(r"\s*(?:case [^:]+|default):", prev) or \
               (j + 2 < end and re.fullmatch(r"\s*(?:case [^:]+|default):", lines[j + 2]) is None and lines[j + 2] != ind + "}"):
                continue
            if not re.match(r"\s*(?:break|continue|return\b|goto\b)", prev) and prev != l:
                continue                                    # (an arm falling into it: not alone)
            body = lines[:k] + lines[k:j] + lines[j + 2:]
            for ret in sorted({r.group(1), "return;"}):
                g = "%sif ((%s == %s)) %s" % (ind, sel, c.group(1), ret)
                new = list(body)
                new.insert(k, g)
                out.append("\n".join(new))
            break
    return out


def heads_variant(text):
    """common_heads, then the ?: values it leaves and the registers and temporaries folded (a variant: the
    compiler may have done the statement before the test, or in each arm)"""
    h = common_heads(text)
    return temporaries(fold_registers(fold_registers(conditional_values(h)))) if h != text else None


def chain_tails(text):
    """if (C1) { A; T } else if (C2) { B; T } else { return R; } - every way on ending in the same tail T (a
    return last), the others leaving: if (C1) { A } else if (C2) { B } else { return R; } T - the compiler's
    one copy of T, jumped to"""
    lines = text.split("\n")
    rel = lambda ls, d: [x[d:] if x[:d].strip() == "" else x.strip() for x in ls]
    leaves = lambda ls: bool(ls) and re.match(r"\s*(?:return\b|goto\b|break;|continue;)", [x for x in ls if x.strip()][-1] or "")

    def strip(arm, t):                                      # (the arm without the tail t - its lines stripped of
        body = [x for x in arm]                             #  their indentation - or None)
        while body and not body[-1].strip():
            body.pop()
        if len(body) >= len(t) and [x.strip() for x in body[-len(t):]] == t:
            return body[:-len(t)], True
        p = _if_parts(body, 0) if body else None
        if p and p[3] == len(body) and p[2] is not None:
            a, b = strip(p[1], t), strip(p[2], t)
            ind = re.match(r"\s*", body[0]).group(0)
            if a and b and (a[1] or b[1]) and (a[1] or leaves(p[1])) and (b[1] or leaves(p[2])):
                return [body[0]] + a[0] + [ind + "} else {"] + b[0] + [ind + "}"], True
        return (body, False) if leaves(body) else None

    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            continue
        th = [x for x in p[1] if x.strip()]
        best = None
        for n in range(1, 7):                               # (the longest tail every way shares)
            if len(th) < n or not re.match(r"\s*return\b", th[-1]):
                break
            t = [x.strip() for x in th[-n:]]
            if any(x.endswith("{") or x.startswith("}") or re.match(r"(?:L_\w+:|case\b|default:)", x) for x in t):
                break
            a, b = strip(p[1], t), strip(p[2], t)
            if a and b and b[1]:
                best = (t, a, b)
        if best:
            t, a, b = best
            ind = re.match(r"\s*", lines[k]).group(0)
            new = [lines[k]] + a[0] + [ind + "} else {"] + b[0] + [ind + "}"] + [ind + x for x in t]
            return "\n".join(lines[:k] + new + lines[p[3]:])
    return None


def goto_returns(text):
    """goto L; ... L: return x; - a jump to a plain return (the compiler's one copy of the return, the others
    jumping to it): the return in the goto's place, the label gone when nothing else jumps to it"""
    rets = dict(re.findall(r"(?m)^[ \t]*(L_\w+):\n[ \t]*(return(?: [\w]+| -?\d+)?;)$", text))
    if not rets:
        return text
    out = re.sub(r"(?m)^([ \t]*)goto (L_\w+);$", lambda m: m.group(1) + rets[m.group(2)] if m.group(2) in rets else m.group(0), text)
    for lab in rets:
        if not re.search(r"\bgoto %s;" % lab, out):
            out = re.sub(r"(?m)^[ \t]*%s:\n" % lab, "", out)
    return out


def char_widens(text):
    """a char local read in a test as (int)c (a variant: the compiler widens it, cbw, before comparing - or
    compares the byte, as the source asked)"""
    names = re.findall(r"(?m)^[ \t]*char (v\d+|[a-z]|r_\w+);", text)
    if not names:
        return None
    rx = re.compile(r"(?<![\w])(?<!\(char\))(%s)\b(?!\s*=[^=])" % "|".join(map(re.escape, names)))
    out = []
    for l in text.split("\n"):
        m = re.match(r"(\s*)(if|while|for|switch) \((.*)\)( \{| break;| continue;| return [^;]*;)?$", l)
        if m:
            l = m.group(1) + m.group(2) + " (" + rx.sub(r"(int)\1", m.group(3)) + ")" + (m.group(4) or "")
        out.append(l)
    t = "\n".join(out)
    return t if t != text else None


def tail_continues(text):
    """a continue last in the last statement of a loop's body (an if's arm closing it): nothing - the loop goes on
    there anyway (the compiler falls into the test; a continue is a jump to it)"""
    lines = text.split("\n")
    changed = True
    while changed:
        changed = False
        for k, l in enumerate(lines):
            if l.strip() != "continue;" or k + 1 >= len(lines):
                continue
            ind = len(l) - len(l.lstrip())
            j = k + 1                                       # (only closing braces after it, to the loop's own)
            while j < len(lines) and lines[j].strip() == "}":
                op = _opener(lines, j)
                if op is None:
                    break
                if re.match(r"\s*(?:while|for)\b.*\{$", lines[op]):
                    if j > k + 1:                            # (inside an if at the body's end, not the body's own)
                        del lines[k]
                        changed = True
                    break
                if not re.match(r"\s*(?:if \(.*\) \{|\} else \{)$", lines[op]):
                    break
                j += 1
            if changed:
                break
    return "\n".join(lines)


def assign_loops(text):
    """for (;;) { x = E; if (C(x)) break; S } - the value tested as it is given: while (!C((x = E))) { S }"""
    lines = text.split("\n")
    for k in range(len(lines) - 2):
        m = re.fullmatch(r"(\s*)for \(;;\) \{", lines[k])
        if not m:
            continue
        ind = m.group(1)
        a = re.fullmatch(r"%s    ([a-z]|v\d+|r_\w+) = ([^;]+);" % re.escape(ind), lines[k + 1])
        b = re.fullmatch(r"%s    if \((.+)\) break;" % re.escape(ind), lines[k + 2])
        if not a or not b or len(re.findall(r"\b%s\b" % a.group(1), b.group(1))) != 1:
            continue
        end = next((j for j in range(k + 3, len(lines)) if lines[j] == ind + "}"), None)
        body = lines[k + 3:end] if end else None
        if body is None or any(x.strip() == "continue;" and len(x) - len(x.lstrip()) == len(ind) + 4 for x in body):
            continue
        cond = re.sub(r"\b%s\b" % a.group(1), "(%s = %s)" % (a.group(1), a.group(2)), b.group(1), count=1)
        neg = _not("(%s)" % cond)
        lines[k:k + 3] = ["%swhile %s {" % (ind, neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg)]
        return "\n".join(lines)
    return None


def store_joins(text):
    """.. r = A; L: X = r; } .. } else { .. r = B; goto L; } - one store the compiler shares between the two arms of
    an if (the jump to it from the other): each arm its own, X = A and X = B (both arms ending there)"""
    lines = text.split("\n")
    for i in range(1, len(lines) - 1):
        m = re.fullmatch(r"\s*(L_\w+):", lines[i])
        st = re.fullmatch(r"(\s*)([\w\->.\[\]]+) = (ax|al|bx|cx|dx);", lines[i + 1]) if m else None
        a = re.fullmatch(r"\s*%s = ([^;]+);" % st.group(3), lines[i - 1]) if st else None
        if not a:
            continue
        lab = m.group(1)
        gs = [j for j, x in enumerate(lines) if x.strip() == "goto %s;" % lab]
        if len(gs) != 1:
            continue
        g = gs[0]
        b = re.fullmatch(r"\s*%s = ([^;]+);" % st.group(3), lines[g - 1])
        if not b:
            continue
        ol, og = _openers(lines, i), _openers(lines, g)
        common = next((x for x in ol if x in og), None)    # (the innermost block around both)
        if common is None:
            continue
        # (the two ways: the arms of one if below that block; after each store only blocks closing to it)
        def closes_to(k):
            j = k + 1
            while j < len(lines) and lines[j].strip() in ("}", "} else {"):
                if lines[j].strip() == "} else {":
                    return j
                j += 1
            return j
        el = next((j for j in range(min(i, g), max(i, g)) if lines[j].strip() == "} else {" and _opener(lines, j) in ol + og), None)
        if el is None:
            continue
        ends = (closes_to(i + 1), closes_to(g))
        # (a store at the very end of the text has no line after it to close to: not this way - it was an IndexError)
        if not all(e < len(lines) and (lines[e].strip() == "} else {" and e == el or len(lines[e]) - len(lines[e].lstrip()) <=
                   len(lines[_opener(lines, el)]) - len(lines[_opener(lines, el)].lstrip())) for e in ends):
            continue
        ind_s, ind_g = st.group(1), re.match(r"\s*", lines[g]).group(0)
        lines[g - 1:g + 1] = ["%s%s = %s;" % (ind_g, st.group(2), b.group(1))]
        if g < i:
            i -= 1
        lines[i - 1:i + 2] = ["%s%s = %s;" % (ind_s, st.group(2), a.group(1))]
        return store_joins("\n".join(lines))
    return text


def loop_collapse(text):
    """L: for (;;) { for (;;) { S } break; } - an outer loop only restarting the inner one (a goto L in S): the
    inner loop alone, the gotos its continues (the model's reading of a loop entered at a shared clean-up)"""
    lines = text.split("\n")
    for k in range(len(lines) - 3):
        m = re.fullmatch(r"(\s*)(L_\w+):", lines[k])
        if not m or lines[k + 1] != m.group(1) + "for (;;) {" or lines[k + 2] != m.group(1) + "    for (;;) {":
            continue
        ind, lab = m.group(1), m.group(2)
        e_in = next((j for j in range(k + 3, len(lines)) if lines[j] == ind + "    }"), None)
        if e_in is None or e_in + 2 >= len(lines) or lines[e_in + 1].strip() != "break;" or lines[e_in + 2] != ind + "}":
            continue
        body = lines[k + 3:e_in]
        gotos = [j for j, x in enumerate(body) if x.strip() == "goto %s;" % lab]
        def in_inner_loop(j):                               # (a loop of its own around the goto: a continue would be
            want = len(body[j]) - len(body[j].lstrip())     #  that loop's - a switch is no loop for a continue)
            for x in reversed(body[:j]):
                d = len(x) - len(x.lstrip())
                if x.strip() and d < want and x.rstrip().endswith("{"):
                    if re.match(r"\s*(?:while|for|do)\b", x) or x.strip() == "do {":
                        return True
                    want = d
            return False
        if not gotos or re.search(r"\bgoto %s;" % lab, "\n".join(lines[:k] + lines[e_in:])) or \
           any(in_inner_loop(j) for j in gotos):
            continue
        for j in gotos:
            body[j] = body[j].replace("goto %s;" % lab, "continue;")
        new = [ind + "for (;;) {"] + [x[4:] if x.startswith(ind + "        ") else x for x in body] + [ind + "}"]
        return "\n".join(lines[:k] + new + lines[e_in + 3:])
    return text


def switch_ifs(text):
    """switch (x) { default: S continue; case K: break; } last in a loop's body - one value passed over: if (x != K)
    { S } (the compiler's one compare either way)"""
    rx = re.compile(r"(?m)^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1    continue;\n\1case (-?\w+):\n\1    break;\n\1\}\n(\1[ \t]*\}|[ \t]*\})")

    def one(m):
        ind, sel, body, k, close = m.groups()
        if len(re.match(r"\s*", close).group(0)) >= len(ind):
            return m.group(0)
        return "%sif ((%s != %s)) {\n%s%s}\n%s" % (ind, sel, k, body, ind, close)
    text = rx.sub(one, text)
    # (switch (x) { case K: break; default: S } last: the value passed over first - if (x == K) continue; S)
    rx2 = re.compile(r"(?m)^([ \t]*)switch \((.+)\) \{\n\1case (-?\w+):\n\1    break;\n\1default:\n((?:\1    .*\n)+?)\1\}\n([ \t]*\})")

    def two(m):
        ind, sel, k, body, close = m.groups()
        if len(re.match(r"\s*", close).group(0)) >= len(ind) or re.search(r"(?m)^%s    break;$" % re.escape(ind), body):
            return m.group(0)
        dd = "".join(x[4:] + "\n" if x.startswith(ind + "    ") else x + "\n" for x in body.rstrip("\n").split("\n"))
        return "%sif ((%s == %s)) continue;\n%s%s" % (ind, sel, k, dd, close)
    text = rx2.sub(two, text)
    # (the same, the default first: switch (x) { default: S case K: break; } - S falls into the break)
    rx3 = re.compile(r"(?m)^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1case (-?\w+):\n\1    break;\n\1\}\n([ \t]*\})")

    def three(m):
        ind, sel, body, k, close = m.groups()
        return two(type("M", (), {"groups": lambda s_: (ind, sel, k, body, close), "group": lambda s_, i=0: m.group(0)})())
    return rx3.sub(three, text)


def ternary_joins(text):
    """if (C) { r = A; L: V = r; S } else { r = B; goto L; } - the value of a ?: the compiler gives in each arm,
    the two meeting at the store: V = C ? A : B; S"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        p = _if_parts(lines, k)
        if not p or p[2] is None or len(p[1]) < 3:
            k += 1
            continue
        a = re.fullmatch(r"(\s*)(ax|al|bx|cx|dx) = ([^;]+);", p[1][0])
        lab = re.fullmatch(r"\s*(L_\w+):", p[1][1]) if a else None
        st = re.fullmatch(r"\s*([\w\->.\[\]]+) = %s;" % a.group(2), p[1][2]) if lab else None
        el = [x.strip() for x in p[2] if x.strip()]
        if not st or len(el) != 2:
            k += 1
            continue
        b = re.fullmatch(r"%s = ([^;]+);" % a.group(2), el[0])
        if not b or el[1] != "goto %s;" % lab.group(1) or \
           len(re.findall(r"\bgoto %s;" % lab.group(1), "\n".join(lines))) != 1:
            k += 1
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        c = p[0] if p[0].startswith("(") else "(%s)" % p[0]
        rest = [x[4:] if x.startswith(ind + "    ") else x for x in p[1][3:]]
        lines[k:p[3]] = ["%s%s = (%s ? %s : %s);" % (ind, a.group(2), c, a.group(3), b.group(1)),   # (the register
                         "%s%s = %s;" % (ind, st.group(1), a.group(2))] + rest                     #  still holds it)
    return "\n".join(lines)


def stored_values(text):
    """V = ax; .. f(.., ax) - the register read again while it still holds what was stored from it (no write to
    it between, no call, no label - a way in from elsewhere): V there"""
    # (inc al; jne: a byte compared with -1 - ((r + 1) & 0xff) == 0 is (char)r == -1)
    text = re.sub(r"\(\(\((\w+) \+ 1\) & 0xff\) ([!=]=) 0\)", r"((char)\1 \2 -1)", text)
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"\s*([a-z]|v\d+|g_[0-9a-f]+|r_\w+) = (ax|al);", l)
        if not m:
            continue
        v, r = m.group(1), m.group(2)
        rx = re.compile(r"\b(?:ax|al|ah)\b" if r == "ax" else r"\b(?:al)\b")
        for j in range(k + 1, len(lines)):
            y = lines[j]
            if re.match(r"\s*L_\w+:", y) or re.match(r"\s*(?:case\b|default:|\} else\b)", y):
                break
            w = re.match(r"(\s*)(?:ax|al|ah) = (.*)$", y)
            if w:                                           # (written: its value read first, then no more)
                if rx.search(w.group(2)) and r == "ax":
                    lines[j] = y[:len(y) - len(w.group(2))] + re.sub(r"\bax\b", v, w.group(2))
                break
            if rx.search(y):
                if r == "ax" and re.search(r"\b(?:al|ah)\b", y):
                    break
                lines[j] = re.sub(r"\b%s\b" % r, v, y)
            calls = [c for c in re.findall(r"\b([A-Za-z_]\w*)\s*\(", y) if c not in ("if", "while", "for", "switch", "sizeof", "return")]
            if calls or re.search(r"\(\*[^()]+\)\s*\(", y) or \
               re.search(r"\b%s\s*=[^=]" % re.escape(v), y) or re.match(r"\s*(?:return|break|continue|goto)\b", y):
                break                                       # (a call - it gives ax - or V given again: done)
    return "\n".join(lines)


def two_case_switches(text):
    """r = E; if ((r != K1)) { if ((r == K2)) { L: S } } else { goto L; } - two values to one arm, the compiler's
    compares of a switch: switch (E) { case K1: case K2: S }"""
    lines = text.split("\n")
    for k in range(len(lines) - 4):
        a = re.fullmatch(r"(\s*)(ax|al|bx|cx|dx) = ([^;]+);", lines[k])
        if not a:
            continue
        ind, r, e = a.groups()
        p = _if_parts(lines, k + 1)
        c1 = re.fullmatch(r"\(\(%s != (-?\d+)\)\)" % r, "(" + p[0] + ")") if p else None
        if not c1 or p[2] is None or [x.strip() for x in p[2] if x.strip()][:1] != [x.strip() for x in p[2] if x.strip()] or \
           not re.fullmatch(r"goto (L_\w+);", "".join(x.strip() for x in p[2])):
            continue
        lab = re.fullmatch(r"goto (L_\w+);", "".join(x.strip() for x in p[2])).group(1)
        q = _if_parts(p[1], 0) if p[1] else None
        c2 = re.fullmatch(r"\(\(%s == (-?\d+)\)\)" % r, "(" + q[0] + ")") if q else None
        if not c2 or q[2] is not None or q[3] != len(p[1]) or not q[1] or q[1][0].strip() != lab + ":":
            continue
        body = [x[8:] if x.startswith(ind + "        ") else x for x in q[1][1:]]
        if re.search(r"\bgoto %s;" % lab, "\n".join(lines[:k] + body + lines[p[3]:])) or \
           any(x.strip() == "break;" for x in body) or re.search(r"\b%s\b" % r, "\n".join(body)):
            continue
        new = ["%sswitch (%s) {" % (ind, e if not e.startswith("(") or not _balanced(e[1:-1]) else e[1:-1]),
               "%scase %s:" % (ind, c1.group(1)), "%scase %s:" % (ind, c2.group(1))] + \
              ["    " + x if x.strip() else x for x in body] + ["%s}" % ind]
        lines[k:p[3]] = new
        return "\n".join(lines)
    return text


def long_tests(text):
    """ax = E; X = ax; X' = dx; ax = (ax | dx); if ((ax == 0)) - a long given and tested at once: if ((X = E) == 0)
    (X a long field's, a long of the data's or a local's two words)"""
    def one(m):
        ind, e, lo, hi, op, rest = m.groups()
        x = _pair_long(lo, hi)
        if not x or x.startswith("(long)"):
            return m.group(0)
        return "%sif (((%s = %s) %s 0))%s" % (ind, x, e, op, rest)
    return re.sub(r"(?m)^([ \t]*)ax = ([^;\n]+);\n[ \t]*([\w\->.]+) = ax;\n[ \t]*([\w\->.]+) = dx;\n[ \t]*ax = \(ax \| dx\);\n"
                  r"[ \t]*if \(\(ax ([!=]=) 0\)\)(.*)$", one, text)


def _pair_long(lo, hi):
    """the long two word expressions are, the low and the high (a long field's, a long of the data's, a long
    local's two words, an int and its sign spread), or None"""
    f, g = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", lo), re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", hi)
    if f and g and f.group(1) == g.group(1) and int(g.group(2)) == int(f.group(2)) + 2:
        LONGINFO.setdefault("fields", set()).add(int(f.group(2)))
        return "*(long *)&%s->f%s" % (f.group(1), f.group(2))
    f, g = re.fullmatch(r"g_([0-9a-f]+)", lo), re.fullmatch(r"g_([0-9a-f]+)", hi)
    if f and g and int(g.group(1), 16) == int(f.group(1), 16) + 2:
        LONGINFO.setdefault("globals", set()).add("g_%04x" % int(f.group(1), 16))
        return lo
    f, g = re.fullmatch(r"v(\d+)", lo), re.fullmatch(r"v(\d+)", hi)
    if f and g and int(g.group(1)) == int(f.group(1)) - 2:
        return "*(long *)&%s" % lo
    if re.fullmatch(r"\(?0 - \(%s >> 15\)\)?" % re.escape(lo), hi) and re.fullmatch(r"[\w\->.\[\]]+", lo):
        return "(long)%s" % lo
    return None


def sunk_pairs(text):
    """if (C) { .. ax = E; } else { .. ax = L; dx = H; } X = ax; X' = dx; - a long both arms leave in dx:ax,
    stored a word at a time after them (a long field's words, a long of the data's, a long local's): the long
    stored in each arm (E a long already - a double converted, a long cast - or the long L and H are)"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        p = _if_parts(lines, k)
        k += 1
        if not p or p[2] is None or p[3] + 1 >= len(lines):
            continue
        st = re.fullmatch(r"(\s*)([\w\->.]+) = ax;", lines[p[3]])
        st2 = re.fullmatch(r"\s*([\w\->.]+) = dx;", lines[p[3] + 1])
        dst = _pair_long(st.group(2), st2.group(1)) if st and st2 else None
        if not dst or dst.startswith("(long)"):
            continue
        if _live_after(lines, p[3] + 1, "ax", r"\b(?:ax|al|ah|dx)\b"):
            continue

        def value(arm):                                     # (the long an arm leaves, and the arm without it)
            body = [x for x in arm if x.strip()]
            if len(body) >= 2:
                a = re.fullmatch(r"\s*(ax|dx) = ([^;]+);", body[-2])
                b = re.fullmatch(r"\s*(ax|dx) = ([^;]+);", body[-1])
                if a and b and a.group(1) != b.group(1):
                    lo, hi = (a, b) if a.group(1) == "ax" else (b, a)
                    v = _pair_long(lo.group(2), hi.group(2))
                    if v:
                        return v, arm[:arm.index(body[-2])]
            if body:
                e = re.fullmatch(r"\s*ax = (.+);", body[-1])
                if e and re.search(r"\(double \*\)|\(float \*\)|\(long\)|\(double\)", e.group(1)) and \
                   not any(re.match(r"\s*dx = ", x) for x in body[-3:]):
                    return e.group(1), arm[:arm.index(body[-1])]
            return None
        a, b = value(p[1]), value(p[2])
        if not a or not b:
            continue
        ind = st.group(1)
        same = lambda v: v == dst or v.replace("*(long *)&", "") == dst.replace("*(long *)&", "")
        if not any(x.strip() for x in a[1] + b[1]) and (same(a[0]) or same(b[0])):
            c = p[0] if p[0].startswith("(") else "(%s)" % p[0]   # (one way the long kept: the other's value or it -
            new = [ind + "%s = %s ? %s : %s;" % (dst, c, a[0], b[0])]   #  a ?:)
        else:
            new = [lines[k - 1]] + a[1] + [ind + "    %s = %s;" % (dst, a[0])] + [ind + "} else {"] + b[1] + \
                  [ind + "    %s = %s;" % (dst, b[0])] + [ind + "}"]
        lines[k - 1:p[3] + 2] = new
    return "\n".join(lines)


def sunk_tails(text):
    """if (C) { .. r = E1; } else { .. r = E2; } S(r); - a statement both arms ended with, the compiler's one copy
    of it after them (cross-jumped), the value it reads given in each: S in each arm, S(E1) and S(E2)"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        p = _if_parts(lines, k)
        k += 1
        if not p or p[2] is None or not p[1] or not p[2] or p[3] >= len(lines):
            continue
        ind = re.match(r"\s*", lines[k - 1]).group(0)
        a = re.fullmatch(r"(\s*)(ax|al|bx|cx|dx) = ([^;]+);", p[1][-1])
        b = re.fullmatch(r"(\s*)(ax|al|bx|cx|dx) = ([^;]+);", p[2][-1])
        s = lines[p[3]]
        if not a or not b or a.group(2) != b.group(2) or not s.startswith(ind) or s[len(ind):len(ind) + 1].isspace():
            continue
        r = a.group(2)
        if not re.fullmatch(r"\s*[^;{}]+;", s) or re.fullmatch(r"\s*[\w\->.\[\]*]+ = %s;" % r, s) or re.match(r"\s*(?:return|goto|break|continue|case|default)\b", s) or \
           len(re.findall(r"\b%s\b" % r, s)) != 1 or _live_after(lines, p[3], r, r"\b(?:ax|al|ah)\b" if r in ("ax", "al") else None):
            continue
        if any(re.search(r"\b%s\b" % r, x) for x in p[1][:-1] + p[2][:-1]):
            continue
        put = lambda e: e if re.fullmatch(r"[\w\->.\[\]]+|[\w]+\(.*\)", e) else "(%s)" % e
        ia, ib = a.group(1), b.group(1)
        new = [lines[k - 1]] + p[1][:-1] + [ia + re.sub(r"\b%s\b" % r, lambda _: put(a.group(3)), s.strip())] + \
              [ind + "} else {"] + p[2][:-1] + [ib + re.sub(r"\b%s\b" % r, lambda _: put(b.group(3)), s.strip())] + [ind + "}"]
        lines[k - 1:p[3] + 1] = new
    return "\n".join(lines)


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
            sets_r = re.match(r"\s*(?:%s|%sl|%sh)\s*=[^=]" % (r, r[0], r[0]), y)   # (or a half of it)
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
            u = re.match(r"\s*%s = %s ([-+*&|^]|<<|>>) (.+);\s*$" % (x, x), lines[k + 1]) or \
                re.match(r"\s*%s = \(%s ([-+*&|^]|<<|>>) ([^()]+)\);\s*$" % (x, x), lines[k + 1])   # (or in parentheses)
            if u and not re.search(r"\b%s\b" % x, e + u.group(2)) and not re.match(r"\s*(?:if|while|return)\b", lines[k]):
                f = u.group(2)
                if re.search(r" [-+*/%&|^<>]{1,2} ", e) and not (e.startswith("(") and e.endswith(")") and _balanced(e[1:-1])):
                    e = "(%s)" % e                          # (E an expression of its own: kept one, whatever op)
                lines[k + 1] = "%s%s = %s %s %s;" % (ind, x, e, u.group(1), f)
                k += 1                                      # (and on: the next step of it too)
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
            elif q and q[3] == len(body) and q[2] and len(q[2]) >= 3 and re.fullmatch(i4 + "    (?:%s)" % st, q[2][-2]) and \
                    q[2][-1] == i4 + "    continue;" and re.match(r"\s*(?:break|return)\b", q[1][-1]) and \
                    i4 + "continue;" not in own and not any(x.strip() == "continue;" for x in q[2][:-1]):
                step = q[2][-2].strip()                     # (if (C) { ..leaves.. } else { S; X = X + K; continue; })
                body = body[:j0 + 1] + q[1] + [i4 + "}"] + [x[4:] for x in q[2][:-2]]
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
            uses = [j for j in range(len(lines)) if j != k and re.search(r"\b%s\b" % x, lines[j]) and
                    not re.match(r"[ \t]*(?:int|char|long|unsigned|register)\b", lines[j])]
            nd = next((j for j in range(k + 1, len(lines)) if re.match(r"\s*%s = " % x, lines[j])), len(lines))
            mine = [j for j in uses if k < j < nd]          # (this value's reads: up to where it is given again)
            if mine == [k + 1] and not any(re.search(r"\b%s\b" % x, lines[j]) for j in range(nd + 1, len(lines))
                                           if j in uses and not any(re.match(r"\s*%s = " % x, lines[q]) for q in range(nd, j))):
                uses = [k + 1]                              # (read once before it is given again: that value's)
            if uses == [k + 1] and len(re.findall(r"\b%s\b" % x, lines[k + 1])) == 1:
                e = m.group(3)
                lines[k + 1] = re.sub(r"\b%s\b" % x, lambda _: e if re.fullmatch(r"[\w\[\]\->.]+", e) else "(%s)" % e, lines[k + 1])
                del lines[k]
                continue
        k += 1
    lines = [l for l in lines if not re.fullmatch(r"[ \t]*int t\d+(?:, t\d+)*;", l)]   # (declared again below: the
    text = "\n".join(lines)                                                         #  ones still read)
    left = sorted(set(re.findall(r"\b(t\d+)\b", text.split("{", 1)[-1])))
    if left:
        head, sep, body = split_body(text)
        if sep:
            text = head + sep + "\n    int %s;" % ", ".join(left) + body
    return text


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
            upd = re.fullmatch(r"(\s*)([a-z]|v\d+|r_\w+|si|di) = \2 ([-+&|^]) %s;\s*" % reg, nxt)
            if upd and reg not in ("si", "di") and re.fullmatch(r"[A-Za-z_]\w*\(.*\)", expr) and \
               not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, later)[0]) and \
               not re.search(r"\b%s\b" % upd.group(2), expr.split("(", 1)[0]):
                j = lines.index(nxt, k + 1)                 # (a call's value added to a local, a parameter: the local
                lines[j] = "%s%s = %s %s %s;" % (upd.group(1), upd.group(2), upd.group(2), upd.group(3), expr)   # updated by it)
                continue
            if not pure and not re.search(r"\w\s*\(|\+\+|--|[^=!<>]=[^=]", expr) and re.fullmatch(r"!?\(.*\)", expr):
                expr, pure = "(%s)" % expr, True       # (a test's value: no call, no store)
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
