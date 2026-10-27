"""image.py -- a built file against the original, byte for byte: are they the same, and where not.

    from codebreaker.compare import image
    image.first_difference(built, original)     # the first offset where they differ (or the shorter length)

Every lane that rebuilds a whole file from its source compares it with the original this way.
"""


def first_difference(a, b):
    """The offset of the first byte where `a` and `b` differ; where one ends first, its length.
    -> None when they are the same."""
    if a == b:
        return None
    n = min(len(a), len(b))
    return next((i for i in range(n) if a[i] != b[i]), n)


def distance(a, b, ma=None, mb=None, unit=1, byteorder="little"):
    """How far `a` is from `b`, unit by unit (a byte, or a word of `unit` bytes), outside what the
    masks leave out: for bytes, ma[i] / mb[i] say whether byte i is left out (a relocated address,
    a call's distance); for words, they are the bits of word i that are left out. When the lengths
    differ the whole tail counts as differing, so a longer or shorter piece is always farther than
    one of the right length.
    -> (verdict, units that differ, units compared, the first that differs or -1)"""
    n = min(len(a), len(b)) // unit
    diff, first = 0, -1
    if unit == 1:
        for i in range(n):
            if (ma and ma[i]) or (mb and mb[i]):
                continue
            if a[i] != b[i]:
                diff += 1
                first = i if first < 0 else first
    else:
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
