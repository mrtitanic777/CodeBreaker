from typing import (
    List,
    Optional,
    Set,
    Tuple,
)


def _builds() -> List[str]: ...


def _is_known(name: str, names: Set[str]) -> bool: ...


def _size(path: str) -> int: ...


def known() -> Set[str]: ...


def stale(now: None = ...) -> List[Tuple[str, int, str]]: ...


def stray() -> List[Tuple[str, int]]: ...

from typing import (
    Any,
    List,
    Optional,
    Set,
    Tuple,
)


def _builds() -> List[Any]: ...


def _is_known(name: str, names: Set[str]) -> bool: ...


def _size(path: str) -> int: ...


def known() -> Set[str]: ...


def stale(now: None = ...) -> List[Tuple[str, int, str]]: ...


def stray() -> List[Tuple[str, int]]: ...
