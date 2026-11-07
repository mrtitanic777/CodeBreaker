from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def code_segments(lines: List[Line]) -> Set[str]: ...


def module(name: str, text: str) -> Dict[str, <class 'Union'>]: ...


def parse(text: str) -> List[Line]: ...


def split_ops(s: str) -> List[<class 'Union'>[Any, str]]: ...


def strip_comment(l: str) -> str: ...


class CTypes:
    def __init__(self, port_texts: List[Any], orig_texts: List[Any]) -> None: ...
    @staticmethod
    def _layouts(texts: List[Any], ptr: int, far: int) -> Layouts: ...


class K:
    def __init__(self, kind: str, to: None = ..., old: int = ..., new: int = ..., origin: frozenset = ...) -> None: ...


class Layouts:
    def __init__(self) -> None: ...


class Line:
    def __init__(self, no: int, text: str) -> None: ...


class Port:
    def __init__(
        self,
        doc: Dict[str, <class 'Union'>],
        ctypes: CTypes,
        cnames: Tuple[()] = ...,
        pascal: Tuple[()] = ...,
        cfuncs: None = ...,
        asm_routines: Tuple[()] = ...,
        underscore: bool = ...
    ) -> None: ...
