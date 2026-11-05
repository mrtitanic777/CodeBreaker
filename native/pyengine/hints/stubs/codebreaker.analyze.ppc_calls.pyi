from re import Pattern
from typing import (
    List,
    Set,
)


def _consensus(arities: List[int], stub: bool) -> <class 'Union'>[int, None]: ...


def _reg(tok: str, pattern: Pattern = ...) -> int: ...


def fpr_writes(mnem: str, ops: List[str]) -> Set[int]: ...


def gpr_writes(mnem: str, ops: List[str]) -> Set[int]: ...
