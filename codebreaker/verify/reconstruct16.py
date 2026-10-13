"""reconstruct16.py -- judged reconstructions of a 16-bit Windows module's functions.

The same path as for 32-bit modules (reconstruct.py), with the 16-bit pieces in place of the
32-bit ones: decompile the module, compile every function and link a 16-bit DLL, run each
function beside the original in the 16-bit emulator on the same inputs, record what was found.

Both sides are entered the way the function is really called (far or near) with the same words
on the stack, and the rebuilt module's data segment is given the original's data, so a global is
at the same offset and starts with the same value on both sides. Compared: how the run ends,
the Windows functions called, in order, the result in AX (DX:AX when the function returns four
bytes), what is left in the data segment and on the far heap, how many bytes of arguments were
removed, and whether SI, DI and DS were preserved.

Not compared yet, and so every match involving them is marked weak: the arguments passed to
Windows functions (how many words each takes is not known here).
"""
import hashlib
import os
import time

SOURCE = "decompiler"
NOT_RUN = ("inconclusive", "judge: the emulator crashed on this function", [], "")
ARG_WORDS = 8
LEFTOVER = 0x7EF707E5                  # stands for "AX held no result" in a fingerprint
REV = 10                                # bump when what is built or compared changes


def vectors():
    n = ARG_WORDS
    return [{"args": [0] * n}, {"args": [0x0400] * n}, {"args": [0x0080] * n},
            {"args": [i + 1 for i in range(n)]}, {"args": [0x100 * (i + 1) for i in range(n)]},
            {"args": [0xFFFF - i for i in range(n)]}]


def fingerprint(path, seg, off, near, wide, v, data_image=None, span=None):
    """What one run of one function did, in a form two builds can be compared on."""
    from codebreaker.emulate import ne_probe
    from codebreaker.verify import oracle as V
    try:
        r = ne_probe.probe(path, seg=seg, offset=off, args=v["args"], near=near, data_image=data_image)
    except ImportError:
        raise
    except Exception as e:
        return {"err": type(e).__name__}
    ex = V._exit_norm(r.get("exit"))
    lay = r.get("layout") or {}
    ds = lay.get("ds_base")
    state, before = {}, {}
    for w in r.get("mem_writes") or []:
        a, n, val = w["addr"], w["size"], w["value"] or 0
        if ds is not None and ds <= a < ds + 0x10000:
            where = ("D", a - ds)
        elif ne_probe.HEAP_BASE <= a < ne_probe.HEAP_BASE + 64 * ne_probe.HEAP_STRIDE:
            where = ("H", a - ne_probe.HEAP_BASE)
        else:
            continue                                         # the stack, or somewhere wild
        for i in range(min(n, 4)):
            k = (where[0], where[1] + i)
            before.setdefault(k, ((w.get("old") or 0) >> (8 * i)) & 0xFF)
            state[k] = (val >> (8 * i)) & 0xFF
    # a byte left holding what it held on entry is not an effect (one compiler clears a flag
    # with a word write, another with a byte write)
    state = {k: x for k, x in state.items() if x != before.get(k)}
    writes = "%d:%s" % (len(state), hashlib.sha1(repr(sorted(state.items())).encode()).hexdigest()[:16]) \
        if state else ""
    regs = r.get("exit_regs") or {}
    entry = r.get("entry_regs") or {}
    base = r.get("code_base") or 0
    lo, hi = span or (off, off)
    cov = tuple(sorted({a - base - lo for a in r.get("path") or () if lo <= a - base < hi}))
    ret = None
    if ex == "ret" and wide is not None:                     # None: the function returns nothing
        ret = (r.get("ret32") if wide else r.get("ret")) or 0
        # An exported function starts `push ds / pop ax`; one that sets no result returns with
        # that still in AX. Its own data selector is a leftover, not a result.
        if ret & 0xFFFF == (entry.get("DS") or -1) and not wide:
            ret = LEFTOVER
    return {"ret": ret, "exit": ex, "truncated": bool(r.get("truncated")),
            "deps": tuple((h.get("api") or "?") for h in r.get("imports_hit") or ()),
            "writes": writes, "pop": r.get("sp_delta") if ex == "ret" else None,
            "saved": (regs.get("SI") == entry.get("SI") and regs.get("DI") == entry.get("DI")
                      and regs.get("DS") == entry.get("DS")) if ex == "ret" else None,
            "cov": cov, "reach": ()}


def _judge(context, item):
    """Worker. item = (key, seg, off, near, wide, span, insns, rebuilt seg, rebuilt off)."""
    from codebreaker.verify import oracle as V
    module_path, dll, data_image = context
    key, seg, off, near, wide, span, insns, bseg, boff = item
    info = {}
    try:
        last = {}

        def original(v):
            last["fp"] = fingerprint(module_path, seg, off, near, wide, v, span=span)
            return last["fp"]

        def rebuilt(v):
            fp = fingerprint(dll, bseg, boff, near, wide, v, data_image=data_image)
            if (last.get("fp") or {}).get("ret") == LEFTOVER and "err" not in fp and fp.get("ret") is not None:
                fp["ret"] = LEFTOVER                         # the original returned no value here
            return fp
        verdict, reason, limits = V.compare(original, rebuilt, vectors(), (), insns=insns, info=info)
    except ImportError:
        raise
    except Exception as e:
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")


def _spans(nef, where):
    """{address: ((start, end), instruction count)} -- each function runs to the next one in its
    segment (or the segment's end)."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    segs = {s["idx"]: s for s in nef.segments()}
    by_seg = {}
    for va, (seg, off) in where.items():
        by_seg.setdefault(seg, []).append((off, va))
    out = {}
    for seg, lst in by_seg.items():
        lst.sort()
        s = segs[seg]
        code = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        for i, (off, va) in enumerate(lst):
            end = lst[i + 1][0] if i + 1 < len(lst) else len(code)
            n = sum(1 for _ in md.disasm(code[off:end], off))
            out[va] = ((off, end), max(n, 1))
    return out


def identity(module_path):
    from codebreaker.verify import oracle as V
    from codebreaker.build import module16
    from codebreaker.decompile import decompiler
    digest = hashlib.sha1(repr(("ne", REV, decompiler.REV, V._ORACLE_REV, module16.CFLAGS, vectors()))
                          .encode()).hexdigest()[:12]
    return V._mod_sha(module_path) or "", digest


def key(seg, off):
    return "%d:%04x" % (seg, off)


def run(module_path, fs=None, log=print):
    """Reconstruct and judge a 16-bit module. -> (module hash, profile digest)."""
    from codebreaker.build import module16, watcom
    from codebreaker.decompile import c16, decompiler
    from codebreaker.formats.ne import NEFile
    from codebreaker.jobs import pool
    from codebreaker.store import functions
    fs = fs or functions.open()
    sha, digest = identity(module_path)
    name = os.path.basename(module_path).lower()
    t0 = time.time()
    work = watcom._scratch("reconstruct16", name)
    m = c16.compilable(module_path, work)
    fs.add_module(sha, name, module_path, "ne")
    fs.add_profile(digest, "16-bit: decompiler %s, flags %s" % (decompiler.REV, " ".join(module16.CFLAGS)))
    keys = {va: key(*m.where[va]) for va in m.by_va}
    fs.add_functions(sha, sorted(keys.values()))
    fs.put_text(sha, SOURCE, "header", m.header)
    sig = hashlib.sha1((m.header + "".join(m.texts[n] for n in m.order)).encode("utf-8", "replace")).hexdigest()
    known = fs.judged(sha, digest, SOURCE)
    fresh = [va for va in sorted(m.by_va) if (known.get(keys[va]) or {}).get("sig") != sig]
    rows = []
    if fresh:
        dll, placed, dropped = module16.build(m.header, m.texts, m.order, m.imports, work,
                                              segments=m.segments)
        nef = NEFile(module_path)
        auto = nef.h.get("autodata_seg")
        data_image = b""
        if auto:
            s = nef.segments()[auto - 1]
            data_image = bytes(nef.data[s["file_off"]:s["file_off"] + s["length"]]) if s["file_off"] else b""
        spans = _spans(nef, {va: m.where[va] for va in m.by_va})
        items = []
        for va in fresh:
            fn = m.by_va[va]
            if fn in dropped or fn not in placed:
                rows.append(dict(addr=keys[va], sig=sig, name=fn, source=m.texts[fn], verdict="no-compile",
                                 reason=dropped.get(fn, "") or "not in the build", problem="compile"))
                continue
            seg, off = m.where[va]
            span, insns = spans[va]
            wide = None if fn in m.novalue else fn in m.wide
            items.append((keys[va], seg, off, fn in m.near, wide, span, insns) + tuple(placed[fn]))
        by_key = {keys[va]: m.by_va[va] for va in fresh}
        judged = pool.run(_judge, items, context=(module_path, dll, data_image), crashed=lambda item: NOT_RUN)
        for k, (verdict, reason, limits, detail) in judged.items():
            fn = by_key[k]
            if verdict in ("equivalent", "consistent"):
                # arguments to Windows functions are not compared yet, and a function that calls
                # a rejected one ran against a stand-in
                extra = {"argc-unknown"} if "(" in m.texts[fn] and any(i + "(" in m.texts[fn] for i in m.imports) else set()
                if any(d + "(" in m.texts[fn] for d in dropped):
                    extra.add("stub-callee")
                limits = sorted(set(limits) | extra)
            rows.append(dict(addr=k, sig=sig, name=fn, source=m.texts[fn], verdict=verdict, reason=reason,
                             limits=",".join(limits), detail=detail))
    fs.record_many(sha, SOURCE, str(decompiler.REV), digest, rows)
    c = fs.tally(digest, sha)
    log("%s: %d functions, %d judged now: %s  (%.0fs)" % (
        name, len(keys), len(fresh), ", ".join("%s %d" % (functions.TITLE[k], c[k]) for k in functions.ORDER if c[k]),
        time.time() - t0))
    return sha, digest
