from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import PEFile
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
    Union,
)


def decode(
    resource: Resource,
    wide: <class 'Union'>[bool, int]
) -> Tuple[Tuple[int, int, int, int, int, int, int, int, int, int, int], Dict[str, List[Tuple[str, str]]], List[int]]: ...


def icon_file(group_data: bytes, member_data: Callable) -> bytes: ...


def read(path: str) -> List[Resource]: ...


def read_ne(nef: NEFile) -> List[Resource]: ...


def read_pe(pe: PEFile) -> List[Resource]: ...


def same_data(a: bytes, b: bytes) -> bool: ...


def type_library_outline(library: Dict[str, <class 'Union'>]) -> str: ...


def type_name(t: int) -> str: ...


class Resource:
    def __init__(
        self,
        type: <class 'Union'>[int, str],
        name: <class 'Union'>[int, str],
        language: <class 'Union'>[int, None],
        flags: int,
        data: bytes
    ) -> None: ...
    def __repr__(self) -> str: ...
    @property
    def key(self) -> <class 'Union'>[Tuple[int, int, int], Tuple[int, str, None], Tuple[int, int, None]]: ...

from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import PEFile
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
    Union,
)


def decode(
    resource: Resource,
    wide: <class 'Union'>[bool, int]
) -> Tuple[Tuple[int, int, int, int, int, int, int, int, int, int, int], Dict[str, List[Tuple[str, str]]], List[int]]: ...


def icon_file(group_data: bytes, member_data: Callable) -> bytes: ...


def read(path: str) -> List[Resource]: ...


def read_ne(nef: NEFile) -> List[Resource]: ...


def read_pe(pe: PEFile) -> List[Resource]: ...


def same_data(a: bytes, b: bytes) -> bool: ...


def type_library_outline(library: Dict[str, <class 'Union'>]) -> str: ...


def type_name(t: int) -> str: ...


class Resource:
    def __init__(
        self,
        type: <class 'Union'>[int, str],
        name: <class 'Union'>[int, str],
        language: <class 'Union'>[int, None],
        flags: int,
        data: bytes
    ) -> None: ...
    @property
    def key(self) -> <class 'Union'>[Tuple[int, int, int], Tuple[int, str, None], Tuple[int, int, None]]: ...

from codebreaker.formats.native_ne import NEFile
from codebreaker.formats.native_pe import PEFile
from typing import (
    Dict,
    List,
    Tuple,
)


def read(path: str) -> List[Resource]: ...


def read_ne(nef: NEFile) -> List[Resource]: ...


def read_pe(pe: PEFile) -> List[Resource]: ...


def type_name(t: int) -> str: ...


def version(
    data: bytes,
    wide: bool
) -> Tuple[Tuple[int, int, int, int, int, int, int, int, int, int, int, int, int], Dict[str, List[Tuple[str, str]]], List[int]]: ...


class Resource:
    def __init__(self, type: int, name: int, language: <class 'Union'>[int, None], flags: int, data: bytes) -> None: ...
    def __repr__(self) -> str: ...


class _Reader:
    def __init__(self, data: bytes, wide: bool, at: int = ...) -> None: ...
    def align(self, n: int = ...) -> None: ...
    def text(self) -> str: ...

from codebreaker.formats.ne import NEFile
from typing import (
    List,
    Tuple,
)


def read(path: str) -> List[<class 'Union'>[codebreaker.formats.resources.Resource, Any]]: ...


def read_ne(nef: NEFile) -> List[<class 'Union'>[codebreaker.formats.resources.Resource, Any]]: ...


def same_data(a: bytes, b: bytes) -> bool: ...


def type_name(t: int) -> str: ...


class Resource:
    def __init__(self, type: int, name: int, language: None, flags: int, data: bytes) -> None: ...
    @property
    def key(self) -> Tuple[int, int, None]: ...
