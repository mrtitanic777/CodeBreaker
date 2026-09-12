#!/usr/bin/env python3
r"""datainit.py -- recover initialized global DATA (pointer tables, vtables, string arrays) as C.

Code is only half a module; its initialized data is source too. The base relocations mark exactly
which data words are absolute pointers -- so every function-pointer table, vtable, and string-
pointer array is locatable. This groups consecutive relocated pointer slots into arrays, classifies
each pointer (code -> sub_<off>, string -> "text", other data -> &data_<va>), and emits C
initializers: `void *g_<va>[] = { sub_a, "Cannot open %s", &data_c, ... };`.
"""
import struct


def reconstruct(module, max_items=4000):
    from codebreaker.core import cores
    from codebreaker.formats.pe import PEFile
    from codebreaker.engines.behavior import datarec
    path = cores.get("target").resolve(module)
    pe = PEFile(path)
    ib = pe.image_base
    relocs = pe.base_relocs()
    if not relocs:
        return {"ok": False, "reason": "no base relocations (no pointer data to recover)"}

    code_lo = code_hi = None
    dat = []
    for s in pe.sections:
        lo = s["vaddr"]; hi = lo + max(s["vsize"], s["rawsize"])
        if s["chars"] & 0x20000000:                          # MEM_EXECUTE
            code_lo, code_hi = lo, hi
        dat.append((lo, hi))

    def classify(v):                                         # v is an absolute VA
        rva = v - ib
        if code_lo is not None and code_lo <= rva < code_hi:
            return "sub_%x" % rva
        lit = datarec.string_at(module, v)
        if lit:
            return ('L"%s"' if lit[0] == "W" else '"%s"') % datarec._esc(lit[1][:60])
        for lo, hi in dat:
            if lo <= rva < hi:
                return "&data_%x" % v
        return "0x%x" % v

    rvas = sorted(r for r in relocs if pe._rva_to_off(r) is not None)[:max_items]
    # group consecutive 4-aligned relocated slots into arrays
    groups, i = [], 0
    while i < len(rvas):
        j = i
        while j + 1 < len(rvas) and rvas[j + 1] == rvas[j] + 4:
            j += 1
        groups.append(rvas[i:j + 1]); i = j + 1

    lines = ["// Reconstructed initialized data -- %s\n" % module]
    narr = nptr = 0
    for g in groups:
        vals = []
        for r in g:
            off = pe._rva_to_off(r)
            vals.append(classify(struct.unpack_from("<I", pe.data, off)[0]))
        va0 = ib + g[0]
        if len(vals) >= 2:
            narr += 1; nptr += len(vals)
            lines.append("void *g_%x[] = { %s };" % (va0, ", ".join(vals)))
        else:
            nptr += 1
            lines.append("void *g_%x = %s;" % (va0, vals[0]))
    return {"ok": True, "c": "\n".join(lines), "pointers": nptr, "arrays": narr}
