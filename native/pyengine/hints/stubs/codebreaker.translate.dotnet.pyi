from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _newest(folder: str, prefix: str = ...) -> <class 'Union'>[str, None]: ...


def _ver(s: str) -> Tuple[int, int, int]: ...


def available() -> bool: ...


def compile(
    sources: List[str],
    out_dll: str,
    references: Tuple[()] = ...,
    target: str = ...,
    nullable: bool = ...,
    unsafe: bool = ...,
    warn: bool = ...,
    tmp: <class 'Union'>[str, None] = ...,
    main: <class 'Union'>[str, None] = ...
) -> Tuple[bool, str]: ...


def env(tmp: <class 'Union'>[str, None] = ...) -> Dict[str, str]: ...


def exe() -> str: ...


def home() -> str: ...


def refs() -> str: ...


def roslyn() -> str: ...


def run(
    dll: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    stdin: None = ...,
    tmp: <class 'Union'>[str, None] = ...
) -> Tuple[int, str, str]: ...


def runtime() -> Tuple[str, str]: ...


def runtimeconfig(dll: str) -> str: ...


def sdk() -> str: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _newest(folder: str, prefix: str = ...) -> <class 'Union'>[str, None]: ...


def _ver(s: str) -> Tuple[int, int, int]: ...


def available() -> bool: ...


def compile(
    sources: List[str],
    out_dll: str,
    references: Tuple[()] = ...,
    target: str = ...,
    nullable: bool = ...,
    unsafe: bool = ...,
    warn: bool = ...,
    tmp: <class 'Union'>[str, None] = ...,
    main: <class 'Union'>[str, None] = ...
) -> Tuple[bool, str]: ...


def env(tmp: <class 'Union'>[str, None] = ...) -> Dict[str, str]: ...


def exe() -> str: ...


def home() -> str: ...


def refs() -> str: ...


def roslyn() -> str: ...


def run(
    dll: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    stdin: None = ...,
    tmp: <class 'Union'>[str, None] = ...
) -> Tuple[int, str, str]: ...


def runtime() -> Tuple[str, str]: ...


def runtimeconfig(dll: str) -> str: ...


def sdk() -> str: ...
