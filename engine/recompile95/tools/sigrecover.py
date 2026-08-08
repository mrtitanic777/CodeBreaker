#!/usr/bin/env python3
r"""sigrecover.py -- recover real function arities (rung 2c), so the fn-ptr cast shims drop.

rc.adapt(link=True) routes every call through `((u32(*)())sub_X)(...)` because the readable
lift's call/def arities disagree. That cast MARKS where true signatures are owed. This module
recovers them from the byte-exact asm, which carries the ground truth the C lost:

  * __stdcall / __fastcall callees clean their own stack -> terminate with `ret N`; the arity
    is N/4 dwords. This is the dominant Win32 convention and the most reliable signal.
  * cdecl / near callees `ret` (0); their arity comes from the caller's push count (the lift
    already recovers that at the call site) -- not decided here.

arities(module) -> {func_offset: nargs} for every function whose terminal `ret N` fixes it.
A call `sub_<off>(a, b, ...)` whose arg count == arities()[off] can drop the cast and use a
real prototype `u32 sub_<off>(u32, ...)`; a mismatch keeps the cast (the honest marker).

    python sigrecover.py --measure kernel32.dll user32.dll   # opportunity report
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
_ENGINE = os.path.dirname(os.path.dirname(HERE))                 # the shared core: engine/
sys.path.insert(0, os.path.join(_ENGINE, "targets", "win95"))
sys.path.insert(0, _ENGINE)
from engine.foundation import layout

# terminal stack-cleanup: `ret 8` (decimal) / `ret 10h` (hex) ... -> N bytes popped = args*4.
# capture the digits AND whether an 'h' suffix marks hex (MASM: bare = decimal, trailing h = hex).
_RET_N = re.compile(r"^\s*ret[dfn]?\s+([0-9a-fA-F]+)(h?)\b", re.M)
_RET_0 = re.compile(r"^\s*ret[dfn]?\s*(?:;|$)", re.M)


def _meta(fname):
    m = re.match(r"(?:se[cg]|obj)(\d+)_([0-9a-fA-F]+)", fname)
    return int(m.group(2), 16) if m else None


def arities(module):
    """{func_offset: nargs} from each per-function asm's terminal `ret N` (stdcall cleanup).
    Functions ending in bare `ret` are omitted (arity is a caller-side question)."""
    d = layout.asm_module_dir(module)
    out = {}
    for f in sorted(glob.glob(os.path.join(d, "*.asm"))):
        off = _meta(os.path.basename(f))
        if off is None:
            continue
        body = open(f, encoding="latin1").read()
        ms = _RET_N.findall(body)                        # [(digits, 'h'|''), ...]
        if ms:
            digits, h = ms[-1]                           # last cleanup ret fixes the convention
            popped = int(digits, 16) if h else int(digits)
            if 0 < popped <= 256 and popped % 4 == 0:
                out[off] = popped // 4
        elif _RET_0.search(body):
            out[off] = None                              # returns, but caller-cleaned (unknown here)
    return out


def measure(module):
    """Coverage: of the module's carved functions, how many get a stdcall arity from `ret N`
    (the reliable, callee-side signal). The remainder either `ret` (caller-cleaned -- arity is a
    call-graph question, see below) or never cleanly return."""
    ar = arities(module)
    stdcall = {o: n for o, n in ar.items() if n is not None}
    caller_cleaned = sum(1 for v in ar.values() if v is None)
    total = len(glob.glob(os.path.join(layout.asm_module_dir(module), "*.asm")))
    return dict(total=total, stdcall=len(stdcall), caller_cleaned=caller_cleaned)


def main():
    if "--measure" in sys.argv:
        mods = [a for a in sys.argv[1:] if not a.startswith("--")]
        tot = dict(total=0, stdcall=0, caller_cleaned=0)
        for m in mods:
            r = measure(m)
            for k in tot:
                tot[k] += r[k]
            print("  %-16s funcs=%-5d stdcall-arity=%-5d (%.0f%%)  caller-cleaned=%-4d"
                  % (m, r["total"], r["stdcall"],
                     100 * r["stdcall"] / (r["total"] or 1), r["caller_cleaned"]))
        print("TOTAL: %d/%d functions got a stdcall arity from `ret N` (%.0f%%); "
              "%d more are caller-cleaned (arity from the call graph -- next step)."
              % (tot["stdcall"], tot["total"],
                 100 * tot["stdcall"] / (tot["total"] or 1), tot["caller_cleaned"]))


if __name__ == "__main__":
    main()
