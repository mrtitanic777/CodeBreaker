from codebreaker.symbols.libraries.coff import Coff
from typing import (
    Callable,
    Dict,
    List,
    Set,
    Tuple,
)


def _code_symbols(
    obj: Coff,
    also: Callable = ...
) -> Dict[int, List[Tuple[int, str]]]: ...


def _compile_powerpc(cl: str, header: str, text: str, name: str, work: str, flags: str) -> Tuple[bytes, List[int]]: ...


def _originals_ne(
    module_path: str,
    where: Dict[int, Tuple[int, int]],
    spans: Dict[int, Tuple[Tuple[int, int], int]]
) -> Dict[int, Tuple[bytes, bytes]]: ...


def _powerpc_objects(obj: bytes) -> Dict[str, Tuple[bytes, List[int]]]: ...


def _trim(code: bytes, mask: bytes) -> Tuple[bytes, bytes]: ...


def _trim_words(code: bytes, mask: List[int]) -> Tuple[bytes, List[int]]: ...


def distance(
    built: <class 'Union'>[Tuple[bytes, bytes], Tuple[bytes, List[int]]],
    original: <class 'Union'>[Tuple[bytes, bytes], Tuple[bytes, List[int]]],
    arch: str = ...
) -> Tuple[str, int, int, int]: ...


def needed_header(header: str, text: str) -> str: ...


def period_source(
    header: str,
    texts: Dict[str, str],
    imports: Set[str],
    targets: Set[int]
) -> Tuple[str, Dict[str, str]]: ...


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
