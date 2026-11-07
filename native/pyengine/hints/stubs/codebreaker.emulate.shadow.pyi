from codebreaker.platforms import Cpu


def mode(cpu: Cpu) -> str: ...

from codebreaker.emulate.native_shadow import Shadow
from codebreaker.emulate.pcode import Lifter
from typing import (
    Callable,
    List,
    Optional,
    Tuple,
)


def Shadow16(
    lifter: Lifter,
    segbase: Callable,
    inputs: List[Tuple[int, int]],
    span: <class 'Union'>[Tuple[int, int], None] = ...
) -> Shadow: ...
