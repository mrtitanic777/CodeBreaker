from typing import (
    Iterator,
    List,
    Tuple,
)


def _close(shadow: str, i: int, pair: str = ...) -> int: ...


def _returned(head: str) -> str: ...


def _tail(
    body: str,
    shadow: str,
    start: int,
    end: int,
    r: str,
    edits: List[<class 'Union'>[Tuple[int, int, str], Any]]
) -> None: ...


def folds(decls: List[<class 'Union'>[str, Any]], body: str, shadow: str) -> None: ...


def returns(head: str, decls: List[<class 'Union'>[str, Any]], body: str, shadow: str) -> <class 'Union'>[str, None]: ...


def statements(shadow: str, lo: int, hi: int) -> List[<class 'Union'>[Tuple[int, int], Any]]: ...


def switches(body: str, shadow: str) -> Iterator[Tuple[int, int, str]]: ...
