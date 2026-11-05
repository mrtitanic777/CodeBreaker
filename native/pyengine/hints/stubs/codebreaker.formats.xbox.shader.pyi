from typing import Tuple


def _cstring(b: bytes, at: int) -> str: ...


def _ctab(
    b: bytes,
    base: int
) -> <class 'Union'>[Tuple[List[codebreaker.formats.xbox.shader.Constant], str], Tuple[List[Any], str]]: ...


def _semantic(entry: int) -> Tuple[str, int]: ...


def _u32(b: bytes, at: int) -> int: ...


def read(blob: bytes) -> Shader: ...


class Constant:
    def __init__(self, **kw) -> None: ...


class Shader:
    def __init__(self) -> None: ...
    def constant_at(self, register_set: str, register: int) -> Tuple[Constant, int]: ...
