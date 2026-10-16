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
    rx_and = re.compile(r"(?m)^(\s*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\4 = 0;\n\s*\}\n"
                        r"\s*\} else \{\n\s*\4 = 0;\n\s*\}$")
    text = rx_and.sub(lambda m: "%s%s = %s && %s;" % (m.group(1), m.group(4), _truth(m.group(2)), _truth(m.group(3))), text)
    return text


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
            st = re.match(r"(\s*)(.+?) = %s;\s*$" % reg, lines[k + 1])
            rest = "\n".join(lines[k + 2:])
            if st and not re.search(r"\b%s\b" % reg, st.group(2)) and not re.search(r"\b%s\b" % reg, re.split(r"\n\s*%s = " % reg, rest)[0]):
                out.append("%s%s = %s;" % (st.group(1), st.group(2), call))
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
    head, body = text.split("{", 1)
    lines = body.split("\n")
    decls = []
    for k, l in enumerate(lines[1:], 1):
        m = re.match(r"\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short)\b)\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$", l)
        if not m:
            if l.strip():
                break
            continue
        reg, ty, star, name, cnt = m.groups()
        size = 2 if star else SIZES.get(ty.split()[-1], 2) * int(cnt or 1)
        decls.append(dict(name=name, reg=bool(reg), size=max(size, 2) if not cnt else size + (size & 1)))
    autos = sorted((d for d in decls if not d["reg"] and re.fullmatch(r"v\d+", d["name"])), key=lambda d: int(d["name"][1:]))
    if not autos:
        return None
    fill = {d["name"]: d for d in decls if re.fullmatch(r"_p\d+", d["name"])}
    regs = [d for d in decls if d["reg"]]
    order = []
    cum = 0
    for d in autos:                                         # (each auto at its place; what lies between: register
        n = int(d["name"][1:])                              #  locals, then a filler)
        gap = n - d["size"] - cum
        while gap >= 2 and regs:
            order.append(regs.pop(0))
            gap -= 2
        if order and order[-1]["name"] in ("v%d" % cum,) and "_p%d" % cum in fill:
            pass
        if "_p%d" % cum in fill:
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
    if not new:
        return None
    for a, b in new.items():
        body = re.sub(r"\b%s\b" % a, b, body)
    return head + "{" + body


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
            pure = re.fullmatch(r"[A-Za-z_]\w*|-?\d+", expr) or (     # (or a value with no effect: no call, no
                not re.search(r"\w\s*\(|\+\+|--|[^=!<>]=[^=]", expr) and expr.startswith("(") and expr.endswith(")"))   # store)
            names = re.findall(r"[A-Za-z_]\w*", expr)
            if pure and len(re.findall(r"\b%s\b" % reg, nxt)) == 1 and \
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
                if "(" in expr and out and out[-1].strip() == expr + ";":
                    continue                                # (the call written twice)
                if "(" in expr:
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
    uses = {}
    for p, off in re.findall(r"\b(\w+)->f(\d+)\b", text):
        uses.setdefault(p, set()).add(int(off))
    if not uses:
        return text
    defs = []
    for p, offs in sorted(uses.items()):
        fields = []
        pos = 0
        for off in sorted(offs):
            if off < pos:
                continue                                    # (overlaps the field before: not this way)
            if off > pos:
                fields.append("char _%d[%d];" % (pos, off - pos))
            byte = (p, off) in widths
            fields.append("%s f%d;" % ("char" if byte else "int", off))
            pos = off + (1 if byte else 2)
        tag = "s_" + p
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
        t = fold_registers(stored_calls(post_increments(widen_bytes(compare_values(branch_returns(logical_values(t)))))))
        t = test_first_loops(post_steps(t))
        t = structs(register_variables(shapes(t), regs, regparam), set(widths))
        variants = [t] + [v for v in [store_increments(t)] if v]
        rule = prj.toolchain().p.get("frame_order")
        for v in list(variants):                            # (the names the frame's order asks for)
            fn = frame_names(v, rule) if rule else None
            variants.insert(variants.index(v) + 1, fn) if fn else None
        out += [localize(prj, v, at, names) for v in variants]
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
    return {(p, int(o)) for p, o in re.findall(r"BYTE\(\s*\(?(\w+)->f(\d+)\)?\s*\)", t)}


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
    head, sep, body = text.partition("{")
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
    used = [r for r in regs if re.search(r"\b%s\b" % r, body)]
    if not used:
        return head + sep + body
    decls = []
    for r in used:
        ptr = re.search(r"\*\s*%s\b|\b%s\s*\[" % (r, r), body)
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
    body = text.split("{", 1)[1] if "{" in text else text
    # registers the model left: a draft for a person, not for the compiler
    left = sorted(set(re.findall(REGS, re.sub(r'"(?:[^"\\]|\\.)*"', "", body))))
    data = sorted(set(re.findall(r"\bg_[0-9a-f]{4}\b", text)))
    head = text.split("(", 1)[0].strip()
    funcs = sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body)) - {head, "if", "while", "for", "switch", "return", "sizeof"})
    decl = []
    fptr = [d for d in data if re.search(r"\(\s*\*\s*%s\s*\)\s*\(" % d, body)]     # (data called through)
    for d in fptr:                                          # (a number given it that is a function's place: it)
        text = re.sub(r"(?m)^(\s*%s = )(\d+);" % d, lambda m: m.group(1) + (names.get(int(m.group(2))) or
                                                                             "f%05x" % int(m.group(2))) + ";", text)
        body = text.split("{", 1)[1] if "{" in text else text
    funcs = sorted(set(funcs) | {w for d in fptr for w in re.findall(r"(?m)^\s*%s = ([A-Za-z_]\w*);" % d, body)})
    ptrs = [d for d in data if d not in fptr and re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:->|\[)" % (d, d), body)]
    ints = [d for d in data if d not in ptrs and d not in fptr]
    if fptr:
        decl.append("extern int %s;" % ", ".join("(*%s)()" % d for d in fptr))
    if ptrs:                                                # (data the draft goes through: pointers)
        for d in ptrs:
            decl.append("extern %s *%s;" % ("struct " + STRUCTPTR[d] if d in STRUCTPTR else "char", d))
    if ints:
        decl.append("extern int %s;" % ", ".join(ints))
    if funcs:
        decl.append("extern %s;" % ", ".join("%s()" % f for f in funcs))
    return ("\n".join(decl) + "\n\n" if decl else "") + text, left


def _prove(prj, tc, src, a, ln):
    raw, con = tc.compile(src.encode("latin1"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    if raw is None:
        last = [l for l in con.strip().splitlines() if "(the pass" not in l]
        return "does not compile: %s" % (last[-1][:100] if last else "")
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


def prove_texts(prj, tc, inv, texts, a, ln, names):
    """-> (source, verdict): of each of the model's texts [(text, register parameters)] and their variants, the
    first the compiler proves identical; else the best of them (one that differs before one that does not
    compile, before one with registers left)"""
    best = None
    for text, regparam in texts:
        src, v = prove_best(prj, tc, inv, text, a, ln, names, regparam)
        if v == "identical":
            return src, v
        rank = 0 if v.startswith("differs") else 1 if v.startswith("does not") else 2
        if best is None or rank < best[0]:
            best = (rank, src, v)
    return best[1], best[2]


def prove_best(prj, tc, inv, text, a, ln, names, regparam=None):
    """-> (source, verdict): of the model's text and its variants, the first the compiler proves identical; else
    the first that compiles (or the first)"""
    regs = tc.p.get("carve", {}).get("register_variables", ["si", "di"])
    if regparam is None:
        regparam = dict(getattr(inv, "LAST", {}).get("regparam") or {})
    widths = byte_fields(inv, bytes(prj.image[a:a + ln]))
    cands = candidates(prj, text, a, names, regs, regparam, widths)
    best = None
    for src, left in cands:
        if left:
            v = "registers left: %s" % " ".join(left)
        else:
            v = _prove(prj, tc, src, a, ln)
            if v != "identical":
                for alt in string_variants(prj, src):       # (a number that is a string's place: the string)
                    if _prove(prj, tc, alt, a, ln) == "identical":
                        src, v = alt, "identical"
                        break
        if v == "identical":
            return src, v
        rank = 0 if v.startswith("differs") else 1 if v.startswith("does not") else 2
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


def model_texts(inv, code, name, base=None):
    """[(text, register parameters)] - the model's text of a function, from each of its structurers (a linear walk;
    a structured control-flow graph, where the model has one): the compiler decides between them"""
    out = []
    fs, params, stmts, pt, loc = inv.recover(code)
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
        src, verdict = prove_texts(prj, tc, inv, texts, a, ln, names)
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
