#!/usr/bin/env python3
r"""win16ord.py -- resolve Win16 API ordinal imports to names (USER/GDI/KERNEL and friends).

The core Win16 DLLs -- USER, GDI, KRNL386/KERNEL, plus KEYBOARD, SOUND, COMMDLG, ... -- export BY
ORDINAL, so every Win16 call in a 16-bit NE module disassembles to `USER.1` / `GDI.68` with no
name. The ordinal->name mapping is ground truth: the shipped modules' export .def files list
`<NAME> @ <ordinal>` for every entry. This ingests those tables and resolves an NE module's
IMPORTORDINAL relocations (module-reference + ordinal, via formats.ne) to the real API name -- so a
whole 16-bit binary's Win16 calls become `MESSAGEBOX`, `CREATEWINDOW`, `TEXTOUT` at once. No
fabricated data: an ordinal with no table entry stays `MODULE.N`.
"""
import os
import re

_REG = {}                                                    # {MODULE: {ordinal: name}}
_ALIAS = {"KERNEL": "KRNL386", "KRNL286": "KRNL386"}         # KERNEL is KRNL386 in Win95
_DEF = re.compile(r"^\s*(\S+)\s+@\s*(\d+)")


def parse_def(path):
    """Parse a Win16 export .def -> {ordinal: NAME}. Lines are `<NAME> @ <ord>`."""
    table, in_exports = {}, False
    with open(path, "r", errors="replace") as f:
        for line in f:
            s = line.split(";", 1)[0].strip()
            if not s:
                continue
            up = s.upper()
            if up.startswith("EXPORTS"):
                in_exports = True
                s = s[7:].strip()
            if up.split()[0] in ("LIBRARY", "EXETYPE", "DESCRIPTION", "STUB", "HEAPSIZE",
                                 "STACKSIZE", "CODE", "DATA", "SEGMENTS", "IMPORTS"):
                continue
            m = _DEF.match(s)
            if m:
                table[int(m.group(2))] = m.group(1)
    return table


def register(module, table):
    _REG[module.upper()] = table


def load_def(path, module=None):
    module = module or os.path.splitext(os.path.basename(path))[0]
    register(module, parse_def(path))
    return _REG[module.upper()]


def load_defs(directory):
    """Load every *.def under a directory tree (module name = file stem)."""
    n = 0
    for dp, _dn, fns in os.walk(directory):
        for fn in fns:
            if fn.lower().endswith(".def"):
                try:
                    load_def(os.path.join(dp, fn))
                    n += 1
                except Exception:
                    pass
    return n


def name_for(module, ordinal):
    """The Win16 API name for (module, ordinal), or None if not in a loaded table."""
    m = module.upper()
    m = _ALIAS.get(m, m)
    t = _REG.get(m) or _REG.get(module.upper())
    return t.get(ordinal) if t else None


def resolve_ne_imports(ne_path):
    """{(seg, offset): (module, ordinal, name)} for a 16-bit NE module's IMPORTORDINAL fixups."""
    from codebreaker.formats.ne import NEFile
    ne = NEFile(ne_path)
    modrefs = ne.module_refs()
    out = {}
    for seg in ne.segments():
        for r in ne.relocations(seg):
            if r.get("kind") != "IMPORTORD":
                continue
            mi = r["module"]
            mod = modrefs[mi - 1] if 1 <= mi <= len(modrefs) else "MOD%d" % mi
            nm = name_for(mod, r["ordinal"])
            out[(seg["idx"], r["offset"])] = (mod, r["ordinal"], nm or "%s.%d" % (mod, r["ordinal"]))
    return out


def reconstruct(ne_path):
    """Summarize the Win16 imports of an NE module, named. {'ok','nresolved','ntotal','by_module'}."""
    try:
        res = resolve_ne_imports(ne_path)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not res:
        return {"ok": False, "reason": "no IMPORTORDINAL fixups (not a Win16 NE, or none)"}
    by_module, named = {}, 0
    for (_s, _o), (mod, ordn, nm) in res.items():
        by_module.setdefault(mod.upper(), set()).add((ordn, nm))
        if not nm.startswith("%s." % mod):
            named += 1
    return {"ok": True, "ntotal": len(res), "nresolved": named,
            "by_module": {m: sorted(v) for m, v in by_module.items()}}
