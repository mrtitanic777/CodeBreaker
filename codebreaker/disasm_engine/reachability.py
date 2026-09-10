#!/usr/bin/env python3
"""
reachability.py — which bytes in a segment are actually reachable CODE.

This is the foundation the safe form of the readability track needs. Instead of
guessing code-vs-data from "looks printable" (which misclassifies printable code
and broke byte-exact), it traces control flow: from every entry point, follow
fall-through and direct branch/call targets. Whatever is NEVER reached is not
executed — a safe, conservative signal for DATA.

Entry points (seeds): carved function starts, exported entries, and
intra-segment relocation targets (code addresses the binary itself references).

Reachable  -> real code (emit live).
Unreachable -> safe to treat as data (nothing branches into it, so labeling it
data can't drop a needed label). Worst case a jump-table-only-reached function
is conservatively called data — byte-exact still holds (db), just less live.

    python reachability.py MOUSE --seg 1
"""
import argparse
from codebreaker.foundation import cores
import re
import sys

JMPCC = {"jmp", "call", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge",
         "jb", "jbe", "ja", "jae", "jc", "jnc", "jo", "jno", "js", "jns",
         "jp", "jnp", "jpe", "jpo", "jcxz", "jecxz", "loop", "loope", "loopne",
         "loopz", "loopnz"}
# instructions with NO fall-through (control does not continue to the next byte)
NOFALL = {"jmp", "ret", "retf", "retn", "iret", "iretf", "iretd", "iretw"}


def reachable(sg):
    """Set of instruction offsets reachable from the segment's entry points."""
    rows = {o: re.sub(r"\s+", " ", a).strip() for o, a in sg.rows}
    starts = sorted(rows)
    nxt = {o: (starts[i + 1] if i + 1 < len(starts) else None)
           for i, o in enumerate(starts)}
    si = getattr(sg, "si", 0)

    seeds = set(getattr(sg, "funcs", {}) or {})
    seeds |= set(getattr(sg, "export_names", {}) or {})
    for site, tgt in (getattr(sg, "fixups", {}) or {}).items():
        m = re.match(r"seg(\d+):0x([0-9a-f]+)$", str(tgt))
        if m and int(m.group(1)) == si:
            seeds.add(int(m.group(2), 16))

    reached = set()
    wl = [s for s in seeds if s in rows]
    while wl:
        o = wl.pop()
        if o in reached or o not in rows:
            continue
        reached.add(o)
        a = rows[o]
        mn = a.split(" ")[0]
        m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", a)      # direct branch/call target
        if m and m.group(1) in JMPCC:
            t = int(m.group(2), 16)
            if t in rows:
                wl.append(t)
        if mn not in NOFALL and "(bad)" not in a:        # fall-through
            n = nxt.get(o)
            if n is not None:
                wl.append(n)
    return reached


def unreachable_rows(sg):
    """Sorted offsets of rows never reached by control flow (data candidates)."""
    reached = reachable(sg)
    return sorted(o for o, _ in sg.rows if o not in reached)


def branch_targets(sg):
    """Every offset that something ENTERS by a jump/call/entry (needs a label)."""
    tgts = set(getattr(sg, "funcs", {}) or {})
    tgts |= set(getattr(sg, "export_names", {}) or {})
    si = getattr(sg, "si", 0)
    for o, a in sg.rows:
        m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
        if m and m.group(1) in JMPCC:
            tgts.add(int(m.group(2), 16))
    for site, tgt in (getattr(sg, "fixups", {}) or {}).items():
        m = re.match(r"seg(\d+):0x([0-9a-f]+)$", str(tgt))
        if m and int(m.group(1)) == si:
            tgts.add(int(m.group(2), 16))
    return tgts


def _branches(sg):
    """[(source, target)] for every direct intra-segment branch/call."""
    out = []
    for o, a in sg.rows:
        m = re.match(r"([a-z]+) 0x([0-9a-f]+)$", re.sub(r"\s+", " ", a).strip())
        if m and m.group(1) in JMPCC:
            out.append((o, int(m.group(2), 16)))
    return out


def safe_data_regions(sg):
    """datacode regions with no EXTERNAL entry — nothing OUTSIDE the region jumps
    or falls into it, and no function-start/export/reloc-target lands in it. A
    region's own internal (misdecoded) jumps don't count, so a data blob that
    objdump littered with fake branches is still safely emitted as data. This is
    stricter than 'no target inside' where it matters (real entries) and looser
    where it doesn't (self-references) -> more data recovered, still label-safe."""
    from codebreaker.build_engine import datacode
    branches = _branches(sg)
    funcs = set(getattr(sg, "funcs", {}) or {})
    exps = set(getattr(sg, "export_names", {}) or {})
    si = getattr(sg, "si", 0)
    ext_fix = set()
    for site, tgt in (getattr(sg, "fixups", {}) or {}).items():
        m = re.match(r"seg(\d+):0x([0-9a-f]+)$", str(tgt))
        if m and int(m.group(1)) == si:
            ext_fix.add(int(m.group(2), 16))
    out = []
    for s, e, k in datacode.data_regions(sg):
        entered = (any(s <= f < e for f in funcs) or any(s <= x < e for x in exps)
                   or any(s <= t < e for t in ext_fix)
                   or any((s <= t < e) and not (s <= src < e) for src, t in branches))
        if not entered:
            out.append((s, e, k))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--seg", type=int, default=1)
    args = ap.parse_args()
    sys.path.append(__import__("os").path.dirname(__import__("os").path.abspath(__file__)))
    from codebreaker.format_loaders import ne as ne_mod
    trace = cores.get('trace')
    target = cores.get('target')
    o = trace.load_ordinals("ORDINALS.md")
    ne = ne_mod.NEFile(target.resolve(args.module))
    sg = trace.Seg(ne, args.seg, o)
    reached = reachable(sg)
    total = len(sg.rows)
    unreached = total - len(reached)
    print("%s seg%d: %d rows, %d reachable code, %d unreachable (%.1f%% -> data candidates)"
          % (args.module, args.seg, total, len(reached), unreached,
             100.0 * unreached / max(1, total)))


if __name__ == "__main__":
    main()
