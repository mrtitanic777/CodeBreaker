"""powerpc_image.py -- what a PowerPC PE image states about its own code.

    from codebreaker.formats import powerpc_image
    img = powerpc_image.read(path)
    img.base                        the address the image is built for
    img.spans                       [(address, file offset, size)]: where the file's bytes sit in memory
    img.functions                   sorted function starts
    img.imports                     {address: name}: the stubs calls to other modules go through
    img.inline                      {address: name}: register save/restore runs entered by `bl`
    img.code                        (first address, last address + 1) of the code section
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

MACHINE = 0x01F2
_MFLR_R12 = 0x7D8802A6
_BLR = 0x4E800020
_MTCTR_R11, _BCTR = 0x7D6903A6, 0x4E800420


class Image(object):
    def __init__(self, path, pe, data, memory_layout):
        self.path, self.base, self.data = path, pe.image_base, data
        self.memory_layout = memory_layout
        if memory_layout:
            self.spans = [(pe.image_base, 0, len(data))]
        else:
            self.spans = []
            for s in pe.sections:
                size = min(s["rawsize"], s["vsize"] or s["rawsize"])
                if size and s["rawptr"]:
                    self.spans.append((pe.image_base + s["vaddr"], s["rawptr"], size))
        self.sections = [(s["name"], pe.image_base + s["vaddr"], s["vsize"] or s["rawsize"], s["chars"])
                         for s in pe.sections]
        self.functions, self.imports, self.inline, self.code = [], {}, {}, (0, 0)
        self.extents, self.entry = {}, pe.image_base + pe.entry if pe.entry else 0

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


def _table(data, off, size):
    """[(start, packed)] -- the function table's records: a start, then the prologue's length in
    words (bits 0-7), the function's length in words (8-29), a 32-bit flag (30) and whether it has
    an exception handler (31)."""
    out = []
    for i in range(size // 8):
        if off is None or off + 8 * i + 8 > len(data):
            break
        begin, packed = struct.unpack_from(">II", data, off + 8 * i)
        if begin and (packed >> 8) & 0x3FFFFF:               # a start and a non-zero length
            out.append((begin, packed))
    return out


def _looks_like_code(data, off):
    """A function of these compilers starts by fetching the link register or adjusting the stack;
    anything with a valid primary opcode and not all zero is accepted as weaker evidence."""
    if off is None or off < 0 or off + 4 > len(data):
        return 0
    w = struct.unpack_from(">I", data, off)[0]
    if w == _MFLR_R12 or (w >> 16) == 0x9421:                # mflr r12 / stwu r1,-N(r1)
        return 2
    return 1 if w and (w >> 26) not in (0, 1, 5, 6) else 0


def _runs(words, base):
    """{address: name} for the entries of register save / restore runs, and the set of those
    reached by `bl` (to be read inline)."""
    names, by_call = {}, set()
    n = len(words)

    def back(i, opcode, ra, name, mark, ds=True):
        j = i - 1
        while j >= 0 and (words[j] >> 26) == opcode and ((words[j] >> 16) & 31) == ra and \
                (not ds or (words[j] & 3) == 0):
            names[base + 4 * j] = "%s_%d" % (name, (words[j] >> 21) & 31)
            if mark:
                by_call.add(base + 4 * j)
            j -= 1

    for i in range(n):
        w = words[i]
        if w == 0x9181FFF8 and i + 1 < n and words[i + 1] == _BLR:           # stw r12,-8(r1); blr
            back(i, 62, 1, "__savegprlr", True)
        elif w == 0x8181FFF8 and i + 2 < n and words[i + 1] == 0x7D8803A6 and words[i + 2] == _BLR:
            back(i, 58, 1, "__restgprlr", False)                             # lwz r12,-8(r1); mtlr r12; blr
        elif w == _BLR and i >= 2:
            if (words[i - 1] >> 26) == 54 and ((words[i - 1] >> 16) & 31) == 12 and \
                    (words[i - 2] >> 26) == 54:
                back(i, 54, 12, "__savefpr", True, ds=False)                 # stfd fN,-X(r12)
            elif (words[i - 1] >> 26) == 50 and ((words[i - 1] >> 16) & 31) == 12 and \
                    (words[i - 2] >> 26) == 50:
                back(i, 50, 12, "__restfpr", True, ds=False)                 # lfd fN,-X(r12)
    return names, by_call


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
    # the function table, read both ways; the layout whose entries point at code is the file's
    score = {}
    for memory in (True, False):
        try:
            off = rva if memory else pe._rva_to_off(rva)
        except Exception:
            off = None
        records = _table(data, off, size)
        starts = [b for b, _p in records]
        img = Image(path, pe, data, memory)
        img.extents = {b: (((p >> 8) & 0x3FFFFF) * 4, (p & 0xFF) * 4, bool(p >> 31)) for b, p in records}
        score[memory] = (sum(_looks_like_code(data, img.offset(s)) for s in starts), starts, img)
    if not score[True][1] and not score[False][1]:           # no table: the headers are all there is
        end = max((s["vaddr"] + max(s["vsize"], 1) for s in pe.sections), default=0)
        memory = len(data) >= end
    else:
        memory = score[True][0] > score[False][0]
    _n, starts, img = score[memory]
    # the code section: the executable one holding the entry point or the first function
    code = None
    for name, va, n, flags in img.sections:
        if (flags & 0x20000000 or name == ".text") and (code is None or any(va <= s < va + n for s in starts[:1])):
            code = (va, va + n)
    if code is None:
        _CACHE[path] = img
        return img
    lo, hi = code
    count = max(0, min(hi - lo, len(img.bytes_at(lo, hi - lo))) // 4)
    words = struct.unpack_from(">%dI" % count, img.bytes_at(lo, 4 * count), 0) if count else ()
    for i in range(len(words) - 3):
        if words[i + 2] == _MTCTR_R11 and words[i + 3] == _BCTR and (words[i] >> 24) == 1 and \
                (words[i + 1] >> 24) == 2:
            img.imports[lo + 4 * i] = "import%d_ord_%d" % ((words[i] >> 16) & 0xFF, words[i] & 0xFFFF)
    runs, by_call = _runs(words, lo)
    found = set(starts)
    for i, w in enumerate(words):
        if (w >> 26) == 18 and (w & 3) == 1:                 # bl
            d = w & 0x03FFFFFC
            if d & 0x02000000:
                d -= 0x04000000
            t = lo + 4 * i + d
            if lo <= t < hi:
                found.add(t)
    img.inline = {va: runs[va] for va in by_call}
    skip = set(runs) | set(img.imports)
    img.functions = sorted(va for va in found if va not in skip and lo <= va < hi)
    img.code = code
    img.run_names = runs
    _CACHE[path] = img
    return img


_LIBRARY_CLASS = ("@std@@", "@tr1@", "@stlp")


def virtual_methods(img):
    """{address: (class, slot)} -- each virtual function whose table still links, through its
    locator, to the class's type descriptor (see the module's notes). Big-endian words; the
    classes of the standard library are left out."""
    if hasattr(img, "_virtual"):
        return img._virtual
    import re
    data, spans = img.data, img.spans

    def at(off):                                         # file offset -> address
        for va, o, n in spans:
            if o <= off < o + n:
                return va + (off - o)
        return None
    descriptors = {}
    for m in re.finditer(rb"\.\?A[VU][\w@?$]{1,300}?\x00", data):
        name_va = at(m.start())
        if name_va is None:
            continue
        td = name_va - 8                                 # the descriptor: its table, 0, the name
        if img.word(td + 4) == 0 and img.offset(img.word(td) or 0) is not None:
            descriptors[td] = m.group()[:-1].decode("ascii", "replace")
    locators = {}
    for va, off, n in spans:
        for i in range(0, n - 19, 4):
            if struct.unpack_from(">I", data, off + i)[0] == 0:
                td = struct.unpack_from(">I", data, off + i + 12)[0]
                if td in descriptors:
                    locators[va + i] = descriptors[td]
    starts, out = set(img.functions), {}
    for va, off, n in spans:
        for i in range(0, n - 3, 4):
            name = locators.get(struct.unpack_from(">I", data, off + i)[0])
            if name is None or any(tag in name for tag in _LIBRARY_CLASS):
                continue
            cls = _class_name(name)
            slot = 0
            while img.word(va + i + 4 + 4 * slot) in starts:
                out.setdefault(img.word(va + i + 4 + 4 * slot), (cls, slot))
                slot += 1
    img._virtual = out
    return out


def _class_name(decorated):
    """`.?AVCFoo@@` -> "CFoo";  `.?AVInner@Outer@@` -> "Outer::Inner"."""
    parts = [p for p in decorated[4:].split("@") if p]
    return "::".join(reversed(parts)) if parts else decorated


def regions(path):
    """[(address, bytes)] -- the image as it sits in memory, for running it."""
    img = read(path)
    return [(va, img.data[off:off + size]) for va, off, size in img.spans]
