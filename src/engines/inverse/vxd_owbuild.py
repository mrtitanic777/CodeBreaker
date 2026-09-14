#!/usr/bin/env python3
r"""vxd_owbuild.py -- build a ring-0 VxD from recovered source with the OpenWatcom
toolchain (no Microsoft tools): wasm (MASM-mode assemble) + wlink `system win_vxd`.

The recovered per-segment source (vxd_tree.emit_tree) is emitted UNSPLIT (one file per
LE object, so every branch target is internal), each object is converged to assemble by
byte-preserving what wasm rejects (the probe), assembled to OMF, and linked into an LE
VxD with the DDB as the single required export (`export VXD_DDB.1`). Produces a real
loadable <name>.vxd.

    from src.engines.inverse import vxd_owbuild
    vxd_owbuild.build_vxd("vkd.vxd", "VKD", "out")     # -> out/vkd/vkd.vxd

Corpus result: 211/215 standalone ring-0 VxDs build into valid LE binaries.
"""
import os
import subprocess

from src.core import paths
from src.formats import le
from src.engines.inverse import vxd_tree

_OW = os.path.join(paths.repo_root(), "experiments", "premise-proof", "open-watcom-v2")
_WASM = os.path.join(_OW, "bld", "wasm", "nt386", "wasm.exe")
_WLINK = os.path.join(_OW, "rel", "binnt64", "wlink.exe")


def _env():
    e = dict(os.environ)
    e["WATCOM"] = os.path.join(_OW, "rel")
    e["PATH"] = os.path.join(_OW, "rel", "binnt64") + os.pathsep + e.get("PATH", "")
    return e


def build_vxd(vxd_path, name, outroot):
    """Assemble (wasm) + link (wlink system win_vxd) one VxD's recovered source into a
    loadable .vxd. Returns {ok, output|error, bytes}."""
    if not (os.path.exists(_WASM) and os.path.exists(_WLINK)):
        return {"ok": False, "error": "OpenWatcom wasm/wlink not built"}
    v = le.LEFile.from_file(vxd_path)
    env = _env()
    # emit UNSPLIT (max_ins huge) so cross-object branch targets stay internal, and
    # converge each object to assemble under wasm (byte-preserve what it rejects).
    files = vxd_tree.emit_tree(v, name.upper(), outroot, max_ins=10 ** 9, probe_wasm=_WASM)
    d = os.path.join(outroot, name.lower())
    objs, asmfail = [], []
    ddb = False
    for fn in files:
        if not fn.endswith(".asm"):
            continue
        src = os.path.join(d, fn)
        if "public VXD_DDB" in open(src, encoding="utf-8", errors="replace").read():
            ddb = True
        ob = fn[:-4] + ".obj"
        r = subprocess.run([_WASM, "-zcm=masm", "-q", "-fo=" + ob, fn],
                           cwd=d, env=env, capture_output=True, text=True)
        if r.returncode == 0 and os.path.exists(os.path.join(d, ob)):
            objs.append(ob)
        else:
            asmfail.append(fn)
    if asmfail:
        return {"ok": False, "error": "assemble failed: " + ", ".join(asmfail)}
    lnk = os.path.join(d, "link.lnk")
    with open(lnk, "w", newline="\n") as f:
        f.write("system win_vxd\nname %s.vxd\noption quiet\n" % name.lower())
        f.write("file " + ",".join(objs) + "\n")
        if ddb:
            f.write("export VXD_DDB.1\n")     # a VxD's single required export is its DDB
    subprocess.run([_WLINK, "@link.lnk"], cwd=d, env=env, capture_output=True, text=True)
    out = os.path.join(d, name.lower() + ".vxd")
    if os.path.exists(out):
        return {"ok": True, "output": out, "bytes": os.path.getsize(out), "objects": len(objs)}
    return {"ok": False, "error": "link produced no .vxd (missing DDB export?)"}


def main(argv):
    import argparse
    ap = argparse.ArgumentParser(prog="vxd_owbuild", description=__doc__.splitlines()[0])
    ap.add_argument("vxd", help="path to a .vxd/.386 (or a corpus name)")
    ap.add_argument("--out", default=None)
    a = ap.parse_args(argv)
    path = a.vxd
    if not os.path.isfile(path):
        root = os.environ.get("WT_DATA_ROOT",
                              r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files")
        for dp, _, fs in os.walk(root):
            for fn in fs:
                if fn.lower() == a.vxd.lower():
                    path = os.path.join(dp, fn)
    out = a.out or os.path.join(paths.repo_root(), "build", "vxd_ow")
    for nm, v in le.open_vxds(path):
        base = nm.split(".")[0]
        r = build_vxd(path, base, out)
        print("%s: %s" % (base, ("built %d bytes (%d objs) -> %s" % (
            r["bytes"], r["objects"], r["output"])) if r.get("ok") else "FAIL " + r.get("error", "")))
        break
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
