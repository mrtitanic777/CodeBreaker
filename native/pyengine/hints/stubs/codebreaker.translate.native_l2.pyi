from ctypes import CDLL
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _dec(d: Dict[str, Any]) -> Any: ...


def _described(name: str) -> Callable: ...


def _doc1(name: str) -> Callable: ...


def _enc(v: Any) -> Any: ...


def _flags(doc: Dict[str, <class 'Union'>], facts: Dict[str, <class 'Union'>]) -> Set[str]: ...


def _function(
    doc: Dict[str, <class 'Union'>],
    moved: <class 'Union'>[Tuple[str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, str], Tuple[str, str], Tuple[str], Tuple[()]] = ...,
    far: <class 'Union'>[Set[str], None] = ...,
    facts: <class 'Union'>[Any, None] = ...
) -> str: ...


def _held(doc: Dict[str, <class 'Union'>]) -> <class 'Union'>[Tuple[str, Callable], Tuple[str, None]]: ...


def _load() -> CDLL: ...


def _module(
    doc: Dict[str, <class 'Union'>],
    moved: None = ...
) -> <class 'Union'>[Tuple[str, Dict[str, str]], Tuple[str, Dict[Any, Any]]]: ...


def _plain(name: str) -> Callable: ...


def _put(doc: Dict[str, <class 'Union'>]) -> str: ...


def _segment_plan(
    functions: List[Tuple[int, str]],
    near: Set[str],
    where: Dict[int, Tuple[int, int]],
    sites: Dict[int, List[Tuple[int, int]]],
    code: Dict[int, <class 'Union'>[bytes, int]],
    data_segment: int,
    other_data: Set[Any],
    first_selector: int
) -> Dict[str, Dict[str, <class 'Union'>]]: ...


def _takes32(signature: str) -> <class 'Union'>[int, None]: ...


def _text(v: Any) -> str: ...


def _type_text(t: Dict[str, <class 'Union'>[<class 'Union'>, str]], name: str = ..., far: bool = ...) -> str: ...


def call(fn: str, *args, plain = ..., facts = ...) -> Any: ...


def call_raw(fn: str, text: str) -> Tuple[int, str]: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


def known_text(doc: Dict[str, <class 'Union'>], text: str) -> None: ...


def roundtrip_text(doc: Dict[str, <class 'Union'>]) -> str: ...


def validate(doc: Dict[str, <class 'Union'>]) -> Tuple[bool, str]: ...


def why_not() -> None: ...

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
