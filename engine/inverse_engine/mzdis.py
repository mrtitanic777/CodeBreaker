#!/usr/bin/env python3
"""mzdis.py - MZ/.COM loader + 16-bit disassembler for the Lattice C RE project.

Usage:
  python mzdis.py <file.exe|file.com> header      # dump MZ header + relocations
  python mzdis.py <file> dis [start] [len]        # linear disasm of load image
                                                  # start/len are file-image offsets (hex ok)
  python mzdis.py <file> relocs                   # list relocation target offsets

The load image is the bytes after the MZ header (for .EXE) or the whole file
(for .COM, loaded at PSP:0100). Offsets printed are *image* offsets (0-based
into the load module), which for a single-segment program == the offset from CS.
"""
import sys, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

def read(path):
    with open(path, "rb") as f:
        return f.read()

def parse_mz(data):
    if data[:2] not in (b"MZ", b"ZM"):
        return None
    (sig, lastpage, pages, nreloc, hdrpara, minalloc, maxalloc,
     ss, sp, cksum, ip, cs, reloff, overlay) = struct.unpack_from("<2sHHHHHHHHHHHHH", data, 0)
    hdr_bytes = hdrpara * 16
    total = (pages - 1) * 512 + (lastpage if lastpage else 512)
    relocs = []
    off = reloff
    for _ in range(nreloc):
        r_off, r_seg = struct.unpack_from("<HH", data, off)
        # file offset of the reloc target word within the load image
        img_off = r_seg * 16 + r_off
        relocs.append((r_seg, r_off, img_off))
        off += 4
    return dict(lastpage=lastpage, pages=pages, nreloc=nreloc, hdrpara=hdrpara,
                hdr_bytes=hdr_bytes, minalloc=minalloc, maxalloc=maxalloc,
                ss=ss, sp=sp, cksum=cksum, ip=ip, cs=cs, reloff=reloff,
                overlay=overlay, total=total, relocs=relocs,
                image=data[hdr_bytes:total] if total else data[hdr_bytes:])

def cmd_header(path):
    data = read(path)
    mz = parse_mz(data)
    if not mz:
        print("Not an MZ file (maybe .COM). Size=%d" % len(data)); return
    print(f"File size        : {len(data)}")
    print(f"Header bytes     : {mz['hdr_bytes']} (0x{mz['hdr_bytes']:x})")
    print(f"Pages/lastpage   : {mz['pages']} / {mz['lastpage']}  => image total {mz['total']} (0x{mz['total']:x})")
    print(f"Image size       : {len(mz['image'])} (0x{len(mz['image']):x})")
    print(f"Entry  CS:IP     : {mz['cs']:04x}:{mz['ip']:04x}  (image off 0x{mz['cs']*16+mz['ip']:x})")
    print(f"Stack  SS:SP     : {mz['ss']:04x}:{mz['sp']:04x}  (image off 0x{mz['ss']*16+mz['sp']:x})")
    print(f"minalloc/maxalloc: 0x{mz['minalloc']:x} / 0x{mz['maxalloc']:x} paras")
    print(f"Relocations      : {mz['nreloc']} (table @0x{mz['reloff']:x})")
    for i,(rs,ro,io) in enumerate(mz['relocs']):
        word = struct.unpack_from("<H", mz['image'], io)[0] if io+2 <= len(mz['image']) else None
        print(f"  [{i}] seg:off {rs:04x}:{ro:04x} image_off 0x{io:04x} -> word 0x{word:04x}" if word is not None
              else f"  [{i}] seg:off {rs:04x}:{ro:04x} image_off 0x{io:04x} (out of range)")

def cmd_relocs(path):
    data = read(path); mz = parse_mz(data)
    reloc_offs = set(io for _,_,io in mz['relocs']) if mz else set()
    for io in sorted(reloc_offs):
        print(f"0x{io:04x}")

def get_image(path):
    data = read(path)
    mz = parse_mz(data)
    if mz:
        return mz['image'], set(io for _,_,io in mz['relocs']), 0   # exe: org 0
    return data, set(), 0x100                                       # com: org 0x100

def cmd_dis(path, start=None, length=None):
    image, relocs, org = get_image(path)
    # 'start' is an ADDRESS (org-based); slice index = addr - org
    start = int(start, 16) if isinstance(start, str) else (start if start is not None else org)
    if length is None:
        length = len(image) - (start - org)
    else:
        length = int(length, 16) if isinstance(length, str) else length
    code = image[start-org:start-org+length]
    md = Cs(CS_ARCH_X86, CS_MODE_16)
    md.detail = False
    import re as _re
    def wrap16(m):
        return "0x%04x" % (int(m.group(0), 16) & 0xffff)
    # skip-and-resume: capstone stops at the first undecodable byte, so loop,
    # emit a `db` for the stuck byte, then continue past it.
    pos = start
    end = start + length
    while pos < end:
        decoded_any = False
        for insn in md.disasm(code[pos-start:end-start], pos):
            b = insn.bytes.hex()
            op_str = _re.sub(r"0xffff[0-9a-f]{4}", wrap16, insn.op_str)
            mark = ""
            for o in range(insn.address, insn.address+insn.size):
                if o in relocs:
                    mark = "  ; <reloc>"
            print(f"{insn.address:04x}: {b:<14} {insn.mnemonic} {op_str}{mark}")
            pos = insn.address + insn.size
            decoded_any = True
        if pos < end:
            bad = code[pos-start]
            print(f"{pos:04x}: {bad:02x}             db 0x{bad:02x}")
            pos += 1

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(__doc__); sys.exit(1)
    path, cmd = sys.argv[1], sys.argv[2]
    if cmd == "header": cmd_header(path)
    elif cmd == "relocs": cmd_relocs(path)
    elif cmd == "dis": cmd_dis(path, *sys.argv[3:5])
    else: print("unknown cmd", cmd)
