from codebreaker.symbols.libraries import Function
from typing import (
    Dict,
    Iterator,
    List,
    Tuple,
)


def _x86(obj: Coff) -> Iterator[Tuple[str, bytes, bytearray, int]]: ...


def functions(data: bytes) -> List[Function]: ...


def members(data: bytes) -> Iterator[Tuple[str, bytes]]: ...


class Coff:
    def __init__(self, data: bytes) -> None: ...
    def relocations(self, sec: Dict[str, int]) -> List[<class 'Union'>[Any, Tuple[int, int, int]]]: ...

from typing import (
    Any,
    Dict,
    List,
)


class Coff:
    def __init__(self, data: bytes) -> None: ...
    def relocations(self, sec: Dict[str, int]) -> List[Any]: ...

from typing import (
    Dict,
    List,
)


class Coff:
    def __init__(self, data: bytes) -> None: ...
    def relocations(self, sec: Dict[str, int]) -> List[<class 'Union'>[Tuple[int, int, int], Any]]: ...
