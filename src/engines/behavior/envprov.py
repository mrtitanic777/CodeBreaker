#!/usr/bin/env python3
r"""envprov.py -- environment provisioner: make env-dependent functions execute instead of faulting.

A minority of functions fault under a bare harness because they assume OS state that isn't there --
an initialized global that holds a pointer, a valid handle, a state struct. They read a null global,
dereference or CALL through it, and fault, so every engine declines them. This provisions plausible
state and retries: each global the function READS is seeded with a pointer to a valid scratch object,
and those objects (and a safe landing) are filled so a dereference-or-call chain resolves to a clean
`ret` instead of a null fault. If the function then runs to completion, it is reconstructed (via the
interproc emitter) on the provisioned trace and tagged env-provisioned.
"""
from src.engines.behavior import tracer, interproc as ip
from src.engines.behavior.globals import _is_global

_OBJ = 0x206000        # provisioned-object region (mapped scratch 0x200000-0x210000)
_SAFE = 0x207800       # safe landing: a `ret`, so a call through a provisioned pointer returns


def reconstruct(module, func=None, arity=4):
    va = int(func, 0) if isinstance(func, str) else int(func)
    _, va0, _code, _sec, _funcs, _sha, _imp = tracer._load(module)
    try:
        base = tracer.trace_function(module, func=hex(va), args=[0] * arity, insn_cap=8000)
    except Exception as e:
        return {"reconstructed": False, "reason": "trace error: %r" % e}
    if base.get("exit") == "ret":
        return {"reconstructed": False, "reason": "returns cleanly (not env-dependent)"}
    g_reads = sorted({m["addr"] for m in base.get("mem_reads", []) if _is_global(m["addr"])})
    if not g_reads:
        return {"reconstructed": False, "reason": "no globals to provision"}

    # Provision: each read-global holds a pointer to its own scratch object; each object is filled
    # with pointers to the safe landing (so a deref or an indirect call resolves cleanly); the safe
    # landing is a `ret`.
    mem = [(_SAFE, b"\xc3" * 0x20)]
    for i, g in enumerate(g_reads):
        obj = _OBJ + i * 0x100
        mem.append((g, obj.to_bytes(4, "little")))
        mem.append((obj, (_SAFE.to_bytes(4, "little")) * 0x40))
    try:
        tr = tracer.trace_function(module, func=hex(va), args=ip._SENT[:arity], stub_externals=True,
                                   stub_ret_unique=True, mem_init=mem, insn_cap=8000)
    except Exception as e:
        return {"reconstructed": False, "reason": "provisioned trace error: %r" % e}
    if tr.get("exit") not in ("ret", "truncated"):
        return {"reconstructed": False, "reason": "still faults after provisioning (%s)" % tr.get("exit")}
    r = ip.emit_from_trace(tr, va, va0, arity, module=module)
    if r.get("reconstructed"):
        r["env_provisioned"] = True
        r["provisioned_globals"] = len(g_reads)
    return r
