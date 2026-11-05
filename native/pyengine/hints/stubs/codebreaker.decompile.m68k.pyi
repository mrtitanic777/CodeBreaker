from typing import (
    Callable,
    List,
    Optional,
)


def _decode(code: bytes, at: int, pc: int) -> Insn: ...


def _line4(
    w: int,
    r: _Reader,
    mode: int,
    reg: int,
    hi: int,
    ea: Callable,
    done: Callable,
    ext_pc: Callable
) -> Insn: ...


def _s8(v: int) -> int: ...


def decode(code: bytes, at: int, pc: int) -> Insn: ...


class Insn:
    def __init__(
        self,
        mnemonic: str,
        size: str,
        operands: List[<class 'Union'>[Any, codebreaker.decompile.m68k.Op]],
        length: int,
        pc: int,
        raw: bytes,
        word: int
    ) -> None: ...


class Op:
    def __init__(
        self,
        kind: str,
        reg: int = ...,
        value: int = ...,
        xreg: None = ...,
        xsize: str = ...,
        size: None = ...
    ) -> None: ...


class _Reader:
    def __init__(self, code: bytes, at: int) -> None: ...
    def word(self) -> int: ...
