from typing import (
    Callable,
    Dict,
    List,
    Optional,
)


def _absolute(op: Op) -> <class 'Union'>[int, None]: ...


def _arith(
    w: int,
    line: int,
    r: _Reader,
    mode: int,
    reg: int,
    hi: int,
    ea: Callable,
    done: Callable
) -> Insn: ...


def _brief(o: Op) -> int: ...


def _category(mode: int, reg: int) -> <class 'Union'>[int, None]: ...


def _decode(code: bytes, at: int, pc: int) -> <class 'Union'>[codebreaker.decompile.m68k.Insn, None]: ...


def _ea(
    r: _Reader,
    mode: int,
    reg: int,
    size: str,
    pc_of_ext: Callable
) -> <class 'Union'>[Op, None]: ...


def _encode(
    word: int,
    mn: str,
    size: str,
    ops: List[<class 'Union'>[codebreaker.decompile.m68k.Op, Any]],
    pc: int
) -> bytes: ...


def _hex(v: int, digits: int = ...) -> str: ...


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


def _ok(mode: int, reg: int, need: int, no_an: bool = ..., no_imm: bool = ...) -> bool: ...


def _parse_operand(s: str, labels: Dict[str, int], pc: int, size: str) -> Op: ...


def _parse_reg(s: str) -> <class 'Union'>[Tuple[str, int], None]: ...


def _parse_regs(s: str) -> int: ...


def _reg(kind: str, n: int) -> str: ...


def _regs(mask: int, predec: bool) -> str: ...


def _s16(v: int) -> int: ...


def _s8(v: int) -> int: ...


def _shapes() -> Dict[str, int]: ...


def _split(s: str) -> List[str]: ...


def _value(expr: str, labels: Dict[str, int], pc: int) -> int: ...


def _wrap(v: int) -> int: ...


def assemble(line: str, pc: int = ..., labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(code: bytes, at: int, pc: int) -> <class 'Union'>[codebreaker.decompile.m68k.Insn, None]: ...


def operand_text(o: Op, labels: Dict[int, str]) -> str: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


class Insn:
    def __init__(
        self,
        mnemonic: str,
        size: str,
        operands: List[<class 'Union'>[codebreaker.decompile.m68k.Op, Any]],
        length: int,
        pc: int,
        raw: bytes,
        word: int
    ) -> None: ...
    def shape(self) -> str: ...


class Op:
    def __init__(
        self,
        kind: str,
        reg: int = ...,
        value: int = ...,
        xreg: <class 'Union'>[Tuple[str, int], None] = ...,
        xsize: str = ...,
        size: <class 'Union'>[int, str, None] = ...
    ) -> None: ...
    def key(self) -> str: ...


class _Reader:
    def __init__(self, code: bytes, at: int) -> None: ...
    def long(self) -> int: ...
    def word(self) -> int: ...

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
