from typing import (
    List,
    Optional,
    Tuple,
)


def fits(
    function: Function,
    code: bytes,
    code_mask: <class 'Union'>[None, bytearray] = ...
) -> bool: ...


def functions(path: str) -> Tuple[int, List[Function]]: ...
