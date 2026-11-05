from ctypes import CDLL
from typing import (
    Any,
    Callable,
    Dict,
    Optional,
    Tuple,
    Union,
)


def _dec(
    d: Dict[str, <class 'Union'>[<class 'Union'>, List[Any], List[str], str, List[<class 'Union'>]]]
) -> <class 'Union'>[Dict[str, <class 'Union'>], Tuple[str, Dict[str, str]], Set[str], Dict[str, str], Tuple[str, int]]: ...


def _described(name: str) -> Callable: ...


def _doc1(name: str) -> Callable: ...


def _enc(
    v: <class 'Union'>[int, Set[str], None, bool, str]
) -> <class 'Union'>[Dict[str, List[str]], int, None, bool, str]: ...


def _function(
    doc: Dict[str, <class 'Union'>],
    moved: Tuple[str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str] = ...,
    far: None = ...,
    facts: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
) -> str: ...


def _held(doc: Dict[str, <class 'Union'>]) -> <class 'Union'>[Tuple[str, Callable], Tuple[str, None]]: ...


def _load() -> CDLL: ...


def _plain(name: str) -> Callable: ...


def _put(doc: Dict[str, <class 'Union'>]) -> str: ...


def _takes32(signature: str) -> <class 'Union'>[int, None]: ...


def _text(v: Any) -> str: ...


def call(
    fn: str,
    *args,
    plain = ...,
    facts = ...
) -> <class 'Union'>[Dict[str, <class 'Union'>], Tuple[str, Dict[str, str]]]: ...


def call_raw(fn: str, text: str) -> Tuple[int, str]: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


def known_text(doc: Dict[str, <class 'Union'>], text: str) -> None: ...
