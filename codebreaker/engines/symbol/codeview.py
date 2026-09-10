#!/usr/bin/env python3
r"""codeview.py -- parse embedded CodeView 4 (NB09) symbols from a PE/NE binary.

Early Windows betas and SDK/checked builds ship with CodeView debug info appended: internal
(non-exported) function names at (segment, offset). Retail Windows is stripped, so these are a
HYPOTHESIS-tier naming source for the reconstruction -- cross-checked by propagating them onto a
retail twin via the structural diff engine (never an offset map: beta offsets are beta-specific).

Format (NB09 / CV4): the signature "NB09" is followed by a u32 offset (relative to the NB09 base)
to the subsection directory. The directory lists subsections {type(u16), imod(u16), lfo(u32),
cb(u32)}; symbol subsections (sstGlobalPub 0x12a, sstGlobalSym 0x129, sstStaticSym 0x134) hold
length-prefixed records {len(u16), type(u16), payload}. We extract the addressed symbols:
  S_PUB32  0x0203: offset(u32) seg(u16) typind(u16) name(pascal)
  S_LPROC32/S_GPROC32 0x0204/5: 3 ptrs, proc-len, dbg-start/end, offset(u32) seg(u16) ...name
Returns {(seg, offset): name} -- seg is the CV segment (1 == first code section), offset is
segment-relative, i.e. the same space the PE carve names sec<i>_<off>.asm in.
"""
import struct
import sys

S_PUB32, S_LPROC32, S_GPROC32 = 0x0203, 0x0204, 0x0205
_SYM_SUBSECS = {0x129: True, 0x12a: True, 0x134: False}   # type -> has 16-byte symhash header


def parse(path_or_bytes):
    """{(seg, offset): name} for every addressed CodeView symbol in the binary, or {} if none."""
    d = path_or_bytes if isinstance(path_or_bytes, (bytes, bytearray)) else open(path_or_bytes, "rb").read()
    base = d.find(b"NB09")
    if base < 0:
        return {}
    lfo_dir = struct.unpack_from("<I", d, base + 4)[0]
    p = base + lfo_dir
    if p + 16 > len(d):
        return {}
    cb_hdr, cb_ent, c_dir = struct.unpack_from("<HHI", d, p)
    q = p + cb_hdr
    out = {}
    for i in range(c_dir):
        if q + i * cb_ent + 12 > len(d):
            break
        subsec, _imod, lfo, cb = struct.unpack_from("<HHIi", d, q + i * cb_ent)
        if subsec in _SYM_SUBSECS:
            _walk(d, base + lfo, cb, _SYM_SUBSECS[subsec], out)
    return out


def _walk(d, off, cb, skip_hash, out):
    s = off + (16 if skip_hash else 0)                    # CV4 symhash header is 16 bytes
    end = min(off + cb, len(d))
    while s + 4 <= end:
        reclen, rectype = struct.unpack_from("<HH", d, s)
        if reclen < 2:
            break
        nxt = s + 2 + reclen
        if nxt > end:
            break
        pay = s + 4
        if rectype == S_PUB32:                            # offset(4) seg(2) typind(2) name
            o, seg = struct.unpack_from("<IH", d, pay)
            _emit(d, pay + 8, seg, o, out)
        elif rectype in (S_LPROC32, S_GPROC32):           # 3 ptrs(12) len(4) dbg s/e(8) off(4) seg(2) typind(2) flags(1) name
            o, seg = struct.unpack_from("<IH", d, pay + 24)
            _emit(d, pay + 24 + 8 + 1, seg, o, out)
        s = nxt


def _emit(d, lenpos, seg, o, out):
    """lenpos = position of the pascal length byte for the symbol name."""
    if lenpos >= len(d):
        return
    ln = d[lenpos]
    name = d[lenpos + 1:lenpos + 1 + ln].decode("latin1", "replace")
    if name and seg:
        out.setdefault((seg, o), name)


def code_names(path_or_bytes, code_seg=1):
    """{offset: name} restricted to the code segment -- the function names, keyed the way the
    carve/diff corpus addresses functions (section-relative offset in the first code section)."""
    return {o: n for (seg, o), n in parse(path_or_bytes).items() if seg == code_seg}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: codeview.py <pe-or-ne-file>")
    syms = parse(sys.argv[1])
    import collections
    by_seg = collections.Counter(seg for seg, _ in syms)
    print("CodeView symbols: %d  (by segment: %s)" % (len(syms), dict(by_seg)))
    for (seg, o), n in list(syms.items())[:20]:
        print("   seg%d:0x%05x  %s" % (seg, o, n))


if __name__ == "__main__":
    main()
