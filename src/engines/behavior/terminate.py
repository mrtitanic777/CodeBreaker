#!/usr/bin/env python3
r"""terminate.py -- terminating-input discovery for the no-ret reconstruction class.

Some functions don't return under the default all-zero args: a count loop runs 2^32 times
(`while(n--)` with n=0), or a scanner walks a null/uninitialised buffer forever. Traced with
trivial args they floor as "does not return cleanly", so every reconstructor declines. But with
a VALID input -- a small count, a short NUL-terminated scratch buffer -- they return in a few
iterations and become reconstructable by the ordinary lanes.

This finds such an input vector (args + mem_init) by trying a small, ordered strategy set and
returning the first that yields a clean return. It does NOT rescue environment-coupled runtime
internals (e.g. the C runtime's heap/locale guts that need the whole CRT initialised) -- those
honestly stay no-ret. Used by recon to retry the lanes on a terminating baseline.
"""
from src.engines.behavior import tracer

SCR = 0x201000                                   # scratch buffer inside the mapped scratch region
_BUF = (SCR, b"ab\x00" + b"\x00" * 252)          # short NUL-terminated string for scan loops


def _rets(module, func, args, mem, insn_cap):
    try:
        t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap, mem_init=mem)
        return t["exit"] == "ret"
    except Exception:
        return False


def find_terminating_args(module, func, arity=4, insn_cap=6000):
    """Return (args, mem_init) that make `func` return cleanly, or (None, None). Ordered cheapest-
    first: small scalars, then per-slot small counts, then per-slot scratch-buffer pointers."""
    arity = max(1, min(arity, 6))
    strategies = [[1] * arity, [2] * arity, [4] * arity, [8] * arity]
    for i in range(arity):                       # one small count in slot i
        a = [0] * arity; a[i] = 4; strategies.append(a)
    cand = [(a, None) for a in strategies]
    for i in range(arity):                       # a scratch buffer (NUL-terminated) in slot i
        a = [0] * arity; a[i] = SCR; cand.append((a, [_BUF]))
    for i in range(arity):                       # buffer + a small count in the next slot
        a = [0] * arity; a[i] = SCR
        if i + 1 < arity:
            a[i + 1] = 4
        cand.append((a, [_BUF]))
    cand.append(([SCR] * arity, [_BUF]))
    for args, mem in cand:
        if _rets(module, func, args, mem, insn_cap):
            return args, mem
    return None, None
