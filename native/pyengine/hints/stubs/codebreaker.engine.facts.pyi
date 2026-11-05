from codebreaker.engine.native_engine import Database
from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def Database(path: str) -> Database: ...


def decode(tag: str, data: bytes) -> Any: ...


def encode(value: Any) -> Tuple[str, bytes]: ...


def hold(cap: <class 'Union'>[int, None] = ..., folder: <class 'Union'>[str, None] = ...) -> List[str]: ...


def open_at(path: str) -> Database: ...


def open_for(binary: str, folder: <class 'Union'>[str, None] = ...) -> Database: ...


def root() -> str: ...


def sha(path: str) -> str: ...


class Row:
    def __init__(
        self,
        db: Database,
        kind: str,
        key: str,
        hash: str,
        tag: str,
        value: bytes,
        blob: None,
        size: int,
        step: str,
        version: str,
        inputs: str,
        made: float,
        took: float
    ) -> None: ...
    def data(self) -> bytes: ...
    @property
    def given(self) -> bool: ...
    def value(self) -> Any: ...
