from codebreaker.formats.le import LEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def _bundle(data: bytes, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _cell(key: str, value: <class 'Union'>[int, None, bool, str, List[str]]) -> str: ...


def _dos(data: bytes, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _driver(
    f: LEFile,
    name: str,
    info: Dict[str, <class 'Union'>],
    part: _Parts
) -> None: ...


def _flags(
    value: int,
    table: <class 'Union'>[Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]]]
) -> List[<class 'Union'>[Any, str]]: ...


def _hex_lines(data: bytes, base: int = ...) -> List[str]: ...


def _le(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _locator(path: str) -> Callable: ...


def _ne(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _other(path: str, info: Dict[str, <class 'Union'>]) -> Dict[str, <class 'Union'>]: ...


def _pe(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _preview(type_id: int) -> str: ...


def _size(n: int) -> str: ...


def _text_kind(path: str, name: str) -> <class 'Union'>[Dict[str, str], None]: ...


def _version_text(data: bytes, wide: bool) -> str: ...


def _when(stamp: int) -> str: ...


def describe(path: str) -> Dict[str, <class 'Union'>]: ...


def folder_map(folder: str, log: None = ...) -> Dict[str, <class 'Union'>]: ...


def hexdump(path: str, offset: int = ..., length: int = ...) -> str: ...


def main(argv: List[str]) -> int: ...


def markdown(info: Dict[str, <class 'Union'>], xex_header: None = ...) -> str: ...


def render(
    value: <class 'Union'>[Dict[str, <class 'Union'>], List[Dict[str, <class 'Union'>]], List[<class 'Union'>], Dict[str, int]],
    key: str = ...,
    indent: int = ...
) -> List[<class 'Union'>[Any, str]]: ...


def resource_body(
    path: str,
    offset: int,
    size: int,
    type_id: int = ...,
    wide: bool = ...,
    limit: int = ...
) -> Dict[str, str]: ...


def strings(path: str, min_len: int = ..., wide: bool = ..., limit: None = ...) -> List[<class 'Union'>]: ...


def xex_facts(path: str) -> Dict[Any, Any]: ...


class _Parts:
    def __call__(self, key: str, make: Callable, default: <class 'Union'>[Any, None] = ...) -> Any: ...
    def __init__(self, info: Dict[str, <class 'Union'>]) -> None: ...

from codebreaker.formats.le import LEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def _bundle(data: bytes, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _cell(key: str, value: <class 'Union'>[int, None, bool, str, List[str]]) -> str: ...


def _dos(data: bytes, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _driver(
    f: LEFile,
    name: str,
    info: Dict[str, <class 'Union'>],
    part: _Parts
) -> None: ...


def _flags(
    value: int,
    table: <class 'Union'>[Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]]]
) -> List[<class 'Union'>[Any, str]]: ...


def _hex_lines(data: bytes, base: int = ...) -> List[str]: ...


def _le(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _locator(path: str) -> Callable: ...


def _ne(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _other(path: str, info: Dict[str, <class 'Union'>]) -> Dict[str, <class 'Union'>]: ...


def _pe(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _preview(type_id: int) -> str: ...


def _version_text(data: bytes, wide: bool) -> str: ...


def _when(stamp: int) -> str: ...


def describe(path: str) -> Dict[str, <class 'Union'>]: ...


def hexdump(path: str, offset: int = ..., length: int = ...) -> str: ...


def main(argv: List[str]) -> int: ...


def render(
    value: <class 'Union'>[Dict[str, <class 'Union'>], List[Dict[str, <class 'Union'>]], List[<class 'Union'>], Dict[str, int]],
    key: str = ...,
    indent: int = ...
) -> List[<class 'Union'>[Any, str]]: ...


def resource_body(
    path: str,
    offset: int,
    size: int,
    type_id: int = ...,
    wide: bool = ...,
    limit: int = ...
) -> Dict[str, str]: ...


def strings(path: str, min_len: int = ..., wide: bool = ..., limit: None = ...) -> List[<class 'Union'>]: ...


def xex_facts(path: str) -> Dict[Any, Any]: ...


class _Parts:
    def __call__(self, key: str, make: Callable, default: <class 'Union'>[Any, None] = ...) -> Any: ...
    def __init__(self, info: Dict[str, <class 'Union'>]) -> None: ...

from codebreaker.formats.native_le import LEFile
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def _cell(key: str, value: <class 'Union'>[int, None, bool, str, List[str]]) -> str: ...


def _dos(data: bytes, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _driver(
    f: LEFile,
    name: str,
    info: Dict[str, <class 'Union'>],
    part: _Parts
) -> None: ...


def _flags(
    value: int,
    table: <class 'Union'>[Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]], Tuple[Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str], Tuple[int, str]]]
) -> List[<class 'Union'>[Any, str]]: ...


def _hex_lines(data: bytes, base: int = ...) -> List[str]: ...


def _le(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _locator(path: str) -> Callable: ...


def _ne(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _other(path: str, info: Dict[str, <class 'Union'>]) -> Dict[str, <class 'Union'>]: ...


def _pe(path: str, info: Dict[str, <class 'Union'>], part: _Parts) -> None: ...


def _preview(type_id: int) -> str: ...


def _size(n: int) -> str: ...


def _text_kind(path: str, name: str) -> <class 'Union'>[Dict[str, str], None]: ...


def _version_text(data: bytes, wide: bool) -> str: ...


def _when(stamp: int) -> str: ...


def categorize(text: str) -> str: ...


def describe(path: str) -> Dict[str, <class 'Union'>]: ...


def folder_map(folder: str, log: None = ...) -> Dict[str, <class 'Union'>]: ...


def hexdump(path: str, offset: int = ..., length: int = ...) -> str: ...


def main(argv: List[str]) -> int: ...


def markdown(info: Dict[str, <class 'Union'>], xex_header: None = ...) -> str: ...


def render(
    value: <class 'Union'>[Dict[str, <class 'Union'>], List[Dict[str, <class 'Union'>]], List[<class 'Union'>], Dict[str, int]],
    key: str = ...,
    indent: int = ...
) -> List[<class 'Union'>[Any, str]]: ...


def resource_body(
    path: str,
    offset: int,
    size: int,
    type_id: int = ...,
    wide: bool = ...,
    limit: int = ...
) -> Dict[str, str]: ...


def strings(path: str, min_len: int = ..., wide: bool = ..., limit: None = ...) -> List[<class 'Union'>]: ...


def xex_facts(path: str) -> Dict[Any, Any]: ...


class _Parts:
    def __call__(self, key: str, make: Callable, default: <class 'Union'>[Any, None] = ...) -> Any: ...
    def __init__(self, info: Dict[str, <class 'Union'>]) -> None: ...
