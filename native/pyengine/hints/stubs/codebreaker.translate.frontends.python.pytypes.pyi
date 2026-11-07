from typing import (
    Any,
    List,
    Optional,
)


def INT(
    lo: <class 'Union'>[int, None] = ...,
    hi: <class 'Union'>[int, None] = ...
) -> T: ...


def _add(x: <class 'Union'>[int, None], y: int) -> <class 'Union'>[int, None]: ...


def _hi(a: <class 'Union'>[int, None], b: <class 'Union'>[int, None]) -> <class 'Union'>[int, None]: ...


def _lo(a: int, b: int) -> int: ...


def bytes_of(
    maxlen: <class 'Union'>[int, None] = ...,
    kind: str = ...
) -> T: ...


def const_int(v: int) -> T: ...


def iadd(
    a: T,
    b: T
) -> T: ...


def iand(
    a: T,
    b: T
) -> T: ...


def ifloordiv(
    a: T,
    b: T
) -> T: ...


def imod(
    a: T,
    b: T
) -> T: ...


def imul(
    a: T,
    b: T
) -> T: ...


def ineg(a: T) -> T: ...


def int_join(
    a: T,
    b: T
) -> T: ...


def ishr(
    a: T,
    b: T
) -> T: ...


def meet(
    a: T,
    lo: <class 'Union'>[int, None] = ...,
    hi: <class 'Union'>[int, None] = ...
) -> T: ...


def s(bits: int) -> T: ...


def top(why: str) -> T: ...


def tuple_of(
    ts: List[T]
) -> T: ...


def u(bits: int) -> T: ...


def widen_int(
    old: T,
    new: T
) -> T: ...


class T:
    def __eq__(self, o: T) -> bool: ...
    def __init__(
        self,
        kind: str,
        args: <class 'Union'>[List[codebreaker.translate.frontends.python.pytypes.T], Tuple[()], Tuple[codebreaker.translate.frontends.python.pytypes.T]] = ...,
        lo: <class 'Union'>[int, None] = ...,
        hi: <class 'Union'>[int, None] = ...,
        name: <class 'Union'>[str, None] = ...,
        site: <class 'Union'>[int, None] = ...,
        why: <class 'Union'>[str, None] = ...
    ) -> None: ...
    def __ne__(self, o: T) -> bool: ...
    def fits_i64(self) -> bool: ...
    def key(self) -> Any: ...
    def nonneg(self) -> bool: ...
    def positive(self) -> bool: ...

from typing import (
    Any,
    List,
    Optional,
)


def INT(
    lo: <class 'Union'>[int, None] = ...,
    hi: <class 'Union'>[int, None] = ...
) -> T: ...


def _add(x: <class 'Union'>[int, None], y: int) -> <class 'Union'>[int, None]: ...


def _hi(a: <class 'Union'>[int, None], b: <class 'Union'>[int, None]) -> <class 'Union'>[int, None]: ...


def _lo(a: int, b: int) -> int: ...


def bytes_of(
    maxlen: <class 'Union'>[int, None] = ...,
    kind: str = ...
) -> T: ...


def const_int(v: int) -> T: ...


def iadd(
    a: T,
    b: T
) -> T: ...


def iand(
    a: T,
    b: T
) -> T: ...


def ifloordiv(
    a: T,
    b: T
) -> T: ...


def imod(
    a: T,
    b: T
) -> T: ...


def imul(
    a: T,
    b: T
) -> T: ...


def ineg(a: T) -> T: ...


def int_join(
    a: T,
    b: T
) -> T: ...


def ishr(
    a: T,
    b: T
) -> T: ...


def meet(
    a: T,
    lo: <class 'Union'>[int, None] = ...,
    hi: <class 'Union'>[int, None] = ...
) -> T: ...


def s(bits: int) -> T: ...


def top(why: str) -> T: ...


def tuple_of(
    ts: List[T]
) -> T: ...


def u(bits: int) -> T: ...


def widen_int(
    old: T,
    new: T
) -> T: ...


class T:
    def __eq__(self, o: T) -> bool: ...
    def __init__(
        self,
        kind: str,
        args: <class 'Union'>[Tuple[codebreaker.translate.frontends.python.pytypes.T], Tuple[()], List[codebreaker.translate.frontends.python.pytypes.T]] = ...,
        lo: <class 'Union'>[int, None] = ...,
        hi: <class 'Union'>[int, None] = ...,
        name: <class 'Union'>[str, None] = ...,
        site: <class 'Union'>[int, None] = ...,
        why: <class 'Union'>[str, None] = ...
    ) -> None: ...
    def __ne__(self, o: T) -> bool: ...
    def fits_i64(self) -> bool: ...
    def key(self) -> Any: ...
    def nonneg(self) -> bool: ...
    def positive(self) -> bool: ...
