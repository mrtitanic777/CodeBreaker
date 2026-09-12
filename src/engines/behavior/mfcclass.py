#!/usr/bin/env python3
r"""mfcclass.py -- recover class names + inheritance from MFC CRuntimeClass structures.

Every MFC class declared with DECLARE_DYNAMIC/DYNCREATE/SERIAL emits a static CRuntimeClass:
{ const char* m_lpszClassName; int m_nObjectSize; UINT m_wSchema; CObject* (*pfnCreate)();
CRuntimeClass* m_pBaseClass; ... }. That structure names the class, its object size, its
serialization schema, and -- via m_pBaseClass -> another CRuntimeClass -- its base class. So it
recovers the class hierarchy WITHOUT RTTI (which this era's binaries lack). Base pointers that
resolve to other recovered CRuntimeClass structs cross-validate the whole set, which keeps the
detection precise. The MFC-specific complement to the rtti engine.
"""
import struct

_CNT_CODE, _MEM_EXEC = 0x20, 0x20000000


def _cstr(pe, va, maxlen=96):
    off = pe._rva_to_off(va - pe.image_base)
    if off is None:
        return None
    end = pe.data.find(b"\x00", off, off + maxlen)
    if end <= off:
        return None
    s = pe.data[off:end]
    return s.decode("latin1") if all(32 <= b < 127 for b in s) else None


def find_runtime_classes(module):
    """{va: {'name','size','schema','base'}} for CRuntimeClass structs (base cross-validated)."""
    from src.formats.pe import PEFile
    from src.core import cores
    pe = PEFile(cores.get("target").resolve(module))
    ib = pe.image_base
    cands = {}
    for s in pe.sections:
        if s["chars"] & (_CNT_CODE | _MEM_EXEC):
            continue
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secva, n, p = ib + s["vaddr"], len(raw), 0
        while p + 24 <= n:
            name_ptr, size, schema = struct.unpack_from("<III", raw, p)
            nm = _cstr(pe, name_ptr) if name_ptr > ib else None
            if nm and nm[:1].isalpha() and 1 <= size <= 0x8000 \
                    and (schema <= 0xFFFF or schema == 0xFFFFFFFF):
                b16, b20 = struct.unpack_from("<II", raw, p + 16)
                cands[secva + p] = {"name": nm, "size": size, "schema": schema,
                                    "_b16": b16, "_b20": b20}
                p += 4
            else:
                p += 4
    for va, c in cands.items():                               # resolve base via cross-validation
        base_va = c["_b16"] if c["_b16"] in cands else (c["_b20"] if c["_b20"] in cands else None)
        c["base"] = cands[base_va]["name"] if base_va else None
        c.pop("_b16", None); c.pop("_b20", None)
    return cands


def reconstruct(module):
    """Recover MFC classes + inheritance from CRuntimeClass structs. {'ok','nclasses','c'}."""
    try:
        rc = find_runtime_classes(module)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    # keep only structs that participate in the hierarchy (named base, or is someone's base) -- this
    # filters coincidental {ptr,int,int} triples, since a real CRuntimeClass links into the graph.
    names = {c["name"] for c in rc.values()}
    bases = {c["base"] for c in rc.values() if c.get("base")}
    good = {va: c for va, c in rc.items() if c.get("base") or c["name"] in bases}
    if not good:
        return {"ok": False, "reason": "no linked CRuntimeClass structures (not MFC, or stripped)"}
    lines = []
    for va, c in sorted(good.items()):
        inh = " : public %s" % c["base"] if c.get("base") else ""
        sch = "" if c["schema"] in (0xFFFF, 0xFFFFFFFF) else " schema %d" % c["schema"]
        lines.append("class %s%s { /* CRuntimeClass @0x%08X, sizeof=%d%s */ };"
                     % (c["name"], inh, va, c["size"], sch))
    return {"ok": True, "nclasses": len(good), "classes": [good[v] for v in sorted(good)],
            "c": "\n".join(lines)}
