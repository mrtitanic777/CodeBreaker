from codebreaker.analyze.dos_functions import Interrupt
from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def annotate(text: str, interrupts: List[Interrupt]) -> str: ...


def describe(interrupt: Interrupt) -> str: ...


def name(
    number: int,
    ah: <class 'Union'>[int, None] = ...,
    al: <class 'Union'>[int, None] = ...
) -> Service: ...


def table() -> Dict[int, <class 'Union'>]: ...


class Service:
    def __init__(self, name: str, arguments: str, exact: bool) -> None: ...

from codebreaker.analyze.dos_functions import Interrupt
from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def annotate(text: str, interrupts: List[<class 'Union'>[Any, codebreaker.analyze.dos_functions.Interrupt]]) -> str: ...


def describe(interrupt: Interrupt) -> str: ...


def name(
    number: int,
    ah: <class 'Union'>[int, None] = ...,
    al: <class 'Union'>[int, None] = ...
) -> Service: ...


def table() -> Dict[int, <class 'Union'>]: ...


class Service:
    def __init__(self, name: str, arguments: str, exact: bool) -> None: ...
