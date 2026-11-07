from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
)


def _read(path: str) -> str: ...


def build(
    m: Prepared,
    module_path: str,
    work: str
) -> Tuple[Tuple[str, str, str, str, str, str, str, Dict[str, Tuple[str, str]]], Dict[str, Tuple[str, str, str]], Dict[Any, Any]]: ...


def check(
    cxx: str,
    include_dir: str,
    unit: str,
    prefix: None = ...
) -> <class 'Union'>[Tuple[bool, List[Any]], Tuple[bool, List[str]]]: ...


def compiler() -> str: ...


def is_module(path: str) -> bool: ...


def jdk() -> Tuple[str, str]: ...


def judge(
    context: Tuple[str, str, None, str, str, str, str, Dict[Any, Any]],
    item: Tuple[str, str, str]
) -> Tuple[str, str, List[Any], str]: ...


def precompile(cxx: str, include_dir: str, work: str) -> str: ...


def prepare(module_path: str, work: str) -> Prepared: ...


def profile(module_path: <class 'Union'>[str, None] = ...) -> Tuple[str, str]: ...


def sha(path: str) -> str: ...


def source_dir(path: str) -> str: ...

from typing import (
    Optional,
    Tuple,
)


def compiler() -> str: ...


def jdk() -> Tuple[str, str]: ...


def profile(module_path: <class 'Union'>[str, None] = ...) -> Tuple[str, str]: ...
