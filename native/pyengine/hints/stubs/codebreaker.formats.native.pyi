from ctypes import CDLL
from inspect import Signature
from struct import error
from typing import (
    Any,
    Callable,
    Dict,
    Optional,
)


def _dumps(args: Any) -> bytes: ...


def _exception(kind: str, message: str) -> error: ...


def _load() -> CDLL: ...


def _positional(sig: Signature, args: Any, kw: Dict[Any, Any], skip: int = ...) -> Any: ...


def asker(
    question: str,
    python: Callable,
    where: Callable = ...,
    data: str = ...,
    result: <class 'Union'>[None, Callable] = ...
) -> Callable: ...


def call(question: str, data: <class 'Union'>[bytes, None], *args) -> Any: ...


def dll_path() -> str: ...


def enabled(reader: str) -> bool: ...


def native_first(
    question: str,
    reader: str,
    data_arg: <class 'Union'>[int, None] = ...,
    args_of: <class 'Union'>[None, Callable] = ...,
    result: <class 'Union'>[None, Callable] = ...
) -> Callable: ...
