from typing import (
    Dict,
    Iterator,
    Union,
)


def all_entries() -> Iterator[Union]: ...


def annotation(basename: str) -> str: ...


def kind(e: Dict[str, <class 'Union'>[str, <class 'Union'>]]) -> str: ...


def lookup(basename: str) -> <class 'Union'>[Dict[str, str], Dict[str, <class 'Union'>], None]: ...
