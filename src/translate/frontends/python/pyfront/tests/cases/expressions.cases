a + b * c - d / e // f % g @ h ** -i ** j
#----
x = not a or b and c or not d
#----
a < b <= c == d != e > f >= g is h is not i in j not in k
#----
y = a if b else c if d else e
#----
f = lambda: 0
#----
f = lambda a, /, b=1, *c, d, e=2, **k: (a, b, c, d, e, k)
#----
g = lambda *, k: k
#----
z = (x := 10) + (y := x * 2)
#----
if (n := len(a)) > 10: pass
#----
v = [y for x in data if (y := f(x))]
#----
a[1:2, ::3, 4:, :5:6, ...]
#----
a[:]
#----
a[b:c:d]
#----
a[*idx]
#----
a[x, *y]
#----
a[1,]
#----
f(a, *b, c=1, *d, **e)
#----
f(x for x in y)
#----
f(*a, b)
#----
print(*[1, 2], sep='')
#----
x = a.b.c(d)[e].f
#----
x = ~a & b | c ^ d << 2 >> 1
#----
x = -1, +2, ~3
#----
x = 1,
#----
x = ()
#----
x = (1)
#----
x = ((1, 2), [3, *rest], {4: 5, **m}, {6, *s})
#----
d = {}
#----
d = {**a, 'b': 1, **c}
#----
s = {x for x in y}
#----
d = {k: v for k, v in items if k for _ in z}
#----
g = (x async for x in aiter() if await x)
#----
async def f():
    return [x async for x in y], await z, [await w for w in v]
#----
def g():
    x = yield
    y = yield 1, 2
    z = yield from gen()
    await_ = (yield)
#----
x = 0x_FF + 0o17 + 0b1010 + 1_000_000 + 0
#----
x = 123456789012345678901234567890 + 0xFFFFFFFFFFFFFFFFFFFFFFFF
#----
x = 1.0 + 1. + .5 + 1e10 + 1E-5 + 1.5e+300 + 1e999 + 3.14_15
#----
x = 1e16, 1e15, 0.0001, 0.00001, 123.456, 1e22, 2.5e-7
#----
x = 1j + 2.5J + 1e3j + 0j + 1e16j
#----
x = 00 + 0_0
#----
x = True, False, None, ...
#----
x = a if b else (lambda: c)
#----
x = [*a, *b]
#----
x = a or (b and c) or d
#----
x = (yield)
#----
x = (a
     + b
     - c)
#----
x = a \
    + b
#----
f(**{'a': 1})
#----
x = a[b][c](d)(e)
#----
x = -a ** -b
#----
x = await_ = a
#----
x = a.b[c].d = e
#----
x = f(a)(b)
#----
print(f(x) for x in y)
