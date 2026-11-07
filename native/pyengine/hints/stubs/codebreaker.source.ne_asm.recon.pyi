from codebreaker.source.ne_asm.segment import Seg
from typing import (
    Any,
    Callable,
    List,
)


def _safe_data_spans(sg: Seg, start: int, end: int) -> List[Any]: ...


def masm_hex(v: int) -> str: ...


def to_masm(asm: str, is_target: Callable) -> str: ...
