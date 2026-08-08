#!/usr/bin/env python3
"""
api_matrix.py — For every NE app/driver/font in the Windows 1.0 distribution,
enumerate which system entry points (module.ordinal) it imports, and how many
times each is referenced. Produces a per-app breakdown and a global histogram
of the most-used KERNEL/GDI/USER ordinals — a purely data-driven view of which
parts of the Windows 1.0 API actually carried the load.
"""
import os, glob, sys
from collections import Counter, defaultdict
from engine.format_loaders.ne import NEFile

def app_imports(path):
    ne = NEFile(path)
    mods = ne.module_refs()
    byord = Counter()
    byname = Counter()
    for s in ne.segments():
        for r in ne.relocations(s):
            if r["kind"] == "IMPORTORD" and 0 < r["module"] <= len(mods):
                byord[(mods[r["module"]-1], r["ordinal"])] += 1
            elif r["kind"] == "IMPORTNAME" and 0 < r["module"] <= len(mods):
                byname[(mods[r["module"]-1], ne.imported_name(r["name_off"]))] += 1
    res = ne.resident_names()
    return dict(module=res[0][0] if res else "?", mods=mods,
                byord=byord, byname=byname, path=path)

def main(directory):
    files = sorted(glob.glob(os.path.join(directory, "*")))
    apps = []
    for f in files:
        if os.path.isdir(f): continue
        try:
            with open(f, "rb") as fh:
                if fh.read(2) != b"MZ": continue
            apps.append(app_imports(f))
        except Exception:
            continue

    global_ord = Counter()
    per_module_ords = defaultdict(Counter)
    print("PER-APPLICATION SYSTEM-CALL USAGE")
    print("="*70)
    for a in apps:
        total = sum(a["byord"].values())
        if not total and not a["byname"]:
            continue
        permod = Counter()
        for (m,o),c in a["byord"].items():
            permod[m]+=1
            global_ord[(m,o)] += c
            per_module_ords[m][o] += c
        name = os.path.basename(a["path"])
        breakdown = " ".join(f"{m}:{permod[m]}" for m in a["mods"] if permod[m])
        print(f"{name:15} {a['module']:12} {total:>4} fixups   distinct-> {breakdown}")

    print("\n\nMOST-USED SYSTEM ENTRY POINTS (by total references across all apps)")
    print("="*70)
    for (m,o),c in global_ord.most_common(40):
        print(f"   {m}.{o:<4} referenced by {c} fixups")

    print("\n\nDISTINCT ORDINALS TOUCHED PER SYSTEM MODULE")
    print("="*70)
    for m in ("KERNEL","GDI","USER","SYSTEM","KEYBOARD","MOUSE","DISPLAY","SOUND","COMM"):
        if per_module_ords[m]:
            ords = sorted(per_module_ords[m])
            print(f"   {m:9} {len(ords):>3} distinct: {ords}")

if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "files/WF")
