from capstone import CsInsn
from codebreaker.translate.l2 import Assembly
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _nasm(i: CsInsn, bits: int, ref: Callable) -> str: ...


def _target(i: CsInsn) -> <class 'Union'>[int, None]: ...


class Writer:
    def __init__(self, path: str) -> None: ...
    def _data(
        self,
        L: Assembly,
        dr: _Driver,
        oi: int,
        base: int,
        a: int,
        end: int
    ) -> None: ...
    def _fixrec(
        self,
        L: Assembly,
        dr: _Driver,
        offs: List[int]
    ) -> None: ...
    def _object(
        self,
        L: Assembly,
        dr: _Driver,
        o: Dict[str, <class 'Union'>],
        raw: frozenset,
        lines_of: Dict[Any, Any]
    ) -> None: ...
    def _place(self, dr: _Driver, raw: frozenset) -> None: ...
    def _site_value(
        self,
        dr: _Driver,
        oi: int,
        at: int,
        rec: Dict[str, <class 'Union'>],
        w: int,
        b: bytes
    ) -> str: ...
    def _text(
        self,
        dr: _Driver,
        oi: int,
        i: CsInsn,
        raw: bool,
        bits: int
    ) -> List[str]: ...
    def ref(self, dr: _Driver, oi: int, a: int) -> str: ...
    def source(
        self,
        raw: frozenset = ...,
        mark_all: bool = ...,
        exact: bool = ...
    ) -> Tuple[str, Dict[int, Tuple[str, int, int]]]: ...


class _Driver:
    def __init__(self, data: bytes, lfa: int, tag: str) -> None: ...
    def _decode(self) -> None: ...
    def _function_starts(self, entries: List[Tuple[int, int]]) -> Set[Tuple[int, int]]: ...
    def _page_len(self, n: int) -> int: ...
    def name(self, oi: int, a: int) -> str: ...
