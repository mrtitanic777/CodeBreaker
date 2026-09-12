#!/usr/bin/env python3
r"""build_vmm32_fromsrc.py -- stage EVERY Win95 VxD rebuilt FROM SOURCE through the
authentic period toolchain (ML 6.11 -> LINK 2.60 /VXD -> RC 3.11 + ADRC2VXD), ready
to inject into a Win95 image for a boot test.

This is the from-source (Path A) analog of build_vmm32.py (the byte-exact LE-writer,
Path B).  vxd_loadable_corpus.py already runs build_from_source on all 222 modules
but DISCARDS the .vxd bytes (it records only buildability metadata); this persists
each r["vxd"] as <STEM>.VXD so inject_and_boot.sh can place them.  The W3-bundled
VMM (vmm32.vxd) is rebuilt from its from-source LE and wrapped W3 as VMM32.VXD;
if the from-source VMM will not wrap, it falls back to the byte-exact build_w3 so
the image still boots (flagged in the output).

    python build_vmm32_fromsrc.py [--oracle DIR] [--out DIR] [--workers N] [--limit N]

Needs DOSBox-X + the DDK (same as vxd_loadable_corpus).  Slow: ~minutes at workers=4.
"""
import argparse
import glob
import os
import sys
import threading
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))
CB = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, CB)
from src.formats import le
from src.engines.inverse import vxd_corpus, vxd_from_source as F
from src.engines.inverse.vxd_asm import VxDDisasm
from src.engines.inverse import vxd_le_writer as W


def _candidates(oracle):
    cand = []
    for pat in vxd_corpus._VXD_EXTS:
        cand += glob.glob(os.path.join(oracle, pat))
    for f in vxd_corpus._EXTRA_FILES:
        cand.append(os.path.join(oracle, f))
    return sorted(p for p in set(cand) if os.path.exists(p) and vxd_corpus._is_le_vxd(p))


def _build_one(name, v):
    """from-source build; returns (bytes|None, stage, ok)."""
    try:
        r = F.build_from_source(v, root=CB)
        return (r.get("vxd") if r.get("ok") else None, r.get("stage"), bool(r.get("ok")))
    except Exception as e:
        return (None, "exc:%s" % str(e)[:40], False)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--oracle", default=os.path.join(CB, "binaries", "win95", "oracle"))
    ap.add_argument("--out", default=os.path.join(HERE, "vmm32stage_fromsrc"))
    ap.add_argument("--workers", type=int, default=4)
    ap.add_argument("--limit", type=int)
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    files = _candidates(a.oracle)

    # enumerate (key, name, v, is_vmm) across every LE member of every container
    todo, vmm = [], None
    for f in files:
        base = os.path.basename(f).lower()
        try:
            mods = le.open_vxds(f)
        except Exception:
            continue
        if base == "vmm32.vxd":
            vmm = mods[0]                                  # (name, v) -- the VMM core
            continue
        if len(mods) != 1:
            continue
        name, v = mods[0]
        todo.append((os.path.splitext(os.path.basename(f))[0].upper(), name, v))
    if a.limit:
        todo = todo[:a.limit]

    lock = threading.Lock()
    t0 = time.time()
    done = {"ok": 0, "fail": 0}
    print("== from-source staging: %d standalone VxDs, %d workers ==" % (len(todo), a.workers), flush=True)

    def work(item):
        stem, name, v = item
        data, stage, ok = _build_one(name, v)
        with lock:
            if ok and data:
                open(os.path.join(a.out, stem + ".VXD"), "wb").write(data)
                done["ok"] += 1
                print("  %-14s OK  (%d B)" % (stem, len(data)), flush=True)
            else:
                done["fail"] += 1
                print("  %-14s FAIL@%s" % (stem, stage), flush=True)

    with ThreadPoolExecutor(max_workers=a.workers) as ex:
        list(as_completed(ex.submit(work, it) for it in todo))

    # VMM32.VXD: from-source VMM core wrapped W3, else byte-exact fallback
    vmm_mode = "none"
    if vmm is not None:
        name, v = vmm
        data, stage, ok = _build_one(name, v)
        try:
            if ok and data:
                lem = le.LEFile.from_buffer(data)          # wrap the from-source LE in W3
                D = VxDDisasm(lem).run(); D.v = lem
                open(os.path.join(a.out, "VMM32.VXD"), "wb").write(W.build_w3(D, lem, name))
                vmm_mode = "from-source"
            else:
                raise RuntimeError("vmm from-source build failed@%s" % stage)
        except Exception as e:
            D = VxDDisasm(v).run(); D.v = v                 # fallback: byte-exact W3
            open(os.path.join(a.out, "VMM32.VXD"), "wb").write(W.build_w3(D, v, name))
            vmm_mode = "byte-exact-fallback (%s)" % str(e)[:50]

    print("\n== staged %d/%d standalone OK, %d fail | VMM32.VXD=%s | %.0fs -> %s ==" % (
        done["ok"], len(todo), done["fail"], vmm_mode, time.time() - t0, a.out), flush=True)


if __name__ == "__main__":
    main()
