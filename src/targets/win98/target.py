#!/usr/bin/env python3
"""
target.py — what "Windows 98" means to the rest of the toolchain.

Fourth release, and the second with more than one executable format in the
core. The lineage:

    Windows 1.0  — one concatenated boot image (WIN100.BIN + .OVL), all NE.
    Windows 3.1  — separate NE modules on a flat CD; `target.py` had to
                   *describe* the module set instead of assuming one path.
    Windows 95   — a spanning Quantum cabinet set that expanded to a corpus in
                   THREE formats at once (~938 modules, ~43 MB of code).
    Windows 98   — the same tri-format shape, larger: a spanning LZX cabinet set
                   (`cab.py`) of ~6,956 files, ~2,121 code-bearing modules and
                   ~129 MB of code — roughly 3× the 95 corpus.

The three facts that shape everything downstream:

  * **Three executable formats coexist.** NE (the 16-bit compatibility layer:
    KRNL386, USER, GDI, the .DRV drivers, most bundled apps — ~567 modules here),
    PE (the 32-bit engines: KERNEL32/USER32/GDI32 and 1,400+ DLLs/EXEs), and LE
    (the VxDs — .VXD/.386/.PDR and the VMM32 container — ~285 modules). A tool
    that assumes NE, as every 3.1 tool did, is wrong here. `format_of()` decides
    per file; `open_module()` dispatches to the right loader.

  * **USER/GDI are still a thin 32-bit skin over the 16-bit core; the kernel is
    not.** In 98 SE, USER32 is ~49 KB of code and USER.EXE (16-bit) ~233 KB;
    GDI32 ~123 KB vs GDI.EXE ~203 KB — most window/graphics behaviour still
    lives in the 16-bit world and the 32-bit side thunks down. But KERNEL32
    (~360 KB) has outgrown KRNL386 (~65 KB): the kernel moved up to 32-bit far
    more than USER/GDI did. So the 16-bit trio is not legacy baggage to skip —
    for USER/GDI it is where the behaviour is. Both halves are `core`.

  * **Module names still collide, and now across formats too.** Key on the file,
    never the NE/PE internal module name — the 3.1 rule, still in force.

Release: Windows 98 Second Edition (volume label WIN98 SE), the CD this corpus
was carved from; retail binaries dated Apr 1999. Recorded here because module
sets differ between 98 First Edition / 98 SE / ME.
"""
import glob
import os
import struct

FILES = "files"

# --------------------------------------------------------------------------
# Tiers, most-central first. Order is the recommended work order.
# --------------------------------------------------------------------------

# The core is deliberately BOTH halves. The 16-bit trio still holds the bulk of
# the window/graphics behaviour; the 32-bit trio is the new public surface.
CORE = [
    # 32-bit engines (PE)
    "KERNEL32.DLL", "USER32.DLL", "GDI32.DLL",
    # 16-bit compatibility core (NE) — where most of USER/GDI actually lives
    "KRNL386.EXE", "USER.EXE", "GDI.EXE",
    # the VMM / VxD bundle — a W3/W4-compressed container (the descendant of
    # 3.1's WIN386.EXE W3 container), not a plain LE. Needs its own loader.
    "VMM32.VXD",
]

# 32-bit system DLLs + their 16-bit companions. Only names that ship in the
# main WIN98 cabinet set; missing companions live in PRECOPY/bootstrap cabs.
SYSTEM = [
    # 32-bit (PE)
    "SHELL32.DLL", "COMDLG32.DLL", "COMCTL32.DLL", "ADVAPI32.DLL",
    "OLE32.DLL", "OLEAUT32.DLL", "OLECLI32.DLL", "OLESVR32.DLL",
    "RPCRT4.DLL", "WINMM.DLL", "VERSION.DLL", "LZ32.DLL", "MPR.DLL",
    "WSOCK32.DLL", "CRTDLL.DLL", "COMMCTRL.DLL", "SETUPAPI.DLL",
    "RUNDLL32.EXE", "RUNDLL.EXE",
    # 16-bit companions (NE)
    "MMSYSTEM.DLL", "TOOLHELP.DLL", "OLECLI.DLL", "OLESVR.DLL",
    "SYSTEM.DRV", "KEYBOARD.DRV", "MOUSE.DRV", "SETUPX.DLL", "DDEML.DLL",
]

APPS = [
    "EXPLORER.EXE", "PROGMAN.EXE", "WINFILE.EXE", "NOTEPAD.EXE", "WRITE.EXE",
    "WORDPAD.EXE", "MSPAINT.EXE", "PBRUSH.EXE", "CALC.EXE", "CARDFILE.EXE",
    "SOL.EXE", "WINMINE.EXE", "FREECELL.EXE", "MSHEARTS.EXE", "CHARMAP.EXE",
    "CLIPBRD.EXE", "REGEDIT.EXE", "SYSEDIT.EXE", "CONTROL.EXE", "WINHELP.EXE",
    "HH.EXE", "TASKMAN.EXE", "DRWATSON.EXE", "PACKAGER.EXE", "SNDVOL32.EXE",
    "SNDREC32.EXE", "MPLAYER.EXE", "CDPLAYER.EXE", "DIALER.EXE", "HYPERTRM.EXE",
    "TELNET.EXE", "WINIPCFG.EXE", "NOTEPAD.EXE", "TERMINAL.EXE",
]

# Third-party / OEM stacks bundled on the media but not Microsoft's Windows.
THIRDPARTY_PREFIXES = ("NW", "NETWARE", "NAV", "AOL", "MSN", "PCTCP",
                       "ODI", "IPX", "SPX", "VREDIR")

TIER_ORDER = ["core", "system", "drivers", "apps", "thirdparty"]

# Driver formats: .DRV is usually NE, VxDs are LE, .MPD/.SYS/.PCI are PE minidrivers.
_DRIVER_EXT = (".DRV", ".VXD", ".386", ".PDR", ".MPD", ".CPL", ".SCR",
               ".3GR", ".ACM", ".ACV", ".TSP", ".DRW")

# Resource-only stubs with no code — excluded from coverage, as in 3.1.
_NOCODE_EXT = (".FON", ".FOT", ".TTF", ".BMP", ".CUR", ".ICO", ".WAV",
               ".HLP", ".CNT", ".INF", ".INI", ".TXT", ".DAT", ".REG")


# --------------------------------------------------------------------------
# Format detection — the central new capability vs. 3.1.
# --------------------------------------------------------------------------

def format_of(path):
    """'NE' | 'PE' | 'LE' | 'LX' | 'MZ' | None. Reads only the two headers."""
    from src.formats import detect
    return detect.format_of(path)


def all_modules():
    """Every executable (NE/PE/LE/LX) module in files/, as paths."""
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
    fmt = format_of(path)
    if fmt == "LE" or base.endswith(_DRIVER_EXT) or base.startswith("KBD"):
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
# Loading & measuring — dispatched by format.
# --------------------------------------------------------------------------

def resolve(name):
    """Accept 'USER', 'USER.EXE', or a path; return a path under files/."""
    # Only treat `name` as an already-formed PATH if it actually contains a
    # separator — NOT merely because it starts with the corpus dir name (a bare
    # module like 'FILESEC.VXD' starts with 'files' but is a name, not a path).
    if os.path.sep in name or "/" in name:
        return name
    cand = os.path.join(FILES, os.path.basename(name))
    if os.path.exists(cand):
        return cand
    for ext in (".DLL", ".EXE", ".DRV", ".VXD", ".386", ".PDR", ".CPL",
                ".MOD", ".SCR", ".MPD"):
        cand = os.path.join(FILES, os.path.basename(name).upper() + ext)
        if os.path.exists(cand):
            return cand
        cand = os.path.join(FILES, os.path.basename(name).lower() + ext.lower())
        if os.path.exists(cand):
            return cand
    if name.upper() in ("KERNEL", "KRNL"):
        return os.path.join(FILES, "krnl386.exe")
    raise ValueError(f"no module matching {name!r} under {FILES}/")


def open_module(name):
    """Open a Win98 module by name or path, dispatched by format.

    NE -> disasm.open_ne (the 16-bit loader ported from 3.1)
    PE -> pe.open_pe     (the new 32-bit reader)
    LE -> not yet implemented (VxD/LE loader is the next parser to write)
    """
    path = resolve(name)
    fmt = format_of(path)
    if fmt == "NE":
        from src.disasm.disasm import open_ne
        return open_ne(path)
    if fmt == "PE":
        from src.formats.pe import open_pe
        return open_pe(path)
    if fmt in ("LE", "LX"):
        raise NotImplementedError(
            f"{os.path.basename(path)} is {fmt} (VxD) — LE loader not written "
            f"yet. See PLAN.md Phase 3.")
    if fmt == "MZ":
        # e.g. VMM32.VXD: a W3/W4-compressed VxD container, not a flat LE.
        # Needs a container decompressor before an LE loader can see inside.
        raise NotImplementedError(
            f"{os.path.basename(path)} is an MZ/W3/W4 container (compressed VxD "
            f"bundle) — no container loader yet. See PLAN.md Phase 3.")
    raise ValueError(f"{path}: unrecognised format {fmt}")


def code_bytes(path):
    """Code size in bytes, by format. LE/LX now measured via le.py (executable
    objects); a W3/W4 container (VMM32) still returns 0 until it is unbundled."""
    fmt = format_of(path)
    try:
        if fmt == "NE":
            from src.disasm.disasm import open_ne
            ne = open_ne(path)
            return sum(s["length"] for s in ne.segments() if s["kind"] == "CODE")
        if fmt == "PE":
            from src.formats.pe import open_pe
            return open_pe(path).code_bytes()
        if fmt in ("LE", "LX"):
            from le import LEFile
            return LEFile(path).code_bytes()
    except Exception:
        return 0
    return 0


def label(path):
    """'USER.EXE [NE] (USER)' — file first, format second, names collide."""
    fmt = format_of(path)
    try:
        mod = open_module(path)
        return f"{os.path.basename(path)} [{fmt}] ({mod.module_name()})"
    except Exception:
        return f"{os.path.basename(path)} [{fmt}]"


def format_census():
    """{format: (count, code_bytes)} over files/ — the tri-format picture."""
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
    print("\nby format (code_bytes; LE pending an LE loader):")
    for fmt, (c, b) in sorted(format_census().items()):
        print(f"  {fmt:<5} {c:>5} modules  {b:>14,} b")
