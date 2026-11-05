from codebreaker.decompile.sm83 import Insn
from codebreaker.formats.gb import ROM
from typing import (
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _walk(rom: ROM) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def reconstruct(path: str, folder: str) -> Dict[str, <class 'Union'>]: ...
