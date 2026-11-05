from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _cr(tok: str) -> int: ...


def _freg(tok: str) -> int: ...


def _imm(tok: str) -> int: ...


def _mem(tok: str) -> Tuple[int, int]: ...


def _reg(tok: str) -> int: ...


def _reg_or_zero(tok: str) -> int: ...


def _rv(n: int, field: str = ...) -> str: ...


def _split(op_str: str) -> List[<class 'Union'>[Any, str]]: ...


def _vreg(tok: str) -> int: ...


def compiler() -> str: ...


def emit_insn(
    va: int,
    mn: str,
    op_str: str,
    labels: Set[int],
    func_lo: int,
    func_hi: int,
    raw: int = ...,
    jump_tables: None = ...
) -> List[str]: ...


def kernel_includes() -> List[str]: ...


def recompile_function(
    body: bytes,
    base_va: int,
    func_va: int,
    size: int,
    img: None = ...,
    img_base: int = ...,
    is_exec: None = ...
) -> Tuple[str, Set[Any], Dict[str, int]]: ...


def runtime_header() -> str: ...
