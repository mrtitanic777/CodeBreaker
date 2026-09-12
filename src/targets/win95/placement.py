#!/usr/bin/env python3
r"""placement.py -- the canonical module -> authored-tree placement + the tree writer.

There is exactly ONE Windows 95 source tree: the repo root beside the corpus
(<design>/.. == Windows95\). Reconstruction writes each module's finished, named, beautified
source DIRECTLY into its component folder there -- no staging directories. This module owns:

  * resolve(module, fmt)     -> "component/stem": where a module lands (existing placement wins,
                                else install-dir / format / known-core / family map, else unsorted).
  * place(module, files, ...) -> write reconstructed files into <root>/<component>/<stem>/, first
                                ARCHIVING whatever is already there into a `legacy/` subfolder (the
                                original is preserved once; later regens refresh the primary only).

Policy (Logan): on a full reconstruction the fresh source is placed directly in the root tree; any
code already sitting in a module's folder is moved into `<module>/legacy/` and the fresh code written
in its place. Nothing is deleted. The first archive captures the original (curated) code; because a
later run won't re-archive over an existing `legacy/`, that original snapshot is never clobbered.
"""
import os
import re
import shutil

from src.core import paths

COMPONENTS = ["base", "comm", "core", "drivers", "games", "mapi", "mmedia", "msdos",
              "ole", "print", "setup", "shell", "unsorted", "vmm32", "vnet"]

# canonical exact module -> component (the Win95 subsystems). 32-bit core is homed distinctly
# from the 16-bit KERNEL/GDI/USER so they never collide.
KNOWN = {
    "kernel32": "core/kernel32", "krnl386": "core/kernel", "krnl286": "core/kernel",
    "gdi32": "core/gdi32", "gdi": "core/gdi", "dispdib": "core/gdi",
    "user32": "core/user32", "user": "core/user",
    "toolhelp": "core/toolhelp", "wow32": "core/thunk", "w32sys": "core/thunk",
    "version": "base/version", "ver": "base/version", "winver": "base/version",
    "lz32": "base/lz32", "lzexpand": "base/lzexpand", "shlwapi": "base/shlwapi",
    "crtdll": "base/crtdll", "msvcrt": "base/msvcrt", "advapi32": "base/advapi32",
    "rpcrt4": "base/rpcrt4", "imagehlp": "base/imagehlp", "win87em": "base/math",
    "commdlg": "base/commdlg", "comdlg32": "base/comdlg32", "mpr": "comm/mpr",
    "ole32": "ole/ole32", "ole2": "ole/ole2", "oleaut32": "ole/oleaut32", "compobj": "ole/compobj",
    "storage": "ole/storage", "typelib": "ole/typelib", "ole2disp": "ole/ole2disp",
    "ole2nls": "ole/ole2nls", "olecli": "ole/olecli", "olesvr": "ole/olesvr",
    "stdole": "ole/stdole", "stdole2": "ole/stdole", "olepro32": "ole/olepro32",
    "shell": "shell/shell", "shell32": "shell/shell32", "comctl32": "shell/comctl32",
    "comctl31": "shell/comctl31", "shdocvw": "shell/shdocvw", "browseui": "shell/browseui",
    "explorer": "shell/explorer", "control": "shell/control", "rundll32": "shell/rundll32",
    "mmsystem": "mmedia/mmsystem", "winmm": "mmedia/winmm", "msvfw32": "mmedia/msvfw32",
    "msvideo": "mmedia/msvideo", "avifile": "mmedia/avifile", "avicap": "mmedia/avicap",
    "dsound": "mmedia/dsound", "mmtask": "mmedia/mmtask", "mplayer": "mmedia/mplayer",
    "mapi": "mapi/mapi", "mapi32": "mapi/mapi32", "cmc": "mapi/cmc",
    "setupx": "setup/setupx", "inst16": "setup/inst16", "finstall": "setup/finstall",
    "netdi": "setup/netdi", "cfgmgr32": "setup/cfgmgr32",
    "sol": "games/sol", "winmine": "games/winmine", "freecell": "games/freecell",
    "mshearts": "games/mshearts",
}
PREFIX = [
    ("mci", "mmedia"), ("msacm", "mmedia"), ("acm", "mmedia"), ("vidc", "mmedia"),
    ("ras", "comm"), ("nw", "comm"), ("ndis", "comm"), ("ipx", "comm"), ("spx", "comm"),
    ("wsock", "comm"), ("winsock", "comm"), ("ws2", "comm"), ("inet", "comm"), ("wininet", "comm"),
    ("ndde", "comm"), ("netdde", "comm"), ("snmp", "comm"), ("mstcp", "comm"), ("tapi", "comm"),
    ("unimodem", "comm"), ("modem", "comm"), ("vredir", "comm"), ("vnet", "comm"),
    ("mapi", "mapi"), ("ole", "ole"), ("mfc", "ole"), ("setup", "setup"), ("cfgmgr", "setup"),
    ("unidrv", "print"), ("pscript", "print"), ("rasdd", "print"), ("dmcolor", "print"),
    ("hp", "print"), ("epson", "print"), ("canon", "print"), ("oki", "print"), ("lexmark", "print"),
    ("ibmppds", "print"), ("paintjt", "print"), ("proprint", "print"), ("fujixl", "print"),
]


def tree_root():
    """The one authored source tree: the repo root beside the corpus (Windows95\\)."""
    return os.environ.get("WT_TREE_ROOT") or os.path.dirname(paths.design_root())


def _suffix_component(n):
    if n.endswith((".vxd", ".386", ".pdr", ".mpd")):
        return "vmm32"
    if n.endswith(".acm"):
        return "mmedia"
    if n.endswith(".cpl"):
        return "setup"
    if n.endswith(".scr"):
        return "shell"
    if n.endswith(".drv"):
        return "drivers"
    return None


def _install_bucket(path):
    p = (path or "").replace("\\", "/").lower()
    if "/iosubsys/" in p:
        return "vmm32"
    if "/command/" in p:
        return "msdos"
    return None


_existing = None


def _existing_placements(root):
    """module-name (lower, no ext) -> 'component/leaf' for every module dir already in the tree."""
    global _existing
    if _existing is not None:
        return _existing
    placed = {}
    for comp in COMPONENTS:
        base = os.path.join(root, comp)
        if not os.path.isdir(base):
            continue
        for dp, dns, fns in os.walk(base):
            if "legacy" in dp.replace("\\", "/").split("/"):
                dns[:] = []
                continue
            fl = {f.lower() for f in fns}
            if not ("status.md" in fl or "scope.md" in fl or "placement.md" in fl
                    or any(f.endswith(".def") for f in fl) or "makefile" in fl):
                continue
            rel = os.path.relpath(dp, root).replace(os.sep, "/")
            mod = os.path.basename(dp).lower()
            for f in fns:
                if f.lower().endswith(".def"):
                    try:
                        m = re.search(r"^\s*(?:LIBRARY|NAME)\s+(\w+)",
                                      open(os.path.join(dp, f), encoding="utf-8", errors="replace").read(),
                                      re.M | re.I)
                        if m:
                            mod = m.group(1).lower()
                    except OSError:
                        pass
                    break
            placed.setdefault(mod, rel)
    _existing = placed
    return placed


def resolve(module, fmt="", path="", root=None):
    """Return ('component/leaf', confidence). Existing placement wins (authoritative)."""
    root = root or tree_root()
    n = module.lower()
    stem = re.sub(r"\.[^.]+$", "", n)
    cur = _existing_placements(root).get(stem) or _existing_placements(root).get(n)
    if cur:
        return cur, "placed"
    ib = _install_bucket(path)
    if ib:
        return "%s/%s" % (ib, stem), "high"
    if fmt in ("LE", "LX"):
        return "vmm32/%s" % stem, "high"
    sfx = _suffix_component(n)
    if sfx:
        return "%s/%s" % (sfx, stem), "med"
    if stem in KNOWN:
        return KNOWN[stem], "high"
    for pre, comp in PREFIX:
        if stem.startswith(pre):
            return "%s/%s" % (comp, stem), "med"
    return "unsorted/%s" % stem, "low"


def _archive_legacy(target_dir):
    """Move whatever is already in target_dir into target_dir/legacy/ -- ONCE. If a legacy/ already
    exists (a prior run archived the original), leave it and just let the caller overwrite the
    primary, so the original snapshot is preserved. Returns the count moved (0 if nothing/again)."""
    legacy = os.path.join(target_dir, "legacy")
    entries = [e for e in os.listdir(target_dir) if e != "legacy"]
    if not entries or os.path.isdir(legacy):
        return 0
    os.makedirs(legacy, exist_ok=True)
    for e in entries:
        shutil.move(os.path.join(target_dir, e), os.path.join(legacy, e))
    return len(entries)


def place(module, files, fmt="", path="", lane="", root=None, dry=False):
    """Place reconstructed source for `module` into the authored tree.

    `files` maps filename -> text content (e.g. {'shlwapi.c': '...'}). Archives any existing code
    in the module's folder into legacy/ (once), then writes the fresh files + a PLACEMENT.md.
    Returns a result dict: {module, place, confidence, archived, written, dir}.
    """
    root = root or tree_root()
    comp, conf = resolve(module, fmt=fmt, path=path, root=root)
    target_dir = os.path.join(root, *comp.split("/"))
    stem = comp.split("/")[-1]
    res = {"module": module, "place": comp, "confidence": conf, "archived": 0,
           "written": list(files), "dir": target_dir}
    if dry:
        return res
    os.makedirs(target_dir, exist_ok=True)
    res["archived"] = _archive_legacy(target_dir)
    for fname, text in files.items():
        with open(os.path.join(target_dir, fname), "w", encoding="utf-8") as f:
            f.write(text)
    with open(os.path.join(target_dir, "PLACEMENT.md"), "w", encoding="utf-8") as f:
        f.write("# %s\n\n- component: `%s`\n- format: %s\n- confidence: **%s**\n- source lane: %s\n"
                "- files: %s\n- note: reconstructed + placed by CodeBreaker; prior contents (if any) in `legacy/`.\n"
                % (module, comp.upper(), fmt or "?", conf, lane or "?", ", ".join(sorted(files))))
    return res
