#!/usr/bin/env python3
"""
cc16.py — compile 16-bit C with the PERIOD compiler (MSVC 1.52c) under DOSBox-X.

The foundation of byte-exact C matching (matching decompilation): to prove a
reconstructed C function is right, compile it with the SAME compiler+flags that
built retail Win95 and check the emitted bytes against the shipped bytes. That
requires the real compiler — Microsoft Visual C++ 1.52c, the Win16 (MS C8)
toolchain — which is a 16-bit DOS program (no NTVDM on x64), so it runs under the
vendored DOSBox-X, exactly like `masmlift`/`link16` drive MASM.

This module just RUNS the compiler: given C source (+ flags), it produces the
.OBJ and the `/Fc` assembly listing. Extracting the code bytes from the OBJ,
diffing against the target, and the permuter that searches flags/source for a
byte-match are the layers built on top (see docs / COMPILER_RE.md's codegen
oracle). First job: confirm MSVC 1.52c runs here and emits code.

    python cc16.py --selftest              # compile a trivial fn, show the asm
    python cc16.py foo.c --flags "/AM /Ox" # compile foo.c, dump obj + listing
"""
import argparse
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.abspath(__file__))
DBX = r"dosbox-x"
MSVC = r"reference/msvc152c"

CONF = r"""[sdl]
autolock=false
[cpu]
cputype=386
core=normal
cycles=max
[dos]
ver=6.22
[autoexec]
mount c "{msvc}"
mount d "{work}"
set PATH=C:\BIN
set INCLUDE=C:\INCLUDE
set LIB=C:\LIB
set TMP=D:\
d:
{cmd}
exit
"""


def compile_c(csrc, flags="/AM /c", stem="t", timeout=180):
    """Compile csrc with MSVC 1.52c under DOSBox-X. Returns dict with obj bytes,
    the /Fc listing text, and the raw compiler log."""
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    if not os.path.exists(os.path.join(MSVC, "BIN", "CL.EXE")):
        sys.exit("MSVC 1.52c CL.EXE not found under %s" % MSVC)
    work = tempfile.mkdtemp(prefix="cc16_")
    try:
        with open(os.path.join(work, stem + ".C"), "w", newline="\r\n") as f:
            f.write(csrc)
        # /Fc emits a mixed source+machine-code listing; /Fo the obj.
        cmd = ("CL %s /Fc%s.COD /Fo%s.OBJ %s.C > %s.LOG 2>&1"
               % (flags, stem, stem, stem, stem))
        conf = os.path.join(work, "cc.conf")
        with open(conf, "w", newline="\r\n") as f:
            f.write(CONF.format(msvc=MSVC, work=work, cmd=cmd))
        subprocess.run([DBX, "-conf", conf, "-nolog"], capture_output=True,
                       timeout=timeout)
        out = {"work": work}
        for ext, key in ((".OBJ", "obj"), (".COD", "cod"), (".LOG", "log")):
            p = os.path.join(work, stem + ext)
            if os.path.exists(p):
                out[key] = open(p, "rb").read() if ext == ".OBJ" else \
                    open(p, encoding="latin1").read()
        return out
    finally:
        pass  # keep work dir for inspection; caller cleans if desired


# Max compiles per DOSBox boot. Each compile is one `CL` line in the [autoexec]
# section, which has a small fixed buffer — a big batch overflows it
# (`E_Exit: Autoexec.bat file overflow`), same limit masmlift chunks around.
COMPILE_BATCH = max(1, int(os.environ.get("WT_CC_BATCH", "40")))


def compile_many(items, flags="/AM /c", timeout=None):
    """Compile MANY (stem, csrc) pairs in ONE DOSBox boot instead of one boot per
    file — the DOSBox boot dominates cost, so this is the ~40x win (mirrors
    masmlift.assemble_many's cold-boot batch). Stems must be unique and DOS 8.3-safe
    (callers use C0..CN). Returns {stem: {obj?, cod?, log?}}; a stem missing 'obj'
    failed to compile. Chunks at COMPILE_BATCH so the [autoexec] buffer can't overflow."""
    if not items:
        return {}
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    if not os.path.exists(os.path.join(MSVC, "BIN", "CL.EXE")):
        sys.exit("MSVC 1.52c CL.EXE not found under %s" % MSVC)
    if len(items) > COMPILE_BATCH:                     # chunk oversized batches
        merged = {}
        for base in range(0, len(items), COMPILE_BATCH):
            merged.update(compile_many(items[base:base + COMPILE_BATCH], flags, timeout))
        return merged
    if timeout is None:
        timeout = 120 + 3 * len(items)
    work = tempfile.mkdtemp(prefix="cc16_")
    try:
        for stem, csrc in items:
            with open(os.path.join(work, stem + ".C"), "w", newline="\r\n") as f:
                f.write(csrc)
        cmd = "\n".join("CL %s /Fc%s.COD /Fo%s.OBJ %s.C > %s.LOG 2>&1"
                        % (flags, s, s, s, s) for s, _ in items)
        conf = os.path.join(work, "cc.conf")
        with open(conf, "w", newline="\r\n") as f:
            f.write(CONF.format(msvc=MSVC, work=work, cmd=cmd))
        env = dict(os.environ, SDL_VIDEODRIVER="dummy")
        subprocess.run([DBX, "-conf", conf, "-nolog"], capture_output=True,
                       timeout=timeout, env=env)
        results = {}
        for stem, _src in items:
            d = {}
            for ext, key in ((".OBJ", "obj"), (".COD", "cod"), (".LOG", "log")):
                p = os.path.join(work, stem + ext)
                if os.path.exists(p):
                    d[key] = open(p, "rb").read() if ext == ".OBJ" else \
                        open(p, encoding="latin1").read()
            results[stem] = d
        return results
    finally:
        try:
            shutil.rmtree(work, ignore_errors=True)
        except Exception:
            pass


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?", help="C source file")
    ap.add_argument("--flags", default="/AM /c")
    ap.add_argument("--selftest", action="store_true")
    args = ap.parse_args()

    if args.selftest:
        src = ("int add2(int a, int b) { return a + b; }\n"
               "long mulshift(long x) { return (x * 3) >> 2; }\n")
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
