from PIL.Image import Image
from typing import (
    List,
    Tuple,
)


def _blocks(w: int, h: int) -> Tuple[int, int]: ...


def _color_block(b: bytes, dxt1: bool) -> List[Tuple[int, int, int]]: ...


def _rgb565(c: int) -> Tuple[int, int, int]: ...


def dds_header(w: int, h: int, fmt: str) -> bytes: ...


def decode_dxt(
    data: bytes,
    w: int,
    h: int,
    fmt: str,
    tiled: bool = ...,
    swapped: bool = ...,
    force_opaque: bool = ...
) -> Image: ...


def tile(data: bytes, w: int, h: int, fmt: str) -> bytes: ...


def tiled_offset_2d(x: int, y: int, width_blocks: int, log2bpp: int) -> int: ...


def to_dds(linear: bytes, w: int, h: int, fmt: str) -> bytes: ...


def untile(data: bytes, w: int, h: int, fmt: str) -> bytes: ...
