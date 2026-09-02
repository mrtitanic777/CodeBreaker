#!/usr/bin/env python3
r"""ghidra_batch.py -- batch-decompile 32-bit+ PE/LE binaries with Ghidra headless,
applying the period Win95 GDTs (BuildGDT output) + the accuracy pipeline
(AggressivePre enables the heavy analyzers; DecompilePipeline does the function
cross-check + GDT apply + decompile-all).

    python engine/ghidra/ghidra_batch.py <input_dir_or_file> <out_dir> [--limit N]

GDT is chosen per binary: VxD/driver (.vxd/.386/LE, or 'vxd' in the name) -> DDK
archive; every other 32-bit PE -> the period Win32 SDK archive. 16-bit NE binaries
are skipped -- those are CodeBreaker's byte-exact msc6dec track, not this.

Config (config.json keys or env): ghidra_home / GHIDRA_HOME, java_home / JAVA_HOME.
"""
import os
import sys
import struct
import subprocess
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))                 # CodeBreaker/
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


def _cfg(key, default=""):
    try:
        from engine.foundation import config
        return config.load_config().get(key, default) or default
    except Exception:
        return default


GHIDRA = os.environ.get("GHIDRA_HOME") or _cfg("ghidra_home") or \
    r"C:\Users\mrtit\Desktop\My Projects\Xbox 360\ghidra_11.3.2_PUBLIC"
JAVA_HOME = os.environ.get("JAVA_HOME") or _cfg("java_home") or r"C:\Program Files\Java\jdk-21"
GDT_DIR = os.path.join(ROOT, "binaries", "ghidra_gdt")
GDT_WIN32 = os.path.join(GDT_DIR, "win32_win95sdk.gdt")
GDT_DDK = os.path.join(GDT_DIR, "ddk_win95_vxd.gdt")
_EXTS = (".dll", ".exe", ".vxd", ".ocx", ".cpl", ".drv", ".sys", ".386", ".scr")


def classify(path):
    """Return (kind, gdt_path_or_None): 'pe'/'le'/'ne'/'other'."""
    try:
        d = open(path, "rb").read(0x400)
        if d[:2] != b"MZ":
            return ("other", None)
        lfa = struct.unpack_from("<I", d, 0x3c)[0]
        with open(path, "rb") as f:
            f.seek(lfa)
            sig = f.read(2)
        name = os.path.basename(path).lower()
        ext = os.path.splitext(path)[1].lower()
        is_drv = ext in (".vxd", ".386", ".sys", ".drv") or "vxd" in name
        if sig == b"PE":
            return ("pe", GDT_DDK if is_drv else GDT_WIN32)
        if sig in (b"LE", b"LX"):
            return ("le", GDT_DDK)
        if sig == b"NE":
            return ("ne", None)
    except Exception:
        pass
    return ("other", None)


def decompile_one(dll, out_c):
    kind, gdt = classify(dll)
    if kind == "ne":
        return {"skip": "16-bit NE -> msc6dec byte-exact track"}
    if kind == "other":
        return {"skip": "not a PE/LE image"}
    env = dict(os.environ)
    env["JAVA_HOME"] = JAVA_HOME
    headless = os.path.join(GHIDRA, "support", "analyzeHeadless.bat")
    with tempfile.TemporaryDirectory(prefix="ghproj_") as proj:
        args = [headless, proj, "b", "-import", dll, "-scriptPath", HERE,
                "-preScript", "AggressivePre.py",
                "-postScript", "DecompilePipeline.py", out_c]
        if gdt and os.path.exists(gdt):
            args.append(gdt)
        args += ["-deleteProject", "-analysisTimeoutPerFile", "900"]
        try:
            subprocess.run(args, capture_output=True, text=True, env=env, timeout=1800)
        except subprocess.TimeoutExpired:
            return {"skip": "ghidra timeout"}
    return {"ok": os.path.exists(out_c), "gdt": os.path.basename(gdt) if gdt else None}


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    inp, outdir = argv[0], argv[1]
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    os.makedirs(outdir, exist_ok=True)
    if os.path.isdir(inp):
        targets = []
        for dp, dns, fns in os.walk(inp):
            for fn in fns:
                if fn.lower().endswith(_EXTS):
                    targets.append(os.path.join(dp, fn))
    else:
        targets = [inp]
    targets = sorted(set(targets))[:limit]
    print("ghidra_batch: %d target(s) -> %s\n" % (len(targets), outdir))
    done = skipped = 0
    for t in targets:
        base = os.path.splitext(os.path.basename(t))[0]
        out_c = os.path.join(outdir, base + ".ghidra.c")
        if os.path.exists(out_c):
            print("  [have] %s" % base)
            continue
        res = decompile_one(t, out_c)
        if res.get("skip"):
            skipped += 1
            print("  [skip] %-26s %s" % (base, res["skip"]))
        elif res.get("ok"):
            done += 1
            print("  [ok]   %-26s gdt=%s" % (base, res.get("gdt")))
        else:
            print("  [FAIL] %-26s (no output)" % base)
    print("\ndone: %d decompiled, %d skipped -> %s" % (done, skipped, outdir))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
