from typing import (
    Any,
    Dict,
    List,
    Tuple,
    Union,
)


def _nd_devices(answer: List[Any], *_) -> List[Any]: ...


def _nd_file_args(path: str, *rest) -> Tuple[bytes, Tuple[str]]: ...


def _nd_program(
    answer: Tuple[str, Dict[str, <class 'Union'>]],
    data: bytes,
    path: str,
    *rest
) -> Program: ...


def _program(path: str, answer: Tuple[str, Dict[str, <class 'Union'>]]) -> Program: ...


class Program:
    def __init__(self, path: str, kind: str) -> None: ...
    def inside(self, linear: int) -> bool: ...
    def word(self, linear: int) -> int: ...

from typing import (
    Any,
    List,
    Tuple,
)


def _nd_file_args(*a) -> Tuple[bytes, Tuple[str]]: ...


def _nd_program(*a) -> Program: ...


def _nd_program_args(*a) -> Tuple[bytes, Tuple[str, str, List[Any], int, int, int, int, bytes]]: ...


class Device:
    def __init__(
        self,
        offset: int,
        nxt: <class 'Union'>[int, None],
        attributes: int,
        strategy: int,
        interrupt: int,
        name: str
    ) -> None: ...


class Program:
    def __init__(self, path: str, kind: str) -> None: ...
    def inside(self, linear: int) -> bool: ...
    def word(self, linear: int) -> int: ...
