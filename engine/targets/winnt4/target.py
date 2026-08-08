#!/usr/bin/env python3
"""
target.py — what "Windows NT 4.0" means to the rest of the toolchain.

Fifth project in the line, and the first NT-family target. The lineage so far
was the DOS/Win16/Win9x branch:

    Windows 1.0  — one concatenated boot image (WIN100.BIN + .OVL), all NE.
    Windows 3.1  — separate NE modules on a flat CD.
    Windows 95   — a spanning Quantum cabinet set; corpus in THREE formats
                   (NE 16-bit core, PE 32-bit engines, LE VxDs).
    Windows 98   — the same tri-format shape, larger (LZX cabinets, VMM32).

Windows NT 4.0 is a **different operating system**, not the next 9x release, and
`target.py` has to stop assuming everything the 9x tools assumed:

  * **One executable format: PE.** There is no 16-bit NE core to reverse and no
    LE/VxD driver layer. Native NT modules — the kernel, the executive, the
    subsystems, the DLLs, and the *drivers* (`.SYS`) — are all 32-bit PE (i386
    here; NT 4 also shipped Alpha/MIPS/PPC, but the ISO is i386). `format_of()`
    is kept for the odd MS-DOS/WOW stub, but the 9x tri-format dispatch is gone.

  * **USER and GDI now live in kernel mode — `WIN32K.SYS`.** This is THE NT 4.0
    architectural change vs NT 3.51: the window manager and the GDI graphics
    engine moved out of the user-mode Win32 server (CSRSS) down into the kernel
    as `win32k.sys`. So the "core" here is not USER.EXE/GDI.EXE (there is no such
    thing) — it is NTOSKRNL + HAL + WIN32K + NTDLL and the Win32 client DLLs
    (KERNEL32/USER32/GDI32) that thunk into them.

  * **The install media is an `I386\\` tree of `expand`-compressed files, not a
    cabinet set.** Files on the CD end in an underscore (`NTOSKRNL.EX_`,
    `USER32.DL_`) and are single-file LZ/SZDD-compressed; `expand.exe` inflates
    them, and `TXTSETUP.SIF` catalogs the set. There is no spanning-CAB chain to
    walk. (`sp6ai386.iso` — Service Pack 6a — carries updated copies of the same
    PE modules: a second, patched baseline to diff against.)

  * **Key on the file, never the PE internal name.** The rule every prior
    project earned, still in force.

Release: Windows NT 4.0 (build 1381), i386, plus Service Pack 6a (`sp6ai386`).

HONESTY NOTE — nothing has been carved yet. The tier lists below are the
*expected* central modules, anchored to well-established NT 4.0 facts (they let
the tools tier whatever actually lands in files/). The corpus census — module
count and code bytes — is **UNKNOWN until the ISO is extracted**; it is NOT
carried over from any prior project and must not be quoted until measured. See
PLAN.md Phase 0.
"""
import glob
import os
import struct

FILES = "files"

# --------------------------------------------------------------------------
# Tiers, most-central first. Order is the recommended work order.
# Expected NT 4.0 i386 modules; refined once files/ is populated from the ISO.
# --------------------------------------------------------------------------

# The kernel-mode core + the Win32 client trio that fronts it. On NT 4 the
# window manager and GDI engine are in WIN32K.SYS (kernel), not in USER/GDI
# user-mode servers — that is the defining NT 4.0 change over NT 3.51.
CORE = [
    "NTOSKRNL.EXE",            # kernel + executive (NTKRNLMP.EXE on SMP)
    "HAL.DLL",                 # hardware abstraction layer
    "WIN32K.SYS",              # kernel-mode Win32: window manager + GDI engine
    "NTDLL.DLL",               # native API stubs + image loader
    "KERNEL32.DLL", "USER32.DLL", "GDI32.DLL",   # Win32 client trio
]

# User-mode system DLLs + the subsystem/service processes that stand up a
# running NT session. Only names known to ship on NT 4.0.
SYSTEM = [
    # Win32 client + RPC/registry/crypto/net surface
    "ADVAPI32.DLL", "RPCRT4.DLL", "SHELL32.DLL", "COMCTL32.DLL",
    "COMDLG32.DLL", "OLE32.DLL", "OLEAUT32.DLL", "OLECLI32.DLL",
    "OLESVR32.DLL", "WINMM.DLL", "VERSION.DLL", "LZ32.DLL", "MPR.DLL",
    "NETAPI32.DLL", "WSOCK32.DLL", "WS2_32.DLL", "CRTDLL.DLL", "MSVCRT.DLL",
    "RASAPI32.DLL", "SECUR32.DLL", "USERENV.DLL",
    # subsystem + session/service control (the NT boot spine)
    "SMSS.EXE", "CSRSS.EXE", "WINLOGON.EXE", "SERVICES.EXE", "LSASS.EXE",
    "SPOOLSS.EXE", "RPCSS.EXE",
    # alternate environment subsystems present on NT 4
    "PSXSS.EXE", "OS2SS.EXE",
]

APPS = [
    "EXPLORER.EXE", "PROGMAN.EXE", "TASKMGR.EXE", "CMD.EXE", "NOTEPAD.EXE",
    "WRITE.EXE", "WORDPAD.EXE", "MSPAINT.EXE", "CALC.EXE", "CLOCK.EXE",
    "CHARMAP.EXE", "CLIPBRD.EXE", "REGEDIT.EXE", "REGEDT32.EXE", "CONTROL.EXE",
    "WINHLP32.EXE", "SNDVOL32.EXE", "SNDREC32.EXE", "MPLAYER.EXE", "CDPLAYER.EXE",
    "DIALER.EXE", "HYPERTRM.EXE", "TELNET.EXE", "WINMINE.EXE", "SOL.EXE",
    "FREECELL.EXE", "PBRUSH.EXE", "DRWTSN32.EXE", "NTVDM.EXE", "WOWEXEC.EXE",
]

# Third-party / OEM stacks bundled on the media but not Microsoft's Windows NT.
THIRDPARTY_PREFIXES = ("NW", "NETWARE", "NAV", "AOL", "MSN")

TIER_ORDER = ["core", "system", "drivers", "apps", "thirdparty"]

# NT kernel-mode drivers are PE .SYS (no VxDs). .CPL/.SCR/.DRV are PE too here.
_DRIVER_EXT = (".SYS", ".DRV", ".CPL", ".SCR", ".ACM", ".AX", ".TSP")

# Resource-only / no-code files — excluded from coverage.
_NOCODE_EXT = (".FON", ".FOT", ".TTF", ".BMP", ".CUR", ".ICO", ".WAV",
               ".HLP", ".CNT", ".INF", ".INI", ".TXT", ".DAT", ".REG",
               ".SIF", ".PNF")


# --------------------------------------------------------------------------
# Format detection — kept generic; native NT 4 modules are PE.
# --------------------------------------------------------------------------

def format_of(path):
    """'NE' | 'PE' | 'LE' | 'LX' | 'MZ' | None. Reads only the two headers.

    On NT 4.0 essentially everything with code is 'PE'; NE/MZ turn up only for
    the odd MS-DOS/WOW support stub, and LE/LX should not appear at all."""
    try:
        with open(path, "rb") as f:
            h = f.read(0x40)
            if h[:2] != b"MZ" or len(h) < 0x40:
                return None
            off = struct.unpack("<H", h[0x3C:0x3E])[0]
            f.seek(off)
            sig = f.read(2)
    except OSError:
        return None
    return {b"NE": "NE", b"PE": "PE", b"LE": "LE", b"LX": "LX"}.get(sig, "MZ")


def all_modules():
    """Every executable (PE, plus any stray NE/MZ) module in files/, as paths."""
    out = []
    for p in sorted(glob.glob(os.path.join(FILES, "*"))):
        if os.path.isfile(p) and format_of(p) in ("NE", "PE", "LE", "LX"):
            out.append(p)
    return out


def _in(base, names):
    up = base.upper()
    return up in (n.upper() for n in names)


def tier_of(path):
    base = os.path.basename(path).upper()
    if _in(base, CORE):
        return "core"
    if _in(base, SYSTEM):
        return "system"
    if _in(base, APPS):
        return "apps"
    if base.startswith(THIRDPARTY_PREFIXES):
        return "thirdparty"
    if base.endswith(_DRIVER_EXT) or base.startswith("KBD"):
        return "drivers"
    return "apps" if base.endswith(".EXE") else "drivers"


def tiers():
    """{tier: [paths]} over every code-bearing module, in TIER_ORDER."""
    out = {t: [] for t in TIER_ORDER}
    for p in all_modules():
        if code_bytes(p) == 0:
            continue
        out[tier_of(p)].append(p)
    return out


# --------------------------------------------------------------------------
# Loading & measuring — PE is the native NT 4 case.
# --------------------------------------------------------------------------

def resolve(name):
    """Accept 'USER32', 'USER32.DLL', or a path; return a path under files/."""
    if os.path.sep in name or "/" in name:
        return name
    cand = os.path.join(FILES, os.path.basename(name))
    if os.path.exists(cand):
        return cand
    for ext in (".DLL", ".EXE", ".SYS", ".DRV", ".CPL", ".SCR", ".AX"):
        cand = os.path.join(FILES, os.path.basename(name).upper() + ext)
        if os.path.exists(cand):
            return cand
        cand = os.path.join(FILES, os.path.basename(name).lower() + ext.lower())
        if os.path.exists(cand):
            return cand
    raise ValueError(f"no module matching {name!r} under {FILES}/")


def open_module(name):
    """Open an NT 4 module by name or path, dispatched by format.

    PE -> pe.open_pe   (the 32-bit reader; the only case that should occur here)
    NE -> disasm.open_ne (only for a stray 16-bit MS-DOS/WOW support stub)
    """
    path = resolve(name)
    fmt = format_of(path)
    if fmt == "PE":
        from engine.format_loaders.pe import open_pe
        return open_pe(path)
    if fmt == "NE":
        from engine.disasm_engine.disasm import open_ne
        return open_ne(path)
    if fmt in ("LE", "LX"):
        raise NotImplementedError(
            f"{os.path.basename(path)} is {fmt} — unexpected on NT 4.0 "
            f"(no VxD/LE layer). Check the file.")
    raise ValueError(f"{path}: unrecognised or non-code format {fmt}")


def code_bytes(path):
    """Code size in bytes. PE is measured via pe.py; a stray NE via disasm."""
    fmt = format_of(path)
    try:
        if fmt == "PE":
            from engine.format_loaders.pe import open_pe
            return open_pe(path).code_bytes()
        if fmt == "NE":
            from engine.disasm_engine.disasm import open_ne
            ne = open_ne(path)
            return sum(s["length"] for s in ne.segments() if s["kind"] == "CODE")
    except Exception:
        return 0
    return 0


def label(path):
    """'USER32.DLL [PE] (USER32)' — file first, format second."""
    fmt = format_of(path)
    try:
        mod = open_module(path)
        return f"{os.path.basename(path)} [{fmt}] ({mod.module_name()})"
    except Exception:
        return f"{os.path.basename(path)} [{fmt}]"


def format_census():
    """{format: (count, code_bytes)} over files/ — expect PE to dominate."""
    import collections
    n = collections.Counter()
    b = collections.Counter()
    for p in all_modules():
        fmt = format_of(p)
        n[fmt] += 1
        b[fmt] += code_bytes(p)
    return {k: (n[k], b[k]) for k in n}


if __name__ == "__main__":
    t = tiers()
    print(f"{'tier':<12} {'modules':>8} {'code bytes':>14}")
    gt = gb = 0
    for name in TIER_ORDER:
        paths = t[name]
        b = sum(code_bytes(p) for p in paths)
        gt += len(paths)
        gb += b
        print(f"  {name:<10} {len(paths):>8} {b:>14,}")
    print(f"  {'TOTAL':<10} {gt:>8} {gb:>14,}")
    print("\nby format (code_bytes):")
    for fmt, (c, b) in sorted(format_census().items()):
        print(f"  {fmt:<5} {c:>5} modules  {b:>14,} b")
