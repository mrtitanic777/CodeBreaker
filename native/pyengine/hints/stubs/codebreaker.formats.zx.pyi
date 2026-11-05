from typing import (
    Dict,
    List,
    Tuple,
)


def _program(
    path: str,
    kind: str,
    memory: bytearray,
    registers: Dict[str, int],
    pc: int,
    extra_entries: Tuple[()] = ...
) -> Program: ...


def _sna(data: bytes, path: str) -> Program: ...


def _tap(data: bytes, path: str) -> Program: ...


def _usr_calls(basic: bytes) -> List[int]: ...


def is_zx(path: str) -> bool: ...


def read(path: str) -> Program: ...
