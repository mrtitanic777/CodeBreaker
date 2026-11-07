from ctypes import CDLL
from inspect import Signature
from typing import (
    Any,
    Callable,
    Dict,
    Optional,
)


def _dumps(args: Any) -> bytes: ...


def _exception(kind: str, message: str) -> <class 'Union'>[struct.error, ValueError, codebreaker.formats.cab.CabError]: ...


def _load() -> CDLL: ...


def _positional(
    sig: Signature,
    args: Any,
    kw: Dict[str, <class 'Union'>[<class 'Union'>, List[str], str, Tuple[bytes]]],
    skip: int = ...
) -> Any: ...


def _unicorn_dll() -> str: ...


def answered(
    question: str,
    data_arg: <class 'Union'>[int, None] = ...,
    args_of: <class 'Union'>[Callable, None] = ...,
    result: <class 'Union'>[Callable, None] = ...
) -> Callable: ...


def ask(
    question: str,
    where: Callable = ...,
    data: str = ...,
    result: <class 'Union'>[Callable, None] = ...
) -> Callable: ...


def call(question: str, data: <class 'Union'>[bytes, None], *args) -> Any: ...


def capstone_dll() -> str: ...


def dll_path() -> str: ...


def required() -> CDLL: ...

from ctypes import CDLL
from inspect import Signature
from typing import (
    Any,
    Callable,
    Dict,
    Optional,
)


def _dumps(args: Any) -> bytes: ...


def _exception(kind: str, message: str) -> <class 'Union'>[codebreaker.formats.cab.CabError, ValueError, struct.error]: ...


def _load() -> CDLL: ...


def _positional(
    sig: Signature,
    args: Any,
    kw: Dict[str, <class 'Union'>[<class 'Union'>, List[str], str, Tuple[bytes]]],
    skip: int = ...
) -> Any: ...


def _unicorn_dll() -> str: ...


def answered(
    question: str,
    data_arg: <class 'Union'>[int, None] = ...,
    args_of: <class 'Union'>[Callable, None] = ...,
    result: <class 'Union'>[Callable, None] = ...
) -> Callable: ...


def ask(
    question: str,
    where: Callable = ...,
    data: str = ...,
    result: <class 'Union'>[Callable, None] = ...
) -> Callable: ...


def call(question: str, data: <class 'Union'>[bytes, None], *args) -> Any: ...


def capstone_dll() -> str: ...


def dll_path() -> str: ...


def required() -> CDLL: ...

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
