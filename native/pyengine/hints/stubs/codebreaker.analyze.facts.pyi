from codebreaker.analyze.image import Image
from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def key(img: Image, va: int) -> str: ...


def open(path: None = ...) -> FactStore: ...


class FactStore:
    def __init__(self, path: None = ...) -> None: ...
    def close(self) -> None: ...
    def get(
        self,
        module: str,
        analysis: <class 'Union'>[str, None] = ...,
        addr: <class 'Union'>[str, None] = ...,
        kind: None = ...
    ) -> List[Dict[str, <class 'Union'>]]: ...
    def put(self, module: str, analysis: str, rows: List[<class 'Union'>]) -> None: ...
    def tally(self, module: str) -> Dict[str, Dict[str, int]]: ...
