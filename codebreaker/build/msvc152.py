"""
cc16.py — compile 16-bit C with the PERIOD compiler (MSVC 1.52c) under DOSBox-X.

The foundation of byte-exact C matching (matching decompilation): to prove a
reconstructed C function is right, compile it with the SAME compiler+flags that
built retail Win98 and check the emitted bytes against the shipped bytes. That
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
import sys

from codebreaker.base import settings as _settings
DBX = _settings.get("dosbox_x") or "dosbox-x"                  # settings.json: dosbox_x
MSVC = _settings.get("msvc152") or ""                          # settings.json: msvc152 (Microsoft C/C++ 8.00c)

def compile_c(csrc, flags="/AM /c", stem="t", timeout=180):
    """Compile csrc with MSVC 1.52c under DOSBox-X (build/dos_tools.py). Returns dict with obj
    bytes, the /Fc listing text, and the raw compiler log; "work" is the folder it ran in (kept for
    inspection; the caller cleans it up)."""
    from codebreaker.build import dos_tools
    if not os.path.exists(DBX):
        sys.exit("dosbox-x not found: %s" % DBX)
    if not os.path.exists(os.path.join(MSVC, "BIN", "CL.EXE")):
        sys.exit("MSVC 1.52c CL.EXE not found under %s" % MSVC)
    # /Fc emits a mixed source+machine-code listing; /Fo the obj.
    cmd = "CL %s /Fc%s.COD /Fo%s.OBJ %s.C > %s.LOG 2>&1" % (flags, stem, stem, stem, stem)
    _ok, made, work = dos_tools.run(
        {"c": MSVC}, [cmd], [stem + ext for ext in (".OBJ", ".COD", ".LOG")], timeout=timeout,
        files={stem + ".C": csrc}, cpu={"cputype": "386", "core": "normal", "cycles": "max"}, dos={"ver": "6.22"},
        variables={"PATH": r"C:\BIN", "INCLUDE": r"C:\INCLUDE", "LIB": r"C:\LIB", "TMP": "D:\\"})
    out = {"work": work}
    for ext, key in ((".OBJ", "obj"), (".COD", "cod"), (".LOG", "log")):
        if stem + ext in made:
            out[key] = made[stem + ext] if ext == ".OBJ" else made[stem + ext].decode("latin1")
    return out


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
