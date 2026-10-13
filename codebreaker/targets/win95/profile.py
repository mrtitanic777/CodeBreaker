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
import os
import shutil
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
        from codebreaker.formats import cab
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


# --------------------------------------------------------------------------
# Format detection — the central new capability vs. 3.1.
# --------------------------------------------------------------------------

def format_of(path):
    """'NE' | 'PE' | 'LE' | 'LX' | 'MZ' | None. Reads only the two headers."""
    from codebreaker.formats import detect
    return detect.format_of(path)


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
