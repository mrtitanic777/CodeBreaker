from codebreaker.decompile.z80 import Insn
from codebreaker.formats.sms import ROM
from typing import (
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _slot(page: int) -> int: ...


def _walk(rom: ROM) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def reconstruct(path: str, folder: str) -> Dict[str, <class 'Union'>]: ...

from codebreaker.decompile.z80 import Insn
from codebreaker.formats.sms import ROM
from typing import (
    Dict,
    List,
    Set,
    Tuple,
    Union,
)


def _slot(page: int) -> int: ...


def _walk(rom: ROM) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def reconstruct(path: str, folder: str) -> Dict[str, <class 'Union'>]: ...
