#!/usr/bin/env python3
"""build_all.py -- build every Windows module of the corpus WHOLE, and tally what came of it.

    python tools/build_all.py [--16 | --32 | --all] [--limit N] [--status]

A function judged one at a time says the function is right. A module built whole -- every
function, its data, its resources, its exports and imports, linked into one file of the
original's format -- says the module is there. For each module this runs `codebreaker build`
(build/module.py) and adds one line to state/build_all.csv:

    linked            the module was built into a file
    exports           how many of the original's exports it has, with the same number and name
    imports extra     imports the original does not have (a sign of something foreign linked in)
    stand-ins         functions that are in the build as nothing (rejected, or never reconstructed)
    ran the same      of the exported functions called with zero arguments in the emulator, how
                      many ended and answered as the original's did

Resumable: a module already in the file is skipped. One module that fails is recorded as failed
and the run goes on. --status prints the tally and does no work.
"""
import csv
import os
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

FIELDS = ["module", "kind", "linked", "functions", "with_text", "stand_ins", "unresolved", "exports", "exports_same",
          "imports_extra", "format_same", "ran", "ran_same", "seconds", "error"]


def tally(rows, log=print):
    for kind in sorted({r["kind"] for r in rows}):
        rs = [r for r in rows if r["kind"] == kind]
        linked = [r for r in rs if r["linked"] == "1"]
        whole = [r for r in linked if r["exports"] == r["exports_same"] and r["imports_extra"] == "0"]
        clean = [r for r in whole if r["stand_ins"] == "0" and r["unresolved"] == "0"]
        ran = sum(int(r["ran"] or 0) for r in linked)
        same = sum(int(r["ran_same"] or 0) for r in linked)
        log("%s: %d modules tried; %d built into a file; %d of those with every export and no foreign import; "
            "%d of those with no stand-in at all; exported functions run: %d, %d the same as the original's" % (
                kind, len(rs), len(linked), len(whole), len(clean), ran, same))


def main(argv):
    from codebreaker.base import corpus, settings
    from codebreaker.build import module
    out = settings.state("build_all.csv")
    rows = []
    if os.path.isfile(out):
        with open(out, newline="", encoding="utf-8") as f:
            rows = list(csv.DictReader(f))
    if "--status" in argv:
        tally(rows)
        return 0
    kinds = ("pe", "ne") if "--all" in argv else ("ne",) if "--16" in argv else ("pe",)
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else 0
    done = {r["module"] for r in rows}
    paths = [p for k in kinds for p in corpus.modules(k) if os.path.basename(p).lower() not in done]
    new = not os.path.isfile(out)
    with open(out, "a", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, FIELDS)
        if new:
            w.writeheader()
        for i, path in enumerate(paths[:limit or None]):
            name, t0 = os.path.basename(path).lower(), time.time()
            row = dict.fromkeys(FIELDS, "")
            row.update(module=name, kind=corpus.lane(path))
            try:
                b = module.build(path, outdir=settings.state("build_all", name), log=lambda s: None)
                row.update(linked=int(bool(b.linked)), functions=b.functions, with_text=b.with_text,
                           stand_ins=len(b.rejected) + len(b.absent), unresolved=len(b.undefined),
                           exports=b.exports_total, exports_same=b.exports_same, imports_extra=len(b.imports_extra),
                           format_same=int(bool(b.format_same)), ran=len(b.ran), ran_same=sum(1 for r in b.ran if r[3]),
                           error=(b.error or "")[:120])
            except KeyboardInterrupt:
                raise
            except Exception as e:
                row.update(linked=0, error="%s: %s" % (type(e).__name__, str(e)[:100]))
            row["seconds"] = "%.0f" % (time.time() - t0)
            w.writerow(row)
            f.flush()
            rows.append({k: str(v) for k, v in row.items()})
            print("[%d/%d] %s: %s" % (i + 1, len(paths), name, "built, %s of %s exports, %s of %s ran the same" % (
                row["exports_same"], row["exports"], row["ran_same"], row["ran"]) if row["linked"] else
                "NOT built: " + str(row["error"])), flush=True)
    tally(rows)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
