from subprocess import Popen
from typing import (
    Callable,
    Dict,
    Optional,
    Union,
)


def _dbx() -> str: ...


def _wait_dosbox(proc: Popen, work: str, timeout: int, aborted: Callable) -> str: ...


def assemble(asm_src: str, stem: str = ..., timeout: int = ..., masmroot: None = ...) -> Dict[str, <class 'Union'>]: ...
