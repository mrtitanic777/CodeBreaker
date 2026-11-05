from codebreaker.engine.native_engine import FunctionStore
from sqlite3 import Connection
from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def FunctionStore(
    path: <class 'Union'>[str, None] = ...,
    readonly: bool = ...
) -> FunctionStore: ...


def _ro(path: str) -> Connection: ...


def databases_folder(path: <class 'Union'>[str, None] = ..., own: <class 'Union'>[bool, None] = ...) -> str: ...


def default_path() -> str: ...


def latest(module_sha: str, path: <class 'Union'>[str, None] = ...) -> Dict[str, str]: ...


def open(
    path: <class 'Union'>[str, None] = ...,
    readonly: bool = ...
) -> <class 'Union'>[codebreaker.engine.native_engine.FunctionStore, None]: ...


def overview(path: <class 'Union'>[str, None] = ...) -> List[Dict[str, <class 'Union'>]]: ...
