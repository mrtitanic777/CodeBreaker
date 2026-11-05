from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _parse_v1(d: bytes) -> Dict[str, <class 'Union'>]: ...


def _safe_rel(name: str) -> str: ...


def build(folder: str, out_path: str, template: bytes) -> str: ...


def parse(path: str) -> Dict[str, <class 'Union'>]: ...


def payloads(path: str) -> List[Tuple[str, bytes]]: ...
