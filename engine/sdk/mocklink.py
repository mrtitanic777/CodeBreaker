#!/usr/bin/env python3
r"""mocklink.py -- segment-level verification for needs-link functions (roadmap 2b).

A "needs-link" function contains an intra-segment call (`e8` near call, or the linker's
`push cs; call near` far-call optimization). It can't be verified in isolation because
that call's encoding + rel16 come from the whole-segment layout, not the function's own
C. This verifies it anyway: recompile the candidate WITH near stub definitions for its
sibling functions in the SAME module -- so the compiler emits the same intra-segment
`e8` near call -- then mask the layout-assigned rel16 (msc6fn._mask now zeroes `e8`
operands) and compare against the shipped bytes.

This covers the bare-`e8` near-call case (OLECLI's needs-link) with NO external linker.
The linker-optimized `push cs; call near` case (far siblings) needs a real LINK pass
and is handled separately.

    from engine.sdk import mocklink
    mocklink.verify(target_bytes, candidate_c)   # {'exact': True/False, 'recipe': ...}
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from engine.inverse_engine import msc6_oracle as ora
from engine.inverse_engine.msc6fn import _mask, EPI

RECIPES = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os")


def _first_func(text):
    """The bytes of the first function in a compiled _TEXT run (up to epilogue+retf)."""
    p = text.find(EPI)
    if p < 0:
        return text
    q = p + len(EPI)
    if text[q:q+1] == b"\xca":
        return text[:q+3]
    if text[q:q+1] in (b"\xcb", b"\xc3"):
        return text[:q+1]
    return text[:q]


def _norm(b):
    return _mask(b).rstrip(b"\x90\x00")


def verify(target, candidate_c, recipes=RECIPES):
    """Recompile the candidate with near sibling stubs so its intra-segment calls become
    the same `e8` near calls, mask the layout-assigned rel16, and compare to `target`."""
    target = bytes(target)
    names = re.findall(r"extern int far (\w+)\(\);", candidate_c)
    if not names:
        return {"exact": None, "reason": "not a needs-link function (no sibling extern)"}
    c = candidate_c
    for n in names:                                       # drop the far externs
        c = c.replace("extern int far %s();\n" % n, "")
    # near + pascal siblings: intra-segment callees clean their own args (callee-cleanup),
    # so the caller pushes left-to-right and emits no `add sp` -- matching the shipped form.
    fwd = "".join("int near pascal %s();\n" % n for n in names)
    stubs = "".join("int near pascal %s(){ return 0; }\n" % n for n in names)  # placed AFTER F
    combined = fwd + c + "\n" + stubs
    a = _norm(target)
    results = []
    for rcp in recipes:
        try:
            rc = ora.mc6_compile(combined, rcp)
        except Exception as e:
            results.append((rcp, str(e))); continue
        fb = _first_func(rc)                              # F is emitted first
        if _norm(fb) == a:
            return {"exact": True, "recipe": rcp, "bytes": fb.hex()}
        results.append((rcp, "%d vs %d B" % (len(_norm(fb)), len(a))))
    return {"exact": False, "results": results}


if __name__ == "__main__":
    import struct
    from engine.format_loaders import ne
    from engine.sdk import reconstruct as R
    OLE = r"C:\Users\mrtit\Documents\GitHub\Windows95\data\files\WINDOWS\SYSTEM\olecli.dll"
    d = open(OLE, "rb").read(); ne_off = struct.unpack_from("<I", d, 0x3c)[0]
    f = ne.NEFile.from_buffer(d, ne_off, OLE)
    rep = R.analyze(OLE)
    nl = [(s["idx"], fn) for s in rep["segments"] for fn in s["funcs"] if fn["status"] == "needs-link"]
    print("OLECLI needs-link functions: %d -- verifying via mock-link:" % len(nl))
    ok = 0
    for segi, fn in nl:
        r = verify(bytes.fromhex(fn["bytes"]), fn["c"])
        ok += bool(r.get("exact"))
        print("  seg%-2d 0x%04x size=%-4d -> %s" % (segi, fn["off"], fn["size"],
              "BYTE-EXACT (%s)" % r["recipe"] if r.get("exact") else r))
    print("mock-link verified: %d / %d needs-link" % (ok, len(nl)))
