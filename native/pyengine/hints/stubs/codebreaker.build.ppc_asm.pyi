from typing import (
    Optional,
    Tuple,
)


def _dform(op: int, rt: int, ra: int, imm: int) -> int: ...


def _imm(tok: str, bits: int = ..., signed: bool = ...) -> int: ...


def _mem(tok: str) -> Tuple[int, int]: ...


def _reg(tok: str) -> int: ...


def _spr_enc(spr: int) -> int: ...


def _xform(op: int, rt: int, ra: int, rb: int, xo: int, rc: int = ...) -> int: ...


def assemble(text: str, base_addr: int = ...) -> bytes: ...


def assemble_one(
    line: str,
    addr: int = ...,
    labels: <class 'Union'>[Dict[str, int], None] = ...
) -> <class 'Union'>[int, None]: ...

from typing import (
    Dict,
    Optional,
    Tuple,
)


def _dform(op: int, rt: int, ra: int, imm: int) -> int: ...


def _imm(tok: str, bits: int = ..., signed: bool = ...) -> int: ...


def _mem(tok: str) -> Tuple[int, int]: ...


def _reg(tok: str) -> int: ...


def _spr_enc(spr: int) -> int: ...


def _xform(op: int, rt: int, ra: int, rb: int, xo: int, rc: int = ...) -> int: ...


def assemble(text: str, base_addr: int = ...) -> bytes: ...


def assemble_one(
    line: str,
    addr: int = ...,
    labels: <class 'Union'>[Dict[str, int], None] = ...
) -> <class 'Union'>[int, None]: ...
