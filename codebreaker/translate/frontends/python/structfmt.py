"""structfmt.py -- a struct module format string, read the way CPython reads it.

    parse('<12H') -> [('H', 2, 'int', 0, 65535)] * 12          (code, size, kind, lo, hi)

Only the standard-size forms the C++ support header implements are accepted: a byte-order prefix of
< > ! = (or none / @, native: Windows x64, where l and L are 4 bytes), and the codes x c b B ? h H i I l L
q Q f d s. Anything else is None (the call is refused).
"""
import re

_INT = {'b': (1, True), 'B': (1, False), 'h': (2, True), 'H': (2, False), 'i': (4, True), 'I': (4, False),
        'l': (4, True), 'L': (4, False), 'q': (8, True), 'Q': (8, False)}


def parse(fmt):
    if not isinstance(fmt, str):
        return None
    order = '@'
    if fmt[:1] and fmt[:1] in '<>!=@':
        order, fmt = fmt[0], fmt[1:]
    items = []
    for count, code in re.findall(r'\s*(\d*)([a-zA-Z?])', fmt):
        n = int(count) if count else 1
        if code == 'x':
            items.append(('x', n, 'pad', 0, 0))
        elif code == 's':
            items.append(('s', n, 'bytes', n, n))
        elif code == 'c':
            items += [('c', 1, 'bytes', 1, 1)] * n
        elif code == '?':
            items += [('?', 1, 'bool', 0, 1)] * n
        elif code in 'fd':
            items += [(code, 4 if code == 'f' else 8, 'float', 0, 0)] * n
        elif code in _INT:
            size, signed = _INT[code]
            lo, hi = (-(1 << (8 * size - 1)), (1 << (8 * size - 1)) - 1) if signed else (0, (1 << (8 * size)) - 1)
            items += [(code, size, 'int', lo, hi)] * n
        else:
            return None
    if re.sub(r'\s*\d*[a-zA-Z?]', '', fmt).strip():          # a character that is no part of a format, or a count with no code
        return None
    if order == '@' and any(c in 'hHiIlLqQfd' for c, *_ in items):
        # native alignment pads between items; the support header implements standard sizes only
        if any(i[0] not in 'xcbB?s' for i in items) and len(items) > 1:
            return None
    return items


def size(items):
    return sum(i[1] for i in items)


def values(items):
    """the items that are values (pads are not)"""
    return [i for i in items if i[0] != 'x']
