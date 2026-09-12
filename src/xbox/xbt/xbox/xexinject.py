"""Static XEX code injection — patch or hook Xbox 360 title code, then re-emit
a valid (devkit-resigned) XEX.

The permanent counterpart to `jrpc2 poke`: instead of editing live memory, this
rewrites the image on disk. It decrypts + decompresses the title to its raw PE
image (via xex.py), patches at a virtual address, and re-emits through the
byte-exact `compress_to_normal` writer.

Operations:
  nop    <xex> <vaddr> [count]  <out>   replace N instructions with `nop`
  bytes  <xex> <vaddr> <hex>    <out>   overwrite raw bytes at a VA
  asm    <xex> <vaddr> "<asm>"  <out>   assemble PowerPC and overwrite at a VA
  hook   <xex> <vaddr> <cave> "<asm>" <out>
                                        trampoline: `b cave` at vaddr; at cave run
                                        <asm>, replay the stolen instruction, `b back`

VA -> image offset = vaddr - load_address (from the XEX security header).
"""

import struct
import sys

from xex import open_xex, compress_to_normal
try:
    import ppcasm
except ImportError:
    ppcasm = None

DESCRIPTION = "patch / hook Xbox 360 XEX code at a virtual address, re-emit a resigned XEX"


def _va_off(x, vaddr):
    load = x.security()["load_address"]
    off = vaddr - load
    return load, off


def patch_image(x, vaddr, blob):
    """Return a copy of the title image with `blob` written at `vaddr`."""
    load, off = _va_off(x, vaddr)
    image = bytearray(x.basefile())
    if off < 0 or off + len(blob) > len(image):
        raise ValueError("VA 0x%08X out of image (load 0x%08X, size 0x%X)"
                         % (vaddr, load, len(image)))
    image[off:off + len(blob)] = blob
    return bytes(image), off


def build_hook(x, vaddr, cave, asm):
    """Trampoline: overwrite the instruction at `vaddr` with `b cave`; at `cave`
    emit <asm>, replay the original instruction, then `b (vaddr+4)`."""
    if ppcasm is None:
        raise RuntimeError("ppcasm unavailable")
    load, off = _va_off(x, vaddr)
    image = bytearray(x.basefile())
    stolen = bytes(image[off:off + 4])                 # the instruction we displace
    # cave body: hook asm @ cave, then the stolen instr, then branch back
    body = bytearray(ppcasm.assemble(asm, cave))
    stolen_va = cave + len(body)
    image[_va_off(x, stolen_va)[1]:_va_off(x, stolen_va)[1] + 4] = stolen
    back = ppcasm.assemble_one("b 0x%X" % (vaddr + 4), stolen_va + 4)
    image[off:off + 4] = ppcasm.assemble_one("b 0x%X" % cave, vaddr).to_bytes(4, "big")
    # write cave: body + stolen + back
    coff = _va_off(x, cave)[1]
    image[coff:coff + len(body)] = body
    image[coff + len(body):coff + len(body) + 4] = stolen
    image[coff + len(body) + 4:coff + len(body) + 8] = struct.pack(">I", back)
    return bytes(image), off


def reemit(x, image, out_path, encrypt=False):
    data = compress_to_normal(x, encrypt, image_in=image)
    open(out_path, "wb").write(data)
    return len(data)


def _verify(out_path, vaddr, expect):
    """Re-open the emitted XEX and confirm the bytes at vaddr match `expect`."""
    x = open_xex(out_path)
    _, off = _va_off(x, vaddr)
    got = x.basefile()[off:off + len(expect)]
    return got == expect


def main():
    a = sys.argv[1:]
    ops = ("nop", "bytes", "asm", "hook")
    if not a or a[0] in ("-h", "--help") or a[0] not in ops:
        print("usage:\n"
              "  xexinject nop   <xex> <vaddr> [count] <out>\n"
              "  xexinject bytes <xex> <vaddr> <hex>    <out>\n"
              "  xexinject asm   <xex> <vaddr> \"<asm>\"  <out>\n"
              "  xexinject hook  <xex> <vaddr> <cave> \"<asm>\" <out>")
        return
    op = a[0]
    x = open_xex(a[1])
    vaddr = int(a[2], 0)
    if op == "nop":
        count = int(a[3], 0) if len(a) > 4 else 1
        out = a[-1]
        blob = b"\x60\x00\x00\x00" * count
        image, off = patch_image(x, vaddr, blob)
    elif op == "bytes":
        blob = bytes.fromhex(a[3].replace(" ", ""))
        out = a[4]
        image, off = patch_image(x, vaddr, blob)
    elif op == "asm":
        if ppcasm is None:
            print("ppcasm unavailable"); return
        blob = ppcasm.assemble(a[3], vaddr)
        out = a[4]
        image, off = patch_image(x, vaddr, blob)
    else:  # hook
        cave = int(a[3], 0)
        image, off = build_hook(x, vaddr, cave, a[4])
        out = a[5]
        blob = None
    n = reemit(x, image, out)
    ok = _verify(out, vaddr, image[off:off + (len(blob) if blob else 4)])
    print("%s @0x%08X -> %s (%d bytes)  [re-read %s]" % (
        op, vaddr, out, n, "OK" if ok else "MISMATCH"))


if __name__ == "__main__":
    main()
