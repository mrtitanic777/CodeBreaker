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
from codebreaker.core import layout

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


def sdk_arities(module):
    """{func_offset: (api_name, sdk_arity)} for this module's NAMED exports whose arity the
    period-SDK knows. The export table (ground truth) gives name<->offset; symdb gives the
    published arity (HYPOTHESIS-tier). Offsets are made section-relative (export RVA minus the
    code section's RVA) so they live in the SAME space as arities()/rc.py's def offsets.
    Empty if the module has no name exports or the SDK isn't loaded."""
    from codebreaker.engines.symbol import names, symdb
    try:
        ne, _exp = names.export_names(module)
    except Exception:
        return {}
    if not hasattr(ne, "exports"):                       # NE/16-bit: no RVA space to rebase
        return {}
    try:
        cs = ne.code_sections()
        base = (cs[0][0] - ne.image_base) if cs else 0   # code section RVA (VA - image_base)
    except Exception:
        base = 0
    out = {}
    for o, n, rva in ne.exports():
        if not n:
            continue
        a = symdb.arity(n)
        if a is None:
            continue
        out[rva - base] = (n, a)
    return out


def crosscheck(module):
    """Compare ret-N arities (byte truth) against the SDK's published arities at the same offset.
    Returns dict(agree, disagree, diffs=[(off,name,retN,sdk)], sdk_only=[(off,name,sdk)]).
    Corroboration only -- ret N always wins; a disagreement flags an export thunk or a
    lift/carve boundary, never a reason to change a byte-verified arity."""
    rn = arities(module)
    sdk = sdk_arities(module)
    agree = disagree = 0
    diffs, fillable, unjoined = [], [], []
    for off, (name, a) in sdk.items():
        if off in rn and rn[off] is not None:            # ret N has a positive arity here
            if rn[off] == a:
                agree += 1
            else:
                disagree += 1
                diffs.append((off, name, rn[off], a))
        elif off in rn:                                  # off IS a carved def, but caller-cleaned
            fillable.append((off, name, a))              #   -> a real cdecl export the SDK fills
        else:
            unjoined.append((off, name, a))              # export RVA didn't land on a carved def
    return dict(agree=agree, disagree=disagree, diffs=diffs,
                fillable=fillable, unjoined=unjoined)


def arities_plus(module):
    """Signature-recovery arities with the SDK folded in, doctrine-ordered:
      positive ret N (byte truth)  >  SDK published arity (hypothesis-tier).
    Returns (merged {off:nargs}, report). A POSITIVE ret-N arity is NEVER overwritten by the
    SDK. The SDK only fills offsets where ret N gave nothing usable -- either absent, or a
    byte-verified caller-cleaned `ret` (None = arity unknowable from the callee side, exactly
    the cdecl-export gap the SDK is here to close). report carries the cross-check for honest
    surfacing (agreement corroborates; a disagreement flags a thunk/boundary, ret N still wins)."""
    merged = dict(arities(module))
    sdk = sdk_arities(module)
    filled = 0
    for off, (_name, a) in sdk.items():
        # Fill ONLY a byte-verified caller-cleaned def (off present, value None): that offset
        # provably IS a carved function, and ret N left its arity unknowable (the cdecl-export
        # gap). Do NOT invent keys for absent offsets -- an export RVA that doesn't land on a
        # carved def start (the common case: 32-bit exports are stdcall, already fixed by ret N)
        # would be a phantom entry no def consumes, inflating the count dishonestly.
        if off in merged and merged[off] is None:
            merged[off] = a
            filled += 1
    rep = crosscheck(module)
    rep["filled"] = filled
    return merged, rep


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
        return
    if "--sdk" in sys.argv:                              # SDK cross-check + fill report
        mods = [a for a in sys.argv[1:] if not a.startswith("--")]
        ta = td = tf = tu = 0
        for m in mods:
            r = crosscheck(m)
            tf += len(r["fillable"]); tu += len(r["unjoined"])
            ta += r["agree"]; td += r["disagree"]
            print("  %-16s SDK-joined: agree=%-4d disagree=%-3d  cdecl-fill=%-3d  unjoined=%d"
                  % (m, r["agree"], r["disagree"], len(r["fillable"]), len(r["unjoined"])))
            for off, name, rn, sdk in r["diffs"][:6]:
                print("       DIFF @%05x %-24s retN=%-2d sdk=%-2d  (thunk/boundary; ret N wins)"
                      % (off, name, rn, sdk))
        print("TOTAL SDK cross-check: %d agree, %d disagree (ret N authoritative); "
              "%d caller-cleaned exports the SDK fills (hypothesis-tier); "
              "%d export RVAs didn't land on a carved def." % (ta, td, tf, tu))
        return


if __name__ == "__main__":
    main()
