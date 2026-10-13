"""reconstruct.py -- the path from a module to judged reconstructions of its functions.

    from codebreaker.verify import reconstruct
    sha, profile = reconstruct.run(module_path)        # results are in the function store

Decompile the module, build the functions that have no verdict yet for the text they have now,
run each beside the original on the same inputs, and record what was found. Functions already
judged for their current text are not built or run again.
"""
import hashlib
import os
import re
import time

SOURCE = "decompiler"                  # who a candidate in the store came from
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


def _is16(module_path):
    from codebreaker.base import corpus
    return corpus.lane(module_path) == "ne"


def identity(module_path):
    """(content hash, profile digest) -- what results for this module are filed under."""
    if _is16(module_path):
        from codebreaker.verify import reconstruct16
        return reconstruct16.identity(module_path)
    from codebreaker.verify import oracle as V
    from codebreaker.verify import pipeline
    return V._mod_sha(module_path) or "", pipeline.profile("pe")[0]


def _reaches(texts, built, dropped):
    """Names in `built` that call a dropped function (now a stand-in), directly or through others."""
    if not dropped:
        return set()
    pat = re.compile(r"\b(%s)\s*\(" % "|".join(re.escape(d) for d in dropped))
    calls = {n: set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", texts[n])) & set(built) for n in built}
    tainted = {n for n in built if pat.search(texts[n].split("{", 1)[-1])}
    grew = True
    while grew:
        grew = False
        for n in built:
            if n not in tainted and calls[n] & tainted:
                tainted.add(n)
                grew = True
    return tainted


def run(module_path, fs=None, log=print):
    """Reconstruct and judge a module, 32-bit or 16-bit. -> (module hash, profile digest)."""
    if _is16(module_path):
        from codebreaker.verify import reconstruct16
        return reconstruct16.run(module_path, fs=fs, log=log)
    from codebreaker.build import whole_module
    from codebreaker.decompile import decompiler
    from codebreaker.formats.pe import open_pe
    from codebreaker.jobs import pool
    from codebreaker.store import functions
    from codebreaker.build import watcom as B
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
    t0 = time.time()
    work = B._scratch("reconstruct", name)
    mod = decompiler.decompile(module_path, addrs, work)
    fs.put_text(sha, SOURCE, "header", mod.header)
    # The module is one translation unit: any change to it may change any function's build.
    sig = hashlib.sha1(("%s\0%s\0%s" % (decompiler.REV, mod.header, "".join(mod.texts[n] for n in mod.order)))
                       .encode("utf-8", "replace")).hexdigest()
    known = fs.judged(sha, digest, SOURCE)
    fresh = [va for va in addrs if va in mod.functions and (known.get(lane.key(va)) or {}).get("sig") != sig]
    rows = [dict(addr=lane.key(va), sig="-", name="", source=None, verdict="no-synth",
                 reason="the decompiler produced nothing for this function", problem="none")
            for va in addrs if va not in mod.functions]
    if fresh:
        # Only functions with a known address are built. The rest are library routines recognised
        # by signature (__allshl, _memcpy): they belong to the C runtime, which is linked.
        placed = set(mod.by_va.values())
        dll, built, dropped = whole_module.build(module_path, mod.header, mod.texts,
                                                 [n for n in mod.order if n in placed], mod.imports,
                                                 work, cflags=decompiler.CFLAGS)
        at, base = {}, 0
        if dll:
            pb = open_pe(dll)
            at, base = pb.export_addrs(), pb.image_base
        tainted = _reaches(mod.texts, built, dropped)
        items = []
        for va in fresh:
            fn, text = mod.functions[va]
            if fn in dropped or fn not in at:
                rows.append(dict(addr=lane.key(va), sig=sig, name=fn, source=text, verdict="no-compile",
                                 reason=dropped.get(fn, "") or "not in the build", problem="compile"))
            else:
                items.append((va, dll, base + at[fn]))
        judged = pool.run(_judge, items, context=module_path, crashed=lambda item: NOT_RUN)
        for va, (verdict, reason, limits, detail) in judged.items():
            fn, text = mod.functions[va]
            if fn in tainted and verdict in pipeline.GOOD:
                limits = sorted(set(limits) | {"stub-callee"})   # a callee was a stand-in
            rows.append(dict(addr=lane.key(va), sig=sig, name=fn, source=text, verdict=verdict,
                             reason=reason, limits=",".join(limits), detail=detail))
    fs.record_many(sha, SOURCE, str(decompiler.REV), digest, rows)
    c = fs.tally(digest, sha)
    log("%s: %d functions, %d judged now: %s  (%.0fs)" % (
        name, len(addrs), len(fresh), ", ".join("%s %d" % (functions.TITLE[k], c[k]) for k in functions.ORDER if c[k]),
        time.time() - t0))
    return sha, digest
