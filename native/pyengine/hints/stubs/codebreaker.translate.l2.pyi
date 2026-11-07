from codebreaker.translate.ir import (
    Node,
    Sym,
    Type,
)
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _value(v: Any) -> Any: ...


def comment(text: str) -> Dict[str, str]: ...


def fragment(
    asm: <class 'Union'>[str, None] = ...,
    name: <class 'Union'>[str, None] = ...,
    syntax: <class 'Union'>[str, None] = ...,
    origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
    reason: <class 'Union'>[str, None] = ...,
    why: None = ...,
    length: <class 'Union'>[int, None] = ...,
    within: <class 'Union'>[str, None] = ...
) -> Dict[str, <class 'Union'>[str, <class 'Union'>]]: ...


def from_json(d: Dict[str, <class 'Union'>[str, <class 'Union'>]]) -> Node: ...


def function(
    name: str,
    text: <class 'Union'>[str, None] = ...,
    origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
    comments: <class 'Union'>[None, List[str]] = ...,
    **fields
) -> Dict[str, <class 'Union'>[str, <class 'Union'>]]: ...


def module(
    units: <class 'Union'>[List[<class 'Union'>], List[Dict[str, str]], Tuple[()]] = ...,
    header: <class 'Union'>[str, None] = ...,
    origin: None = ...
) -> Dict[str, <class 'Union'>]: ...


def schema() -> Dict[str, <class 'Union'>]: ...


def sym_from(d: Dict[str, <class 'Union'>[str, <class 'Union'>]]) -> Sym: ...


def sym_json(s: Sym) -> Dict[str, <class 'Union'>[str, <class 'Union'>]]: ...


def texts(doc: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def to_json(node: Node) -> Dict[str, <class 'Union'>[str, <class 'Union'>]]: ...


def type_from(
    d: <class 'Union'>[Dict[str, str], Dict[str, <class 'Union'>], None]
) -> <class 'Union'>[codebreaker.translate.ir.Type, None]: ...


def type_json(t: Type) -> Dict[str, <class 'Union'>[str, <class 'Union'>]]: ...


def units(
    doc: Dict[str, <class 'Union'>],
    node: <class 'Union'>[str, None] = ...
) -> List[<class 'Union'>[Any, Dict[str, <class 'Union'>], Dict[str, str]]]: ...


def validate(doc: Dict[str, <class 'Union'>]) -> None: ...


class Assembly:
    def __init__(
        self,
        lines: <class 'Union'>[Tuple[()], List[str]] = ...,
        syntax: <class 'Union'>[str, None] = ...,
        module: <class 'Union'>[str, None] = ...,
        name: str = ...,
        reason: str = ...
    ) -> None: ...
    def document(self) -> Dict[str, <class 'Union'>]: ...
    def part(
        self,
        name: str,
        reason: str = ...,
        origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
        length: <class 'Union'>[int, None] = ...
    ) -> None: ...


class Made:
    def __copy__(self) -> Made: ...
    def __getstate__(self) -> Dict[str, <class 'Union'>]: ...
    def __init__(
        self,
        name: str = ...,
        header: str = ...,
        texts: <class 'Union'>[Dict[str, str], None] = ...,
        order: <class 'Union'>[None, List[str]] = ...,
        by_va: <class 'Union'>[Dict[int, str], None] = ...,
        **facts
    ) -> None: ...
    def _functions(self) -> List[Dict[str, <class 'Union'>]]: ...
    def _view(
        self,
        key: str,
        make: Callable
    ) -> <class 'Union'>[Dict[int, str], List[str], codebreaker.translate.l2._Texts, Dict[int, Tuple[str, str]]]: ...
    def add(self, name: str, text: str, va: <class 'Union'>[int, None] = ...) -> None: ...
    @property
    def by_va(self) -> Dict[int, str]: ...
    @property
    def functions(self) -> Dict[int, Tuple[str, str]]: ...
    @property
    def order(self) -> List[str]: ...


class _Texts:
    def __init__(self, made: Made) -> None: ...
    def __setitem__(self, name: str, text: str) -> None: ...

from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Union,
)


def fragment(
    asm: <class 'Union'>[str, None] = ...,
    name: <class 'Union'>[str, None] = ...,
    syntax: <class 'Union'>[str, None] = ...,
    origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
    reason: <class 'Union'>[str, None] = ...,
    why: None = ...,
    length: <class 'Union'>[int, None] = ...,
    within: None = ...
) -> Dict[str, <class 'Union'>[<class 'Union'>, str]]: ...


def function(
    name: str,
    text: <class 'Union'>[str, None] = ...,
    origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
    comments: None = ...,
    **fields
) -> Dict[str, <class 'Union'>]: ...


def module(
    units: <class 'Union'>[Tuple[()], List[<class 'Union'>]] = ...,
    header: <class 'Union'>[str, None] = ...,
    origin: None = ...
) -> Dict[str, <class 'Union'>]: ...


def texts(doc: Dict[str, <class 'Union'>]) -> Dict[str, str]: ...


def units(doc: Dict[str, <class 'Union'>], node: <class 'Union'>[str, None] = ...) -> List[Dict[str, <class 'Union'>]]: ...


class Assembly:
    def __init__(
        self,
        lines: List[str] = ...,
        syntax: <class 'Union'>[str, None] = ...,
        module: <class 'Union'>[str, None] = ...,
        name: str = ...,
        reason: str = ...
    ) -> None: ...
    def document(self) -> Dict[str, <class 'Union'>]: ...
    def part(
        self,
        name: str,
        reason: str = ...,
        origin: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...,
        length: <class 'Union'>[int, None] = ...
    ) -> None: ...


class Made:
    def __init__(
        self,
        name: str = ...,
        header: str = ...,
        texts: <class 'Union'>[Dict[str, str], None] = ...,
        order: <class 'Union'>[List[str], None] = ...,
        by_va: <class 'Union'>[Dict[int, str], None] = ...,
        **facts
    ) -> None: ...
    def _functions(self) -> List[Dict[str, <class 'Union'>]]: ...
    def _view(
        self,
        key: str,
        make: Callable
    ) -> <class 'Union'>[Dict[int, str], codebreaker.translate.l2._Texts, List[str]]: ...
    @property
    def by_va(self) -> Dict[int, str]: ...
    @property
    def order(self) -> List[str]: ...


class _Texts:
    def __init__(self, made: Made) -> None: ...
