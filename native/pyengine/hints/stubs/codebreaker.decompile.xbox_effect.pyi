from codebreaker.formats.xbox.effect import (
    Effect,
    Parameter,
    State,
)
from typing import List


def _array_selection(
    fx: Effect,
    state: State
) -> <class 'Union'>[Tuple[List[int], str], None]: ...


def _literal(p: Parameter, values: List[<class 'Union'>[int, float]]) -> str: ...


def _string_annotations(fx: Effect, p: Parameter) -> str: ...


def _texture_of(fx: Effect, state: State) -> str: ...


def _type_name(p: Parameter) -> str: ...


def to_fx(data: bytes, name: str = ...) -> str: ...

from codebreaker.formats.xbox.effect import (
    Effect,
    Parameter,
    State,
)
from typing import List


def _array_selection(fx: Effect, state: State) -> None: ...


def _literal(p: Parameter, values: List[<class 'Union'>[int, float]]) -> str: ...


def _string_annotations(fx: Effect, p: Parameter) -> str: ...


def _texture_of(fx: Effect, state: State) -> str: ...


def _type_name(p: Parameter) -> str: ...


def to_fx(data: bytes, name: str = ...) -> str: ...
