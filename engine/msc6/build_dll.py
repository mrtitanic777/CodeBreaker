#!/usr/bin/env python3
r"""build_dll.py -- whole-DLL two-toolchain build + per-segment oracle verify.

The shipped 16-bit Win95 DLLs were compiled with MSC 6.0a's `cl` (byte-exact code)
and linked with the Windows SDK's LINK 5.60 into the NE. Our MSC 6.0a here is
compile-only (no LINK/libs), so the build uses BOTH toolchains:

    compile each module  ->  MSC 6.0a  cl /c /AM /G2 /Gsw /Ox   (byte-exact code)
    link the objs        ->  MSVC 1.52c LINK 5.60  @lz.lnk       (NE assembly, DLL runtime)

This closes the loop that single-function refine/mocklink cannot: DGROUP-direct
globals get their fixed offsets from the linked DATA segment, intra-segment (sibling)
calls get their real rel16, and switch tables land at the segment tail -- so those
tiers can finally be verified IN CONTEXT against the disc oracle, per segment.

    python -m engine.msc6.build_dll <recon_dir> [--oracle <dll>] [--work <dir>]

<recon_dir> holds the reconstruction: the module .c files, lz.lnk, lzexpand.def,
and the self-contained headers. Oracle defaults to the canonical disc extraction.
"""
import os
import re
import sys
import struct
import shutil
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))                 # CodeBreaker/


def _cfg(key, default=""):
    try:
        from engine.foundation import config
        return config.load_config().get(key, default) or default
    except Exception:
        return default


def _paths():
    dbx = _cfg("dosbox_x")
    tc = _cfg("msc6_toolchain") or os.path.join(ROOT, "binaries", "toolchains", "period")
    return (dbx,
            os.path.join(tc, "compilers", "msc-6.0a"),      # cl (compile-only)
            os.path.join(tc, "compilers", "msvc-1.52c"))    # link + rc + libs


def _win(p):
    return p.replace("/", "\\")


def build(recon_dir, work_dir=None, modules=None):
    """Stage the reconstruction, build the DLL with the two toolchains, return its path."""
    dbx, msc6, msvc = _paths()
    if not (dbx and os.path.exists(dbx)):
        raise RuntimeError("dosbox_x not configured")
    work = work_dir or tempfile.mkdtemp(prefix="lzwhole_")
    os.makedirs(work, exist_ok=True)
    for fn in os.listdir(recon_dir):
        if fn.lower().endswith((".c", ".h", ".lnk", ".def")):
            shutil.copy2(os.path.join(recon_dir, fn), work)
    ws = os.path.join(msvc, "BIN", "WINSTUB.EXE")             # for the .def STUB directive
    if os.path.exists(ws):
        shutil.copy2(ws, work)
    if modules is None:                                       # module order from lz.lnk
        lnk = open(os.path.join(work, "lz.lnk")).read()
        modules = re.findall(r"([A-Za-z_]\w*)\.obj", lnk)
    with open(os.path.join(work, "BUILD.BAT"), "w", newline="") as f:
        f.write("@echo off\r\ndel *.obj\r\ndel lzexpand.dll\r\ndel done.txt\r\n")
        for m in modules:
            f.write("cl /c /AM /G2 /Gsw /Ox %s.c >> CL.TXT\r\n" % m)
        f.write("link @lz.lnk > LINK.TXT\r\nrc lzexpand.dll > RC.TXT\r\necho DONE > done.txt\r\n")
    with open(os.path.join(work, "b.conf"), "w", newline="") as f:
        f.write("[cpu]\r\ncycles=max\r\n[autoexec]\r\n")
        f.write("mount c %s\r\nmount e %s\r\nmount d %s\r\n" % (_win(msc6), _win(msvc), _win(work)))
        f.write("set PATH=C:\\BIN;E:\\BIN\r\nset LIB=E:\\LIB\r\n")
        f.write("d:\r\ncall BUILD.BAT\r\nexit\r\n")
    subprocess.run([dbx, "-conf", os.path.join(work, "b.conf"), "-fastlaunch"],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=600)
    dll = os.path.join(work, "lzexpand.dll")
    return dll if os.path.exists(dll) else None


def _segments(path):
    d = open(path, "rb").read(); lfa = struct.unpack_from("<I", d, 0x3c)[0]
    n = struct.unpack_from("<H", d, lfa + 0x1c)[0]
    st = struct.unpack_from("<H", d, lfa + 0x22)[0] + lfa
    al = struct.unpack_from("<H", d, lfa + 0x32)[0] or 9
    out = []
    for i in range(n):
        s, l, fl, mn = struct.unpack_from("<HHHH", d, st + i * 8)
        fo = s << al
        out.append((l, d[fo:fo + l]))
    return out


def compare(dll, oracle):
    """Per-segment build-vs-oracle report; returns (identical, total)."""
    B, O = _segments(dll), _segments(oracle)
    print("built segs=%d  oracle segs=%d  (built=%d B, oracle=%d B)"
          % (len(B), len(O), os.path.getsize(dll), os.path.getsize(oracle)))
    ident = 0
    for i in range(max(len(B), len(O))):
        bl = B[i][0] if i < len(B) else None
        ol = O[i][0] if i < len(O) else None
        tag = ""
        if i < len(B) and i < len(O):
            if B[i][1] == O[i][1]:
                tag = "IDENTICAL"; ident += 1
            elif bl == ol:
                tag = "same-len-differ"
            else:
                tag = "diff-len"
        print("  seg%-2d built=%-5s oracle=%-5s %s"
              % (i + 1, bl if bl is not None else "-", ol if ol is not None else "-", tag))
    print("=> %d / %d segments byte-identical" % (ident, len(O)))
    return ident, len(O)


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    recon = argv[0]
    oracle = argv[argv.index("--oracle") + 1] if "--oracle" in argv else \
        os.path.join(ROOT, "binaries", "win95", "oracle", "lzexpand.dll")
    work = argv[argv.index("--work") + 1] if "--work" in argv else None
    dll = build(recon, work)
    if not dll:
        print("build produced no DLL (check CL.TXT/LINK.TXT in the work dir)")
        return 1
    print("built: %s" % dll)
    if os.path.exists(oracle):
        compare(dll, oracle)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
