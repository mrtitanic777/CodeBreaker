#!/usr/bin/env python3
"""win95tree.py -- assemble reconstructed DLL source into a Win95-style source tree.

The reconstruction pipeline produces per-DLL source in recovered/<dll>/; this tool
files each one into a plausible Microsoft "Chicago" source tree under WIN95/, driven
by the editable map win95_dllmap.json.

IMPORTANT (no-fabrication): the layout is a PLAUSIBLE reconstruction, not verified
Microsoft ground truth.  A DLL is placed only via a high-confidence exact_map entry or
a transparent family rule; anything else lands in WIN95/UNSORTED/ with a @REVIEW note.
Placement is recorded so classification is auditable and correctable.

Usage:
  win95tree.py init                       # create the tree skeleton (+ SCOPE/README)
  win95tree.py classify [--all]           # dry-run: show where every triaged DLL WOULD go
  win95tree.py place <dll> <srcdir>       # file recovered source into its component dir
  win95tree.py status                     # placement coverage report
"""
import json, os, re, sys, shutil

HERE = os.path.dirname(os.path.abspath(__file__))
MAP = os.path.join(HERE, "win95_dllmap.json")


def load_map():
    return json.load(open(MAP))


def tree_root(m):
    # After the repo reorg the WIN95 tree IS the repo root; env.sh exports
    # WT_WIN95_ROOT to it. Pre-reorg it sits at HERE/<root> (e.g. HERE/WIN95).
    return os.environ.get("WT_WIN95_ROOT") or os.path.join(HERE, m["root"])


def classify(dll, m):
    """Return (relpath, confidence, reason).  Never guesses beyond exact_map/rules."""
    d = dll.lower()
    if d in m["exact_map"]:
        return m["exact_map"][d], "high", "exact_map"
    for pat, comp, note in m["rules"]:
        if re.search(pat, d):
            return comp, "rule", note
    return m["default"], "none", "unclassified -> @REVIEW"


def cmd_init(m):
    root = tree_root(m)
    os.makedirs(root, exist_ok=True)
    for comp, spec in m["layout"].items():
        os.makedirs(os.path.join(root, comp), exist_ok=True)
        for ch in spec.get("children", []):
            os.makedirs(os.path.join(root, comp, ch), exist_ok=True)
    # out-of-scope scaffold with a SCOPE.md so the tree mirrors MS shape honestly
    for comp, why in m["scaffold_out_of_scope"].items():
        if comp.startswith("_"):
            continue
        p = os.path.join(root, comp)
        os.makedirs(p, exist_ok=True)
        open(os.path.join(p, "SCOPE.md"), "w").write(
            "# %s -- OUT OF RECONSTRUCTION SCOPE\n\n%s\n\n"
            "This directory is scaffold only: it mirrors the shape of Microsoft's tree, "
            "but the 16-bit-NE reconstruction pipeline never populates it.\n" % (comp, why))
    # externally-populated components (real vendored source, not pipeline output):
    # ensure the dir exists but NEVER overwrite the hand-written SCOPE.md that carries
    # its provenance -- only seed a placeholder note if the dir has no SCOPE.md yet.
    for comp, why in m.get("populated_external", {}).items():
        if comp.startswith("_"):
            continue
        p = os.path.join(root, comp)
        os.makedirs(p, exist_ok=True)
        sc = os.path.join(p, "SCOPE.md")
        if not os.path.exists(sc):
            open(sc, "w").write(
                "# %s -- populated from external source\n\n%s\n" % (comp, why))
    # TOOLS pointer + root README
    open(os.path.join(root, "TOOLS", "TOOLCHAIN.md"), "w").write(
        "# Build toolchain\n\nThe period toolchain lives at `../../../toolchain/` "
        "(version-keyed; resolve via `toolchain/toolchain.py`). Not duplicated here.\n")
    open(os.path.join(root, "README.md"), "w").write(_root_readme(m))
    print("initialized tree at", root)


def _root_readme(m):
    return (
        "# WIN95 -- reconstructed source tree\n\n"
        "A **plausible reconstruction** of the Microsoft Chicago (Windows 95) source layout, "
        "assembled by `win95tree.py` from the reconstruction pipeline's per-DLL output. It is "
        "**not** verified Microsoft ground truth -- the component names are real, the exact "
        "hierarchy is a reasoned reconstruction, and it is corrected as evidence emerges by "
        "editing `../win95_dllmap.json`.\n\n"
        "Scope: **16-bit NE user-mode DLLs only** (189 of the shipped SYSTEM modules). The "
        "ring-0 VMM/VxDs and 32-bit/VxD network stacks are scaffolded (see each `SCOPE.md`) "
        "but never populated by this pipeline. The **MS-DOS real-mode base** (`MSDOS/`) is not "
        "NE-pipeline output either, but is populated from real vendored source -- the complete "
        "buildable MS-DOS 7.10 tree (see `MSDOS/SCOPE.md`).\n\n"
        "Every placed DLL carries a `PLACEMENT.md` recording how it was classified "
        "(exact-map / family-rule) so the filing is auditable. Unclassified DLLs are filed "
        "under `UNSORTED/` with a `@REVIEW` note, never force-fit.\n")


def cmd_place(dll, srcdir, m):
    if not os.path.isdir(srcdir):
        print("source dir not found:", srcdir); return 1
    rel, conf, reason = classify(dll, m)
    base = dll.lower().rsplit(".", 1)[0]
    # leaf = the DLL's own dir; but don't double it when the mapped path already ends in it
    if rel.split("/")[-1].upper() == base.upper():
        dest = os.path.join(tree_root(m), *rel.split("/"))
    else:
        dest = os.path.join(tree_root(m), *rel.split("/"), base.upper())
    rel = os.path.relpath(dest, tree_root(m)).replace(os.sep, "/")
    os.makedirs(dest, exist_ok=True)
    copied = []
    for fn in sorted(os.listdir(srcdir)):
        sp = os.path.join(srcdir, fn)
        if os.path.isfile(sp):
            shutil.copy2(sp, os.path.join(dest, fn)); copied.append(fn)
    review = "\n> **@REVIEW**: unclassified -- verify this placement.\n" if conf == "none" else ""
    open(os.path.join(dest, "PLACEMENT.md"), "w").write(
        "# %s\n\n- component: `%s`\n- confidence: **%s** (%s)\n- source: `%s`\n- files: %s\n%s"
        % (dll, rel, conf, reason, os.path.relpath(srcdir, HERE), ", ".join(copied), review))
    print("placed %s -> %s/%s  [%s: %s]  (%d files)"
          % (dll, m["root"], rel, conf, reason, len(copied)))
    return 0


def cmd_classify(m, show_all):
    # triage_system.json is DATA (a scan of the corpus), resolved from the data root
    # (WT_DATA_ROOT / config) rather than next to this engine module.
    droot = os.environ.get("WT_DATA_ROOT", "")
    cands = [os.environ.get("WT_TRIAGE", ""),
             os.path.join(droot, "triage_system.json"),
             os.path.join(droot, "_work", "triage_system.json"),
             os.path.join(HERE, "_work", "triage_system.json")]
    tri = next((p for p in cands if p and os.path.exists(p)), "")
    if not tri:
        print("triage_system.json not found (set WT_TRIAGE or place it under WT_DATA_ROOT)"); return 1
    rows = [r for r in json.load(open(tri)) if r["format"] == "NE16"]
    buckets = {}
    for r in rows:
        rel, conf, reason = classify(r["file"], m)
        top = rel.split("/")[0]
        buckets.setdefault(top, []).append((r["file"], conf))
    print("== classification of %d NE16 DLLs (dry run) ==" % len(rows))
    for comp in sorted(buckets, key=lambda c: (-len(buckets[c]), c)):
        hi = sum(1 for _, c in buckets[comp] if c != "none")
        print("  %-10s %3d  (%d confident / %d review)"
              % (comp, len(buckets[comp]), hi, len(buckets[comp]) - hi))
    uns = buckets.get("UNSORTED", [])
    print("\nUNSORTED (%d) -- need hand review or a map entry:" % len(uns))
    if show_all:
        print("   " + " ".join(f for f, _ in uns))
    return 0


def cmd_status(m):
    root = tree_root(m)
    if not os.path.isdir(root):
        print("tree not initialized (run: win95tree.py init)"); return 1
    placed = []
    for dp, _, fns in os.walk(root):
        if "PLACEMENT.md" in fns:
            placed.append(os.path.relpath(dp, root))
    print("== placed reconstructions: %d ==" % len(placed))
    for p in sorted(placed):
        print("  " + p.replace(os.sep, "/"))
    return 0


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 2
    m = load_map()
    cmd = sys.argv[1]
    if cmd == "init":      return cmd_init(m) or 0
    if cmd == "classify":  return cmd_classify(m, "--all" in sys.argv)
    if cmd == "place":     return cmd_place(sys.argv[2], sys.argv[3], m)
    if cmd == "status":    return cmd_status(m)
    print(__doc__); return 2


if __name__ == "__main__":
    raise SystemExit(main())
