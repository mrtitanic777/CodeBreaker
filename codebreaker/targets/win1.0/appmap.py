#!/usr/bin/env python3
"""
appmap.py — generate a segment map for ANY of the thirteen bundled applications.

WRITE's map was written by a bespoke script. Twelve more bespoke scripts is the
same mistake this project already fixed once at the function level: the cost was
never the analysis, it was doing the mechanical part by hand each time.

For an app this emits, per code segment: its size, its function count, whether
it is PRELOAD or load-on-call, the names its entry table gives it, the distinct
system APIs it calls (from the relocation table — facts, not readings), and
every function start as an individual `segN:0xHHHH` citation.

WHAT IT DOES NOT DO
    It does not name a segment. A one-line family reading is EVIDENCE-BACKED
    PROSE that a human writes after looking at the profile, and it goes in the
    per-app READINGS table below. A segment with no entry there is emitted
    saying so. That blank is a finding, not a placeholder: it means the
    relocation table had nothing to say, which is itself information about what
    the segment does (arithmetic on the program's own structures).

    The alternative — deriving a family from the API profile with a rule table —
    would manufacture a plausible label for every segment and make the honest
    blanks disappear. That is the failure mode this project exists to avoid.

Usage:
    python appmap.py CALC              # write reconstructed/calc_map.c
    python appmap.py CALC --dry        # print the profile, write nothing
"""
import os, re, sys, collections
import segprofile, rsrc, idord, trace
from codebreaker.foundation import layout

# Human readings, added as segments are actually examined. Keyed (APP, seg).
# Each is prose a person wrote after reading that segment's profile. Segments
# absent from this table are emitted as unread — deliberately.
READINGS = {
 # --- PAINT -------------------------------------------------------------
 # PAINT is the clearest case in the whole box: seg2 calls RegisterClass six
 # times and PAINT exports exactly six window procedures, and after that the
 # segmentation is ONE SEGMENT PER DRAWING TOOL, most of them a few hundred
 # bytes calling nothing but GDI primitives and SetROP2.
 ("PAINT", 1):  "the main window, and the menu layer with it — "
                "EnableMenuItem x40, CheckMenuItem x18, ChangeMenu x6",
 ("PAINT", 2):  "class registration: RegisterClass x6, one per exported "
                "window procedure, plus LoadIcon and the stock objects",
 ("PAINT", 3):  "bitmap and cursor setup — CreateBitmap x7, LoadString x10",
 ("PAINT", 4):  "the pattern editor window (PATEDITWNDPROC)",
 ("PAINT", 6):  "the drawing canvas (CANVASWNDPROC)",
 ("PAINT", 7):  "the zoomed-out view (ZOOMOUTWNDPROC)",
 ("PAINT", 8):  "the zoomed-in view (ZOOMINWNDPROC)",
 ("PAINT", 9):  "the palette window (PALETTEWNDPROC)",
 ("PAINT", 10): "file handling — OpenFile x9, MessageBox x8, lstrcpy x18",
 ("PAINT", 11): "the file dialogs — DlgDirList x4; exports TRACKFILEOPEN, "
                "TRACKFILESAVEAS, TRACKFILECHANGED",
 ("PAINT", 12): "printing — Escape x6, MulDiv x10; exports ABORTPRINT, ABORTTEST",
 ("PAINT", 27): "**the fill tool.** 102 bytes, one function, and its only two "
                "system calls are FloodFill and GetPixel",
 ("PAINT", 24): "blitting a selection — BitBlt x25, DeleteDC x18, CopyRect x4",
 ("PAINT", 26): "blitting a selection — BitBlt x19, DeleteDC x13, CopyRect x4",
 # --- CALC --------------------------------------------------------------
 ("CALC", 1):   "the whole program in one segment: 131 functions, exporting "
                "CALCWNDPROC and ABOUT",
}


def resources_summary(ne):
    """Menu command ids and string table, as evidence lines."""
    menu, out = {}, []
    for r in ne.resources():
        if r["type"] != "MENU":
            continue
        d = ne.data[r["off"]:r["off"] + r["length"]]
        path = []
        try:
            items = rsrc.menu(d)
        except Exception:
            continue
        for depth, ident, txt, fl in items:
            del path[depth:]
            if ident is None:
                path.append(txt); continue
            if ident and txt:
                menu[ident] = " > ".join(path + [txt.replace("\t", " / ")])
    strs = dict(rsrc.strings(ne, ne.resources()))
    return menu, strs


def build(app, dry=False):
    ne, segs = segprofile.profile(app)
    byseg = {d["seg"]: d for d in segs if "err" not in d}
    menu, strs = resources_summary(ne)
    o2n = {}
    for nm, od in ne.nonresident_names()[1:] + ne.resident_names()[1:]:
        o2n[od] = nm
    nexp = len(o2n)
    nent = len(ne.entries())
    total = sum(d["size"] for d in byseg.values())
    nfn = sum(d["nfn"] for d in byseg.values())

    if dry:
        for i in sorted(byseg):
            d = byseg[i]
            print("seg%-3d %6d B %3d fns %s %s" % (
                i, d["size"], d["nfn"], "PRE " if d["preload"] else "LOC ",
                " ".join(n for n, _ in d["exports"])))
            if d["apis"]:
                print("     " + ", ".join("%s(%d)" % (a, c)
                                          for a, c in d["apis"].most_common(12)))
        return None

    L = []; w = L.append
    w("/*")
    w(" * %s_map.c  —  %s.EXE, ALL %d CODE SEGMENTS" % (app.lower(), app, len(byseg)))
    w(" *")
    w(" * %d bytes of code, %d functions, %d segments." % (total, nfn, len(byseg)))
    w(" *")
    w(" * Each segment is placed by the system APIs it calls. Those come from the")
    w(" * relocation table, so they are facts about the binary rather than")
    w(" * readings of it. Every function is CITED and is a **documented partial**:")
    w(" * where it lives and what its segment reaches for are established; its")
    w(" * body is not transcribed.")
    w(" *")
    w(" * A segment with no family line below is one whose relocation table had")
    w(" * nothing to say. That is reported rather than filled in.")
    w(" *")
    w(" * (Generated by appmap.py from rsrc.py + segprofile.py.)")
    w(" *")

    if menu:
        w(" * ==========================================================================")
        w(" * THE COMMAND SET, FROM THE MENU RESOURCE")
        w(" * ==========================================================================")
        w(" *")
        w(" * The menu resource pairs each command id with the text the original")
        w(" * authors wrote. A WM_COMMAND handler compares wParam against these")
        w(" * exact values, so this table is what makes the handlers findable.")
        w(" *")
        for cid in sorted(menu):
            w(" *     0x%04x  %s" % (cid, menu[cid]))
        w(" *")

    w(" * ==========================================================================")
    w(" * THE ENTRY TABLE")
    w(" * ==========================================================================")
    w(" *")
    w(" * %d entries, %d of them named. The unnamed ones carry no interface: they" % (nent, nexp))
    w(" * are the entry-table indirections that make a MOVEABLE segment callable.")
    if nexp:
        w(" * The named ones are the places where Windows calls INTO the program —")
        w(" * window procedures, dialog procedures, enumeration callbacks.")
        w(" *")
        ents = {e["ordinal"]: e for e in ne.entries() if "offset" in e}
        for od in sorted(o2n):
            e = ents.get(od)
            if e:
                w(" *     %s.%-3d  %-22s seg%d:0x%04x"
                  % (app, od, o2n[od], e["seg"], e["offset"]))
    w(" *")
    w(" * ==========================================================================")
    w(" * THE SEGMENTS")
    w(" * ==========================================================================")

    for i in sorted(byseg):
        d = byseg[i]
        w(" *")
        w(" * ------------------------------------------------------------------")
        w(" * seg%-3d  %5d B  %2d fns  %s"
          % (i, d["size"], d["nfn"], "PRELOAD" if d["preload"] else "load-on-call"))
        rd = READINGS.get((app, i))
        if rd:
            w(" *   %s" % rd.upper())
        else:
            w(" *   (no reading — the relocation table does not say)")
        if d["exports"]:
            w(" *   exports: " + ", ".join("%s@seg%d:0x%04x" % (n, i, o)
                                           for n, o in d["exports"]))
        if d["apis"]:
            ev = ", ".join("%s(%d)" % (a, c) for a, c in d["apis"].most_common(16))
            for k in range(0, len(ev), 68):
                w(" *   %s %s" % ("calls:   " if k == 0 else "         ", ev[k:k + 68]))
        line = " *   "
        for s in d["starts"]:
            tok = "seg%d:0x%04x " % (i, s)
            if len(line) + len(tok) > 74:
                w(line); line = " *   "
            line += tok
        if line.strip(" *"):
            w(line)

    if strs:
        w(" *")
        w(" * ==========================================================================")
        w(" * THE STRING TABLE")
        w(" * ==========================================================================")
        w(" *")
        w(" * What a program apologises for is a description of what it does.")
        w(" *")
        for sid in sorted(strs):
            t = strs[sid].replace("*/", "* /")
            w(" *     %-6d %r" % (sid, t))

    w(" */")
    w("")
    w("#include <windows.h>")
    w('#include "../win1.h"')
    w("")
    w("/* Nothing is transcribed in this file. Every address above is cited so the")
    w(" * ledger records that it has been read and placed; each remains a")
    w(" * documented partial until its body is written out. */")

    src = "\n".join(L) + "\n"
    path = layout.module_out_path("%s_map.c" % app.lower(), "reconstructed")
    open(path, "w", encoding="utf-8").write(src)
    got = open(path, encoding="utf-8").read()
    n = len(re.findall(r"seg\d+:0x[0-9a-f]{4}", got))
    assert n >= nfn, "expected >= %d citations, wrote %d" % (nfn, n)
    return path, len(got), n, nfn


def main():
    app = sys.argv[1].upper()
    r = build(app, dry="--dry" in sys.argv)
    if r:
        print("%s: %d bytes, %d citations covering %d functions" % r)


if __name__ == "__main__":
    main()
