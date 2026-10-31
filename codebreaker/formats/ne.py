#!/usr/bin/env python3
"""
ne.py — A New Executable (NE) parser for the Windows 1.0 RE project.

The NE format debuted with Windows 1.0 (1985). This parser reads the DOS MZ
stub, the NE header, segment table, resident/non-resident name tables (exports),
module-reference + imported-name tables (imports), the entry table, and the
resource table. It is deliberately dependency-free and readable.

Usage:
    python3 ne.py FILE [--segments] [--relocs] [--resources] [--all]
"""
import struct

from codebreaker.formats import native

RSRC_TYPES = {  # predefined resource type ids (high bit set)
    1:"CURSOR",2:"BITMAP",3:"ICON",4:"MENU",5:"DIALOG",6:"STRING",
    7:"FONTDIR",8:"FONT",9:"ACCELERATOR",10:"RCDATA",11:"MESSAGETABLE",
    12:"GROUP_CURSOR",14:"GROUP_ICON",16:"VERSION",
}


def _native_class(cls):
    """The C++ reader's class for `cls` while it is switched on (docs/NATIVE.md), else None."""
    if cls in (NEFile, BootImageModule) and native.enabled("ne"):
        from codebreaker.formats import native_ne
        return native_ne.CLASSES[cls]
    return None


class NEFile:
    def __new__(cls, *args, **kw):
        return object.__new__(_native_class(cls) or cls)

    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError("not an MZ file")
        # e_lfanew is a 4-byte field; reading 2 mis-located the header of any file whose DOS stub
        # is 64 KB or more (uninstal.exe: 0x11080).
        self.ne_off = struct.unpack_from("<I", d, 0x3C)[0]
        if d[self.ne_off:self.ne_off+2] != b"NE":
            raise ValueError(f"no NE signature at 0x{self.ne_off:x} (got {d[self.ne_off:self.ne_off+2]!r})")
        self._parse_header()

    @classmethod
    def from_buffer(cls, data, ne_off, path="<buffer>"):
        """Parse an NE module that begins directly at ne_off (no MZ stub).
        Segment sector offsets are relative to ne_off for such embedded modules."""
        if _native_class(cls) is not None:
            return _native_class(cls).from_buffer(data, ne_off, path)
        self = cls.__new__(cls)
        self.path = path
        self.data = data
        self.ne_off = ne_off
        if data[ne_off:ne_off+2] != b"NE":
            raise ValueError(f"no NE at 0x{ne_off:x}")
        self._parse_header()
        self.base = ne_off   # segment offsets are relative to this
        return self

    base = 0   # byte added to segment sector offsets (nonzero for embedded modules)

    def u16(self, off): return struct.unpack_from("<H", self.data, off)[0]
    def u32(self, off): return struct.unpack_from("<I", self.data, off)[0]

    def _parse_header(self):
        o = self.ne_off
        h = {}
        h["linker_ver"]   = self.data[o+0x02]
        h["linker_rev"]   = self.data[o+0x03]
        h["entry_off"]    = self.u16(o+0x04)
        h["entry_len"]    = self.u16(o+0x06)
        h["crc"]          = self.u32(o+0x08)
        h["flags"]        = self.u16(o+0x0C)
        h["autodata_seg"] = self.u16(o+0x0E)
        h["heap"]         = self.u16(o+0x10)
        h["stack"]        = self.u16(o+0x12)
        h["csip"]         = self.u32(o+0x14)
        h["sssp"]         = self.u32(o+0x18)
        h["n_segs"]       = self.u16(o+0x1C)
        h["n_modrefs"]    = self.u16(o+0x1E)
        h["nonres_size"]  = self.u16(o+0x20)
        h["seg_tab"]      = self.u16(o+0x22)
        h["rsrc_tab"]     = self.u16(o+0x24)
        h["resname_tab"]  = self.u16(o+0x26)
        h["mod_tab"]      = self.u16(o+0x28)
        h["imp_tab"]      = self.u16(o+0x2A)
        h["nonres_off"]   = self.u32(o+0x2C)   # from start of file
        h["n_movable"]    = self.u16(o+0x30)
        h["align_shift"]  = self.u16(o+0x32) or 9
        h["n_resources"]  = self.u16(o+0x34)
        h["exetype"]      = self.data[o+0x36]
        h["expect_winver"]= self.u16(o+0x3E) if h["seg_tab"] >= 0x40 else None
        self.h = h

    # ---- name tables -------------------------------------------------------
    def _read_name_table(self, file_off):
        """Returns list of (name, ordinal). First entry = module/description."""
        out = []
        p = file_off
        while p < len(self.data):
            ln = self.data[p]; p += 1
            if ln == 0: break
            name = self.data[p:p+ln].decode("latin1"); p += ln
            ordv = self.u16(p); p += 2
            out.append((name, ordv))
        return out

    def resident_names(self):
        return self._read_name_table(self.ne_off + self.h["resname_tab"])

    def nonresident_names(self):
        if not self.h["nonres_off"]: return []
        return self._read_name_table(self.h["nonres_off"])

    def module_name(self):
        """This module's own name — the first resident-name-table entry."""
        r = self.resident_names()
        return r[0][0] if r else "SELF"

    def module_refs(self):
        """Imported module names, in module-reference-table order (1-based)."""
        names = []
        base = self.ne_off + self.h["mod_tab"]
        imp = self.ne_off + self.h["imp_tab"]
        for i in range(self.h["n_modrefs"]):
            off = self.u16(base + i*2)
            ln = self.data[imp + off]
            names.append(self.data[imp+off+1: imp+off+1+ln].decode("latin1"))
        return names

    def imported_name(self, off):
        base = self.ne_off + self.h["imp_tab"]
        if not (0 <= base + off < len(self.data)):
            return "?"                                    # out-of-range import-name offset
        ln = self.data[base+off]
        return self.data[base+off+1:base+off+1+ln].decode("latin1", "replace")

    # ---- segment table -----------------------------------------------------
    def segments(self):
        segs = []
        base = self.ne_off + self.h["seg_tab"]
        shift = self.h["align_shift"]
        for i in range(self.h["n_segs"]):
            o = base + i*8
            sector = self.u16(o)
            length = self.u16(o+2)
            flags  = self.u16(o+4)
            minalloc = self.u16(o+6)
            segs.append(dict(
                idx=i+1,
                file_off=(sector << shift) + (self.base if sector else 0),
                length=length if length else (0x10000 if sector else 0),
                flags=flags,
                minalloc=minalloc if minalloc else 0x10000,
                kind="DATA" if flags & 1 else "CODE",
            ))
        return segs

    def relocations(self, seg):
        """Parse per-segment relocation records (if RELOCINFO set)."""
        if not (seg["flags"] & 0x0100): return []
        end = seg["file_off"] + seg["length"]
        if end + 2 > len(self.data): return []
        n = self.u16(end)
        recs = []
        p = end + 2
        # A relocated segment base (segfix.py) does NOT relocate its reloc table,
        # so the count/table read here can be garbage or run off the buffer.
        # Bail out rather than raise: the code disassembles fine without fixups.
        if p + 8 * n > len(self.data): return []
        for _ in range(n):
            src_type  = self.data[p]      # source addressing type
            rel_type  = self.data[p+1]    # bit0=ADDITIVE; low 2 bits = target kind
            offset    = self.u16(p+2)
            b4 = self.u16(p+4); b6 = self.u16(p+6)
            target_kind = rel_type & 3
            # 0=INTERNALREF, 1=IMPORTORDINAL, 2=IMPORTNAME, 3=OSFIXUP
            if target_kind == 0:
                info = dict(kind="INTERNAL", seg=self.data[p+4], entry=b6)
            elif target_kind == 1:
                info = dict(kind="IMPORTORD", module=b4, ordinal=b6)
            elif target_kind == 2:
                info = dict(kind="IMPORTNAME", module=b4, name_off=b6)
            else:
                info = dict(kind="OSFIXUP", ftype=b4)
            recs.append(dict(src=src_type & 0x0f, rel_type=rel_type,
                             additive=bool(rel_type & 4), offset=offset, **info))
            p += 8
        return recs

    # ---- entry table -------------------------------------------------------
    def export_ordinals(self):
        """The ordinals the entry table actually exports, in order (unused gaps skipped).
        Exports by ordinal only have no resident name, so the name tables alone miss them."""
        return [e["ordinal"] for e in self.entries() if e.get("kind") != "UNUSED"]

    def entries(self):
        """Parse entry-table bundles -> list of dict(ordinal, seg, offset, flags)."""
        out = []
        p = self.ne_off + self.h["entry_off"]
        end = p + self.h["entry_len"]
        ordinal = 1
        while p < end:
            cnt = self.data[p]; seg_ind = self.data[p+1]; p += 2
            if cnt == 0:
                break
            if seg_ind == 0x00:
                # unused/skip block
                for _ in range(cnt):
                    out.append(dict(ordinal=ordinal, kind="UNUSED"))
                    ordinal += 1
                continue
            if seg_ind == 0xFF:  # MOVEABLE
                for _ in range(cnt):
                    flags = self.data[p]
                    # p+1,p+2 = INT 3F, p+3 = segment, p+4,5 = offset
                    seg = self.data[p+3]
                    off = self.u16(p+4)
                    out.append(dict(ordinal=ordinal, kind="MOVEABLE",
                                    seg=seg, offset=off, flags=flags))
                    p += 6; ordinal += 1
            else:  # FIXED, seg_ind is the segment number
                for _ in range(cnt):
                    flags = self.data[p]
                    off = self.u16(p+1)
                    out.append(dict(ordinal=ordinal, kind="FIXED",
                                    seg=seg_ind, offset=off, flags=flags))
                    p += 3; ordinal += 1
        return out

    # ---- resources ---------------------------------------------------------
    def resources(self):
        base = self.ne_off + self.h["rsrc_tab"]
        if self.h["rsrc_tab"] == self.h["resname_tab"]:
            return []  # no resource table
        p = base
        shift = self.u16(p); p += 2
        out = []
        while True:
            type_id = self.u16(p)
            if type_id == 0: break
            count = self.u16(p+2)
            p += 8  # skip reserved dword
            tname = RSRC_TYPES.get(type_id & 0x7fff, f"#{type_id&0x7fff}") if type_id & 0x8000 else None
            res_type = tname if type_id & 0x8000 else self._rsrc_string(base, type_id)
            for _ in range(count):
                off   = self.u16(p) << shift
                length= self.u16(p+2) << shift
                fl    = self.u16(p+4)
                rid   = self.u16(p+6)
                name  = (f"#{rid&0x7fff}" if rid & 0x8000
                         else self._rsrc_string(base, rid))
                out.append(dict(type=res_type, id=name, off=off, length=length, flags=fl))
                p += 12
        return out

    def _rsrc_string(self, base, off):
        p = base + off
        ln = self.data[p]
        return self.data[p+1:p+1+ln].decode("latin1")


# ---- Windows 1.x: the boot image, and absolute entries --------------------------------------
PRELOAD = 0x0040                    # segment / resource flag: loaded at start (else on demand)


class BootImageModule(NEFile):
    """One system module inside a Windows 1.x boot image (see open_boot_image).

    The image is two files and a module is spread over both: what is loaded at start is in the
    resident file (WIN100.BIN), what is loaded on demand in the overlay (WIN100.OVL). A segment
    or resource says which by its PRELOAD flag, and its offset counts from the start of THAT
    file; the two ranges overlap, so reading from the wrong file gives plausible nonsense. The
    names of the exports -- the non-resident name table -- are in the overlay too, at the
    header's offset counted in alignment units, not bytes. `data` here is the resident file
    followed by the overlay, with every offset this class returns pointing into that."""
    overlay_at = 0

    def segments(self):
        out = NEFile.segments(self)
        for s in out:
            if s["file_off"] and not s["flags"] & PRELOAD:
                s["file_off"] += self.overlay_at
        return out

    def resources(self):
        out = NEFile.resources(self)
        for r in out:
            if not r["flags"] & PRELOAD:
                r["off"] += self.overlay_at
        return out

    def nonresident_names(self):
        if not self.h["nonres_off"] or self.overlay_at >= len(self.data):
            return []
        return self._read_name_table(self.overlay_at + (self.h["nonres_off"] << self.h["align_shift"]))


def _place(nef):
    """(bytes, the module's place) as the C++ reader takes a module (native_ne.where)."""
    return nef.data, (nef.ne_off, nef.base, nef.overlay_at if isinstance(nef, BootImageModule) else None)


@native.native_first("ne.embedded", "ne")
def embedded(data, start=0x40):
    """[(offset, module name)] of every NE header inside a file that holds several modules."""
    out = []
    i = data.find(b"NE", start)
    while i != -1 and i + 0x40 <= len(data):
        nseg, = struct.unpack_from("<H", data, i + 0x1C)
        segtab, = struct.unpack_from("<H", data, i + 0x22)
        resnm, = struct.unpack_from("<H", data, i + 0x26)
        if 0 < nseg < 255 and 0x40 <= segtab < 0x100 and segtab <= resnm < 0x1000 and i + resnm < len(data):
            ln = data[i + resnm]
            name = data[i + resnm + 1:i + resnm + 1 + ln]
            if 0 < ln <= 8 and name.isalnum() and name[:1].isalpha() and name == name.upper():
                out.append((i, name.decode("ascii")))
        i = data.find(b"NE", i + 2)
    return out


def open_boot_image(path, overlay=None):
    """{module name: BootImageModule} for a Windows 1.x boot image, in file order. `overlay` is
    the demand-loaded half (default: the file of the same name ending .OVL beside `path`);
    without it only what is loaded at start can be read."""
    import os
    if overlay is None:
        stem = os.path.splitext(os.path.basename(path))[0].lower()
        folder = os.path.dirname(os.path.abspath(path))
        overlay = next((os.path.join(folder, f) for f in os.listdir(folder)
                        if f.lower() == stem + ".ovl"), None)
    with open(path, "rb") as f:
        resident = f.read()
    extra = b""
    if overlay:
        with open(overlay, "rb") as f:
            extra = f.read()
    out = {}
    for off, name in embedded(resident):
        m = BootImageModule.from_buffer(resident + extra, off, path)
        m.base, m.overlay_at = 0, len(resident)
        out.setdefault(name, m)
    return out


@native.native_first("ne._tables_end", "ne", args_of=_place)
def _tables_end(nef):
    """Offset just past the header and every table that belongs with it."""
    h, d, o = nef.h, nef.data, nef.ne_off
    end = max(o + 0x40, o + h["seg_tab"] + 8 * h["n_segs"], o + h["entry_off"] + h["entry_len"],
              o + h["mod_tab"] + 2 * h["n_modrefs"])
    p = o + h["resname_tab"]                                 # resident names
    while p < len(d) and d[p]:
        p += d[p] + 3
    end = max(end, p + 1)
    if h["rsrc_tab"] != h["resname_tab"]:                    # resource table, then its names
        p = o + h["rsrc_tab"] + 2
        while nef.u16(p):
            p += 8 + 12 * nef.u16(p + 2)
        p += 2
        while p < len(d) and d[p]:
            p += d[p] + 1
        end = max(end, p + 1)
    imp = o + h["imp_tab"]                                   # imported names that are referred to
    refs = [nef.u16(o + h["mod_tab"] + 2 * i) for i in range(h["n_modrefs"])]
    refs += [r["name_off"] for s in nef.segments() for r in nef.relocations(s) if r["kind"] == "IMPORTNAME"]
    for off in refs:
        if imp + off < len(d):
            end = max(end, imp + off + 1 + d[imp + off])
    return end


@native.native_first("ne.standalone", "ne", args_of=_place)
def standalone(nef):
    """The bytes of an ordinary one-file executable holding the module `nef` -- for a module
    read out of a boot image, so that everything that takes a path can take it. Header and
    tables are kept as they are; segments (with their relocations), resources and the
    non-resident names are laid out after them and the tables pointed at the new places."""
    h, d, o = nef.h, nef.data, nef.ne_off
    shift = h["align_shift"]
    out = bytearray(0x40)
    out[:2] = b"MZ"
    struct.pack_into("<H", out, 0x18, 0x40)
    struct.pack_into("<I", out, 0x3C, 0x40)
    out += d[o:_tables_end(nef)]

    def place(blob, sh):
        out.extend(bytes(-len(out) % (1 << sh)))
        at = len(out)
        out.extend(blob)
        return at >> sh

    for i, s in enumerate(nef.segments()):
        if not s["file_off"]:
            continue
        end = s["file_off"] + s["length"]
        if s["flags"] & 0x0100 and end + 2 <= len(d):
            end += 2 + 8 * nef.u16(end)
        struct.pack_into("<H", out, 0x40 + h["seg_tab"] + 8 * i, place(d[s["file_off"]:end], shift))
    if h["rsrc_tab"] != h["resname_tab"]:
        res = iter(nef.resources())
        p = o + h["rsrc_tab"]
        rshift = nef.u16(p)
        p += 2
        while nef.u16(p):
            count = nef.u16(p + 2)
            p += 8
            for _ in range(count):
                r = next(res)
                struct.pack_into("<H", out, 0x40 + p - o, place(d[r["off"]:r["off"] + r["length"]], rshift))
                p += 12
    names = nef.nonresident_names()
    table = b"".join(bytes([len(n)]) + n.encode("latin1") + struct.pack("<H", v) for n, v in names) + bytes(1)
    struct.pack_into("<I", out, 0x40 + 0x2C, len(out) if names else 0)
    struct.pack_into("<H", out, 0x40 + 0x20, len(table) if names else 0)
    if names:
        out += table
    return bytes(out)
