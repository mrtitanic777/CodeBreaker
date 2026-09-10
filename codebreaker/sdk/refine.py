#!/usr/bin/env python3
r"""refine.py -- oracle-guided byte-exact refinement (Option 2).

The automated inverse (msc6dec) declines the hard functions (buffer/string/far-ptr
code). This tool closes the gap the other way: given the TARGET bytes and a CANDIDATE
C (hand-written, or from a readable decompiler), it compiles the candidate with the
MSC 6.0a oracle, masks relocations, and either confirms byte-exact or LOCALIZES the
first divergence -- the exact byte offset plus a disassembly of what the shipped code
does there vs. what the candidate produced. That turns "rewrite until it matches" into
a tight, pinpointed loop: a human (or an agent) edits one spot and re-runs.

It also tries a few cheap AUTO-transforms first (both recipes, then done) so trivially
different candidates pass without human effort.

    from codebreaker.sdk import refine
    r = refine.check(target_bytes, candidate_c)
    print(r["exact"], r.get("report"))
"""
import os, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from codebreaker.inverse_engine import msc6_oracle as ora
from codebreaker.inverse_engine.msc6fn import _mask

_MD = Cs(CS_ARCH_X86, CS_MODE_16)
# /G2 = 286 codegen (push imm8); the plain /Gsw forms are 8086 (sub ax,ax;push ax;push ax).
# Shipped DLLs mix both, so try both CPU targets before declaring a divergence.
RECIPES = ("/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /Gsw /Ox", "/AM /Gsw /Os")


def _norm(b):
    return _mask(b).rstrip(b"\x90\x00")


def _disasm_at(code, off, ctx=3):
    """Disassemble a window around byte offset `off`, marking the line covering it."""
    start = max(0, off - 8)
    out = []
    for ins in _MD.disasm(bytes(code[start:start + 24]), start):
        mark = ">>" if ins.address <= off < ins.address + ins.size else "  "
        out.append("    %s %04x  %-10s %-8s %s" % (mark, ins.address, ins.bytes.hex(),
                                                   ins.mnemonic, ins.op_str))
        if len(out) >= 2 * ctx:
            break
    return "\n".join(out)


def localize(target, produced):
    """Structured report of the first divergence between two masked byte strings."""
    a, b = _norm(target), _norm(produced)
    if a == b:
        return None
    k = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), min(len(a), len(b)))
    return ("byte %d differs (shipped=%d bytes, candidate=%d)\n"
            "  --- shipped ---\n%s\n  --- candidate ---\n%s"
            % (k, len(a), len(b), _disasm_at(target, k), _disasm_at(produced, k)))


def check(target, candidate_c, recipes=RECIPES):
    """Compile `candidate_c` under each recipe; return byte-exactness + a localized
    divergence report for the closest recipe when not exact."""
    target = bytes(target)
    results = []
    for rcp in recipes:
        try:
            rc = ora.mc6_compile(candidate_c, rcp)
        except Exception as e:
            results.append((rcp, None, str(e))); continue
        if _norm(rc) == _norm(target):
            return {"exact": True, "recipe": rcp, "bytes": rc.hex()}
        results.append((rcp, rc, None))
    # not exact under any recipe -- localize against the recipe that compiled + is closest
    best = None
    for rcp, rc, err in results:
        if rc is None:
            continue
        d = sum(1 for x, y in zip(_norm(rc), _norm(target)) if x != y) + \
            abs(len(_norm(rc)) - len(_norm(target)))
        if best is None or d < best[0]:
            best = (d, rcp, rc)
    if best is None:
        return {"exact": False, "report": "candidate did not compile: %s" % results}
    report = localize(target, best[2])
    diag = _diagnose(report)                              # match against the divergence KB
    return {"exact": False, "recipe": best[1], "distance": best[0],
            "report": report, "diagnosis": diag}


def _diagnose(report):
    """Consult the divergence-pattern KB for a likely cause + known fix of this miss."""
    try:
        from codebreaker.inverse_engine import divergence_kb as kb
    except Exception:
        return []
    return [{"pattern": p["id"], "cause": p["cause"], "fix": p["fix"]}
            for p in kb.diagnose(report or "")]


def _selftest():
    """Prove the loop end-to-end on a real declined function: hand-write candidate C
    for GetExpandedName (seg8:0x122), refine until byte-exact."""
    import struct
    from codebreaker.format_loaders import ne
    from codebreaker.sdk import reconstruct as R
    binp = r"C:\Users\mrtit\Documents\GitHub\Windows95\data\files\setup\inf\lzexpand.dll"
    d = open(binp, "rb").read(); ne_off = struct.unpack_from("<I", d, 0x3c)[0]
    f = ne.NEFile.from_buffer(d, ne_off, binp); segs = f.segments()
    seg = segs[7]; segb = d[seg["file_off"]:seg["file_off"] + seg["length"]]
    e = R._func_end(segb, 0x122, seg["length"]); target = segb[0x122:e]
    # a first candidate -- deliberately close; the loop pinpoints what's off
    cand = r"""
typedef unsigned int WORD;
extern WORD far pascal G();
int far F(char far *src)
{
    char buf[12];
    int i;
    for (i = 0; i < 8; i++)
        buf[i] = src[i];
    buf[i] = 0;
    return G((char far *)buf, (char far *)0x64) == 1;
}
"""
    r = check(target, cand)
    print("exact:", r["exact"])
    if not r["exact"]:
        print("recipe:", r.get("recipe"), "distance:", r.get("distance"))
        print(r["report"])


if __name__ == "__main__":
    _selftest()
