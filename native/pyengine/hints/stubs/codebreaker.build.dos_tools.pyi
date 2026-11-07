from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def emulator() -> str: ...


def masm(source: str, stem: str = ..., timeout: int = ..., root: None = ...) -> Dict[str, <class 'Union'>]: ...


def masm_root() -> str: ...


def run(
    mounts: Dict[str, str],
    bat_lines: List[str],
    artifacts: List[str],
    timeout: int = ...,
    files: <class 'Union'>[Dict[str, str], None] = ...,
    cpu: <class 'Union'>[Dict[str, str], None] = ...,
    dos: None = ...,
    variables: <class 'Union'>[Dict[str, str], None] = ...,
    work: <class 'Union'>[str, None] = ...
) -> Tuple[bool, Dict[str, bytes], str]: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def emulator() -> str: ...


def run(
    mounts: Dict[str, str],
    bat_lines: List[str],
    artifacts: List[str],
    timeout: int = ...,
    files: None = ...,
    cpu: <class 'Union'>[Dict[str, str], None] = ...,
    dos: None = ...,
    variables: <class 'Union'>[Dict[str, str], None] = ...,
    work: <class 'Union'>[str, None] = ...
) -> Tuple[bool, Dict[str, bytes], str]: ...

from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def emulator() -> str: ...


def run(
    mounts: Dict[str, str],
    bat_lines: List[str],
    artifacts: List[str],
    timeout: int = ...,
    files: <class 'Union'>[Dict[str, str], None] = ...,
    cpu: None = ...,
    dos: None = ...,
    variables: None = ...,
    work: None = ...
) -> Tuple[bool, Dict[str, bytes], str]: ...
