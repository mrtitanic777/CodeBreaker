#!/usr/bin/env python3
"""
annotate.py — non-invasive readability layer for reconstructed .asm.

Adds COMMENT lines labeling the data regions a byte-exact reconstruction emits as
opaque `db` bytes — e.g. `; ---- data: string "GetAsyncKeyState" ----` — using
the `datacode` classifier. It inserts comments ONLY; it never changes an
instruction or a byte, so the reconstruction still reassembles to the shipped
bytes exactly. This is the safe way to gain readability without risking the
byte-exact guarantee (unlike emitting data AS data, which needs reachability
analysis to avoid misclassifying printable code).

    python annotate.py MOUSE               # re-annotate reconstructed/asm/mouse/*.asm
"""
import argparse
import glob
import os
import re
import sys
from src.core import layout

_OFF = re.compile(r";\s*([0-9a-f]{4})\b")
# ORDINALS.md rows: `| KERNEL.15 | `GLOBALALLOC` |` (NE) and
# `| KERNEL32.dll.50 | `AddAtomA` |` (PE). Module = everything before the last dot.
_ORD_RX = re.compile(r"\|\s*([A-Za-z0-9_.]+)\.(\d+)\s*\|\s*`([^`]+)`")
_ords = None


def _ordmap():
    """{(module_lower, ordinal): name} parsed once from ORDINALS.md."""
    global _ords
    if _ords is None:
        _ords = {}
        try:
            for m in _ORD_RX.finditer(open("ORDINALS.md", encoding="utf-8",
                                            errors="replace").read()):
                _ords[(m.group(1).lower(), int(m.group(2)))] = m.group(3)
        except OSError:
            pass
    return _ords


def _resolve(tgt):
    """'KERNEL.15' -> '-> KERNEL.GlobalAlloc  [KERNEL.15]' if it resolves, else
    '-> KERNEL.15'. Intra-segment 'segN:0xNNNN' passes through unchanged."""
    m = re.match(r"(.+)\.(\d+)$", tgt)
    if not m:
        return "-> %s" % tgt
    mod, ordn = m.group(1).lower(), int(m.group(2))
    om = _ordmap()
    for key in (mod, mod + ".exe", mod + ".dll"):
        name = om.get((key, ordn))
        if name:
            return "-> %s.%s  [%s]" % (mod.upper(), name, tgt)
    return "-> %s" % tgt


_ANN_LINE = re.compile(r"^\s*;\s*(----|====)")
_ANN_NOTE = re.compile(r"\s+-> \S.*$")


def _strip(asm_text):
    """Remove any annotation this tool previously added, so re-annotating is
    idempotent (no duplicated labels / call notes)."""
    out = []
    for ln in asm_text.splitlines():
        if _ANN_LINE.match(ln):
            continue
        out.append(_ANN_NOTE.sub("", ln))
    return "\n".join(out)


def annotate(asm_text, sg, start, end):
    """Return asm_text with readability comments — comment/label only, so it
    assembles to the identical bytes:
      * a named FUNCTION header (from the export table),
      * `-> MODULE.Api` on each external call site (from the fixup table),
      * a `; ---- data: string "..." ----` label on each data region.
    """
    from src.engines.build import datacode
    lines = _strip(asm_text).splitlines()
    line_off = [(i, int(m.group(1), 16)) for i, ln in enumerate(lines)
                if (m := _OFF.search(ln))]
    off_to_idx = {}
    for i, o in line_off:
        off_to_idx.setdefault(o, i)

    # 1) call-site targets -> the row (line) that contains each fixup site
    rows = sorted(o for o, _ in sg.rows if start <= o < end)
    span = {o: (rows[i + 1] if i + 1 < len(rows) else end) for i, o in enumerate(rows)}
    call_note = {}                                  # line index -> "-> ..."
    for site, tgt in (getattr(sg, "fixups", {}) or {}).items():
        if not (start <= site < end):
            continue
        r = max((o for o in rows if o <= site), default=None)
        if r is None or site >= span.get(r, r + 1):
            continue
        idx = off_to_idx.get(r)
        if idx is not None:
            call_note[idx] = _resolve(tgt)

    # 2) data regions -> a label inserted before the covering line
    inserts = {}                                    # line index -> [comment, ...]
    for s, e, k in datacode.data_regions(sg):
        if not (s >= start and e <= end and e > s):
            continue
        below = [i for i, o in line_off if o <= s]
        di = below[-1] if below else (line_off[0][0] if line_off else 0)
        if k == "str":
            txt = "".join(chr(b) if 0x20 <= b < 0x7F else "." for b in sg.code[s:e])
            if len(txt) > 56:
                txt = txt[:56] + "..."          # keep the label under MASM's line limit
            inserts.setdefault(di, []).append(
                '        ; ---- data: string "%s"  (%04x-%04x) ----' % (txt, s, e))
        else:
            inserts.setdefault(di, []).append(
                "        ; ---- data: table/blob  (%04x-%04x) ----" % (s, e))

    # 3) function header from the export table
    header = None
    name = (getattr(sg, "export_names", {}) or {}).get(start)
    if name:
        header = "; ========  FUNCTION: %s  (seg%d:0x%04x)  ========" % (
            name, getattr(sg, "si", 0), start)

    out = []
    if header:
        out.append(header[:120])
    for i, ln in enumerate(lines):
        for c in inserts.get(i, []):
            out.append(c)
        if i in call_note:                          # keep the line under MASM's ~128 limit
            ln = (ln.rstrip() + "   " + call_note[i])[:120]
        out.append(ln)
    return "\n".join(out) + "\n"


def _annotate_module(module):
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from src.formats import ne as ne_mod
    import trace, target
    o = trace.load_ordinals("ORDINALS.md")
    ne = ne_mod.NEFile(target.resolve(module))
    segs = {}
    d = os.path.join(layout.asm_dir(), module.lower())
    files = glob.glob(os.path.join(d, "seg*_*.asm"))
    n_ann = 0
    for f in files:
        m = re.search(r"seg(\d+)_([0-9a-f]+)\.asm$", os.path.basename(f))
        if not m:
            continue
        seg = int(m.group(1)); start = int(m.group(2), 16)
        if seg not in segs:
            segs[seg] = trace.Seg(ne, seg, o)
        sg = segs[seg]
        fn = sg.funcs.get(start)
        end = fn["end"] if fn else len(sg.code)
        text = open(f, encoding="utf-8", errors="replace").read()
        ann = annotate(text, sg, start, end)
        if ann != text:
            open(f, "w", newline="\r\n", encoding="utf-8").write(ann)
            n_ann += 1
    return len(files), n_ann


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    args = ap.parse_args()
    total, ann = _annotate_module(args.module)
    print("annotated %d of %d reconstructed .asm file(s) for %s (data regions labeled)"
          % (ann, total, args.module.upper()))


if __name__ == "__main__":
    main()
