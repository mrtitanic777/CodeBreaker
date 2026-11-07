from typing import (
    Dict,
    Optional,
    Union,
)


def kit_dir(name: str) -> str: ...


def load(
    name: str,
    variant: None = ...
) -> <class 'Union'>[codebreaker.exact.xenix_c.XenixC, codebreaker.exact.toolchain.Toolchain]: ...


class Toolchain:
    def __init__(self, profile: Dict[str, <class 'Union'>], variant: None = ...) -> None: ...
