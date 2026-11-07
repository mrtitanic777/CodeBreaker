from typing import (
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
)


def _rank(path: str) -> Tuple[int, int, str]: ...


def _readable(path: str) -> str: ...


def _unpack_boot_images(idx: Dict[str, List[str]]) -> None: ...


def candidates(name: str) -> List[<class 'Union'>[Any, str]]: ...


def current() -> str: ...


def design_dir() -> str: ...


def index(refresh: bool = ...) -> Dict[str, List[str]]: ...


def lane(path: str) -> <class 'Union'>[str, None]: ...


def modules(
    want_lane: <class 'Union'>[str, None] = ...,
    exts: <class 'Union'>[Tuple[str, str, str, str, str], Tuple[str, str, str, str, str, str]] = ...
) -> List[str]: ...


def names() -> List[str]: ...


def owner(path: str) -> str: ...


def reference(*parts) -> str: ...


def resolve(name: str) -> str: ...


def retail(name: str) -> <class 'Union'>[str, None]: ...


def root(name: <class 'Union'>[str, None] = ...) -> str: ...


def selected(name: str) -> Iterator[None]: ...


def sha(path: str) -> str: ...


def take_option(argv: List[str]) -> List[str]: ...


def use(name: <class 'Union'>[str, None]) -> str: ...

from typing import (
    Dict,
    Iterator,
    List,
    Optional,
    Tuple,
)


def _rank(path: str) -> Tuple[int, int, str]: ...


def _readable(path: str) -> str: ...


def _unpack_boot_images(idx: Dict[str, List[str]]) -> None: ...


def candidates(name: str) -> List[<class 'Union'>[Any, str]]: ...


def current() -> str: ...


def design_dir() -> str: ...


def index(refresh: bool = ...) -> Dict[str, List[str]]: ...


def lane(path: str) -> <class 'Union'>[str, None]: ...


def modules(
    want_lane: <class 'Union'>[str, None] = ...,
    exts: <class 'Union'>[Tuple[str, str, str, str, str], Tuple[str, str, str, str, str, str]] = ...
) -> List[str]: ...


def names() -> List[str]: ...


def owner(path: str) -> str: ...


def reference(*parts) -> str: ...


def resolve(name: str) -> str: ...


def retail(name: str) -> <class 'Union'>[str, None]: ...


def root(name: <class 'Union'>[str, None] = ...) -> str: ...


def selected(name: str) -> Iterator[None]: ...


def sha(path: str) -> str: ...


def take_option(argv: List[str]) -> List[str]: ...


def use(name: <class 'Union'>[str, None]) -> str: ...
