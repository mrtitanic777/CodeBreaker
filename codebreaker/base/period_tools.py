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
