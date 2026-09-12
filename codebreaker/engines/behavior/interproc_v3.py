#!/usr/bin/env python3
r"""interproc_v3.py -- bottom-up, whole-module interprocedural reconstruction.

interproc reconstructs ONE function, and its internal `sub_x(...)` calls stay anonymous because
the callee has not been looked at yet. v3 fixes the order: it reconstructs a module in
callgraph reverse-topological order (callees before callers), building a summary for each function
as it goes -- a proposed name (from its own API fingerprint, else propagated up from the names its
callees already earned), its calling convention, and its stack-arg count. When a caller is then
reconstructed, its internal callees already have names, so `sub_1a2b(...)` is emitted as
`/* CreateComObject */ sub_1a2b(...)` -- the orchestration reads as what it orchestrates.

The summary table is the deliverable; interproc.emit_from_trace consumes its `names` map so the
labels appear inline. Names are hypotheses (tagged), never treated as recovered symbols.
"""
from codebreaker.engines.behavior import tracer, callgraph, callconv, funcname, interproc as ip


def _api_set(module, off, va0, arity=4):
    """The set of external API names a function calls (for fingerprint naming)."""
    try:
        tr = tracer.trace_function(module, func=hex(va0 + off), args=ip._SENT[:arity],
                                   stub_externals=True, insn_cap=8000)
    except Exception:
        return set(), []
    apis = {c["api"].split("!")[-1] for c in tr.get("calls", []) if c.get("api")}
    internal = [c for c in tr.get("calls", []) if not c.get("api")
                and (c.get("resolved_target") or c.get("target"))]
    return apis, internal


def _propagate_name(apis, internal_offs, names):
    """Name a function: its own API fingerprint first, else borrow a distinctive callee's name."""
    direct = funcname.suggest_from_apis(apis)
    if direct:
        return direct
    # no direct fingerprint -- if it has exactly one named internal callee, it is likely a wrapper
    named = [names[o] for o in internal_offs if o in names and names[o]]
    if len(set(named)) == 1 and named:
        return named[0] + "_wrapper"
    return None


def reconstruct_module(module, limit=None):
    """Bottom-up pass over a module. Returns {off: summary} where summary has name/conv/argbytes."""
    _, va0, _code, _sec, funcs, _sha, _imp = tracer._load(module)
    order = callgraph.analyze(module)["recon_order"]                 # callees before callers
    if limit:
        order = order[:limit]
    summaries = {}
    names = {}                                                       # off -> proposed name (so far)
    for off in order:
        apis, internal = _api_set(module, off, va0)
        internal_offs = []
        for c in internal:
            t = c.get("resolved_target") or c.get("target")
            if t is not None:
                internal_offs.append(t - va0 if t >= va0 else t)
        nm = _propagate_name(apis, internal_offs, names)
        cc = None
        try:
            cc = callconv.detect(module, va0 + off)
        except Exception:
            pass
        summaries[off] = {"name": nm, "conv": (cc or {}).get("conv"),
                          "argbytes": (cc or {}).get("argbytes"),
                          "stack_args": (cc or {}).get("stack_args"),
                          "apis": sorted(apis), "ncallees": len(internal_offs)}
        if nm:
            names[off] = nm
    return summaries, names


def name_map(module, limit=None):
    """Just the {off: proposed_name} map -- what emit_from_trace wants to label internal calls."""
    return reconstruct_module(module, limit=limit)[1]
