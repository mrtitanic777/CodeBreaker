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
