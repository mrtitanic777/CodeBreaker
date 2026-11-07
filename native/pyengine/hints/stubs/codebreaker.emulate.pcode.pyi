from typing import (
    Any,
    Tuple,
)


def _exe() -> str: ...


def _varnode(text: str, window: bool) -> <class 'Union'>[str, None, Tuple[str, int, int]]: ...


def lifter(mode: str = ...) -> Lifter: ...


def stamp(mode: str = ...) -> Tuple[str, str]: ...


class Lifted:
    def __init__(self, length: int, ops: Any) -> None: ...


class Lifter:
    def __init__(self, exe: str, sla: str, mode: str) -> None: ...
    def lift(self, code: bytes, offset: int) -> Lifted: ...
    def line(self, code: bytes, offset: int) -> str: ...

from typing import Tuple


def _exe() -> str: ...


def lifter(mode: str = ...) -> Lifter: ...


def stamp(mode: str = ...) -> Tuple[str, str]: ...


class Lifter:
    def __init__(self, exe: str, sla: str, mode: str) -> None: ...

from typing import (
    Any,
    Tuple,
)


def _exe() -> str: ...


def _varnode(text: str, window: bool) -> <class 'Union'>[str, None, Tuple[str, int, int]]: ...


def lifter(mode: str = ...) -> Lifter: ...


def stamp(mode: str = ...) -> Tuple[str, str]: ...


class Lifted:
    def __init__(self, length: int, ops: Any) -> None: ...


class Lifter:
    def __init__(self, exe: str, sla: str, mode: str) -> None: ...
    def lift(self, code: bytes, offset: int) -> Lifted: ...

from typing import Tuple


def _exe() -> str: ...


def lifter(mode: str = ...) -> Lifter: ...


def stamp(mode: str = ...) -> Tuple[str, str]: ...


class Lifter:
    def __init__(self, exe: str, sla: str, mode: str) -> None: ...
    def line(self, code: bytes, offset: int) -> str: ...
