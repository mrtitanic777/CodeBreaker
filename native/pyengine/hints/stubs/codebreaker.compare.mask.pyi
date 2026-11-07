from typing import (
    List,
    Optional,
    Set,
    Tuple,
)


def blank(code: <class 'Union'>[bytes, bytearray], offsets: <class 'Union'>[List[int], Set[int]]) -> bytes: ...


def differing(a: bytes, b: bytes, *masks) -> List[<class 'Union'>[int, Any]]: ...


def distance(
    a: bytes,
    b: bytes,
    ma: <class 'Union'>[bytes, List[int], None] = ...,
    mb: <class 'Union'>[bytes, List[int], None] = ...,
    unit: int = ...,
    byteorder: str = ...
) -> Tuple[str, int, int, int]: ...


def of(offsets: Set[int], size: int) -> bytearray: ...


def same(a: bytes, b: bytes, *masks) -> bool: ...

from typing import Set


def blank(code: bytearray, offsets: Set[int]) -> bytes: ...

from typing import (
    List,
    Optional,
    Tuple,
)


def blank(code: <class 'Union'>[bytes, bytearray], offsets: <class 'Union'>[List[int], Set[int]]) -> bytes: ...


def distance(
    a: bytes,
    b: bytes,
    ma: <class 'Union'>[List[int], None] = ...,
    mb: <class 'Union'>[List[int], None] = ...,
    unit: int = ...,
    byteorder: str = ...
) -> Tuple[str, int, int, int]: ...
