#!/usr/bin/env python3
"""
ne.py -- the New Executable (NE) reader, answered by the C++ (native/formats/ne: formats.dll,
through native.py). Its Python reader is gone (docs/NATIVE.md, step 4); the porting engine's copy
of it, native/ported/ne's input, is native/ported/ne/src_python/ne.py. This module is the API: each
method's def is its signature and documentation, and formats.dll answers.

The NE format debuted with Windows 1.0 (1985). The reader gives the DOS MZ stub, the NE header,
segment table, resident/non-resident name tables (exports), module-reference + imported-name
tables (imports), the entry table, and the resource table.
"""
from codebreaker.formats import native

PRELOAD = 0x0040                    # segment / resource flag: loaded at start (else on demand)


def where(m):
    """A module's place, as every question takes it first."""
    return (m.ne_off, m.base, m.overlay_at if isinstance(m, BootImageModule) else None)


class NEFile:
    """An NE module read from `path` (ne_off, h: the header's fields), every method answered by
    formats.dll with the module's place."""
    base = 0   # byte added to segment sector offsets (nonzero for embedded modules)

    def __init__(self, path):
        native.required()
        self.path = path
        self.data = open(path, "rb").read()
        state = native.call("ne.open", self.data)
        self.ne_off, self.h = state["ne_off"], state["h"]

    @classmethod
    def from_buffer(cls, data, ne_off, path="<buffer>"):
        """Parse an NE module that begins directly at ne_off (no MZ stub).
        Segment sector offsets are relative to ne_off for such embedded modules."""
        native.required()
        self = object.__new__(cls)
        self.path, self.data, self.ne_off = path, data, ne_off
        self.h = native.call("ne.from_buffer", data, ne_off)
        self.base = ne_off   # segment offsets are relative to this
        return self

    def _parse_header(self):
        self.h = native.call("ne._parse_header", self.data, self.ne_off)

    @native.ask("ne.u16", where)
    def u16(self, off):
        pass

    @native.ask("ne.u32", where)
    def u32(self, off):
        pass

    @native.ask("ne._read_name_table", where)
    def _read_name_table(self, file_off):
        """Returns list of (name, ordinal). First entry = module/description."""

    @native.ask("ne.resident_names", where)
    def resident_names(self):
        pass

    @native.ask("ne.nonresident_names", where)
    def nonresident_names(self):
        pass

    @native.ask("ne.module_name", where)
    def module_name(self):
        """This module's own name — the first resident-name-table entry."""

    @native.ask("ne.module_refs", where)
    def module_refs(self):
        """Imported module names, in module-reference-table order (1-based)."""

    @native.ask("ne.imported_name", where)
    def imported_name(self, off):
        pass

    @native.ask("ne.segments", where)
    def segments(self):
        pass

    @native.ask("ne.relocations", where)
    def relocations(self, seg):
        """Parse per-segment relocation records (if RELOCINFO set)."""

    @native.ask("ne.export_ordinals", where)
    def export_ordinals(self):
        """The ordinals the entry table actually exports, in order (unused gaps skipped).
        Exports by ordinal only have no resident name, so the name tables alone miss them."""

    @native.ask("ne.entries", where)
    def entries(self):
        """Parse entry-table bundles -> list of dict(ordinal, seg, offset, flags)."""

    @native.ask("ne.resources", where)
    def resources(self):
        pass

    @native.ask("ne._rsrc_string", where)
    def _rsrc_string(self, base, off):
        pass


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


def _place(nef):
    """(bytes, the module's place) as the C++ reader takes a module."""
    return nef.data, where(nef)


@native.answered("ne.embedded")
def embedded(data, start=0x40):
    """[(offset, module name)] of every NE header inside a file that holds several modules."""


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


@native.answered("ne._tables_end", args_of=_place)
def _tables_end(nef):
    """Offset just past the header and every table that belongs with it."""


@native.answered("ne.standalone", args_of=_place)
def standalone(nef):
    """The bytes of an ordinary one-file executable holding the module `nef` -- for a module
    read out of a boot image, so that everything that takes a path can take it. Header and
    tables are kept as they are; segments (with their relocations), resources and the
    non-resident names are laid out after them and the tables pointed at the new places."""
