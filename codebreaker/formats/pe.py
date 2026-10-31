#!/usr/bin/env python3
"""
pe.py — PE32 reader for the era's 32-bit modules.

Windows 3.1 was NE all the way down, so `ne.py` was the only loader the
toolchain needed. This is the first release in the lineage where the
core ships in two worlds at once: the 16-bit compatibility layer (KRNL386,
USER, GDI — still NE, still parsed by `ne.py`) and the new 32-bit engines
(KERNEL32, USER32, GDI32 and 600+ other modules — PE). This is the 32-bit half.

Deliberately minimal and dependency-free, matching `ne.py`'s scope: enough to
tier and measure the corpus (section table, code size) and to feed the ordinal
work (export directory, name<->ordinal). Disassembly of 32-bit code is a
separate, later concern; this does not attempt it.
"""
import struct

from codebreaker.formats import native

_MACHINE = {0x014C: "i386", 0x8664: "x64", 0x01C0: "arm", 0xAA64: "arm64"}
_SUBSYSTEM = {1: "native", 2: "GUI", 3: "console", 5: "OS/2", 7: "POSIX",
              9: "Windows CE GUI", 10: "EFI application", 14: "Xbox"}
_IMAGE_SCN_CNT_CODE = 0x00000020
_IMAGE_SCN_MEM_EXECUTE = 0x20000000


class PEFile:
    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError(f"{path}: no MZ header")
        self.e_lfanew = struct.unpack("<I", d[0x3C:0x40])[0]
        if d[self.e_lfanew:self.e_lfanew + 4] != b"PE\0\0":
            raise ValueError(f"{path}: not a PE image")
        self._parse()

    def _parse(self):
        d = self.data
        coff = self.e_lfanew + 4
        (self.machine, self.n_sections, self.timestamp, _psym, _nsym,
         self.opt_size, self.characteristics) = struct.unpack(
            "<HHIIIHH", d[coff:coff + 20])
        opt = coff + 20
        self.magic = struct.unpack("<H", d[opt:opt + 2])[0]  # 0x10b PE32, 0x20b PE32+
        self.is_plus = self.magic == 0x20B
        # entry point, image base
        self.entry = struct.unpack("<I", d[opt + 16:opt + 20])[0]
        if self.is_plus:
            self.image_base = struct.unpack("<Q", d[opt + 24:opt + 32])[0]
            dd_off = opt + 108   # NumberOfRvaAndSizes; directories at +112
        else:
            self.image_base = struct.unpack("<I", d[opt + 28:opt + 32])[0]
            dd_off = opt + 92    # NumberOfRvaAndSizes; directories at +96
        self.n_datadir = struct.unpack("<I", d[dd_off:dd_off + 4])[0]
        self.datadirs = []
        p = dd_off + 4
        for _ in range(self.n_datadir):
            va, sz = struct.unpack("<II", d[p:p + 8])
            self.datadirs.append((va, sz))
            p += 8
        # section table follows the optional header
        sec = coff + 20 + self.opt_size
        self.sections = []
        for _ in range(self.n_sections):
            raw = d[sec:sec + 40]
            name = raw[:8].rstrip(b"\0").decode("latin1", "replace")
            (vsize, vaddr, rawsize, rawptr, _r1, _r2, _r3, _r4,
             chars) = struct.unpack("<IIIIIIHHI", raw[8:40])
            self.sections.append(dict(
                name=name, vsize=vsize, vaddr=vaddr,
                rawsize=rawsize, rawptr=rawptr, chars=chars))
            sec += 40

    # -- helpers ----------------------------------------------------------
    def machine_name(self):
        return _MACHINE.get(self.machine, f"{self.machine:#06x}")

    def is_dll(self):
        return bool(self.characteristics & 0x2000)

    def _rva_to_off(self, rva):
        for s in self.sections:
            if s["vaddr"] <= rva < s["vaddr"] + max(s["vsize"], s["rawsize"]):
                return s["rawptr"] + (rva - s["vaddr"])
        return None

    def code_bytes(self):
        """Sum of raw sizes of executable/code sections."""
        total = 0
        for s in self.sections:
            if s["chars"] & (_IMAGE_SCN_CNT_CODE | _IMAGE_SCN_MEM_EXECUTE):
                total += s["rawsize"]
        return total

    def module_name(self):
        """The export directory's internal name, or '' if not exported."""
        if len(self.datadirs) <= 0:
            return ""
        va, sz = self.datadirs[0]
        if not va:
            return ""
        off = self._rva_to_off(va)
        if off is None:
            return ""
        name_rva = struct.unpack("<I", self.data[off + 12:off + 16])[0]
        no = self._rva_to_off(name_rva)
        if no is None:
            return ""
        end = self.data.index(b"\0", no)
        return self.data[no:end].decode("latin1", "replace")

    def export_addrs(self):
        """{name: rva} for every export that has a name."""
        return {name: rva for _ord, name, rva in self.exports() if name}

    def exports(self):
        """[(ordinal, name_or_None, rva)] from the export directory."""
        if not self.datadirs or not self.datadirs[0][0]:
            return []
        va, _sz = self.datadirs[0]
        off = self._rva_to_off(va)
        if off is None:
            return []
        d = self.data
        (_flags, _ts, _vmaj, _vmin, _name_rva, ord_base,
         n_funcs, n_names, funcs_rva, names_rva, ords_rva) = struct.unpack(
            "<IIHHIIIIIII", d[off:off + 40])
        fo = self._rva_to_off(funcs_rva)
        out = []
        eat = []
        for i in range(n_funcs):
            rva = struct.unpack("<I", d[fo + i * 4:fo + i * 4 + 4])[0] if fo else 0
            eat.append(rva)
            out.append([ord_base + i, None, rva])
        no = self._rva_to_off(names_rva)
        oo = self._rva_to_off(ords_rva)
        if no and oo:
            for i in range(n_names):
                nrva = struct.unpack("<I", d[no + i * 4:no + i * 4 + 4])[0]
                idx = struct.unpack("<H", d[oo + i * 2:oo + i * 2 + 2])[0]
                po = self._rva_to_off(nrva)
                if po is None:
                    continue
                end = d.index(b"\0", po)
                nm = d[po:end].decode("latin1", "replace")
                if idx < len(out):
                    out[idx][1] = nm
        return [(o, n, r) for o, n, r in out if r]  # drop empty slots

    def imports(self):
        """{iat_slot_va: (dll, name)} from the import directory (data dir 1).

        A `call dword ptr [iat_slot_va]` in the code resolves to an imported
        API through this map — the PE analogue of NE's module.ordinal imports,
        and what lets pe_trace name cross-module calls instead of leaving them
        as raw `[0x....]`.  name is the imported symbol, or '#<ordinal>' for a
        by-ordinal import.
        """
        if len(self.datadirs) < 2 or not self.datadirs[1][0]:
            return {}
        va, _sz = self.datadirs[1]
        off = self._rva_to_off(va)
        if off is None:
            return {}
        d = self.data
        out = {}
        i = 0
        while True:
            rec = d[off + i * 20: off + i * 20 + 20]
            if len(rec) < 20:
                break
            ilt, _ts, _fwd, name_rva, iat_rva = struct.unpack("<IIIII", rec)
            if ilt == 0 and iat_rva == 0:
                break                                   # null terminator
            i += 1
            no = self._rva_to_off(name_rva)
            dll = ""
            if no is not None:
                end = d.index(b"\0", no)
                dll = d[no:end].decode("latin1", "replace")
            thunk_rva = ilt or iat_rva                  # ILT preferred; IAT if bound
            to = self._rva_to_off(thunk_rva)
            if to is None:
                continue
            j = 0
            while True:
                val = struct.unpack("<I", d[to + j * 4: to + j * 4 + 4])[0]
                if val == 0:
                    break
                if val & 0x80000000:
                    nm = "#%d" % (val & 0xFFFF)          # by ordinal
                else:
                    po = self._rva_to_off(val)
                    if po is None:
                        nm = "?"
                    else:
                        e = d.index(b"\0", po + 2)      # skip 2-byte hint
                        nm = d[po + 2:e].decode("latin1", "replace")
                out[self.image_base + iat_rva + j * 4] = (dll, nm)
                j += 1
        return out

    def code_sections(self):
        """[(va_start, bytes)] for executable sections."""
        out = []
        for s in self.sections:
            if s["chars"] & (_IMAGE_SCN_CNT_CODE | _IMAGE_SCN_MEM_EXECUTE):
                raw = self.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
                out.append((self.image_base + s["vaddr"], raw, s["name"]))
        return out

    def base_relocs(self):
        """Set of RVAs that carry an absolute 32-bit address (HIGHLOW, type 3).

        The PE analogue of NE's FIXUPP relocation sites. Data directory 5 (.reloc)
        lists every field the loader would patch if the image were not loaded at
        its preferred ImageBase. On disk each such field already holds the
        preferred-base absolute value, so a faithful reconstruction that emits the
        literal reproduces the shipped bytes exactly; the mask only guards the
        rare case where our encoding differs at a relocated field. Type 3 = the
        4-byte absolute (HIGHLOW) used throughout a 32-bit image; type 0 = padding.
        """
        if len(self.datadirs) < 6 or not self.datadirs[5][0]:
            return set()
        va, size = self.datadirs[5]
        off = self._rva_to_off(va)
        if off is None:
            return set()
        d = self.data
        out = set()
        p, endp = off, off + size
        while p + 8 <= endp:
            page_rva, block_size = struct.unpack("<II", d[p:p + 8])
            if block_size < 8:
                break
            n = (block_size - 8) // 2
            for i in range(n):
                e = struct.unpack("<H", d[p + 8 + i * 2:p + 8 + i * 2 + 2])[0]
                typ, roff = e >> 12, e & 0x0FFF
                if typ == 3:                       # IMAGE_REL_BASED_HIGHLOW
                    out.add(page_rva + roff)
            p += block_size
        return out


    def build_meta(self):
        """Structural build metadata: machine, bitness, image base, entry, link
        timestamp, linker version, subsystem, EXE/DLL. All from the headers."""
        d = self.data
        opt = self.e_lfanew + 4 + 20                      # optional header start
        subsys_off = opt + (72 if self.is_plus else 68)   # Subsystem field
        return {
            "machine": self.machine_name(),
            "magic": "PE32+" if self.is_plus else "PE32",
            "kind": "DLL" if self.is_dll() else "EXE",
            "image_base": self.image_base,
            "entry_rva": self.entry,
            "entry_va": self.image_base + self.entry if self.entry else 0,
            "timestamp": self.timestamp,
            "linker_version": "%d.%d" % (d[opt + 2], d[opt + 3]),
            "subsystem": _SUBSYSTEM.get(
                struct.unpack("<H", d[subsys_off:subsys_off + 2])[0],
                "0x%x" % struct.unpack("<H", d[subsys_off:subsys_off + 2])[0]),
        }

    def debug_codeview(self):
        """The CodeView record from the DEBUG data directory (index 6), or None.

        Modern toolchains emit an RSDS record here pointing at the matching PDB
        (a GUID+age key); older ones emit NB10 (a timestamp+age key). This is the
        PE debug-directory form, distinct from the appended NB09 CodeView blob
        that symbol_engine/codeview.py reads.

        RSDS -> {'format':'RSDS','guid':str,'age':int,'pdb':str}
        NB10 -> {'format':'NB10','signature':int,'age':int,'pdb':str}
        """
        if len(self.datadirs) < 7 or not self.datadirs[6][0]:
            return None
        va, size = self.datadirs[6]
        off = self._rva_to_off(va)
        if off is None:
            return None
        d = self.data
        for i in range(size // 28):
            rec = d[off + i * 28: off + i * 28 + 28]
            if len(rec) < 28:
                break
            typ = struct.unpack("<I", rec[12:16])[0]
            if typ != 2:                                  # IMAGE_DEBUG_TYPE_CODEVIEW
                continue
            dsize = struct.unpack("<I", rec[16:20])[0]
            dptr = struct.unpack("<I", rec[24:28])[0]     # PointerToRawData
            cv = d[dptr:dptr + dsize]
            if cv[:4] == b"RSDS":
                g = struct.unpack("<IHH8s", cv[4:20])
                guid = "%08X-%04X-%04X-%s-%s" % (
                    g[0], g[1], g[2], g[3][:2].hex().upper(), g[3][2:].hex().upper())
                return {"format": "RSDS", "guid": guid,
                        "age": struct.unpack("<I", cv[20:24])[0],
                        "pdb": cv[24:].split(b"\0", 1)[0].decode("latin1", "replace")}
            if cv[:4] == b"NB10":
                return {"format": "NB10",
                        "signature": struct.unpack("<I", cv[8:12])[0],
                        "age": struct.unpack("<I", cv[12:16])[0],
                        "pdb": cv[16:].split(b"\0", 1)[0].decode("latin1", "replace")}
        return None

    # -- the rest of what an inspector shows (used by formats/inspect.py) -------------------
    # Every reader below checks its offsets: a damaged table gives a shorter answer, not an error.
    def _u(self, fmt, off):
        """One little-endian value at a file offset, or 0 when it lies outside the file."""
        if off is None or off < 0 or off + struct.calcsize(fmt) > len(self.data):
            return 0
        return struct.unpack_from("<" + fmt, self.data, off)[0]

    def _text(self, off, limit=512):
        """The zero-terminated string at a file offset ('' when the offset is not in the file)."""
        if off is None or not 0 <= off < len(self.data):
            return ""
        end = self.data.find(b"\0", off, off + limit)
        return self.data[off:end if end >= 0 else off + limit].decode("latin1", "replace")

    def file_offset(self, rva):
        """File offset of an RVA. One that lies in the headers, before any section, is its own
        offset; one that lies nowhere is None."""
        off = self._rva_to_off(rva)
        if off is None and 0 <= rva < self._u("I", self.e_lfanew + 24 + 60):
            off = rva
        return off if off is not None and off < len(self.data) else None

    def optional_header(self):
        """The optional header's fields by name (PE32 and PE32+)."""
        opt = self.e_lfanew + 24
        u16, u32 = (lambda o: self._u("H", opt + o)), (lambda o: self._u("I", opt + o))
        return {
            "magic": self.magic, "linker_version": "%d.%d" % (self._u("B", opt + 2), self._u("B", opt + 3)),
            "code_size": u32(4), "initialized_data_size": u32(8), "uninitialized_data_size": u32(12),
            "entry_point": self.entry, "code_base": u32(20), "image_base": self.image_base,
            "section_alignment": u32(32), "file_alignment": u32(36),
            "os_version": "%d.%d" % (u16(40), u16(42)), "image_version": "%d.%d" % (u16(44), u16(46)),
            "subsystem_version": "%d.%d" % (u16(48), u16(50)), "image_size": u32(56),
            "headers_size": u32(60), "checksum": u32(64), "subsystem": u16(68),
            "subsystem_name": _SUBSYSTEM.get(u16(68), "%d" % u16(68)), "dll_characteristics": u16(70)}

    def directory(self, index):
        """(rva, size) of one data directory; (0, 0) when the image has none at that index."""
        return self.datadirs[index] if index < len(self.datadirs) else (0, 0)

    def export_table(self):
        """{'module', 'ordinal_base', 'timestamp', 'symbols': [{name, ordinal, hint, rva, forward}]}.
        `hint` is the name's index in the name table (-1 for an export by ordinal only);
        `forward` is 'OTHER.Function' when the export is passed on to another module."""
        va, size = self.directory(0)
        off = self.file_offset(va) if va else None
        if off is None or off + 40 > len(self.data):
            return {"module": "", "ordinal_base": 0, "timestamp": 0, "symbols": []}
        base, n_funcs, n_names = self._u("I", off + 16), self._u("I", off + 20), self._u("I", off + 24)
        funcs, names, ords = (self.file_offset(self._u("I", off + o)) for o in (28, 32, 36))
        named = {}
        if names is not None and ords is not None:
            for i in range(min(n_names, 0x10000)):
                if names + i * 4 + 4 > len(self.data) or ords + i * 2 + 2 > len(self.data):
                    break
                named[self._u("H", ords + i * 2)] = (self._text(self.file_offset(self._u("I", names + i * 4))), i)
        symbols = []
        if funcs is not None:
            for i in range(min(n_funcs, 0x10000)):
                if funcs + i * 4 + 4 > len(self.data):
                    break
                rva = self._u("I", funcs + i * 4)
                if not rva:
                    continue
                name, hint = named.get(i, ("", -1))
                forward = self._text(self.file_offset(rva)) if va <= rva < va + size else ""
                symbols.append({"name": name, "ordinal": base + i, "hint": hint, "rva": rva, "forward": forward})
        return {"module": self._text(self.file_offset(self._u("I", off + 12))), "ordinal_base": base,
                "timestamp": self._u("I", off + 4), "symbols": symbols}

    def import_table(self, delay=False):
        """[{'module', 'symbols': [{name, ordinal, hint, slot}]}] from the import directory, or from
        the delay-load directory. An import by ordinal has name '' and hint -1; one by name has
        ordinal -1. `slot` is the RVA of the address-table entry the loader fills in."""
        va, _size = self.directory(13 if delay else 1)
        off = self.file_offset(va) if va else None
        if off is None:
            return []
        step, width = (32 if delay else 20), (8 if self.is_plus else 4)
        top = 1 << (width * 8 - 1)
        out = []
        for k in range(4096):
            rec = off + k * step
            if rec + step > len(self.data):
                break
            if delay:
                name_rva, iat, lookup = (self._u("I", rec + o) for o in (4, 12, 16))
                if not self._u("I", rec) & 1:                     # old layout: addresses, not RVAs
                    name_rva, iat, lookup = (max(0, v - self.image_base) if v else 0 for v in (name_rva, iat, lookup))
            else:
                lookup, name_rva, iat = (self._u("I", rec + o) for o in (0, 12, 16))
            if not (name_rva or iat or lookup):
                break
            table = self.file_offset(lookup or iat)
            symbols = []
            for j in range(16384 if table is not None else 0):
                if table + (j + 1) * width > len(self.data):
                    break
                val = self._u("Q" if self.is_plus else "I", table + j * width)
                if not val:
                    break
                if val & top:
                    symbols.append({"name": "", "ordinal": val & 0xFFFF, "hint": -1, "slot": iat + j * width})
                    continue
                at = self.file_offset(val & 0x7FFFFFFF)
                if at is None:                                    # bound: the table already holds addresses
                    break
                symbols.append({"name": self._text(at + 2), "ordinal": -1, "hint": self._u("H", at),
                                "slot": iat + j * width})
            out.append({"module": self._text(self.file_offset(name_rva)) or "?", "symbols": symbols})
        return out

    def resource_tree(self):
        """[{'type': name or number, 'id', 'items': [{name, id, language, rva, offset, size}]}] --
        the resource directory's three levels (type, name, language) flattened to one list of
        items per type. `id` is None for a type or item that goes by a name."""
        va, _size = self.directory(2)
        base = self.file_offset(va) if va else None
        if base is None:
            return []

        def entries(at):
            n = self._u("H", at + 12) + self._u("H", at + 14)
            out = []
            for i in range(min(n, 8192)):
                p = at + 16 + i * 8
                if p + 8 > len(self.data):
                    break
                ident, to = self._u("I", p), self._u("I", p + 4)
                label = None
                if ident & 0x80000000:
                    s = base + (ident & 0x7FFFFFFF)
                    label = self.data[s + 2:s + 2 + self._u("H", s) * 2].decode("utf-16-le", "replace")
                out.append((label, None if label is not None else ident, to))
            return out

        types = []
        for tname, tid, to in entries(base):
            items = []
            for nname, nid, no in (entries(base + (to & 0x7FFFFFFF)) if to & 0x80000000 else ()):
                for _l, lang, lo in (entries(base + (no & 0x7FFFFFFF)) if no & 0x80000000 else ()):
                    if lo & 0x80000000 or base + lo + 16 > len(self.data):
                        continue
                    rva = self._u("I", base + lo)
                    items.append({"name": nname if nname is not None else "#%d" % nid, "id": nid,
                                  "language": lang or 0, "rva": rva, "offset": self.file_offset(rva),
                                  "size": self._u("I", base + lo + 4)})
            types.append({"type": tname if tname is not None else tid, "id": tid, "items": items})
        return types

    def reloc_blocks(self):
        """[(page_rva, entries)] -- the blocks of the base relocation directory."""
        va, size = self.directory(5)
        off = self.file_offset(va) if va else None
        out = []
        end = (off or 0) + size
        while off is not None and off + 8 <= min(end, len(self.data)):
            block = self._u("I", off + 4)
            if block < 8:
                break
            out.append((self._u("I", off), (block - 8) // 2))
            off += block
        return out

    def tls(self):
        """The thread-local storage directory's addresses, or None."""
        va, _size = self.directory(9)
        off = self.file_offset(va) if va else None
        if off is None:
            return None
        w, fmt = (8, "Q") if self.is_plus else (4, "I")
        return {"start_va": self._u(fmt, off), "end_va": self._u(fmt, off + w),
                "index_va": self._u(fmt, off + 2 * w), "callbacks_va": self._u(fmt, off + 3 * w)}

    def debug_entries(self):
        """[{type, timestamp, size, offset}] -- every record of the debug directory (`type` is the
        IMAGE_DEBUG_TYPE number)."""
        va, size = self.directory(6)
        off = self.file_offset(va) if va else None
        out = []
        for i in range(min(size // 28, 64) if off is not None else 0):
            e = off + i * 28
            if e + 28 > len(self.data):
                break
            out.append({"type": self._u("I", e + 12), "timestamp": self._u("I", e + 4),
                        "size": self._u("I", e + 16), "offset": self._u("I", e + 24)})
        return out

    def certificates(self):
        """[{length, revision, type}] -- the attribute certificates (this directory holds a FILE
        OFFSET, not an RVA)."""
        off, size = self.directory(4)
        out = []
        end = min(off + size, len(self.data))
        while off and off + 8 <= end:
            length = self._u("I", off)
            if length < 8:
                break
            out.append({"length": length, "revision": self._u("H", off + 4), "type": self._u("H", off + 6)})
            off += (length + 7) & ~7
        return out

    def clr(self):
        """The .NET runtime header, or None for a native image."""
        va = _clr_directory(self.datadirs)[0]
        off = self.file_offset(va) if va else None
        if off is None or off + 72 > len(self.data):
            return None
        return {"runtime_version": "%d.%d" % (self._u("H", off + 4), self._u("H", off + 6)),
                "flags": self._u("I", off + 16), "entry_token": self._u("I", off + 20)}

    def rich(self):
        """The linker's 'Rich' record between the DOS stub and the PE header: {key, entries:
        [(product, build, count)]}, or None when the image has none."""
        stub = self.data[:self.e_lfanew]
        end = stub.rfind(b"Rich")
        if end < 0 or end + 8 > len(stub):
            return None
        key = struct.unpack_from("<I", stub, end + 4)[0]
        start = stub.rfind(struct.pack("<I", 0x536E6144 ^ key), 0, end)          # "DanS", encoded
        entries = []
        for p in range(start + 16, end - 7, 8) if start >= 0 else ():
            comp, count = (v ^ key for v in struct.unpack_from("<II", stub, p))
            entries.append((comp >> 16, comp & 0xFFFF, count))
        return {"key": key, "entries": entries}


def open_pe(path):
    if native.enabled("pe"):                         # the C++ reader (docs/NATIVE.md)
        from codebreaker.formats import native_pe
        return native_pe.PEFile(path)
    return PEFile(path)


def _clr_directory(datadirs):
    """(rva, size) of the CLR runtime header's directory (14), or (0, 0): a native image."""
    va, size = datadirs[14] if len(datadirs) > 14 else (0, 0)
    return (va, size) if va and size else (0, 0)


@native.native_first("pe.is_dotnet", "pe", data_arg=None)
def is_dotnet(path):
    """Whether the file is a .NET assembly: a PE image with a CLR runtime header (data directory
    14). Reads the headers only."""
    try:
        with open(path, "rb") as f:
            head = f.read(0x40)
            if len(head) < 0x40 or head[:2] != b"MZ":
                return False
            f.seek(struct.unpack_from("<I", head, 0x3C)[0])
            nt = f.read(24 + 112 + 16 * 8)                      # (PE32+: the largest optional header)
    except OSError:
        return False
    if len(nt) < 26 or nt[:4] != b"PE\0\0":
        return False
    at = 24 + (108 if struct.unpack_from("<H", nt, 24)[0] == 0x20B else 92)    # NumberOfRvaAndSizes
    if at + 4 > len(nt):
        return False
    count = min(struct.unpack_from("<I", nt, at)[0], (len(nt) - at - 4) // 8)
    return _clr_directory([struct.unpack_from("<II", nt, at + 4 + 8 * k) for k in range(count)]) != (0, 0)
