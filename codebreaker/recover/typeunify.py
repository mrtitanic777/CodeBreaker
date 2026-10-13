#!/usr/bin/env python3
r"""typeunify.py -- interprocedural type unification: propagate types across the call graph.

interproc's local type propagation types a function's parameters from the SDK calls IT makes
directly. But a function often just forwards its parameter to an INTERNAL helper that is itself
typed (because the helper calls a typed API). This computes a function's parameter types including
that transitive flow: recurse into internal callees (depth-bounded, memoized), and when a caller
passes its parameter aI to a callee argument whose type is known, aI inherits that type.
"""
from codebreaker.emulate import trace32 as tracer
from codebreaker.recover import interproc as ip

_CACHE = {}


def param_types(module, va, arity=4, depth=2):
    """[type-or-None per parameter] for the function, unified across the call graph."""
    key = (module, va, depth)
    if key in _CACHE:
        return _CACHE[key]
    _CACHE[key] = ["unsigned"] * arity                       # guard against recursion cycles
    from codebreaker.symbols import symdb
    _, va0, _c, _s, _f, _sha, _imp = tracer._load(module)
    try:
        tr = tracer.trace_function(module, func=hex(va), args=ip._SENT[:arity],
                                   stub_externals=True, insn_cap=8000)
    except Exception:
        return _CACHE[key]
    types = [None] * arity
    for c in tr.get("calls", []):
        if not (c.get("resolved_target") or c.get("target")):
            continue
        nm = ip._name_target(c, va0)
        if not nm:
            continue
        # callee parameter types: from the SDK proto, or recurse into an internal function
        if c.get("api"):
            _ret, cptypes = ip._sig(symdb, nm)
        elif depth > 0 and c.get("resolved_target") and c["resolved_target"] >= va0:
            cptypes = param_types(module, c["resolved_target"], arity, depth - 1)
        else:
            cptypes = []
        real = ip._call_args(c.get("args_stack") or [], c.get("argc"))
        for p, v in enumerate(real):
            if p >= len(cptypes) or not cptypes[p] or cptypes[p] == "unsigned":
                continue
            for k, s in enumerate(ip._SENT):                 # this caller-param flows into a typed slot
                if v == s and k < arity and types[k] is None:
                    types[k] = cptypes[p]
    result = [t or "unsigned" for t in types]
    _CACHE[key] = result
    return result
