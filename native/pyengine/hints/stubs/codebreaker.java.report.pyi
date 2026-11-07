from codebreaker.java.ledger import (
    Event,
    Ledger,
)
from typing import (
    Dict,
    List,
    Optional,
)


def _bar(n: int, width: int = ..., total: <class 'Union'>[int, None] = ...) -> str: ...


def _safe(name: str) -> str: ...


def _write_file_report(
    fname: str,
    evs: List[Event],
    ledger: Ledger,
    files_dir: str
) -> None: ...


def _write_index(ledger: Ledger, report_dir: str, meta: Dict[str, str]) -> None: ...


def write_reports(
    ledger: Ledger,
    output_dir: <class 'Union'>[str, pathlib.WindowsPath],
    report_dir: None = ...,
    meta: <class 'Union'>[Dict[str, str], None] = ...
) -> str: ...
