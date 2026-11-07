from capstone import CsInsn
from codebreaker.formats.ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _body(stream: Iterator[Any], lo: int, hi: int) -> List[CsInsn]: ...


def _digest(parts: List[bytes]) -> str: ...


def _function(
    key: str,
    name: str,
    named: bool,
    body: List[CsInsn],
    lo: int,
    hi: int,
    relocated: <class 'Union'>[Dict[int, Tuple[str, str]], Set[int]],
    direct: Callable,
    site: Callable,
    slot: Callable,
    inside: Callable
) -> Function: ...


def _key16(seg: int, off: int) -> str: ...


def _load_dos(path: str) -> Side: ...


def _load_ne(path: str) -> Side: ...


def _load_pe(path: str) -> Side: ...


def _sites(
    nef: NEFile,
    seg: Dict[str, <class 'Union'>],
    segs: Dict[int, Dict[str, <class 'Union'>]],
    entries: Dict[int, Dict[str, <class 'Union'>]],
    refs: List[str]
) -> Dict[int, Tuple[str, str]]: ...


def compare(path_a: str, path_b: str, least: float = ...) -> Comparison: ...


def likeness(
    fa: Function,
    fb: Function,
    memo: None = ...
) -> float: ...


def load(path: str) -> Side: ...


def match(
    a: Side,
    b: Side,
    least: float = ...
) -> Dict[str, Tuple[str, str]]: ...


def shared(
    paths: List[str],
    level: str = ...,
    least: int = ...
) -> List[<class 'Union'>[Tuple[int, Dict[str, List[str]]], Any]]: ...


class Comparison:
    def __init__(
        self,
        a: Side,
        b: Side,
        pair: Dict[str, Tuple[str, str]],
        seconds: float
    ) -> None: ...
    def as_json(self, listings: bool = ..., fs: None = ...) -> Dict[str, <class 'Union'>]: ...
    def code(self) -> Dict[str, int]: ...
    def counts(self) -> Dict[str, int]: ...
    def names(self) -> List[Any]: ...
    def reconstructions(self, fs: None = ...) -> List[Any]: ...


class Function:
    @property
    def count(self) -> int: ...


class Side:
    def __init__(self, path: str, kind: str, code: int = ...) -> None: ...
    def add(self, f: Function) -> None: ...
    def done(self) -> Side: ...
    @property
    def size(self) -> int: ...

from capstone import CsInsn
from codebreaker.formats.ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _body(stream: Iterator[Any], lo: int, hi: int) -> List[CsInsn]: ...


def _digest(parts: List[bytes]) -> str: ...


def _function(
    key: str,
    name: str,
    named: bool,
    body: List[CsInsn],
    lo: int,
    hi: int,
    relocated: <class 'Union'>[Set[int], Dict[int, Tuple[str, str]]],
    direct: Callable,
    site: Callable,
    slot: Callable,
    inside: Callable
) -> Function: ...


def _key16(seg: int, off: int) -> str: ...


def _load_dos(path: str) -> Side: ...


def _load_ne(path: str) -> Side: ...


def _load_pe(path: str) -> Side: ...


def _sites(
    nef: NEFile,
    seg: Dict[str, <class 'Union'>],
    segs: Dict[int, Dict[str, <class 'Union'>]],
    entries: Dict[int, Dict[str, <class 'Union'>]],
    refs: List[str]
) -> Dict[int, Tuple[str, str]]: ...


def compare(path_a: str, path_b: str, least: float = ...) -> Comparison: ...


def likeness(
    fa: Function,
    fb: Function,
    memo: None = ...
) -> float: ...


def load(path: str) -> Side: ...


def match(
    a: Side,
    b: Side,
    least: float = ...
) -> Dict[str, Tuple[str, str]]: ...


def shared(
    paths: List[str],
    level: str = ...,
    least: int = ...
) -> List[<class 'Union'>[Tuple[int, Dict[str, List[str]]], Any]]: ...


class Comparison:
    def __init__(
        self,
        a: Side,
        b: Side,
        pair: Dict[str, Tuple[str, str]],
        seconds: float
    ) -> None: ...
    def as_json(self, listings: bool = ..., fs: None = ...) -> Dict[str, <class 'Union'>]: ...
    def code(self) -> Dict[str, int]: ...
    def counts(self) -> Dict[str, int]: ...
    def names(self) -> List[Any]: ...
    def reconstructions(self, fs: None = ...) -> List[Any]: ...


class Function:
    @property
    def count(self) -> int: ...


class Side:
    def __init__(self, path: str, kind: str, code: int = ...) -> None: ...
    def add(self, f: Function) -> None: ...
    def done(self) -> Side: ...
    @property
    def size(self) -> int: ...

from capstone import CsInsn
from codebreaker.formats.native_ne import NEFile
from typing import (
    Any,
    Callable,
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
    Union,
)


def _body(stream: Iterator[Any], lo: int, hi: int) -> List[CsInsn]: ...


def _digest(parts: List[bytes]) -> str: ...


def _function(
    key: str,
    name: str,
    named: bool,
    body: List[CsInsn],
    lo: int,
    hi: int,
    relocated: <class 'Union'>[Dict[int, Tuple[str, str]], Set[int]],
    direct: Callable,
    site: Callable,
    slot: Callable,
    inside: Callable
) -> Function: ...


def _key16(seg: int, off: int) -> str: ...


def _load_dos(path: str) -> Side: ...


def _load_ne(path: str) -> Side: ...


def _load_pe(path: str) -> Side: ...


def _sites(
    nef: NEFile,
    seg: Dict[str, <class 'Union'>],
    segs: Dict[int, Dict[str, <class 'Union'>]],
    entries: Dict[int, Dict[str, <class 'Union'>]],
    refs: List[str]
) -> Dict[int, Tuple[str, str]]: ...


def compare(path_a: str, path_b: str, least: float = ...) -> Comparison: ...


def likeness(
    fa: Function,
    fb: Function,
    memo: None = ...
) -> float: ...


def load(path: str) -> Side: ...


def match(
    a: Side,
    b: Side,
    least: float = ...
) -> Dict[str, Tuple[str, str]]: ...


def shared(
    paths: List[str],
    level: str = ...,
    least: int = ...
) -> List[<class 'Union'>[Tuple[int, Dict[str, List[str]]], Any]]: ...


class Comparison:
    def __init__(
        self,
        a: Side,
        b: Side,
        pair: Dict[str, Tuple[str, str]],
        seconds: float
    ) -> None: ...
    def as_json(self, listings: bool = ..., fs: None = ...) -> Dict[str, <class 'Union'>]: ...
    def code(self) -> Dict[str, int]: ...
    def counts(self) -> Dict[str, int]: ...
    def names(self) -> List[Any]: ...
    def reconstructions(self, fs: None = ...) -> List[Any]: ...


class Function:
    @property
    def count(self) -> int: ...


class Side:
    def __init__(self, path: str, kind: str, code: int = ...) -> None: ...
    def add(self, f: Function) -> None: ...
    def done(self) -> Side: ...
    @property
    def size(self) -> int: ...
