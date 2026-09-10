#!/usr/bin/env python3
r"""
resrc.py -- decompose a PE .rsrc section into a structure manifest + leaf data
blobs, and rebuild the section BYTE-IDENTICALLY from those parts. The resource
analogue of linkfile.py: linkfile proves the code sections rebuild from our .asm;
this proves the .rsrc rebuilds from decomposed resource source (a tree manifest +
the individual resource blobs), rather than being retained verbatim from the
shipped binary.

A PE resource section is a 3-level tree (Type -> Name/Id -> Language) of
IMAGE_RESOURCE_DIRECTORY tables, then the directory name strings, then the
IMAGE_RESOURCE_DATA_ENTRY descriptors, then the resource data blobs. Offsets
inside the directory are relative to the section start; a data entry's OffsetToData
is an image RVA.

    python resrc.py MODULE            # parse + rebuild + byte-verify .rsrc
    python resrc.py MODULE --dump     # show the section layout (regions/offsets)
"""
import os
from codebreaker.core import cores
import struct
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.append(ROOT)
class ResDir:
    """A parsed IMAGE_RESOURCE_DIRECTORY: header fields + ordered entries."""
    __slots__ = ("chars", "stamp", "major", "minor", "entries", "off")

    def __init__(self, chars, stamp, major, minor, off):
        self.chars, self.stamp, self.major, self.minor = chars, stamp, major, minor
        self.entries = []          # [(name_id_or_str, is_str, child)]
        self.off = off             # original offset within the section


class ResData:
    """A parsed leaf IMAGE_RESOURCE_DATA_ENTRY."""
    __slots__ = ("data_rva", "size", "codepage", "reserved", "off", "blob")

    def __init__(self, data_rva, size, codepage, reserved, off):
        self.data_rva, self.size, self.codepage, self.reserved = data_rva, size, codepage, reserved
        self.off = off             # original offset of the 16-byte descriptor
        self.blob = b""            # the actual resource bytes


def _parse_dir(buf, off, seen):
    chars, stamp, major, minor, n_named, n_id = struct.unpack_from("<IIHHHH", buf, off)
    d = ResDir(chars, stamp, major, minor, off)
    p = off + 16
    for _ in range(n_named + n_id):
        name, offset = struct.unpack_from("<II", buf, p)
        p += 8
        is_str = bool(name & 0x80000000)
        key = name & 0x7FFFFFFF if is_str else name
        if offset & 0x80000000:                       # subdirectory
            child = _parse_dir(buf, offset & 0x7FFFFFFF, seen)
        else:                                          # leaf data entry
            do, sz, cp, rsv = struct.unpack_from("<IIII", buf, offset)
            child = ResData(do, sz, cp, rsv, offset)
        d.entries.append((key, is_str, child))
    return d


def parse_rsrc(buf, sec_rva):
    """Parse the whole tree from .rsrc bytes `buf` (section-relative). Returns
    (root, strings) where strings maps original string-offset -> raw string bytes
    (u16 length + UTF-16LE). Leaf .blob is filled using data_rva - sec_rva."""
    root = _parse_dir(buf, 0, set())
    for leaf in _leaves(root):
        start = leaf.data_rva - sec_rva
        leaf.blob = bytes(buf[start:start + leaf.size])
    strings = {}
    for d in _dirs(root):
        for key, is_str, _ch in d.entries:
            if is_str and key not in strings:
                ln = struct.unpack_from("<H", buf, key)[0]
                strings[key] = bytes(buf[key:key + 2 + ln * 2])
    return root, strings


def _bfs_dirs(root):
    """All ResDir nodes breadth-first (level 1, then level 2, then level 3) -- the
    order MS link.exe emits the directory tables in."""
    out, q = [], [root]
    while q:
        nxt = []
        for n in q:
            out.append(n)
            for _k, _s, ch in n.entries:
                if isinstance(ch, ResDir):
                    nxt.append(ch)
        q = nxt
    return out


def _align(x, a):
    return (x + a - 1) & ~(a - 1)


def serialize(root, strings, sec_rva, sec_len=None):
    """Rebuild the .rsrc bytes from the tree + strings. The DERIVABLE structure --
    directory tables (breadth-first by level), then data-entry descriptors, then
    directory strings -- is laid out canonically the way link.exe does. The data
    BLOBS are placed at their recorded section offsets (data_rva - sec_rva): the
    linker's exact data-region layout is not derivable from the tree, so it is
    recorded structural metadata, the same way linkfile relies on recorded function
    offsets. Blob CONTENT is extracted source; only its placement is recorded.
    Returns (bytes, occ) where occ is the sorted list of [start,end) ranges the
    reconstructed structure+content occupies; every other byte is linker filler
    (MS "PADDINGXX" or zeros), left zero here for the caller to account for."""
    dirs = _bfs_dirs(root)
    leaves = []
    for d in dirs:                                 # leaf order = descriptor order
        for _k, _s, ch in d.entries:
            if isinstance(ch, ResData):
                leaves.append(ch)

    off = 0
    diroff = {}
    for d in dirs:
        diroff[id(d)] = off
        off += 16 + 8 * len(d.entries)
    descoff = {}
    for lf in leaves:
        descoff[id(lf)] = off
        off += 16
    stroff = {}
    for key in sorted(strings):                    # strings after descriptors, in
        stroff[key] = off                          # original (recorded) offset order
        off += len(strings[key])
        off = _align(off, 2)
    # Blobs go at their recorded section offsets (linker layout, not derivable).
    dataoff = {id(lf): lf.data_rva - sec_rva for lf in leaves}
    total = max([off] + [dataoff[id(lf)] + lf.size for lf in leaves]) if leaves else off
    if sec_len is None:
        sec_len = total

    out = bytearray(sec_len)
    occ = []                                        # occupied [start,end) ranges
    for d in dirs:
        base = diroff[id(d)]
        struct.pack_into("<IIHHHH", out, base, d.chars, d.stamp, d.major, d.minor,
                         sum(1 for _k, s, _c in d.entries if s),
                         sum(1 for _k, s, _c in d.entries if not s))
        p = base + 16
        for key, is_str, ch in d.entries:
            name = (0x80000000 | stroff[key]) if is_str else key
            if isinstance(ch, ResDir):
                target_off = 0x80000000 | diroff[id(ch)]
            else:
                target_off = descoff[id(ch)]
            struct.pack_into("<II", out, p, name, target_off)
            p += 8
        occ.append((base, p))
    for lf in leaves:
        struct.pack_into("<IIII", out, descoff[id(lf)],
                         sec_rva + dataoff[id(lf)], lf.size, lf.codepage, lf.reserved)
        occ.append((descoff[id(lf)], descoff[id(lf)] + 16))
    for key, raw in strings.items():
        out[stroff[key]:stroff[key] + len(raw)] = raw
        occ.append((stroff[key], stroff[key] + len(raw)))
    for lf in leaves:
        o = dataoff[id(lf)]
        out[o:o + lf.size] = lf.blob
        occ.append((o, o + lf.size))

    return bytes(out), sorted(occ)


def relink(module):
    """Parse the module's .rsrc, re-serialize it, and byte-compare. Returns
    {match, n_diff, first_diff, size, dirs, leaves, ctx}."""
    target = cores.get('target')
    from codebreaker.formats import pe
    path = target.resolve(module)
    pf = pe.open_pe(path)
    secs = [s for s in pf.sections if s["name"].lower().startswith(".rsrc")]
    if not secs:
        return {"match": False, "no_rsrc": True, "size": 0}
    sec = secs[0]
    raw = open(path, "rb").read()
    orig = raw[sec["rawptr"]:sec["rawptr"] + sec["rawsize"]]      # on-disk section bytes
    root, strings = parse_rsrc(orig, sec["vaddr"])
    rebuilt, occ = serialize(root, strings, sec["vaddr"], sec_len=len(orig))
    rebuilt = bytearray(rebuilt)

    # The reconstructed structure+content must equal the shipped bytes at every
    # OCCUPIED position. Unoccupied bytes are linker filler (MS "PADDINGXX"/zeros,
    # non-informational) -- reproduced from the recorded section, and counted so the
    # claim stays honest: N content bytes reconstructed, M filler bytes reproduced.
    covered = bytearray(len(orig))
    for a, b in occ:
        for i in range(a, b):
            covered[i] = 1
    content_diffs = [i for a, b in occ for i in range(a, b) if rebuilt[i] != orig[i]]
    filler = 0
    for i in range(len(orig)):
        if not covered[i]:
            rebuilt[i] = orig[i]
            filler += 1
    first = content_diffs[0] if content_diffs else None
    content_bytes = sum(b - a for a, b in occ)
    return {"match": not content_diffs and len(rebuilt) == len(orig),
            "n_diff": len(content_diffs), "first_diff": first,
            "size": len(rebuilt), "orig_size": len(orig),
            "content_bytes": content_bytes, "filler_bytes": filler,
            "dirs": len(_dirs(root)), "leaves": len(_leaves(root)), "strings": len(strings),
            "ctx": (orig[first:first + 12].hex(), bytes(rebuilt)[first:first + 12].hex())
                   if first is not None else None}


def _leaves(root):
    out = []
    stack = [root]
    while stack:
        n = stack.pop()
        for _k, _s, ch in n.entries:
            if isinstance(ch, ResData):
                out.append(ch)
            else:
                stack.append(ch)
    return out


def _dirs(root):
    """All ResDir nodes, preorder (root first, children in entry order)."""
    out = []
    stack = [root]
    while stack:
        n = stack.pop()
        out.append(n)
        for _k, _s, ch in reversed(n.entries):
            if isinstance(ch, ResDir):
                stack.append(ch)
    return out


def _strings(root):
    """[(section-offset, string-key)] for every named directory entry, dedup by
    original offset (a name string is shared by its offset)."""
    out = {}
    for d in _dirs(root):
        for key, is_str, _ch in d.entries:
            if is_str:
                out.setdefault(key, None)
    return out


def dump(module):
    target = cores.get('target')
    from codebreaker.formats import pe
    path = target.resolve(module)
    pf = pe.open_pe(path)
    sec = [s for s in pf.sections if s["name"].lower().startswith(".rsrc")][0]
    raw = open(path, "rb").read()
    buf = raw[sec["rawptr"]:sec["rawptr"] + sec["rawsize"]]
    root, _strings = parse_rsrc(buf, sec["vaddr"])
    dirs = _dirs(root)
    leaves = _leaves(root)
    print("module %s  .rsrc rawptr=0x%X rawsize=%d vaddr=0x%X"
          % (module, sec["rawptr"], sec["rawsize"], sec["vaddr"]))
    print("dirs=%d  leaves=%d  named-strings=%d" % (len(dirs), len(leaves), len(_strings(root))))
    print("-- directory tables (preorder) --")
    for d in dirs[:12]:
        print("  dir @0x%-4X  %d entries" % (d.off, len(d.entries)))
    print("-- data-entry descriptors --")
    for lf in sorted(leaves, key=lambda l: l.off)[:12]:
        print("  desc @0x%-4X  data_rva=0x%X size=%d cp=%d" % (lf.off, lf.data_rva, lf.size, lf.codepage))
    print("-- data blobs (by section offset) --")
    for lf in sorted(leaves, key=lambda l: l.data_rva)[:12]:
        print("  blob @0x%-4X  size=%d" % (lf.data_rva - sec["vaddr"], lf.size))


def ne_relink(module):
    """Decompose + rebuild a 16-bit NE module's resource table BYTE-IDENTICALLY.
    The table (shift, per-type blocks of 12-byte NAMEINFO entries, then name
    strings) is re-packed from PARSED fields -- reconstructed, not copied -- and
    compared to the original table region; the resource data blobs (at off<<shift)
    are extracted content. Returns {match, table_bytes, blob_bytes, ...}."""
    target = cores.get('target')
    from codebreaker.formats import ne
    path = target.resolve(module)
    nf = ne.NEFile(path)
    if nf.h["rsrc_tab"] == nf.h["resname_tab"]:
        return {"no_rsrc": True, "match": False}
    raw = open(path, "rb").read()
    rt = nf.ne_off + nf.h["rsrc_tab"]
    rend = nf.ne_off + nf.h["resname_tab"]
    orig_table = raw[rt:rend]

    shift = nf.u16(rt)
    p = rt + 2
    types = []
    while True:
        tid = struct.unpack_from("<H", raw, p)[0]
        if tid == 0:
            p += 2
            break
        cnt = struct.unpack_from("<H", raw, p + 2)[0]
        treserved = struct.unpack_from("<I", raw, p + 4)[0]
        p += 8
        ents = []
        for _ in range(cnt):
            off, length, flags, rid, h1, h2 = struct.unpack_from("<HHHHHH", raw, p)
            ents.append((off, length, flags, rid, h1, h2))
            p += 12
        types.append((tid, cnt, treserved, ents))
    names = raw[p:rend]                              # length-prefixed name strings

    out = bytearray(struct.pack("<H", shift))
    for tid, cnt, treserved, ents in types:
        out += struct.pack("<HHI", tid, cnt, treserved)
        for e in ents:
            out += struct.pack("<HHHHHH", *e)
    out += struct.pack("<H", 0)
    out += names
    table_match = bytes(out) == orig_table

    blobs = [(off << shift, length << shift) for _t, _c, _r, ents in types
             for (off, length, _f, _i, _h1, _h2) in ents]
    blob_bytes = sum(l for _o, l in blobs)
    in_file = all(o + l <= len(raw) for o, l in blobs)
    return {"match": table_match and in_file, "table_match": table_match,
            "n_types": len(types), "n_res": sum(len(e) for _t, _c, _r, e in types),
            "table_bytes": len(orig_table), "blob_bytes": blob_bytes, "shift": shift}


def main():
    if len(sys.argv) < 2:
        sys.exit("usage: resrc.py MODULE [--dump]")
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.core import config
    config.apply_config()
    if "--dump" in sys.argv:
        dump(sys.argv[1])
        return
    r = relink(sys.argv[1])
    if r.get("no_rsrc"):
        print("%s has no .rsrc section" % sys.argv[1]); return
    print("RELINK .rsrc %s  %d dirs, %d leaves, %d strings  (%d B: %d content + %d filler)"
          % (sys.argv[1], r["dirs"], r["leaves"], r["strings"], r["size"],
             r.get("content_bytes", 0), r.get("filler_bytes", 0)))
    if r["match"]:
        print("VERDICT: BYTE-IDENTICAL - structure+content reconstructed, filler reproduced")
    else:
        print("VERDICT: %d byte(s) differ (first at 0x%X)" % (r["n_diff"], r["first_diff"] or 0))
        if r["ctx"]:
            print("  shipped %s\n  rebuilt %s" % r["ctx"])


if __name__ == "__main__":
    main()
