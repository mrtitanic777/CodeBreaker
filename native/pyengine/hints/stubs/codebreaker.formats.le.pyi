from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _file_args(path: str) -> Tuple[bytes, Tuple[str]]: ...


def _image_args(lf: LEFile, *rest) -> Tuple[bytes, Tuple[int]]: ...


def _object_of(lf: LEFile, answer: Tuple[int, int]) -> Tuple[Dict[str, <class 'Union'>], int]: ...


def _opened(data: bytes, pairs: List[Tuple[str, int]]) -> List[Tuple[str, LEFile]]: ...


def _vxds_result(
    answer: List[Tuple[str, int]],
    data: bytes,
    path: str
) -> List[Tuple[str, LEFile]]: ...


def _where(lf: LEFile) -> Tuple[int]: ...


class LEFile:
    def __init__(s, data: bytes, lfa: int) -> None: ...
    @classmethod
    def from_buffer(cls, data: bytes) -> LEFile: ...


class W3File:
    def __init__(s, data: bytes, lfa: int) -> None: ...

from codebreaker.formats.native_le import LEFile
from typing import (
    List,
    Tuple,
    Type,
)


def _file_args(path: str) -> Tuple[bytes, Tuple[str]]: ...


def _image_args(
    lf: LEFile,
    *rest
) -> <class 'Union'>[Tuple[bytes, Tuple[int]], Tuple[bytes, Tuple[int, Dict[str, <class 'Union'>]]]]: ...


def _members_result(
    answer: Tuple[None, List[Tuple[str, int]]],
    data: bytes,
    path: str
) -> List[Tuple[str, LEFile]]: ...


def _native_class(
    cls: <class 'Union'>[Type[codebreaker.formats.native_le.LEFile], Type[codebreaker.formats.le.LEFile]]
) -> <class 'Union'>[Type[codebreaker.formats.native_le.LEFile], None]: ...


def _vxds_result(
    answer: List[Tuple[str, int]],
    data: bytes,
    path: str
) -> List[Tuple[str, LEFile]]: ...


class LEFile:
    @staticmethod
    def __new__(cls: Type[LEFile], *args, **kw) -> LEFile: ...
    @classmethod
    def from_buffer(cls, data: bytes) -> LEFile: ...
    @classmethod
    def from_file(cls, path: str) -> LEFile: ...
