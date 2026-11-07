from typing import (
    List,
    Tuple,
)


def available() -> bool: ...


def mc6_compile(csrc: str, flags: str = ..., timeout: int = ...) -> bytes: ...


def mc6_compile_batch(jobs: List[Tuple[str, str]], timeout: int = ...) -> List[bytes]: ...


def mc_compile(csrc: str, stem: str = ..., timeout: int = ...): ...
