#!/usr/bin/env python3
r"""ghidra_decomp.py -- reproduce dogbolt.org's "Decompiler Explorer -> Ghidra"
export, locally and in batch.

dogbolt = plain Ghidra headless (auto-detected loader + DEFAULT analysis) piped
through the stock CppExporter. This driver does exactly that: import a binary, run
default auto-analysis, then ExportC.java (CppExporter, create-header) -> one .c with
`#include "<out>.h"`, the DAT_ globals, and every function. That .c is the canonical
input to autobuild95's decompile-driven Recover stage.

Deliberately NOT the GDT + AggressivePre + DecompilePipeline path of ghidra_batch.py
-- this matches dogbolt byte-for-byte (modulo the Ghidra version), so the Recover
ground truth is reproducible without the online tool or its 2 MB upload cap.

    python -m codebreaker.decompile.ghidra_headless <input_dir_or_file> <out_dir> [--limit N] [--jobs N]
    python -m codebreaker.decompile.ghidra_headless <one_binary> <out.c>     # single file

Config (config.json keys or env): ghidra_home / GHIDRA_HOME, java_home / JAVA_HOME.
Requires Ghidra 12.1.3 + JDK 21. Handles PE / NE (16-bit) / LE alike, like dogbolt.
"""
import os
import sys
import tempfile
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))                 # CodeBreaker/
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

# reuse the toolchain resolution + format classifier from ghidra_batch
from codebreaker.decompile.ghidra_headless_batch import GHIDRA, JAVA_HOME, classify, _EXTS  # noqa: E402


def decompile_export_one(binary, out_c, timeout=1800, analysis_timeout=900):
    """Import + default-analyze + CppExporter -> out_c (and out_c's .h sibling)."""
    kind, _gdt = classify(binary)
    if kind == "other":
        return {"skip": "not a PE/NE/LE image"}
    env = dict(os.environ)
    env["JAVA_HOME"] = JAVA_HOME
    headless = os.path.join(GHIDRA, "support", "analyzeHeadless.bat")
    with tempfile.TemporaryDirectory(prefix="ghdec_") as proj:
        args = [headless, proj, "d", "-import", binary,
                "-scriptPath", HERE, "-postScript", "ExportC.java", out_c,
                "-deleteProject", "-analysisTimeoutPerFile", str(analysis_timeout)]
        try:
            r = subprocess.run(args, capture_output=True, text=True, env=env, timeout=timeout)
        except subprocess.TimeoutExpired:
            return {"skip": "ghidra timeout"}
    ok = os.path.exists(out_c) and os.path.getsize(out_c) > 0
    return {"ok": ok, "kind": kind, "log": (r.stdout or "")[-400:] if not ok else ""}


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    inp, out = argv[0], argv[1]

    # single-file mode: 2nd arg looks like a file path (has an extension, parent exists)
    if not os.path.isdir(inp):
        os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
        res = decompile_export_one(inp, out)
        if res.get("ok"):
            print("[ok] %s (%s) -> %s" % (os.path.basename(inp), res["kind"], out))
            return 0
        print("[FAIL] %s: %s" % (os.path.basename(inp), res.get("skip") or "no output"))
        if res.get("log"):
            print(res["log"])
        return 1

    # batch mode: input dir -> out dir
    import concurrent.futures
    outdir = out
    limit = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    jobs = int(argv[argv.index("--jobs") + 1]) if "--jobs" in argv else max(1, min((os.cpu_count() or 4) - 4, 6))
    os.makedirs(outdir, exist_ok=True)
    targets = []
    for dp, _dns, fns in os.walk(inp):
        for fn in fns:
            if fn.lower().endswith(_EXTS):
                targets.append(os.path.join(dp, fn))
    todo = []
    for t in sorted(set(targets)):
        base = os.path.splitext(os.path.basename(t))[0]
        out_c = os.path.join(outdir, base + ".decomp.c")
        if not os.path.exists(out_c):
            todo.append((t, out_c))
    todo = todo[:limit]
    print("ghidra_decomp: %d to do, %d workers -> %s" % (len(todo), jobs, outdir), flush=True)
    done = skipped = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = {ex.submit(decompile_export_one, t, o): (t, o) for (t, o) in todo}
        for i, fut in enumerate(concurrent.futures.as_completed(futs), 1):
            t, _o = futs[fut]
            base = os.path.splitext(os.path.basename(t))[0]
            try:
                res = fut.result()
            except Exception as e:
                res = {"skip": "worker error: %s" % str(e)[:60]}
            if res.get("skip"):
                skipped += 1
                tag, note = "[skip]", res["skip"]
            elif res.get("ok"):
                done += 1
                tag, note = "[ok]  ", res["kind"]
            else:
                tag, note = "[FAIL]", "no output"
            print("  %4d/%d %s %-26s %s" % (i, len(todo), tag, base, note), flush=True)
    print("\ndone: %d exported, %d skipped -> %s" % (done, skipped, outdir), flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
