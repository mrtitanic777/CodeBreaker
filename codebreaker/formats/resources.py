"""resources.py -- the resources of a Windows module: reading them out and decoding each kind.

A module's dialogs, menus, strings, icons and version block are source its authors wrote, kept in
the file as data. This reads the resource tree of a 32-bit (PE) or 16-bit (NE) module and decodes
the kinds whose layout is known into plain Python values that say what the resource IS -- so two
builds of a resource can be compared by meaning, and so it can be written back out as `.rc` text
(build/resources.py does that).

    from codebreaker.formats import resources
    for r in resources.read(path):            # every resource of a PE or NE file
        r.type, r.name, r.language, r.flags, r.data
    resources.type_name(r.type)               # "DIALOG", "STRING", ... or the type as it stands
    resources.decode(r, wide)                 # the resource as a value; None when its kind is opaque
    resources.icon_file(group, member_data)   # a whole .ico / .cur file from a group resource
    resources.bitmap_file(data)               # a .bmp file from a bitmap resource
    resources.type_library(data)              # what a type library says of itself (name, GUIDs, types)

`wide` is True for a 32-bit module (text is UTF-16, fields are aligned to four bytes) and False
for a 16-bit one (text is single-byte, fields are packed). A type or a name is an int (an
ordinal) or a str.

Two things about 16-bit modules: a resource's length is stored in alignment units, so its data
comes with up to a unit of padding after it (`same_data` compares with that in mind); and a
16-bit resource has no language.
"""
import struct

from codebreaker.formats import native
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe

CURSOR, BITMAP, ICON, MENU, DIALOG, STRING, FONTDIR, FONT, ACCELERATOR, RCDATA = range(1, 11)
MESSAGETABLE, GROUP_CURSOR, GROUP_ICON, VERSION = 11, 12, 14, 16
TYPES = {1: "CURSOR", 2: "BITMAP", 3: "ICON", 4: "MENU", 5: "DIALOG", 6: "STRING", 7: "FONTDIR",
         8: "FONT", 9: "ACCELERATOR", 10: "RCDATA", 11: "MESSAGETABLE", 12: "GROUP_CURSOR",
         14: "GROUP_ICON", 16: "VERSION", 17: "DLGINCLUDE", 19: "PLUGPLAY", 20: "VXD",
         21: "ANICURSOR", 22: "ANIICON", 23: "HTML", 24: "MANIFEST"}
MEMBER_OF = {GROUP_ICON: ICON, GROUP_CURSOR: CURSOR}
CONTROL_CLASSES = {0x80: "BUTTON", 0x81: "EDIT", 0x82: "STATIC", 0x83: "LISTBOX", 0x84: "SCROLLBAR",
                   0x85: "COMBOBOX"}


class Resource(object):
    """One resource: `type` and `name` (int or str), `language` (None in a 16-bit module),
    `flags` (the 16-bit memory flags, else 0) and its `data`."""
    __slots__ = ("type", "name", "language", "flags", "data")

    def __init__(self, type, name, language, flags, data):          # noqa: A002 (the format's own word)
        self.type, self.name, self.language, self.flags, self.data = type, name, language, flags, data

    @property
    def key(self):
        return (self.type, self.name, self.language)

    def __repr__(self):
        return "<%s %r lang=%r %d bytes>" % (type_name(self.type), self.name, self.language, len(self.data))


def type_name(t):
    return TYPES.get(t, "#%d" % t) if isinstance(t, int) else t


# ---------------------------------------------------------------------------------------------
# reading the tree
# ---------------------------------------------------------------------------------------------
def read(path):
    """Every resource of a module, in the order the file lists them. [] when it has none."""
    with open(path, "rb") as f:
        head = f.read(0x40)
        if len(head) < 0x40 or head[:2] != b"MZ":
            raise ValueError("%s: not an executable" % path)
        f.seek(struct.unpack_from("<I", head, 0x3C)[0])
        sig = f.read(2)
    if sig == b"PE":
        return read_pe(open_pe(path))
    if sig == b"NE":
        return read_ne(NEFile(path))
    return []


def read_pe(pe):
    """The resources of a 32-bit module (the directory itself is walked by PEFile.resource_tree)."""
    out = []
    for t in pe.resource_tree():
        rtype = t["id"] if t["id"] is not None else t["type"]
        for item in t["items"]:
            off, size = item.get("offset"), item.get("size") or 0
            if off is None or off + size > len(pe.data):
                continue
            name = item["id"] if item.get("id") is not None else item["name"]
            out.append(Resource(rtype, name, item.get("language") or 0, 0, pe.data[off:off + size]))
    return out


def read_ne(nef):
    """The resource table of a 16-bit module: per type, a list of (offset, length, flags, name)
    (formats.dll: native/formats/win/resources.cc)."""
    h = nef.h
    return [Resource(t, n, None, f, d)
            for t, n, f, d in native.call("resources.read_ne", nef.data, nef.ne_off, h["rsrc_tab"], h["resname_tab"])]


def same_data(a, b):
    """Two resource bodies are the same when one is the other plus zero padding (a 16-bit
    module stores lengths in alignment units)."""
    n = min(len(a), len(b))
    return a[:n] == b[:n] and not any(a[n:]) and not any(b[n:])


# ---------------------------------------------------------------------------------------------
# decoding each kind (formats.dll: native/formats/win/resources.cc)
# ---------------------------------------------------------------------------------------------
@native.answered("resources.strings", args_of=lambda data, wide: (data, (wide,)))
def strings(data, wide):
    """A string-table block: its 16 strings (block n holds the ids (n-1)*16 .. (n-1)*16+15)."""


@native.answered("resources.dialog", args_of=lambda data, wide: (data, (wide,)))
def dialog(data, wide):
    """A dialog template -> {extended, style, exstyle, help, x, y, cx, cy, menu, class, title,
    font, controls}. `font` is None or (points, face) -- (points, weight, italic, charset, face)
    for an extended template. A control's `class` is a str or one of CONTROL_CLASSES' atoms."""


@native.answered("resources.menu", args_of=lambda data, wide: (data, (wide,)))
def menu(data, wide):
    """A menu template -> {extended, help, items}. An item is {text, id, flags, items} (items is
    None unless it opens a submenu); in an extended menu it is {text, id, type, state, help, items}."""


@native.answered("resources.accelerators", args_of=lambda data, wide: (data, (wide,)))
def accelerators(data, wide):
    """An accelerator table -> [(flags, key, id)]; the last entry's end mark is not in flags."""


@native.answered("resources.version", args_of=lambda data, wide: (data, (wide,)))
def version(data, wide):
    """A version block -> (fixed, strings, translations): the 13 numbers of the fixed part (or
    None), {language block: [(key, text)]} and the translation words."""


@native.answered("resources.group")
def group(data):
    """An icon or cursor group -> (kind, [entry]): kind 1 = icons, 2 = cursors; an entry is
    (width, height, colours, planes, bits, bytes, member id)."""


def icon_file(group_data, member_data):
    """The .ico (or .cur) file a group resource and its members came from. `member_data(id)`
    gives a member's bytes. A cursor member starts with its hot spot, which a .cur file keeps in
    the directory instead."""
    members = [member_data(entry[6]) for entry in group(group_data)[1]]
    return native.call("resources.icon_file", group_data, members)


@native.answered("resources.bitmap_file")
def bitmap_file(data):
    """The .bmp file of a bitmap resource (the resource is the file without its 14-byte header)."""


@native.answered("resources.type_library")
def type_library(data):
    """What a type library says of itself -> {format, name, guid, types: [(kind, name, guid)]},
    or None when `data` is not one. Only the documented layout (MSFT) is read that far; of the
    earlier one (SLTG) just the format is reported. Either way a type library is carried as a
    binary payload: rebuilding one needs the interface compiler, which is not in this toolchain."""


def type_library_outline(library):
    """A type library's contents as an outline in interface-definition syntax: for reading, not
    for compiling (members and their types are not in it)."""
    out = ["// Outline of a type library (%s layout). Not compilable: only what each type is called." % library["format"]]
    if library["guid"]:
        out.append("[ uuid(%s) ]" % library["guid"])
    out.append("library %s\n{" % (library["name"] or "unnamed"))
    for kind, name, guid in library["types"]:
        out.append("    %s%s %s;" % ("[ uuid(%s) ] " % guid if guid else "", kind, name or "/* no name */"))
    return "\n".join(out + ["}", ""])


_DECODERS = {STRING: strings, DIALOG: dialog, MENU: menu, ACCELERATOR: accelerators, VERSION: version}


def decode(resource, wide):
    """A resource as a value that means the same for any two encodings of it, or None when its
    kind is not one this module decodes (it is then compared as bytes)."""
    fn = _DECODERS.get(resource.type)
    if fn is None:
        return None
    try:
        value = fn(resource.data, wide)
        if fn is version and value[0]:
            value = (value[0][:11],) + value[1:]             # without the file date: a compiler stamps it
        return value
    except (ValueError, IndexError, struct.error, UnicodeDecodeError):
        return None
