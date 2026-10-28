"""mask.py -- the parts of a function left out when it is compared with another build of it.

    from codebreaker.compare import mask
    mask.of(offsets, size)                  a byte mask: 1 for every offset in `offsets`
    mask.blank(code, offsets)               the bytes with those offsets set to zero
    mask.same(a, b, *masks)                 the same length, and the same outside every mask
    mask.differing(a, b, *masks)            the offsets where they differ outside every mask
    mask.same_words(a, b, ignore)           words: the same outside the bits `ignore` leaves out
    mask.distance(a, b, ma, mb, unit=1)     how far apart, unit by unit (a byte, or a word)

Two builds of the same code differ where a loader or linker filled something in: a relocated
address, the distance of a call, a far call the linker rewrote. Every comparison of one function
with another -- a library's function against a program, a compiled reconstruction against the
original, one version of a module against the next -- leaves those places out, and does it here.

A mask comes in one of two forms. For code read a byte at a time (x86) it says, byte by byte,
whether the byte is left out: a sequence of flags, any true value meaning left out (`of` makes
one from a set of offsets). For code made of fixed-size words (PowerPC) it says, word by word,
which bits of the word are left out -- a relocation there fills a field of the instruction, and
its opcode and registers are still compared.
"""


def of(offsets, size):
    """A byte mask `size` long that leaves out every offset in `offsets` (those outside it ignored)."""
    out = bytearray(size)
    for o in offsets:
        if 0 <= o < size:
            out[o] = 1
    return out


def blank(code, offsets):
    """`code` with the bytes at `offsets` set to zero: what is left to compare, or to digest."""
    out = bytearray(code)
    for o in offsets:
        if 0 <= o < len(out):
            out[o] = 0
    return bytes(out)


def same(a, b, *masks):
    """Whether `a` and `b` are as long as each other and the same at every byte no mask leaves out."""
    if len(a) != len(b):
        return False
    for i in range(len(a)):
        if a[i] != b[i] and not any(m[i] for m in masks if m):
            return False
    return True


def differing(a, b, *masks):
    """The offsets (up to the shorter one's length) where `a` and `b` differ and no mask leaves the
    byte out."""
    return [i for i in range(min(len(a), len(b))) if a[i] != b[i] and not any(m[i] for m in masks if m)]


def same_words(a, b, ignore):
    """Whether two runs of words are the same outside the bits `ignore` leaves out of each word."""
    return all(not ((x ^ y) & ~m) for x, y, m in zip(a, b, ignore))


def distance(a, b, ma=None, mb=None, unit=1, byteorder="little"):
    """How far `a` is from `b`, unit by unit (a byte, or a word of `unit` bytes), outside what the
    masks leave out: for bytes, ma[i] / mb[i] say whether byte i is left out (a relocated address,
    a call's distance); for words, they are the bits of word i that are left out. When the lengths
    differ the whole tail counts as differing, so a longer or shorter piece is always farther than
    one of the right length.
    -> (verdict, units that differ, units compared, the first that differs or -1)"""
    n = min(len(a), len(b)) // unit
    if unit == 1:
        found = differing(a, b, ma, mb)
        diff, first = len(found), found[0] if found else -1
    else:
        diff, first = 0, -1
        full = (1 << 8 * unit) - 1
        for i in range(n):
            ignore = (ma[i] if ma else 0) | (mb[i] if mb else 0)
            wa = int.from_bytes(a[unit * i:unit * i + unit], byteorder)
            wb = int.from_bytes(b[unit * i:unit * i + unit], byteorder)
            if (wa ^ wb) & ~ignore & full:
                diff += 1
                first = i if first < 0 else first
    extra = abs(len(a) - len(b)) // unit
    diff += extra
    if first < 0 and extra:
        first = n
    verdict = "identical" if diff == 0 else "same size" if len(a) == len(b) else "different"
    return verdict, diff, max(len(a), len(b)) // unit, first
