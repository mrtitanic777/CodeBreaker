from codebreaker.source.ne_asm.segment import Seg
from typing import (
    Dict,
    List,
    Tuple,
)


def _texty(run: bytes) -> bool: ...


def data_regions(sg: Seg) -> List[Tuple[int, int, str]]: ...


def find_strings(code: bytes, minlen: int = ...) -> List[Tuple[int, int, str]]: ...


def garbage_spans(rows: Dict[int, str], code: bytes) -> List[Tuple[int, int]]: ...
