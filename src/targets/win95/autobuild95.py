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
import os
import sys
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


def load(path):
    pe = PE(path)
    if pe.is_pe:
        return pe
    ne = NE(path)
    if ne.is_ne:
        return ne
    return None


# ------------------------------------------------------------------ classifier
_THUNK_MARKERS = ("SMapLS", "SUnMapLS", "ThunkConnect32", "ThunkInitLSF", "FT_Prolog", "QT_Thunk")


def classify(path):
    """Return {format, subtype, recipe, why} describing how the module was built."""
    b = load(path)
    if b is None:
        return {"format": "?", "subtype": "unknown", "recipe": None, "why": "not MZ/PE/NE"}
    if isinstance(b, NE):
        return {"format": "NE", "subtype": "win16-c",
                "recipe": "msc6-dosbox", "why": "16-bit NE; %d segments" % b.nseg}
    # PE: flat-thunk veneer vs 32-bit C logic
    imps = b.imports()
    k32 = imps.get("KERNEL32.dll") or imps.get("KERNEL32.DLL") or []
    has_thunk = any(any(m in n for m in _THUNK_MARKERS) for n in k32)
    exp = b.exports()
    textsz = next((vs for nm, va, vs, rs, pr in b.secs if nm == ".text"), 0)
    if has_thunk:
        return {"format": "PE", "subtype": "flat-thunk",
                "recipe": "thunk-ml-cl-link", "n_exports": len(exp), "n_imports": sum(len(v) for v in imps.values()),
                "why": "imports flat-thunk helpers (%s)" % ",".join(
                    sorted({m for n in k32 for m in _THUNK_MARKERS if m in n}))}
    return {"format": "PE", "subtype": "win32-c", "recipe": "msvc-native",
            "n_exports": len(exp), "why": "32-bit PE, no flat-thunk markers; .text=%d B" % textsz}


# ------------------------------------------------------------------ differ
def bindiff(built, shipped):
    """Structural diff of two binaries -> report dict + a verification verdict."""
    bo, sh = load(built), load(shipped)
    if bo is None or sh is None:
        return {"error": "could not parse one of the inputs"}
    r = {"built": os.path.basename(built), "shipped": os.path.basename(shipped),
         "size_built": len(bo.d), "size_shipped": len(sh.d)}
    if isinstance(bo, PE) and isinstance(sh, PE):
        r["kind"] = "PE"
        r["entry_match"] = (bo.entry == sh.entry)
        r["entry"] = (hex(bo.entry), hex(sh.entry))
        r["sections_built"] = [s[0] for s in bo.secs]
        r["sections_shipped"] = [s[0] for s in sh.secs]
        r["sections_match"] = (r["sections_built"] == r["sections_shipped"])
        eb, es = dict(bo.exports()), dict(sh.exports())
        r["exports_match"] = (sorted(bo.exports()) == sorted(sh.exports()))
        r["n_exports"] = (len(eb), len(es))
        ib = {d: set(v) for d, v in bo.imports().items()}
        ii = {d: set(v) for d, v in sh.imports().items()}
        r["import_dlls_built"] = sorted(ib); r["import_dlls_shipped"] = sorted(ii)
        allsyms_b = {"%s!%s" % (d, n) for d, v in ib.items() for n in v}
        allsyms_s = {"%s!%s" % (d, n) for d, v in ii.items() for n in v}
        r["imports_only_in_shipped"] = sorted(allsyms_s - allsyms_b)
        r["imports_only_in_built"] = sorted(allsyms_b - allsyms_s)
        r["n_imports"] = (len(allsyms_b), len(allsyms_s))
    elif isinstance(bo, NE) and isinstance(sh, NE):
        r["kind"] = "NE"
        r["nseg"] = (bo.nseg, sh.nseg)
        r["seg_kinds_built"] = [s[0] for s in bo.segs]
        r["seg_kinds_shipped"] = [s[0] for s in sh.segs]
        r["seg_kinds_match"] = (r["seg_kinds_built"] == r["seg_kinds_shipped"])
        r["exports_match"] = (sorted(bo.resident_names()) == sorted(sh.resident_names()))
    else:
        r["error"] = "format mismatch: built=%s shipped=%s" % (type(bo).__name__, type(sh).__name__)
        return r
    # verdict
    if r.get("kind") == "PE":
        exact = r["entry_match"] and r["sections_match"] and r["exports_match"] and not r["imports_only_in_shipped"] and not r["imports_only_in_built"]
        struct_ok = r["entry_match"] and r["sections_match"] and r["exports_match"]
        r["verdict"] = "byte-surface-identical" if exact else ("structural-parity" if struct_ok else "partial")
    else:
        r["verdict"] = "structural-parity" if (r["seg_kinds_match"] and r["exports_match"]) else "partial"
    return r


def _fmt_diff(r):
    L = []
    L.append("  kind=%s  size built=%d shipped=%d  verdict=%s" %
             (r.get("kind", "?"), r.get("size_built", 0), r.get("size_shipped", 0), r.get("verdict", "?")))
    if r.get("kind") == "PE":
        L.append("  entry %s %s" % (r["entry"], "MATCH" if r["entry_match"] else "differ"))
        L.append("  sections %s%s" % ("MATCH" if r["sections_match"] else "differ",
                 "" if r["sections_match"] else " (%s vs %s)" % (r["sections_built"], r["sections_shipped"])))
        L.append("  exports %s %s" % (r["n_exports"], "MATCH" if r["exports_match"] else "differ"))
        L.append("  imports %s  only-in-shipped=%s  only-in-built=%s" %
                 (r["n_imports"], r["imports_only_in_shipped"], r["imports_only_in_built"]))
    elif r.get("kind") == "NE":
        L.append("  segments %s  kinds %s  exports %s" %
                 (r["nseg"], "MATCH" if r["seg_kinds_match"] else "differ",
                  "MATCH" if r["exports_match"] else "differ"))
    if r.get("error"):
        L.append("  ERROR: " + r["error"])
    return "\n".join(L)


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    import target
    mod = argv[0]
    path = target.resolve(mod) if not os.path.exists(mod) else mod
    c = classify(path)
    print("=== classify %s ===" % os.path.basename(path))
    for k, v in c.items():
        print("  %-12s %s" % (k, v))
    built = _opt(argv, "--diff")
    if built:
        print("=== diff %s vs %s ===" % (os.path.basename(built), os.path.basename(path)))
        print(_fmt_diff(bindiff(built, path)))


if __name__ == "__main__":
    main()
