#!/usr/bin/env python3
r"""autobuild95.py -- the authentic-rebuild + binary-verify engine, aimable at any Win95 module.

Automates the workflow proven by hand on LZ32.DLL: CLASSIFY a binary by how Microsoft actually built
it, RECOVER authentic source for that class, BUILD it with the period toolchain, DIFF the result
against the shipped binary, and DOCUMENT the verification level. This file is the FOUNDATION --
the binary model (PE/NE parse), the classifier, and the differ; the per-class recoverers and the
build drivers plug into it.

The guiding standard (design/RECONSTRUCTION_STANDARD.md): authentic-method + binary-verified, not
source-text-exact. The differ reports what is actually verifiable -- entry point, section layout,
exports, imports, size -- never "we guessed the names."

    wt --target win95 autobuild <binary> [--classify] [--diff <built.dll>]
"""
import struct


# ------------------------------------------------------------------ binary model (PE + NE)
class PE:
    """Minimal PE32 reader: sections, entry, exports, imports (name + by-ordinal)."""
    def __init__(self, path):
        self.path = path
        self.d = d = open(path, "rb").read()
        self.ok = len(d) > 0x40 and d[:2] == b"MZ"
        self.e = struct.unpack("<I", d[0x3c:0x40])[0] if self.ok else 0
        self.is_pe = self.ok and d[self.e:self.e + 4] == b"PE\x00\x00"
        if not self.is_pe:
            return
        opt = self.e + 24
        self.opt = opt
        self.nsec = struct.unpack("<H", d[self.e + 6:self.e + 8])[0]
        self.entry = struct.unpack("<I", d[opt + 16:opt + 20])[0]
        self.file_align = struct.unpack("<I", d[opt + 36:opt + 40])[0]
        self.sec_align = struct.unpack("<I", d[opt + 32:opt + 36])[0]
        self.sizeimg = struct.unpack("<I", d[opt + 56:opt + 60])[0]
        sh = opt + struct.unpack("<H", d[self.e + 20:self.e + 22])[0]
        self.secs = []
        for i in range(self.nsec):
            o = sh + i * 40
            nm = d[o:o + 8].rstrip(b"\x00").decode("latin1")
            vs = struct.unpack("<I", d[o + 8:o + 12])[0]
            va = struct.unpack("<I", d[o + 12:o + 16])[0]
            rs = struct.unpack("<I", d[o + 16:o + 20])[0]
            pr = struct.unpack("<I", d[o + 20:o + 24])[0]
            self.secs.append((nm, va, vs, rs, pr))
        self.dd = opt + 96

    def _o2(self, r):
        for nm, va, vs, rs, pr in self.secs:
            if va <= r < va + max(vs, rs, 1):
                return pr + (r - va)
        return None

    def _file2rva(self, o):
        for nm, va, vs, rs, pr in self.secs:
            if rs and pr <= o < pr + rs:
                return va + (o - pr)
        return None

    def exports(self):
        out = []
        rva = struct.unpack("<I", self.d[self.dd:self.dd + 4])[0]
        if not rva:
            return out
        eo = self._o2(rva)
        base = struct.unpack("<I", self.d[eo + 16:eo + 20])[0]
        nnam = struct.unpack("<I", self.d[eo + 24:eo + 28])[0]
        anam = struct.unpack("<I", self.d[eo + 32:eo + 36])[0]
        aord = struct.unpack("<I", self.d[eo + 36:eo + 40])[0]
        no, oo = self._o2(anam), self._o2(aord)
        for i in range(nnam):
            nr = struct.unpack("<I", self.d[no + i * 4:no + i * 4 + 4])[0]
            od = struct.unpack("<H", self.d[oo + i * 2:oo + i * 2 + 2])[0]
            out.append((base + od, self.d[self._o2(nr):].split(b"\x00")[0].decode("latin1")))
        return sorted(out)

    def imports(self):
        """-> dict dll -> list of 'name' or '@ordinal'."""
        out = {}
        rva = struct.unpack("<I", self.d[self.dd + 8:self.dd + 12])[0]
        if not rva:
            return out
        io = self._o2(rva)
        while True:
            oft, ts, fw, nr, ft = struct.unpack("<IIIII", self.d[io:io + 20])
            io += 20
            if nr == 0:
                break
            dll = self.d[self._o2(nr):].split(b"\x00")[0].decode("latin1")
            names = []
            t = self._o2(oft or ft)
            while True:
                v = struct.unpack("<I", self.d[t:t + 4])[0]
                t += 4
                if v == 0:
                    break
                if v & 0x80000000:
                    names.append("@%d" % (v & 0xFFFF))
                else:
                    names.append(self.d[self._o2(v) + 2:].split(b"\x00")[0].decode("latin1"))
            out[dll] = names
        return out

    def export_addrs(self):
        """-> dict name -> function RVA (AddressOfFunctions[ordinal]); the stub location, so we can
        read each export's code (arity, marshalling) straight from the binary."""
        rva = struct.unpack("<I", self.d[self.dd:self.dd + 4])[0]
        if not rva:
            return {}
        eo = self._o2(rva)
        nnam = struct.unpack("<I", self.d[eo + 24:eo + 28])[0]
        afun = struct.unpack("<I", self.d[eo + 28:eo + 32])[0]
        anam = struct.unpack("<I", self.d[eo + 32:eo + 36])[0]
        aord = struct.unpack("<I", self.d[eo + 36:eo + 40])[0]
        fo, no, oo = self._o2(afun), self._o2(anam), self._o2(aord)
        out = {}
        for i in range(nnam):
            nr = struct.unpack("<I", self.d[no + i * 4:no + i * 4 + 4])[0]
            od = struct.unpack("<H", self.d[oo + i * 2:oo + i * 2 + 2])[0]
            frva = struct.unpack("<I", self.d[fo + od * 4:fo + od * 4 + 4])[0]
            nm = self.d[self._o2(nr):].split(b"\x00")[0].decode("latin1")
            out[nm] = frva
        return out

    def read_at(self, rva, n):
        """Raw bytes at an RVA (for reading a stub's code)."""
        o = self._o2(rva)
        return self.d[o:o + n] if o is not None else b""


class NE:
    """Minimal NE reader: segment table (code/data + sizes), entry/resident names."""
    def __init__(self, path):
        self.path = path
        self.d = d = open(path, "rb").read()
        self.ok = len(d) > 0x40 and d[:2] == b"MZ"
        self.e = struct.unpack("<I", d[0x3c:0x40])[0] if self.ok else 0
        self.is_ne = self.ok and d[self.e:self.e + 2] == b"NE"
        if not self.is_ne:
            return
        e = self.e
        self.nseg = struct.unpack("<H", d[e + 0x1c:e + 0x1e])[0]
        self.align = struct.unpack("<H", d[e + 0x32:e + 0x34])[0] or 9
        segtbl = e + struct.unpack("<H", d[e + 0x22:e + 0x24])[0]
        self.segs = []
        for i in range(self.nseg):
            o = segtbl + i * 8
            off, sz, flags, mina = struct.unpack("<HHHH", d[o:o + 8])
            self.segs.append(("DATA" if flags & 1 else "CODE", off << self.align, sz, flags))

    def resident_names(self):
        e = self.e
        rt = e + struct.unpack("<H", self.d[e + 0x26:e + 0x28])[0]
        out = []
        p = rt
        while True:
            ln = self.d[p]
            if ln == 0:
                break
            nm = self.d[p + 1:p + 1 + ln].decode("latin1")
            ordv = struct.unpack("<H", self.d[p + 1 + ln:p + 1 + ln + 2])[0]
            out.append((ordv, nm))
            p += 1 + ln + 2
        return out

    def export_ordinals(self):
        """The ordinals the NE ENTRY TABLE actually exports (by-name AND by-ordinal), skipping gaps.
        Ordinals are numbered sequentially from 1; a bundle of type 0 is a run of UNUSED ordinals.
        Needed to verify only real exports -- LZ-style APIs export by ordinal with no resident name,
        so resident_names() alone misses them."""
        d, e = self.d, self.e
        et = e + struct.unpack("<H", d[e + 0x04:e + 0x06])[0]
        out, ordn, p = [], 1, et
        try:
            while p < len(d):
                cnt = d[p]
                if cnt == 0:                       # terminating bundle
                    break
                typ = d[p + 1]
                p += 2
                if typ == 0x00:                    # unused bundle -> `cnt` gaps in the ordinal space
                    ordn += cnt
                    continue
                step = 6 if typ == 0xFF else 3     # 0xFF moveable (6 bytes), else fixed-in-seg (3)
                for _ in range(cnt):
                    out.append(ordn); ordn += 1; p += step
        except IndexError:
            pass
        return out


def _omf_publics(data):
    """Exact public symbol names from an OMF object's PUBDEF records (0x90/0x91). Length-prefixed
    names, so no greedy-match errors -- the authoritative `_name@N` decorations wcc386 emitted."""
    out, i, n = set(), 0, len(data)

    def idx(p):                                          # OMF index: 1 byte, or 2 if high bit set
        b = data[p]
        return (((b & 0x7f) << 8) | data[p + 1], p + 2) if (b & 0x80) else (b, p + 1)

    while i + 3 <= n:
        rectype = data[i]
        length = data[i + 1] | (data[i + 2] << 8)
        body, end = i + 3, i + 3 + length
        if end > n:
            break
        if rectype in (0x90, 0x91):
            p = body
            _grp, p = idx(p)
            seg, p = idx(p)
            if seg == 0:
                p += 2                                   # base frame present when segment index == 0
            offsz = 2 if rectype == 0x90 else 4
            while p < end - 1:                           # end-1 is the record checksum
                ln = data[p]
                p += 1
                if p + ln > end:
                    break
                out.add(data[p:p + ln].decode("latin1"))
                p += ln + offsz
                _t, p = idx(p)
        i = end
    return out


# ------------------------------------------------------------------ RECOVER from a decompile (logic modules)
