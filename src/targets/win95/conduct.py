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

from src.engines.recon import asmrecon
import asmrecon32
from src.core import parallel
import segdiff
import target
from src.disasm import trace
from src.core import layout

LEDGER_DIR = os.path.join(layout.runtime_dir(), "conduct")
CHUNK = 40                 # functions assembled per DOSBox boot (batched reconcile)


def _is_pe(module):
    try:
        return target.format_of(target.resolve(module)) == "PE"
    except Exception:
        return False


_LE_CACHE = {}


def _le_open(module):
    """The LEFile for `module` -- for a W3 container (VMM32.VXD) this is the first
    bundled LE module (parsed MZ-less via from_buffer); otherwise the standard LE."""
    if module in _LE_CACHE:
        return _LE_CACHE[module]
    import struct, le
    path = target.resolve(module)
    d = open(path, "rb").read()
    lf = None
    if d[:2] == b"MZ":
        lfa = struct.unpack_from("<I", d, 0x3C)[0]
        if d[lfa:lfa + 2] == b"W3":
            off = struct.unpack_from("<I", d, lfa + 0x10 + 8)[0]   # first entry offset
            lf = le.LEFile.from_buffer(d[off:])
    if lf is None:
        lf = le.LEFile(path)
    _LE_CACHE[module] = lf
    return lf


def _is_w3(module):
    try:
        import struct
        d = open(target.resolve(module), "rb").read()
        return d[:2] == b"MZ" and d[struct.unpack_from("<I", d, 0x3C)[0]:][:2] == b"W3"
    except Exception:
        return False


def _fmt(module):
    """'NE' | 'PE' | 'LE' | ... — the module's executable format (W3 -> LE inner)."""
    try:
        f = target.format_of(target.resolve(module))
    except Exception:
        return None
    if f in ("NE", "PE", "LE"):
        return f
    return "LE" if _is_w3(module) else f


def _module_done(module):
    """True if a prior run already drove this module to completion — its saved
    report says every carved function is byte-exact (or it has no code). Lets a
    tier RESUME skip finished modules WITHOUT re-disassembling them."""
    p = os.path.join(LEDGER_DIR, os.path.basename(module).lower() + ".report.txt")
    try:
        with open(p) as f:
            t = f.read()
    except Exception:
        return False
    return "VERDICT: COMPLETE" in t or "VERDICT: EMPTY" in t


def _tier_modules(tier):
    """Conductable module basenames in a tier, smallest code first so the quick wins bank
    before the long-pole modules. Covers NE, PE AND LE/LX (VxD): LE code is 32-bit flat, so
    an object's bytes feed the same reconcile path (le.py carves them, and linkfile verifies
    the whole VxD byte-exact — 223 LE modules are banked this way alongside 338 NE / 634 PE)."""
    try:
        paths = target.tiers().get(tier, [])
    except Exception:
        paths = []
    out = []
    for p in paths:
        try:
            if target.format_of(p) in ("NE", "PE", "LE"):
                out.append((target.code_bytes(p), os.path.basename(p)))
        except Exception:
            continue
    return [n for _sz, n in sorted(out)]


def _segments(module):
    from src.formats import ne as ne_mod
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
            from src.formats.pe import open_pe
            return len(open_pe(target.resolve(module)).code_sections()) or 1
        from src.formats import ne as ne_mod
        ne = ne_mod.NEFile(target.resolve(module))
        return sum(1 for s in ne.segments() if s.get("kind") == "CODE") or 1
    except Exception:
        return 1


def _pe_sections(module):
    """The PE analog of _segments: yield (index, PeSeg) per executable section.
    Index is 1-based over code sections so the ledger key format is shared."""
    from src.formats.pe import open_pe
    from src.formats.pe_seg import PeSeg
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
                from src.engines.symbol import annotate
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


def recover_c_pe(module, emitdir=None, limit=None):
    """C-inversion companion to the ASM reconcile (Track B): recover byte-exact C for the PE
    module's leaf functions via the MSVC-2.0 model inverse (oracle_router.recover32), writing a
    sec<i>_<off>.c beside each sec<i>_<off>.asm.  Returns (recovered, sampled).  Every emitted C
    is byte-exact by construction (self-verified through msvc2model.emit); the rest are declined."""
    import capstone
    from src.formats.pe_trace import PeCarve
    from src.engines.inverse import oracle_router, msvc2cov
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    carve = PeCarve(module)
    secbase = {va: i for i, (va, _d, _n) in enumerate(carve.sections, 1)}
    rec = tot = 0
    for s, f in sorted(carve.funcs.items()):
        b = msvc2cov._leaf_bytes(md, carve, s, f["end"])
        if not b or len(b) < 3:
            continue
        tot += 1
        r = oracle_router.recover32(b)
        if r.get("exact"):
            rec += 1
            if emitdir:
                base, _ = carve._sec_of(s)
                os.makedirs(emitdir, exist_ok=True)
                open(os.path.join(emitdir, "sec%d_%05x.c" % (secbase.get(base, 0), s - base)),
                     "w", newline="\r\n").write(
                         "/* %s  (msvc2 byte-exact) */\n%s\n" % (f.get("name") or "sub_%x" % s, r["c"]))
        if limit and tot >= limit:
            break
    return rec, tot


def _record(sg, seg_i, start, end, asm, res, fdb, n, fmt, emitdir):
    """Turn a batched reconcile result into a ledger record, emitting the .asm for
    byte-exact functions (NE gets comment-only annotate labels; PE/LE stay raw)."""
    exact = bool(res and res["exact"])
    if emitdir and exact and asm is not None:
        if fmt == "PE":
            fname = "sec%d_%05x.asm" % (seg_i, start)
        elif fmt == "LE":
            fname = "obj%d_%05x.asm" % (seg_i, start)
        else:                                    # NE: comment-only readability labels
            try:
                from src.engines.symbol import annotate
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
    if fmt == "PE":                              # rva -> ledger credits vs carve_pe
        rec["rva"] = (sg.va - sg.base) + start
    return rec


def _disasm_segjobs(module, only_seg=None):
    """Disassemble+carve a module -> (fmt, [(seg_i, sg, mask)]) with REAL sg,
    driving the disassembly bar. Split out of run() so a tier can disassemble the
    WHOLE queue first (phase 1) and reconstruct it after (phase 2). The
    reconstruction-relevant slice of each sg is picklable (see _sg_dict), so
    phase 1 can cache it to disk instead of holding every carve in RAM."""
    import progress
    fmt = _fmt(module)                          # NE | PE | LE
    is_pe = fmt == "PE"
    bn = os.path.basename(module).upper()
    raw = []
    if is_pe:
        from src.formats.pe import open_pe
        from src.formats.pe_seg import PeSeg
        from src.formats.pe_trace import PeCarve
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
            raw.append((i, sg, sg.mask))
    elif fmt == "LE":
        from le_seg import LeSeg
        _le = _le_open(module)                          # inner LE for W3 containers
        objs = _le.objects()
        exe = [(i, ob) for i, ob in enumerate(objs, 1) if ob["exec"]]
        total_db = sum(ob["vsize"] for _i, ob in exe) or 1
        progress.op("Disassembling %s" % bn, total_db)
        acc = 0
        for i, ob in exe:
            if only_seg and i != only_seg:
                continue
            progress.status("Disassembling: %s  (object %d/%d)" % (bn, i, len(objs)))
            sg = LeSeg(module, obj_index=i, le=_le)
            acc += ob["vsize"]
            progress.overall(min(acc, total_db), total_db)
            raw.append((i, sg, sg.mask))
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
            mask = segdiff.shipped_ne_segment(target.resolve(module), seg_i)[1]
            raw.append((seg_i, sg, mask))
    return fmt, raw


def _reconstruct_core(module, fmt, raw, ledger, ledger_path, emitdir, jobs,
                      limit, quiet):
    """Reconstruct from disasm jobs [(seg_i, sg, mask)]: compute todo from the
    ledger, run the chunked byte-exact reconcile (one DOSBox boot per chunk), and
    write the ledger. sg may be a real Seg or a cached _LiteSeg — only the bounded
    attribute set (code/rows/funcs/mask/name/va/base/export_names/fixups) is used."""
    import progress
    is32 = fmt in ("PE", "LE")

    # A carve artifact can hand us a single "function" spanning a whole code
    # segment (e.g. DSKAPI16 seg6: 57 KB / 20831 rows). 16-bit MASM 4.00 cannot
    # assemble a module that large -- it exhausts and emits nothing, leaving the
    # function forever "partial". Split an oversized function into row-aligned
    # pieces (no instruction is cut) that each assemble; contiguous pieces cover
    # [start,end) so the concatenation stays byte-exact. Cross-piece branches fall
    # to db (byte-exact), a small readability cost only at the seams.
    max_fn = max(512, int(os.environ.get("WT_MAX_FN_BYTES", "4096")))

    def _split_oversized(sg, start, end):
        if end - start <= max_fn:
            return [(start, end)]
        rows = sorted(o for o, _ in sg.rows if start <= o < end)
        if not rows:                               # pure data: fixed byte cuts
            return [(a, min(a + max_fn, end)) for a in range(start, end, max_fn)]
        out, a = [], start
        for o in rows:
            if o > a and o - a >= max_fn:
                out.append((a, o)); a = o
        out.append((a, end))
        return out

    def _todo(seg_i, sg):
        out = []
        for _o, f in sorted(sg.funcs.items()):
            if f["end"] <= f["start"]:
                continue
            for a, b in _split_oversized(sg, f["start"], f["end"]):
                if ledger.get("%d:%04x" % (seg_i, a), {}).get("status") != "exact":
                    out.append((a, b))
        return out

    segjobs = [(seg_i, sg, mask, _todo(seg_i, sg)) for seg_i, sg, mask in raw]
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
            recon_many = asmrecon32.reconcile_many if is32 else asmrecon.reconcile_many
            # Retry transient failures. Under many warm workers, a temp file can be
            # momentarily locked (DOSBox still holds it -> PermissionError) or swept
            # by another thread (FileNotFoundError); a single such error must NOT
            # kill all CHUNK functions. recon_many records nothing until it returns,
            # so re-running is clean. Only a persistent error falls through to errs.
            batch = last_exc = None
            for attempt in range(4):
                if progress.cancelled():
                    break
                try:
                    batch = recon_many(sg, chunk, seg_mask)
                    break
                except (PermissionError, FileNotFoundError, OSError) as e:
                    last_exc = e
                    time.sleep(0.4 * (attempt + 1))    # let the lock/dir settle
                except Exception as e:                 # non-transient: don't spin
                    last_exc = e
                    break
            if batch is None:                    # whole-chunk failure -> per-fn errors
                return [{"seg": seg_i, "off": s, "size": e2 - s, "status": "error",
                         "err": repr(last_exc)[:120], "live": 0, "total": 0, "db": 0}
                        for (s, e2) in chunk]
            out = []
            for (start, end), (asm, res, fdb, n) in zip(chunk, batch):
                r = _record(sg, seg_i, start, end, asm, res, fdb, n, fmt, emitdir)
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

        # Cap the fan-out at the warm-pool size: more threads than workers just
        # block on lease(). masmlift.POOL_MAX is env-tunable (WT_POOL_MAX), so the
        # launcher controls how many DOSBoxes run at once (e.g. 10 on a 12-core box).
        from src.engines.recon import masmlift
        nested = parallel.tmap(work_chunk, chunks, jobs=min(jobs, masmlift.POOL_MAX),
                               label="conduct seg%d" % seg_i)
        for chunk_res in nested:
            for r in chunk_res:
                ledger["%d:%04x" % (r["seg"], r["off"])] = r
        json.dump(ledger, open(ledger_path, "w"), indent=1)

    return ledger, ledger_path, attempted


# -- two-phase (disassemble the whole queue, then reconstruct it) disk cache ----
DISASM_CACHE = os.path.join(layout.runtime_dir(), "disasm_cache")


class _LiteSeg:
    """Picklable reconstruction-only view of a segment — exactly the attributes
    the reconcile / annotate / reachability path reads. Lets phase 1 cache the
    disassembly to disk and phase 2 reconstruct it without the heavy carve
    (capstone maps, file bytes) resident in RAM for the whole queue."""
    __slots__ = ("code", "rows", "funcs", "mask", "name", "va", "base",
                 "export_names", "fixups", "si")

    def __init__(self, d):
        self.code = d["code"]; self.rows = d["rows"]; self.funcs = d["funcs"]
        self.mask = set(d["mask"]); self.name = d["name"]
        self.va = d["va"]; self.base = d["base"]
        self.export_names = d["export_names"]; self.fixups = d["fixups"]
        self.si = d["si"]


def _sg_dict(sg, seg_i, mask):
    return {"seg_i": seg_i, "code": bytes(sg.code), "rows": list(sg.rows),
            "funcs": {s: dict(f) for s, f in sg.funcs.items()},
            "mask": sorted(mask or ()), "name": getattr(sg, "name", "seg%d" % seg_i),
            "va": getattr(sg, "va", 0), "base": getattr(sg, "base", 0),
            "export_names": dict(getattr(sg, "export_names", {}) or {}),
            "fixups": dict(getattr(sg, "fixups", {}) or {}),
            "si": getattr(sg, "si", seg_i)}


def _cache_path(module):
    return os.path.join(DISASM_CACHE, os.path.basename(module).lower() + ".pkl")


def _cache_disasm(module, only_seg=None):
    """Phase 1: disassemble `module` and cache the lite result to disk."""
    import pickle
    fmt, raw = _disasm_segjobs(module, only_seg)
    os.makedirs(DISASM_CACHE, exist_ok=True)
    data = {"fmt": fmt, "segs": [_sg_dict(sg, seg_i, mask) for seg_i, sg, mask in raw]}
    with open(_cache_path(module), "wb") as f:
        pickle.dump(data, f)
    return sum(len(sd["funcs"]) for sd in data["segs"])


def _load_disasm(module):
    """Phase 2: load cached disasm as (fmt, [(seg_i, _LiteSeg, mask)])."""
    import pickle
    with open(_cache_path(module), "rb") as f:
        data = pickle.load(f)
    raw = [(sd["seg_i"], _LiteSeg(sd), set(sd["mask"])) for sd in data["segs"]]
    return data["fmt"], raw


def run(module, jobs=6, limit=None, only_seg=None, ledger_path=None, quiet=False,
        emit=False):
    """Reconstruct one module end-to-end: disassemble, then reconstruct."""
    import progress
    ledger_path = ledger_path or os.path.join(LEDGER_DIR, module.lower() + ".json")
    os.makedirs(os.path.dirname(ledger_path), exist_ok=True)
    ledger = json.load(open(ledger_path)) if os.path.exists(ledger_path) else {}
    emitdir = None
    if emit:
        emitdir = layout.asm_module_out_dir(module)      # component dir when tree active
    progress.caption("Reconstructing %s" % module.upper())
    fmt, raw = _disasm_segjobs(module, only_seg)
    return _reconstruct_core(module, fmt, raw, ledger, ledger_path, emitdir,
                             jobs, limit, quiet)


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
    try:                                       # also show it in the UI Report tab
        import progress
        progress.report(text)
    except Exception:
        pass
    if save:
        p = os.path.join(LEDGER_DIR, module.lower() + ".report.txt")
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "w").write(text + "\n")
        print("\nreport saved: %s" % p)
    return text


def _feed_coverage(i, modules, totals, args):
    """Push the running run-wide picture to the UI Coverage tab + status bar."""
    import progress
    try:
        done_n = i + 1
        exactp = 100.0 * totals["exact"] / max(1, totals["rows"])
        livep = 100.0 * totals["li"] / max(1, totals["ti"])
        tiername = args.tier or "run"
        linkline = ("\nlinked identical: %d / %d modules" % (
            totals["link_ok"], totals["link_total"])) if args.verify_link else ""
        progress.coverage(
            "this run:  %s\n%s\n"
            "modules done:    %d / %d\n"
            "functions:       %d exact / %d carved   (%.3f%% byte-exact)\n"
            "readability:     %d / %d live   (%.1f%%)\n"
            "partial:         %d\nerrors:          %d%s"
            % (tiername, "-" * 52, done_n, len(modules),
               totals["exact"], totals["rows"], exactp,
               totals["li"], totals["ti"], livep,
               totals["partial"], totals["errors"], linkline))
        progress.status_bar(
            "%s - %d / %d done   |   %.3f%% byte-exact   |   %d errors"
            % (tiername, done_n, len(modules), exactp, totals["errors"]))
    except Exception:
        pass


def _verify_link(module, totals):
    """Whole-file LINK a fully byte-exact module and confirm the rebuilt file is
    byte-identical to the shipped binary. Updates `totals` and logs to the UI +
    reports/link_verify.jsonl. A diff here (should not happen for a fully-exact
    module) is surfaced loudly rather than silently passed."""
    import progress
    try:
        import linkfile
        r = linkfile.link_module(module)
    except Exception as e:
        progress.detail("LINK ERR  %-16s %s" % (os.path.basename(module).upper(), repr(e)[:40]))
        print("  link-verify error on %s: %r" % (module, e))
        return
    totals["link_total"] += 1
    ok = bool(r.get("match"))
    totals["link_ok"] += ok
    bn = os.path.basename(module).upper()
    if ok:
        progress.detail("LINKED    %-16s byte-identical  (%d fn, %d B)"
                        % (bn, r.get("n_funcs", 0), r.get("size", 0)))
    else:
        progress.detail("LINK DIFF %-16s %d byte(s) differ @0x%X"
                        % (bn, r.get("n_diff", -1), r.get("first_diff") or 0))
    try:
        rec = {"module": os.path.basename(module), "match": ok,
               "n_diff": r.get("n_diff"), "n_funcs": r.get("n_funcs"),
               "size": r.get("size"), "fmt": r.get("fmt")}
        p = os.path.join(LEDGER_DIR, "..", "link_verify.jsonl")
        with open(os.path.normpath(p), "a", encoding="utf-8") as f:
            f.write(json.dumps(rec) + "\n")
    except Exception:
        pass


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
    try:                                       # wire DOSBox/MASM tool paths (idempotent;
        from src.core import config
        config.apply_config()                 # conduct.py directly must work too)
    except Exception:
        pass
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
    ap.add_argument("--verify-link", action="store_true",
                    help="after each fully byte-exact module, whole-file LINK it and "
                         "verify the rebuilt file is byte-identical to the shipped "
                         "binary (records to reports/link_verify.jsonl, shows a "
                         "running linked-count in the UI)")
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

    # RESUME: for a batch/tier, drop modules a prior run already finished, so it
    # picks up where it left off instead of re-disassembling everything. With
    # --verify-link we KEEP the finished ones (verify-only, no re-reconstruction) so
    # a completed tier can still be proven to link byte-identical, module by module.
    done_set = set(m for m in modules if _module_done(m))
    if len(modules) > 1 and not args.verify_link:
        full = len(modules)
        modules = [m for m in modules if m not in done_set]
        if full != len(modules):
            print("resume: %d of %d modules already complete; %d left to do"
                  % (full - len(modules), full, len(modules)))
        if not modules:
            print("nothing to do — every module is already byte-exact.")
            return

    import progress
    # The progress UI is ALWAYS on unless explicitly suppressed for CI: the tool
    # must never run without the user being able to watch it.
    if not args.no_ui:
        progress.enable()
    progress.manage()                          # conductor drives the meter itself

    def _nav(i, verb):
        progress.caption("%s %s" % (verb, os.path.basename(modules[i]).upper()))
        progress.next_module(modules[i + 1].upper() if i + 1 < len(modules) else "")
        progress.modules_left(len(modules) - (i + 1))

    two_phase = len(modules) > 1
    nfuncs = {}                                # carved-function count per module,
    #                                            for a FUNCTION-based time-left ETA
    #                                            (code bytes over-weight the giant
    #                                            data-heavy DLLs and inflate it wildly)

    # ===== PHASE 1 — disassemble the WHOLE queue first (clean two-phase flow:
    # all disassembly up front, then all reconstruction, not interleaved) =====
    if two_phase:
        for i, m in enumerate(modules):
            if progress.cancelled():
                break
            if m in done_set:                      # verify-only: no disasm needed
                continue
            _nav(i, "Disassembling")
            if os.path.exists(_cache_path(m)):     # resume: already disassembled
                try:
                    _f, _raw = _load_disasm(m)
                    nfuncs[m] = sum(len(sg.funcs) for _si, sg, _mk in _raw)
                except Exception:
                    pass
                progress.detail("disasm  %-16s (cached)" % os.path.basename(m).upper())
                continue
            print("disasm: %s" % m)
            try:
                nf = _cache_disasm(m, only_seg=args.seg)
                nfuncs[m] = nf
                progress.detail("disasm  %-16s %d functions carved"
                                % (os.path.basename(m).upper(), nf))
            except Exception as e:
                print("  disasm failed: %r" % e)
                progress.detail("disasm  %-16s FAILED" % os.path.basename(m).upper())

    # ===== PHASE 2 — reconstruct the whole queue (from the phase-1 cache) =====
    if two_phase and not progress.cancelled():
        progress.detail("---- disassembly done; reconstructing %d module(s) ----"
                        % len(modules))
    remaining_funcs = sum(nfuncs.get(m, 0) for m in modules)
    worked_funcs = 0
    sess_t0 = time.time()
    totals = {"exact": 0, "rows": 0, "li": 0, "ti": 0, "partial": 0, "errors": 0,
              "link_ok": 0, "link_total": 0}
    for i, m in enumerate(modules):
        if progress.cancelled():
            break
        # verify-only: module was already byte-exact on entry, so skip all
        # reconstruction and just whole-file LINK it to prove it rebuilds identical.
        if args.verify_link and m in done_set:
            _nav(i, "Verifying")
            lp = os.path.join(LEDGER_DIR, os.path.basename(m).lower() + ".json")
            try:
                led = json.load(open(lp)) if os.path.exists(lp) else {}
            except Exception:
                led = {}
            a = analyze(led)
            for k in ("li", "ti"):
                totals[k] += a[k]
            totals["exact"] += len(a["exact"]); totals["rows"] += len(a["rows"])
            totals["partial"] += len(a["partial"]); totals["errors"] += len(a["errors"])
            _verify_link(m, totals)
            _feed_coverage(i, modules, totals, args)
            continue
        _nav(i, "Reconstructing")
        # time-left: remaining FUNCTIONS / measured functions-per-second (reconstruct
        # time tracks function count, not code bytes — the giant data-heavy DLLs
        # have huge byte size but reconstruct in proportion to their function count).
        elapsed = time.time() - sess_t0
        if worked_funcs > 0 and elapsed > 0 and two_phase:
            progress.session_eta(remaining_funcs / (worked_funcs / elapsed))
        print("conductor: %s" % m)
        attempted = 0
        try:
            if two_phase:
                ledger_path = os.path.join(LEDGER_DIR, os.path.basename(m).lower() + ".json")
                os.makedirs(os.path.dirname(ledger_path), exist_ok=True)
                ledger = json.load(open(ledger_path)) if os.path.exists(ledger_path) else {}
                emitdir = None
                if args.emit:
                    emitdir = layout.asm_module_out_dir(os.path.basename(m))
                try:
                    fmt, raw = _load_disasm(m)         # from phase 1
                except Exception:
                    fmt, raw = _disasm_segjobs(m, args.seg)    # cache missing: fall back
                ledger, path, attempted = _reconstruct_core(
                    m, fmt, raw, ledger, ledger_path, emitdir, jobs, args.limit, quiet=True)
                try:
                    os.remove(_cache_path(m))          # free the disasm cache once used
                except OSError:
                    pass
            else:
                ledger, path, attempted = run(m, jobs=jobs, limit=args.limit,
                                              only_seg=args.seg, emit=args.emit)
            if attempted > 0:
                worked_funcs += nfuncs.get(m, 0)
            a = analyze(ledger)
            for k in ("li", "ti"):
                totals[k] += a[k]
            totals["exact"] += len(a["exact"]); totals["rows"] += len(a["rows"])
            totals["partial"] += len(a["partial"]); totals["errors"] += len(a["errors"])
            report(m, ledger)
            # optional continuous verify: whole-file LINK a fully byte-exact module
            # and confirm the rebuilt file is byte-identical to the shipped binary.
            if args.verify_link and len(a["exact"]) > 0 and not a["partial"] and not a["errors"]:
                _verify_link(m, totals)
            _feed_coverage(i, modules, totals, args)
            print("\nledger: %s (attempted %d this run)" % (path, attempted))
            if args.emit:
                print("emitted byte-exact .asm to reconstructed/asm/%s/" % os.path.basename(m).lower())
        except Exception as e:                     # one bad module must not kill the tier
            print("MODULE FAILED: %s: %r" % (m, e))
            progress.detail("FAILED  %-16s %s" % (os.path.basename(m).upper(), repr(e)[:48]))
            totals["errors"] += 1
        remaining_funcs -= nfuncs.get(m, 0)

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
    try:
        from src.engines.recon import masmlift
        masmlift.close_pools()                 # shut down the warm DOSBox workers
    except Exception:
        pass
    progress.finish(summ, beep=True)           # window stays open until user closes it


if __name__ == "__main__":
    main()
