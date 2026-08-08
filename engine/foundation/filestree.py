#!/usr/bin/env python3
r"""filestree.py — organize the flat files/ corpus into a real Win95 INSTALL TREE.

The corpus lands FLAT: files/<name> for all ~2375 shipped Windows 95 files (DLLs, VxDs,
drivers, fonts, help, media, cursors, INFs, data…). That's a carve dump, not an install.
A real Windows 95 machine lays these out BY PLACE — WINDOWS\SYSTEM for the binaries,
WINDOWS\SYSTEM\IOSUBSYS for the disk stack, WINDOWS\FONTS, WINDOWS\HELP, WINDOWS\MEDIA,
WINDOWS\CURSORS, WINDOWS\INF, WINDOWS\COMMAND for the DOS utils, and loose data at the
WINDOWS root. This tool assigns every flat file an install path and moves it into shape.

    wt --target win95 filestree --dry-run     # show where each file would go (no move)
    wt --target win95 filestree --manifest    # write files/FILESTREE.map (name -> path)
    wt --target win95 filestree --fallbacks    # just the guessed remainder to eyeball
    wt --target win95 filestree --apply        # MOVE files into files/WINDOWS/…
    wt --target win95 filestree --unapply      # flatten back to files/ (undo --apply)

files/ is GITIGNORED (copyrighted MS corpus) — moves are NOT git-tracked, so there is no
git safety net. Reversibility comes from this tool: FILESTREE.map is the authored record,
and --apply/--unapply are exact inverses (idempotent os.rename; --unapply walks the tree
and moves every basename back to the root). The module resolver (target._files_index) is
already tree-aware — it finds a file at ANY depth — so every module still resolves after
the move without any resolver change.

Classification is deterministic and ordered, mirroring srctree.py:
    installtree known-location override  >  IOSUBSYS/COMMAND filename heuristics  >
    extension map  >  honest header-sniff fallback (SYSTEM for unknown binaries,
    WINDOWS root for unknown data) — fallbacks are always LOGGED, never hidden.

The pre-existing files/setup/ subtree (installer INFs + media, already nested) is PRESERVED
untouched: never scanned for a move, never disturbed by --unapply.
"""
import os
from engine.foundation import cores
import re
import sys

FILES = "files"                       # corpus root (wt chdirs into win95/ before running)
MANIFEST = "FILESTREE.map"            # the authored path record, lives at files/ root
PRESERVE = {"setup"}                  # pre-existing nested subtrees — left exactly as-is

# ---------------------------------------------------------------------------
# 1. known-location override — code modules whose real home is NOT WINDOWS\SYSTEM
#    but \WINDOWS (shell / core apps), so the install-path view places them correctly.
# ---------------------------------------------------------------------------
def _seed_override():
    # code modules whose real install home is \WINDOWS rather than \WINDOWS\SYSTEM.
    return {"explorer.exe": "WINDOWS", "notepad.exe": "WINDOWS", "regedit.exe": "WINDOWS",
            "winhelp.exe": "WINDOWS", "write.exe": "WINDOWS", "command.com": "WINDOWS/COMMAND"}

OVERRIDE = _seed_override()

# ---------------------------------------------------------------------------
# 2a. IOSUBSYS — the disk / CD-ROM / SCSI I/O subsystem. *.mpd and *.pdr are its
#     minidrivers by extension; a subset of *.vxd are its layered block drivers.
#     Matched ANCHORED on the whole base name so bios/ebios/biosxlat/vnetbios
#     (which merely CONTAIN "ios") are NOT swept in — those stay in SYSTEM.
# ---------------------------------------------------------------------------
IOSUBSYS_VXD = re.compile(
    r"^(ios|int13|cdfs|cdtsd|cdvsd|diskmgmt|disktsd|diskvsd|drvspacx|necatapi|"
    r"scsi1hlp|scsiport|vcdfsd|voltrack|apix|aspienum|torisan\d*|esdi\w*|"
    r"atapi\w*|aha\d\w*|ncr\w*|aic\w*|sparrow|mkecr\w*|dpt\w*)$", re.I)

# ---------------------------------------------------------------------------
# 2b. COMMAND — the WINDOWS\COMMAND DOS command-line utilities. *.com and *.bat
#     by extension; a curated set of well-known *.exe utils by name (kept tight so
#     GUI look-alikes like SYSEDIT/SYSTRAY/SYSMON are NOT misfiled as DOS tools).
# ---------------------------------------------------------------------------
COMMAND_EXE = set("""
    attrib chkdsk debug deltree diskcopy doskey edit fc fdisk find format keyb
    label mem mode more move mscdex nlsfunc scandisk sort start subst sys xcopy
    xcopy32 choice cvt cvt1 drvspace dblspace emm386 himem ramdrive setver
    smartdrv country display ega append share restart lock unlock scandskw
""".split())

# ---------------------------------------------------------------------------
# 3. Extension -> install dir (only after overrides + IOSUBSYS/COMMAND heuristics).
# ---------------------------------------------------------------------------
EXT_FONTS   = {"fon", "ttf", "fot"}
EXT_HELP    = {"hlp", "cnt", "gid"}
EXT_MEDIA   = {"wav", "mid", "rmi", "avi"}
EXT_CURSORS = {"cur", "ani"}
EXT_INF     = {"inf"}
# binaries / system-resident modules and driver data — all install to WINDOWS\SYSTEM
EXT_SYSTEM  = {"dll", "exe", "drv", "386", "cpl", "ocx", "acm", "acv", "tsp", "flt",
               "scr", "kbd", "nls", "tlb", "vbx", "ax", "mod", "3gr", "drw", "spd",
               "dos", "nw4", "trn", "bin", "csp", "cpi", "mfm", "apd", "efx", "ckm",
               "uc", "sbk", "idf", "hpm", "mib", "dwn", "sys", "pci", "vxd"}
# loose user/data/config files — install to the WINDOWS root
EXT_DATA    = {"ini", "reg", "dat", "grp", "pif", "bmp", "ico", "txt", "gif", "htm",
               "html", "htr", "url", "cfg", "scp", "dun", "cpe", "doc", "xls", "ppt",
               "pre", "shw", "wpd", "wpc", "wpg", "wpf", "wpw", "wk4", "wb2", "sam",
               "ht", "cdf", "cnf", "adm", "prf", "pat", "msg", "new", "ins", "isp",
               "icw", "msn", "evy", "rat", "tsk", "tpl", "frm", "ned", "pbk", "vbp",
               "ms", "zip", "jpg", "wk1", "wks", "wr1", "ccp"}

SYSTEM   = "WINDOWS/SYSTEM"
IOSUBSYS = "WINDOWS/SYSTEM/IOSUBSYS"
COLOR    = "WINDOWS/SYSTEM/COLOR"
COMMAND  = "WINDOWS/COMMAND"
ROOT     = "WINDOWS"

_BIN_FMTS = ("NE", "PE", "LE", "LX", "MZ")


def _split(name):
    """('kernel32', 'dll') from 'kernel32.dll'  |  ('networks', '') for no-extension."""
    b = name.lower()
    if "." in b:
        stem, ext = b.rsplit(".", 1)
        return stem, ext
    return b, ""


def _sniff(path):
    """Header format of `path` ('NE'|'PE'|'LE'|'LX'|'MZ'|None), via target.format_of.
    Used only for the honest last-resort fallback (unknown extension): a real binary
    goes to SYSTEM, anything else is loose data at the WINDOWS root."""
    try:
        target = cores.get('target')
        return target.format_of(path)
    except Exception:
        return None


def classify(name, path=None):
    """(install_dir, reason) for a flat file name. Ordered exactly like the header
    docstring. `path` (the file's current location) is only consulted for the
    fallback header sniff on an unrecognized extension."""
    base, ext = _split(name)
    # 1. installtree known-location override (code modules with a non-SYSTEM home)
    if name.lower() in OVERRIDE:
        return OVERRIDE[name.lower()], "installtree"
    # 2a. IOSUBSYS disk subsystem — before the generic .vxd extension rule
    if ext in ("mpd", "pdr"):
        return IOSUBSYS, "ext:%s" % ext
    if ext == "vxd" and IOSUBSYS_VXD.match(base):
        return IOSUBSYS, "iosubsys-vxd"
    # 2b. COMMAND DOS utilities — before the generic .exe extension rule
    if ext in ("com", "bat"):
        return COMMAND, "ext:%s" % ext
    if ext == "exe" and base in COMMAND_EXE:
        return COMMAND, "command-util"
    # 3. extension map
    if ext in EXT_FONTS:
        return "WINDOWS/FONTS", "ext:%s" % ext
    if ext in EXT_HELP:
        return "WINDOWS/HELP", "ext:%s" % ext
    if ext in EXT_MEDIA:
        return "WINDOWS/MEDIA", "ext:%s" % ext
    if ext in EXT_CURSORS:
        return "WINDOWS/CURSORS", "ext:%s" % ext
    if ext in EXT_INF:
        return "WINDOWS/INF", "ext:%s" % ext
    if ext == "icm":
        return COLOR, "ext:icm"
    if ext in EXT_SYSTEM:
        return SYSTEM, "ext:%s" % ext
    if ext in EXT_DATA:
        return ROOT, "ext:%s" % ext
    # 4. honest fallback — sniff the header: a binary is SYSTEM, everything else is
    #    loose data at the WINDOWS root. Always logged (reason starts "fallback").
    fmt = _sniff(path) if path else None
    if fmt in _BIN_FMTS:
        return SYSTEM, "fallback:bin(%s)" % fmt
    return ROOT, "fallback:data"


# ---------------------------------------------------------------------------
# Scanning / manifest
# ---------------------------------------------------------------------------
def _flat_files():
    """Sorted [name] of every file directly at files/ root (the movable set) —
    excluding the manifest itself. The pre-existing setup/ subtree is a DIR, not a
    flat file, so it is naturally excluded."""
    if not os.path.isdir(FILES):
        return []
    return sorted(fn for fn in os.listdir(FILES)
                  if os.path.isfile(os.path.join(FILES, fn)) and fn != MANIFEST)


def _scan():
    """[(name, install_dir, reason)] for every flat file, deterministically ordered."""
    out = []
    for fn in _flat_files():
        comp, reason = classify(fn, os.path.join(FILES, fn))
        out.append((fn, comp, reason))
    return out


# ---------------------------------------------------------------------------
# Move / undo — idempotent os.rename, FILESTREE.map as the authored record.
# ---------------------------------------------------------------------------
def _prune_empty(root):
    for dp, _dns, _fns in os.walk(root, topdown=False):
        if os.path.abspath(dp) == os.path.abspath(root):
            continue
        rel = os.path.relpath(dp, root).replace("\\", "/").split("/")[0]
        if rel in PRESERVE:                          # never prune a preserved subtree
            continue
        if not os.listdir(dp):
            os.rmdir(dp)


def apply_tree():
    """Move each flat file into files/<install_dir>/<name> per the classification.
    Idempotent: a file already gone (moved) is skipped."""
    rows = _scan()
    moved = 0
    for fn, comp, _reason in rows:
        src = os.path.join(FILES, fn)
        if not os.path.isfile(src):                  # already moved / absent
            continue
        dst_dir = os.path.join(FILES, comp)
        os.makedirs(dst_dir, exist_ok=True)
        os.rename(src, os.path.join(dst_dir, fn))
        moved += 1
    print("moved %d files into files/WINDOWS/…  (%d preserved subtree(s): %s)"
          % (moved, len(PRESERVE), ", ".join(sorted(PRESERVE))))


def unapply_tree():
    """Flatten the install tree back: walk files/ recursively (skipping preserved
    subtrees), move every nested basename back to files/ root, prune the emptied dirs.
    The exact inverse of --apply; the manifest and root-level files are left in place."""
    moved = 0
    for dp, dns, fns in os.walk(FILES, topdown=True):
        if os.path.abspath(dp) == os.path.abspath(FILES):
            dns[:] = [d for d in dns if d not in PRESERVE]   # don't descend into setup/
            continue                                          # root files stay put
        for fn in fns:
            dst = os.path.join(FILES, fn)
            if os.path.exists(dst):
                print("  SKIP (name already at root): %s" % fn)
                continue
            os.rename(os.path.join(dp, fn), dst)
            moved += 1
    _prune_empty(FILES)
    print("flattened %d files back to files/ root" % moved)


def write_manifest(rows):
    path = os.path.join(FILES, MANIFEST)
    with open(path, "w", encoding="utf-8") as f:
        f.write("# flat file -> install path under files/  (generated by filestree.py)\n")
        f.write("# authored reversibility record; --apply/--unapply are exact inverses\n")
        f.write("# filename\tinstall_path\treason\n")
        for fn, comp, reason in rows:
            f.write("%s\t%s/%s\t%s\n" % (fn, comp, fn, reason))
    print("wrote %s  (%d entries)" % (os.path.join(FILES, MANIFEST), len(rows)))


def main():
    args = set(sys.argv[1:]) or {"--dry-run"}
    if "--apply" in args:
        apply_tree(); return
    if "--unapply" in args:
        unapply_tree(); return

    rows = _scan()

    if "--manifest" in args:
        write_manifest(rows); return

    from collections import Counter
    bydir = Counter(c for _f, c, _r in rows)
    fallbacks = [(f, c, r) for f, c, r in rows if r.startswith("fallback")]

    if "--fallbacks" in args:
        print("FALLBACKS (%d) — guessed by header sniff, review these:\n" % len(fallbacks))
        for fn, comp, reason in fallbacks:
            print("  %-28s -> %-22s (%s)" % (fn, comp, reason))
        return

    # default: --dry-run distribution
    print("=" * 64)
    print("files/ install-tree classification  (%d flat files)" % len(rows))
    print("=" * 64)
    for comp in sorted(bydir, key=lambda c: (-bydir[c], c)):
        print("  %-26s %5d" % (comp, bydir[comp]))
    print("-" * 64)
    print("  fallbacks: %d  (run --fallbacks to review)" % len(fallbacks))
    print("  preserved untouched: %s" % ", ".join("files/%s/" % p for p in sorted(PRESERVE)))
    print("  next: --manifest to write files/FILESTREE.map, then --apply")


if __name__ == "__main__":
    main()
