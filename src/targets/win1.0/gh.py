#!/usr/bin/env python3
"""
gh.py — read Ghidra's analysis from the repo, without launching Ghidra.

`ghidra_export.json` is produced by ghidra_scripts/win1_annotate.py and holds,
for every function Ghidra knows: its block, entry, size, PASCAL parameter
count, callers, callees and decompiled C. This makes that queryable in one
command, so the Ghidra round trip only happens when the image or the names
change — not every time a function needs reading.

    python gh.py NAME            the pseudocode for a function
    python gh.py --calls NAME    who calls it, and what it calls
    python gh.py --block BLOCK   every function in a segment, by size
    python gh.py --grep TEXT     functions whose pseudocode mentions TEXT
    python gh.py --stats         what the export covers

REMINDER, because it matters more than the convenience: decompiler output is a
HYPOTHESIS. Ghidra invents variable names, and on segmented 16-bit it can get
far-pointer arithmetic and some control flow wrong. Treat it exactly like the
winevdm cross-check — read it, then confirm against the bytes (`mfn.py --at`)
before anything reaches the reconstruction.
"""
import json, sys, collections

EXPORT = "ghidra_export.json"


def load():
    try:
        return json.load(open(EXPORT, encoding="utf-8"))
    except OSError:
        print(f"{EXPORT} not found — regenerate it with:\n"
              f"  python ghidra_manifest.py\n"
              f"  analyzeHeadless ./ghidra_proj Win1 -import win1_image.bin "
              f"-loader BinaryLoader -loader-baseAddr 0 "
              f"-processor \"x86:LE:16:Real Mode\" -scriptPath ./ghidra_scripts "
              f"-postScript win1_load.py $PWD -noanalysis\n"
              f"  analyzeHeadless ./ghidra_proj Win1 -process win1_image.bin "
              f"-noanalysis -scriptPath ./ghidra_scripts "
              f"-postScript win1_annotate.py $PWD")
        sys.exit(1)


def main():
    a = sys.argv[1:]
    fns = load()
    by = {f["name"]: f for f in fns}

    if not a or "--stats" in a:
        blocks = collections.Counter(f["block"] for f in fns)
        withc = sum(1 for f in fns if f.get("c"))
        named = sum(1 for f in fns if not f["name"].startswith(("FUN_", "USER_",
                                                               "GDI_", "KERNEL_")))
        print(f"{len(fns)} functions, {withc} decompiled, {named} carrying a "
              f"recovered name")
        print(f"{len(blocks)} segments; the ten largest:")
        for b, n in blocks.most_common(10):
            print(f"  {b:24s} {n:4d} functions")
        return

    if "--block" in a:
        want = a[a.index("--block") + 1]
        rows = sorted((f for f in fns if want in f["block"]),
                      key=lambda f: -f["size"])
        print(f"# {len(rows)} functions in blocks matching {want!r}")
        for f in rows:
            print(f"  {f['entry']:>12s} {f['size']:6d}B  {f['params']}p  {f['name']}")
        return

    if "--grep" in a:
        want = a[a.index("--grep") + 1].lower()
        for f in fns:
            if want in (f.get("c") or "").lower():
                print(f"  {f['name']:28s} {f['entry']:>12s} {f['block']}")
        return

    if "--calls" in a:
        nm = a[a.index("--calls") + 1]
        f = by.get(nm)
        if not f:
            print(f"{nm}: not in the export"); sys.exit(1)
        print(f"{nm}  {f['block']} @{f['entry']}  {f['size']}B  "
              f"{f['params']} args")
        print("  calls  : " + (", ".join(f["calls"]) or "(nothing)"))
        print("  callers: " + (", ".join(f["callers"]) or "(none recorded)"))
        return

    nm = a[0]
    f = by.get(nm)
    if not f:
        near = [k for k in by if nm.lower() in k.lower()][:8]
        print(f"{nm}: not in the export" + (f"; did you mean {near}?" if near else ""))
        sys.exit(1)
    print(f"/* {f['block']} @{f['entry']}  {f['size']} bytes  "
          f"{f['params']} PASCAL args */")
    print(f.get("c") or "/* not decompiled */")


if __name__ == "__main__":
    main()
