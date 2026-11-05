from typing import (
    Any,
    Dict,
    List,
    Tuple,
)
from unicorn.unicorn_py3.arch.intel import UcIntel


def unpacked(
    module: bytes,
    relocations: List[Any],
    cs: int,
    ip: int,
    ss: int,
    sp: int,
    min_alloc: int = ...,
    limit: int = ...,
    moved: bool = ...
) -> Tuple[bytes, List[Any], int, int, int, int]: ...


class _Run:
    def __init__(
        self,
        module: bytes,
        relocations: List[Any],
        cs: int,
        ip: int,
        ss: int,
        sp: int,
        load: int,
        limit: int,
        moved: bool = ...
    ) -> None: ...
    def _block(self, uc: UcIntel, address: int, size: int, _user: None) -> None: ...
    def _state(self, address: int) -> Dict[str, int]: ...
    def _write(
        self,
        uc: UcIntel,
        _access: int,
        address: int,
        size: int,
        _value: int,
        _user: None
    ) -> None: ...
    def image(self) -> bytes: ...
