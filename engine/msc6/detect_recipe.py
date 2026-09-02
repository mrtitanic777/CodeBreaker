#!/usr/bin/env python3
"""detect_recipe.py -- automatically pin a DLL's compiler recipe from its own bytes.

Different DLLs were built with different flags (LZEXPAND=/Ox, VER=/Os, ...). This
tool determines the recipe per DLL with NO human input: it locates the DLL's WEP
(Windows Exit Procedure -- a tiny, near-universal export whose source space is a
handful of known forms), brute-forces {WEP source forms} x {model} x {/O level} in
ONE DOSBox launch, and reports the flag set(s) that reproduce WEP's exact bytes.

Because compile+diff is free, this is the brute-force engine applied to recipe ID.
It never guesses: if nothing matches, or several models remain ambiguous, it says so
(and the caller pins the residual ambiguity from a second, data-referencing function).

Usage: detect_recipe.py <dll> [--keep]
Writes _work/recipe/<dll>/recipe.json.
"""
import json, os, re, struct, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
from engine.msc6 import paths as _paths
from engine.msc6 import toolchain as tc
SYS = _paths.SYS_DIR                 # shipped-DLL corpus (oracles), from config msc6_data
DBX = _paths.DOSBOX                  # DOS host, from config dosbox_x

# candidate WEP bodies (the source space of a Windows Exit Procedure)
WEP_SRC = {
    "ret1":    "int far pascal WEP(nParam) int nParam; { return 1; }",
    "empty":   "int far pascal WEP(nParam) int nParam; { }",
    "loci":    "int far pascal WEP(nParam) int nParam; { int i; }",
    "loci1":   "int far pascal WEP(nParam) int nParam; { int i; return 1; }",
}
MODELS = ["AS", "AM", "AC", "AL"]          # small / medium / compact / large
OPTS = ["Ox", "Os", "O", "Ot", "Og"]       # optimization levels
COMPILERS = ["msc-6.0a", "msvc-1.52c"]     # MSC 6.0a (5.20-era) / C8 1.52c (5.60-era)
PROLOGUE = ["Gsw", "Gs", "Gw", ""]         # windows loadds / stack-off / windows / none


def parse_ne(path):
    d = open(path, "rb").read()
    ne = struct.unpack_from("<I", d, 0x3c)[0]
    if d[ne:ne+2] != b"NE":
        return None, None
    return d, ne


def find_wep(d, ne):
    """Return (seg_index0, offset, wep_bytes) for the exported WEP, or None."""
    g = lambda o: struct.unpack_from("<H", d, ne+o)[0]
    # find ordinal of name "WEP" in resident + nonresident name tables
    wep_ord = None
    for base, absolute in ((ne+g(0x26), False), (struct.unpack_from("<I", d, ne+0x2c)[0], True)):
        p = base
        while 0 < p < len(d):
            l = d[p]
            if l == 0:
                break
            nm = d[p+1:p+1+l].decode("latin1", "replace")
            ordn = struct.unpack_from("<H", d, p+1+l)[0]
            if nm.upper() == "WEP":
                wep_ord = ordn
            p += 1+l+2
    if wep_ord is None:
        return None
    # entry table: ordinal -> (seg, off)
    et = ne+g(0x04); p = et; o = 1; loc = None
    while p < et+g(0x06):
        c, ind = d[p], d[p+1]; p += 2
        if c == 0:
            break
        if ind == 0xFF:
            for _ in range(c):
                if o == wep_ord:
                    loc = (d[p+3], struct.unpack_from("<H", d, p+4)[0])
                p += 6; o += 1
        elif ind == 0:
            o += c
        else:
            for _ in range(c):
                if o == wep_ord:
                    loc = (ind, struct.unpack_from("<H", d, p+1)[0])
                p += 3; o += 1
    if not loc:
        return None
    seg, off = loc
    al = g(0x32) or 9; segtbl = ne+g(0x22)
    sec, length, flags, _ = struct.unpack_from("<HHHH", d, segtbl+(seg-1)*8)
    if off == 0 and length <= 0x40:
        # WEP owns a tiny segment: take it whole (keeps a trailing /Ox NOP, which the
        # compiled object also emits -- trimming at retf would drop it and break matching)
        body = d[(sec << al):(sec << al)+length]
    else:
        body = d[(sec << al)+off:(sec << al)+length]
        m = re.search(rb'\xca..|\xcb', body)
        if m:
            end = m.end()
            if end < len(body) and body[end] == 0x90:  # keep a trailing NOP if present
                end += 1
            body = body[:end]
    return seg-1, off, body


def obj_code(path):
    try:
        d = open(path, "rb").read()
    except OSError:
        return None
    p = 0; c = b''
    while p < len(d):
        rec = d[p]; ln = struct.unpack_from("<H", d, p+1)[0]; body = d[p+3:p+3+ln-1]; p += 3+ln
        if rec in (0xA0, 0xA1):
            i = 0; si = body[i]; i += 1
            if si >= 0x80:
                i += 1
            i += 2; c += body[i:]
    return c


def detect(dll, keep=False, cache=None):
    path = os.path.join(SYS, dll)
    d, ne = parse_ne(path)
    if d is None:
        return {"dll": dll, "error": "not NE"}
    wep = find_wep(d, ne)
    if wep is None:
        return {"dll": dll, "recipe": None, "note": "no WEP export -- pin from smallest function (@REVIEW)"}
    target = wep[2]
    if cache is None:
        cache = compile_matrix()
    ms, _ = match_cache(target, cache)
    if not ms:
        result = {"dll": dll, "wep_bytes": target.hex(), "matches": [], "recipe": None,
                  "note": "no WEP source form matched -- @REVIEW (library WEP / other compiler)"}
        od = os.path.join(HERE, "_work", "recipe", dll.rsplit(".", 1)[0].lower())
        os.makedirs(od, exist_ok=True); json.dump(result, open(os.path.join(od, "recipe.json"), "w"), indent=1)
        return result
    # linker stamp as compiler prior + prefer /Gsw (same as the corpus sweep)
    lv = (d[ne+2], d[ne+3]); exp = "msc-6.0a" if lv < (5, 30) else "msvc-1.52c"
    f = [m for m in ms if m["compiler"] == exp] or ms
    g = [m for m in f if m["prologue"] == "/Gsw"] or f
    ax = {k: sorted(set(m[k] for m in g)) for k in ("opt", "model", "compiler", "prologue")}
    one = lambda k: ax[k][0] if len(ax[k]) == 1 else ax[k]
    result = {"dll": dll, "wep_bytes": target.hex(), "matches": ms,
              "compiler": one("compiler"), "model": one("model"),
              "prologue": one("prologue"), "opt": one("opt"),
              "ambiguous": any(len(ax[k]) != 1 for k in ("opt", "model", "compiler", "prologue")),
              "recipe": None}
    if ms:
        cp = ax["compiler"][0]; o = ax["opt"][0]; p = ax["prologue"][0]
        mdl = "/AM" if len(ax["model"]) != 1 else ax["model"][0]
        pflag = "" if p == "/G2only" else " " + p
        result["recipe"] = "cl /c %s /G2%s %s  [%s]" % (mdl, pflag, o, cp)
    else:
        result["note"] = "no WEP source form matched -- @REVIEW (library WEP / other compiler)"
    od = os.path.join(HERE, "_work", "recipe", dll.rsplit(".", 1)[0].lower())
    os.makedirs(od, exist_ok=True)
    json.dump(result, open(os.path.join(od, "recipe.json"), "w"), indent=1)
    return result


def recipe_flags(dll, cache=None):
    """Return the cl compile flags (e.g. '/c /AM /G2 /Gsw /Os') for a DLL, for the
    synthesizer's compile+diff loop -- so it optimizes against the RIGHT compiler.
    Returns None (caller -> @REVIEW) when the recipe can't be pinned. Never guesses.
    Prefers a cached _work/recipe/<dll>/recipe.json; else detects."""
    rj = os.path.join(HERE, "_work", "recipe", dll.rsplit(".", 1)[0].lower(), "recipe.json")
    r = None
    if os.path.exists(rj):
        try:
            r = json.load(open(rj))
        except Exception:
            r = None
    if r is None or "opt" not in r:
        r = detect(dll, cache=cache)
    opt = r.get("opt")
    if not opt or isinstance(opt, list):          # unmatched or opt-ambiguous
        return None
    # model is usually WEP-ambiguous -> default /AM (medium, the DLL norm); confirm on a data-ref fn
    model = r.get("model")
    model = model if isinstance(model, str) else "/AM"
    prol = r.get("prologue")
    pflag = "" if (prol == "/G2only" or isinstance(prol, list)) else " " + (prol or "/Gsw")
    return "/c %s /G2%s %s" % (model, pflag, opt)


def compile_matrix():
    """Compile the DLL-INDEPENDENT WEP probe matrix ONCE per compiler; return
    {(compiler,src,model,prologue,opt): code}. One DOSBox launch per compiler."""
    cache = {}
    for comp in COMPILERS:
        try:
            cdir = tc.resolve("compiler", comp, "dir")
        except Exception:
            continue
        work = os.path.join(HERE, "_work", "recipe", "_matrix_" + comp)
        os.makedirs(work, exist_ok=True)
        for k, src in WEP_SRC.items():
            open(os.path.join(work, "%s.c" % k), "w").write(src+"\n")
        bat = ["@echo off", "path c:\\bin"]
        combos = []; idx = 0
        for si in WEP_SRC:
            for mi in MODELS:
                for pi in PROLOGUE:
                    for oi in OPTS:
                        g = "/G2" + (" /"+pi if pi else "")
                        bat.append("cl /c /%s %s /%s /Foo%d.obj %s.c >nul" % (mi, g, oi, idx, si))
                        combos.append(("o%d" % idx, comp, si, mi, pi, oi)); idx += 1
        bat.append("echo DONE > rdone.txt")
        open(os.path.join(work, "BLD.BAT"), "w").write("\r\n".join(bat)+"\r\n")
        # msvc-1.52c is Phar-Lap hosted -> its BIN (with DOSXNT) must be on PATH
        path = "C:\\BIN"
        open(os.path.join(work, "r.conf"), "w").write(
            "[cpu]\ncycles=20000\n[autoexec]\nmount c %s\nmount d %s\nset PATH=%s\nd:\ncall BLD.BAT\nexit\n"
            % (cdir.replace("/", "\\"), work.replace("/", "\\"), path))
        # reuse an already-compiled matrix (objects on disk) unless --recompile
        already = os.path.exists(os.path.join(work, "rdone.txt")) and \
            len([f for f in os.listdir(work) if f.lower().endswith(".obj")]) >= idx and \
            "--recompile" not in sys.argv
        if already:
            for tag, cp, si, mi, pi, oi in combos:
                c = obj_code(os.path.join(work, "%s.obj" % tag))
                if c is not None:
                    cache[(cp, si, "/"+mi, "/"+(pi or "G2only"), "/"+oi)] = c
            continue
        for f in os.listdir(work):
            if f.lower().endswith(".obj"):
                os.remove(os.path.join(work, f))
        try:
            subprocess.run([DBX, "-conf", os.path.join(work, "r.conf"), "-exit"],
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=400)
        except Exception:
            pass
        for tag, cp, si, mi, pi, oi in combos:
            c = obj_code(os.path.join(work, "%s.obj" % tag))
            if c is not None:
                cache[(cp, si, "/"+mi, "/"+(pi or "G2only"), "/"+oi)] = c
    return cache


def match_cache(target, cache):
    ms = [{"compiler": cp, "src": s, "model": m, "prologue": p, "opt": o}
          for (cp, s, m, p, o), c in cache.items() if c == target]
    opts = sorted(set(x["opt"] for x in ms))
    models = sorted(set(x["model"] for x in ms))
    comps = sorted(set(x["compiler"] for x in ms))
    prols = sorted(set(x["prologue"] for x in ms))
    return ms, {"opt": opts, "model": models, "compiler": comps, "prologue": prols}


def sweep():
    """Compile the matrix once, then auto-detect the recipe for every NE16 DLL."""
    tri = os.path.join(HERE, "_work", "triage_system.json")
    ne16 = [r["file"] for r in json.load(open(tri)) if r["format"] == "NE16"]
    print("compiling WEP probe matrix once (%d variants)..." % (len(WEP_SRC)*len(MODELS)*len(OPTS)))
    cache = compile_matrix()
    print("  matrix compiled: %d/%d variants produced objects" % (len(cache), len(WEP_SRC)*len(MODELS)*len(OPTS)))
    rows = []
    for dll in sorted(ne16, key=str.lower):
        d, ne = parse_ne(os.path.join(SYS, dll))
        if d is None:
            continue
        wep = find_wep(d, ne)
        if wep is None:
            rows.append({"dll": dll, "opt": None, "status": "NO-WEP"}); continue
        ms, _ = match_cache(wep[2], cache)
        if not ms:
            rows.append({"dll": dll, "opt": None, "status": "NO-MATCH"}); continue
        # disambiguate with the linker stamp as a compiler prior + prefer /Gsw prologue
        lv = (d[ne+2], d[ne+3])
        exp_comp = "msc-6.0a" if lv < (5, 30) else "msvc-1.52c"
        f = [m for m in ms if m["compiler"] == exp_comp] or ms
        g = [m for m in f if m["prologue"] == "/Gsw"] or f
        ax = {k: sorted(set(m[k] for m in g)) for k in ("opt", "compiler", "prologue")}
        amb = any(len(ax[k]) != 1 for k in ("opt", "compiler", "prologue"))
        rows.append({"dll": dll, "status": "AMBIG" if amb else "OK",
                     "compiler": ax["compiler"][0] if len(ax["compiler"]) == 1 else "ambig",
                     "opt": ax["opt"][0] if len(ax["opt"]) == 1 else ax["opt"],
                     "prologue": ax["prologue"][0] if len(ax["prologue"]) == 1 else "ambig",
                     "matches": len(ms)})
        od = os.path.join(HERE, "_work", "recipe", dll.rsplit(".", 1)[0].lower())
        os.makedirs(od, exist_ok=True)
        json.dump(rows[-1], open(os.path.join(od, "recipe.json"), "w"), indent=1)
    json.dump(rows, open(os.path.join(HERE, "_work", "RECIPE_SPREAD.json"), "w"), indent=1)
    from collections import Counter
    print("\n== RECIPE SPREAD over %d NE16 DLLs ==" % len(rows))
    st = Counter(r["status"] for r in rows)
    for k, v in st.most_common():
        print("  %-10s %d" % (k, v))
    ov = Counter((r.get("compiler"), r.get("opt")) for r in rows if r["status"] == "OK")
    print("\nunambiguous (compiler, /O) detected:")
    for k, v in ov.most_common():
        print("  %-28s %d" % (str(k), v))
    print("\nspread -> _work/RECIPE_SPREAD.json (+ per-DLL recipe.json)")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__); raise SystemExit(2)
    if sys.argv[1] == "--sweep":
        sweep(); raise SystemExit(0)
    r = detect(sys.argv[1], "--keep" in sys.argv)
    print("%-16s recipe: %s" % (r["dll"], r.get("recipe") or r.get("note")))
    if r.get("matches"):
        print("  opt=%s  model=%s  (%d WEP-form matches)%s"
              % (r["opt"], r["model"], len(r["matches"]),
                 "  [model ambiguous from WEP -- confirm on a data-ref fn]" if isinstance(r["model"], list) else ""))
