"""reconstruct.py -- the one path from a module to judged reconstructions of its functions.

    from codebreaker.verify import reconstruct
    reconstruct.run(module_path)                       # every available decompiler, in order
    reconstruct.run(module_path, engines=("core",))    # one of them

For each decompiler in turn: ask it for the module's functions, build the ones that have no
verdict yet for the source they have now, run each beside the original on the same inputs, and
record what was found in the function store. A later decompiler is only asked about functions
no earlier one fully verified -- one verified reconstruction is enough, and judging is the
slow part.

Which decompiler produced a candidate makes no difference here: the building is its business
(codebreaker.decompile.engines), the judging and recording are the same for all of them.
"""
import os
import time

NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")


def _judge(module_path, item):
    """Worker: judge one function. item = (address, dll, entry). -> (verdict, reason, limits, detail)"""
    from codebreaker.verify import oracle as V
    from codebreaker.verify import pipeline
    pipeline._arm()
    va, dll, entry = item
    info = {}
    try:
        verdict, reason, limits = V.compare(
            lambda v: V._fp_pe(module_path, va, v),
            lambda v: V._fp_pe(dll, entry, v, rebuilt=module_path),
            V._vectors(4, "pe"), V._confirm_vectors(4, "pe"),
            insns=V.insn_count(module_path, va),
            directed=lambda cov, rch: V.directed_vectors(module_path, va, cov, reached=rch),
            info=info)
    except ImportError:
        raise
    except Exception as e:                                  # one function must not sink the rest
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")


def identity(module_path):
    """(content hash, profile digest) -- what results for this module are filed under."""
    from codebreaker.verify import oracle as V
    from codebreaker.verify import pipeline
    return V._mod_sha(module_path) or "", pipeline.profile("pe")[0]


def run(module_path, engines=None, fs=None, log=print, cascade=True):
    """Reconstruct and judge a module. -> (module hash, profile digest); results are in the store."""
    from codebreaker.decompile import engines as E
    from codebreaker.jobs import pool
    from codebreaker.store import functions
    from codebreaker.verify import body32 as B
    from codebreaker.verify import pipeline
    pipeline._arm()
    fs = fs or functions.open()
    lane = pipeline.LANES["pe"]
    addrs = lane.functions(module_path)
    sha, digest = identity(module_path)
    name = os.path.basename(module_path).lower()
    fs.add_module(sha, name, module_path, "pe")
    fs.add_profile(digest, pipeline.profile("pe")[1])
    fs.add_functions(sha, [lane.key(va) for va in addrs])
    settled = set()
    for ename in engines or E.DEFAULT:
        eng = E.get(ename)
        if not eng.available():
            log("%s: not available here, skipped" % ename)
            continue
        want = [va for va in addrs if lane.key(va) not in settled] if cascade else list(addrs)
        if not want:
            break
        t0 = time.time()
        prepared = eng.prepare(module_path, want, B._scratch(ename, name), log)
        if getattr(prepared, "preamble", None):
            fs.put_text(sha, ename, "header", prepared.preamble)
        known = fs.judged(sha, digest, ename)
        fresh = [va for va in want if va in prepared.source
                 and (known.get(lane.key(va)) or {}).get("sig") != prepared.sig[va]]
        rows = [dict(addr=lane.key(va), sig="-", name="", source=None, verdict="no-synth",
                     reason=prepared.problem.get(va) or "the decompiler found no function here", problem="none")
                for va in want if va not in prepared.source]
        if fresh:
            built = prepared.build(fresh)
            for va in fresh:
                if va not in built.entry:
                    fn, text = prepared.source[va]
                    rows.append(dict(addr=lane.key(va), sig=prepared.sig[va], name=fn, source=text,
                                     verdict="no-compile", reason=built.problem.get(va, ""), problem="compile"))
            items = [(va,) + tuple(built.entry[va]) for va in fresh if va in built.entry]
            judged = pool.run(_judge, items, context=module_path, crashed=lambda item: NOT_RUN)
            for va, (verdict, reason, limits, detail) in judged.items():
                if verdict in pipeline.GOOD:
                    limits = sorted(set(limits) | built.weak.get(va, set()))
                fn, text = prepared.source[va]
                rows.append(dict(addr=lane.key(va), sig=prepared.sig[va], name=fn, source=text, verdict=verdict,
                                 reason=reason, limits=",".join(limits), detail=detail))
        fs.record_many(sha, ename, eng.rev(), digest, rows)
        good = {a for a, r in fs.judged(sha, digest, ename).items()
                if functions.classify(r["verdict"], r["limits"])[0] == "verified"}
        settled |= good
        log("%s: asked about %d functions, judged %d now, verifies %d  (%.0fs)" % (
            ename, len(want), len(fresh), len(good), time.time() - t0))
    return sha, digest
