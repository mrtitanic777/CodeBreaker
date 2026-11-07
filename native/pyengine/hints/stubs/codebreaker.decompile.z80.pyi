from typing import (
    Any,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _balanced(s: str) -> bool: ...


def _build() -> Dict[Tuple[bytes, int], <class 'Union'>]: ...


def _canonical(insn: Insn) -> bool: ...


def _cb(op: int, target: str = ...) -> str: ...


def _choices(count: int) -> Iterator[List[Any]]: ...


def _ed(op: int) -> <class 'Union'>[str, None]: ...


def _fields(pattern: str) -> List[<class 'Union'>[Any, str]]: ...


def _flow(pattern: str) -> str: ...


def _index(pattern: str, ix: str) -> <class 'Union'>[str, None]: ...


def _lone_prefix(code: bytes, at: int, pc: int) -> Insn: ...


def _main(op: int) -> str: ...


def _patterns() -> Dict[str, <class 'Union'>]: ...


def _plain_text(insn: Insn, labels: Dict[int, str]) -> str: ...


def _split_operands(s: str) -> List[str]: ...


def assemble(line: str, pc: int = ..., labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(code: <class 'Union'>[bytes, bytearray], at: int, pc: int) -> Insn: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


def value(expr: str, labels: Dict[str, int], pc: int = ...) -> int: ...


class Insn:
    def __init__(
        self,
        pattern: str,
        size: int,
        values: List[<class 'Union'>[Any, int]],
        target: <class 'Union'>[int, None],
        flow: str,
        pc: int,
        raw: bytes
    ) -> None: ...

from typing import (
    Dict,
    Tuple,
    Union,
)


def _build() -> Dict[Tuple[bytes, int], <class 'Union'>]: ...


def _cb(op: int, target: str = ...) -> str: ...


def _ed(op: int) -> <class 'Union'>[str, None]: ...


def _index(pattern: str, ix: str) -> <class 'Union'>[str, None]: ...


def _main(op: int) -> str: ...


def value(expr: str, labels: Dict[str, int], pc: int = ...) -> int: ...

from typing import (
    Any,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _balanced(s: str) -> bool: ...


def _build() -> Dict[Tuple[bytes, int], <class 'Union'>]: ...


def _canonical(insn: Insn) -> bool: ...


def _cb(op: int, target: str = ...) -> str: ...


def _choices(count: int) -> Iterator[List[Any]]: ...


def _ed(op: int) -> <class 'Union'>[str, None]: ...


def _fields(pattern: str) -> List[<class 'Union'>[Any, str]]: ...


def _flow(pattern: str) -> str: ...


def _index(pattern: str, ix: str) -> <class 'Union'>[str, None]: ...


def _lone_prefix(code: bytes, at: int, pc: int) -> Insn: ...


def _main(op: int) -> str: ...


def _patterns() -> Dict[str, <class 'Union'>]: ...


def _plain_text(insn: Insn, labels: Dict[int, str]) -> str: ...


def _split_operands(s: str) -> List[str]: ...


def assemble(line: str, pc: int = ..., labels: <class 'Union'>[Dict[str, int], None] = ...) -> bytes: ...


def decode(code: <class 'Union'>[bytes, bytearray], at: int, pc: int) -> Insn: ...


def text(insn: Insn, labels: <class 'Union'>[Dict[int, str], None] = ...) -> str: ...


def value(expr: str, labels: Dict[str, int], pc: int = ...) -> int: ...


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
