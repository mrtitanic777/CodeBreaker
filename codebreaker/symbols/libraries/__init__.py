"""libraries -- finding a library's functions in a program that was linked with it.

    from codebreaker.symbols import libraries
    kind, found = libraries.functions(path)     every function of a library or object file:
                                                kind 16, 32 (x86) or "powerpc"; found [Function]
    libraries.fits(function, code, code_mask)   whether `code` starts with the library function
    index = libraries.Index(words)              a program's code, to search for functions in:
    index.find(words, ignore)                   every place a function of words is

A linked program's library functions are the library's bytes except where the linker filled
something in -- an address, the distance of a call -- and the library's own relocation records
say where that is. So a library function is told by its bytes outside those places (its mask,
compare/mask.py): where a program has the same, it has that function, and nobody wrote it.

The readers, one per kind of file, do nothing but read: each function's name, bytes and mask,
and where in its section it starts --

    libraries/omf.py      16-bit object modules and .LIB files (Microsoft C, Visual C++ 1.5)
    libraries/coff.py     32-bit objects and `!<arch>` archives (Visual C++; the Xbox 360 XDK)

What is done with a match is for the one asking: symbols/runtime_library.py keeps digests of the
C runtime's and MFC's functions and names a module's functions by them; symbols/xdk_libraries.py
names an Xbox 360 program's XDK functions; verify/gate16/runtime16.py sets Microsoft C 8's
16-bit library routines beside each function of a module.
"""
import struct
from collections import defaultdict, namedtuple

from codebreaker.compare import mask as masks

Function = namedtuple("Function", "name code mask at")
Function.__doc__ = """One function of a library: its symbol, its bytes, its mask (a byte mask for x86;
for PowerPC the bits left out of each word) and its offset in its section."""


def functions(path):
    """(kind, [Function]) for every function in a library or object file: kind 16 for an OMF
    file, else the machine of its COFF objects -- 32 (x86) or "powerpc"."""
    from codebreaker.symbols.libraries import coff, omf
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] == b"!<arch>\n":
        out, kind = [], None
        for _label, body in coff.members(data):
            try:
                found = coff.functions(body)
            except (struct.error, IndexError):
                continue
            if found and kind is None:
                kind = _KIND.get(struct.unpack_from("<H", body, 0)[0])
            out += found
        return kind or 32, out
    if data[:2] == b"\x4c\x01":
        return 32, coff.functions(data)
    if data[:2] == b"\xf2\x01":
        return "powerpc", coff.functions(data)
    return 16, omf.functions(data)


_KIND = {0x014C: 32, 0x01F2: "powerpc"}


def fits(function, code, code_mask=None):
    """Whether `code` (with `code_mask`, the places it has that a loader filled in) starts with the
    library function: as long at least, and the same at every byte neither leaves out."""
    n = len(function.code)
    if n > len(code):
        return False
    theirs = bytes(code_mask[:n]) if code_mask else b""
    return masks.same(function.code, code[:n], function.mask, theirs + bytes(n - len(theirs)))


class Index(object):
    """A program's code as a run of units (the words of PowerPC code), indexed by each pair of
    neighbours: a function is looked for where its rarest pair of unmasked units is (a prologue's
    pair is everywhere; a pair the program does not have at all rules it out at once)."""

    def __init__(self, units):
        self.units = units
        self.pairs = defaultdict(list)
        for i in range(len(units) - 1):
            self.pairs[(units[i], units[i + 1])].append(i)

    def find(self, sig, ignore):
        """Every place, in order, where the units `sig` are, outside the bits `ignore` leaves out of
        each; [] when no two neighbours of it are whole (nothing to look it up by)."""
        anchor, fewest = None, None
        for i in range(len(ignore) - 1):
            if not ignore[i] and not ignore[i + 1]:
                n = len(self.pairs.get((sig[i], sig[i + 1]), ()))
                if fewest is None or n < fewest:
                    anchor, fewest = i, n
                    if n == 0:
                        break
        if anchor is None or not fewest:
            return []
        hits = []
        for p in self.pairs.get((sig[anchor], sig[anchor + 1]), ()):
            s = p - anchor
            if s < 0 or s + len(sig) > len(self.units):
                continue
            if masks.same_words(self.units[s:s + len(sig)], sig, ignore):
                hits.append(s)
        return hits
