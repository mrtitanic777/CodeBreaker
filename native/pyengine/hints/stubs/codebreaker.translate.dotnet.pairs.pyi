from codebreaker.translate.backends.csharp import Program
from codebreaker.translate.dotnet.judge import Run
from codebreaker.translate.ir import Unit
from typing import (
    Any,
    List,
    Optional,
    Tuple,
)


def _clean(path: str) -> str: ...


def _write_csharp(
    units: List[Unit],
    work: str
) -> Tuple[Program, List[str]]: ...


def csharp_to_cpp(files: List[str], work: str, main_class: str = ...) -> Verdict: ...


def csharp_to_csharp(files: List[str], work: str, main: None = ...) -> Verdict: ...


def csharp_units(
    files: List[str],
    work: str
) -> <class 'Union'>[Tuple[List[codebreaker.translate.ir.Unit], List[Tuple[str, str, str]]], Tuple[List[codebreaker.translate.ir.Unit], List[Any]]]: ...


def java_to_csharp(src_dir: str, main: str, work: str) -> Verdict: ...


def java_units(src_dir: str, work: str) -> List[Unit]: ...


class Verdict:
    def __init__(
        self,
        verdict: str,
        detail: str,
        refused: List[Any] = ...,
        files: List[str] = ...,
        orig: <class 'Union'>[Run, None] = ...,
        trans: <class 'Union'>[Run, None] = ...
    ) -> None: ...
