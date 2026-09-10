#!/usr/bin/env python3
r"""
cc16.py (win1.0) — compile 16-bit C with the ERA-CORRECT compiler for Windows 1.0:
Microsoft C Compiler 3.00 (1985), under DOSBox-X.

This is the win1.0 override of the generic `cc16` oracle. The matching-decompilation
pipeline proves a reconstructed C function is right by compiling it with the SAME
compiler+flags that built the retail binary and checking the emitted bytes against
the shipped bytes (see matchdecomp.py / COMPILER_RE.md). Windows 1.0 (1985) and its
bundled apps were built with **MS C 3.0** and MASM — NOT the MSVC 1.52c that win3.1
uses — so win1.0 needs its own oracle. MS C 3.0 is a real-mode 8086 DOS compiler
(no NTVDM on x64), so it runs under the vendored DOSBox-X, exactly like win3.1's cc16
drives MSVC 1.52c and `masmlift` drives MASM.

Toolchain (vendored from Misc/Microsoft C Compiler 3.0 into win1.0/reference/msc30):
  CL.EXE            driver — runs the passes P0→P1→P2→P3, then LINK
  P0/P1/P2/P3.EXE   preprocessor / parser / codegen / optimizer passes
  INCLUDE/, LIB/    the DOS C runtime headers + S/M/L-model libraries
Default model is SMALL (/AS) — win1.0's C apps (CALC, CLOCK, NOTEPAD, …) are small
model. The permuter overrides flags/model as it searches for a byte-match.

IMPORTANT — MS C 3.0 is a PRE-ANSI (K&R) compiler: it rejects prototype parameter
syntax. Function templates for the byte-match must be written K&R style:
    int add2(a, b)          NOT   int add2(int a, int b)
    int a; int b;
    { return a + b; }
The il.py decompiler emits ANSI prototypes for readability; a template fed to this
oracle must first be down-converted to K&R (params declared after the paren list).

    python cc16.py --selftest               # compile a trivial fn, show the .COD asm
    python cc16.py foo.c --flags "/AS /Ox"  # compile foo.c, dump obj + listing

Layer boundary: this module only RUNS the compiler and returns (obj bytes, /Fc
listing, log). Extracting code bytes from the OBJ, diffing vs the target, and the
flag/source search sit on top (objextract / segdiff / permute / oracle).
"""
import argparse
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.abspath(__file__))
DBX = r"dosbox-x"
# era-correct compiler for win1.0 (1985): MS C 3.0, vendored from Misc/.
MSC = r"reference/msc30"

CONF = r"""[sdl]
autolock=false
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=6.22
[autoexec]
mount c "{msc}"
mount d "{work}"
set PATH=C:\BIN
set INCLUDE=C:\INCLUDE
set LIB=C:\LIB
set TMP=D:\
d:
{cmd}
exit
"""


def compile_c(csrc, flags="/AS /c", stem="t", timeout=180):
    """Compile csrc with MS C 3.0 (CL driver) under DOSBox-X. Returns a dict with
    the obj bytes, the /Fc combined listing text (.COD), and the raw compiler log.

    Default flags: /AS (small model) /c (compile only, no link). /Fc emits a mixed
    source+machine-code listing; /Fo names the obj. MS C 3.0's CL accepts the same
    /F* listing/name switches as later MSC/MSVC drivers."""
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    cl = os.path.join(MSC, "BIN", "CL.EXE")
    if not os.path.exists(cl):
        sys.exit("MS C 3.0 CL.EXE not found under %s (run the vendor step)" % MSC)
    work = tempfile.mkdtemp(prefix="cc16w1_")
    with open(os.path.join(work, stem + ".C"), "w", newline="\r\n") as f:
        f.write(csrc)
    # CL runs P0..P3; /Fc<name> combined listing, /Fo<name> obj. NB: DOS COMMAND.COM
    # supports only a single `>` redirect (no `2>&1`) — CL writes errors to stdout too.
    cmd = ("CL %s /Fc%s.COD /Fo%s.OBJ %s.C > %s.LOG"
           % (flags, stem, stem, stem, stem))
    conf = os.path.join(work, "cc.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write(CONF.format(msc=MSC, work=work, cmd=cmd))
    subprocess.run([DBX, "-conf", conf, "-nolog"], capture_output=True, timeout=timeout)
    out = {"work": work}
    for ext, key in ((".OBJ", "obj"), (".COD", "cod"), (".LOG", "log")):
        p = os.path.join(work, stem + ext)
        if os.path.exists(p):
            out[key] = open(p, "rb").read() if ext == ".OBJ" else \
                open(p, encoding="latin1").read()
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?", help="C source file")
    ap.add_argument("--flags", default="/AS /c")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        # K&R style — MS C 3.0 predates ANSI prototypes (see module docstring).
        src = ("int add2(a, b)\n"
               "int a; int b;\n"
               "{ return a + b; }\n"
               "long mulshift(x)\n"
               "long x;\n"
               "{ return (x * 3) >> 2; }\n")
        r = compile_c(src, flags=args.flags)
    else:
        if not args.csrc:
            sys.exit("give a C file or --selftest")
        r = compile_c(open(args.csrc).read(), flags=args.flags)

    if "cod" in r:
        print("=== /Fc listing (codegen) ===")
        print(r["cod"][:3000])
    if "obj" in r:
        print("=== OBJ: %d bytes ===" % len(r["obj"]))
    else:
        print("=== NO OBJ produced — compiler log: ===")
        print(r.get("log", "(no log)")[:2000])
    print("# work dir: %s" % r["work"])


if __name__ == "__main__":
    main()
