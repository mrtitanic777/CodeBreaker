from typing import (
    Any,
    Callable,
    List,
    Optional,
)


def apply_overlays(
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    overlay_dir: str,
    prog: <class 'Union'>[Callable, None] = ...
) -> List[Any]: ...


def overlays_root() -> str: ...


def project_key(
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    java_dir: <class 'Union'>[str, pathlib.WindowsPath, None] = ...
) -> str: ...


def resolve_overlay_dir(
    out_dir: <class 'Union'>[str, pathlib.WindowsPath],
    java_dir: <class 'Union'>[str, pathlib.WindowsPath, None] = ...,
    explicit: None = ...
) -> str: ...
