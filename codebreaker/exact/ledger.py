"""ledger.py -- every one-function source of a project compiled by its toolchain and put against the program.

    python -m codebreaker exact PROJECT ledger [--learn] [-v] [--write]

Each src/fADDR.c is compiled on its own; its first function is held to the program at ADDR, the
others at their places in SYMBOLS. A project with several toolchains has each file compiled by
each; a function is identical when one of them gives its bytes, and the ledger says which did.
--learn: the places identical functions show are added to SYMBOLS (a name is only learnt from a
file all of whose placed functions are identical under one toolchain). --write: src/LEDGER is
written (otherwise only the counts are printed).

    ledger.run(project, learn=False) -> [(file, function, address, length, verdict, notes, by)]
        verdict: identical | differs | no place | does not compile
        by:      the labels of the toolchains that give the function's bytes
"""
import bisect
import os
import re

from codebreaker.exact import compare, omf


def check_file(prj, tc, path, at, syms, learnt):
    src = open(path, "rb").read()
    raw, con = tc.compile(src, headers=prj.headers(), include=prj.include, switches=prj.switches)
    if raw is None:
        last = con.strip().splitlines()
        return [(os.path.basename(path), None, at, 0, "does not compile", last[-1:])]
    o = omf.read(raw)
    stubs = _stubs(src)
    starts = prj.starts()
    out = []
    got = {}
    for ti, text in enumerate(omf.code_segments(o)):        # (#pragma alloc_text: more code segments than one)
        fs = omf.functions(o, text)
        first = at if ti == 0 else None
        if first is None and len(fs) > 1 and fs[0][0] == "(module start)" and fs[1][0] in syms:
            first = syms[fs[1][0]][1] - fs[1][1]           # (an assembly module placed by its first name)
        together = prj.whole(path) and first is not None    # (a whole source file: its text lies in one piece)
        if together and ti == 0 and prj.cfg.get("named_by", "") == "first procedure":
            # (the file is named after a procedure of it - its first frame, as a rule; the main module has code
            # before it): of the module's start and its frames, the one its bytes agree with the program at
            code = o.seg[text][:o.filled.get(text, 0)]
            hole = omf.holes(o, text)
            cands = [0] + [m.start() for m in re.finditer(b"\x55\x8b\xec", code)]

            def agree(k0):
                b = at - k0
                if b < 0:
                    return -1
                return sum(1 for x in range(min(len(code), 256)) if x not in hole and b + x < len(prj.image) and prj.image[b + x] == code[x])
            k0 = max(cands, key=lambda k: (agree(k), -k))
            first = at - k0
        places = [(of, ln, first if k == 0 and first is not None else
                   (first - fs[0][1] + of) if together else
                   (syms[nm][1] if nm in syms and syms[nm][0] in ("C", "?") else None)) for k, (nm, of, ln) in enumerate(fs)]

        def where(x, places=places, first=first, fs=fs):
            for of, ln, a in places:
                if a is not None and of <= x < of + ln:
                    return a + x - of
            return first - fs[0][1] + x if first is not None else -1
        for k, (name, off, ln) in enumerate(fs):
            if name.upper().lstrip("_") in stubs:
                continue
            a = places[k][2]
            if a is None:
                out.append((os.path.basename(path), name, None, ln, "no place", []))
                continue
            bad, notes = compare.function(o, prj.image, name, off, ln, a, prj, syms, got, text, where)
            code = o.seg[text]
            if bad == [ln - 1] and k == len(fs) - 1 and off + ln == len(code) and len(code) % 2 == 0 and code[-1] == 0 \
                    and ln >= 4 and code[-4] == 0xE9:
                bad = []                                    # (the assembler's pad to an even length: the next
                notes.append("? the last byte is the pad to an even length; the next module begins there")   # module's first byte)
            if starts and not _reaches(prj, starts, a, ln):
                notes.append("ends at %05x, before the next function's start" % (a + ln))
            ok = compare.identical(bad, notes)
            out.append((os.path.basename(path), name, a, ln, "identical" if ok else "differs", notes))
            if ok:
                got[name] = ("C", a)
    if all(r[4] == "identical" for r in out if r[2] is not None):
        learnt.update({k: v for k, v in got.items() if ":" not in k})
    return out


def _stubs(src):
    """after a line /* SAMEFILE */: empty stand-ins of functions of the program's same file, which a far function
    calls another way than one of another file (push cs, call near) - they are compiled and not compared"""
    s = src.decode("latin1")
    m = s.find("/* SAMEFILE */")
    return {x.upper().lstrip("_") for x in re.findall(r"(\w+)\s*\([^)]*\)[^{;]*\{\s*\}", s[m:])} if m >= 0 else set()


def _reaches(prj, starts, a, ln):
    """the function goes on to the next start (or to a pad of zeros before it: a segment's end)"""
    i = bisect.bisect_right(starts, a)
    end = min(([starts[i]] if i < len(starts) else []) + ([prj.code_end] if prj.code_end else []) or [a + ln])
    e = a + ln
    tail = prj.image[e:end]
    return e + (e & 1) == end + (end & 1) or (0 < len(tail) <= 16 and not tail.strip(b"\0"))


def _merge(per):
    """one file's rows under each toolchain [(label, rows)] -> its rows, a function identical when one gave it"""
    base = max((rs for lb, rs in per), key=len)            # (one that compiled, if any did)
    rows = []
    for k, r in enumerate(base):
        by = [lb for lb, rs in per if k < len(rs) and rs[k][4] == "identical"]
        best = next((rs[k] for lb, rs in per if k < len(rs) and rs[k][4] == "identical"), None)
        if best is None:                                    # the first that compiled says what is wrong
            best = next((rs[k] for lb, rs in per if k < len(rs) and rs[k][4] != "does not compile"), r)
        rows.append(best + (by,))
    return rows


def run(prj, learn=False, verbose=False):
    tcs = prj.toolchains()
    syms = prj.symbols()
    learnt = {}
    rows = []
    for at, path in prj.sources():
        per = [(lb, check_file(prj, tc, path, at, dict(syms), learnt)) for lb, tc in tcs]
        r = _merge(per)
        rows.extend(r)
        if verbose:
            for x in r:
                print("%-10s %-24s %s %s %s" % (x[0], x[1], x[4], ",".join(x[6]), "; ".join(x[5][:2])))
    if learn and learnt:
        s = prj.symbols()
        for k, v in learnt.items():
            s.setdefault(k, v)
        prj.save_symbols(s)
    return rows


def write(prj, rows):
    with open(prj.path("LEDGER"), "w", newline="\n") as f:
        f.write("# file, function, bytes, compared with %s, the toolchains that give it (codebreaker exact ledger)\n" % os.path.basename(prj.exe))
        for fn, name, a, ln, v, notes, by in rows:
            f.write("%s %s %d %s%s\n" % (fn, name or "-", ln, v, " " + ",".join(by) if by and len(prj.toolchains()) > 1 else ""))


def main(prj, argv):
    sets = prj.sets()
    rows = []
    for k, part in enumerate(sets):                         # (each set of sources with its own toolchain)
        r = run(part, learn="--learn" in argv, verbose="-v" in argv)
        if len(sets) > 1:
            same_ = sum(1 for x in r if x[4] == "identical")
            print("   %s (%s): %d functions, %d identical" % (os.path.relpath(part.src, prj.dir), part.cfg.get("toolchain"), len(r), same_))
        rows += r
    if "--write" in argv:
        write(prj, rows)
    n = len(rows)
    same = sum(1 for r in rows if r[4] == "identical")
    print("%d functions, %d identical, %d differ, %d no place, %d files do not compile" % (
        n, same, sum(1 for r in rows if r[4] == "differs"), sum(1 for r in rows if r[4] == "no place"),
        sum(1 for r in rows if r[4] == "does not compile")))
    tcs = [lb for lb, tc in prj.toolchains()]
    if len(tcs) > 1:
        for lb in tcs:
            only = sum(1 for r in rows if r[6] == [lb])
            print("   %-10s gives %d (%d of them it alone)" % (lb, sum(1 for r in rows if lb in r[6]), only))
    return 0
