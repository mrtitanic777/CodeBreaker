from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _annotations(r: _Reader, ptr: int, count: int) -> Tuple[List[Any], int]: ...


def _copy(p: Parameter) -> Parameter: ...


def _state(r: _Reader, ptr: int) -> Tuple[State, int]: ...


def _typedef(
    r: _Reader,
    at: int,
    parent: <class 'Union'>[Parameter, None] = ...
) -> Tuple[Parameter, int]: ...


def _value(
    r: _Reader,
    p: Parameter,
    at: int,
    ptr: int,
    numbers: bool
) -> Tuple[int, int]: ...


def read(data: bytes) -> Effect: ...


class Effect:
    def __init__(self) -> None: ...
    def parameter(self, name: str) -> Parameter: ...


class Parameter:
    def __init__(self) -> None: ...


class Pass:
    def __init__(self, name: str, annotations: List[Any], states: List[State]) -> None: ...
    def state(self, name: str) -> State: ...


class State:
    def __init__(self, operation: int, index: int, parameter: Parameter) -> None: ...
    @property
    def text(self) -> <class 'Union'>[str, None]: ...
    @property
    def value(self) -> int: ...


class Technique:
    def __init__(self, name: str, annotations: List[Any], passes: List[Pass]) -> None: ...


class _Reader:
    def __init__(self, data: bytes, base: int, big: bool) -> None: ...
    def name(self, at: int) -> str: ...
    def u32(self, at: int) -> int: ...

from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _annotations(r: _Reader, ptr: int, count: int) -> Tuple[List[Any], int]: ...


def _copy(p: Parameter) -> Parameter: ...


def _state(r: _Reader, ptr: int) -> Tuple[State, int]: ...


def _typedef(
    r: _Reader,
    at: int,
    parent: <class 'Union'>[Parameter, None] = ...
) -> Tuple[Parameter, int]: ...


def _value(
    r: _Reader,
    p: Parameter,
    at: int,
    ptr: int,
    numbers: bool
) -> Tuple[int, int]: ...


def read(data: bytes) -> Effect: ...


class Effect:
    def __init__(self) -> None: ...
    def parameter(self, name: str) -> Parameter: ...


class Parameter:
    def __init__(self) -> None: ...


class Pass:
    def __init__(self, name: str, annotations: List[Any], states: List[State]) -> None: ...
    def state(self, name: str) -> State: ...


class State:
    def __init__(self, operation: int, index: int, parameter: Parameter) -> None: ...
    @property
    def text(self) -> <class 'Union'>[str, None]: ...
    @property
    def value(self) -> int: ...


class Technique:
    def __init__(self, name: str, annotations: List[Any], passes: List[Pass]) -> None: ...


class _Reader:
    def __init__(self, data: bytes, base: int, big: bool) -> None: ...
    def name(self, at: int) -> str: ...
    def u32(self, at: int) -> int: ...

from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _annotations(r: _Reader, ptr: int, count: int) -> Tuple[List[Any], int]: ...


def _copy(p: Parameter) -> Parameter: ...


def _state(r: _Reader, ptr: int) -> Tuple[State, int]: ...


def _typedef(
    r: _Reader,
    at: int,
    parent: <class 'Union'>[Parameter, None] = ...
) -> Tuple[Parameter, int]: ...


def _value(
    r: _Reader,
    p: Parameter,
    at: int,
    ptr: int,
    numbers: bool
) -> Tuple[int, int]: ...


def read(data: bytes) -> Effect: ...


class Effect:
    def __init__(self) -> None: ...
    def parameter(self, name: str) -> Parameter: ...


class Parameter:
    def __init__(self) -> None: ...


class Pass:
    def __init__(self, name: str, annotations: List[Any], states: List[State]) -> None: ...
    def state(self, name: str) -> State: ...


class State:
    def __init__(self, operation: int, index: int, parameter: Parameter) -> None: ...
    @property
    def text(self) -> <class 'Union'>[str, None]: ...
    @property
    def value(self) -> int: ...


class Technique:
    def __init__(self, name: str, annotations: List[Any], passes: List[Pass]) -> None: ...


class _Reader:
    def __init__(self, data: bytes, base: int, big: bool) -> None: ...
    def name(self, at: int) -> str: ...
    def u32(self, at: int) -> int: ...
