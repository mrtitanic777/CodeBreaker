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

from codebreaker.translate.l2 import Made
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _call(hit: Dict[str, <class 'Union'>], inputs: List[Any], entry: Dict[str, int], segs: None = ...) -> str: ...


def _derive(
    v: Dict[str, <class 'Union'>[<class 'Union'>, List[int]]],
    values: Dict[str, int],
    names: Dict[str, <class 'Union'>[<class 'Union'>, Tuple[str, int, int, int]]],
    ds: int
) -> Dict[str, <class 'Union'>[<class 'Union'>, List[int]]]: ...


def _from_explorer(
    g: Dict[str, <class 'Union'>[<class 'Union'>, List[int], List[<class 'Union'>]]],
    ds: int
) -> Dict[str, <class 'Union'>[<class 'Union'>, List[int]]]: ...


def _jumps(
    module_path: str,
    seg: int
) -> <class 'Union'>[Tuple[bytes, Dict[int, None]], Tuple[bytes, Dict[Any, Any]], Tuple[bytes, Dict[int, <class 'Union'>]]]: ...


def _scattered(n: int, sel: int) -> List[Dict[str, List[int]]]: ...


def _to_explorer(
    v: Dict[str, <class 'Union'>[<class 'Union'>, List[int]]],
    ds: int
) -> Dict[str, <class 'Union'>[<class 'Union'>, List[int], List[<class 'Union'>]]]: ...


def concolic_ready() -> bool: ...


def directed(
    module_path: str,
    seg: int,
    span: Tuple[int, int],
    most: int = ...
) -> List[<class 'Union'>[Dict[str, List[int]], Any]]: ...


def explore_vectors(
    module_path: str,
    seg: int,
    off: int,
    near: bool,
    span: Tuple[int, int],
    seeds: List[Dict[str, List[int]]],
    known: <class 'Union'>[Dict[int, Any], None] = ...,
    **probe
) -> <class 'Union'>[Tuple[List[Dict[str, <class 'Union'>]], codebreaker.verify.concolic.Exploration, List[<class 'Union'>]], Tuple[List[Any], codebreaker.verify.concolic.Exploration, List[Tuple[Dict[str, List[int]], Dict[str, <class 'Union'>]]]], Tuple[List[Dict[str, List[int]]], codebreaker.verify.concolic.Exploration, List[<class 'Union'>]], Tuple[List[<class 'Union'>], codebreaker.verify.concolic.Exploration, List[<class 'Union'>]], Tuple[List[Any], codebreaker.verify.concolic.Exploration, List[<class 'Union'>]]]: ...


def key(seg: int, off: int) -> str: ...


def prepare(module_path: str, work: str) -> Made: ...


def trail(
    module_path: str,
    seg: int,
    span: Tuple[int, int],
    path: List[int]
) -> List[<class 'Union'>[Any, Tuple[int, bool]]]: ...


def vectors() -> List[Dict[str, List[int]]]: ...
