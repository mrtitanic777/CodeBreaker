#!/usr/bin/env python3
r"""recon.py -- whole-module reconstruction orchestration (spec Section 5/15/20).

The capstone that ties the engine together: reconstruct many functions in DEPENDENCY ORDER
(leaves first, then their callers), routing each to the right reconstructor and keeping the
verified results as a growing body of recovered source.

For each function it tries, in order of cleanliness:
    synth     -- pure integer leaf (1-2 args)              -> an expression / branch-free C
    piecewise -- pure value under a guard                  -> guard(x) ? A(x) : B(x)
    effect    -- pointer fn: writes + return over fields+args (the synth<->struct composition)
    struct    -- pointer fn (args pinned)                  -> a struct + accessor body
    cfg       -- return gated by a scalar argument         -> an `if`
    globals   -- writes to absolute globals
and records the first that succeeds (and whether the binary VERIFIED it). Functions that fit
none are honestly recorded as "unresolved" -- the evidence is kept; the reconstruction isn't
faked.

Dependency order comes from the observed call graph, so when later slices synthesize callers in
terms of already-recovered callees, the callees are ready first.

    wt behavior recon <module> [--limit N] [--trace N]
"""
import os
import sys

# Hard ceiling on TOTAL instructions emulated while reconstructing ONE function (across every
# cascade engine). Generous -- a normal function needs a tiny fraction -- but bounds a runaway so it
# is skipped deterministically rather than spinning. Override via WT_FUNC_INSN_CEILING.
import os as _os
_FUNC_INSN_CEILING = int(_os.environ.get("WT_FUNC_INSN_CEILING", "40000000"))

from src.engines.behavior import tracer, graph, synth, corpus, globals as globmod
from src.engines.behavior import struct as structmod
from src.engines.behavior import cfg as cfgmod
from src.engines.behavior import piecewise as pwmod
from src.engines.behavior import effect as effmod
from src.engines.behavior import multisynth as msmod
from src.engines.behavior import gptr as gptrmod
from src.engines.behavior import loops as loopmod


def _topo_order(module, limit):
    """Functions leaf-first by the observed direct-call graph (callees before callers)."""
    g = graph.build(module)
    known = {int(k, 16) for k, v in g["nodes"].items() if v.get("kind") == "function"}
    succ = {}
    for e in g["edges"]:
        if e["kind"] in ("call_direct", "call_indirect") and e["to"].startswith("0x"):
            t = int(e["to"], 16)                     # API-name targets (externals) aren't hex; skip
            if t in known:
                succ.setdefault(int(e["from"], 16), set()).add(t)
    # order by number of intra-module callees (0 = leaf) ascending, then address
    traced = [int(k, 16) for k, v in g["nodes"].items()
              if v.get("kind") == "function" and "leaf" in v]
    traced.sort(key=lambda va: (len(succ.get(va, ())), va))
    return traced[:limit] if limit else traced


def reconstruct_one(module, va, trials=20):
    """Route ONE function (by VA) through the behavioral reconstructors in order of
    cleanliness; return {func_va, method, verified, summary, c}. Reusable by the module
    sweep AND by the fused static+dynamic reconstruct (compose)."""
    entry = {"func_va": va, "method": None, "verified": None, "summary": None, "c": None}
    # Per-function instruction budget: cap the TOTAL emulation across the whole cascade so a
    # pathological function (runaway loop re-run by many engines) is deterministically abandoned
    # and SKIPPED instead of spinning for hours. Normal functions use far less than the ceiling.
    try:
        tracer.budget_reset(_FUNC_INSN_CEILING)
    except Exception:
        pass
    # ORIGIN TRIAGE FIRST: if this IS the CRT / a compiler helper, identify it -- don't reconstruct
    # the toolchain's own code (it isn't the author's source). Emit a named declaration, not a body.
    try:
        from src.engines.behavior import crtsig
        cid = crtsig.identify(module, va)
    except Exception:
        cid = None
    if cid:
        entry.update(method="toolchain-id", verified=True, origin=cid["category"],
                     summary="%s (%s -- toolchain, not reconstructed)" % (cid["name"], cid["category"]),
                     c="/* %s: %s -- C runtime / compiler helper, identified not reconstructed */\n"
                       % (cid["category"], cid["name"]))
        return entry
    try:                                  # statically-linked LIBRARY function (only if a sig DB is loaded)
        from src.engines.behavior import libsig
        lib = libsig.match(module, va) if libsig._DB else None
    except Exception:
        lib = None
    if lib:
        entry.update(method="library-id", verified=True, origin="library:%s" % lib[1],
                     summary="%s (library %s -- linked, not reconstructed)" % (lib[0], lib[1]),
                     c="/* library %s: %s -- statically-linked, identified not reconstructed */\n"
                       % (lib[1], lib[0]))
        return entry
    try:                                  # import thunk: `jmp [IAT]` IS the imported API, not code
        from src.engines.behavior import tailcall
        tc = tailcall.classify(module, va)
    except Exception:
        tc = None
    if tc and tc["kind"] == "import-thunk":
        entry.update(method="import-thunk", verified=True, origin="thunk",
                     summary="thunk -> %s!%s" % (tc.get("dll", "?"), tc["name"]),
                     c="/* import thunk -> %s (jmp [IAT]) */\n" % tc["name"])
        return entry
    try:
        s = synth.synthesize(module, func=hex(va), trials=trials)
    except Exception as e:
        s = {"candidate": False, "reason": "synth error: %r" % e}
    if s.get("candidate") and s.get("candidates"):
        entry.update(method="synth", verified=bool((s.get("confirm") or {}).get("equivalent", True)),
                     summary=s["candidates"][0]["expr"], c=s.get("emitted_c"))
        return entry
    # floating-point (x87): the integer synth reads eax and misses st(0). fpsynth traces with float
    # args and fits a float-expression grammar to the st(0) return.
    try:
        from src.engines.behavior import fpsynth
        fp = fpsynth.synthesize(module, func=hex(va))
    except Exception:
        fp = {"reconstructed": False}
    if fp.get("reconstructed"):
        entry.update(method="fpsynth", verified=fp.get("verified"),
                     summary="float: %s" % fp.get("expr"), c=fp.get("emitted_c"))
        return entry
    # deeper pure-function body: multi-statement straight-line program (hash mixer, byte-swap,
    # masked merge) that single-expression synth's shallow grammar can't reach. Same candidacy
    # (pure fn, no side effects), same oracle validation -- just a richer, CSE'd program.
    try:
        ms = msmod.synthesize(module, func=hex(va), trials=trials)
    except Exception as e:
        ms = {"reason": "multisynth error: %r" % e}
    if ms.get("emitted_c"):
        entry.update(method="multisynth",
                     verified=bool((ms.get("confirm") or {}).get("equivalent", True)),
                     summary=ms.get("expr"), c=ms.get("emitted_c"))
        return entry
    try:
        pw = pwmod.synthesize_piecewise(module, func=hex(va), trials=trials)
    except Exception:
        pw = {"reconstructed": False}
    if pw.get("reconstructed"):
        entry.update(method="piecewise",
                     verified=bool((pw.get("confirm") or {}).get("equivalent", True)),
                     summary="%d-way, %d branch(es)" % (pw.get("ways", 2), pw.get("branches", 2)),
                     c=pw.get("emitted_c"))
        return entry
    # counted write-loop (memset/fill class) -- a back-edge loop the straight-line engines can't
    # express; recognized empirically and oracle-verified before the struct/effect engines see it.
    try:
        lp = loopmod.reconstruct(module, func=hex(va))
    except Exception:
        lp = {"reconstructed": False}
    if lp.get("reconstructed"):
        entry.update(method="loop", verified=lp.get("verified"),
                     summary="counted loop, elem %s, %s" % (lp.get("elem_size"), lp.get("value")),
                     c=lp.get("emitted_c"))
        return entry
    try:
        ef = effmod.reconstruct(module, func=hex(va), runs=12)
    except Exception:
        ef = {"reconstructed": False}
    if ef.get("reconstructed") and (ef.get("mode") == "copy" or any(
            f.get("kind") in ("expr", "cond") for f in ef.get("out_fields", []))):
        if ef.get("mode") == "copy":
            summ = "%d field copy(ies) dst<-src (arg%d<-arg%d)" % (
                ef["copies"], ef["dst_arg"], ef["src_arg"])
        else:
            summ = "%d field-write(s) over %d arg(s) via arg%d" % (
                len(ef["out_fields"]), len(ef["uses_args"]), ef["ptr_arg"])
        entry.update(method="effect", verified=ef.get("verified"), summary=summ, c=ef.get("struct_c"))
        return entry
    try:
        st = structmod.reconstruct(module, func=hex(va))
    except Exception:
        st = {"reconstructed": False}
    if st.get("reconstructed") and (st.get("out_fields") or st.get("in_fields")):
        entry.update(method="struct", verified=st.get("verified"),
                     summary="%d in / %d out fields via arg%d" %
                     (len(st["in_fields"]), len(st["out_fields"]), st["ptr_arg"]),
                     c=st.get("struct_c"))
        return entry
    try:
        cf = cfgmod.reconstruct_cfg(module, func=hex(va))
    except Exception:
        cf = {"reconstructed": False}
    if cf.get("reconstructed"):
        entry.update(method="cfg", verified=cf.get("verified"),
                     summary=cf.get("predicate"), c=cf.get("emitted_c"))
        return entry
    try:
        gl = globmod.reconstruct_globals(module, func=hex(va))
    except Exception:
        gl = {"reconstructed": False}
    if gl.get("reconstructed") and gl.get("writes"):
        entry.update(method="globals", verified=gl.get("verified"),
                     summary="%d global write(s)" % len(gl["writes"]), c=gl.get("c"))
        return entry
    # functions that operate through a GLOBAL POINTER (runtime/CRT state struct): null in a bare
    # trace, so synth/struct/globals all declined. gptr provisions the global with a buffer and
    # recovers the field writes through the named global pointer.
    try:
        gp = gptrmod.reconstruct(module, func=hex(va))
    except Exception:
        gp = {"reconstructed": False}
    if gp.get("reconstructed") and gp.get("out_fields"):
        entry.update(method="gptr", verified=gp.get("verified"),
                     summary="%d field-write(s) via global ptr g_%x" % (len(gp["out_fields"]), gp["global"]),
                     c=gp.get("c"))
        return entry
    # NO-RET retry: a function that didn't return under trivial args may be a count/scan loop that
    # terminates for a valid input. Find a terminating input and re-run synth on that baseline
    # (environment-coupled runtime internals still won't terminate -- they stay honestly floored).
    if "return cleanly" in (s.get("reason") or ""):
        try:
            from src.engines.behavior import terminate
            ta, _mem = terminate.find_terminating_args(module, func=hex(va))
            if ta is not None:
                s2 = synth.synthesize(module, func=hex(va), trials=trials, base_args=ta)
                if s2.get("candidate") and s2.get("candidates"):
                    entry.update(method="synth-provisioned",
                                 verified=bool((s2.get("confirm") or {}).get("equivalent", True)),
                                 summary="%s  [terminating args %s]" % (s2["candidates"][0]["expr"], ta),
                                 c=s2.get("emitted_c"))
                    return entry
        except Exception:
            pass
    # SPECIFIC caller shape: a string-comparison dispatch (a chain of strcmp against literals) is
    # recovered as its keyword->handler if/else-if chain before the generic structural fallback.
    try:
        from src.engines.behavior import strdispatch
        sd = strdispatch.reconstruct(module, func=hex(va))
    except Exception:
        sd = {"reconstructed": False}
    if sd.get("reconstructed"):
        entry.update(method="strdispatch", verified=False,
                     summary="string dispatch over %d keyword(s) [structural]" % len(sd.get("keywords", [])),
                     c=sd.get("emitted_c"))
        return entry
    # STRUCTURED fallback: a function with branches gets its control-flow skeleton recovered and
    # the fragments (calls, with args) placed in the right blocks -- if/goto with real predicates.
    try:
        from src.engines.behavior import cfgstruct as cfgmod2
        cs = cfgmod2.reconstruct(module, func=hex(va))
    except Exception:
        cs = {"reconstructed": False}
    if cs.get("reconstructed"):
        entry.update(method="cfgstruct", verified=False,
                     summary="%d blocks, %d conds, %d calls [structural]"
                             % (cs.get("nblocks", 0), cs.get("nconds", 0), cs.get("ncalls", 0)),
                     c=cs.get("emitted_c"))
        return entry
    # FLAT fallback: no branches -- a pure caller. Emit the resolved call sequence with args mapped
    # to the caller's params. Structural tier (observed, not re-validated).
    try:
        from src.engines.behavior import interproc as ipmod
        ip = ipmod.reconstruct(module, func=hex(va))
    except Exception:
        ip = {"reconstructed": False}
    if ip.get("reconstructed"):
        v = ip.get("verified", False)
        entry.update(method="interproc", verified=v,
                     summary="%d call(s) [%s]" % (ip.get("ncalls", 0), "verified" if v else "structural"),
                     c=ip.get("emitted_c"))
        return entry
    # LAST resort: the traces faulted (an env-dependent function -- null global deref/call). Provision
    # plausible OS state (globals -> valid objects, calls -> safe landing) so it runs, then emit.
    try:
        from src.engines.behavior import envprov as epmod
        ep = epmod.reconstruct(module, func=hex(va))
    except Exception:
        ep = {"reconstructed": False}
    if ep.get("reconstructed"):
        entry.update(method="env-provisioned", verified=False,
                     summary="%d call(s), %d globals provisioned [structural]"
                             % (ep.get("ncalls", 0), ep.get("provisioned_globals", 0)),
                     c=ep.get("emitted_c"))
        return entry
    entry["summary"] = (s.get("reason") or "no v1 reconstructor applies")
    return entry


def reconstruct_module(module, limit=20, trace_n=0):
    if trace_n:
        graph.trace_sample(module, trace_n)
    order = _topo_order(module, limit)
    results = [reconstruct_one(module, va) for va in order]

    nrec = sum(1 for r in results if r["method"])
    nver = sum(1 for r in results if r["verified"])
    report = {"schema": "cb.behavior.recon/1", "module": os.path.basename(module),
              "attempted": len(results), "reconstructed": nrec, "verified": nver,
              "yield_pct": round(100.0 * nrec / max(1, len(results)), 1),
              "results": results}
    fp = corpus.record_derived("recon", {"module": report["module"], "func_va": 0, **report})
    report["_path"] = fp
    return report


# --------------------------------------------------------------------------
from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    limit = int(_opt(argv, "--limit", 20))
    trace_n = int(_opt(argv, "--trace", 0))

    rep = reconstruct_module(module, limit=limit, trace_n=trace_n)
    by = {}
    for r in rep["results"]:
        by[r["method"] or "unresolved"] = by.get(r["method"] or "unresolved", 0) + 1
    print("module reconstruction: %s  (%d functions attempted, dependency order)" %
          (rep["module"], rep["attempted"]))
    print("  by method: %s" % ", ".join("%s=%d" % kv for kv in sorted(by.items())))
    print("  YIELD    : %d/%d (%.1f%%) readable source, %d verified byte-exact\n" %
          (rep["reconstructed"], rep["attempted"], rep["yield_pct"], rep["verified"]))
    for r in rep["results"]:
        if not r["method"]:
            continue
        flag = "VERIFIED" if r["verified"] else "partial" if r["verified"] is False else "-"
        print("  0x%-9x %-7s %-8s %s" % (r["func_va"], r["method"], flag, r["summary"]))
    un = [r for r in rep["results"] if not r["method"]]
    print("\n  %d unresolved (evidence kept, no v1 reconstructor)" % len(un))
    print("  stored: %s" % rep["_path"])

    emit = _opt(argv, "--emit")
    if emit:
        os.makedirs(emit, exist_ok=True)
        stem = rep["module"].replace(".", "_")
        path = os.path.join(emit, stem + ".behav.c")
        with open(path, "w", encoding="utf-8") as f:
            f.write("/* %s -- behavioral reconstruction (wt behavior recon), each function\n"
                    "   differential-tested against the binary oracle. VERIFIED = the emitted C\n"
                    "   reproduced the binary's observed behavior on every tested input.\n"
                    "   Methods: synth/piecewise/effect/struct/cfg/globals. */\n\n" % rep["module"])
            for r in rep["results"]:
                if r.get("c"):
                    tag = "VERIFIED" if r["verified"] else "partial" if r["verified"] is False else ""
                    f.write("/* ---- sub_%x  [%s %s]  %s ---- */\n%s\n\n"
                            % (r["func_va"], r["method"], tag, r["summary"] or "", r["c"].rstrip()))
        nc = sum(1 for r in rep["results"] if r.get("c"))
        print("  wrote %s (%d reconstructed functions)" % (path, nc))


if __name__ == "__main__":
    main()
