#!/usr/bin/env python3
"""
segprofile.py — what each code segment of a module DOES, from the calls it makes.

The cheapest strong evidence about an unread segment is the set of system APIs
it calls. Those come from the relocation table, so they are facts about the
binary, not readings of it: a segment that calls OpenFile and _lread does file
I/O; one that calls ExtTextOut and SelectObject draws; one that calls DialogBox
raises a dialog.

This does NOT name anything. It reports, per segment:
    - the exported names the entry table gives that segment (the anchor)
    - its size and how many functions were carved out of it
    - the DISTINCT system APIs it calls, which is the profile
    - how many of its functions are already cited

Used to group a large application's segments into families before writing them
up, the same way the call graph was used on GDI and USER.

Usage:
    python segprofile.py WRITE
    python segprofile.py WRITE --seg 34
    python segprofile.py WRITE --uncited          # only segments not yet read
"""
import sys, re, collections
import idord, trace


def profile(mod):
    ne = idord.open_module(mod)
    ords = trace.load_ordinals("ORDINALS.md")
    o2n = {}
    for nm, od in ne.nonresident_names()[1:] + ne.resident_names()[1:]:
        o2n[od] = nm
    segexp = collections.defaultdict(list)
    for e in ne.entries():
        if "offset" in e and e["ordinal"] in o2n:
            segexp[e["seg"]].append((o2n[e["ordinal"]], e["offset"]))

    out = []
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, ords)
        except Exception as ex:
            out.append(dict(seg=i, err=str(ex))); continue
        apis = collections.Counter()
        for site, lbl in sg.fixups.items():
            m = re.match(r"^(KERNEL|GDI|USER|SYSTEM|KEYBOARD|MOUSE|DISPLAY|SOUND|COMM)\.(\d+)$", lbl)
            if m:
                # ORDINALS.md holds a NAME for a resolved ordinal and a PROSE
                # DESCRIPTION for an unresolved one ("a 9-word file operation,
                # identity unknown..."). Only an identifier is a name; anything
                # else must stay an ordinal, or a profile line reads as though
                # the function had been identified when it has not.
                nm = ords.get(lbl)
                apis[nm if nm and re.match(r"^[A-Za-z_]\w*$", nm) else lbl] += 1
        out.append(dict(seg=i, size=s["length"], nfn=len(sg.funcs),
                        preload=bool(s["flags"] & 0x0040),
                        exports=segexp.get(i, []), apis=apis,
                        starts=sorted(sg.funcs)))
    return ne, out


def main():
    mod = sys.argv[1].upper()
    ne, segs = profile(mod)
    only = None
    if "--seg" in sys.argv:
        only = int(sys.argv[sys.argv.index("--seg") + 1])
    for d in segs:
        if only and d["seg"] != only:
            continue
        if "err" in d:
            print("seg%-3d  (undisassemblable: %s)" % (d["seg"], d["err"])); continue
        exp = " ".join("%s@0x%04x" % (n, o) for n, o in d["exports"])
        print("\nseg%-3d %6d B  %3d fns  %s  %s"
              % (d["seg"], d["size"], d["nfn"],
                 "PRELOAD    " if d["preload"] else "LOAD-ON-CALL", exp))
        top = d["apis"].most_common(14 if not only else 60)
        if top:
            print("      calls: " + ", ".join("%s(%d)" % (a, c) for a, c in top))
        if only:
            print("      function starts: " +
                  " ".join("0x%04x" % s for s in d["starts"]))


if __name__ == "__main__":
    main()
