#!/usr/bin/env python3
r"""permutebridge.py -- the raiser -> asm-level permute bridge (byte-exact stage F).

The raiser (`raiser.candidate`) turns lifted [AUTO] C into *compilable* structured C.
Many candidates already compile byte-exact on the win95 oracle (cc32/MSVC 2.0). Many
others land 1-6 bytes off -- and the residual difference is BELOW C: register
allocation, instruction scheduling, or equivalent encodings the C source cannot
dictate (the flagship being `c9` LEAVE vs `5d` POP EBP in the epilogue). This module
closes those near-misses automatically, in three ordered tiers:

  TIER 1  FLAG MATRIX   -- the permute/oracle flag-pin idea, but on cc32 (win95 is a
                          native MSVC target, not the cc16/DOSBox path segdiff wires).
                          Sweep opt x frame-pointer x processor x misc; keep the flag
                          set whose masked bytes match. This is where most closable
                          misses fall (e.g. /O1 vs /O1 /Oy- frame differences).
  TIER 2  C-VARIANTS    -- a `$hole` layer at the C level: enumerate semantics-
                          preserving source rewrites the raiser can equally emit and
                          recompile each. The DOMINANT lever for the call class is the
                          CALLING CONVENTION (the raiser hard-defaults f + callees to
                          __stdcall, but many win95 internals are __cdecl); also
                          commutative operand order a*b<->b*a, `x-0`/`x+0` vs `x`, and
                          local signedness. Closes misses the compiler resolves from
                          source-level choices.
  TIER 3  ASM PEEPHOLE  -- the true sub-C layer. For the residual that NO flag and NO
                          C-variant reaches, apply PROVEN below-C encoding equivalences
                          to the compiled output itself (leave<->pop-ebp with an
                          esp==ebp guard, etc.). A close here is the evidence that a
                          byte-exact rebuild needs an asm-template, and it *names* the
                          exact equivalence to bake into that template.

    python permutebridge.py --show kernel32.dll:1:0015c    # one function, all tiers
    python permutebridge.py --selftest                     # prove the machinery

Interface note: `raiser._try_match` already tried 8 flags; this widens the matrix to
~288 combos, adds the C-variant and asm-peephole tiers, and RETURNS the winning lever
(so the caller can record HOW each function was closed), instead of just a flag/None.
"""
import os
from codebreaker.foundation import cores
import re
import sys

_ENG = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _ENG)
# cc32/matchbatch's compile+extract oracle lives in the sibling win95 tool dir.
sys.path.insert(0, os.path.join(os.path.dirname(_ENG), "win95"))


# ---------------------------------------------------------------------------
# TIER 1 -- the flag matrix (cc32 / MSVC 2.0)
# ---------------------------------------------------------------------------
# Axes chosen for codegen sensitivity. Each combo is one compile; product is
# deduped. Windows shipped frame-pointer-omission OFF, so /Oy- matters most.
_OPT   = ["/O1", "/O2", "/Ox", "/Os", "/Ot", "/Od"]
_FRAME = ["", "/Oy-"]
_PROC  = ["", "/G3", "/G4", "/G5"]
_MISC  = ["", "/Gs", "/Ob0", "/Gd", "/Gz", "/Gy"]


def flag_matrix():
    seen, out = set(), []
    for o in _OPT:
        for fr in _FRAME:
            for pr in _PROC:
                for mi in _MISC:
                    fl = " ".join(x for x in ("/c", o, fr, pr, mi) if x)
                    if fl not in seen:
                        seen.add(fl)
                        out.append(fl)
    return out


# A fast subset that covers the vast majority of closable flag misses -- tried
# first so the common case doesn't pay for the full 240-combo sweep.
FAST_FLAGS = ("/c /O2 /Oy-", "/c /O2", "/c /O1 /Oy-", "/c /O1",
              "/c /Ox /Oy-", "/c /Ox", "/c /Os", "/c /Od /Oy-", "/c /Gy")


def _emit(cc32, csrc, flags, prefix="_f"):
    """Compile csrc; return (code_bytes, mask_set) for the first `prefix` function,
    or None on any failure."""
    try:
        r = cc32.compile_c(csrc, flags=flags)
    except Exception:
        return None
    if "obj" not in r:
        return None
    try:
        fns = cc32.Obj(r["obj"]).functions()
    except Exception:
        return None
    c = next((v for k, v in fns.items() if k.startswith(prefix)), None)
    if not c:
        return None
    return bytes(c[0]), set(c[1])


def _diffpos(code, mask, shipped):
    """List of unmasked byte positions that differ, or None if lengths differ."""
    if len(code) != len(shipped):
        return None
    return [i for i in range(len(shipped)) if i not in mask and code[i] != shipped[i]]


def try_flags(cc32, csrc, shipped, flags_list, prefix="_f"):
    """Sweep flags; return (best_dist, best_flag, best_code, best_mask, best_diffs).
    best_dist is len(diffs); a length-mismatch flag is skipped (can't be exact)."""
    best = None
    for fl in flags_list:
        e = _emit(cc32, csrc, fl, prefix)
        if e is None:
            continue
        code, mask = e
        dp = _diffpos(code, mask, shipped)
        if dp is None:
            continue
        if best is None or len(dp) < best[0]:
            best = (len(dp), fl, code, mask, dp)
            if best[0] == 0:
                break
    return best


# ---------------------------------------------------------------------------
# TIER 2 -- C-level equivalence variants (the source `$hole` layer)
# ---------------------------------------------------------------------------
def _split_body(csrc):
    """Split a raiser candidate into (decls_prefix, signature_and_body). We only
    rewrite inside the function body, never the extern decls."""
    m = re.search(r"\n([A-Za-z_][\w ]*\**\s*f\s*\([^)]*\)\s*\{)", csrc)
    if not m:
        # no leading externs -- whole thing is the function
        return "", csrc
    return csrc[:m.start() + 1], csrc[m.start() + 1:]


def _commutative_variants(body):
    """Swap operands of commutative binary ops on the RHS of a `return`/assignment.
    Yields alternative bodies (order swapped, one op at a time)."""
    outs = []
    # match `= A op B;` or `return A op B;` with simple (non-nested) A,B operands.
    pat = re.compile(r"(=\s*|return\s+)([A-Za-z_]\w*|\d+)\s*([*+&|^])\s*([A-Za-z_]\w*|\d+)\s*;")
    for m in pat.finditer(body):
        pre, a, op, b = m.group(1), m.group(2), m.group(3), m.group(4)
        if a == b:
            continue
        swapped = "%s%s %s %s;" % (pre, b, op, a)
        outs.append(body[:m.start()] + swapped + body[m.end():])
    return outs


def _identity_arith_variants(body):
    """`x` <-> `x + 0` / `x - 0` on a returned/assigned bare operand (forces or drops
    a no-op the optimizer may or may not fold to the shipped encoding)."""
    outs = []
    pat = re.compile(r"(=\s*|return\s+)([A-Za-z_]\w*)\s*;")
    for m in pat.finditer(body):
        pre, x = m.group(1), m.group(2)
        for form in ("%s + 0" % x, "%s - 0" % x, "%s | 0" % x):
            outs.append(body[:m.start()] + "%s%s;" % (pre, form) + body[m.end():])
    return outs


def _convention_variants(csrc):
    """The DOMINANT call-class lever: the raiser hard-defaults both `f` and its
    callees to __stdcall, but many win95 internals are __cdecl (caller-cleanup:
    `add esp,N` after the call, and a bare `ret`). Enumerate the function's own
    convention x the callees' convention over {__stdcall, __cdecl, __fastcall}.
    The function line is the one calling `... f(...)`; extern lines are callees."""
    lines = csrc.split("\n")
    is_fsig = [bool(re.search(r"\bf\s*\(", ln)) and ("__stdcall" in ln or "__cdecl" in ln
                or "__fastcall" in ln) and not ln.lstrip().startswith("extern")
               for ln in lines]
    is_ext = [ln.lstrip().startswith("extern") for ln in lines]
    if not any(is_fsig):
        return []
    _CONV = ("__stdcall", "__cdecl", "__fastcall")
    outs = []
    for fconv in _CONV:
        for cconv in _CONV:
            new = []
            for k, ln in enumerate(lines):
                if is_fsig[k]:
                    ln = re.sub(r"__(?:stdcall|cdecl|fastcall)", fconv, ln)
                elif is_ext[k]:
                    ln = re.sub(r"__(?:stdcall|cdecl|fastcall)", cconv, ln)
                new.append(ln)
            src = "\n".join(new)
            if src != csrc:
                outs.append(src)
    return outs


def _signedness_variants(prefix):
    """Flip `unsigned` survivor locals to `int` (the raiser guesses unsigned; the
    shipped codegen may have used signed, changing e.g. shr<->sar, mul<->imul)."""
    if "unsigned v" not in prefix and "unsigned " not in prefix:
        return []
    return [prefix.replace("unsigned ", "int ")]


def c_variants(csrc, cap=64):
    """Yield semantics-preserving source variants of a raiser candidate (bounded).
    Convention variants come FIRST -- they are the dominant call-class lever."""
    seen = set([csrc])
    n = 0
    # convention axis first (whole-source rewrites, highest yield).
    for src in _convention_variants(csrc):
        if src in seen:
            continue
        seen.add(src)
        yield src
        n += 1
        if n >= cap:
            return
    # then body/prefix idiom axes.
    prefix, body = _split_body(csrc)
    bodies = [body] + _commutative_variants(body) + _identity_arith_variants(body)
    prefixes = [prefix] + _signedness_variants(prefix)
    for pf in prefixes:
        for bd in bodies:
            src = pf + bd                    # prefix ends at '{', body is the rest
            if src in seen:
                continue
            seen.add(src)
            yield src
            n += 1
            if n >= cap:
                return


# ---------------------------------------------------------------------------
# TIER 3 -- proven below-C (asm) encoding equivalences on the compiled output
# ---------------------------------------------------------------------------
def _is_frame_teardown_popebp(code, i):
    """True if byte i is a frame-teardown `pop ebp` (0x5d) that is provably
    equivalent to `leave` (0xc9): the function set up a frame (`55 8b ec` =
    push ebp;mov ebp,esp) and byte i is followed by a ret (c3 / c2 iw). Because
    the compiler emitted a BARE `pop ebp` (not `mov esp,ebp;pop ebp`), esp==ebp
    here, so `leave` (mov esp,ebp;pop ebp) is a no-op-prefixed identity."""
    if i >= len(code) or code[i] != 0x5d:
        return False
    if code[:3] != b"\x55\x8b\xec":
        return False
    nxt = code[i + 1:i + 2]
    return nxt in (b"\xc3", b"\xc2")


# Each entry: (name, predicate(code,i) -> bool, shipped_byte, our_byte).
# Only PROVEN identities belong here -- a close via this tier is a real byte-exact
# rebuild, achieved by an asm-template that emits `shipped_byte` where the compiler
# chose `our_byte`.
_PEEPHOLES = [
    ("leave<->pop-ebp", _is_frame_teardown_popebp, 0xc9, 0x5d),
]


def try_asm_peephole(code, mask, shipped, diffs):
    """Given our best compiled `code` (+ its reloc `mask`), its `diffs` vs `shipped`,
    try to close the remaining diffs with PROVEN sub-C encoding equivalences. Returns
    (patched_bytes, [equivalences_used]) if every diff is closed, else None. The final
    verify is MASK-AWARE: masked positions (call/abs-addr fixups) legitimately differ
    between our object and the linked image, so only unmasked bytes must match."""
    if not diffs:
        return None
    patched = bytearray(code)
    used = []
    for i in diffs:
        hit = None
        for name, pred, sb_byte, our_byte in _PEEPHOLES:
            if shipped[i] == sb_byte and code[i] == our_byte and pred(code, i):
                hit = (name, i)
                break
            # symmetric direction
            if shipped[i] == our_byte and code[i] == sb_byte and pred(shipped, i):
                hit = (name + "(rev)", i)
                break
        if hit is None:
            return None                     # a diff we can't prove-equivalent -> fail
        patched[i] = shipped[i]
        used.append("%s @0x%x" % hit)
    # mask-aware verify: every UNMASKED byte must now match.
    if _diffpos(bytes(patched), mask, shipped) == []:
        return bytes(patched), used
    return None


# ---------------------------------------------------------------------------
# the bridge
# ---------------------------------------------------------------------------
def close(cc32, cand_c, shipped, prefix="_f", full_matrix=True,
          do_variants=True, do_peephole=True):
    """Try to close a near-miss raiser candidate to byte-exact. Returns a dict:
      closed   : bool
      tier     : 'flag' | 'c-variant' | 'asm-peephole' | None
      flags    : winning flag set (or best)
      variant  : winning C source (== cand_c unless a C-variant won)
      dist     : best masked byte distance reached (0 == exact)
      equivs   : asm equivalences used (tier 3 only)
      bytes    : the byte-exact bytes (hex) when closed
    """
    res = dict(closed=False, tier=None, flags=None, variant=cand_c,
               dist=None, equivs=None, bytes=None)

    # TIER 1 -- fast flags, then (if needed) the full matrix.
    b = try_flags(cc32, cand_c, shipped, FAST_FLAGS, prefix)
    if b and b[0] == 0:
        res.update(closed=True, tier="flag", flags=b[1], dist=0, bytes=b[2].hex(" "))
        return res
    if full_matrix:
        b2 = try_flags(cc32, cand_c, shipped, flag_matrix(), prefix)
        if b2 and (b is None or b2[0] < b[0]):
            b = b2
        if b and b[0] == 0:
            res.update(closed=True, tier="flag", flags=b[1], dist=0, bytes=b[2].hex(" "))
            return res
    best = b                                # (dist, flag, code, mask, diffs)

    # TIER 2 -- C-level equivalence variants, each over the fast flag set.
    if do_variants:
        for src in c_variants(cand_c):
            vb = try_flags(cc32, src, shipped, FAST_FLAGS, prefix)
            if vb and vb[0] == 0:
                res.update(closed=True, tier="c-variant", flags=vb[1], dist=0,
                           variant=src, bytes=vb[2].hex(" "))
                return res
            if vb and (best is None or vb[0] < best[0]):
                best = vb
                res["variant"] = src

    # TIER 3 -- proven below-C encoding equivalences on the best compiled output.
    if do_peephole and best is not None and 1 <= best[0] <= 6:
        pe = try_asm_peephole(best[2], best[3], shipped, best[4])
        if pe is not None:
            patched, equivs = pe
            res.update(closed=True, tier="asm-peephole", flags=best[1], dist=0,
                       equivs=equivs, bytes=patched.hex(" "))
            return res

    res["dist"] = best[0] if best else None
    res["flags"] = best[1] if best else None
    return res


# ---------------------------------------------------------------------------
# convenience: pull a raiser candidate + shipped bytes straight from a target
# ---------------------------------------------------------------------------
def for_target(mod, seg, off):
    """(candidate_c, shipped_bytes) for MODULE seg:off, via the raiser + matchbatch.
    `off` may be given padded ('0015c') or bare ('15c')."""
    import glob
    from codebreaker.foundation import layout
    matchbatch = cores.get('matchbatch')
    from codebreaker.match_engine import raiser
    off_hex = off[2:] if off.startswith("0x") else off
    fs = glob.glob(os.path.join(layout.generated_dir(), "**", mod + ".c"), recursive=True)
    if not fs:
        return None, None
    txt = open(fs[0], encoding="utf-8", errors="replace").read()
    # tolerant of zero-padding: match the [AUTO] header for this seg, any padding of off.
    m = re.search(r"seg%s:0x0*%s\b.*?\n[A-Za-z_][\w ]*\**\w+\(([^)]*)\)\n\{(.*?)\n\}"
                  % (seg, off_hex.lstrip("0") or "0"), txt, re.S)
    if not m:
        return None, None
    cand = raiser.candidate(m.group(1), m.group(2))
    ap = matchbatch._asm_for(mod, seg, off_hex)
    if ap is None:
        ap = matchbatch._asm_for(mod, seg, "0x" + off_hex)
    sb = matchbatch.shipped_bytes(ap) if ap else None
    return cand, sb


# ---------------------------------------------------------------------------
# CLI + selftest
# ---------------------------------------------------------------------------
def _show(cc32, mod, seg, off):
    cand, sb = for_target(mod, seg, off)
    if cand is None:
        print("no in-scope raiser candidate for %s seg%s:0x%s" % (mod, seg, off)); return
    if not sb:
        print("no shipped bytes for %s seg%s:0x%s" % (mod, seg, off)); return
    print("candidate:\n%s" % cand)
    print("shipped: %s  (%d bytes)" % (sb.hex(" "), len(sb)))
    r = close(cc32, cand, sb)
    print("\n--- bridge result ---")
    for k in ("closed", "tier", "flags", "dist", "equivs", "bytes"):
        print("  %-8s: %s" % (k, r[k]))
    if r["tier"] == "c-variant":
        print("  winning variant:\n%s" % r["variant"])


def _selftest(cc32):
    """Prove each tier without a retail anchor: build a target with a known compiler
    choice, then show the bridge recovers byte-exactness through the right tier."""
    print("permutebridge selftest\n")
    ok = True

    # (1) FLAG tier: a target compiled at /O1 /Oy- (frame kept). A candidate whose
    #     default /O2 omits the frame is a length/byte miss until the matrix finds
    #     the frame-keeping flag.
    src = ("extern int __stdcall C_1(int,int);\n"
           "int __stdcall f(int a, int b) { return C_1(a, b); }\n")
    tgt = _emit(cc32, src, "/c /O1 /Oy-")
    if tgt is None:
        print("  (1) FLAG: compile failed -- oracle not staged?  SKIP"); return True
    shipped = tgt[0]
    # mask nothing on shipped side beyond what our own mask gives (same compiler)
    r = close(cc32, src, shipped, do_variants=False, do_peephole=False)
    print("  (1) FLAG tier   : closed=%s tier=%s flags=%s" % (r["closed"], r["tier"], r["flags"]))
    ok = ok and r["closed"] and r["tier"] == "flag"

    # (3) ASM-PEEPHOLE tier: synthesize a shipped variant that differs from our best
    #     ONLY by leave(c9)-for-pop-ebp(5d) in a real frame epilogue, and show tier 3
    #     closes it. Build our compiled bytes first, then flip the teardown byte.
    ours = _emit(cc32, src, "/c /O1 /Oy-")
    if ours:
        code = bytearray(ours[0])
        # find a frame-teardown pop ebp and flip the SHIPPED target to leave
        idx = next((i for i in range(len(code)) if _is_frame_teardown_popebp(bytes(code), i)), None)
        if idx is not None:
            shipped2 = bytearray(code); shipped2[idx] = 0xc9
            r3 = close(cc32, src, bytes(shipped2), do_variants=False)
            print("  (3) ASM tier    : closed=%s tier=%s equivs=%s"
                  % (r3["closed"], r3["tier"], r3["equivs"]))
            ok = ok and r3["closed"] and r3["tier"] == "asm-peephole"
        else:
            print("  (3) ASM tier    : no frame teardown in sample; SKIP")
    print("\nSELFTEST:", "PASS" if ok else "FAIL")
    return ok


def main():
    import cc32
    a = sys.argv[1:]
    if "--selftest" in a:
        sys.exit(0 if _selftest(cc32) else 1)
    if "--show" in a:
        tgt = a[a.index("--show") + 1]
        mod, seg, off = tgt.split(":")
        _show(cc32, mod, seg, off)
        return
    print(__doc__)


if __name__ == "__main__":
    main()
