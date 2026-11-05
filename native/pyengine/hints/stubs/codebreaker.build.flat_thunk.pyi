from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def classify(path: str) -> Dict[str, <class 'Union'>]: ...


def load(path: str) -> PE: ...


class PE:
    def __init__(self, path: str) -> None: ...
    def exports(self) -> List[Tuple[int, str]]: ...
    def imports(self) -> Dict[str, List[str]]: ...
