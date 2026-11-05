"""powerpc_image.py -- what a PowerPC PE image states about its own code.

    from codebreaker.formats import powerpc_image
    img = powerpc_image.read(path)
    img.base                        the address the image is built for
    img.spans                       [(address, file offset, size)]: where the file's bytes sit in memory
    img.functions                   sorted function starts
    img.imports                     {address: name}: the stubs calls to other modules go through
    img.inline                      {address: name}: register save/restore runs entered by `bl`
    img.code                        (first address, last address + 1) of the code: every executable section
    img.extents                     {start: (size, prologue size, has an exception handler)}, from .pdata
    img.entry                       the entry point's address (0 when there is none)
    img.word(address)               one big-endian instruction word, or None
    virtual_methods(img)            {address: (class, slot)}: the virtual functions run-time type
                                    information names (see below)

Big-endian PowerPC code in a PE container (machine type 0x1F2), as produced by Microsoft's
PowerPC compilers. Three things about such an image are stated by the image and need no guessing:

Functions. The exception table (.pdata) has one record per function that has unwind information:
its start and its length. Small leaf functions have no record, so every `bl` target inside the
code section is a function start as well.

Imports. A call to another module goes through a 16-byte stub in the code section: two record
words naming the library (by index) and the ordinal, then `mtctr r11; bctr`. The loader rewrites
the two words into a load of the function's address. A stub is named `import<library>_ord_<n>`.

Prologue and epilogue helpers. Functions that save many registers do not do so inline: they call
into a run of `std rN,-X(r1)` instructions that ends `stw r12,-8(r1); blr` (entering at the
instruction for the first register they need), and leave by jumping into the matching run of
`ld rN,-X(r1)` that ends `lwz r12,-8(r1); mtlr r12; blr`. The same exists for floating-point
registers, relative to r12 (`stfd` / `lfd` runs ending `blr`). These runs are part of every
caller's own prologue and epilogue, not functions in their own right: the ones entered by `bl`
are listed in `inline` so their instructions are read as the caller's; the general-register
restore run is reached by a plain jump and simply continues the caller.

Classes. Microsoft's C++ compilers leave run-time type information beside each virtual table of a
class that has any: the word before the table points at a locator (signature 0) whose fourth
word points at the class's type descriptor, which holds the class's decorated name (`.?AVCFoo@@`).
Where that chain is intact, every function the table lists is a virtual function of that class,
named for certain; the standard library's own classes are left out.

Some files hold the image as it sits in memory (every section at its own address) rather than
packed as a file on disk; their section headers still give file positions that are no longer
true. Which of the two a file is, is decided by where the function table's entries point at code.
"""
import os
import struct

from codebreaker.formats import native

MACHINE = 0x01F2


class Image(object):
    """The image: the attributes listed above, read by formats.dll (native/formats/win/powerpc_image.cc)."""

    def __init__(self, path, base, data, answer):
        self.path, self.base, self.data = path, base, data
        self.__dict__.update(answer)

    def offset(self, va):
        for a, off, n in self.spans:
            if a <= va < a + n:
                return off + (va - a)
        return None

    def word(self, va):
        off = self.offset(va)
        if off is None or off + 4 > len(self.data):
            return None
        return struct.unpack_from(">I", self.data, off)[0]

    def bytes_at(self, va, size):
        off = self.offset(va)
        return b"" if off is None else self.data[off:off + size]


def _load_base(path, pe):
    """The address the image runs at: the XEX's load address when the file is a basefile taken from
    one (xex.basefile writes it beside the file), else the PE header's."""
    import json
    try:
        with open(path + ".json", encoding="utf-8") as f:
            return json.load(f).get("base") or pe.image_base
    except (OSError, ValueError):
        return pe.image_base


_CACHE = {}


def read(path):
    """-> Image for a PowerPC PE file (cached per path)."""
    path = os.path.abspath(path)
    if path in _CACHE:
        return _CACHE[path]
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    if pe.machine != MACHINE:
        raise ValueError("%s: not a PowerPC image (machine type %#x)" % (os.path.basename(path), pe.machine))
    with open(path, "rb") as f:
        data = f.read()
    rva, size = pe.datadirs[3] if len(pe.datadirs) > 3 else (0, 0)
    try:
        in_file = pe._rva_to_off(rva)
    except Exception:
        in_file = None
    base = _load_base(path, pe)
    img = Image(path, base, data, native.call("powerpc.read", data, base, rva, size, in_file, pe.entry, pe.sections))
    _CACHE[path] = img
    return img


def virtual_methods(img):
    """{address: (class, slot)} -- each virtual function whose table still links, through its
    locator, to the class's type descriptor (see the module's notes). Big-endian words; the
    classes of the standard library are left out."""
    if hasattr(img, "_virtual"):
        return img._virtual
    img._virtual = native.call("powerpc.virtual", img.data, img.spans, img.functions)
    return img._virtual


def regions(path):
    """[(address, bytes)] -- the image as it sits in memory, for running it."""
    img = read(path)
    return [(va, img.data[off:off + size]) for va, off, size in img.spans]
