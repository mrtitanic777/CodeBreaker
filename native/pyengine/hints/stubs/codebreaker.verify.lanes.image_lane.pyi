from codebreaker.platforms import Platform
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


class Lane:
    def __init__(
        self,
        kind: str,
        rev: int,
        states: List[Dict[str, <class 'Union'>[int, <class 'Union'>]]],
        effects: Callable,
        what: <class 'Union'>[Tuple[str, str, str, str, str, str, str], Tuple[str, str, str, str, str, str, str, str, str, str], Tuple[str, str, str, str, str, str], Tuple[str, str, str, str, str, str, str, str]],
        start: Callable,
        subjects: <class 'Union'>[None, Callable] = ...,
        key: str = ...,
        same: str = ...
    ) -> None: ...
    def build(
        self,
        m: Prepared,
        module_path: str,
        work: str
    ) -> <class 'Union'>[Tuple[Tuple[bool, str, str], Dict[str, Tuple[str, int]], Dict[Any, Any]], Tuple[Tuple[bool, Tuple[codebreaker.formats.msx.Program, bytes], Tuple[codebreaker.formats.msx.Program, bytes]], Dict[str, Tuple[str, int]], Dict[Any, Any]], Tuple[Tuple[bool, Tuple[codebreaker.formats.zx.Program, bytes], Tuple[codebreaker.formats.zx.Program, bytes]], Dict[str, Tuple[str, int]], Dict[Any, Any]]]: ...
    def export(self, namespace: Dict[str, <class 'Union'>]) -> None: ...
    def judge(
        self,
        context: <class 'Union'>[Tuple[bool, str, str], Tuple[bool, Tuple[codebreaker.formats.zx.Program, bytes], Tuple[codebreaker.formats.zx.Program, bytes]], Tuple[bool, Tuple[codebreaker.formats.msx.Program, bytes], Tuple[codebreaker.formats.msx.Program, bytes]]],
        item: Tuple[str, int]
    ) -> Tuple[str, str, List[Any], str]: ...
    def platform(self) -> Platform: ...
    def prepare(self, module_path: str, work: str) -> Prepared: ...
    def profile(self, module_path: <class 'Union'>[str, None] = ...) -> Tuple[str, str]: ...
