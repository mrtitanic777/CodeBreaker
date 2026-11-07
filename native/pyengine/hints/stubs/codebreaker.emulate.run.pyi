from typing import (
    Any,
    Optional,
)


def ended(e: str) -> str: ...


class Run:
    def __init__(
        self,
        exit: str,
        results: <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, None], Dict[str, int], None] = ...,
        writes: <class 'Union'>[List[codebreaker.emulate.run.Write], Tuple[()]] = ...,
        capped: bool = ...,
        calls: <class 'Union'>[List[codebreaker.emulate.run.Call], Tuple[()]] = ...,
        io: <class 'Union'>[List[codebreaker.emulate.run.IO], List[Tuple[str, int, int]], Tuple[()]] = ...,
        stack: <class 'Union'>[int, None] = ...,
        preserved: <class 'Union'>[None, Dict[str, Tuple[int, int]]] = ...,
        path: Any = ...,
        steps: <class 'Union'>[int, None] = ...,
        truncated: <class 'Union'>[bool, None] = ...,
        endian: str = ...,
        raw: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
    ) -> None: ...
    @property
    def ended(self) -> str: ...
    def kept(self, names: <class 'Union'>[Tuple[str], None, Tuple[str, str, str]] = ...) -> bool: ...

from typing import (
    Any,
    Dict,
    Optional,
    Union,
)


def ended(e: str) -> str: ...


class Run:
    def __init__(
        self,
        exit: str,
        results: <class 'Union'>[Dict[str, <class 'Union'>[<class 'Union'>, int]], None] = ...,
        writes: <class 'Union'>[List[codebreaker.emulate.run.Write], Tuple[()]] = ...,
        capped: bool = ...,
        calls: <class 'Union'>[List[codebreaker.emulate.run.Call], Tuple[()]] = ...,
        io: <class 'Union'>[List[Tuple[str, int, int]], Tuple[()]] = ...,
        stack: <class 'Union'>[int, None] = ...,
        preserved: <class 'Union'>[None, Dict[str, Tuple[int, int]]] = ...,
        path: Any = ...,
        steps: <class 'Union'>[int, None] = ...,
        truncated: bool = ...,
        endian: str = ...,
        raw: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
    ) -> None: ...
    @property
    def ended(self) -> str: ...

from typing import (
    Any,
    Optional,
)


def ended(e: str) -> str: ...


class Run:
    def __init__(
        self,
        exit: str,
        results: <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, int], None] = ...,
        writes: <class 'Union'>[List[codebreaker.emulate.run.Write], Tuple[()]] = ...,
        capped: bool = ...,
        calls: <class 'Union'>[List[codebreaker.emulate.run.Call], Tuple[()]] = ...,
        io: <class 'Union'>[List[Tuple[str, int, int]], Tuple[()]] = ...,
        stack: <class 'Union'>[int, None] = ...,
        preserved: <class 'Union'>[None, Dict[str, Tuple[int, int]]] = ...,
        path: Any = ...,
        steps: <class 'Union'>[int, None] = ...,
        truncated: bool = ...,
        endian: str = ...,
        raw: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
    ) -> None: ...
    @property
    def ended(self) -> str: ...
    def kept(self, names: <class 'Union'>[Tuple[str], None, Tuple[str, str, str]] = ...) -> bool: ...
