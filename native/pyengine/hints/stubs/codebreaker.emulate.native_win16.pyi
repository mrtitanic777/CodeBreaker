from ctypes import CDLL
from typing import (
    Callable,
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
) -> List[<class 'Union'>[Tuple[int, int, int, int, int], Any, Tuple[int, int, int, int, int, int, int, int]]]: ...


def enabled() -> bool: ...


def require() -> None: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        eip_id,
        near,
        fill,
        cap,
        cs_base,
        near_ret,
        far_ret,
        sent_lo,
        sent_hi,
        snap_base,
        gov,
        intercept,
        on_sentinel,
        on_intercept,
        on_intr,
        shadow = ...,
        own = ...,
        on_own = ...
    ) -> None: ...
    def _wrap(self, fn: Callable) -> Callable: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def raised(self) -> None: ...
    def reset(self) -> None: ...

from ctypes import CDLL
from typing import (
    Callable,
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
) -> List[<class 'Union'>[Tuple[int, int, int, int, int], Tuple[int, int, int, int, int, int, int, int], Any]]: ...


def enabled() -> bool: ...


def require() -> None: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        eip_id,
        near,
        fill,
        cap,
        cs_base,
        near_ret,
        far_ret,
        sent_lo,
        sent_hi,
        snap_base,
        gov,
        intercept,
        on_sentinel,
        on_intercept,
        on_intr,
        shadow = ...,
        own = ...,
        on_own = ...
    ) -> None: ...
    def _wrap(self, fn: Callable) -> Callable: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def raised(self) -> None: ...

from ctypes import CDLL
from typing import (
    Callable,
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
) -> List[<class 'Union'>[Tuple[int, int, int, int, int], Any, Tuple[int, int, int, int, int, int, int, int]]]: ...


def enabled() -> bool: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        eip_id,
        near,
        fill,
        cap,
        cs_base,
        near_ret,
        far_ret,
        sent_lo,
        sent_hi,
        snap_base,
        gov,
        intercept,
        on_sentinel,
        on_intercept,
        on_intr
    ) -> None: ...
    def _wrap(self, fn: Callable) -> Callable: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def raised(self) -> None: ...
