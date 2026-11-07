from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
)


def _jdk() -> Tuple[str, str]: ...


def diagnostics(out_dir: str) -> List[Any]: ...


def dump(
    src_dir: <class 'Union'>[str, pathlib.WindowsPath],
    out_dir: str,
    classpath: <class 'Union'>[Tuple[()], List[str]] = ...,
    files: None = ...,
    release: None = ...
) -> Dict[str, <class 'Union'>[Dict[str, <class 'Union'>], <class 'Union'>]]: ...


def load(out_dir: str) -> Dict[str, <class 'Union'>[Dict[str, <class 'Union'>], <class 'Union'>]]: ...


def run(
    src_dir: <class 'Union'>[str, pathlib.WindowsPath],
    out_dir: str,
    classpath: <class 'Union'>[Tuple[()], List[str]] = ...,
    files: None = ...,
    release: None = ...
) -> int: ...


def sources(src_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> List[str]: ...


def tool(javac: <class 'Union'>[str, None] = ...) -> str: ...
