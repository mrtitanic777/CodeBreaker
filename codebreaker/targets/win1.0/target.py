#!/usr/bin/env python3
"""
target.py — what "Windows 1.0" means to the CodeBreaker orchestration
(conduct/asmrecon/segdiff), the win1.0 profile of the unified workbench.

Windows 1.0 is the ancestor case the win95 target.py's own lineage note names:
"one concatenated boot image (WIN100.BIN + .OVL), all NE." Two packagings:

  * **apps** — ordinary standalone NE `.EXE`s in `files/WF/` (CLOCK, CALC, …).
    These resolve to a real path, exactly what conduct's `ne.NEFile(resolve())`
    wants — so the conductor drives them with no bundle-carve.
  * **system modules** (KERNEL/GDI/USER/SYSTEM/KEYBOARD/MOUSE/DISPLAY/SOUND/COMM)
    — carved from the WIN100.BIN/OVL bundle, they have no standalone path. They
    are ALREADY 100% byte-exact (whole-module masmlift), so the conductor's
    path-based opener is deferred for them (see resolve() below); when Stage 1
    wants the per-function %-live ledger for them, open via cfg.open_any (which
    returns an in-memory NE) through a small opener shim.

Everything downstream (objextract, segdiff, the db-fallback) is format-identical
to the sibling projects because MASM emits the same OMF `.OBJ`.
"""
import glob
import os
import struct

FILES = os.path.join("files", "WF")

# The 9 hand-asm system modules live INSIDE WIN100.BIN/OVL (no standalone file).
SYSMODS = {"KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD", "MOUSE",
           "DISPLAY", "SOUND", "COMM"}


def format_of(path):
    """'NE' | 'MZ' | None. Reads only the two headers (win1.0 is NE-only)."""
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
    return {b"NE": "NE"}.get(sig, "MZ")


def resolve(name):
    """Accept 'CLOCK', 'CLOCK.EXE', or a path; return the app's path under
    files/WF/. System modules are bundled (no path) — raise so a caller that
    truly needs a standalone file fails loudly instead of silently missing."""
    if os.path.sep in name or name.startswith(FILES):
        return name
    base = os.path.basename(name)
    up = base.upper()
    if up in SYSMODS:
        raise NotImplementedError(
            "%s is a WIN100.BIN-bundled system module (no standalone path). "
            "It is already byte-exact; open via cfg.open_any when Stage 1 needs "
            "its per-function ledger." % up)
    cand = os.path.join(FILES, base)
    if os.path.exists(cand):
        return cand
    for ext in (".EXE", ".exe"):
        cand = os.path.join(FILES, up + ext)
        if os.path.exists(cand):
            return cand
        cand = os.path.join(FILES, base.lower() + ext)
        if os.path.exists(cand):
            return cand
    raise ValueError("no module matching %r under %s/" % (name, FILES))


def all_apps():
    """Every standalone NE app under files/WF/, as paths."""
    out = []
    for p in sorted(glob.glob(os.path.join(FILES, "*.EXE"))):
        if os.path.isfile(p) and format_of(p) == "NE":
            out.append(p)
    return out


def code_bytes(path):
    """Code size in bytes (sum of CODE segment lengths) via win1.0's NE loader."""
    try:
        from codebreaker.format_loaders import ne as ne_mod
        ne = ne_mod.NEFile(path)
        return sum(s["length"] for s in ne.segments() if s.get("kind") == "CODE")
    except Exception:
        return 0


def tiers():
    """{tier: [paths]} — win1.0 has one conductable tier over the standalone
    apps (system modules are bundled + already byte-exact)."""
    return {"apps": [p for p in all_apps() if code_bytes(p) > 0]}


TIER_ORDER = ["apps"]


if __name__ == "__main__":
    t = tiers()
    print("%-8s %8s %14s" % ("tier", "modules", "code bytes"))
    for name in TIER_ORDER:
        paths = t.get(name, [])
        b = sum(code_bytes(p) for p in paths)
        print("  %-6s %8d %14d" % (name, len(paths), b))
        for p in paths:
            print("      %-16s %8d b" % (os.path.basename(p), code_bytes(p)))
