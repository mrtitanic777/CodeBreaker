#!/usr/bin/env python3
"""Extract all files from the eight MS C 4.0 360K FAT12 floppy images.

Pure-Python FAT12 reader (no deps). Writes extracted/diskNN/<FILE>.
Usage: python extract_disks.py
"""
import struct, os

BASE = os.path.join(os.path.dirname(__file__), "..", "..",
                    "Microsoft C Compiler 4.0 (5.25-360k)")
OUT = os.path.join(os.path.dirname(__file__), "..", "..", "extracted")


def read_fat12(data: bytes):
    bps = struct.unpack('<H', data[11:13])[0]
    spc = data[13]
    rsvd = struct.unpack('<H', data[14:16])[0]
    nfat = data[16]
    rootent = struct.unpack('<H', data[17:19])[0]
    spf = struct.unpack('<H', data[22:24])[0]
    fat = data[rsvd * bps:(rsvd + spf) * bps]
    root_start = (rsvd + nfat * spf) * bps
    data_start = root_start + rootent * 32

    def fatval(c):
        off = c + c // 2
        v = struct.unpack('<H', fat[off:off + 2])[0]
        return (v >> 4) if (c & 1) else (v & 0xFFF)

    files = []
    for i in range(rootent):
        e = data[root_start + i * 32:root_start + i * 32 + 32]
        if e[0] == 0:
            break
        if e[0] == 0xE5:
            continue
        attr = e[11]
        if attr & 0x08 or attr & 0x10 or (attr & 0x0F) == 0x0F:
            continue  # volume label / dir / LFN
        name = e[0:8].decode('latin1').rstrip()
        ext = e[8:11].decode('latin1').rstrip()
        clus = struct.unpack('<H', e[26:28])[0]
        sz = struct.unpack('<I', e[28:32])[0]
        fn = name + ('.' + ext if ext else '')
        content = bytearray()
        c = clus
        while 2 <= c < 0xFF0:
            start = data_start + (c - 2) * spc * bps
            content += data[start:start + spc * bps]
            c = fatval(c)
        files.append((fn, bytes(content[:sz])))
    return files


def main():
    total = 0
    for n in range(1, 9):
        img = os.path.join(BASE, f"disk{n:02d}.img")
        d = os.path.join(OUT, f"disk{n:02d}")
        os.makedirs(d, exist_ok=True)
        for fn, content in read_fat12(open(img, 'rb').read()):
            open(os.path.join(d, fn), 'wb').write(content)
            total += 1
    print("extracted", total, "files to", os.path.abspath(OUT))


if __name__ == "__main__":
    main()
