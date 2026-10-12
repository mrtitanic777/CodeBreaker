#!/usr/bin/env python3
r"""pipeline.py -- the one behavioural-verification pipeline, for every lane.

For a set of functions of one retail module:

    synthesise C  ->  (skip if this exact input was verified before)  ->  compile  ->
    run retail and rebuilt on the oracle with the same inputs  ->  verdict + reason  ->
    bank the bodies that behave identically  ->  record every result

The 32-bit (PE) and 16-bit (NE) lanes used to be two separate copies of this sequence that had
drifted apart. The sequence now lives here once; a lane only supplies what really differs: how to
list functions, synthesise a body, compile, and take a fingerprint.

    from src.targets.win95.verification import pipeline, store
    res = pipeline.verify("pe", path, funcs, outdir, store=store.open())
    # res: {key: {"verdict", "fname", "reason", "cached"}}

Verdicts: equivalent (a clean return matched), consistent (matched, but only through an identical
fault or truncation), divergent, inconclusive (nothing could be observed), no-synth, no-compile.
A divergent body carries the first thing that differed as its reason: exit-differs, deps-differ
(the sequence of external calls) or ret-differs.
"""
import hashlib
import os
import sys
from collections import Counter

from src.core import paths, toolchain

PIPE_REV = 3                    # bump when the pipeline changes what a verdict means (3: SEH bodies build)
GOOD = ("equivalent", "consistent")


def _sha(*parts):
    h = hashlib.sha1()
    for p in parts:
        h.update(str(p).encode("utf-8", "replace")); h.update(b"\0")
    return h.hexdigest()


def why_divergent(rs, bs):
    """The first observable difference between the retail and rebuilt fingerprints."""
    for a, b in zip(rs, bs):
        if "err" in a or "err" in b:
            continue
        if a["exit"] != b["exit"]:
            return "exit-differs"
        if a["deps"] != b["deps"]:
            return "deps-differ"
        if a["ret"] != b["ret"]:
            return "ret-differs"
    return ""


def why_inconclusive(rs, bs):
    return "; ".join(sorted({"retail: " + a["err"] for a in rs if "err" in a} |
                            {"rebuilt: " + b["err"] for b in bs if "err" in b}))


def _judge(rs, bs):
    from src.targets.win95 import verify95
    verdict = verify95._body_verdict(rs, bs)
    reason = (why_divergent(rs, bs) if verdict == "divergent"
              else why_inconclusive(rs, bs) if verdict == "inconclusive" else "")
    return verdict, reason


def _arm():
    from src.targets.win95 import verify95
    verify95._arm()
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)                 # several win95 tools still import siblings bare


# =============================================================================================
# Lanes
# =============================================================================================
class PELane(object):
    """32-bit PE. Bodies are compiled together in batches, so a body that calls a batch-mate is
    verified against that mate's real reconstruction rather than a stub."""
    name = "pe"

    def functions(self, path):
        _arm()
        import pe32util
        return sorted(pe32util.carve(path).funcs)

    def key(self, va):
        return "%x" % va

    def verify(self, path, funcs, outdir, known):
        """-> ({key: result}, [(fname, source, verdict)] to bank)."""
        from src.formats.pe import open_pe
        from src.targets.win95 import bodyverify as B, verify95
        results, order, src, va_of = {}, [], {}, {}
        for va in funcs:
            k = self.key(va)
            try:
                fn, c = B.synth(path, va, 4, fast=True)
            except Exception as e:
                fn, c = None, type(e).__name__
            if not fn:
                results[k] = {"verdict": "no-synth", "fname": "", "reason": str(c or "")[:120]}
            elif fn not in src:                  # one body reached at two addresses: keep the first
                src[fn] = c; va_of[fn] = va; order.append(fn)
        # A body's verdict depends on its own source AND on every batch-mate it (transitively)
        # calls, because those link to their real bodies. Sign all of it.
        calls = {fn: (B._stub_refs(src[fn], fn) & set(src)) for fn in order}

        def closure(fn):
            seen, work = set(), [fn]
            while work:
                f = work.pop()
                if f not in seen:
                    seen.add(f); work += list(calls[f])
            return seen
        clo = {fn: closure(fn) for fn in order}
        sig = {fn: _sha(PIPE_REV, verify95._ORACLE_REV, "pe", fn, src[fn],
                        *[src[g] for g in sorted(clo[fn] - {fn})]) for fn in order}
        todo = []
        for fn in order:
            row = known.get(self.key(va_of[fn]))
            if row and row.get("sig") == sig[fn] and row.get("verdict") not in ("no-synth",):
                results[self.key(va_of[fn])] = {"verdict": row["verdict"], "fname": fn,
                                                "reason": row.get("reason") or "", "cached": True,
                                                "sig": sig[fn]}
            else:
                todo.append(fn)
        bank = []
        if todo:
            need = set()
            for fn in todo:
                need |= clo[fn]                  # the mates a body links against must be compiled too
            subs = B._batch_or_bisect([(fn, src[fn]) for fn in order if fn in need], outdir)
            vecs = verify95._vectors(4, "pe")
            done = set()
            for dll, exports in subs:
                if not dll:
                    continue
                pb = open_pe(dll); bbase = pb.image_base; addrs = pb.export_addrs()
                for fn in exports:
                    if fn not in todo or not addrs.get(fn):
                        continue
                    done.add(fn)
                    rs = [verify95._fp_pe(path, va_of[fn], v) for v in vecs]
                    bs = [verify95._fp_pe(dll, bbase + addrs[fn], v) for v in vecs]
                    verdict, reason = _judge(rs, bs)
                    results[self.key(va_of[fn])] = {"verdict": verdict, "fname": fn,
                                                    "reason": reason, "sig": sig[fn]}
                    if verdict == "equivalent":
                        bank.append((fn, src[fn], verdict))
            for fn in todo:
                if fn not in done:
                    results[self.key(va_of[fn])] = {"verdict": "no-compile", "fname": fn,
                                                    "reason": "", "sig": sig[fn]}
        return results, bank

    def save(self, path, fname, source, verdict):
        from src.targets.win95 import bodyverify as B
        B._save_verified(path, fname, source)


class NELane(object):
    """16-bit NE. One body per rebuilt DLL (exported at ordinal 1); Win16 API calls are linked as
    real imports so the oracle sees them, internal callees are stubbed. A Win16 body normally
    faults on synthetic arguments, so a match is usually 'consistent', and that is banked."""
    name = "ne"

    def functions(self, path):
        _arm()
        from src.engines.behavior import interproc16
        from src.formats.ne import NEFile
        return list(interproc16.all_functions(NEFile(path)))

    def key(self, f):
        return "%d:%04x" % (f[0], f[1])

    def verify(self, path, funcs, outdir, known):
        from src.targets.win95 import bodyverify16 as B16, ne_dynoracle as NO, verify95
        results, bank = {}, []
        vecs = verify95._vectors(4, "ne")
        for seg, off in funcs:
            k = self.key((seg, off))
            try:
                fn, c = B16.synth16(path, seg, off)
            except Exception as e:
                fn, c = None, type(e).__name__
            if not fn or fn in ("LibMain", "WEP"):       # the DLL's own entry/exit are not targets
                results[k] = {"verdict": "no-synth", "fname": fn or "",
                              "reason": "entry" if fn else str(c or "")[:120]}
                continue
            sig = _sha(PIPE_REV, verify95._ORACLE_REV, "ne", fn, c)
            row = known.get(k)
            if row and row.get("sig") == sig:
                results[k] = {"verdict": row["verdict"], "fname": fn, "reason": row.get("reason") or "",
                              "cached": True, "sig": sig}
                continue
            dll, clog = B16.compile_body16(c, fn, os.path.join(outdir, "%d_%x" % (seg, off)))
            if not dll:
                results[k] = {"verdict": "no-compile", "fname": fn, "reason": str(clog)[:120], "sig": sig}
                continue
            rs, bs = [], []
            for v in vecs:
                try:
                    rs.append(B16._fp(NO.probe(path, seg=seg, offset=off, args=v["args"])))
                except ImportError:
                    raise
                except Exception as e:
                    rs.append({"err": type(e).__name__})
                try:
                    bs.append(B16._fp(NO.probe(dll, ordinal=1, args=v["args"])))
                except ImportError:
                    raise
                except Exception as e:
                    bs.append({"err": type(e).__name__})
            verdict, reason = _judge(rs, bs)
            results[k] = {"verdict": verdict, "fname": fn, "reason": reason, "sig": sig}
            if verdict in GOOD:
                bank.append((fn, c, verdict))
        return results, bank

    def save(self, path, fname, source, verdict):
        from src.targets.win95 import bodyverify16 as B16
        B16._save_verified16(path, fname, source, verdict)


LANES = {"pe": PELane(), "ne": NELane()}


# =============================================================================================
# The pipeline
# =============================================================================================
def functions(lane, path):
    return LANES[lane].functions(path)


def verify(lane, path, funcs=None, outdir=None, store=None, bank=True):
    """Verify `funcs` (default: every function) of the retail module at `path`.
    Returns {key: {"verdict", "fname", "reason", "cached"}}. With a `store`, verdicts whose inputs
    are unchanged are reused, and every result is recorded."""
    from src.targets.win95 import bodyverify as B, verify95
    _arm()
    L = LANES[lane]
    if funcs is None:
        funcs = L.functions(path)
    outdir = outdir or B._scratch(lane, os.path.basename(path).lower())
    mod_sha = verify95._mod_sha(path) or ""
    known = store.get_many(lane, mod_sha) if (store and mod_sha) else {}
    results, to_bank = L.verify(path, funcs, outdir, known)
    if bank:
        for fname, source, verdict in to_bank:
            L.save(path, fname, source, verdict)
    if store and mod_sha:
        build = toolchain.decompiler_build_id()
        module = os.path.basename(path).lower()
        store.put_many((lane, module, mod_sha, k, r.get("fname", ""), r.get("sig", ""),
                        r["verdict"], r.get("reason", ""), build)
                       for k, r in results.items() if not r.get("cached"))
    for r in results.values():
        r.setdefault("cached", False)
        r.pop("sig", None)
    return results


def tally(results):
    """Counter of verdicts for a verify() result."""
    return Counter(r["verdict"] for r in results.values())
