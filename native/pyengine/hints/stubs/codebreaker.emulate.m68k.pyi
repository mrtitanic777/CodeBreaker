from codebreaker.decompile.m68k import (
    Insn,
    Op,
)
from codebreaker.emulate.run import Run
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def _addr_arith(name: str) -> Callable: ...


def _arith(name: str) -> Callable: ...


def _bcd(name: str) -> Callable: ...


def _bit(name: str) -> Callable: ...


def _div(signed: bool) -> Callable: ...


def _execute(cpu: CPU, insn: Insn) -> None: ...


def _extended(name: str) -> Callable: ...


def _moveq(
    cpu: CPU,
    insn: Insn,
    size: str,
    ops: List[Op]
) -> None: ...


def _mul(signed: bool) -> Callable: ...


def _quick(name: str) -> Callable: ...


def _rts(cpu: CPU, insn: Insn, size: str, ops: List[Any]) -> None: ...


def _stop_run(why: str) -> Callable: ...


def record(out: Dict[str, <class 'Union'>]) -> Run: ...


def recorded(bus: Bus, entry: int, **kw) -> Run: ...


def run(
    bus: Bus,
    entry: int,
    steps: int = ...,
    d: None = ...,
    a: None = ...,
    sp: int = ...,
    sr: int = ...
) -> Dict[str, <class 'Union'>]: ...


class Bus:
    def __init__(self, memory: <class 'Union'>[bytearray, None] = ..., size: int = ...) -> None: ...
    def read(self, a: int, n: int) -> int: ...
    def write(self, a: int, n: int, v: int) -> None: ...


class CPU:
    def __init__(self, bus: Bus) -> None: ...
    def logic(self, r: int, size: str) -> None: ...
    def pop(self, size: str = ...) -> int: ...
    def push(self, v: int, size: str = ...) -> None: ...
    def rd(self, a: int, size: str) -> int: ...
    def setf(
        self,
        x: None = ...,
        n: <class 'Union'>[int, None] = ...,
        z: <class 'Union'>[bool, None] = ...,
        v: <class 'Union'>[int, None] = ...,
        c: <class 'Union'>[int, None] = ...
    ) -> None: ...
    def step(self) -> None: ...
    def wr(self, a: int, size: str, v: int) -> None: ...
