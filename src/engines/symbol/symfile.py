#!/usr/bin/env python3
r"""symfile.py -- load Microsoft's own Win9x `.SYM` (MAPSYM) symbol files as a naming + source-file
oracle.

The period SDK/DDK ship `.SYM` files (DEBUG/*.SYM) for the core modules -- USER, GDI, KRNL386,
SHELL32, OLE32, OLEAUT32, COMCTL32, the VxD stack (VMM/IOS/VPICD/...), and ~70 more. Each is the
MAPSYM output of the module's link `.MAP`: a list of SEGMENTS, each named after its source object
(`_MSGBOX`, `_MESSAGES`, `_WINUTIL`, `_DRAW`, ...), each carrying its symbols as
`{in-segment offset -> name}` -- real function names, INCLUDING internal ones.

So one `.SYM` gives three things the binary alone can't: real function names, the source-file
PARTITIONING (which function lived in which object), and the source-file NAMES (segment names).

FAITHFULNESS GATE: these SDK `.SYM` are RTM-era and the corpus is OSR2, so a name is only a
HYPOTHESIS until anchored. We align each `.SYM` segment to the real binary by its EXPORTS: for
names in both the `.SYM` and the module's export table, the implied segment base
(export_address - sym_offset) must agree; only then do we trust that segment's INTERNAL names,
mapping each `sym_offset -> real address`. Segments with no consistent export anchor are dropped
(never guessed).

    from src.engines.symbol import symfile
    nm = symfile.name_map("user.exe")     # {address -> real name}, export-verified
    segs = symfile.source_segments("user.exe")   # [(segname, [names...])] -- the source-file map
"""
import os
import glob
import struct

_CACHE = {}
_MAP_CACHE = {}
_SEG_MARKER16 = 0xFF00
_SEG_MARKER32 = 0xFF000000


# ---------------------------------------------------------------- locating a .sym
_ALIAS = {"KERNEL": "KRNL386", "KRNL386": "KRNL386", "KRNL286": "KRNL286"}


def find(module):
    """Locate the .SYM for a module name (e.g. 'user.exe', 'shell32.dll', 'VMM'). Searches the
    reference SDK/DDK DEBUG trees. Returns a path or None."""
    from src.core import paths
    base = os.path.basename(str(module)).upper()
    for ext in (".DLL", ".EXE", ".DRV", ".VXD", ".386"):
        if base.endswith(ext):
            base = base[:-len(ext)]; break
    base = _ALIAS.get(base, base)
    root = paths.design_root()
    hits = []
    for cand in (base, base + "32"):
        hits += glob.glob(os.path.join(root, "**", cand + ".SYM"), recursive=True)
        hits += glob.glob(os.path.join(root, "**", cand + ".sym"), recursive=True)
    if not hits:
        return None
    # The corpus is x86, but the SDK also ships PowerPC/Alpha/MIPS cross-build .sym (under SCT/
    # DEBUG16/<arch>/) whose layout + offsets are a DIFFERENT architecture -- reject those. Among the
    # x86 copies (incl. localized ARABIC/HEBREW builds, whose symbol names are identical) any is fine.
    _NONX86 = ("PPC", "ALPHA", "AXP", "MIPS", "R4000", "R4100")
    def _rank(p):
        u = p.upper()
        nonx86 = any(("\\%s\\" % a) in u or ("/%s/" % a) in u for a in _NONX86)
        return (nonx86, 0 if ("I386" in u or "X86" in u) else 1, len(p))
    hits.sort(key=_rank)
    # drop non-x86 entirely if any x86 copy exists
    x86 = [h for h in hits if not _rank(h)[0]]
    return (x86 or hits)[0]


# ---------------------------------------------------------------- parsing (MAPSYM)
def _name_at(d, i):
    if i >= len(d):
        return None
    n = d[i]
    if not (1 <= n <= 64) or i + 1 + n > len(d):
        return None
    s = d[i + 1:i + 1 + n]
    if not all(32 <= b < 127 for b in s):
        return None
    c = chr(s[0])
    if not (c.isalpha() or c == "_"):          # strict first char: breaks runs cleanly at SEGDEF filler
        return None
    return s.decode("latin1")


def _scan_run(d, i, width):
    recs = []
    while i + width + 1 < len(d):
        off = int.from_bytes(d[i:i + width], "little")
        nm = _name_at(d, i + width)
        if nm is None:
            break
        recs.append((off, nm))
        i += width + 1 + len(nm)
    return recs, i


def parse(path):
    """Parse a .SYM -> {'module', 'segments':[{'name','bits','symbols':[(off,name)]}]}.
    A record whose offset is the segment marker (0xFF00 / 0xFF000000) begins a new segment and
    names it; subsequent records are that segment's symbols at in-segment offsets."""
    if path in _MAP_CACHE:
        return _MAP_CACHE[path]
    d = open(path, "rb").read()
    module = d[0x10:0x10 + d[0x0f]].decode("latin1", "replace") if len(d) > 0x10 else "?"
    segments = []
    i = 0x10
    while i < len(d):
        # each maximal SYMDEF run is one segment; its leading marker record names the source object
        r16, e16 = _scan_run(d, i, 2)
        r32, e32 = _scan_run(d, i, 4)
        if len(r32) > len(r16) and len(r32) >= 3:
            recs, width, end = r32, 32, e32
        elif len(r16) >= 3:
            recs, width, end = r16, 16, e16
        else:
            i += 1
            continue
        # a run can hold several source-object segments; each begins with a MARKER record
        # (offset 0xFF00 at 16-bit / 0xFF000000 at 32-bit) whose name is the object/source name.
        def _is_marker(off):
            return off == _SEG_MARKER16 or off == _SEG_MARKER32 or (off >> 24) == 0xFF
        cur = {"name": "", "bits": width, "symbols": []}
        started = False
        for off, nm in recs:
            if _is_marker(off):
                if started or cur["symbols"]:
                    segments.append(cur)
                cur = {"name": nm, "bits": width, "symbols": []}
                started = True
            else:
                cur["symbols"].append((off, nm))
        segments.append(cur)
        i = end
    out = {"module": module, "segments": segments}
    _MAP_CACHE[path] = out
    return out


# ---------------------------------------------------------------- export-anchored alignment
def _pe_exports(module):
    """{NAME: rva} for a PE module, from its export table."""
    from src.core import cores
    from src.formats.pe import PEFile
    try:
        pe = PEFile(cores.get("target").resolve(module))
        out = {}
        for e in pe.exports():                       # (ordinal, name, rva) tuples
            if isinstance(e, (tuple, list)) and len(e) >= 3 and e[1]:
                out[str(e[1]).upper()] = e[2]
            elif isinstance(e, dict):
                nm = (e.get("name") or "").upper(); rva = e.get("rva") or e.get("address")
                if nm and rva:
                    out[nm] = rva
        return out
    except Exception:
        return {}


def _ne_exports(module):
    """{NAME: (seg, offset)} for an NE module, from its entry/name tables."""
    from src.core import cores
    from src.formats.ne import NEFile
    try:
        nef = NEFile(cores.get("target").resolve(module) if not os.path.isabs(str(module)) else module)
        ents = {e["ordinal"]: (e.get("seg"), e.get("offset")) for e in nef.entries()
                if e.get("seg") is not None}
        out = {}
        for nm, o in list(nef.resident_names()) + list(nef.nonresident_names()):
            if o in ents and nm:
                out[nm.upper()] = ents[o]
        return out
    except Exception:
        return {}


def _fmt(module):
    from src.core import cores
    try:
        from src.targets.win95 import target  # optional
    except Exception:
        target = None
    try:
        p = cores.get("target").resolve(module)
        from src.formats.pe import PEFile
        from src.formats.ne import NEFile
        with open(p, "rb") as f:
            head = f.read(2)
        # quick: try NE then PE
        try:
            NEFile(p); return "NE", p
        except Exception:
            pass
        PEFile(p); return "PE", p
    except Exception:
        return None, None


def name_map(module):
    """{address -> real name}, export-anchored. For PE the address is an RVA; for NE it is the
    packed (seg<<16)|offset. Only segments with a consistent export anchor contribute (faithful)."""
    key = str(module).upper()
    if key in _CACHE:
        return _CACHE[key]
    out = {}
    sp = find(module)
    if not sp:
        _CACHE[key] = out
        return out
    fmt, _p = _fmt(module)
    sym = parse(sp)
    if fmt == "PE":
        exp = _pe_exports(module)
        if exp:
            for seg in sym["segments"]:
                names = seg["symbols"]
                anchors = [(exp[nm.upper()], off) for off, nm in names if nm.upper() in exp]
                if not anchors:
                    continue
                bases = [rva - off for rva, off in anchors]
                base = max(set(bases), key=bases.count)               # modal base
                if bases.count(base) < max(1, len(anchors) // 2):     # anchors must agree
                    continue
                for off, nm in names:
                    out[base + off] = nm
    elif fmt == "NE":
        exp = _ne_exports(module)    # {NAME:(seg,off)}
        if exp:
            # map each .sym segment (in order) to the NE segment its exports land in
            for seg in sym["segments"]:
                names = seg["symbols"]
                segvotes = {}
                for off, nm in names:
                    t = exp.get(nm.upper())
                    if t and t[1] == off:            # NE in-segment offset matches the .sym offset
                        segvotes[t[0]] = segvotes.get(t[0], 0) + 1
                if not segvotes:
                    continue
                ne_seg = max(segvotes, key=segvotes.get)
                for off, nm in names:
                    out[(ne_seg << 16) | (off & 0xFFFF)] = nm
    _CACHE[key] = out
    return out


def source_segments(module):
    """[(segment/source-file name, [symbol names])] -- the source-file partitioning for the tree."""
    sp = find(module)
    if not sp:
        return []
    return [(s["name"], [nm for _o, nm in s["symbols"]]) for s in parse(sp)["segments"] if s["symbols"]]


def coverage(module):
    sp = find(module)
    if not sp:
        return {"sym": None}
    s = parse(sp)
    nm = name_map(module)
    return {"sym": os.path.basename(sp), "segments": len(s["segments"]),
            "symbols": sum(len(x["symbols"]) for x in s["segments"]), "anchored": len(nm)}


def main():
    import sys
    from src.core import cores
    cores.set_target(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.dirname(os.path.abspath(__file__))))), "src", "targets", "win95"))
    for m in sys.argv[1:] or ["user.exe", "gdi.exe"]:
        print(m, "->", coverage(m))


if __name__ == "__main__":
    main()
