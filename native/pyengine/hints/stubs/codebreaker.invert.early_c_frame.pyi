from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
)


def _chain_swaps(copies: List[<class 'Union'>[Any, Tuple[str, str]]]) -> List[<class 'Union'>[Any, Tuple[str, str]]]: ...


def apply_copy_swaps(offsets: Dict[str, int], copies: List[<class 'Union'>[Any, Tuple[str, str]]]) -> Dict[str, str]: ...


def layout(
    temps: List[Any],
    locals_: List[Tuple[str, str, int]],
    params: None = ...
) -> Tuple[Dict[str, int], int, Dict[Any, Any], int]: ...


def tsize(t: str, count: int = ...) -> int: ...
