#!/usr/bin/env python3
"""
splitall.py — run the period-build pipeline (splitbuild.py) over EVERY
reconstructed app and print one scoreboard.

This is the whole-project answer to "does the reconstruction compile the way
Microsoft shipped it?": each app is split into its real per-segment modules and
every module is compiled by MS C 3.0 under DOSBox-X. One DOSBox session per app,
run sequentially — concurrent runs would fight over build16/_sb.conf.

Stems are 8.3-safe and fixed here so results are comparable run to run: the
longest emitted name is <STEM><seg><chunk>, which must stay <= 8 characters.

Usage:  python splitall.py            (all apps)
        python splitall.py calc paint_core     (just these)
"""
import os
import re
import subprocess
import sys
from codebreaker.core import layout

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE

# app source (in reconstructed/) -> module stem
APPS = [
    ("calc",        "CALC"),
    ("calendar",    "CAL"),
    ("cardfile",    "CARD"),
    ("clipbrd",     "CLIP"),
    ("clock",       "CLK"),
    ("control",     "CTL"),
    ("notepad",     "NOTE"),
    ("paint_core",  "PNT"),
    ("paint_tools", "PTOOL"),
    ("reversi",     "REV"),
    ("spooler",     "SPL"),
    ("terminal",    "TERM"),
    ("write_dlg",   "WDLG"),
    ("write_wnd",   "WWND"),
]

TALLY = re.compile(r"(\d+)/(\d+) segment modules compile clean")


def main():
    want = [a.lower() for a in sys.argv[1:]]
    apps = [(a, s) for a, s in APPS if not want or a in want]
    results = []
    for app, stem in apps:
        src = os.path.join(layout.recon_dir(ROOT), app + ".c")
        if not os.path.exists(src):
            print("  (missing: %s)" % src)
            continue
        print("=" * 70)
        print("%-12s -> %s" % (app, stem))
        sys.stdout.flush()
        p = subprocess.run([sys.executable, os.path.join(HERE, "splitbuild.py"),
                            src, stem], capture_output=True, text=True,
                           encoding="utf-8", errors="replace")
        out = p.stdout or ""
        m = TALLY.search(out)
        ok, total = (int(m.group(1)), int(m.group(2))) if m else (0, 0)
        fails = [ln.split()[0] for ln in out.splitlines()
                 if " ERROR" in ln and not ln.startswith("MODULE")]
        results.append((app, ok, total, fails))
        print("  %d/%d clean%s" % (ok, total,
                                   ("   failing: " + ", ".join(fails)) if fails else ""))
        sys.stdout.flush()
    print("\n" + "=" * 70)
    print("%-14s %-9s %s" % ("APP", "MODULES", "FAILING"))
    print("-" * 70)
    tok = ttot = 0
    for app, ok, total, fails in results:
        tok += ok; ttot += total
        print("%-14s %d/%-7d %s" % (app, ok, total, ", ".join(fails)))
    print("-" * 70)
    print("%d/%d segment modules compile clean across %d apps"
          % (tok, ttot, len(results)))


if __name__ == "__main__":
    main()
