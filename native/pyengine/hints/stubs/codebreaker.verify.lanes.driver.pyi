from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _memory(fill: <class 'Union'>[int, None] = ...) -> List[Tuple[int, bytes]]: ...


def _mode() -> str: ...


def _paths_profile() -> List[<class 'Union'>]: ...


def _vector(
    eax: int = ...,
    ebx: int = ...,
    ecx: int = ...,
    edx: int = ...,
    esi: int = ...,
    edi: int = ...,
    stack: <class 'Union'>[List[int], Tuple[()]] = ...,
    fill: <class 'Union'>[int, None] = ...,
    ret: int = ...,
    carry: int = ...,
    flags: int = ...
) -> Dict[str, <class 'Union'>]: ...


def concolic_ready() -> bool: ...


def inputs(label: str) -> Tuple[List[Dict[str, <class 'Union'>]], List[Dict[str, <class 'Union'>]]]: ...


def profile(module_path: <class 'Union'>[str, None] = ...) -> Tuple[str, str]: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _memory(fill: <class 'Union'>[int, None] = ...) -> List[Tuple[int, bytes]]: ...


def _mode() -> str: ...


def _paths_profile() -> List[<class 'Union'>]: ...


def _vector(
    eax: int = ...,
    ebx: int = ...,
    ecx: int = ...,
    edx: int = ...,
    esi: int = ...,
    edi: int = ...,
    stack: <class 'Union'>[List[int], Tuple[()]] = ...,
    fill: <class 'Union'>[int, None] = ...,
    ret: int = ...,
    carry: int = ...,
    flags: int = ...
) -> Dict[str, <class 'Union'>]: ...


def concolic_ready() -> bool: ...


def inputs(label: str) -> Tuple[List[Dict[str, <class 'Union'>]], List[Dict[str, <class 'Union'>]]]: ...


def profile(module_path: <class 'Union'>[str, None] = ...) -> Tuple[str, str]: ...
