#!/usr/bin/env python3
r"""
linkall.py — whole-file byte-identical relink of the ENTIRE conducted corpus, and
a coverage report (the win95 link_coverage.md analog for 3.1's NE corpus).

Per-function conduct proves each function assembles to the shipped bytes; linkfile
proves a WHOLE module rebuilds byte-for-byte. This runs linkfile across every
module we conducted (reconstructed/asm/*), fanning out across cores, saves each
byte-identical rebuild to staged/win31-all/, and writes reports/LINK_COVERAGE.md.

Concurrency is capped low by default (-j 6): each module link fires many DOSBox
assembler boots, and 12-way fan-out oversubscribes DOSBox (the same crash that hit
`conduct --tier apps -j 12`). Pass -j to override.

    python linkall.py                 # link everything, write the report
    python linkall.py -j 4
"""
import os
import sys

from codebreaker.core import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)

STAGE = os.path.join("staged", "win31-all")
REPORT = os.path.join("reports", "LINK_COVERAGE.md")


def _modules():
    """Every conducted module's asm dir name — resolved at any depth (flat or the
    component tree), so linkall works before and after `srctree --apply`."""
    return sorted(os.path.basename(d) for d in layout.asm_module_dirs())


def _link_one(name):
    import target, linkfile
    try:
        r = linkfile.link_module(name)
    except Exception as e:
        return {"name": name, "ok": False, "err": str(e)[:70], "match": False,
                "placed": 0, "n_funcs": 0, "n_diff": None}
    rec = {"name": name, "ok": True, "err": None, "match": r["match"],
           "placed": r["placed"], "n_funcs": r["n_funcs"], "n_diff": r["n_diff"],
           "all_placed": r.get("all_placed"), "first_diff": r.get("first_diff")}
    if r["match"]:
        os.makedirs(STAGE, exist_ok=True)
        fn = os.path.basename(target.resolve(name)).lower()
        open(os.path.join(STAGE, fn), "wb").write(r["rebuilt"])
    return rec


def main():
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.core import parallel
    jobs = parallel.resolve_jobs(sys.argv, default=6)
    mods = _modules()
    if not mods:
        sys.exit("no reconstructed/asm/ modules — run conduct --emit first")
    print("linkall: %d modules, -j %d" % (len(mods), jobs))
    results = parallel.tmap(_link_one, mods, jobs=jobs, label="linkall")

    ident = [r for r in results if r["match"]]
    incomplete = [r for r in results if r["ok"] and not r["match"] and not r.get("all_placed", True)]
    diffs = [r for r in results if r["ok"] and not r["match"] and r.get("all_placed", True)]
    errs = [r for r in results if not r["ok"]]

    os.makedirs(os.path.dirname(REPORT), exist_ok=True)
    with open(REPORT, "w", newline="\n") as f:
        f.write("# Windows 3.1 — whole-file link coverage\n\n")
        f.write("Every conducted module relinked whole-file via `linkfile` and byte-"
                "compared to the shipped binary. BYTE-IDENTICAL = our source rebuilds "
                "the exact shipped file (uncarved gaps are shipped-passthrough).\n\n")
        f.write("| metric | count |\n|---|---|\n")
        f.write("| modules linked | %d |\n" % len(results))
        f.write("| **BYTE-IDENTICAL** | **%d** (%.1f%%) |\n"
                % (len(ident), 100.0 * len(ident) / max(1, len(results))))
        f.write("| incomplete (assembly dropped) | %d |\n" % len(incomplete))
        f.write("| genuine diffs | %d |\n" % len(diffs))
        f.write("| errors (unresolved/other) | %d |\n\n" % len(errs))
        if diffs:
            f.write("## Genuine diffs (investigate)\n\n")
            for r in sorted(diffs, key=lambda x: x["n_diff"]):
                f.write("- `%s` — %d byte(s) differ, first @0x%X (%d/%d placed)\n"
                        % (r["name"], r["n_diff"], r["first_diff"] or 0, r["placed"], r["n_funcs"]))
            f.write("\n")
        if incomplete:
            f.write("## Incomplete (re-run; assembly dropped a chunk)\n\n")
            for r in incomplete:
                f.write("- `%s` — %d/%d placed\n" % (r["name"], r["placed"], r["n_funcs"]))
            f.write("\n")
        if errs:
            f.write("## Errors\n\n")
            for r in errs:
                f.write("- `%s` — %s\n" % (r["name"], r["err"]))
            f.write("\n")

    print("=" * 60)
    print("BYTE-IDENTICAL: %d / %d  (%.1f%%)"
          % (len(ident), len(results), 100.0 * len(ident) / max(1, len(results))))
    print("incomplete: %d   diffs: %d   errors: %d" % (len(incomplete), len(diffs), len(errs)))
    print("report: %s   rebuilds: %s" % (REPORT, STAGE))
    print("=" * 60)


if __name__ == "__main__":
    main()
