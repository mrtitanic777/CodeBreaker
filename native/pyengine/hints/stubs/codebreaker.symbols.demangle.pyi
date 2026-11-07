from typing import List


def declaration(name: str) -> <class 'Union'>[str, None]: ...


def main(argv: List[str]) -> int: ...


def parse(name: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def plain(name: str) -> <class 'Union'>[Tuple[str, str, int], Tuple[str, str, None]]: ...


def readable(name: str) -> str: ...


def short(name: str) -> <class 'Union'>[str, None]: ...


def stack_bytes(name: str) -> <class 'Union'>[int, None]: ...


def type_name(decorated: str) -> <class 'Union'>[str, None]: ...

from typing import (
    List,
    Tuple,
)


def declaration(name: str) -> <class 'Union'>[str, None]: ...


def main(argv: List[str]) -> int: ...


def parse(name: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def plain(name: str) -> Tuple[str, str, int]: ...


def readable(name: str) -> str: ...


def stack_bytes(name: str) -> int: ...

def plain(name: str) -> <class 'Union'>[Tuple[str, str, int], Tuple[str, str, None]]: ...
