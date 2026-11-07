from codebreaker.emulate.pcode import Lifter
from ctypes import CDLL
from typing import (
    Any,
    List,
    Optional,
    Tuple,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _lib() -> CDLL: ...


def _uch(uc: UcIntel) -> int: ...


def _words(ptr: <class 'Union'>[int, None], n: int, k: int) -> List[Any]: ...


def enabled() -> bool: ...


def handle_of(shadow: <class 'Union'>[codebreaker.emulate.native_shadow.Shadow, None]) -> <class 'Union'>[int, None]: ...


def require() -> None: ...


class Shadow:
    def __del__(self) -> None: ...
    def __init__(
        self,
        kind: int,
        lifter: Lifter,
        inputs: List[Tuple[int, int]],
        span: <class 'Union'>[None, Tuple[int, int]] = ...,
        real_mode: bool = ...
    ) -> None: ...
    def _export(self) -> Any: ...
    def after_call(
        self,
        m: UcIntel,
        symbolic_result: bool,
        value: int,
        args_at: <class 'Union'>[int, None] = ...,
        nargs: int = ...,
        why: <class 'Union'>[str, None] = ...,
        word: int = ...,
        carry: int = ...
    ) -> None: ...
    def enter(
        self,
        places: List[<class 'Union'>[Tuple[str, int, str], Any, Tuple[int, int, int], Tuple[int, int]]],
        bits: int = ...
    ) -> None: ...
    def step(self, m: UcIntel, address: int, cs_base: int = ...) -> None: ...

def handle_of(shadow: None) -> None: ...
