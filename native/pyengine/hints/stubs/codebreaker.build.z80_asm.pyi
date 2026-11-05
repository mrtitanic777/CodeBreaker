from typing import (
    List,
    Tuple,
)


def _items(text: str) -> List[str]: ...


def _put(image: bytearray, bank: int, bank_size: int, pos: int, data: <class 'Union'>[bytes, bytearray]) -> None: ...


def _size_guess(line: str, pc: int) -> bytes: ...


def _strip(line: str) -> str: ...


def build(source: str, folder: str = ...) -> bytes: ...


def build_file(path: str) -> bytes: ...


def build_flat(source: str, folder: str = ...) -> Tuple[int, bytes]: ...
