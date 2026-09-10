#!/usr/bin/env python3
"""
genordinals.py — build ORDINALS.md from the binaries' own export tables.

Read, not inferred. Windows 1.0 shipped no export names and had to recover them
from behaviour; Windows 3.1 and 95 export names, so the ordinal->name map is
ground truth and this file is generated, not curated.

Win95 is tri-format, and the two halves carry names very differently:

  * **NE (16-bit: KRNL386/USER/GDI, the .DRV drivers, DDEML, TOOLHELP, ...)** —
    names live in the resident/non-resident name tables, exactly as in 3.1.
    An ordinal with an entry-table slot but no name is a genuine unnamed export.

  * **PE (32-bit: KERNEL32/USER32/GDI32/SHELL32/...)** — names live in the export
    directory. **Win95's KERNEL32 (and others) export largely BY ORDINAL ONLY**:
    the export address table has the function but the name-pointer table does
    not name it. That is not a bug in this tool — it is the retail reality, and
    it is why the reference tier (Geoff Chappell's ordinal history, Wine .spec
    files) exists. Ordinal-only PE exports are reported as such; their names are
    a job for cross-reference in FINDINGS.md, never invented here.

  * **LE (VxDs)** — no LE loader yet (PLAN.md Phase 3); skipped, and reported as
    skipped so the omission is visible.

Usage:
    python genordinals.py                 # regenerate ORDINALS.md (core+system)
    python genordinals.py --all           # every code-bearing module
    python genordinals.py --module USER   # dump one module to stdout
"""
import sys
from codebreaker.foundation import cores

target = cores.get('target')


def ne_map(ne):
    """{ordinal: (name, table)} for an NE module."""
    out = {}
    for name, ordn in ne.nonresident_names():
        if ordn:
            out[ordn] = (name, "non-resident")
    for name, ordn in ne.resident_names():
        if ordn:
            out[ordn] = (name, "resident")
    return out


def ne_unnamed(ne, named):
    """Ordinals with a real FIXED/MOVEABLE entry slot but no exported name."""
    out = []
    try:
        for e in ne.entries():
            o = e.get("ordinal")
            if o and o not in named and e.get("kind") in ("FIXED", "MOVEABLE"):
                out.append(o)
    except Exception:
        pass
    return sorted(out)


def emit_ne(path, base, fh):
    ne = target.open_module(path)
    m = ne_map(ne)
    unnamed = ne_unnamed(ne, m)
    fh.write(f"\n## {base}  [NE]  (module `{ne.module_name()}`)\n\n")
    fh.write(f"{len(m)} named exports")
    if unnamed:
        fh.write(f", {len(unnamed)} entry-table slots with no exported name")
    fh.write("\n\n| Ordinal | Name | Table |\n|--------:|------|-------|\n")
    for o in sorted(m):
        name, src = m[o]
        fh.write(f"| {ne.module_name()}.{o} | `{name}` | {src} |\n")
    if unnamed:
        fh.write(f"\nUnnamed entry-table ordinals ({len(unnamed)}): "
                 f"{', '.join(str(u) for u in unnamed[:60])}")
        if len(unnamed) > 60:
            fh.write(f" … (+{len(unnamed) - 60} more)")
        fh.write("\n")
    return len(m), len(unnamed)


def emit_pe(path, base, fh):
    pe = target.open_module(path)
    exp = pe.exports()   # [(ordinal, name_or_None, rva)]
    named = [(o, n) for o, n, r in exp if n]
    ordinal_only = sorted(o for o, n, r in exp if not n)
    fh.write(f"\n## {base}  [PE]  (module `{pe.module_name()}`)\n\n")
    fh.write(f"{len(exp)} exports — {len(named)} named, "
             f"{len(ordinal_only)} ordinal-only")
    if not named and ordinal_only:
        fh.write("  \n*Exports entirely by ordinal — names must come from the "
                 "reference tier (Chappell / Wine .spec), confirmed against the "
                 "binary. See FINDINGS.md.*")
    fh.write("\n\n")
    if named:
        fh.write("| Ordinal | Name |\n|--------:|------|\n")
        for o, n in sorted(named):
            fh.write(f"| {pe.module_name()}.{o} | `{n}` |\n")
    if ordinal_only:
        fh.write(f"\nOrdinal-only exports ({len(ordinal_only)}): "
                 f"{', '.join(str(o) for o in ordinal_only[:60])}")
        if len(ordinal_only) > 60:
            fh.write(f" … (+{len(ordinal_only) - 60} more)")
        fh.write("\n")
    return len(named), len(ordinal_only)


def emit(path, fh):
    base = path.replace("\\", "/").split("/")[-1]
    fmt = target.format_of(path)
    if fmt == "NE":
        return ("NE",) + emit_ne(path, base, fh)
    if fmt == "PE":
        return ("PE",) + emit_pe(path, base, fh)
    # LE / W4 / other: no loader yet
    fh.write(f"\n## {base}  [{fmt}]\n\n*No loader for {fmt} yet "
             f"(PLAN.md Phase 3) — exports not enumerated.*\n")
    return (fmt, 0, 0)


def main():
    if "--module" in sys.argv:
        emit(target.resolve(sys.argv[sys.argv.index("--module") + 1]), sys.stdout)
        return

    if "--all" in sys.argv:
        t = target.tiers()
        paths = [p for tier in target.TIER_ORDER for p in t[tier]]
    else:
        paths = [target.resolve(p) for p in target.CORE + target.SYSTEM
                 if _exists(p)]

    named_total = unnamed_total = 0
    with open("ORDINALS.md", "w", encoding="utf-8") as fh:
        fh.write("# Windows 95 (OSR2.5) — ordinal → name\n\n"
                 "**Generated by `genordinals.py` — do not hand-edit.**\n\n"
                 "Names are *read from the binaries' own export tables*, not "
                 "recovered from behaviour — ground truth, no confidence rating. "
                 "The 16-bit NE modules name their exports in the resident / "
                 "non-resident tables. Many 32-bit PE modules (notably KERNEL32) "
                 "export **by ordinal only**; those names are not in the binary "
                 "and must be supplied from the reference tier and confirmed "
                 "against the bytes — that work lives in `FINDINGS.md`, not here.\n")
        for p in paths:
            fmt, a, b = emit(p, fh)
            named_total += a
            unnamed_total += b
    print(f"ORDINALS.md: {named_total} named exports, "
          f"{unnamed_total} unnamed/ordinal-only, across {len(paths)} modules")


def _exists(name):
    import os
    try:
        return os.path.exists(target.resolve(name))
    except ValueError:
        return False


if __name__ == "__main__":
    main()
