from capstone import (
    Cs,
    CsInsn,
)
from typing import (
    Iterator,
    Optional,
    Tuple,
)


def _decode_one(code: bytes, off: int, vaddr: int) -> CsInsn: ...


def _md() -> Cs: ...


def listing(
    code: bytes,
    base_vaddr: int,
    start: int = ...,
    count: <class 'Union'>[int, None] = ...
) -> Iterator[Tuple[int, bytes, str, str]]: ...
