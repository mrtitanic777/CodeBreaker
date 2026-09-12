#!/usr/bin/env python3
r"""cxxeh.py -- recover C++ try/catch structure from MSVC __CxxFrameHandler tables.

seh.py recovers Win32 SEH (__try/__except). MSVC C++ exceptions are a different mechanism: the
compiler emits a FuncInfo table (magic 0x19930520) describing the function's try blocks, and for
each try block a handler array naming the caught TYPE (a TypeDescriptor, `.?AVexception@@`) and the
address of its catch body. Reading those tables recovers the authored `try { } catch (T) { }`
shape -- which types a function catches and where -- that no control-flow engine can see. This
scans for the FuncInfo magic and walks FuncInfo -> TryBlockMap -> HandlerType -> TypeDescriptor.
"""
import struct

_MAGICS = (0x19930520, 0x19930521, 0x19930522)


def _rd(pe, va, n):
    off = pe._rva_to_off(va - pe.image_base)
    if off is None or off + n > len(pe.data):
        return None
    return pe.data[off:off + n]


def _typename(pe, td_va):
    """TypeDescriptor at td_va -> readable caught type ('class exception'), or 'void' for catch(...)."""
    if td_va == 0:
        return "..."                                          # catch-all
    nb = _rd(pe, td_va + 8, 64)
    if nb is None:
        return "?"
    z = nb.find(b"\x00")
    raw = nb[:z].decode("latin1", "replace") if z > 0 else ""
    from src.engines.behavior.rtti import _type_name
    return _type_name(raw) if raw.startswith(".?A") else (raw or "?")


def _parse_funcinfo(pe, fi_va):
    """FuncInfo -> [{'tryLow','tryHigh','catches':[(type, handler_va)]}], or None."""
    fi = _rd(pe, fi_va, 28)
    if fi is None or struct.unpack_from("<I", fi, 0)[0] not in _MAGICS:
        return None
    _magic, _maxstate, _punwind, ntry, ptrymap = struct.unpack_from("<IIIII", fi, 0)
    if ntry == 0 or ntry > 256 or ptrymap == 0:
        return []
    blocks = []
    for t in range(ntry):
        tb = _rd(pe, ptrymap + t * 20, 20)
        if tb is None:
            break
        low, high, chigh, ncatch, pharr = struct.unpack_from("<iiiiI", tb, 0)
        catches = []
        for c in range(min(ncatch, 64)):
            h = _rd(pe, pharr + c * 16, 16)
            if h is None:
                break
            _adj, ptype, _disp, handler = struct.unpack_from("<IIiI", h, 0)
            catches.append((_typename(pe, ptype), handler))
        blocks.append({"tryLow": low, "tryHigh": high, "catches": catches})
    return blocks


def find_funcinfos(module):
    """[(funcinfo_va, blocks)] for every C++ EH FuncInfo in the module."""
    from src.formats.pe import PEFile
    from src.core import cores
    pe = PEFile(cores.get("target").resolve(module))
    ib, out = pe.image_base, []
    for s in pe.sections:
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        base = ib + s["vaddr"]
        for m in _iter_magic(raw):
            blocks = _parse_funcinfo(pe, base + m)
            if blocks:
                out.append((base + m, blocks))
    return out


def _iter_magic(raw):
    i = 0
    n = len(raw)
    while i + 4 <= n:
        v = int.from_bytes(raw[i:i + 4], "little")
        if v in _MAGICS:
            yield i
        i += 4


def reconstruct(module):
    """Recover C++ try/catch shapes from a module's FuncInfo tables. {'ok','nfuncs','c'}."""
    try:
        fis = find_funcinfos(module)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not fis:
        return {"ok": False, "reason": "no C++ EH tables (no __CxxFrameHandler FuncInfo)"}
    lines = []
    for fi_va, blocks in fis:
        lines.append("/* C++ EH FuncInfo @0x%08X -- %d try block(s) */" % (fi_va, len(blocks)))
        for b in blocks:
            lines.append("try { /* states %d..%d */" % (b["tryLow"], b["tryHigh"]))
            lines.append("}")
            for typ, handler in b["catches"]:
                lines.append("catch (%s) { /* handler @0x%08X */ }" % (typ, handler))
    return {"ok": True, "nfuncs": len(fis), "ntry": sum(len(b) for _v, b in fis),
            "c": "\n".join(lines)}
