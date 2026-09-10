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

from codebreaker.foundation import paths
HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = paths.repo_root()                                      # CodeBreaker/ (canonical resolver)


def _cfg(key, default=""):
    try:
        from codebreaker.foundation import config
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


def _level0(fb, solved, do_c_verify):
    """Resolve one function to a byte-exact reconstruction, Level-0 FIRST. Returns
    (kind, source, exact) or ('gap', None, False):
      - a segment-switch thunk -> its MASM stub (byte-exact by round-trip, no oracle);
      - a banked C shape already in solved.json -> reuse without recompiling;
      - otherwise decompile with msc6dec and (optionally) prove it via the frame-matched oracle.
    This is the fast-path: the bank and the template short-circuit the compiler before any
    decompilation or code generation is attempted."""
    from codebreaker.inverse_engine import seg_thunk
    t = seg_thunk.verify(fb)
    if t is not None:
        return "asm", seg_thunk.emit_asm(t), True
    from codebreaker.inverse_engine.auto_sweep import signature
    sig = signature(fb)
    if sig in solved and not str(solved[sig]).startswith("SEG-SWITCH"):
        return "c", solved[sig], True                      # Level-0 hit: pre-verified banked C
    from codebreaker.inverse_engine import msc6dec
    from codebreaker.sdk import reconstruct as R
    try:
        src, meta = msc6dec.invert(fb, syms=R._synth_syms(fb))
    except Exception:
        src = None
    if not src:
        return "gap", None, False
    if not do_c_verify:
        return "c", src, None                              # candidate, exactness deferred
    from codebreaker.inverse_engine import oracle_router as router
    return "c", src, router.byte_exact(fb, src).get("exact", False)


def reconstruct_module(dll_path, verify=True):
    """Whole-module reconstruction from the Level-0 bank + templates. For each framed function
    resolve a byte-exact source (thunk MASM / banked or decompiled C) and report per-segment
    coverage -- the inputs a build assembles from. Returns (report, pieces)."""
    import json
    from codebreaker.sdk import reconstruct as R
    from codebreaker.format_loaders import ne
    from codebreaker.inverse_engine.auto_sweep import signature
    solved_path = paths.engine_dir("inverse_engine", "_sweepdb", "solved.json")
    solved = json.load(open(solved_path)) if os.path.exists(solved_path) else {}
    d = open(dll_path, "rb").read(); lfa = struct.unpack_from("<I", d, 0x3c)[0]
    f = ne.NEFile.from_buffer(d, lfa, dll_path)
    segb = {s["idx"]: d[s["file_off"]:s["file_off"] + s["length"]]
            for s in f.segments() if s["kind"] == "CODE"}
    from codebreaker.inverse_engine import seg_thunk
    rep = R.analyze(dll_path)
    pieces = []; pend = []
    for sg in rep["segments"]:
        sb = segb.get(sg["idx"], b"")
        for fn in sg["funcs"]:
            if not fn.get("framed"):
                pieces.append((sg["idx"], fn["off"], "leaf", None, None)); continue
            fb = bytes(sb[fn["off"]:fn["off"] + fn["size"]])
            t = seg_thunk.verify(fb)
            if t is not None:                              # thunk template -> MASM (byte-exact)
                pieces.append((sg["idx"], fn["off"], "asm", seg_thunk.emit_asm(t), True)); continue
            if fn.get("status") == "needs-link":           # intra-seg calls: verifiable only linked
                pieces.append((sg["idx"], fn["off"], "needs-link", fn.get("c"), None)); continue
            kind, src, exact = _level0(fb, solved, do_c_verify=False)
            if kind == "c" and exact is None and verify:
                pend.append((len(pieces), fb, src))
            pieces.append((sg["idx"], fn["off"], kind, src, exact))
    if verify and pend:                                    # one oracle batch per compiler
        from codebreaker.inverse_engine import oracle_router as router
        for (i, _fb, _s), r in zip(pend, router.byte_exact_batch([(fb, s) for _i, fb, s in pend])):
            pieces[i] = pieces[i][:4] + (r.get("exact", False),)
    framed = [p for p in pieces if p[2] != "leaf"]
    thunks = sum(1 for p in framed if p[2] == "asm")
    cexact = sum(1 for p in framed if p[2] == "c" and p[4])
    needslink = sum(1 for p in framed if p[2] == "needs-link")
    gaps = sum(1 for p in framed if p[2] == "gap" or (p[2] == "c" and not p[4]))
    report = {"module": os.path.basename(dll_path), "framed": len(framed),
              "thunks": thunks, "c_exact": cexact, "needs_link": needslink, "gaps": gaps,
              "solved": thunks + cexact,
              "coverage": (thunks + cexact) / max(1, len(framed)),
              "link_potential": (thunks + cexact + needslink) / max(1, len(framed))}
    return report, pieces


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
    if "--reconstruct" in argv:                            # whole-module Level-0 coverage
        rep, _pieces = reconstruct_module(argv[0])
        print("== reconstruct %s ==" % rep["module"])
        print("   framed functions : %d" % rep["framed"])
        print("   thunks (MASM)    : %d" % rep["thunks"])
        print("   C byte-exact     : %d" % rep["c_exact"])
        print("   needs-link (C)   : %d  (verifiable only in the whole-module link)" % rep["needs_link"])
        print("   gaps             : %d" % rep["gaps"])
        print("   => solved %d / %d standalone (%.0f%%);  link potential %.0f%%"
              % (rep["solved"], rep["framed"], 100 * rep["coverage"], 100 * rep["link_potential"]))
        return 0
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
