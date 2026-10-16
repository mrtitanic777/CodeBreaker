"""aout.py -- a Version 7 a.out object file for the 8086 (XENIX's C compiler), read as omf.read reads OMF.

The file: 16 bytes of header (0407h/0107h magic, text, data, bss and symbol sizes, the entry, a
word, and whether there is relocation), the text, the data, then - unless the last header word
says none - one relocation word for every word of the text and of the data (0: none), then the
symbols (8 characters, a type word, a value word).

A relocation word: bit 0 set, the word is relative to the place after it (a call or jump); bits
1-3 what it names (1 the text, 2 the data, 3 the bss, 4 an external symbol, bits 4-14 its number);
bit 15 set, the operand begins one byte on (the 8086 has operands at odd places too).

    o = aout.read(data)       o.seg o.segs o.filled o.pubs o.ext o.comm o.fix o.raw   (as omf.read)
        o.fix kinds: 'off' a word that holds a place, 'rel' a place relative to the word's end;
        targets ('seg', '_TEXT'|'_DATA'|'_BSS') or ('ext', name). A relative word's own value in
        the object counts from the start of the text, so its displacement is set to make it count
        as an OMF self-relative fixup's would.
"""
import struct

MAGICS = (0o407, 0o410, 0o411, 0x0107, 0x0109)


def is_aout(data):
    return len(data) >= 16 and struct.unpack_from("<H", data, 0)[0] in MAGICS


class Obj(object):
    pass


def read(data):
    magic, tsz, dsz, bsz, ssz, entry, _, norel = struct.unpack_from("<8H", data, 0)
    o = Obj()
    text = bytes(data[16:16 + tsz])
    dat = bytes(data[16 + tsz:16 + tsz + dsz])
    o.seg = {"_TEXT": text, "_DATA": dat, "_BSS": bytes(bsz)}
    o.filled = {"_TEXT": tsz, "_DATA": dsz}
    o.segs = [("_TEXT", "CODE", tsz, 1), ("_DATA", "DATA", dsz, 2), ("_BSS", "BSS", bsz, 2)]
    o.groups = {}
    p = 16 + tsz + dsz
    rel_t = rel_d = []
    if not norel:
        rel_t = [struct.unpack_from("<H", data, p + k)[0] for k in range(0, tsz, 2)]
        rel_d = [struct.unpack_from("<H", data, p + tsz + k)[0] for k in range(0, dsz, 2)]
        p += tsz + dsz
    syms = []
    for q in range(p, p + ssz, 12):
        nm = data[q:q + 8].split(b"\0")[0].decode("latin1")
        ty, val = struct.unpack_from("<HH", data, q + 8)
        syms.append((nm, ty, val))
    o.syms = syms
    o.pubs, o.ext, o.comm = [], [], {}
    seg_of = {2: "_TEXT", 3: "_DATA", 4: "_BSS"}
    for nm, ty, val in syms:
        if ty & 0x20 and ty & 7 in seg_of:
            off = val - (tsz if ty & 7 == 3 else tsz + dsz if ty & 7 == 4 else 0)
            o.pubs.append((nm, seg_of[ty & 7], off))
        elif ty & 0x20 and ty & 7 == 0:
            o.ext.append(nm)
            if val:
                o.comm[nm] = (val, False)               # (an external with a size: a communal variable)
    o.fix = []
    for segname, rels in (("_TEXT", rel_t), ("_DATA", rel_d)):
        for j, v in enumerate(rels):
            if not v:
                continue
            k = 2 * j + (v >> 15)
            kind = (v >> 1) & 7
            pcrel = v & 1
            if kind == 4:
                tgt = ("ext", syms[(v & 0x7FFF) >> 4][0])
            else:
                tgt = ("seg", {1: "_TEXT", 2: "_DATA", 3: "_BSS"}.get(kind, "?"))
            disp = 0
            if kind in (2, 3):                              # (the word counts from the start of the text)
                disp = -(tsz if kind == 2 else tsz + dsz)
            if pcrel:
                o.fix.append((segname, k, "rel", tgt, k + 2))
            else:
                o.fix.append((segname, k, "off", tgt, disp))
    o.raw = bytes(data)
    return o
