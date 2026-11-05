from codebreaker.formats.native_ne import NEFile
from codebreaker.formats.native_pe import PEFile
from typing import (
    Dict,
    List,
    Tuple,
)


def read(path: str) -> List[Resource]: ...


def read_ne(nef: NEFile) -> List[Resource]: ...


def read_pe(pe: PEFile) -> List[Resource]: ...


def type_name(t: int) -> str: ...


def version(
    data: bytes,
    wide: bool
) -> Tuple[Tuple[int, int, int, int, int, int, int, int, int, int, int, int, int], Dict[str, List[Tuple[str, str]]], List[int]]: ...


class Resource:
    def __init__(self, type: int, name: int, language: <class 'Union'>[int, None], flags: int, data: bytes) -> None: ...
    def __repr__(self) -> str: ...


class _Reader:
    def __init__(self, data: bytes, wide: bool, at: int = ...) -> None: ...
    def align(self, n: int = ...) -> None: ...
    def text(self) -> str: ...
