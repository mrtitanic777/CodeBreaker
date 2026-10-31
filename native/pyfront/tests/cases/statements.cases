"""Module docstring."""
import os
import os.path as osp, sys
from . import a
from .. import b as c
from ...pkg.mod import (d, e as f,)
from .... import g
from x import *
#----
x = y = z = 1
a, b = b, a
[a, *b] = c
(a, b), c = d
a.b = c[d] = e
x: int
x: int = 5
(x): int = 5
a.b: str
a[0]: float = 1.0
x += 1; y -= 2; z *= 3
x /= 1; x //= 2; x %= 3; x **= 4; x @= 5; x &= 6; x |= 7; x ^= 8; x <<= 9; x >>= 10
#----
del a, b[c], d.e
del (a, b)
del [a]
del a,
#----
global a, b
nonlocal c
#----
assert x
assert x, "msg"
raise
raise E
raise E from cause
#----
pass; break; continue
#----
if a:
    pass
elif b:
    pass
elif c:
    x = 1
else:
    y = 2
#----
while x:
    x -= 1
else:
    done()
#----
for i in range(10):
    continue
else:
    pass
#----
for a, (b, c) in items: pass
#----
for *a, b in x: pass
#----
for x, in y: pass
#----
async def f():
    async for x in y:
        pass
    async with a as b, c:
        pass
#----
with a: pass
with a as b, c as (d, e): pass
with (a as b, c as d,): pass
with (a, b): pass
with (a): pass
with (yield): pass
with (open(x)) as f: pass
with (a, b) as c: pass
#----
try:
    pass
except:
    pass
#----
try:
    pass
except E as e:
    pass
except (A, B):
    pass
else:
    pass
finally:
    pass
#----
try:
    pass
finally:
    pass
#----
try:
    pass
except* ValueError as e:
    pass
except* (TypeError, KeyError):
    pass
#----
@decorator
@decorator.attr(arg)
@(lambda f: f)
def f(a, b: int = 1, *args: str, c, d=2, **kwargs: dict) -> None:
    """Docstring."""
    return a
#----
def f(a, /, b, *, c): pass
#----
def f(a=1, /, b=2, *, c, d=3): pass
#----
def f(*, a): pass
#----
def f(**k): pass
#----
def f(*args: *Ts): pass
#----
def f(): return
#----
class A: pass
#----
@dataclass
class B(A, metaclass=M, **kw):
    x: int = 0
    def m(self): ...
#----
class C(): pass
#----
def f[T, *Ts, **P](x: T) -> T: pass
#----
class K[T: int, U = str, *V = (), **W = []]: pass
#----
type X = int
type Y[T] = list[T]
#----
type = 5
type.x = 1
match = 3
case = 4
_ = 5
#----
if x:
    pass
# comment


    # indented comment
y = 1
#----
if x:
	pass
#----
def f():
    if a:
        if b:
            pass
    return
#----
x = 1;
#----
class A:

    def f(self):

        pass

#----
if a: b; c
else: d
#----
while 1:
    if x: break
#----
from __future__ import annotations
#----
import a.b.c
#----
lambda: (yield)
#----
x = [
    1,
    2,
]
#----
print(a, end='')
#----
@property
def x(self): return self._x
#----
except_ = 1
#----
def f(a, b=1, /): pass
#----
def f(a, *, b=1, c): pass
#----
def f(a,): pass
#----
def f(*a,): pass
#----
x = 1 if y else 2
#----

#----
# only a comment
#----
x = 1
# trailing comment without newline
