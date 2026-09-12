#!/usr/bin/env python3
"""
difdec.py — differential decode validator.

The whole project's byte-level reading rests on ONE decoder (objdump, via
disasm.py / trace.py). A single decoder cannot be checked against itself. This
tool adds a second, independent opinion — the 8086 opcode ground truth harvested
from gzili's disassembler (opc8086.py) — and reports every place the two
disagree. Two kinds of disagreement, both meaningful:

  ANACHRONISM  objdump decoded a 186/286/386 instruction (ENTER, PUSHA, a
               32-bit register form, any 0F-prefixed opcode). Windows 1.01 is
               8086-only, so on real code this cannot happen: the region is
               almost certainly DATA that a linear sweep walked through as code,
               or a decode that desynchronised on a data byte. Either way it
               marks the code/data boundary the carver got wrong.

  ILLEGAL      the leading opcode byte is undefined on the 8086 (objdump often
               still prints something, or `(bad)`). Same conclusion.

A clean module — one whose carved functions are all genuine 8086 code — reports
ZERO of both. So the count per module is a direct quality signal: it says how
much of what we carved as code is actually code.

This is a VALIDATOR, not a re-decoder: it trusts objdump's instruction
boundaries and only judges whether each decoded instruction is 8086-legal. What
it flags is handed back as offsets to look at, never silently "fixed".

Usage:
    python difdec.py KERNEL GDI USER          # system modules
    python difdec.py --apps                    # every app binary
    python difdec.py CARDFILE --seg 4          # one segment, verbose
    python difdec.py --all                     # everything, summary table
"""
import os
import re
import subprocess
import sys
import tempfile

import opc8086
import idord
import ledger
import trace
from codebreaker.disasm.disasm import open_ne

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE

APPS = ["CALC", "CALENDAR", "CARDFILE", "CLIPBRD", "CLOCK", "CONTROL",
        "NOTEPAD", "PAINT", "REVERSI", "SPOOLER", "TERMINAL", "WRITE"]

# objdump line: "   1a3c:\t55                   \tpush   bp"
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\t([0-9a-fA-F ]+)\t(.*)$")

# the byte columns objdump prints may include a segment/rep prefix before the
# real opcode; these are the legal 8086 prefixes we skip to find the opcode.
PREFIX = {0x26, 0x2E, 0x36, 0x3E, 0xF0, 0xF2, 0xF3}


def decode(blob, base=0):
    """-> [(offset, [bytes], asm_text)] from objdump, bytes column kept."""
    fd, tmp = tempfile.mkstemp(suffix="_dd.bin")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(blob)
        out = subprocess.run(
            ["objdump", "-D", "-b", "binary", "-mi386",
             "-Maddr16,data16,intel", "--adjust-vma=%d" % base, tmp],
            capture_output=True, text=True).stdout
    finally:
        try:
            os.remove(tmp)
        except OSError:
            pass
    rows = []
    for l in out.splitlines():
        m = LINE.match(l)
        if not m:
            continue
        off = int(m.group(1), 16)
        bs = [int(x, 16) for x in m.group(2).split()]
        rows.append((off, bs, m.group(3).strip()))
    return rows


def opcode_of(bs):
    """The real opcode byte, skipping legal 8086 prefixes."""
    for b in bs:
        if b in PREFIX:
            continue
        return b
    return bs[0] if bs else None


def judge(off, bs, asm):
    """-> (kind, reason) if this instruction is not 8086-legal, else None."""
    if not bs:
        return None
    if "(bad)" in asm:
        return ("ILLEGAL", "objdump could not decode (bad)")
    op = opcode_of(bs)
    if op is None:
        return None
    r = opc8086.post8086_reason(op, asm)
    if r:
        kind = "ANACHRONISM" if op in opc8086.POST8086 \
            or opc8086.POST8086_MNEM.search(asm) else "ILLEGAL"
        return (kind, r)
    return None


def carved_regions(ne):
    """[(seg_index, off, size)] for every carved function, or whole segments
    if carving is unavailable (still a valid — if coarser — check)."""
    out, o = [], trace.load_ordinals(os.path.join(ROOT, "ORDINALS.md"))
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, o)
            for start, f in sg.funcs.items():
                out.append((i, start, max(1, f["end"] - start)))
        except Exception:
            out.append((i, 0, s["length"]))
    return out


def scan(ne, only_seg=None):
    """-> (findings, n_instr). findings = [(seg, off, kind, reason, asm)]."""
    segs = {s["idx"]: s for s in ne.segments()}
    findings, ninstr = [], 0
    for seg, off, size in carved_regions(ne):
        if only_seg and seg != only_seg:
            continue
        s = segs[seg]
        blob = ne.data[s["file_off"] + off: s["file_off"] + off + size]
        for ioff, bs, asm in decode(blob, off):
            ninstr += 1
            v = judge(ioff, bs, asm)
            if v:
                findings.append((seg, ioff, v[0], v[1], asm))
    return findings, ninstr


def run(name, ne, only_seg=None, verbose=False):
    findings, ninstr = scan(ne, only_seg)
    anach = sum(1 for f in findings if f[2] == "ANACHRONISM")
    illeg = sum(1 for f in findings if f[2] == "ILLEGAL")
    pct = 100.0 * (ninstr - len(findings)) / ninstr if ninstr else 100.0
    print("%-10s %7d instr   %4d anachronism   %4d illegal   %6.2f%% 8086-clean"
          % (name, ninstr, anach, illeg, pct))
    if verbose:
        for seg, off, kind, reason, asm in findings[:60]:
            print("    seg%d:0x%04x  %-11s %-40s | %s"
                  % (seg, off, kind, reason[:40], asm[:40]))
        if len(findings) > 60:
            print("    ... %d more" % (len(findings) - 60))
    return dict(instr=ninstr, anach=anach, illeg=illeg, clean=pct)


def main():
    args = sys.argv[1:]
    verbose = "-v" in args or "--seg" in args
    only_seg = None
    if "--seg" in args:
        only_seg = int(args[args.index("--seg") + 1])
    names = [a.upper() for a in args if not a.startswith("-")
             and not a.isdigit()]

    print("differential decode: objdump vs the 8086 ground truth (opc8086)\n")
    rows = []
    if "--apps" in args or "--all" in args:
        for a in APPS:
            try:
                rows.append((a, run(a, open_ne(os.path.join(
                    ROOT, "files", "WF", a + ".EXE")))))
            except Exception as e:
                print("%-10s (skipped: %s)" % (a, e))
    if "--all" in args or (not names and "--apps" not in args):
        for m in ledger.SYSMODS:
            try:
                rows.append((m, run(m, idord.open_module(m))))
            except Exception as e:
                print("%-10s (skipped: %s)" % (m, e))
    for n in names:
        p = os.path.join(ROOT, "files", "WF", n + ".EXE")
        ne = open_ne(p) if os.path.exists(p) else idord.open_module(n)
        rows.append((n, run(n, ne, only_seg, verbose=True)))

    if len(rows) > 1:
        ti = sum(r["instr"] for _, r in rows)
        ta = sum(r["anach"] for _, r in rows)
        tl = sum(r["illeg"] for _, r in rows)
        print("-" * 74)
        print("%-10s %7d instr   %4d anachronism   %4d illegal   %6.2f%% clean"
              % ("TOTAL", ti, ta, tl,
                 100.0 * (ti - ta - tl) / ti if ti else 100.0))


if __name__ == "__main__":
    main()
