from ctypes import CDLL
from typing import (
    Dict,
    List,
    Optional,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _lib() -> CDLL: ...


def _words(
    ptr: <class 'Union'>[int, None],
    n: int,
    fmt: str
) -> List[<class 'Union'>[Tuple[int, int, int, int, int, int, int, int], Tuple[int, int, int, int, int, int, int, int, int, int, int, int, int, int, int], Any, Tuple[int, int, int, int], Tuple[int, int, int, int, int]]]: ...


def enabled() -> bool: ...


def require() -> None: ...


class Run:
    def __init__(
        self,
        uc: UcIntel,
        *,
        reg_ids,
        ins_in,
        ins_out,
        vm_addr,
        ret,
        scratch,
        spans,
        cap,
        service_eax,
        service_carry,
        gov,
        answers,
        shadow = ...
    ) -> None: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def raised(self) -> None: ...

from ctypes import CDLL
from typing import (
    Dict,
    List,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _lib() -> CDLL: ...


def _words(
    ptr: <class 'Union'>[int, None],
    n: int,
    fmt: str
) -> List[<class 'Union'>[Any, Tuple[int, int, int, int, int, int, int, int], Tuple[int, int, int, int, int, int, int, int, int, int, int, int, int, int, int], Tuple[int, int, int, int], Tuple[int, int, int, int, int]]]: ...


def enabled() -> bool: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        reg_ids,
        ins_in,
        ins_out,
        vm_addr,
        ret,
        scratch,
        spans,
        cap,
        service_eax,
        service_carry,
        gov,
        answers
    ) -> None: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def raised(self) -> None: ...
