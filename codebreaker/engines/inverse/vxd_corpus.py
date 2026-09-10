#!/usr/bin/env python3
r"""vxd_corpus.py -- drive the whole VxD corpus through object-level assemble-and-compare.

Runs vxd_masm.compare_module on every VxD (and every module of the vmm32 W3 bundle), proving --
or measuring -- byte-exact CODE recovery across all ~215 drivers. It is the corpus-scale version
of the single-driver test: emit each object as MASM, assemble with the period DDK ML.EXE, and
compare the assembled image to the oracle object byte-for-byte.

Results are checkpointed to a JSON after every module, so the run is RESUMABLE -- re-invoking skips
modules already recorded. This matters because a full pass is many hours of DOSBox assembly.

    python -m engine.inverse_engine.vxd_corpus binaries/win95/oracle results.json
    python -m engine.inverse_engine.vxd_corpus ... --redo vkd    # force-refresh one module
"""
import os
import sys
import glob
import json
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)


def _one_module(name, v):
    from codebreaker.engines.inverse import vxd_masm
    t0 = time.time()
    try:
        rep = vxd_masm.compare_module(v)
        objs = [{"obj": r.get("obj"), "match": bool(r.get("match")), "ok": bool(r.get("ok")),
                 "len": r.get("oracle_len"), "symbolic": r.get("symbolic_insns"),
                 "forced_raw": r.get("forced_raw"), "raw_fallback": bool(r.get("raw_fallback")),
                 "diffs": r.get("n_diffs"), "err": r.get("error")} for r in rep["objects"]]
        return {"byte_exact": rep["byte_exact"], "objects": objs, "n_obj": len(objs),
                "n_exact": sum(1 for o in objs if o["match"]), "symbolic": rep["symbolic"],
                "forced_raw": rep["forced_raw"], "raw_fallback_objs": rep.get("raw_fallback_objs", 0),
                "seconds": round(time.time() - t0, 1)}
    except Exception as e:
        import traceback
        return {"error": str(e)[:120], "trace": traceback.format_exc()[-300:]}


# VxDs also ship as IOS port/mini-port drivers (.pdr/.mpd) and a few under misc extensions -- all LE
# Ring-0 modules the engine reconstructs identically. Include them so the corpus is the COMPLETE set.
_VXD_EXTS = ("*.vxd", "*.386", "*.pdr", "*.mpd")
_EXTRA_FILES = ("csmapper.sys", "bigmem.drv", "drvspace.bin")


def _is_le_vxd(path):
    """True if `path` is an LE/LX/W3/W4 Ring-0 module (so a stray real-mode .sys/.drv is skipped)."""
    try:
        import struct
        d = open(path, "rb").read(0x100)
        if d[:2] != b"MZ":
            return False
        lfa = struct.unpack_from("<I", d, 0x3c)[0]
        full = open(path, "rb").read(lfa + 2)
        return full[lfa:lfa + 2] in (b"LE", b"LX", b"W3", b"W4")
    except Exception:
        return False


def run(srcdir, out_json, redo=None, workers=4):
    """Assemble-and-compare every module of the corpus, `workers` in parallel (each an independent
    DOSBox), checkpointing to out_json after every module so the run resumes where it stopped."""
    import threading
    from concurrent.futures import ThreadPoolExecutor, as_completed
    from codebreaker.formats import le
    redo = set(redo or [])
    results = json.load(open(out_json)) if os.path.exists(out_json) else {}
    cand = []
    for pat in _VXD_EXTS:
        cand += glob.glob(os.path.join(srcdir, pat))
    for f in _EXTRA_FILES:
        cand.append(os.path.join(srcdir, f))
    files = sorted(p for p in set(cand) if os.path.exists(p) and _is_le_vxd(p))
    todo = []
    for p in files:
        try:
            mods = le.open_vxds(p)
        except Exception as e:
            results[os.path.basename(p)] = {"error": str(e)[:80]}
            continue
        for name, v in mods:
            key = name.lower()
            if key in results and key not in redo and "error" not in results[key]:
                continue                                    # resume: already recorded
            todo.append((key, name, v))
    lock = threading.Lock()
    t_all = time.time()
    print("== %d modules to process, %d workers ==" % (len(todo), workers), flush=True)
    with ThreadPoolExecutor(max_workers=workers) as ex:
        futs = {ex.submit(_one_module, name, v): (key, name) for key, name, v in todo}
        for fut in as_completed(futs):
            key, name = futs[fut]
            r = fut.result()
            with lock:
                results[key] = r
                _save(results, out_json)
            if "error" in r:
                print("  %-14s ERROR %s" % (name, r["error"][:60]), flush=True)
            else:
                print("  %-14s %2d/%2d byte-exact  sym=%-6d raw=%-5d fb=%d  %-10s [%ds]" % (
                    name, r["n_exact"], r["n_obj"], r["symbolic"], r["forced_raw"],
                    r["raw_fallback_objs"], "BYTE-EXACT" if r["byte_exact"] else "PARTIAL",
                    r["seconds"]), flush=True)
    _summary(results, time.time() - t_all)
    return results


def _save(results, path):
    tmp = path + ".tmp"
    json.dump(results, open(tmp, "w"), indent=1)
    os.replace(tmp, path)


def _summary(results, secs):
    mods = [r for r in results.values() if "error" not in r]
    full = sum(1 for r in mods if r.get("byte_exact"))
    tobj = sum(r.get("n_obj", 0) for r in mods)
    eobj = sum(r.get("n_exact", 0) for r in mods)
    sym = sum(r.get("symbolic", 0) for r in mods)
    fb = sum(r.get("raw_fallback_objs", 0) for r in mods)
    errs = sum(1 for r in results.values() if "error" in r)
    print("\n== corpus: %d modules | %d fully byte-exact | %d/%d objects byte-exact "
          "| %d symbolic re-encodes | %d raw-fallback objs | %d errors | %.0fs ==" % (
              len(mods), full, eobj, tobj, sym, fb, errs, secs), flush=True)


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    srcdir = args[0] if args else "binaries/win95/oracle"
    out = args[1] if len(args) > 1 else "recovered/vxd_byte_exact.json"
    redo = sys.argv[sys.argv.index("--redo") + 1].split(",") if "--redo" in sys.argv else []
    workers = int(sys.argv[sys.argv.index("--workers") + 1]) if "--workers" in sys.argv else 4
    run(srcdir, out, redo=redo, workers=workers)
