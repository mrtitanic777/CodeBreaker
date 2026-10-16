"""linkwhole.py -- a program linked whole from its source files by a period linker, and compared with the original.

    python -m codebreaker exact PROJECT link [--keep DIR] [-v]       (a project whose "link" names a "linker")

For a program made of whole source files (each a module: PL/M, assembler, C compiled whole), each file of each
source set is compiled by its set's toolchain, the objects are put in the order of their places in the program
(the address in each file's name), and the linker the project names is run on them under the DOS emulator. The
file it writes is compared with the original byte for byte.

project.json "link":
    "linker"     the LINK.EXE to run (a path from the project's folder, like "image")
    "switches"   its switches, e.g. "/STACK:3072"
    "output"     the run file's name (default NEW);  "map": false - no map (NUL), else one of that name
                 (the names change nothing the linker makes of the modules, but some linkers leave a trace of
                 their names' places in the header)
    "names"      the objects' names: "sequence:M" (default: MA, MB, .. - short, so the command line holds
                 them), "pairs:D" (DA .. DZ, EA ..), or "files"
    "placed"     the pattern of files of no code that have a place in the order all the same (group 1 the
                 address), compiled by the first set's toolchain - e.g. a module of shared declarations
    "last"       files of no code and no place: after all the others, in this order
    "find"       [{"file", "toolchain", "variant"}]: modules with no place in their name, each placed where
                 its own code is found in the program (the bytes the linker fills in passed over)
    "first"      objects before all - [NAME, file]: NAME.OBJ on the command line
    "libraries"  libraries after the objects
                 (a file is from the project's folder, or "KIT:path" in a kit)
    "class"      {CLASS: {"segments": [names], "files": [files]}}: those files' segments of those names are given
                 the class CLASS before the link (a class the compiler cannot give: the linker orders by class)
    "accept"     what may differ, each with "why": {"tail_zeros": N} (the original has N zero bytes more at
                 its end; "record": as many as fill its last 128-byte record), {"header": [FROM, TO]} (header bytes FROM..TO-1, and the checksum counting them)
    "psp"        the segment DOS loads the linker at (some linkers leave their own addresses in the header)
"""
import os
import re
import shutil
import struct
import tempfile


def _cfg(prj):
    return prj.cfg.get("link") or {}


def wanted(prj):
    return isinstance(prj.cfg.get("link"), dict) and "linker" in prj.cfg["link"]


def modules(prj):
    """[(place, file name, object bytes)] in link order"""
    lk = _cfg(prj)
    out = []
    sets = prj.sets()
    for part in sets:
        tc = part.toolchain()
        for at, path in part.sources():
            out.append((at, path, part, tc))
    main = sets[0]
    if lk.get("placed"):
        for f in sorted(os.listdir(main.src)):
            m = re.fullmatch(lk["placed"], f, re.I)
            if m:
                out.append((int(m.group(1), 16), os.path.join(main.src, f), main, main.toolchain()))
    for x in lk.get("find", []):
        from codebreaker.exact import toolchain as tcm
        tc = tcm.load(x["toolchain"], x.get("variant"))
        path = os.path.join(main.src, x["file"])
        raw, con = tc.compile(open(path, "rb").read(), headers=main.headers(), include=main.include)
        at = _found(prj, raw) if raw is not None else None
        if at is None:
            raise SystemExit("%s: %s" % (x["file"], "does not compile" if raw is None else "its code is not found in the program"))
        out.append((at, path, None, raw))
    out.sort(key=lambda x: x[0])
    for k, f in enumerate(lk.get("last", [])):
        out.append((0x100000 + k, os.path.join(main.src, f), main, main.toolchain()))
    objs = []
    for at, path, part, tc in out:
        if part is None:                                    # (compiled already: placed by its bytes)
            objs.append((at, os.path.basename(path), tc))
            continue
        raw, con = tc.compile(open(path, "rb").read(), headers=part.headers(), include=part.include, switches=part.switches)
        if raw is None:
            raise SystemExit("%s does not compile:\n%s" % (os.path.basename(path), (con or "")[-1500:]))
        objs.append((at, os.path.basename(path), raw))
    return objs


def _found(prj, raw):
    """the image address of the object's code: the one place its bytes (but those the linker fills in) are found"""
    from codebreaker.exact import omf
    o = omf.read(raw)
    cs = omf.code_segments(o) or [s for s, cls, ln, al in o.segs if o.filled.get(s)]
    code = bytes(o.seg[cs[0]][:o.filled.get(cs[0], 0)])
    hole = omf.holes(o, cs[0])
    best = (0, 0)
    r = 0
    while r < len(code):                                    # the longest run with no hole, to look for
        e = r
        while e < len(code) and e not in hole:
            e += 1
        best = max(best, (e - r, r))
        r = e + 1
    n, r = best
    if n < 8:
        return None
    img = prj.image
    hits = []
    k = img.find(code[r:r + n])
    while k >= 0:
        a = k - r
        if a >= 0 and a + len(code) <= len(img) and all(img[a + x] == code[x] for x in range(len(code)) if x not in hole):
            hits.append(a)
        k = img.find(code[r:r + n], k + 1)
    return hits[0] if len(hits) == 1 else None


def _own(b):
    """the object module up to its end record (an assembler's file may be padded to its disk sector)"""
    p = 0
    while p + 3 <= len(b):
        t = b[p]
        p += 3 + (b[p + 1] | b[p + 2] << 8)
        if t in (0x8A, 0x8B):
            return b[:p]
    return b


def _file(prj, spec):
    from codebreaker.exact import toolchain as tcm
    return tcm.kit_path(None, spec, base=prj.dir)


def _name(scheme, k, f):
    kind, _, pre = (scheme or "sequence:M").partition(":")
    if kind == "files":
        return re.sub(r"[^A-Z0-9]", "", os.path.splitext(f)[0].upper())[:8]
    if kind == "pairs":
        return chr(ord(pre or "D") + k // 26) + chr(65 + k % 26)
    return (pre or "M") + (chr(65 + k) if k < 26 else chr(48 + k - 26) if k < 36 else "%02d" % k)


def _index(b, q):
    return (((b[q] & 0x7F) << 8) | b[q + 1], q + 2) if b[q] & 0x80 else (b[q], q + 1)


def reclass(obj, cls, segments):
    """the object with its segments of those names (a name, or one ending in _NAME) in the class cls"""
    out = bytearray()
    names = []
    added = None
    i = 0
    want = {s.upper() for s in segments}
    while i + 3 <= len(obj):
        t = obj[i]
        n = obj[i + 1] | obj[i + 2] << 8
        body = bytearray(obj[i + 3:i + 2 + n])
        i += 3 + n
        if t == 0x96:                                       # LNAMES: the class's name added to the first
            p = 0
            while p < len(body):
                names.append(body[p + 1:p + 1 + body[p]].decode("latin1"))
                p += 1 + body[p]
            if added is None:
                body += bytes([len(cls)]) + cls.encode()
                names.append(cls)
                added = len(names)
        elif t == 0x98 and added is not None:               # SEGDEF: ACBP [frame offset] length name class overlay
            p = 1 + (3 if (body[0] >> 5) == 0 else 0) + 2
            nm, q = _index(body, p)
            nmq = names[nm - 1].upper() if 0 < nm <= len(names) else ""
            if nmq in want or any(nmq.endswith("_" + s) for s in want):
                if body[q] & 0x80 or added > 0x7F:
                    raise SystemExit("reclass: a two-byte name index is not handled")
                body[q] = added
        r = bytes([t]) + (len(body) + 1).to_bytes(2, "little") + bytes(body)
        out += r + bytes([(-sum(r)) & 0xFF])
    return bytes(out)


def run(prj, objs, keep=None):
    """-> (the file written or None, the linker's words, its map)"""
    from codebreaker.exact.emu import dos
    lk = _cfg(prj)
    w = keep or tempfile.mkdtemp(prefix="exlinkw_")
    os.makedirs(w, exist_ok=True)
    try:
        shutil.copy(_file(prj, lk["linker"]), os.path.join(w, "LINK.EXE"))
        names = []
        for n, f in lk.get("first", []):
            shutil.copy(_file(prj, f), os.path.join(w, n + ".OBJ"))
            names.append(n)
        cls = {f: (c, v.get("segments", [])) for c, v in lk.get("class", {}).items() for f in v.get("files", [])}
        for k, (at, f, raw) in enumerate(objs):
            raw = _own(raw)
            if f in cls:
                raw = reclass(raw, cls[f][0], cls[f][1])
            n = _name(lk.get("names"), k, f)
            names.append(n)
            open(os.path.join(w, n + ".OBJ"), "wb").write(raw)
        libs = []
        for f in lk.get("libraries", []):
            shutil.copy(_file(prj, f), w)
            libs.append(os.path.splitext(os.path.basename(f))[0].upper())
        out = lk.get("output", "NEW")
        cmd = "+".join(names) + "," + out + "," + (out if lk.get("map", True) else "NUL") + ("," + "+".join(libs) if libs else "") + lk.get("switches", "") + ";"
        psp = lk.get("psp")
        psp = int(psp, 0) if isinstance(psp, str) else psp
        if len(cmd) > 120:                                  # (a DOS command line holds 127 characters: the answers in
            open(os.path.join(w, "L.LNK"), "wb").write(("\r\n".join(cmd.rstrip(";").split(",")) + ";\r\n").encode())
            r = dos.run(os.path.join(w, "LINK.EXE"), w, "@L.LNK", psp=psp)     # a response file, one a line)
        else:
            r = dos.run(os.path.join(w, "LINK.EXE"), w, cmd, psp=psp)
        con = r.console.decode("latin1").replace("\r", "")
        p = os.path.join(w, out + ".EXE")
        exe = open(p, "rb").read() if os.path.exists(p) else None
        mp = os.path.join(w, out + ".MAP")
        return exe, con, open(mp, encoding="latin1").read() if os.path.exists(mp) else ""
    finally:
        if not keep:
            shutil.rmtree(w, ignore_errors=True)


def compare(prj, exe):
    """-> (identical, [lines]): the file against the original, but for what the project accepts"""
    ref = prj.file
    if exe == ref:
        return True, []
    lk = _cfg(prj)
    skip = set()
    said = []
    n = len(exe)
    for a in lk.get("accept", []):
        tz = a.get("tail_zeros")
        if tz == "record":
            tz = -len(exe) % 128
        if tz and len(ref) == len(exe) + tz and not any(ref[len(exe):]):
            n = len(ref)
            said.append("the original has %d zero bytes more at its end (%s)" % (tz, a.get("why", "")))
        if "header" in a:
            lo, hi = (int(x, 0) if isinstance(x, str) else x for x in a["header"])
            d = [k for k in range(lo, hi) if k < min(len(ref), len(exe)) and ref[k] != exe[k]]
            if d:
                skip |= set(range(lo, hi)) | {0x12, 0x13}
                said.append("header bytes %02x..%02x and the checksum (%s)" % (lo, hi - 1, a.get("why", "")))
    if n != len(ref):
        return False, ["the file is %d bytes, the original %d" % (len(exe), len(ref))]
    bad = [k for k in range(min(len(exe), len(ref))) if k not in skip and exe[k] != ref[k]]
    if bad:
        h = struct.unpack_from("<H", ref, 8)[0] * 16
        return False, ["%d bytes differ, the first at file offset %05x (%s)" % (len(bad), bad[0], "the header" if bad[0] < h else "image %05x" % (bad[0] - h))]
    return True, said


def main(prj, argv):
    keep = argv[argv.index("--keep") + 1] if "--keep" in argv else None
    objs = modules(prj)
    if "-v" in argv:
        for at, f, raw in objs:
            print("   %05x %s" % (at if at < 0x100000 else 0, f))
    exe, con, mp = run(prj, objs, keep)
    msgs = [l for l in con.split("\n") if l.strip() and "Microsoft" not in l and "Copyright" not in l and "Version" not in l]
    for l in msgs[:12]:
        print("   LINK: " + l)
    if exe is None:
        print("no program was written")
        return 1
    ok, lines = compare(prj, exe)
    for l in lines:
        print("   " + l)
    print("%s linked from %d modules: %d bytes; %s" % (os.path.basename(prj.exe), len(objs), len(exe),
          ("the FILE IDENTICAL" if exe == prj.file else "identical but for what the project accepts") if ok else "not identical"))
    return 0 if ok else 1
