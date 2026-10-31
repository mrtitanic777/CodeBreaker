"""native_pe.py -- pe.PEFile answered by the C++ (native/formats/pe), through native.py.

pe.open_pe hands one out while CB_NATIVE_FORMATS(_PE) is on: the same attributes, every method
asking formats.dll. A temporary shim (docs/NATIVE.md, step 4).
"""
from codebreaker.formats import native, pe

METHODS = ("machine_name", "is_dll", "_rva_to_off", "code_bytes", "module_name", "export_addrs", "exports",
           "imports", "code_sections", "base_relocs", "build_meta", "debug_codeview", "_u", "_text",
           "file_offset", "optional_header", "directory", "export_table", "import_table", "resource_tree",
           "reloc_blocks", "tls", "debug_entries", "certificates", "clr", "rich")


class PEFile(pe.PEFile):
    """pe.PEFile with every reader in C++."""

    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        try:
            self.__dict__.update(native.call("pe.parse", self.data, path))
        except native.Fallback:
            pe.PEFile.__init__(self, path)


for _name in METHODS:
    setattr(PEFile, _name, native.asker("pe." + _name, getattr(pe.PEFile, _name)))
