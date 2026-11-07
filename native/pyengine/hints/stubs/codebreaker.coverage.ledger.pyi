from codebreaker.engine.native_engine import FunctionStore
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _classes(fs: FunctionStore, sha: str, digest: str) -> Tuple[Dict[str, str], int]: ...


def _count_cells(r: Dict[str, <class 'Union'>]) -> List[<class 'Union'>]: ...


def _counts(
    fs: FunctionStore,
    sha: str,
    digest: str,
    path: str,
    kind: str,
    stamp: <class 'Union'>[Tuple[int, int, float], None] = ...
) -> Dict[str, int]: ...


def _pct(a: int, b: int) -> str: ...


def _stamps(fs: FunctionStore) -> Dict[str, Tuple[int, int, float]]: ...


def _state(r: Dict[str, <class 'Union'>]) -> str: ...


def _table(head: List[str], rows: List[List[<class 'Union'>]]) -> List[str]: ...


def _total(rows: List[<class 'Union'>[Any, Dict[str, <class 'Union'>]]]) -> Dict[str, int]: ...


def corpus_modules(refresh: bool = ...) -> List[Dict[str, <class 'Union'>]]: ...


def extents(path: str, kind: str, addrs: List[str]) -> Dict[str, int]: ...


def main(argv: List[str]) -> int: ...


def measure(
    fs: <class 'Union'>[codebreaker.engine.native_engine.FunctionStore, None] = ...,
    module: <class 'Union'>[str, None] = ...,
    refresh: bool = ...
) -> Dict[str, <class 'Union'>]: ...


def open_store(
    path: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[codebreaker.engine.native_engine.FunctionStore, None]: ...


def profiles(fs: <class 'Union'>[codebreaker.engine.native_engine.FunctionStore, None] = ...) -> Dict[str, str]: ...


def reasons(
    fs: FunctionStore,
    sha: str,
    digest: str,
    most: int = ...
) -> Dict[str, List[Tuple[str, int]]]: ...


def render(led: Dict[str, <class 'Union'>], every: bool = ...) -> List[str]: ...


def write_csv(led: Dict[str, <class 'Union'>], path: str) -> int: ...

from codebreaker.engine.native_engine import FunctionStore
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _classes(fs: FunctionStore, sha: str, digest: str) -> Tuple[Dict[str, str], int]: ...


def _count_cells(r: Dict[str, <class 'Union'>]) -> List[<class 'Union'>]: ...


def _counts(
    fs: FunctionStore,
    sha: str,
    digest: str,
    path: str,
    kind: str,
    stamp: <class 'Union'>[Tuple[int, int, float], None] = ...
) -> Dict[str, int]: ...


def _pct(a: int, b: int) -> str: ...


def _stamps(fs: FunctionStore) -> Dict[str, Tuple[int, int, float]]: ...


def _state(r: Dict[str, <class 'Union'>]) -> str: ...


def _table(head: List[str], rows: List[List[<class 'Union'>]]) -> List[str]: ...


def _total(rows: List[<class 'Union'>[Dict[str, <class 'Union'>], Any]]) -> Dict[str, int]: ...


def corpus_modules(refresh: bool = ...) -> List[Dict[str, <class 'Union'>]]: ...


def extents(path: str, kind: str, addrs: List[str]) -> Dict[str, int]: ...


def main(argv: List[str]) -> int: ...


def measure(
    fs: <class 'Union'>[None, codebreaker.engine.native_engine.FunctionStore] = ...,
    module: <class 'Union'>[str, None] = ...,
    refresh: bool = ...
) -> Dict[str, <class 'Union'>]: ...


def open_store(
    path: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[None, codebreaker.engine.native_engine.FunctionStore]: ...


def profiles(fs: <class 'Union'>[None, codebreaker.engine.native_engine.FunctionStore] = ...) -> Dict[str, str]: ...


def reasons(
    fs: FunctionStore,
    sha: str,
    digest: str,
    most: int = ...
) -> Dict[str, List[Tuple[str, int]]]: ...


def render(led: Dict[str, <class 'Union'>], every: bool = ...) -> List[str]: ...


def write_csv(led: Dict[str, <class 'Union'>], path: str) -> int: ...
