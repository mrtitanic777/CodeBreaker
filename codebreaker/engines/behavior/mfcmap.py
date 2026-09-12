#!/usr/bin/env python3
r"""mfcmap.py -- recover MFC message maps (the event-handler wiring of an MFC app).

An MFC window/command target routes every message through a static `AFX_MSGMAP_ENTRY` array: each
entry is {message, code, id, lastId, sig, handler}, and the array ends in a zero terminator. That
table IS the authored `BEGIN_MESSAGE_MAP` block -- it says which method handles WM_PAINT, which
handles command ID_FILE_OPEN, and so on. Recovering it hands back the app's entire event wiring as
source, which is often the most important thing in an MFC program. This scans the data sections for
those arrays (24-byte records whose handler points into code, terminated by zeros) and re-emits the
`ON_WM_*` / `ON_COMMAND` / `ON_MESSAGE` map.
"""
import struct
from codebreaker.engines.behavior import tracer

_CNT_CODE, _MEM_EXEC = 0x20, 0x20000000
_WM_COMMAND, _WM_NOTIFY = 0x111, 0x4E

# common window messages -> their ON_WM_* macro name (subset that covers typical handlers)
_WM = {0x0001: "ON_WM_CREATE", 0x0002: "ON_WM_DESTROY", 0x0003: "ON_WM_MOVE",
       0x0005: "ON_WM_SIZE", 0x0006: "ON_WM_ACTIVATE", 0x0007: "ON_WM_SETFOCUS",
       0x0008: "ON_WM_KILLFOCUS", 0x000A: "ON_WM_ENABLE", 0x000F: "ON_WM_PAINT",
       0x0010: "ON_WM_CLOSE", 0x0014: "ON_WM_ERASEBKGND", 0x001C: "ON_WM_ACTIVATEAPP",
       0x0024: "ON_WM_GETMINMAXINFO", 0x0100: "ON_WM_KEYDOWN", 0x0101: "ON_WM_KEYUP",
       0x0102: "ON_WM_CHAR", 0x0111: None, 0x0112: "ON_WM_SYSCOMMAND",
       0x0113: "ON_WM_TIMER", 0x0114: "ON_WM_HSCROLL", 0x0115: "ON_WM_VSCROLL",
       0x0200: "ON_WM_MOUSEMOVE", 0x0201: "ON_WM_LBUTTONDOWN", 0x0202: "ON_WM_LBUTTONUP",
       0x0203: "ON_WM_LBUTTONDBLCLK", 0x0204: "ON_WM_RBUTTONDOWN", 0x0205: "ON_WM_RBUTTONUP",
       0x0207: "ON_WM_MBUTTONDOWN", 0x020A: "ON_WM_MOUSEWHEEL",
       0x0024: "ON_WM_GETMINMAXINFO", 0x0086: "ON_WM_NCACTIVATE"}


def _code_sections(pe):
    return [(pe.image_base + s["vaddr"], pe.image_base + s["vaddr"] + max(s["vsize"], s["rawsize"]))
            for s in pe.sections if s["chars"] & (_CNT_CODE | _MEM_EXEC)]


def _in_code(va, ranges):
    return any(lo <= va < hi for lo, hi in ranges)


def find_msgmaps(module):
    """[(array_va, [entry, ...])] where each entry is a dict; scans data sections for AFX_MSGMAP_ENTRY."""
    from codebreaker.formats.pe import PEFile
    from codebreaker.core import cores
    pe = PEFile(cores.get("target").resolve(module))
    code = _code_sections(pe)
    ib = pe.image_base
    out = []
    for s in pe.sections:
        if s["chars"] & (_CNT_CODE | _MEM_EXEC):
            continue
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secva, n = ib + s["vaddr"], len(raw)
        i = 0
        while i + 24 <= n:
            entries, j = [], i
            while j + 24 <= n:
                msg, code_, nid, lastid, sig, pfn = struct.unpack_from("<IIIIII", raw, j)
                if msg == 0 and pfn == 0 and sig == 0:        # AfxSig_end terminator
                    break
                if not _in_code(pfn, code) or msg > 0xBFFF or sig > 0x40:
                    entries = []                              # not a message map
                    break
                entries.append({"msg": msg, "code": code_, "id": nid, "lastid": lastid,
                                "sig": sig, "pfn": pfn})
                j += 24
            if entries and j + 24 <= n:                       # a run followed by a valid terminator
                out.append((secva + i, entries))
                i = j + 24
            else:
                i += 4
    return out, ib


def _macro(e, va0):
    handler = "sub_%x" % (e["pfn"] - va0) if e["pfn"] >= va0 else "fn_%x" % e["pfn"]
    if e["msg"] == _WM_COMMAND:
        if e["lastid"] and e["lastid"] != e["id"]:
            return "ON_COMMAND_RANGE(0x%X, 0x%X, %s)" % (e["id"], e["lastid"], handler)
        return "ON_COMMAND(0x%X, %s)  /* -> %s */" % (e["id"], handler, handler)
    if e["msg"] == _WM_NOTIFY:
        return "ON_NOTIFY(%d, 0x%X, %s)" % (e["code"], e["id"], handler)
    macro = _WM.get(e["msg"])
    if macro:
        return "%s()  /* -> %s */" % (macro, handler)
    return "ON_MESSAGE(0x%04X, %s)" % (e["msg"], handler)


def _guid(b):
    a, c, e = struct.unpack_from("<IHH", b, 0)
    return "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" % (
        a, c, e, b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15])


def _cstr(pe, va, maxlen=128):
    off = pe._rva_to_off(va - pe.image_base)
    if off is None:
        return None
    end = pe.data.find(b"\x00", off, off + maxlen)
    s = pe.data[off:end] if end > off else b""
    return s.decode("latin1") if s and all(32 <= b < 127 for b in s) else None


def find_interface_maps(pe, data_ranges, code):
    """[(array_va, [iid_guid, ...])] -- AFX_INTERFACEMAP_ENTRY {const IID* piid; size_t nOffset}."""
    ib = pe.image_base
    out = []
    for s in pe.sections:
        if s["chars"] & (_CNT_CODE | _MEM_EXEC):
            continue
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secva, n, i = ib + s["vaddr"], len(raw), 0
        while i + 8 <= n:
            iids, j = [], i
            while j + 8 <= n:
                piid, noff = struct.unpack_from("<II", raw, j)
                if piid == 0:                                # {NULL, offset} terminator
                    break
                goff = pe._rva_to_off(piid - ib) if piid > ib else None
                if goff is None or noff > 0x10000:
                    iids = []; break
                gb = pe.data[goff:goff + 16]
                if len(gb) < 16 or gb == b"\x00" * 16:
                    iids = []; break
                z = gb.find(b"\x00")                          # a NUL-terminated printable run is a
                if z >= 3 and all(32 <= b < 127 for b in gb[:z]):  # string (a name ptr), not a GUID
                    iids = []; break
                iids.append(_guid(gb)); j += 8
            if len(iids) >= 1 and j + 8 <= n and struct.unpack_from("<I", raw, j)[0] == 0:
                out.append((secva + i, iids)); i = j + 8
            else:
                i += 4
    return out


def find_dispatch_maps(pe):
    """[(array_va, [member_name, ...])] -- AFX_DISPMAP_ENTRY arrays keyed on their name-string field."""
    ib = pe.image_base
    out = []
    for s in pe.sections:
        if s["chars"] & (_CNT_CODE | _MEM_EXEC):
            continue
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secva, n = ib + s["vaddr"], len(raw)
        for stride in (28, 24, 32):                          # AFX_DISPMAP_ENTRY size varies by version
            i = 0
            while i + stride <= n:
                names, j = [], i
                while j + stride <= n:
                    p0 = struct.unpack_from("<I", raw, j)[0]
                    if p0 == 0:
                        break
                    nm = _cstr(pe, p0) if p0 > ib else None
                    if not nm or not nm[:1].isalpha():
                        names = []; break
                    names.append(nm); j += stride
                if len(names) >= 2:                           # a real dispatch map has several members
                    out.append((secva + i, names)); i = j + stride
                else:
                    i += 4
            if out:
                break                                        # first stride that yields maps wins
    return out


def reconstruct(module):
    """Recover MFC message / interface / dispatch maps as source. {'ok','nmaps','c'}."""
    from codebreaker.formats.pe import PEFile
    from codebreaker.core import cores
    try:
        maps, va0 = find_msgmaps(module)
        pe = PEFile(cores.get("target").resolve(module))
        code = _code_sections(pe)
        imaps = find_interface_maps(pe, None, code)
        dmaps = find_dispatch_maps(pe)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not (maps or imaps or dmaps):
        return {"ok": False, "reason": "no MFC maps found (not MFC, or stripped)"}
    blocks = []
    for va, entries in maps:
        lines = ["BEGIN_MESSAGE_MAP(/* class @ 0x%08X */)" % va]
        for e in entries:
            lines.append("    " + _macro(e, va0))
        lines.append("END_MESSAGE_MAP()")
        blocks.append("\n".join(lines))
    for va, iids in imaps:
        lines = ["BEGIN_INTERFACE_MAP(/* class @ 0x%08X */)" % va]
        for iid in iids:
            lines.append("    INTERFACE_PART(iid %s)" % iid)
        lines.append("END_INTERFACE_MAP()")
        blocks.append("\n".join(lines))
    for va, names in dmaps:
        lines = ["BEGIN_DISPATCH_MAP(/* class @ 0x%08X */)" % va]
        for nm in names:
            lines.append('    DISP_FUNCTION_OR_PROPERTY("%s")' % nm)
        lines.append("END_DISPATCH_MAP()")
        blocks.append("\n".join(lines))
    return {"ok": True, "nmaps": len(maps), "ninterfacemaps": len(imaps), "ndispatchmaps": len(dmaps),
            "nentries": sum(len(e) for _v, e in maps), "c": "\n\n".join(blocks)}
