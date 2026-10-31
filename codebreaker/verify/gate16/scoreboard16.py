"""scoreboard16.py -- where each of a 16-bit module's functions differs from the original, and whose
work that difference is.

    python -m codebreaker score16 <16-bit module> [--segment N] [--json FILE]

Every reconstructed function is compiled twice from the same C, in the same source files and with
the same switches: by Microsoft C 8.00c (the compiler that built the module) and by OpenWatcom with
-zmsc (the fork that is to make Microsoft C 8's choices). Three comparisons follow:

    C          Microsoft C 8's bytes against the original's: where they differ, the C is not what was
               written -- the decompiler's work
    compiler   OpenWatcom's bytes against Microsoft C 8's, from the same C: where they differ, the
               fork does not yet do what Microsoft C 8 does -- the compiler's work
    fits       OpenWatcom's length against the original's: a function no longer than the original
               goes back into its own place, and nothing in the segment moves

The decompiler's patterns are counted too (far pointers split into int pairs, CONCAT22, MK_FP,
segment registers read through helpers, single-use temporaries), with the bytes the functions that
show each one are off by, so the largest is the next to remove.
"""
import json
import os
import re
import subprocess
import tempfile
from codebreaker import platforms
from codebreaker.verify import verdicts as V

# what the decompiler writes that a programmer does not, and that a compiler turns into more code
PATTERNS = {
    "CONCAT": re.compile(r"\bCONCAT\d\d\("),
    "MK_FP": re.compile(r"\bMK_FP\("),
    "segment helper": re.compile(r"\bcodebreaker_(ss|ds|cs|es)\(\)"),
    "split far (>> 0x10)": re.compile(r">>\s*0x10\b"),
    "xunknown": re.compile(r"\bxunknown\d\b"),
    "unaff_/in_/extraout_": re.compile(r"\b(unaff|in|extraout)_[A-Z]{2}\b"),
    "SUB/ZEXT/SEXT": re.compile(r"\b(SUB|ZEXT|SEXT)\d\d\("),
    "constant DS address": re.compile(r"\*\s*\(\s*\w+\s*\*\s*\)\s*0x[0-9a-f]{2,4}\b"),
}


def watcom_flags(ms):
    """OpenWatcom's switches for Microsoft's (tests/msc8/suite.py keeps the same mapping)"""
    out = ["-bt=dos", "-zq", "-w0", "-zl", "-ecc", "-zu", "-zmsc", "-j", "-zastd=c99"]   # (dos: no Windows prolog -- /Gw gives -zW)
    for f in ms.split():
        f = f.lstrip("/-")
        if f.startswith("A"):
            out.append({"AS": "-ms", "AM": "-mm", "AC": "-mc", "AL": "-ml"}.get(f, "-mm"))
        elif f.startswith("G"):
            for c in f[1:]:
                out += {"2": ["-2"], "3": ["-3"], "s": ["-s"], "w": ["-zW"]}.get(c, [])
        elif f.startswith("O"):
            opts = f[1:]
            out.append("-od" if opts == "d" else "-obmilr" if "x" in opts else "-o" + opts)
            if "s" in opts and "x" in opts:
                out.append("-os")
    return out


def _same(a, b):
    """the code alike, a fix-up's bytes (either side's) aside"""
    (ca, ma), (cb, mb) = a, b
    return len(ca) == len(cb) and all(x == y or ma[i] or mb[i] for i, (x, y) in enumerate(zip(ca, cb)))


def _watcom(files, texts, header, settings_of, log):
    """{function: (bytes, mask) or None}: each source file compiled by OpenWatcom -zmsc as Microsoft C
    compiled it (the same functions together, the same switches)"""
    from codebreaker.build import msc6
    P = platforms.of("ne")
    wcc = P.compiler.tool()
    env = P.compiler.env()
    work = tempfile.mkdtemp(prefix="score16_")
    with open(os.path.join(work, "m.h"), "w", newline="\n") as f:
        f.write(header)
    out, why = {}, {}

    def one(stem, names):
        with open(os.path.join(work, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write('#include "m.h"\n\n' + "\n\n".join(texts[n] for n in names) + "\n")
        r = subprocess.run([wcc] + watcom_flags(settings_of[names[0]][1]) + ["-fo=" + stem + ".obj", stem + ".c"],
                           cwd=work, env=env, capture_output=True, text=True, errors="replace")
        obj = os.path.join(work, stem + ".obj")
        if r.returncode == 0 and os.path.isfile(obj):
            return open(obj, "rb").read(), ""
        errs = [ln for ln in (r.stdout + r.stderr).splitlines() if "Error!" in ln]
        return None, (errs[0] if errs else (r.stdout + r.stderr).strip()[-200:])

    for k, names in enumerate(files):
        names = [n for n in names if n in texts]
        if not names:
            continue
        data, err = one("f%04d" % k, names)
        if data is None and len(names) > 1:          # (one function's error does not sink its file)
            for j, n in enumerate(names):
                d, e = one("f%04d_%02d" % (k, j), [n])
                out[n] = msc6.function_code(d, n) if d else None
                if d is None:
                    why[n] = e
            continue
        for n in names:
            out[n] = msc6.function_code(data, n) if data else None
            if data is None:
                why[n] = err
    _watcom.refused = why
    if log and why:
        log("  OpenWatcom refused %d functions, e.g. %s: %s" % (len(why), next(iter(why)), next(iter(why.values()))))
    return out


def run(module_path, segment=None, log=print):
    from codebreaker.verify.authored import match, match16
    _chosen, results = match16.run(module_path, log=log, fold=False)   # (the C as the decompiler wrote it, as the build has it)
    names = [n for n in results if n in getattr(match16.run, "code", {})]
    m_where = getattr(match16.run, "where", None)
    texts = match16.run.texts
    seg_of = getattr(match16.run, "segment", {})
    if segment is not None:
        names = [n for n in names if seg_of.get(n) == segment]
    files = [[n for n in f if n in names] for f in match16.run.files]
    settings_of = match16.run.setting
    ow = _watcom([f for f in files if f], texts, match16.run.header, settings_of, log)
    rows = {}
    for n in names:
        ms, orig = match16.run.code[n]
        o = ow.get(n)
        msv = match.distance(ms, orig)[:3] if ms else (V.NO_COMPILE, 0, 0)
        orig_len = len(match._trim(*orig)[0])
        rows[n] = {
            "segment": seg_of.get(n),
            "original": orig_len,
            "msc8": len(match._trim(*ms)[0]) if ms else None,
            "msc8_vs_original": msv[0],
            "msc8_bytes_off": msv[1],
            "watcom": len(match._trim(*o)[0]) if o else None,
            "watcom_vs_msc8": ("same" if ms and o and _same(match._trim(*o), match._trim(*ms)) else
                               "differs" if ms and o else "n/a"),
            "fits": bool(o) and len(match._trim(*o)[0]) <= orig_len,
            "patterns": {p: len(rx.findall(texts.get(n, ""))) for p, rx in PATTERNS.items()
                         if rx.search(texts.get(n, ""))},
            "library": V.match(results[n][0]) == V.IDENTIFIED,
            "c": texts.get(n, ""),
            "hex": {"original": orig[0].hex(), "msc8": ms[0].hex() if ms else "", "watcom": o[0].hex() if o else ""},
        }
    return rows


def report(rows, out=print):
    c = [r for r in rows.values() if not r["library"]]
    n = len(c)
    ident = sum(V.match(r["msc8_vs_original"]) == V.IDENTICAL for r in c)
    same_ow = sum(r["watcom_vs_msc8"] == "same" for r in c)
    fits = sum(r["fits"] for r in c)
    out("%d functions (runtime library aside)" % n)
    out("  C        Microsoft C 8 gives the original's bytes: %4d   (decompiler's work: %d)" % (ident, n - ident))
    out("  compiler OpenWatcom -zmsc gives Microsoft C 8's:   %4d   (compiler's work: %d)" % (same_ow, n - same_ow))
    out("  fits     OpenWatcom's no longer than the original:  %4d   (would move: %d)" % (fits, n - fits))
    off = sum(max(0, (r["watcom"] or 0) - r["original"]) for r in c)
    out("  bytes over the original's length, all together: %d" % off)
    tally = {}
    for r in c:
        if V.match(r["msc8_vs_original"]) == V.IDENTICAL:
            continue
        for p in r["patterns"]:
            t = tally.setdefault(p, [0, 0])
            t[0] += 1
            t[1] += r["msc8_bytes_off"]
    out("  the decompiler's patterns in functions Microsoft C 8 does not match (functions, bytes off):")
    for p, (k, b) in sorted(tally.items(), key=lambda kv: -kv[1][1]):
        out("    %-24s %4d  %6d" % (p, k, b))


def show(row, out=print):
    """a function's C, and its original's, Microsoft C 8's and OpenWatcom's instructions side by side"""
    md = platforms.decoder(16)
    out(row["c"])
    cols = [["%s %s" % (i.mnemonic, i.op_str) for i in md.disasm(bytes.fromhex(row["hex"][k]), 0)]
            for k in ("original", "msc8", "watcom")]
    out("  %-32s| %-32s| %s" % ("original", "Microsoft C 8", "OpenWatcom -zmsc"))
    for i in range(max(len(c) for c in cols)):
        out("  %-32s| %-32s| %s" % tuple(c[i] if i < len(c) else "" for c in cols))


def main(argv):
    """python -m codebreaker score16 <16-bit module> [--segment N] [--json FILE]
       python -m codebreaker score16 --show FUNCTION --json FILE   (one function from a saved board)"""
    if "--show" in argv:
        rows = json.load(open(argv[argv.index("--json") + 1]))
        for fn in argv[argv.index("--show") + 1].split(","):
            print("=====", fn)
            show(rows[fn])
        return 0
    from codebreaker.base import corpus
    seg = int(argv[argv.index("--segment") + 1]) if "--segment" in argv else None
    js = argv[argv.index("--json") + 1] if "--json" in argv else None
    args = [a for a in argv if not a.startswith("--") and a not in (str(seg), js)]
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    rows = run(path, seg)
    report(rows)
    if js:
        with open(js, "w") as f:
            json.dump(rows, f, indent=1)
    return 0
