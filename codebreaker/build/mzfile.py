"""mzfile.py -- an MZ program's file written the way the original's is laid out.

    from codebreaker.build import mzfile
    tpl = mzfile.template(original_header)        the header's own data, build outputs blanked
    data = mzfile.compose(tpl, module, relocs, cs, ip, ss, sp, min_alloc)

A linker writes a header of its own: the fields, the relocation table, padding. Many of the
originals carry more in theirs -- MS-DOS 7's utilities keep their messages ("Incorrect MS-DOS
version", "Extended Error %1"), their version block (VS_FIXEDFILEINFO) and their parse tables
in the header, behind the relocation table. That data is part of the program as written, so it
is kept as a file beside the source (<stem>.hdr): the original header with what the BUILD
decides blanked -- the sizes, the relocation count and table, the minimum memory, the stack and
the entry point -- and the rest kept: the header's size, the maximum memory and checksum words
(linker options of the original's build), where the relocation table goes, and the data.
compose() fills the blanks from what was built.
"""
import struct

_BUILT = ((0x02, 2), (0x04, 2), (0x06, 2), (0x0A, 2), (0x0E, 2), (0x10, 2), (0x14, 2), (0x16, 2))


def template(header):
    """The original header with the build's outputs blanked. -> bytes"""
    t = bytearray(header)
    nreloc, reloc_off = struct.unpack_from("<H", t, 6)[0], struct.unpack_from("<H", t, 0x18)[0]
    for at, n in _BUILT:
        t[at:at + n] = bytes(n)
    t[reloc_off:reloc_off + 4 * nreloc] = bytes(4 * nreloc)
    return bytes(t)


def compose(tpl, module, relocs, cs, ip, ss, sp, min_alloc, full_last=False):
    """The file: the template's header with the blanks filled -- `relocs` [(offset, segment)]
    written at the template's relocation table place -- then the load module.
    ValueError when the relocation table does not fit where the template has room for it."""
    head = bytearray(tpl)
    reloc_off = struct.unpack_from("<H", head, 0x18)[0]
    room = head[reloc_off:reloc_off + 4 * len(relocs)]
    if reloc_off + 4 * len(relocs) > len(head) or any(room):
        raise ValueError("%d relocations do not fit in the header's room for them" % len(relocs))
    for i, (off, seg) in enumerate(relocs):
        struct.pack_into("<HH", head, reloc_off + 4 * i, off, seg)
    total = len(head) + len(module)
    # (a last page that is full: 0, or 512 as some linkers wrote it -- `full_last`)
    last = total % 512 or (512 if full_last else 0)
    struct.pack_into("<HHH", head, 2, last, (total + 511) // 512, len(relocs))
    struct.pack_into("<H", head, 0x0A, min_alloc)
    struct.pack_into("<HH", head, 0x0E, ss, sp)
    struct.pack_into("<HH", head, 0x14, ip, cs)
    return bytes(head) + bytes(module)
