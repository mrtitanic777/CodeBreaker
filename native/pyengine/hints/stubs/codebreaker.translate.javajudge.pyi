from codebreaker.translate.ir import Unit
from typing import (
    List,
    Optional,
    Tuple,
)


def _host_cxx() -> str: ...


def _run(cmd: List[str], cwd: None = ..., timeout: int = ..., env: None = ...) -> Tuple[int, str, str]: ...


def build(units: List[Unit], work: str, backend: str) -> Tuple[str, str]: ...


def lower_program(src: str, work: str, release: None = ...) -> List[Unit]: ...

from codebreaker.translate.ir import Unit
from typing import (
    List,
    Optional,
    Tuple,
)


def _host_cxx() -> str: ...


def _run(cmd: List[str], cwd: None = ..., timeout: int = ..., env: None = ...) -> Tuple[int, str, str]: ...


def build(units: List[Unit], work: str, backend: str) -> Tuple[str, str]: ...


def lower_program(src: str, work: str, release: None = ...) -> List[Unit]: ...
