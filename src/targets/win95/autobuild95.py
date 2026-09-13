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


# ------------------------------------------------------------------ toolchain resolver
def _first(*cands):
    for c in cands:
        if c and os.path.exists(c):
            return c
    return None


def _reference_roots():
    """Candidate 'reference' dirs (Win95 DDK + WIN32SDK + our persisted thunk-1.01). Robust across
    however data_root/design_root resolve under a given dispatch -- we search all plausible roots."""
    from src.core import paths
    cands = []
    env = os.environ.get("WT_REFERENCE_ROOT")
    if env:
        cands.append(env)
    try:
        cands.append(os.path.join(paths.design_root(), "reference"))
    except Exception:
        pass
    dr = os.environ.get("WT_DATA_ROOT", "")
    if dr:
        cands.append(os.path.join(os.path.dirname(dr), "reference"))   # corpus parent = design
        cands.append(os.path.join(dr, "reference"))
    # last-resort absolute (this project's layout)
    cands.append(r"C:\Users\mrtit\Documents\GitHub\Windows95\design\reference")
    seen, out = set(), []
    for c in cands:
        if c and c not in seen and os.path.isdir(c):
            seen.add(c); out.append(c)
    return out


def toolchain():
    """Locate the period build tools. Flat-thunk path is all native (runs on modern Windows):
    thunk.exe (prefer the v1.01 from the Chicago beta SDK -> closest imports), MASM 6.11c, MSVC 2.0."""
    from src.core import paths
    refs = _reference_roots()

    def ref_find(*rel):
        return _first(*[os.path.join(r, *rel) for r in refs])

    cb = paths.repo_root()
    msvc2 = os.path.join(cb, "toolchains", "period", "compilers", "msvc-2.0")
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
    L = ["@echo off", "cd /d " + _winpath(outdir),
         "set INCLUDE=" + _winpath(tc["include"]),
         "set LIB=" + ";".join(_winpath(x) for x in tc["lib_dirs"]),
         "set PATH=" + _winpath(os.path.dirname(tc["cl"])) + ";%PATH%",
         "del %s.dll *.obj %s.asm %s 2>nul" % (module, thkstem, res),
         '"%s" -o %s.asm %s > _t.log 2>&1' % (_winpath(tc["thunk"]), thkstem, thk),
         '"%s" /c /W0 /DIS_32 /Fo%s_thk.obj %s.asm >> _t.log 2>&1' % (_winpath(tc["ml"]), thkstem, thkstem)]
    for c in disc["cfiles"]:
        L.append('"%s" /c /W3 /DWIN32 %s >> _t.log 2>&1' % (_winpath(tc["cl"]), os.path.basename(c)))
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
    srcdir = _opt(argv, "--build")
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
    main()
