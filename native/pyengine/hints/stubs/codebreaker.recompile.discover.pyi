from codebreaker.formats.powerpc_image import Image
from typing import (
    Dict,
    Optional,
    Set,
    Tuple,
)


def _image(path: str) -> Image: ...


def _plausible_start(code: bytes, lo: int, a: int) -> bool: ...


def discover(
    path: str,
    pointer_scan: bool = ...,
    extra: None = ...
) -> Tuple[Dict[int, int], Dict[str, int], Image]: ...


def from_constants(img: Image, known: Dict[int, int]) -> Set[int]: ...


def from_pointers(img: Image, known: Dict[int, int]) -> Set[int]: ...

from codebreaker.formats.powerpc_image import Image
from typing import (
    Dict,
    Optional,
    Set,
    Tuple,
)


def _image(path: str) -> Image: ...


def _plausible_start(code: bytes, lo: int, a: int) -> bool: ...


def discover(
    path: str,
    pointer_scan: bool = ...,
    extra: None = ...
) -> Tuple[Dict[int, int], Dict[str, int], Image]: ...


def from_constants(img: Image, known: Dict[int, int]) -> Set[int]: ...


def from_pointers(img: Image, known: Dict[int, int]) -> Set[int]: ...
