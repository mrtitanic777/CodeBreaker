from typing import (
    Any,
    Dict,
    List,
    Optional,
)


def _headers_key(folder: str) -> str: ...


def code_key(kind: str) -> str: ...


def pass_key(cc: str, folder: str, f: str, kind: str, *extra, headers = ...) -> str: ...


def reach_key(folder: str, f: str) -> str: ...


def run(cc: str, folder: str, files: List[str]) -> Dict[Any, Any]: ...
