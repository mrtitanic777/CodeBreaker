#!/usr/bin/env python3
"""
splitbuild.py — full segment-split build of one app, end to end:
    segsplit -> ansi2knr each piece -> compile each module under MS C 3.0.
Reports, per segment module, OBJ produced / first error. Proves the
multi-segment reconstruction compiles the way Microsoft shipped it.

Usage:  python splitbuild.py <reconstructed/app.c> <STEM>
        (STEM is a short 8.3-safe prefix, e.g. CAL for calendar)
"""
import os, re, sys, shutil, subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
B16  = os.path.join(ROOT, "build16")
DBX  = os.path.join(ROOT, "refs", "tools", "dosbox-x", "bin", "x64", "Release",
                    "dosbox-x.exe")

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
set TMP=C:\{wd}
cd C:\{wd}
call BUILD.BAT
exit
"""

RECIPE = "msc /u /AS /Gsw /Oas /Zpe /Ze /NT _TEXT %s; > %s.out 2> %s.err"


def main():
    src, stem = sys.argv[1], sys.argv[2].upper()
    wd = stem + "SP"
    outdir = os.path.join(B16, wd)
    if os.path.isdir(outdir):
        shutil.rmtree(outdir)
    os.makedirs(outdir)
    # 1. hoist declarations to block tops (C 3.0 needs decls before statements),
    #    on a working copy, then split THAT into segment modules.
    work = os.path.join(outdir, "_src.c")
    shutil.copyfile(src, work)
    subprocess.run([sys.executable, os.path.join(HERE, "hoistdecl.py"), work],
                   check=True)
    subprocess.run([sys.executable, os.path.join(HERE, "segsplit.py"), work,
                    outdir, "--stem", stem], check=True)
    os.remove(work)
    # 2. ansi2knr each emitted .C and .H
    mods = []
    for fn in sorted(os.listdir(outdir)):
        p = os.path.join(outdir, fn)
        if fn.lower().endswith((".c", ".h")):
            subprocess.run([sys.executable, os.path.join(HERE, "ansi2knr.py"), p],
                           capture_output=True)
        if fn.lower().endswith(".c"):
            mods.append(os.path.splitext(fn)[0])
    # 3. batch to compile each module
    lines = ["@echo off", "echo split build > build.log"]
    for m in mods:
        lines += [
            "del %s.obj" % m,
            RECIPE % (m, m, m),
            "echo === %s === >> build.log" % m,
            "if exist %s.obj echo   OBJ-OK >> build.log" % m,
            "if not exist %s.obj echo   NO-OBJ >> build.log" % m,
        ]
    lines.append("echo DONE >> build.log")
    open(os.path.join(outdir, "BUILD.BAT"), "w").write("\r\n".join(lines) + "\r\n")
    # 4. run
    conf = os.path.join(B16, "_sb.conf")
    open(conf, "w").write(CONF.format(c_root=B16, wd=wd))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=1200, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** timed out")
    # 5. report
    print("\n%-10s %-7s %s" % ("MODULE", "RESULT", "FIRST ERROR"))
    print("-" * 70)
    ok = 0
    ERR = re.compile(r": (?:error|fatal)|Stack overflow|syntax error", re.I)
    for m in mods:
        objf = os.path.join(outdir, m + ".OBJ")
        errf = os.path.join(outdir, m + ".ERR")
        has = os.path.exists(objf) and os.path.getsize(objf) > 0
        first = ""
        if os.path.exists(errf):
            for ln in open(errf, encoding="cp437", errors="replace"):
                if ERR.search(ln):
                    first = ln.strip(); break
        if has and not first:
            ok += 1; print("%-10s %-7s" % (m, "OK"))
        else:
            print("%-10s %-7s %s" % (m, "ERROR", first[:44]))
    print("-" * 70)
    print("%d/%d segment modules compile clean" % (ok, len(mods)))


if __name__ == "__main__":
    main()
