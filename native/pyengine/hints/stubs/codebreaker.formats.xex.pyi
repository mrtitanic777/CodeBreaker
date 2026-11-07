from typing import (
    Any,
    Iterator,
    List,
    Tuple,
)


def _headers_result(answer: Tuple[List[Tuple[int, int, int]], None], data: bytes, *_) -> Iterator[Any]: ...


def _sha(path: str) -> str: ...


def basefile(path: str) -> str: ...


def basic(path: str, out: str) -> str: ...

from typing import (
    Any,
    Iterator,
    List,
    Tuple,
)


def _headers_result(answer: Tuple[List[Tuple[int, int, int]], None], data: bytes, *_) -> Iterator[Any]: ...


def header(path: str) -> Header: ...


def import_libraries(image_path: str) -> <class 'Union'>[None, List[str]]: ...


def program() -> str: ...


class Header:
    def __init__(self) -> None: ...
