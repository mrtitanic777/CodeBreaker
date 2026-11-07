from re import (
    Match,
    Pattern,
)
from typing import Callable


def _inside(m: Match) -> str: ...


def blank(text: str, whole: bool = ..., across_lines: bool = ...) -> str: ...


def sub_code(text: str, rx: Pattern, fn: Callable) -> str: ...
