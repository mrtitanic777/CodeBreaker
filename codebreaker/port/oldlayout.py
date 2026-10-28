"""oldlayout.py -- the records a program keeps as bytes, kept in their old layout.

A compiler of the 8086 writes its records to its own files as they lie in memory: `p = (char *)&e->e_v`, then the
bytes, so many of them; it reads them back the same way and goes on. On the 8086 a pointer in such a record was two
bytes, and the bytes of one member were the bytes of another (a union: a long here, two pointers there). Laid out
anew - pointers of eight bytes - the counts no longer fit and the overlaps are gone.

So those records keep the layout they had: the program's own ints 16 bits, its longs 32, its pointers two bytes - a
handle (dos_ph) that stands for the pointer, turned back into it (dos_hp) wherever the program reads the member. A
record is one of them when the program takes it as bytes - `(char *)&x`, `(char *)&x.member`, `(char *)p` of a pointer
to it. Every read and write of a pointer member is found in clang's tree of the port (-ast-dump=json, as arith16
does) and given the conversion:

  r = e->e_sym          r = ((struct sym *)dos_hp(e->e_sym))
  e->e_sym = s          e->e_sym = dos_ph(s)

and the members are declared uint16_t in the record. A value of the program's that is no address - a symbol's
number put where its pointer was - stays the number (dos_ph keeps what is below 8000h as it is).
"""
import collections
import os
import re

from codebreaker.base import cache
from codebreaker.port import arith16


def _walk(node, fn, parents=()):
    if isinstance(node, dict):
        fn(node, parents)
        for v in node.get("inner", []) or []:
            _walk(v, fn, parents + (node,))
    elif isinstance(node, list):
        for v in node:
            _walk(v, fn, parents)


def _records(tree):
    """{RecordDecl id: (name, [FieldDecl ids], parent record id)}; {FieldDecl id: (record id, name, type)}"""
    recs, fields = {}, {}

    def fn(n, parents):
        if n.get("kind") == "RecordDecl" and n.get("completeDefinition"):
            par = next((p.get("id") for p in reversed(parents) if p.get("kind") == "RecordDecl"), None)
            name = n.get("name") or ""
            tag = ("%s %s" % (n.get("tagUsed", "struct"), name)) if name else ""
            recs[n["id"]] = (tag, [x["id"] for x in n.get("inner", []) or [] if x.get("kind") == "FieldDecl"], par)
            for x in n.get("inner", []) or []:
                if x.get("kind") == "FieldDecl":
                    fields[x["id"]] = (n["id"], x.get("name", ""), (x.get("type") or {}).get("qualType", ""))
    _walk(tree, fn)
    return recs, fields


def _top(recs, rid):
    """the named record a record is inside (itself when named)"""
    seen = set()
    while rid in recs and not recs[rid][0] and recs[rid][2] and rid not in seen:
        seen.add(rid)
        rid = recs[rid][2]
    return recs.get(rid, ("",))[0]


def _rtype(t):
    """'struct expr *' -> 'struct expr'; 'union (unnamed at ...)' -> None"""
    t = re.sub(r"\s+", " ", (t or "").replace("const ", "")).strip()
    m = re.fullmatch(r"((?:struct|union) \w+)(?: \*)?", t)
    return m.group(1) if m else None


def callee_name(call):
    """the name of the function a CallExpr calls (through its decay to a pointer, parentheses) - or ''"""
    c = ((call or {}).get("inner") or [{}])[0]
    while c.get("kind") in ("ImplicitCastExpr", "ParenExpr") and c.get("inner"):
        c = c["inner"][0]
    return (c.get("referencedDecl") or {}).get("name", "") if c.get("kind") == "DeclRefExpr" else ""


def byte_records(trees):
    """the named records the program takes as bytes: (char *)&x, (char *)&x.m, (char *)p"""
    out = set()

    def fn(n, parents):
        if n.get("kind") in ("ImplicitCastExpr", "CStyleCastExpr") and \
                (n.get("type") or {}).get("qualType", "").replace(" ", "") in ("void*", "constvoid*"):
            x = arith16.bare((n.get("inner") or [{}])[0])   # (read(fd, p, 12): a record's pointer given as the
            t = (x.get("type") or {}).get("qualType", "")   #  bytes to fill or write)
            call = next((p for p in reversed(parents) if p.get("kind") == "CallExpr"), None)
            name = callee_name(call)
            if t.endswith("*") and _rtype(t) and re.fullmatch(r"(?:dos_)?(?:read|write|fread|fwrite)", name):
                out.add(_rtype(t))
            return
        if n.get("kind") != "CStyleCastExpr" or not (n.get("type") or {}).get("qualType", "").replace(" ", "") in (
                "char*", "unsignedchar*"):
            return
        x = arith16.bare((n.get("inner") or [{}])[0])
        if x.get("kind") == "ImplicitCastExpr":
            x = arith16.bare((x.get("inner") or [{}])[0])
        if x.get("kind") == "UnaryOperator" and x.get("opcode") == "&":
            y = arith16.bare((x.get("inner") or [{}])[0])
            if y.get("kind") == "MemberExpr":               # (the bytes of one number - (char *)&p->type: its
                mt = (y.get("type") or {}).get("qualType", "")   #  bytes the same in either layout)
                if not (_rtype(mt) or "[" in mt or mt.startswith(("struct", "union"))):
                    return
            while y.get("kind") == "MemberExpr":            # (a member of a record: the record it is in)
                base = arith16.bare((y.get("inner") or [{}])[0])
                bt = _rtype((base.get("type") or {}).get("qualType"))
                if bt:
                    out.add(bt)
                    return
                y = base
            t = _rtype((y.get("type") or {}).get("qualType"))
            if t:
                out.add(t)
        else:                                               # ((char *)h given to read or write: its bytes)
            t = (x.get("type") or {}).get("qualType", "")
            call = next((p for p in reversed(parents) if p.get("kind") == "CallExpr"), None)
            name = callee_name(call)
            if t.endswith("*") and _rtype(t) and re.fullmatch(r"(?:dos_)?(?:read|write|fread|fwrite)", name):
                out.add(_rtype(t))
    for tree in trees:
        _walk(tree, fn)
    return out


PTR_PARAMS = {}                                         # ({old-style function the port keeps: [whether each
                                                        #  parameter is a pointer]} - set by the port's pass)


def edits_of(tree, text, path, olds):
    """[(offset, depth, text)] - every read and write of a pointer member of the records kept old; and the
    {(record, member): its pointer type} found"""
    recs, fields = _records(tree)
    ptrf = {}
    for fid, (rid, name, t) in fields.items():
        if _top(recs, rid) in olds and t.rstrip().endswith("*") and "(" not in t:
            ptrf[fid] = t
    arith16.MAIN[0] = path
    arith16.TEXT[0] = text
    out, found, refused = [], {}, []

    intf = {fid for fid, (rid, name, t) in fields.items()   # (its int members: a pointer kept in one, a word)
            if _top(recs, rid) in olds and re.fullmatch(r"(?:unsigned |signed )?(?:short|int|u?int16_t|intptr_t)( int)?",
                                                         t.strip())}   # (intptr_t: a word declare makes one again)

    def tptr(x):
        return (x.get("type") or {}).get("qualType", "")

    def int_word(n, parents):
        """an int member of a record kept old given a pointer, or read as one: the handle of it"""
        par = parents[-1] if parents else {}
        k = par.get("kind")
        p = place(n, path)
        if not p:
            return
        depth = len(parents)
        if k == "BinaryOperator" and par.get("opcode") == "=" and (par.get("inner") or [None])[0] is n:
            rhs = par["inner"][1]
            while (rhs.get("kind") == "ParenExpr" or rhs.get("kind") == "ImplicitCastExpr" and   # (n->i2 =
                   rhs.get("castKind") == "IntegralCast") and rhs.get("inner"):   #  (intptr_t)(f()): to 16 bits too)
                rhs = rhs["inner"][0]
            if rhs.get("kind") == "ImplicitCastExpr" and rhs.get("castKind") == "PointerToIntegral" or \
                    rhs.get("kind") == "CStyleCastExpr" and rhs.get("castKind") == "PointerToIntegral":
                q = place(rhs, path)
                if rhs.get("kind") == "CStyleCastExpr" and rhs.get("inner"):   # ((intptr_t)(f()): the pointer
                    q = place(rhs["inner"][0], path) or q   #  inside the cast made the handle)
                if q:                                       # (a pointer given it: its handle)
                    out.append((q[0], depth, "dos_ph("))
                    out.append((q[1], -depth, ")"))
                    gp = next((x for x in reversed(parents[:-1]) if x.get("kind") != "ParenExpr"), {})
                    w = place(par, path)                    # (the value of it as a pointer: the pointer again)
                    if w and gp.get("castKind") == "IntegralToPointer":
                        out.append((w[0], depth - 1, "dos_hp("))
                        out.append((w[1], -(depth - 1), ")"))
            return
        up = next((x for x in reversed(parents) if not (x.get("kind") == "ParenExpr" or
                   x.get("kind") == "ImplicitCastExpr" and x.get("castKind") in ("LValueToRValue", "IntegralCast"))), {})
        if up.get("kind") in ("ImplicitCastExpr", "CStyleCastExpr") and up.get("castKind") == "IntegralToPointer":
            out.append((p[0], depth, "dos_hp("))           # (read as a pointer: what the handle stands for)
            out.append((p[1], -depth, ")"))
            return
        if up.get("kind") == "BinaryOperator" and up.get("opcode") == "=" and len(up.get("inner") or []) == 2 and \
                up["inner"][0] is not n and up["inner"][0].get("kind") == "DeclRefExpr" and \
                re.sub(r"\s+", "", tptr(up["inner"][0])) in ("intptr_t", "longlong"):   # (g_74be = n->i2, the
            out.append((p[0], depth, "(intptr_t)dos_hp("))  #  variable one the port made hold pointers: the
            out.append((p[1], -depth, ")"))                 #  pointer the handle stands for)
            return
        if up.get("kind") == "CallExpr" and PTR_PARAMS:     # (given to an old-style function's parameter that is
            args = (up.get("inner") or [])[1:]              #  a pointer - no conversion said so: the definition's
            at = next((i for i, x in enumerate(parents) if x is up), None)   # parameter does)
            chain = list(parents[at + 1:]) + [n] if at is not None else []
            arg = chain[0] if chain else None
            k = next((i for i, a in enumerate(args) if a is arg), None)
            ps = PTR_PARAMS.get(callee_name(up))
            if k is not None and ps and k < len(ps) and ps[k]:
                out.append((p[0], depth, "dos_hp("))
                out.append((p[1], -depth, ")"))

    def fn(n, parents):
        if n.get("kind") != "MemberExpr":
            return
        fid = (n.get("referencedMemberDecl") or "")
        if fid in intf:
            int_word(n, parents)
            return
        if fid not in ptrf:
            return
        p = place(n, path)
        if not p:
            return
        t = ptrf[fid]
        found[(_top(recs, fields[fid][0]), fields[fid][1])] = t
        depth = len(parents)
        par = parents[-1] if parents else {}
        k = par.get("kind")
        if k == "BinaryOperator" and par.get("opcode") == "=" and (par.get("inner") or [None])[0] is n:
            rhs = par["inner"][1]
            q = place(rhs, path)
            if q:                                           # (a write: the handle of what is given)
                out.append((q[0], depth, "dos_ph("))
                out.append((q[1], -depth, ")"))
                gp = parents[-2] if len(parents) > 1 else {}
                w = place(par, path)
                gk, gi = gp.get("kind"), gp.get("inner") or []
                cond = {"IfStmt": 0, "WhileStmt": 0, "SwitchStmt": 0, "DoStmt": 1, "ForStmt": 2}.get(gk)
                used = gk not in ("CompoundStmt", "CaseStmt", "DefaultStmt", "LabelStmt", None) and \
                    (cond is None or (len(gi) > cond and gi[cond] is par))   # (a statement of an if: not used)
                if w and used:
                    out.append((w[0], depth - 1, "((%s)dos_hp(" % t))   # (its value used - t = n->left = x: the
                    out.append((w[1], -(depth - 1), "))"))              #  pointer again, not the handle)
            return
        if k == "UnaryOperator" and par.get("opcode") in ("&", "++", "--") or \
                (k == "CompoundAssignOperator" and (par.get("inner") or [None])[0] is n):
            refused.append((p[0], par.get("opcode")))
            return
        out.append((p[0], depth, "((%s)dos_hp(" % t))      # (a read: the pointer the handle stands for)
        out.append((p[1], -depth, "))"))
    _walk(tree, fn)
    return out, found, refused


def place(n, path):
    """(start, end) of an expression in the file path - its first token where it is written, its last where the
    macro that gave it is used (e->e_sym, e_sym a macro) - None when it is not all in the file"""
    r = n.get("range") or {}
    b, e = r.get("begin", {}), r.get("end", {})

    def loc(x):
        if "expansionLoc" in x:
            return x["expansionLoc"].get("offset"), x["expansionLoc"].get("tokLen", 0),                 x["expansionLoc"].get("_file") or x.get("_file")
        if "spellingLoc" in x:
            return None, 0, None
        return x.get("offset"), x.get("tokLen", 0), x.get("_file")
    b0, bl, bf = loc(b)
    e0, el, ef = loc(e)
    if b0 is None or e0 is None or bf != path or ef != path or e0 < b0:
        return None
    if b0 == e0 and "expansionLoc" in b and re.match(r"\s*\(", arith16.TEXT[0][e0 + el:e0 + el + 40] or ""):
        return None                                         # (all inside a macro with arguments)
    return b0, e0 + el


def apply(text, edits):
    """the insertions made: at one place the outer ones' openings before the inner, closings after"""
    by = collections.defaultdict(list)
    for at, depth, s in edits:
        by[at].append((depth, s))
    for at in sorted(by, reverse=True):
        opens = sorted([x for x in by[at] if x[0] >= 0], key=lambda x: x[0])        # (outer first)
        closes = sorted([x for x in by[at] if x[0] < 0], key=lambda x: x[0])        # (-depth: inner first)
        s = "".join(c for d, c in closes) + "".join(o for d, o in opens)   # (the inner closed first)
        text = text[:at] + s + text[at:]
    return text


def null_reads(text):
    """((struct n *)dos_hp(h))->op, and l = ((struct n *)dos_hp(h)); ... l->op with l never tested: a read
    through what may be NULL - on DOS the start of the data segment (no fault) - dos_hpr, the null segment for
    handle 0; a pointer the function tests (if (!l), l == 0, l && ...) keeps dos_hp, its NULL a NULL"""
    from codebreaker.port.pre_ansi import blank
    cast = re.compile(r"\(\((?:struct|union)\s+\w+\s*\*\s*\)dos_hp\(")

    def close(code, i):                                     # (the ')' that closes the '(' at i)
        d = 0
        for k in range(i, len(code)):
            d += {"(": 1, ")": -1}.get(code[k], 0)
            if d == 0:
                return k
        return -1
    code = blank(text)
    at = set()                                              # (where dos_hp( starts, to become dos_hpr()
    for m in cast.finditer(code):
        k = close(code, m.start())
        if k > 0 and re.match(r"\s*->", code[k + 1:]):
            at.add(m.end() - len("dos_hp("))
    lines, clines = text.split("\n"), code.split("\n")
    starts = [0]
    for l in lines:
        starts.append(starts[-1] + len(l) + 1)
    # (a pointer variable is left as it is: given on, a callee would take the null segment for a place -
    #  a run's fault there names it, and sanitize.low_reads reads through it alone)
    for i in sorted(at, reverse=True):
        text = text[:i] + "dos_hpr(" + text[i + len("dos_hp("):]
    return text


def declare(text, olds, found):
    """the pointer members of the records kept old declared uint16_t (a handle), in their definitions"""
    names = collections.defaultdict(dict)
    for rec in olds:
        names[rec]
    for (rec, m), t in found.items():
        names[rec][m] = t
    for rec, ms in names.items():
        tag = rec.split()[-1]
        for m in re.finditer(r"\b(?:struct|union)\s+%s\s*\{" % re.escape(tag), text):
            d, k = 0, m.end() - 1
            while k < len(text):
                d += {"{": 1, "}": -1}.get(text[k], 0)
                if d == 0:
                    break
                k += 1
            body = text[m.end():k]

            def split(x):                                   # (struct cse *c_next, *c_prev; - one declaration each)
                base, dcls = x.group(2), [d.strip() for d in x.group(3).split(",")]
                return x.group(1) + " ".join("%s %s;" % (base, d) for d in dcls)   # (on its one line still)
            body = re.sub(r"(?m)^([ \t]*)((?:struct|union)\s+\w+|\w+(?:\s+\w+)*?)\s*(\**\s*\w+(?:\s*\[[^\]]*\])?"
                          r"(?:\s*,\s*\**\s*\w+(?:\s*\[[^\]]*\])?)+)\s*;", split, body)
            for name, t in ms.items():
                body = re.sub(r"(?m)(^[ \t]*|;[ \t]*|\*/[ \t]*)(?:const\s+)?(?:struct\s+|union\s+)?\w+(?:\s+\w+)*\s*\*+\s*%s\s*;" %
                              re.escape(name), lambda x: "%suint16_t %s;\t/* (a handle: %s) */" % (
                                  x.group(1), name, t), body)
            body = re.sub(r"(?m)(^[ \t]*|;[ \t]*|\{[ \t]*)intptr_t(\s+\w+\s*;)",   # (a word made as wide as a
                          r"\1int16_t\2\t/* (one word, as it was) */", body)   #  pointer: one word again, here)
            text = text[:m.end()] + body + text[k:]
    return text


def mentions(folder, f, tags, seen=None):
    """whether a file, or a header of the program's it includes, names one of the tags"""
    seen = set() if seen is None else seen
    if f in seen:
        return False
    seen.add(f)
    try:
        t = open(os.path.join(folder, f), encoding="latin1").read()
    except OSError:
        return False
    if any(re.search(r"\b%s\b" % re.escape(g), t) for g in tags):
        return True
    return any(mentions(folder, h, tags, seen) for h in re.findall(r'#\s*include\s*"([^"]+)"', t))


def run(cc, folder, files):
    """-> ({file: its text with the conversions}, set of records kept old, {(record, member): type}, refusals)"""
    olds = set()                                            # (one file's tree at a time: a tree holds the system's
    hk = arith16._headers_key(folder)                       #  headers too - all of them at once were gigabytes;
    for f in files:                                         #  a file's result kept by its text and its headers')
        key = arith16.pass_key(cc, folder, f, "records", headers=hk)
        got = cache.value("port-passes", key)
        if got is not None:
            olds |= set(got["olds"])
            continue
        tree = arith16.dump(cc, f, folder)
        mine = set()
        if tree:
            arith16.mark_files(tree, [None])
            mine = byte_records([tree])
            olds |= mine
        cache.keep("port-passes", key, {"olds": sorted(mine)})
        del tree
    out, found_all, refused = {}, {}, []
    if not olds:
        return out, olds, found_all, refused
    tags = [r.split()[-1] for r in olds]
    for f in files:
        if not mentions(folder, f, tags):                   # (a file that never names them, nor its headers: none)
            continue
        key = arith16.pass_key(cc, folder, f, "oldlayout", sorted(olds), sorted(PTR_PARAMS.items()), headers=hk)
        got = cache.value("port-passes", key)
        if got is None:
            tree = arith16.dump(cc, f, folder)
            if not tree:
                continue
            arith16.mark_files(tree, [None])
            path = next((v for v in arith16._files(tree) if v.replace("\\", "/").endswith(f)), f)
            text = open(folder + "/" + f, encoding="latin1").read()
            e, found, ref = edits_of(tree, text, path, olds)
            got = {"new": apply(text, e) if e else None, "found": [[k[0], k[1], v] for k, v in found.items()],
                   "refused": [list(x) for x in ref]}
            cache.keep("port-passes", key, got)
            del tree
        found_all.update({(a_, b_): v for a_, b_, v in got["found"]})
        refused += [(f, at, op) for at, op in got["refused"]]
        if got["new"] is not None:
            out[f] = got["new"]
    return out, olds, found_all, refused
