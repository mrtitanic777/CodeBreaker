from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _globals(code: bytes) -> Dict[int, Tuple[str, str]]: ...


def _kind(c_type: str) -> str: ...


def _segment_signatures(module_path: str) -> Dict[Tuple[int, int], List[Tuple[str, str]]]: ...


def compiler_of(code: bytes) -> str: ...


def module(module_path: str, limit: int = ..., flags: None = ..., log: None = ...) -> Dict[str, <class 'Union'>]: ...


def signature(name: str) -> <class 'Union'>[Tuple[str, List[Any]], Tuple[str, List[Tuple[str, str]]], None]: ...
