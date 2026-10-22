#!/usr/bin/env python3
"""Microsoft LINK /EXEPACK decompressor.

Reverses the EXEPACK compression Microsoft LINK applied to the MS C 6.0a passes.
Produces a runnable unpacked MZ (real entry/stack, rebuilt relocation table).

Usage: python unexepack.py <packed.exe> <out.exe>
"""
import sys, struct

def u16(b, o): return b[o] | (b[o+1] << 8)

def unexepack(data):
    if data[:2] != b'MZ': raise SystemExit("not MZ")
    e_cblp   = u16(data, 2)      # bytes on last page
    e_cp     = u16(data, 4)      # pages
    e_crlc   = u16(data, 6)      # reloc count (0 for packed)
    e_cparhdr= u16(data, 8)      # header size in paragraphs
    e_cs     = u16(data, 0x16)   # CS of exepack block (rel to load module)
    e_ip     = u16(data, 0x14)

    hdr_len = e_cparhdr * 16
    img_len = e_cp * 512 - (512 - e_cblp if e_cblp else 0)   # MZ image length
    load = bytearray(data[hdr_len:img_len])                  # load module (packed)

    # EXEPACK header sits at CS:0 within the load module
    ep = e_cs * 16
    real_ip   = u16(load, ep + 0x00)
    real_cs   = u16(load, ep + 0x02)
    exep_size = u16(load, ep + 0x06)
    real_sp   = u16(load, ep + 0x08)
    real_ss   = u16(load, ep + 0x0a)
    dest_len  = u16(load, ep + 0x0c)   # paragraphs
    sig       = u16(load, ep + 0x0e)
    if sig != 0x4252:
        raise SystemExit(f"no RB signature (got {sig:#06x}) - not 16-byte EXEPACK variant")

    packed = load[:ep]                          # compressed region
    block  = bytes(load[ep:ep + exep_size])     # header+stub+relocs+string

    # --- decompress (RLE, read backward) ---
    dst_size = dest_len * 16
    out = bytearray(dst_size)
    src = len(packed) - 1
    dcur = dst_size - 1
    while src >= 0 and packed[src] == 0xFF:     # skip fill padding
        src -= 1
    while True:
        cmd = packed[src]; src -= 1
        length = packed[src] << 8; src -= 1     # high byte first (reading downward)
        length |= packed[src];   src -= 1       # low byte
        op = cmd & 0xFE
        if op == 0xB0:                          # fill
            val = packed[src]; src -= 1
            for _ in range(length):
                out[dcur] = val; dcur -= 1
        elif op == 0xB2:                        # copy
            for _ in range(length):
                out[dcur] = packed[src]; dcur -= 1; src -= 1
        else:
            raise SystemExit(f"bad opcode {cmd:#04x} at src~{src}")
        if cmd & 1:
            break
    # In-place scheme: only the upper region was compressed. The low region
    # [0 .. dcur] is stored uncompressed at the start of the load module.
    out[0:dcur + 1] = packed[0:dcur + 1]

    # --- parse packed relocation table (16 groups: 0x0000..0xF000) ---
    # The table can sit after the embedded "Packed file is corrupt" string, followed
    # by zero padding to the block end. Scan for 16 groups that parse cleanly and are
    # followed only by 0x00 padding, with sane reloc offsets.
    blen = len(block)
    relocs = None
    for start in range(0x10, blen):
        o = start; ok = True; rl = []
        for grp in range(16):
            if o + 2 > blen: ok = False; break
            cnt = u16(block, o); o += 2
            if cnt > 0x2000 or o + 2*cnt > blen: ok = False; break
            for i in range(cnt):
                off = u16(block, o); o += 2
                rl.append((grp << 12, off))
        if not ok or not rl:
            continue
        if any(b != 0 for b in block[o:blen]):     # trailing must be zero padding
            continue
        if any(off >= dst_size + 0x1000 for _, off in rl):  # sane offsets
            continue
        relocs = rl; break
    if relocs is None:
        raise SystemExit("could not locate reloc table")

    # --- build output MZ ---
    nrel = len(relocs)
    reloc_tbl_off = 0x1c
    hdr_paras = ((reloc_tbl_off + 4*nrel) + 15) // 16
    new_hdr_len = hdr_paras * 16
    total = new_hdr_len + len(out)
    pages = (total + 511) // 512
    lastpage = total - (pages - 1) * 512

    hdr = bytearray(new_hdr_len)
    hdr[0:2] = b'MZ'
    struct.pack_into('<H', hdr, 2, lastpage & 0x1ff if lastpage != 512 else 0)
    struct.pack_into('<H', hdr, 4, pages)
    struct.pack_into('<H', hdr, 6, nrel)
    struct.pack_into('<H', hdr, 8, hdr_paras)
    struct.pack_into('<H', hdr, 0x0a, 0x0000)      # minalloc
    struct.pack_into('<H', hdr, 0x0c, 0xffff)      # maxalloc
    struct.pack_into('<H', hdr, 0x0e, real_ss)
    struct.pack_into('<H', hdr, 0x10, real_sp)
    struct.pack_into('<H', hdr, 0x12, 0x0000)      # checksum
    struct.pack_into('<H', hdr, 0x14, real_ip)
    struct.pack_into('<H', hdr, 0x16, real_cs)
    struct.pack_into('<H', hdr, 0x18, reloc_tbl_off)
    struct.pack_into('<H', hdr, 0x1a, 0x0000)      # overlay
    o = reloc_tbl_off
    for seg, off in relocs:
        struct.pack_into('<H', hdr, o, off); struct.pack_into('<H', hdr, o+2, seg); o += 4

    return bytes(hdr) + bytes(out), dict(real_cs=real_cs, real_ip=real_ip,
        real_ss=real_ss, real_sp=real_sp, dest_len=dest_len, nrel=nrel,
        packed_len=len(packed), unpacked_img=len(out))

if __name__ == '__main__':
    data = open(sys.argv[1], 'rb').read()
    out, info = unexepack(data)
    open(sys.argv[2], 'wb').write(out)
    print(f"OK {sys.argv[1]} -> {sys.argv[2]}")
    for k, v in info.items():
        print(f"  {k:12} = {v}  ({v:#x})" if isinstance(v, int) else f"  {k:12} = {v}")
