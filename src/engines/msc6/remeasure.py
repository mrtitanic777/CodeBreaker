#!/usr/bin/env python3
r"""remeasure.py -- full-corpus VERIFIED re-measure of the Windows95 reconstruction tree.

For each reconstructed DLL in the authoritative Windows95 tree (WT_WIN95_ROOT), re-run the
per-DLL reconstruction WITH oracle verification (`emit --verify`) in a throwaway STAGING dir,
then write the true byte-exact tally back into that DLL's STATUS.md.

MEASURE-ONLY: the hand-organized source in the tree is never touched -- only the four count
lines of each STATUS.md are refreshed. (Full regeneration of a DLL's source is a separate,
deliberate, one-at-a-time operation.)

    wt msc6 remeasure base/lzexpand         # one DLL
    wt msc6 remeasure --all                 # the whole corpus
    wt msc6 remeasure --all --limit 5       # first N DLLs (a smoke run)
"""
import os
import re
import sys
import glob
import tempfile
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))            # CodeBreaker/
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
from src.toolchain.sdk import w95paths
from src.core import paths

_COUNT = re.compile(r"(\d+)\s+verified byte-exact,\s+(\d+)\s+candidate,\s+"
                    r"(\d+)\s+pending-link,\s+(\d+)\s+@RECON-UNKNOWN")
_BIN_EXTS = (".dll", ".exe", ".drv", ".vxd", ".386", ".cpl", ".scr", ".mpd", ".ocx")
_INDEX = None


def _build_index():
    global _INDEX
    _INDEX = {}
    files_root = os.path.join(w95paths.corpus_root(), "files")
    if not os.path.isdir(files_root):
        return
    for dp, dns, fns in os.walk(files_root):
        for fn in fns:
            _INDEX.setdefault(fn.lower(), []).append(os.path.join(dp, fn))


def find_input(dllname):
    """Locate the retail input binary matching a reconstructed DLL, by basename."""
    if _INDEX is None:
        _build_index()
    for ext in _BIN_EXTS:
        cands = _INDEX.get((dllname + ext).lower())
        if cands:
            # prefer WINDOWS/SYSTEM copies, then the shortest path
            return sorted(cands, key=lambda p: (0 if "system" in p.lower() else 1, len(p)))[0]
    return None


def _update_status(path, v, c, p, u):
    out = []
    for ln in open(path, encoding="utf-8").read().splitlines():
        if ln.startswith("- VERIFIED byte-exact:"):
            ln = "- VERIFIED byte-exact: %d" % v
        elif ln.startswith("- CANDIDATE"):
            ln = "- CANDIDATE (msc6dec output, unverified): %d" % c
        elif ln.startswith("- pending link:"):
            ln = "- pending link: %d" % p
        elif ln.startswith("- @RECON-UNKNOWN"):
            ln = "- @RECON-UNKNOWN (to complete): %d" % u
        out.append(ln)
    open(path, "w", encoding="utf-8").write("\n".join(out) + "\n")


def remeasure_one(dll_dir, timeout=600):
    """dll_dir: path relative to WT_WIN95_ROOT, e.g. 'base/lzexpand'."""
    root = w95paths.win95_root()
    status = os.path.join(root, dll_dir, "STATUS.md")
    if not os.path.isfile(status):
        return {"dll": dll_dir, "skip": "no STATUS.md"}
    dllname = os.path.basename(dll_dir)
    inp = find_input(dllname)
    if not inp:
        return {"dll": dll_dir, "skip": "no input binary (%s.*)" % dllname}
    with tempfile.TemporaryDirectory(prefix="remeasure_") as staging:
        cmd = [sys.executable, paths.engine_dir("sdk", "emit.py"), inp, staging, "--verify"]
        try:
            r = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT, timeout=timeout)
        except subprocess.TimeoutExpired:
            return {"dll": dll_dir, "skip": "emit timed out (%ss)" % timeout}
        except Exception as e:
            return {"dll": dll_dir, "skip": "emit failed: %s" % e}
    m = _COUNT.search(r.stdout + "\n" + r.stderr)
    if not m:
        return {"dll": dll_dir, "skip": "counts unparsed", "raw": (r.stdout + r.stderr).strip()[-160:]}
    v, c, p, u = map(int, m.groups())
    _update_status(status, v, c, p, u)
    return {"dll": dll_dir, "verified": v, "candidate": c, "pending": p, "unknown": u}


def all_dlls():
    root = w95paths.win95_root()
    dirs = []
    for dp, dns, fns in os.walk(root):
        dns[:] = [d for d in dns if d != ".git"]
        if "STATUS.md" in fns:
            dirs.append(os.path.relpath(dp, root).replace(os.sep, "/"))
    return sorted(dirs)


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    if argv[0] == "--all":
        limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
        dlls = all_dlls()[:limit]
        print("re-measuring %d DLL(s) against the Windows95 tree...\n" % len(dlls))
        tv = tc = tp = tu = done = 0
        skips = []
        for d in dlls:
            res = remeasure_one(d)
            if res.get("skip"):
                skips.append((d, res["skip"]))
                print("  [skip] %-30s %s" % (d, res["skip"]))
                continue
            done += 1
            tv += res["verified"]; tc += res["candidate"]; tp += res["pending"]; tu += res["unknown"]
            print("  %-30s  %4d exact  %4d cand  %4d link  %4d unknown"
                  % (d, res["verified"], res["candidate"], res["pending"], res["unknown"]))
        total = tv + tc + tp + tu
        print("\n== corpus re-measure: %d DLL(s) measured, %d skipped ==" % (done, len(skips)))
        print("   VERIFIED byte-exact : %d" % tv)
        print("   CANDIDATE           : %d" % tc)
        print("   pending-link        : %d" % tp)
        print("   @RECON-UNKNOWN      : %d" % tu)
        if total:
            print("   byte-exact coverage : %.1f%% (%d / %d functions)" % (100.0 * tv / total, tv, total))
        return 0
    print(remeasure_one(argv[0]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
