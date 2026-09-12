#!/usr/bin/env python3
"""
segdiff_ppc.py — the PowerPC byte-match harness (the segdiff.py analogue).

Turns "is this reconstructed C byte-exact against the shipped Xbox 360 code?"
into an automated verdict, so the permuter can search for a match. Same shape as
the Win95 `segdiff`, three differences that come from the platform:

  * **compile** with the XDK PowerPC cl.exe (ppc_cc) instead of MSVC 1.52c.
  * **our-side mask** is the compiled object's PPC relocations, at BIT
    granularity (ppc_obj) — a relocated `bl`/`lis`/`addi` still has its opcode
    and registers compared; only the address/displacement field is ignored.
  * **shipped side** comes from the decoded XEX image (xex.py) — a linked PE, so
    there are no per-function reloc records; the same mask (from our object) is
    applied to both sides, which is what makes an opcode-stream match provable.

Interface-compatible with `segdiff` (compile_group / diff return the same keys),
so `permute.py --arch ppc` drives this unchanged.

    python segdiff_ppc.py --selftest
    python segdiff_ppc.py recon.c --xex default.xex --vaddr 0x820FBC58 --size 0x40 --func caller
"""
import argparse
import sys

import ppc_cc
import ppc_obj

BIG = 10 ** 9


# ---------------------------------------------------------------------------
# our side: compile a group, get each function's bytes + bit-level reloc mask
# ---------------------------------------------------------------------------
def compile_group(csrc, flags="/c /O2"):
    r = ppc_cc.compile_c(csrc, flags=flags)
    if "obj" not in r:
        raise RuntimeError("compile produced no OBJ:\n" + r.get("log", "")[:600])
    return ppc_obj.Obj(r["obj"]).functions()


# ---------------------------------------------------------------------------
# shipped side: pull a function's bytes from the decoded XEX image
# ---------------------------------------------------------------------------
def shipped_xex_function(xex_path, vaddr, size):
    """Bytes of the function at virtual address `vaddr` (length `size`) from the
    decoded XEX image. `vaddr` is an image virtual address (e.g. 0x820FBC58)."""
    import xex
    x = xex.open_xex(xex_path)
    img = x.basefile()
    base = x.image_base or 0
    # basefile is the loaded image -> flat by virtual address (see disasm_ppc).
    for s in xex._pe_sections(img):
        sec_va = base + s["vaddr"]
        if sec_va <= vaddr < sec_va + s["raw_size"]:
            off = s["vaddr"] + (vaddr - sec_va)     # NOT raw_off (stale on-disk)
            return img[off:off + size]
    raise ValueError("vaddr 0x%X not in any section of %s" % (vaddr, xex_path))


# ---------------------------------------------------------------------------
# the differ — mask is {byte_offset: ignore_bitmask} on EITHER side
# ---------------------------------------------------------------------------
def _merge(omask, smask):
    m = dict(omask or {})
    for k, v in (smask or {}).items():
        m[k] = m.get(k, 0) | v
    return m


def diff(our, omask, shipped, smask):
    """Compare two byte strings ignoring the masked BITS at each offset."""
    mask = _merge(omask, smask)
    n = min(len(our), len(shipped))
    first = None
    ndiff = 0
    masked_bytes = 0
    for i in range(n):
        m = mask.get(i, 0)
        if m:
            masked_bytes += 1
        if (our[i] & ~m) != (shipped[i] & ~m):
            ndiff += 1
            if first is None:
                first = i
    return {
        "exact": (len(our) == len(shipped) and ndiff == 0),
        "length_match": len(our) == len(shipped),
        "our_len": len(our), "shipped_len": len(shipped),
        "n_diff": ndiff, "first_diff": first,
        "masked": masked_bytes,
    }


def render(res, our=None, shipped=None, omask=None, smask=None):
    v = "BYTE-EXACT" if res["exact"] else (
        "length ok, content differs" if res["length_match"] else "length differs")
    out = ["verdict: %s" % v,
           "  our=%d shipped=%d bytes, masked=%d, diffs=%d%s"
           % (res["our_len"], res["shipped_len"], res["masked"], res["n_diff"],
              (", first@0x%x" % res["first_diff"]) if res["first_diff"] is not None else "")]
    if not res["exact"] and our is not None and res["first_diff"] is not None:
        mask = _merge(omask or {}, smask or {})
        i = res["first_diff"]
        lo = max(0, (i & ~3) - 4)
        hi = min(len(our), len(shipped), i + 8)
        o = " ".join(("%02x" % (our[k] & ~mask.get(k, 0))) for k in range(lo, hi))
        s = " ".join(("%02x" % (shipped[k] & ~mask.get(k, 0))) for k in range(lo, hi))
        out += ["  @0x%x ours    : %s" % (lo, o),
                "  @0x%x shipped : %s" % (lo, s)]
    return "\n".join(out)


# ---------------------------------------------------------------------------
# selftest: prove the mechanism with the real XDK compiler, no XEX needed
# ---------------------------------------------------------------------------
GROUP = r"""
extern int g_table[8];
int helper(int x) { return x + 1; }
int caller(int y) { return helper(y) * 2 + g_table[y & 7]; }
"""


def _selftest():
    print("segdiff_ppc selftest (XDK PowerPC compiler)\n")
    if not ppc_cc.available():
        print("XDK compiler not found (%s) — set: XboxTranspiler config xdk <path>"
              % ppc_cc.paths()["cl"])
        return False
    ok = True

    g = compile_group(GROUP)
    print("compiled group, functions:", ", ".join(sorted(g)))
    cb, cm = g["caller"]

    # (a) a function matches itself exactly
    res = diff(cb, cm, cb, cm)
    print("(a) round-trip caller vs itself :", render(res).splitlines()[0])
    ok = ok and res["exact"]

    # (b) a real code change is detected
    g2 = compile_group(GROUP.replace("* 2 +", "* 3 +"))
    cb2, cm2 = g2["caller"]
    res2 = diff(cb2, cm2, cb, cm)
    print("(b) mutated caller vs original  :", render(res2).splitlines()[0],
          "(BUG: should differ)" if res2["exact"] else "")
    ok = ok and not res2["exact"]

    # (c) the mask ignores the ADDRESS bits but NOT the opcode: change only the
    #     external's *address usage* shape and confirm masking behaves. Here we
    #     prove a differently-based build (same opcodes) still matches once the
    #     REFHI/REFLO immediates are masked — by zeroing those bytes in a copy.
    cb_relinked = bytearray(cb)
    for off in cm:                     # scribble the masked (address) bytes
        cb_relinked[off] ^= 0xFF
    res3 = diff(bytes(cb_relinked), cm, cb, cm)
    print("(c) address bits differ, opcodes same:", render(res3).splitlines()[0],
          "" if res3["exact"] else "(mask should have absorbed it)")
    ok = ok and res3["exact"]

    print("\nSELFTEST:", "PASS" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?")
    ap.add_argument("--xex")
    ap.add_argument("--vaddr", type=lambda x: int(x, 0))
    ap.add_argument("--size", type=lambda x: int(x, 0))
    ap.add_argument("--func")
    ap.add_argument("--flags", default="/c /O2")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        sys.exit(0 if _selftest() else 1)
    if not (args.csrc and args.xex and args.vaddr and args.func):
        ap.error("need csrc + --xex --vaddr --func (or --selftest)")

    g = compile_group(open(args.csrc).read(), flags=args.flags)
    if args.func not in g:
        sys.exit("function %s not in group; have: %s" % (args.func, ", ".join(g)))
    our, omask = g[args.func]
    size = args.size or len(our)
    shipped = shipped_xex_function(args.xex, args.vaddr, size)
    res = diff(our, omask, shipped, {})
    print(render(res, our, shipped, omask, {}))
    sys.exit(0 if res["exact"] else 1)


if __name__ == "__main__":
    main()
