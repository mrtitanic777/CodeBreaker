"""readable.py -- the sources with the names SYMBOLS has put in for the engine's address-names, proven to change nothing.

    python -m codebreaker exact PROJECT readable [--write]

The sources the engine writes name a function fXXXXX and a variable g_XXXX after where they are. Where src/SYMBOLS
has a name for that place (the names stage carries them from another version; a person may write one), each
source file gets it in place of the address-name: a name of C code `_name` is `name` in C. The files are compiled
again, every file into the ledger, and the change is kept only if no function that was identical stops being so
(the names of functions and variables are the linker's; a local's name, which can move it in the frame, is left).
Without --write the renaming is only tried and reported.
"""
import os
import re
import shutil
import tempfile

from codebreaker.exact import ledger, project as projmod

ADDRNAME = re.compile(r"\b(?:_?f([0-9a-f]{5})|g_([0-9a-f]{1,4}))\b")


def mapping(prj):
    """{address-name: name} - from SYMBOLS, for the places it has a real name for"""
    out = {}
    for n, (t, a) in prj.symbols().items():
        if ":" in n or "@" in n or ADDRNAME.fullmatch(n.lstrip("_")) or ADDRNAME.fullmatch(n):
            continue
        c = n[1:] if n.startswith("_") else n               # (the C name of a public _name)
        if not re.fullmatch(r"[A-Za-z_]\w*", c):
            continue
        if t in ("C", "?"):
            out.setdefault(("f", a), c)
        if t in ("D", "?") and a < 0x10000:
            out.setdefault(("g", a), c)
    return out


def rename(text, names):
    """the text with each address-name that has a name renamed - unless the name is in the text already, as
    something else"""
    have = set(re.findall(r"\b[A-Za-z_]\w*\b", text))
    done = {}

    def one(m):
        k = ("f", int(m.group(1), 16)) if m.group(1) else ("g", int(m.group(2), 16))
        n = names.get(k)
        if n and (n not in have or done.get(n) == k):
            done[n] = k
            return n
        return m.group(0)
    parts = re.split(r'(/\*.*?\*/|"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\')', text, flags=re.S)
    return "".join(p if k % 2 else ADDRNAME.sub(one, p) for k, p in enumerate(parts))   # (not in a remark or a
                                                                                        #  string: another program's
                                                                                        #  place, or text)


def counts(prj):
    rows = ledger.run(prj)
    return {(r[0], r[1]): r[4] for r in rows}


def main(prj, argv):
    names = mapping(prj)
    if not names:
        print("SYMBOLS has no names for the address-names (the names stage puts them there)")
        return 1
    files = [p for at, p in prj.sources()]
    new = {}
    for p in files:
        t = open(p, encoding="latin1", newline="").read()
        r = rename(t, names)
        if r != t:
            new[p] = r
    print("%d of %d files would take names (%d names known)" % (len(new), len(files), len(names)))
    if not new:
        return 0
    before = counts(prj)
    w = tempfile.mkdtemp(prefix="readable_")
    try:                                                    # (the project again, its sources renamed)
        src = os.path.join(w, "src")
        shutil.copytree(prj.src, src)
        for p, t in new.items():
            open(os.path.join(src, os.path.relpath(p, prj.src)), "w", encoding="latin1", newline="").write(t)
        twin = projmod.Project(prj.dir, override={"src": src})
        after = counts(twin)
    finally:
        shutil.rmtree(w, ignore_errors=True)
    same_b = {k for k, v in before.items() if v == "identical"}
    same_a = {(f, n if n in after else n) for (f, n), v in after.items() if v == "identical"}
    lost = len(same_b) - len([1 for v in after.values() if v == "identical"])
    print("identical before: %d, after the renaming: %d" % (len(same_b), sum(1 for v in after.values() if v == "identical")))
    if lost > 0:
        print("not kept: the renaming loses %d" % lost)
        return 1
    if "--write" in argv:
        for p, t in new.items():
            open(p, "w", encoding="latin1", newline="").write(t)
        print("written: %d files" % len(new))
    return 0
