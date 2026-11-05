from codebreaker.symbols.libraries.coff import Coff
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
)


def _code_symbols(
    obj: Coff,
    also: Callable = ...
) -> Dict[int, List[Tuple[int, str]]]: ...


def _compile_powerpc(cl: str, header: str, text: str, name: str, work: str, flags: str) -> Tuple[bytes, List[int]]: ...


def _powerpc_objects(obj: bytes) -> Dict[str, Tuple[bytes, List[int]]]: ...


def _trim_words(code: bytes, mask: List[int]) -> Tuple[bytes, List[int]]: ...


def distance(
    built: Tuple[bytes, List[int]],
    original: Tuple[bytes, List[int]],
    arch: str = ...
) -> Tuple[str, int, int, int]: ...


class Arch:
    def __init__(
        self,
        unit: int,
        byteorder: str,
        flag_sets: <class 'Union'>[Tuple[str, str, str, str, str, str, str], Tuple[str, str, str, str, str, str, str, str, str, str], Tuple[str, str, str, str, str, str]],
        trim: Callable,
        object_functions: <class 'Union'>[Callable, None],
        originals: Callable
    ) -> None: ...
