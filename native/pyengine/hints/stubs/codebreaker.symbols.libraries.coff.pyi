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
