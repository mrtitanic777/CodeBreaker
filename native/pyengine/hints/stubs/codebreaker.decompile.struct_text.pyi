from typing import (
    Dict,
    Set,
    Tuple,
    Union,
)


def declarations(planned: Dict[str, Dict[str, Tuple[str, Dict[int, Tuple[str, int]]]]], used: Set[str]) -> str: ...


def kind(base: str, stars: str) -> Tuple[str, int]: ...


def plan(
    module_path: str,
    docs: Dict[str, <class 'Union'>],
    by_va: Dict[int, str],
    declared: Dict[str, str]
) -> Dict[str, Dict[str, Tuple[str, Dict[int, Tuple[str, int]]]]]: ...

from typing import (
    Any,
    Dict,
    Union,
)


def plan(
    module_path: str,
    docs: Dict[str, <class 'Union'>],
    by_va: Dict[int, str],
    declared: Dict[str, str]
) -> Dict[Any, Any]: ...
