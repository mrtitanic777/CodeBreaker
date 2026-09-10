#!/usr/bin/env python3
"""
conduct.py — the conductor: drive a whole module to byte-exact, unattended.

This is the orchestrator that replaces "spawn N agents". Point it at a module
and it:
  * enumerates every carved function across the CODE segments
  * reconstructs each to byte-exact via `asmrecon` (disasm→MASM + db-fallback),
    fanning out across CPU cores
  * keeps a RESUMABLE per-function ledger (reports/conduct/<module>.json) — a
    re-run skips functions already proven byte-exact and continues the rest
  * reports aggregate faithfulness: functions byte-exact, and the "% live"
    (real instructions vs db) — plus a gap queue of anything that didn't reach
    100% live, ranked, so the residue is honest and actionable

No LLM, no agents, no tokens — deterministic local grinding. Because the
db-fallback guarantees byte-exactness, the meaningful number is % live: how much
of the module reassembles as genuine instructions.

    python conduct.py MOUSE                 # grind the whole module
    python conduct.py GDI --seg 1 --limit 50 -j 8
    python conduct.py --selftest
"""
import argparse
import json
import os
import sys
import threading
import time

import asmrecon
import asmrecon32
from codebreaker.core import layout
from codebreaker.core import parallel
from codebreaker.engines.recon import segdiff
import target
from codebreaker.disasm import trace

LEDGER_DIR = os.path.join("reports", "conduct")
CHUNK = 40                 # functions assembled per DOSBox boot (batched reconcile)


def _is_pe(module):
    try:
        return target.format_of(target.resolve(module)) == "PE"
    except Exception:
        return False


def _tier_modules(tier):
    """Conductable (NE/PE) module basenames in a tier, smallest code first so the
    quick wins bank before the long-pole modules. LE/LX (VxD) modules are skipped
    — there is no LE loader yet, so they are not reconstructable."""
    try:
        paths = target.tiers().get(tier, [])
    except Exception:
        paths = []
    out = []
    for p in paths:
        try:
            if target.format_of(p) in ("NE", "PE"):
                out.append((target.code_bytes(p), os.path.basename(p)))
        except Exception:
            continue
    return [n for _sz, n in sorted(out)]


def _segments(module):
    from codebreaker.formats import ne as ne_mod
    ne = ne_mod.NEFile(target.resolve(module))
    o = trace.load_ordinals("ORDINALS.md")
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") == "CODE":
            yield i, trace.Seg(ne, i, o)


def _count_code_segs(module):
    """How many CODE segments / sections a module has (cheap header read) — the
    denominator for the disassembly-phase progress bar."""
    try:
        if _is_pe(module):
            from codebreaker.formats.pe import open_pe
            return len(open_pe(target.resolve(module)).code_sections()) or 1
        from codebreaker.formats import ne as ne_mod
        ne = ne_mod.NEFile(target.resolve(module))
        return sum(1 for s in ne.segments() if s.get("kind") == "CODE") or 1
    except Exception:
        return 1


def _pe_sections(module):
    """The PE analog of _segments: yield (index, PeSeg) per executable section.
    Index is 1-based over code sections so the ledger key format is shared."""
    from codebreaker.formats.pe import open_pe
    from codebreaker.formats.pe_seg import PeSeg
    pe = open_pe(target.resolve(module))
    names = [name for _va, _b, name in pe.code_sections()]
    for i, name in enumerate(names, 1):
        yield i, PeSeg(module, section=name)


def _reconcile_one(sg, seg_i, start, end, seg_mask, emitdir=None):
    try:
        asm, res, fdb, n = asmrecon.reconcile(sg, start, end, seg_mask, verbose=False)
        exact = bool(res and res["exact"])
        if emitdir and exact:                  # save the byte-exact reconstruction
            try:                               # comment-only readability labels
                from codebreaker.engines.symbol import annotate
                asm = annotate.annotate(asm, sg, start, end)
            except Exception:
                pass
            open(os.path.join(emitdir, "seg%d_%04x.asm" % (seg_i, start)),
                 "w", newline="\r\n").write(asm)
        return {"seg": seg_i, "off": start, "size": end - start,
                "status": "exact" if exact else "partial",
                "live": n - len(fdb), "total": n, "db": len(fdb)}
    except Exception as e:
        return {"seg": seg_i, "off": start, "size": end - start, "status": "error",
                "err": repr(e)[:120], "live": 0, "total": 0, "db": 0}


def _reconcile_one_pe(sg, sec_i, start, end, seg_mask, emitdir=None):
    """PE (32-bit) counterpart of _reconcile_one, via asmrecon32."""
    import asmrecon32
    try:
        asm, res, fdb, n = asmrecon32.reconcile32(sg, start, end, seg_mask)
        exact = bool(res and res["exact"])
        if emitdir and exact:
            open(os.path.join(emitdir, "sec%d_%05x.asm" % (sec_i, start)),
                 "w", newline="\r\n").write(asm)
        # rva (image-base-relative) lets the ledger credit this against carve_pe's
        # ('R', rva) keys — the same convention PE citations already use.
        rva = (sg.va - sg.base) + start
        return {"seg": sec_i, "off": start, "size": end - start, "rva": rva,
                "status": "exact" if exact else "partial",
                "live": n - len(fdb), "total": n, "db": len(fdb)}
    except Exception as e:
        return {"seg": sec_i, "off": start, "size": end - start, "status": "error",
                "err": repr(e)[:120], "live": 0, "total": 0, "db": 0}


def _record(sg, seg_i, start, end, asm, res, fdb, n, is_pe, emitdir):
    """Turn a batched reconcile result into a ledger record, emitting the .asm for
    byte-exact functions (NE gets comment-only annotate labels; PE stays raw)."""
    exact = bool(res and res["exact"])
    if emitdir and exact and asm is not None:
        if is_pe:
            fname = "sec%d_%05x.asm" % (seg_i, start)
        else:
            try:
                from codebreaker.engines.symbol import annotate
                asm = annotate.annotate(asm, sg, start, end)
            except Exception:
                pass
            fname = "seg%d_%04x.asm" % (seg_i, start)
        try:
            open(os.path.join(emitdir, fname), "w", newline="\r\n").write(asm)
        except Exception:
            pass
    rec = {"seg": seg_i, "off": start, "size": end - start,
           "status": "exact" if exact else "partial",
           "live": n - len(fdb), "total": n, "db": len(fdb)}
    if is_pe:
        rec["rva"] = (sg.va - sg.base) + start
    return rec


def run(module, jobs=6, limit=None, only_seg=None, ledger_path=None, quiet=False,
        emit=False):
    from codebreaker.core import progress
    ledger_path = ledger_path or os.path.join(LEDGER_DIR, module.lower() + ".json")
    os.makedirs(os.path.dirname(ledger_path), exist_ok=True)
    ledger = json.load(open(ledger_path)) if os.path.exists(ledger_path) else {}
    emitdir = None
    if emit:
        emitdir = os.path.join(layout.asm_dir(), module.lower())
        os.makedirs(emitdir, exist_ok=True)

    is_pe = _is_pe(module)
    progress.caption("Reconstructing %s" % module.upper())
    bn = os.path.basename(module).upper()

    def _todo(seg_i, sg):
        return [(f["start"], f["end"]) for _o, f in sorted(sg.funcs.items())
                if f["end"] > f["start"]
                and ledger.get("%d:%04x" % (seg_i, f["start"]), {}).get("status") != "exact"]

    # DISASSEMBLY PHASE — the up-front disassemble+carve stall, now with a real
    # progress bar. PE drives a BYTE-level bar through the capstone sweep (built
    # ONCE and shared across sections) and each section's disassembly, so a big
    # module fills the bar smoothly instead of freezing. NE keeps a per-segment
    # bar (objdump is an opaque subprocess, and NE segments are small anyway).
    segjobs = []
    if is_pe:
        from codebreaker.formats.pe import open_pe
        from codebreaker.formats.pe_seg import PeSeg
        from codebreaker.formats.pe_trace import PeCarve
        path = target.resolve(module)
        sections = open_pe(path).code_sections()          # [(va, bytes, name)]
        secbytes = sum(len(b) for _v, b, _n in sections) or 1
        total_db = 2 * secbytes                            # sweep + per-section disasm
        progress.op("Disassembling %s" % bn, total_db)
        acc = [0]

        def _cb(done):
            progress.overall(min(acc[0] + done, total_db), total_db)

        progress.status("Disassembling: %s  (analyzing %d section(s))" % (bn, len(sections)))
        carve = PeCarve(path, progress_cb=_cb)             # bar 0..secbytes
        acc[0] = secbytes
        for i, (va, code, name) in enumerate(sections, 1):
            if only_seg and i != only_seg:
                acc[0] += len(code)
                continue
            progress.status("Disassembling: %s  (section %d/%d)" % (bn, i, len(sections)))
            sg = PeSeg(module, section=name, carve=carve, progress_cb=_cb)
            acc[0] += len(code)
            segjobs.append((i, sg, sg.mask, _todo(i, sg)))
    else:
        n_segs = _count_code_segs(module)
        progress.op("Disassembling %s" % bn, n_segs)
        carved = 0
        for seg_i, sg in _segments(module):
            carved += 1
            progress.status("Disassembling: %s  (segment %d/%d)" % (bn, seg_i, n_segs))
            progress.overall(carved, n_segs)
            if only_seg and seg_i != only_seg:
                continue
            seg_mask = segdiff.shipped_ne_segment(target.resolve(module), seg_i)[1]
            segjobs.append((seg_i, sg, seg_mask, _todo(seg_i, sg)))
    if limit:                                  # trim to `limit` functions total
        left = limit
        for i, (si, sg, sm, todo) in enumerate(segjobs):
            segjobs[i] = (si, sg, sm, todo[:max(0, left)])
            left -= len(todo)
    total = sum(len(t) for _s, _g, _m, t in segjobs)
    total_bytes = sum(e - s for _si, _g, _m, t in segjobs for (s, e) in t)
    progress.op("Reconstructing %s" % module.upper(), total)   # main bar = overall

    done = [0]
    bytes_done = [0]                           # running file-bytes reconstructed
    lock = threading.Lock()
    attempted = 0
    for seg_i, sg, seg_mask, todo in segjobs:
        if not todo or progress.cancelled():
            continue
        attempted += len(todo)
        if not quiet:
            print("  seg%d: reconstructing %d function(s)..." % (seg_i, len(todo)))

        # Batch functions into chunks: each chunk is reconstructed with ONE DOSBox
        # boot per pass (masmlift.assemble_many) instead of one boot per function,
        # the dominant cost. Chunks fan out across cores as before.
        chunks = [todo[i:i + CHUNK] for i in range(0, len(todo), CHUNK)]

        def work_chunk(chunk, seg_i=seg_i, sg=sg, seg_mask=seg_mask):
            if progress.cancelled():
                return [{"seg": seg_i, "off": s, "size": e - s, "status": "skipped",
                         "live": 0, "total": 0, "db": 0} for (s, e) in chunk]
            recon_many = asmrecon32.reconcile_many if is_pe else asmrecon.reconcile_many
            try:
                batch = recon_many(sg, chunk, seg_mask)
            except Exception as e:               # whole-chunk failure -> per-fn errors
                return [{"seg": seg_i, "off": s, "size": e2 - s, "status": "error",
                         "err": repr(e)[:120], "live": 0, "total": 0, "db": 0}
                        for (s, e2) in chunk]
            out = []
            for (start, end), (asm, res, fdb, n) in zip(chunk, batch):
                r = _record(sg, seg_i, start, end, asm, res, fdb, n, is_pe, emitdir)
                out.append(r)
                with lock:
                    done[0] += 1
                    bytes_done[0] += r.get("size", 0)
                    progress.overall(done[0], total)
                    progress.bytes_done(bytes_done[0], total_bytes)
                    if r["status"] == "error":
                        progress.error()
                    tag = "OK" if r["status"] == "exact" else r["status"]
                    progress.detail("seg%d:0x%04x  %-8s  %d db  (%dB)"
                                    % (seg_i, start, tag, r.get("db", 0), r["size"]))
            return out

        nested = parallel.tmap(work_chunk, chunks, jobs=min(jobs, 8),
                               label="conduct seg%d" % seg_i)
        for chunk_res in nested:
            for r in chunk_res:
                ledger["%d:%04x" % (r["seg"], r["off"])] = r
        json.dump(ledger, open(ledger_path, "w"), indent=1)

    return ledger, ledger_path, attempted


def analyze(ledger):
    """Distil the per-function ledger into an assessment of the whole file."""
    rows = list(ledger.values())
    exact = [r for r in rows if r["status"] == "exact"]
    partial = [r for r in rows if r["status"] == "partial"]
    errors = [r for r in rows if r["status"] == "error"]
    ti = sum(r["total"] for r in rows)
    li = sum(r["live"] for r in rows)
    db = sum(r["db"] for r in rows)
    recon_bytes = sum(r["size"] for r in exact)
    low = sorted((r for r in rows if r["total"] and r["live"] < r["total"]),
                 key=lambda r: (r["live"] - r["total"]))
    if errors:
        verdict = ("ERRORS", "%d function(s) could not be processed" % len(errors))
    elif partial:
        verdict = ("INCOMPLETE", "%d function(s) not yet byte-exact" % len(partial))
    elif not rows:
        verdict = ("EMPTY", "no functions were carved")
    else:
        verdict = ("COMPLETE", "every carved function reassembles to the shipped bytes")
    return dict(rows=rows, exact=exact, partial=partial, errors=errors, ti=ti, li=li,
                db=db, recon_bytes=recon_bytes, low=low, verdict=verdict)


def format_report(module, ledger):
    """A human-readable analysis report on the file — works headless. Says what
    happened, what's byte-exact, what has work left, and any errors."""
    a = analyze(ledger)
    try:
        fmt = target.format_of(target.resolve(module))
    except Exception:
        fmt = "?"
    live_pct = 100.0 * a["li"] / max(1, a["ti"])
    L = []
    L.append("=" * 64)
    L.append("CodeBreaker analysis report - %s" % module.upper())
    L.append("generated: %s" % time.strftime("%Y-%m-%d %H:%M:%S"))
    L.append("=" * 64)
    L.append("module format:      %s" % fmt)
    L.append("functions carved:   %d" % len(a["rows"]))
    L.append("  byte-exact:       %d" % len(a["exact"]))
    L.append("  partial:          %d" % len(a["partial"]))
    L.append("  errors:           %d" % len(a["errors"]))
    L.append("instructions:       %d total  |  %d live (%.1f%%)  |  %d db"
             % (a["ti"], a["li"], live_pct, a["db"]))
    L.append("reconstructed:      %d bytes across %d byte-exact function(s)"
             % (a["recon_bytes"], len(a["exact"])))
    L.append("")
    L.append("VERDICT: %s - %s" % a["verdict"])
    L.append("")
    if a["errors"]:
        L.append("ERRORS (need attention):")
        for r in a["errors"][:20]:
            L.append("  seg%d:0x%04x  %s" % (r["seg"], r["off"], r.get("err", "?")))
        L.append("")
    if a["low"]:
        L.append("WORK LEFT - least-live functions (heavily db'd = most MASM-")
        L.append("divergent / least understood; byte-exact but worth revisiting):")
        for r in a["low"][:15]:
            pct = 100.0 * r["live"] / max(1, r["total"])
            L.append("  seg%d:0x%04x  %d/%d live (%.0f%%)  %d db  (%dB)"
                     % (r["seg"], r["off"], r["live"], r["total"], pct, r["db"], r["size"]))
    else:
        L.append("No work left: every function is 100% live (no db fallbacks).")
    L.append("=" * 64)
    return "\n".join(L)


def report(module, ledger, save=True):
    text = format_report(module, ledger)
    print("\n" + text)
    if save:
        p = os.path.join(LEDGER_DIR, module.lower() + ".report.txt")
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "w").write(text + "\n")
        print("\nreport saved: %s" % p)
    return text


def _selftest():
    print("conductor selftest on MOUSE (limit 3)\n")
    lp = os.path.join(LEDGER_DIR, "_selftest_mouse.json")
    if os.path.exists(lp):
        os.remove(lp)
    ledger1, _p, n1 = run("MOUSE", jobs=4, limit=3, ledger_path=lp, quiet=False)
    print(format_report("MOUSE", ledger1))
    first_keys = dict(ledger1)                 # snapshot
    all_exact = all(r["status"] == "exact" for r in ledger1.values()) and len(ledger1) > 0
    # resume: a second run must NOT recompute the already-exact functions — it
    # continues with new ones. Invariant: the first run's entries are preserved.
    ledger2, _p2, n2 = run("MOUSE", jobs=4, limit=6, ledger_path=lp, quiet=True)
    preserved = all(ledger2.get(k) == v for k, v in first_keys.items())
    print("\nresume check: run1 attempted %d, run2 attempted %d, "
          "run1 entries preserved=%s" % (n1, n2, preserved))
    os.remove(lp)
    ok = all_exact and preserved and len(ledger2) >= len(ledger1)
    print("SELFTEST:", "PASS - byte-exact + resume preserves prior work"
          if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module", nargs="*")
    ap.add_argument("--tier", choices=target.TIER_ORDER,
                    help="run every conductable (NE/PE) module in a tier under "
                         "ONE progress window")
    ap.add_argument("--seg", type=int)
    ap.add_argument("--limit", type=int)
    ap.add_argument("--emit", action="store_true",
                    help="write each byte-exact reconstruction to reconstructed/asm/")
    ap.add_argument("--ui", action="store_true",
                    help="(default) show the native Win95 progress meter")
    ap.add_argument("--no-ui", action="store_true",
                    help="suppress the progress meter (CI/headless only)")
    ap.add_argument("--report", action="store_true",
                    help="regenerate the analysis report from the saved ledger (no run)")
    ap.add_argument("--selftest", action="store_true")
    jobs = parallel.resolve_jobs(sys.argv, default=6)
    args = ap.parse_args()

    if args.selftest:
        sys.exit(0 if _selftest() else 1)

    if args.report:                            # report-only: read the ledger, assess
        if not args.module:
            ap.error("give a module to report on")
        m = args.module[0]
        lp = os.path.join(LEDGER_DIR, m.lower() + ".json")
        if not os.path.exists(lp):
            sys.exit("no ledger yet for %s — run the conductor first" % m)
        report(m, json.load(open(lp)))
        return

    modules = list(args.module)
    if args.tier:
        modules = _tier_modules(args.tier)
        if not modules:
            sys.exit("no conductable (NE/PE) modules found in tier %s" % args.tier)
        print("tier %s: %d conductable module(s)" % (args.tier, len(modules)))
    if not modules:
        ap.error("give a module, --tier, or --selftest")

    from codebreaker.core import progress
    # The progress UI is ALWAYS on unless explicitly suppressed for CI: the tool
    # must never run without the user being able to watch it.
    if not args.no_ui:
        progress.enable()
    progress.manage()                          # conductor drives the meter itself

    # Run every module under ONE window: finish() (which blocks on the window)
    # is called once, at the very end — so a whole tier flows without the user
    # closing a window per module.
    totals = {"exact": 0, "rows": 0, "li": 0, "ti": 0, "partial": 0, "errors": 0}
    for i, m in enumerate(modules):
        if progress.cancelled():
            break
        # show the NEXT module (after this one) + how many modules remain
        progress.next_module(modules[i + 1].upper() if i + 1 < len(modules) else "")
        progress.modules_left(len(modules) - (i + 1))
        print("conductor: %s" % m)
        ledger, path, attempted = run(m, jobs=jobs, limit=args.limit,
                                      only_seg=args.seg, emit=args.emit)
        a = analyze(ledger)
        for k in ("li", "ti"):
            totals[k] += a[k]
        totals["exact"] += len(a["exact"]); totals["rows"] += len(a["rows"])
        totals["partial"] += len(a["partial"]); totals["errors"] += len(a["errors"])
        report(m, ledger)
        print("\nledger: %s (attempted %d this run)" % (path, attempted))
        if args.emit:
            print("emitted byte-exact .asm to reconstructed/asm/%s/" % m.lower())

    # one final outcome + summary for the whole batch
    if progress.cancelled():
        summ = "Failed - cancelled"
        progress.set_ui_state("stop")
    else:
        summ = ("Done - %d/%d byte-exact, %.0f%% live"
                % (totals["exact"], totals["rows"],
                   100.0 * totals["li"] / max(1, totals["ti"])))
        if len(modules) > 1:
            summ = "%d modules: %s" % (len(modules), summ)
        if totals["errors"]:
            progress.error(); progress.set_ui_state("stop")
        elif totals["partial"]:
            progress.warn()
        else:
            progress.set_ui_state("ok")
    progress.finish(summ, beep=True)           # window stays open until user closes it


if __name__ == "__main__":
    main()
