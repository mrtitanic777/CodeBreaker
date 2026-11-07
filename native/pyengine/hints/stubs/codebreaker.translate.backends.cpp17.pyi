from codebreaker.translate.backends.cpp03 import (
    Bodies17,
    Context,
)
from codebreaker.translate.ir import Unit
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def Bodies(
    ctx: Context,
    units: List[Unit]
) -> Bodies17: ...


def _native() -> None: ...


def program(units: List[Unit], main: None = ...) -> str: ...


def write_unit(unit: Unit, banner: None = ...) -> Tuple[str, str, Dict[str, Tuple[int, int]]]: ...

from codebreaker.translate.backends.cpp03 import (
    Bodies17,
    Context,
)
from codebreaker.translate.ir import Unit
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def Bodies(
    ctx: Context,
    units: List[Unit]
) -> Bodies17: ...


def _native() -> None: ...


def program(units: List[Unit], main: None = ...) -> str: ...


def write_unit(unit: Unit, banner: None = ...) -> Tuple[str, str, Dict[str, Tuple[int, int]]]: ...
