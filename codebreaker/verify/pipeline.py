#!/usr/bin/env python3
r"""pipeline.py -- the one behavioural-verification pipeline, for every lane.

For a set of functions of one retail module:

    synthesise C  ->  (skip if this exact input was verified before)  ->  compile  ->
    run retail and rebuilt on the oracle with the same inputs  ->  verdict + reason  ->
    bank the bodies that behave identically  ->  record every result

The 32-bit (PE) and 16-bit (NE) lanes used to be two separate copies of this sequence that had
drifted apart. The sequence now lives here once; a lane only supplies what really differs: how to
list functions, synthesise a body, compile, and take a fingerprint.

    from codebreaker.verify import pipeline, store
    res = pipeline.verify("pe", path, funcs, outdir, store=store.open())
    # res: {key: {"verdict", "fname", "reason", "cached"}}

Verdicts: equivalent (a clean return matched), consistent (matched, but only through an identical
fault or truncation), divergent, inconclusive (nothing could be observed), no-synth, no-compile.
A divergent body carries the first thing that differed as its reason: exit-differs, deps-differ
(the sequence of external calls) or ret-differs.
"""
import re
import hashlib
import os
import sys
from collections import Counter

from codebreaker.base import paths
from codebreaker.base import period_tools as toolchain

PIPE_REV = 19                   # what a verdict means (13: one build per batch; bodies the compiler rejects are dropped by name)
MAX_SUPPORT = 1500              # out-of-batch callee bodies pulled in per batch
_STATIC_NAME = re.compile(r"sub_([0-9A-Fa-f]{6})")     # the static decompiler's name for an rva
GOOD = ("equivalent", "consistent")


def _sha(*parts):
    h = hashlib.sha1()
    for p in parts:
        h.update(str(p).encode("utf-8", "replace")); h.update(b"\0")
    return h.hexdigest()


def why_divergent(rs, bs):
    """The first observable difference between the retail and rebuilt fingerprints."""
    from codebreaker.verify import oracle as verify95
    for a, b in zip(rs, bs):
        if "err" in a or "err" in b:
            continue
        d = verify95.first_difference(a, b)
        if d:
            return d
    return ""


_PROFILES = {}


def profile(lane):
    """(digest, settings) -- everything OTHER than a body's own source that a verdict depends on:
    the pipeline and oracle revisions, the inputs each body is run on, the compiler and its flags.
    The digest is part of every body's signature, so changing any of these re-verifies the corpus
    without anyone having to remember to bump a number; the settings are stored beside it."""
    if lane not in _PROFILES:
        import json
        from codebreaker.verify import body32 as B
        from codebreaker.verify import oracle as verify95
        cc = toolchain.ow_tool("wcc386" if lane == "pe" else "wcc")
        try:
            st = os.stat(cc)
            compiler = "%s:%d:%d" % (os.path.basename(cc), st.st_size, int(st.st_mtime))
        except OSError:
            compiler = ""
        settings = {
            "lane": lane, "pipeline": PIPE_REV, "oracle": verify95._ORACLE_REV,
            "inputs": [v["args"] for v in verify95._vectors(4, lane)],
            "confirm_inputs": [v["args"] for v in verify95._confirm_vectors(4, lane)],
            "max_calls": verify95._MAX_CALLS, "max_support": MAX_SUPPORT,
            "directed": [verify95.MAX_DIRECTED, verify95._HARVEST_DEPTH] if lane == "pe" else [],
            "cflags": list(B._CFLAGS) if lane == "pe" else [], "runtime": B._RTL if lane == "pe" else "",
            "compiler": compiler,
        }
        text = json.dumps(settings, sort_keys=True)
        _PROFILES[lane] = (hashlib.sha1(text.encode()).hexdigest()[:12], text)
    return _PROFILES[lane]


def why_inconclusive(rs, bs):
    return "; ".join(sorted({"retail: " + a["err"] for a in rs if "err" in a} |
                            {"rebuilt: " + b["err"] for b in bs if "err" in b}))


def _judge(rs, bs):
    from codebreaker.verify import oracle as verify95
    verdict = verify95._body_verdict(rs, bs)
    reason = (why_divergent(rs, bs) if verdict == "divergent"
              else why_inconclusive(rs, bs) if verdict == "inconclusive" else "")
    return verdict, reason


def _arm():
    from codebreaker.verify import oracle as verify95
    verify95._arm()
    tdir = paths.target_dir("win95")
    if tdir not in sys.path:
        sys.path.insert(0, tdir)                 # several win95 tools still import siblings bare


def support_bodies(path, src, imports, roots):
    """Add to `src` the real bodies of callees that `roots` reach but `src` lacks, transitively,
    and return their names. A callee without its real body links to a `return 0` stand-in, and its
    caller is then judged on behaviour that is not the reconstruction's."""
    from codebreaker.formats.pe import open_pe
    from codebreaker.verify import body32 as B
    pe = open_pe(path)
    by_name = pe.export_addrs()
    support, tried, work = [], set(), list(roots)
    while work and len(support) < MAX_SUPPORT:
        fn = work.pop()
        for ref in sorted(B._stub_refs(src[fn], fn)):
            if ref in src or ref in imports or ref in tried:
                continue
            tried.add(ref)
            m = _STATIC_NAME.fullmatch(ref)
            rva = int(m.group(1), 16) if m else by_name.get(ref)
            if rva is None:
                continue
            try:
                blk = B._static_body(path, pe.image_base + rva)
                c = B._cure_body(B._beautify(blk)) if blk else None
            except Exception:
                c = None
            if c and B._defined_name(c) == ref:
                src[ref] = c
                support.append(ref)
                work.append(ref)
    return support


# =============================================================================================
# Lanes
# =============================================================================================
class PELane(object):
    """32-bit PE. Bodies are compiled together in batches, so a body that calls a batch-mate is
    verified against that mate's real reconstruction rather than a stub."""
    name = "pe"

    def functions(self, path):
        _arm()
        from codebreaker.formats import pe_util as pe32util
        return sorted(pe32util.carve(path).funcs)

    def key(self, va):
        return "%x" % va

    def verify(self, path, funcs, outdir, known):
        """-> ({key: result}, [(fname, source, verdict)] to bank)."""
        from codebreaker.formats.pe import open_pe
        from codebreaker.verify import body32 as B
        from codebreaker.verify import oracle as verify95
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
        # A callee that is not in this batch (a batch is one chunk of a module's functions) would
        # link to a `return 0` stand-in, and the caller would then be judged on behaviour that is
        # not the reconstruction's. Pull in the real bodies of such callees, transitively, as
        # SUPPORT: compiled and linked, not themselves judged here.
        imports = B.import_map(path)
        support = support_bodies(path, src, imports, order)
        # A body's verdict depends on its own source AND on every body it (transitively) calls,
        # because those link to their real bodies. Sign all of it.
        calls = {fn: (B._stub_refs(src[fn], fn) & set(src)) for fn in order + support}

        def closure(fn):
            seen, work = set(), [fn]
            while work:
                f = work.pop()
                if f not in seen:
                    seen.add(f); work += list(calls[f])
            return seen
        clo = {fn: closure(fn) for fn in order}
        prof = profile("pe")[0]
        sig = {fn: _sha(prof, "pe", fn, src[fn],
                        *[src[g] for g in sorted(clo[fn] - {fn})]) for fn in order}
        # A callee that could not be given its real body links to a `return 0` stand-in: the
        # caller's verdict is then weaker than it looks, and says so.
        linked = set(src) | set(imports)
        stubbed = {fn: any(B._stub_refs(src[g], g) - linked for g in clo[fn]) for fn in order}
        todo = []
        for fn in order:
            row = known.get(self.key(va_of[fn]))
            if row and row.get("sig") == sig[fn] and row.get("verdict") not in ("no-synth",):
                results[self.key(va_of[fn])] = {"verdict": row["verdict"], "fname": fn,
                                                "reason": row.get("reason") or "", "cached": True,
                                                "sig": sig[fn], "limits": row.get("limits") or ""}
            else:
                todo.append(fn)
        bank = []
        if todo:
            need = set()
            for fn in todo:
                need |= clo[fn]                  # the mates a body links against must be compiled too
            wanted = [(fn, src[fn]) for fn in order + support if fn in need]
            dll, exports, rejected = B.batch_build(wanted, outdir, imports=imports)
            subs = [(dll, exports)] if dll else B._batch_or_bisect(wanted, outdir, imports=imports)
            # Bisecting around a body that does not compile leaves the good ones split across
            # several DLLs, where a callee in another half is a `return 0` stand-in again. Now that
            # the bad bodies are known, build every good one together.
            good = [fn for dll, exports in subs if dll for fn in exports]
            if sum(1 for dll, _e in subs if dll) > 1:
                dll, exports = B.batch_compile([(fn, src[fn]) for fn in order + support if fn in set(good)],
                                               os.path.join(outdir, "all"), imports)
                if dll:
                    subs = [(dll, exports)]
            built = {}
            for dll, exports in subs:
                if dll:
                    for fn in exports:
                        built[fn] = dll
            # ...and a callee that is still missing from a body's own DLL is a stand-in: say so.
            for fn in order:
                if any(built.get(g) != built.get(fn) for g in clo[fn]):
                    stubbed[fn] = True
            vecs = verify95._vectors(4, "pe")
            more = verify95._confirm_vectors(4, "pe")
            done = set()
            for dll, exports in subs:
                if not dll:
                    continue
                try:
                    pb = open_pe(dll); bbase = pb.image_base; addrs = pb.export_addrs()
                except (OSError, ValueError):
                    continue                     # built, then gone or unreadable: not a result
                from codebreaker.verify import whole_module as ghidra_engine
                mine = [fn for fn in exports if fn in todo and addrs.get(fn)]
                fn_of = {va_of[fn]: fn for fn in mine}
                judged = ghidra_engine.judge_all(path, dll, [(va_of[fn], bbase + addrs[fn]) for fn in mine])
                for va, (verdict, reason, limits, detail) in judged.items():
                    fn = fn_of[va]
                    done.add(fn)
                    info = {"detail": detail}
                    if stubbed[fn] and verdict in GOOD:
                        limits = sorted(set(limits) | {"stub-callee"})
                    results[self.key(va_of[fn])] = {"verdict": verdict, "fname": fn, "reason": reason,
                                                    "sig": sig[fn], "limits": ",".join(limits),
                                                    "detail": info.get("detail", "")}
                    if verdict == "equivalent":
                        bank.append((fn, src[fn], verdict))
            for fn in todo:
                if fn not in done:
                    results[self.key(va_of[fn])] = {"verdict": "no-compile", "fname": fn,
                                                    "reason": rejected.get(fn, ""), "sig": sig[fn]}
        return results, bank

    def save(self, path, fname, source, verdict):
        from codebreaker.verify import body32 as B
        B._save_verified(path, fname, source)


class NELane(object):
    """16-bit NE. One body per rebuilt DLL (exported at ordinal 1); Win16 API calls are linked as
    real imports so the oracle sees them, internal callees are stubbed. A Win16 body normally
    faults on synthetic arguments, so a match is usually 'consistent', and that is banked."""
    name = "ne"

    def functions(self, path):
        _arm()
        from codebreaker.recover import interproc16
        from codebreaker.formats.ne import NEFile
        return list(interproc16.all_functions(NEFile(path)))

    def key(self, f):
        return "%d:%04x" % (f[0], f[1])

    def verify(self, path, funcs, outdir, known):
        from codebreaker.verify import body16 as B16
        from codebreaker.emulate import ne_probe as NO
        from codebreaker.verify import oracle as verify95
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
            sig = _sha(profile("ne")[0], "ne", fn, c)
            row = known.get(k)
            if row and row.get("sig") == sig:
                results[k] = {"verdict": row["verdict"], "fname": fn, "reason": row.get("reason") or "",
                              "cached": True, "sig": sig, "limits": row.get("limits") or ""}
                continue
            dll, clog = B16.compile_body16(c, fn, os.path.join(outdir, "%d_%x" % (seg, off)))
            if not dll:
                results[k] = {"verdict": "no-compile", "fname": fn, "reason": str(clog)[:120], "sig": sig}
                continue
            def probe(v, **where):
                try:
                    return B16._fp(NO.probe(args=v["args"], **where))
                except ImportError:
                    raise
                except Exception as e:
                    return {"err": type(e).__name__}
            verdict, reason, limits = verify95.compare(
                lambda v: probe(v, dll=path, seg=seg, offset=off),
                lambda v: probe(v, dll=dll, ordinal=1), vecs)
            # Internal callees are stand-ins in this lane; a body that calls one says so.
            if verdict in GOOD and re.search(r"\bsub_\w+\s*\(", c.replace(fn + "(", "(")):
                limits = sorted(set(limits) | {"stub-callee"})
            results[k] = {"verdict": verdict, "fname": fn, "reason": reason, "sig": sig,
                          "limits": ",".join(limits)}
            if verdict in GOOD:
                bank.append((fn, c, verdict))
        return results, bank

    def save(self, path, fname, source, verdict):
        from codebreaker.verify import body16 as B16
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
    from codebreaker.verify import body32 as B
    from codebreaker.verify import oracle as verify95
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
        digest, settings = profile(lane)
        store.put_profile(digest, lane, settings)
        store.put_many((lane, module, mod_sha, k, r.get("fname", ""), r.get("sig", ""),
                        r["verdict"], r.get("reason", ""), build, r.get("limits", ""), digest,
                        r.get("detail", ""))
                       for k, r in results.items() if not r.get("cached"))
    for r in results.values():
        r.setdefault("cached", False)
        r.pop("sig", None)
    return results


def tally(results):
    """Counter of verdicts for a verify() result."""
    return Counter(r["verdict"] for r in results.values())
