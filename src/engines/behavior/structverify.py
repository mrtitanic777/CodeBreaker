#!/usr/bin/env python3
r"""structverify.py -- verification for the structural tiers (interproc / cfgstruct).

interproc emits the call sequence OBSERVED in one trace. When is that the COMPLETE behavior, not
just one path? When the function is single-path: no conditional branches, so every input drives the
same instructions -- the observed call sequence is input-independent and therefore faithful. This
checks branchlessness (allowing unconditional jmp / ret) and upgrades such a reconstruction from
'structural' (observed) to 'verified'. Branchy functions stay structural -- honestly not
over-claimed, since a single trace only fills the path it took.
"""
from src.engines.behavior import tracer, cfgstruct


def is_single_path(module, va):
    """True if the function has no conditional branches (one path -> observed behavior is complete)."""
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        fend = None
        for o, f in sorted(funcs.items()):
            if o == off:
                fend = f["end"]; break
        if fend is None:
            nexts = sorted(o for o in funcs if o > off)
            fend = nexts[0] if nexts else min(off + 0x400, len(code))
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        for ins in md.disasm(code[off:fend], va):
            if ins.mnemonic in cfgstruct._CC:
                return False
        return True
    except Exception:
        return False


def verify(module, va):
    """Verdict for a structural reconstruction: 'verified' if single-path, else 'structural'."""
    return "verified" if is_single_path(module, va) else "structural"
