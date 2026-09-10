#!/usr/bin/env python3
r"""pipeline.py -- the two-pass reconstruction pipeline + gap instrumentation.

Every function flows through a strict hierarchy:

    Pass 1  templates.synthesize()  -- whole-function pattern match (fast, cross-DLL),
            oracle-verified byte-exact. If it matches, emit and stop.
    Pass 2  msc6dec.invert()        -- instruction-level inverter for arbitrary bodies.
            inverted | needs-link (intra-seg call, verify at link) | declined.
    Gap     on decline, LOG the basic block + opcode sequence + decline reason to a
            JSONL rule-gap database, so the highest-frequency 16-bit/MSC-6 idioms can be
            eliminated systematically (each fix auto-handles a whole class corpus-wide).

    from codebreaker.inverse_engine import pipeline
    rep = pipeline.reconstruct_binary("....dll", gaplog="gaps.jsonl", verify=mocklink.verify)
    pipeline.print_report(rep)
    pipeline.classify_gaps("gaps.jsonl")     # group declines by failure class

The point of the ratio (template : msc6dec : gap) is to be TRACKED across engine versions
as gaps are closed -- it is the thesis metric (years vs weeks).
"""
import os
import re
import sys
import json
import struct
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_MD = Cs(CS_ARCH_X86, CS_MODE_16)


def _disasm(fb, limit=24):
    return [(i.address, i.bytes.hex(), i.mnemonic, i.op_str)
            for n, i in enumerate(_MD.disasm(bytes(fb), 0)) if n < limit]


# ---- failure-class buckets (16-bit / MSC 6.0 compiler idioms) -------------------
_CLASSES = [
    ("segment-reg", re.compile(r"\b(mov (ax|es|ds), (ds|es|ax)|push (ds|es|cs)|lds|les)\b", re.I)),
    ("far-near-ptr", re.compile(r"\b(les|lds|__a[NF]|movsw|movsb|far)\b", re.I)),
    ("switch-table", re.compile(r"jmp .*\[.*\+.*\]|\bxlat\b", re.I)),
    ("dxax-long", re.compile(r"\b(cwd|cdq|adc dx|sbb dx|dx:ax|__aFuldiv|__aFulmul|idiv|imul)\b", re.I)),
    ("dgroup-store", re.compile(r"mov (word|byte) ptr \[0x[0-9a-f]+\]", re.I)),
    ("float-emu", re.compile(r"\b(fld|fst|fmul|fadd|__fp|wait)\b", re.I)),
]


def _classify(reason, disasm):
    text = (reason or "") + " " + " ".join("%s %s" % (m, o) for _, _, m, o in disasm[:6])
    for name, rx in _CLASSES:
        if rx.search(text):
            return name
    return "other"


def reconstruct_binary(path, gaplog=None, verify=None, limit=None):
    """Run the two-pass pipeline over every function of an NE binary. Returns a report
    with per-status counts; appends declines to `gaplog` (JSONL)."""
    from codebreaker.sdk import reconstruct as R
    from codebreaker.inverse_engine import templates
    from codebreaker.format_loaders import ne
    rep = R.analyze(path)                                    # Pass 2 (msc6dec) already run here
    # map seg idx -> raw segment bytes so we can extract EVERY function's bytes (declines
    # don't carry bytes in the analyze report)
    d = open(path, "rb").read(); lfa = struct.unpack_from("<I", d, 0x3c)[0]
    segbytes = {}
    try:
        for s in ne.NEFile.from_buffer(d, lfa, path).segments():
            if s.get("length"):
                segbytes[s["idx"]] = d[s["file_off"]:s["file_off"] + s["length"]]
    except Exception:
        pass
    counts = Counter()
    gaps = []
    name = os.path.basename(path)
    for seg in rep["segments"]:
        sb = segbytes.get(seg["idx"])
        for fn in seg["funcs"]:
            fb = bytes.fromhex(fn["bytes"]) if fn.get("bytes") else None
            if fb is None and sb is not None and fn.get("off") is not None:
                e = R._func_end(sb, fn["off"], len(sb))
                if e:
                    fb = bytes(sb[fn["off"]:e])
            status = fn.get("status", "?")
            # Pass 1: template fast-path (only if there IS a whole-function match)
            if fb and status not in ("inverted",):
                for tname, c in templates.candidates(fb):
                    if verify is None or verify(fb, c).get("exact"):
                        status = "template"; fn["template"] = tname; fn["c"] = c
                        break
            if status == "template":
                counts["template"] += 1
            elif status == "inverted":
                counts["msc6dec"] += 1
            elif status == "needs-link":
                counts["needs-link"] += 1
            elif status == "leaf/no-frame":
                counts["leaf"] += 1
            else:                                            # declined/error -> gap
                counts["gap"] += 1
                if fb:
                    dis = _disasm(fb)
                    gaps.append({"dll": name, "seg": seg["idx"], "off": fn.get("off"),
                                 "size": len(fb), "reason": str(fn.get("reason"))[:200],
                                 "klass": _classify(fn.get("reason"), dis),
                                 "head": " | ".join("%s %s" % (m, o) for _, _, m, o in dis[:8])})
            if limit and sum(counts.values()) >= limit:
                break
    if gaplog and gaps:
        with open(gaplog, "a", encoding="utf-8") as f:
            for g in gaps:
                f.write(json.dumps(g) + "\n")
    return {"dll": name, "counts": dict(counts), "total": sum(counts.values()), "gaps": gaps}


def print_report(rep):
    c = rep["counts"]; t = rep["total"] or 1
    print("== %s: %d functions ==" % (rep["dll"], rep["total"]))
    order = ["template", "msc6dec", "needs-link", "leaf", "gap"]
    for k in order:
        if c.get(k):
            print("   %-11s %4d  (%.0f%%)" % (k, c[k], 100.0 * c[k] / t))
    auto = c.get("template", 0) + c.get("msc6dec", 0)
    print("   -> auto-inversion (template+msc6dec): %d/%d = %.0f%%" % (auto, t, 100.0 * auto / t))


def classify_gaps(gaplog):
    """Group the logged declines by failure class -- the highest-frequency first, so the
    top gaps to close in msc6dec are obvious."""
    klass = Counter(); byclass = {}
    with open(gaplog, encoding="utf-8") as f:
        for line in f:
            g = json.loads(line)
            klass[g["klass"]] += 1
            byclass.setdefault(g["klass"], []).append(g)
    print("== decline gaps by class (%d total) ==" % sum(klass.values()))
    for k, n in klass.most_common():
        ex = byclass[k][0]
        print("   %-14s %4d   e.g. %s:seg%s:0x%x  %s"
              % (k, n, ex["dll"], ex["seg"], ex["off"] or 0, ex["reason"][:70]))
    return klass


def corpus(paths, gaplog=None):
    """Regression suite: run the pipeline over many gold NE binaries, aggregate the
    template:msc6dec:gap ratio corpus-wide. Track this across engine versions -- it is
    the coverage number that must go UP as msc6dec gaps close."""
    agg = Counter(); ndll = 0; per = []
    for p in paths:
        try:
            rep = reconstruct_binary(p, gaplog=gaplog)
        except Exception:
            continue
        if rep["total"] == 0:
            continue
        ndll += 1
        for k, v in rep["counts"].items():
            agg[k] += v
        per.append((rep["dll"], rep["total"], rep["counts"]))
    tot = sum(agg.values()) or 1
    print("== corpus regression: %d NE binaries, %d functions ==" % (ndll, sum(agg.values())))
    for k in ("template", "msc6dec", "needs-link", "leaf", "gap"):
        if agg.get(k):
            print("   %-11s %6d  (%.0f%%)" % (k, agg[k], 100.0 * agg[k] / tot))
    auto = agg.get("template", 0) + agg.get("msc6dec", 0)
    print("   -> auto-inversion corpus-wide: %d/%d = %.1f%%" % (auto, tot, 100.0 * auto / tot))
    return agg


def _ne_files(root):
    import glob
    out = []
    for p in glob.glob(os.path.join(root, "*")):
        if not p.lower().endswith((".dll", ".exe", ".drv", ".386", ".ocx", ".cpl")):
            continue
        try:
            d = open(p, "rb").read(0x400)
            if len(d) < 0x40 or d[:2] != b"MZ":
                continue
            lfa = struct.unpack_from("<I", d, 0x3c)[0]
            f = open(p, "rb"); f.seek(lfa)
            if f.read(2) == b"NE":
                out.append(p)
        except Exception:
            pass
    return sorted(out)


def main(argv):
    if not argv:
        print(__doc__); return 2
    if argv[0] == "classify":
        classify_gaps(argv[1]); return 0
    if argv[0] == "corpus":
        gaplog = argv[argv.index("--gaplog") + 1] if "--gaplog" in argv else None
        lim = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
        files = _ne_files(argv[1])[:lim]
        corpus(files, gaplog=gaplog)
        return 0
    gaplog = argv[argv.index("--gaplog") + 1] if "--gaplog" in argv else None
    rep = reconstruct_binary(argv[0], gaplog=gaplog)
    print_report(rep)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
