from codebreaker.analyze.image import Image
from typing import (
    Dict,
    Union,
)


def _build(img: Image) -> Graph: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def build(img: Image) -> Graph: ...


class Graph:
    def __init__(self) -> None: ...

from codebreaker.analyze.image import Image


def _build(img: Image) -> Graph: ...


def build(img: Image) -> Graph: ...


class Graph:
    def __init__(self) -> None: ...

from codebreaker.analyze.image import (
    Function,
    Image,
    Ins,
)
from typing import (
    Dict,
    Iterator,
    List,
    Set,
    Tuple,
    Union,
)


def _build(img: Image) -> Graph: ...


def _references(
    img: Image,
    fn: Function,
    ins: Ins
) -> Iterator[Tuple[int, str]]: ...


def _relocated(img: Image, ins: Ins) -> Set[int]: ...


def _strongly_connected(nodes: List[int], edges: Dict[int, <class 'Union'>]) -> List[List[int]]: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def build(img: Image) -> Graph: ...


class Graph:
    def __init__(self) -> None: ...
