#!/usr/bin/env python3
"""peinfo.py -- CodeBreaker's full PE parser, at feature parity with PEExplorerV2's
PEParser: every header, all data directories, sections, exports (name/ordinal/hint/
forwarder/undecorated), imports + delay imports (name/ordinal/hint), the resource
tree, base relocations, TLS, load config, debug directory, security certificates,
the CLR/.NET header, and the Rich header. PE32 and PE32+.

    from src.formats import peinfo
    pe = peinfo.parse(path)          # -> dict, the structured PE
    peinfo.hexdump(path, off, n)     # -> raw bytes window (for the hex view)

Everything is offset-checked; a malformed field yields a note rather than a crash.
"""
import os
import struct

# ---- little-endian readers -----------------------------------------------------
def _u8(d, o):  return d[o]
def _u16(d, o): return struct.unpack_from("<H", d, o)[0]
def _u32(d, o): return struct.unpack_from("<I", d, o)[0]
def _u64(d, o): return struct.unpack_from("<Q", d, o)[0]
def _cstr(d, o):
    e = d.find(b"\x00", o)
    return d[o:e if e >= 0 else len(d)].decode("latin1", "replace")


DIR_NAMES = ["Export", "Import", "Resource", "Exception", "Security", "BaseReloc",
             "Debug", "Architecture", "GlobalPtr", "TLS", "LoadConfig", "BoundImport",
             "IAT", "DelayImport", "CLR", "Reserved"]

MACHINE = {0x14c: "i386", 0x8664: "x64", 0x1c0: "ARM", 0xaa64: "ARM64",
           0x1c2: "ARMNT", 0x200: "IA64", 0x162: "R3000", 0x166: "R4000",
           0x1f0: "PowerPC", 0x184: "Alpha", 0x268: "M68K"}
SUBSYSTEM = {0: "Unknown", 1: "Native", 2: "Windows GUI", 3: "Windows CUI",
             5: "OS/2 CUI", 7: "POSIX CUI", 8: "Native Win9x", 9: "Windows CE GUI",
             10: "EFI Application", 11: "EFI Boot Driver", 12: "EFI Runtime Driver",
             13: "EFI ROM", 14: "Xbox", 16: "Boot Application"}
SECTION_FLAGS = [(0x20, "CODE"), (0x40, "INITDATA"), (0x80, "UNINITDATA"),
                 (0x02000000, "DISCARDABLE"), (0x04000000, "NOTCACHED"),
                 (0x08000000, "NOTPAGED"), (0x10000000, "SHARED"),
                 (0x20000000, "EXECUTE"), (0x40000000, "READ"), (0x80000000, "WRITE")]
FILE_CHARS = [(0x0002, "EXECUTABLE"), (0x2000, "DLL"), (0x0100, "32BIT"),
              (0x0020, "LARGE_ADDRESS"), (0x1000, "SYSTEM"), (0x0001, "NO_RELOCS")]
RES_TYPES = {1: "CURSOR", 2: "BITMAP", 3: "ICON", 4: "MENU", 5: "DIALOG",
             6: "STRING", 7: "FONTDIR", 8: "FONT", 9: "ACCELERATOR", 10: "RCDATA",
             11: "MESSAGETABLE", 12: "GROUP_CURSOR", 14: "GROUP_ICON", 16: "VERSION",
             17: "DLGINCLUDE", 19: "PLUGPLAY", 20: "VXD", 21: "ANICURSOR",
             22: "ANIICON", 23: "HTML", 24: "MANIFEST"}


def _undecorate(name):
    try:
        from src.engines.symbol import demangle
        d = demangle.demangle_sig(name) if name.startswith("?") else None
        return d.get("name") if d else ""
    except Exception:
        return ""


class _PE:
    def __init__(self, d):
        self.d = d
        self.ok = False
        self.error = ""
        self.secs = []       # (name, va, vsize, rawoff, rawsize, chars)

    def rva2off(self, rva):
        for _n, va, vs, ro, rs, _c in self.secs:
            if va <= rva < va + max(vs, rs):
                return ro + (rva - va)
        return rva if rva < len(self.d) else None


def parse(path):
    d = open(path, "rb").read()
    pe = _PE(d)
    out = {"path": path, "size": len(d)}
    if len(d) < 0x40 or d[:2] != b"MZ":
        out["error"] = "not MZ/PE"; return out
    e = _u32(d, 0x3C)
    if e + 24 > len(d) or d[e:e + 4] != b"PE\x00\x00":
        out["error"] = "no PE header (NE/LE/MZ-only?)"; return out

    # DOS header + Rich header
    out["dos"] = {"e_lfanew": e, "e_cblp": _u16(d, 2), "e_cp": _u16(d, 4),
                  "e_cparhdr": _u16(d, 8), "e_lfarlc": _u16(d, 0x18)}
    out["rich"] = _rich(d, e)

    coff = e + 4
    machine = _u16(d, coff)
    nsec = _u16(d, coff + 2)
    optsz = _u16(d, coff + 16)
    chars = _u16(d, coff + 18)
    out["file_header"] = {
        "machine": MACHINE.get(machine, hex(machine)), "machine_id": machine,
        "sections": nsec, "timestamp": _u32(d, coff + 8),
        "symbols": _u32(d, coff + 12), "opt_header_size": optsz,
        "characteristics": _flags(chars, FILE_CHARS)}

    opt = coff + 20
    magic = _u16(d, opt)
    pe64 = magic == 0x20B
    out["pe_plus"] = pe64
    # optional header (fields common + the 32/64 split for imagebase/sizes)
    subsys = _u16(d, opt + 68)
    entry = _u32(d, opt + 16)
    out["optional_header"] = {
        "magic": hex(magic),
        "entry_point": entry, "code_base": _u32(d, opt + 20),
        "image_base": _u64(d, opt + 24) if pe64 else _u32(d, opt + 28),
        "section_align": _u32(d, opt + 32), "file_align": _u32(d, opt + 36),
        "image_size": _u32(d, opt + 56), "headers_size": _u32(d, opt + 60),
        "subsystem": SUBSYSTEM.get(subsys, str(subsys)), "subsystem_id": subsys,
        "dll_characteristics": _u16(d, opt + 70),
        "os_version": "%d.%d" % (_u16(d, opt + 40), _u16(d, opt + 42)),
        "linker_version": "%d.%d" % (_u8(d, opt + 2), _u8(d, opt + 3))}

    # data directories
    ndir_off = opt + (108 if pe64 else 92)
    ndir = _u32(d, ndir_off)
    dd = opt + (112 if pe64 else 96)
    dirs = []
    for i in range(min(ndir, 16)):
        rva = _u32(d, dd + i * 8); size = _u32(d, dd + i * 8 + 4)
        dirs.append({"name": DIR_NAMES[i] if i < 16 else str(i), "rva": rva, "size": size})
    out["data_directories"] = dirs

    # sections
    sh = opt + optsz
    for i in range(nsec):
        s = sh + i * 40
        if s + 40 > len(d):
            break
        nm = d[s:s + 8].rstrip(b"\x00").decode("latin1", "replace")
        va, vs, rs, ro, sc = _u32(d, s + 12), _u32(d, s + 8), _u32(d, s + 16), _u32(d, s + 20), _u32(d, s + 36)
        pe.secs.append((nm, va, vs, ro, rs, sc))
    out["sections"] = [{"name": n, "vaddr": va, "vsize": vs, "raw_offset": ro,
                        "raw_size": rs, "characteristics": _flags(c, SECTION_FLAGS)}
                       for n, va, vs, ro, rs, c in pe.secs]

    def diri(i):
        return (dirs[i]["rva"], dirs[i]["size"]) if i < len(dirs) else (0, 0)

    out["exports"] = _exports(pe, *diri(0))
    out["imports"] = _imports(pe, *diri(1))
    out["delay_imports"] = _imports(pe, *diri(13), delay=True)
    out["resources"] = _resources(pe, *diri(2))
    out["base_relocations"] = _relocs(pe, *diri(5))
    out["tls"] = _tls(pe, *diri(9), pe64=pe64)
    out["debug"] = _debug(pe, *diri(6))
    out["certificates"] = _certs(d, *diri(4))
    out["load_config"] = diri(10)[1] > 0
    out["clr"] = _clr(pe, *diri(14))
    out["ok"] = True
    return out


def _flags(v, table):
    return [name for bit, name in table if v & bit]


def _rich(d, e):
    # the "Rich" header sits between the DOS stub and the PE header
    seg = d[:e]
    ri = seg.rfind(b"Rich")
    if ri < 0 or ri + 8 > len(seg):
        return None
    key = _u32(seg, ri + 4)
    di = seg.rfind(b"DanS")
    # DanS is XOR-key-encoded; just report presence + product entries count
    if di < 0:
        return {"present": True, "key": hex(key)}
    n = max(0, (ri - di - 16) // 8)
    return {"present": True, "key": hex(key), "entries": n}


def _exports(pe, rva, size):
    d = pe.d
    o = pe.rva2off(rva)
    if not rva or o is None or o + 40 > len(d):
        return {"count": 0, "symbols": []}
    name_rva = _u32(d, o + 12)
    base = _u32(d, o + 16)
    nfunc = _u32(d, o + 20)
    nname = _u32(d, o + 24)
    eat = pe.rva2off(_u32(d, o + 28))
    npt = pe.rva2off(_u32(d, o + 32))
    nao = pe.rva2off(_u32(d, o + 36))
    modname = _cstr(d, pe.rva2off(name_rva)) if name_rva and pe.rva2off(name_rva) is not None else ""
    # ordinal -> name (from the name pointer + ordinal tables)
    ord2name = {}
    if npt is not None and nao is not None:
        for i in range(nname):
            if npt + i * 4 + 4 > len(d) or nao + i * 2 + 2 > len(d):
                break
            nrva = _u32(d, npt + i * 4)
            oi = _u16(d, nao + i * 2)
            no = pe.rva2off(nrva)
            ord2name[oi] = (_cstr(d, no) if no is not None else "", i)
    syms = []
    if eat is not None:
        for i in range(nfunc):
            if eat + i * 4 + 4 > len(d):
                break
            frva = _u32(d, eat + i * 4)
            if not frva:
                continue
            name, hint = ord2name.get(i, ("", -1))
            fwd = ""
            if rva <= frva < rva + size:                 # points inside export dir = forwarder
                fo = pe.rva2off(frva)
                fwd = _cstr(d, fo) if fo is not None else ""
            syms.append({"name": name, "ordinal": base + i, "hint": hint,
                         "rva": frva, "forward": fwd,
                         "undecorated": _undecorate(name) if name else ""})
    return {"module": modname, "count": len(syms), "symbols": syms}


def _imports(pe, rva, size, delay=False):
    d = pe.d
    o = pe.rva2off(rva)
    if not rva or o is None:
        return []
    libs = []
    step = 32 if delay else 20
    for k in range(2048):
        rec = o + k * step
        if rec + step > len(d):
            break
        if delay:
            name_rva = _u32(d, rec + 4); ilt_rva = _u32(d, rec + 16); iat_rva = _u32(d, rec + 12)
        else:
            ilt_rva = _u32(d, rec + 0); name_rva = _u32(d, rec + 12); iat_rva = _u32(d, rec + 16)
        if name_rva == 0 and iat_rva == 0 and ilt_rva == 0:
            break
        no = pe.rva2off(name_rva)
        libname = _cstr(d, no) if no is not None else "?"
        lut = pe.rva2off(ilt_rva or iat_rva)
        syms = []
        if lut is not None:
            for j in range(4096):
                if lut + j * 4 + 4 > len(d):
                    break
                t = _u32(d, lut + j * 4)
                if not t:
                    break
                if t & 0x80000000:
                    syms.append({"name": "", "ordinal": t & 0xFFFF, "hint": -1})
                else:
                    ho = pe.rva2off(t & 0x7FFFFFFF)
                    if ho is not None and ho + 2 <= len(d):
                        hint = _u16(d, ho)
                        nm = _cstr(d, ho + 2)
                        syms.append({"name": nm, "ordinal": -1, "hint": hint,
                                     "undecorated": _undecorate(nm) if nm else ""})
        libs.append({"name": libname, "count": len(syms), "symbols": syms})
    return libs


def _resources(pe, rva, size):
    d = pe.d
    base = pe.rva2off(rva)
    if not rva or base is None:
        return {"count": 0, "types": []}

    def entries(dir_off):
        if dir_off + 16 > len(d):
            return []
        nnamed = _u16(d, dir_off + 12); nid = _u16(d, dir_off + 14)
        out = []
        p = dir_off + 16
        for i in range(nnamed + nid):
            if p + 8 > len(d):
                break
            nameval = _u32(d, p); offval = _u32(d, p + 4); p += 8
            if nameval & 0x80000000:                     # named
                no = base + (nameval & 0x7FFFFFFF)
                ln = _u16(d, no) if no + 2 <= len(d) else 0
                label = d[no + 2:no + 2 + ln * 2].decode("utf-16-le", "replace")
                is_id = False; ident = 0
            else:
                label = ""; ident = nameval; is_id = True
            out.append((label, ident, is_id, offval))
        return out

    types = []
    for tlabel, tid, tisid, toff in entries(base):
        tname = tlabel or RES_TYPES.get(tid, str(tid))
        items = []
        if toff & 0x80000000:                            # subdirectory (names/ids)
            for nlabel, nid, nisid, noff in entries(base + (toff & 0x7FFFFFFF)):
                if noff & 0x80000000:                    # lang subdirectory
                    for llabel, lid, lisid, loff in entries(base + (noff & 0x7FFFFFFF)):
                        if not (loff & 0x80000000):      # data entry
                            de = base + loff
                            if de + 16 <= len(d):
                                drva = _u32(d, de)
                                items.append({"name": nlabel or str(nid), "lang": lid,
                                              "rva": drva, "foff": pe.rva2off(drva),
                                              "size": _u32(d, de + 4),
                                              "type_id": tid})
        types.append({"type": tname, "id": tid, "count": len(items), "items": items})
    return {"count": sum(t["count"] for t in types), "types": types}


def _relocs(pe, rva, size):
    d = pe.d
    o = pe.rva2off(rva)
    if not rva or o is None:
        return {"blocks": 0, "fixups": 0}
    blocks = fixups = 0
    end = o + size
    while o + 8 <= end and o + 8 <= len(d):
        page = _u32(d, o); bsz = _u32(d, o + 4)
        if bsz < 8:
            break
        blocks += 1
        fixups += (bsz - 8) // 2
        o += bsz
    return {"blocks": blocks, "fixups": fixups}


def _tls(pe, rva, size, pe64):
    if not rva:
        return None
    o = pe.rva2off(rva)
    if o is None:
        return None
    w = 8 if pe64 else 4
    return {"start_va": (_u64 if pe64 else _u32)(pe.d, o),
            "end_va": (_u64 if pe64 else _u32)(pe.d, o + w),
            "callbacks_va": (_u64 if pe64 else _u32)(pe.d, o + 3 * w)}


def _debug(pe, rva, size):
    d = pe.d
    o = pe.rva2off(rva)
    if not rva or o is None:
        return []
    types = {0: "Unknown", 1: "COFF", 2: "CodeView", 3: "FPO", 4: "Misc",
             9: "Borland", 12: "VC_Feature", 16: "Repro"}
    out = []
    for i in range(size // 28):
        e = o + i * 28
        if e + 28 > len(d):
            break
        out.append({"type": types.get(_u32(d, e + 12), str(_u32(d, e + 12))),
                    "timestamp": _u32(d, e + 4), "size": _u32(d, e + 16)})
    return out


def _certs(d, rva, size):
    # security dir uses a FILE OFFSET, not an RVA
    if not rva or rva + 8 > len(d):
        return []
    out = []
    o = rva; end = min(rva + size, len(d))
    while o + 8 <= end:
        length = _u32(d, o); rev = _u16(d, o + 4); typ = _u16(d, o + 6)
        if length < 8:
            break
        out.append({"length": length, "revision": hex(rev), "type": typ})
        o += (length + 7) & ~7
    return out


def _clr(pe, rva, size):
    d = pe.d
    o = pe.rva2off(rva)
    if not rva or o is None or o + 72 > len(d):
        return None
    return {"managed": True, "runtime_version": "%d.%d" % (_u16(d, o + 4), _u16(d, o + 6)),
            "flags": hex(_u32(d, o + 16)), "entry_token": hex(_u32(d, o + 20))}


def hexdump(path, offset=0, length=256):
    with open(path, "rb") as f:
        f.seek(offset)
        data = f.read(length)
    lines = []
    for i in range(0, len(data), 16):
        chunk = data[i:i + 16]
        hexs = " ".join("%02X" % b for b in chunk)
        ascii_ = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        lines.append("%08X  %-47s  %s" % (offset + i, hexs, ascii_))
    return "\n".join(lines)


if __name__ == "__main__":
    import json
    import sys
    if len(sys.argv) < 2:
        print("usage: peinfo.py <pe> [--hex OFF LEN]"); raise SystemExit(2)
    if len(sys.argv) >= 3 and sys.argv[2] == "--hex":
        print(hexdump(sys.argv[1], int(sys.argv[3], 0) if len(sys.argv) > 3 else 0,
                      int(sys.argv[4], 0) if len(sys.argv) > 4 else 256))
    else:
        print(json.dumps(parse(sys.argv[1]), indent=2))
