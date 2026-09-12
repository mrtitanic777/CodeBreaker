#!/usr/bin/env python3
r"""arity.py -- pin a function's parameter count by consensus across all its call sites.

A single caller only shows how many arguments IT passes -- which may be fewer than the callee
takes, or wrong if the trace under-discovered them. But the whole program is evidence: count the
arguments pushed at EVERY call site of an internal function and take the consensus, cross-checked
against the callee-clean `ret N` byte count. That pins the true parameter count that any one caller
can only guess, so the reconstructed signature has the right number of parameters.
"""
from src.engines.behavior import tracer

_BOUNDARY = {"call", "ret", "retn", "jmp", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge",
             "jb", "jbe", "ja", "jae", "js", "jns", "leave", "int3"}
_BETWEEN = {"mov", "lea", "xor", "nop", "movzx", "movsx"}     # arg-setup insns allowed between pushes


def _pushes_before(rows, k):
    """Count the run of argument pushes immediately preceding rows[k] (a call)."""
    n = 0
    for j in range(k - 1, -1, -1):
        mn = rows[j][1]
        if mn == "push":
            n += 1
        elif mn in _BETWEEN:
            continue                                          # value computed into a reg, then pushed
        else:
            break
    return n


def _esp_args(rows, k):
    """Fallback: distinct [esp+N] argument stores since the previous call (esp-relative arg passing)."""
    import re
    offs = set()
    for j in range(k - 1, -1, -1):
        mn, ops = rows[j][1], rows[j][2]
        if mn in ("call",) or rows[j][1] in _BOUNDARY and mn != "mov":
            break
        m = re.search(r"\[esp(?:\s*\+\s*(0x[0-9a-f]+|\d+))?\]", ops)
        if mn == "mov" and m and ops.split(",")[0].strip().endswith("]"):
            offs.add(int(m.group(1), 0) if m.group(1) else 0)
    return len(offs)


def consensus(module, va):
    """{'argc','sites','votes','retn_argc'} pinning the parameter count, or None."""
    try:
        import capstone
        from src.engines.behavior import callgraph, callconv
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        g = callgraph.build(module)
    except Exception:
        return None
    off = va - va0
    callers = [o for o in g if off in g[o]["calls"]]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    votes = {}
    for co in callers:
        rows = [(i.address, i.mnemonic, i.op_str, i.size)
                for i in md.disasm(code[co:funcs[co]["end"]], va0 + co)]
        for k, (a, mn, ops, sz) in enumerate(rows):
            if mn == "call" and ops.startswith("0x"):
                try:
                    if int(ops, 16) - va0 != off:
                        continue
                except ValueError:
                    continue
                argc = _pushes_before(rows, k) or _esp_args(rows, k)
                votes[argc] = votes.get(argc, 0) + 1
    retn = None
    try:
        cc = callconv.detect(module, va)
        retn = cc.get("stack_args") if cc else None
    except Exception:
        pass
    if not votes:
        return {"argc": retn, "sites": 0, "votes": {}, "retn_argc": retn} if retn is not None else None
    # consensus: the most-voted count (ties -> the larger, since under-counting is the common error)
    best = max(votes, key=lambda a: (votes[a], a))
    if retn is not None and retn in votes:                    # ret N agrees with a site -> trust it
        best = retn
    return {"argc": best, "sites": sum(votes.values()), "votes": votes, "retn_argc": retn}
