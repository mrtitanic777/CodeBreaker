"""dos.py -- the ONE reader for real-mode programs: what DOS, or the boot sector, would load.

    from codebreaker.formats import dos
    dos.kind(path)        "mz" | "com" | "driver" | "kernel" | None   (None: not a program -- e.g.
                          COUNTRY.SYS and the LOGO*.SYS pictures carry a program's extension only)
    dos.packer(path)      "EXEPACK" | "PKLITE" | "LZEXE" | "DIET" | "UPX" | "moves itself" | None
    p = dos.read(path)    a Program: the bytes as they sit in memory once loaded (and unpacked)
    dos.read(path, run_movers=False)   a program that moves itself read as it is stored: the
                          moving is its own code, which its source has to have
    dos.programs()        every real-mode program of the corpus, one path per name
    dos.unpacked_file(p)  a packed program written back as a plain MZ file (`info --unpack`)

Four kinds of file, one result:

  mz      a header, a relocation table, a load module, sometimes an overlay behind it. The load
          module goes to segment LOAD and every relocation (a word holding a segment) has LOAD
          added. A load module stored compressed is unpacked first, so the Program holds the
          program and not its unpacker: EXEPACK statically (a backwards run-length scheme with its
          own relocation table), anything else by running the unpacker (codebreaker.emulate.unpack).
          A program that is not compressed but starts by moving itself (COMMAND.COM) is run the
          same way, up to where it continues in the moved copy.
  com     no header: the file goes to LOAD:0100 behind a 256-byte program prefix, entry there.
  driver  a device driver: a chain of device headers (next, attributes, strategy routine,
          interrupt routine, name) at offset 0 -- of the file, or of an MZ load module.
  kernel  IO.SYS / WINBOOT.SYS, three programs in one file: the first sectors are the loader the
          boot sector starts at 0070:0200 (it moves itself to the top of memory); what follows is
          the BIOS and DOS kernel, which the loader reads to 0070:0000; and the MZ header describes
          only the last part, SYSINIT, with a "header size" that skips the other two.

A Program's `image` is one flat stretch of real-mode memory starting at linear address `base`
(linear = segment * 16 + offset); `spans` says which parts of it hold bytes from the file.
"""
import os
import struct

LOAD = 0x1000                       # the segment a program is loaded at
KERNEL_SEGMENT = 0x0070             # where the boot sector and the loader put IO.SYS
LOADER_SEGMENT = 0x9000             # where the loader moves itself with 640 KB of memory
SYSINIT_SEGMENT = 0x4000            # SYSINIT is placed by the kernel at run time; a free segment
_SIGNATURES = (b"NE", b"PE", b"LE", b"LX")
EXTENSIONS = (".exe", ".com", ".sys", ".dos", ".mod", ".new", ".vxd", ".386", ".bin", ".ovl")


class Device(object):
    """One device header of a driver."""
    def __init__(self, offset, nxt, attributes, strategy, interrupt, name):
        self.offset, self.next, self.attributes = offset, nxt, attributes
        self.strategy, self.interrupt, self.name = strategy, interrupt, name
        self.character = bool(attributes & 0x8000)

    def __repr__(self):
        return "Device(%r at %#x, strategy %#x, interrupt %#x)" % (self.name, self.offset, self.strategy, self.interrupt)


class Program(object):
    """A real-mode program as loaded. See the module docstring."""
    def __init__(self, path, kind):
        self.path, self.kind = path, kind
        self.name = os.path.basename(path).lower()
        self.packer = None              # how the load module was stored
        self.unpacked = None            # "static" | "emulated" when it was unpacked
        self.base, self.image = LOAD * 16, b""
        self.spans = []                 # [(linear address, length, label)]
        self.entries = []               # [(linear address, name, "far" | "near")]
        self.segments = {}              # entry address -> the code segment it runs with
        self.relocations = []           # linear addresses of words that hold a segment
        self.devices = []               # [Device], offsets relative to their segment
        self.device_segment = LOAD
        self.cs = self.ip = self.ss = self.sp = 0
        self.ds = LOAD                  # the data segment register while the program runs
        self.overlay = b""              # bytes of the file behind the load module
        self.notes = []

    def inside(self, linear):
        return any(lo <= linear < lo + n for lo, n, _label in self.spans)

    def word(self, linear):
        i = linear - self.base
        return struct.unpack_from("<H", self.image, i)[0] if 0 <= i <= len(self.image) - 2 else None


# what a file is -------------------------------------------------------------------------------
def _mz(data, stub=False):
    """The fields of an MZ header, or None. `size` is the end of the load module in the file.
    A Windows module's DOS stub is None unless `stub`."""
    if len(data) < 0x1C or data[:2] not in (b"MZ", b"ZM"):
        return None
    (last, pages, nreloc, hdr, mn, mx, ss, sp, _ck, ip, cs, reloc_off) = struct.unpack_from("<12H", data, 2)
    size = pages * 512 - ((512 - last) if last else 0)
    h = dict(nreloc=nreloc, header=hdr * 16, min_alloc=mn, max_alloc=mx, ss=ss, sp=sp, ip=ip, cs=cs,
             reloc_off=reloc_off, size=min(size, len(data)) if size > hdr * 16 else len(data))
    if len(data) >= 0x40:
        new = struct.unpack_from("<I", data, 0x3C)[0]
        if 0x40 <= new < len(data) - 1 and data[new:new + 2] in _SIGNATURES and not stub:
            return None                                       # a Windows module with a DOS stub
    return h


def devices(image, at=0, segments=(0, 0xFFFF), limit=64):
    """The device headers chained from offset `at` of one segment's bytes. [] when what is there
    is not a device header: the routines must lie inside the image, a character device must
    have a printable name, and the chain must end in a link of offset FFFF."""
    out, seen = [], set()
    while len(out) < limit and at not in seen and 0 <= at <= len(image) - 18:
        seen.add(at)
        nxt, seg, attr, strategy, interrupt = struct.unpack_from("<5H", image, at)
        raw = image[at + 10:at + 18]
        if not (0 < strategy < len(image) and 0 < interrupt < len(image)) or attr & 0x0780 == 0x0780:
            return []
        if attr & 0x8000:
            if not all(0x20 <= c < 0x7F for c in raw) or raw[:1] == b" " and raw.strip():
                return []
            name = raw.decode("ascii").rstrip()
        else:
            name = "block device (%d units)" % raw[0]
        out.append(Device(at, None if nxt == 0xFFFF else nxt, attr, strategy, interrupt, name))
        if nxt == 0xFFFF:
            return out
        if seg not in segments:
            return []
        at = nxt
    return out if out and out[-1].next is None else []


def _kernel_body(data, h):
    """Where the BIOS/DOS part of an IO.SYS starts in the file: a sector boundary holding a jump
    followed at once by the chain of built-in devices (CON, AUX, PRN, ...) whose links name
    KERNEL_SEGMENT. None when the file is not laid out that way."""
    if h["header"] <= h["size"] // 2 or h["header"] >= h["size"]:
        return None
    for start in range(0x200, min(h["header"], 0x4000), 0x200):
        body = data[start:h["header"]]
        if body[:1] not in (b"\xE9", b"\xEB"):
            continue
        for at in range(2, 0x40):
            if len(devices(body[:0x10000], at, (KERNEL_SEGMENT,))) >= 3:
                return start, at
    return None


def _kind(data, name=""):
    h = _mz(data)
    if h:
        if _kernel_body(data, h):
            return "kernel"
        wrapped = (h["cs"], h["ip"]) == (0, 0) and devices(data[h["header"]:h["size"]][:0x10000])
        return "driver" if wrapped else "mz"                  # a driver linked as an .EXE
    if data[:2] in (b"MZ", b"ZM"):
        if driver_and_vxd(data):
            return "driver+vxd"
        return "dos+windows" if dos_and_windows(data) else None
    if devices(data[:0x10000]):
        return "driver"
    if name.lower().endswith(".com") and 0 < len(data) <= 0xFF00:
        return "com"
    return None


def chain(data):
    """Programs stored one after another in one file, each with its own MZ header: [(offset,
    bytes)], or [] for one alone. CodeView 4.1's CV.EXE is three -- a stub that checks the CPU and
    memory, the DOSX16 extender, and CodeView itself (448 KB, run in protected mode) -- and only
    the stub was taken apart: 7 functions of a debugger. (A program's overlays, stored after it the
    same way, come out as parts too.) Each part starts at the next byte, or after up to 15 bytes
    of zeros to a paragraph."""
    parts, at = [], 0
    while at < len(data):
        h = _mz(data[at:])
        if not h or h["size"] <= h["header"] or at + h["size"] > len(data):
            break
        parts.append((at, h["size"]))
        nxt = at + h["size"]
        pad = len(data[nxt:nxt + 16]) - len(data[nxt:nxt + 16].lstrip(b"\0"))
        if pad > 15 or data[nxt + pad:nxt + pad + 2] != b"MZ":
            break
        at = nxt + pad
    return parts if len(parts) > 1 else []


def driver_and_vxd(data):
    """A real-mode device driver and a VxD in one file (DRVSPACE.BIN): an MZ header whose load
    module is the driver -- device headers at its start -- and whose new-header field points at
    an LE behind it, the VxD. -> (header size, LE offset), or None."""
    if len(data) < 0x40 or data[:2] != b"MZ":
        return None
    hdr = struct.unpack_from("<H", data, 8)[0] * 16
    lfa = struct.unpack_from("<I", data, 0x3C)[0]
    if not (hdr < lfa < len(data) - 2 and data[lfa:lfa + 2] == b"LE" and devices(data[hdr:lfa][:0x10000])):
        return None
    return hdr, lfa


STUB_PROGRAM = 0x400        # a stub this big is a program of its own, not "cannot be run in DOS mode"


def dos_and_windows(data):
    """A DOS program and a Windows module in one file (REGEDIT.EXE, XCOPY32.EXE, UNINSTAL.EXE):
    the module's DOS stub is itself a real program -- run from DOS, it does the work there.
    -> (MZ fields, offset of the module's new header), or None."""
    h = _mz(data, stub=True)
    if not h or len(data) < 0x40:
        return None
    lfa = struct.unpack_from("<I", data, 0x3C)[0]
    if not (h["size"] <= lfa < len(data) - 1 and data[lfa:lfa + 2] in (b"NE", b"PE")):
        return None
    return (h, lfa) if h["size"] - h["header"] >= STUB_PROGRAM else None


def driver_part(path):
    """The real-mode driver of a driver+vxd file, as a Program (a driver image)."""
    with open(path, "rb") as f:
        data = f.read()
    hdr, lfa = driver_and_vxd(data)
    p = Program(path, "driver")
    p.name = "the real-mode driver of %s" % os.path.basename(path).lower()
    p.image = bytes(data[hdr:lfa])
    p.spans = [(LOAD * 16, len(p.image), "driver")]
    p.devices = devices(p.image[:0x10000])
    p.cs = LOAD
    for d in p.devices:
        label = "".join(c if c.isalnum() else "_" for c in d.name.split(" (")[0]).strip("_") or "device"
        label = "dev_" + label if label[0].isdigit() else label
        label = "%s_%X" % (label, d.offset) if not d.character else label
        for va, role in ((LOAD * 16 + d.strategy, "strategy"), (LOAD * 16 + d.interrupt, "interrupt")):
            if va not in p.segments:
                p.entries.append((va, "%s_%s" % (label, role), "far"))
                p.segments[va] = LOAD
    return p


def kind(path):
    """What kind of real-mode program a file is, or None. See the module docstring."""
    try:
        with open(path, "rb") as f:
            return _kind(f.read(), os.path.basename(path))
    except OSError:
        return None


def _packer(data, h):
    entry = h["header"] + h["cs"] * 16 + h["ip"]
    if 2 <= entry <= len(data) and data[entry - 2:entry] == b"RB" and h["ip"] in (0x10, 0x12, 0x14):
        return "EXEPACK"
    if data[0x1C:0x20] in (b"LZ09", b"LZ91"):
        return "LZEXE"
    head = data[0x1C:0x60]
    if b"PKLITE" in head or b"PKlite" in head:
        return "PKLITE"
    if data[0x1C:0x1F] == b"dlz" or b"dlz" in data[entry:entry + 0x40]:
        return "DIET"
    if b"UPX!" in data[:0x400]:
        return "UPX"
    start = bytes(data[entry:entry + 0x30])                   # a start that copies and far-returns
    end = start.find(bytes([0xCB]))
    if end > 0 and bytes([0xF3, 0xA4]) in bytes(data[h["header"]:entry + 0x30]) and not any(
            b in start[:end] for b in (0xE8, 0x9A, 0xCD, 0xE9, 0xEB)):
        return "moves itself"
    return None


def packer(path):
    """How an MZ program's load module is stored compressed, or None when it is not (or the
    scheme is not one this knows by its marks)."""
    with open(path, "rb") as f:
        data = f.read()
    h = _mz(data)
    return _packer(data, h) if h else None


def programs(extensions=EXTENSIONS):
    """One path per program name of the corpus that kind() accepts, by name."""
    from codebreaker.base import corpus
    return [cands[0] for name, cands in sorted(corpus.index().items())
            if name.endswith(tuple(extensions)) and kind(cands[0])]


# EXEPACK --------------------------------------------------------------------------------------
def unpack_exepack(module, cs, ip):
    """Undo EXEPACK on a load module. -> (load module, [relocation offsets], cs, ip, ss, sp)

    The packed module ends in a small segment (the entry's CS): a header of words -- the real
    IP, CS, (scratch), this segment's size, (in the 20-byte header one more word), the real SP,
    SS, the unpacked size in paragraphs, (in the 18- and 20-byte headers a padding count), and
    "RB" -- then the unpacker, its error message, and the relocation table. The data before it
    is read backwards from its end: skip 0xFF padding, then commands of
    a byte and a length word -- B0 fills `length` bytes with the byte before it, B2 copies
    `length` bytes -- written backwards from the unpacked size; bit 0 marks the last command.
    The relocation table is sixteen lists, one per 64 KB (segment 0x0000, 0x1000, ...), each a
    count and that many offsets."""
    at = cs * 16
    words = struct.unpack_from("<%dH" % (ip // 2), module, at)
    if words[-1] != 0x4252:
        raise ValueError("not EXEPACK: no RB before the entry point")
    real_ip, real_cs, _scratch, own_size = words[:4]
    real_sp, real_ss, out_paras, skip = words[4:7] + (1,) if ip == 0x10 else words[-5:-1]
    buf = bytearray(module[:at]) + bytearray(max(0, out_paras * 16 - at))
    src, dst = at - (skip - 1) * 16, out_paras * 16
    while src > 0 and buf[src - 1] == 0xFF:
        src -= 1
    while True:
        if src < 3:
            raise ValueError("EXEPACK data runs out")
        command, length = buf[src - 1], buf[src - 3] | buf[src - 2] << 8
        src -= 3
        if command & 0xFE == 0xB0:
            src -= 1
            dst -= length
            if dst < 0 or src < 0:
                raise ValueError("EXEPACK data runs out")
            buf[dst:dst + length] = bytes([buf[src]]) * length
        elif command & 0xFE == 0xB2:
            src -= length
            dst -= length
            if dst < 0 or src < 0:
                raise ValueError("EXEPACK data runs out")
            buf[dst:dst + length] = buf[src:src + length]
        else:
            raise ValueError("EXEPACK command %#x" % command)
        if command & 1:
            break
    relocs = []
    table = module.find(b"Packed file is corrupt", at, at + own_size)
    if table < 0:
        raise ValueError("EXEPACK relocation table not found")
    table += 22
    for seg in range(16):
        count = struct.unpack_from("<H", module, table)[0]
        table += 2
        for off in struct.unpack_from("<%dH" % count, module, table):
            relocs.append(seg * 0x10000 + off)
        table += 2 * count
    return bytes(buf[:out_paras * 16]), relocs, real_cs, real_ip, real_ss, real_sp


def unpacked_file(p):
    """A Program read from a packed MZ file, written back as a plain MZ file that DOS runs the
    same way: the unpacked load module with its relocation table (grouped per 64 KB, as EXEPACK
    kept it), the real entry and stack, room above the image for the stack, and whatever the file
    held behind its load module. -> bytes"""
    if p.kind != "mz" or not p.image:
        raise ValueError("%s: not an MZ program" % p.name)
    img, base, table = bytearray(p.image), LOAD * 16, []
    for r in p.relocations:
        at = r - base
        struct.pack_into("<H", img, at, (struct.unpack_from("<H", img, at)[0] - LOAD) & 0xFFFF)
        table.append(((at >> 16) << 12, at & 0xFFFF))
    cs, ss = (p.cs - LOAD) & 0xFFFF, (p.ss - LOAD) & 0xFFFF
    header = (0x1C + 4 * len(table) + 15) & ~15
    total = header + len(img)
    top = max(len(img), ss * 16 + p.sp)
    h = bytearray(header)
    struct.pack_into("<13H", h, 0, 0x5A4D, total % 512, (total + 511) // 512, len(table), header // 16,
                     ((top - len(img) + 15) // 16) & 0xFFFF, 0xFFFF, ss, p.sp, 0, p.ip, cs, 0x1C)
    for i, (seg, off) in enumerate(table):
        struct.pack_into("<HH", h, 0x1C + 4 * i, off, seg)
    return bytes(h) + bytes(img) + p.overlay


# reading --------------------------------------------------------------------------------------
def _load_module(data, h, p, segment, notes, run_movers=True):
    """The load module of an MZ file, unpacked when it was stored packed and relocated for
    `segment`. -> (bytes, [relocation offsets], cs, ip, ss, sp); p.packer / p.unpacked are set."""
    module = bytes(data[h["header"]:h["size"]])
    relocs = []
    for i in range(h["nreloc"]):
        if h["reloc_off"] + 4 * i + 4 > len(data):
            break
        off, seg = struct.unpack_from("<HH", data, h["reloc_off"] + 4 * i)
        if seg * 16 + off + 2 <= len(module):
            relocs.append(seg * 16 + off)
    cs, ip, ss, sp = h["cs"], h["ip"], h["ss"], h["sp"]
    p.packer = _packer(data, h)
    if p.packer == "EXEPACK":
        try:
            module, relocs, cs, ip, ss, sp = unpack_exepack(module, cs, ip)
            p.unpacked = "static"
        except (ValueError, struct.error) as e:
            notes.append("EXEPACK not undone statically (%s)" % e)
    if p.packer and not p.unpacked and (run_movers or p.packer != "moves itself"):
        from codebreaker.emulate import unpack
        try:
            module, relocs, cs, ip, ss, sp = unpack.unpacked(module, relocs, cs, ip, ss, sp, h["min_alloc"],
                                                             moved=p.packer == "moves itself")
            p.unpacked = "emulated"
        except Exception as e:                               # the program is then read as stored
            notes.append("%s not undone (%s: %s); this is the unpacker" % (p.packer, type(e).__name__, e))
    img = bytearray(module)
    for at in relocs:
        if at + 2 <= len(img):
            struct.pack_into("<H", img, at, (struct.unpack_from("<H", img, at)[0] + segment) & 0xFFFF)
    return bytes(img), sorted(relocs), cs, ip, ss, sp


def _data_segment(p, entry):
    """The data segment a program sets up: the first `mov ds, reg` after its entry point, when
    the register was loaded with a relocated constant or copied from CS just before."""
    code = p.image[entry - p.base:entry - p.base + 0x200]
    cs = (entry - p.ip) // 16 if p.kind != "com" else LOAD
    relocs = set(p.relocations)
    value = {}
    i = 0
    while i < len(code) - 3:
        b = code[i]
        if 0xB8 <= b <= 0xBF and entry + i + 1 in relocs:    # mov reg, segment constant
            value[b - 0xB8] = struct.unpack_from("<H", code, i + 1)[0]
            i += 3
        elif b == 0x8C and code[i + 1] & 0xF8 == 0xC8:       # mov reg, cs
            value[code[i + 1] & 7] = cs
            i += 2
        elif b == 0x8E and code[i + 1] & 0xF8 == 0xD8:       # mov ds, reg
            return value.get(code[i + 1] & 7)
        elif b == 0x0E and code[i + 1] == 0x1F:              # push cs / pop ds
            return cs
        elif b in (0xE8, 0xE9, 0xEB, 0xC3, 0xCB, 0x9A, 0xEA, 0xCD):
            break
        else:
            i += 1
    return None


def read(path, run_movers=True):
    """Read a real-mode program. -> Program. ValueError when the file is not one. With
    run_movers False a program that moves itself is read as stored (see the top)."""
    with open(path, "rb") as f:
        data = f.read()
    what = _kind(data, os.path.basename(path))
    if not what:
        raise ValueError("%s: not a real-mode program" % os.path.basename(path))
    p = Program(path, what)
    h = _mz(data)
    if what == "com":
        p.image = b"\xCD\x20" + bytes(0xFE) + data            # the program prefix starts with int 20h
        p.spans = [(LOAD * 16 + 0x100, len(data), "program")]
        p.cs = p.ss = LOAD
        p.ip, p.sp = 0x100, 0xFFFE
        p.entries = [(LOAD * 16 + 0x100, "entry", "near")]
        p.segments = {LOAD * 16 + 0x100: LOAD}
    elif what == "kernel":
        _read_kernel(data, p)
    elif h:                                                   # a program, or a driver linked as one
        _read_mz(data, h, p, what, run_movers)
    elif what == "dos+windows":                               # the DOS program; the module is not overlay
        h, p.windows_at = dos_and_windows(data)
        _read_mz(data, h, p, "mz", run_movers)
        p.overlay = b""
    else:
        p.image = data
        p.spans = [(LOAD * 16, len(data), "driver")]
        p.devices = devices(data[:0x10000])
        p.cs = LOAD
    for d in p.devices:
        seg = p.device_segment * 16
        label = "".join(c if c.isalnum() else "_" for c in d.name.split(" (")[0]).strip("_") or "device"
        label = "dev_" + label if label[0].isdigit() else label    # an assembler name cannot start with a digit
        label = "%s_%X" % (label, d.offset) if not d.character else label
        for va, role in ((seg + d.strategy, "strategy"), (seg + d.interrupt, "interrupt")):
            if va not in p.segments:                          # the devices of one driver share routines
                p.entries.append((va, "%s_%s" % (label, role), "far"))
                p.segments[va] = p.device_segment
    return p


def _read_mz(data, h, p, what, run_movers=True):
    """An MZ program (or a driver linked as one) loaded at LOAD, unpacked when stored packed."""
    p.image, relocs, cs, p.ip, ss, p.sp = _load_module(data, h, p, LOAD, p.notes, run_movers)
    p.cs, p.ss = (LOAD + cs) & 0xFFFF, (LOAD + ss) & 0xFFFF
    p.relocations = [LOAD * 16 + r for r in relocs]
    p.spans = [(LOAD * 16, len(p.image), "program")]
    p.overlay = bytes(data[h["size"]:])
    entry = (p.cs * 16 + p.ip) & 0xFFFFF
    if what == "mz" and p.inside(entry):
        p.entries = [(entry, "entry", "far")]
        p.segments = {entry: p.cs}
    p.devices = [] if p.packer else devices(p.image[:0x10000])
    p.ds = _data_segment(p, entry) if p.entries else None
    if p.ds is None:                                          # one segment for all, or the first of several
        p.ds = p.cs if not p.relocations or p.cs < LOAD else LOAD


def _read_kernel(data, p):
    """IO.SYS: loader, BIOS + DOS kernel, SYSINIT -- each where it runs."""
    h = _mz(data)
    start, first_device = _kernel_body(data, h)
    body = bytes(data[start:h["header"]])
    loader = bytes(data[:start])
    sysinit, relocs, cs, ip, ss, sp = _load_module(data, h, p, SYSINIT_SEGMENT, p.notes)
    p.base = KERNEL_SEGMENT * 16
    end = LOADER_SEGMENT * 16 + len(loader)
    img = bytearray(end - p.base)
    for seg, part in ((KERNEL_SEGMENT, body), (SYSINIT_SEGMENT, sysinit), (LOADER_SEGMENT, loader)):
        img[seg * 16 - p.base:seg * 16 - p.base + len(part)] = part
    if KERNEL_SEGMENT * 16 + len(body) > SYSINIT_SEGMENT * 16 or SYSINIT_SEGMENT * 16 + len(sysinit) > LOADER_SEGMENT * 16:
        raise ValueError("%s: the parts of the kernel image overlap" % p.name)
    p.image = bytes(img)
    p.spans = [(KERNEL_SEGMENT * 16, len(body), "bios and dos"), (SYSINIT_SEGMENT * 16, len(sysinit), "sysinit"),
               (LOADER_SEGMENT * 16, len(loader), "loader")]
    p.relocations = [SYSINIT_SEGMENT * 16 + r for r in relocs]
    p.cs, p.ip, p.ss, p.sp = SYSINIT_SEGMENT + cs, ip, SYSINIT_SEGMENT + ss, sp
    p.ds = KERNEL_SEGMENT
    p.device_segment = KERNEL_SEGMENT
    p.devices = devices(body[:0x10000], first_device, (KERNEL_SEGMENT,))
    p.segments = {KERNEL_SEGMENT * 16: KERNEL_SEGMENT, LOADER_SEGMENT * 16 + 0x200: LOADER_SEGMENT,
                  p.cs * 16 + p.ip: p.cs}
    p.entries = [(KERNEL_SEGMENT * 16, "bios_entry", "far"),
                 (LOADER_SEGMENT * 16 + 0x200, "loader_entry", "far"),
                 (p.cs * 16 + p.ip, "sysinit_entry", "far")]
    p.notes.append("only the first 64 KB of the bios and dos part is addressed from segment %04X; the DOS kernel "
                   "behind it is moved at run time, so absolute references into it are not resolved" % KERNEL_SEGMENT)


EXEPACK_SEGMENT = 0x5000             # where the EXEPACK unpacker's code is read (it runs behind SYSINIT)


def kernel_parts(path):
    """IO.SYS as the programs it is made of, each where it runs. -> (parts, layout):
    parts [(stem, Program)] -- the loader and the BIOS + DOS kernel, stored in the file as they
    run; SYSINIT as the MZ program it is (unpacked: the file stores it as LINK /EXEPACK did);
    and the EXEPACK unpacker's code -- and layout, how SYSINIT was packed (build/exepack.Layout).
    The file is: loader, kernel, SYSINIT packed. The loader's first bytes are the MZ header that
    describes the packed SYSINIT; to the loader they are data."""
    from codebreaker.build import exepack
    with open(path, "rb") as f:
        data = f.read()
    whole = read(path)
    if whole.kind != "kernel":
        raise ValueError("%s: not a kernel image" % whole.name)
    h = _mz(data)
    start, _first = _kernel_body(data, h)
    out = []
    for stem, segment, lo, hi, label in (("msload", LOADER_SEGMENT, 0, start, "loader"),
                                         ("msdos", KERNEL_SEGMENT, start, h["header"], "bios and dos")):
        p = Program(path, "kernel part")
        p.name = "%s (the %s of %s)" % (stem, label, whole.name)
        p.base = segment * 16
        p.image = bytes(data[lo:hi])
        p.spans = [(p.base, len(p.image), label)]
        p.entries = [e for e in whole.entries if p.base <= e[0] < p.base + len(p.image)]
        p.segments = {va: s for va, s in whole.segments.items() if p.base <= va < p.base + len(p.image)}
        p.ds = whole.ds if segment == KERNEL_SEGMENT else segment
        if segment == KERNEL_SEGMENT:
            p.devices, p.device_segment = whole.devices, whole.device_segment
        out.append((stem, p))
    stored = bytes(data[h["header"]:h["size"]])
    layout = exepack.layout(stored, h["cs"], h["ip"])
    p = Program(path, "mz")
    p.name = "sysinit (of %s)" % whole.name
    _read_mz(data, h, p, "mz")
    p.in_kernel = True                                        # its file is IO.SYS: built into it, not a file of its own
    if p.unpacked != "static":
        raise ValueError("%s: SYSINIT is not stored as EXEPACK" % whole.name)
    out.append(("sysinit", p))
    out.append(("exepack", exepack_stub(path, layout)))
    return out, layout


def pklite_head(path, start):
    """PKLITE's entry code and decompressor (the first `start` bytes of the load module it
    stored) as a program of its own: it runs at the start of the program, in the program prefix's
    segment, then copies its decompressor to the top of memory."""
    with open(path, "rb") as f:
        data = f.read()
    h = _mz(data)
    head = Program(path, "kernel part")
    head.name = "pklite (the entry code and decompressor PKLITE put in front of %s)" % os.path.basename(path).lower()
    head.base = LOAD * 16
    head.image = bytes(data[h["header"]:h["header"] + start])
    head.spans = [(head.base, len(head.image), "pklite")]
    head.cs = head.ds = LOAD - 0x10                           # CS = the program prefix, IP = 100h
    head.entries = [(head.base, "start", "far")]
    head.segments = {head.base: head.cs}
    return head


def exepack_stub(path, layout):
    """The EXEPACK unpacker's code (build/exepack.Layout.stub_code) as a program of its own:
    LINK put the same one behind every program it packed."""
    stub = Program(path, "kernel part")
    stub.name = "exepack (the unpacker LINK /EXEPACK put behind %s)" % os.path.basename(path).lower()
    stub.base = EXEPACK_SEGMENT * 16 + 2 * layout.words       # its code follows the header words
    stub.image = layout.stub_code
    stub.spans = [(stub.base, len(stub.image), "exepack")]
    stub.cs = stub.ds = EXEPACK_SEGMENT
    stub.entries = [(stub.base, "unpack", "far")]
    stub.segments = {stub.base: EXEPACK_SEGMENT}
    return stub
