def _cb(op: int) -> str: ...


def _main(op: int) -> <class 'Union'>[str, None]: ...

from typing import (
    Any,
    Iterator,
    List,
    Optional,
)


def _cb(op: int) -> str: ...


def _choices(count: int) -> Iterator[List[Any]]: ...


def _fields(pattern: str) -> List[<class 'Union'>[Any, str]]: ...


def _flow(pattern: str) -> str: ...


def _main(op: int) -> <class 'Union'>[str, None]: ...


def assemble(line: str, pc: int = ..., labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(
    code: <class 'Union'>[bytes, bytearray],
    at: int,
    pc: int
) -> <class 'Union'>[codebreaker.decompile.sm83.Insn, None]: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


class Insn:
    def __init__(
        self,
        pattern: str,
        size: int,
        values: List[<class 'Union'>[int, Any]],
        target: <class 'Union'>[int, None],
        flow: str,
        pc: int,
        raw: bytes
    ) -> None: ...
