from codebreaker.exact.emu.dos import Result
from ctypes import CDLL


def _load() -> CDLL: ...


def dll_path() -> str: ...


def enabled() -> bool: ...


def run(
    exe: str,
    workdir: str,
    cmdline: str,
    stdin: bytes,
    max_blocks: int,
    trace_calls: bool,
    env: bytes,
    psp: <class 'Union'>[int, None],
    top: None,
    fast: bool,
    fs: <class 'Union'>[MemFS, None],
    dpmi: <class 'Union'>[bool, None],
    fast_limit: float
) -> Result: ...
