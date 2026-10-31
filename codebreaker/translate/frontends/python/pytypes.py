"""pytypes.py -- the Python front end's types: what the inference knows about a value.

A type is immutable and compared by value. Kinds:

    bottom             nothing known yet (a fixpoint starts here)
    top                no one C++ type fits; `why` says what clashed -- the function is refused
    none bool float str
    int                with an interval [lo, hi] of Python (unbounded) integers; None = unbounded.
                       The C++ is int64_t, so an int whose interval leaves int64 is refused.
    bytes bytearray    `hi`: the most bytes it can hold (a slice d[a:a+4] holds at most 4)
    tuple              `args`: the element types, in order
    opt                `args[0]` or None
    list dict set      a container object: `site` names it. Containers are mutable and aliased, so
                       their element types are facts about the object, not the value: every place a
                       container literal is written is a site, and sites that flow together are one
                       object class (union-find in infer.Program). A dict site whose every use names
                       a constant string key is a record (a struct); any other is a dict.
    inst               an instance of a class of the program: `name` is the class's qualified name
    cls fn module      a class, function or module as a value (calls resolve through them)
    extern             something of a library the front end does not map: `name` says which
"""

I64_MIN, I64_MAX = -2 ** 63, 2 ** 63 - 1
# No object in memory has more bytes or elements than the address space: user mode on Windows x64 is
# 2**47 bytes (128 TiB). len(), an index into a sequence, a count of loop passes over one are bounded so.
MEM = 2 ** 47


class T(object):
    __slots__ = ('kind', 'args', 'lo', 'hi', 'name', 'site', 'why')

    def __init__(self, kind, args=(), lo=None, hi=None, name=None, site=None, why=None):
        self.kind = kind
        self.args = tuple(args)
        self.lo = lo
        self.hi = hi
        self.name = name
        self.site = site
        self.why = why

    def key(self):
        return (self.kind, self.args, self.lo, self.hi, self.name, self.site)

    def __eq__(self, o):
        return isinstance(o, T) and self.key() == o.key()

    def __ne__(self, o):
        return not self == o

    def __hash__(self):
        return hash(self.key())

    def __repr__(self):
        k = self.kind
        if k == 'int':
            return 'int[%s..%s]' % (self.lo, self.hi)
        if k in ('bytes', 'bytearray') and self.hi is not None:
            return '%s(<=%d)' % (k, self.hi)
        if k in ('list', 'dict', 'set'):
            return '%s#%s' % (k, self.site)
        if k in ('tuple', 'opt'):
            return '%s[%s]' % (k, ', '.join(repr(a) for a in self.args))
        if k in ('inst', 'cls', 'fn', 'module', 'extern'):
            return '%s %s' % (k, self.name)
        if k == 'top':
            return 'top(%s)' % self.why
        return k

    # -- questions
    @property
    def is_int(self):
        return self.kind == 'int'

    def fits_i64(self):
        return self.kind == 'int' and self.lo is not None and self.hi is not None \
            and I64_MIN <= self.lo and self.hi <= I64_MAX

    def nonneg(self):
        return self.kind == 'int' and self.lo is not None and self.lo >= 0

    def positive(self):
        return self.kind == 'int' and self.lo is not None and self.lo > 0

    def has_zero(self):
        return not (self.kind == 'int' and ((self.lo is not None and self.lo > 0) or (self.hi is not None and self.hi < 0)))


BOTTOM = T('bottom')
NONE = T('none')
BOOL = T('bool')
FLOAT = T('float')
STR = T('str')
BYTES = T('bytes')
BYTEARRAY = T('bytearray')


def top(why):
    return T('top', why=why)


HUGE = 2 ** 128                         # a bound past this is as good as none (and keeps the arithmetic small)


def INT(lo=None, hi=None):
    if lo is not None and lo < -HUGE:
        lo = None
    if hi is not None and hi > HUGE:
        hi = None
    return T('int', lo=lo, hi=hi)


def const_int(v):
    return T('int', lo=v, hi=v)


SIZE = INT(0, MEM)                      # len(), a count
INDEX = INT(-MEM, MEM)                  # anything that indexed a sequence successfully
FIND = INT(-1, MEM)                     # str.find


def u(bits):
    return INT(0, 2 ** bits - 1) if bits <= 128 else INT(0, None)


def s(bits):
    return INT(-2 ** (bits - 1), 2 ** (bits - 1) - 1) if bits <= 128 else INT()


def bytes_of(maxlen=None, kind='bytes'):
    return T(kind, hi=maxlen)


def opt(t):
    if t.kind in ('opt', 'none', 'top', 'bottom'):
        return t
    return T('opt', (t,))


def tuple_of(ts):
    return T('tuple', ts)


def strip_opt(t):
    return t.args[0] if t.kind == 'opt' else t


# ---------------------------------------------------------------------------------------- intervals
def _lo(a, b):
    return None if a is None or b is None else min(a, b)


def _hi(a, b):
    return None if a is None or b is None else max(a, b)


def int_join(a, b):
    return INT(_lo(a.lo, b.lo), _hi(a.hi, b.hi))


def _add(x, y):
    return None if x is None or y is None else x + y


def iadd(a, b):
    return INT(_add(a.lo, b.lo), _add(a.hi, b.hi))


def isub(a, b):
    return INT(None if a.lo is None or b.hi is None else a.lo - b.hi,
               None if a.hi is None or b.lo is None else a.hi - b.lo)


def ineg(a):
    return INT(None if a.hi is None else -a.hi, None if a.lo is None else -a.lo)


def imul(a, b):
    if None in (a.lo, a.hi, b.lo, b.hi):
        if a.lo == a.hi == 0 or b.lo == b.hi == 0:
            return const_int(0)
        if a.nonneg() and b.nonneg():
            return INT(a.lo * b.lo, None)
        return INT()
    c = (a.lo * b.lo, a.lo * b.hi, a.hi * b.lo, a.hi * b.hi)
    return INT(min(c), max(c))


def ifloordiv(a, b):
    """a // b (Python's floor), b's interval not holding 0 or the division raises."""
    if None in (a.lo, a.hi, b.lo, b.hi):
        if a.nonneg() and b.positive():
            return INT(0, a.hi)
        return INT()
    divs = [d for d in (b.lo, b.hi, -1, 1) if b.lo <= d <= b.hi and d != 0]
    if not divs:                                         # always a division by zero: it raises
        return INT()
    c = [x // d for x in (a.lo, a.hi) for d in divs]
    return INT(min(c), max(c))


def imod(a, b):
    """a % b: the sign of b (Python)."""
    if b.positive():
        hi = None if b.hi is None else b.hi - 1
        if a.nonneg() and a.hi is not None and (hi is None or a.hi < hi):
            return INT(a.lo if b.lo > a.hi else 0, a.hi)
        return INT(0, hi)
    if b.hi is not None and b.hi < 0:
        return INT(None if b.lo is None else b.lo + 1, 0)
    return INT(None if b.lo is None else min(0, b.lo + 1), None if b.hi is None else max(0, b.hi - 1))


def ishl(a, b):
    if b.lo is None or b.lo < 0 or b.hi is None or b.hi > 128 or a.lo is None or a.hi is None:
        return INT() if not (a.nonneg()) else INT(0, None)
    c = (a.lo << b.lo, a.lo << b.hi, a.hi << b.lo, a.hi << b.hi)
    return INT(min(c), max(c))


def ishr(a, b):
    if b.lo is None or b.lo < 0:
        return INT()
    if a.lo is None or a.hi is None:
        if a.nonneg():
            return INT(0, None)
        return INT()
    c = [a.lo >> b.lo, a.hi >> b.lo]
    if b.hi is not None:
        c += [a.lo >> b.hi, a.hi >> b.hi]
    else:
        c += [0 if a.lo >= 0 else -1]
    return INT(min(c), max(c))


def _bits_hi(x):
    return (1 << max(x, 0).bit_length()) - 1


def iand(a, b):
    if a.nonneg() and b.nonneg():
        his = [h for h in (a.hi, b.hi) if h is not None]
        return INT(0, min(his) if his else None)
    if a.nonneg():
        return INT(0, a.hi)
    if b.nonneg():
        return INT(0, b.hi)
    if None in (a.lo, a.hi, b.lo, b.hi):
        return INT()
    m = max(_bits_hi(abs(a.lo)), _bits_hi(a.hi), _bits_hi(abs(b.lo)), _bits_hi(b.hi))
    return INT(-m - 1, m)


def ior(a, b):
    if a.nonneg() and b.nonneg():
        if a.hi is None or b.hi is None:
            return INT(0, None)
        return INT(max(a.lo, b.lo), _bits_hi(max(a.hi, b.hi)))
    if None in (a.lo, a.hi, b.lo, b.hi):
        return INT()
    m = max(_bits_hi(abs(a.lo)), _bits_hi(a.hi), _bits_hi(abs(b.lo)), _bits_hi(b.hi))
    return INT(-m - 1, m)


ixor = ior


def iinvert(a):
    return INT(None if a.hi is None else -a.hi - 1, None if a.lo is None else -a.lo - 1)


def ipow(a, b):
    if None in (a.lo, a.hi, b.lo, b.hi) or b.lo < 0 or b.hi > 128 or max(abs(a.lo), abs(a.hi)) > 2 ** 64:
        return INT()
    c = [x ** y for x in (a.lo, a.hi) for y in (b.lo, b.hi)]
    if a.lo < 0 < a.hi:
        c += [0]
    if b.lo != b.hi:
        c += [x ** (y) for x in (a.lo, a.hi) for y in (b.lo + 1, max(b.lo, b.hi - 1))]
    return INT(min(c), max(c))


def meet(a, lo=None, hi=None):
    """`a` limited to [lo, hi] (a guard proved it)."""
    nlo = a.lo if lo is None else (lo if a.lo is None else max(a.lo, lo))
    nhi = a.hi if hi is None else (hi if a.hi is None else min(a.hi, hi))
    if nlo is not None and nhi is not None and nlo > nhi:       # the branch cannot run: keep the guard's
        return INT(nlo, nlo)
    return INT(nlo, nhi)


THRESHOLDS = (0, 1, 255, 0xFFFF, 0xFFFFFFFF, MEM, I64_MAX)


def widen_int(old, new):
    """Widening with thresholds: a bound that grew moves out to the next threshold (then to unbounded)."""
    lo, hi = new.lo, new.hi
    if old.lo is not None and (lo is None or lo < old.lo):
        lo = next((-t for t in THRESHOLDS if lo is not None and -t <= lo), None)
    if old.hi is not None and (hi is None or hi > old.hi):
        hi = next((t for t in THRESHOLDS if hi is not None and t >= hi), None)
    return INT(lo, hi)
