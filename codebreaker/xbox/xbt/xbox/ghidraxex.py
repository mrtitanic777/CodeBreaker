#!/usr/bin/env python3
"""ghidraxex -- headless XEX -> Ghidra analysis + dumps (native staging).

Drives Ghidra's analyzeHeadless with the XEXLoaderWV loader, which decompresses /
decrypts the image, maps the PE sections, and (crucially) resolves Xbox import
ordinals to real API names. A DumpXex post-script then emits functions / imports
/ exports / strings per module. If the loader fails on a module, this falls back
to extracting the basefile PE *natively* (our xex.py) and loading it with
Ghidra's PE loader forcing the Xenon language.

Ported from re_tools/ghidra_pipeline.py; the staging is native (xex.py) rather
than shelling out to xextool, so the only external dependency is Ghidra itself
(with the XEXLoaderWV extension installed).

Usage:
    ghidraxex <file.xex> [more.xex ...]      analyze specific modules
    ghidraxex --dir <folder>                 every standalone XEX in a folder
    ghidraxex <file.xex> --decompile         also emit full decompilation (slow)
    ghidraxex <file.xex> --out <dir>         where dumps land (default ./ghidra_dumps)
    ghidraxex <file.xex> --timeout 600
    ghidraxex <file.xex> --dry               print the headless command, don't run
"""
DESCRIPTION = "headless XEX -> Ghidra (XEXLoaderWV) analysis + function/import dumps"

import json
import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import xex  # noqa: E402
import ghidra as _bridge  # reuse the bridge's Ghidra path discovery  # noqa: E402

GHIDRA_XEX_LOADER = "XEXLoaderWVLoader"
GHIDRA_XEX_LANGUAGE = "PowerPC:BE:64:A2ALT-32addr"


def _script_dir():
    """Directory holding DumpXex.py (bundled under ghidra_scripts)."""
    here = os.path.dirname(os.path.abspath(__file__))
    for c in (os.path.join(os.path.dirname(here), "cli", "ghidra_scripts"),
              os.path.join(getattr(sys, "_MEIPASS", here), "ghidra_scripts"),
              os.path.join(here, "ghidra_scripts")):
        if os.path.isfile(os.path.join(c, "DumpXex.py")):
            return c
    return os.path.join(os.path.dirname(here), "cli", "ghidra_scripts")


def _headless():
    hl = _bridge.HEADLESS
    if not os.path.exists(hl):
        sys.exit("analyzeHeadless not found: %s\n(set the Ghidra path in xbt/core/ghidra.py)" % hl)
    return hl


def _run(src, dump_dir, proj_dir, decompile, timeout, loader, extra_args=None, dry=False):
    """One headless import + analysis + DumpXex. Returns (ok, summary, log, dt)."""
    os.makedirs(dump_dir, exist_ok=True)
    os.makedirs(proj_dir, exist_ok=True)
    post = [dump_dir] + (["decompile"] if decompile else [])
    cmd = [_headless(), proj_dir, "xexkit",
           "-import", src, "-loader", loader,
           *(extra_args or []),
           "-scriptPath", _script_dir(),
           "-postScript", "DumpXex.py", *post,
           "-overwrite"]
    if dry:
        print("# " + " ".join('"%s"' % c if " " in c else c for c in cmd))
        return True, {}, "", 0.0
    sp = os.path.join(dump_dir, "summary.json")
    if os.path.exists(sp):
        os.remove(sp)                                  # avoid a stale read
    t0 = time.time()
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True, errors="replace",
                              timeout=timeout, stdin=subprocess.DEVNULL)
        log = proc.stdout + "\n--- STDERR ---\n" + proc.stderr
    except subprocess.TimeoutExpired as e:
        log = (e.stdout or "") + "\n--- TIMEOUT ---\n"
    dt = time.time() - t0
    with open(os.path.join(dump_dir, "headless.log"), "w", encoding="utf-8", errors="replace") as f:
        f.write(log)
    summary = {}
    if os.path.exists(sp):
        try:
            summary = json.load(open(sp, encoding="utf-8"))
        except Exception:
            pass
    return bool(summary), summary, log, dt


def run_one(xex_path, out_base, decompile, timeout, dry=False):
    stem = os.path.splitext(os.path.basename(xex_path))[0]
    dump_dir = os.path.join(out_base, stem)
    proj_dir = os.path.join(out_base, "_proj")
    print("[ghidraxex] %s -> %s" % (os.path.basename(xex_path), dump_dir))

    # Tier 1: XEXLoaderWV on the original XEX (handles decrypt/decompress + imports)
    ok, summary, log, dt = _run(xex_path, dump_dir, proj_dir, decompile, timeout,
                                 GHIDRA_XEX_LOADER, dry=dry)
    method = "xexloaderwv"
    if dry:
        return {"file": os.path.basename(xex_path), "status": "dry"}

    # Tier 2: loader failed -> native basefile PE + Ghidra PE loader (Xenon language)
    if not ok:
        try:
            print("  [retry] loader failed - extracting basefile natively (xex.py)")
            base = xex.open_xex(xex_path).basefile()
            os.makedirs(proj_dir, exist_ok=True)
            staged = os.path.join(out_base, stem + ".basefile")
            with open(staged, "wb") as f:
                f.write(base)
            ok, summary, log, dt = _run(staged, dump_dir, proj_dir, decompile, timeout,
                                        "PeLoader", extra_args=["-processor", GHIDRA_XEX_LANGUAGE])
            if ok:
                method = "basefile-pe"
        except Exception as e:
            log += "\n--- native basefile extract failed: %s ---\n" % e

    status = "ok" if ok else "FAILED"
    extra = ""
    if summary:
        extra = "  functions=%s imports=%s strings=%s" % (
            summary.get("functions"), summary.get("imports"), summary.get("strings"))
        if method != "xexloaderwv":
            extra += "  via %s" % method
    print("  [%s] %.0fs%s" % (status, dt, extra))
    if not ok:
        for ln in log.splitlines()[-10:]:
            print("    " + ln)
    return {"file": os.path.basename(xex_path), "status": status,
            "seconds": round(dt, 1), "method": method if ok else None,
            "dump_dir": dump_dir, "summary": summary}


def main():
    import argparse
    ap = argparse.ArgumentParser(description="Headless XEX -> Ghidra analysis + dumps.")
    ap.add_argument("files", nargs="*", help="XEX files to analyze")
    ap.add_argument("--dir", help="analyze every standalone XEX in this folder")
    ap.add_argument("--out", default="ghidra_dumps", help="output dir (default ./ghidra_dumps)")
    ap.add_argument("--decompile", action="store_true", help="also emit full decompilation (slow)")
    ap.add_argument("--timeout", type=int, default=1800, help="per-file timeout seconds")
    ap.add_argument("--dry", action="store_true", help="print the headless command, don't run")
    args = ap.parse_args()

    targets = list(args.files)
    if args.dir:
        import sysfiles
        for name in sorted(os.listdir(args.dir)):
            p = os.path.join(args.dir, name)
            if os.path.isfile(p) and sysfiles.classify(p)[0] == "xex":
                targets.append(p)
    if not targets:
        ap.error("no targets - pass XEX files or --dir")

    print("[ghidraxex] Ghidra: %s" % _bridge.GHIDRA)
    results = []
    for t in targets:
        if not os.path.exists(t):
            print("[ghidraxex] skip missing %s" % t); continue
        results.append(run_one(t, args.out, args.decompile, args.timeout, dry=args.dry))
    if not args.dry:
        ok = sum(1 for r in results if r.get("status") == "ok")
        print("\n[ghidraxex] done: %d/%d ok. Dumps in %s/<module>/" % (ok, len(results), args.out))


if __name__ == "__main__":
    main()
