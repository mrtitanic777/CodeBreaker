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
