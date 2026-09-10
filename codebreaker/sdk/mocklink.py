#!/usr/bin/env python3
r"""mocklink.py -- segment-level verification for needs-link functions (roadmap 2b).

A "needs-link" function contains an intra-segment call (`e8` near call, or the linker's
`push cs; call near` far-call optimization). It can't be verified in isolation because
that call's encoding + rel16 come from the whole-segment layout, not the function's own
C. This verifies it anyway: recompile the candidate WITH near stub definitions for its
sibling functions in the SAME module -- so the compiler emits the same intra-segment
`e8` near call -- then mask the layout-assigned rel16 (msc6fn._mask now zeroes `e8`
operands) and compare against the shipped bytes.

Two intra-segment call forms exist and this covers both:
  * bare `e8` near call         -- sibling is a NEAR function (declare it `near pascal`);
  * `push cs; call near` (0e e8) -- sibling is a FAR function the linker collapsed because
    the target landed in the same segment. mc6_compile already links to an image, so
    declaring the sibling `far pascal` in the SAME module makes the linker emit exactly
    that `push cs; call near` -- no external LINK pass, no byte surgery.
We try whichever form the shipped bytes indicate first, then the other as a fallback.

    from codebreaker.sdk import mocklink
    mocklink.verify(target_bytes, candidate_c)   # {'exact': True/False, 'recipe': ...}
"""
import os, re, sys
from codebreaker.inverse_engine import msc6_oracle as ora
from codebreaker.inverse_engine.msc6fn import _mask, EPI

# /G2 = 286 (push imm8); plain /Gsw = 8086 (sub ax,ax;push ax;push ax). Shipped DLLs mix both.
RECIPES = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /Gsw /Ox", "/AM /Gsw /Os")


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
    """Recompile the candidate with sibling stubs so its intra-segment calls take the same
    encoding the shipped code uses, mask the layout-assigned rel16, and compare to `target`.

    Siblings are pascal (callee-cleanup) so the caller pushes left-to-right and emits no
    `add sp` -- matching the shipped form. They are declared NEAR (bare `e8` near call) or
    FAR (linker collapses the same-segment far call to `push cs; call near`). The shipped
    bytes say which: a `0e e8` means the far-call optimization, so try FAR first."""
    target = bytes(target)
    names = re.findall(r"extern int far (\w+)\(\);", candidate_c)
    if not names:
        return {"exact": None, "reason": "not a needs-link function (no sibling extern)"}
    c = candidate_c
    for n in names:                                       # drop the far externs
        c = c.replace("extern int far %s();\n" % n, "")
    a = _norm(target)
    NEAR = ("near", "int near pascal %s();\n", "int near pascal %s(){ return 0; }\n")
    FAR = ("far", "int far pascal %s();\n", "int far pascal %s(){ return 0; }\n")
    kinds = [FAR, NEAR] if b"\x0e\xe8" in target else [NEAR, FAR]
    results = []
    for kind, decl, defn in kinds:
        fwd = "".join(decl % n for n in names)
        stubs = "".join(defn % n for n in names)          # placed AFTER F
        combined = fwd + c + "\n" + stubs
        for rcp in recipes:
            try:
                rc = ora.mc6_compile(combined, rcp)
            except Exception as e:
                results.append(("%s/%s" % (kind, rcp), str(e))); continue
            fb = _first_func(rc)                          # F is emitted first
            if _norm(fb) == a:
                return {"exact": True, "recipe": "%s-siblings %s" % (kind, rcp), "bytes": fb.hex()}
            results.append(("%s/%s" % (kind, rcp), "%d vs %d B" % (len(_norm(fb)), len(a))))
    return {"exact": False, "results": results}


if __name__ == "__main__":
    import struct
    from codebreaker.format_loaders import ne
    from codebreaker.sdk import reconstruct as R
    from codebreaker.sdk import w95paths
    OLE = w95paths.input_dll("WINDOWS", "SYSTEM", "olecli.dll")
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
