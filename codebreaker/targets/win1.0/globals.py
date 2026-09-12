#!/usr/bin/env python3
"""
globals.py — identify a module's global variables by WHO WRITES THEM.

A bare `ds:0x16` in a disassembly says nothing. But the set of functions that
WRITE a global, and the instructions they use to do it, usually says everything:

    seg1:0x033c  inc WORD PTR ds:0x16     inside CreateWindow
    seg1:0x0886  dec WORD PTR ds:0x16     inside DestroyWindow

A word incremented by exactly one function and decremented by exactly its
inverse is a COUNT of the thing those two create and destroy. That conclusion is
evidence, not inference — and it is mechanical, so a tool can do the gathering
and leave only the naming to a human.

This is deliberately a REPORTER, not a namer. It prints what touches each
global and how; it never writes a name into ORDINALS.md. The project's rule is
that names come from evidence a person has weighed, and the whole value here is
putting that evidence on one screen.

    python globals.py USER              every global, most-written first
    python globals.py USER --seg 1      restrict to one segment
    python globals.py USER --at 0x16    everything about one global
    python globals.py USER --counters   only the inc/dec pairs (the clearest)

Reads the two-file image through idord.open_module, so overlay segments are
covered too.
"""
import bisect
import re
import sys
from collections import defaultdict

import idord
import trace

# `mov ds:0x16,ax` / `inc WORD PTR ds:0x16` — a write; `mov ax,ds:0x16` and
# `cmp WORD PTR ds:0x16,0x1` — a read. The distinction is what makes the report
# worth reading, so it is drawn from the mnemonic and the operand ORDER.
WRITE_RE = re.compile(r"^(mov|inc|dec|add|sub|and|or|xor)\s+(BYTE|WORD|DWORD) PTR ds:0x([0-9a-f]+)")
MOVW_RE = re.compile(r"^mov\s+ds:0x([0-9a-f]+),")
READ_RE = re.compile(r"ds:0x([0-9a-f]+)")


def scan(mod, only_seg=None):
    """{offset: [(seg, addr, asm, is_write, containing function)]}"""
    ords_ = trace.load_ordinals("ORDINALS.md")
    ne = idord.open_module(mod)

    # entry points give the names; function starts give the containment
    names = {}
    for e in ne.entries():
        if "offset" in e and e.get("seg"):
            nm = ords_.get("%s.%d" % (mod, e["ordinal"]))
            names[(e["seg"], e["offset"])] = nm or "%s.%d" % (mod, e["ordinal"])

    out = defaultdict(list)
    for si in range(1, 64):
        if only_seg and si != only_seg:
            continue
        try:
            sg = trace.Seg(ne, si, ords_)
        except Exception:
            continue
        if not getattr(sg, "code", None):
            continue
        try:
            rows = trace.disasm_whole(sg.code, base=0)
        except Exception:
            continue
        starts = sorted(sg.funcs)
        for off, asm in rows:
            m = READ_RE.search(asm)
            if not m:
                continue
            w = WRITE_RE.match(asm)
            mv = MOVW_RE.match(asm)
            # `mov ds:X,reg` writes; `mov reg,ds:X` reads. WRITE_RE's `mov`
            # branch only matches the PTR form, which is ambiguous, so settle it
            # by whether ds: appears before the comma.
            is_write = bool(mv) or (bool(w) and w.group(1) != "mov")
            if w and w.group(1) == "mov":
                is_write = asm.split(",")[0].find("ds:") >= 0
            addr = int(m.group(1), 16)
            i = bisect.bisect_right(starts, off) - 1
            fs = starts[i] if i >= 0 else 0
            fn = names.get((si, fs), "sub_%04x" % fs)
            out[addr].append((si, off, asm.strip(), is_write, fn))
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    mod = args[0].upper() if args else "USER"
    seg = None
    if "--seg" in sys.argv:
        seg = int(sys.argv[sys.argv.index("--seg") + 1])

    table = scan(mod, seg)

    if "--at" in sys.argv:
        want = int(sys.argv[sys.argv.index("--at") + 1], 0)
        refs = table.get(want, [])
        print("# %s ds:0x%02x — %d references" % (mod, want, len(refs)))
        for si, off, asm, isw, fn in sorted(refs):
            print("  %-5s seg%-2d:%04x  %-34s %s" %
                  ("WRITE" if isw else "read", si, off, asm, fn))
        return

    rows = []
    for addr, refs in table.items():
        writers = [r for r in refs if r[3]]
        wf = sorted(set(r[4] for r in writers))
        rows.append((len(writers), len(refs), addr, wf,
                     sorted(set(x[2].split()[0] for x in writers))))

    if "--counters" in sys.argv:
        # the clearest signal there is: something inc'd in one place and dec'd
        # in another is a count, and its two writers say a count OF WHAT.
        print("# %s — globals with matched inc/dec writers (each is a COUNT)" % mod)
        for nw, nr, addr, wf, mn in sorted(rows, key=lambda r: -r[0]):
            if "inc" in mn and "dec" in mn:
                print("  ds:0x%02x  %d writes, %d reads" % (addr, nw, nr))
                for f in wf:
                    print("            written by %s" % f)
        return

    print("# %s globals, most-written first  (%d distinct addresses)"
          % (mod, len(rows)))
    print("# a global with FEW writers is nameable; one with many is a scratch slot")
    for nw, nr, addr, wf, mn in sorted(rows, key=lambda r: (-r[0], r[2])):
        tag = "  <-- COUNTER" if ("inc" in mn and "dec" in mn) else ""
        print("  ds:0x%-4x %2d writes %3d reads   %s%s"
              % (addr, nw, nr, ", ".join(wf[:3]) or "-", tag))


if __name__ == "__main__":
    main()
