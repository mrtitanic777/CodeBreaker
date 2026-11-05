from codebreaker.decompile.describe import Description
from codebreaker.translate.l2 import Made
from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _by_their_names(out: Dict[str, str], names: Dict[int, str]) -> Dict[str, str]: ...


def _exe() -> str: ...


def _grow(
    d: Description,
    line_for: Callable,
    rounds: int = ...
) -> Tuple[Dict[str, str], List[str], Dict[int, str]]: ...


def _is_x86(module_path: str) -> bool: ...


def _l2_key() -> List[<class 'Union'>[Any, str]]: ...


def _language_stamp() -> str: ...


def _languages() -> str: ...


def _resuming(
    description: str,
    only: <class 'Union'>[List[int], Tuple[()]],
    timeout: int,
    env: Dict[str, str],
    wanted: List[int]
) -> Tuple[str, str, bool]: ...


def _run(
    description: str,
    only: <class 'Union'>[List[int], Tuple[()]] = ...,
    timeout: int = ...,
    max_instructions: None = ...
) -> Tuple[Dict[str, str], List[str], Dict[int, str], str]: ...


def _run_resuming(
    description: str,
    only: <class 'Union'>[List[int], Tuple[()]],
    timeout: int,
    max_instructions: None = ...
) -> Tuple[str, str, bool]: ...


def _run_whole(
    description: str,
    only: <class 'Union'>[List[int], Tuple[()]] = ...,
    timeout: int = ...,
    max_instructions: None = ...
) -> Tuple[Dict[str, str], List[str], Dict[int, str], str]: ...


def _structs(description: str) -> str: ...


def as_c(path: str) -> Tuple[str, int, int]: ...


def available() -> bool: ...


def c_from_l2(
    text: str,
    facts: Dict[str, <class 'Union'>],
    images: None = ...,
    module: <class 'Union'>[Dict[str, <class 'Union'>], None] = ...
) -> str: ...


def decompile(module_path: str, funcs: None = ..., outdir: None = ...) -> Made: ...


def decompile_dos(
    module_path: str,
    outdir: None = ...
) -> Tuple[Dict[str, str], List[str], Description]: ...


def frame16_on() -> bool: ...


def generation() -> Tuple[int, str, str]: ...


def main(argv: List[str]) -> int: ...


def raw(
    module_path: str,
    funcs: List[int],
    outdir: str,
    timeout: int = ...,
    l2: <class 'Union'>[bool, None] = ...,
    write: bool = ...
) -> Decompiled: ...


def run(
    description: str,
    only: <class 'Union'>[List[int], Tuple[()]] = ...,
    timeout: int = ...,
    max_instructions: None = ...,
    l2: <class 'Union'>[bool, None] = ...,
    write: bool = ...
) -> Decompiled: ...


def split_notes(text: str) -> Tuple[str, Dict[str, <class 'Union'>]]: ...


def tree_images() -> bool: ...


class Decompiled:
    @staticmethod
    def of(
        texts: Dict[str, str],
        order: List[str],
        by_va: Dict[int, str],
        err: str,
        notes: Dict[str, <class 'Union'>]
    ) -> Decompiled: ...
