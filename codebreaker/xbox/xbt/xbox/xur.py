"""XUR (compiled XUI) reader — Xbox 360 dashboard / game UI scenes.

XUR is the binary form of an XUI scene (the dashboard/game UI markup). This
parses the XUR5 container — the big-endian header and section table — and pulls
the human-readable content out of the string-table (STRN) section: element
names, texture paths, and on-screen text a scene references.

Layout (clean-room from the documented format / XUIHelper by SGCSam):
  header (20 bytes, BE):  magic 'XUIB', version(=5), flags, toolVersion(u16),
                          fileSize, sectionCount(u16)
  section table:          sectionCount x { magic(u32), offset(u32), length(u32) }
  section data:           based at 20 + sectionCount*12
  STRN section:           repeated { u16 length, UTF-16BE chars[length] }

This reads the container and its strings; it does not (yet) rebuild the full
object tree back into XUI XML — that needs the per-control property schema.

Commands:
    xur info    <file.xur>      header + section table (magics, offsets, sizes)
    xur strings <file.xur>      the STRN string table (names / paths / text)
"""

import os
import struct
import sys

MAGIC = 0x58554942                                       # 'XUIB'
STRN = 0x5354524E                                        # 'STRN'


def _tag(v):
    return struct.pack(">I", v).decode("latin-1")


class XUR:
    def __init__(self, data):
        self.d = data
        magic, self.version, self.flags = struct.unpack_from(">III", data, 0)
        if magic != MAGIC:
            raise ValueError("not a XUR (magic %r)" % _tag(magic))
        self.tool_version, self.file_size, self.section_count = \
            struct.unpack_from(">HIH", data, 12)
        self.sections = []                               # (magic, offset, length)
        for i in range(self.section_count):
            o = 20 + i * 12
            m, off, length = struct.unpack_from(">III", data, o)
            self.sections.append((m, off, length))
        self.data_base = 20 + self.section_count * 12

    def section_data(self, magic):
        for m, off, length in self.sections:
            if m == magic:
                s = self.data_base + off
                return self.d[s:s + length]
        return None

    def strings(self):
        blob = self.section_data(STRN)
        if blob is None:
            return []
        out = []
        o = 0
        while o + 2 <= len(blob):
            n = struct.unpack_from(">H", blob, o)[0]
            o += 2
            s = blob[o:o + n * 2].decode("utf-16-be", "replace")
            out.append(s)
            o += n * 2
        return out


def build_strn(strings):
    """Serialise a STRN section body (for tests / round-trips)."""
    body = b""
    for s in strings:
        e = s.encode("utf-16-be")
        body += struct.pack(">H", len(s)) + e
    return body


def set_strings(data, new_strings):
    """Re-emit an XUR with its STRN string table replaced. Rebuilds only the
    STRN body and shifts every later section by the size delta (preserving the
    original layout of all other sections), fixing the section table offsets/
    lengths and the header file size. Byte-identical to the input when the
    strings are unchanged."""
    x = XUR(data)
    strn = next(((i, off, length) for i, (m, off, length) in enumerate(x.sections)
                 if m == STRN), None)
    if strn is None:
        raise ValueError("no STRN section to edit")
    idx, soff, slen = strn
    body = build_strn(new_strings)
    delta = len(body) - slen
    db = x.data_base
    area = data[db:]
    new_area = area[:soff] + body + area[soff + slen:]
    out = bytearray(data[:db])                          # header + section table
    struct.pack_into(">I", out, 14, x.file_size + delta)    # file size
    for i, (m, off, length) in enumerate(x.sections):
        eo = 20 + i * 12
        if i == idx:
            struct.pack_into(">I", out, eo + 8, len(body))     # STRN new length
        elif off > soff:
            struct.pack_into(">I", out, eo + 4, off + delta)   # shift later section
    return bytes(out) + new_area


def set_string(data, index, text):
    """Replace a single string (by index) and re-emit the XUR."""
    strs = XUR(data).strings()
    if not (0 <= index < len(strs)):
        raise IndexError("string index %d out of range (0..%d)" % (index, len(strs) - 1))
    strs[index] = text
    return set_strings(data, strs)


DESCRIPTION = "read/edit Xbox 360 XUR (compiled XUI) scenes: info/strings + set (repack)"


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n  xur {info|strings} <file.xur>\n"
              "  xur set <file.xur> <index> <text> <out.xur>   edit a UI string, repack")
        return
    if a[0] == "set":
        if len(a) < 5:
            print("usage: xur set <file.xur> <index> <text> <out.xur>"); return
        out = set_string(open(a[1], "rb").read(), int(a[2], 0), a[3])
        open(a[4], "wb").write(out)
        print("set string [%s] -> %s (%d bytes)" % (a[2], a[4], len(out)))
        return
    op = a[0] if a[0] in ("info", "strings") else "info"
    path = a[1] if a[0] in ("info", "strings") else a[0]
    x = XUR(open(path, "rb").read())
    if op == "info":
        print("magic       : XUIB  version %d  toolVersion 0x%X" % (x.version, x.tool_version))
        print("file size   : %d (actual %d)" % (x.file_size, len(x.d)))
        print("sections    : %d" % x.section_count)
        for m, off, length in x.sections:
            print("   %-6s offset=0x%-6X length=%d" % (_tag(m), off, length))
    else:
        strs = x.strings()
        for i, s in enumerate(strs):
            print("  [%3d] %s" % (i, s))
        print("total: %d strings" % len(strs))


if __name__ == "__main__":
    main()
