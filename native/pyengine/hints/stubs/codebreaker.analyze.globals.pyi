from codebreaker.analyze.image import (
    Function,
    Image,
    Ins,
)
from typing import (
    Dict,
    List,
    Tuple,
    Union,
)


def _dereferenced(fn: Function, i: int, reg: str) -> bool: ...


def _operand(
    img: Image,
    ins: Ins,
    va: int
) -> <class 'Union'>[Tuple[None, None], Tuple[Tuple[str, None, None, int, int, None, int], int], Tuple[Tuple[str, None, str, int, int, None, int], int]]: ...


def _pointer_tables(img: Image) -> List[<class 'Union'>[Any, Dict[str, <class 'Union'>]]]: ...


def _slot_kind(img: Image, text: Dict[int, Tuple[str, str, bool]], target: int) -> str: ...


def _type(
    img: Image,
    g: Dict[str, <class 'Union'>],
    string: <class 'Union'>[None, Tuple[str, str, bool]],
    size: int
) -> str: ...


def _variables(img: Image) -> Dict[int, <class 'Union'>]: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def pointer_tables(img: Image) -> List[<class 'Union'>[Any, Dict[str, <class 'Union'>]]]: ...


def variables(img: Image) -> Dict[int, <class 'Union'>]: ...
