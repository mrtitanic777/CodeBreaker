#!/usr/bin/env python3
r"""constants.py -- flag / enum recovery: render a magic constant as its SDK name.

A call argument that is a raw constant is often an SDK flag or enum: `LocalAlloc(0x40, ...)` is
`LocalAlloc(LMEM_ZEROINIT, ...)`, `MessageBoxA(..., 0x24)` is `MB_YESNO | MB_ICONQUESTION`. This
parses the SDK headers' `#define NAME value` constants, groups them by family prefix, and -- for
the flag arguments of common APIs (a curated api/arg -> family map, for precision) -- decomposes
the value into that family's named flag bits. High precision: only render when the value is fully
covered by one family's constants; otherwise leave the hex.
"""
import os
import re

_INC = ("WIN32SDK", "MSTOOLS", "INCLUDE")   # inside the reference SDK tree (paths.reference)
_HEADERS = ("WINBASE.H", "WINUSER.H", "WINNT.H", "WINGDI.H", "WINREG.H", "WINNLS.H", "COMMDLG.H")

# (callee lowercased, arg index) -> family prefix whose flags this argument is drawn from.
_FAMILY = {
    ("localalloc", 0): "LMEM_", ("localreptr", 1): "LMEM_",
    ("globalalloc", 0): "GMEM_", ("globalflags", 0): "GMEM_",
    ("heapalloc", 1): "HEAP_", ("heapfree", 1): "HEAP_", ("heaprealloc", 1): "HEAP_",
    ("messageboxa", 3): "MB_", ("messageboxw", 3): "MB_", ("messageboxexa", 3): "MB_",
    ("createwindowexa", 0): "WS_EX_", ("createwindowexw", 0): "WS_EX_",
    ("showwindow", 1): "SW_",
    ("virtualalloc", 2): "MEM_", ("virtualalloc", 3): "PAGE_",
    ("virtualprotect", 2): "PAGE_", ("createfilemappinga", 2): "PAGE_",
    ("mapviewoffile", 1): "FILE_MAP_",
    ("createfilea", 1): "GENERIC_", ("createfilew", 1): "GENERIC_",
    ("createfilea", 6): "FILE_ATTRIBUTE_", ("createfilew", 6): "FILE_ATTRIBUTE_",
    ("setwindowpos", 6): "SWP_", ("openprocess", 0): "PROCESS_",
    ("regopenkeyexa", 3): "KEY_", ("regcreatekeyexa", 4): "REG_OPTION_",
    ("formatmessagea", 0): "FORMAT_MESSAGE_",
}

_DB = None


def _load():
    """{name: value} and {prefix: {name: value}} parsed from the SDK headers, cached."""
    global _DB
    if _DB is not None:
        return _DB
    from codebreaker.core import paths
    names = {}
    for h in _HEADERS:
        p = paths.reference(*_INC, h)
        try:
            txt = open(p, encoding="latin1").read()
        except OSError:
            continue
        for m in re.finditer(r"^#define\s+([A-Z][A-Z0-9_]+)\s+(0x[0-9A-Fa-f]+|\d+)L?\b", txt, re.M):
            nm, val = m.group(1), m.group(2)
            try:
                names[nm] = int(val, 0)
            except ValueError:
                pass
    fam = {}
    for nm, v in names.items():
        pre = nm[:nm.index("_") + 1] if "_" in nm else nm
        # index by every prefix segment so WS_EX_ and WS_ both resolve
        parts = nm.split("_")
        for i in range(1, len(parts)):
            fam.setdefault("_".join(parts[:i]) + "_", {})[nm] = v
    _DB = {"names": names, "fam": fam}
    return _DB


def _decompose(value, family):
    """Render `value` as an OR of `family`'s flag names if fully covered, else None."""
    db = _load()
    fl = db["fam"].get(family)
    if not fl:
        return None
    if value == 0:
        z = [n for n, v in fl.items() if v == 0]
        return z[0] if z else None
    # exact single-constant match first
    exact = [n for n, v in fl.items() if v == value]
    if exact:
        return min(exact, key=len)
    # greedy bit decomposition using single-or-multi-bit family flags, largest first
    remaining = value
    picked = []
    for n, v in sorted(fl.items(), key=lambda kv: -kv[1]):
        if v and (remaining & v) == v:
            picked.append((v, n)); remaining &= ~v
        if remaining == 0:
            break
    if remaining == 0 and picked:
        return " | ".join(n for _v, n in sorted(picked, reverse=True))
    return None


def render(callee, argpos, value):
    """Named SDK flag/enum for `value` at (callee, argpos), or None if not a known flag argument."""
    if callee is None or value is None:
        return None
    fam = _FAMILY.get((callee.lower(), argpos))
    if not fam:
        return None
    return _decompose(value & 0xFFFFFFFF, fam)
