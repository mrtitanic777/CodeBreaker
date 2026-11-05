from typing import (
    List,
    Tuple,
)


def _opened(data: bytes, pairs: List[Tuple[str, int]]) -> List[Tuple[str, LEFile]]: ...


def _where(lf: LEFile) -> Tuple[int]: ...


def members_result(
    answer: Tuple[None, List[Tuple[str, int]]],
    data: bytes,
    path: str
) -> List[Tuple[str, LEFile]]: ...


def vxds_result(
    answer: List[Tuple[str, int]],
    data: bytes,
    path: str
) -> List[Tuple[str, LEFile]]: ...


class LEFile:
    def __init__(s, data: bytes, lfa: int) -> None: ...
    @classmethod
    def from_buffer(cls, data: bytes) -> LEFile: ...
