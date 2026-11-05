from typing import (
    Dict,
    List,
    Tuple,
)


def _parse_bit(token: str) -> Tuple[int, bool]: ...


def privileges(data: bytes) -> Tuple[List[int], Dict[int, int]]: ...


def set_privileges(data: bytes, changes: List[Tuple[int, bool]]) -> bytes: ...
