from codebreaker.translate.ir import Unit
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _key(
    path: str,
    language: str,
    std: None,
    includes: Tuple[()],
    quote_includes: Tuple[()],
    defines: Tuple[()],
    own: None
) -> str: ...


def clang() -> str: ...


def dump(
    path: str,
    language: <class 'Union'>[str, None] = ...,
    std: None = ...,
    includes: Tuple[()] = ...,
    quote_includes: Tuple[()] = ...,
    defines: Tuple[()] = ...
) -> Dict[str, <class 'Union'>]: ...


def flags(
    language: str,
    std: None = ...,
    includes: Tuple[()] = ...,
    quote_includes: Tuple[()] = ...,
    defines: Tuple[()] = ...
) -> List[str]: ...


def l2_file(
    path: str,
    language: <class 'Union'>[str, None] = ...,
    std: None = ...,
    includes: Tuple[()] = ...,
    quote_includes: Tuple[()] = ...,
    defines: Tuple[()] = ...,
    own: None = ...,
    cache: bool = ...
) -> Dict[str, <class 'Union'>]: ...


def language_of(path: str) -> str: ...


def lower_file(
    path: str,
    language: None = ...,
    std: None = ...,
    includes: Tuple[()] = ...,
    quote_includes: Tuple[()] = ...,
    defines: Tuple[()] = ...,
    own: None = ...
) -> Unit: ...
