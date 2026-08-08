#!/usr/bin/env python3
"""
names.py — the per-module naming oracle: what we can call each function, and how
sure we are.

Merges the two naming sources into one view, keyed by how much they can be
trusted (CONTEXT.md ground-truth hierarchy):

  EXPORT   ordinal -> name read straight from THIS binary's own NE export
           tables (resident + non-resident). Ground truth — the name Microsoft
           shipped, at an address the binary itself declares.
  [midage] internal-symbol name from the Midageproject .SYM data
           (reference/midage). HYPOTHESIS ONLY, and for the 16-bit/32-bit core
           it is Win98/ME data (offsets do NOT map to OSR2.5) — a *lead* for an
           unnamed internal, to confirm by segment/call-graph shape, never by
           offset.

So a reconstruction can look up "what is this export called" (authoritative) and
"does the leaked symbol set suggest a name for this internal" (hypothesis) in
one place, with the confidence tagged.

Usage:
    python names.py USER.EXE                 # export names + midage availability
    python names.py MMSYSTEM.DLL --midage    # also dump midage internal-name leads
"""
import argparse
from engine.foundation import cores
import os
import sys

target = cores.get('target')
from engine.foundation import layout


def export_names(mod):
    """{ordinal: (name, table)} from the binary itself. NE only."""
    ne = target.open_module(mod)
    out = {}
    if hasattr(ne, "nonresident_names"):
        for n, o in ne.nonresident_names():
            if o:
                out[o] = (n, "non-resident")
        for n, o in ne.resident_names():
            if o:
                out[o] = (n, "resident")
    elif hasattr(ne, "exports"):          # PE
        for o, n, _rva in ne.exports():
            if n:
                out[o] = (n, "pe-export")
    return ne, out


def midage_path(mod):
    base = os.path.basename(mod).lower()
    p = os.path.join(layout.reference_sub("midage"), "db", "components", base + ".yaml")
    return p if os.path.exists(p) else None


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--midage", action="store_true",
                    help="also print midage internal-name leads (hypothesis)")
    args = ap.parse_args()

    try:
        ne, exp = export_names(args.module)
    except Exception as e:
        sys.exit(f"cannot open {args.module}: {e}")

    modname = ne.module_name()
    fmt = target.format_of(target.resolve(args.module))
    print(f"# {os.path.basename(args.module)}  [{fmt}]  module `{modname}`")
    print(f"# {len(exp)} EXPORT names (ground truth, from this binary)")
    for o in sorted(exp):
        nm, tbl = exp[o]
        print(f"  {modname}.{o:<5} {nm:<28} ({tbl})")

    mp = midage_path(args.module)
    if not mp:
        print(f"\n# midage: no entry for this module — internals stay AMBIGUOUS "
              f"until named from the disassembly.")
        return
    print(f"\n# midage: {mp} present (HYPOTHESIS — see version caveat)")
    if args.midage:
        from engine.symbol_engine import midage
        data = midage.parse(mp)
        ver = midage.pick_version(data, None)
        print(f"# --- midage internal-name leads [{ver}] "
              f"(offsets are {ver}, NOT OSR2.5 — confirm by shape) ---")
        n = 0
        for s in data[ver]["segments"]:
            for nm, off in s["symbols"]:
                # skip the ones that are just the exports we already have
                if nm.upper() in {v[0].upper() for v in exp.values()}:
                    continue
                oo = f"0x{off:08x}" if off is not None else "?"
                print(f"  [midage] {s['name']:<12} {oo:>10}  {nm}")
                n += 1
                if n >= 400:
                    print("  … (truncated at 400)"); return


if __name__ == "__main__":
    main()
