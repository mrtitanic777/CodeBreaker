from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def carve(module: str) -> PeCarve: ...


def disasm_whole32(code: bytes, base: int = ..., progress_cb: None = ...) -> List[Tuple[int, str]]: ...


def ensure_target() -> None: ...


def function_spans(path: str) -> Dict[int, <class 'Union'>]: ...


class PeCarve:
    def __init__(self, path: str, progress_cb: None = ...) -> None: ...


class PeSeg:
    def __init__(
        self,
        module: str,
        section: <class 'Union'>[str, None] = ...,
        carve: <class 'Union'>[PeCarve, None] = ...,
        progress_cb: None = ...
    ) -> None: ...


class _Insns:
    def __init__(self, carve: PeCarve, addresses: List[int]) -> None: ...

from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def carve(module: str) -> PeCarve: ...


def disasm_whole32(code: bytes, base: int = ..., progress_cb: None = ...) -> List[Tuple[int, str]]: ...


def ensure_target() -> None: ...


def function_spans(path: str) -> Dict[int, <class 'Union'>]: ...


class PeCarve:
    def __init__(self, path: str, progress_cb: None = ...) -> None: ...
    def _carve(self) -> Dict[int, <class 'Union'>[Dict[str, <class 'Union'>], <class 'Union'>]]: ...
    def _decode_from(self, va: int) -> None: ...
    def _find_starts(self) -> Set[int]: ...
    def _sec_of(self, va: int) -> <class 'Union'>[Tuple[int, bytes], Tuple[None, None]]: ...
    def _sweep(self) -> None: ...


class PeSeg:
    def __init__(
        self,
        module: str,
        section: <class 'Union'>[str, None] = ...,
        carve: <class 'Union'>[PeCarve, None] = ...,
        progress_cb: None = ...
    ) -> None: ...
