#!/usr/bin/env python3
"""
scoreboard.py — run the period-build pipeline across every hand-reconstructed
app and report, per file, whether MS C 3.0 accepts it and (if not) the first
error. This turns "does the full project compile?" into a concrete punch-list.

Pipeline per source (the same one proven on clock.c):
    1. copy reconstructed/<app>.c  ->  build16/SCORE/<APP>.C
    2. ansi2knr.py           (ANSI prototypes/defs -> K&R; (void) -> ())
    3. msc /u /AS /Gsw /Oas /Zpe /Ze /NT _TEXT <app>;     (compile only)

One DOSBox-X session compiles all of them; each compile's errors go to
<APP>.ERR. We then read every .ERR and classify:
    OK        - produced an .OBJ, no error lines
    ERROR     - first error line shown (the thing to fix next)

Comment hygiene and _asm extraction are NOT auto-applied here — a file that
needs them shows up as ERROR with the tell-tale line, which is the point: the
scoreboard says exactly what each file still needs.

Usage:  python scoreboard.py
"""
import os, re, sys, shutil, subprocess
from codebreaker.foundation import layout

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
REC  = layout.recon_dir(ROOT)
B16  = os.path.join(ROOT, "build16")
WORK = os.path.join(B16, "SCORE")
DBX  = os.path.join(ROOT, "refs", "tools", "dosbox-x", "bin", "x64", "Release",
                    "dosbox-x.exe")

# main app modules (the _map.c files are evidence maps, not compilable units)
APPS = ["calc", "calendar", "cardfile", "clipbrd", "clock", "control",
        "notepad", "paint_core", "paint_tools", "reversi", "spooler",
        "terminal", "write_dlg", "write_wnd"]

CONF = r"""[dosbox]
memsize=32
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=5.00
[sdl]
autolock=false
[autoexec]
mount c {c_root}
c:
set PATH=C:\BIN30;C:\BIN
set INCLUDE=C:\INC30
set LIB=C:\LIB30
set TMP=C:\SCORE
cd C:\SCORE
call SCORE.BAT
exit
"""

ERRLINE = re.compile(r": (?:error|fatal|syntax error)", re.I)


def prep():
    if os.path.isdir(WORK):
        shutil.rmtree(WORK)
    os.makedirs(WORK)
    present = []
    for a in APPS:
        src = os.path.join(REC, a + ".c")
        if not os.path.exists(src):
            print("  (missing: %s.c)" % a); continue
        dst = os.path.join(WORK, a.upper() + ".C")
        shutil.copyfile(src, dst)
        subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"), dst],
                       capture_output=True)
        present.append(a)
    # the batch: compile each, capturing errors per file
    lines = ["@echo off", "echo scoreboard > score.log"]
    for a in present:
        u = a.upper()
        lines += [
            "del %s.obj" % u,
            "echo === %s === >> score.log" % u,
            "msc /u /AS /Gsw /Oas /Zpe /Ze /NT _TEXT %s; > %s.out 2> %s.err" % (u, u, u),
            "if exist %s.obj echo   OBJ-OK >> score.log" % u,
            "if not exist %s.obj echo   NO-OBJ >> score.log" % u,
        ]
    lines += ["echo DONE >> score.log"]
    open(os.path.join(WORK, "SCORE.BAT"), "w").write("\r\n".join(lines) + "\r\n")
    return present


def run():
    conf = os.path.join(B16, "_score.conf")
    open(conf, "w").write(CONF.format(c_root=B16))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=1800, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** DOSBox-X timed out — results may be partial")


def report(present):
    print("\n%-14s  %-8s  %s" % ("MODULE", "RESULT", "FIRST ERROR (if any)"))
    print("-" * 78)
    ok = 0
    for a in present:
        u = a.upper()
        errf = os.path.join(WORK, u + ".ERR")
        objf = os.path.join(WORK, u + ".OBJ")
        has_obj = os.path.exists(objf) and os.path.getsize(objf) > 0
        first = ""
        if os.path.exists(errf):
            txt = open(errf, encoding="cp437", errors="replace").read()
            for ln in txt.splitlines():
                if ERRLINE.search(ln):
                    first = ln.strip(); break
        if has_obj and not first:
            ok += 1
            print("%-14s  %-8s" % (a, "OK"))
        else:
            print("%-14s  %-8s  %s" % (a, "ERROR", first[:60]))
    print("-" * 78)
    print("%d/%d modules compile clean" % (ok, len(present)))


if __name__ == "__main__":
    print("preparing %d modules (copy + ansi2knr)..." % len(APPS))
    present = prep()
    print("compiling all in one DOSBox-X session (this takes a few minutes)...")
    run()
    report(present)
