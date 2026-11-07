from typing import (
    Dict,
    List,
    Optional,
)


def _candidates() -> List[str]: ...


def _find_inc_dir() -> str: ...


def _parse(inc_dir: str) -> Dict[str, List[str]]: ...


def load(inc_dir: None = ...) -> Services: ...


class Services:
    def __init__(s, tables: Dict[str, List[str]]) -> None: ...
