from typing import (
    Dict,
    Optional,
    Tuple,
)


def _place(nef: BootImageModule) -> Tuple[bytes, Tuple[int, int, int]]: ...


def open_boot_image(
    path: str,
    overlay: <class 'Union'>[str, None] = ...
) -> Dict[str, BootImageModule]: ...


def where(
    m: <class 'Union'>[codebreaker.formats.ne.BootImageModule, codebreaker.formats.ne.NEFile]
) -> <class 'Union'>[Tuple[int, int, int], Tuple[int, int, None]]: ...


class NEFile:
    def __init__(self, path: str) -> None: ...
    @classmethod
    def from_buffer(cls, data: bytes, ne_off: int, path: str = ...) -> BootImageModule: ...

from typing import (
    Dict,
    Optional,
    Tuple,
)


def _place(nef: BootImageModule) -> Tuple[bytes, Tuple[int, int, int]]: ...


def open_boot_image(
    path: str,
    overlay: <class 'Union'>[str, None] = ...
) -> Dict[str, BootImageModule]: ...


def where(
    m: <class 'Union'>[codebreaker.formats.ne.NEFile, codebreaker.formats.ne.BootImageModule]
) -> <class 'Union'>[Tuple[int, int, int], Tuple[int, int, None]]: ...


class NEFile:
    def __init__(self, path: str) -> None: ...
    @classmethod
    def from_buffer(cls, data: bytes, ne_off: int, path: str = ...) -> BootImageModule: ...

from codebreaker.formats.native_ne import (
    BootImageModule,
    NEFile,
)
from typing import (
    Dict,
    Optional,
    Tuple,
    Type,
)


def _native_class(
    cls: <class 'Union'>[Type[codebreaker.formats.ne.BootImageModule], Type[codebreaker.formats.ne.NEFile]]
) -> <class 'Union'>[Type[codebreaker.formats.native_ne.NEFile], Type[codebreaker.formats.native_ne.BootImageModule]]: ...


def _place(nef: BootImageModule) -> Tuple[bytes, Tuple[int, int, int]]: ...


def open_boot_image(
    path: str,
    overlay: <class 'Union'>[str, None] = ...
) -> Dict[str, BootImageModule]: ...


class NEFile:
    @staticmethod
    def __new__(cls: Type[NEFile], *args, **kw) -> NEFile: ...
    @classmethod
    def from_buffer(cls, data: bytes, ne_off: int, path: str = ...) -> BootImageModule: ...
