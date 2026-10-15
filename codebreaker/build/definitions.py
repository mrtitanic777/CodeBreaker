"""definitions.py -- what a module offers and what it needs: exports, imports, and their source forms.

A module's interface is read from the original and written three ways: a module-definition
file (`.def`, the form a person reads and edits), linker directives (what the build passes to
the linker), and a C header declaring the exported functions.

    from codebreaker.build import definitions
    definitions.exports(path)      # [Export]: ordinal, name, where it points, what kind of thing
    definitions.imports(path)      # {(module, name or ordinal)}: every function it takes from others
    definitions.definition_file(path)                   # the text of a .def
    definitions.export_directives(path, symbol_at)      # (linker lines, [(export, why it is missing)])
    definitions.public_header(path, declaration_at)     # the text of a C header

How the linker is told (facts found the hard way):

  * An export is tied to the function AT ITS ADDRESS, not to a function of the same name: the
    reconstruction's name for a function need not be the exported name, and an export by
    ordinal has no name at all. `symbol_at(address)` gives the object-file symbol there.
  * A symbol holding `@`, `?` or `$` (a decorated __stdcall name, a C++ name) must be quoted:
    unquoted, the linker reads `@` as "take directives from this file". Quoting is harmless
    for a plain symbol in an export, so every symbol is quoted.
  * The linker cannot export by ordinal alone: such an export gets a made-up name
    (`ordinal_N`), which the build's check reports as an extra name.
  * A 16-bit module is a library when its header says so (flag 0x8000), whatever its file is
    called -- KRNL386, USER and GDI are libraries named .EXE.
"""
import os
import re
import struct

from codebreaker.base import corpus
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe


class Export(object):
    """One export. `address` is a virtual address (32-bit) or (segment, offset) (16-bit);
    `kind` is "code", "data" or "forward" (`target` then names the function elsewhere);
    `resident` says a 16-bit name is in the resident table."""
    __slots__ = ("ordinal", "name", "address", "kind", "resident", "target", "shared_data")

    def __init__(self, ordinal, name, address, kind, resident=False, target="", shared_data=True):
        self.ordinal, self.name, self.address = ordinal, name, address
        self.kind, self.resident, self.target = kind, resident, target
        self.shared_data = shared_data       # 16-bit: the loader gives it the module's data segment

    def __repr__(self):
        return "<export %s @%d %s>" % (self.name or "(no name)", self.ordinal, self.kind)


def is_library(path):
    """Whether a module is a DLL: the header's flag, not the file's name."""
    if corpus.lane(path) == "ne":
        return bool(NEFile(path).h["flags"] & 0x8000)
    return open_pe(path).is_dll()


def module_name(path):
    """The name the module calls itself (falls back to the file's name)."""
    stem = os.path.splitext(os.path.basename(path))[0].upper()
    try:
        if corpus.lane(path) == "ne":                         # (a name field padded with NULs or spaces)
            return NEFile(path).module_name().replace(chr(0), " ").strip() or stem
        return os.path.splitext(open_pe(path).module_name())[0].upper() or stem
    except (ValueError, IndexError, struct.error):
        return stem


def description(path):
    """A 16-bit module's description line (the first entry of its non-resident names)."""
    if corpus.lane(path) != "ne":
        return ""
    names = NEFile(path).nonresident_names()
    return names[0][0].replace(chr(0), " ").strip() if names else ""


def exports(path):
    """Every export of a module, by ordinal."""
    out = []
    if corpus.lane(path) == "ne":
        nef = NEFile(path)
        segs = {s["idx"]: s for s in nef.segments()}
        names = {}
        for resident, table in ((True, nef.resident_names()[1:]), (False, nef.nonresident_names()[1:])):
            for name, ordinal in table:
                names.setdefault(ordinal, (name, resident))
        for e in nef.entries():
            if e.get("kind") not in ("FIXED", "MOVEABLE"):
                continue
            name, resident = names.get(e["ordinal"], (None, False))
            if not name and not e["flags"] & 1:              # an entry point for the module's own far
                continue                                     # calls between segments, not an export
            seg = segs.get(e["seg"])
            if e["seg"] == 0xFE:                             # a constant, not a place in the module
                kind = "data"
            else:
                kind = "data" if seg is None or seg["kind"] == "DATA" else "code"
            out.append(Export(e["ordinal"], name, (e["seg"], e["offset"]), kind, resident,
                              shared_data=bool(e["flags"] & 2)))
        return out
    pe = open_pe(path)
    lo, size = pe.datadirs[0] if pe.datadirs else (0, 0)
    code = [(s["vaddr"], s["vaddr"] + max(s["vsize"], s["rawsize"])) for s in pe.sections if s["chars"] & 0x20000020]
    for ordinal, name, rva in pe.exports():
        if lo <= rva < lo + size:                            # inside the export table: a forwarder's text
            off = pe._rva_to_off(rva)
            target = pe.data[off:pe.data.index(b"\0", off)].decode("latin-1")
            out.append(Export(ordinal, name, 0, "forward", target=target))
        else:
            kind = "code" if any(a <= rva < b for a, b in code) else "data"
            out.append(Export(ordinal, name, pe.image_base + rva, kind))
    return sorted(out, key=lambda e: e.ordinal)


def imports(path):
    """{(MODULE, name or ordinal)} -- everything a module takes from other modules. A 16-bit
    module's imports are read from its relocations (it has no import list as such)."""
    out = set()
    if corpus.lane(path) == "ne":
        nef = NEFile(path)
        refs = nef.module_refs()
        for s in nef.segments():
            for r in nef.relocations(s):
                if r["kind"] not in ("IMPORTORD", "IMPORTNAME"):
                    continue
                mod = refs[r["module"] - 1].upper() if 0 < r["module"] <= len(refs) else "?"
                out.add((mod, r["ordinal"] if r["kind"] == "IMPORTORD" else nef.imported_name(r["name_off"])))
        return out
    for dll, name in open_pe(path).imports().values():
        mod = os.path.splitext(dll)[0].upper()
        out.add((mod, int(name[1:]) if name.startswith("#") else name))
    return out


def definition_file(path):
    """The module's interface as a module-definition file."""
    ne = corpus.lane(path) == "ne"
    lines = ["; %s -- module definition, read from the original's own tables." % os.path.basename(path).lower(),
             "%s %s" % ("LIBRARY" if is_library(path) else "NAME", module_name(path))]
    if description(path):
        lines.append("DESCRIPTION '%s'" % description(path).replace("'", "''"))
    if ne:
        nef = NEFile(path)
        lines += ["EXETYPE WINDOWS", "HEAPSIZE %d" % nef.h["heap"]] + (
            ["STACKSIZE %d" % nef.h["stack"]] if nef.h["stack"] else [])
    found = exports(path)
    if found:
        lines.append("EXPORTS")
    for e in found:
        text = "    %-32s @%d" % (e.name or "ordinal_%d" % e.ordinal, e.ordinal)
        if e.kind == "forward":
            text = "    %s = %s @%d" % (e.name, e.target, e.ordinal)
        if not e.name:
            text += " NONAME"
        if ne and e.resident:
            text += " RESIDENTNAME"
        if e.kind == "data":
            text += " DATA" if not ne else "        ; data"
        lines.append(text.rstrip())
    taken = sorted(imports(path), key=lambda i: (i[0], str(i[1])))
    if taken:
        lines.append("IMPORTS")
        lines += ["    %s.%s" % (mod, what) for mod, what in taken]
    return "\n".join(lines) + "\n"


def export_directives(path, symbol_at):
    """Linker `export` lines for every export whose address has a symbol in the build.
    `symbol_at(export)` -> the object-file symbol for the thing the export points at, or None.
    -> ([line], [(Export, why it could not be exported)])"""
    ne = corpus.lane(path) == "ne"
    lines, missing = [], []
    for e in exports(path):
        if e.kind == "forward":
            missing.append((e, "forwarded to %s: this linker cannot forward" % e.target))
            continue
        symbol = symbol_at(e)
        if not symbol:
            missing.append((e, "exported data is not a symbol of its own" if e.kind == "data"
                            else "no function was reconstructed at its address"))
            continue
        name = e.name or "ordinal_%d" % e.ordinal
        if not re.fullmatch(r"[A-Za-z_][\w]*", name):
            name = "'%s'" % name
        # (NODATA: the entry is not marked as using the module's data segment -- the loader then
        # leaves its prologue alone, and the function runs on its caller's DS, as the original's)
        lines.append("export %s.%d='%s'%s%s" % (name, e.ordinal, symbol, " resident" if ne and e.resident else "",
                                               " nodata" if ne and e.kind == "code" and not e.shared_data else ""))
    return lines, missing


def public_header(path, declaration_at):
    """A C header declaring the module's exported functions. `declaration_at(export)` gives
    the reconstruction's declaration of the function an export points at, or None."""
    stem = re.sub(r"\W", "_", os.path.splitext(os.path.basename(path))[0]).upper()
    out = ["/* %s -- the functions %s exports, as reconstructed. Argument names and types are the"
           % (os.path.basename(path).lower().rsplit(".", 1)[0] + "_public.h", module_name(path)),
           " * decompiler's; the export names and ordinals are the original's. */",
           "#ifndef %s_PUBLIC_H" % stem, "#define %s_PUBLIC_H" % stem, ""]
    for e in exports(path):
        label = "%s @%d" % (e.name or "(by ordinal only)", e.ordinal)
        decl = declaration_at(e) if e.kind == "code" else None
        if decl:
            out.append("%-72s /* %s */" % (decl, label))
        else:
            out.append("/* %s: %s */" % (label, {"forward": "forwarded to " + e.target, "data": "data"}.get(
                e.kind, "no reconstruction at its address")))
    return "\n".join(out + ["", "#endif", ""])
