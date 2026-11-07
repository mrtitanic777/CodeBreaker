from typing import (
    Any,
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _read() -> Tuple[Dict[str, <class 'Union'>], Dict[str, str]]: ...


def definition(s: Struct, known: Set[Any] = ...) -> str: ...


def layout(name: str) -> Struct: ...


def pointed_to(type_text: str) -> <class 'Union'>[str, None]: ...


class Struct:
    def __init__(self, name: str, size: int, fields: List[Tuple[int, int, str, str]]) -> None: ...
    def typed(self, offset: int, size: int) -> Tuple[str, str]: ...
