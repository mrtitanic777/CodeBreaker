#!/usr/bin/env python3
r"""Decompress a Win95 W4 (compressed VMM32) container into its uncompressed W3 payload.

An installed Win95 builds vmm32.vxd as a **W4**: an MZ stub whose real-mode loader, when it maps the
image, decompresses a body of 8 KB pages. Each page is compressed with an MRCI/DoubleSpace-family
LZ77 code (a bit-oriented prefix code -- literals, plus back-references with 6/8/12-bit distances at
bases 0/64/320, max distance 4415 = end-of-page, and a gamma length code from 2). The decompressor is
NOT a separate tool -- it is the routine *inside the W4's own real-mode stub*, an 8-way-unrolled
bit machine. Rather than re-derive that bit machine by hand, this module locates that routine and
runs it directly under the Unicorn CPU emulator -- so the output is Microsoft's own, bit-for-bit.

The W4 header (at e_lfanew): 'W4', version(2), page size(2, = 0x2000), page count(2), 'DS' marker,
then a page-count array of DWORD file offsets to each compressed block; a block whose length equals
the page size is stored verbatim. The decompressed pages concatenate to the W3 container (which
starts 'W3' and lists the bundled VxD modules).

    from w4 import decompress_w4
    w3 = decompress_w4(open('vmm32.vxd','rb').read())     # -> uncompressed W3 payload

CLI:  python w4.py vmm32.vxd [out.w3]        (prints the module list; writes the W3 if out given)

Requires the `unicorn` package (pip install unicorn).
"""
import struct, sys

# entry of the stub's decompressor: pushaw; push ds; push es; mov word cs:[imm], si
_DEC_SIG = b"\x60\x1e\x06\x2e\x89\x36"


def _find_decompressor(stub):
    i = stub.find(_DEC_SIG)
    if i < 0:
        raise ValueError("W4 decompressor routine not found in the stub")
    return i


def decompress_w4(data):
    """Return the uncompressed W3 payload of a W4 vmm32.vxd (or `data` unchanged if already W3/LE)."""
    lfa = struct.unpack_from("<I", data, 0x3c)[0]
    if data[lfa:lfa + 2] != b"W4":
        return data                                   # already uncompressed (W3) or a plain module
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UcError
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS,
                                   UC_X86_REG_SI, UC_X86_REG_DI, UC_X86_REG_SP,
                                   UC_X86_REG_AX, UC_X86_REG_CX)
    page = struct.unpack_from("<H", data, lfa + 4)[0]
    npages = struct.unpack_from("<H", data, lfa + 6)[0]
    tab = lfa + 0x10
    offs = [struct.unpack_from("<I", data, tab + i * 4)[0] for i in range(npages)] + [len(data)]

    hdr = struct.unpack_from("<H", data, 8)[0] * 16      # MZ header paragraphs -> cs base
    stub = data[0:lfa]
    dec = _find_decompressor(stub)                       # file offset of the decompressor routine
    cs_base = hdr
    dec_off = dec - cs_base                              # cs-relative offset of the routine
    CS = cs_base >> 4
    SRC, DST, STK = 0x2000, 0x2400, 0x2800

    def one(block):
        uc = Uc(UC_ARCH_X86, UC_MODE_16)
        uc.mem_map(0, 0x40000)
        uc.mem_write(0, stub)
        uc.mem_write(SRC * 16, block)
        uc.mem_write(DST * 16, b"\x00" * (page + 64))
        uc.reg_write(UC_X86_REG_CS, CS)
        uc.reg_write(UC_X86_REG_DS, SRC); uc.reg_write(UC_X86_REG_SI, 0)
        uc.reg_write(UC_X86_REG_ES, DST); uc.reg_write(UC_X86_REG_DI, 0)
        uc.reg_write(UC_X86_REG_SS, STK); uc.reg_write(UC_X86_REG_SP, 0x2000)
        uc.reg_write(UC_X86_REG_AX, page); uc.reg_write(UC_X86_REG_CX, len(block))
        ret = cs_base + 0x3000                           # sentinel return target
        uc.mem_write(STK * 16 + 0x2000, struct.pack("<H", 0x3000))
        try:
            uc.emu_start(cs_base + dec_off, ret, count=4_000_000)
        except UcError:
            pass
        return bytes(uc.mem_read(DST * 16, page))

    out = bytearray()
    for p in range(npages):
        blk = data[offs[p]:offs[p + 1]]
        out += blk if len(blk) == page else one(blk)     # a full-size block is stored verbatim
    return bytes(out)


def modules(w3):
    """[(name, offset, size), ...] of the VxDs bundled in a W3 payload."""
    if w3[:2] != b"W3":
        return []
    cnt = struct.unpack_from("<I", w3, 4)[0]
    out = []
    for i in range(cnt):
        p = 0x10 + i * 16
        nm = w3[p:p + 8].rstrip(b" \x00").decode("latin1")
        mo, ms = struct.unpack_from("<II", w3, p + 8)
        out.append((nm, mo, ms))
    return out


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__.strip().splitlines()[-1]); sys.exit(2)
    w3 = decompress_w4(open(sys.argv[1], "rb").read())
    mods = modules(w3)
    print("decompressed %d bytes; %d bundled modules:" % (len(w3), len(mods)))
    print("  " + " ".join(n for n, _, _ in mods))
    if len(sys.argv) > 2:
        open(sys.argv[2], "wb").write(w3)
        print("wrote", sys.argv[2])
