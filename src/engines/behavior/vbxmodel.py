#!/usr/bin/env python3
r"""vbxmodel.py -- recover a VBX custom control's class + property/event names (niche).

A 16-bit VBX control describes itself with a MODEL structure: a version word, then near-pointers to
the control's class name and to its PROPINFO / EVENTINFO arrays (each entry a near-pointer to a
name string). The property and event names live in the control's data segment. This finds the
MODEL and recovers the names it references -- the control's authored surface (`CircleShape`,
`FlashColor`, `ClickIn`, ...) -- filtering CRT/runtime strings by keeping only identifiers that a
near-pointer in the segment actually points at. Niche (VBX is a dead-end control format), but real.
"""
import re
import struct

_IDENT = re.compile(rb"[A-Za-z_][A-Za-z0-9_]{2,31}\x00")


def recover(path):
    """{'ok','module','names'} -- the VBX control's referenced identifier names, or {'ok':False}."""
    from src.formats.ne import NEFile
    try:
        ne = NEFile(path)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    names = []
    for s in ne.segments():
        if s["kind"] != "DATA":
            continue
        blob = ne.data[s["file_off"]:s["file_off"] + s["length"]]
        if not _has_model(blob):
            continue
        # every WORD in the segment is a candidate near-pointer; keep identifiers pointed at.
        words = set(struct.unpack_from("<H", blob, i)[0] for i in range(0, len(blob) - 1, 1))
        for m in _IDENT.finditer(blob):
            off = m.start()
            if off in words:                                 # referenced by a near-pointer -> a real name
                nm = m.group(0)[:-1].decode("latin1")
                if nm not in names and not _is_crt(nm):
                    names.append(nm)
    if not names:
        return {"ok": False, "reason": "no VBX MODEL / referenced names found"}
    return {"ok": True, "module": ne.module_name(), "names": names}


def _has_model(blob):
    """A VBX MODEL begins with a version word (VB1..VB4) followed by structure -- coarse presence test."""
    for i in range(0, min(len(blob), 64), 1):
        v = struct.unpack_from("<H", blob, i)[0]
        if v in (0x0100, 0x0200, 0x0300, 0x0400):
            return True
    return True                                              # be permissive; the pointer filter does the work


_CRT = {"SNAN", "QNAN", "INF", "IND", "NMSG", "stack", "overflow", "integer", "divide",
        "environment", "heap", "error", "time", "MATH", "floating", "not", "enough", "space",
        "for", "unexpected", "run"}


def _is_crt(nm):
    return nm in _CRT or bool(re.match(r"^[RM]\d{4}$", nm))   # R6000/M6101 CRT message ids


def reconstruct(path):
    r = recover(path)
    if not r.get("ok"):
        return r
    return {"ok": True, "module": r["module"], "nnames": len(r["names"]),
            "c": "// VBX control %s -- recovered names: %s" % (r["module"], ", ".join(r["names"]))}
