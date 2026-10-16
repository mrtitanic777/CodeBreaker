"""pascal_join.py -- a Pascal program written in pieces, joined into its compilands, compiled, compared.

A compiland of an MS-Pascal program is one source (a PROGRAM or a MODULE) holding all its procedures;
the reconstruction keeps it in pieces so that each procedure can be written and checked alone:

    <dir>/GLOBALS.pas    types and variables          <dir>/EXTERNS.pas   procedures of other compilands
    <dir>/FORWARDS.pas   forward declarations         <dir>/MAIN.pas      the compiland's own block
    <dir>/pADDR.pas      the procedure P<ADDR> (ADDR: its place in the program); one nested in it is
                         <dir>/pADDR.d/pNEST.pas, pulled in by a line {+pNEST}
    a heading marked {STUB} (something to call) or {TODO} (not written yet) is not compared

joined in address order, compiled by the project's toolchain (exact/mspascal.py), each procedure held
to the program at its place. The compiler of 1981 runs out of memory on a large compiland: it is
compiled in "parts" runs, each with the procedures of one run in full and the others as empty bodies
(their headings: something to call).

project.json:
    "compilands": {NAME: {"dir", "kind": PROGRAM|MODULE, "main": its own block's place, "parts": runs}}
    "procedures": a JSON file {"0xADDR": {"size", "unit"}} - every procedure of the program, its compiland
                  ("unit": the compiland's number, "units": {NAME: number})
    "dgroup": the image offset of DS:0 (it may be below the image: negative)

    pascal_join.run(project) -> rows as ledger.run's
"""
import json
import os
import re

from codebreaker.exact import compare, omf

NL = "\n"


class Compiland(object):
    def __init__(self, prj, name, u):
        self.prj, self.name, self.u = prj, name, u
        root = os.path.join(prj.dir, prj.cfg.get("root", "."))
        over = u.get("over", prj.cfg.get("over", []))
        self.dirs = [os.path.join(root, o, u["dir"]) for o in over] + [os.path.join(root, d) for d in u.get("dirs", [u["dir"]])]
        self.src = self.dirs[-1]
        self.headline = ("PROGRAM %s (INPUT, OUTPUT);" if u["kind"] == "PROGRAM" else "MODULE %s;") % name
        self.empty = "BEGIN" + NL + "END." + NL if u["kind"] == "PROGRAM" else "END." + NL
        mp = prj.cfg.get("map")                             # (pieces of another program's: where each is in this one)
        self.map = json.load(open(os.path.join(root, mp))).get(name, {}) if mp and os.path.exists(os.path.join(root, mp)) else {}
        self.over = bool(over)
        # EDITS (FILE|old|new, FILE|+|a line; * every piece, *~ a regular expression) and REMOVED, lowest overlay first
        self.edits = [tuple(l.rstrip(NL).split("|")) for d in reversed(self.dirs) if os.path.exists(os.path.join(d, "EDITS"))
                      for l in open(os.path.join(d, "EDITS"), encoding="latin1") if l.count("|") == 2] if self.over else []
        self.removed = set(x for d in self.dirs if self.over and os.path.exists(os.path.join(d, "REMOVED"))
                           for x in open(os.path.join(d, "REMOVED")).read().split())
        kp = prj.cfg.get("kinds")                           # (each procedure with the stack check or without)
        self.kinds = {int(k, 16): v.get("kind") for k, v in json.load(open(os.path.join(root, kp))).items()} if kp and os.path.exists(os.path.join(root, kp)) else {}

    def addr_of(self, h):
        return int(self.map[h.upper()], 16) if self.map and h.upper() in self.map else int(h, 16)

    def where(self, f):
        for d in self.dirs:
            if os.path.exists(os.path.join(d, f)):
                return os.path.join(d, f)
        return os.path.join(self.src, f)

    def pieces(self):
        ps = {f for d in self.dirs if os.path.isdir(d) for f in os.listdir(d) if re.fullmatch(r"p[0-9a-f]{5}\.pas", f)} - self.removed
        return sorted(ps, key=lambda f: (self.map.get(f[1:6].upper(), f[1:6]) if self.map else f))

    def check(self, f):
        """the metacommand that gives the procedure of piece f the stack check it has in the program"""
        if not self.kinds:
            return ""
        a = self.addr_of(f[1:6]) if (not self.map or f[1:6].upper() in self.map) else None
        k = self.kinds.get(a) if a is not None else None
        return "" if k is None else ("{$STACKCK+}" if k == "check" else "{$STACKCK-}") + NL

    def expand(self, text, d, own=None):
        out = []
        again = False
        for line in text.split(NL):
            m = re.fullmatch(r"\s*\{\+(p[0-9a-f]{5})\}\s*", line)
            if m:
                out.append(self.check(m.group(1)) + self.expand(open(self.where(d + "/" + m.group(1) + ".pas"), encoding="latin1").read().rstrip(NL), d, m.group(1)))
                again = True
            else:
                if again and own and re.match(r"BEGIN\b", line):
                    out.append(self.check(own).rstrip(NL))   # (back to the outer procedure's own setting)
                    again = False
                out.append(line)
        return NL.join(out)

    def piece(self, f):
        t = self.check(f) + self.expand(open(self.where(f), encoding="latin1").read().rstrip(NL), f[:-4] + ".d", f) + NL
        for name, old, new in self.edits:
            if name == "*":
                t = t.replace(old, new)
            if name == "*~":
                t = re.sub(old, new, t)
        return t

    def rd(self, f):
        p = self.where(f)
        t = open(p, encoding="latin1").read().rstrip(NL) + NL if os.path.exists(p) else ""
        for name, old, new in self.edits:
            if name == f:
                if old == "+":
                    t += new + NL
                elif old in t:
                    t = t.replace(old, new)
        if f == "GLOBALS.pas" and self.edits:              # (one declaration a line again)
            t = NL.join(re.sub(r";\s+(?=G_[0-9A-F]{4})", ";" + NL + "  ", l) if re.match(r"  G_[0-9A-F]{4}", l) else l for l in t.split(NL))
        if f == "MAIN.pas":
            for name, old, new in self.edits:
                if name == "*":
                    t = t.replace(old, new)
                if name == "*~":
                    t = re.sub(old, new, t)
        return t.replace(" {$STACKCK-}", "") if self.u.get("stackck") else t

    def result_types(self):
        out = {}
        for t in [self.rd("FORWARDS.pas")] + [self.rd(f) for f in self.pieces()]:
            for m in re.finditer(r"^FUNCTION +(P[0-9A-F]{5})(\([^)]*\))?: *([A-Z_0-9]+)", t, re.M):
                out[m.group(1)] = m.group(3)
        return out

    def stub(self, f, rt):
        head = self.rd(f).split(NL)[0]
        m = re.match(r"(PROCEDURE|FUNCTION) +(P[0-9A-F]{5})", head)
        body = ""
        if m and m.group(1) == "FUNCTION":
            t = rt.get(m.group(2), "WORD")
            body = "  %s := %s" % (m.group(2), "FALSE" if t == "BOOLEAN" else ("0" if t in ("WORD", "BYTE", "INTEGER") else "NIL")) + NL
        return head.replace("{TODO}", "") + NL + "BEGIN" + NL + body + "END;" + NL

    def parts(self):
        ps = self.pieces()
        size = [len(self.piece(f)) for f in ps]
        tot = sum(size)
        n = self.u.get("parts", 1)
        out = [[] for _ in range(n)]
        acc = 0
        for f, s in zip(ps, size):
            out[min(n - 1, acc * n // max(tot, 1))].append(f)
            acc += s
        out = [x for x in out if x]
        if os.path.exists(self.where("MAIN.pas")) or self.u["kind"] == "MODULE":
            (out[-1] if out else out.append([]) or out[-1]).append("MAIN.pas")
        return out

    def source(self, part):
        G, X, F = self.rd("GLOBALS.pas"), self.rd("EXTERNS.pas"), self.rd("FORWARDS.pas")
        rt = self.result_types()
        body = {f: ((self.rd(f) or self.empty) if f == "MAIN.pas" else self.piece(f)) for f in part}
        used = set(re.findall(r"[A-Z_][A-Z_0-9]*", "".join(body.values())))
        head, sep, var = G.partition("VAR" + NL)

        def keep(text, pat):
            return "".join(l + NL for l in text.split(NL) if l and (not re.match(pat, l) or re.match(pat, l).group(1) in used))
        kept = keep(var, r"  (G_[0-9A-F]{4}):") if sep else ""
        t = self.headline + NL + head + ("VAR" + NL + kept if kept.strip() else "")
        t += keep(X, r"(?:PROCEDURE|FUNCTION) +([A-Z_0-9]+)") + keep(F, r"(?:PROCEDURE|FUNCTION) +([A-Z_0-9]+)")
        for f in self.pieces():
            if f in body:
                t += body[f]
            elif "P" + f[1:6].upper() in used:
                t += self.stub(f, rt)
        return t + (body["MAIN.pas"] if "MAIN.pas" in body else self.empty)

    def names_of(self, part):
        out = set()
        for f in part:
            if f == "MAIN.pas":
                out.add("MAIN")
                continue
            for line in self.piece(f).split(NL):
                m = re.match(r"\s*(?:PROCEDURE|FUNCTION) +P([0-9A-F]{5})", line)
                if m and "{STUB}" not in line and "{TODO}" not in line and "FORWARD" not in line:
                    out.add(m.group(1))
        return sorted(out)


def _procedures(prj):
    p = os.path.join(prj.dir, prj.cfg.get("root", "."), prj.cfg["procedures"])
    return {int(k, 16): v for k, v in json.load(open(p)).items()}


def run(prj, verbose=False):
    tc = prj.toolchain()
    procs = _procedures(prj)
    units = prj.cfg.get("units", {})
    img = prj.image
    rows = []
    for name, u in prj.cfg["compilands"].items():
        c = Compiland(prj, name, u)
        unit = units.get(name, u.get("unit"))
        mine = sorted(a for a, v in procs.items() if v.get("unit") == unit)
        helper_at = max((a + procs[a]["size"] for a in mine), default=None)     # (the entry helper: after them all)
        for part in c.parts():
            text = c.source(part)
            raw, con = tc.compile(text.encode("latin1"), name="T.PAS")
            label = "%s %s" % (name, part[0])
            if raw is None:
                rows.append((label, None, None, 0, "does not compile", con.strip().splitlines()[-1:]))
                continue
            o = omf.read(raw)
            seg = [s for s, cls, ln, al in o.segs if s not in ("HEAP", "STACK", "DATA", "CONST", "MEMORY") and o.filled.get(s)][0]
            fs = omf.functions(o, seg)
            at_of = {}                                      # (two names at one offset - a main program's block and
            for n, of, ln in fs:                            # ENTGQQ, the entry: each has the code that starts there)
                at_of[of] = max(at_of.get(of, 0), ln)
            off_of = {n.upper(): (of, at_of[of]) for n, of, ln in fs}
            # where each compiled procedure stands in the program: P<ADDR> at ADDR, the block at main, the helper after
            places = []
            main_at = int(u["main"], 0) if isinstance(u["main"], str) else u["main"]
            for n, of, ln in fs:
                m = re.fullmatch(r"P([0-9A-F]{5})", n.upper())
                a = c.addr_of(m.group(1)) if m and (not c.map or m.group(1) in c.map) else None if m else \
                    main_at if n.upper() == name.upper() else helper_at if n == "(helper)" else None
                places.append((of, at_of[of], a))

            def where(x, places=places):
                for of, ln, a in places:
                    if a is not None and of <= x < of + max(ln, 1):
                        return a + x - of
                return -1
            for h in c.names_of(part):
                pn = name.upper() if h == "MAIN" else "P" + h
                if h != "MAIN" and c.map and h not in c.map:
                    rows.append((label, pn, None, 0, "no place", ["not in the program (no place in the map)"]))
                    continue
                a = main_at if h == "MAIN" else c.addr_of(h)
                if pn not in off_of:
                    rows.append((label, pn, a, 0, "no place", ["not in the compiled code"]))
                    continue
                of, ln = off_of[pn]
                size = procs.get(a, {}).get("size")
                bad, notes = compare.function(o, img, pn, of, ln, a, prj, dict(prj.symbols()), {}, seg, where)
                if size is not None and size != ln:
                    notes.append("%d bytes, the program's %d" % (ln, size))
                verdict = "identical" if compare.identical(bad, notes) else "differs"
                eq = u.get("equivalent", prj.cfg.get("equivalent"))
                if verdict == "differs" and eq and size:    # (the same code but for another compiler state's habits?)
                    import importlib
                    mod, _, fn = eq.partition(":")
                    hole = omf.holes(o, seg)
                    got = o.seg[seg][of:of + ln]
                    want = img[a:a + size]
                    from codebreaker.exact.habits import cmpp
                    cmpp.HDRLEN = getattr(tc, "helper_data", 3)   # (the bytes after the entry helper's call)
                    try:
                        same, why = getattr(importlib.import_module(mod), fn)(want, got, {k - of for k in hole if of <= k < of + ln})
                    except Exception as e:
                        same, why = False, ["%s" % e]
                    if same:
                        verdict = "equivalent"
                        notes = ["the same code but for the habits of another compiler state: " + "; ".join(why)[:200]]
                rows.append((label, pn, a, ln, verdict, notes))
                if verbose:
                    print("%-22s %-10s %05x %s %s" % (label, pn, a or 0, verdict, "; ".join(notes[:2])))
    return rows
