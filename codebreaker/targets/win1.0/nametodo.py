#!/usr/bin/env python3
"""
nametodo.py — the human-reading / naming work-list ("corpus") for win1.0.

Byte-exact ASM and the readable-C `[AUTO]` baseline are done — that is MACHINE
work (every function reassembles to shipped bytes; the whole codebase lifts to C).
What remains is the deliberately-last HUMAN pass: read a byte-exact function and
promote it to understood, named source. The honest measure of that pass is the
coverage ledger's *cited* set — an address a hand-written reconstruction actually
names (reconstructed/*.c and system/*.c; the `auto/` scaffolding is EXCLUDED,
nobody has read it). So:

    corpus = carve(every byte-exact function) - cited(already human-read)

This tool joins that gap set (straight from ledger.py / appledger.py, the same
numbers the dashboard shows) with the three signals a human needs to work it:
  * size        — bytes of code to read (from the byte-exact carve)
  * callers     — in-degree from the call graph (hub functions cascade meaning)
  * evidence    — is a name already available? System-module gaps that sit on an
                  export ordinal have Microsoft's own .SYM name (99.5% recovered,
                  in ORDINALS.md) — a SEMI-AUTOMATABLE apply, not a read. App
                  internals and un-exported sysmod helpers must be read cold.

Writes:
  reports/naming_corpus.csv   one row per function still to name (the queue)
  NAMING_CORPUS.md            summary + the highest-leverage functions first

    wt --target win1.0 nametodo
"""
import glob
import os
import re
import collections

import _wtroot
import ledger
import appledger
import trace
from codebreaker.foundation import layout
from codebreaker.disasm_engine.disasm import open_ne

ROOT = _wtroot.data_root(os.path.dirname(os.path.abspath(__file__)))
CARDFILE_NOTE = "CARDFILE shipped with MS source — cross-checkable, not blind."


def _defined_names():
    """Lower-cased names of every function DEFINED in a hand reconstruction.

    A function whose export name is already defined here has been read and named
    — it is only missing the `segN:` / `MOD.ord` citation token that would let
    the ledger credit it. That is a different (near-free, honest) task than
    reading one cold, so the corpus must not lump the two together."""
    names = set()
    for f in (glob.glob(os.path.join(layout.recon_dir(ROOT), "*.c")) +
              glob.glob(os.path.join(layout.recon_sub("system", ROOT), "*.c"))):
        src = open(f, encoding="utf-8", errors="replace").read()
        for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{", src):
            names.add(m.group(1).lower())
    return names


def _indegree(globs):
    """Call-graph in-degree per offset, pooled over a module's auto-C file(s).

    The auto-C function *definitions* are placeholder-named (a known emitter
    quirk: every def prints <mod>_00000), but the call *sites* carry the real
    target offset — `sub_13c0(...)` — so in-degree by offset is recoverable and
    is what we use to rank importance. Offset is seg-agnostic here, so a hub in
    a multi-segment module is ranked by its offset's total callers (good enough
    to float the hubs to the top; not a precise per-(seg,off) count)."""
    deg = collections.Counter()
    for g in globs:
        for p in glob.glob(os.path.join(layout.generated_dir(ROOT), g)):
            txt = open(p, encoding="latin1").read()
            for m in re.finditer(r"\bsub_([0-9a-f]+)\s*\(", txt):
                deg[int(m.group(1), 16)] += 1
    return deg


def system_rows():
    """One row per uncited (un-human-read) function across the 9 system modules."""
    os.chdir(ROOT)                         # ledger.py globs are ROOT-relative
    names = trace.load_ordinals("ORDINALS.md")
    defined = _defined_names()
    tot = ledger.module_totals()
    cites = ledger.cited_addresses(tot)
    rows = []
    for m in ledger.SYSMODS:
        ne = tot[m]["ne"]
        fns = tot[m]["fns"]
        seen = cites.get(m, set())
        ent = {}                           # (seg,off) -> ordinal, from the entry table
        for e in ne.entries():
            if "offset" in e and "ordinal" in e:
                ent[(e["seg"], e["offset"])] = e["ordinal"]
        deg = _indegree(["%s_seg*.c" % m.lower()])
        offcnt = collections.Counter(off for (_, off) in fns)   # disambiguate
        for (seg, off), sz in fns.items():
            if (seg, off) in seen:
                continue                   # already human-read
            ordn = ent.get((seg, off))
            nm = names.get("%s.%d" % (m, ordn)) if ordn else None
            if nm and nm.lower() in defined:
                act = "cite existing recon"      # done, just needs the address token
            elif nm:
                act = "apply .SYM name"          # name known, not yet reconstructed
            elif ordn:
                act = "resolve ordinal"
            else:
                act = "read + coin name"
            rows.append(dict(
                tier="system", module=m, seg=seg, off=off, size=sz,
                callers=(deg.get(off, 0) if offcnt[off] == 1 else 0),
                ordinal=("%s.%d" % (m, ordn)) if ordn else "",
                evidence=nm or "", action=act))
    return rows


def app_rows():
    """One row per uncited function across the 13 bundled apps."""
    os.chdir(ROOT)
    rows = []
    for app in sorted(appledger.OWNS):
        try:
            ne, fns, code, cited = appledger.app_state(app)
        except Exception:
            continue
        deg = _indegree(["%s.c" % app.lower(),
                         "%s_*.c" % app.lower()])   # paint_core etc.
        offcnt = collections.Counter(off for (_, off) in fns)   # disambiguate
        for (seg, off), sz in fns.items():
            if (seg, off) in cited:
                continue
            rows.append(dict(
                tier="app", module=app, seg=seg, off=off, size=sz,
                callers=(deg.get(off, 0) if offcnt[off] == 1 else 0),
                ordinal="", evidence="",
                action="read + coin name"))
    return rows


def main():
    rows = system_rows() + app_rows()
    rows.sort(key=lambda r: (-r["callers"], -r["size"]))

    outdir = os.path.join(ROOT, "reports")
    os.makedirs(outdir, exist_ok=True)
    csvp = os.path.join(outdir, "naming_corpus.csv")
    with open(csvp, "w", encoding="utf-8", newline="") as f:
        f.write("tier,module,loc,size,callers,ordinal,evidence_name,action\n")
        for r in rows:
            f.write("%s,%s,seg%d:0x%04x,%d,%d,%s,%s,%s\n" % (
                r["tier"], r["module"], r["seg"], r["off"], r["size"],
                r["callers"], r["ordinal"], r["evidence"].replace(",", ";"),
                r["action"]))

    sysrows = [r for r in rows if r["tier"] == "system"]
    approws = [r for r in rows if r["tier"] == "app"]
    cite = [r for r in rows if r["action"] == "cite existing recon"]
    sym = [r for r in sysrows if r["action"] == "apply .SYM name"]
    read = [r for r in rows if r["action"].startswith("read")]
    gb = sum(r["size"] for r in rows)

    # per-module rollup
    permod = collections.OrderedDict()
    for r in rows:
        d = permod.setdefault(r["module"], dict(tier=r["tier"], n=0, b=0, sym=0))
        d["n"] += 1
        d["b"] += r["size"]
        if r["action"] == "apply .SYM name":
            d["sym"] += 1

    md = []
    md.append("# Windows 1.01 — naming corpus (the human-reading work-list)\n")
    md.append("Auto-generated by `wt --target win1.0 nametodo`. Every row is a "
              "byte-exact function that **no hand-written reconstruction cites yet** "
              "— i.e. nobody has read and named it. This is the project's sole "
              "remaining layer (fidelity is done); see `ProjectStatus.md`.\n")
    md.append("## The queue at a glance\n")
    md.append("| | functions to name | code bytes |")
    md.append("|---|---:|---:|")
    md.append("| **System modules** (9) | %d | %d |" %
              (len(sysrows), sum(r["size"] for r in sysrows)))
    md.append("| **Apps** (13) | %d | %d |" %
              (len(approws), sum(r["size"] for r in approws)))
    md.append("| **TOTAL** | **%d** | **%d** |\n" % (len(rows), gb))
    md.append("Three very different kinds of work:\n")
    md.append("- **%d — cite existing reconstruction.** A hand reconstruction "
              "*already defines this function by its export name* (e.g. "
              "`SelectObject`, `CreateWindow`); it is only missing the `segN:` / "
              "`MOD.ord` citation token, so the ledger under-counts real work. "
              "Verify the name↔address binding and add the citation. Near-free, "
              "honest credit — not new reading." % len(cite))
    md.append("- **%d — apply .SYM name.** Sits on an export ordinal whose "
              "Microsoft `.SYM` name is recovered (ORDINALS.md) but which nobody "
              "has reconstructed yet. The name is a *head start*, not a substitute "
              "for reading the body." % len(sym))
    md.append("- **%d — read cold** and name from evidence (app internals + "
              "un-exported system helpers). %s\n" % (len(read), CARDFILE_NOTE))

    md.append("> **What \"to name\" means.** A function counts as "
              "done only when a hand-written reconstruction *cites its address* "
              "(`segN:0xHHHH` or a resolved `MOD.ord`). A skeleton that references "
              "the address by a placeholder — `Entry_0131(...)`, `Sub_0x1a2c(...)` "
              "— is deliberately **not** credited: a placeholder is the absence of "
              "an identity, not one. So a few rows here (e.g. WRITE's `MmwWndProc`) "
              "already have partial scaffolds; they leave the queue once read "
              "through and cited, not before.\n")
    md.append("## By module (biggest gaps first)\n")
    md.append("| module | tier | to name | code B | .SYM-nameable |")
    md.append("|---|---|---:|---:|---:|")
    for mod, d in sorted(permod.items(), key=lambda kv: -kv[1]["b"]):
        md.append("| %s | %s | %d | %d | %s |" %
                  (mod, d["tier"], d["n"], d["b"], d["sym"] or "—"))
    md.append("")

    md.append("## Highest-leverage first — top 30 hub functions to name\n")
    md.append("Sorted by caller count: naming a hub cascades understanding to "
              "everything that calls it. `callers` is in-degree from the call graph.\n")
    md.append("| # | module | loc | size B | callers | evidence name | action |")
    md.append("|--:|---|---|---:|---:|---|---|")
    for i, r in enumerate(rows[:30], 1):
        md.append("| %d | %s | seg%d:0x%04x | %d | %d | %s | %s |" %
                  (i, r["module"], r["seg"], r["off"], r["size"], r["callers"],
                   r["evidence"] or "—", r["action"]))
    md.append("")
    md.append("## How to work it\n")
    md.append("1. **Reclaim existing work first** — the %d *cite existing recon* "
              "rows are already reconstructed by name; verify each name↔address "
              "binding (ORDINALS ordinal → entry-table address) and add the "
              "citation so the ledger credits the work that is already done.\n"
              % len(cite))
    asm_rel = os.path.relpath(layout.asm_dir(ROOT), ROOT).replace("\\", "/")
    gen_rel = os.path.relpath(layout.generated_dir(ROOT), ROOT).replace("\\", "/")
    md.append("2. **Then the hubs** — work the top of the table down. Read the "
              "byte-exact asm (`%s/<mod>/segN_0xOFF.asm`) and the machine-C "
              "(`%s/`) for the same address side by side; name it, write it into "
              "the module's hand reconstruction with a `segN:0xHHHH` citation, and "
              "it leaves the queue (the ledger counts the citation).\n"
              % (asm_rel, gen_rel))
    md.append("3. **Re-measure** — `wt --target win1.0 nametodo` regenerates this "
              "list; `python ledger.py` / `appledger.py` show the coverage move.\n")
    mdp = os.path.join(ROOT, "NAMING_CORPUS.md")
    open(mdp, "w", encoding="utf-8").write("\n".join(md))

    print("naming corpus: %d functions still to name (%d KB of code)" % (len(rows), gb // 1024))
    print("  system modules: %d   apps: %d" % (len(sysrows), len(approws)))
    print("  cite existing recon (done, needs address token): %d" % len(cite))
    print("  .SYM name recovered, not yet reconstructed:       %d" % len(sym))
    print("  must be read cold + named from evidence:          %d" % len(read))
    print("\ntop 12 highest-leverage (most callers) to name first:")
    print("  %-9s %-13s %6s %7s  %s" % ("MODULE", "loc", "size", "callers", "evidence"))
    for r in rows[:12]:
        print("  %-9s seg%d:0x%04x  %6d %7d  %s" %
              (r["module"], r["seg"], r["off"], r["size"], r["callers"],
               r["evidence"] or "-"))
    print("\n-> reports/naming_corpus.csv  (%d rows)" % len(rows))
    print("-> NAMING_CORPUS.md")


if __name__ == "__main__":
    main()
