#!/usr/bin/env python3
r"""toolchain.py -- the single registry for Win95's ACTUAL build toolchain.

The byte-exact evidence (codegen bake-off + the linker version stamped in the shipped
PE headers) pins the exact Win95 tools, and every one of them is vendored locally under
`<target>/docs/reference/toolchain/`:

    role        tool                       version        source            our dir
    ------------------------------------------------------------------------------------
    compiler    CL (C2 backend)            9.00           MSVC 2.0          msvc20/VC
    linker      LINK (core modules)        2.50           MSVC 2.0          msvc20/VC/BIN
    linker      LINK (VxD / multimedia)    2.60.5046      Win95 DDK         ddk_link260
    assembler   ML (MASM)                  6.11c          Win95 DDK         masm611c

The two linkers are the important subtlety: KERNEL32/GDI32/USER32 were linked with the
stock MSVC 2.0 LINK 2.50, while the multimedia DLLs (MSVFW32/MSACM32) and the VxDs were
linked with the DDK's patched LINK 2.60 (its README: "should only be used by ddk
developers ... w.r.t. linking VxDs"). Same CL 9.00 compiler across the board -- only the
link stage differs. So per-function CODE bytes come from one compiler; WHOLE-BINARY
reproduction needs the right linker per module, which is what linker_for_module() picks.

This module only RESOLVES paths + versions (no compiling/linking here); cc32 owns the
compile step (it already defaults to msvc20). Everything returns "" when a tool isn't
staged, so callers degrade gracefully.
"""
import os
import subprocess

from codebreaker.base import layout


# --- component/module -> which linker built it ---------------------------------
# Multimedia DLLs shipped linked with the DDK's LINK 2.60 (verified: MSVFW32/MSACM32
# stamp linker 2.60 in their PE headers); VxDs likewise (the DDK linker's stated
# purpose). Everything else = the stock MSVC 2.0 LINK 2.50.
_MMEDIA_MODULES = {
    "msvfw32.dll", "msacm32.dll", "mmsystem.dll", "msvideo.dll", "msacm.dll",
    "mciole.dll", "mciavi.drv", "msvidc32.dll", "msrle32.dll", "mmci.dll",
    "dispdib.dll", "avifil32.dll", "mciqtz.drv", "mciseq.drv", "mciwave.drv",
}


def _tc(base=""):
    """The vendored-toolchain root: <target>/docs/reference/toolchain."""
    return layout.reference_sub("toolchain", base=base)


def _first(*cands):
    for c in cands:
        if c and os.path.exists(c):
            return c
    return ""


# --- compiler ------------------------------------------------------------------
def compiler_vc(base=""):
    """VC root of the era-correct compiler (CL 9.00 == MSVC 2.0), matching cc32's
    default preference: msvc20 -> 21 -> 22 -> 42. Returns the dir holding BIN/CL.EXE."""
    tc = _tc(base)
    for cand in (os.path.join(tc, "msvc20", "VC"),
                 os.path.join(tc, "msvc21", "VC"),
                 os.path.join(tc, "msvc22", "VC"),
                 os.path.join(tc, "msvc42", "DEVSTUDIO", "VC")):
        if os.path.exists(os.path.join(cand, "BIN", "CL.EXE")):
            return cand
    return ""


def compiler_exe(base=""):
    vc = compiler_vc(base)
    return os.path.join(vc, "BIN", "CL.EXE") if vc else ""


# --- linkers -------------------------------------------------------------------
def linker(kind="core", base=""):
    """LINK.EXE for a link KIND:
      'core'            -> MSVC 2.0 LINK 2.50   (kernel32/gdi32/user32/...)
      'vxd' | 'mmedia'  -> DDK    LINK 2.60     (multimedia DLLs, VxDs)
    Returns "" if that linker isn't staged."""
    tc = _tc(base)
    if kind in ("vxd", "mmedia"):
        return _first(os.path.join(tc, "ddk_link260", "LINK.EXE"))
    return _first(os.path.join(tc, "msvc20", "VC", "BIN", "LINK.EXE"),
                  os.path.join(tc, "msvc21", "VC", "BIN", "LINK.EXE"),
                  os.path.join(tc, "msvc22", "VC", "BIN", "LINK.EXE"))


def linker_kind_for_module(mod):
    """Which linker built module `mod` (basename, e.g. 'msvfw32.dll'): 'vxd' for VxDs,
    'mmedia' for the multimedia DLLs (both -> LINK 2.60), else 'core' (LINK 2.50).
    Uses the component tree when available (component 'mmedia'/'mm' -> 2.60), then the
    curated module set, then the .vxd extension."""
    m = os.path.basename(mod).lower()
    if m.endswith(".vxd"):
        return "vxd"
    if m in _MMEDIA_MODULES:
        return "mmedia"
    try:                                             # component-tree hint, if present
        comp = layout.tree_map().get("generated/%s.c" % m) if layout.has_tree() else None
        if comp in ("mmedia", "mm", "multimedia"):
            return "mmedia"
    except Exception:
        pass
    return "core"


def linker_for_module(mod, base=""):
    """The LINK.EXE that built `mod` (per linker_kind_for_module)."""
    return linker(linker_kind_for_module(mod), base=base)


# --- assembler -----------------------------------------------------------------
def assembler(base=""):
    """ML.EXE (MASM 6.11c, the Win95 DDK assembler). Returns "" if not staged."""
    tc = _tc(base)
    return _first(os.path.join(tc, "masm611c", "ML.EXE"),
                  os.path.join(tc, "masm611c", "BIN", "ML.EXE"))


# --- introspection -------------------------------------------------------------
def _banner(exe, args=None):
    """First version-ish line an MS tool prints (to stderr) when run with no input."""
    if not exe or not os.path.exists(exe):
        return "(missing)"
    try:
        env = os.environ.copy()
        env["PATH"] = os.path.dirname(exe) + os.pathsep + env.get("PATH", "")
        p = subprocess.run([exe] + (args or []), env=env, input=b"",
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=20)
        for ln in p.stdout.decode("latin1", "replace").splitlines():
            if "Version" in ln or "Compiler" in ln or "Assembler" in ln or "Linker" in ln:
                return ln.strip()
    except Exception as e:
        return "(err: %s)" % str(e)[:40]
    return "(no banner)"


def inventory(base=""):
    """[(role, kind, path, banner)] for the full staged toolchain -- for oraclebake."""
    rows = [
        ("compiler", "CL 9.00 / MSVC 2.0", compiler_exe(base)),
        ("linker", "core (LINK 2.50)", linker("core", base)),
        ("linker", "vxd/mmedia (LINK 2.60)", linker("vxd", base)),
        ("assembler", "ML 6.11c", assembler(base)),
    ]
    return [(role, kind, path, _banner(path)) for role, kind, path in rows]


def main():
    print("Win95 build toolchain inventory  (%s)" % _tc())
    print("=" * 72)
    for role, kind, path, banner in inventory():
        mark = "OK " if path and os.path.exists(path) else "-- "
        print("  %s %-10s %-24s %s" % (mark, role, kind, banner))
        print("        %s" % (path or "(not staged)"))


if __name__ == "__main__":
    main()


# ===============================================================================
# The RECONSTRUCTION pipeline's own tools: the native decompilers and OpenWatcom.
# (The registry above is the period Microsoft toolchain used for byte-exact work.)
# Every consumer used to rebuild these paths by hand -- the decompiler bin dir in six
# files, the OpenWatcom root in thirteen -- so pinning a different build meant editing
# each one. They resolve here now; nothing else should spell these paths.
# ===============================================================================
from codebreaker.base import paths as _paths

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


def openwatcom_src():
    """The OpenWatcom SOURCE tree that rel/ was built from (holds bld/, the per-target builds)."""
    return os.path.dirname(openwatcom_root())


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
