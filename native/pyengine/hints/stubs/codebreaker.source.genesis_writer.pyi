from codebreaker.decompile.m68k import Insn
from codebreaker.formats.genesis import ROM
from typing import (
    Dict,
    Iterator,
    List,
    Set,
    Tuple,
    Union,
)


def _length(insn: Insn) -> int: ...


def _refs(insn: Insn) -> Iterator[Tuple[int, str]]: ...


def _walk(
    rom: ROM
) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def reconstruct(path: str, folder: str) -> Dict[str, <class 'Union'>]: ...
