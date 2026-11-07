from codebreaker.emulate.run import Run
from codebreaker.formats.le import LEFile
from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def answers_in(service: str) -> str: ...


def load(path: str, member: None = ...) -> Image: ...


def record(rec: Dict[str, <class 'Union'>]) -> Run: ...


def recorded(image: Image, entry: int, *a, **kw) -> Run: ...


def run(
    image: Image,
    entry: int,
    registers: <class 'Union'>[Dict[str, int], None] = ...,
    stack: Tuple[()] = ...,
    memory: Tuple[()] = ...,
    service_eax: int = ...,
    service_carry: int = ...,
    cap: int = ...,
    flags: int = ...,
    shadow: None = ...
) -> Dict[str, <class 'Union'>]: ...


class Image:
    def __init__(self, lf: LEFile) -> None: ...

from codebreaker.emulate.run import Run
from codebreaker.formats.native_le import LEFile
from typing import (
    Dict,
    Optional,
    Tuple,
    Union,
)


def answers_in(service: str) -> str: ...


def load(path: str, member: None = ...) -> Image: ...


def record(rec: Dict[str, <class 'Union'>]) -> Run: ...


def recorded(image: Image, entry: int, *a, **kw) -> Run: ...


def run(
    image: Image,
    entry: int,
    registers: <class 'Union'>[Dict[str, int], None] = ...,
    stack: Tuple[()] = ...,
    memory: Tuple[()] = ...,
    service_eax: int = ...,
    service_carry: int = ...,
    cap: int = ...,
    flags: int = ...,
    shadow: None = ...
) -> Dict[str, <class 'Union'>]: ...


class Image:
    def __init__(self, lf: LEFile) -> None: ...
