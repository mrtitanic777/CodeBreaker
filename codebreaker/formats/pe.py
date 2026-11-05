#!/usr/bin/env python3
"""
pe.py -- PE32 / PE32+ reader for the era's 32-bit modules, answered by the C++ (native/formats/pe:
formats.dll, through native.py). Its Python reader is gone (docs/NATIVE.md, step 4): this module
is the API -- each method's def is its signature and its documentation, and formats.dll answers.

Windows 3.1 was NE all the way down, so `ne.py` was the only loader the toolchain needed. This is
the first release in the lineage where the core ships in two worlds at once: the 16-bit
compatibility layer (KRNL386, USER, GDI -- still NE) and the new 32-bit engines (KERNEL32, USER32,
GDI32 and 600+ other modules -- PE). This is the 32-bit half: enough to tier and measure the
corpus (section table, code size), feed the ordinal work (export directory, name<->ordinal), and
show a module's structure (PE Explorer's views).
"""
from codebreaker.formats import native


class PEFile:
    """A PE image read from `path`: e_lfanew, machine, image_base, entry, sections, datadirs ...
    (pe.parse's attributes), every method answered by formats.dll."""

    def __init__(self, path):
        native.required()
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        self.__dict__.update(native.call("pe.parse", self.data, path))

    @native.ask("pe.machine_name")
    def machine_name(self):
        pass

    @native.ask("pe.is_dll")
    def is_dll(self):
        pass

    @native.ask("pe._rva_to_off")
    def _rva_to_off(self, rva):
        pass

    @native.ask("pe.code_bytes")
    def code_bytes(self):
        """Sum of raw sizes of executable/code sections."""

    @native.ask("pe.module_name")
    def module_name(self):
        """The export directory's internal name, or '' if not exported."""

    @native.ask("pe.export_addrs")
    def export_addrs(self):
        """{name: rva} for every export that has a name."""

    @native.ask("pe.exports")
    def exports(self):
        """[(ordinal, name_or_None, rva)] from the export directory."""

    @native.ask("pe.imports")
    def imports(self):
        """{iat_slot_va: (dll, name)} from the import directory (data dir 1).

        A `call dword ptr [iat_slot_va]` in the code resolves to an imported
        API through this map — the PE analogue of NE's module.ordinal imports,
        and what lets pe_trace name cross-module calls instead of leaving them
        as raw `[0x....]`.  name is the imported symbol, or '#<ordinal>' for a
        by-ordinal import.
        """

    @native.ask("pe.code_sections")
    def code_sections(self):
        """[(va_start, bytes)] for executable sections."""

    @native.ask("pe.base_relocs")
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

    @native.ask("pe.build_meta")
    def build_meta(self):
        """Structural build metadata: machine, bitness, image base, entry, link
        timestamp, linker version, subsystem, EXE/DLL. All from the headers."""

    @native.ask("pe.debug_codeview")
    def debug_codeview(self):
        """The CodeView record from the DEBUG data directory (index 6), or None.

        Modern toolchains emit an RSDS record here pointing at the matching PDB
        (a GUID+age key); older ones emit NB10 (a timestamp+age key). This is the
        PE debug-directory form, distinct from the appended NB09 CodeView blob
        that symbol_engine/codeview.py reads.

        RSDS -> {'format':'RSDS','guid':str,'age':int,'pdb':str}
        NB10 -> {'format':'NB10','signature':int,'age':int,'pdb':str}
        """

    @native.ask("pe._u")
    def _u(self, fmt, off):
        """One little-endian value at a file offset, or 0 when it lies outside the file."""

    @native.ask("pe._text")
    def _text(self, off, limit=512):
        """The zero-terminated string at a file offset ('' when the offset is not in the file)."""

    @native.ask("pe.file_offset")
    def file_offset(self, rva):
        """File offset of an RVA. One that lies in the headers, before any section, is its own
        offset; one that lies nowhere is None."""

    @native.ask("pe.optional_header")
    def optional_header(self):
        """The optional header's fields by name (PE32 and PE32+)."""

    @native.ask("pe.directory")
    def directory(self, index):
        """(rva, size) of one data directory; (0, 0) when the image has none at that index."""

    @native.ask("pe.export_table")
    def export_table(self):
        """{'module', 'ordinal_base', 'timestamp', 'symbols': [{name, ordinal, hint, rva, forward}]}.
        `hint` is the name's index in the name table (-1 for an export by ordinal only);
        `forward` is 'OTHER.Function' when the export is passed on to another module."""

    @native.ask("pe.import_table")
    def import_table(self, delay=False):
        """[{'module', 'symbols': [{name, ordinal, hint, slot}]}] from the import directory, or from
        the delay-load directory. An import by ordinal has name '' and hint -1; one by name has
        ordinal -1. `slot` is the RVA of the address-table entry the loader fills in."""

    @native.ask("pe.resource_tree")
    def resource_tree(self):
        """[{'type': name or number, 'id', 'items': [{name, id, language, rva, offset, size}]}] --
        the resource directory's three levels (type, name, language) flattened to one list of
        items per type. `id` is None for a type or item that goes by a name."""

    @native.ask("pe.reloc_blocks")
    def reloc_blocks(self):
        """[(page_rva, entries)] -- the blocks of the base relocation directory."""

    @native.ask("pe.tls")
    def tls(self):
        """The thread-local storage directory's addresses, or None."""

    @native.ask("pe.debug_entries")
    def debug_entries(self):
        """[{type, timestamp, size, offset}] -- every record of the debug directory (`type` is the
        IMAGE_DEBUG_TYPE number)."""

    @native.ask("pe.certificates")
    def certificates(self):
        """[{length, revision, type}] -- the attribute certificates (this directory holds a FILE
        OFFSET, not an RVA)."""

    @native.ask("pe.clr")
    def clr(self):
        """The .NET runtime header, or None for a native image."""

    @native.ask("pe.rich")
    def rich(self):
        """The linker's 'Rich' record between the DOS stub and the PE header: {key, entries:
        [(product, build, count)]}, or None when the image has none."""


def open_pe(path):
    return PEFile(path)


@native.answered("pe.is_dotnet", data_arg=None)
def is_dotnet(path):
    """Whether the file is a .NET assembly: a PE image with a CLR runtime header (data directory
    14). Reads the headers only."""
