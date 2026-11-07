from codebreaker.formats.resources import Resource
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _members(
    found: List[Resource],
    wide: <class 'Union'>[bool, int]
) -> Dict[Tuple[int, int, None], List[Resource]]: ...


def _memory(flags: int, wide: int) -> str: ...


def _number(value: int) -> str: ...


def _stem(resource: Resource) -> str: ...


def _style(value: int) -> str: ...


def _write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any],
    wide: int
) -> <class 'Union'>[Tuple[str, List[<class 'Union'>], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, int]]], Dict[str, List[<class 'Union'>]], int]]: ...


def compare(
    original: List[Resource],
    rebuilt: List[Resource],
    wide: <class 'Union'>[bool, int],
    result: <class 'Union'>[Result, None] = ...
) -> Result: ...


def compile_rc(rc: str, wide: int, microsoft: bool = ...) -> Tuple[str, str]: ...


def dialog_text(r: Resource, wide: int) -> List[str]: ...


def ident(value: <class 'Union'>[int, str], wide: int) -> str: ...


def quoted(text: str, wide: int, counted: bool = ...) -> str: ...


def read_res(path: str, wide: int) -> List[Resource]: ...


def rebuild(module_path: str, outdir: str, rounds: int = ...) -> Result: ...


def version_text(r: Resource, wide: int) -> List[str]: ...


def write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any] = ...
) -> <class 'Union'>[Tuple[str, List[<class 'Union'>], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, int]]], Dict[str, List[<class 'Union'>]], int]]: ...


class Result:
    def __init__(self) -> None: ...
    @property
    def same(self) -> bool: ...

from codebreaker.formats.resources import Resource
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
)


def _members(
    found: List[Resource],
    wide: <class 'Union'>[bool, int]
) -> Dict[Tuple[int, int, None], List[Resource]]: ...


def _memory(flags: int, wide: int) -> str: ...


def _number(value: int) -> str: ...


def _stem(resource: Resource) -> str: ...


def _style(value: int) -> str: ...


def _write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any],
    wide: int
) -> <class 'Union'>[Tuple[str, List[<class 'Union'>], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, int]]], Dict[str, List[<class 'Union'>]], int]]: ...


def compare(
    original: List[Resource],
    rebuilt: List[Resource],
    wide: <class 'Union'>[bool, int],
    result: <class 'Union'>[Result, None] = ...
) -> Result: ...


def compile_rc(rc: str, wide: int, microsoft: bool = ...) -> Tuple[str, str]: ...


def dialog_text(r: Resource, wide: int) -> List[str]: ...


def ident(value: <class 'Union'>[int, str], wide: int) -> str: ...


def quoted(text: str, wide: int, counted: bool = ...) -> str: ...


def read_res(path: str, wide: int) -> List[Resource]: ...


def rebuild(module_path: str, outdir: str, rounds: int = ...) -> Result: ...


def version_text(r: Resource, wide: int) -> List[str]: ...


def write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any] = ...
) -> <class 'Union'>[Tuple[str, List[<class 'Union'>], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int], Tuple[str, List[Tuple[int, int, Tuple[int, int, int]]], Dict[str, List[<class 'Union'>]], int]]: ...


class Result:
    def __init__(self) -> None: ...
    @property
    def same(self) -> bool: ...

from codebreaker.formats.resources import Resource
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _members(found: List[Resource], wide: <class 'Union'>[bool, int]) -> Dict[Any, Any]: ...


def _memory(flags: int, wide: int) -> str: ...


def _stem(resource: Resource) -> str: ...


def _write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any],
    wide: int
) -> Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int]: ...


def compare(
    original: List[Resource],
    rebuilt: List[Resource],
    wide: <class 'Union'>[bool, int],
    result: <class 'Union'>[Result, None] = ...
) -> Result: ...


def compile_rc(rc: str, wide: int, microsoft: bool = ...) -> Tuple[str, str]: ...


def ident(value: int, wide: int) -> str: ...


def quoted(text: str, wide: int, counted: bool = ...) -> str: ...


def read_res(path: str, wide: int) -> List[Resource]: ...


def rebuild(module_path: str, outdir: str, rounds: int = ...) -> Result: ...


def version_text(r: Resource, wide: int): ...


def write_rc(
    module_path: str,
    outdir: str,
    verbatim: Set[Any] = ...
) -> Tuple[str, List[Tuple[int, int, Tuple[int, int, None]]], Dict[str, List[<class 'Union'>]], int]: ...


class Result:
    def __init__(self) -> None: ...
    @property
    def same(self) -> bool: ...
