"""vxdfile.py -- a built VxD's LE laid out in a file the way the original's is.

    from codebreaker.build import vxdfile
    text = vxdfile.definitions(original, lfa)         the .def its names came from (DESCRIPTION, EXPORTS)
    le = vxdfile.relay(built, at, resource)

OpenWatcom's linker writes an LE with its own layout: its pages straight behind the tables. The
originals were linked by Microsoft's LINK: the pages start on a 512-byte
boundary of the file, the non-resident name table behind them holds the description and the
exports named in the .def (DESCRIPTION, EXPORTS), and a Win95 VxD carries its version resource
behind that, pointed at by the header (at B8h: where in the file, at BCh: how long). The
description and exports come from the driver's .def, which the linker is given (build/driver.py
link), so the name table is the linker's. relay()
takes the built LE -- header, loader and fix-up sections, pages: all as linked -- and puts it
at `at` in such a file: every offset the LE gives from the start of the FILE (the pages, the
non-resident names, the resource) is set for where it now is.
"""
import struct

PAGE_ALIGN = 0x200


def _u(d, at):
    return struct.unpack_from("<I", d, at)[0]


def definitions(original, lfa):
    """The .def text the original's non-resident name table was made from. -> str"""
    at, n = _u(original, lfa + 0x88), _u(original, lfa + 0x8C)
    table, names, i = original[at:at + n], [], 0
    while i < len(table) and table[i]:
        k = table[i]
        names.append((table[i + 1:i + 1 + k].decode("latin1"), struct.unpack_from("<H", table, i + 1 + k)[0]))
        i += 3 + k
    lines = []
    for name, ordinal in names:
        if ordinal == 0:
            lines.append("DESCRIPTION '%s'" % name.replace("'", "''"))
    exports = [(n, o) for n, o in names if o]
    if exports:
        lines.append("EXPORTS")
        lines += ["    %s @%d" % e for e in exports]
    return "\n".join(lines) + "\n"


def relay(built, at, resource=b""):
    """The LE of `built` (a whole VxD file) laid out to start at file offset `at` (a multiple of
    16) as Microsoft's LINK lays it out: tables, pages from the next 512-byte boundary, the
    non-resident name table as linked, then `resource`. -> the bytes from `at` on."""
    lfa = _u(built, 0x3C)
    pages_from = _u(built, lfa + 0x80)                       # file offsets in the built file
    nonres_at, nonres_len = _u(built, lfa + 0x88), _u(built, lfa + 0x8C)
    head = bytearray(built[lfa:pages_from])
    pages = built[pages_from:nonres_at] if nonres_at else built[pages_from:]
    names = built[nonres_at:nonres_at + nonres_len] if nonres_at else b""
    start = (at + len(head) + PAGE_ALIGN - 1) // PAGE_ALIGN * PAGE_ALIGN
    head += bytes(start - at - len(head))
    struct.pack_into("<I", head, 0x80, start)
    if names:
        struct.pack_into("<I", head, 0x88, start + len(pages))
    if resource:
        struct.pack_into("<II", head, 0xB8, start + len(pages) + len(names), len(resource))
    return bytes(head) + pages + names + resource
