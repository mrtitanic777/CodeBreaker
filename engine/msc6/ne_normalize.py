#!/usr/bin/env python3
"""ne_normalize.py -- compact a linked 16-bit NE DLL's DOS-stub area.

LINK 5.20 (win16ddk build) forces the NE header to a 0x400 file boundary,
padding the DOS stub area to 1024 bytes; retail Win16 binaries carry a minimal
128-byte stub with the NE header at 0x80.  This tool rewrites the file so the
DOS stub is exactly the reference stub (retail's 128-byte "cannot be run in DOS
mode") and the NE header + everything after it slides down to sit right behind
it, then fixes every ABSOLUTE (file-offset / sector) field so the result still
loads.  Segment DATA and relocations are copied verbatim -- no code byte moves
relative to the NE header, so segment-level byte-exactness is preserved.

Absolute fields patched (everything else in the NE header is NE-relative):
  * MZ e_lfanew (0x3c)                 -> new NE start
  * per-segment sector number (segtbl) -= (shift bytes >> align)
  * nonresident-names table offset (NE+0x2c, DWORD, file-absolute) -= shift
  * fast-load/gangload sector offset  (NE+0x38, WORD)              -= sectors

Usage: ne_normalize.py <in.dll> <ref_stub.bin|ref.dll> <out.dll>
  ref may be a raw stub file OR a donor NE/PE whose leading MZ stub (up to its
  own e_lfanew) is used as the reference stub.
"""
import struct, sys


def load_ref_stub(path):
    b = open(path, 'rb').read()
    if b[:2] == b'MZ':
        lfanew = struct.unpack_from('<H', b, 0x3c)[0]
        if 0 < lfanew <= len(b):
            return b[:lfanew]
    return b


def normalize(inp, refstub, outp):
    d = bytearray(open(inp, 'rb').read())
    ne = struct.unpack_from('<H', d, 0x3c)[0]
    stub = load_ref_stub(refstub)
    new_ne = len(stub)
    if new_ne % 16:
        raise SystemExit("reference stub length 0x%x is not paragraph-aligned" % new_ne)
    if new_ne > ne:
        raise SystemExit("stub (0x%x) is larger than current NE offset (0x%x); nothing to compact" % (new_ne, ne))
    shift = ne - new_ne                       # bytes the NE (and all after) moves earlier
    align = struct.unpack_from('<H', d, ne + 0x32)[0] or 9
    if shift % (1 << align):
        raise SystemExit("shift 0x%x not a multiple of the sector size 0x%x" % (shift, 1 << align))
    sect_delta = shift >> align

    body = d[ne:]                             # NE header + tables + segments, verbatim
    out = bytearray(stub) + body

    # MZ e_lfanew
    struct.pack_into('<H', out, 0x3c, new_ne)

    n = struct.unpack_from('<H', out, new_ne + 0x1c)[0]     # segment count
    segtbl = struct.unpack_from('<H', out, new_ne + 0x22)[0]
    for i in range(n):
        p = new_ne + segtbl + i * 8
        sec = struct.unpack_from('<H', out, p)[0]
        if sec:                               # 0 = no file image (pure BSS): leave it
            struct.pack_into('<H', out, p, sec - sect_delta)

    # nonresident-names table offset is FILE-ABSOLUTE (unlike the resident one)
    nonres = struct.unpack_from('<I', out, new_ne + 0x2c)[0]
    if nonres:
        struct.pack_into('<I', out, new_ne + 0x2c, nonres - shift)

    # fast-load / gangload area sector offset (length stays)
    flo = struct.unpack_from('<H', out, new_ne + 0x38)[0]
    if flo:
        struct.pack_into('<H', out, new_ne + 0x38, flo - sect_delta)

    open(outp, 'wb').write(out)
    print("in=%d  stub 0x%x->0x%x  shift=0x%x (%d sectors)  out=%d bytes"
          % (len(d), ne, new_ne, shift, sect_delta, len(out)))
    return out


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print(__doc__); raise SystemExit(2)
    normalize(sys.argv[1], sys.argv[2], sys.argv[3])
