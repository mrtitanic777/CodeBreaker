#!/usr/bin/env python3
"""
segfix.py — DEPRECATED / SUPERSEDED.  DO NOT USE FOR NEW WORK.

Its premise was wrong.  The segments it tried to "relocate" are not mis-located
inside WIN100.BIN at all: they live in the SECOND system file, WIN100.OVL.  The
NE segment table says which file a segment is in — flag 0x0040 (PRELOAD) set =>
WIN100.BIN, clear => WIN100.OVL — and that rule holds for 64/64 code segments.
`idord.open_module()` now implements it and reads both files as one image, which
is the correct and complete fix.

Left in the tree only because its scoring helpers (_score_base / _is_prologue)
are useful as an INDEPENDENT check, and because searching the wrong file for a
plausible base is how the bogus "USER.92 = window-flag setter" name got into
ORDINALS.md — a lesson worth keeping visible.

Original docstring follows.
---------------------------------------------------------------------------
segfix.py — Robust segment locator for the embedded WIN100.BIN modules.

Some multi-segment modules (USER, GDI) have segment-table sector values that
`sector << align_shift` resolves to the WRONG bundle offset for a subset of
segments (they land on another segment's / module's bytes — the disassembly
comes out as mid-instruction garbage). Rather than trust that arithmetic, this
tool VALIDATES each segment against the NE entry table and, when it is wrong,
SEARCHES the bundle for the offset that is right.

The check is evidence-based and self-correcting: a code segment's exported
ordinals must begin with a Windows-1.0 far-proc prologue. The known prologue
byte patterns are:
    1e 58 90 45 55 8b ec   push ds; pop ax;  nop; inc bp; push bp; mov bp,sp
    8c d8 90 45 55 8b ec   mov ax,ds;        nop; inc bp; push bp; mov bp,sp
    45 55 8b ec            inc bp; push bp; mov bp,sp        (DS already in ax)
    55 8b ec               push bp; mov bp,sp                (near proc)
For a candidate base, we score how many of the segment's exported offsets land
on such a prologue; the base with the best score (and >=1 hit) wins. This finds
the true segment data no matter how the sector value was mangled.

    from segfix import open_module_fixed
    ne = open_module_fixed("USER")   # segments relocated to their true offsets
"""
import re
from idord import open_module

PROLOGUES = [
    bytes.fromhex("1e58904555"),      # push ds; pop ax; nop; inc bp; push bp
    bytes.fromhex("8cd8904555"),      # mov ax,ds; nop; inc bp; push bp
    bytes.fromhex("45558bec"),        # inc bp; push bp; mov bp,sp
    bytes.fromhex("558bec"),          # push bp; mov bp,sp
]

def _is_prologue(data, pos):
    for p in PROLOGUES:
        if data[pos:pos+len(p)] == p:
            return True
    return False

def _score_base(data, base, offsets):
    """How many exported offsets land on a prologue at this base."""
    hits = 0
    for off in offsets:
        pos = base + off
        if 0 <= pos < len(data) - 8 and _is_prologue(data, pos):
            hits += 1
    return hits

import struct
def _reloc_score(data, base, seglen):
    """DEFINITIVE validator: at the true base the per-segment relocation table
    sits at base+seglen; its record count is sane and every fixup-site offset it
    lists points at an unrelocated placeholder word (0xFFFF or 0x0000) in the
    code. Returns (matched, total) or (0,0) if the table is unreadable/insane."""
    end = base + seglen
    if end + 2 > len(data):
        return (0, 0)
    n = struct.unpack_from("<H", data, end)[0]
    if n == 0 or n > seglen // 3:                # implausible count -> wrong base
        return (0, 0)
    p = end + 2
    if p + n*8 > len(data):
        return (0, 0)
    matched = 0
    for _ in range(n):
        off = struct.unpack_from("<H", data, p+2)[0]  # fixup site offset in seg
        p += 8
        if off + 2 <= seglen:
            w = struct.unpack_from("<H", data, base+off)[0]
            if w == 0xFFFF:                       # unrelocated far-ptr placeholder
                matched += 1                      # (0x0000 excluded: zero regions)
    return (matched, n)

def open_module_fixed(modname, verbose=False):
    """Open a module and relocate any mis-placed CODE segment to the offset its
    entry-table ordinals validate against."""
    ne = open_module(modname)
    data = ne.data
    # group exported offsets by segment
    by_seg = {}
    for e in ne.entries():
        if e.get("kind") in ("FIXED", "MOVEABLE") and "offset" in e:
            by_seg.setdefault(e["seg"], []).append(e["offset"])

    ne._seg_override = {}          # seg_index -> corrected file_off
    for s in ne.segments():
        if s["kind"] != "CODE" or s["idx"] not in by_seg:
            continue
        offs = sorted(set(by_seg[s["idx"]]))
        seglen = s["length"]
        cur = s["file_off"]

        def combined(base):
            # rank by PROLOGUE first (a real exported function start is the
            # strongest signal and cannot be faked by 0xFFFF/0x0000 fill regions),
            # then relocation-placeholder count as a tiebreaker.
            pro = _score_base(data, base, offs)
            rel, _ = _reloc_score(data, base, seglen)
            return (pro, rel)

        cur_p, cur_r = combined(cur)
        if cur_p >= max(1, len(offs)//2):            # already validates
            continue
        best_base, best = cur, (cur_p, cur_r)
        maxoff = max(offs)
        for base in range(0, len(data) - max(maxoff, seglen) - 8, 16):
            sc = combined(base)
            if sc > best:
                best_base, best = base, sc
        if best_base != cur and best > (cur_p, cur_r):
            ne._seg_override[s["idx"]] = best_base
            if verbose:
                print(f"  {modname} seg{s['idx']}: {cur} -> {best_base} "
                      f"(prologue {best[0]}/{len(offs)}, reloc {best[1]})")

    # Patch segments() so the WHOLE toolchain (trace.Seg, idord) sees the
    # corrected file_offs automatically. We cache a corrected segment list and
    # return it from a bound override.
    _base_segments = ne.segments
    _corrected = None
    def segments_fixed():
        nonlocal _corrected
        if _corrected is None:
            _corrected = _base_segments()
            for idx, base in ne._seg_override.items():
                _corrected[idx-1] = dict(_corrected[idx-1], file_off=base)
        return _corrected
    ne.segments = segments_fixed
    return ne

def seg_fileoff(ne, seg_index):
    """Corrected file_off for a segment (override if segfix relocated it)."""
    ov = getattr(ne, "_seg_override", {})
    if seg_index in ov:
        return ov[seg_index]
    return ne.segments()[seg_index-1]["file_off"]


if __name__ == "__main__":
    import sys
    mod = sys.argv[1].upper() if len(sys.argv) > 1 else "USER"
    print(f"# segfix {mod}: validating/relocating segments against the entry table")
    open_module_fixed(mod, verbose=True)
