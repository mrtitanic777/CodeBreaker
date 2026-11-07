from codebreaker.engine.native_engine import Database
from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def Database(path: str) -> Database: ...


def databases(folder: None = ...) -> List[Any]: ...


def decode(tag: str, data: bytes) -> Any: ...


def encode(value: Any) -> Tuple[str, bytes]: ...


def hold(
    cap: <class 'Union'>[int, None] = ...,
    folder: <class 'Union'>[str, None] = ...
) -> List[<class 'Union'>[Any, str]]: ...


def max_bytes() -> int: ...


def open_at(path: str) -> Database: ...


def open_for(binary: str, folder: <class 'Union'>[str, None] = ...) -> Database: ...


def root() -> str: ...


def sha(path: str) -> str: ...


def work(db: Database, *parts) -> str: ...


class Row:
    def __init__(
        self,
        db: Database,
        kind: str,
        key: str,
        hash: str,
        tag: str,
        value: <class 'Union'>[bytes, None],
        blob: <class 'Union'>[str, None],
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

from codebreaker.engine.native_engine import Database
from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def Database(path: str) -> Database: ...


def databases(folder: None = ...) -> List[Any]: ...


def decode(tag: str, data: bytes) -> Any: ...


def encode(value: Any) -> Tuple[str, bytes]: ...


def hold(cap: None = ..., folder: None = ...) -> List[Any]: ...


def max_bytes() -> int: ...


def open_at(path: str) -> Database: ...


def open_for(binary: str, folder: None = ...) -> Database: ...


def root() -> str: ...


def sha(path: str) -> str: ...


def work(db: Database, *parts) -> str: ...


class Row:
    def __init__(
        self,
        db: Database,
        kind: str,
        key: str,
        hash: str,
        tag: str,
        value: <class 'Union'>[bytes, None],
        blob: <class 'Union'>[str, None],
        size: int,
        step: str,
        version: str,
        inputs: str,
        made: float,
        took: float
    ) -> None: ...
    def data(self) -> bytes: ...
    def value(
        self
    ) -> <class 'Union'>[Dict[str, <class 'Union'>], Tuple[codebreaker.translate.l2.Made, List[str]], Dict[str, Dict[str, <class 'Union'>]], str, List[str]]: ...

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
