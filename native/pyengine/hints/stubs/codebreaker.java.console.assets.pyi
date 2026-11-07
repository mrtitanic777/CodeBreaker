from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _png_colortype(raw: bytes) -> <class 'Union'>[int, None]: ...


def _strip_comments(text: str) -> str: ...


def code_resource_refs(code_dir: str) -> Dict[str, List[str]]: ...


def coverage(jar_path: str, code_dir: str) -> Dict[str, <class 'Union'>]: ...


def extract(
    jar_path: str,
    content_dir: str,
    only: None = ...,
    normalize_png: bool = ...,
    converted: None = ...
) -> List[str]: ...


def list_console_assets() -> List[str]: ...


def list_jar_assets(jar_path: str) -> List[str]: ...


def normalize_png_to_rgba(raw: bytes) -> Tuple[bytes, bool]: ...


def png_needs_rgba(raw: bytes) -> bool: ...
