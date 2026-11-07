from codebreaker.translate.backends.java import Program
from codebreaker.translate.ir import Unit
from typing import (
    List,
    Optional,
    Tuple,
)


def _class_name(stem: str) -> str: ...


def _compare(a: Tuple[bytes, bytes, int], b: Tuple[bytes, bytes, int]) -> None: ...


def _jdk() -> Tuple[str, str]: ...


def _norm(b: bytes) -> bytes: ...


def _run(cmd: List[str], stdin: bytes, cwd: str, timeout: int = ...) -> Tuple[bytes, bytes, int]: ...


def _unit(
    path: str,
    language: str,
    std: None,
    includes: Tuple[()],
    quote_includes: Tuple[()],
    defines: Tuple[()],
    own: None = ...
) -> Unit: ...


def judge(
    files: List[str],
    work: str,
    name: None = ...,
    language: None = ...,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...,
    stdin: bytes = ...,
    args: Tuple[()] = ...
) -> Verdict: ...


def native(
    files: List[str],
    out_exe: str,
    language: str,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...
) -> Tuple[bool, str]: ...


def runtime_classes() -> str: ...


def translate(
    files: List[str],
    name: str,
    language: <class 'Union'>[str, None] = ...,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...
) -> Program: ...


class Verdict:
    def __init__(
        self,
        name: str,
        verdict: str,
        reason: str = ...,
        tier: <class 'Union'>[str, None] = ...,
        java: <class 'Union'>[str, None] = ...,
        native: <class 'Union'>[Tuple[bytes, bytes, int], None] = ...,
        jvm: <class 'Union'>[Tuple[bytes, bytes, int], None] = ...,
        notes: <class 'Union'>[Tuple[()], List[str]] = ...
    ) -> None: ...

from codebreaker.translate.backends.java import Program
from codebreaker.translate.ir import Unit
from typing import (
    List,
    Optional,
    Tuple,
)


def _class_name(stem: str) -> str: ...


def _compare(a: Tuple[bytes, bytes, int], b: Tuple[bytes, bytes, int]) -> None: ...


def _jdk() -> Tuple[str, str]: ...


def _norm(b: bytes) -> bytes: ...


def _run(cmd: List[str], stdin: bytes, cwd: str, timeout: int = ...) -> Tuple[bytes, bytes, int]: ...


def _unit(
    path: str,
    language: str,
    std: None,
    includes: Tuple[()],
    quote_includes: Tuple[()],
    defines: Tuple[()],
    own: None = ...
) -> Unit: ...


def judge(
    files: List[str],
    work: str,
    name: None = ...,
    language: None = ...,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...,
    stdin: bytes = ...,
    args: Tuple[()] = ...
) -> Verdict: ...


def native(
    files: List[str],
    out_exe: str,
    language: str,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...
) -> Tuple[bool, str]: ...


def runtime_classes() -> str: ...


def translate(
    files: List[str],
    name: str,
    language: <class 'Union'>[str, None] = ...,
    std: None = ...,
    defines: Tuple[()] = ...,
    includes: Tuple[()] = ...
) -> Program: ...


class Verdict:
    def __init__(
        self,
        name: str,
        verdict: str,
        reason: str = ...,
        tier: <class 'Union'>[str, None] = ...,
        java: <class 'Union'>[str, None] = ...,
        native: <class 'Union'>[Tuple[bytes, bytes, int], None] = ...,
        jvm: <class 'Union'>[Tuple[bytes, bytes, int], None] = ...,
        notes: <class 'Union'>[Tuple[()], List[str]] = ...
    ) -> None: ...
