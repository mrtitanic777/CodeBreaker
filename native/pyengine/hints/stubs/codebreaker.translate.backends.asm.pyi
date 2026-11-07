from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def comment(text: str, syntax: None = ...) -> str: ...


def lines(doc: Dict[str, <class 'Union'>]) -> Tuple[str, Dict[int, int]]: ...


def module(doc: Dict[str, <class 'Union'>]) -> str: ...


def unit(
    u: Dict[str, <class 'Union'>[str, <class 'Union'>]],
    syntax: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[str, None]: ...

from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def lines(doc: Dict[str, <class 'Union'>]) -> Tuple[str, Dict[int, int]]: ...


def module(doc: Dict[str, <class 'Union'>]) -> str: ...


def unit(u: Dict[str, <class 'Union'>[<class 'Union'>, str]], syntax: <class 'Union'>[str, None] = ...) -> str: ...
