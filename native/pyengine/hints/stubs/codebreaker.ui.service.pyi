from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def _functions(module: str, addr: <class 'Union'>[str, None] = ...) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def _row(row: <class 'Union'>[Dict[str, <class 'Union'>], None], addr: str) -> Dict[str, <class 'Union'>]: ...


def call(what: str, /, /, *args, **kw) -> <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, List[Any]], None]: ...


def client(start: None = ...) -> None: ...


def find() -> None: ...


def folder() -> str: ...


def functions(module: str, addr: <class 'Union'>[str, None] = ...) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def mode() -> str: ...


def record() -> None: ...


def registered(name: str) -> Callable: ...


class Jobs:
    def __init__(self) -> None: ...
    def all(self) -> List[Any]: ...
    def start(self, action: str, module: str): ...
