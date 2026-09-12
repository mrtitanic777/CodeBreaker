#!/usr/bin/env python3
r"""ghidra_manifest.py -- index the Ghidra decompile drafts (build/ghidra/*.ghidra.c)
so the ~890-binary batch output is browsable + prioritizable the moment it lands.

Each draft's header carries the pipeline stats:
    // <ok> ok, <fail> fail
    // Ghidra pipeline: <name> | gdt_applied=<0|1> before=<n> created=<n> total=<n>

This reads them into a per-DLL manifest and an aggregate summary, and (with --by
fail|funcs|name) sorts so the messiest / biggest ones surface first for refinement.
16- vs 32-bit is inferred from which GDT the driver applied (win16 vs win32/ddk).

    python engine/ghidra/ghidra_manifest.py <dir> [--by fail|funcs|name] [--csv out.csv]
"""
import os
import re
import sys
import glob

_OK = re.compile(r"//\s*(\d+)\s+ok,\s*(\d+)\s+fail")
_PIPE = re.compile(r"gdt_applied=(\d+)\s+before=(\d+)\s+created=(\d+)\s+total=(\d+)")
_GDT = re.compile(r"gdt=(\S+)")   # only present if the batch log kept it; fallback below


def scan_one(path):
    head = ""
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for _ in range(3):
            head += f.readline()
    ok = fail = created = total = -1
    gdt_applied = 0
    m = _OK.search(head)
    if m:
        ok, fail = int(m.group(1)), int(m.group(2))
    m = _PIPE.search(head)
    if m:
        gdt_applied, _before, created, total = (int(m.group(1)), int(m.group(2)),
                                                int(m.group(3)), int(m.group(4)))
    return {"name": os.path.basename(path)[:-len(".ghidra.c")], "ok": ok, "fail": fail,
            "created": created, "total": total, "gdt_applied": gdt_applied,
            "bytes": os.path.getsize(path)}


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    d = argv[0]
    by = argv[argv.index("--by") + 1] if "--by" in argv else "funcs"
    rows = [scan_one(p) for p in glob.glob(os.path.join(d, "*.ghidra.c"))]
    if not rows:
        print("no drafts in %s" % d)
        return 1
    key = {"fail": lambda r: -r["fail"], "funcs": lambda r: -(r["ok"] or 0),
           "name": lambda r: r["name"]}.get(by, lambda r: -(r["ok"] or 0))
    rows.sort(key=key)

    tot_ok = sum(r["ok"] for r in rows if r["ok"] > 0)
    tot_fail = sum(r["fail"] for r in rows if r["fail"] > 0)
    tot_created = sum(r["created"] for r in rows if r["created"] > 0)
    with_fail = [r for r in rows if r["fail"] > 0]
    print("== Ghidra drafts: %d DLLs | %d functions decompiled, %d failed, %d cross-check-created =="
          % (len(rows), tot_ok, tot_fail, tot_created))
    print("   %d DLL(s) have >=1 failed function -- candidates for a closer look\n" % len(with_fail))
    print("  %-24s %6s %5s %7s %s" % ("dll", "funcs", "fail", "created", "gdt"))
    for r in rows[:40]:
        print("  %-24s %6d %5d %7d %s"
              % (r["name"], r["ok"], r["fail"], r["created"], "applied" if r["gdt_applied"] else "-"))
    if len(rows) > 40:
        print("  ... %d more" % (len(rows) - 40))

    if "--csv" in argv:
        out = argv[argv.index("--csv") + 1]
        with open(out, "w") as f:
            f.write("dll,funcs,fail,created,gdt_applied,bytes\n")
            for r in rows:
                f.write("%s,%d,%d,%d,%d,%d\n"
                        % (r["name"], r["ok"], r["fail"], r["created"], r["gdt_applied"], r["bytes"]))
        print("\ncsv -> %s" % out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
