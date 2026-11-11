"""repair.py -- a function that differs from the program by what the comparison names, mended in its source and
kept only when the compiler proves it.

    python -m codebreaker exact PROJECT repair [--write] [-v]

The ledger's comparison says where a function parts from the program; some of what it says is a fix it can
try. For each such function the source file is changed, compiled again, and held to the program: a change is
kept only if the function comes out identical and no function of the file that was identical stops being so.
Without --write the changes are only reported.

  values each +N   bytes that differ alone, each by the same amount (an immediate): the source names a constant
                   or an enumeration's member with another value. Tried, in order: the names that end in a
                   number equal to such a value (an enumeration named by its members' ordinals: DK46) with the
                   number moved by N - those alone, then every name of that family; a literal of that value
                   made the program's.

A Pascal program's change is written to the project's own overlay (its first source folder), so the lower
layers stay what they are for the other programs built from them.
"""
import os
import re

from codebreaker.exact import ledger

NOTE = re.compile(r"\? (\d+) values? (?:each )?([+-]\d+) in the program \(([^)]*)\)")


def _shifts(notes):
    for n in notes:
        m = NOTE.search(n)
        if m:
            return int(m.group(2)), [int(x.strip().lstrip("+"), 16) for x in m.group(3).split(",") if x.strip()]
    return None


def candidates(text, mine, d):
    """the source changed so that the values `mine` become mine + d -> [(what, text)]"""
    out = []
    want = {v & 0xFF for v in mine}
    member = re.compile(r"\b([A-Z][A-Z_]*[A-Z_])([1-9]\d{0,2}|0)\b")    # (a family named by ordinals: DK0, DK46, TK12)
    named = {}
    for m in member.finditer(text):
        if int(m.group(2)) & 0xFF in want:
            named.setdefault(m.group(1), set()).add(m.group(0))
    for fam, these in sorted(named.items()):
        rx = re.compile(r"\b(%s)([1-9]\d{0,2}|0)\b" % re.escape(fam))
        t = rx.sub(lambda m: m.group(1) + str(int(m.group(2)) + d) if m.group(0) in these else m.group(0), text)
        if t != text:
            out.append(("%s moved by %+d" % (", ".join(sorted(these)), d), t))
        t2 = rx.sub(lambda m: m.group(1) + str(int(m.group(2)) + d), text)
        if t2 not in (text, t):
            out.append(("every %s<n> moved by %+d" % (fam, d), t2))
    for v in sorted(want):
        hits = list(re.finditer(r"(?<![\w.])%d(?![\w.])" % v, text))
        for k, m in enumerate(hits):                        # (one at a time, then all)
            out.append(("the literal %d made %d (line %d)" % (v, v + d, text.count("\n", 0, m.start()) + 1),
                        text[:m.start()] + str(v + d) + text[m.end():]))
        if len(hits) > 1:
            out.append(("every literal %d made %d" % (v, v + d), re.sub(r"(?<![\w.])%d(?![\w.])" % v, str(v + d), text)))
    return out


def _pascal(prj, verbose):
    from codebreaker.formats import omf
    from codebreaker.exact import pascal_join
    rows = pascal_join.run(prj, False)
    out = []
    for label, pn, a, ln, verdict, notes in rows:
        s = _shifts(notes) if verdict == "differs" else None
        if not s or not pn.startswith("P"):
            continue
        d, offs = s
        name = label.split()[0]
        u = prj.cfg["compilands"][name]
        c = pascal_join.Compiland(prj, name, u)
        h = pn[1:]
        part = next(p for p in c.parts() if h in c.names_of(p))
        raw, con = prj.toolchain().compile(c.source(part).encode("latin1"), name="T.PAS")
        o = omf.read(raw)
        seg = [x for x, cls, l_, al in o.segs if x not in ("HEAP", "STACK", "DATA", "CONST", "MEMORY") and o.filled.get(x)][0]
        of = {n.upper(): q for n, q, l_ in omf.functions(o, seg)}["P" + h]
        mine = [o.seg[seg][of + k] for k in offs]
        path = _piece_of(c, h)
        if path is None:
            continue
        rel = os.path.relpath(path, next(dd for dd in c.dirs if path.startswith(dd)))
        dest = os.path.join(c.dirs[0], rel)
        out.append(dict(fn=pn, at=a, path=path, dest=dest, mine=mine, d=d, unit=name,
                        check=lambda prj=prj, name=name: [r for r in pascal_join.run(_only(prj, name), False)]))
    return out


def _only(prj, name):
    import copy
    p = copy.copy(prj)
    p.cfg = dict(prj.cfg)
    p.cfg["compilands"] = {name: prj.cfg["compilands"][name]}
    return p


def _piece_of(c, h):
    """the file whose first line declares P<h> (a piece, or one nested in a piece's .d folder), the top layer's"""
    head = re.compile(r"\s*(PROCEDURE|FUNCTION)\s+P%s\b" % h, re.I)
    for d in c.dirs:
        for root, dirs, files in os.walk(d):
            if os.path.relpath(root, d).count(os.sep) > 3:
                continue
            for f in files:
                if f.lower() == "p%s.pas" % h.lower():
                    p = os.path.join(root, f)
                    if head.match(open(p, encoding="latin1").readline()):
                        return p
    return None


def _sourced(prj, verbose):
    """the ledger's files (one function a file, or whole files): each differing function the note can mend"""
    from codebreaker.formats import omf
    out = []
    for part in prj.sets():
        tcs = part.toolchains()
        syms = part.symbols()
        for at, path in part.sources():
            per = [(lb, ledger.check_file(part, tc, path, at, dict(syms), {})) for lb, tc in tcs]
            rows = ledger._merge(per)
            for fn, name, a, ln, verdict, notes, by in rows:
                s = _shifts(notes) if verdict == "differs" else None
                if not s:
                    continue
                d, offs = s
                tc = tcs[0][1]
                raw, con = tc.compile(open(path, "rb").read(), headers=part.headers(), include=part.include, switches=part.switches)
                o = omf.read(raw)
                hit = next(((seg, q) for seg in omf.code_segments(o) for n, q, l_ in omf.functions(o, seg) if n == name), None)
                if hit is None:
                    continue
                mine = [o.seg[hit[0]][hit[1] + k] for k in offs]

                def check(part=part, tcs=tcs, path=path, at=at, syms=syms):
                    per = [(lb, ledger.check_file(part, tc, path, at, dict(syms), {})) for lb, tc in tcs]
                    return [r[:6] for r in ledger._merge(per)]
                out.append(dict(fn=name, at=a, path=path, dest=path, mine=mine, d=d, check=check))
    return out


def main(prj, argv):
    write = "--write" in argv
    todo = _pascal(prj, "-v" in argv) if prj.cfg.get("compilands") else _sourced(prj, "-v" in argv)
    fixed = 0
    for j in todo:
        before = {r[1]: r[4] for r in j["check"]()}
        text = open(j["path"], encoding="latin1", newline="").read()
        kept = None
        for what, t in candidates(text, j["mine"], j["d"]):
            os.makedirs(os.path.dirname(j["dest"]), exist_ok=True)
            had = open(j["dest"], encoding="latin1", newline="").read() if os.path.exists(j["dest"]) else None
            open(j["dest"], "w", encoding="latin1", newline="").write(t)
            after = {r[1]: r[4] for r in j["check"]()}
            good = after.get(j["fn"]) == "identical" and all(after.get(k) == "identical" for k, v in before.items() if v == "identical")
            if good and write:
                kept = what
                break
            if had is None:                                 # (not kept, or only asked: as it was)
                os.remove(j["dest"])
            else:
                open(j["dest"], "w", encoding="latin1", newline="").write(had)
            if good:
                kept = what + " (not written: --write)"
                break
        print("%s %05x: %s" % (j["fn"], j["at"], kept or "no change of those tried makes it the program's"))
        if kept:
            fixed += 1
    print("%d of %d mended" % (fixed, len(todo)))
    return 0
