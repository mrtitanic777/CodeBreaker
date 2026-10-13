"""watcom.py -- compiling and linking reconstructed C with OpenWatcom.

The pieces a whole-module build needs: where the tools are and the environment they run in,
the flags a reconstruction is compiled with, how a module's imports are declared and linked
so calls reach the same DLL functions the original calls, and a check that catches calls
whose argument count disagrees with the callee's definition before the compiler does.
"""
import os
import re
from codebreaker.build import watcom as toolchain
from codebreaker.formats.pe import open_pe
import struct


def _ow():
    return {"root": toolchain.openwatcom_root(), "wcc": toolchain.ow_cc("pe"),
            "wlink": toolchain.ow_tool("wlink"), "inc": toolchain.ow_include("pe")}


def _scratch(*parts):
    """Build scratch lives in the temp dir, not beside the source (it grew to many thousands of
    files under src/targets/win95 and slowed every search of the repo)."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bodyverify", *parts)


# -ecc makes __cdecl the DEFAULT calling convention. Without it wcc386 passes arguments in registers
# (its __watcall), so a rebuilt function read its parameters from EAX/EDX/EBX/ECX while the oracle --
# like every caller of the original -- puts them on the stack. A rebuilt function that depended on an
# argument therefore never saw it, and failed verification however good the reconstruction was. A
# convention written on a signature (__stdcall, __fastcall) still wins.
_CFLAGS = ["-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecc"]


# The compiler emits calls to its own runtime helpers for some constructs: __TryInit/__TryUnwind/
# __Except for __try/__except, and 64-bit arithmetic and floating-point helpers. They live in the
# C runtime library; without it every body using such a construct failed to link. Local stubs
# for a body's callees are defined in the object and still take precedence.
_RTL = "library clib3r.lib\n"


# Where a rebuilt DLL is based. The linker's default (0x400000) is exactly where ordinary computed
# values fall -- the sum of two of the oracle's scratch pointers, say -- and a value inside the
# rebuilt image is taken for "an address of this build's own data", which the original cannot
# have: a correct body was then reported as passing a different argument. Up here nothing collides.
IMAGE_BASE = 0x6E000000


_BASE = "option offset=0x%X\n" % IMAGE_BASE


def _ow_env(ow):
    return toolchain.ow_env("pe")


# ---------------------------------------------------------------------------------------------
# Real imports. A rebuilt body must call the retail module's imported functions THROUGH AN
# IMPORT, not through a local stand-in. The oracle records an external call when execution
# lands on an import; a local `int HeapAlloc(){return 0;}` is invisible to it. With stand-ins
# the rebuilt side never called anything, so any body whose retail path reached an API could
# not match -- most of the code that matters. (The 16-bit lane had the same flaw.)
# ---------------------------------------------------------------------------------------------
_IMPORT_MAPS = {}


def import_map(module):
    """{name: dll} for the functions a retail PE imports by name (the first DLL wins)."""
    if module not in _IMPORT_MAPS:
        out = {}
        try:
            for dll, name in open_pe(module).imports().values():
                if name and re.fullmatch(r"[A-Za-z_]\w*", name):
                    out.setdefault(name, os.path.splitext(dll)[0].upper())
        except Exception:
            pass
        _IMPORT_MAPS[module] = out
    return _IMPORT_MAPS[module]


def _omf_externs(data):
    """External symbol names an OMF object refers to (EXTDEF records, 0x8C)."""
    out, i, n = [], 0, len(data)
    while i + 3 <= n:
        rectype = data[i]
        end = i + 3 + (data[i + 1] | (data[i + 2] << 8))
        if end > n:
            break
        if rectype == 0x8C:
            q = i + 3
            while q < end - 1:                           # end-1 is the record checksum
                ln = data[q]
                out.append(data[q + 1:q + 1 + ln].decode("latin1"))
                q += 1 + ln
                q += 2 if (q < end and data[q] & 0x80) else 1     # type index
        i = end
    return out


def _import_lines(obj, names, imports):
    """wlink `import` directives binding each imported name's decorated symbol to DLL.name."""
    ext = _omf_externs(obj)
    out = []
    for n in sorted(names):
        for sym in ext:
            if sym.lstrip("_").split("@")[0] == n:
                out.append("import '%s' '%s'.'%s'\n" % (sym, imports[n], n))
                break
    return "".join(out)


def _first(*cands):
    for c in cands:
        if c and os.path.exists(c):
            return c
    return ""


# ===============================================================================
# The RECONSTRUCTION pipeline's own tools: the native decompilers and OpenWatcom.
# (The registry above is the period Microsoft toolchain used for byte-exact work.)
# Every consumer used to rebuild these paths by hand -- the decompiler bin dir in six
# files, the OpenWatcom root in thirteen -- so pinning a different build meant editing
# each one. They resolve here now; nothing else should spell these paths.
# ===============================================================================

# lane -> decompiler executable (one engine, four loaders)
DECOMPILERS = {"pe": "win95pedec.exe", "ne": "win95dec.exe",
               "le": "win95ledec.exe", "dos": "win95mzdec.exe"}


def decompiler_dir():
    """Directory holding the native decompiler executables. CB_DECOMPILER_BIN pins another
    build (a frozen copy for a long sweep, or the old side of an A/B comparison)."""
    from codebreaker.base import settings
    return os.environ.get("CB_DECOMPILER_BIN") or settings.get("decompiler_bin")


def decompiler(which):
    """Absolute path of a native decompiler, by lane ('pe'/'ne'/'le'/'dos') or executable
    name ('win95pedec.exe'). Returns "" when it is not built."""
    exe = DECOMPILERS.get(which, which)
    if not exe.lower().endswith(".exe"):
        exe += ".exe"
    if exe == DECOMPILERS["ne"] and os.environ.get("CB_WIN95DEC"):   # older single-tool pin
        return _first(os.environ["CB_WIN95DEC"])
    return _first(os.path.join(decompiler_dir(), exe))


def decompiler_build_id():
    """A short id that changes whenever any decompiler executable changes (size + mtime).
    Verification results are only valid for the build that produced them; key caches on this."""
    import hashlib
    h = hashlib.sha1()
    for lane in sorted(DECOMPILERS):
        p = decompiler(lane)
        if p:
            st = os.stat(p)
            h.update(("%s:%d:%d;" % (lane, st.st_size, int(st.st_mtime))).encode())
    return h.hexdigest()[:12]


def openwatcom_root():
    """Root of the from-source OpenWatcom release tree (CB_OPENWATCOM overrides)."""
    from codebreaker.base import settings
    return os.environ.get("CB_OPENWATCOM") or settings.get("openwatcom")


def ow_tool(name):
    """Absolute path of an OpenWatcom tool ('wcc', 'wcc386', 'wlink', 'wasm', 'wrc').
    Absolute because Windows CreateProcess searches the CALLER's PATH, not the env passed in.
    Returns "" when OpenWatcom is not built."""
    if not name.lower().endswith(".exe"):
        name += ".exe"
    return _first(os.path.join(openwatcom_root(), "binnt64", name))


def ow_cc(cls):
    """The C compiler for a module class: 'ne' (16-bit Windows) -> wcc, else wcc386."""
    return ow_tool("wcc" if cls == "ne" else "wcc386")


def ow_include(cls):
    """Header search dirs for a module class: Win16 uses h/win, Win32 uses h/nt."""
    h = os.path.join(openwatcom_root(), "h")
    return [h, os.path.join(h, "win" if cls == "ne" else "nt")]


def ow_lib(cls):
    """Runtime library dir for a module class (the Win16 one holds libentry.obj + clib*.lib)."""
    root = openwatcom_root()
    return os.path.join(root, "lib286", "win") if cls == "ne" else os.path.join(root, "lib386", "nt")


def ow_env(cls):
    """Process environment for running OpenWatcom tools on a module class."""
    root = openwatcom_root()
    env = dict(os.environ)
    env["NoDefaultCurrentDirectoryInExePath"] = ""       # OW runs its own tools from cwd
    env["WATCOM"] = root
    env["INCLUDE"] = os.pathsep.join(ow_include(cls))
    env["WLINK_LNK"] = os.path.join(root, "binnt64", "wlink.lnk")
    env["PATH"] = os.path.join(root, "binnt64") + os.pathsep + env.get("PATH", "")
    return env


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
