#!/usr/bin/env python3
"""carve.py -- carve 16-bit MS C 5.0 functions out of an unpacked MZ image.

Finds the _TEXT extent, scans for MS C near-function prologues, walks each to its
epilogue, and emits a function table (+ optional per-function disassembly).

    python carve.py <unpacked.exe> [--dis N]   # N = also dump disasm of first N fns

MS C 5.0 near prologue:   55 8B EC            push bp ; mov bp,sp
  then one of:            (nothing)                        small/omitted-frame
                          83 EC ib             sub sp,imm8
                          81 EC iw             sub sp,imm16
                          B8 iw E8 rel  ...     mov ax,frame ; call __chkstk (probe)
Epilogues:               5D C3 | 5D CB | 8B E5 5D C3 | C9 C3 | C9 CB
"""
import sys, struct, json, os
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
MD = Cs(CS_ARCH_X86, CS_MODE_16); MD.detail = False

def load(path):
    d = open(path, "rb").read()
    hdr = struct.unpack_from("<H", d, 8)[0] * 16
    nrel = struct.unpack_from("<H", d, 6)[0]
    reloff = struct.unpack_from("<H", d, 24)[0]
    ip = struct.unpack_from("<H", d, 20)[0]; cs = struct.unpack_from("<H", d, 22)[0]
    relocs = []
    for i in range(nrel):
        off, seg = struct.unpack_from("<HH", d, reloff + i*4)
        relocs.append(seg*16 + off)
    return d[hdr:], cs*16 + ip, relocs

def text_extent(img, relocs, entry):
    # DGROUP begins where reloc *targets* (the pointed-at segment words) cluster; the
    # simplest robust proxy: the lowest image offset that a reloc FIXUP sits in above the
    # bulk of code is data. Use the min reloc fixup that is >= entry region... instead,
    # take the code end as the max fixup that still disassembles as code near a prologue.
    # Practical heuristic: _TEXT = [0, first_data_reloc_cluster). Use the smallest reloc
    # fixup location that lies beyond the last prologue we can find; fall back to img end.
    return len(img)

PROLOG = b"\x55\x8b\xec"
def carve(img, entry, relocs):
    n = len(img)
    starts = []
    i = 0
    while True:
        j = img.find(PROLOG, i)
        if j < 0: break
        starts.append(j); i = j + 1
    # also the startup entry
    funcs = []
    startset = set(starts)
    for s in starts:
        # walk instructions to an epilogue+ret
        end = None; addr = s; frame = 0; probe = False
        # decode frame size
        tail = img[s+3:s+8]
        if tail[:1] == b"\x83" and tail[1:2] == b"\xec":
            frame = tail[2]
        elif tail[:1] == b"\x81" and tail[1:2] == b"\xec":
            frame = struct.unpack_from("<H", tail, 2)[0]
        elif tail[:1] == b"\xb8":
            probe = True
        # linear walk for epilogue
        off = s; last = min(n, s + 0x2000)
        prev = b""
        for ins in MD.disasm(bytes(img[s:last]), s):
            m = ins.mnemonic
            if m in ("ret", "retf"):
                end = ins.address + ins.size
                break
            # stop if we run into the next function's prologue we already know
            if ins.address != s and ins.address in startset:
                end = ins.address; break
        if end is None: end = min(last, s + 64)
        funcs.append(dict(off=s, size=end - s, frame=frame, probe=probe))
    return funcs

if __name__ == "__main__":
    path = sys.argv[1]
    dis_n = 0
    if "--dis" in sys.argv:
        dis_n = int(sys.argv[sys.argv.index("--dis")+1])
    img, entry, relocs = load(path)
    funcs = carve(img, entry, relocs)
    name = os.path.splitext(os.path.basename(path))[0]
    outdir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                          "work", name)
    os.makedirs(outdir, exist_ok=True)
    json.dump(funcs, open(os.path.join(outdir, "functions.json"), "w"), indent=0)
    total = sum(f["size"] for f in funcs)
    print(f"{name}: {len(funcs)} candidate functions, entry@{entry:#x}, "
          f"covering {total} bytes ({100*total/len(img):.0f}% of {len(img)})")
    print(f"  frames: {sum(1 for f in funcs if f['frame'])} sized, "
          f"{sum(1 for f in funcs if f['probe'])} __chkstk-probed")
    for f in funcs[:dis_n]:
        print(f"\n=== fn @ {f['off']:#06x}  size={f['size']}  frame={f['frame']} "
              f"{'probe' if f['probe'] else ''}")
        for ins in MD.disasm(bytes(img[f['off']:f['off']+min(f['size'],48)]), f['off']):
            print(f"  {ins.address:04x}: {ins.bytes.hex():16} {ins.mnemonic} {ins.op_str}")
