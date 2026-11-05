from codebreaker.formats.media import Entry
from codebreaker.formats.xbox.fatx import (
    FatxEntry,
    FatxPartition,
    FatxReader,
)
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
)


def _fatx(path: str) -> <class 'Union'>[None, List[codebreaker.formats.xbox.volumes.FatxVolume]]: ...


def _god(folder: str, header: None = ...) -> List[_Listed]: ...


def _nand(path: str) -> List[_Listed]: ...


def _payloads(kind: str, path: str, found: List[Tuple[str, bytes]]) -> List[_Listed]: ...


def _stfs(path: str) -> List[_Listed]: ...


def _xdvdfs(
    device: <class 'Union'>[codebreaker.formats.xbox.gdf._FileDevice, codebreaker.formats.xbox.god.SvodReader],
    label: str,
    where: str,
    kind: str = ...,
    base: None = ...,
    descriptor: None = ...
) -> _Listed: ...


def volumes(
    path: str,
    long_names: bool = ...
) -> <class 'Union'>[List[codebreaker.formats.xbox.volumes._Listed], None, List[codebreaker.formats.xbox.volumes.FatxVolume]]: ...


class FatxVolume:
    def __init__(
        self,
        reader: FatxReader,
        partition: FatxPartition
    ) -> None: ...
    def _all(self) -> Dict[str, FatxEntry]: ...
    def files(self) -> List[Entry]: ...
    def read(self, entry: Entry) -> bytes: ...


class _Listed:
    def __init__(
        self,
        kind: str,
        label: str,
        where: str,
        boot: Dict[str, <class 'Union'>[str, int]],
        listing: Callable,
        reader: Callable
    ) -> None: ...
    def files(self) -> List[Entry]: ...
    def read(self, entry: Entry) -> bytes: ...
