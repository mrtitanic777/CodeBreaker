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

from codebreaker.exact import compare
from codebreaker.formats import omf


def rows(prj):
    out = []
    p = prj.path("MODULES")
    if not os.path.exists(p):
        return []
    for l in open(p):
        l = l.split("#")[0].strip()
        if not l:
            continue
        f = l.split(None, 2)                                # name code-start [data-start] [switches]: the data's start
        rest = f[2] if len(f) > 2 else ""                   # may be left out (the data is not checked then)
        m = re.match(r"([0-9a-fA-F]{4,5})\b\s*(.*)", rest)
        d, sw = (int(m.group(1), 16), m.group(2).strip()) if m else (None, rest.strip())
        out.append((f[0], int(f[1], 16) if f[1] != "-" else None, d, sw or None))
    code = sorted(a for n, a, d, x in out if a is not None)
    lib = lib_start(prj)
    if lib is not None:                                     # (the library's part ends the module before it: the
        code = sorted(set(code) | {lib})                    #  program's own modules may go on after it)
    res = []
    for n, a, d, x in out:
        if (x or "").startswith("@"):                       # (a part of the file named, in a code segment of its
            continue                                        #  own: compiled with it - see parts)
        e = None
        if a is not None:
            later = [c for c in code if c > a]
            e = later[0] if later else (prj.code_end if prj.code_end and prj.code_end > a else prj.own_end())
        res.append((n, a, e, d, x))
    return res


def lib_start(prj):
    """where the library's part begins: the first code place src/CRTLIB names ("code ADDR", "code=ADDR-", or a
    line's note that begins with its place)"""
    p = prj.path("CRTLIB")
    if not os.path.exists(p):
        return None
    at = []
    for l in open(p):
        if l.startswith("#"):
            continue
        at += [int(x, 16) for x in re.findall(r"\bcode[= ]([0-9a-f]{5})\b", l)]
        m = re.match(r"\S+(?:\s+!\S+)?\s+#\s*([0-9a-f]{5})\b", l)
        if m:
            at.append(int(m.group(1), 16))
    return min(at) if at else None


def parts(prj, name):
    """[(start, end, segment)]: the functions of the file that #pragma alloc_text puts in another code segment
    (a row of src/MODULES: name start @SEGMENT)"""
    p = prj.path("MODULES")
    if not os.path.exists(p):
        return []
    rs = []
    for l in open(p):
        f = l.split("#")[0].split()
        if len(f) >= 2 and re.fullmatch(r"[0-9a-fA-F]{4,5}", f[1]):
            rs.append((int(f[1], 16), f[0], f[2] if len(f) > 2 else ""))
    rs.sort()
    out = []
    for k, (a, n, x) in enumerate(rs):
        if n == name and x.startswith("@"):
            out.append((a, rs[k + 1][0] if k + 1 < len(rs) else prj.own_end(), x[1:]))
    return out


def starts(prj):
    """the code starts src/MODULES gives (no ends: nothing else is asked of the project)"""
    p = prj.path("MODULES")
    out = []
    if os.path.exists(p):
        for l in open(p):
            f = l.split("#")[0].split()
            if len(f) >= 2 and re.fullmatch(r"[0-9a-fA-F]{4,5}", f[1]):
                out.append(int(f[1], 16))
    return sorted(out)


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


def combine(paths, default, header_tag, before=None):
    """the files' texts as one source; the switches the functions' files ask for. before: {path: text put
    before its body} (a pragma)"""
    before = before or {}
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
        for d in re.findall(r"/\* SAMEFILE \*/\n([^\n{;]*\([^)]*\))\s*\{\s*\}", t):     # (the empty stand-ins of the
            key = "extern %s;" % re.sub(r"\(.*\)", "()", d).strip()                     #  file's other functions:
            if key not in seen:                                                         #  declared instead - they
                seen.add(key)                                                           #  are here now)
                ext.append(key)
        t = re.sub(r"/\* SAMEFILE \*/\n[^\n{;]*\([^)]*\)\s*\{\s*\}\n\n?", "", t)
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
            elif l.startswith("#include"):                  # (once each file, however the line is written)
                key = re.sub(r"[\s\"<>]", "", l.split("/*")[0][8:]).lower()
                if key not in [re.sub(r"[\s\"<>]", "", x.split("/*")[0][8:]).lower() for x in incs]:
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
        bodies.append((before.get(p, "") + "\n".join(pre + lines[k:])).strip("\n"))
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
    whole = os.path.join(prj.src, mod)
    if os.path.exists(whole):                               # (a file as it is: of data only, or an assembler's)
        text = open(whole, encoding="latin1").read()
        seg = first_segment(prj, start)
        if seg and not mod.lower().endswith(".c"):
            text = text.replace("_TEXT", seg)               # (the first code segment's name, in the assembly too)
        return text, [], set()
    paths = files_of(prj.src, start, end) if start is not None else []
    dp = os.path.join(prj.src, os.path.splitext(mod)[0] + "-data.c")
    if os.path.exists(dp):
        paths.append(dp)
    before = {}
    for a, e, seg in parts(prj, mod):                       # (the functions of another code segment, after a pragma)
        pp = files_of(prj.src, a, e)
        fns = []
        for q in pp:
            for d in re.findall(r"(?m)^([A-Za-z][^;(\n]*?\b(\w+))\([^)\n]*\)\s*$", open(q, encoding="latin1").read()):
                if d[1] not in fns:
                    fns.append(d[1])
        if pp:
            before[pp[0]] = "#pragma alloc_text(%s, %s)\n\n" % (seg, ", ".join(fns))
        paths += pp
    tc = prj.toolchain()
    default = prj.switches if prj.switches is not None else tc.p.get("switches", "")
    text, flags = combine(paths, default, tc.p.get("file_switches"), before)
    fl = sw or (sorted(flags)[0] if flags else default)
    seg = first_segment(prj, start)
    if seg and tc.p.get("segment_switch"):                  # (a module of the first code segment: its name given)
        fl = (fl + " " if fl else "") + tc.p["segment_switch"].format(name=seg)
    if tc.p.get("file_switches") and fl:
        text = "/* %s: %s */\n" % (tc.p["file_switches"], fl) + text
    inpart = lambda a: any(lo <= a < hi for lo, hi, s in parts(prj, mod))
    addrs = [int(m.group(1), 16) for p in paths for m in [re.fullmatch(r"f([0-9a-f]{5})\.c", os.path.basename(p))]
             if m and not inpart(int(m.group(1), 16))]     # (the functions of the main code segment: another one's are
                                                            #  not made stand-ins)
    return text, addrs, flags


def set_of(prj, mod):
    """(the source set, its toolchain) whose files' pattern names this file - an "also" set's, else the project's"""
    for part in prj.sets()[1:]:
        for k in ("unplaced", "sources"):
            if part.cfg.get(k) and re.fullmatch(part.cfg[k], mod, re.I):
                return part, part.toolchain()
    return prj, prj.toolchain()


def first_segment(prj, start):
    """the name the project gives the first code segment (project "first_segment"), for a module that lies in it"""
    name = prj.cfg.get("first_segment")
    if not name or start is None:
        return None
    later = [s for s in (getattr(prj, "code_segments", None) or []) if s > 0]
    return name if not later or start < min(later) else None


def compile_module(prj, row, tc=None):
    s = source(prj, row)
    if s is None:
        return None, "", []
    text, addrs, flags = s
    part = prj
    if os.path.exists(os.path.join(prj.src, row[0])):      # (a whole file: by the toolchain of the set it is of)
        part, own = set_of(prj, row[0])
        tc = own if part is not prj else (tc or own)
    tc = tc or prj.toolchain()
    raw, con = tc.compile(text.encode("latin1"), headers=part.headers(), include=part.include, switches=part.switches)
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
    d = o.seg.get("_DATA", b"")[:o.filled.get("_DATA", 0)] if dstart is not None else b""     # (no data start: not checked)
    ds = img[prj.dgroup:]
    W = lambda b, k: struct.unpack_from("<H", b, k)[0]
    holes = {fo: (tgt, disp) for seg, fo, kind, tgt, disp in o.fix if seg == "_DATA" and kind == "off"}
    dbad = []
    k = 0
    while k < len(d):
        if k in holes:
            tgt, disp = holes[k]
            if k + 2 > len(d):                              # (a fix-up word the object's data cannot hold: damaged)
                dbad.append("%04x: a fix-up word past the object's data" % (dstart + k))
                break
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
            mod, ("%05x" % row[1]) if row[1] is not None else "  -  ", len(j["res"]), len(s), row[3] or 0, (row[3] or 0) + j["dlen"],
            "IDENTICAL" if not j["dbad"] else "%d differ" % len(j["dbad"])))
        if "-v" in argv:
            for x in j["res"]:
                if x[3] != "same":
                    print("     %-12s %s %s" % (x[0], ("%05x" % x[1]) if x[1] is not None else "?", x[3]))
            for x in j["dbad"][:20]:
                print("     data " + x)
    print("%d functions in the modules, %d identical" % (tot, same))
    return 0
