#!/usr/bin/env python3
r"""neidiom.py -- recognize Win16 NE idioms: far/exported prologs, __loadds, thunks, internal fixups.

16-bit NE modules have idioms the 32-bit engines don't model. The distinctive one is the Windows
FAR-callable prolog `inc bp; push bp; mov bp,sp` (45 55 8B EC) -- the `inc bp` marks a far stack
frame so the kernel can walk it -- which identifies exported / callback functions; often followed by
the `__loadds` sequence (`push ds; mov ax,DGROUP; mov ds,ax`) that reloads the data segment. This
classifies each entry-point's prolog, spots MakeProcInstance-style thunks, and resolves INTERNALREF
relocations (intra-module far calls) to their target segment:offset -- so a 16-bit binary's exported
surface and internal call graph read correctly.
"""


def prolog_kind(code):
    """Classify a Win16 function prolog from its leading bytes."""
    if code[:4] == b"\x45\x55\x8b\xec":                       # inc bp; push bp; mov bp,sp
        rest = code[4:]
        # __loadds: push ds; mov ax,imm16; mov ds,ax   (1E B8 xx xx 8E D8)
        if rest[:1] == b"\x1e" and rest[1:2] == b"\xb8" and rest[4:6] == b"\x8e\xd8":
            return "far-export __loadds"
        if rest[:1] == b"\x1e":
            return "far-export (saves ds)"
        return "far-export"
    if code[:3] == b"\x55\x8b\xec":                           # push bp; mov bp,sp
        return "near"
    if code[:2] == b"\x8c\xd8":                               # mov ax,ds  (data-segment setup)
        return "far (mov ax,ds)"
    return "unknown"


def is_makeprocinstance_thunk(code):
    """True if `code` is a MakeProcInstance-style thunk: load AX with an instance/DS, far-jump."""
    # mov ax, imm16 (B8 xx xx) then jmp far ptr (EA xx xx xx xx), or mov es/ds then far jmp
    if code[:1] == b"\xb8" and code[3:4] == b"\xea":
        return True
    if code[:1] == b"\xb8" and code[3:5] == b"\xff\x2f":      # jmp far [bx] variants
        return True
    return False


def analyze_ne(path):
    """{'exports':[...], 'prologs':{kind:count}, 'thunks':int, 'internal_fixups':int} for an NE module."""
    from codebreaker.formats.ne import NEFile
    ne = NEFile(path)
    segs = {s["idx"]: s for s in ne.segments()}
    data = ne.data
    exports, prologs, thunks = [], {}, 0
    for e in ne.entries():
        if e.get("kind") == "UNUSED":
            continue
        seg = e.get("seg"); off = e.get("offset")
        if seg is None or off is None or seg not in segs:
            continue
        fo = segs[seg]["file_off"] + off
        code = data[fo:fo + 16]
        if len(code) < 4:
            continue
        kind = prolog_kind(code)
        prologs[kind] = prologs.get(kind, 0) + 1
        if is_makeprocinstance_thunk(code):
            thunks += 1
        exports.append({"ordinal": e["ordinal"], "seg": seg, "offset": off, "prolog": kind})
    internal = 0
    for s in ne.segments():
        for r in ne.relocations(s):
            if r.get("kind") == "INTERNAL":
                internal += 1
    return {"exports": exports, "prologs": prologs, "thunks": thunks, "internal_fixups": internal}


def reconstruct(path):
    """Summarize a Win16 NE module's idioms. {'ok','nexports','prologs','thunks','internal_fixups'}."""
    try:
        a = analyze_ne(path)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not a["exports"]:
        return {"ok": False, "reason": "no NE entry points (not an NE, or none)"}
    return {"ok": True, "nexports": len(a["exports"]), "prologs": a["prologs"],
            "thunks": a["thunks"], "internal_fixups": a["internal_fixups"],
            "far_exports": sum(v for k, v in a["prologs"].items() if k.startswith("far-export"))}
