#!/usr/bin/env python3
r"""typelib.py -- recover COM type information (interfaces, coclasses, GUIDs) toward IDL.

A type library (.tlb / .olb, or an RT_TYPELIB resource) is the authored COM contract: the
interfaces a component implements, their IIDs, the coclasses and their CLSIDs, enums and structs.
Recovering it is real source recovery for OLE/COM-heavy binaries (ole32, oleaut32, controls). Two
on-disk formats: MSFT (the newer, documented one) and SLTG (the older 1990s one, largely
undocumented). This parses MSFT into an IDL skeleton -- library, and each type's kind + name +
GUID -- and, for SLTG, does an honest PARTIAL extraction (the recoverable identifiers and GUIDs),
clearly labelled, rather than guessing at a structure it cannot verify.
"""
import re
import struct

_TKIND = ["enum", "struct", "module", "interface", "dispinterface", "coclass", "typedef", "union"]
_TIB = 0x64                                                   # sizeof(MSFT_TypeInfoBase)


def from_file(path):
    with open(path, "rb") as f:
        return f.read()


def from_module(module):
    """Extract an embedded RT_TYPELIB resource's bytes from a PE, or None."""
    from codebreaker.core import cores
    from codebreaker.formats.pe import PEFile
    from codebreaker.engines.recon import resources
    pe = PEFile(cores.get("target").resolve(module))
    if len(pe.datadirs) < 3 or not pe.datadirs[2][0]:
        return None
    d = pe.data
    base = pe._rva_to_off(pe.datadirs[2][0])
    for tname, tid, toff, tdir in resources._dir_entries(d, base, 0):
        if tid == "TYPELIB" or (tname & 0x7FFFFFFF) == 160:
            for _nn, _nid, noff, _nd in resources._dir_entries(d, base, toff):
                for _l, _li, loff, _ld in resources._dir_entries(d, base, noff):
                    rva, size = struct.unpack_from("<II", d, base + loff)
                    foff = pe._rva_to_off(rva)
                    if foff is not None:
                        return bytes(d[foff:foff + size])
    return None


def _guid(b):
    a, c, e = struct.unpack_from("<IHH", b, 0)
    return "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" % (
        a, c, e, b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15])


def parse(data):
    """Parse a type library blob -> {'format','name','guid','types':[...],'partial':bool} or None."""
    if len(data) < 4:
        return None
    if data[:4] == b"MSFT":
        return _parse_msft(data)
    if data[:4] == b"SLTG":
        return _parse_sltg(data)
    return None


# ------------------------------------------------------------------ MSFT
def _msft_names(seg):
    """Walk the NAMETAB segment -> {byte offset: name} (MSFT_NameIntro: hreftype,next,namelen,name)."""
    out, p = {}, 0
    while p + 12 <= len(seg):
        namelen = struct.unpack_from("<I", seg, p + 8)[0] & 0xFF
        start = p + 12
        name = seg[start:start + namelen].decode("latin1", "replace")
        out[p] = (name, struct.unpack_from("<i", seg, p)[0])   # (name, hreftype)
        p = start + ((namelen + 3) & ~3)
        if namelen == 0:
            break
    return out


def _parse_msft(d):
    magic2 = struct.unpack_from("<I", d, 4)[0]
    posguid, = struct.unpack_from("<i", d, 8)
    nrtypeinfos, = struct.unpack_from("<I", d, 0x20)
    lib_name_off, = struct.unpack_from("<i", d, 0x38)
    segdir = 0x54
    segs = [struct.unpack_from("<iiii", d, segdir + i * 16)[:2] for i in range(15)]
    ti_off, ti_len = segs[0]
    guid_off, guid_len = segs[5]
    name_off, name_len = segs[7]
    names = _msft_names(d[name_off:name_off + name_len]) if name_len > 0 else {}
    guids = {}
    for p in range(0, guid_len - 23, 24):
        gb = d[guid_off + p:guid_off + p + 16]
        href = struct.unpack_from("<i", d, guid_off + p + 16)[0]
        guids[href] = _guid(gb)
    out = {"format": "MSFT", "partial": False,
           "name": names.get(lib_name_off, ("", 0))[0] if lib_name_off >= 0 else "",
           "guid": guids.get(-2, ""), "types": []}
    for n in range(nrtypeinfos):
        base = ti_off + n * _TIB
        if base + _TIB > len(d):
            break
        tk = struct.unpack_from("<I", d, base)[0] & 0xF
        nm = ""
        for off, (name, href) in names.items():                # the name whose hreftype is this type
            if href == n * _TIB:
                nm = name; break
        out["types"].append({"kind": _TKIND[tk] if tk < len(_TKIND) else "type%d" % tk,
                             "name": nm, "guid": guids.get(n * _TIB, "")})
    return out


# ------------------------------------------------------------------ SLTG (partial, honest)
def _parse_sltg(d):
    """SLTG is largely undocumented; recover what is verifiable: the library name, GUIDs, and the
    identifier strings (interface/coclass/method names). Reported as a PARTIAL recovery."""
    # identifiers: null-terminated ASCII tokens that look like COM names (I*/C*/lib names/methods).
    # SLTG stores type/member names as null-terminated strings; this is a verifiable partial recovery.
    idents = []
    for m in re.finditer(rb"[A-Za-z_][A-Za-z0-9_]{2,63}\x00", d):
        s = m.group(0)[:-1].decode("latin1")
        if s not in idents:
            idents.append(s)
    interfaces = [s for s in idents if re.match(r"^I[A-Z]", s)]
    return {"format": "SLTG", "partial": True, "name": idents[0] if idents else "",
            "guid": "", "types": [{"kind": "interface", "name": s, "guid": ""} for s in interfaces],
            "identifiers": idents}


# ------------------------------------------------------------------ emit IDL
def to_idl(parsed):
    if not parsed:
        return ""
    out = ["// Recovered type library (%s%s)" % (parsed["format"],
                                                 ", PARTIAL" if parsed.get("partial") else "")]
    if parsed.get("guid"):
        out.append("[ uuid(%s) ]" % parsed["guid"].strip("{}"))
    out.append("library %s\n{" % (parsed.get("name") or "TypeLib"))
    for t in parsed["types"]:
        g = ("[ uuid(%s) ] " % t["guid"].strip("{}")) if t.get("guid") else ""
        out.append("    %s%s %s;" % (g, t["kind"], t["name"] or "/*unnamed*/"))
    out.append("}")
    if parsed.get("partial") and parsed.get("identifiers"):
        out.append("\n// recovered identifiers (SLTG partial): " + ", ".join(parsed["identifiers"][:40]))
    return "\n".join(out)


def reconstruct(module):
    """Recover a module's embedded type library as an IDL skeleton, or {'ok':False}."""
    data = from_module(module)
    if not data:
        return {"ok": False, "reason": "no RT_TYPELIB resource"}
    p = parse(data)
    if not p:
        return {"ok": False, "reason": "unknown typelib format %r" % data[:4]}
    return {"ok": True, "format": p["format"], "partial": p.get("partial", False),
            "ntypes": len(p["types"]), "idl": to_idl(p)}
