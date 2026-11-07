from ctypes import CDLL
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _load() -> CDLL: ...


def _sha_id(lib: CDLL, sha: str) -> int: ...


def _static_id(lib: CDLL, lo: int, hi: int, image: bytes, table: Dict[Any, Any]) -> int: ...


def _words(ptr: <class 'Union'>[int, None], n: int, fmt: str) -> List[Any]: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


def require() -> None: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        statics,
        init_pages,
        mapped,
        sentinels,
        saved,
        sha,
        gov,
        fast,
        lean,
        reads,
        quiet,
        stop_wild,
        stub,
        unique,
        branches,
        pads,
        insn_cap,
        stub_ret,
        stub_cleanup,
        func,
        sec,
        lean_ranges,
        sig_resolver,
        outs,
        cache = ...,
        sentinel_from = ...,
        shadow = ...
    ) -> None: ...
    def _api(self, code: int) -> str: ...
    def attach_code(self) -> None: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def detach(self) -> None: ...
    def restore(self) -> None: ...
    def settle(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def start(self, uc: UcIntel, begin: int, until: int, count: int = ...) -> None: ...
    def switched(self) -> bool: ...

from ctypes import CDLL
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _load() -> CDLL: ...


def _sha_id(lib: CDLL, sha: str) -> int: ...


def _static_id(lib: CDLL, lo: int, hi: int, image: bytes, table: Dict[Any, Any]) -> int: ...


def _words(
    ptr: <class 'Union'>[int, None],
    n: int,
    fmt: str
) -> List[<class 'Union'>[Any, Tuple[int, int, int, int, int], Tuple[int, int, int, int, int, int, int, int], Tuple[int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int]]]: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


def require() -> None: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        statics,
        init_pages,
        mapped,
        sentinels,
        saved,
        sha,
        gov,
        fast,
        lean,
        reads,
        quiet,
        stop_wild,
        stub,
        unique,
        branches,
        pads,
        insn_cap,
        stub_ret,
        stub_cleanup,
        func,
        sec,
        lean_ranges,
        sig_resolver,
        outs,
        cache = ...,
        sentinel_from = ...,
        shadow = ...
    ) -> None: ...
    def _api(self, code: int) -> str: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def settle(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def start(self, uc: UcIntel, begin: int, until: int, count: int = ...) -> None: ...

from ctypes import CDLL
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Union,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def _load() -> CDLL: ...


def _sha_id(lib: CDLL, sha: str) -> int: ...


def _static_id(
    lib: CDLL,
    lo: int,
    hi: int,
    image: bytes,
    table: <class 'Union'>[Dict[Tuple[int, int], <class 'Union'>], Dict[int, <class 'Union'>], Dict[<class 'Union'>, <class 'Union'>]]
) -> int: ...


def _words(ptr: <class 'Union'>[int, None], n: int, fmt: str) -> List[Any]: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


class Run:
    def __del__(self) -> None: ...
    def __init__(
        self,
        uc: UcIntel,
        *,
        statics,
        init_pages,
        mapped,
        sentinels,
        saved,
        sha,
        gov,
        fast,
        lean,
        reads,
        quiet,
        stop_wild,
        stub,
        unique,
        branches,
        pads,
        insn_cap,
        stub_ret,
        stub_cleanup,
        func,
        sec,
        lean_ranges,
        sig_resolver,
        outs,
        cache = ...,
        sentinel_from = ...
    ) -> None: ...
    def _api(self, code: int) -> str: ...
    def attach_code(self) -> None: ...
    def collect(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def detach(self) -> None: ...
    def restore(self) -> None: ...
    def settle(self, rec: Dict[str, <class 'Union'>]) -> None: ...
    def start(self, uc: UcIntel, begin: int, until: int, count: int = ...) -> None: ...
    def switched(self) -> bool: ...

from ctypes import CDLL


def _load() -> CDLL: ...


def dll_path() -> str: ...
