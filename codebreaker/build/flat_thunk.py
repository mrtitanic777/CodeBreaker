r"""flat_thunk.py -- the authentic-rebuild + binary-verify engine, aimable at any Win95 module.

Automates the workflow proven by hand on LZ32.DLL: CLASSIFY a binary by how Microsoft actually built
it, RECOVER authentic source for that class, BUILD it with the period toolchain, DIFF the result
against the shipped binary, and DOCUMENT the verification level. This file is the FOUNDATION --
the binary model (PE/NE parse), the classifier, and the differ; the per-class recoverers and the
build drivers plug into it.

The guiding standard (design/RECONSTRUCTION_STANDARD.md): authentic-method + binary-verified, not
source-text-exact. The differ reports what is actually verifiable -- entry point, section layout,
exports, imports, size -- never "we guessed the names."

    python -m codebreaker flat-thunk <binary> [--classify] [--diff <built.dll>]
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


# ------------------------------------------------------------------ toolchain resolver
def _first(*cands):
    for c in cands:
        if c and os.path.exists(c):
            return c
    return None


def _reference_roots():
    """The 'reference' folder (Win95 DDK + WIN32SDK + the thunk compiler 1.01): corpus.reference()."""
    from codebreaker.base import corpus
    cands = [corpus.reference()]
    seen, out = set(), []
    for c in cands:
        if c and c not in seen and os.path.isdir(c):
            seen.add(c); out.append(c)
    return out


def toolchain():
    """Locate the period build tools. Flat-thunk path is all native (runs on modern Windows):
    thunk.exe (prefer the v1.01 from the Chicago beta SDK -> closest imports), MASM 6.11c, MSVC 2.0."""
    from codebreaker.base import settings
    refs = _reference_roots()

    def ref_find(*rel):
        return _first(*[os.path.join(r, *rel) for r in refs])

    cl = settings.get("period_compiler") or ""                # the BIN folder of Visual C++ 2.0
    msvc2 = os.path.dirname(cl.rstrip("\\/")) if os.path.basename(cl.rstrip("\\/")).upper() == "BIN" else cl
    win32lib = ref_find("WIN32SDK", "MSTOOLS", "LIB", "I386", "THUNK32.LIB")
    tc = {
        "thunk": ref_find("thunk-1.01", "THUNK.EXE") or ref_find("WIN32SDK", "MSTOOLS", "BIN", "I386", "THUNK.EXE"),
        "ml": ref_find("Microsoft Windows 95 DDK", "MASM611C", "ML.EXE"),
        "cl": os.path.join(msvc2, "BIN", "CL.EXE"),
        "link": os.path.join(msvc2, "BIN", "LINK.EXE"),
        "lib": os.path.join(msvc2, "BIN", "LIB.EXE"),
        "rc": os.path.join(msvc2, "BIN", "RC.EXE"),
        "include": os.path.join(msvc2, "INCLUDE"),
        "lib_dirs": [os.path.join(msvc2, "LIB")] + ([os.path.dirname(win32lib)] if win32lib else []),
    }
    return tc


def _winpath(p):
    return p.replace("/", "\\")


import glob as _glob
import re as _re


def _discover_flat_thunk(srcdir):
    """Find the pieces of a flat-thunk module in srcdir. Returns a dict or {'error':..}.
      thk   = the *.thk thunk script
      cfiles= every *.c (veneer)
      moddef= the module .def (LIBRARY <MODULE>, not the *k32 import-lib def)
      k32def= the *k32.def (KERNEL32 by-ordinal import lib)
      rc    = optional *.rc ; module = LIBRARY name from moddef (lowercased)
    """
    thk = _glob.glob(os.path.join(srcdir, "*.thk"))
    defs = _glob.glob(os.path.join(srcdir, "*.def"))
    k32 = [d for d in defs if os.path.basename(d).lower().endswith("k32.def")]
    moddefs = [d for d in defs if d not in k32]
    cfiles = _glob.glob(os.path.join(srcdir, "*.c"))
    rc = _glob.glob(os.path.join(srcdir, "*.rc"))
    if not thk:
        return {"error": "no .thk in %s" % srcdir}
    if not moddefs:
        return {"error": "no module .def in %s" % srcdir}
    if not k32:
        return {"error": "no *k32.def (KERNEL32 ordinal import lib) in %s" % srcdir}
    if not cfiles:
        return {"error": "no veneer .c in %s" % srcdir}
    m = _re.search(r"^\s*LIBRARY\s+(\w+)", open(moddefs[0], encoding="latin1").read(), _re.M | _re.I)
    module = (m.group(1) if m else os.path.splitext(os.path.basename(moddefs[0]))[0]).lower()
    return {"thk": thk[0], "cfiles": cfiles, "moddef": moddefs[0], "k32def": k32[0],
            "rc": rc[0] if rc else None, "module": module}


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


_BYVAL = {"POINT": 8, "POINTL": 8, "SIZE": 8, "SIZEL": 8, "RECT": 16, "RECTL": 16, "double": 8,
          "__int64": 8, "LARGE_INTEGER": 8, "ULARGE_INTEGER": 8, "DWORDLONG": 8, "LONGLONG": 8}


def _param_bytes(part):
    """Fallback size of one stdcall parameter (used only if the PUBDEF lookup misses): pointers 4,
    known by-value structs/64-bit types 8/16, else the machine word 4."""
    part = part.strip()
    if part in ("", "void", "..."):
        return 0
    if "*" in part:
        return 4
    toks = [t for t in part.split() if t not in ("const", "far", "near", "FAR", "NEAR",
                                                  "unsigned", "signed")]
    return _BYVAL.get(toks[0] if toks else "", 4)


def _ow_paths():
    """(wcc386, wlink, include-dirs) for the from-source OpenWatcom toolchain -- the compiler the
    project builds with (no Microsoft tools). None if the OW tree is not present."""
    from codebreaker.build import watcom as toolchain
    ow = toolchain.openwatcom_root()
    wcc = toolchain.ow_cc("pe")
    wlink = toolchain.ow_tool("wlink")
    incs = [os.path.join(ow, "h", "nt"), os.path.join(ow, "h")]
    if not wcc:
        return None
    return {"root": ow, "wcc386": wcc, "wlink": wlink, "include": incs}


def compile_ow(srcdir):
    """Compile every recovered veneer .c in `srcdir` with OpenWatcom wcc386 against OpenWatcom's own
    Win32 headers -- the from-source toolchain, no Microsoft tools. This is the project's real build
    compiler (owbuild/vmm32 lanes); it verifies the recovered source is compilable as authored.
    Returns {ok, results:[(file, rc, first_error)]}."""
    import subprocess, glob as _g
    ow = _ow_paths()
    if not ow:
        return {"ok": False, "error": "OpenWatcom not built (experiments/premise-proof/open-watcom-v2)"}
    env = dict(os.environ)
    env["WATCOM"] = ow["root"]
    env["INCLUDE"] = ";".join(ow["include"])
    env["PATH"] = os.path.dirname(ow["wcc386"]) + os.pathsep + env.get("PATH", "")
    results = []
    for c in sorted(_g.glob(os.path.join(srcdir, "*.c"))):
        r = subprocess.run([ow["wcc386"], "-bt=nt", "-zq", "-w0", os.path.basename(c)],
                           cwd=srcdir, env=env, capture_output=True, text=True, timeout=180)
        err = next((l for l in (r.stdout + r.stderr).splitlines() if "Error" in l), "")
        results.append((os.path.basename(c), r.returncode, err.strip()[:100]))
    ok = all(rc == 0 for _f, rc, _e in results)
    return {"ok": ok, "results": results}


def build_flat_thunk_ow(srcdir, outdir=None):
    """Build a flat-thunk veneer to a real DLL with the FROM-SOURCE OpenWatcom toolchain -- no
    Microsoft tools. Compiles the recovered veneer .c with wcc386 (auto-exporting each function via
    __declspec(dllexport), so no fragile export-decoration guessing), stubs the thunk-compiler's
    connection routine (<module>_ThunkConnect32 -- the real one is generated by the thunk compiler at
    body curation), resolves KERNEL32/USER32/GDI32 against OpenWatcom's own import libs, and links
    `system nt_dll`. Produces a loadable DLL whose export table matches the retail module's function
    exports. Returns {ok, dll, module, n_exports, log}."""
    import subprocess, glob as _g
    ow = _ow_paths()
    if not ow:
        return {"ok": False, "error": "OpenWatcom not built"}
    srcdir = os.path.abspath(srcdir)
    outdir = os.path.abspath(outdir or os.path.join(srcdir, "_ow"))
    os.makedirs(outdir, exist_ok=True)
    cfiles = _g.glob(os.path.join(srcdir, "*.c"))
    if not cfiles:
        return {"ok": False, "error": "no veneer .c in %s" % srcdir}
    cpath = cfiles[0]
    module = os.path.splitext(os.path.basename(cpath))[0]
    env = dict(os.environ)
    env["WATCOM"] = ow["root"]
    env["INCLUDE"] = ";".join(ow["include"])
    env["PATH"] = os.path.dirname(ow["wcc386"]) + os.pathsep + env.get("PATH", "")
    libdir = os.path.join(ow["root"], "lib386", "nt")

    # copy the veneer verbatim (authentic source; exports come from directives, not __declspec).
    text = open(cpath, encoding="utf-8", errors="replace").read()
    open(os.path.join(outdir, module + ".c"), "w", newline="\n").write(text)
    # the thunk connection routine is emitted by the thunk compiler; stub it for the scaffold link.
    open(os.path.join(outdir, "thkstub.c"), "w", newline="\n").write(
        "#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n"
        "/* scaffold stub for the thunk compiler's connection routine (real one at body curation) */\n"
        "BOOL __stdcall %s_ThunkConnect32(LPSTR a,LPSTR b,HINSTANCE h,DWORD r){return TRUE;}\n" % module)
    log = []
    for c in (module + ".c", "thkstub.c"):
        r = subprocess.run([ow["wcc386"], "-bt=nt", "-zq", "-w0", c], cwd=outdir, env=env,
                           capture_output=True, text=True, timeout=180)
        log.append("$ wcc386 %s -> %d\n%s" % (c, r.returncode, (r.stdout + r.stderr).strip()))
        if r.returncode != 0:
            return {"ok": False, "error": "compile %s failed" % c, "log": "\n".join(log)}

    # export directives: OpenWatcom decorates a public symbol by convention -- stdcall `_name@N`
    # (N = total parameter BYTES), cdecl `_name`. The authoritative N is in the object's PUBDEF
    # records; parse them so by-value structs (POINT=8, RECT=16) get the right N, not a guessed
    # nargs*4. Alias each clean export name to its decorated public symbol.
    obj_bytes = open(os.path.join(outdir, module + ".obj"), "rb").read()
    publics = _omf_publics(obj_bytes)                   # set of exact public symbol strings
    by_clean = {}                                       # clean name -> decorated symbol
    for sym in publics:
        base = sym[1:] if sym.startswith("_") else sym  # drop leading underscore
        base = base.split("@", 1)[0]                    # drop @N
        by_clean.setdefault(base, sym)
    _STDCALL = {"WINAPI", "APIENTRY", "PASCAL", "CALLBACK", "STDAPICALLTYPE", "ACMAPI", "__stdcall"}
    exp_lines, exported = [], []
    for mo in _re.finditer(r"(?m)^([A-Za-z_][\w \*]*?)\s+(\w+)\s+(\w+)\s*\(([^;{]*)\)\s*\{", text):
        conv, name, params = mo.group(2), mo.group(3), mo.group(4)
        if name == "DllMain":
            continue
        internal = by_clean.get(name)                   # exact symbol from PUBDEF (best)
        if not internal:                                # fallback: compute the stdcall decoration
            pl = params.strip()
            nbytes = 0 if pl in ("", "void") else sum(_param_bytes(p) for p in _split_params(pl))
            internal = ("_%s" % name) if (conv.endswith("APIV") or conv in ("__cdecl", "cdecl")) \
                else ("_%s@%d" % (name, nbytes))
        exp_lines.append("export %s='%s'" % (name, internal))
        exported.append(name)
    lnk = os.path.join(outdir, "link.lnk")
    with open(lnk, "w", newline="\n") as f:
        f.write("system nt_dll\nname %s.dll\noption quiet\n" % module)
        f.write("libpath %s\nlibrary kernel32,user32,gdi32\n" % libdir.replace("\\", "/"))
        f.write("file %s.obj\nfile thkstub.obj\n" % module)
        for l in exp_lines:
            f.write(l + "\n")
    r = subprocess.run([ow["wlink"], "@link.lnk"], cwd=outdir, env=env, capture_output=True, text=True)
    log.append("$ wlink -> %d\n%s" % (r.returncode, (r.stdout + r.stderr).strip()))
    dll = os.path.join(outdir, module + ".dll")
    if os.path.exists(dll):
        try:
            nexp = len(PE(dll).exports())
        except Exception:
            nexp = 0
        return {"ok": True, "dll": dll, "module": module, "n_exports": nexp,
                "bytes": os.path.getsize(dll), "log": "\n".join(log)}
    return {"ok": False, "error": "link produced no DLL", "log": "\n".join(log)}


def build_flat_thunk(srcdir, outdir=None):
    """Build a flat-thunk module from its recovered source (native chain, no DOSBox):
      thunk.exe -> .asm ; ml -DIS_32 -> thunk obj ; cl each veneer .c -> obj ; rc (opt) ;
      lib (KERNEL32 by-ordinal import lib) ; link /NODEFAULTLIB /ENTRY:DllMain@12 -> <module>.dll.
    Discovers the pieces in srcdir. Returns {ok, dll, module, log}."""
    import subprocess, shutil
    srcdir = os.path.abspath(srcdir)
    outdir = os.path.abspath(outdir or os.path.join(srcdir, "_build"))
    os.makedirs(outdir, exist_ok=True)
    tc = toolchain()
    for k in ("thunk", "ml", "cl", "link", "lib"):
        if not tc[k] or not os.path.exists(tc[k]):
            return {"ok": False, "log": "missing tool: %s (%s)" % (k, tc[k])}
    disc = _discover_flat_thunk(srcdir)
    if disc.get("error"):
        return {"ok": False, "log": disc["error"]}
    module = disc["module"]
    # stage sources into outdir (keep the tracked tree clean)
    for f in os.listdir(srcdir):
        if f.rsplit(".", 1)[-1].lower() in ("thk", "c", "h", "def", "rc", "ico", "res"):
            shutil.copy2(os.path.join(srcdir, f), outdir)
    thk = os.path.basename(disc["thk"]); thkstem = os.path.splitext(thk)[0]
    cobjs = [os.path.splitext(os.path.basename(c))[0] + ".obj" for c in disc["cfiles"]]
    k32 = os.path.basename(disc["k32def"]); k32stem = os.path.splitext(k32)[0]
    moddef = os.path.basename(disc["moddef"])
    rc = os.path.basename(disc["rc"]) if disc["rc"] else None
    res = (module + ".res") if rc else ""
    # INCLUDE: the compiler's own headers PLUS the Win32 SDK headers (msacm.h/vfw.h/commctrl.h...),
    # so a veneer's real SDK-typed signatures resolve -- the authentic build config.
    win32inc = None
    for r in _reference_roots():
        p = os.path.join(r, "WIN32SDK", "MSTOOLS", "INCLUDE")
        if os.path.isdir(p):
            win32inc = p
            break
    incdirs = ([win32inc] if win32inc else []) + [tc["include"]]
    L = ["@echo off", "cd /d " + _winpath(outdir),
         "set INCLUDE=" + ";".join(_winpath(x) for x in incdirs),
         "set LIB=" + ";".join(_winpath(x) for x in tc["lib_dirs"]),
         "set PATH=" + _winpath(os.path.dirname(tc["cl"])) + ";%PATH%",
         "del %s.dll *.obj %s.asm %s 2>nul" % (module, thkstem, res),
         '"%s" -o %s.asm %s > _t.log 2>&1' % (_winpath(tc["thunk"]), thkstem, thk),
         '"%s" /c /W0 /DIS_32 /Fo%s_thk.obj %s.asm >> _t.log 2>&1' % (_winpath(tc["ml"]), thkstem, thkstem)]
    for c in disc["cfiles"]:
        # /DWIN32 /D_WIN32: the SDK headers gate their 32-bit types on _WIN32 (msacm.h:288 etc.)
        L.append('"%s" /c /W3 /DWIN32 /D_WIN32 %s >> _t.log 2>&1' % (_winpath(tc["cl"]), os.path.basename(c)))
    if rc:
        L.append('"%s" /r /fo %s %s >> _t.log 2>&1' % (_winpath(tc["rc"]), res, rc))
    L += ['"%s" /NOLOGO /DEF:%s /OUT:%s.lib /MACHINE:IX86 >> _t.log 2>&1'
          % (_winpath(tc["lib"]), k32, k32stem),
          '"%s" /DLL /NOLOGO /NODEFAULTLIB /ENTRY:DllMain@12 /DEF:%s /OUT:%s.dll %s %s_thk.obj %s %s.lib kernel32.lib thunk32.lib >> _t.log 2>&1'
          % (_winpath(tc["link"]), moddef, module, " ".join(cobjs), thkstem, res, k32stem),
          "echo RC=%errorlevel% >> _t.log"]
    bat = os.path.join(outdir, "_build.bat")
    open(bat, "w").write("\r\n".join(L) + "\r\n")
    try:
        subprocess.run(["cmd", "/c", _winpath(bat)], capture_output=True, text=True, timeout=300)
    except Exception as e:
        return {"ok": False, "log": "build run failed: %r" % e}
    tlog = os.path.join(outdir, "_t.log")
    log = open(tlog, encoding="latin1").read() if os.path.exists(tlog) else ""
    dll = os.path.join(outdir, module + ".dll")
    return {"ok": os.path.exists(dll), "dll": dll if os.path.exists(dll) else None,
            "module": module, "log": log}


# ------------------------------------------------------------------ RECOVER (flat-thunk)
# KERNEL32 by-ordinal flat-thunk / handle helpers (names from the Windows Me symbol file; Geoff
# Chappell, "Ordinal-Only Exports In KERNEL32 4.0"). Value = (name, stdcall-arg-bytes or None).
_KERNEL32_ORDINALS = {
    17: ("CommonUnimpStub", 0), 20: ("DosFileHandleToWin32Handle", 4),
    21: ("Win32HandleToDosFileHandle", 4), 22: ("DisposeLZ32Handle", 4),
    41: ("ThunkInitLSF", None), 89: ("FT_PrologPrime", None), 90: ("QT_ThunkPrime", None),
}


# ------------------------------------------------------------------ stub analysis (x86-32)
# A flat-thunk export is a tiny stdcall stub: set up args, marshal 32:16 pointers via SMapLS,
# call the 16-bit side through QT_Thunk/FT_Prolog, unmap, and `ret N`. From the stub bytes alone
# we can recover the REAL signature the compiler enforced -- the `ret N` epilogue pops exactly the
# argument bytes (stdcall), so N/4 = the DWORD-sized argument count; and each SMapLS call marshals
# one 32-bit pointer argument down to a 16:16 far pointer, so counting them bounds the pointer args.
#
# A conservative, table-driven length decoder walks the stub; on an opcode it does not model it
# stops (returns 0) rather than mis-measure -- so arity is recovered where the walk is certain and
# the module cleanly falls back to the (void) scaffold where it is not. Never a WRONG signature.
_ARITH_RM = set(range(0x00, 0x04)) | set(range(0x08, 0x0C)) | set(range(0x10, 0x14)) | \
    set(range(0x18, 0x1C)) | set(range(0x20, 0x24)) | set(range(0x28, 0x2C)) | \
    set(range(0x30, 0x34)) | set(range(0x38, 0x3C))                       # add..cmp r/m,r & r,r/m
_ARITH_AX = {0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D}              # op eax,imm
_MODRM_ONLY = _ARITH_RM | {0x62, 0x63, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B,
                           0x8C, 0x8D, 0x8E, 0x8F, 0xD0, 0xD1, 0xD2, 0xD3, 0xFE,
                           0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF}   # x87 FPU (ModRM form)
_MODRM_IMM8 = {0x6B, 0x80, 0x82, 0x83, 0xC0, 0xC1, 0xC6}
_MODRM_IMMW = {0x69, 0x81, 0xC7}


def _modrm_len(d, i):
    """Bytes in the ModRM (+SIB +disp) cluster starting at d[i]."""
    if i >= len(d):
        return 0
    modrm = d[i]
    mod, rm = modrm >> 6, modrm & 7
    n = 1
    if mod == 3:
        return n
    if rm == 4:                                              # SIB
        n += 1
        base = d[i + 1] & 7 if i + 1 < len(d) else 0
        if mod == 0 and base == 5:
            n += 4
    if mod == 1:
        n += 1
    elif mod == 2:
        n += 4
    elif mod == 0 and rm == 5:
        n += 4
    return n


def _ilen(d, i):
    """Length of the x86-32 instruction at d[i]; 0 if the opcode is not modelled (caller stops)."""
    start = i
    opsize = False
    while i < len(d) and d[i] in (0xF0, 0xF2, 0xF3, 0x2E, 0x36, 0x3E, 0x26, 0x64, 0x65, 0x66, 0x67):
        if d[i] == 0x66:
            opsize = True
        i += 1
    if i >= len(d):
        return 0
    immw = 2 if opsize else 4
    op = d[i]
    i += 1
    if op == 0x0F:                                           # two-byte
        if i >= len(d):
            return 0
        op2 = d[i]
        i += 1
        if 0x80 <= op2 <= 0x8F:                              # jcc rel32
            return i - start + immw
        if op2 in (0x1F,) or 0x90 <= op2 <= 0x9F or op2 in (0xA3, 0xA5, 0xAB, 0xAD, 0xAF,
                                                            0xB6, 0xB7, 0xBE, 0xBF, 0xB0, 0xB1) \
                or 0x40 <= op2 <= 0x4F:                       # setcc/movzx/movsx/imul/cmov/bt/nop r/m
            return i - start + _modrm_len(d, i)
        if op2 in (0xA2, 0x31, 0x77, 0x05, 0x34, 0x0B):       # cpuid/rdtsc/emms/sysenter/ud2 -- no operand
            return i - start
        return 0
    if op in _MODRM_ONLY:
        return i - start + _modrm_len(d, i)
    if op in _MODRM_IMM8:
        return i - start + _modrm_len(d, i) + 1
    if op in _MODRM_IMMW:
        return i - start + _modrm_len(d, i) + immw
    if op in (0xF6, 0xF7):                                    # grp3: test(/0,/1) has imm; rest don't
        ml = _modrm_len(d, i)
        reg = (d[i] >> 3) & 7 if i < len(d) else 7
        return i - start + ml + ((1 if op == 0xF6 else immw) if reg in (0, 1) else 0)
    if op in (0xFF,):                                         # grp5 inc/dec/call/jmp/push r/m
        return i - start + _modrm_len(d, i)
    if 0x50 <= op <= 0x5F or 0x90 <= op <= 0x9D or 0x40 <= op <= 0x4F \
            or op in (0xC3, 0xC9, 0xCB, 0xCC, 0xF4, 0xF5, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0x98, 0x99):
        return i - start                                     # push/pop/inc/dec/nop/ret/leave/flags/one-byte
    if 0xB8 <= op <= 0xBF:                                    # mov reg,imm32
        return i - start + immw
    if 0xB0 <= op <= 0xB7:                                    # mov reg8,imm8
        return i - start + 1
    if op in (0x68,):                                         # push imm32
        return i - start + immw
    if op in (0x6A, 0xEB) or 0x70 <= op <= 0x7F or op in (0xA8, 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C):
        return i - start + 1                                 # push imm8 / jmp rel8 / jcc rel8 / test al / op al,imm8
    if op in (0xE8, 0xE9):                                    # call/jmp rel32
        return i - start + immw
    if op in (0xA9,) or op in _ARITH_AX:                     # test eax,imm / op eax,imm
        return i - start + immw
    if op in (0xA0, 0xA1, 0xA2, 0xA3):                       # mov moffs (address-sized)
        return i - start + 4
    if op == 0xC2:                                           # ret imm16
        return i - start + 2
    return 0


def _imagebase(pe):
    return struct.unpack("<I", pe.d[pe.opt + 28:pe.opt + 32])[0]


def _smapls_iat_rvas(pe):
    """Absolute VAs that, when the target of a stub's call, mark a 32:16 pointer marshalling:
    the IAT slots of KERNEL32!SMapLS/SUnMapLS, plus any `jmp [slot]` incremental-link thunks that
    forward to them. Used to count pointer arguments in a stub."""
    base = _imagebase(pe)
    d = pe.d
    rva = struct.unpack("<I", d[pe.dd + 8:pe.dd + 12])[0]
    if not rva:
        return set()
    io = pe._o2(rva)
    slot_vas = set()
    while io is not None:
        oft, ts, fw, nr, ft = struct.unpack("<IIIII", d[io:io + 20])
        io += 20
        if nr == 0:
            break
        dll = d[pe._o2(nr):].split(b"\x00")[0].decode("latin1").upper()
        t = pe._o2(oft or ft)
        slot_rva = ft
        while t is not None:
            v = struct.unpack("<I", d[t:t + 4])[0]
            if v == 0:
                break
            if "KERNEL32" in dll and not (v & 0x80000000):
                nm = d[pe._o2(v) + 2:].split(b"\x00")[0].decode("latin1")
                if nm.startswith("SMapLS") or nm.startswith("SUnMapLS"):
                    slot_vas.add((base + slot_rva) & 0xFFFFFFFF)
            t += 4
            slot_rva += 4
    trig = set(slot_vas)
    # scan for `jmp dword ptr [slot]` (FF 25 abs32) thunks that forward to those IAT slots
    for m in _re.finditer(rb"\xff\x25", d):
        o = m.start()
        try:
            operand = struct.unpack("<I", d[o + 2:o + 6])[0]
        except struct.error:
            continue
        if operand in slot_vas:
            thunk_rva = pe._file2rva(o) if hasattr(pe, "_file2rva") else None
            if thunk_rva is not None:
                trig.add((base + thunk_rva) & 0xFFFFFFFF)
    return trig


def _stub_signature(pe, frva, maxlen=8192):
    """Recover (argbytes, nptr) for an export at RVA `frva`, or (None, 0) if it does not cleanly
    resolve. argbytes = stdcall bytes popped by `ret N` (every stdcall return in a function pops the
    same N, so the first one reached on the linearly-decoded path is authoritative); nptr = SMapLS
    marshalling calls seen (32-bit pointer args). `maxlen` bounds the walk to the function's extent.
    A `ret N` is only accepted when N is a plausible stdcall count (N%4==0, N<=256) so a decode
    desync at inline data cannot fabricate a signature -- unresolved beats wrong."""
    d = pe.read_at(frva, maxlen)
    if not d:
        return None, 0
    trig = getattr(pe, "_smapls_rvas", None) or set()
    base = _imagebase(pe)
    i, nptr = 0, 0
    while i < len(d):
        b = d[i]
        if b == 0xC2:                                        # ret imm16 -> stdcall epilogue
            n = struct.unpack("<H", d[i + 1:i + 3])[0]
            return (n, nptr) if (n % 4 == 0 and n <= 256) else (None, nptr)
        if b == 0xC3:                                        # ret -> no stack cleanup (0 args, or cdecl)
            return 0, nptr
        if b == 0xCC:                                        # int3 padding before a ret -> unknown
            return None, nptr
        if b == 0xE8 and i + 5 <= len(d):                    # call rel32 -> SMapLS jump-thunk?
            tgt = (base + frva + i + 5 + struct.unpack("<i", d[i + 1:i + 5])[0]) & 0xFFFFFFFF
            if tgt in trig:
                nptr += 1
        elif b == 0xFF and i + 6 <= len(d) and d[i + 1] == 0x15:   # call dword ptr [abs] -> IAT SMapLS?
            if struct.unpack("<I", d[i + 2:i + 6])[0] in trig:
                nptr += 1
        ln = _ilen(d, i)
        if ln <= 0:                                          # opcode not modelled (e.g. inline jump
            break                                            # table) -> try the epilogue mode-scan
        i += ln
    # Fallback: a function with an inline switch jump table defeats linear sweep. Its stdcall
    # epilogue `ret N` still appears at every return site with the SAME N, so pattern-scan for
    # `C2 NN 00` (N a plausible stdcall count) and accept N only if it repeats -- a lone data
    # coincidence will not, but a real multi-return epilogue does.
    n = _argbytes_by_epilogue(d)
    return (n, nptr) if n is not None else (None, nptr)


def _argbytes_by_epilogue(d):
    """Plausible `ret N` immediate from a function's bytes (N%4==0, N<=256) without decoding -- robust
    to a decode desync at an inline jump table. Accept a value if it either repeats (>=2 return sites,
    the usual multi-return case) or occurs once immediately after an epilogue terminator (pop reg /
    leave -- a single-return function); an ambiguous single-occurrence set is left unresolved."""
    from collections import Counter
    _TERM = set(range(0x58, 0x60)) | {0xC9}          # pop eax..edi / leave
    c, termd = Counter(), {}
    for i in range(len(d) - 2):
        if d[i] == 0xC2 and d[i + 2] == 0x00:
            n = d[i + 1]
            if n % 4 == 0 and 0 < n <= 256:
                c[n] += 1
                if i > 0 and d[i - 1] in _TERM:
                    termd[n] = termd.get(n, 0) + 1
    if not c:
        return None
    n, cnt = c.most_common(1)[0]
    if cnt >= 2:
        return n
    if len(termd) == 1:                              # exactly one terminator-preceded candidate
        return next(iter(termd))
    return None


def _thunk_dll_targets(pe, module):
    """The 16-bit target DLL name for a flat-thunk module, read from the ThunkConnect32 string
    args in the image ("<x>.DLL","<module>.DLL"). Returns (target16, candidates)."""
    d = pe.d
    up = module.upper()
    cands, seen = [], set()
    for m in _re.finditer(rb"[A-Za-z_][A-Za-z0-9_]{1,10}\.[Dd][Ll][Ll]", d):
        nm = m.group(0).decode("latin1").upper()
        if nm in seen:
            continue
        seen.add(nm)
        cands.append(nm)
    # a flat-thunk's 16-bit partner is almost always the 32-bit name with a trailing "32" dropped
    # (AVICAP32->AVICAP, MSACM32->MSACM); a few well-known ones rename (COMCTL32->COMMCTRL,
    # MSVFW32->MSVIDEO). Take the partner name only if it is actually present in the image --
    # authoritative -- else fall back to the lone non-system .DLL string; else leave it ambiguous.
    _ALIAS = {"COMCTL32": "COMMCTRL.DLL", "MSVFW32": "MSVIDEO.DLL", "WINMM": "MMSYSTEM.DLL",
              "SHELL32": "SHELL.DLL", "COMDLG32": "COMMDLG.DLL", "MPR": "PMSPL.DLL",
              "VERSION": "VER.DLL", "WSOCK32": "WINSOCK.DLL", "OLE32": "OLE2.DLL"}
    candidates_16 = []
    if up in _ALIAS:
        candidates_16.append(_ALIAS[up])
    candidates_16.append((up[:-2] if up.endswith("32") else up) + ".DLL")
    for c16 in candidates_16:
        if c16 in cands:
            return c16, cands
    sysdlls = {"KERNEL32.DLL", "USER32.DLL", "GDI32.DLL", "ADVAPI32.DLL", "WINMM.DLL",
               "SHELL32.DLL", "COMDLG32.DLL", "COMCTL32.DLL", "OLE32.DLL", "OLEAUT32.DLL",
               "RPCRT4.DLL", "VERSION.DLL", "LZ32.DLL", "MPR.DLL", "WSOCK32.DLL", "IMM32.DLL",
               "WINSPOOL.DRV", "DDEML.DLL", "MSVCRT.DLL", "MSVCRT20.DLL", "CRTDLL.DLL",
               "WOW32.DLL", "DCIMAN32.DLL", "MMSYSTEM.DLL", up + ".DLL"}
    others = [c for c in cands if c not in sysdlls]
    target = others[0] if len(others) == 1 else None
    return target, cands


def _load_sdk_protos(win16=False):
    """Per-export SDK prototypes keyed by export name -> {"ret", "params", "conv", "header", "doc"}:
    the 32-bit or the 16-bit SDK headers' prototypes (symbols/prototypes.py), so a 16-bit export
    gets its real 16-bit signature (FAR pointers, UINT/WORD) rather than the 32-bit one. Absent,
    callers fall back to recovered arity -- never a fabricated prototype."""
    from codebreaker.symbols import prototypes
    out = {}
    for name, p in prototypes.table()["16" if win16 else "32"].items():
        params = ", ".join((t + " " + n).strip() for t, n in p["parameters"]) or "void"
        out[name] = {"ret": p["returns"], "params": params, "conv": p.get("marker") or "WINAPI",
                     "header": (p.get("header") or "").upper(), "doc": "documented"}
    return out


def _sig_params(sig, proto):
    """Parameter list text for an export. Real SDK prototype wins; else the stdcall arity recovered
    from `ret N` (each arg a machine word, the first `nptr` flagged as marshalled pointers); None if
    the stub did not resolve (keeps the honest (void)+TODO scaffold)."""
    if proto and proto.get("params") is not None:
        return proto["params"], False
    argbytes, nptr = sig
    if argbytes is None:
        return None, False
    nargs = argbytes // 4
    if nargs == 0:
        return "void", (nptr == 0)
    ps = []
    for i in range(nargs):
        ps.append(("LPVOID p%d" % (i + 1)) if i < nptr else ("DWORD a%d" % (i + 1)))
    return ", ".join(ps), True


def _sig_ret(proto):
    return proto["ret"] if (proto and proto.get("ret")) else "DWORD"


def _sig_conv(proto):
    """The function's calling-convention keyword. From the SDK prototype when known (WINAPI, or the
    varargs VFWAPIV for ICCompress-style APIs) -- so the definition's convention matches the header
    and does not clash (C2373); else WINAPI (flat thunks are stdcall)."""
    c = (proto or {}).get("conv")
    return c if c else "WINAPI"


# SDK headers whose contents <windows.h> already pulls in -- an explicit #include would be redundant.
_IN_WINDOWS_H = {"WINUSER.H", "WINGDI.H", "WINBASE.H", "WINREG.H", "WINNLS.H", "WINNT.H",
                 "WINDOWS.H", "WINDEF.H", "WINCON.H", "WINSVC.H", "WINERROR.H"}


# some SDK headers only compile if a prerequisite is included first (the original source did so):
# msacm.h/vfw.h use the multimedia types from mmsystem.h; mciavi.h builds on both.
_HEADER_PREREQ = {"msacm.h": ["mmsystem.h", "mmreg.h"], "vfw.h": ["mmsystem.h", "commdlg.h"],
                  "digitalv.h": ["mmsystem.h"], "mmreg.h": ["mmsystem.h"],
                  "mciavi.h": ["mmsystem.h", "vfw.h"], "prsht.h": ["commctrl.h"],
                  "commctrl.h": ["objbase.h"]}       # IStream in ImageList_Read/Write, prop sheets

_CKW = {"void", "char", "int", "long", "short", "unsigned", "signed", "double", "float",
        "const", "struct", "union", "enum", "far", "near", "huge", "volatile"}


def _split_params(s):
    """Split a parameter list on top-level commas (respecting nested parens, e.g. a function-pointer
    parameter's own arg list)."""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    return out


def _def_params(params):
    """Ensure every parameter in a list has a NAME, so the text is valid in a function DEFINITION
    (the period C compiler rejects unnamed params in a definition: `void f(HWND,BOOL){}` -> C2055).
    SDK prototypes often omit names (`DragAcceptFiles(HWND, BOOL)`); synthesize aN for the bare ones.
    A parameter is considered named when its last token is a lowercase/mixed-case identifier that is
    not a type keyword and the param does not end in `*` (SDK types are ALL-CAPS or keywords, names
    are camelCase)."""
    p = (params or "").strip()
    if p == "" or p.lower() == "void":
        return "void"
    out = []
    for i, part in enumerate(_split_params(p), 1):
        part = part.strip()
        if part in ("...", ""):
            out.append(part or "void")
            continue
        # the parameter is named iff its trailing identifier (ignoring a glued `*`, e.g. `int*pn`
        # or `UINT FAR *puMxId`) is a mixed/lower-case name -- SDK types are ALL-CAPS or keywords.
        toks = part.split()
        if len(toks) == 1:
            # a single token is always a bare type (HWND, LPCSTR, int, va_list) -- no name present
            is_type = True
        else:
            m = _re.search(r"([A-Za-z_]\w*)\s*$", part)
            last = m.group(1) if m else ""
            # a type token is a keyword or a multi-char ALL-CAPS name (HWND, LPSTR); a single upper
            # letter (X, Y, W, H) is a legitimate parameter name, so require len>=2 for the type test.
            is_type = (not last) or last.lower() in _CKW or (last.isupper() and len(last) >= 2)
        out.append(part if not is_type else (part + " a%d" % i))
    return ", ".join(out)


def _needed_includes(protos_used):
    """The #include lines a recovered module needs so its SDK-typed signatures resolve: the real
    header each used prototype came from (msacm.h/vfw.h/commctrl.h/...), minus the ones <windows.h>
    already provides, with each header's prerequisites emitted first (mmsystem.h before msacm.h) so
    it parses. Emitting the authentic headers is how the original source compiled -- the struct/handle
    types (LPACMFORMATDETAILS, OPENFILENAME, ...) are then real, not fabricated."""
    hdrs = []
    seen = set()

    def add(h):
        hl = h.lower()
        if hl in seen or h.upper() in _IN_WINDOWS_H:
            return
        for pre in _HEADER_PREREQ.get(hl, []):
            add(pre)
        seen.add(hl)
        hdrs.append(hl)

    for p in protos_used:
        h = (p or {}).get("header")
        if h:
            add(h)
    return hdrs


def _is_thunk_data(nm):
    """True for an export that is thunk DATA, not a callable API -- the ThunkData32 tables the thunk
    compiler emits (e.g. VerFThk_ThunkData32, C16ThkSL01). These are real exports (kept in the .def)
    but must not be scaffolded as functions."""
    return (nm.endswith("ThunkData32") or nm.endswith("ThunkData") or "_ThunkData" in nm
            or nm.endswith("ThkSL") or _re.match(r"^[A-Za-z0-9]+Thk(SL)?_?ThunkData", nm) is not None)


# ------------------------------------------------------------------ RECOVER (flat-thunk)
def recover_flat_thunk(pe_path, outdir):
    """Emit recovered flat-thunk source from the binary. The .def (exports) and <mod>k32.def
    (KERNEL32 by-ordinal imports) are recovered VERBATIM from the binary's tables; per-export
    SIGNATURES (stdcall arity + pointer-arg count) and the 16-bit target DLL are recovered from the
    stub code and the ThunkConnect32 strings; bodies remain scaffolds (marshalling flagged for
    curation). Returns a summary dict."""
    pe = PE(pe_path)
    if not pe.is_pe:
        return {"error": "not a PE: %s" % pe_path}
    module = os.path.splitext(os.path.basename(pe_path))[0].lower()
    os.makedirs(outdir, exist_ok=True)
    exps = pe.exports()
    imps = pe.imports()
    k32 = imps.get("KERNEL32.dll") or imps.get("KERNEL32.DLL") or []

    # locate SMapLS in the IAT so a `call SMapLS` in a stub is recognisable (marks a pointer arg).
    addrs = pe.export_addrs()
    pe._smapls_rvas = _smapls_iat_rvas(pe)
    sa = sorted(addrs.values())

    def _extent(rva):
        nxt = [a for a in sa if a > rva]
        return min((nxt[0] - rva) if nxt else 8192, 8192)   # bound the walk to this function

    sigs = {nm: _stub_signature(pe, rva, _extent(rva)) for nm, rva in addrs.items()}
    target16, dll_cands = _thunk_dll_targets(pe, module)

    # 1) module .def -- exports, verbatim
    with open(os.path.join(outdir, module + ".def"), "w") as f:
        f.write("LIBRARY   %s\nEXPORTS\n" % module.upper())
        for ordv, nm in exps:
            f.write("    %-18s @%d\n" % (nm, ordv))

    # 2) <mod>k32.def -- the KERNEL32 by-ordinal helpers this binary imports, named via the oracle
    ords = sorted(int(n[1:]) for n in k32 if n.startswith("@"))
    unknown = [o for o in ords if o not in _KERNEL32_ORDINALS]
    with open(os.path.join(outdir, module + "k32.def"), "w") as f:
        f.write("; KERNEL32 by-ordinal flat-thunk / handle helpers imported by %s.\n" % module.upper())
        f.write("; Names from the Windows Me symbol file (Geoff Chappell). NONAME = by-ordinal import.\n")
        f.write("; NOTE: ThunkInitLSF@41 is the Microsoft-INTERNAL connect (never in a public SDK); we\n")
        f.write(";       connect via the public ThunkConnect32 instead -- see the engine docs.\n")
        f.write("LIBRARY KERNEL32\nEXPORTS\n")
        for o in ords:
            if o == 41:       # internal ThunkInitLSF -- deliberately not imported (use ThunkConnect32)
                continue
            nm, argb = _KERNEL32_ORDINALS.get(o, ("Kernel32_Ord%d" % o, None))
            dec = ("%s@%d" % (nm, argb)) if argb is not None else nm
            f.write("    %-28s @%d NONAME\n" % (dec, o))

    protos = _load_sdk_protos()
    dll16 = target16 or "?16.DLL"          # real 16-bit target when unambiguous, else placeholder
    n_arity, n_ptr, n_proto = 0, 0, 0

    # 3) .thk -- one thunked function per export; arity recovered from the stub's stdcall epilogue,
    #    pointer args (from SMapLS) flagged with input/output/inout markers for the thunk compiler.
    with open(os.path.join(outdir, module + ".thk"), "w") as f:
        f.write("/* %s.thk -- flat-thunk script. Function list from the export table; per-function\n"
                " * ARITY recovered from the stub's `ret N` (stdcall) and pointer args from the SMapLS\n"
                " * marshalling. Pointer input/output DIRECTION is defaulted to input -- curate where a\n"
                " * function writes back through a pointer (mark output/inout). 16-bit target: %s. */\n"
                "enablemapdirect3216 = true;\n\n"
                "typedef unsigned short WORD;  typedef unsigned long DWORD;  typedef int INT;\n"
                "typedef long LONG;  typedef char *LPSTR;  typedef void *LPVOID;\n"
                "typedef void VOID;  typedef int HFILE;\n\n" % (module, dll16))
        # 16-bit Windows has no wide APIs: A/W exports share ONE 16-bit target (the ANSI form), so
        # emit each base once, preferring the A variant's (LPSTR) prototype. Thunk-data exports are
        # not functions -- note them and skip.
        by_base = {}
        for ordv, nm in exps:
            if _is_thunk_data(nm):
                continue
            b = nm[:-1] if nm.endswith(("A", "W")) else nm
            # prefer an A export (or a suffix-less one) as the canonical 16-bit target
            if b not in by_base or (nm.endswith("A") and not by_base[b][1].endswith("A")):
                by_base[b] = (ordv, nm)
        td = [nm for _o, nm in exps if _is_thunk_data(nm)]
        if td:
            f.write("/* thunk-data exports (emitted by the thunk compiler, not APIs): %s */\n\n"
                    % ", ".join(td))
        for b, (ordv, nm) in sorted(by_base.items(), key=lambda kv: kv[1][0]):
            proto = protos.get(nm) or protos.get(b + "A") or protos.get(b)
            params, known = _sig_params(sigs.get(nm, (None, 0)), proto)
            ret = _sig_ret(proto)
            if params is None:
                f.write("/* @%d %s -- stub did not resolve; TODO: signature + pointer directions */\n"
                        "%s %s16(void)\n{\n}\n\n" % (ordv, nm, ret, b))
                continue
            f.write("/* @%d %s -- arity recovered from stub%s */\n"
                    "%s %s16(%s)\n{\n}\n\n" % (ordv, nm,
                    " (SDK prototype)" if proto else "", ret, b, params))

    # 4) veneer .c -- DllMain (real 16-bit target) + a thunk-through per export with its recovered
    #    signature. Bodies remain scaffolds (the marshal/thunk/return is filled during curation).
    inc = _needed_includes([protos.get(nm) for _o, nm in exps if protos.get(nm)])
    inc_block = "".join("#include <%s>\n" % h for h in inc)
    with open(os.path.join(outdir, module + ".c"), "w") as f:
        f.write("/* %s.c -- 32-bit flat-thunk veneer. DllMain + per-export thunk-through.\n"
                " * SIGNATURES are recovered from each stub (stdcall arity via `ret N`; pointer args\n"
                " * via SMapLS). Per-function tags: documented/undocumented = published API; behavioral\n"
                " * = body reconstructed from observed behaviour (not byte-exact); scaffold = body not\n"
                " * yet implemented. Curation fills the marshalling and confirms doc/undoc + accuracy. */\n"
                "#define WIN32_LEAN_AND_MEAN\n#include <windows.h>\n%s\n"
                "extern BOOL __stdcall %s_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);\n\n"
                "/* DLL entry -- connects the 32:16 thunk to the 16-bit target %s */\n"
                "BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)\n{\n"
                "    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h);\n"
                "        if (!%s_ThunkConnect32(\"%s\", \"%s.DLL\", h, r)) return FALSE; }\n"
                "    else if (r == DLL_PROCESS_DETACH) %s_ThunkConnect32(\"%s\", \"%s.DLL\", h, r);\n"
                "    return TRUE;\n}\n\n" % (module, inc_block, module, dll16, module, dll16,
                                            module.upper(), module, dll16, module.upper()))
        for ordv, nm in exps:
            if _is_thunk_data(nm):                   # data export, emitted by the thunk compiler
                f.write("/* @%d %s -- thunk-data table (generated by the thunk compiler from %s.thk;\n"
                        " * exported by name in the .def, no C definition here) */\n\n"
                        % (ordv, nm, module))
                continue
            if not _re.match(r"^[A-Za-z_]\w*$", nm):   # decorated/non-identifier export (e.g. _f@4)
                f.write("/* @%d %s -- decorated/non-identifier export name; kept in the .def, no C\n"
                        " * definition here (the name is not a legal C identifier) */\n\n" % (ordv, nm))
                continue
            # #undef the name before defining it: many SDK names are macros in the headers -- A/W
            # aliases (`#define PlaySound PlaySoundA`) and function-like macros (`ICCompress` ->
            # ICSendMessage in vfw.h) -- which would corrupt our real definition. A no-op when the
            # name is not a macro.
            undef = "#undef %s\n" % nm
            proto = protos.get(nm)
            sig = sigs.get(nm, (None, 0))
            params, known = _sig_params(sig, proto)
            ret = _sig_ret(proto)
            if proto:
                n_proto += 1
            if params is None:                       # stub unresolved -> honest scaffold
                f.write("/* @%d %s -- API (mark documented/undocumented); scaffold. "
                        "TODO: signature + marshal + thunk + return */\n"
                        "%sDWORD WINAPI %s(void) { return 0; }\n\n" % (ordv, nm, undef, nm))
                continue
            n_arity += 1
            argbytes, nptr = sig
            n_ptr += nptr
            tag = ("documented" if (proto and proto.get("doc") == "documented") else
                   ("%d-arg" % (argbytes // 4)) + (", %d ptr" % nptr if nptr else ""))
            body = "{ }" if ret.strip().lower() == "void" else "{ return 0; }"   # void: no return value
            f.write("/* @%d %s -- %s; scaffold. TODO: marshal args, thunk to %s, return */\n"
                    "%s%s %s %s(%s) %s\n\n" % (ordv, nm, tag, dll16, undef, ret,
                                               _sig_conv(proto), nm, _def_params(params), body))

    return {"module": module, "n_exports": len(exps), "k32_ordinals": ords,
            "unknown_ordinals": unknown, "outdir": outdir, "target16": target16,
            "dll_candidates": dll_cands, "n_signatures_recovered": n_arity,
            "n_pointer_args": n_ptr, "n_sdk_prototypes": n_proto,
            "recovered": ["%s.def" % module, "%sk32.def" % module,
                          "%d/%d signatures (stub arity)" % (n_arity, len(exps))],
            "scaffolded": ["%s.thk" % module, "%s.c" % module]}


# ------------------------------------------------------------------ RECOVER from a decompile (logic modules)
def _opt(argv, name):
    return argv[argv.index(name) + 1] if name in argv and argv.index(name) + 1 < len(argv) else None


def main(argv=None):
    from codebreaker.base import corpus
    argv = sys.argv[1:] if argv is None else argv
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return 0
    mod = argv[0]
    path = corpus.retail(mod) if not os.path.exists(mod) else mod
    c = classify(path)
    print("=== classify %s ===" % os.path.basename(path))
    for k, v in c.items():
        print("  %-12s %s" % (k, v))
    recdir = _opt(argv, "--recover")
    if recdir:
        print("=== recover (flat-thunk) -> %s ===" % recdir)
        rr = recover_flat_thunk(path, recdir)
        if rr.get("error"):
            print("  " + rr["error"]); return
        print("  module=%s  exports=%d  KERNEL32 ordinals=%s%s" % (
            rr["module"], rr["n_exports"], rr["k32_ordinals"],
            ("  UNKNOWN=%s" % rr["unknown_ordinals"]) if rr["unknown_ordinals"] else ""))
        print("  16-bit target: %s%s" % (
            rr.get("target16") or "?16.DLL (ambiguous)",
            ("   candidates=%s" % rr["dll_candidates"]) if not rr.get("target16") else ""))
        print("  signatures recovered from stubs: %d/%d  (%d pointer args, %d SDK prototypes)" % (
            rr["n_signatures_recovered"], rr["n_exports"], rr["n_pointer_args"], rr["n_sdk_prototypes"]))
        print("  recovered (from binary): %s" % ", ".join(rr["recovered"]))
        print("  scaffolded (need curation): %s" % ", ".join(rr["scaffolded"]))
        if "--ow-compile" in argv:                          # verify with the from-source OpenWatcom cc
            cr = compile_ow(recdir)
            if cr.get("error"):
                print("  ow-compile: " + cr["error"])
            else:
                for fn, rc, err in cr["results"]:
                    print("  ow-compile %-16s %s%s" % (fn, "OK" if rc == 0 else "FAIL", "  " + err if err else ""))
    built = _opt(argv, "--diff")
    srcdir = _opt(argv, "--build")
    owdir = _opt(argv, "--ow-build")
    if owdir:
        print("=== build (flat-thunk, from-source OpenWatcom) from %s ===" % owdir)
        res = build_flat_thunk_ow(owdir)
        if not res["ok"]:
            print("  BUILD FAILED: %s\n" % res.get("error", "") +
                  "\n".join("    " + l for l in res.get("log", "").splitlines()[-12:]))
            return
        built = res["dll"]
        print("  built %s (%d bytes, %d exports) -- no Microsoft tools" % (
            os.path.basename(built), res["bytes"], res["n_exports"]))
    if srcdir:
        print("=== build (flat-thunk) from %s ===" % srcdir)
        res = build_flat_thunk(srcdir)
        if not res["ok"]:
            print("  BUILD FAILED:\n" + "\n".join("    " + l for l in res["log"].splitlines()[-12:]))
            return
        built = res["dll"]
        print("  built %s (%d bytes)" % (os.path.basename(built), os.path.getsize(built)))
    if built:
        print("=== diff %s vs %s ===" % (os.path.basename(built), os.path.basename(path)))
        print(_fmt_diff(bindiff(built, path)))


if __name__ == "__main__":
    sys.exit(main() or 0)
