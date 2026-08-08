#!/usr/bin/env python3
"""
sweep.py — replace raw `Call_MOD_ORD(...)` markers in the reconstruction with
the real API names, for every ordinal ORDINALS.md currently names.

The reconstruction deliberately leaves an unidentified ordinal as a raw
`Call_USER_137(...)` marker rather than a plausible-looking name. When an
ordinal is later PROVEN, those markers become real calls — this does that pass
in one go, so no confirmed name is left un-swept.

Two rules learned the hard way:
  * the CODE substitution requires the trailing "(" — that is what makes it
    exact, and it is why the code was never damaged by the comment bug;
  * every ordinal pattern is anchored with a non-digit lookahead. Without it
    `USER.1` matches inside `USER.101` and silently corrupts comments (it did:
    18 files, repaired).

Usage:
    python sweep.py [--dry] [--comments]
      --dry       report what would change, write nothing
      --comments  also normalise "USER.NN — unidentified (...)" annotations
"""
import glob, os, re, sys
import trace
from engine.foundation import layout


def main():
    dry = "--dry" in sys.argv
    do_comments = "--comments" in sys.argv
    ordinals = trace.load_ordinals("ORDINALS.md")

    names = {}
    for key, nm in ordinals.items():
        if "." not in key or not nm or nm.startswith("0x"):
            continue
        mod, o = key.split(".")
        if not o.isdigit() or not re.match(r"^[A-Za-z_]\w*$", nm):
            continue
        names[(mod, int(o))] = nm

    files = glob.glob(os.path.join(layout.recon_dir(), "*.c")) + glob.glob(os.path.join(layout.recon_sub("system"), "*.c"))
    total, per = 0, {}
    for f in files:
        s = open(f, encoding="utf-8", errors="replace").read()
        orig = s
        for (mod, o), nm in names.items():
            s, n = re.subn(r"\bCall_%s_%d\(" % (mod, o), nm + "(", s)
            if n:
                per[f"{mod}.{o} -> {nm}"] = per.get(f"{mod}.{o} -> {nm}", 0) + n
                total += n
            if do_comments:
                s = re.sub(r"%s\.%d(?!\d)\s*[-—]+\s*unidentified"
                           r"(?:\s*\((?:inferred[^)]*|[A-Za-z_]+\?)\))?" % (mod, o),
                           "%s.%d = %s (confirmed)" % (mod, o, nm), s)
        if s != orig and not dry:
            open(f, "w", encoding="utf-8").write(s)

    for k, v in sorted(per.items(), key=lambda x: -x[1]):
        print(f"  {v:4d}  {k}")
    print(f"{'(dry) ' if dry else ''}{total} call sites swept across {len(files)} files")

    left = 0
    for f in files:
        left += len(re.findall(r"Call_[A-Z]+_\d+", open(f, encoding="utf-8",
                                                        errors="replace").read()))
    print(f"raw markers remaining: {left}")


if __name__ == "__main__":
    main()
