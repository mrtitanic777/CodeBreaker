from codebreaker.emulate.run import Run
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _call(hit: Dict[str, <class 'Union'>], inputs: List[int], entry: Dict[str, int], segs: None = ...) -> str: ...


def _effects(r: Run, call: Callable) -> Tuple[str, Ellipsis]: ...


def _scattered(n: int, sel: int) -> List[Dict[str, List[int]]]: ...


def concolic_ready() -> bool: ...


def fingerprint(
    path: str,
    seg: int,
    off: int,
    near: bool,
    wide: bool,
    v: Dict[str, List[int]],
    data_image: None = ...,
    span: None = ...,
    own: <class 'Union'>[Tuple[Tuple[int, int]], Tuple[()]] = ...,
    frame: None = ...,
    ran: <class 'Union'>[Run, None] = ...,
    segs: None = ...,
    steps: None = ...
) -> Dict[str, <class 'Union'>]: ...


def profile(module_path: str) -> Tuple[str, str]: ...


def vectors() -> List[Dict[str, List[int]]]: ...
