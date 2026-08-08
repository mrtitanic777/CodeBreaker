#!/usr/bin/env python3
"""
target.py — what "Windows 95" means to the rest of the toolchain.

Third release, third medium, and the first with more than one executable format
in the core. The lineage:

    Windows 1.0  — one concatenated boot image (WIN100.BIN + .OVL), all NE.
    Windows 3.1  — separate NE modules on a flat CD; `target.py` had to
                   *describe* the module set instead of assuming one path.
    Windows 95   — a spanning Quantum cabinet set (`cab.py`) that expands to a
                   corpus in THREE formats at once.

The three facts that shape everything downstream:

  * **Three executable formats coexist.** NE (the 16-bit compatibility layer:
    KRNL386, USER, GDI, the .DRV drivers, most bundled apps), PE (the 32-bit
    engines: KERNEL32/USER32/GDI32 and 600+ DLLs/EXEs), and LE (the VxDs —
    .VXD/.386/.PDR and the VMM32 container). A tool that assumes NE, as every
    3.1 tool did, is wrong here. `format_of()` decides per file; `open_module()`
    dispatches to the right loader.

  * **The 32-bit core is a thin skin over the 16-bit core.** In OSR2, USER32 is
    ~22 KB of code and USER.EXE (16-bit) is ~200 KB: most of USER/GDI still
    lives in the 16-bit world and the 32-bit side thunks down to it. So the
    16-bit trio is not legacy baggage to skip — it is where the behaviour is.
    Both halves are `core`.

  * **Module names still collide, and now across formats too.** Key on the file,
    never the NE/PE internal module name — the 3.1 rule, still in force.

Release: OSR2.5 (volume label WIN_95C, 4.00.950 C), the CD this corpus was
carved from. Build dates Aug 1996. Recorded here because module sets differ
between 95 RTM / OSR1 / OSR2 / OSR2.5.
"""
import glob
import os
import shutil
import struct
import subprocess

FILES = "files"                                          # the resolve dir (a lazily-filled cache)
MEDIA = os.path.join("disc", "src", "WIN95")             # the ORIGINAL install media (loose + CABs)
# module-name -> extension candidates, so a bare 'USER' resolves to USER.EXE etc.
_RES_EXT = (".dll", ".exe", ".drv", ".vxd", ".386", ".pdr", ".cpl", ".mod", ".scr", ".mpd")

_FILE_INDEX = None
_MEDIA_MANIFEST = None


def _files_index():
    """{basename.lower(): path} for every file under files/, at ANY depth. Cached per
    process. This is what lets the corpus be organized into a real install-tree
    (files/WINDOWS/SYSTEM/…) without every resolver learning the layout — module names
    are unique, so a filename still resolves to one path wherever it physically sits."""
    global _FILE_INDEX
    if _FILE_INDEX is None:
        idx = {}
        if os.path.isdir(FILES):
            for dp, _dns, fns in os.walk(FILES):
                for fn in fns:
                    idx.setdefault(fn.lower(), os.path.join(dp, fn))
        _FILE_INDEX = idx
    return _FILE_INDEX


def _ensure_from_media(name):
    """ON-DEMAND: materialize a module from the ORIGINAL media (disc/src/WIN95) into files/ --
    a loose file, else the file inside its spanning cabinet (7-Zip single-file extract). files/
    is thus a LAZILY-FILLED CACHE: it need not be pre-extracted; delete it and it repopulates as
    tools ask for binaries. Returns the new path (also indexing it), or None if the media / file
    isn't there. The original media is the ground-truth donor + verification source (see docs);
    win95re.iso is our REBUILD and would be circular for verification, so we pull from disc/src."""
    if not os.path.isdir(MEDIA):
        return None
    b = os.path.basename(name).lower()
    cands = {b} | {b + e for e in _RES_EXT}
    os.makedirs(FILES, exist_ok=True)
    for dp, _dns, fns in os.walk(MEDIA):                  # 1. a loose file on the media
        for fn in fns:
            if fn.lower() in cands:
                dest = os.path.join(FILES, fn)
                if not os.path.exists(dest):
                    shutil.copy2(os.path.join(dp, fn), dest)
                _files_index()[fn.lower()] = dest
                return dest
    try:                                                 # 2. inside a cabinet
        import cab
        global _MEDIA_MANIFEST
        if _MEDIA_MANIFEST is None:
            _MEDIA_MANIFEST = {c.name.lower(): c for c in cab.manifest(MEDIA)}
        cf = next((_MEDIA_MANIFEST[c] for c in cands if c in _MEDIA_MANIFEST), None)
        if cf is None:
            return None
        subprocess.run([cab.find_7zip(), "e", "-y", "-o" + FILES,
                        cab._head_cabinet(MEDIA), cf.name], capture_output=True, text=True)
        dest = os.path.join(FILES, cf.name)
        if os.path.exists(dest):
            _files_index()[cf.name.lower()] = dest
            return dest
    except Exception:
        pass
    return None

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
    # the VMM / VxD bundle — a W4-compressed container (the descendant of
    # 3.1's WIN386.EXE W3 container), not a plain LE. Needs its own loader.
    "VMM32.VXD",
]

# 32-bit system DLLs + their 16-bit companions. Only names that ship in the
# main WIN95 cabinet set; missing companions live in PRECOPY/bootstrap cabs.
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


_MODULE_EXTS = (".dll", ".exe", ".drv", ".vxd", ".386", ".pdr", ".cpl", ".scr",
                ".mpd", ".acm", ".ax", ".tsp", ".ocx", ".mod", ".sys")


def _media_module_names():
    """Executable-looking filenames on the ORIGINAL media (disc/src): loose files + every
    cabinet's contents. Lets the corpus be ENUMERATED without a pre-extracted files/ tree."""
    names = set()
    if not os.path.isdir(MEDIA):
        return names
    for dp, _dns, fns in os.walk(MEDIA):
        for fn in fns:
            if fn.lower().endswith(_MODULE_EXTS):
                names.add(fn)
    try:
        import cab
        for cf in cab.manifest(MEDIA):
            if cf.name.lower().endswith(_MODULE_EXTS):
                names.add(cf.name)
    except Exception:
        pass
    return names


def all_modules():
    """Every executable (NE/PE/LE/LX) module, as paths under files/, at any depth. When files/
    is pre-extracted, enumerate it directly (fast). When files/ is EMPTY but the original media
    (disc/src) is present, enumerate the corpus from the cabinet manifest + loose media files
    and materialize each on demand via resolve() -- so files/ can be dropped entirely and the
    tier/coverage tools still see the whole corpus (paying a one-time extract)."""
    idx = _files_index()
    if idx:
        return [p for p in sorted(set(idx.values()))
                if os.path.isfile(p) and format_of(p) in ("NE", "PE", "LE", "LX")]
    out = []
    for name in sorted(_media_module_names()):           # files/ empty -> media-backed
        try:
            p = resolve(name)                            # extracts on demand
            if format_of(p) in ("NE", "PE", "LE", "LX"):
                out.append(p)
        except Exception:
            pass
    return sorted(set(out))


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
    idx = _files_index()
    b = os.path.basename(name).lower()
    if b in idx:                                         # exact filename (any depth)
        return idx[b]
    for ext in _RES_EXT:                                  # bare module name -> add ext
        if b + ext in idx:
            return idx[b + ext]
    if name.upper() in ("KERNEL", "KRNL") and "krnl386.exe" in idx:
        return idx["krnl386.exe"]
    p = _ensure_from_media(name)                          # files/ miss -> pull from the media
    if p:
        return p
    raise ValueError(f"no module matching {name!r} under {FILES}/ (and not on media {MEDIA}/)")


def open_module(name):
    """Open a Win95 module by name or path, dispatched by format.

    NE -> disasm.open_ne (the 16-bit loader ported from 3.1)
    PE -> pe.open_pe     (the new 32-bit reader)
    LE -> not yet implemented (VxD/LE loader is the next parser to write)
    """
    path = resolve(name)
    fmt = format_of(path)
    if fmt == "NE":
        from engine.disasm_engine.disasm import open_ne
        return open_ne(path)
    if fmt == "PE":
        from engine.format_loaders.pe import open_pe
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
            from engine.disasm_engine.disasm import open_ne
            ne = open_ne(path)
            return sum(s["length"] for s in ne.segments() if s["kind"] == "CODE")
        if fmt == "PE":
            from engine.format_loaders.pe import open_pe
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
