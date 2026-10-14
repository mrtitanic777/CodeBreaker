#!/usr/bin/env python3
"""build_all.py -- build every Windows module of the corpus WHOLE, and tally what came of it.

    python tools/build_all.py [--16 | --32 | --all] [--limit N] [--again] [--status]

A function judged one at a time says the function is right. A module built whole -- every
function, its data, its resources, its exports and imports, linked into one file of the
original's format -- says the module is there. For each module this runs `codebreaker build`
(build/module.py) and keeps one line in state/build_all.csv:

    linked            the module was built into a file
    exports           how many of the original's exports it has, with the same number and name
    imports extra     imports the original does not have (a sign of something foreign linked in)
    stand-ins         functions that are in the build as nothing (rejected, or never reconstructed)
    ran the same      of the exported functions called with zero arguments in the emulator, how
                      many ended and answered as the original's did

Several modules are built side by side -- the workers (base/parallel.py) shared out between
them -- and the biggest first, so that the long builds overlap rather than come one after another
at the end. Resumable: a module already in the file is skipped. With --again every module is
looked at once more, but one whose reconstruction (its header and every function's text) and
the builder's own code are what they were at its last build keeps its line without being built.
One module that fails is recorded as failed and the run goes on. --status prints the tally.
"""
import csv
import hashlib
import os
import sys
import time
from concurrent.futures import ProcessPoolExecutor, as_completed

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

FIELDS = ["module", "kind", "linked", "functions", "with_text", "stand_ins", "unresolved", "exports", "exports_same",
          "imports_extra", "format_same", "ran", "ran_same", "seconds", "error", "source"]
# the code a whole-module build is made by: a change in any of it is a reason to build again
BUILDER = ("build/module.py", "build/functions.py", "build/watcom.py", "build/data.py", "build/resources.py",
           "build/definitions.py", "decompile/c_text.py", "decompile/describe.py", "decompile/decompiler.py")


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


def _builder_digest():
    h = hashlib.sha1()
    for rel in BUILDER:
        with open(os.path.join(ROOT, "codebreaker", rel), "rb") as f:
            h.update(f.read())
    return h.hexdigest()


def source_digest(path):
    """A digest of what a whole build of the module is made from: the original file, the
    reconstruction (header and every function's text) and the builder's code."""
    from codebreaker.base import corpus, settings
    from codebreaker.verify import reconstruct
    kind = reconstruct.kind_of(path)
    m = kind.prepare(path, settings.state("build_all", os.path.basename(path).lower(), "decompiled"))
    h = hashlib.sha1((corpus.sha(path) + _builder_digest()).encode())
    h.update(m.header.encode("utf-8", "replace"))
    for name in m.order:
        h.update(b"\0" + name.encode() + b"\0" + m.texts[name].encode("utf-8", "replace"))
    return h.hexdigest()


def _one(path, previous, jobs):
    """Build one module (in a worker process). -> its csv row"""
    os.environ["CB_JOBS"] = str(jobs)                        # this build's share of the workers
    from codebreaker.base import corpus, settings
    from codebreaker.build import module
    name, t0 = os.path.basename(path).lower(), time.time()
    row = dict.fromkeys(FIELDS, "")
    row.update(module=name, kind=corpus.lane(path))
    try:
        row["source"] = source_digest(path)
    except Exception:
        row["source"] = ""
    if previous and row["source"] and previous.get("source") == row["source"]:
        return dict(previous, kept="1")                       # nothing it is made from has changed
    try:
        b = module.build(path, outdir=settings.state("build_all", name), log=lambda s: None)
        row.update(linked=int(bool(b.linked)), functions=b.functions, with_text=b.with_text,
                   stand_ins=len(b.rejected) + len(b.absent), unresolved=len(b.undefined),
                   exports=b.exports_total, exports_same=b.exports_same, imports_extra=len(b.imports_extra),
                   format_same=int(bool(b.format_same)), ran=len(b.ran), ran_same=sum(1 for r in b.ran if r[3]),
                   error=(b.error or "")[:120])
    except Exception as e:
        row.update(linked=0, error="%s: %s" % (type(e).__name__, str(e)[:100]))
    row["seconds"] = "%.0f" % (time.time() - t0)
    return {k: str(v) for k, v in row.items()}


def _write(out, rows):
    tmp = out + ".tmp"
    with open(tmp, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, FIELDS, extrasaction="ignore")
        w.writeheader()
        for r in rows:
            w.writerow({k: r.get(k, "") for k in FIELDS})
    os.replace(tmp, out)


def main(argv):
    from codebreaker.base import corpus, parallel, settings
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
    again = "--again" in argv
    by_name = {r["module"]: r for r in rows}
    paths = [p for k in kinds for p in corpus.modules(k) if again or os.path.basename(p).lower() not in by_name]
    paths.sort(key=lambda p: -os.path.getsize(p))                # biggest first: the long builds overlap
    paths = paths[:limit or None]
    workers = parallel.workers()
    side_by_side = max(1, min(3, workers // 2, len(paths)))
    jobs = max(1, workers // side_by_side)
    kept = 0
    with ProcessPoolExecutor(max_workers=side_by_side) as ex:
        futures = {ex.submit(_one, p, by_name.get(os.path.basename(p).lower()), jobs): p for p in paths}
        for i, fut in enumerate(as_completed(futures)):
            path = futures[fut]
            name = os.path.basename(path).lower()
            try:
                row = fut.result()
            except Exception as e:                           # the worker itself fell over
                row = dict.fromkeys(FIELDS, "")
                row.update(module=name, kind=corpus.lane(path), linked="0", error="worker: %s" % str(e)[:100])
            was_kept = row.pop("kept", None)
            kept += bool(was_kept)
            by_name[name] = row
            _write(out, list(by_name.values()))
            print("[%d/%d] %s: %s" % (i + 1, len(paths), name, "unchanged since its last build, kept" if was_kept else
                  "built, %s of %s exports, %s of %s ran the same" % (row["exports_same"], row["exports"], row["ran_same"],
                                                                      row["ran"]) if row["linked"] == "1" else
                  "NOT built: " + str(row["error"])), flush=True)
    if kept:
        print("%d modules unchanged since their last build: kept" % kept)
    tally(list(by_name.values()))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
