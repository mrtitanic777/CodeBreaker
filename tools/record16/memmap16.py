"""Find a 16-bit module's segments in a dump of a running standard-mode Windows (Setup's
mini-Windows: protected mode, paging off, so linear = physical).

    segs = memmap16.locate(dump_bytes, ldt_base, ldt_limit, ne_path)
      -> {segment number: {"selector": s, "base": linear, "limit": n}}

A loaded code segment is the file's bytes with its fix-ups patched in: it is recognised by the
bytes no fix-up touches. The module's data segment (written to as it runs) by its own bytes
likewise, with a looser bar."""
import struct
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from codebreaker.formats.ne import NEFile


def descriptors(dump, ldt_base, ldt_limit):
    """[(selector, base, limit, access)] of the LDT's present data/code descriptors."""
    out = []
    for i in range((ldt_limit + 1) // 8):
        d = dump[ldt_base + 8 * i: ldt_base + 8 * i + 8]
        if len(d) < 8:
            break
        lim = d[0] | d[1] << 8 | (d[6] & 0x0F) << 16
        base = d[2] | d[3] << 8 | d[4] << 16 | d[7] << 24
        acc = d[5]
        if acc & 0x80 and acc & 0x10:                       # present, code or data
            out.append(((i << 3) | 7, base, lim, acc))     # (an LDT selector, RPL 3)
    return out


def _fixup_mask(nef, seg):
    """The offsets of `seg` its fix-ups patch (each site and its chain), as a set."""
    data = nef.data[seg["file_off"]:seg["file_off"] + seg["length"]]
    width = {0: 1, 2: 2, 3: 4, 5: 2, 11: 6, 13: 4}
    out = set()
    for r in nef.relocations(seg):
        o, n = r["offset"], 0
        sites = [o]
        if not r["additive"]:
            sites = []
            while o != 0xFFFF and o < len(data) - 1 and n < 4000:
                sites.append(o)
                o = data[o] | data[o + 1] << 8
                n += 1
        for s in sites:
            out.update(range(s, s + width.get(r["src"], 2)))
    return out


def signatures(ne_path, sample=192):
    """{segment: (length, [(offset, byte)])} -- up to `sample` bytes no fix-up touches."""
    nef = NEFile(ne_path)
    out = {}
    for seg in nef.segments():
        if not seg["file_off"] or not seg["length"]:
            continue
        data = nef.data[seg["file_off"]:seg["file_off"] + seg["length"]]
        mask = _fixup_mask(nef, seg)
        step = max(1, len(data) // (sample * 2))
        picks = [(o, data[o]) for o in range(0, len(data), step) if o not in mask][:sample]
        out[seg["idx"]] = (seg["length"], seg["kind"], picks)
    return out


def locate(dump, ldt_base, ldt_limit, ne_path, sigs=None):
    sigs = sigs or signatures(ne_path)
    descs = descriptors(dump, ldt_base, ldt_limit)
    found = {}
    for sg, (length, kind, picks) in sigs.items():
        best = None
        for sel, base, lim, acc in descs:
            if lim + 1 < length or base + length > len(dump):
                continue
            hit = sum(1 for o, b in picks if dump[base + o] == b)
            score = hit / max(1, len(picks))
            if best is None or score > best[0]:
                best = (score, sel, base, lim)
        need = 0.98 if kind == "CODE" else 0.6
        if best and best[0] >= need:
            found[sg] = {"selector": best[1], "base": best[2], "limit": best[3], "score": round(best[0], 3)}
    return found
