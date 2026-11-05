from codebreaker.decompile.m6502 import Insn
from codebreaker.formats.nes import ROM
from typing import (
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _banks(rom: ROM) -> List[Tuple[str, int, int, int]]: ...


def _lines(data: List[<class 'Union'>[int, Any]]) -> List[<class 'Union'>[Any, str]]: ...


def _walk(rom: ROM) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def reconstruct(path: str, folder: str) -> Dict[str, <class 'Union'>]: ...
