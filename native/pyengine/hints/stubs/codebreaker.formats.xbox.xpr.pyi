from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def _selfcheck() -> Tuple[bool, str]: ...


def build_xpr0(pixels: bytes, width: int, height: int, xfmt: int = ..., mipmaps: int = ...) -> bytes: ...


def to_dds(info: Dict[str, <class 'Union'>], width: None = ..., height: None = ...) -> bytes: ...

from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def _decode_format_dword(f: int) -> Dict[str, <class 'Union'>]: ...


def _selfcheck() -> Tuple[bool, str]: ...


def build_xpr0(pixels: bytes, width: int, height: int, xfmt: int = ..., mipmaps: int = ...) -> bytes: ...


def parse(data: bytes) -> Dict[str, <class 'Union'>]: ...


def parse_xpr0(data: bytes) -> Dict[str, <class 'Union'>]: ...


def parse_xpr2(data: bytes) -> Dict[str, <class 'Union'>]: ...


def to_dds(info: Dict[str, <class 'Union'>], width: None = ..., height: None = ...) -> bytes: ...
