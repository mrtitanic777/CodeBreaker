"""modules.py -- the program's source files: their functions and data put together, compiled whole, checked.

    python -m codebreaker exact PROJECT modules [MODULE..] [-v]

The functions are written one to a file (src/fADDR.c) so that each can be compiled and compared
alone. The original had them in modules; src/MODULES gives, one a line:

    name   code-start | -   data-start   [switches]
        name.c   a source file: its code from code-start to the next module's, its data from data-start
        NAME.OBJ an object of the kit, linked as it is
        -        a file of data only (src/NAME-data.c)

A module's data is in src/dADDR.c (what the file defines before the function at ADDR) and src/eADDR.c
(after it, at the file's end); its functions are the src/fADDR.c of its code. They are put together
in order - the includes once, each extern once, the functions of other files that return something
else than an int declared - compiled once, and every function is compared with the program, and the
file's initialized data (_DATA) with the data segment, word for word where the linker fills a word.

    modules.rows(project) -> [(name, code start, code end, data start, switches)]
    modules.source(project, row) -> (text, [function addresses]) or None
    modules.check(project, row) -> dict(res=[(name, address, length, verdict)], dlen, dbad, obj)
"""
import glob
import os
import re
import struct

from codebreaker.exact import compare, omf


def rows(prj):
    out = []
    p = prj.path("MODULES")
    if not os.path.exists(p):
        return []
    for l in open(p):
        l = l.split("#")[0].strip()
        if not l:
            continue
        f = l.split(None, 3)
        out.append((f[0], int(f[1], 16) if f[1] != "-" else None, int(f[2], 16), f[3].strip() if len(f) > 3 else None))
    code = sorted(a for n, a, d, x in out if a is not None)
    res = []
    for n, a, d, x in out:
        e = None
        if a is not None:
            later = [c for c in code if c > a]
            e = later[0] if later else prj.own_end()
        res.append((n, a, e, d, x))
    return res


def files_of(src, start, end):
    out = []
    for p in glob.glob(os.path.join(src, "[dfe]*.c")):
        m = re.fullmatch(r"([dfe])([0-9a-f]{5})\.c", os.path.basename(p))
        if m and start <= int(m.group(2), 16) < end:
            out.append((int(m.group(2), 16), "dfe".index(m.group(1)), p))
    return [p for a, k, p in sorted(out)]


_DT = {}


def deftypes(srcdir):
    """{function: the type it returns} - the program's functions that return something else than an int"""
    if srcdir not in _DT:
        out = {}
        for p in glob.glob(os.path.join(srcdir, "f*.c")):
            t = open(p, encoding="latin1").read()
            for m in re.finditer(r"(?m)^((?:unsigned \w+|unsigned|struct \w+|union \w+|\w+)(?: ?\*+)?)\n(f[0-9a-f]{5})\(", t):
                ty = m.group(1).strip()
                if ty in ("int", "static") or ty.startswith(("static", "extern")):
                    continue
                out[m.group(2)] = ty
        _DT[srcdir] = out
    return _DT[srcdir]


def combine(paths, default, header_tag):
    """the files' texts as one source; the switches the functions' files ask for"""
    incs, ext, seen, bodies, flags = [], [], set(), [], set()
    fs_rx = re.compile(r"/\* %s: ([^*]*?) \*/\n" % header_tag) if header_tag else None
    for p in paths:
        t = open(p, encoding="latin1").read().replace("\r\n", "\n")
        m = fs_rx.match(t) if fs_rx else None
        if os.path.basename(p).startswith("f"):
            flags.add(m.group(1) if m else default)
        if m:
            t = t[m.end():]
        t = re.sub(r"/\* [A-Z]+: [0-9, ]+ \*/\n", "", t)    # (a module compiles as it is: no pass's askings answered)
        lines = t.split("\n")
        k = 0
        pre = []
        while k < len(lines):
            l = lines[k]
            if l.startswith("/*"):
                while True:
                    pre.append(lines[k])
                    if "*/" in lines[k]:
                        break
                    k += 1
            elif l.startswith("#include"):
                if l not in incs:
                    incs.append(l)
            elif l.startswith("extern ") and re.sub(r"\s*/\*.*\*/\s*$", "", l).endswith(";") and "{" not in l:
                key = re.sub(r"\s*/\*.*\*/\s*$", "", l).strip()
                if key not in seen:
                    seen.add(key)
                    ext.append(l)
            elif l.strip() == "":
                pass
            else:
                break
            k += 1
        bodies.append("\n".join(pre + lines[k:]).strip("\n"))
    have = "\n".join(ext)
    fwd = []
    srcdir = os.path.dirname(paths[0]) if paths else "."
    body = "\n".join(bodies)
    for fn, ty in deftypes(srcdir).items():
        if not re.search(r"\b%s\(" % fn, body) or re.search(r"\b%s\(\)" % fn, have + body):
            continue
        stars = len(ty) - len(ty.rstrip("*"))
        fwd.append("extern %s %s%s();" % (ty.rstrip("*").strip(), "*" * stars, fn))
    return "\n".join(incs + ext + fwd) + "\n\n" + "\n\n".join(bodies) + "\n", flags


def source(prj, row):
    """(the module's whole text, the addresses of its functions in order) or None (an object of the kit)"""
    mod, start, end, dstart, sw = row
    if mod.upper().endswith(".OBJ"):
        return None
    paths = files_of(prj.src, start, end) if start is not None else []
    dp = os.path.join(prj.src, os.path.splitext(mod)[0] + "-data.c")
    if os.path.exists(dp):
        paths.append(dp)
    tc = prj.toolchain()
    default = prj.switches if prj.switches is not None else tc.p.get("switches", "")
    text, flags = combine(paths, default, tc.p.get("file_switches"))
    fl = sw or (sorted(flags)[0] if flags else default)
    if tc.p.get("file_switches") and fl:
        text = "/* %s: %s */\n" % (tc.p["file_switches"], fl) + text
    addrs = [int(m.group(1), 16) for p in paths for m in [re.fullmatch(r"f([0-9a-f]{5})\.c", os.path.basename(p))] if m]
    return text, addrs, flags


def compile_module(prj, row, tc=None):
    s = source(prj, row)
    if s is None:
        return None, "", []
    text, addrs, flags = s
    tc = tc or prj.toolchain()
    raw, con = tc.compile(text.encode("latin1"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    return raw, con, addrs


def check(prj, row, raw=None):
    mod, start, end, dstart, sw = row
    if mod.upper().endswith(".OBJ"):
        return dict(res=[], dlen=0, dbad=[], obj=True)
    if raw is None:
        raw, con, addrs = compile_module(prj, row)
        if raw is None:
            return dict(error=con)
    o = omf.read(raw)
    img = prj.image
    syms = prj.symbols()
    got = {}
    res = []
    text = omf.code_segments(o)[0] if omf.code_segments(o) else "_TEXT"
    for name, off, ln in omf.functions(o, text):
        own = re.fullmatch(r"_?f([0-9a-f]{5})", name)
        a = int(own.group(1), 16) if own else (syms[name][1] if name in syms and syms[name][0] == "C" else None)
        if a is None:
            res.append((name, None, ln, "no place"))
            continue
        bad, notes = compare.function(o, img, name, off, ln, a, prj, syms, got, text)
        res.append((name, a, ln, "same" if compare.identical(bad, notes) else "differs %d" % len(bad)))
    d = o.seg.get("_DATA", b"")[:o.filled.get("_DATA", 0)]
    ds = img[prj.dgroup:]
    W = lambda b, k: struct.unpack_from("<H", b, k)[0]
    holes = {fo: (tgt, disp) for seg, fo, kind, tgt, disp in o.fix if seg == "_DATA" and kind == "off"}
    dbad = []
    k = 0
    while k < len(d):
        if k in holes:
            tgt, disp = holes[k]
            if dstart + k + 2 > len(ds):
                dbad.append("%04x: past the program's data" % (dstart + k))
                k += 2
                continue
            theirs = W(ds, dstart + k)
            mine = (W(d, k) + disp) & 0xFFFF
            if tgt[0] != "ext" and tgt[1] == "_DATA" and theirs != (dstart + mine) & 0xFFFF:
                dbad.append("%04x: -> %04x, the program's -> %04x" % (dstart + k, dstart + mine, theirs))
            k += 2
            continue
        if dstart + k >= len(ds) or ds[dstart + k] != d[k]:
            dbad.append("%04x: %02x, the program's %02x" % (dstart + k, d[k], ds[dstart + k] if dstart + k < len(ds) else -1))
        k += 1
    return dict(res=res, dlen=len(d), dbad=dbad, raw=raw)


def main(prj, argv):
    want = [a for a in argv if not a.startswith("-")]
    tot = same = 0
    for row in rows(prj):
        mod = row[0]
        if want and mod not in want and os.path.splitext(mod)[0] not in want:
            continue
        j = check(prj, row)
        if j.get("obj"):
            print("%-14s the kit's object" % mod)
            continue
        if "error" in j:
            print("%-14s does not compile: %s" % (mod, j["error"].strip().splitlines()[-1:]))
            continue
        s = [x for x in j["res"] if x[3] == "same"]
        tot += len(j["res"])
        same += len(s)
        print("%-14s %s  %3d functions, %3d identical; data %04x..%04x %s" % (
            mod, ("%05x" % row[1]) if row[1] is not None else "  -  ", len(j["res"]), len(s), row[3], row[3] + j["dlen"],
            "IDENTICAL" if not j["dbad"] else "%d differ" % len(j["dbad"])))
        if "-v" in argv:
            for x in j["res"]:
                if x[3] != "same":
                    print("     %-12s %s %s" % (x[0], ("%05x" % x[1]) if x[1] is not None else "?", x[3]))
            for x in j["dbad"][:20]:
                print("     data " + x)
    print("%d functions in the modules, %d identical" % (tot, same))
    return 0
