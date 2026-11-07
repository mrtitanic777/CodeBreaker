from codebreaker.source.ne_asm.segment import Seg
from typing import (
    List,
    Set,
    Tuple,
)


def _branches(sg: Seg) -> List[Tuple[int, int]]: ...


def branch_targets(sg: Seg) -> Set[int]: ...


def safe_data_regions(sg: Seg) -> List[Tuple[int, int, str]]: ...
