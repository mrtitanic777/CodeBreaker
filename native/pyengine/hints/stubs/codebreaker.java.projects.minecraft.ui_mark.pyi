from re import Pattern
from typing import (
    Dict,
    Optional,
    Set,
    Union,
)


def _def_regex(cls: str) -> Pattern: ...


def discover_ui_classes(src_dir: str, extra: None = ...) -> Set[str]: ...


def is_ui_class_name(name: str) -> bool: ...


def mark(src_dir: str, dry_run: bool = ..., extra_classes: None = ...) -> Dict[str, <class 'Union'>]: ...


def render_index(result: Dict[str, <class 'Union'>]) -> str: ...


def run(
    src_dir: str,
    dry_run: bool = ...,
    extra_classes: None = ...,
    index_dir: None = ...
) -> Dict[str, <class 'Union'>]: ...

from re import Pattern
from typing import (
    Dict,
    Optional,
    Set,
    Union,
)


def _def_regex(cls: str) -> Pattern: ...


def discover_ui_classes(src_dir: str, extra: None = ...) -> Set[str]: ...


def is_ui_class_name(name: str) -> bool: ...


def mark(src_dir: str, dry_run: bool = ..., extra_classes: None = ...) -> Dict[str, <class 'Union'>]: ...


def render_index(result: Dict[str, <class 'Union'>]) -> str: ...


def run(
    src_dir: str,
    dry_run: bool = ...,
    extra_classes: None = ...,
    index_dir: None = ...
) -> Dict[str, <class 'Union'>]: ...
