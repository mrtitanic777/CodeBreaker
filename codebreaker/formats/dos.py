"""dos.py -- the ONE reader for real-mode programs: what DOS, or the boot sector, would load.
Answered by the C++ (native/formats/dos: formats.dll); its Python reader is gone (docs/NATIVE.md, step 4).

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
          own relocation table), anything else by running the unpacker (in formats.dll, on Unicorn).
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

from codebreaker.formats import native


# ---- the C++ reader (native/formats/dos): its Python is gone (docs/NATIVE.md, step 4) ----------
def _nd_file_args(path, *rest):
    """(the file's bytes, (its name, *rest)) -- as the path-taking readers read a file."""
    with open(path, "rb") as f:
        return f.read(), (os.path.basename(path),) + rest


def _nd_lower_name_args(path, *rest):
    with open(path, "rb") as f:
        return f.read(), rest + (os.path.basename(path).lower(),)


def _nd_program_args(p, *rest):
    """unpacked_file(p): the program's image, and what is read of it besides."""
    return p.image, (p.kind, p.name, p.relocations, p.cs, p.ss, p.sp, p.ip, p.overlay)


def _nd_data_segment_args(p, entry):
    return p.image, (p.base, p.ip, p.kind, p.relocations, entry)


def _program(path, answer):
    kind, state = answer
    p = Program(path, kind)
    state["devices"] = _nd_devices(state["devices"])
    p.__dict__.update(state)
    return p


def _nd_program(answer, data, path, *rest):
    return _program(path, answer)


def _nd_devices(answer, *_):
    return [Device(*t) for t in answer]


LOAD = 0x1000                       # the segment a program is loaded at
KERNEL_SEGMENT = 0x0070             # where the boot sector and the loader put IO.SYS
LOADER_SEGMENT = 0x9000             # where the loader moves itself with 640 KB of memory
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
@native.answered("dos._mz")
def _mz(data, stub=False):
    """The fields of an MZ header, or None. `size` is the end of the load module in the file.
    A Windows module's DOS stub is None unless `stub`."""


@native.answered("dos.devices", result=_nd_devices)
def devices(image, at=0, segments=(0, 0xFFFF), limit=64):
    """The device headers chained from offset `at` of one segment's bytes. [] when what is there
    is not a device header: the routines must lie inside the image, a character device must
    have a printable name, and the chain must end in a link of offset FFFF."""


@native.answered("dos._kernel_body")
def _kernel_body(data, h):
    """Where the BIOS/DOS part of an IO.SYS starts in the file: a sector boundary holding a jump
    followed at once by the chain of built-in devices (CON, AUX, PRN, ...) whose links name
    KERNEL_SEGMENT. None when the file is not laid out that way."""


@native.answered("dos._kind")
def _kind(data, name=""):
    ...


@native.answered("dos.chain")
def chain(data):
    """Programs stored one after another in one file, each with its own MZ header: [(offset,
    bytes)], or [] for one alone. CodeView 4.1's CV.EXE is three -- a stub that checks the CPU and
    memory, the DOSX16 extender, and CodeView itself (448 KB, run in protected mode) -- and only
    the stub was taken apart: 7 functions of a debugger. (A program's overlays, stored after it the
    same way, come out as parts too.) Each part starts at the next byte, or after up to 15 bytes
    of zeros to a paragraph."""


@native.answered("dos.driver_and_vxd")
def driver_and_vxd(data):
    """A real-mode device driver and a VxD in one file (DRVSPACE.BIN): an MZ header whose load
    module is the driver -- device headers at its start -- and whose new-header field points at
    an LE behind it, the VxD. -> (header size, LE offset), or None."""


@native.answered("dos.dos_and_windows")
def dos_and_windows(data):
    """A DOS program and a Windows module in one file (REGEDIT.EXE, XCOPY32.EXE, UNINSTAL.EXE):
    the module's DOS stub is itself a real program -- run from DOS, it does the work there.
    -> (MZ fields, offset of the module's new header), or None."""


@native.answered("dos.driver_part", args_of=_nd_lower_name_args, result=_nd_program)
def driver_part(path):
    """The real-mode driver of a driver+vxd file, as a Program (a driver image)."""


@native.answered("dos.kind", data_arg=None)
def kind(path):
    """What kind of real-mode program a file is, or None. See the module docstring."""


@native.answered("dos._packer")
def _packer(data, h):
    ...


@native.answered("dos.packer", args_of=_nd_file_args)
def packer(path):
    """How an MZ program's load module is stored compressed, or None when it is not (or the
    scheme is not one this knows by its marks)."""


def programs(extensions=EXTENSIONS):
    """One path per program name of the corpus that kind() accepts, by name."""
    from codebreaker.base import corpus
    return [cands[0] for name, cands in sorted(corpus.index().items())
            if name.endswith(tuple(extensions)) and kind(cands[0])]


# EXEPACK --------------------------------------------------------------------------------------
@native.answered("dos.unpack_exepack")
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


@native.answered("dos.unpack_emulated")
def unpack_emulated(module, relocations, cs, ip, ss, sp, moved=False):
    """Run the unpacker of a packed load module to the real entry point, in the emulator (twice, at
    two load segments: words that differ by the distance are the relocations). With `moved`, run a
    program that only moves itself until it continues in the moved copy. -> (load module with no
    load segment applied, [relocation offsets], cs, ip, ss, sp), the segments relative to the load
    segment. RuntimeError when no real entry point is reached or the two runs do not agree."""


@native.answered("dos.unpacked_file", args_of=_nd_program_args)
def unpacked_file(p):
    """A Program read from a packed MZ file, written back as a plain MZ file that DOS runs the
    same way: the unpacked load module with its relocation table (grouped per 64 KB, as EXEPACK
    kept it), the real entry and stack, room above the image for the stack, and whatever the file
    held behind its load module. -> bytes"""


@native.answered("dos._data_segment", args_of=_nd_data_segment_args)
def _data_segment(p, entry):
    """The data segment a program sets up: the first `mov ds, reg` after its entry point, when
    the register was loaded with a relocated constant or copied from CS just before."""


@native.answered("dos.read", args_of=_nd_file_args, result=_nd_program)
def read(path, run_movers=True):
    """Read a real-mode program. -> Program. ValueError when the file is not one. With
    run_movers False a program that moves itself is read as stored (see the top)."""


@native.answered("dos.pklite_head", args_of=_nd_lower_name_args, result=_nd_program)
def pklite_head(path, start):
    """PKLITE's entry code and decompressor (the first `start` bytes of the load module it
    stored) as a program of its own: it runs at the start of the program, in the program prefix's
    segment, then copies its decompressor to the top of memory."""


