#!/usr/bin/env python3
r"""classlayout.py -- recover C++ / COM class layouts from vtables.

A C++ object with virtual methods (and every COM interface) begins with a vtable: a run of
function pointers in a data section. Recovering those runs reconstructs the class's shape -- its
virtual methods, in order -- as a C struct of function pointers, which for a COM interface IS the
authored interface (the first three slots are always IUnknown's QueryInterface/AddRef/Release, the
next four IDispatch's). This scans the data sections for runs of pointers that all target carved
function starts, and emits each as a `struct <name>Vtbl { ... };` with the methods it dispatches,
annotating the IUnknown/IDispatch prefix when the shape matches a COM interface.
"""
from src.engines.behavior import tracer

_CNT_CODE, _MEM_EXEC = 0x20, 0x20000000
_IUNKNOWN = ["QueryInterface", "AddRef", "Release"]
_IDISPATCH = ["GetTypeInfoCount", "GetTypeInfo", "GetIDsOfNames", "Invoke"]


def find_vtables(module, min_slots=3):
    """[(vtable_va, [target_va, ...])] for each pointer-run in a data section, + the code base va0."""
    from src.formats.pe import PEFile
    from src.core import cores
    pe = PEFile(cores.get("target").resolve(module))
    _, va0, _code, _sec, funcs, _sha, _imp = tracer._load(module)
    starts = {va0 + o for o in funcs}
    ib = pe.image_base
    out = []
    for s in pe.sections:
        if s["chars"] & (_CNT_CODE | _MEM_EXEC):
            continue                                          # data sections only (skip code)
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secva, i, n = ib + s["vaddr"], 0, len(raw)
        while i + 4 <= n:
            j, slots = i, []
            while j + 4 <= n:
                v = int.from_bytes(raw[j:j + 4], "little")
                if v in starts:
                    slots.append(v); j += 4
                else:
                    break
            if len(slots) >= min_slots:
                out.append((secva + i, slots)); i = j
            else:
                i += 4
    return out, va0


def _slot_name(i, ncom):
    if ncom >= 3 and i < 3:
        return _IUNKNOWN[i]
    if ncom >= 7 and i < 7:
        return _IDISPATCH[i - 3]
    return "method%d" % i


def layout(vt_va, slots, va0, names=None):
    """Emit a `struct vtbl_<va> { ... }` for one vtable. `names`: optional {off: recovered name}."""
    ncom = 7 if len(slots) >= 7 else (3 if len(slots) >= 3 else 0)   # COM prefix guess by shape
    is_com = ncom > 0
    lines = ["/* vtable at 0x%08X -- %d slots%s */" % (vt_va, len(slots),
             ", COM-shaped (IUnknown%s prefix)" % ("/IDispatch" if ncom >= 7 else "") if is_com else "")]
    lines.append("struct vtbl_%x {" % vt_va)
    for i, tgt in enumerate(slots):
        off = tgt - va0
        target = (names or {}).get(off) or "sub_%x" % off
        method = _slot_name(i, ncom)
        lines.append("    void *%-16s /* [%d] -> %s */" % (method + ";", i, target))
    lines.append("};")
    return "\n".join(lines)


def reconstruct(module, names=None):
    """Recover all vtables in a module as class-layout structs. Returns {'ok','nvtables','c'}."""
    try:
        vts, va0 = find_vtables(module)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not vts:
        return {"ok": False, "reason": "no vtables found"}
    blocks = [layout(va, slots, va0, names) for va, slots in vts]
    return {"ok": True, "nvtables": len(vts), "va0": va0,
            "slots_total": sum(len(s) for _v, s in vts), "c": "\n\n".join(blocks)}
