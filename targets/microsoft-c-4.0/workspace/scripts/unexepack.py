#!/usr/bin/env python3
"""Deterministic Microsoft EXEPACK unpacker.

Reverses the EXEPACK wrapper on the MS C 4.0 tools to recover a plain MZ .EXE
with the original entry point (CS:IP), stack (SS:SP), and relocation table.

Format (verified against MSC.EXE, disk01):
  * MZ header of a packed file: relocs=0, IP=0x10, SP=0x80, e_cparhdr=0x20.
  * A 16-byte EXEPACK header sits at image offset CS*16:
        +0x00 real_ip   +0x02 real_cs   +0x04 mem_start   +0x06 exepack_size
        +0x08 real_sp   +0x0A real_ss   +0x0C dest_len(paras)   +0x0E 'RB'
    (decompressor code begins at +0x10, hence the packed IP=0x10.)
  * Compressed data = image[0 : CS*16], unpacked *downward* from the top:
        skip trailing 0xFF padding, then read commands high->low:
        cmd byte b: op=b&0xFE, last=b&0x01
          op 0xB0 FILL: count=word below cmd, fill=byte below that
          op 0xB2 COPY: count=word below cmd, then `count` literal bytes below
        stop after a command whose last-bit is set.
  * Packed reloc table lives in the exepack block after the decompressor:
        16 groups (segment bases 0x0000,0x1000,...,0xF000); each = word count
        followed by `count` 16-bit offsets. Located by exact-consumption scan.

Usage:
  python unexepack.py <packed.exe> <out.exe>
  python unexepack.py <packed.exe>            # -> <packed>.unpacked.exe next to out
"""
import struct, sys, json, os

FILL, COPY = 0xB0, 0xB2


def u16(b, o): return struct.unpack('<H', b[o:o + 2])[0]


def is_exepack(mz_image_hdr):
    ip, cs = u16(mz_image_hdr, 20), u16(mz_image_hdr, 22)
    return u16(mz_image_hdr, 6) == 0 and ip == 0x10  # relocs==0, IP==0x10


def parse_reloc_table(block, start):
    """Parse 16 count-prefixed offset groups from block[start:].
    Returns (relocs, end) or None if malformed. relocs = [(seg, off), ...]."""
    relocs, p, n = [], start, len(block)
    for group in range(16):
        if p + 2 > n:
            return None
        cnt = u16(block, p); p += 2
        if cnt > 0x2000:            # sanity: no group has that many fixups
            return None
        base = group * 0x1000
        for _ in range(cnt):
            if p + 2 > n:
                return None
            relocs.append((base, u16(block, p))); p += 2
    return relocs, p


def find_relocs(block, hdr_len, dest_len, unpacked):
    """Scan for the reloc-table start: 16 groups that end at block end (mod
    zero padding) and whose targets are valid segment words in the image."""
    ulen = len(unpacked)
    best = None
    for s in range(hdr_len, len(block) - 1):
        r = parse_reloc_table(block, s)
        if not r:
            continue
        relocs, end = r
        if end > len(block) or any(block[end:]):   # trailing must be zero pad
            continue
        # validate: each fixup lands inside the image and holds a plausible seg
        ok = True
        for seg, off in relocs:
            lin = seg * 16 + off
            if lin + 2 > ulen:
                ok = False; break
            if u16(unpacked, lin) >= dest_len + 0x10:  # segment word in range
                ok = False; break
        if ok and relocs:
            # prefer the earliest fully-valid parse
            best = relocs
            break
    return best or []


def unpack(data: bytes):
    hdr = u16(data, 8) * 16
    img = bytearray(data[hdr:])
    assert is_exepack(data), "not an EXEPACK image (relocs!=0 or IP!=0x10)"
    ep = u16(data, 22) * 16                        # CS*16
    h = img[ep:ep + 16]
    real_ip, real_cs, _mem, esize, real_sp, real_ss, dest_len, sig = \
        struct.unpack('<8H', h)
    assert sig == 0x4252, f"missing 'RB' signature (got {sig:#06x})"

    unpacked_len = dest_len * 16
    out = bytearray(unpacked_len)
    dst = unpacked_len
    src = ep - 1
    while src >= 0 and img[src] == 0xFF:           # skip paragraph padding
        src -= 1
    while True:
        cmd = img[src]; src -= 1
        op, last = cmd & 0xFE, cmd & 0x01
        count = img[src - 1] | (img[src] << 8); src -= 2
        if op == FILL:
            fill = img[src]; src -= 1
            dst -= count
            out[dst:dst + count] = bytes([fill]) * count
        elif op == COPY:
            src -= count
            dst -= count
            out[dst:dst + count] = img[src + 1:src + 1 + count]
        else:
            raise ValueError(f"Packed file is corrupt: bad opcode {cmd:#04x} at {src+1:#x}")
        if last:
            break
    # EXEPACK may leave the first `dst` bytes uncompressed in place: the command
    # stream regenerates only out[dst:], and the low region is the original
    # image head, still present at the bottom of the packed load module.
    skip = dst
    if skip:
        out[0:skip] = img[0:skip]

    block = img[ep:ep + esize]
    relocs = find_relocs(block, 16, dest_len, out)

    return {
        "image": bytes(out),
        "relocs": relocs,
        "cs": real_cs, "ip": real_ip, "ss": real_ss, "sp": real_sp,
        "dest_len": dest_len, "exepack_size": esize,
    }


def build_mz(u):
    img, relocs = u["image"], u["relocs"]
    nrel = len(relocs)
    hdr_len = (0x1C + nrel * 4 + 15) & ~15         # paragraph-aligned
    total = hdr_len + len(img)
    pages = (total + 511) // 512
    last = total % 512
    # min alloc: cover BSS/stack above the file image; grab all memory (max=FFFF)
    top = max(len(img), u["ss"] * 16 + u["sp"])
    minalloc = (top - len(img) + 15) // 16
    h = bytearray(hdr_len)
    struct.pack_into('<13H', h, 0,
                     0x5A4D, last, pages, nrel, hdr_len // 16,
                     minalloc & 0xFFFF, 0xFFFF,
                     u["ss"], u["sp"], 0, u["ip"], u["cs"], 0x1C)
    off = 0x1C
    for seg, o in relocs:
        struct.pack_into('<HH', h, off, o, seg); off += 4
    return bytes(h) + img


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 2
    src = sys.argv[1]
    out = sys.argv[2] if len(sys.argv) > 2 else \
        os.path.splitext(src)[0] + ".unpacked.exe"
    u = unpack(open(src, 'rb').read())
    mz = build_mz(u)
    open(out, 'wb').write(mz)
    man = {"source": os.path.abspath(src), "output": os.path.abspath(out),
           "oep_cs": u["cs"], "oep_ip": u["ip"], "ss": u["ss"], "sp": u["sp"],
           "image_len": len(u["image"]), "relocs": len(u["relocs"]),
           "file_len": len(mz)}
    open(out + ".json", 'w').write(json.dumps(man, indent=2))
    print(f"unpacked {os.path.basename(src)} -> {os.path.basename(out)}")
    print(f"  OEP  CS:IP = {u['cs']:04x}:{u['ip']:04x}   SS:SP = {u['ss']:04x}:{u['sp']:04x}")
    print(f"  image = {len(u['image'])} bytes   relocs = {len(u['relocs'])}   file = {len(mz)} bytes")
    return 0


if __name__ == "__main__":
    sys.exit(main())
