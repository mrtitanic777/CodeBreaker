#!/usr/bin/env python3
r"""
win386.py — conduct WIN386.EXE's 29 W3/LE submodules to byte-exact.

WIN386.EXE is a W3 container: an MZ loader + a W3 header/entry-table indexing 29
bundled LE modules (VMM + 28 VxDs). conduct's tier machinery skips LE and only
ever touches the FIRST submodule of a W3; this driver walks ALL of them.

For each submodule it parses the embedded LE (le.py), views each executable object
as a 32-bit section (le_seg.LeSeg), carves functions, and reconstructs each to
byte-exact via the SAME 32-bit pipeline as PE (asmrecon32 = MASM 5.10A .386/USE32,
chunked one DOSBox boot per 40 funcs). LE fixups are not masked yet, so fixup
sites fall to `db` — byte-exactness is still guaranteed (db-fallback), only %live
is lower there. Resumable per-submodule ledgers land in reports/conduct/win386.<sub>.json.

    python win386.py --list                       # the 29 submodules
    python win386.py --only WIN386 --limit 5 -j 4  # VMM smoke test
    python win386.py -j 6                          # conduct all 29
"""
import json
import os
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)

CHUNK = 40
LEDGER_DIR = os.path.join("reports", "conduct")
EMIT_ROOT = os.path.join("reconstructed", "asm")


def submodules(path):
    """[(name, le_file_offset)] from the W3 entry table."""
    d = open(path, "rb").read()
    lfa = struct.unpack_from("<I", d, 0x3C)[0]
    if d[lfa:lfa + 2] != b"W3":
        sys.exit("%s is not a W3 container" % path)
    count = struct.unpack_from("<I", d, lfa + 4)[0]
    subs = []
    for i in range(count):                          # entry table starts at lfa+0x10
        rec = d[lfa + 0x10 + i * 16: lfa + 0x10 + (i + 1) * 16]
        name = rec[0:8].rstrip(b" \x00").decode("latin1", "replace")
        off, _size = struct.unpack("<II", rec[8:16])
        subs.append((name, off))
    return d, subs


def _split_oversized(sg, start, end, max_fn=4096):
    """VxD code is hand-written asm (few 55 8B EC prologues), so the carve yields a
    few huge 'functions'. 32-bit MASM can't assemble a giant span (it exhausts and
    emits nothing -> all db), so split on instruction boundaries into <=max_fn
    pieces. Contiguous pieces cover [start,end) so the concatenation stays byte-
    exact; only cross-piece branches fall to db (a small %live cost at the seams)."""
    if end - start <= max_fn:
        return [(start, end)]
    rows = sorted(o for o, _ in sg.rows if start <= o < end)
    if not rows:                                    # pure data: fixed byte cuts
        return [(a, min(a + max_fn, end)) for a in range(start, end, max_fn)]
    out, a = [], start
    for o in rows:
        if o > a and o - a >= max_fn:
            out.append((a, o)); a = o
    out.append((a, end))
    return out


def conduct_submodule(name, le, jobs, limit):
    """Reconstruct one embedded LE submodule; return its ledger dict."""
    from codebreaker.format_loaders.le_seg import LeSeg
    import asmrecon32
    from codebreaker.foundation import parallel
    ledger = {}
    objs = le.objects()
    exe = [i for i, o in enumerate(objs, 1) if o["exec"]]
    emitdir = os.path.join(EMIT_ROOT, "win386." + name.lower())
    os.makedirs(emitdir, exist_ok=True)
    for oi in exe:
        sg = LeSeg("WIN386.EXE", obj_index=oi, le=le)
        todo = []
        for _s, f in sorted(sg.funcs.items()):
            if f["end"] > f["start"]:
                todo.extend(_split_oversized(sg, f["start"], f["end"]))
        if limit:
            todo = todo[:limit]
        if not todo:
            continue
        chunks = [todo[i:i + CHUNK] for i in range(0, len(todo), CHUNK)]

        def work(chunk, sg=sg, oi=oi):
            try:
                batch = asmrecon32.reconcile_many(sg, chunk, sg.mask)
            except Exception as e:
                return [{"seg": oi, "off": s, "size": e2 - s, "status": "error",
                         "err": repr(e)[:100], "live": 0, "total": 0, "db": 0}
                        for (s, e2) in chunk]
            out = []
            for (s, e2), (asm, res, fdb, n) in zip(chunk, batch):
                exact = bool(res and res["exact"])
                if exact and asm is not None:
                    open(os.path.join(emitdir, "obj%d_%05x.asm" % (oi, s)),
                         "w", newline="\r\n").write(asm)
                out.append({"seg": oi, "off": s, "size": e2 - s,
                            "status": "exact" if exact else "partial",
                            "live": n - len(fdb), "total": n, "db": len(fdb)})
            return out

        for cr in parallel.tmap(work, chunks, jobs=jobs,
                                label="win386 %s obj%d" % (name, oi)):
            for r in cr:
                ledger["%d:%04x" % (r["seg"], r["off"])] = r
    os.makedirs(LEDGER_DIR, exist_ok=True)
    json.dump(ledger, open(os.path.join(LEDGER_DIR, "win386." + name.lower() + ".json"), "w"))
    return ledger


def _tally(ledger):
    fn = len(ledger)
    ex = sum(1 for r in ledger.values() if r["status"] == "exact")
    live = sum(r["live"] for r in ledger.values())
    tot = sum(r["total"] for r in ledger.values())
    db = sum(r["db"] for r in ledger.values())
    err = sum(1 for r in ledger.values() if r["status"] == "error")
    return fn, ex, live, tot, db, err


def main():
    from codebreaker.foundation import parallel
    jobs = parallel.resolve_jobs(sys.argv, default=6)
    limit = None
    if "--limit" in sys.argv:
        limit = int(sys.argv[sys.argv.index("--limit") + 1])
    only = sys.argv[sys.argv.index("--only") + 1] if "--only" in sys.argv else None
    path = __import__("target").resolve("WIN386.EXE")
    d, subs = submodules(path)
    if "--list" in sys.argv:
        for nm, off in subs:
            print("  %-10s LE@0x%X" % (nm, off))
        print("%d submodules" % len(subs))
        return

    import le as le_mod
    tot_fn = tot_ex = tot_live = tot_tot = tot_db = tot_err = 0
    print("WIN386.EXE — conducting %d LE submodule(s), -j %d" %
          (1 if only else len(subs), jobs))
    for nm, off in subs:
        if only and nm.upper() != only.upper():
            continue
        # Parse the embedded LE against the WHOLE file with the header at `off`:
        # its data_pages field is a file-ABSOLUTE offset, so slicing d[off:] mis-
        # resolves the page data for later submodules (returns 0 bytes). e_lfanew=off
        # reads the header at the submodule while page reads stay file-relative.
        lf = le_mod.LEFile.from_buffer(d, e_lfanew=off)
        led = conduct_submodule(nm, lf, jobs, limit)
        fn, ex, live, tt, db, err = _tally(led)
        tot_fn += fn; tot_ex += ex; tot_live += live; tot_tot += tt; tot_db += db; tot_err += err
        print("  %-10s  %4d fn  %4d byte-exact  %5.1f%% live  %4d db  %s"
              % (nm, fn, ex, 100.0 * live / max(1, tt), db,
                 "ERR %d" % err if err else ""))
    print("=" * 60)
    print("TOTAL  %d fn  %d byte-exact (%.1f%%)  %.1f%% live  %d db  errors=%d"
          % (tot_fn, tot_ex, 100.0 * tot_ex / max(1, tot_fn),
             100.0 * tot_live / max(1, tot_tot), tot_db, tot_err))
    print("=" * 60)


if __name__ == "__main__":
    main()
