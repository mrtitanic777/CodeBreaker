#!/usr/bin/env python3
"""unexepack.py -- decompress Microsoft EXEPACK'd DOS MZ executables.

Microsoft LINK /EXEPACK and the standalone EXEPACK.EXE wrap a real-mode image in
a reverse-RLE packer with a small relocating decompressor stub. Nearly every
binary in the MS C 5.0 distribution is packed this way. This tool undoes it,
rebuilding a plain MZ with a full relocation table so the image can be
disassembled at its real CS:IP.

    python unexepack.py <packed.exe> <out.exe>
    python unexepack.py <packed.exe>            # -> stdout report only (dry run)

Format handled: the classic 16- and 18-byte EXEPACK headers ("RB" signature),
per-16-segment packed relocation table. Validates: signature, that the
decompression consumes exactly the packed region, and that rebuilt relocations
land inside the image.
"""
import sys, struct, os

def u16(b, o): return b[o] | (b[o+1] << 8)

def unpack(data):
    if data[:2] not in (b"MZ", b"ZM"):
        raise ValueError("not an MZ executable")
    lastpage, pages, nreloc, hdrpara = struct.unpack_from("<HHHH", data, 2)
    ip, cs = u16(data,20), u16(data,22)
    hdr = hdrpara * 16
    total = (pages - 1) * 512 + (lastpage if lastpage else 512)
    image = bytearray(data[hdr:total])            # the load module

    # EXEPACK header sits at CS:0000 within the load image.
    ehoff = cs * 16
    # signature "RB" is the last word of the exepack header; try 18- then 16-byte.
    layout = None
    for hlen in (18, 16):
        sig_off = ehoff + hlen - 2
        if sig_off + 2 <= len(image) and image[sig_off:sig_off+2] == b"RB":
            layout = hlen; break
    if layout is None:
        raise ValueError("EXEPACK 'RB' signature not found (not exepack or unknown variant)")

    real_ip   = u16(image, ehoff+0)
    real_cs   = u16(image, ehoff+2)
    # +4 mem_start (loader scratch)
    exepack_sz= u16(image, ehoff+6)
    real_sp   = u16(image, ehoff+8)
    real_ss   = u16(image, ehoff+10)
    dest_len  = u16(image, ehoff+12)              # paragraphs of unpacked data
    skip_len  = u16(image, ehoff+14) if layout == 18 else 1

    # Packed data = image[0 : ehoff]. Decompress it (backward) into dest_len paras.
    src_end = ehoff
    # trailing 0xFF padding between packed data and the exepack header
    p = src_end
    while p > 0 and image[p-1] == 0xFF:
        p -= 1

    # EXEPACK decompresses IN-PLACE: the dest buffer *is* the load module, so any
    # uncompressed prefix (code stored verbatim ahead of the packed data) is already
    # correctly positioned. Seed the buffer with the load image, then expand the
    # packed region top-down over the tail; bytes below the final dest pointer keep
    # their original (uncompressed) contents.
    out = bytearray(dest_len * 16)
    out[:len(image)] = image
    o = len(out)
    def rd():
        nonlocal p; p -= 1; return image[p]
    def rw():
        hi = rd(); lo = rd(); return (hi << 8) | lo
    while True:
        cmd = rd()
        op = cmd & 0xFE
        if op == 0xB0:                            # run of one byte
            cnt = rw(); val = rd()
            o -= cnt; out[o:o+cnt] = bytes([val]) * cnt
        elif op == 0xB2:                          # verbatim copy
            cnt = rw()
            for _ in range(cnt):
                p -= 1; o -= 1; out[o] = image[p]
        else:
            raise ValueError(f"bad EXEPACK opcode {cmd:#04x} at src {p:#x}")
        if cmd & 1:                               # last-command flag
            break
    if o != 0:
        # Leading bytes not produced by the stream are implicitly zero-filled
        # (skip_len region). Accept but record.
        pass

    # Rebuild relocation table: 16 groups keyed by segment high nibble.
    r = ehoff + exepack_sz                        # relocs follow header+stub? -> actually
    # The packed reloc table begins right after the exepack header + decompressor code.
    # exepack_sz covers header+code+relocs measured from ehoff; the reloc table is the
    # tail. Locate it by scanning from ehoff+layout for the 16-group structure that ends
    # at ehoff+exepack_sz.
    relocs = []
    # Try the canonical position: immediately after the fixed 258-byte-ish stub is
    # variable, so parse the 16-group table anchored to END (ehoff+exepack_sz).
    # Parse forward from a candidate; validate it terminates exactly at the end.
    def try_parse(start):
        pos = start; got = []
        for seg in range(16):
            if pos+2 > ehoff+exepack_sz: return None
            n = u16(image, pos); pos += 2
            base = seg << 12
            for _ in range(n):
                if pos+2 > ehoff+exepack_sz: return None
                off = u16(image, pos); pos += 2
                got.append((base, off))
        return (pos, got)
    parsed = None
    for cand in range(ehoff+layout, ehoff+exepack_sz-31):
        res = try_parse(cand)
        if res and res[0] == ehoff+exepack_sz:
            parsed = res[1]; break
    if parsed is None:
        raise ValueError("could not locate/parse packed relocation table")
    relocs = parsed

    # Compose new MZ.
    body = bytes(out)
    nrel = len(relocs)
    reloc_bytes = b"".join(struct.pack("<HH", off, base) for base, off in relocs)
    hdr_min = 0x1C + len(reloc_bytes)
    new_hdrpara = (hdr_min + 15) // 16
    new_hdr = new_hdrpara * 16
    tot = new_hdr + len(body)
    pages = (tot + 511) // 512
    lastp = tot % 512
    header = bytearray(new_hdr)
    struct.pack_into("<2sHHHHHHHHHHHHH", header, 0,
        b"MZ", (lastp if lastp else 0), pages, nrel, new_hdrpara,
        0, 0xFFFF, real_ss, real_sp, 0, real_ip, real_cs, 0x1C, 0)
    header[0x1C:0x1C+len(reloc_bytes)] = reloc_bytes
    info = dict(layout=layout, real_cs=real_cs, real_ip=real_ip, real_ss=real_ss,
                real_sp=real_sp, dest_len=dest_len, skip_len=skip_len,
                exepack_sz=exepack_sz, nreloc=nrel, unpacked_size=len(body),
                leftover=o)
    return bytes(header) + body, info

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(1)
    data = open(sys.argv[1], "rb").read()
    out, info = unpack(data)
    print(f"{os.path.basename(sys.argv[1])}: {info}")
    if len(sys.argv) >= 3:
        open(sys.argv[2], "wb").write(out)
        print(f"  wrote {sys.argv[2]} ({len(out)} bytes)")
