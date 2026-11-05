from codebreaker.decompile.z80 import Insn
from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def walk(
    image: bytes,
    origin: int,
    entries: Dict[int, str]
) -> Tuple[Dict[int, Insn], Set[int], List[int]]: ...


def write(
    image: bytes,
    origin: int,
    entries: Dict[int, str],
    folder: str,
    stem: str,
    ports: <class 'Union'>[Dict[int, str], None] = ...,
    names: <class 'Union'>[Dict[int, str], None] = ...,
    header: List[str] = ...
) -> Dict[str, <class 'Union'>]: ...
