from codebreaker.build.module import Built
from codebreaker.engine.native_engine import (
    Context,
    Database,
)
from codebreaker.platforms import Platform
from codebreaker.verify.lanes.java import Prepared
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _build_stamp(p: Platform) -> Callable: ...


def _built(
    p: Platform,
    path: str,
    m: Prepared,
    work: str
) -> Dict[str, <class 'Union'>]: ...


def _code() -> str: ...


def _declare(p: Platform) -> str: ...


def _file(module: str) -> str: ...


def _image_stamp() -> Tuple[str, List[Tuple[str, str]], List[str]]: ...


def _keepable(b: Built) -> Built: ...


def _mark(work: str) -> str: ...


def _module_stamp(p: Platform) -> Callable: ...


def _patched(
    ctx: Context,
    L: str
) -> <class 'Union'>[codebreaker.verify.lanes.java.Prepared, codebreaker.translate.l2.Made]: ...


def _switches() -> List[Tuple[str, str]]: ...


def _tools(p: Platform) -> List[Tuple[str, str]]: ...


def _work(ctx: Context) -> str: ...


def build(
    path: str,
    outdir: <class 'Union'>[str, None] = ...,
    call: bool = ...,
    log: Callable = ...
) -> Built: ...


def judged(path: str, log: None = ...) -> Judged: ...


def on(path: str) -> bool: ...


def open_module(
    path: str,
    p: <class 'Union'>[Platform, None] = ...
) -> Database: ...


def pe_image(ctx: Context, key: str) -> Dict[str, <class 'Union'>]: ...


class Judged:
    def __init__(self, got: Dict[str, <class 'Union'>], made: Dict[str, List[int]]) -> None: ...

from codebreaker.platforms import Platform
from typing import Callable


def _build_stamp(p: Platform) -> Callable: ...


def _declare(p: Platform) -> str: ...


def _module_stamp(p: Platform) -> Callable: ...
