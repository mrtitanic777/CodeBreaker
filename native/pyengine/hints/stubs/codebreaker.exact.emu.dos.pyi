from io import BufferedReader
from typing import (
    Optional,
    Tuple,
)


def ropen(path: str, mode: str = ..., **kw) -> BufferedReader: ...


def run(
    exe: str,
    workdir: str,
    cmdline: str = ...,
    stdin: bytes = ...,
    max_blocks: int = ...,
    trace_calls: bool = ...,
    env: bytes = ...,
    instrument: None = ...,
    instrument_when: None = ...,
    driver: None = ...,
    psp: <class 'Union'>[int, None] = ...,
    top: None = ...,
    fast: bool = ...,
    fs: <class 'Union'>[None, MemFS] = ...,
    dpmi: <class 'Union'>[bool, None] = ...
) -> Result: ...


class MemFS:
    def __init__(self, hide: Tuple[()] = ...) -> None: ...
    def disk(self, p: str) -> bytes: ...
    def isfile(self, p: str) -> bool: ...
    def key(self, p: str) -> str: ...
    def put(self, p: str, b: bytes) -> None: ...
    def read(self, p: str) -> bytes: ...
