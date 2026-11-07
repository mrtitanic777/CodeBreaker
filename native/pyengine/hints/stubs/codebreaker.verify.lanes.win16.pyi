from codebreaker.emulate.run import Run
from codebreaker.formats.ne import NEFile
from codebreaker.translate.l2 import Made
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _call(
    hit: Dict[str, <class 'Union'>],
    inputs: List[<class 'Union'>[Any, int]],
    entry: Dict[str, int],
    segs: None = ...
) -> str: ...


def _effects(r: Run, call: Callable) -> Tuple[str, Ellipsis]: ...


def _scattered(n: int, sel: int) -> List[Dict[str, List[int]]]: ...


def _spans(
    nef: NEFile,
    where: Dict[int, Tuple[int, int]]
) -> Dict[int, Tuple[Tuple[int, int], int]]: ...


def concolic_ready() -> bool: ...


def directed(
    module_path: str,
    seg: int,
    span: Tuple[int, int],
    most: int = ...
) -> List[<class 'Union'>[Dict[str, List[int]], Any]]: ...


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


def key(seg: int, off: int) -> str: ...


def prepare(module_path: str, work: str) -> Made: ...


def profile(module_path: str) -> Tuple[str, str]: ...


def vectors() -> List[Dict[str, List[int]]]: ...

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
