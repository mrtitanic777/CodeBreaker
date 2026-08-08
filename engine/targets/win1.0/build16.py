#!/usr/bin/env python3
"""
build16.py — THE standard build for this project: a real 16-bit Windows 1.01
             executable, using the period toolchain under DOSBox-X.

*** THIS IS THE ONLY BUILD THAT COUNTS ***

The gcc `-fsyntax-only` check in build.py is a fast well-formedness lint and
nothing more — it never emits code, and it targets a 64-bit machine. We do not
ship, or care about, a 64-bit build. The deliverable is a 16-bit NE executable
built with the tools Microsoft actually used, and this driver produces exactly
that.

THE TOOLCHAIN, ESTABLISHED EMPIRICALLY (see win1-period-build memory):
  * Microsoft C 3.00  — NOT 4.0. C 4.0's object records silently fail to link
    with the period linker; C 3.0's link. C 3.0 needs /Ze to accept the
    near/far/pascal keywords WINDOWS.H uses.
  * Microsoft MASM 4.00 — timeline-correct (Oct 1985) and assembles cleanly.
  * LINK4.EXE (the SDK's NE linker) + RC.EXE (resource compiler).
  * Run headless in DOSBox-X (portable build) with cycles=max and the dummy
    SDL video driver, so no window and no admin are needed.

The compile recipe per C module (from the SDK's own makefiles) is:
    msc /u /AS /Gsw /Oas /Zpe /Ze /NT <segname> <module>;
each .ASM is:
    masm -D?PLM -D?WIN <module>.asm;
and the link uses the app's response file with `slibw swlibc/NOD` as libraries
(a Windows app takes the WINDOWS C runtime with no default-library search — the
DOS crt0 and the Windows winstart otherwise both get pulled and conflict).

VALIDATED: CARDFILE built from Microsoft's (typo-corrected) source came out
36,464 bytes vs the shipped 36,528 — every NE header field identical, the data
segment byte-identical, code within 0.05%.

Usage:
    python build16.py WORKDIR BATCH.BAT
        Runs the batch inside DOSBox-X. The workdir is a subdirectory
        of build16/ (the mounted C: root). Prints the build log and returns 0
        if it produced no linker/compiler error markers.

    python build16.py --env
        Print the toolchain layout and verify every tool is present.
"""
import os, sys, subprocess, re

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
B16 = os.path.join(ROOT, "build16")               # the mounted C: root
DBX = os.path.join(ROOT, "refs", "tools", "dosbox-x", "bin", "x64", "Release",
                   "dosbox-x.exe")

# C: layout inside build16/. BIN30/INC30/LIB30 are the C 3.0 environment.
CONF = r"""[dosbox]
memsize=32
title=win101-build

[cpu]
cputype=386
core=normal
cycles=max

[dos]
ver=5.00

[render]
scaler=none

[sdl]
autolock=false

[autoexec]
mount c {c_root}
c:
set PATH=C:\BIN30;C:\BIN
set INCLUDE=C:\INC30
set LIB=C:\LIB30
set TMP=C:\{workdir}
cd C:\{workdir}
call {batch}
exit
"""

TOOLS = {
    "BIN30/MSC.EXE":  "Microsoft C 3.0 compiler driver",
    "BIN30/P1.EXE":   "C 3.0 compiler pass 1",
    "BIN30/P2.EXE":   "C 3.0 compiler pass 2",
    "BIN30/LINK4.EXE": "NE linker (SDK)",
    "BIN30/RC.EXE":   "resource compiler (SDK)",
    "BIN/MASM.EXE":   "Macro Assembler 4.0",
    "INC30/WINDOWS.H": "the 1.01 SDK header",
    "INC30/CMACROS.INC": "assembler macros",
    "LIB30/SLIBW.LIB": "Windows API import library",
    "LIB30/SWLIBC.LIB": "Windows C runtime",
    "LIB/CMACROS.INC": "where the ASM's `\\lib\\cmacros.inc` resolves (C:\\LIB)",
}

# Link/compile output that means the build did NOT cleanly succeed.
BAD = re.compile(r"Unresolved external|defined more than once|error|cannot find|"
                 r"cannot open|FATAL|syntax error", re.I)


def check_env():
    print("DOSBox-X : %s  %s" % (DBX, "OK" if os.path.exists(DBX) else "*** MISSING ***"))
    print("C: root  : %s" % B16)
    missing = 0
    for rel, desc in TOOLS.items():
        p = os.path.join(B16, rel.replace("/", os.sep))
        ok = os.path.exists(p)
        missing += not ok
        print("  %-20s %-34s %s" % (rel, desc, "OK" if ok else "*** MISSING ***"))
    return missing == 0


def run(workdir, batch, timeout=600):
    conf = os.path.join(B16, "_build.conf")
    with open(conf, "w") as f:
        f.write(CONF.format(c_root=B16, workdir=workdir, batch=batch))
    env = dict(os.environ, SDL_VIDEODRIVER="dummy")
    try:
        subprocess.run([DBX, "-conf", conf, "-fastlaunch"], cwd=B16, env=env,
                       timeout=timeout, capture_output=True)
    except subprocess.TimeoutExpired:
        print("*** DOSBox-X timed out after %ds — build likely incomplete" % timeout)
    # the batch writes its log; find the newest .log in the workdir
    wd = os.path.join(B16, workdir)
    logs = [os.path.join(wd, f) for f in os.listdir(wd) if f.lower().endswith(".log")]
    if not logs:
        print("(no .log produced by the batch)")
        return 1
    log = max(logs, key=os.path.getmtime)
    text = open(log, encoding="cp437", errors="replace").read()
    print(text)
    bad = [l for l in text.splitlines() if BAD.search(l)]
    if bad:
        print("\n*** %d problem line(s) flagged:" % len(bad))
        for l in bad[:12]:
            print("   " + l.strip())
        return 1
    return 0


def main():
    if "--env" in sys.argv:
        sys.exit(0 if check_env() else 1)
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(2)
    if not os.path.exists(DBX):
        print("DOSBox-X not found at %s — run `python build16.py --env`" % DBX)
        sys.exit(1)
    sys.exit(run(sys.argv[1], sys.argv[2]))


if __name__ == "__main__":
    main()
