from capstone import CsInsn
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _native_cfg(insns: List[CsInsn]) -> Tuple[List[Block], Dict[int, int], int]: ...


def _tree(nodes: List[List[<class 'Union'>]]) -> List[Tuple[str, int]]: ...


def build_cfg(
    insns: List[CsInsn],
    lo_addr: int,
    hi_addr: int
) -> Tuple[List[Block], Dict[int, int], int]: ...


def structure(blocks: List[Block], entry: int = ...) -> List[Tuple[str, int]]: ...


class Block:
    def __init__(s, bid: int, lo: int, hi: int, insns: List[CsInsn], addr: int) -> None: ...
