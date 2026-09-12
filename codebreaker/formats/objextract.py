#!/usr/bin/env python3
"""
objextract.py — pull the real code bytes out of an OMF .OBJ (byte-exact stage F).

`cc16.py` compiles reconstructed C with the period compiler and emits a .OBJ.
To compare against the shipped target bytes we need the *actual machine code* the
compiler produced — which lives in the OBJ's OMF records, not the /Fc listing.
This module parses those records deterministically:

  LNAMES / SEGDEF  -> which logical segment is the code segment (_TEXT)
  LEDATA           -> the enumerated code bytes, assembled at their offsets
  FIXUPP           -> the relocation sites -> a MASK of bytes that carry
                      absolute addresses (they differ between builds and must be
                      ignored when byte-matching; only the opcode stream matters)

Output per segment: the code bytes + a fixup mask. Matching a function is then
"do these bytes equal the shipped bytes, ignoring masked relocation sites",
which is the mechanical acceptance test the whole no-LLM pipeline rests on.

    python objextract.py --selftest          # compile a fn via cc16, extract it
    python objextract.py foo.obj             # dump segments of an existing OBJ
    python objextract.py foo.obj --seg _TEXT # just the code segment, hex + mask
"""
import argparse
from codebreaker.core import cores
import sys

# OMF record types we care about (even = 16-bit, odd = 32-bit variant)
LNAMES = 0x96
SEGDEF, SEGDEF32 = 0x98, 0x99
LEDATA, LEDATA32 = 0xA0, 0xA1
FIXUPP, FIXUPP32 = 0x9C, 0x9D
PUBDEF, PUBDEF32 = 0x90, 0x91

# FIXUPP location type -> width in bytes of the field being relocated
LOC_WIDTH = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2, 9: 4, 11: 6, 13: 4}


def _records(data):
    """Yield (rtype, body) for each OMF record. body excludes type/len/checksum."""
    i = 0
    n = len(data)
    while i + 3 <= n:
        rtype = data[i]
        rlen = data[i + 1] | (data[i + 2] << 8)
        body = data[i + 3:i + 3 + rlen - 1]      # last byte of rlen is checksum
        yield rtype, body
        i += 3 + rlen
    return


def _index(body, p):
    """Read an OMF index (1 or 2 bytes) at p. Returns (value, new_p)."""
    b = body[p]
    if b & 0x80:
        return (((b & 0x7F) << 8) | body[p + 1]), p + 2
    return b, p + 1


class Obj:
    """Parsed OMF object: segments {name: bytearray}, fixup masks {name: set(offsets)}."""

    def __init__(self, data):
        self.lnames = [""]          # 1-based
        self.segs = []              # [(name, class, length)] 1-based via segs[idx-1]
        self.seg_bytes = {}         # segname -> bytearray (assembled LEDATA)
        self.seg_mask = {}          # segname -> set of relocated byte offsets
        self.pubs = {}              # public symbol name -> (segname, offset)
        self._last = None           # (segname, base_offset) of most recent LEDATA
        self._parse(data)

    def _parse(self, data):
        for rtype, body in _records(data):
            if rtype == LNAMES:
                p = 0
                while p < len(body):
                    ln = body[p]; p += 1
                    self.lnames.append(body[p:p + ln].decode("latin1")); p += ln
            elif rtype in (SEGDEF, SEGDEF32):
                self._segdef(body, wide=(rtype == SEGDEF32))
            elif rtype in (LEDATA, LEDATA32):
                self._ledata(body, wide=(rtype == LEDATA32))
            elif rtype in (FIXUPP, FIXUPP32):
                self._fixupp(body, wide=(rtype == FIXUPP32))
            elif rtype in (PUBDEF, PUBDEF32):
                self._pubdef(body, wide=(rtype == PUBDEF32))

    def _segdef(self, body, wide):
        acbp = body[0]
        p = 1
        if (acbp >> 5) == 0:              # absolute segment: frame(2)+offset(1)
            p += 3
        p += 4 if wide else 2             # segment length
        name_i, p = _index(body, p)
        class_i, p = _index(body, p)
        name = self.lnames[name_i] if name_i < len(self.lnames) else "?%d" % name_i
        cls = self.lnames[class_i] if class_i < len(self.lnames) else ""
        self.segs.append((name, cls))
        self.seg_bytes.setdefault(name, bytearray())
        self.seg_mask.setdefault(name, set())

    def _ledata(self, body, wide):
        seg_i, p = _index(body, 0)
        if wide:
            off = int.from_bytes(body[p:p + 4], "little"); p += 4
        else:
            off = body[p] | (body[p + 1] << 8); p += 2
        chunk = bytes(body[p:])
        name = self.segs[seg_i - 1][0] if 0 < seg_i <= len(self.segs) else "_TEXT"
        buf = self.seg_bytes.setdefault(name, bytearray())
        if len(buf) < off + len(chunk):
            buf.extend(b"\x00" * (off + len(chunk) - len(buf)))
        buf[off:off + len(chunk)] = chunk
        self._last = (name, off)

    def _fixupp(self, body, wide):
        # Best-effort: record which byte ranges are relocated so they can be
        # masked during comparison. THREAD subrecords carry no location; skip.
        if not self._last:
            return
        name, base = self._last
        mask = self.seg_mask.setdefault(name, set())
        p = 0
        while p < len(body):
            first = body[p]
            if not (first & 0x80):        # THREAD subrecord: 1 byte + index(es)
                # method byte tells how many indices follow; skip conservatively
                method = (first >> 2) & 0x03
                p += 1
                _idx, p = _index(body, p)
                continue
            # FIXUP subrecord: locat is 2 bytes
            locat = (first << 8) | body[p + 1]
            loc = (locat >> 10) & 0x0F
            data_off = locat & 0x03FF
            width = LOC_WIDTH.get(loc, 2)
            for k in range(width):
                mask.add(base + data_off + k)
            p += 2
            # Fix Data byte, then frame/target indices + optional displacement.
            fixdata = body[p]; p += 1
            f = (fixdata >> 7) & 1
            frame = (fixdata >> 4) & 0x07
            t = (fixdata >> 3) & 1
            targt = fixdata & 0x03
            if not f and frame in (0, 1, 2):
                _fi, p = _index(body, p)
            if not t:
                _ti, p = _index(body, p)
            if not (fixdata & 0x04):      # P bit clear -> target displacement present
                p += 4 if wide else 2

    def _pubdef(self, body, wide):
        grp_i, p = _index(body, 0)
        seg_i, p = _index(body, p)
        if seg_i == 0:                    # base frame present when no base segment
            p += 2
        segname = self.segs[seg_i - 1][0] if 0 < seg_i <= len(self.segs) else "?"
        while p < len(body):
            ln = body[p]; p += 1
            name = body[p:p + ln].decode("latin1"); p += ln
            if wide:
                off = int.from_bytes(body[p:p + 4], "little"); p += 4
            else:
                off = body[p] | (body[p + 1] << 8); p += 2
            _ti, p = _index(body, p)      # type index
            self.pubs[name] = (segname, off)

    def functions(self):
        """{name: (bytes, mask)} by slicing the code segment between consecutive
        public symbols. Names keep the compiler's leading '_'. The last function
        runs to the end of the segment (trailing padding included)."""
        name, code, mask = self.code_segment()
        here = sorted(((off, nm) for nm, (sg, off) in self.pubs.items() if sg == name))
        out = {}
        for i, (off, nm) in enumerate(here):
            end = here[i + 1][0] if i + 1 < len(here) else len(code)
            out[nm] = (code[off:end], {m - off for m in mask if off <= m < end})
        return out

    def code_segment(self):
        """Return (name, bytes, mask) for the first CODE-class segment (_TEXT)."""
        for name, cls in self.segs:
            if "CODE" in cls.upper() or name.upper().endswith("TEXT"):
                return name, bytes(self.seg_bytes.get(name, b"")), self.seg_mask.get(name, set())
        if self.segs:
            name = self.segs[0][0]
            return name, bytes(self.seg_bytes.get(name, b"")), self.seg_mask.get(name, set())
        return None, b"", set()


def _hexdump(data, mask, width=16):
    out = []
    for i in range(0, len(data), width):
        row = data[i:i + width]
        cells = []
        for j, b in enumerate(row):
            cells.append("--" if (i + j) in mask else "%02x" % b)
        out.append("  %04x  %s" % (i, " ".join(cells)))
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("obj", nargs="?", help="path to an OMF .OBJ file")
    ap.add_argument("--seg", help="only dump this segment name")
    ap.add_argument("--selftest", action="store_true",
                    help="compile a trivial fn via cc16 and extract its code")
    args = ap.parse_args()

    if args.selftest:
        cc16 = cores.get('cc16')
        src = "int add2(int a, int b) { return a + b; }\n"
        r = cc16.compile_c(src, flags="/AM /c")
        if "obj" not in r:
            sys.exit("cc16 produced no OBJ; log:\n" + r.get("log", "")[:800])
        obj = Obj(r["obj"])
        name, code, mask = obj.code_segment()
        print("segments:", [s for s in obj.segs])
        print("code segment: %s  (%d bytes, %d relocated)" % (name, len(code), len(mask)))
        print(_hexdump(code, mask))
        ok = len(code) > 0
        print("\nSELFTEST:", "PASS - extracted code bytes" if ok else "FAIL - no code")
        sys.exit(0 if ok else 1)

    if not args.obj:
        ap.error("give an .OBJ path or --selftest")
    obj = Obj(open(args.obj, "rb").read())
    if args.seg:
        code = bytes(obj.seg_bytes.get(args.seg, b""))
        print("%s: %d bytes" % (args.seg, len(code)))
        print(_hexdump(code, obj.seg_mask.get(args.seg, set())))
    else:
        for name, cls in obj.segs:
            b = obj.seg_bytes.get(name, b"")
            print("  %-12s class=%-8s %6d bytes  %d relocated"
                  % (name, cls, len(b), len(obj.seg_mask.get(name, set()))))


if __name__ == "__main__":
    main()
