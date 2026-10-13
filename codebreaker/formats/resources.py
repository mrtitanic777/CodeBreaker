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
    """The resource directory of a 32-bit module: three levels (type, name, language)."""
    if len(pe.datadirs) < 3 or not pe.datadirs[2][0]:
        return []
    base = pe._rva_to_off(pe.datadirs[2][0])
    if base is None:
        return []
    d, out = pe.data, []

    def entries(off, seen):
        p = base + off
        if off in seen or p + 16 > len(d):
            return
        seen.add(off)
        named, ids = struct.unpack_from("<HH", d, p + 12)
        for i in range(named + ids):
            if p + 24 + i * 8 > len(d):
                return
            name, where = struct.unpack_from("<II", d, p + 16 + i * 8)
            if name & 0x80000000:
                q = base + (name & 0x7FFFFFFF)
                n = struct.unpack_from("<H", d, q)[0]
                name = d[q + 2:q + 2 + 2 * n].decode("utf-16-le", "replace")
            yield name, where & 0x7FFFFFFF, bool(where & 0x80000000)

    seen = set()
    for rtype, toff, tdir in entries(0, seen):
        if not tdir:
            continue
        for name, noff, ndir in entries(toff, seen):
            if not ndir:
                continue
            for lang, loff, ldir in entries(noff, seen):
                if ldir or base + loff + 16 > len(d):
                    continue
                rva, size = struct.unpack_from("<II", d, base + loff)
                off = pe._rva_to_off(rva)
                if off is None or off + size > len(d):
                    continue
                out.append(Resource(rtype, name, lang if isinstance(lang, int) else 0, 0, d[off:off + size]))
    return out


def read_ne(nef):
    """The resource table of a 16-bit module: per type, a list of (offset, length, flags, name)."""
    h, d = nef.h, nef.data
    if h["rsrc_tab"] == h["resname_tab"]:
        return []
    base = nef.ne_off + h["rsrc_tab"]

    def ident(v):
        if v & 0x8000:
            return v & 0x7FFF
        n = d[base + v]
        return d[base + v + 1:base + v + 1 + n].decode("latin-1")

    shift = nef.u16(base)
    if shift > 15:
        return []
    p, out = base + 2, []
    while p + 8 <= len(d):
        tid, count = nef.u16(p), nef.u16(p + 2)
        if tid == 0:
            break
        p += 8
        for _ in range(count):
            if p + 12 > len(d):
                return out
            off, length, flags, rid = nef.u16(p) << shift, nef.u16(p + 2) << shift, nef.u16(p + 4), nef.u16(p + 6)
            out.append(Resource(ident(tid), ident(rid), None, flags, d[off:off + length]))
            p += 12
    return out


def same_data(a, b):
    """Two resource bodies are the same when one is the other plus zero padding (a 16-bit
    module stores lengths in alignment units)."""
    n = min(len(a), len(b))
    return a[:n] == b[:n] and not any(a[n:]) and not any(b[n:])


# ---------------------------------------------------------------------------------------------
# reading fields
# ---------------------------------------------------------------------------------------------
class _Reader(object):
    """A cursor over a resource body; text and alignment follow `wide`."""

    def __init__(self, data, wide, at=0):
        self.d, self.wide, self.p = data, wide, at

    def u8(self):
        self.p += 1
        return self.d[self.p - 1]

    def u16(self):
        self.p += 2
        return struct.unpack_from("<H", self.d, self.p - 2)[0]

    def i16(self):
        self.p += 2
        return struct.unpack_from("<h", self.d, self.p - 2)[0]

    def u32(self):
        self.p += 4
        return struct.unpack_from("<I", self.d, self.p - 4)[0]

    def take(self, n):
        if self.p + n > len(self.d):
            raise ValueError("resource body ends early")
        self.p += n
        return self.d[self.p - n:self.p]

    def align(self, n=4):
        self.p = (self.p + n - 1) // n * n

    def text(self):
        """A zero-terminated string."""
        if self.wide:
            e = self.p
            while e + 2 <= len(self.d) and self.d[e:e + 2] != b"\0\0":
                e += 2
            if e + 2 > len(self.d):
                raise ValueError("unterminated text")
            s = self.d[self.p:e].decode("utf-16-le", "surrogatepass")
            self.p = e + 2
            return s
        e = self.d.index(b"\0", self.p)
        s = self.d[self.p:e].decode("latin-1")
        self.p = e + 1
        return s

    def text_or_ordinal(self):
        """A string, or an ordinal in its place (an int): 0xFFFF + word in a 32-bit resource,
        0xFF + word in a 16-bit one."""
        if self.wide:
            if struct.unpack_from("<H", self.d, self.p)[0] == 0xFFFF:
                self.p += 2
                return self.u16()
        elif self.d[self.p] == 0xFF:
            self.p += 1
            return self.u16()
        return self.text()


# ---------------------------------------------------------------------------------------------
# decoding each kind
# ---------------------------------------------------------------------------------------------
def strings(data, wide):
    """A string-table block: its 16 strings (block n holds the ids (n-1)*16 .. (n-1)*16+15)."""
    r, out = _Reader(data, wide), []
    for _ in range(16):
        if r.p >= len(data):
            out.append("")
            continue
        if wide:
            n = r.u16()
            out.append(r.take(2 * n).decode("utf-16-le", "surrogatepass"))
        else:
            out.append(r.take(r.u8()).decode("latin-1"))
    if any(data[r.p:]):
        raise ValueError("data after the sixteenth string")
    return out


def dialog(data, wide):
    """A dialog template -> {extended, style, exstyle, help, x, y, cx, cy, menu, class, title,
    font, controls}. `font` is None or (points, face) -- (points, weight, italic, charset, face)
    for an extended template. A control's `class` is a str or one of CONTROL_CLASSES' atoms."""
    r = _Reader(data, wide)
    dlg = {"extended": False, "help": 0, "exstyle": 0}
    if wide and data[:4] == b"\x01\x00\xff\xff":
        r.p = 4
        dlg["extended"] = True
        dlg["help"], dlg["exstyle"], dlg["style"] = r.u32(), r.u32(), r.u32()
        count = r.u16()
    else:
        dlg["style"] = r.u32()
        if wide:
            dlg["exstyle"] = r.u32()
        count = r.u16() if wide else r.u8()
    dlg["x"], dlg["y"], dlg["cx"], dlg["cy"] = r.i16(), r.i16(), r.i16(), r.i16()
    dlg["menu"], dlg["class"], dlg["title"] = r.text_or_ordinal(), r.text_or_ordinal(), r.text()
    dlg["font"] = None
    if dlg["style"] & 0x40:                                  # DS_SETFONT
        if dlg["extended"]:
            points, weight = r.u16(), r.u16()
            italic, charset = r.u8(), r.u8()
            dlg["font"] = (points, weight, italic, charset, r.text())
        else:
            points = r.u16()
            dlg["font"] = (points, r.text())
    dlg["controls"] = []
    for _ in range(count):
        c = {"help": 0, "exstyle": 0}
        if wide:
            r.align()
        if dlg["extended"]:
            c["help"], c["exstyle"], c["style"] = r.u32(), r.u32(), r.u32()
            c["x"], c["y"], c["cx"], c["cy"] = r.i16(), r.i16(), r.i16(), r.i16()
            c["id"] = r.u32()
        elif wide:
            c["style"], c["exstyle"] = r.u32(), r.u32()
            c["x"], c["y"], c["cx"], c["cy"] = r.i16(), r.i16(), r.i16(), r.i16()
            c["id"] = r.u16()
        else:
            c["x"], c["y"], c["cx"], c["cy"] = r.i16(), r.i16(), r.i16(), r.i16()
            c["id"], c["style"] = r.u16(), r.u32()
        if wide:
            c["class"] = r.text_or_ordinal()
        else:
            c["class"] = r.u8() if data[r.p] & 0x80 else r.text()
        c["text"] = r.text_or_ordinal()
        c["extra"] = bytes(r.take(r.u16() if wide else r.u8()))
        dlg["controls"].append(c)
    return dlg


def menu(data, wide):
    """A menu template -> {extended, help, items}. An item is {text, id, flags, items} (items is
    None unless it opens a submenu); in an extended menu it is {text, id, type, state, help, items}."""
    r = _Reader(data, wide)
    version, offset = r.u16(), r.u16()
    if version == 1 and wide:
        out = {"extended": True, "help": r.u32()}
        r.p = 4 + offset

        def level():
            items = []
            while True:
                r.align()
                item = {"type": r.u32(), "state": r.u32(), "id": r.u32(), "help": 0, "items": None}
                kind = r.u16()
                item["text"] = r.text()
                if kind & 0x01:                              # opens a submenu
                    r.align()
                    item["help"] = r.u32()
                    item["items"] = level()
                items.append(item)
                if kind & 0x80:
                    return items
    elif version == 0:
        out = {"extended": False, "help": 0}
        r.p = 4 + offset

        def level():
            items = []
            while True:
                flags = r.u16()
                item = {"flags": flags & ~0x80, "id": 0, "items": None}
                if not flags & 0x10:                         # MF_POPUP has no id
                    item["id"] = r.u16()
                item["text"] = r.text()
                if flags & 0x10:
                    item["items"] = level()
                items.append(item)
                if flags & 0x80:                             # MF_END
                    return items
    else:
        raise ValueError("menu template version %d" % version)
    out["items"] = level()
    return out


def accelerators(data, wide):
    """An accelerator table -> [(flags, key, id)]; the last entry's end mark is not in flags."""
    size, out = (8 if wide else 5), []
    for p in range(0, len(data) - size + 1, size):
        if wide:
            flags, key, ident = struct.unpack_from("<HHH", data, p)
        else:
            flags, key, ident = struct.unpack_from("<BHH", data, p)
        out.append((flags & 0x7F, key, ident))
        if flags & 0x80:
            break
    return out


def version(data, wide):
    """A version block -> (fixed, strings, translations): the 13 numbers of the fixed part (or
    None), {language block: [(key, text)]} and the translation words."""
    def node(p, limit):
        """-> (key, value bytes, is text, [children], end). A block that claims to run past
        the one holding it (some compilers of the time counted wrongly) is cut off there."""
        if wide:
            length, vlen, kind = struct.unpack_from("<HHH", data, p)
            r = _Reader(data, True, p + 6)
        else:
            length, vlen = struct.unpack_from("<HH", data, p)
            kind, r = 0, _Reader(data, False, p + 4)
        if length < 4:
            raise ValueError("version block length")
        end = min(p + length, limit)
        key = r.text()
        r.align()
        if wide and kind == 1 and r.p + 2 * vlen <= end:
            vlen *= 2                                        # counted in characters
        value = data[r.p:min(r.p + vlen, end)]
        q, kids = (r.p + vlen + 3) & ~3, []
        while q + 4 <= end:
            k = node(q, end)
            kids.append(k)
            q = (k[4] + 3) & ~3
        return key, value, kind == 1, kids, end

    def text_of(value):
        s = value.decode("utf-16-le", "replace") if wide else value.decode("latin-1")
        return s.split("\0", 1)[0]

    key, value, _t, kids, _e = node(0, len(data))
    if key != "VS_VERSION_INFO":
        raise ValueError("not a version block")
    fixed = struct.unpack_from("<13I", value) if len(value) >= 52 else None
    table, translations = {}, []
    for k, v, _t, sub, _e in kids:
        if k == "StringFileInfo":
            for lang, _v, _t2, entries, _e2 in sub:
                table[lang] = [(ek, text_of(ev)) for ek, ev, _t3, _s, _e3 in entries]
        elif k == "VarFileInfo":
            for _vk, vv, _t2, _s, _e2 in sub:
                translations += list(struct.unpack_from("<%dH" % (len(vv) // 2), vv))
        else:
            raise ValueError("a version block with a %r part" % k)
    return fixed, table, translations


def group(data):
    """An icon or cursor group -> (kind, [entry]): kind 1 = icons, 2 = cursors; an entry is
    (width, height, colours, planes, bits, bytes, member id)."""
    _zero, kind, count = struct.unpack_from("<HHH", data)
    if kind not in (1, 2) or 6 + 14 * count > len(data):
        raise ValueError("not an icon or cursor group")
    out = []
    for i in range(count):
        p = 6 + 14 * i
        if kind == 1:
            w, h, colours, _r, planes, bits, size, ident = struct.unpack_from("<BBBBHHIH", data, p)
        else:
            w, h, planes, bits, size, ident = struct.unpack_from("<HHHHIH", data, p)
            colours = 0
        out.append((w, h, colours, planes, bits, size, ident))
    return kind, out


def icon_file(group_data, member_data):
    """The .ico (or .cur) file a group resource and its members came from. `member_data(id)`
    gives a member's bytes. A cursor member starts with its hot spot, which a .cur file keeps in
    the directory instead."""
    kind, entries = group(group_data)
    head = struct.pack("<HHH", 0, kind, len(entries))
    at, table, body = 6 + 16 * len(entries), b"", b""
    for w, h, colours, planes, bits, _size, ident in entries:
        image = member_data(ident)
        if kind == 2:
            hot_x, hot_y = struct.unpack_from("<HH", image)
            image = image[4:]
            table += struct.pack("<BBBBHHII", w & 0xFF, (h // 2) & 0xFF, 0, 0, hot_x, hot_y, len(image), at)
        else:
            table += struct.pack("<BBBBHHII", w, h, colours, 0, planes, bits, len(image), at)
        body += image
        at += len(image)
    return head + table + body


def bitmap_file(data):
    """The .bmp file of a bitmap resource (the resource is the file without its 14-byte header)."""
    size = struct.unpack_from("<I", data)[0] if len(data) >= 4 else 0
    palette = 0
    if size == 12 and len(data) >= 12:                       # the OS/2 1.x header: 3-byte palette entries
        bits = struct.unpack_from("<H", data, 10)[0]
        palette = 3 * (1 << bits) if bits <= 8 else 0
    elif size >= 40 and len(data) >= 40:
        bits, compression = struct.unpack_from("<HI", data, 14)
        used = struct.unpack_from("<I", data, 32)[0]
        palette = 4 * (used or ((1 << bits) if bits <= 8 else 0)) + (12 if compression == 3 and size == 40 else 0)
    return b"BM" + struct.pack("<IHHI", 14 + len(data), 0, 0, 14 + size + palette) + data


_TYPE_KINDS = ("enum", "struct", "module", "interface", "dispinterface", "coclass", "typedef", "union")


def type_library(data):
    """What a type library says of itself -> {format, name, guid, types: [(kind, name, guid)]},
    or None when `data` is not one. Only the documented layout (MSFT) is read that far; of the
    earlier one (SLTG) just the format is reported. Either way a type library is carried as a
    binary payload: rebuilding one needs the interface compiler, which is not in this toolchain."""
    kind = {b"MSFT": "MSFT", b"SLTG": "SLTG"}.get(bytes(data[:4]))
    if not kind:
        return None
    out = {"format": kind, "name": "", "guid": "", "types": []}
    if kind == "SLTG" or len(data) < 0x54:
        return out
    try:
        own_guid, flags = struct.unpack_from("<i", data, 8)[0], struct.unpack_from("<I", data, 0x14)[0]
        count, own_name = struct.unpack_from("<I", data, 0x20)[0], struct.unpack_from("<i", data, 0x38)[0]
        # after the header: one offset per type (and one more word when the library names a
        # help-string DLL), then the directory of the file's fifteen segments
        directory = 0x54 + 4 * count + (4 if flags & 0x100 else 0)
        segments = [struct.unpack_from("<ii", data, directory + 16 * i) for i in range(15)]
        info_at, guid_at, name_at = segments[0][0], segments[5][0], segments[7][0]

        def guid(at):
            if at < 0:
                return ""
            b = data[guid_at + at:guid_at + at + 16]
            a, c, e = struct.unpack_from("<IHH", b)
            return "%08X-%04X-%04X-%s-%s" % (a, c, e, b[8:10].hex().upper(), b[10:16].hex().upper())

        def name(at):
            if at < 0:
                return ""
            length = data[name_at + at + 8]
            return data[name_at + at + 12:name_at + at + 12 + length].decode("latin-1")
        out["name"], out["guid"] = name(own_name), guid(own_guid)
        for n in range(count):
            at = info_at + 0x64 * n                          # one fixed-size record per type
            k = struct.unpack_from("<I", data, at)[0] & 0xF
            out["types"].append((_TYPE_KINDS[k] if k < len(_TYPE_KINDS) else "type %d" % k,
                                 name(struct.unpack_from("<i", data, at + 0x34)[0]),
                                 guid(struct.unpack_from("<i", data, at + 0x2C)[0])))
    except (struct.error, IndexError):
        pass
    return out


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
