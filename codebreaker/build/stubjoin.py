"""stubjoin.py -- a Windows module put behind a DOS program of its own, as one file.

    from codebreaker.build import stubjoin
    data = stubjoin.join(stub, built, at)

Every Windows module starts with a DOS program, its stub; usually the linker's own two-line
"cannot be run in DOS mode". Some carry a real one -- REGEDIT's is a registry editor for DOS,
XCOPY32's and UNINSTAL's do their work from DOS too -- and those are built as programs of their
own (source/dos_writer.py). join() takes the module as OpenWatcom linked it, with its own stub,
and puts it behind that program: its new header at `at` (the original's place, when the program
is no longer than that), and every offset the module gives from the start of the FILE moved
with it.

  PE   the headers (signature, file header, optional header, section table) at `at`, the
       section data from the next file-alignment boundary; moved: each section's raw data,
       relocations and line numbers, the symbol table, the debug data, the certificates;
       SizeOfHeaders
  NE   the header and its tables at `at`, the segments and resources from the next boundary of
       the module's alignment unit; moved: each segment's and resource's sector, the
       non-resident name table
"""
import struct


def _u(d, at, f="<I"):
    return struct.unpack_from(f, d, at)[0]


def _align(n, a):
    return (n + a - 1) // a * a


def join(stub, built, at=None):
    """`stub`: the DOS program (an MZ file, its new-header field at 3Ch to be set); `built`: the
    Windows module as linked. -> the file."""
    lfa = _u(built, 0x3C)
    at = at if at and at >= len(stub) else _align(len(stub), 16)
    if built[lfa:lfa + 2] == b"PE":
        tail = _pe(built, lfa, at)
    elif built[lfa:lfa + 2] == b"NE":
        tail = _ne(built, lfa, at)
    else:
        raise ValueError("not a PE or NE module")
    head = bytearray(stub) + bytes(at - len(stub))
    struct.pack_into("<I", head, 0x3C, at)
    return bytes(head) + tail


def _pe(b, lfa, at):
    opt = lfa + 24
    nsec, optsize = _u(b, lfa + 6, "<H"), _u(b, lfa + 20, "<H")
    falign, old_soh = _u(b, opt + 36), _u(b, opt + 60)
    plus = _u(b, opt, "<H") == 0x20B
    dirs = opt + (112 if plus else 96)
    sec = opt + optsize
    headers = bytearray(b[lfa:sec + 40 * nsec])
    new_soh = _align(at + len(headers), falign)
    delta = new_soh - old_soh

    def move(off):                                         # a field of the headers holding a file offset
        v = _u(headers, off - lfa)
        if v:
            struct.pack_into("<I", headers, off - lfa, v + delta)
    struct.pack_into("<I", headers, opt + 60 - lfa, new_soh)
    move(lfa + 12)                                         # PointerToSymbolTable
    body = bytearray(b[old_soh:])
    sections = []
    for i in range(nsec):
        s = sec + 40 * i
        sections.append((_u(b, s + 12), _u(b, s + 16), _u(b, s + 20)))      # rva, raw size, raw offset
        for f in (20, 24, 28):                              # raw data, relocations, line numbers
            move(s + f)
    # the certificates (data directory 4) are a file offset; the debug directory's entries hold one
    if _u(b, dirs + 4 * 8):
        move(dirs + 4 * 8)
    drva, dsize = _u(b, dirs + 6 * 8), _u(b, dirs + 6 * 8 + 4)
    for rva, rsize, raw in sections:
        if drva and rva <= drva < rva + rsize:
            at_debug = raw + drva - rva - old_soh
            for k in range(dsize // 28):
                e = at_debug + 28 * k + 24
                if _u(body, e):
                    struct.pack_into("<I", body, e, _u(body, e) + delta)
    return bytes(headers) + bytes(new_soh - at - len(headers)) + bytes(body)


def _ne(b, lfa, at):
    shift = _u(b, lfa + 0x32, "<H")
    unit = 1 << shift
    nseg, segtab, restab = _u(b, lfa + 0x1C, "<H"), _u(b, lfa + 0x22, "<H"), _u(b, lfa + 0x24, "<H")
    resname = _u(b, lfa + 0x26, "<H")
    # where the data starts: the first segment or resource (sector offsets from the file start)
    sectors = [_u(b, lfa + segtab + 8 * i, "<H") for i in range(nseg)]
    res = []                                               # offsets (in the header) of resource sectors
    if restab != resname:
        r = lfa + restab
        rshift = _u(b, r, "<H")
        r += 2
        while _u(b, r, "<H"):
            count = _u(b, r + 2, "<H")
            for k in range(count):
                res.append(r + 8 + 12 * k)
            r += 8 + 12 * count
        if rshift != shift:
            raise ValueError("resources aligned differently from segments")
    firsts = [s for s in sectors if s] + [_u(b, o, "<H") for o in res if _u(b, o, "<H")]
    data_from = min(firsts) * unit if firsts else len(b)
    nonres = _u(b, lfa + 0x2C)
    header = bytearray(b[lfa:data_from])                   # the header, its tables -- and the
    new_data = _align(at + len(header), unit)              # non-resident names when before the data
    delta = (new_data - data_from) // unit
    for i, s in enumerate(sectors):
        if s:
            struct.pack_into("<H", header, segtab + 8 * i, s + delta)
    for o in res:
        v = _u(b, o, "<H")
        if v:
            struct.pack_into("<H", header, o - lfa, v + delta)
    if nonres:
        moved = at - lfa if nonres < data_from else delta * unit
        struct.pack_into("<I", header, 0x2C, nonres + moved)
    return bytes(header) + bytes(new_data - at - len(header)) + bytes(b[data_from:])
