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
16-bit Windows, PowerPC, drivers, real-mode programs -- is in one small module per kind, each providing:

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


# the name results are filed under -> the module that knows that kind
KINDS = {"pe": "codebreaker.verify.win32", "ne": "codebreaker.verify.win16",
         "powerpc": "codebreaker.verify.powerpc", "le": "codebreaker.verify.driver",
         "dos": "codebreaker.verify.dos"}


def kind_named(name):
    import importlib
    return importlib.import_module(KINDS[name])


def kind_name(module_path):
    """Which kind a file is: 'pe', 'ne', 'powerpc', 'le' -- or None when nothing reconstructs it."""
    from codebreaker.base import corpus
    lane = corpus.lane(module_path)
    if lane == "pe":
        from codebreaker.formats.pe import open_pe
        return "powerpc" if open_pe(module_path).machine == 0x01F2 else "pe"
    if lane is None:                                         # .COM files and plain driver images have no header
        from codebreaker.formats import dos
        return "dos" if dos.kind(module_path) else None
    return lane if lane in KINDS else None


def kind_of(module_path):
    name = kind_name(module_path)
    if name is None:
        from codebreaker.base import corpus
        raise ValueError("%s: no way to reconstruct a %s file yet" % (
            os.path.basename(module_path), corpus.lane(module_path) or "non-executable"))
    return kind_named(name)


_DIGESTS = {}


def digests():
    """{kind: digest of the verifier settings in force now} (None for a kind whose tools are not
    set up on this machine). Results are filed per kind under these."""
    if not _DIGESTS:
        for name in KINDS:
            try:
                _DIGESTS[name] = kind_named(name).profile("")[0]
            except Exception:
                _DIGESTS[name] = None
    return _DIGESTS


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


def signatures(m, whole):
    """{name: what that function's verdict was reached for}: its own text, the header, and the
    text of every function it can reach -- it is built and run together with those, so a change
    in one of them is a change in what was judged. A function none of that changed for keeps its
    verdict, however much of the rest of the module changed.

    Functions that call each other in a ring are one group with one signature. `whole` is the
    signature for a module that is not made of separate texts."""
    import hashlib
    texts = getattr(m, "texts", None)
    if not texts or not getattr(m, "header", None):
        return {n: whole for n in getattr(m, "order", ())}
    names = set(texts)
    word = re.compile(r"[A-Za-z_]\w*")
    calls = {n: sorted((set(word.findall(t)) & names) - {n}) for n, t in texts.items()}
    # strongly connected groups (Tarjan, without recursion: call chains are long), callees first
    index, low, on, stack, group, order, count = {}, {}, set(), [], {}, [], 0
    for root in texts:
        if root in index:
            continue
        work = [(root, 0)]
        while work:
            n, i = work.pop()
            if i == 0:
                index[n] = low[n] = count
                count += 1
                stack.append(n)
                on.add(n)
            for j in range(i, len(calls[n])):
                c = calls[n][j]
                if c not in index:
                    work.append((n, j + 1))
                    work.append((c, 0))
                    break
                if c in on:
                    low[n] = min(low[n], index[c])
            else:
                if low[n] == index[n]:
                    members = []
                    while True:
                        x = stack.pop()
                        on.discard(x)
                        members.append(x)
                        if x == n:
                            break
                    order.append(members)
                    for x in members:
                        group[x] = len(order) - 1
                if work:
                    parent = work[-1][0]
                    low[parent] = min(low[parent], low[n])
    base = hashlib.sha1(m.header.encode("utf-8", "replace")).hexdigest()
    sig_of, out = {}, {}
    for g, members in enumerate(order):                      # callees come before their callers
        h = hashlib.sha1(base.encode())
        for x in sorted(members):
            h.update(x.encode() + b"\0" + texts[x].encode("utf-8", "replace") + b"\0")
        for other in sorted({group[c] for x in members for c in calls[x]} - {g}):
            h.update(sig_of[other].encode())
        sig_of[g] = h.hexdigest()
        for x in members:
            out[x] = sig_of[g]
    return out


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
    t1 = time.time()
    sig = signatures(m, m.sig) if getattr(kind, "PER_FUNCTION", False) else {n: m.sig for n in m.order}
    fs.add_module(sha, name, module_path, kind.KIND)
    fs.add_profile(digest, settings)
    fs.add_functions(sha, m.all_keys)
    fs.put_text(sha, SOURCE, "header", m.header)
    fs.set_identified(sha, {names.key(module_path, address): found
                            for address, found in runtime_library.identify(module_path).items()})
    known = fs.judged(sha, digest, SOURCE)
    fresh = [n for n in m.order if n in m.key and (known.get(m.key[n]) or {}).get("sig") != sig[n]]
    have = set(m.key.values())
    rows = [dict(addr=k, sig="-", name="", source=None, verdict="no-synth",
                 reason="the decompiler produced nothing for this function", problem="none")
            for k in m.all_keys if k not in have]
    t2 = t3 = time.time()
    if fresh:
        context, items, dropped = kind.build(m, module_path, work)
        t2 = time.time()
        tainted = _reaches(m.texts, list(items), dropped)
        for n in fresh:
            if n not in items:
                rows.append(dict(addr=m.key[n], sig=sig[n], name=n, source=m.texts[n], verdict="no-compile",
                                 reason=dropped.get(n, "") or "not in the build", problem="compile"))
        name_of = {m.key[n]: n for n in fresh if n in items}
        judged = pool.run(kind.judge, [items[n] for n in fresh if n in items], context=context,
                          crashed=lambda item: NOT_RUN)
        for k, (verdict, reason, limits, detail) in judged.items():
            n = name_of[k]
            if n in tainted and verdict in GOOD:
                limits = sorted(set(limits) | {"stub-callee"})   # a function it calls was a stand-in
            rows.append(dict(addr=k, sig=sig[n], name=n, source=m.texts[n], verdict=verdict, reason=reason,
                             limits=",".join(limits), detail=detail))
        t3 = time.time()
    fs.record_many(sha, SOURCE, str(decompiler.REV), digest, rows)
    c = fs.scores(digest, sha)
    log("%s: %d functions, %d judged now: %s  (%.0fs: decompile %.0f, build %.0f, judge %.0f)" % (
        name, len(m.all_keys), len(fresh),
        ", ".join("%s %d" % (functions.TITLE[k], c[k]) for k in functions.SCORED if c[k]), time.time() - t0,
        t1 - t0, t2 - t1, t3 - t2))
    return sha, digest
