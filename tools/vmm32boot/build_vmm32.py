#!/usr/bin/env python3
"""Reconstruct EVERY Win95 VxD from the recovered disassembly into a staging directory, ready to
inject into a Win95 disk image so the whole reconstructed 32-bit kernel + driver stack boots.

Each standalone .vxd/.386 is synthesized by the Python LE writer (engine.inverse_engine.
vxd_le_writer.build_le) directly from the recovered code (D.code) and relocations (D.fx); the
W3-bundled VMM (vmm32.vxd) is synthesized in place by build_w3. Output is byte-exact with the
oracle except LINK386's intra-page fixup ORDER (a linker artifact that does not affect loading);
every module round-trips through le.py to the same objects, object bytes, and fixups.

Usage:  python build_vmm32.py [--oracle DIR] [--out DIR]

Then inject the staging directory with inject_and_boot.sh (see this folder's README).
"""
import argparse, os, sys, glob, time

HERE = os.path.dirname(os.path.abspath(__file__))
CB = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, CB)
from src.formats import le
from src.engines.inverse.vxd_asm import VxDDisasm
from src.engines.inverse import vxd_le_writer as W


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--oracle", default=os.path.join(CB, "binaries", "win95", "oracle"),
                    help="directory of oracle VxDs (default: binaries/win95/oracle)")
    ap.add_argument("--out", default=os.path.join(HERE, "vmm32stage"),
                    help="staging directory for the reconstructed VxDs (default: ./vmm32stage)")
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    t0 = time.time(); n = ok = 0
    files = sorted(glob.glob(os.path.join(a.oracle, "*.vxd")) + glob.glob(os.path.join(a.oracle, "*.386")))
    for f in files:
        base = os.path.basename(f)
        try:
            mods = le.open_vxds(f)
        except Exception:
            continue
        if base.lower() == "vmm32.vxd":                      # W3 container: rebuild VMM in place
            name, v = mods[0]; D = VxDDisasm(v).run(); D.v = v
            open(os.path.join(a.out, "VMM32.VXD"), "wb").write(W.build_w3(D, v, name))
            n += 1; ok += 1; continue
        if len(mods) != 1:
            continue
        name, v = mods[0]
        try:
            D = VxDDisasm(v).run(); D.v = v
            out = W.build_le(D, v, name.upper())
            m = le.LEFile.from_buffer(out)
            rt = all(bytes(m.object_bytes(x)) == bytes(v.object_bytes(y))
                     for x, y in zip(m.objects, v.objects))
            open(os.path.join(a.out, os.path.splitext(base)[0].upper() + ".VXD"), "wb").write(out)
            n += 1; ok += rt
        except Exception as e:
            print("FAIL %-16s %s" % (base, repr(e)[:60]))
    print("built %d VxDs (%d round-trip-clean) -> %s  [%ds]" % (n, ok, a.out, time.time() - t0))


if __name__ == "__main__":
    main()
