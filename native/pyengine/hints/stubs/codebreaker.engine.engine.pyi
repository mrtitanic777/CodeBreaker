from codebreaker.engine.native_engine import (
    Context,
    Database,
    Run,
)
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def Run(
    db: Database,
    log: <class 'Union'>[Callable, None] = ...
) -> Run: ...


def _binary_hash(db: Database) -> str: ...


def _file(ctx: Context, key: str) -> str: ...


def _step_of(kind: str) -> Step: ...


def get(
    db: Database,
    kind: str,
    key: str = ...,
    log: <class 'Union'>[Callable, None] = ...
) -> Dict[str, <class 'Union'>]: ...


def jobs() -> int: ...


def last_run(db: Database) -> Dict[str, List[int]]: ...


def open(path: str, folder: None = ...) -> Database: ...


def step(
    kind: str,
    reads: Tuple[str, Ellipsis] = ...,
    platform: str = ...,
    uses: Tuple[()] = ...,
    stamp: <class 'Union'>[Callable, None] = ...,
    batch: bool = ...,
    always: bool = ...,
    version: None = ...
) -> Callable: ...


class Step:
    def __init__(
        self,
        kind: str,
        fn: Callable,
        reads: Tuple[str, Ellipsis],
        platform: str,
        uses: Tuple[()],
        stamp: <class 'Union'>[Callable, None],
        batch: bool,
        always: bool,
        version: None
    ) -> None: ...
    def version(self) -> str: ...


class Unkept:
    def __init__(self, value: List[str]) -> None: ...

from codebreaker.engine.native_engine import (
    Context,
    Database,
    Run,
)
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def Run(db: Database, log: None = ...) -> Run: ...


def _binary_hash(db: Database) -> str: ...


def _file(ctx: Context, key: str) -> str: ...


def _give_binary(db: Database, value: Dict[str, <class 'Union'>]) -> None: ...


def _step_modules() -> Dict[str, str]: ...


def _step_of(kind: str) -> Step: ...


def forget(db: Database, kind: str, key: <class 'Union'>[str, None] = ...) -> int: ...


def get(
    db: Database,
    kind: str,
    key: str = ...,
    log: None = ...
) -> <class 'Union'>[str, List[<class 'Union'>]]: ...


def get_many(
    db: Database,
    kind: str,
    keys: List[str],
    log: None = ...
) -> Dict[str, str]: ...


def jobs() -> int: ...


def last_run(db: Database) -> Dict[str, List[int]]: ...


def load(platform: str) -> None: ...


def open(path: str, folder: <class 'Union'>[str, None] = ...) -> Database: ...


def put(db: Database, kind: str, key: str, value: str) -> str: ...


def step(
    kind: str,
    reads: Tuple[str, Ellipsis] = ...,
    platform: str = ...,
    uses: Tuple[()] = ...,
    stamp: <class 'Union'>[Callable, None] = ...,
    batch: bool = ...,
    always: bool = ...,
    version: <class 'Union'>[str, None] = ...
) -> Callable: ...


def why(db: Database, kind: str, key: str = ...) -> Dict[str, <class 'Union'>]: ...


class Step:
    def __init__(
        self,
        kind: str,
        fn: Callable,
        reads: Tuple[str, Ellipsis],
        platform: str,
        uses: Tuple[()],
        stamp: <class 'Union'>[Callable, None],
        batch: bool,
        always: bool,
        version: <class 'Union'>[str, None]
    ) -> None: ...
    def version(self) -> str: ...


class Unkept:
    def __init__(self, value: str) -> None: ...
