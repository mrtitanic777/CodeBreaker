"""symbolic.py -- expressions over a run's inputs, made cheaply and handed to the solver only when
asked.

The shadow of a run (emulate/shadow.py) makes an expression for every value an instruction
derives from an input -- a compare's six flags, most of which no jump ever reads. Built as z3
terms they cost the run more than the run itself, so here an expression is a plain tuple, and
z3() makes the solver's term of only those a path's conditions use (once each).

    a bit-vector   (op, bits, ...)        op: var const add sub and or xor mul udiv urem sdiv
                                          srem neg not shl lshr ashr zext sext extract concat
                                          ite popcount bool
    a condition    (op, 0, ...)           op: eq ult ule slt sle bnot band bor bxor nz true

`bool` is a p-code boolean: a byte that is 1 where its condition holds, else 0; `nz` is the
condition that a bit-vector is not zero. The constructors fold what is plain (a condition
tested for being 1 is that condition) so that the conditions stay the size of what was
compared."""
import z3

TRUE = ("true", 0)


def var(name, bits):
    return ("var", bits, name)


def const(value, bits):
    return ("const", bits, value & ((1 << bits) - 1))


def bits(e):
    return e[1]


def binary(op, a, b):
    return (op, a[1], a, b)


def unary(op, a):
    return (op, a[1], a)


def zext(a, to):
    return a if a[1] == to else ("zext", to, a)


def sext(a, to):
    return a if a[1] == to else ("sext", to, a)


def extract(a, lo, width):
    """`width` bits of `a` from bit `lo`."""
    if lo == 0 and width == a[1]:
        return a
    if a[0] == "concat":                                     # (a part of a part: that part)
        hi_part, lo_part = a[2], a[3]
        if lo >= lo_part[1]:
            return extract(hi_part, lo - lo_part[1], width)
        if lo + width <= lo_part[1]:
            return extract(lo_part, lo, width)
    if a[0] == "zext" and lo + width <= a[2][1]:
        return extract(a[2], lo, width)
    if a[0] == "extract":
        return extract(a[2], a[3] + lo, width)
    return ("extract", width, a, lo)


def concat(hi, lo):
    """hi:lo. Two neighbouring parts of one value: that part."""
    if hi[0] == "extract" and lo[0] == "extract" and hi[2] is lo[2] and hi[3] == lo[3] + lo[1]:
        return extract(hi[2], lo[3], hi[1] + lo[1])
    if hi[0] == "const" and lo[0] == "const":
        return const((hi[2] << lo[1]) | lo[2], hi[1] + lo[1])
    return ("concat", hi[1] + lo[1], hi, lo)


def to_bool(c):
    """A condition as a p-code boolean byte."""
    return ("bool", 8, c)


def nonzero(e):
    """The condition that bit-vector `e` is not zero."""
    if e[0] == "bool":
        return e[2]
    if e[0] == "zext" and e[2][0] == "bool":
        return e[2][2]
    return ("nz", 0, e)


def compare(op, a, b):
    """eq / ult / ule / slt / sle of two bit-vectors (a condition)."""
    if op == "eq":
        for x, y in ((a, b), (b, a)):                        # (a boolean byte against 0 or 1)
            if x[0] == "bool" and y[0] == "const":
                return x[2] if y[2] == 1 else negate(x[2]) if y[2] == 0 else ("eq", 0, x, y)
    return (op, 0, a, b)


def negate(c):
    if c[0] == "bnot":
        return c[2]
    return ("bnot", 0, c)


def logic(op, x, y):
    return (op, 0, x, y)


def inputs(e, memo):
    """The names of the inputs expression `e` is over (a frozenset; `memo` by identity)."""
    got = memo.get(id(e))
    if got is not None:
        return got[1]
    if e[0] == "var":
        out = frozenset((e[2],))
    else:
        out = frozenset()
        for x in e[2:]:
            if isinstance(x, tuple):
                out |= inputs(x, memo)
    memo[id(e)] = (e, out)
    return out


def z3_of(e, memo, ctx=None):
    """The z3 term of expression `e` (made once per node: `memo`, by identity), in z3 context
    `ctx` (None: z3's main context). One memo is for one context."""
    got = memo.get(id(e))
    if got is not None:
        return got[1]
    op = e[0]
    if op == "var":
        t = z3.BitVec(e[2], e[1], ctx)
    elif op == "const":
        t = z3.BitVecVal(e[2], e[1], ctx)
    elif op == "true":
        t = z3.BoolVal(True, ctx)
    else:
        args = [z3_of(x, memo, ctx) for x in e[2:] if isinstance(x, tuple)]
        if op in _BV2:
            t = _BV2[op](*args)
        elif op == "neg":
            t = -args[0]
        elif op == "not":
            t = ~args[0]
        elif op == "zext":
            t = z3.ZeroExt(e[1] - e[2][1], args[0])
        elif op == "sext":
            t = z3.SignExt(e[1] - e[2][1], args[0])
        elif op == "extract":
            t = z3.Extract(e[3] + e[1] - 1, e[3], args[0])
        elif op == "concat":
            t = z3.Concat(args[0], args[1])
        elif op == "ite":
            t = z3.If(args[0], args[1], args[2])
        elif op == "bool":
            t = z3.If(args[0], z3.BitVecVal(1, 8, ctx), z3.BitVecVal(0, 8, ctx))
        elif op == "popcount":
            a, width = args[0], e[2][1]
            t = z3.BitVecVal(0, e[1], ctx)
            for k in range(width):
                t = t + z3.ZeroExt(e[1] - 1, z3.Extract(k, k, a))
        elif op in _CMP:
            t = _CMP[op](*args)
        elif op == "bnot":
            t = z3.Not(args[0])
        elif op == "band":
            t = z3.And(*args)
        elif op == "bor":
            t = z3.Or(*args)
        elif op == "bxor":
            t = z3.Xor(*args)
        elif op == "nz":
            t = args[0] != 0
        else:
            raise ValueError("no such expression: %r" % (op,))
    memo[id(e)] = (e, t)                                     # (e kept: its id stays its own)
    return t


def _shift(kind):
    def f(a, b):
        return a << b if kind == "shl" else z3.LShR(a, b) if kind == "lshr" else a >> b
    return f


_BV2 = {"add": lambda a, b: a + b, "sub": lambda a, b: a - b, "and": lambda a, b: a & b,
        "or": lambda a, b: a | b, "xor": lambda a, b: a ^ b, "mul": lambda a, b: a * b,
        "udiv": z3.UDiv, "urem": z3.URem, "sdiv": lambda a, b: a / b, "srem": z3.SRem,
        "shl": _shift("shl"), "lshr": _shift("lshr"), "ashr": _shift("ashr")}
_CMP = {"eq": lambda a, b: a == b, "ult": z3.ULT, "ule": z3.ULE, "slt": lambda a, b: a < b,
        "sle": lambda a, b: a <= b}
