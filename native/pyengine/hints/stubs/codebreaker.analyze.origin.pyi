from typing import (
    Dict,
    Optional,
)


def module16(path: str, more: <class 'Union'>[Dict[int, str], None] = ...) -> Result: ...

from typing import (
    Dict,
    Optional,
)


def module16(path: str, more: <class 'Union'>[Dict[int, str], None] = ...) -> Result: ...

from capstone import Cs
from typing import (
    Dict,
    Optional,
    Set,
    Tuple,
)


def _judge(
    o: Origin,
    md: Cs,
    image: bytes,
    start: int,
    end: int,
    imported: Dict[str, str],
    starts: Set[int] = ...,
    g3: bool = ...
) -> None: ...


def _linker16(path: str) -> Tuple[int, int]: ...


def _module16(path: str, more: <class 'Union'>[Dict[int, str], None] = ...) -> Result: ...


def module16(path: str, more: <class 'Union'>[Dict[int, str], None] = ...) -> Result: ...


class Origin:
    def __init__(self, va: int, name: str) -> None: ...


class Result:
    def __init__(self) -> None: ...
