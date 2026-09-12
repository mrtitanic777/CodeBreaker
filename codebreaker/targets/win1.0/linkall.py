#!/usr/bin/env python3
"""
linkall.py — link EVERY reconstructed app against its own shipped binary and
report one table: what links, what is still missing, and how close the rebuilt
NE is to the retail one.

The number that matters per app is STUBS: functions the reconstruction calls and
has not transcribed. The linker names them, mkstubs.py counts them, and that
count is a direct, per-app measure of remaining work — far better than "% of
functions", because it counts only what the transcribed code actually reaches.

PAINT and WRITE are each ONE binary reconstructed across TWO source files; they
are merged here (verified to share no function or global names) before linking.

Runs sequentially: every app needs its own DOSBox-X session and they would
otherwise fight over build16/_lk.conf.

Usage:  python linkall.py              (all apps)
        python linkall.py calc paint   (just these)
"""
import os
import re
import subprocess
import sys

from codebreaker.core import layout
HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE

# app binary -> (stem, [reconstructed sources])
APPS = [
    ("CALC",     "CALC", ["calc.c"]),
    ("CALENDAR", "CAL",  ["calendar.c"]),
    ("CARDFILE", "CARD", ["cardfile.c"]),
    ("CLIPBRD",  "CLIP", ["clipbrd.c"]),
    ("CLOCK",    "CLK",  ["clock.c"]),
    ("CONTROL",  "CTL",  ["control.c"]),
    ("NOTEPAD",  "NOTE", ["notepad.c"]),
    ("PAINT",    "PNT",  ["paint_core.c", "paint_tools.c"]),
    ("REVERSI",  "REV",  ["reversi.c"]),
    ("SPOOLER",  "SPL",  ["spooler.c"]),
    ("TERMINAL", "TERM", ["terminal.c"]),
    ("WRITE",    "WRT",  ["write_dlg.c", "write_wnd.c"]),
]

INCLUDE_LINE = re.compile(r'(?m)^#\s*include\s*[<"](windows|winexp)\.h[>"].*$')


def merge(sources, out):
    """Concatenate an app's sources, keeping only the FIRST copy of each
    system include (a second <winexp.h> would redeclare every lstr* it names)."""
    parts, seen_inc = [], False
    for i, s in enumerate(sources):
        text = open(os.path.join(layout.recon_dir(ROOT), s),
                    encoding="utf-8").read()
        # No include rewriting here: segsplit strips <windows.h>/<winexp.h>
        # from the source and re-emits them once per module, so duplicates
        # across merged files take care of themselves. (Rewriting them was
        # worse than useless — the include lines carry trailing multi-line
        # comments, and commenting a comment out broke the parse.)
        parts.append("/* ---- %s ---- */\n%s" % (s, text))
    open(out, "w", encoding="utf-8", newline="").write("\n".join(parts))
    return out


NUM = re.compile(r"modules: (\d+) compiled, (\d+) failed")
STUB = re.compile(r"stubbing (\d+) untranscribed")
LEFT = re.compile(r"unresolved after stubbing: (\d+)\s+intersegment near-call fixups: (\d+)")
LINKED = re.compile(r"LINKED: .*?\((\d+) bytes\)")
FLAGS = re.compile(r"flags\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)")
SEGS = re.compile(r"segments\s+(\d+)\s+(\d+)")
SIZE = re.compile(r"size: rebuilt (\d+) B, shipped (\d+) B")


def main():
    want = [a.lower() for a in sys.argv[1:] if not a.startswith("-")]
    rows = []
    tmp = os.path.join(ROOT, "build16", "_merged")
    os.makedirs(tmp, exist_ok=True)
    for app, stem, srcs in APPS:
        if want and app.lower() not in want:
            continue
        print("=" * 72)
        print("%-10s stem=%-5s sources=%s" % (app, stem, ", ".join(srcs)))
        sys.stdout.flush()
        if len(srcs) > 1:
            src = merge(srcs, os.path.join(tmp, app.lower() + "_all.c"))
        else:
            src = os.path.join(layout.recon_dir(ROOT), srcs[0])
        p = subprocess.run(
            [sys.executable, os.path.join(HERE, "link16.py"), app, stem,
             "--rc", "--stubs", "--src", src],
            capture_output=True, text=True, encoding="utf-8", errors="replace")
        out = (p.stdout or "") + (p.stderr or "")
        def g(rx, n=1, cast=int, default=None):
            m = rx.search(out)
            return cast(m.group(n)) if m else default
        row = dict(
            app=app,
            ok=g(NUM, 1, int, 0), bad=g(NUM, 2, int, 0),
            stubs=g(STUB, 1, int, 0),
            left=g(LEFT, 1, int, None), fixups=g(LEFT, 2, int, None),
            built=g(SIZE, 1, int, None), shipped=g(SIZE, 2, int, None),
            fl_a=g(FLAGS, 1, str), fl_b=g(FLAGS, 2, str),
            sg_a=g(SEGS, 1, int), sg_b=g(SEGS, 2, int),
            res=resources_match(stem, app),
        )
        rows.append(row)
        print("  modules %d/%d   stubs %s   unresolved %s   fixups %s   %s"
              % (row["ok"], row["ok"] + row["bad"], row["stubs"],
                 row["left"], row["fixups"],
                 ("EXE %d B" % row["built"]) if row["built"] else "NO EXE"))
        sys.stdout.flush()

    print("\n" + "=" * 92)
    print("%-10s %7s %6s %6s %6s  %-11s %-9s %8s %8s %5s"
          % ("APP", "MODULES", "STUBS", "UNRES", "FIXUP", "SEGS r/s",
             "FLAGS r/s", "BUILT", "SHIPPED", "RES"))
    print("-" * 92)
    tot_stub = 0
    for r in rows:
        tot_stub += r["stubs"] or 0
        print("%-10s %3d/%-3d %6s %6s %6s  %-11s %-9s %8s %8s %5s"
              % (r["app"], r["ok"], r["ok"] + r["bad"], r["stubs"],
                 r["left"], r["fixups"],
                 "%s/%s" % (r["sg_a"], r["sg_b"]),
                 "%s/%s" % (r["fl_a"], r["fl_b"]),
                 r["built"] or "-", r["shipped"] or "-",
                 r["res"]))
    print("-" * 92)
    print("%d functions still to transcribe across %d apps (the stub total)"
          % (tot_stub, len(rows)))


def resources_match(stem, app):
    """Do the bound resources match the shipped binary, type for type?"""
    try:
        from codebreaker.disasm.disasm import open_ne
        import collections
        a = collections.Counter(
            r["type"] for r in open_ne(os.path.join(
                ROOT, "build16", stem + "LK", stem + ".EXE")).resources())
        b = collections.Counter(
            r["type"] for r in open_ne(os.path.join(
                ROOT, "files", "WF", app + ".EXE")).resources())
        return "yes" if a == b else "NO"
    except Exception:
        return "-"


if __name__ == "__main__":
    main()
