#!/usr/bin/env python3
r"""tailcall.py -- identify import thunks and tail calls (so they aren't mis-reconstructed).

Two small but common shapes the other engines mishandle:
  * an IMPORT THUNK is a one-instruction function, `jmp dword ptr [IAT slot]` -- it IS the imported
    API, not code to reconstruct; naming it hands callers the real API and removes a fake body.
  * a TAIL CALL ends a function with `jmp <another function>` instead of `call; ret`; cfgstruct
    emits that terminal jump as a `goto` to nowhere, when it means `return f(args);`.
This classifies both from the disassembly + the PE import table, so recon can name a thunk instead
of reconstructing it and the emitters can render a tail call as a return.
"""
from codebreaker.engines.behavior import tracer

_JMP = ("jmp",)


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, imports = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    rows = [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]
    return rows, va0, funcs, imports


def _abs_ptr(ops):
    """The absolute address in a `dword ptr [0xADDR]` operand, or None."""
    import re
    m = re.search(r"\[(0x[0-9a-f]+)\]", ops)
    return int(m.group(1), 0) if m else None


def classify(module, va):
    """{'kind':'import-thunk','name':...} | {'kind':'tail-call','target':...,'name':...} | None."""
    try:
        rows, va0, funcs, imports = _rows(module, va)
    except Exception:
        return None
    if not rows:
        return None
    # import thunk: a lone `jmp dword ptr [IAT]`
    real = [r for r in rows if r[1] != "int3" and not (r[1] == "nop")]
    if len(real) == 1 and real[0][1] == "jmp":
        p = _abs_ptr(real[0][2])
        if p is not None and p in imports:
            dll, nm = imports[p]
            return {"kind": "import-thunk", "name": nm, "dll": dll}
    # tail call: the function's last instruction is an unconditional jmp to another function start
    last = rows[-1]
    if last[1] == "jmp" and last[2].startswith("0x"):
        try:
            tgt = int(last[2], 16)
        except ValueError:
            tgt = None
        if tgt is not None:
            toff = tgt - va0
            here = va - va0
            in_self = any(r[0] == tgt for r in rows)         # a jump within this function is a loop
            if not in_self and toff in funcs and toff != here:
                return {"kind": "tail-call", "target": tgt, "name": "sub_%x" % toff}
        p = _abs_ptr(last[2])                                 # tail call THROUGH an import
        if p is not None and p in imports:
            return {"kind": "tail-call", "target": p, "name": imports[p][1]}
    return None
