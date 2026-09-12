#!/usr/bin/env python3
r"""fieldname.py -- name struct fields from SDK layouts (p->f_4 -> p->hwnd).

The struct/gptr engines recover a pointer's field OFFSETS but emit them anonymously (`p->f_4`).
symdb holds 1,126 SDK struct layouts as ordered (type, field) lists. This computes each layout's
field offsets (from C type sizes, natural alignment) and, given a set of accessed offsets, either
names them against a KNOWN struct (from a recovered pointer type) or finds the best-fitting SDK
struct by offset coverage -- turning `p->f_4` into `p->hwnd`, `p->f_0` into `p->cbSize`.
"""
import re
from src.engines.symbol import symdb

_SIZE = {"BYTE": 1, "CHAR": 1, "char": 1, "UCHAR": 1, "BOOLEAN": 1,
         "WORD": 2, "SHORT": 2, "USHORT": 2, "WCHAR": 2, "ATOM": 2,
         "DWORD": 4, "LONG": 4, "ULONG": 4, "INT": 4, "int": 4, "UINT": 4, "BOOL": 4,
         "FLOAT": 4, "COLORREF": 4, "LPARAM": 4, "WPARAM": 4, "LRESULT": 4, "HRESULT": 4,
         "LARGE_INTEGER": 8, "double": 8, "DWORDLONG": 8, "__int64": 8}


def _tsize(t):
    t = t.strip()
    if t.endswith("*") or t.startswith(("LP", "P", "H")) or t in ("HANDLE", "HWND", "HDC", "HMENU"):
        return 4                                             # any pointer / handle
    return _SIZE.get(t, 4)                                   # default a machine word


def offsets_of(name):
    """{field_name: offset} for an SDK struct, computing natural-aligned offsets. None if unknown."""
    layout = symdb.struct(name)
    if not layout:
        return None
    out = {}; cur = 0
    for typ, field in layout:
        m = re.match(r"(\w+)\s*\[\s*(\d+)\s*\]", field)       # arrays: name[N]
        cnt = int(m.group(2)) if m else 1
        fname = m.group(1) if m else field
        sz = _tsize(typ)
        al = min(sz, 4)
        cur = (cur + al - 1) & ~(al - 1)
        out[fname] = cur
        cur += sz * cnt
    return out


def name_fields(struct_name, offsets):
    """{offset: field_name} mapping accessed offsets onto a known SDK struct's fields."""
    lay = offsets_of(struct_name)
    if not lay:
        return {}
    by_off = {}
    for f, o in lay.items():
        by_off.setdefault(o, f)
    out = {}
    for o in offsets:
        if o in by_off:
            out[o] = by_off[o]
    return out


def best_match(offsets, min_cover=0.6):
    """(struct_name, {offset:field}) whose layout best covers the accessed offsets, or None."""
    offs = set(offsets)
    if not offs:
        return None
    best = None
    for name in symdb._load()["struct"]:
        lay = offsets_of(name)
        if not lay:
            continue
        layoffs = set(lay.values())
        hit = len(offs & layoffs)
        if hit and hit / len(offs) >= min_cover:
            score = (hit, -len(layoffs))                     # cover the accesses, prefer a tight struct
            if best is None or score > best[0]:
                best = (score, name)
    if not best:
        return None
    name = best[1]
    return name, name_fields(name, offs)
