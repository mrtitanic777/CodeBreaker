from sqlite3 import Connection
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _connect(path: str) -> Connection: ...


def _count(kind: str, found: bool) -> None: ...


def _db(kind: str) -> Connection: ...


def _drop_others(path: str, gen: str) -> None: ...


def _hold(base: str, cap: int) -> int: ...


def _size(path: str) -> int: ...


def _stores(base: <class 'Union'>[str, None] = ...) -> Dict[str, List[Tuple[str, bool]]]: ...


def files_bytes(*folders) -> str: ...


def folder(path: str) -> str: ...


def generation(kind: str) -> str: ...


def get(kind: str, k: str) -> <class 'Union'>[bytes, None]: ...


def hold(cap: None = ...) -> int: ...


def key(*parts) -> str: ...


def max_bytes() -> int: ...


def off() -> bool: ...


def put(kind: str, k: str, data: bytes) -> None: ...


def root(kind: <class 'Union'>[str, None] = ...) -> str: ...


def roots() -> List[Tuple[str, int]]: ...


def sizes(base: <class 'Union'>[str, None] = ...) -> Dict[str, int]: ...


def tool(path: str) -> str: ...


def tool_bytes(path: str) -> str: ...
