from capstone import CsInsn
from codebreaker.decompile.describe import Description
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
    Union,
)


def _callee_key(i: CsInsn, seg: int) -> <class 'Union'>[Tuple[str, int], None, Tuple[str, int, int]]: ...


def _calls(
    img: bytes,
    seg: int,
    lo: int,
    hi: int,
    name_of: Callable,
    removes: Callable = ...
) -> List[<class 'Union'>[Tuple[str, int, None, bool], Any, Tuple[str, int, int, bool]]]: ...


def _described(
    path: str,
    where: str,
    keep: bool = ...
) -> Tuple[Description, Dict[int, str], Dict[int, <class 'Union'>]]: ...


def _description(path: str, where: str) -> Description: ...


def _removes(d: Description, pops: Dict[int, <class 'Union'>]) -> Callable: ...


def _stack(ins: List[CsInsn], seg: int, prologue: int, removes: Callable) -> Dict[int, Tuple[int, bool]]: ...


def mismatches(original: str, folder: str, image: str, names: List[str]) -> Dict[str, str]: ...
