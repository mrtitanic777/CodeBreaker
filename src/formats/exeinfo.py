#!/usr/bin/env python3
"""exeinfo.py -- one inspector for every Win95 executable format. Detects the format
and returns a structured dict at the same depth peinfo gives PE, for NE (16-bit
Win16), LE/LX (VxD), and plain DOS/MZ. PE dispatches to peinfo; the others are
parsed here.

    from src.formats import exeinfo
    info = exeinfo.parse(path)     # {"format": "PE"|"NE"|"LE"|"MZ"|None, ...}

Each format returns format-appropriate sections (PE: sections/directories; NE:
segments/entries; LE: objects; MZ: relocations) plus a common `summary`, so the
console can render whatever a given file offers. Offset-checked throughout.
"""
import struct

from src.formats import peinfo

_u8 = lambda d, o: d[o]
_u16 = lambda d, o: struct.unpack_from("<H", d, o)[0]
_u32 = lambda d, o: struct.unpack_from("<I", d, o)[0]


def _pstr(d, o):
    """length-prefixed (Pascal) string used by NE name tables."""
    n = d[o]
    return d[o + 1:o + 1 + n].decode("latin1", "replace"), o + 1 + n


def detect(path):
    try:
        d = open(path, "rb").read(0x40)
    except OSError:
        return None
    if len(d) < 0x40 or d[:2] != b"MZ":
        return None
    e = _u32(d, 0x3C)
    try:
        sig = open(path, "rb").read(e + 2)[e:e + 2]
    except OSError:
        return "MZ"
    if e and 0 < e < 0x10000000:
        if sig == b"PE":
            return "PE"
        if sig == b"NE":
            return "NE"
        if sig in (b"LE", b"LX"):
            return "LE"
    return "MZ"


def parse(path):
    fmt = detect(path)
    if fmt == "PE":
        r = peinfo.parse(path); r["format"] = "PE"
    elif fmt == "NE":
        r = _ne(open(path, "rb").read(), path)
    elif fmt == "LE":
        r = _le(open(path, "rb").read(), path)
    elif fmt == "MZ":
        r = _mz(open(path, "rb").read(), path)
    else:
        return {"format": None, "path": path, "ok": False, "error": "unrecognized"}
    # tag each resource with how the previewer should render it
    if r.get("resources"):
        from src.formats import resview
        for ty in r["resources"].get("types", []):
            for it in ty.get("items", []):
                it["kind"] = resview.preview_kind(it.get("type_id", -1))
    return r


# --------------------------------------------------------------------------- NE
NE_FLAGS = [(0x8000, "LIBRARY"), (0x2000, "ERRORS"), (0x0800, "PROTECTED_MODE"),
            (0x0002, "GLOBAL_INIT"), (0x0004, "PROTECTED_ONLY")]
NE_SEG_FLAGS = [(0x0001, "DATA"), (0x0010, "MOVABLE"), (0x0020, "SHAREABLE"),
                (0x0040, "PRELOAD"), (0x0080, "EXECUTE_ONLY/READ_ONLY"),
                (0x0100, "RELOCINFO"), (0x1000, "DISCARDABLE")]
NE_OS = {1: "OS/2", 2: "Windows", 3: "DOS4", 4: "Windows386"}


def _flags(v, tbl):
    return [n for b, n in tbl if v & b]


def _ne(d, path):
    e = _u32(d, 0x3C)

    def g16(o): return _u16(d, e + o)
    def g32(o): return _u32(d, e + o)
    align = g16(0x32) or 9
    out = {"format": "NE", "path": path, "size": len(d), "ok": True,
           "header": {
               "linker": "%d.%d" % (d[e + 2], d[e + 3]),
               "flags": _flags(g16(0x0C), NE_FLAGS), "flags_raw": hex(g16(0x0C)),
               "auto_data_seg": g16(0x0E), "heap": g16(0x10), "stack": g16(0x12),
               "entry_cs_ip": hex(g32(0x14)), "entry_ss_sp": hex(g32(0x18)),
               "segments": g16(0x1C), "module_refs": g16(0x1E),
               "movable_entries": g16(0x30), "align_shift": align,
               "resource_segs": g16(0x34),
               "target_os": NE_OS.get(d[e + 0x36], str(d[e + 0x36]))}}

    # segment table
    seg_off = e + g16(0x22)
    nseg = g16(0x1C)
    segs = []
    for i in range(nseg):
        s = seg_off + i * 8
        if s + 8 > len(d):
            break
        sector = _u16(d, s); length = _u16(d, s + 2); fl = _u16(d, s + 4); minalloc = _u16(d, s + 6)
        segs.append({"index": i + 1, "file_offset": sector << align,
                     "length": length or (0x10000 if fl & 0 else length),
                     "flags": _flags(fl, NE_SEG_FLAGS),
                     "kind": "DATA" if fl & 1 else "CODE", "min_alloc": minalloc})
    out["segments"] = segs

    # exports: resident + nonresident name tables (name, ordinal); entry 0 = module
    def names_at(o, first_is_module):
        res = []
        idx = 0
        while o < len(d) and d[o]:
            nm, o = _pstr(d, o)
            if o + 2 > len(d):
                break
            ordn = _u16(d, o); o += 2
            if not (first_is_module and idx == 0):
                res.append({"name": nm, "ordinal": ordn})
            idx += 1
        return res
    resident = names_at(e + g16(0x26), True)
    modname = ""
    ro = e + g16(0x26)
    if ro < len(d) and d[ro]:
        modname = _pstr(d, ro)[0]
    nonres_off = g32(0x2C)
    nonres = names_at(nonres_off, True) if 0 < nonres_off < len(d) else []
    nrdesc = ""
    if 0 < nonres_off < len(d) and d[nonres_off]:
        nrdesc = _pstr(d, nonres_off)[0]
    exports = sorted(resident + nonres, key=lambda x: x["ordinal"])
    out["module_name"] = modname
    out["description"] = nrdesc
    out["exports"] = {"count": len(exports), "symbols": exports}

    # imports: module-reference table -> imported-names table
    modref_off = e + g16(0x28)
    impnames_off = e + g16(0x2A)
    nmod = g16(0x1E)
    imports = []
    for i in range(nmod):
        po = modref_off + i * 2
        if po + 2 > len(d):
            break
        noff = impnames_off + _u16(d, po)
        if noff < len(d):
            imports.append({"name": _pstr(d, noff)[0]})
    out["imports"] = imports

    # entry table (bundle count only, for the summary)
    out["entry_table"] = {"offset": e + g16(0x04), "length": g16(0x06)}

    out["resources"] = _ne_resources(d, e, align)
    return out


def _ne_resources(d, e, align):
    ro = e + _u16(d, e + 0x24)
    if ro + 2 > len(d):
        return {"count": 0, "types": []}
    shift = _u16(d, ro); p = ro + 2
    types = []
    RT = {0x8001: "CURSOR", 0x8002: "BITMAP", 0x8003: "ICON", 0x8004: "MENU",
          0x8005: "DIALOG", 0x8006: "STRING", 0x8007: "FONTDIR", 0x8008: "FONT",
          0x8009: "ACCELERATOR", 0x800A: "RCDATA", 0x800C: "GROUP_CURSOR",
          0x800E: "GROUP_ICON", 0x8010: "VERSION"}
    guard = 0
    while p + 8 <= len(d) and guard < 256:
        tid = _u16(d, p)
        if tid == 0:
            break
        count = _u16(d, p + 2)
        p += 8                                   # skip type id, count, reserved dword
        tname = RT.get(tid, ("#%d" % (tid & 0x7FFF)) if tid & 0x8000 else "name@%d" % tid)
        items = []
        for _i in range(count):
            if p + 12 > len(d):
                break
            off = _u16(d, p) << shift; ln = _u16(d, p + 2) << shift; rid = _u16(d, p + 6)
            items.append({"name": ("#%d" % (rid & 0x7FFF)) if rid & 0x8000 else str(rid),
                          "offset": off, "foff": off, "size": ln,
                          "type_id": tid & 0x7FFF if tid & 0x8000 else tid})
            p += 12
        types.append({"type": tname, "count": len(items), "items": items})
        guard += 1
    return {"count": sum(t["count"] for t in types), "types": types}


# --------------------------------------------------------------------------- LE
def _le(d, path):
    e = _u32(d, 0x3C)

    def g16(o): return _u16(d, e + o)
    def g32(o): return _u32(d, e + o)
    out = {"format": "LE", "path": path, "size": len(d), "ok": True,
           "header": {
               "cpu": {2: "80286", 3: "80386", 4: "80486"}.get(g16(0x08), hex(g16(0x08))),
               "os": {1: "OS/2", 4: "Windows"}.get(g16(0x0A), hex(g16(0x0A))),
               "module_flags": hex(g32(0x10)), "pages": g32(0x14),
               "page_size": g32(0x28), "eip_object": g32(0x18), "eip": hex(g32(0x1C)),
               "esp_object": g32(0x20), "objects": g32(0x44),
               "imported_modules": g32(0x74)}}
    # object table
    obj_off = e + g32(0x40); nobj = g32(0x44)
    objs = []
    for i in range(nobj):
        o = obj_off + i * 24
        if o + 24 > len(d):
            break
        vsize = _u32(d, o); base = _u32(d, o + 4); fl = _u32(d, o + 8)
        pmi = _u32(d, o + 12); pcnt = _u32(d, o + 16)
        flags = []
        if fl & 0x1: flags.append("READ")
        if fl & 0x2: flags.append("WRITE")
        if fl & 0x4: flags.append("EXEC")
        if fl & 0x2000: flags.append("32BIT")
        objs.append({"index": i + 1, "vsize": vsize, "reloc_base": hex(base),
                     "flags": flags, "page_index": pmi, "page_count": pcnt})
    out["objects"] = objs
    # imported module names (length-prefixed)
    imps = []
    io = e + g32(0x70)
    for _i in range(g32(0x74)):
        if io >= len(d) or not d[io]:
            break
        nm, io = _pstr(d, io)
        imps.append({"name": nm})
    out["imports"] = imps
    return out


# --------------------------------------------------------------------------- MZ
def _mz(d, path):
    def g16(o): return _u16(d, o)
    nrel = g16(6); rel_off = g16(0x18)
    relocs = []
    for i in range(nrel):
        ro = rel_off + i * 4
        if ro + 4 > len(d):
            break
        relocs.append({"offset": g16(ro), "segment": g16(ro + 2),
                       "linear": (g16(ro + 2) * 16 + g16(ro)) & 0xFFFFF})
    hdr_par = g16(8)
    out = {"format": "MZ", "path": path, "size": len(d), "ok": True,
           "header": {
               "bytes_last_page": g16(2), "pages": g16(4), "relocations": nrel,
               "header_paragraphs": hdr_par, "header_size": hdr_par * 16,
               "min_alloc": g16(0x0A), "max_alloc": g16(0x0C),
               "init_ss": hex(g16(0x0E)), "init_sp": hex(g16(0x10)),
               "init_ip": hex(g16(0x14)), "init_cs": hex(g16(0x16)),
               "entry_linear": hex((g16(0x16) * 16 + g16(0x14)))},
           "relocations": {"count": nrel, "items": relocs[:512]}}
    return out


def hexdump(path, offset=0, length=256):
    return peinfo.hexdump(path, offset, length)


if __name__ == "__main__":
    import json
    import sys
    if len(sys.argv) < 2:
        print("usage: exeinfo.py <file>"); raise SystemExit(2)
    print(json.dumps(parse(sys.argv[1]), indent=2))
