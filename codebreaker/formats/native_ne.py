"""native_ne.py -- ne.NEFile and ne.BootImageModule answered by the C++ (native/formats/ne).

While CB_NATIVE_FORMATS(_NE) is on, NEFile(path), NEFile.from_buffer(...) and BootImageModule
hand out these (ne.NEFile.__new__ picks them): the same attributes, every method asking
formats.dll with the module's place -- ne_off, base, and for a boot image's module its
overlay_at -- read from the object at each call. A temporary shim (docs/NATIVE.md, step 4).
"""
from codebreaker.formats import native, ne

METHODS = ("u16", "u32", "_read_name_table", "resident_names", "nonresident_names", "module_name", "module_refs",
           "imported_name", "segments", "relocations", "entries", "export_ordinals", "resources", "_rsrc_string")


def where(m):
    """A module's place, as every question takes it first."""
    return (m.ne_off, m.base, m.overlay_at if isinstance(m, ne.BootImageModule) else None)


class NEFile(ne.NEFile):
    """ne.NEFile with every reader in C++."""

    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        try:
            state = native.call("ne.open", self.data)
        except native.Fallback:
            return ne.NEFile.__init__(self, path)
        self.ne_off, self.h = state["ne_off"], state["h"]

    @classmethod
    def from_buffer(cls, data, ne_off, path="<buffer>"):
        self = object.__new__(cls)
        self.path, self.data, self.ne_off = path, data, ne_off
        try:
            self.h = native.call("ne.from_buffer", data, ne_off)
        except native.Fallback:
            if data[ne_off:ne_off + 2] != b"NE":
                raise ValueError(f"no NE at 0x{ne_off:x}")
            ne.NEFile._parse_header(self)
        self.base = ne_off
        return self

    def _parse_header(self):
        try:
            self.h = native.call("ne._parse_header", self.data, self.ne_off)
        except native.Fallback:
            ne.NEFile._parse_header(self)


class BootImageModule(NEFile, ne.BootImageModule):
    """ne.BootImageModule with every reader in C++ (segments, resources and non-resident names
    read from the overlay where the module keeps them there)."""


for _name in METHODS:
    setattr(NEFile, _name, native.asker("ne." + _name, getattr(ne.NEFile, _name), where))
for _name in ("segments", "resources", "nonresident_names"):
    setattr(BootImageModule, _name, native.asker("ne." + _name, getattr(ne.BootImageModule, _name), where))

CLASSES = {ne.NEFile: NEFile, ne.BootImageModule: BootImageModule}
