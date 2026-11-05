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
