#!/usr/bin/env python3
"""
symscore.py — score this project's invented names against Microsoft's own.

The SDK's SYMDEB files carry the original internal symbol names for KERNEL, GDI
and USER. `symfile.py --check` established that the debug and retail builds
share a layout, so a symbol offset can be compared against a retail address.

That makes something possible that almost no reverse-engineering project ever
gets: **a scored answer key for names that were invented blind.**

Reports four populations:
    MATCH    we named it and Microsoft agrees
    DIFFER   we named it and Microsoft calls it something else — every one of
             these needs reading, because a differing name is either a harmless
             synonym or a wrong reading, and only the body says which
    NEW       we had no name and now do
    DATA      a symbol at an address we carved as a function, whose name says
             it is a table (RG..., SZ..., ...) — our carver's mistakes

Usage:
    python symscore.py USER
    python symscore.py USER --seg 1
    python symscore.py --all
"""
import re, sys, os
import symfile, idord, trace

SYSMODS = ["KERNEL", "GDI", "USER"]
IDENT = re.compile(r"^[A-Za-z_]\w*$")
# Microsoft's Hungarian for data in these modules: rg = array, sz = string.
# A symbol so prefixed sitting at an address we carved as a function means the
# CARVER was wrong, not the symbol.
#
# `P` is NOT in this list. It was, and it classified POSTMESSAGE, PEEKMESSAGE,
# POSTQUITMESSAGE and PAINTRECT as data — the pointer prefix is a LOWERCASE p
# (pStackTop, pStackMin), and uppercase P is just a word starting with P.
#
# RGN is excluded too: RGNWALK and RGNEW are region routines, not `rg` arrays.
# The prefix rule is a hint, and a hint that swallows a whole subsystem's
# vocabulary is worse than no hint.
DATAPFX = re.compile(r"^(RG(?!N)|SZ)[A-Z]")


def sym_groups(mod):
    """{segment_NAME: {offset: SYMBOL}} using the inline segment-name records."""
    path = os.path.join(symfile.SYMDIR, mod + ".SYM")
    f = symfile.flat(path)
    cur, groups, order = None, {}, []
    for off, n in f:
        if n.startswith("_") and not n.startswith("__"):
            cur = n
            if cur not in groups:
                groups[cur] = {}
                order.append(cur)
            continue
        if cur is not None:
            groups[cur].setdefault(off, n)
    return groups, order


def correlate(mod, groups, order):
    """Map each .SYM segment NAME to a RETAIL segment index, from evidence.

    *** THIS MUST BE DERIVED, NOT ASSUMED ***

    The first version of this numbered the .SYM segments by their order in the
    file and called that the retail segment index. It is not. The result looked
    like a scorecard with ten disagreements in it, and every one of them was at
    offset 0x0000 of some segment with our name for one segment and Microsoft's
    name for another — the same set of names, permuted. That is the signature of
    a bad mapping, not of bad readings, and reported as-is it would have been a
    false accusation against work that was correct.

    The test that actually carries evidence: a .SYM segment belongs to the
    retail segment whose INDEPENDENTLY CARVED function starts its symbol offsets
    land on. A wrong pairing scores near zero because offsets land mid-
    instruction; the right one scores high. Pairings that stay ambiguous are
    reported as unmapped rather than guessed.
    """
    ne = idord.open_module(mod)
    ords = trace.load_ordinals("ORDINALS.md")
    starts = {}
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            starts[i] = set(trace.Seg(ne, i, ords).funcs)
        except Exception:
            pass

    scores = {}
    for nm in order:
        offs = set(groups[nm])
        if not offs:
            continue
        best = []
        for i, st in starts.items():
            hit = len(offs & st)
            best.append((hit / len(offs), hit, i))
        best.sort(reverse=True)
        scores[nm] = best[:3]

    mapping, taken = {}, set()
    for nm, best in sorted(scores.items(), key=lambda kv: -kv[1][0][0]):
        frac, hit, i = best[0]
        # Require a real majority AND that the runner-up is clearly worse, so a
        # tiny segment matching several places by luck stays unmapped.
        second = best[1][0] if len(best) > 1 else 0.0
        if frac >= 0.5 and hit >= 2 and i not in taken and frac > second + 0.15:
            mapping[nm] = i
            taken.add(i)
    return mapping, scores


def score(mod, only=None):
    ne = idord.open_module(mod)
    ords = trace.load_ordinals("ORDINALS.md")
    groups, order = sym_groups(mod)
    mapping, scores = correlate(mod, groups, order)
    syms = {}
    for nm, i in mapping.items():
        syms[i] = groups[nm]
    print("  .SYM->retail mapping proved for %d of %d segments: %s"
          % (len(mapping), len(order),
             ", ".join("%s=seg%d" % (n, i) for n, i in sorted(mapping.items(),
                                                              key=lambda kv: kv[1]))))
    unmapped = [n for n in order if n not in mapping and groups.get(n)]
    if unmapped:
        print("  unmapped (reported, not guessed): %s" % ", ".join(unmapped))

    # ordinal -> (seg, off) from the retail entry table
    ent = {}
    for e in ne.entries():
        if "offset" in e:
            ent[e["ordinal"]] = (e["seg"], e["offset"])

    match, differ, new, data = [], [], [], []
    for od, (s, off) in sorted(ent.items()):
        if only and s != only:
            continue
        ms = syms.get(s, {}).get(off)
        if not ms:
            continue
        ours = ords.get("%s.%d" % (mod, od))
        if DATAPFX.match(ms):
            data.append((od, s, off, ours, ms))
        elif ours and IDENT.match(ours):
            (match if ours.upper() == ms.upper() else differ).append((od, s, off, ours, ms))
        else:
            new.append((od, s, off, ms))
    return match, differ, new, data, syms


def main():
    mods = SYSMODS if "--all" in sys.argv else [sys.argv[1].upper()]
    only = int(sys.argv[sys.argv.index("--seg") + 1]) if "--seg" in sys.argv else None
    for mod in mods:
        try:
            m, d, n, dat, syms = score(mod, only)
        except Exception as e:
            print("%s: %s" % (mod, e)); continue
        tot = len(m) + len(d) + len(n) + len(dat)
        print("\n===== %s =====" % mod)
        print("  symbols available            : %d across %d segments"
              % (sum(len(v) for v in syms.values()), len(syms)))
        print("  entry-table addresses covered: %d" % tot)
        print("    MATCH  we named it, MS agrees   : %d" % len(m))
        print("    DIFFER we named it, MS disagrees: %d" % len(d))
        print("    NEW    no name before           : %d" % len(n))
        print("    DATA   carved as code, is data  : %d" % len(dat))
        if d:
            print("\n  -- DIFFER (each needs reading) --")
            for od, s, off, a, b in d[:40]:
                print("     %s.%-4d seg%d:0x%04x  ours=%-26s MS=%s" % (mod, od, s, off, a, b))
        if dat:
            print("\n  -- carved as a function but named as data --")
            for od, s, off, a, b in dat[:15]:
                print("     %s.%-4d seg%d:0x%04x  %s" % (mod, od, s, off, b))


if __name__ == "__main__":
    main()
