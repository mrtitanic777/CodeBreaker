from codebreaker.emulate.run import (
    Call,
    Run,
)
from codebreaker.verify.concolic import Exploration
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Type,
    Union,
)


def alternating(n: int, even: <class 'Union'>[int, Callable], odd: <class 'Union'>[int, Callable]) -> List[int]: ...


def argument(
    v: int,
    inputs: List[int] = ...,
    above: <class 'Union'>[int, None] = ...,
    small: bool = ...,
    frame: Tuple[()] = ...,
    image: None = ...
) -> str: ...


def call(name: str, args: <class 'Union'>[List[str], None] = ...) -> str: ...


def calls(
    seq: List[Call],
    argument: Type[str],
    name: Type[str] = ...,
    skip: None = ...,
    cap: None = ...
) -> Tuple[str]: ...


def changed(
    run: Run,
    place: <class 'Union'>[None, Callable] = ...,
    word: <class 'Union'>[None, Callable] = ...,
    most: int = ...,
    everything: bool = ...
) -> <class 'Union'>[Dict[Tuple[str, int], int], Dict[int, <class 'Union'>], Dict[int, int]]: ...


def concolic_on() -> bool: ...


def counting(n: int, first: int = ..., step: int = ...) -> List[int]: ...


def descending(n: int, top: int) -> List[int]: ...


def explored(
    run: Callable,
    seeds: List[<class 'Union'>],
    derive: Callable,
    own: Callable,
    known: <class 'Union'>[Dict[int, List[Tuple[int, bool]]], None] = ...,
    steps: None = ...,
    more: int = ...
) -> Exploration: ...


def fingerprint(
    run: Run,
    deps: Tuple[str, Ellipsis],
    ret: <class 'Union'>[int, None] = ...,
    writes: str = ...,
    saved: <class 'Union'>[bool, None] = ...,
    cov: Tuple[()] = ...,
    reach: Tuple[()] = ...,
    **more
) -> Dict[str, <class 'Union'>]: ...


def guided_on() -> bool: ...


def left(
    run: Run,
    place: <class 'Union'>[None, Callable] = ...,
    word: <class 'Union'>[None, Callable] = ...,
    most: int = ...,
    everything: bool = ...
) -> <class 'Union'>[str, None]: ...


def writes(state: Dict[Tuple[str, int], int]) -> str: ...
