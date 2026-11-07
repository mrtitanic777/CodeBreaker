from codebreaker.source.ne_asm.segment import Seg
from typing import (
    Any,
    Dict,
    Optional,
    Tuple,
    Union,
)


def build_debug_index(debug_exe: str, debug_map: str, ordinals: Dict[Any, Any]) -> Dict[<class 'Union'>, str]: ...


def match_module(
    retail_exe: str,
    sig2name: Dict[<class 'Union'>, str],
    ordinals: Dict[Any, Any],
    min_len: int = ...
) -> Tuple[Dict[str, str], Tuple[int, int]]: ...


def norm_sig(sg: Seg, start: int) -> Tuple[str, Ellipsis]: ...


def recover(
    retail_exe: str,
    debug_exe: str,
    debug_map: str,
    ordinals: Dict[Any, Any],
    out_json: None = ...
) -> Tuple[Dict[str, str], Tuple[int, int]]: ...
