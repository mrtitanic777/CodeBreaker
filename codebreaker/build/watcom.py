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


def _ow():
    return {"root": toolchain.openwatcom_root(), "wcc": toolchain.ow_cc("pe"),
            "wlink": toolchain.ow_tool("wlink"), "inc": toolchain.ow_include("pe")}


def _scratch(*parts):
    """Build scratch lives in the temp dir, not beside the source (it grew to many thousands of
    files under src/targets/win95 and slowed every search of the repo)."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bodyverify", *parts)


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


def _import_lines(obj, names, imports, numbered=None):
    """wlink `import` directives binding each imported name's decorated symbol to DLL.name --
    or to DLL.number for one the original imports by number (`numbered`: {name: (DLL, number, ...)})."""
    ext = _omf_externs(obj)
    out = []
    for n in sorted(names):
        for sym in ext:
            if sym.lstrip("_").split("@")[0] == n:
                if numbered and n in numbered:
                    out.append("import '%s' '%s'.%d\n" % (sym, numbered[n][0], numbered[n][1]))
                elif n in imports:
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
