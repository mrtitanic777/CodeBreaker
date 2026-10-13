#!/usr/bin/env python3
r"""stackframe.py -- recover a function's stack frame: size + typed local variables.

cfgstruct names `[ebp-N]` accesses `local_N` inline but never declares them. This recovers the
frame: the frame size from the `sub esp, N` prologue, and every `[ebp-N]` slot the body touches,
sized by the widest access (byte/word/dword) -- so the reconstructed function can open with real
local declarations (`unsigned local_8; unsigned short local_c; unsigned char buf_10;`).
"""
import re
from codebreaker.emulate import trace32 as tracer

_WIDTH = {"byte": 1, "word": 2, "dword": 4}
_LOCAL = re.compile(r"(byte|word|dword) ptr \[ebp\s*-\s*(0x[0-9a-f]+|\d+)\]")
_CTYPE = {1: "unsigned char", 2: "unsigned short", 4: "unsigned"}


def recover(module, va):
    """{'framesize': int, 'locals': [(offset, size)]} or None."""
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        fend = next((f["end"] for o, f in sorted(funcs.items()) if o == off), None)
        if fend is None:
            nx = sorted(o for o in funcs if o > off); fend = nx[0] if nx else off + 0x400
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = list(md.disasm(code[off:fend], va))
    except Exception:
        return None
    if not rows:
        return None
    framesize = 0
    for ins in rows[:10]:                                    # prologue: sub esp, <framesize>
        if ins.mnemonic == "sub" and ins.op_str.replace(" ", "").startswith("esp,"):
            try:
                framesize = int(ins.op_str.split(",")[1].strip(), 0)
            except ValueError:
                pass
            break
    slots = {}                                               # offset -> max access width
    for ins in rows:
        for m in _LOCAL.finditer(ins.op_str):
            w = _WIDTH[m.group(1)]; o = int(m.group(2), 0)
            slots[o] = max(slots.get(o, 0), w)
    if not slots and not framesize:
        return None
    return {"framesize": framesize, "locals": sorted(slots.items())}


def declarations(module, va):
    """C local declarations for the function's frame, or [] if none recovered."""
    fr = recover(module, va)
    if not fr or not fr["locals"]:
        return []
    return ["    %s local_%x;" % (_CTYPE.get(sz, "unsigned"), o) for o, sz in fr["locals"]]
