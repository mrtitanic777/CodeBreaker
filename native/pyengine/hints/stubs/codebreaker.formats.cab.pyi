from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _by_path(path: str, *rest) -> Tuple[None, Tuple[str]]: ...


def _cabinet(
    answer: Tuple[Dict[str, <class 'Union'>], List[Tuple[str, int, int, int, int, int, int, str]], List[Tuple[int, int, int]], int],
    _data: None,
    path: str
) -> Cabinet: ...


class Cabinet:
    def __init__(
        self,
        path: str,
        info: Dict[str, <class 'Union'>],
        files: List[Entry],
        folders: List[Tuple[int, int, int]],
        block_reserve: int
    ) -> None: ...

from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _by_path(path: str, *rest) -> Tuple[None, Tuple[str]]: ...


def _cabinet(
    answer: Tuple[Dict[str, <class 'Union'>], List[Tuple[str, int, int, int, int, int, int, str]], List[Tuple[int, int, int]], int],
    _data: None,
    path: str
) -> Cabinet: ...


class Cabinet:
    def __init__(
        self,
        path: str,
        info: Dict[str, <class 'Union'>],
        files: List[Entry],
        folders: List[Tuple[int, int, int]],
        block_reserve: int
    ) -> None: ...

from typing import (
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _blocks(cab: Cabinet, k: int) -> Iterator[Tuple[bytes, int]]: ...


def _chain_text(neighbour: <class 'Union'>[Tuple[str, str], str, None]) -> bytes: ...


def _folder_start(cab: Cabinet, k: int) -> Tuple[Cabinet, int]: ...


def _mszip_blocks(stream: bytes, short_blocks: bool) -> List[Tuple[bytes, int]]: ...


def _safe_path(outdir: str, name: str) -> str: ...


def _text(data: bytes, at: int) -> Tuple[bytes, int]: ...


def _timestamp(entry: Entry) -> float: ...


def _unpack_mszip(_kind: int) -> Callable: ...


def _unpack_quantum(kind: int) -> Callable: ...


def _unpack_stored(_kind: int) -> Callable: ...


def checksum(payload: bytes, uncompressed: int) -> int: ...


def extract(path: str, outdir: str, names: <class 'Union'>[None, List[str]] = ...) -> Dict[str, str]: ...


def folder_bytes(cab: Cabinet, k: int, upto: <class 'Union'>[int, None] = ...) -> bytearray: ...


def read(path: str) -> Cabinet: ...


def write(
    files: List[Tuple[str, bytes]],
    out_path: str,
    compress: str = ...,
    set_id: int = ...,
    index: int = ...,
    prev: <class 'Union'>[str, None] = ...,
    next: <class 'Union'>[Tuple[str, str], None] = ...,
    short_blocks: bool = ...
) -> int: ...


class Cabinet:
    def __init__(
        self,
        path: str,
        info: Dict[str, <class 'Union'>],
        files: List[Entry],
        folders: List[Tuple[int, int, int]],
        block_reserve: int
    ) -> None: ...
    def folder_of(self, entry: Entry) -> int: ...
