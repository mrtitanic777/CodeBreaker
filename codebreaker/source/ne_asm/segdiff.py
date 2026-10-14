"""
segdiff.py — the whole-segment byte-match harness (byte-exact stage F).

Turns "is this reconstructed C byte-exact?" into an automated verdict. The hard
part (learned in the 3.1 project): you cannot match a function in isolation,
because a near call/jmp to a *neighbouring* function is a self-relative
displacement that only becomes concrete when the neighbour is in the SAME
compiland. So this harness compiles a GROUP of functions together (their mutual
calls resolve to real displacements instead of masked relocations), then diffs
against the shipped bytes.

Comparison is mask-aware on BOTH sides: our side masks OMF FIXUPP sites
(objextract); the shipped side masks NE relocation sites (ne.relocations,
following the fixup chains). Absolute addresses legitimately differ between
builds, so masking them is what makes a true opcode-stream match possible.

    python segdiff.py --selftest                       # prove the mechanism
    python segdiff.py recon.c --module KRNL386 --seg 1 --off 0x1234 --func _Foo
                                                        # verdict vs real bytes
"""
import argparse
import sys

from codebreaker.build import msvc152 as cc16
from codebreaker.invert import omf as objextract

# NE relocation source-type -> width in bytes of the relocated field
def _masm():
    from codebreaker.build import masm
    return masm


def _segdiff():
    from codebreaker.source.ne_asm import segdiff
    return segdiff


def _segment():
    from codebreaker.source.ne_asm import segment
    return segment


def _msvc152():
    from codebreaker.build import msvc152
    return msvc152


class _Corpus(object):
    """What the old target registry answered: where a module of the corpus is."""
    @staticmethod
    def resolve(module):
        from codebreaker.base import corpus
        return corpus.resolve(module) or corpus.retail(module) or module


def _target():
    return _Corpus


NE_SRC_WIDTH = {0: 1, 2: 2, 3: 4, 5: 2, 11: 6}


# ---------------------------------------------------------------------------
# our side: compile a group, get each function's bytes + fixup mask
# ---------------------------------------------------------------------------
def compile_group(csrc, flags="/AM /Ox /c"):
    r = cc16.compile_c(csrc, flags=flags)
    if "obj" not in r:
        raise RuntimeError("compile produced no OBJ:\n" + r.get("log", "")[:600])
    return objextract.Obj(r["obj"]).functions()


# ---------------------------------------------------------------------------
# shipped side: pull a segment's bytes + a relocation mask from an NE module
# ---------------------------------------------------------------------------
def shipped_ne_segment(module_path, seg_index):
    from codebreaker.formats import ne as ne_mod
    ne = ne_mod.NEFile(module_path)
    seg = ne.segments()[seg_index - 1]
    code = ne.data[seg["file_off"]:seg["file_off"] + seg["length"]]
    mask = set()
    # The masked set MUST match linkfile's NE relocation restore pass exactly: the
    # rebuild leaves shipped bytes only at the sites linkfile restores, so any byte
    # masked here but NOT restored there is a false "already-matching" that lets a
    # mis-disassembled table entry stay live and then diff in the real rebuild.
    # linkfile requires the WHOLE fixup site (w bytes) to fit in the segment before
    # visiting it (`cur + size <= length`); the old bound here (`cur < len(code)-1`)
    # only required the 2-byte next-link to fit, so a record whose site overran the
    # segment end was still followed, over-masking a whole chain of interior sites
    # linkfile never touches. Use linkfile's bound so the two agree.
    for rec in ne.relocations(seg):
        w = NE_SRC_WIDTH.get(rec["src"], 2)
        off = rec["offset"]
        if rec.get("additive"):
            if off + w <= len(code):
                for k in range(w):
                    mask.add(off + k)
            continue
        seen = set()                      # non-additive: follow the fixup chain
        cur = off
        while cur != 0xFFFF and 0 <= cur and cur + w <= len(code) and cur not in seen:
            seen.add(cur)
            for k in range(w):
                mask.add(cur + k)
            if cur + 1 >= len(code):
                break                     # can't read the 2-byte next-link
            cur = code[cur] | (code[cur + 1] << 8)
    return code, mask


# ---------------------------------------------------------------------------
# the differ
# ---------------------------------------------------------------------------
def diff(our, omask, shipped, smask):
    """Compare two byte strings ignoring masked positions on either side."""
    skip = omask | smask
    n = min(len(our), len(shipped))
    first = None
    ndiff = 0
    for i in range(n):
        if i in skip:
            continue
        if our[i] != shipped[i]:
            ndiff += 1
            if first is None:
                first = i
    return {
        "exact": (len(our) == len(shipped) and ndiff == 0),
        "length_match": len(our) == len(shipped),
        "our_len": len(our), "shipped_len": len(shipped),
        "n_diff": ndiff, "first_diff": first,
        "masked": len(skip),
    }


def render(res, our=None, shipped=None, omask=set(), smask=set()):
    v = "BYTE-EXACT" if res["exact"] else (
        "length ok, content differs" if res["length_match"] else "length differs")
    out = ["verdict: %s" % v,
           "  our=%d shipped=%d bytes, masked=%d, diffs=%d%s"
           % (res["our_len"], res["shipped_len"], res["masked"], res["n_diff"],
              (", first@0x%x" % res["first_diff"]) if res["first_diff"] is not None else "")]
    if not res["exact"] and our is not None and res["first_diff"] is not None:
        i = res["first_diff"]
        lo = max(0, i - 4)
        window = range(lo, min(len(our), len(shipped), i + 8))
        skip = omask | smask
        o = " ".join(("--" if k in skip else "%02x" % our[k]) for k in window)
        s = " ".join(("--" if k in skip else "%02x" % shipped[k]) for k in window)
        out += ["  @0x%x ours    : %s" % (lo, o),
                "  @0x%x shipped : %s" % (lo, s)]
    return "\n".join(out)


# ---------------------------------------------------------------------------
# selftest: prove the mechanism without needing a retail anchor
# ---------------------------------------------------------------------------
GROUP = r"""
int helper(int x)        { return x + 1; }
int caller(int y)        { return helper(y) * 2 + helper(y + 1); }
"""


def _selftest():
    print("segdiff selftest\n")
    ok = True

    # (a) round-trip: a group matches itself exactly
    g = compile_group(GROUP)
    cb, cm = g["_caller"]
    res = diff(cb, cm, cb, cm)
    print("(a) round-trip caller vs itself:", render(res).splitlines()[0])
    ok = ok and res["exact"]

    # (b) mutation is detected and located
    g2 = compile_group(GROUP.replace("* 2 +", "* 3 +"))
    cb2, cm2 = g2["_caller"]
    res2 = diff(cb2, cm2, cb, cm)
    print("(b) mutated caller vs original:", render(res2).splitlines()[0],
          "" if not res2["exact"] else "(BUG: should differ)")
    ok = ok and not res2["exact"]

    # (c) whole-compiland resolution: compiling the callee IN the group makes
    #     caller's call to it CONCRETE (fewer masked bytes) than when the callee
    #     is external. This is the reason isolation fails.
    grouped_masked = len(cm)
    alone = compile_group("extern int helper(int);\n"
                          "int caller(int y){return helper(y)*2+helper(y+1);}\n")
    _ab, am = alone["_caller"]
    print("(c) caller masked bytes: grouped=%d, callee-external=%d  -> %s"
          % (grouped_masked, len(am),
             "grouping resolves the calls" if grouped_masked < len(am) else "??"))
    ok = ok and grouped_masked < len(am)

    print("\nSELFTEST:", "PASS" if ok else "FAIL")
    return ok


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?", help="reconstructed C (a function group)")
    ap.add_argument("--module"); ap.add_argument("--seg", type=int)
    ap.add_argument("--off"); ap.add_argument("--size", type=lambda x: int(x, 0))
    ap.add_argument("--func", help="which function in the group to check")
    ap.add_argument("--flags", default="/AM /Ox /c")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        sys.exit(0 if _selftest() else 1)

    if not (args.csrc and args.module and args.seg is not None and args.off and args.func):
        ap.error("need csrc + --module --seg --off --func (or --selftest)")
    target = _target()
    path = target.resolve(args.module)
    off = int(args.off, 0)
    g = compile_group(open(args.csrc).read(), flags=args.flags)
    if args.func not in g:
        sys.exit("function %s not in the compiled group; have: %s"
                 % (args.func, ", ".join(g)))
    our, omask = g[args.func]
    seg_bytes, seg_mask = shipped_ne_segment(path, args.seg)
    size = args.size or len(our)
    shipped = seg_bytes[off:off + size]
    smask = {m - off for m in seg_mask if off <= m < off + size}
    res = diff(our, omask, shipped, smask)
    print(render(res, our, shipped, omask, smask))
    sys.exit(0 if res["exact"] else 1)


if __name__ == "__main__":
    main()
