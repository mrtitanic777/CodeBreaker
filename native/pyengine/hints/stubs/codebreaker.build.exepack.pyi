from typing import (
    List,
    Tuple,
)


def layout(module: bytes, cs: int, ip: int) -> Layout: ...


def pack(
    image: bytes,
    relocs: List[int],
    entry: Tuple[int, int],
    stack: Tuple[int, int],
    fills: List[Tuple[int, int, int]],
    stub_code: bytes,
    words: int = ...
) -> Tuple[bytes, Tuple[int, int, int, int]]: ...


class Layout:
    def __init__(self, fills: List[Tuple[int, int, int]], stub_code: bytes, size: int, words: int) -> None: ...
