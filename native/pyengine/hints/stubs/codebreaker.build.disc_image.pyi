from typing import (
    List,
    Optional,
    Tuple,
)


def _boot_catalog(image_lba: int, image_size: int) -> bytes: ...


def _both16(v: int) -> bytes: ...


def _both32(v: int) -> bytes: ...


def _clean(text: str, limit: int) -> str: ...


def _descriptor(kind: int, body: bytes = ...) -> bytes: ...


def _directory(node: _Node) -> bytes: ...


def _numbered(root: _Node) -> List[_Node]: ...


def _path_table(folders: List[_Node], big_endian: bool) -> bytes: ...


def _record(ident: bytes, lba: int, size: int, is_dir: bool, stamp: bytes) -> bytes: ...


def _scan(
    folder: str
) -> <class 'Union'>[Tuple[codebreaker.build.disc_image._Node, List[Any]], Tuple[codebreaker.build.disc_image._Node, List[str]]]: ...


def _sectors(size: int) -> int: ...


def level1_name(name: str, is_dir: bool) -> Tuple[str, str]: ...


def write(folder: str, out_path: str, volume_id: str = ..., boot_image: <class 'Union'>[str, None] = ...) -> int: ...


class _BootFile:
    def __init__(self, source: str, lba: int, size: int) -> None: ...


class _Node:
    def __init__(
        self,
        source: str,
        key: Tuple[str, str],
        is_dir: bool,
        parent: <class 'Union'>[codebreaker.build.disc_image._Node, None]
    ) -> None: ...
