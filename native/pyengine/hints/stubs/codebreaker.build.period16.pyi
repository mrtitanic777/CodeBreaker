from typing import (
    List,
    Optional,
    Tuple,
)


def _kit() -> str: ...


def _response_file(folder: str) -> str: ...


def _win(p: str) -> str: ...


def build(folder: str, work: <class 'Union'>[str, None] = ..., flags: str = ...) -> str: ...


def compare(built: str, original: str, log: None = ...) -> Tuple[int, int]: ...


def segments(path: str) -> List[Tuple[int, bytes]]: ...
