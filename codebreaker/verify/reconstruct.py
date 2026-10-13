"""reconstruct.py -- the path from a module to judged reconstructions of its functions.

    from codebreaker.verify import reconstruct
    sha, profile = reconstruct.run(module_path)        # results are in the function store

Decompile the module, build the functions that have no verdict yet for the text they have now,
run each beside the original on the same inputs, and record what was found. Functions already
judged for their current text are not built or run again.

Functions that are a runtime library's (the C runtime, compiler helpers: symbols/runtime_library.py)
are recorded as such, with their name and library. That is said in addition: they are
decompiled, built and judged like every other function.

The path is the same whatever the module is. What differs between kinds of module -- 32-bit or
16-bit Windows, PowerPC -- is in one small module per kind (win32.py, win16.py, powerpc.py), each providing:

    KIND                          the name results are filed under
    profile(path)                 (digest, settings): everything but the source a verdict depends on
    prepare(path, work)           the decompiled module: header, texts {name: C}, order,
                                  key {name: address}, all_keys (every function), sig
    build(prepared, path, work)   (context, {name: item to judge}, {name: why it was not built})
    judge(context, item)          (verdict, reason, limits, detail) -- runs in a worker process

A new kind of module is one such file.
"""
import os
import re
import time

SOURCE = "decompiler"                  # who a candidate in the store came from
NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")
GOOD = ("equivalent", "consistent")


def kind_of(module_path):
    from codebreaker.base import corpus
    lane = corpus.lane(module_path)
    if lane == "ne":
        from codebreaker.verify import win16
        return win16
    if lane == "pe":
        from codebreaker.formats.pe import open_pe
        if open_pe(module_path).machine == 0x01F2:
            from codebreaker.verify import powerpc
            return powerpc
        from codebreaker.verify import win32
        return win32
    raise ValueError("%s: no way to reconstruct a %s file yet" % (os.path.basename(module_path), lane or "non-executable"))


def identity(module_path):
    """(content hash, profile digest) -- what results for this module are filed under."""
    from codebreaker.base import corpus
    return corpus.sha(module_path) or "", kind_of(module_path).profile(module_path)[0]


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
    """Reconstruct and judge a module. -> (module hash, profile digest)."""
    from codebreaker.build import watcom
    from codebreaker.decompile import decompiler
    from codebreaker.jobs import pool
    from codebreaker.store import functions
    from codebreaker.base import corpus
    from codebreaker.symbols import names, runtime_library
    kind = kind_of(module_path)
    fs = fs or functions.open()
    sha = corpus.sha(module_path) or ""
    digest, settings = kind.profile(module_path)
    name = os.path.basename(module_path).lower()
    t0 = time.time()
    work = watcom._scratch("reconstruct", kind.KIND, name)
    m = kind.prepare(module_path, work)
    fs.add_module(sha, name, module_path, kind.KIND)
    fs.add_profile(digest, settings)
    fs.add_functions(sha, m.all_keys)
    fs.put_text(sha, SOURCE, "header", m.header)
    fs.set_identified(sha, {names.key(module_path, address): found
                            for address, found in runtime_library.identify(module_path).items()})
    known = fs.judged(sha, digest, SOURCE)
    fresh = [n for n in m.order if n in m.key and (known.get(m.key[n]) or {}).get("sig") != m.sig]
    have = set(m.key.values())
    rows = [dict(addr=k, sig="-", name="", source=None, verdict="no-synth",
                 reason="the decompiler produced nothing for this function", problem="none")
            for k in m.all_keys if k not in have]
    if fresh:
        context, items, dropped = kind.build(m, module_path, work)
        tainted = _reaches(m.texts, list(items), dropped)
        for n in fresh:
            if n not in items:
                rows.append(dict(addr=m.key[n], sig=m.sig, name=n, source=m.texts[n], verdict="no-compile",
                                 reason=dropped.get(n, "") or "not in the build", problem="compile"))
        name_of = {m.key[n]: n for n in fresh if n in items}
        judged = pool.run(kind.judge, [items[n] for n in fresh if n in items], context=context,
                          crashed=lambda item: NOT_RUN)
        for k, (verdict, reason, limits, detail) in judged.items():
            n = name_of[k]
            if n in tainted and verdict in GOOD:
                limits = sorted(set(limits) | {"stub-callee"})   # a function it calls was a stand-in
            rows.append(dict(addr=k, sig=m.sig, name=n, source=m.texts[n], verdict=verdict, reason=reason,
                             limits=",".join(limits), detail=detail))
    fs.record_many(sha, SOURCE, str(decompiler.REV), digest, rows)
    c = fs.scores(digest, sha)
    log("%s: %d functions, %d judged now: %s  (%.0fs)" % (
        name, len(m.all_keys), len(fresh),
        ", ".join("%s %d" % (functions.TITLE[k], c[k]) for k in functions.SCORED if c[k]), time.time() - t0))
    return sha, digest
