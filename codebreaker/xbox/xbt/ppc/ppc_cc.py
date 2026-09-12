#!/usr/bin/env python3
"""
ppc_cc.py — compile C with the Xbox 360 XDK PowerPC compiler (the cc16 analogue).

The foundation of byte-exact C matching for the 360, exactly as cc16.py is for
Win95: to prove a reconstructed C function is right, compile it with the SAME
compiler that built the retail title and check the emitted PowerPC bytes against
the shipped bytes. That compiler is the XDK's **cl.exe … for PowerPC** (MSVC
16.00, the Xenon target). Unlike MSVC 1.52c it is a native win32 program, so it
runs directly — no DOSBox — and emits a PE/COFF object (machine 0x1F2).

This module just RUNS the compiler: given C source (+ flags) it produces the
.obj and, with /FAcs, a mixed source+machine-code listing. Extracting per-
function bytes + a relocation mask (ppc_obj.py), diffing against the shipped XEX
bytes and the permuter that searches source for a match (segdiff_ppc.py,
permute.py --arch ppc) are the layers on top.

    python ppc_cc.py --selftest                 # compile a trivial fn, show asm
    python ppc_cc.py foo.c --flags "/O2 /c"     # compile foo.c, dump obj + listing
"""
import argparse
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(os.path.dirname(ROOT), "core"))
import config as _config                                       # noqa: E402

# The XDK, discovered on this machine. Overridable via `XboxTranspiler config
# xdk <path>` (or the XBOXTRANSPILER_XDK env var) so the tool is not pinned.
DEFAULT_XDK = _config.DEFAULTS["xdk"]


def _xdk_root():
    # env override wins, else the shared project config / discovered default
    return os.environ.get("XBOXTRANSPILER_XDK") or _config.tool("xdk", DEFAULT_XDK)


def paths():
    xdk = _xdk_root()
    return {
        "xdk": xdk,
        "cl": os.path.join(xdk, "bin", "win32", "cl.exe"),
        "link": os.path.join(xdk, "bin", "win32", "link.exe"),
        "bin": os.path.join(xdk, "bin", "win32"),
        "include": os.path.join(xdk, "include", "xbox"),
        "lib": os.path.join(xdk, "lib", "xbox"),
    }


def available():
    return os.path.exists(paths()["cl"])


# Default flags: /c compile-only, /O2 to match retail optimisation, /FAcs for
# the annotated listing. The permuter overrides these while searching.
DEFAULT_FLAGS = "/c /O2"


def compile_c(csrc, flags=DEFAULT_FLAGS, stem="t", listing=True, timeout=120):
    """Compile csrc with the XDK PowerPC cl.exe. Returns dict with obj bytes,
    the /FAcs listing text (if requested), and the raw compiler log.

    Args are passed as a LIST (no shell) so leading-slash flags reach cl.exe
    verbatim — going through a shell (esp. git-bash) mangles /O2 into a path."""
    p = paths()
    if not os.path.exists(p["cl"]):
        raise RuntimeError("XDK PowerPC cl.exe not found: %s\n"
                           "set it with: XboxTranspiler config xdk <SDK path>"
                           % p["cl"])
    work = tempfile.mkdtemp(prefix="ppc_cc_")
    cpath = os.path.join(work, stem + ".c")
    with open(cpath, "w", newline="\n") as f:
        f.write(csrc)

    env = dict(os.environ)
    env["INCLUDE"] = p["include"]
    env["LIB"] = p["lib"]
    env["PATH"] = p["bin"] + os.pathsep + env.get("PATH", "")  # find c1/c2 dlls

    args = [p["cl"]] + flags.split() + [
        "/Fo" + os.path.join(work, stem + ".obj"),
        "/nologo",
    ]
    if listing:
        args += ["/FAcs", "/Fa" + os.path.join(work, stem + ".cod")]
    args += [cpath]

    r = subprocess.run(args, cwd=work, env=env, capture_output=True,
                       text=True, timeout=timeout)
    out = {"work": work, "log": (r.stdout or "") + (r.stderr or ""),
           "cmd": " ".join(args)}
    objp = os.path.join(work, stem + ".obj")
    if os.path.exists(objp):
        out["obj"] = open(objp, "rb").read()
    codp = os.path.join(work, stem + ".cod")
    if os.path.exists(codp):
        out["cod"] = open(codp, encoding="latin1", errors="replace").read()
    return out


def capture_il(csrc, flags=DEFAULT_FLAGS, stem="t", timeout=120):
    """Capture the MSVC front-end IL bundle for `csrc`.

    cl.exe is a two-stage pipeline: the C++ front end (c1xx) writes a 5-file IL
    bundle (`_CL_<hash>.{ex,gl,sy,in,db}`) to disk, then the back end (c2) reads
    it and deletes it. Compiling with `/Bd /d2nop` makes c2 abort on the bogus
    back-end flag *after* the bundle is written but *before* cl deletes it, so
    the intermediate representation can be scraped — the technique from c2-rs.

    Returns {"bundle": {ext: bytes}, "il_base": str, "log": str} or raises if
    the XDK cl.exe is unavailable. The bundle is the actual compiler IR, a finer
    seam than source for matching decompilation.
    """
    import glob
    p = paths()
    if not os.path.exists(p["cl"]):
        raise RuntimeError("XDK PowerPC cl.exe not found: %s "
                           "(config xdk <SDK path>)" % p["cl"])
    work = tempfile.mkdtemp(prefix="ppc_il_")
    cpath = os.path.join(work, stem + ".c")
    with open(cpath, "w", newline="\n") as f:
        f.write(csrc)
    env = dict(os.environ)
    env["INCLUDE"] = p["include"]
    env["LIB"] = p["lib"]
    env["PATH"] = p["bin"] + os.pathsep + env.get("PATH", "")
    # point the compiler's scratch at our work dir so the _CL_* bundle lands
    # where we can watch it (cl writes it to %TEMP%).
    env["TEMP"] = env["TMP"] = work
    # /Bd echoes each pass's argv (so we can read the -il path); /d2nop aborts c2
    # after the bundle is written but before cl deletes it; we race a watcher to
    # grab the files in the window they exist (cl cleans up on exit).
    args = [p["cl"]] + flags.split() + ["/Bd", "/d2nop", "/nologo", cpath]
    logp = os.path.join(work, "_log.txt")
    grabbed = {}
    with open(logp, "wb") as lf:
        proc = subprocess.Popen(args, cwd=work, env=env, stdout=lf, stderr=subprocess.STDOUT)
        import time
        exts = ("ex", "gl", "sy", "in", "db")

        def sweep():
            # the bundle is named <base><ext> with NO separator (_CL_<hash>ex).
            for h in glob.glob(os.path.join(work, "_CL_*")):
                for ext in exts:
                    if h.endswith(ext) and ext not in grabbed:
                        try:
                            grabbed[ext] = open(h, "rb").read()
                        except OSError:
                            pass
        while True:
            sweep()
            if proc.poll() is not None:
                sweep()
                break
            time.sleep(0.0005)
        try:
            proc.wait(timeout=timeout)
        except Exception:
            proc.kill()
    log = open(logp, encoding="latin1", errors="replace").read()
    # the c1 pass echo carries `-il <base>`: the base path is the NEXT token
    # (cl writes the bundle to the global TEMP, not the work dir).
    il_base = None
    toks = log.replace("\n", " ").split()
    for i, tok in enumerate(toks):
        if tok == "-il" and i + 1 < len(toks):
            il_base = toks[i + 1].strip('"')
            break
        if tok.startswith("-il") and len(tok) > 3:
            il_base = tok[3:].strip('"')
            break
    bundle = dict(grabbed)                          # what the watcher raced out
    for ext in ("ex", "gl", "sy", "in", "db"):      # backfill any survivors (<base><ext>)
        if ext in bundle:
            continue
        cands = ([il_base + ext] if il_base else []) \
            + glob.glob(os.path.join(work, "_CL_*" + ext))
        for h in cands:
            if os.path.exists(h):
                bundle[ext] = open(h, "rb").read()
                break
    return {"work": work, "log": log, "il_base": il_base, "bundle": bundle}


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csrc", nargs="?", help="C source file")
    ap.add_argument("--flags", default=DEFAULT_FLAGS)
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("--capture-il", action="store_true",
                    help="capture the MSVC IL bundle (front-end IR) instead of compiling")
    args = ap.parse_args()

    if not available():
        print("XDK PowerPC compiler not found at %s" % paths()["cl"])
        print("set it with:  XboxTranspiler config xdk \"<SDK path>\"")
        sys.exit(2)

    if args.capture_il:
        src = open(args.csrc).read() if args.csrc else "int f(int a){return a+1;}\n"
        r = capture_il(src, flags=args.flags)
        print("IL bundle base: %s" % r["il_base"])
        if r["bundle"]:
            for ext, data in sorted(r["bundle"].items()):
                print("  _CL_*.%-2s  %d bytes" % (ext, len(data)))
        else:
            print("  (no IL bundle captured — compiler log:)")
            print(r["log"][:1500])
        return

    if args.selftest:
        src = ("int add2(int a, int b) { return a + b; }\n"
               "long madd(long x, long y) { return x * 3 + y; }\n")
        r = compile_c(src, flags="/c /O2")
    elif args.csrc:
        r = compile_c(open(args.csrc).read(), flags=args.flags)
    else:
        ap.error("give a C file or --selftest")

    if "cod" in r:
        print("=== /FAcs listing (codegen) ===")
        print(r["cod"][:3000])
    if "obj" in r:
        print("=== OBJ: %d bytes (PowerPC COFF) ===" % len(r["obj"]))
        print("# work dir: %s" % r["work"])
    else:
        print("=== NO OBJ produced — compiler log: ===")
        print(r.get("log", "(no log)")[:2000])
        sys.exit(1)


if __name__ == "__main__":
    main()
