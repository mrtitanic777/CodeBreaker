from codebreaker.formats.pe import PEFile
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _load_base(path: str, pe: PEFile) -> int: ...


def read(path: str) -> Image: ...


def regions(path: str) -> List[Tuple[int, bytes]]: ...


class Image:
    def __init__(self, path: str, base: int, data: bytes, answer: Dict[str, <class 'Union'>]) -> None: ...
    def bytes_at(self, va: int, size: int) -> bytes: ...
    def offset(self, va: int) -> int: ...
    def word(self, va: int) -> int: ...

from codebreaker.formats.pe import PEFile
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _load_base(path: str, pe: PEFile) -> int: ...


def read(path: str) -> Image: ...


def regions(path: str) -> List[Tuple[int, bytes]]: ...


class Image:
    def __init__(self, path: str, base: int, data: bytes, answer: Dict[str, <class 'Union'>]) -> None: ...
    def bytes_at(self, va: int, size: int) -> bytes: ...
    def offset(self, va: int) -> int: ...
    def word(self, va: int) -> int: ...
