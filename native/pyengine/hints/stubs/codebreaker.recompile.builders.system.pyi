from typing import (
    Callable,
    List,
)


def _emit_trap(to: int, a_signed: str, a_unsigned: str, b_signed: str, b_unsigned: str) -> List[str]: ...


def emit(
    mn: str,
    ops: List[<class 'Union'>[Any, str]],
    _reg: Callable,
    _vreg: Callable,
    _freg: Callable,
    _imm: Callable,
    _cr: Callable,
    _mem: Callable,
    _rv: Callable,
    _base_expr: Callable,
    _addr: Callable,
    va: int,
    WORD: int
) -> <class 'Union'>[None, List[str]]: ...

from typing import (
    Callable,
    List,
)


def _emit_trap(to: int, a_signed: str, a_unsigned: str, b_signed: str, b_unsigned: str) -> List[str]: ...


def emit(
    mn: str,
    ops: List[<class 'Union'>[Any, str]],
    _reg: Callable,
    _vreg: Callable,
    _freg: Callable,
    _imm: Callable,
    _cr: Callable,
    _mem: Callable,
    _rv: Callable,
    _base_expr: Callable,
    _addr: Callable,
    va: int,
    WORD: int
) -> <class 'Union'>[None, List[str]]: ...
