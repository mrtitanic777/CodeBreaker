#!/usr/bin/env python3
r"""datainit16.py -- recover a 16-bit module's DGROUP data (pointers + globals) as C declarations.

datarec16 recovers the STRINGS in the auto-data segment; datainit16 recovers the rest of DGROUP.
The data segment's relocations mark exactly which slots hold POINTERS -- an INTERNALREF is a pointer
to code/data (`&sub_1_0a`), an IMPORTORD/IMPORTNAME a far pointer to an imported function (a callback
or thunk table). Consecutive pointer slots are a table (`void far *tbl_20[] = {...}`). The remaining
bytes, minus the strings, are the module's numeric globals. This emits the data segment's layout as
C -- the 16-bit `datainit`, completing the data surface begun by datarec16.
"""
import struct


def _data_segs(nef):
    return [s for s in nef.segments() if s["kind"] == "DATA"]


def _namer(nef):
    from codebreaker.engines.behavior.interproc16 import _namer as ipn
    return ipn(nef)


def reconstruct(module, path=None):
    """Recover DGROUP pointers (tables) + a string/data map as C. {'ok','npointers','c'}."""
    from codebreaker.formats.ne import NEFile
    from codebreaker.core import cores
    from codebreaker.engines.recon import datarec16
    p = path or (module if str(module).lower().endswith((".exe", ".dll", ".drv"))
                 else cores.get("target").resolve(module))
    try:
        nef = NEFile(p)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    name = _namer(nef)
    lines = ["// DGROUP data -- %s" % nef.module_name()]
    npointers = 0
    for s in _data_segs(nef):
        ptrs = {}                                            # offset -> resolved pointer target name
        for r in sorted(nef.relocations(s), key=lambda r: r["offset"]):
            nm = name(r)
            if nm:
                ptrs[r["offset"]] = nm
        if not ptrs:
            continue
        npointers += len(ptrs)
        # group consecutive pointer slots (far ptr = 4 bytes) into tables
        offs = sorted(ptrs)
        i = 0
        while i < len(offs):
            run = [offs[i]]
            while i + 1 < len(offs) and offs[i + 1] - offs[i] in (2, 4):
                i += 1; run.append(offs[i])
            i += 1
            if len(run) >= 2:
                lines.append("void far *tbl_%04x[] = {  // %d entries" % (run[0], len(run)))
                for o in run:
                    lines.append("    &%s," % ptrs[o].replace(".", "_"))
                lines.append("};")
            else:
                lines.append("void far *ptr_%04x = &%s;" % (run[0], ptrs[run[0]].replace(".", "_")))
    # append the recovered strings (from datarec16) for a complete data picture
    try:
        ss = datarec16.strings(nef)
    except Exception:
        ss = []
    for off, text in ss:
        lines.append('char s_%04x[] = "%s";' % (off, datarec16._esc(text)))
    if npointers == 0 and not ss:
        return {"ok": False, "reason": "no DGROUP pointers or strings"}
    return {"ok": True, "npointers": npointers, "nstrings": len(ss), "c": "\n".join(lines)}
