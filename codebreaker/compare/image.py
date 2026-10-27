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
