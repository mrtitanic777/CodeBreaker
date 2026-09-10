#!/usr/bin/env python3
"""
pe.py — PE32 reader for the Windows 95 32-bit modules.

Windows 3.1 was NE all the way down, so `ne.py` was the only loader the
toolchain needed. Windows 95 is the first release in this lineage where the
core ships in two worlds at once: the 16-bit compatibility layer (KRNL386,
USER, GDI — still NE, still parsed by `ne.py`) and the new 32-bit engines
(KERNEL32, USER32, GDI32 and 600+ other modules — PE). This is the 32-bit half.

Deliberately minimal and dependency-free, matching `ne.py`'s scope: enough to
tier and measure the corpus (section table, code size) and to feed the ordinal
work (export directory, name<->ordinal). Disassembly of 32-bit code is a
separate, later concern; this does not attempt it.
"""
import struct

_MACHINE = {0x014C: "i386", 0x8664: "x64", 0x01C0: "arm"}
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


def open_pe(path):
    return PEFile(path)


if __name__ == "__main__":
    import sys
    for p in sys.argv[1:]:
        pe = PEFile(p)
        exp = pe.exports()
        print(f"{p}: {pe.machine_name()} {'DLL' if pe.is_dll() else 'EXE'} "
              f"module={pe.module_name()!r} code={pe.code_bytes():,}b "
              f"sections={len(pe.sections)} exports={len(exp)}")
