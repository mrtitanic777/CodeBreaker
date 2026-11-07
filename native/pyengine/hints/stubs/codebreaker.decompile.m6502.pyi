from typing import Optional


def assemble(line: str, pc: int, labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(code: <class 'Union'>[bytes, bytearray], offset: int, pc: int) -> Insn: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


class Insn:
    def __init__(
        self,
        at: int,
        op: int,
        mnemonic: str,
        mode: str,
        operand: <class 'Union'>[int, None],
        size: int,
        target: <class 'Union'>[int, None]
    ) -> None: ...

from typing import Optional


def assemble(line: str, pc: int, labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(code: <class 'Union'>[bytes, bytearray], offset: int, pc: int) -> Insn: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


class Insn:
    def __init__(
        self,
        at: int,
        op: int,
        mnemonic: str,
        mode: str,
        operand: <class 'Union'>[int, None],
        size: int,
        target: <class 'Union'>[int, None]
    ) -> None: ...
