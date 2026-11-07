from PIL.Image import Image
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _get_assets(r: R, L: Dict[str, <class 'Union'>], layerStart: int) -> None: ...


def _layer(r: R, sect_off: int) -> Dict[str, <class 'Union'>]: ...


def _psl(r: R) -> Dict[str, <class 'Union'>]: ...


def all_assets(P: Dict[str, <class 'Union'>]) -> List[Dict[str, int]]: ...


def asset_bytes(data: bytes, a: Dict[str, int]) -> bytes: ...


def find_asset(
    P: Dict[str, <class 'Union'>],
    assetID: int,
    layer_idx: <class 'Union'>[int, None] = ...
) -> <class 'Union'>[Dict[str, int], None]: ...


def find_assets(
    P: Dict[str, <class 'Union'>],
    assetID: int,
    layer_idx: <class 'Union'>[int, None] = ...
) -> List[<class 'Union'>[Dict[str, int], Any]]: ...


def parse(data: bytes) -> Dict[str, <class 'Union'>]: ...


def texture(
    data: bytes,
    P: Dict[str, <class 'Union'>],
    descriptor_id: int,
    layer_idx: None = ...,
    force_opaque: bool = ...
) -> Tuple[Image, int, int, int]: ...


def texture_descriptor(desc: bytes) -> Tuple[int, int, int, int]: ...


class R:
    def __init__(self, d: bytes) -> None: ...
    def i32(self) -> int: ...
    def raw(self, n: int) -> bytes: ...
    def u32(self) -> int: ...
    def u64(self) -> int: ...
