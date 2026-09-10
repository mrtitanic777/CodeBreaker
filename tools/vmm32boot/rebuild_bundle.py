#!/usr/bin/env python3
r"""Reconstruct an installed machine's EXACT vmm32.vxd bundle from the recovered disassembly.

`build_vmm32.py` boots a *replacement* kernel: a VMM-only W3 plus standalone VxDs. This script does
the stronger thing -- it takes the machine's own `vmm32.vxd` (the compressed **W4** an installed Win95
actually runs), decompresses it to its **W3** payload, and rebuilds every one of the bundled modules
*in place* from the recovered code and relocations. The output is the same bundle, same module set,
same layout -- but with each module's bytes re-synthesized by the LE writer rather than copied.

For every LE sub-image in the bundle:
  * object bytes come from the recovered disassembly (`VxDDisasm(...).run().code`), byte-exact;
  * the fixup page + record tables are re-emitted by `vxd_le_writer._emit_fixups`.
The re-emitted fixups differ from the originals only in the ORDER LINK386 wrote intra-page records;
they decode to the identical relocation set (verify with --check), so the rebuilt bundle loads and
runs exactly like the original. Point Win95 at it (see README / inject scripts) and it boots to logon.

A W3 sub-image leaves its non-resident-name offset (LE header +0x88) *module-relative* after bundling,
so the data-page extent must be derived from the page layout, not that field -- the engine now does.

    python rebuild_bundle.py  vmm32.vxd  vmm32-rebuilt.vxd            # W4 or W3 in; uncompressed W3 out
    python rebuild_bundle.py  vmm32.vxd  vmm32-rebuilt.vxd  --check   # also verify relocation sets match

W4 input needs the Unicorn-based decompressor in w4.py (pip install unicorn); a W3 input needs neither.
"""
import argparse, os, struct, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
CB = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, CB)

from codebreaker.format_loaders import le
from codebreaker.inverse_engine.vxd_asm import VxDDisasm
from codebreaker.inverse_engine import vxd_le_writer as W
from w4 import decompress_w4


def rebuild(data, check=False):
    """Return (rebuilt_container, stats). `data` is a W4 or W3 vmm32.vxd; the result is an
    uncompressed W3 bundle with every module re-synthesized from the recovered disassembly."""
    lfa = struct.unpack_from("<I", data, 0x3c)[0]
    sig = bytes(data[lfa:lfa + 2])
    if sig == b"W4":                                    # decompress the body, keep the real-mode stub
        container = bytearray(data[:lfa]) + bytearray(decompress_w4(bytes(data)))
    elif sig == b"W3":
        container = bytearray(data)
    else:
        raise ValueError("not a W3/W4 container (sig %r at e_lfanew)" % sig)
    if bytes(container[lfa:lfa + 2]) != b"W3":
        raise ValueError("decompressed payload does not start 'W3'")

    w3 = le.W3File(bytearray(container), lfa)
    out = bytearray(container)
    total = rebuilt = exact = reloc_ok = 0
    t0 = time.time()
    for e in w3.entries:
        off = e["off"]
        if bytes(container[off:off + 2]) != b"LE":      # a non-LE member (rare) -- leave verbatim
            continue
        total += 1
        v = le.LEFile(bytearray(container), off)
        D = VxDDisasm(v).run(); D.v = v
        U = lambda o: struct.unpack_from("<I", v.d, v.lfa + o)[0]
        ps = v.pagesize; dp = U(0x80)
        for o in v.objects:                             # recovered object bytes, byte-exact
            base = dp + (o["pmidx"] - 1) * ps; cb = D.code[o["idx"]]
            out[base:base + len(cb)] = cb
        fxpage, fxrec = W._emit_fixups(D, v)            # re-emitted fixup tables
        if len(fxpage) + len(fxrec) == U(0x30):
            out[v.lfa + U(0x68):v.lfa + U(0x68) + len(fxpage)] = fxpage
            out[v.lfa + U(0x6c):v.lfa + U(0x6c) + len(fxrec)] = fxrec
            rebuilt += 1
        # byte-exact except inside this module's fixup window?
        fp = v.lfa + U(0x68); fend = fp + U(0x30); endmod = off + e["size"]
        if not any(out[i] != container[i] for i in range(off, min(endmod, len(out))) if not fp <= i < fend):
            exact += 1
        if check:                                       # relocation set preserved (order-independent)?
            a = le.LEFile(bytearray(container), off).fixups()
            b = le.LEFile(out, off).fixups()
            reloc_ok += (a == b)
    stats = dict(total=total, rebuilt=rebuilt, exact=exact, reloc_ok=reloc_ok,
                 checked=check, secs=time.time() - t0)
    return out, stats


def main():
    ap = argparse.ArgumentParser(description="Rebuild an exact vmm32.vxd bundle from the disassembly.")
    ap.add_argument("infile", help="vmm32.vxd from an installed machine (W4) or install media (W3)")
    ap.add_argument("outfile", help="reconstructed uncompressed-W3 bundle to write")
    ap.add_argument("--check", action="store_true", help="verify each module's relocation set matches")
    a = ap.parse_args()
    out, s = rebuild(bytearray(open(a.infile, "rb").read()), check=a.check)
    open(a.outfile, "wb").write(out)
    print("rebuilt %d/%d LE modules in place, %d byte-exact-except-fixup-order [%.0fs]"
          % (s["rebuilt"], s["total"], s["exact"], s["secs"]))
    if s["checked"]:
        print("relocation set identical to original: %d/%d modules" % (s["reloc_ok"], s["total"]))
    print("wrote %s (%d bytes)" % (a.outfile, len(out)))


if __name__ == "__main__":
    main()
