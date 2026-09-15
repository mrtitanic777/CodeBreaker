#!/usr/bin/env python3
r"""nerecon.py -- reconstruction organizer for the Win95 16-bit NE GUI core (KRNL386/GDI/USER).

The NE (New Executable) GUI subsystem is the layer between the real-mode DOS base and the desktop.
Unlike the flat MZ/.SYS binaries, an NE module is already segmented (an explicit segment table) and
it EXPORTS its functions by name (the resident/non-resident name tables + the entry table map an
ordinal to a segment:offset). That gives us the AUTHENTIC authored names straight from the binary --
GlobalAlloc, LocalAlloc, BitBlt, TextOut, CreateWindow, SendMessage, ... -- not inferred labels.

This mirrors dosrecon for NE: parse the NE structure, name every exported function authentically,
decode + name the rest, emit one MASM SEGMENT per NE segment (multi-segment, as built), and drive
toward assemble + link (LINK 5.60 emits NE) + byte-verify.

    wt --target win95 nerecon [--krnl386|--gdi|--user|--all] [--build]

Read-only against the corpus; writes under design/recon/ne/.
"""
import os
import re
import sys
import struct

from src.core import paths

try:
    import capstone
except ImportError:
    capstone = None

_MODS = {
    "krnl386": ("krnl386.exe", "KRNL386.EXE -- the Win95 16-bit KERNEL (memory, tasks, modules, files)"),
    "gdi":     ("gdi.exe",     "GDI.EXE -- the Win95 16-bit Graphics Device Interface"),
    "user":    ("user.exe",    "USER.EXE -- the Win95 16-bit window/message/input manager"),
}


def _win95_root():
    root = paths.design_root()
    return os.path.dirname(root) if os.path.basename(root) == "design" else \
        os.path.join(os.path.dirname(paths.repo_root()), "Windows95")


def _corpus(name):
    root = paths.design_root()
    if not os.path.isdir(os.path.join(root, "corpus")):
        root = os.path.join(_win95_root(), "design")
    for dp, _dn, fn in os.walk(os.path.join(root, "corpus")):
        for f in fn:
            if f.lower() == name.lower():
                return os.path.join(dp, f)
    return None


# authentic ordinal->name export tables live as .def files in the Win95 tree
_DEFS = {
    "krnl386": ("core", "kernel", "KRNL386", "kernel.def"),
    "gdi":     ("core", "gdi", "gdi.def"),
    "user":    ("core", "user", "user.def"),
}


def _load_def(which):
    """Parse a module .def EXPORTS section -> {ordinal: Name}. Authentic Win16 API names."""
    parts = _DEFS.get(which)
    if not parts:
        return {}
    p = os.path.join(_win95_root(), *parts)
    if not os.path.isfile(p):
        return {}
    out = {}
    for line in open(p, encoding="latin1", errors="replace"):
        m = re.search(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s+@(\d+)", line)
        if m:
            out[int(m.group(2))] = m.group(1)
    return out


class NE:
    """Parsed NE structure: segments, exports (ordinal->name and ordinal->(seg,off)), imports."""

    def __init__(self, data):
        self.data = data
        self.nepos = struct.unpack("<I", data[0x3c:0x40])[0]
        ne = data[self.nepos:]
        if ne[:2] != b"NE":
            raise ValueError("not an NE module")
        self.seg_cnt = struct.unpack("<H", ne[0x1c:0x1e])[0]
        self.modref_cnt = struct.unpack("<H", ne[0x1e:0x20])[0]
        self.seg_tbl = struct.unpack("<H", ne[0x22:0x24])[0]
        self.resnam_tbl = struct.unpack("<H", ne[0x26:0x28])[0]
        self.modref_tbl = struct.unpack("<H", ne[0x28:0x2a])[0]
        self.impnam_tbl = struct.unpack("<H", ne[0x2a:0x2c])[0]
        self.ent_tbl = struct.unpack("<I", ne[0x2c:0x30])[0]
        self.ent_len = struct.unpack("<H", ne[0x30:0x32])[0]
        self.nonres_tbl = struct.unpack("<I", ne[0x36:0x3a])[0]
        self.nonres_sz = struct.unpack("<H", ne[0x20:0x22])[0]
        self.align = struct.unpack("<H", ne[0x32:0x34])[0] or 9
        self._segs()
        self._entries()
        self._names()
        self._modrefs()

    def _segs(self):
        self.segs = []          # (idx, file_off, length, flags, is_data, moveable, has_reloc)
        st = self.nepos + self.seg_tbl
        for i in range(self.seg_cnt):
            off, ln, flg, _mem = struct.unpack("<HHHH", self.data[st + i * 8:st + i * 8 + 8])
            self.segs.append({
                "idx": i + 1, "file": off << self.align, "len": ln, "flags": flg,
                "data": bool(flg & 1), "moveable": bool(flg & 0x10), "reloc": bool(flg & 0x100),
            })

    def _entries(self):
        """Entry table -> {ordinal: (segidx, offset)}. Bundles of fixed/moveable entries."""
        self.ent = {}
        p = self.nepos + self.ent_tbl
        end = p + self.ent_len
        ordn = 1
        d = self.data
        while p < end:
            cnt = d[p]; typ = d[p + 1]; p += 2
            if cnt == 0:
                break
            for _ in range(cnt):
                if typ == 0:                      # unused ordinals
                    ordn += 1; continue
                if typ == 0xFF:                   # moveable: flags, 3Fcd, seg, off (6 bytes)
                    seg = d[p + 3]; off = struct.unpack("<H", d[p + 4:p + 6])[0]; p += 6
                else:                             # fixed: flags, off (3 bytes); seg = typ
                    seg = typ; off = struct.unpack("<H", d[p + 1:p + 3])[0]; p += 3
                self.ent[ordn] = (seg, off); ordn += 1

    def _names(self):
        """resident + non-resident name tables -> {ordinal: name} (the exports' authentic names)."""
        self.exports = {}
        for base, sz in ((self.nepos + self.resnam_tbl, None), (self.nonres_tbl, self.nonres_sz)):
            p = base
            d = self.data
            first = True
            while p < len(d):
                ln = d[p]
                if ln == 0:
                    break
                nm = d[p + 1:p + 1 + ln].decode("latin1", "replace")
                ordv = struct.unpack("<H", d[p + 1 + ln:p + 1 + ln + 2])[0]
                p += 1 + ln + 2
                if not first:                     # first entry of each table is the module name (ord 0)
                    self.exports[ordv] = nm
                first = False

    def _modrefs(self):
        """imported module names (by index) for import relocations."""
        self.modrefs = []
        d = self.data
        for i in range(self.modref_cnt):
            noff = struct.unpack("<H", d[self.nepos + self.modref_tbl + i * 2:
                                         self.nepos + self.modref_tbl + i * 2 + 2])[0]
            p = self.nepos + self.impnam_tbl + noff
            ln = d[p]
            self.modrefs.append(d[p + 1:p + 1 + ln].decode("latin1", "replace"))

    def export_map(self, defnames=None):
        """{(segidx, offset): name} for exported functions. Prefer the authentic .def ordinal name;
        fall back to the module's own resident/non-resident name table."""
        defnames = defnames or {}
        out = {}
        for ordn, (seg, off) in self.ent.items():
            nm = defnames.get(ordn) or self.exports.get(ordn)
            if nm:
                out[(seg, off)] = nm
        return out


def _analyze(which):
    fname, desc = _MODS[which]
    path = _corpus(fname)
    if not path:
        print("  MISSING: %s" % fname); return None
    ne = NE(open(path, "rb").read())
    defn = _load_def(which)
    emap = ne.export_map(defn)
    codesegs = [s for s in ne.segs if not s["data"]]
    print("== %s ==" % which)
    print("  %s" % desc)
    print("  segments: %d (%d code, %d data); modrefs: %s"
          % (ne.seg_cnt, len(codesegs), ne.seg_cnt - len(codesegs), ne.modrefs or "(none)"))
    print("  entry-table ordinals: %d ; .def names: %d ; functions named authentically: %d"
          % (len(ne.ent), len(defn), len(emap)))
    print("  sample: %s" % ", ".join(sorted(set(n for n in emap.values()))[:14]))
    return ne


def main(argv):
    if capstone is None:
        print("nerecon: needs capstone"); return 2
    which = "all"
    for a in argv:
        if a in ("--krnl386", "--gdi", "--user"):
            which = a[2:]
        elif a == "--all":
            which = "all"
    targets = ["krnl386", "gdi", "user"] if which == "all" else [which]
    for t in targets:
        _analyze(t)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
