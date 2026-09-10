#!/usr/bin/env python3
"""
appledger.py — the coverage ledger for the APPLICATION layer.

`ledger.py` measures the nine system modules by byte coverage: for every address
the reconstruction cites, the size of the function carved at that address. The
application layer never got the same treatment — it was counted only as "how
many C functions exist", which says nothing about how much of each EXE has been
read.

This tool applies the system-module measurement to the thirteen bundled apps.

THE CITATION-OWNERSHIP RULE (learned the hard way on GDI/USER)
    A `segN:0xHHHH` in calc.c is CALC's segment N. The same string in write_wnd.c
    is WRITE's segment N — a completely different function. So the cited-set for
    an app is built ONLY from the files that are about that app. Pooling them
    over-counts every app by the size of every other.

Usage:
    python appledger.py               # per-app byte coverage
    python appledger.py --gaps APP    # that app's uncited functions, biggest first
    python appledger.py --gaps APP --all
"""
import glob, re, sys, collections
import trace
from codebreaker.foundation import layout
from codebreaker.disasm_engine.disasm import open_ne

# Which reconstruction files are ABOUT which EXE. Nothing is inferred from the
# name at match time: the mapping is written out so a mistake is visible.
OWNS = {
    "CALC":     ["calc.c"],
    "CALENDAR": ["calendar.c"],
    "CARDFILE": ["cardfile.c"],
    "CLIPBRD":  ["clipbrd.c"],
    "CLOCK":    ["clock.c"],
    "CONTROL":  ["control.c"],
    "NOTEPAD":  ["notepad.c"],
    "PAINT":    ["paint_core.c", "paint_tools.c"],
    "REVERSI":  ["reversi.c"],
    "SPOOLER":  ["spooler.c"],
    "TERMINAL": ["terminal.c"],
    "WRITE":    ["write_dlg.c", "write_wnd.c", "write_map.c",
                 "write_segments_map.c"],
    "MSDOSD":   [],
}


def carve(ne):
    """{(seg, off): size} for every function carved out of the module."""
    out, o = {}, trace.load_ordinals("ORDINALS.md")
    for i, s in enumerate(ne.segments(), 1):
        if s["kind"] != "CODE":
            continue
        try:
            sg = trace.Seg(ne, i, o)
        except Exception:
            continue
        for start, f in sg.funcs.items():
            out[(i, start)] = max(0, f["end"] - start)
    return out


def app_state(app):
    ne = open_ne("files/WF/%s.EXE" % app)
    fns = carve(ne)
    code = sum(s["length"] for s in ne.segments() if s["kind"] == "CODE")
    # appmap.py writes reconstructed/<app>_map.c by convention. Include it
    # automatically so a generated map is never silently uncounted — the
    # failure this ledger exists to catch is progress being CLAIMED, but a map
    # that exists and is not measured is the same error pointing the other way.
    cited = set()
    for base in OWNS[app] + ["%s_map.c" % app.lower()]:
        try:
            # honor the migrated src/ tree (+ component nesting) via layout
            src = open(layout.module_path(base, "reconstructed"),
                       encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        for mo in re.finditer(r"\bseg(\d+):0x([0-9a-fA-F]{2,4})", src):
            cited.add((int(mo.group(1)), int(mo.group(2), 16)))
    return ne, fns, code, cited


def main():
    if "--gaps" in sys.argv:
        app = sys.argv[sys.argv.index("--gaps") + 1].upper()
        ne, fns, code, cited = app_state(app)
        rows = sorted(((sz, s, o) for (s, o), sz in fns.items()
                       if (s, o) not in cited), reverse=True)
        lim = len(rows) if "--all" in sys.argv else 40
        print("# %s: %d of %d functions not yet cited (%d B uncited), largest first"
              % (app, len(rows), len(fns), sum(r[0] for r in rows)))
        bys = collections.Counter()
        for sz, s, o in rows:
            bys[s] += sz
        print("# uncited bytes by segment: " +
              "  ".join("seg%d=%d" % (s, b) for s, b in sorted(bys.items())))
        for sz, s, o in rows[:lim]:
            print("  seg%-3d:0x%04x  %6d B" % (s, o, sz))
        return

    print("APPLICATION LAYER     fns   cited    code B    cited B   byte cover")
    tf = tc = tb = tcb = 0
    for app in sorted(OWNS):
        try:
            ne, fns, code, cited = app_state(app)
        except Exception as e:
            print("  %-11s  --  (%s)" % (app, e))
            continue
        hit = [k for k in fns if k in cited]
        cb = sum(fns[k] for k in hit)
        tf += len(fns); tc += len(hit); tb += code; tcb += cb
        print("  %-11s %6d %7d %9d %10d %9.1f%%"
              % (app, len(fns), len(hit), code, cb, 100 * cb / max(1, code)))
    print("  %-11s %6d %7d %9d %10d %9.1f%%"
          % ("TOTAL", tf, tc, tb, tcb, 100 * tcb / max(1, tb)))


if __name__ == "__main__":
    main()
