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
import struct, sys, os

# ---- NE header flags -------------------------------------------------------
NE_FLAGS = [
    (0x0001, "SINGLEDATA (shared DLL)"),
    (0x0002, "MULTIPLEDATA (per-instance data)"),
    (0x2000, "ERRORS (linker errors present)"),
    (0x8000, "LIBRARY (DLL, not a program)"),
]
SEG_FLAGS = [
    (0x0001, "DATA"),          # else CODE
    (0x0002, "ALLOCATED"),
    (0x0004, "LOADED"),
    (0x0010, "MOVEABLE"),      # else FIXED
    (0x0020, "SHAREABLE"),     # PURE
    (0x0040, "PRELOAD"),       # else LOADONCALL
    (0x0080, "EXECUTEONLY/READONLY"),
    (0x0100, "RELOCINFO"),     # has relocation records
    (0x0800, "DISCARDABLE"),
]
EXETYPE = {0: "unknown", 1: "OS/2", 2: "Windows", 3: "European MS-DOS 4.x",
           4: "Windows 386", 5: "BOSS"}
RSRC_TYPES = {  # predefined resource type ids (high bit set)
    1:"CURSOR",2:"BITMAP",3:"ICON",4:"MENU",5:"DIALOG",6:"STRING",
    7:"FONTDIR",8:"FONT",9:"ACCELERATOR",10:"RCDATA",11:"MESSAGETABLE",
    12:"GROUP_CURSOR",14:"GROUP_ICON",16:"VERSION",
}

def bits(flags, table):
    return [name for mask, name in table if flags & mask]

class NEFile:
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


# ---------------------------------------------------------------------------
def dump(path, show_segs=False, show_relocs=False, show_res=False):
    ne = NEFile(path)
    h = ne.h
    res = ne.resident_names()
    modname = res[0][0] if res else "?"
    nonres = ne.nonresident_names()
    desc = nonres[0][0] if nonres else ""
    print("="*74)
    print(f"{os.path.basename(path)}   module={modname!r}")
    if desc: print(f"  description: {desc!r}")
    print("-"*74)
    print(f"  NE @0x{ne.ne_off:x}  linker {h['linker_ver']}.{h['linker_rev']:02d}"
          f"  target={EXETYPE.get(h['exetype'],h['exetype'])}"
          + (f" (win {h['expect_winver']>>8}.{h['expect_winver']&0xff:02d})" if h['expect_winver'] else ""))
    ftags = bits(h["flags"], NE_FLAGS)
    print(f"  flags: 0x{h['flags']:04x} {ftags}")
    print(f"  segments={h['n_segs']}  modrefs={h['n_modrefs']}  "
          f"resources={h['n_resources']}  movable_entries={h['n_movable']}")
    print(f"  CS:IP={h['csip']>>16:x}:{h['csip']&0xffff:04x}  "
          f"SS:SP={h['sssp']>>16:x}:{h['sssp']&0xffff:04x}  "
          f"heap={h['heap']} stack={h['stack']}  autodata_seg={h['autodata_seg']}")

    mods = ne.module_refs()
    if mods:
        print(f"  imports modules: {', '.join(mods)}")

    exports = [(n,o) for n,o in res[1:]] + [(n,o) for n,o in nonres[1:]]
    if exports:
        print(f"  exports ({len(exports)}):")
        for n,o in sorted(exports, key=lambda x:x[1]):
            print(f"      @{o:<4} {n}")

    if show_segs:
        print("  segments:")
        for s in ne.segments():
            fl = bits(s["flags"], SEG_FLAGS)
            print(f"    #{s['idx']:<2} {s['kind']:4} off=0x{s['file_off']:06x} "
                  f"len=0x{s['length']:04x} alloc=0x{s['minalloc']:04x} {fl}")

    if show_relocs:
        for s in ne.segments():
            rl = ne.relocations(s)
            if rl:
                print(f"  seg #{s['idx']} relocations ({len(rl)}):")
                for r in rl[:200]:
                    if r.get("kind")=="IMPORTORD":
                        m = mods[r['module']-1] if 0 < r['module'] <= len(mods) else f"?{r['module']}"
                        tgt = f"{m}.{r['ordinal']}"
                    elif r.get("kind")=="IMPORTNAME":
                        m = mods[r['module']-1] if 0 < r['module'] <= len(mods) else f"?{r['module']}"
                        tgt = f"{m}.{ne.imported_name(r['name_off'])}"
                    else:
                        tgt = f"seg{r.get('seg')}:{r.get('entry')}"
                    print(f"      +0x{r['offset']:04x} -> {r.get('kind')}: {tgt}")

    if show_res:
        rs = ne.resources()
        if rs:
            print(f"  resources ({len(rs)}):")
            for r in rs:
                print(f"    {r['type']:<12} {r['id']:<10} off=0x{r['off']:06x} len=0x{r['length']:04x}")
    return ne


if __name__ == "__main__":
    args = sys.argv[1:]
    if not args:
        print(__doc__); sys.exit(1)
    path = args[0]
    allf = "--all" in args
    dump(path,
         show_segs="--segments" in args or allf,
         show_relocs="--relocs" in args or allf,
         show_res="--resources" in args or allf)
