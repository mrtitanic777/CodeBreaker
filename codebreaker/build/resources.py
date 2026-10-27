"""resources.py -- a module's resources as source: `.rc` text and payload files, compiled back to `.res`.

    python -m codebreaker resources <module> [--out DIR]

    from codebreaker.build import resources
    r = resources.rebuild(module_path, outdir)      # write, compile, compare; r.res is the .res
    r.total, r.identical, r.equivalent, r.different, r.missing
    r.as_text, r.as_files, r.opaque, r.fallback     # how each resource was written

What is written, per kind of resource:

    text        string tables, dialogs, menus, accelerator tables and version blocks are decoded
                (formats/resources.py) and written as the `.rc` statements they came from
    files       an icon or cursor group with its member images becomes a real .ico / .cur file,
                a bitmap a .bmp, each named by an ICON / CURSOR / BITMAP statement
    opaque      kinds with no source form here (fonts, type libraries, raw data, anything
                user-defined) are carried as a payload file named by a raw-data statement

The `.rc` is compiled with OpenWatcom's resource compiler and the result is compared with the
original, resource by resource: IDENTICAL bytes, or EQUIVALENT (it decodes to the same thing --
the compiler pads, orders or numbers something differently). A resource that comes back
different, or whose statement the compiler rejects, is written again as a payload of its
original bytes ("fallback"), so the `.res` always holds the module's resources; how many needed
that is reported, because those are not yet source.

EQUIVALENT covers three things the compiler decides for itself: it numbers the member images of
an icon or cursor group, it pads, and it stamps the build time into a version block's file date
(the original's is zero).

Facts about the compiler, each found by a round trip that failed:

  * `-zm` makes it write the Microsoft `.res` layout (its default is a layout of its own). That
    file is for comparing; the LINKER must be given the default layout (`r.link_res`), because
    from a Microsoft-layout file it binds every resource with language 0.
  * In the Microsoft layout it does not pad bodies consistently, so entries do not always start
    on four-byte boundaries (`read_res` looks for the next entry rather than computing it).
  * A quoted type or name is taken literally: a resource called MENU is not the keyword.
  * A CONTROL statement's style has WS_CHILD and WS_VISIBLE unless they are taken out with NOT.
  * A string ends at a zero character, however it is written: text with one inside has no
    source form.
  * Text outside Latin-1 needs a UTF-8 source (`-zku8`), and there the compiler miscounts a
    string table's strings (UTF-8 bytes instead of characters): in a module that needs UTF-8,
    string tables with anything beyond ASCII are carried as payloads.
  * It writes a 16-bit version block with lengths that do not add up; 16-bit version blocks
    are carried as payloads.
  * It keeps only 16 bits of a control's or menu item's identifier, also in the extended
    templates that have room for 32 (where -1 is common): those are carried as payloads.
  * Without the preprocessor (`-zn`) a comment is a syntax error.
  * Like every OpenWatcom tool it has to be started by its full path.
"""
import os
import re
import struct
import subprocess

from codebreaker.base import corpus
from codebreaker.build import watcom
from codebreaker.formats import resources as R

_MEMORY = ((0x40, "PRELOAD", "LOADONCALL"), (0x10, "MOVEABLE", "FIXED"), (0x1000, "DISCARDABLE", ""),
           (0x20, "PURE", "IMPURE"))
_MENU_OPTIONS = ((0x01, "GRAYED"), (0x02, "INACTIVE"), (0x08, "CHECKED"), (0x20, "MENUBARBREAK"),
                 (0x40, "MENUBREAK"), (0x4000, "HELP"))
_KEY_OPTIONS = ((0x02, "NOINVERT"), (0x04, "SHIFT"), (0x08, "CONTROL"), (0x10, "ALT"))
_ERROR_LINE = re.compile(r"\((\d+)\): Error!")


# How text is written, which is also how a resource is decoded: 0 for a 16-bit module, WIDE for a
# 32-bit one whose text fits Latin-1, UNICODE for a 32-bit one with text beyond it.
WIDE, UNICODE = 1, 2


class NoSourceForm(Exception):
    """This resource uses something the `.rc` statements written here cannot say."""


class BeyondLatin1(Exception):
    """The module has text outside Latin-1: its `.rc` has to be UTF-8."""


class Result(object):
    """What a rebuild produced and how it compares with the original."""

    def __init__(self):
        self.rc = self.res = self.link_res = None           # link_res: the .res in the layout the linker binds
        self.total = self.identical = self.equivalent = 0
        self.different, self.missing, self.extra = [], [], []
        self.as_text = self.as_files = self.opaque = self.fallback = 0
        self.not_source = []                                 # keys of the resources carried as fallback payloads
        self.error = ""

    @property
    def same(self):
        return not self.error and not self.different and not self.missing and not self.extra and \
            self.identical + self.equivalent == self.total

    def line(self):
        if self.error:
            return "FAILED: %s" % self.error
        return ("%d resources: %d identical, %d equivalent, %d different, %d missing%s  "
                "(written as text %d, as image files %d, opaque payload %d, fallback payload %d)" % (
                    self.total, self.identical, self.equivalent, len(self.different), len(self.missing),
                    ", %d that the original does not have" % len(self.extra) if self.extra else "",
                    self.as_text, self.as_files, self.opaque, self.fallback))


# ---------------------------------------------------------------------------------------------
# the pieces of a statement
# ---------------------------------------------------------------------------------------------
def quoted(text, wide, counted=False):
    """A string literal. Control characters and everything above ASCII are written as
    three-digit octal escapes (a fixed width, so a digit after one cannot join it, and no code
    page is involved: the compiler takes each as one character of that value). That reaches
    Latin-1. A 32-bit module with text beyond it is written in UTF-8 instead, where the compiler
    gets a length-counted string (`counted`: a string table's) wrong -- it counts UTF-8 bytes --
    so such a string has no source form there. Nor has a string with a zero character in it
    anywhere: the compiler ends the string at it."""
    out = []
    for ch in text:
        c = ord(ch)
        if ch == '"':
            out.append('""')
        elif ch == "\\":
            out.append("\\\\")
        elif ch in "\n\t\r":
            out.append({"\n": "\\n", "\t": "\\t", "\r": "\\r"}[ch])
        elif c == 0:
            raise NoSourceForm("a zero character inside a string")
        elif c < 0x20 or c == 0x7F:
            out.append("\\%03o" % c)
        elif c <= 0x7F:
            out.append(ch)
        elif wide != UNICODE:
            if c > 0xFF:
                raise BeyondLatin1()
            out.append("\\%03o" % c)
        elif counted or 0xD800 <= c <= 0xDFFF:
            raise NoSourceForm("text the compiler cannot take")
        else:
            out.append(ch)
    return '"%s"' % "".join(out)


def ident(value, wide):
    """A resource's name or type as a token: a number, or the string in quotes."""
    return str(value) if isinstance(value, int) else quoted(value, wide)


def _memory(flags, wide):
    if wide:
        return ""
    return "".join(" " + (on if flags & bit else off) for bit, on, off in _MEMORY if (on if flags & bit else off))


def _number(value):
    """An identifier. An extended dialog or menu has room for 32 bits, but the compiler keeps 16
    however the number is written (-1 comes out as 0xFFFF)."""
    if value > 0xFFFF:
        raise NoSourceForm("an identifier wider than 16 bits")
    return "%d" % value


def _style(value):
    """A control's style: the two bits a CONTROL statement adds by itself are removed when the
    original does not have them."""
    s = "0x%08XL" % value
    if not value & 0x10000000:
        s += " | NOT 0x10000000L"
    if not value & 0x40000000:
        s += " | NOT 0x40000000L"
    return s


def _stem(resource):
    def part(v):
        return str(v) if isinstance(v, int) else re.sub(r"[^A-Za-z0-9]", "_", v)
    s = "%s_%s" % (R.type_name(resource.type).replace("#", "type"), part(resource.name))
    return re.sub(r"[^A-Za-z0-9_]", "_", s).lower() + ("_%04x" % resource.language if resource.language else "")


# ---------------------------------------------------------------------------------------------
# statements for the kinds that have a source form
# ---------------------------------------------------------------------------------------------
def dialog_text(r, wide):
    d = R.dialog(r.data, wide)
    head = "%s %s%s %d, %d, %d, %d" % (ident(r.name, wide), "DIALOGEX" if d["extended"] else "DIALOG",
                                      _memory(r.flags, wide), d["x"], d["y"], d["cx"], d["cy"])
    if d["help"]:
        head += ", %d" % d["help"]
    out = [head, "STYLE 0x%08XL" % d["style"]]
    if d["exstyle"]:
        out.append("EXSTYLE 0x%08XL" % d["exstyle"])
    if d["title"]:
        out.append("CAPTION %s" % quoted(d["title"], wide))
    if d["menu"] != "":
        out.append("MENU %s" % ident(d["menu"], wide))
    if d["class"] != "":
        out.append("CLASS %s" % ident(d["class"], wide))
    if d["font"]:
        f = d["font"]
        out.append("FONT %d, %s" % (f[0], quoted(f[-1], wide)) + (", %d, %d, %d" % f[1:4] if len(f) == 5 else ""))
    out.append("BEGIN")
    for c in d["controls"]:
        if c["extra"]:
            raise NoSourceForm("a control with creation data")
        cls = c["class"]
        if isinstance(cls, int):
            if cls not in R.CONTROL_CLASSES:
                raise NoSourceForm("control class atom 0x%x" % cls)
            cls = R.CONTROL_CLASSES[cls]
        elif cls.upper() in R.CONTROL_CLASSES.values():
            raise NoSourceForm("a standard control class spelled out")   # the compiler would make it an atom
        line = "  CONTROL %s, %s, %s, %s, %d, %d, %d, %d" % (
            ident(c["text"], wide), _number(c["id"]), quoted(cls, wide), _style(c["style"]), c["x"], c["y"], c["cx"], c["cy"])
        if c["exstyle"] or c["help"]:
            line += ", 0x%08XL" % c["exstyle"]
        if c["help"]:
            line += ", %d" % c["help"]
        out.append(line)
    out.append("END")
    return out


def menu_text(r, wide):
    m = R.menu(r.data, wide)
    if m["help"]:
        raise NoSourceForm("a menu with a help id")
    out = ["%s %s%s" % (ident(r.name, wide), "MENUEX" if m["extended"] else "MENU", _memory(r.flags, wide))]

    def level(items, depth):
        pad = "  " * depth
        out.append(pad + "BEGIN")
        for it in items:
            text = quoted(it["text"], wide)
            if m["extended"]:
                tail = ", %s, 0x%XL, 0x%XL" % (_number(it["id"]), it["type"], it["state"])
                if it["items"] is not None:
                    out.append("%s  POPUP %s%s, %d" % (pad, text, tail, it["help"]))
                    level(it["items"], depth + 1)
                else:
                    out.append("%s  MENUITEM %s%s" % (pad, text, tail))
                continue
            flags = it["flags"] & ~0x10
            if flags & ~sum(bit for bit, _n in _MENU_OPTIONS):
                raise NoSourceForm("menu item flags 0x%x" % flags)
            options = "".join(", " + name for bit, name in _MENU_OPTIONS if flags & bit)
            if it["items"] is not None:
                out.append("%s  POPUP %s%s" % (pad, text, options))
                level(it["items"], depth + 1)
            elif not it["text"] and not it["id"] and not flags:
                out.append(pad + "  MENUITEM SEPARATOR")
            elif not it["text"]:
                raise NoSourceForm("an item with no text that is not a separator")
            else:
                out.append("%s  MENUITEM %s, %d%s" % (pad, text, it["id"], options))
        out.append(pad + "END")
    level(m["items"], 0)
    return out


def accelerators_text(r, wide):
    out = ["%s ACCELERATORS%s" % (ident(r.name, wide), _memory(r.flags, wide)), "BEGIN"]
    for flags, key, ident_ in R.accelerators(r.data, wide):
        if flags & ~0x1F:
            raise NoSourceForm("accelerator flags 0x%x" % flags)
        out.append("  %d, %d, %s%s" % (key, ident_, "VIRTKEY" if flags & 1 else "ASCII",
                                      "".join(", " + n for bit, n in _KEY_OPTIONS if flags & bit)))
    out.append("END")
    return out


def version_text(r, wide):
    if not wide:
        # the compiler leaves the last string of a 16-bit block without its terminator and
        # padding while counting them in every enclosing length: what it writes cannot be read
        raise NoSourceForm("the compiler writes an unsound 16-bit version block")
    fixed, table, translations = R.version(r.data, wide)
    if not fixed or fixed[0] != 0xFEEF04BD or fixed[11] or fixed[12]:
        raise NoSourceForm("a version block without the usual fixed part")
    out = ["%s VERSIONINFO" % ident(r.name, wide)]
    for word, (ms, ls) in (("FILEVERSION", fixed[2:4]), ("PRODUCTVERSION", fixed[4:6])):
        out.append("%s %d, %d, %d, %d" % (word, ms >> 16, ms & 0xFFFF, ls >> 16, ls & 0xFFFF))
    for word, value in zip(("FILEFLAGSMASK", "FILEFLAGS", "FILEOS", "FILETYPE", "FILESUBTYPE"), fixed[6:11]):
        out.append("%s 0x%XL" % (word, value))
    out.append("BEGIN")
    if table:
        out += ['  BLOCK "StringFileInfo"', "  BEGIN"]
        for lang, entries in table.items():
            out += ["    BLOCK %s" % quoted(lang, wide), "    BEGIN"]
            out += ["      VALUE %s, %s" % (quoted(k, wide), quoted(v, wide)) for k, v in entries]
            out.append("    END")
        out.append("  END")
    if translations:
        out += ['  BLOCK "VarFileInfo"', "  BEGIN",
                '    VALUE "Translation", %s' % ", ".join("0x%X" % w for w in translations), "  END"]
    out.append("END")
    return out


_TEXT = {R.DIALOG: dialog_text, R.MENU: menu_text, R.ACCELERATOR: accelerators_text, R.VERSION: version_text}
_DECODE_ERRORS = (NoSourceForm, ValueError, IndexError, struct.error, UnicodeError)


# ---------------------------------------------------------------------------------------------
# writing the .rc
# ---------------------------------------------------------------------------------------------
def _members(found, wide):
    """{(member type, id): [resource]} and, per group that can be put back together as a file,
    the members it takes: {group key: [member resource]}."""
    by_id = {}
    for r in found:
        if r.type in (R.ICON, R.CURSOR):
            by_id.setdefault((r.type, r.name), []).append(r)
    taken = {}
    for g in found:
        if g.type not in R.MEMBER_OF:
            continue
        try:
            _kind, entries = R.group(g.data)
        except (ValueError, struct.error):
            continue
        mine = []
        for e in entries:
            cands = by_id.get((R.MEMBER_OF[g.type], e[6]), [])
            pick = next((m for m in cands if m.language == g.language), cands[0] if cands else None)
            if pick is None:
                break
            mine.append(pick)
        else:
            taken[g.key] = mine
    return taken


def write_rc(module_path, outdir, verbatim=()):
    """Write `<module>.rc` and its payload files under `outdir`. Resources whose key is in
    `verbatim` are written as payloads of their original bytes whatever their kind.
    -> (rc path, [(first line, last line, resource key)], {how written: [resource key]}, how text is written)"""
    wide = WIDE if corpus.lane(module_path) == "pe" else 0
    try:
        return _write_rc(module_path, outdir, set(verbatim), wide)
    except BeyondLatin1:
        return _write_rc(module_path, outdir, set(verbatim), UNICODE)


def _write_rc(module_path, outdir, verbatim, wide):
    found = R.read(module_path)
    stem = os.path.splitext(os.path.basename(module_path))[0].lower()
    files = os.path.join(outdir, "res")
    os.makedirs(files, exist_ok=True)
    groups = _members(found, wide)
    if any(k in verbatim for k in groups):                   # member numbering is the compiler's: all or none
        verbatim |= set(groups)
    groups = {k: v for k, v in groups.items() if k not in verbatim}
    in_group = {id(m) for members in groups.values() for m in members}
    lines = ["/* %s -- the resources of %s. Generated; see codebreaker/build/resources.py. */" % (
        stem + ".rc", os.path.basename(module_path).lower())]
    spans, how, language = [], {"text": [], "files": [], "opaque": [], "fallback": []}, [None]

    def emit(keys, text, kind, lang):
        if wide and lang != language[0]:
            lines.append("LANGUAGE 0x%02X, 0x%02X" % (lang & 0x3FF, lang >> 10))
            language[0] = lang
        first = len(lines) + 1
        lines.extend(text)
        for k in keys:
            spans.append((first, len(lines), k))
            how[kind].append(k)

    def payload(r, ext, data):
        name = _stem(r) + ext
        n = 1
        while os.path.exists(os.path.join(files, name)) and open(os.path.join(files, name), "rb").read() != data:
            n += 1
            name = "%s_%d%s" % (_stem(r), n, ext)
        with open(os.path.join(files, name), "wb") as f:
            f.write(data)
        return '"res/%s"' % name

    def raw(r, kind):
        library = R.type_library(r.data)
        name = payload(r, ".tlb" if library else ".bin", r.data)
        if library:                                          # beside the payload, an outline to read
            with open(os.path.join(outdir, name.strip('"')[:-4] + ".idl"), "w", newline="\n") as f:
                f.write(R.type_library_outline(library))
        emit([r.key], ["%s %s%s %s" % (ident(r.name, wide), ident(r.type, wide), _memory(r.flags, wide), name)],
             kind, r.language)

    tables = {}                                              # (language, flags) -> [(id, text)]
    for r in found:
        if id(r) in in_group:
            continue
        if r.key in verbatim:
            raw(r, "fallback")
        elif r.type == R.STRING and isinstance(r.name, int):
            try:
                texts = [(16 * (r.name - 1) + i, quoted(t, wide, True))
                         for i, t in enumerate(R.strings(r.data, wide)) if t]
            except _DECODE_ERRORS:
                raw(r, "fallback")
                continue
            tables.setdefault((r.language, r.flags), []).append((r.key, texts))
        elif r.type in _TEXT:
            try:
                emit([r.key], _TEXT[r.type](r, wide), "text", r.language)
            except _DECODE_ERRORS:
                raw(r, "fallback")
        elif r.key in groups:
            members = {m.name: m.data for m in groups[r.key]}
            word, ext = ("ICON", ".ico") if r.type == R.GROUP_ICON else ("CURSOR", ".cur")
            try:
                image = R.icon_file(r.data, members.__getitem__)
            except _DECODE_ERRORS:
                raw(r, "fallback")
                for m in groups[r.key]:
                    raw(m, "fallback")
                continue
            emit([r.key] + [m.key for m in groups[r.key]],
                 ["%s %s%s %s" % (ident(r.name, wide), word, _memory(r.flags, wide), payload(r, ext, image))],
                 "files", r.language)
        elif r.type == R.BITMAP:
            emit([r.key], ["%s BITMAP%s %s" % (ident(r.name, wide), _memory(r.flags, wide),
                                              payload(r, ".bmp", R.bitmap_file(r.data)))], "files", r.language)
        else:
            raw(r, "fallback" if r.type in (R.STRING, R.GROUP_ICON, R.GROUP_CURSOR) else "opaque")
    for (lang, flags), blocks in tables.items():
        text = ["STRINGTABLE%s" % _memory(flags, wide), "BEGIN"]
        text += ["  %d, %s" % pair for _key, texts in blocks for pair in texts]
        emit([key for key, _t in blocks], text + ["END"], "text", lang)
    rc = os.path.join(outdir, stem + ".rc")
    with open(rc, "w", newline="\n", encoding="utf-8" if wide == UNICODE else "ascii") as f:
        f.write("\n".join(lines) + "\n")
    return rc, spans, how, wide


# ---------------------------------------------------------------------------------------------
# compiling, and reading what the compiler wrote
# ---------------------------------------------------------------------------------------------
def compile_rc(rc, wide, microsoft=True):
    """Run the resource compiler. -> (.res path or None, its messages). `microsoft` chooses the
    layout of the .res: the Microsoft one, which read_res reads, or the compiler's own
    (`<name>_link.res`), which is the one to hand the linker -- from a Microsoft-layout file
    the linker binds every resource with language 0."""
    wrc = watcom.ow_tool("wrc")
    if not wrc:
        raise RuntimeError("OpenWatcom's resource compiler is not set up (settings.json: openwatcom)")
    res = os.path.splitext(rc)[0] + (".res" if microsoft else "_link.res")
    try:
        os.remove(res)
    except OSError:
        pass
    # -r: only the .res; -zm: the Microsoft layout; -zku8: the source is UTF-8 and becomes UTF-16.
    # (The preprocessor is left on: without it a comment is a syntax error.)
    args = [wrc, "-q", "-r", "-bt=" + ("nt" if wide else "windows")] + (["-zm"] if microsoft else []) + (
        ["-zku8"] if wide == UNICODE else [])
    p = subprocess.run(args + ["-fo=" + os.path.basename(res), os.path.basename(rc)], cwd=os.path.dirname(rc),
                       env=watcom.ow_env("pe" if wide else "ne"), capture_output=True, text=True, errors="replace",
                       timeout=600)
    log = (p.stdout or "") + (p.stderr or "")
    return (res if p.returncode == 0 and os.path.isfile(res) else None), log


def read_res(path, wide):
    """The resources in a compiled `.res` file (Microsoft layout)."""
    with open(path, "rb") as f:
        d = f.read()
    out, p = [], 0

    def name16(p):
        if d[p] == 0xFF:
            return struct.unpack_from("<H", d, p + 1)[0], p + 3
        e = d.index(b"\0", p)
        return d[p:e].decode("latin-1"), e + 1

    def name32(p):
        if struct.unpack_from("<H", d, p)[0] == 0xFFFF:
            return struct.unpack_from("<H", d, p + 2)[0], p + 4
        e = p
        while d[e:e + 2] != b"\0\0":
            if e > len(d):
                raise IndexError(e)
            e += 2
        return d[p:e].decode("utf-16-le", "replace"), e + 2

    def entry_at(p):
        if p + 32 > len(d):
            return False
        size, header = struct.unpack_from("<II", d, p)
        if not 24 <= header <= 4096 or p + header + size > len(d):
            return False
        try:                                                 # the header is as long as its names make it
            _t, q = name32(p + 8)
            _n, q = name32(q)
        except (struct.error, IndexError):
            return False
        return ((q - p + 3) & ~3) + 16 == header

    while p + (16 if wide else 8) <= len(d):
        if wide:
            # The Microsoft compiler pads every body to four bytes, so every entry starts on a
            # four-byte boundary. This compiler leaves some bodies unpadded: an entry can start
            # anywhere, the fixed part of its header is then moved up to the next boundary of the
            # file, and the next entry is looked for in the bytes after the body.
            p = next((p + k for k in range(16) if entry_at(p + k)), None)
            if p is None:
                break
            size, header = struct.unpack_from("<II", d, p)
            rtype, q = name32(p + 8)
            name, q = name32(q)
            q = (q + 3) & ~3                                 # padding the header's length does not count
            _dv, flags, lang = struct.unpack_from("<IHH", d, q)
            body = q + 16
            data = d[body:body + size]
            p = body + size
            if rtype == 0 and name == 0 and not size:
                continue                                     # the marker a 32-bit .res starts with
        else:
            while p < len(d) and d[p] == 0:                  # padding between entries
                p += 1
            if p + 8 > len(d):
                break
            rtype, q = name16(p)
            name, q = name16(q)
            flags, size = struct.unpack_from("<HI", d, q)
            lang, data = None, d[q + 6:q + 6 + size]
            p = q + 6 + size
        out.append(R.Resource(rtype, name, lang, flags, data))
    return out


def compare(original, rebuilt, wide, result=None):
    """Compare two sets of resources. An icon or cursor group is compared with its member images
    in order, since the compiler numbers members itself. -> Result (counts and the keys that differ)."""
    result = result or Result()

    def fold(key):
        return tuple(k.upper() if isinstance(k, str) else k for k in key)

    def images(found):
        groups = _members(found, wide)
        by_key = {fold(r.key): r for r in found}
        out, used = {}, set()
        for key, members in groups.items():
            kind, _entries = R.group(by_key[fold(key)].data)
            out[fold(key)] = (kind, [bytes(m.data) for m in members])
            used |= {id(m) for m in members}
        return out, used

    old_groups, old_used = images(original)
    new_groups, new_used = images(rebuilt)
    new = {}
    for r in rebuilt:
        if id(r) not in new_used:
            new.setdefault(fold(r.key), []).append(r)
    seen = set()
    for r in original:
        if id(r) in old_used:
            continue
        result.total += 1
        key = fold(r.key)
        seen.add(key)
        twin = new.get(key)
        if not twin:
            result.missing.append(r.key)
        elif len(twin) > 1:
            result.different.append(r.key)
        elif key in old_groups:
            same = key in new_groups and old_groups[key][0] == new_groups[key][0] and \
                len(old_groups[key][1]) == len(new_groups[key][1]) and \
                all(R.same_data(a, b) for a, b in zip(old_groups[key][1], new_groups[key][1]))
            if same and r.data == twin[0].data:
                result.identical += 1
            elif same:
                result.equivalent += 1
            else:
                result.different.append(r.key)
        elif r.data == twin[0].data or (not wide and R.same_data(r.data, twin[0].data)):
            result.identical += 1
        else:
            a, b = R.decode(r, wide), R.decode(twin[0], wide)
            if a is not None and a == b:
                result.equivalent += 1
            else:
                result.different.append(r.key)
    result.extra = [r.key for k, rs in new.items() if k not in seen for r in rs]
    return result


def rebuild(module_path, outdir, rounds=12):
    """Write a module's resources as source, compile them, and compare with the original. A
    resource that does not survive is carried as a payload of its original bytes and the build
    repeated, so the `.res` this returns is complete whenever the compiler accepts it at all."""
    original = R.read(module_path)
    verbatim, result = set(), Result()
    if not original:
        result.error = "the module has no resources"
        return result
    for _ in range(rounds):
        result = Result()
        result.rc, spans, how, wide = write_rc(module_path, outdir, verbatim)
        res, log = compile_rc(result.rc, wide)
        if not res:
            m = _ERROR_LINE.search(log)
            line = int(m.group(1)) if m else -1
            bad = {k for first, last, k in spans if first <= line <= last} - verbatim
            if not bad:
                result.error = "the resource compiler rejected the file: %s" % (log.strip()[-200:] or "no message")
                return result
            verbatim |= bad
            continue
        result.res = res
        compare(original, read_res(res, wide), wide, result)
        result.as_text, result.as_files = len(how["text"]), len(how["files"])
        result.opaque, result.fallback, result.not_source = len(how["opaque"]), len(how["fallback"]), how["fallback"]
        again = set(result.different) - verbatim
        if not again:
            result.link_res = compile_rc(result.rc, wide, microsoft=False)[0]
            break
        verbatim |= again
    return result


def main(argv):
    """python -m codebreaker resources <module> [--out DIR]

    Without --out the resources go into the module's project folder: Converted/<name>/assets/resources/."""
    out = argv[argv.index("--out") + 1] if "--out" in argv else None
    names = [a for a in argv if not a.startswith("--") and a != out]
    if not names:
        print(__doc__)
        return 2
    bad = 0
    for name in names:
        path = corpus.retail(name)
        if not path:
            print("%s: no such module in the corpus" % name)
            bad += 1
            continue
        proj = None
        where = out
        if not where:
            from codebreaker.base import converted
            proj = converted.project(path)
            where = os.path.join(proj.assets, "resources")
        r = rebuild(path, where)
        if proj:
            proj.record("resources", [where], notes="its resources as files and a .rc script: %s" % r.line())
        print("%s: %s" % (os.path.basename(path).lower(), r.line()))
        for key in r.different + r.missing:
            print("    not reproduced: %s %r" % (R.type_name(key[0]), key[1]))
        if r.rc:
            print("    %s" % r.rc)
        bad += 0 if r.same else 1
    return 1 if bad else 0
