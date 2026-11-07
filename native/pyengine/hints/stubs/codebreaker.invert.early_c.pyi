from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _flags() -> Tuple[bool, bool, bool, bool, bool, bool, bool, bool, None, bool, None]: ...


def recover(code: bytes, base: int = ..., cxmap: None = ...) -> Tuple[int, List[str], List[str], str, List[Any]]: ...


def recover_cfg(code: bytes, base: int = ..., cxmap: None = ...) -> Tuple[int, List[str], List[str], str, List[Any]]: ...


def to_c(
    name: str,
    fs: int,
    params: List[str],
    stmts: List[str],
    ptype: str = ...,
    locals: <class 'Union'>[List[Any], None] = ...,
    longset: None = ...
) -> str: ...
