from typing import Tuple


def _tools(compiler: str = ...) -> Tuple[str, str]: ...


def available(compiler: str = ...) -> bool: ...


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...

from typing import Tuple


def object_code(obj: bytes) -> Tuple[bytes, bytes]: ...
