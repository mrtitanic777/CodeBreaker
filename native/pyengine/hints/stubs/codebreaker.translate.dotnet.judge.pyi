from typing import (
    List,
    Optional,
    Tuple,
)


def _fresh(work: str) -> str: ...


def _norm(s: str) -> str: ...


def compare(
    orig: Run,
    trans: Run,
    refused: <class 'Union'>[None, List[str]] = ...
) -> Tuple[str, str]: ...


def cpp(
    cpp_dir: str,
    main_class: str,
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    jobs: int = ...
) -> Run: ...


def csharp(
    files: List[str],
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    name: str = ...,
    main: <class 'Union'>[str, None] = ...
) -> Run: ...


def java(
    src_dir: str,
    main: str,
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    classpath: Tuple[()] = ...
) -> Run: ...


class Run:
    def __init__(
        self,
        ok: bool,
        exit: <class 'Union'>[int, None] = ...,
        out: str = ...,
        err: str = ...,
        log: str = ...
    ) -> None: ...

from typing import (
    List,
    Optional,
    Tuple,
)


def _fresh(work: str) -> str: ...


def _norm(s: str) -> str: ...


def compare(
    orig: Run,
    trans: Run,
    refused: <class 'Union'>[List[str], None] = ...
) -> Tuple[str, str]: ...


def cpp(
    cpp_dir: str,
    main_class: str,
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    jobs: int = ...
) -> Run: ...


def csharp(
    files: List[str],
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    name: str = ...,
    main: <class 'Union'>[str, None] = ...
) -> Run: ...


def java(
    src_dir: str,
    main: str,
    work: str,
    args: Tuple[()] = ...,
    timeout: int = ...,
    classpath: Tuple[()] = ...
) -> Run: ...


class Run:
    def __init__(
        self,
        ok: bool,
        exit: <class 'Union'>[int, None] = ...,
        out: str = ...,
        err: str = ...,
        log: str = ...
    ) -> None: ...
