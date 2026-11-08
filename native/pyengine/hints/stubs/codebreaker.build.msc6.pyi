from typing import (
    Optional,
    Tuple,
)


def _tools(compiler: str = ...) -> Tuple[str, str]: ...


def available(compiler: str = ...) -> bool: ...


def compile_many(
    sources: <class 'Union'>[Dict[int, str], Dict[Tuple[str, int], str]],
    header: str,
    flags: str,
    work: <class 'Union'>[str, None] = ...,
    timeout: int = ...,
    compiler: str = ...,
    names: None = ...,
    batch: int = ...,
    together: None = ...
) -> <class 'Union'>[Dict[Tuple[str, int], Tuple[bytes, bytes]], Dict[int, Tuple[bytes, bytes]]]: ...


def dialect(header: str, text: str) -> Tuple[str, str]: ...


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...

from typing import Tuple


def _tools(compiler: str = ...) -> Tuple[str, str]: ...


def available(compiler: str = ...) -> bool: ...


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...

from typing import Tuple


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...

from typing import Tuple


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...
