from typing import (
    Any,
    Callable,
    List,
    Tuple,
)


def definition_file(path: str) -> str: ...


def description(path: str) -> str: ...


def export_directives(path: str, symbol_at: Callable) -> Tuple[List[str], List[Any]]: ...


def exports(path: str) -> List[Export]: ...


def imports(path: str) -> <class 'Union'>[Set[<class 'Union'>], Set[Tuple[str, int]]]: ...


def is_library(path: str) -> bool: ...


def module_name(path: str) -> str: ...


def public_header(path: str, declaration_at: Callable) -> str: ...


class Export:
    def __init__(
        self,
        ordinal: int,
        name: str,
        address: <class 'Union'>[int, Tuple[int, int]],
        kind: str,
        resident: bool = ...,
        target: str = ...,
        shared_data: bool = ...
    ) -> None: ...
