from ast import (
    Call,
    Constant,
)
from codebreaker.translate.frontends.python.infer import Eval
from codebreaker.translate.frontends.python.pytypes import T
from typing import (
    Any,
    Dict,
    List,
    Tuple,
)


def _elem(
    ev: Eval,
    t: T
) -> T: ...


def _kind_ok(t: T, want: str) -> bool: ...


def _lazy(
    ev: Eval,
    n: Call,
    args: List[<class 'Union'>[Any, T, None]],
    kw: Dict[str, T],
    ok: bool
) -> None: ...


def _simple(
    ev: Eval,
    n: Call,
    name: str,
    args: List[<class 'Union'>[Any, T]],
    kw: Dict[Any, Any]
) -> T: ...


def _sort_key(
    ev: Eval,
    n: Call,
    e: T,
    kw: Dict[str, <class 'Union'>[<class 'Union'>, None]],
    state: Dict[str, T],
    name: str,
    result: T
) -> Tuple[T, Tuple[str, str]]: ...


def _str(n: Constant) -> str: ...


def _struct(
    ev: Eval,
    n: Call,
    name: str,
    args: List[T],
    kw: Dict[Any, Any]
) -> Tuple[T, Tuple[str, str, str]]: ...


def builtin(
    ev: Eval,
    n: Call,
    name: str,
    args: List[<class 'Union'>[Any, T, None]],
    kw: Dict[str, <class 'Union'>[<class 'Union'>, T, None]],
    state: Dict[str, T]
) -> <class 'Union'>[Tuple[T, Tuple[str, str]], T]: ...


def call(
    ev: Eval,
    n: Call,
    recv: T,
    attr: str,
    args: List[<class 'Union'>[Any, T]],
    kw: Dict[str, T],
    state: Dict[str, T]
) -> Tuple[T, Tuple[str, str]]: ...


def from_bytes(
    ev: Eval,
    n: Call,
    args: List[T],
    kw: Dict[str, T]
) -> Tuple[T, Tuple[str, str]]: ...


def std_call(
    ev: Eval,
    n: Call,
    name: str,
    args: List[T],
    kw: Dict[Any, Any],
    state: Dict[str, T]
) -> <class 'Union'>[Tuple[T, Tuple[str, str, str]], T]: ...
