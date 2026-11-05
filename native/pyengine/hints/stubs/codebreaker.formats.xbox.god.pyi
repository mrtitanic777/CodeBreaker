from io import BufferedReader
from typing import (
    Callable,
    Tuple,
)


def _con_header(
    template: bytes,
    *,
    title_id,
    media_id,
    content_type,
    blocks_allocated,
    part_count,
    parts_total_size,
    mht_hash,
    title = ...
) -> bytes: ...


def _phys_offset_in_part(block_in_part: int) -> int: ...


def install_to_god(data: bytes) -> bytes: ...


def is_nxe_install(data: bytes) -> bool: ...


def write_god(
    volume_read: Callable,
    volume_size: int,
    out_dir: str,
    template: bytes,
    *,
    title_id,
    media_id,
    content_type = ...,
    title = ...
) -> Tuple[str, str, int]: ...


class SvodReader:
    def __init__(self, data_dir: str) -> None: ...
    def _f(self, idx: int) -> BufferedReader: ...
    def read_at(self, offset: int, length: int) -> bytes: ...
