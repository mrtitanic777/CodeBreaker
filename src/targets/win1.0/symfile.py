#!/usr/bin/env python3
"""
symfile.py — parse the SYMDEB .SYM files shipped in the Windows 1.01 SDK.

WHAT THESE ARE
    refs/sdk101/d04/SYM/{GDI,KERNEL,USER}.SYM are symbol files for SYMDEB,
    shipped by Microsoft with the 1.01 SDK. They carry the ORIGINAL INTERNAL
    NAMES of the system modules — the functions this project has spent its
    entire life citing as `seg1:0xNNNN` because the retail binary's entry table
    names only the exports.

    They are authoritative in a way nothing else available is: they are
    Microsoft's own symbols for this exact product version.

THE CATCH, WHICH MATTERS
    The symbols belong to the DEBUG build (d05/DEBUG/USER.EXE), not to the
    retail WIN100.BIN this project reconstructs. **A name from here may not sit
    at the same address in the retail image.** So a symbol is evidence about
    WHAT EXISTS and WHAT IT IS CALLED; it becomes evidence about a retail
    address only once the two builds are shown to agree at that address.

    `--check` does exactly that test and reports the answer rather than assuming
    it. Nothing downstream should use these names against retail addresses until
    that check passes.

FORMAT
    A chain of MAPDEF records (one per module), each holding a chain of SEGDEF
    records (one per segment), each holding symbol records. A symbol is

        WORD offset;  BYTE cbName;  char name[cbName];

    Offsets and paragraph pointers are in 16-byte paragraphs from file start.

Usage:
    python symfile.py USER                # every symbol, by segment
    python symfile.py USER --check        # do the debug-vs-retail address test
    python symfile.py USER --seg 1
"""
import struct, sys, re, os

SYMDIR = "refs/sdk101/d04/SYM"


def _name(d, p):
    n = d[p]
    return d[p + 1:p + 1 + n].decode("cp437", "replace"), p + 1 + n


def parse(path):
    """-> {segment_index: [(offset, name), ...]}, module_name.

    Walks the MAPDEF/SEGDEF chains. Where the chain pointers do not lead
    somewhere sane the parser stops rather than resyncing on plausible-looking
    bytes: a resynchronised symbol table is a table of names attached to the
    wrong addresses, which is worse than no table.
    """
    d = open(path, "rb").read()
    out, modname = {}, None

    pmap = 0
    seen = set()
    while pmap * 16 < len(d) and pmap not in seen:
        seen.add(pmap)
        b = pmap * 16
        if b + 16 > len(d):
            break
        (pnextmap, _flags, psegentry, cconst, pconstdef,
         cseg, csegname) = struct.unpack_from("<HHHHHHH", d, b)
        nm, _ = _name(d, b + 14)
        if modname is None:
            modname = nm

        # segment chain
        pseg = psegentry
        segi = 0
        sseen = set()
        while pseg and pseg * 16 < len(d) and pseg not in sseen:
            sseen.add(pseg)
            sb = pseg * 16
            pnextseg, csym, psymdef = struct.unpack_from("<HHH", d, sb)
            segi += 1
            syms = []
            # the symbol pointer table: csym WORDs of file offsets from sb
            for k in range(csym):
                po = sb + psymdef + 2 * k
                if po + 2 > len(d):
                    break
                so = struct.unpack_from("<H", d, po)[0]
                q = sb + so
                if q + 3 > len(d):
                    continue
                off = struct.unpack_from("<H", d, q)[0]
                try:
                    n2, _ = _name(d, q + 2)
                except Exception:
                    continue
                if n2 and all(32 <= ord(c) < 127 for c in n2):
                    syms.append((off, n2))
            if syms:
                out[segi] = sorted(set(syms))
            pseg = pnextseg
        pmap = pnextmap
        if not pnextmap:
            break
    return out, modname


def flat(path):
    """Fallback: every (offset, name) record in file order, ungrouped.

    Used only for REPORTING what names exist. It cannot say which segment a
    name belongs to, so it must never be used to attach a name to an address.
    """
    d = open(path, "rb").read()
    out, p = [], 0
    while p + 3 < len(d):
        off = struct.unpack_from("<H", d, p)[0]
        n = d[p + 2]
        if 3 <= n <= 40 and p + 3 + n <= len(d):
            s = d[p + 3:p + 3 + n]
            if re.fullmatch(rb"[A-Za-z_][A-Za-z0-9_$]*", s):
                out.append((off, s.decode()))
                p += 3 + n
                continue
        p += 1
    return out


def check(mod):
    """Do the DEBUG-build symbols line up with the RETAIL binary?

    Test: the retail module's entry table gives (ordinal -> seg:offset) for
    every export, and the resident/non-resident name tables give the export's
    NAME. If the debug build has the same layout, a symbol with that name sits
    at that same offset. Report the hit rate; do not assume it.
    """
    import idord
    ne = idord.open_module(mod)
    o2n = {}
    for nm, od in ne.resident_names()[1:] + ne.nonresident_names()[1:]:
        o2n[od] = nm.upper()
    want = {}
    for e in ne.entries():
        if "offset" in e and e["ordinal"] in o2n:
            want[o2n[e["ordinal"]]] = (e["seg"], e["offset"])

    # The MAPDEF/SEGDEF chain walk does not reach the symbol tables in these
    # files, so grouping comes from the segment-name records that appear inline
    # (_TEXT and friends). Every symbol after such a record belongs to it, until
    # the next one. That is a structural fact of the file, not a resync.
    f = flat(os.path.join(SYMDIR, mod + ".SYM"))
    segnames, cur, groups = [], None, {}
    for off, n in f:
        if n.startswith("_") and not n.startswith("__"):
            cur = n
            segnames.append(n)
            groups.setdefault(cur, [])
            continue
        if cur:
            groups[cur].append((off, n))

    # _TEXT is segment 1; further named segments follow in file order.
    order = {nm: i + 1 for i, nm in enumerate(segnames)}
    bysegoff = {}
    for nm, lst in groups.items():
        for off, n in lst:
            bysegoff.setdefault(n.upper(), []).append((order[nm], off))
    print("   .SYM segments: %s" % ", ".join("%s=%d" % (n, order[n]) for n in segnames))

    hit = miss = absent = 0
    examples = []
    for n, (s, off) in sorted(want.items()):
        got = bysegoff.get(n)
        if not got:
            absent += 1
            continue
        if (s, off) in got:
            hit += 1
        else:
            miss += 1
            if len(examples) < 8:
                examples.append((n, (s, off), got[:3]))
    print("%s: %d named exports in the retail entry table" % (mod, len(want)))
    print("   symbol present AND at the same seg:offset : %d" % hit)
    print("   symbol present but at a DIFFERENT address : %d" % miss)
    print("   name not in the .SYM at all               : %d" % absent)
    for n, w, g in examples:
        print("      %-22s retail seg%d:0x%04x   sym %s" % (n, w[0], w[1], g))
    if hit and not miss:
        print("\n   => the debug and retail layouts AGREE on every testable export.")
    elif miss > hit:
        print("\n   => layouts DIFFER. Names are usable; addresses are NOT"
              " transferable without per-function correlation.")
    else:
        print("\n   => mixed. Treat addresses as unproven.")


def main():
    mod = sys.argv[1].upper()
    if "--check" in sys.argv:
        check(mod); return
    path = os.path.join(SYMDIR, mod + ".SYM")
    syms, name = parse(path)
    if not syms:
        f = flat(path)
        print("# %s: chain parse found nothing; %d records in flat order" % (mod, len(f)))
        for off, n in f[:400]:
            print("   0x%04x  %s" % (off, n))
        return
    only = int(sys.argv[sys.argv.index("--seg") + 1]) if "--seg" in sys.argv else None
    print("# %s (%s): %d segments, %d symbols"
          % (mod, name, len(syms), sum(len(v) for v in syms.values())))
    for s in sorted(syms):
        if only and s != only:
            continue
        print("\n## segment %d — %d symbols" % (s, len(syms[s])))
        for off, n in syms[s]:
            print("   0x%04x  %s" % (off, n))


if __name__ == "__main__":
    main()
