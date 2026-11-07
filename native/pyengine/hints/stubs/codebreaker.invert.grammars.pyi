from typing import (
    Dict,
    Iterator,
)


def g_bufread(ctx: Dict[str, int]) -> Iterator[str]: ...


def g_thunk(ctx: Dict[str, int]) -> Iterator[str]: ...
