from codebreaker.source.ne_asm.segment import Seg
from typing import (
    Any,
    Dict,
    Optional,
    Set,
    Tuple,
    Union,
)


def _branch(objasm: str) -> <class 'Union'>[Tuple[None, None], Tuple[str, int]]: ...


def _emit(
    sg: Seg,
    start: int,
    end: int,
    funcs: None = ...,
    seg_i: int = ...,
    force_db: Set[Any] = ...,
    data_off: Set[Any] = ...
) -> Tuple[str, Set[int]]: ...


def _procname(sg: Seg, off: int) -> str: ...


def _stage_cmacros(root: str) -> str: ...


def assemble(asm_text: str, root: str = ...) -> Dict[str, <class 'Union'>]: ...


def reconcile_range(
    sg: Seg,
    start: int,
    end: int,
    funcs: None = ...,
    root: str = ...,
    max_passes: int = ...,
    backend: str = ...
) -> Tuple[str, Dict[str, <class 'Union'>], Set[Any], int]: ...


def verify_range(
    sg: Seg,
    start: int,
    end: int,
    funcs: None = ...,
    root: str = ...,
    backend: str = ...
) -> Dict[str, <class 'Union'>]: ...
