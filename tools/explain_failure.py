#!/usr/bin/env python3
"""explain_failure.py -- show exactly how a rebuilt function differs from the original.

    python tools/explain_failure.py comctl32.dll sub_01AD15 [--trace]

Runs the original and the embedded core's rebuilt function on the first input where the verifier
saw a difference, and prints side by side: how each ended, what it returned, the API calls with
their arguments, and the memory each wrote (address, size, value) -- marking what differs.
`--trace` adds the rebuilt side's last instructions.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    from src.core import corpus
    from src.engines.behavior import tracer
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B, verify95 as V
    from src.targets.win95.verification import ghidra_core, pipeline, store
    pipeline._arm()
    module, fn = os.path.basename(argv[0]).lower(), argv[1]
    path = corpus.retail(module)
    pe = open_pe(path)
    d = B._scratch("pe-core", module)
    dlls = sorted(glob.glob(os.path.join(d, "g*.dll")), key=os.path.getmtime)
    if not dlls:
        raise SystemExit("no core build for %s" % module)
    dll = dlls[-1]
    pb = open_pe(dll)
    ex = pb.export_addrs()
    names = ghidra_core.function_names(path, pipeline.functions("pe", path))
    va = next((v for v, n in names.items() if n == fn), None)
    if va is None or fn not in ex:
        raise SystemExit("%s: not a known function / not in the build" % fn)
    row = store.open().db.execute("SELECT verdict, reason, detail FROM body WHERE lane='pe-core' AND module=? "
                                  "AND fname=?", (module, fn)).fetchone()
    print("%s %s @ %08x  recorded: %s" % (module, fn, va, row))
    vecs = V._vectors(4, "pe") + V._confirm_vectors(4, "pe")
    cov, rch = set(), set()
    for v in vecs:
        f = V._fp_pe(path, va, v)
        cov |= set(f.get("cov") or ())
        rch |= set(f.get("reach") or ())
    vecs += V.directed_vectors(path, va, cov, reached=rch)
    own = V._image_span(dll)
    for v in vecs:
        a = V._fp_pe(path, va, v)
        b = V._fp_pe(dll, pb.image_base + ex[fn], v, rebuilt=path)
        if "err" in a or "err" in b or not V.first_difference(a, b):
            continue
        print("input %s  api-return=%x  memory=%s  -> %s" % (
            [hex(x) for x in v["args"]], v.get("stub_ret", 0), "provisioned" if v.get("mem_init") else "empty",
            V.first_difference(a, b)))
        res = {}
        for side, p, f, ov in (("original", path, va, None), ("rebuilt", dll, pb.image_base + ex[fn], path)):
            res[side] = tracer.trace_function(p, func=f, args=v["args"], mem_init=v.get("mem_init"),
                                              stub_externals=True, sig_resolver=V._argc,
                                              stub_ret=int(v.get("stub_ret", 0)), overlay=ov)
        for side in ("original", "rebuilt"):
            r = res[side]
            print("  %-8s ends: %-22s returns: %s" % (side, r.get("exit"), hex(r.get("ret") or 0)))
            print("  %-8s ebx,esi,edi,ebp on return: %s   stack moved: %s" % (
                "", " ".join("%08x" % x for x in r.get("saved_regs") or ()), r.get("esp_delta")))
        ca = [(x[0].split("!")[-1], x[1]) for x in res["original"].get("api_calls") or []]
        cb = [(x[0].split("!")[-1], x[1]) for x in res["rebuilt"].get("api_calls") or []]
        for i in range(max(len(ca), len(cb))):
            x = ca[i] if i < len(ca) else None
            y = cb[i] if i < len(cb) else None
            fmt = lambda c: "-" if c is None else "%s(%s)" % (c[0], ",".join("%x" % q for q in (c[1] or [])))
            same = x is not None and y is not None and x[0] == y[0] and [
                V._norm_arg(q, v["args"]) for q in (x[1] or [])] == [V._norm_arg(q, v["args"], own) for q in (y[1] or [])]
            print("  call %-2d %s %-60s | %s" % (i, " " if same else "*", fmt(x)[:60], fmt(y)[:60]))
            if i > 14:
                break

        def final(r, o):
            st = {}
            for w in r.get("mem_writes") or []:
                ad, n, val = w["addr"], w["size"], w["value"] or 0
                if V._STACK_LO <= ad < V._STACK_HI or (o and o[0] <= ad < o[1]):
                    continue
                for k in range(min(n, 8)):
                    st[ad + k] = (val >> (8 * k)) & 0xFF
            return st
        wa, wb = final(res["original"], None), final(res["rebuilt"], own)
        diff = sorted(k for k in set(wa) | set(wb) if wa.get(k) != wb.get(k))
        print("  memory: original wrote %d bytes, rebuilt %d; %d bytes differ" % (len(wa), len(wb), len(diff)))
        runs, start, prev = [], None, None
        for k in diff:
            if start is None or k != prev + 1:
                if start is not None:
                    runs.append((start, prev))
                start = k
            prev = k
        if start is not None:
            runs.append((start, prev))
        for lo, hi in runs[:10]:
            fa = " ".join("%02x" % wa[k] if k in wa else "--" for k in range(lo, min(hi, lo + 15) + 1))
            fb = " ".join("%02x" % wb[k] if k in wb else "--" for k in range(lo, min(hi, lo + 15) + 1))
            print("    %08x..%08x  original: %s" % (lo, hi, fa))
            print("    %s  rebuilt:  %s" % (" " * 18, fb))
        if "--trace" in argv:
            ks = list(res["rebuilt"]["mnemonics"].items())
            for k, mn in (ks if "--all" in argv else ks[-24:]):
                print("      rebuilt %s %s" % (k, mn))
        return 0
    print("no differing input found now (the build may have changed since it was judged)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
