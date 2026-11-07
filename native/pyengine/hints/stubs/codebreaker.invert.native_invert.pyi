from capstone import CsInsn
from codebreaker.invert.flow16 import Block
from ctypes import CDLL
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _dec(
    x: <class 'Union'>[Dict[str, List[List[<class 'Union'>]]], int, str, List[<class 'Union'>]]
) -> <class 'Union'>[Dict[str, <class 'Union'>], int, str, Tuple[str, int]]: ...


def _load() -> CDLL: ...


def candidates(fb: bytes) -> List[<class 'Union'>[Any, Tuple[str, str]]]: ...


def cfg(insns: List[CsInsn], switch: Callable, cmpswitch: bool) -> List[List[<class 'Union'>]]: ...


def dll_path() -> str: ...


def early_c(
    which: int,
    code: bytes,
    base: int,
    cxmap: None,
    flags: Tuple[bool, bool, bool, bool, bool, bool, bool, bool, None, bool, None],
    retval: None,
    last: Dict[str, <class 'Union'>],
    switch: None,
    callinfo: None,
    callregs: None
) -> Tuple[int, List[str], List[str], str, List[Any]]: ...


def early_c_text(which: int, args: List[<class 'Union'>]) -> str: ...


def msc6(
    code: bytes,
    imports: Dict[int, str],
    syms: <class 'Union'>[None, Dict[int, Tuple[str, str]]],
    rtype: str,
    params: <class 'Union'>[List[Tuple[str, str]], None],
    nearmap: None,
    func_base: int
) -> <class 'Union'>[Tuple[None, str], Tuple[str, Dict[str, <class 'Union'>]]]: ...


def msc6_registers(which: int, code: bytes, imports: None = ...) -> Tuple[None, str]: ...


def msvc2_invert(code: bytes, mask: None = ...) -> Dict[str, <class 'Union'>]: ...


def segment_sigs(segb: bytes) -> Dict[int, List[Tuple[str, str]]]: ...


def structure(blocks: List[Block], entry: int, nocap: None) -> List[List[<class 'Union'>]]: ...


def thunk(fb: bytes, name: str = ..., exact: bool = ...) -> None: ...
