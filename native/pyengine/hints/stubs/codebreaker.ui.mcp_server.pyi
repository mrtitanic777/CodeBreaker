from mcp.server.mcpserver.server import MCPServer
from typing import (
    Callable,
    Dict,
    Tuple,
    Union,
)


def _cut_function(text: str, addr: str) -> <class 'Union'>[str, None]: ...


def _fit(value: Dict[str, <class 'Union'>], most: int = ...) -> Dict[str, <class 'Union'>]: ...


def _forward(fn: Callable) -> Callable: ...


def _ledger_now(refresh: bool = ...) -> Tuple[Dict[str, <class 'Union'>], Dict[str, str]]: ...


def _said(fn: Callable) -> Callable: ...


def _same_place(addr: str, header: str) -> bool: ...


def _version() -> str: ...


def build(writes: bool = ...) -> MCPServer: ...


def commands() -> dict: ...


def coverage(refresh: bool = ...) -> dict: ...


def demangle(decorated: list[str]) -> dict: ...


def jobs() -> dict: ...


def token() -> str: ...


class Bearer:
    async def __call__(self, scope: Dict[str, <class 'Union'>], receive: None, send: Callable) -> None: ...
    def __init__(self, app: Callable, secret: str, in_path: bool = ...) -> None: ...


class Slow:
    def __init__(self) -> None: ...
    def get(
        self,
        key: str,
        work: Callable,
        age: float = ...,
        wait: <class 'Union'>[int, float] = ...,
        again: bool = ...
    ) -> <class 'Union'>[Tuple[Dict[str, str], int], Tuple[None, int], Tuple[int, int]]: ...

def jobs() -> dict: ...
